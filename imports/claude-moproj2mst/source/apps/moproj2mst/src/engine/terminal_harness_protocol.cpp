#include "terminal_harness_protocol.hpp"

namespace m2m::engine::terminal_harness {
namespace {
constexpr std::uint32_t magic = 0x3548544du;
template<std::size_t N> void put(std::array<std::uint8_t, N>& out, std::size_t at, std::uint32_t value)
{
    for (unsigned i = 0; i < 4; ++i) out[at + i] = static_cast<std::uint8_t>(value >> (8 * i));
}
template<std::size_t N> std::uint32_t get(const std::array<std::uint8_t, N>& in, std::size_t at)
{
    std::uint32_t value = 0;
    for (unsigned i = 0; i < 4; ++i) value |= static_cast<std::uint32_t>(in[at + i]) << (8 * i);
    return value;
}
template<std::size_t N> std::uint32_t checksum(const std::array<std::uint8_t, N>& in)
{
    std::uint32_t hash = 2166136261u;
    for (std::size_t i = 0; i < N - 4; ++i) hash = (hash ^ in[i]) * 16777619u;
    return hash;
}
bool validAnswer(std::uint32_t value)
{
    return value == 0 || value == 0x4000 || value == 0x10000 || value == 0x400000;
}
bool emptySnapshot(const SaveSnapshot& value)
{
    return !value.flags && !value.checkedSubprojects && !value.answer && !value.pendingDefault;
}
bool validSnapshot(const SaveSnapshot& value)
{
    if (value.flags & ~snapshotMask) return false;
    if (!(value.flags & snapshotPresent)) return emptySnapshot(value);
    if (!validAnswer(value.answer) || !validAnswer(value.pendingDefault)) return false;
    if ((value.flags & snapshotProjectReturned) && !(value.flags & snapshotProjectAttempted)) return false;
    if ((value.flags & snapshotMstReturned) && !(value.flags & snapshotMstAttempted)) return false;
    return true;
}
}
WireFrame encode(const Frame& frame)
{
    WireFrame out{};
    put(out, 0, magic); put(out, 4, 1); put(out, 8, static_cast<std::uint32_t>(frame.kind));
    put(out, 12, frame.site); put(out, 16, static_cast<std::uint32_t>(frame.nonce));
    put(out, 20, static_cast<std::uint32_t>(frame.nonce >> 32)); put(out, 24, frame.status);
    put(out, 28, frame.error); put(out, 32, frame.detail); put(out, 36, checksum(out));
    return out;
}
WireFrameV2 encodeV2(const FrameV2& frame)
{
    WireFrameV2 out{};
    put(out, 0, magic); put(out, 4, 2); put(out, 8, static_cast<std::uint32_t>(frame.kind));
    put(out, 12, frame.site); put(out, 16, static_cast<std::uint32_t>(frame.nonce));
    put(out, 20, static_cast<std::uint32_t>(frame.nonce >> 32)); put(out, 24, frame.status);
    put(out, 28, frame.error); put(out, 32, frame.detail); put(out, 36, frame.handlerProfile);
    put(out, 40, frame.proof); put(out, 44, frame.save.flags); put(out, 48, frame.save.checkedSubprojects);
    put(out, 52, frame.save.answer); put(out, 56, frame.save.pendingDefault); put(out, 60, checksum(out));
    return out;
}
Collector::Collector(std::uint64_t nonce, std::uint32_t childPid) : nonce_(nonce), childPid_(childPid) {}
void Collector::fail(const char* message)
{
    if (transcript_.invalid) return;
    transcript_.invalid = true;
    transcript_.diagnostic = message;
}
bool Collector::accept()
{
    if (++transcript_.frames > maxFrames) { fail("excess protocol records"); return false; }
    const auto nonce = static_cast<std::uint64_t>(get(partial_, 16)) |
                       (static_cast<std::uint64_t>(get(partial_, 20)) << 32);
    if (get(partial_, 0) != magic || get(partial_, 4) != 1 ||
        get(partial_, 36) != checksum(partial_)) { fail("invalid protocol header/checksum"); return false; }
    if (nonce != nonce_ || !nonce_) { fail("stale protocol nonce"); return false; }
    if (get(partial_, 12) != selfTestSite) { fail("wrong self-test source site"); return false; }
    const auto kind = static_cast<Kind>(get(partial_, 8));
    const auto status = get(partial_, 24), error = get(partial_, 28), detail = get(partial_, 32);
    if (kind == Kind::Started) {
        if (transcript_.started || status || error || detail != childPid_) fail("invalid/repeated Started");
        else transcript_.started = true;
    } else if (!transcript_.started || transcript_.returned || transcript_.backendFailure) {
        fail("record after final outcome or before Started");
    } else if (kind == Kind::Returned) {
        if (transcript_.boundary || status > 7 || error || detail) fail("conflicting/invalid Returned");
        else { transcript_.returned = true; transcript_.returnedCode = status; }
    } else if (kind == Kind::Boundary) {
        if (transcript_.boundary || status != terminalStatus || error || detail != 1)
            fail("invalid/repeated SelfTestEndpoint boundary");
        else transcript_.boundary = true;
    } else if (kind == Kind::BackendFailure) {
        if (status || error == 0 || detail) fail("invalid backend-failure record");
        else transcript_.backendFailure = true;
    } else fail("unknown protocol record kind");
    return !transcript_.invalid;
}
bool Collector::feed(std::span<const std::uint8_t> bytes)
{
    if (finished_) { fail("bytes after protocol EOF"); return false; }
    if (transcript_.invalid) return false;
    for (auto byte : bytes) {
        partial_[used_++] = byte;
        if (used_ == frameBytes) {
            used_ = 0;
            if (!accept()) return false;
        }
    }
    return true;
}
void Collector::finish()
{
    if (finished_) return;
    finished_ = true;
    if (used_) fail("truncated protocol record");
    else if (!transcript_.started ||
             (!transcript_.returned && !transcript_.boundary && !transcript_.backendFailure))
        fail("protocol EOF without final outcome");
}
CollectorV2::CollectorV2(std::uint64_t nonce, std::uint32_t childPid) : nonce_(nonce), childPid_(childPid)
{
    transcript_.profile = ProtocolProfile::RemeshV2;
}
void CollectorV2::fail(const char* message)
{
    if (transcript_.invalid) return;
    transcript_.invalid = true;
    transcript_.diagnostic = message;
}
bool CollectorV2::accept()
{
    if (++transcript_.frames > maxFrames) { fail("excess v2 protocol records"); return false; }
    const auto nonce = static_cast<std::uint64_t>(get(partial_, 16)) |
                       (static_cast<std::uint64_t>(get(partial_, 20)) << 32);
    if (get(partial_, 0) != magic || get(partial_, 4) != 2 || get(partial_, 60) != checksum(partial_)) {
        fail("invalid v2 header/checksum"); return false;
    }
    if (!nonce_ || nonce != nonce_) { fail("stale v2 nonce"); return false; }
    if (get(partial_, 12) != remeshSite) { fail("wrong v2 source site"); return false; }
    const auto kind = static_cast<Kind>(get(partial_, 8));
    const auto status = get(partial_, 24), error = get(partial_, 28), detail = get(partial_, 32);
    const auto profile = get(partial_, 36), proof = get(partial_, 40);
    const SaveSnapshot save{get(partial_, 44), get(partial_, 48), get(partial_, 52), get(partial_, 56)};
    if (!validSnapshot(save)) { fail("invalid v2 Save snapshot"); return false; }
    if (kind == Kind::Started) {
        if (transcript_.started || status || error || detail != childPid_ || profile || proof || !emptySnapshot(save))
            fail("invalid/repeated v2 Started");
        else transcript_.started = true;
    } else if (!transcript_.started || transcript_.returned || transcript_.backendFailure) {
        fail("v2 record after final outcome or before Started");
    } else if (kind == Kind::Returned) {
        if (transcript_.boundary || status > 7 || error || detail || profile || proof)
            fail("conflicting/invalid v2 Returned");
        else {
            transcript_.returned = true; transcript_.returnedCode = status;
            if (save.flags & snapshotPresent) transcript_.save = save;
        }
    } else if (kind == Kind::Boundary) {
        constexpr auto required = snapshotPresent | snapshotProjectAttempted | snapshotProjectReturned |
                                  snapshotMstAttempted | snapshotMstReturned;
        if (transcript_.boundary || status != terminalStatus || error != 22 || detail != 1 ||
            profile != 1 || proof != 1 || (save.flags & required) != required)
            fail("invalid/repeated v2 remesh boundary");
        else { transcript_.boundary = true; transcript_.save = save; }
    } else if (kind == Kind::BackendFailure) {
        if (status || error < 1 || error > 3 || detail || profile || proof || !emptySnapshot(save))
            fail("invalid v2 backend failure");
        else { transcript_.backendFailure = true; transcript_.failureTag = error; }
    } else fail("unknown v2 record kind");
    return !transcript_.invalid;
}
bool CollectorV2::feed(std::span<const std::uint8_t> bytes)
{
    if (finished_) { fail("v2 bytes after EOF"); return false; }
    if (transcript_.invalid) return false;
    for (auto byte : bytes) {
        partial_[used_++] = byte;
        if (used_ == frameV2Bytes) {
            used_ = 0;
            if (!accept()) return false;
        }
    }
    return true;
}
void CollectorV2::finish()
{
    if (finished_) return;
    finished_ = true;
    if (used_) fail("truncated v2 record");
    else if (!transcript_.started ||
             (!transcript_.returned && !transcript_.boundary && !transcript_.backendFailure))
        fail("v2 EOF without final outcome");
}
Disposition classify(const Transcript& t, bool stopped, std::uint32_t status, StopReason reason)
{
    // Sticky supervisor state dominates even an otherwise valid terminal-looking exit.
    if (reason == StopReason::Timeout) return Disposition::Timeout;
    if (reason != StopReason::None || !stopped || t.invalid || !t.started || t.backendFailure)
        return Disposition::HarnessFailure;
    if (t.profile != ProtocolProfile::SafetyV1 && t.profile != ProtocolProfile::RemeshV2)
        return Disposition::HarnessFailure;
    if (t.profile == ProtocolProfile::RemeshV2 && t.boundary && !t.save)
        return Disposition::HarnessFailure;
    if (t.boundary && !t.returned && status == terminalStatus)
        return t.profile == ProtocolProfile::RemeshV2 ? Disposition::CandidateProcessTerminal : Disposition::SelfTestTerminal;
    if (t.returned && !t.boundary) {
        // V2 transports the adapter result in its record; OS exit0 means the worker protocol completed.
        // V1 self-tests retain their original rule that the process exit equals the returned payload.
        if (t.profile == ProtocolProfile::RemeshV2)
            return status == 0 && t.returnedCode <= 7 ? Disposition::Returned : Disposition::HarnessFailure;
        if (status == t.returnedCode) return Disposition::Returned;
    }
    return Disposition::HarnessFailure;
}
const char* name(Disposition value)
{
    switch (value) {
    case Disposition::Returned: return "returned";
    case Disposition::SelfTestTerminal: return "self-test-terminal";
    case Disposition::Timeout: return "timeout";
    case Disposition::CandidateProcessTerminal: return "candidate-process-terminal";
    default: return "harness-failure";
    }
}
const char* name(StopReason value)
{
    switch (value) {
    case StopReason::None: return "none";
    case StopReason::Protocol: return "protocol";
    case StopReason::BackendFailure: return "backend-failure";
    case StopReason::CaptureLimit: return "capture-limit";
    case StopReason::LaunchFailure: return "launch-failure";
    case StopReason::Timeout: return "timeout";
    case StopReason::Preparation: return "preparation";
    case StopReason::Inspection: return "inspection";
    default: return "identity";
    }
}
} // namespace m2m::engine::terminal_harness
