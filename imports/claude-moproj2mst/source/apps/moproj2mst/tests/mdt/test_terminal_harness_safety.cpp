// ctest-labels: m2m-mdt
// Candidate-only process/protocol self-test. No converter/remesh/native-handler calls.
#include <testkit/check.hpp>
#include "../support/test_paths.hpp"
#include "../../src/engine/terminal_harness_supervisor.hpp"
#include <array>
#include <atomic>
#include <fstream>
#include <memory>
#include <string>

namespace th = m2m::engine::terminal_harness;
namespace fs = std::filesystem;
namespace {
const fs::path& runRoot()
{
    static const fs::path root = [] {
        const auto scratch = m2m_test::scratchRoot();
        for (unsigned i = 0; i < 1000; ++i) {
            const auto path = m2m_test::scratchOutput(scratch / ("terminal-safety-" +
                std::to_string(GetCurrentProcessId()) + "-" + std::to_string(GetTickCount64()) + "-" + std::to_string(i)));
            if (fs::create_directory(path)) return path;
        }
        throw std::runtime_error("cannot allocate exclusive terminal harness scratch");
    }();
    return root;
}
th::Options options(th::ChildCase mode, std::uint32_t deadline = 4000)
{
    static std::atomic<unsigned> sequence{0};
    static const auto image = th::ownExecutable();
    static const auto hash = th::executableSha256(image);
    th::Options out;
    out.allowOwnedWorkerTermination = true; // explicit Testkit-owned child opt-in
    out.executable = image;
    out.expectedSha256 = hash;
    out.directory = runRoot() / ("case-" + std::to_string(++sequence));
    out.childCase = mode;
    out.deadlineMs = deadline;
    return out;
}
th::Observation run(const th::Options& config)
{
    auto result = th::supervise(config);
    if (fs::exists(config.directory)) {
        std::ofstream receipt(config.directory / "receipt.txt", std::ios::binary);
        REQUIRE(receipt.good());
        receipt << "schema=terminal-harness-safety-v1\n"
                << "qualification=self-test-endpoint;no-remesh-conversion;native-handler-prelude-unimplemented\n"
                << "case=" << static_cast<unsigned>(config.childCase) << '\n'
                << "executable=" << config.executable.string() << '\n'
                << "expected_sha256=" << config.expectedSha256 << '\n'
                << "sha256_before=" << result.sha256Before << '\n'
                << "sha256_after=" << result.sha256After << '\n'
                << "disposition=" << th::name(result.disposition) << '\n'
                << "stop_reason=" << th::name(result.stopReason) << '\n'
                << "created=" << result.childCreated << "\nresumed=" << result.resumed
                << "\nstopped=" << result.childStopped << "\nstop_requested=" << result.supervisorStopRequested
                << "\npid=" << result.childPid << "\nstatus=" << result.childStatus
                << "\nelapsed_ms=" << result.elapsedMs << "\ndeadline_ms=" << config.deadlineMs
                << "\nstdout_bytes=" << result.stdoutBytes << "\nstderr_bytes=" << result.stderrBytes
                << "\nstarted=" << result.transcript.started << "\nboundary=" << result.transcript.boundary
                << "\nreturned=" << result.transcript.returned << "\nbackend_failure=" << result.transcript.backendFailure
                << "\nprotocol_invalid=" << result.transcript.invalid << "\nframes=" << result.transcript.frames
                << "\ndiagnostic=" << result.diagnostic << '\n';
        receipt.close();
        CHECK(!receipt.fail());
    }
    return result;
}
void stoppedAndBound(const th::Observation& result, std::uint64_t bound = 6500)
{
    CHECK(result.childCreated); CHECK(result.childStopped);
    CHECK(result.elapsedMs < bound);
    CHECK_EQ(result.sha256Before, result.sha256After);
}
th::Frame started() { return {th::Kind::Started, 123, th::selfTestSite, 0, 0, 456}; }
th::Frame boundary() { return {th::Kind::Boundary, 123, th::selfTestSite, th::terminalStatus, 0, 1}; }
void finishRun(bool passed)
{
    if (!passed) {
        std::fprintf(stderr, "terminal harness failure evidence: %s\n", runRoot().string().c_str());
        return;
    }
    const auto root = fs::canonical(runRoot());
    const auto allowed = fs::canonical(m2m_test::scratchRoot());
    const auto relative = root.lexically_relative(allowed);
    if (relative.empty() || relative.is_absolute() || *relative.begin() == ".." ||
        root.filename().wstring().find(L"terminal-safety-") != 0)
        throw std::runtime_error("refusing terminal harness cleanup outside owned scratch");
    // One bounded checkout diagnostic replaces accumulating successful case directories.
    const auto receiptPath = m2m_test::scratchOutput(allowed / "terminal-harness-last-success.txt");
    std::ofstream aggregate(receiptPath, std::ios::binary | std::ios::trunc);
    if (!aggregate) throw std::runtime_error("cannot retain terminal harness summary");
    aggregate << "qualification=self-test-only;successful-fixtures-cleaned;not-native-handler-or-conversion\n";
    std::size_t total = 0;
    for (const auto& entry : fs::directory_iterator(root)) {
        const auto receipt = entry.path() / "receipt.txt";
        if (!entry.is_directory() || entry.is_symlink() || !fs::is_regular_file(receipt)) continue;
        const auto size = fs::file_size(receipt);
        if (size > 8192 || total + size > 256 * 1024) throw std::runtime_error("summary receipt exceeds bound");
        std::ifstream input(receipt, std::ios::binary);
        if (!input) throw std::runtime_error("cannot read owned terminal receipt");
        aggregate << "\n[" << entry.path().filename().string() << "]\n" << input.rdbuf();
        if (input.bad()) throw std::runtime_error("cannot retain owned terminal receipt");
        total += static_cast<std::size_t>(size);
    }
    aggregate.close();
    if (aggregate.fail()) throw std::runtime_error("cannot flush terminal harness summary");
    fs::remove_all(root); // canonical target checked above, exactly the exclusively created leaf
}
}

