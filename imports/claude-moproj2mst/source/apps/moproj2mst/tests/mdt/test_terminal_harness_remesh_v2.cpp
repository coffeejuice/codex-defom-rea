// ctest-labels: m2m-mdt
// V2 wire/control fixtures only. No remesh/core service is invoked by this executable.
#include <testkit/check.hpp>
#include "../support/test_paths.hpp"
#include "../../src/engine/terminal_harness_supervisor.hpp"
#include <atomic>
#include <fstream>
#include <memory>
#include <stdexcept>

namespace th = m2m::engine::terminal_harness;
namespace fs = std::filesystem;
namespace {
constexpr std::uint64_t nonce = 0x0102030405060708ULL;
constexpr std::uint32_t pid = 789;
th::FrameV2 start() { return {th::Kind::Started, nonce, th::remeshSite, 0, 0, pid}; }
th::FrameV2 boundary()
{
    // Deliberately transport all raw fields; this is not a reached native Save observation.
    return {th::Kind::Boundary, nonce, th::remeshSite, th::terminalStatus, 22, 1, 1, 1,
            {th::snapshotMask, 37, 0x10000, 0x400000}};
}
const fs::path& runRoot()
{
    static const auto value = [] {
        for (unsigned i = 0; i < 1000; ++i) {
            const auto path = m2m_test::scratchOutput(m2m_test::scratchRoot() / ("terminal-v2-" +
                std::to_string(GetCurrentProcessId()) + "-" + std::to_string(GetTickCount64()) + "-" + std::to_string(i)));
            if (fs::create_directory(path)) return path;
        }
        throw std::runtime_error("cannot reserve v2 test root");
    }();
    return value;
}
th::Options options(th::ChildCase mode = th::ChildCase::Returned, unsigned deadline = 4000)
{
    static std::atomic<unsigned> sequence{0};
    static const auto image = th::ownExecutable();
    static const auto hash = th::executableSha256(image);
    th::Options out;
    out.allowOwnedWorkerTermination = true;
    out.executable = image; out.expectedSha256 = hash;
    out.directory = runRoot() / ("case-" + std::to_string(++sequence));
    out.childCase = mode; out.deadlineMs = deadline;
    out.protocolProfile = th::ProtocolProfile::RemeshV2;
    return out;
}
th::Observation run(const th::Options& supplied)
{
    const auto config = supplied; // preserve receipt identity if a callback changes its caller's Options
    auto observed = th::supervise(supplied);
    if (fs::exists(config.directory)) {
        std::ofstream out(config.directory / "receipt.txt", std::ios::binary);
        REQUIRE(out.good());
        out << "schema=terminal-v2-controls-v1\nqualification=protocol-fixtures-only;no-remesh-source-dispatch\n"
            << "executable=" << config.executable.string() << "\nexpected_sha=" << config.expectedSha256
            << "\nbefore_sha=" << observed.sha256Before << "\nafter_sha=" << observed.sha256After
            << "\ncase=" << static_cast<unsigned>(config.childCase)
            << "\ndisposition=" << th::name(observed.disposition) << "\nreason=" << th::name(observed.stopReason)
            << "\ncreated=" << observed.childCreated << "\nstopped=" << observed.childStopped
            << "\nstop_requested=" << observed.supervisorStopRequested << "\nstatus=" << observed.childStatus
            << "\nelapsed_ms=" << observed.elapsedMs << "\nprepared=" << observed.casePrepared
            << "\ninspected=" << observed.caseInspected << "\nlease_held_until_parent_exit=" << observed.preparationHeldUntilParentExit
            << "\nstarted=" << observed.transcript.started << "\nboundary=" << observed.transcript.boundary
            << "\nreturned=" << observed.transcript.returned << "\ninvalid=" << observed.transcript.invalid
            << "\ndiagnostic=" << observed.diagnostic << '\n';
        out.close(); CHECK(!out.fail());
    }
    return observed;
}
struct LeaseState { bool inspected = false, heldAtInspection = false, destroyed = false, throwInspection = false; };
class Lease final : public th::PreparedCase {
    fs::path file_;
    std::shared_ptr<LeaseState> state_;
    HANDLE held_ = INVALID_HANDLE_VALUE;
public:
    Lease(const fs::path& directory, std::shared_ptr<LeaseState> state)
        : file_(directory / "prepared.dat"), state_(std::move(state))
    {
        { std::ofstream file(file_, std::ios::binary); file << "KEEP";
          if (!file) throw std::runtime_error("cannot seed prepared fixture"); }
        held_ = CreateFileW(file_.c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
        if (held_ == INVALID_HANDLE_VALUE) throw std::runtime_error("cannot hold prepared fixture");
    }
    ~Lease() override { CloseHandle(held_); state_->destroyed = true; }
    void inspectStoppedCase(const th::Observation& observed) override
    {
        state_->inspected = observed.childStopped;
        HANDLE denied = CreateFileW(file_.c_str(), GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE,
                                     nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
        const auto error = GetLastError();
        state_->heldAtInspection = denied == INVALID_HANDLE_VALUE && error == ERROR_SHARING_VIOLATION && !state_->destroyed;
        if (denied != INVALID_HANDLE_VALUE) CloseHandle(denied);
        if (state_->throwInspection) throw std::runtime_error("requested stopped inspection failure");
    }
};
void finished(const th::Observation& observed, unsigned bound = 6500)
{
    CHECK(observed.childCreated); CHECK(observed.childStopped); CHECK(observed.elapsedMs < bound);
    CHECK_EQ(observed.sha256Before, observed.sha256After);
    CHECK(!observed.preparationHeldUntilParentExit);
}
void finishRun(bool passed)
{
    if (!passed) { std::fprintf(stderr, "v2 harness failure evidence: %s\n", runRoot().string().c_str()); return; }
    const auto root = fs::canonical(runRoot()), allowed = fs::canonical(m2m_test::scratchRoot());
    const auto relative = root.lexically_relative(allowed);
    if (relative.empty() || relative.is_absolute() || *relative.begin() == ".." ||
        root.filename().wstring().find(L"terminal-v2-") != 0)
        throw std::runtime_error("refusing v2 cleanup outside owned leaf");
    std::ofstream output(m2m_test::scratchOutput(allowed / "terminal-v2-last-success.txt"), std::ios::binary | std::ios::trunc);
    if (!output) throw std::runtime_error("cannot retain v2 summary");
    output << "qualification=protocol-control-only;successful-fixtures-cleaned\n";
    std::uintmax_t total = 0;
    for (const auto& entry : fs::directory_iterator(root)) {
        const auto receipt = entry.path() / "receipt.txt";
        if (!entry.is_directory() || entry.is_symlink() || !fs::is_regular_file(receipt)) continue;
        const auto size = fs::file_size(receipt);
        if (size > 8192 || (total += size) > 256 * 1024) throw std::runtime_error("v2 receipt exceeds bound");
        std::ifstream input(receipt, std::ios::binary);
        if (!input) throw std::runtime_error("cannot read v2 receipt");
        output << "\n[" << entry.path().filename().string() << "]\n" << input.rdbuf();
        if (input.bad()) throw std::runtime_error("cannot aggregate v2 receipt");
    }
    output.close(); if (output.fail()) throw std::runtime_error("cannot flush v2 summary");
    fs::remove_all(root); // resolved exclusive child of allowed scratch, verified above
}
}

TEST_CASE(remesh_v2_protocol_preserves_full_snapshot_raw_answers_and_fixed_offsets)
{
    const auto value = boundary(); const auto bytes = th::encodeV2(value);
    CHECK_EQ(bytes.size(), 64u); CHECK_EQ(bytes[4], 2u); CHECK_EQ(bytes[12], 2u);
    CHECK_EQ(bytes[16], 8u); CHECK_EQ(bytes[23], 1u);
    CHECK_EQ(bytes[28], 22u); CHECK_EQ(bytes[48], 37u); CHECK_EQ(bytes[54], 1u); CHECK_EQ(bytes[58], 64u);
    th::CollectorV2 collector(nonce, pid);
    REQUIRE(collector.feed(th::encodeV2(start())));
    REQUIRE(collector.feed(std::span(bytes.data(), 17)));
    CHECK(!collector.transcript().boundary);
    REQUIRE(collector.feed(std::span(bytes.data() + 17, bytes.size() - 17)));
    collector.finish(); REQUIRE(!collector.transcript().invalid);
    REQUIRE(collector.transcript().save); CHECK(*collector.transcript().save == value.save);
    CHECK(collector.transcript().profile == th::ProtocolProfile::RemeshV2);
    CHECK(th::classify(collector.transcript(), true, th::terminalStatus, th::StopReason::None) == th::Disposition::CandidateProcessTerminal);
    // Classification of synthetic wire values is not a launched remesh behavior claim.
}

TEST_CASE(remesh_v2_returned_can_preserve_partial_or_absent_snapshot_without_defaulting)
{
    for (bool present : {false, true}) {
        th::CollectorV2 collector(nonce, pid); REQUIRE(collector.feed(th::encodeV2(start())));
        th::FrameV2 returned{th::Kind::Returned, nonce, th::remeshSite, 7};
        if (present) returned.save = {th::snapshotPresent | th::snapshotProjectSucceeded | th::snapshotMstSucceeded, 0, 0x4000, 0};
        REQUIRE(collector.feed(th::encodeV2(returned))); collector.finish();
        CHECK(!collector.transcript().invalid); CHECK(collector.transcript().save.has_value() == present);
        if (present) CHECK(*collector.transcript().save == returned.save);
        CHECK(th::classify(collector.transcript(), true, 0, th::StopReason::None) == th::Disposition::Returned);
        CHECK(th::classify(collector.transcript(), true, 7, th::StopReason::None) == th::Disposition::HarnessFailure);
        CHECK(th::classify(collector.transcript(), true, th::terminalStatus, th::StopReason::None) == th::Disposition::HarnessFailure);
    }
    for (std::uint32_t code = 0; code <= 7; ++code) {
        th::CollectorV2 collector(nonce, pid); REQUIRE(collector.feed(th::encodeV2(start())));
        REQUIRE(collector.feed(th::encodeV2({th::Kind::Returned, nonce, th::remeshSite, code}))); collector.finish();
        CHECK(th::classify(collector.transcript(), true, 0, th::StopReason::None) == th::Disposition::Returned);
        auto invalid = collector.transcript(); invalid.returnedCode = 8;
        CHECK(th::classify(invalid, true, 0, th::StopReason::None) == th::Disposition::HarnessFailure);
    }
    th::Collector v1(nonce, pid);
    REQUIRE(v1.feed(th::encode({th::Kind::Started, nonce, th::selfTestSite, 0, 0, pid})));
    REQUIRE(v1.feed(th::encode({th::Kind::Returned, nonce, th::selfTestSite, 7}))); v1.finish();
    CHECK(th::classify(v1.transcript(), true, 7, th::StopReason::None) == th::Disposition::Returned);
    CHECK(th::classify(v1.transcript(), true, 0, th::StopReason::None) == th::Disposition::HarnessFailure);
}

TEST_CASE(remesh_v2_rejects_reserved_incoherent_and_wrong_source_profile_proof_fields)
{
    for (unsigned fault = 0; fault < 14; ++fault) {
        auto value = boundary();
        switch (fault) {
        case 0: value.nonce ^= 1; break;
        case 1: value.site = th::selfTestSite; break;
        case 2: value.handlerProfile = 2; break;
        case 3: value.proof = 2; break;
        case 4: value.status = 7; break;
        case 5: value.error = 34; break;
        case 6: value.detail = 2; break;
        case 7: value.save.flags |= 1u << 20; break;
        case 8: value.save.flags &= ~th::snapshotPresent; break;
        case 9: value.save.answer = 99; break;
        case 10: value.save.pendingDefault = 99; break;
        case 11: value.save.flags &= ~th::snapshotMstAttempted; break;
        case 12: value.save = {}; break;
        default: value.save.flags &= ~th::snapshotProjectReturned; break;
        }
        th::CollectorV2 collector(nonce, pid); REQUIRE(collector.feed(th::encodeV2(start())));
        CHECK(!collector.feed(th::encodeV2(value))); CHECK(collector.transcript().invalid);
    }
}

TEST_CASE(remesh_v2_decoder_does_not_auto_upgrade_or_accept_partial_checksum_and_conflicts)
{
    const auto bytes = th::encodeV2(boundary());
    th::CollectorV2 partial(nonce, pid); REQUIRE(partial.feed(th::encodeV2(start())));
    REQUIRE(partial.feed(std::span(bytes.data(), bytes.size() - 1))); partial.finish(); CHECK(partial.transcript().invalid);
    th::CollectorV2 checksum(nonce, pid); REQUIRE(checksum.feed(th::encodeV2(start())));
    auto bad = bytes; bad[60] ^= 1; CHECK(!checksum.feed(bad));
    th::CollectorV2 v2(nonce, pid); REQUIRE(v2.feed(th::encode({th::Kind::Started, nonce, th::selfTestSite, 0, 0, pid})));
    v2.finish(); CHECK(v2.transcript().invalid);
    th::Collector v1(nonce, pid); CHECK(!v1.feed(th::encodeV2(start())));
    th::CollectorV2 conflict(nonce, pid); REQUIRE(conflict.feed(th::encodeV2(start())));
    REQUIRE(conflict.feed(bytes)); CHECK(!conflict.feed(th::encodeV2({th::Kind::Returned, nonce})));
}

TEST_CASE(remesh_v2_backend_failure_and_supervisor_stop_dominate_terminal_looking_status)
{
    for (unsigned tag : {1u, 2u, 3u}) {
        th::CollectorV2 collector(nonce, pid); REQUIRE(collector.feed(th::encodeV2(start())));
        REQUIRE(collector.feed(th::encodeV2(boundary())));
        REQUIRE(collector.feed(th::encodeV2({th::Kind::BackendFailure, nonce, th::remeshSite, 0, tag})));
        collector.finish(); CHECK_EQ(collector.transcript().failureTag, tag);
        CHECK(th::classify(collector.transcript(), true, th::terminalStatus, th::StopReason::None) == th::Disposition::HarnessFailure);
    }
    for (unsigned invalidTag : {0u, 4u}) {
        th::CollectorV2 invalid(nonce, pid); REQUIRE(invalid.feed(th::encodeV2(start())));
        CHECK(!invalid.feed(th::encodeV2({th::Kind::BackendFailure, nonce, th::remeshSite, 0, invalidTag})));
    }
    th::CollectorV2 collector(nonce, pid); REQUIRE(collector.feed(th::encodeV2(start())));
    REQUIRE(collector.feed(th::encodeV2(boundary()))); collector.finish();
    CHECK(th::classify(collector.transcript(), true, th::terminalStatus, th::StopReason::Timeout) == th::Disposition::Timeout);
    CHECK(th::classify(collector.transcript(), true, th::terminalStatus, th::StopReason::Inspection) == th::Disposition::HarnessFailure);
    auto missing = collector.transcript(); missing.save.reset();
    CHECK(th::classify(missing, true, th::terminalStatus, th::StopReason::None) == th::Disposition::HarnessFailure);
}

TEST_CASE(remesh_v2_earlier_writer_false_snapshot_survives_boundary_then_endpoint_failure)
{
    for (auto failed : {th::snapshotProjectSucceeded, th::snapshotMstSucceeded}) {
        auto value = boundary();
        value.save = {th::snapshotMask & ~th::snapshotModalPending & ~failed, 1, 0x10000, 0};
        th::CollectorV2 collector(nonce, pid);
        REQUIRE(collector.feed(th::encodeV2(start())));
        REQUIRE(collector.feed(th::encodeV2(value)));
        REQUIRE(collector.transcript().save); CHECK(*collector.transcript().save == value.save);
        CHECK(th::classify(collector.transcript(), true, th::terminalStatus, th::StopReason::None) ==
              th::Disposition::CandidateProcessTerminal);
        REQUIRE(collector.feed(th::encodeV2({th::Kind::BackendFailure, nonce, th::remeshSite, 0,
                                            static_cast<std::uint32_t>(th::FailureTag::EndpointReturned)})));
        collector.finish();
        CHECK(!collector.transcript().invalid); CHECK_EQ(collector.transcript().frames, 3u);
        CHECK(collector.transcript().boundary); CHECK(collector.transcript().backendFailure);
        CHECK(!collector.transcript().returned); REQUIRE(collector.transcript().save);
        CHECK(*collector.transcript().save == value.save); // empty failure metadata cannot erase prior effects
        CHECK(th::classify(collector.transcript(), true, th::terminalStatus, th::StopReason::None) ==
              th::Disposition::HarnessFailure);
        CHECK(th::classify(collector.transcript(), true, th::supervisorStopStatus, th::StopReason::BackendFailure) ==
              th::Disposition::HarnessFailure);
    }
}

TEST_CASE(remesh_v2_publication_failure_before_boundary_has_no_snapshot_and_cannot_return_success)
{
    th::CollectorV2 collector(nonce, pid);
    REQUIRE(collector.feed(th::encodeV2(start())));
    REQUIRE(collector.feed(th::encodeV2({th::Kind::BackendFailure, nonce, th::remeshSite, 0,
                                        static_cast<std::uint32_t>(th::FailureTag::PublicationFailed)})));
    const auto failure = collector.transcript();
    CHECK(!failure.invalid); CHECK(failure.started); CHECK(failure.backendFailure);
    CHECK(!failure.boundary); CHECK(!failure.returned); CHECK(!failure.save);
    CHECK_EQ(failure.frames, 2u); CHECK_EQ(failure.failureTag, 1u);
    CHECK(th::classify(failure, true, th::terminalStatus, th::StopReason::None) == th::Disposition::HarnessFailure);
    CHECK(th::classify(failure, true, th::supervisorStopStatus, th::StopReason::BackendFailure) == th::Disposition::HarnessFailure);
    CHECK(!collector.feed(th::encodeV2({th::Kind::Returned, nonce})));
    collector.finish(); CHECK(collector.transcript().invalid);
}

TEST_CASE(remesh_v2_parent_prepares_and_holds_fixture_past_stop_and_inspection)
{
    auto state = std::make_shared<LeaseState>(); auto config = options(th::ChildCase::PreparedFileProbe);
    config.prepareOwnedCase = [state, &config](const fs::path& directory) {
        // The supervisor's immutable launch snapshot must resist callback mutation of its caller.
        config.executable = directory / "not-an-executable.exe";
        config.protocolProfile = th::ProtocolProfile::SafetyV1;
        return std::make_unique<Lease>(directory, state);
    };
    const auto originalDirectory = config.directory;
    const auto result = run(config); finished(result);
    CHECK(result.disposition == th::Disposition::Returned); CHECK(result.transcript.profile == th::ProtocolProfile::RemeshV2);
    CHECK(result.casePrepared); CHECK(result.caseInspected); CHECK(state->inspected); CHECK(state->heldAtInspection); CHECK(state->destroyed);
    HANDLE released = CreateFileW((originalDirectory / "prepared.dat").c_str(), GENERIC_WRITE,
                                  FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
    CHECK(released != INVALID_HANDLE_VALUE); if (released != INVALID_HANDLE_VALUE) CloseHandle(released);
}

TEST_CASE(remesh_v2_preparation_failure_is_before_launch_and_inspection_failure_is_sticky)
{
    auto config = options(); config.prepareOwnedCase = [](const fs::path&) -> std::unique_ptr<th::PreparedCase> { return nullptr; };
    auto result = run(config); CHECK(!result.childCreated); CHECK(result.stopReason == th::StopReason::Preparation);
    config = options(); auto state = std::make_shared<LeaseState>(); state->throwInspection = true;
    config.prepareOwnedCase = [state](const fs::path& path) { return std::make_unique<Lease>(path, state); };
    result = run(config); finished(result);
    CHECK(result.disposition == th::Disposition::HarnessFailure); CHECK(result.stopReason == th::StopReason::Inspection);
    CHECK(state->inspected); CHECK(state->heldAtInspection); CHECK(state->destroyed); CHECK(!result.caseInspected);
    config = options(th::ChildCase::Returned, 500); state = std::make_shared<LeaseState>();
    config.prepareOwnedCase = [state](const fs::path& path) {
        auto lease = std::make_unique<Lease>(path, state);
        Sleep(600); // exceed the deadline beyond Windows timer-tick variance
        return lease;
    };
    result = run(config);
    CHECK(!result.childCreated); CHECK(result.stopReason == th::StopReason::Timeout);
    CHECK(result.disposition == th::Disposition::Timeout); CHECK(state->destroyed); CHECK(!state->inspected);
}

TEST_CASE(remesh_v2_controls_reject_bad_wire_and_keep_half_complete_boundary_stalls_bounded)
{
    for (auto mode : {th::ChildCase::BadChecksum, th::ChildCase::StaleNonce, th::ChildCase::TruncatedExit}) {
        const auto result = run(options(mode)); finished(result);
        CHECK(result.disposition == th::Disposition::HarnessFailure);
    }
    for (auto mode : {th::ChildCase::HalfFrameStall, th::ChildCase::BoundaryStall}) {
        auto state = std::make_shared<LeaseState>(); auto config = options(mode, 1000);
        state->throwInspection = true; // cannot replace the earlier timeout reason
        config.prepareOwnedCase = [state](const fs::path& path) { return std::make_unique<Lease>(path, state); };
        const auto result = run(config); finished(result, 3500);
        CHECK(result.disposition == th::Disposition::Timeout); CHECK(result.stopReason == th::StopReason::Timeout);
        CHECK(result.transcript.boundary == (mode == th::ChildCase::BoundaryStall));
        CHECK(result.supervisorStopRequested); CHECK_EQ(result.childStatus, th::supervisorStopStatus);
        CHECK(state->inspected); CHECK(state->heldAtInspection); CHECK(state->destroyed);
    }
}

int main(int argc, char** argv)
{
    if (argc != 1) return th::childMain(argc, argv);
    const int code = testkit::run_all();
    try { finishRun(code == 0); }
    catch (const std::exception& error) { std::fprintf(stderr, "v2 diagnostic/cleanup failed: %s\n", error.what()); return code ? code : 1; }
    return code;
}
