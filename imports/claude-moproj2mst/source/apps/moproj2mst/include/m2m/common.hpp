// m2m/common.hpp: errors, the per-call context and the seam conventions (owner: G0).
//
// Seam rule (plan section 5): every binary function a packet ports is declared in an include/m2m
// header with its address in an `// @src 0x14...` comment. Until the owning packet merges, its body
// is a stub in src/stubs/<packet>.cpp that throws m2m::NotPorted. A packet implements the functions
// in its own src files and deletes them from its stub file (or the whole stub file).
#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include "moproj2mst.h"

namespace m2m {

namespace doe { struct StateVarConstructorContext; }
namespace engine { class ConversionSession; }

// A failure with one of the M2M_E_* result codes. The message is UTF-8.
struct Error : std::runtime_error {
    int code;
    Error(int c, const std::string& what) : std::runtime_error(what), code(c) {}
};

// Thrown by a seam stub: the function at `src` is not ported yet. The C ABI turns it into
// M2M_E_UNSUPPORTED with the text "not ported yet: 0x<src> <name>".
struct NotPorted : Error {
    std::uint64_t src;
    NotPorted(std::uint64_t addr, const char* name);
};

[[noreturn]] void not_ported(std::uint64_t addr, const char* name);

// Stub body for a seam function: M2M_STUB(0x140131680, "mst header")
#define M2M_STUB(addr, name) ::m2m::not_ported(addr, name)

// Explicit snapshot of the original settings singleton for one conversion. The original
// constructor 0x1400403c0 initializes +0xc90 to zero; 0x140040810 can load the persisted
// Setting/MoInputMode. Reading a user's installation settings is outside this adapter.
struct OriginalSettings {
    int moInputMode = 0;
    // Explicit captured preferences/globals for lazy StateVar construction.
    // Null means unavailable, not an observed empty QSettings store.
    std::shared_ptr<const doe::StateVarConstructorContext> doeStateVarConstructor;
};

// Per-call state that is not part of the original's model. Replaces the original's process
// globals that are not MST-writer state (those are in mst/state.hpp).
struct Ctx {
    // Actual conversion-owned process/session state; borrowed by Open/Save.
    // Bare packet fixtures may omit it, but the public driver always binds it.
    engine::ConversionSession* session = nullptr;
    OriginalSettings originalSettings;
    unsigned flags = 0;          // M2M_FLAG_*
    unsigned codepage = 1252;    // output code page (m2m_set_codepage)
    int guiUnitOverride = 0;     // m2m_set_gui_unit: 0 = derive (P17 guiStartupUnit), 1 SI, 2 English
    std::vector<std::string> warnings;   // UTF-8, one per line in the ABI message
    // Original qWarning observations are independent of adapter warnings/STRICT.
    // Native Qt message-handler routing remains a separately qualified boundary.
    std::vector<std::string> originalDiagnostics;
    // files this conversion's Save steps write (FileEffects) before the MST: an MST writer's
    // existence check counts them as present (UTF-16 paths as written)
    std::vector<std::u16string> pendingWrites;
    std::vector<std::u16string> pendingDirectories; // explicit earlier mkdir effects, not inferred from writes
    // Full native Save only: step5 writers perform their original immediate
    // file attempts while the already-open MST stream is being generated.
    bool nativeMstSideFiles = false;
    // Native Save property dispatch: recovered QFile writers attempt their
    // output at the original call site. Unrecovered writers remain recorded.
    bool nativePropertyFiles = false;
    // Operation <Data> blobs are written during project parsing (1400937b0),
    // before active-operation Open. Top-level item blobs have a separate contract.
    bool nativeOperationBlobs = false;
    std::function<void(const std::u16string&)> beforeNativeOperationBlob;
    // Top-level MoItem (14008e3d0): retain the last Data child, then append a
    // manager row only after successful QFile open; write/close results ignored.
    bool nativeItemBlobs = false;
    std::function<void(const std::u16string&)> beforeNativeItemBlob;
    struct BlobDirectoryAttempt { std::u16string path; bool succeeded; };
    std::vector<BlobDirectoryAttempt> blobDirectoryAttempts;

    void warn(std::string text) { warnings.push_back(std::move(text)); }
    bool strict() const { return (flags & M2M_FLAG_STRICT) != 0; }
};

}  // namespace m2m
