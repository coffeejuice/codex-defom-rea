#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <span>
#include <string>
#include <optional>

namespace m2m::engine::terminal_harness {

// Candidate harness self-test only. No remesh/native-handler source occurrence.
inline constexpr std::uint32_t selfTestSite = 1;
inline constexpr std::uint32_t terminalStatus = 0xc0000417u;
inline constexpr std::uint32_t supervisorStopStatus = 0xe1050001u;
inline constexpr std::size_t frameBytes = 40;
inline constexpr std::size_t maxFrames = 4;
enum class ProtocolProfile { SafetyV1, RemeshV2 };
inline constexpr std::uint32_t remeshSite = 2;
inline constexpr std::size_t frameV2Bytes = 64;
inline constexpr std::uint32_t snapshotPresent = 1u << 0;
inline constexpr std::uint32_t snapshotOpened = 1u << 1;
inline constexpr std::uint32_t snapshotValidated = 1u << 2;
inline constexpr std::uint32_t snapshotModalPending = 1u << 3;
inline constexpr std::uint32_t snapshotProjectAttempted = 1u << 4;
inline constexpr std::uint32_t snapshotProjectReturned = 1u << 5;
inline constexpr std::uint32_t snapshotProjectSucceeded = 1u << 6;
inline constexpr std::uint32_t snapshotMstAttempted = 1u << 7;
inline constexpr std::uint32_t snapshotMstReturned = 1u << 8;
inline constexpr std::uint32_t snapshotMstSucceeded = 1u << 9;
inline constexpr std::uint32_t snapshotMask = (1u << 10) - 1;
struct SaveSnapshot {
    std::uint32_t flags = 0;
    std::uint32_t checkedSubprojects = 0;
    std::uint32_t answer = 0;
    std::uint32_t pendingDefault = 0;
    bool operator==(const SaveSnapshot&) const = default;
};
// Explicit wire tags, independent of private writer enum ordinals.
enum class FailureTag : std::uint32_t { PublicationFailed = 1, EndpointReturned = 2, InvalidFailureCode = 3 };
enum class Kind : std::uint32_t { Started = 1, Returned = 2, Boundary = 3, BackendFailure = 4 };
struct Frame {
    Kind kind;
    std::uint64_t nonce;
    std::uint32_t site = selfTestSite;
    std::uint32_t status = 0;
    std::uint32_t error = 0;
    std::uint32_t detail = 0;
};
using WireFrame = std::array<std::uint8_t, frameBytes>;
WireFrame encode(const Frame&);
struct FrameV2 {
    Kind kind;
    std::uint64_t nonce;
    std::uint32_t site = remeshSite;
    std::uint32_t status = 0;
    std::uint32_t error = 0;
    std::uint32_t detail = 0; // Started PID; Boundary ordinal1
    std::uint32_t handlerProfile = 0; // Boundary FreshDefault=1 only
    std::uint32_t proof = 0; // Boundary NativeFreshDefaultStartup=1 only
    SaveSnapshot save;
};
using WireFrameV2 = std::array<std::uint8_t, frameV2Bytes>;
WireFrameV2 encodeV2(const FrameV2&);

struct Transcript {
    ProtocolProfile profile = ProtocolProfile::SafetyV1;
    bool started = false;
    bool returned = false;
    bool boundary = false;
    bool backendFailure = false;
    bool invalid = false;
    std::uint32_t returnedCode = 0;
    std::size_t frames = 0;
    std::optional<SaveSnapshot> save;
    std::uint32_t failureTag = 0;
    std::string diagnostic;
};

class CollectorV2 {
public:
    CollectorV2(std::uint64_t nonce, std::uint32_t childPid);
    bool feed(std::span<const std::uint8_t>);
    void finish();
    const Transcript& transcript() const { return transcript_; }
private:
    void fail(const char*);
    bool accept();
    std::uint64_t nonce_;
    std::uint32_t childPid_;
    WireFrameV2 partial_{};
    std::size_t used_ = 0;
    bool finished_ = false;
    Transcript transcript_;
};
class Collector {
public:
    Collector(std::uint64_t nonce, std::uint32_t childPid);
    bool feed(std::span<const std::uint8_t>);
    void finish();
    const Transcript& transcript() const { return transcript_; }
private:
    void fail(const char*);
    bool accept();
    std::uint64_t nonce_;
    std::uint32_t childPid_;
    WireFrame partial_{};
    std::size_t used_ = 0;
    bool finished_ = false;
    Transcript transcript_;
};

enum class Disposition { Returned, SelfTestTerminal, HarnessFailure, Timeout, CandidateProcessTerminal };
enum class StopReason { None, Protocol, BackendFailure, CaptureLimit, LaunchFailure, Timeout, Identity, Preparation, Inspection };
Disposition classify(const Transcript&, bool childStopped, std::uint32_t childStatus, StopReason);
const char* name(Disposition);
const char* name(StopReason);

} // namespace m2m::engine::terminal_harness