TEST_CASE(terminal_protocol_fragments_are_not_a_return_or_a_remesh_occurrence)
{
    th::Collector collector(123, 456);
    const auto first = th::encode(started());
    CHECK(collector.feed(std::span(first.data(), 3)));
    CHECK(!collector.transcript().started);
    CHECK(collector.feed(std::span(first.data() + 3, first.size() - 3)));
    CHECK(collector.feed(th::encode(boundary())));
    collector.finish();
    CHECK(!collector.transcript().invalid);
    CHECK(collector.transcript().boundary); CHECK(!collector.transcript().returned);
    CHECK(th::classify(collector.transcript(), true, th::terminalStatus, th::StopReason::None) ==
          th::Disposition::SelfTestTerminal);
    CHECK(th::classify(collector.transcript(), true, th::terminalStatus, th::StopReason::Timeout) ==
          th::Disposition::Timeout);
    CHECK(th::classify(collector.transcript(), true, th::terminalStatus, th::StopReason::Protocol) ==
          th::Disposition::HarnessFailure);
    CHECK(th::classify(collector.transcript(), true, 0, th::StopReason::None) == th::Disposition::HarnessFailure);
}

TEST_CASE(terminal_protocol_rejects_stale_wrong_malformed_truncated_and_conflicting_records)
{
    for (unsigned fault = 0; fault < 7; ++fault) {
        th::Collector collector(123, 456);
        REQUIRE(collector.feed(th::encode(started())));
        auto value = boundary();
        if (fault == 0) value.nonce = 124;
        if (fault == 1) value.site = 999; // must never accept a fabricated remesh occurrence
        if (fault == 2) value.kind = static_cast<th::Kind>(99);
        auto bytes = th::encode(value);
        if (fault == 3) bytes[4] ^= 1;
        if (fault == 4) bytes[36] ^= 1;
        if (fault == 5) {
            REQUIRE(collector.feed(std::span(bytes.data(), bytes.size() - 1)));
            collector.finish();
        } else if (fault == 6) {
            REQUIRE(collector.feed(bytes));
            CHECK(!collector.feed(th::encode({th::Kind::Returned, 123})));
        } else CHECK(!collector.feed(bytes));
        CHECK(collector.transcript().invalid);
        CHECK(th::classify(collector.transcript(), true, th::terminalStatus, th::StopReason::None) ==
              th::Disposition::HarnessFailure);
    }
    th::Collector empty(123, 456); empty.finish();
    CHECK(empty.transcript().invalid);
}

