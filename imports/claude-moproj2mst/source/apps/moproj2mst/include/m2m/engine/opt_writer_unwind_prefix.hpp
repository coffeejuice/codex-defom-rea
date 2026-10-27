// Prepared .OPT writer state-4 unwind only; no public Save or native cleanup.
#pragma once

#include <array>
#include <cstdint>
#include <optional>
#include <vector>

namespace m2m::engine {

// Logical tokens supplied by a prepared fixture, never pointers to dereference.
struct OptWriterUnwindIdentities {
    std::uint64_t stream = 0;
    std::uint64_t file = 0;
    std::uint64_t path = 0;
};

enum class OptWriterUnwindAdmission {
    Unavailable,
    AdmittedCppUnwindAtState4
};

enum class OptWriterUnwindResource { Stream, File, Path };
enum class OptWriterCleanupReturn { Unavailable, Returned };

struct OptWriterUnwindPrepared {
    OptWriterUnwindAdmission admission = OptWriterUnwindAdmission::Unavailable;
    OptWriterUnwindIdentities identities;
    std::uint64_t exceptionIdentity = 0;
    // Stream, file, path order. Unavailable retains an entered call without
    // asserting that it returned, threw, terminated, or left resources intact.
    std::array<OptWriterCleanupReturn, 3> cleanupReturns{};
};

enum class OptWriterUnwindEventKind {
    SuccessorStatePublished,
    CleanupEntered,
    CleanupReturned
};

struct OptWriterUnwindEvent {
    OptWriterUnwindEventKind kind;
    OptWriterUnwindResource resource;
    std::uint64_t identity;
    int fromState;
    int toState;
};

enum class OptWriterUnwindStop {
    AdmissionUnavailable,
    InvalidPreparedIdentity,
    CleanupReturnUnavailable,
    State4CleanupReturned
};

struct OptWriterUnwindPrefix {
    OptWriterUnwindStop stop = OptWriterUnwindStop::AdmissionUnavailable;
    OptWriterUnwindIdentities identities;
    std::uint64_t exceptionIdentity = 0;
    std::optional<int> state;
    std::optional<OptWriterUnwindResource> awaitingReturn;
    std::vector<OptWriterUnwindEvent> events;
};

// PE-bound writer 140907a10, FuncInfo 1427ba1a8: state 4 -> 1 -> 0 -> -1.
// For example, getter +138 at 140907c17 lies in state 4. The premise is
// admitted C++ unwind through this frame, not merely a failing/throwing call.
// Each successor state is published BEFORE its cleanup action is entered.
// Actions are QTextStream(frame+40), QFile(frame+60), then the by-value path
// QString through frame+240. An unavailable return prevents later actions.
//
// This pure projection calls no destructor, callback, sink.close or OS API.
// Returned cleanup premises are supplied, not observed. Stream destruction
// may attempt a conditional flush; QFile destruction reaches close. Their
// internals, physical bytes, cleanup exceptions and frame continuation are
// outside this model. Completion retains the exception token and means only
// these local cleanup calls returned; it supplies no writer false result,
// caller continuation, owner lifetime, accepted Close or public admission.
OptWriterUnwindPrefix projectOptWriterState4Unwind(
    const OptWriterUnwindPrepared& prepared);

} // namespace m2m::engine
