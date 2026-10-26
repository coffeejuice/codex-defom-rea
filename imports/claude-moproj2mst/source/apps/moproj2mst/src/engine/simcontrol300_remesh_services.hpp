#pragma once

#include "m2m/engine/simcontrol300_remesh.hpp"
#include <cstdint>
#include <optional>

namespace m2m::engine::simcontrol300::detail {

// Explicit admission by an owned isolated worker, not the host CRT's handler.
enum class RemeshTerminalProfile { Unknown, FreshDefault, Custom };
enum class RemeshTerminalSite { FirstNullFilePrintf };
enum class RemeshTerminalProof { NativeFreshDefaultStartup };

struct RemeshTerminalRequest {
    RemeshTerminalSite site = RemeshTerminalSite::FirstNullFilePrintf;
    RemeshTerminalProof proof = RemeshTerminalProof::NativeFreshDefaultStartup;
    std::uint64_t sourceAddress = 0x1400abcf0ULL;
    std::uint32_t expectedProcessStatus = 0xc0000417U;
    int crtErrno = 22; // EINVAL, not numeric ERANGE34.
    unsigned formatAttemptOrdinal = 1;
};

// Classifies only the handler profile; the writer establishes enabled/openfail.
std::optional<RemeshTerminalRequest> remeshTerminalRequestFor(RemeshTerminalProfile) noexcept;

enum class RemeshTerminalDispatchFailure {
    PublicationFailed,
    EndpointReturned,
    InvalidFailureCode
};

class RemeshTerminalService {
public:
    virtual ~RemeshTerminalService() = default;
    virtual RemeshTerminalProfile profile() const noexcept = 0;
    // An admitted successful endpoint does not return. Any return is failure,
    // without a noreturn promise that could make a bad backend undefined.
    virtual RemeshTerminalDispatchFailure dispatch(const RemeshTerminalRequest&) noexcept = 0;
    // Best-effort failure publication, then the writer parks without unwinding.
    // The isolated supervisor must retain a deadline and stop its owned child.
    virtual void failClosed(const RemeshTerminalRequest&, RemeshTerminalDispatchFailure) noexcept = 0;
};

// Borrowed synchronously; no service escapes into Session or public results.
// The public writer delegates with nullptr and retains its existing guard.
RemeshSaveReport saveRemeshAtProjectSaveWithTerminalService(
    const RemeshState&, const std::string& absoluteDirectory, NumericLocale,
    RemeshTerminalService*);

} // namespace m2m::engine::simcontrol300::detail