TEST_CASE(terminal_supervisor_requires_opt_in_self_image_hash_and_scratch_containment)
{
    auto config = options(th::ChildCase::Terminal);
    config.allowOwnedWorkerTermination = false;
    auto result = run(config);
    CHECK(!result.childCreated); CHECK(result.disposition == th::Disposition::HarnessFailure);
    config.allowOwnedWorkerTermination = true;
    config.expectedSha256 = std::string(64, '0');
    result = run(config);
    CHECK(!result.childCreated); CHECK(result.disposition == th::Disposition::HarnessFailure);
    config = options(th::ChildCase::Terminal);
    config.executable = runRoot() / "not-the-self-test.exe";
    result = run(config);
    CHECK(!result.childCreated); CHECK(result.disposition == th::Disposition::HarnessFailure);
    config = options(th::ChildCase::Terminal);
    config.directory = th::ownExecutable().parent_path() / ("outside-test-scratch-" + config.directory.filename().string());
    const bool existed = fs::exists(config.directory);
    result = th::supervise(config); // must reject before creating any output there
    CHECK(!result.childCreated); CHECK(result.disposition == th::Disposition::HarnessFailure);
    CHECK(fs::exists(config.directory) == existed);
}

TEST_CASE(terminal_supervisor_normal_control_and_file_capture_return_without_pipe_deadlock)
{
    for (auto mode : {th::ChildCase::Returned, th::ChildCase::SmallOutput}) {
        const auto config = options(mode);
        const auto result = run(config);
        stoppedAndBound(result);
        CHECK(result.disposition == th::Disposition::Returned);
        CHECK(result.stopReason == th::StopReason::None);
        CHECK_EQ(result.childStatus, 0u); CHECK(result.transcript.returned);
        CHECK(!result.supervisorStopRequested);
        CHECK(fs::exists(config.directory / "unwound.marker"));
        if (mode == th::ChildCase::SmallOutput) {
            CHECK_EQ(result.stdoutBytes, 32768u); CHECK_EQ(result.stderrBytes, 32768u);
        }
    }
}

TEST_CASE(terminal_self_test_endpoint_terminates_its_child_without_unwinding_helper_scope)
{
    const auto config = options(th::ChildCase::Terminal);
    const auto result = run(config);
    stoppedAndBound(result);
    CHECK(result.disposition == th::Disposition::SelfTestTerminal);
    CHECK(result.stopReason == th::StopReason::None); CHECK(!result.supervisorStopRequested);
    CHECK_EQ(result.childStatus, th::terminalStatus);
    CHECK(result.transcript.started); CHECK(result.transcript.boundary); CHECK(!result.transcript.returned);
    CHECK(!fs::exists(config.directory / "unwound.marker"));
    // This probe is a helper scope, never a ConversionSession or native default handler.
}

TEST_CASE(terminal_supervisor_rejects_malformed_flood_and_same_code_without_valid_boundary)
{
    for (auto mode : {th::ChildCase::TruncatedExit, th::ChildCase::BadChecksum, th::ChildCase::StaleNonce,
                      th::ChildCase::ProtocolFlood, th::ChildCase::TerminalWithoutBoundary,
                      th::ChildCase::BoundaryNormalExit, th::ChildCase::ConflictingReturn}) {
        const auto config = options(mode);
        const auto result = run(config);
        stoppedAndBound(result);
        CHECK(result.disposition == th::Disposition::HarnessFailure);
        CHECK(result.disposition != th::Disposition::SelfTestTerminal);
    }
}

