#pragma once

#include "terminal_harness_protocol.hpp"
#include <filesystem>
#include <string>
#include <functional>
#include <memory>

namespace m2m::engine::terminal_harness {

enum class WorkerRole { SelfTest, RemeshConversion, ProductIsolatedSave };
enum class ChildCase : unsigned {
    Returned, Terminal, HalfFrameStall, TruncatedExit, BadChecksum, StaleNonce,
    ProtocolFlood, OutputFlood, BackendReturned, PublicationFailure,
    BoundaryStall, TerminalWithoutBoundary, BoundaryNormalExit, ConflictingReturn,
    InheritanceProbe, SmallOutput, PreparedFileProbe,
    ConversionNormal, ConversionDenied, ConversionDeniedProjectFalse, ConversionDeniedMstFalse,
    ConversionUnknown, ConversionCustom, ConversionPublicationFailure, ConversionEndpointReturned,
    ProductIsolated
};
bool validWorkerSelection(WorkerRole, ProtocolProfile, ChildCase) noexcept;
struct Observation;
class PreparedCase {
public:
    virtual ~PreparedCase() = default;
    // Called once before any child exists; throwing aborts launch as preparation failure.
    virtual void bindNonce(std::uint64_t) {}
    // Called only after confirmed stop, while parent denial/fixture leases remain held.
    virtual void inspectStoppedCase(const Observation&) {}
};
using PrepareOwnedCase = std::function<std::unique_ptr<PreparedCase>(const std::filesystem::path&)>;
struct Options {
    bool allowOwnedWorkerTermination = false;
    std::filesystem::path executable; // restricted to this test process's own image
    std::string expectedSha256;
    std::filesystem::path directory; // new exclusive leaf under caller-validated build scratch
    ChildCase childCase = ChildCase::Returned;
    std::uint32_t deadlineMs = 4000;
    std::uint64_t captureLimitBytes = 128 * 1024;
    std::uintptr_t unrelatedHandle = 0; // deliberate inheritance-negative probe only
    bool failBeforeJobAssignment = false;
    ProtocolProfile protocolProfile = ProtocolProfile::SafetyV1;
    PrepareOwnedCase prepareOwnedCase; // receives only the validated new exclusive leaf
    WorkerRole workerRole = WorkerRole::SelfTest;
    std::filesystem::path explicitScratchRoot; // nonempty only for ProductIsolatedSave; direct new leaf
};
struct Observation {
    Disposition disposition = Disposition::HarnessFailure;
    StopReason stopReason = StopReason::None;
    Transcript transcript;
    bool childCreated = false;
    bool resumed = false;
    bool childStopped = false;
    bool supervisorStopRequested = false;
    std::uint32_t childPid = 0;
    std::uint32_t childStatus = 0;
    std::uint64_t elapsedMs = 0;
    std::uint64_t stdoutBytes = 0, stderrBytes = 0;
    std::string sha256Before, sha256After;
    std::string diagnostic;
    bool casePrepared = false;
    bool caseInspected = false;
    bool preparationHeldUntilParentExit = false; // fail-closed only if OS stop could not be confirmed
};

std::filesystem::path ownExecutable();
std::string executableSha256(const std::filesystem::path&);
Observation supervise(const Options&);
// Only the dedicated test main calls this. Requires the explicit opt-in child role.
int childMain(int argc, char** argv);

} // namespace m2m::engine::terminal_harness
