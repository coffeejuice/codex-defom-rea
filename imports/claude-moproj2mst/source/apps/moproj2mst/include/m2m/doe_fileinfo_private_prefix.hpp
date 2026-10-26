#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>
#include <optional>
#include <type_traits>
#include <vector>

#include "m2m/doe_icon_shell_prefix.hpp"

namespace m2m::doe {

class DoeFileInfoPrivatePrefix;
class DoeFileInfoPrivateArena;
const DoeFileInfoPrivatePrefix& appendDoeFileInfoPrivatePrefix(
    DoeFileInfoPrivateArena& arena, const DoeIconShellPrefix& icon);

// Partial 0xe0-byte allocation only. The constructor writes its first32-bit
// reference word; the uninitialized tail has no public field/read interface.
// This is neither a completed QFileInfoPrivate nor a published QFileInfo wrapper.
class alignas(8) DoeFileInfoPrivateValue final {
public:
    DoeFileInfoPrivateValue(const DoeFileInfoPrivateValue&) = delete;
    DoeFileInfoPrivateValue& operator=(const DoeFileInfoPrivateValue&) = delete;
    std::uint32_t referenceWord() const noexcept { return reference_; }

private:
    friend const DoeFileInfoPrivatePrefix& appendDoeFileInfoPrivatePrefix(
        DoeFileInfoPrivateArena&, const DoeIconShellPrefix&);
    DoeFileInfoPrivateValue() noexcept : reference_(0) {}
    std::uint32_t reference_;
    std::byte unavailable_[0xdc]; // Intentionally not initialized or interpreted.
};
static_assert(sizeof(DoeFileInfoPrivateValue) == 0xe0, "Recovered QFileInfo private allocation is 0xe0 bytes");
static_assert(alignof(DoeFileInfoPrivateValue) == 8, "Recovered x64 private payload alignment");
static_assert(std::is_standard_layout_v<DoeFileInfoPrivateValue>, "Reference is the first payload word");

enum class DoeFileInfoPrivateStage { AllocationPending, ReferenceInitialized };
enum class DoeFileInfoPrivateNextBoundary { QStringPathOwnership };

class DoeFileInfoPrivatePrefix final {
public:
    DoeFileInfoPrivateStage stage() const noexcept { return stage_; }
    const DoeFileInfoPrivateValue* value() const noexcept { return value_.get(); }
    std::size_t initializedPrefixBytes() const noexcept
    { return value_ ? sizeof(std::uint32_t) : 0; }
    std::optional<DoeFileInfoPrivateNextBoundary> nextBoundary() const noexcept
    {
        return stage_ == DoeFileInfoPrivateStage::ReferenceInitialized
            ? std::optional<DoeFileInfoPrivateNextBoundary>(DoeFileInfoPrivateNextBoundary::QStringPathOwnership)
            : std::nullopt;
    }
    const char* guardMessage() const noexcept;

private:
    friend const DoeFileInfoPrivatePrefix& appendDoeFileInfoPrivatePrefix(
        DoeFileInfoPrivateArena&, const DoeIconShellPrefix&);
    DoeFileInfoPrivatePrefix() = default;
    std::unique_ptr<DoeFileInfoPrivateValue> value_;
    DoeFileInfoPrivateStage stage_ = DoeFileInfoPrivateStage::AllocationPending;
};

// Caller-owned diagnostics for an unreturned constructor prefix. Keeping these
// adapter bytes after a guard does not claim native liveness: a real native
// constructor exception has its own raw-allocation cleanup. No back references,
// QString/QByteArray owners, engines, callbacks or per-record reset are provided.
class DoeFileInfoPrivateArena final {
public:
    DoeFileInfoPrivateArena() = default;
    DoeFileInfoPrivateArena(const DoeFileInfoPrivateArena&) = delete;
    DoeFileInfoPrivateArena& operator=(const DoeFileInfoPrivateArena&) = delete;
    DoeFileInfoPrivateArena(DoeFileInfoPrivateArena&&) = delete;
    DoeFileInfoPrivateArena& operator=(DoeFileInfoPrivateArena&&) = delete;

    std::size_t size() const noexcept { return prefixes_.size(); }
    const DoeFileInfoPrivatePrefix& at(std::size_t ordinal) const { return *prefixes_.at(ordinal); }

private:
    friend const DoeFileInfoPrivatePrefix& appendDoeFileInfoPrivatePrefix(
        DoeFileInfoPrivateArena&, const DoeIconShellPrefix&);
    std::vector<std::unique_ptr<DoeFileInfoPrivatePrefix>> prefixes_;
};

// Isolated fixed DOE request/new-shell admission. Initializes ref0 only, then
// returns at the QStringPathOwnership guard. No native constructor return,
// outer-wrapper publication/ref1, shared-null increment or engine is invented.
const DoeFileInfoPrivatePrefix& appendDoeFileInfoPrivatePrefix(
    DoeFileInfoPrivateArena& arena, const DoeIconShellPrefix& icon);

} // namespace m2m::doe