TEST_CASE(terminal_deadline_covers_half_frames_and_stalls_after_complete_boundary)
{
    for (auto mode : {th::ChildCase::HalfFrameStall, th::ChildCase::BoundaryStall}) {
        const auto config = options(mode, 1000);
        const auto result = run(config);
        stoppedAndBound(result, 3500);
        CHECK(result.disposition == th::Disposition::Timeout);
        CHECK(result.stopReason == th::StopReason::Timeout); CHECK(result.supervisorStopRequested);
        CHECK_EQ(result.childStatus, th::supervisorStopStatus);
        CHECK(result.transcript.started); CHECK(!result.transcript.returned);
        CHECK(result.transcript.boundary == (mode == th::ChildCase::BoundaryStall));
        CHECK(!fs::exists(config.directory / "unwound.marker"));
    }
}

TEST_CASE(terminal_backend_return_and_publication_failure_cannot_continue_or_pass_terminal)
{
    for (auto mode : {th::ChildCase::BackendReturned, th::ChildCase::PublicationFailure}) {
        const auto config = options(mode);
        const auto result = run(config);
        stoppedAndBound(result);
        CHECK(result.disposition == th::Disposition::HarnessFailure);
        CHECK(result.supervisorStopRequested); CHECK(!result.transcript.returned);
        CHECK(result.stopReason == (mode == th::ChildCase::BackendReturned ?
              th::StopReason::BackendFailure : th::StopReason::Protocol));
        CHECK(!fs::exists(config.directory / "unwound.marker"));
    }
}

TEST_CASE(terminal_output_flood_is_bounded_by_capture_size)
{
    auto config = options(th::ChildCase::OutputFlood);
    config.captureLimitBytes = 65536;
    const auto result = run(config);
    stoppedAndBound(result);
    CHECK(result.disposition == th::Disposition::HarnessFailure);
    CHECK(result.stopReason == th::StopReason::CaptureLimit);
    CHECK(result.stdoutBytes > config.captureLimitBytes || result.stderrBytes > config.captureLimitBytes);
}

TEST_CASE(terminal_worker_does_not_inherit_unrelated_parent_handle)
{
    SECURITY_ATTRIBUTES attributes{sizeof(SECURITY_ATTRIBUTES), nullptr, TRUE};
    HANDLE event = CreateEventW(&attributes, TRUE, FALSE, nullptr);
    REQUIRE(event != nullptr);
    struct Release { HANDLE h; ~Release() { CloseHandle(h); } } release{event};
    DWORD flags = 0;
    REQUIRE(GetHandleInformation(event, &flags)); REQUIRE((flags & HANDLE_FLAG_INHERIT) != 0);
    auto config = options(th::ChildCase::InheritanceProbe);
    config.unrelatedHandle = reinterpret_cast<std::uintptr_t>(event);
    const auto result = run(config);
    stoppedAndBound(result);
    CHECK(result.disposition == th::Disposition::Returned);
    CHECK_EQ(WaitForSingleObject(event, 0), static_cast<DWORD>(WAIT_TIMEOUT));
}

TEST_CASE(terminal_preassignment_failure_stops_suspended_child_before_any_worker_code)
{
    auto config = options(th::ChildCase::Terminal);
    config.failBeforeJobAssignment = true;
    const auto result = run(config);
    stoppedAndBound(result);
    CHECK(!result.resumed); CHECK(result.supervisorStopRequested);
    CHECK(result.stopReason == th::StopReason::LaunchFailure);
    CHECK(result.disposition == th::Disposition::HarnessFailure);
    CHECK(!result.transcript.started); CHECK_EQ(result.childStatus, th::supervisorStopStatus);
    CHECK(!fs::exists(config.directory / "unwound.marker"));
}

int main(int argc, char** argv)
{
    if (argc != 1) return th::childMain(argc, argv);
    const int result = testkit::run_all();
    try { finishRun(result == 0); }
    catch (const std::exception& error) {
        std::fprintf(stderr, "terminal harness diagnostic/cleanup failed: %s\n", error.what());
        return result ? result : 1;
    }
    return result;
}
