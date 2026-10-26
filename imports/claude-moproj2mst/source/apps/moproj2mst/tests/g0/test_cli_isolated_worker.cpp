// ctest-labels: cli isolated-worker
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <testkit/check.hpp>
#include "../../src/cli/isolated_save.hpp"
#include "../../src/cli/isolated_save_job.hpp"
#include "../support/test_paths.hpp"
#include "moproj2mst.h"
#include <array>
#include <fstream>
#include <memory>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace fs = std::filesystem;
namespace th = m2m::engine::terminal_harness;
namespace {
constexpr std::wstring_view role = L"--m2m-isolated-save-worker-v1";
fs::path freshPath()
{
    static unsigned sequence = 0;
    const auto path = m2m_test::scratchOutput(m2m_test::scratchRoot() /
        ("cli-worker-" + std::to_string(GetCurrentProcessId()) + "-" +
         std::to_string(GetTickCount64()) + "-" + std::to_string(++sequence)));
    REQUIRE(!fs::exists(path));
    return path;
}
struct EmptyCurrentDirectory {
    fs::path previous = fs::current_path();
    EmptyCurrentDirectory() {
        const auto empty = freshPath();
        REQUIRE(fs::create_directory(empty));
        fs::current_path(empty);
    }
    ~EmptyCurrentDirectory() { std::error_code error; fs::current_path(previous, error); }
};
struct Pipe {
    HANDLE read = INVALID_HANDLE_VALUE, write = INVALID_HANDLE_VALUE;
    Pipe() { REQUIRE(CreatePipe(&read, &write, nullptr, 0)); }
    ~Pipe() { CloseHandle(read); CloseHandle(write); }
    void checkUnused() const {
        DWORD available = 0;
        REQUIRE(PeekNamedPipe(read, nullptr, 0, nullptr, &available, nullptr));
        CHECK_EQ(available, 0U);
    }
};
using Arguments = std::array<std::wstring, 7>;
Arguments arguments(const Pipe& pipe)
{
    return {L"test_cli_isolated_worker", std::wstring(role), L"--allow-owned-worker-termination", L"25",
        std::to_wstring(reinterpret_cast<std::uintptr_t>(pipe.write)), L"1", L"0"};
}
std::array<wchar_t*, 7> pointers(Arguments& values)
{
    std::array<wchar_t*, 7> result{};
    for (std::size_t i = 0; i < result.size(); ++i) result[i] = values[i].data();
    return result;
}
int invoke(Arguments values)
{
    auto argv = pointers(values);
    return m2m::cli::isolatedSaveWorkerMain(static_cast<int>(argv.size()), argv.data());
}
th::Options options()
{
    th::Options result;
    result.allowOwnedWorkerTermination = true;
    result.executable = th::ownExecutable();
    result.expectedSha256 = th::executableSha256(result.executable);
    result.directory = freshPath();
    result.workerRole = th::WorkerRole::ProductIsolatedSave;
    result.childCase = th::ChildCase::ProductIsolated;
    result.protocolProfile = th::ProtocolProfile::RemeshV2;
    result.deadlineMs = 20000;
    return result;
}
m2m::cli::IsolatedJob missingProjectJob(const fs::path& leaf)
{
    m2m::cli::IsolatedJob job;
    job.project = (leaf / "missing.moproj").u16string();
    job.output = (leaf / "out.MST").u16string();
    job.workingDirectory = leaf.u16string();
    job.flags = M2M_FLAG_WRITE_SAVE;
    job.codepage = 1252;
    return job;
}
enum class JobMode { Valid, MissingDirectory, StaleNonce };
class JobLease final : public th::PreparedCase {
    fs::path leaf_;
    JobMode mode_;
public:
    JobLease(const fs::path& leaf, JobMode mode) : leaf_(leaf), mode_(mode) {}
    void bindNonce(std::uint64_t nonce) override {
        auto job = missingProjectJob(leaf_);
        job.nonce = nonce;
        if (mode_ == JobMode::MissingDirectory)
            job.workingDirectory = (leaf_ / "missing-caller-directory").u16string();
        if (mode_ == JobMode::StaleNonce) job.nonce = nonce == 1 ? 2 : 1;
        m2m::cli::writeIsolatedJob(leaf_, job);
    }
};
void record(const th::Options& config, const th::Observation& observed)
{
    std::ofstream out(config.directory / "worker-test-observation.txt", std::ios::binary);
    REQUIRE(out.good());
    out << "scope=product-worker-code-in-owned-test-child;not-original-runtime\n"
        << "qualification=candidate-fixed-endpoint;native-default-handler-prelude-unimplemented\n"
        << "child_stopped=" << observed.childStopped << "\nstatus=" << observed.childStatus
        << "\nreturned=" << observed.transcript.returned << "\nadapter_code=" << observed.transcript.returnedCode
        << "\ndiagnostic=" << observed.diagnostic << '\n';
    out.close(); CHECK(!out.fail());
}
}

TEST_CASE(cli_isolated_worker_rejects_wrong_arity_and_null_arguments_before_protocol)
{
    EmptyCurrentDirectory empty; Pipe pipe;
    auto values = arguments(pipe); auto argv = pointers(values);
    CHECK_EQ(m2m::cli::isolatedSaveWorkerMain(0, nullptr), 65);
    CHECK_EQ(m2m::cli::isolatedSaveWorkerMain(7, nullptr), 65);
    CHECK_EQ(m2m::cli::isolatedSaveWorkerMain(6, argv.data()), 65);
    CHECK_EQ(m2m::cli::isolatedSaveWorkerMain(8, argv.data()), 65);
    argv[5] = nullptr;
    CHECK_EQ(m2m::cli::isolatedSaveWorkerMain(7, argv.data()), 65);
    pipe.checkUnused();
}

TEST_CASE(cli_isolated_worker_requires_exact_role_optin_and_product_case)
{
    EmptyCurrentDirectory empty; Pipe pipe;
    for (const auto& [index, replacement] : std::vector<std::pair<unsigned, std::wstring>>{
        {1, L"--terminal-remesh-conversion-worker"}, {1, L"--m2m-isolated-save-worker-v2"},
        {2, L""}, {2, L"--allow-owned-worker-termination-extra"},
        {3, L"0"}, {3, L"24"}, {3, L"26"}}) {
        auto values = arguments(pipe); values[index] = replacement;
        CHECK_EQ(invoke(std::move(values)), 65); pipe.checkUnused();
    }
}

TEST_CASE(cli_isolated_worker_rejects_signed_hex_overflow_zero_nonce_and_unrelated_handle)
{
    EmptyCurrentDirectory empty; Pipe pipe;
    for (const auto& [index, replacement] : std::vector<std::pair<unsigned, std::wstring>>{
        {3, L"+25"}, {3, L" 25"}, {3, L"25x"}, {3, L"4294967296"},
        {4, L"0"}, {4, L"-1"}, {4, L"0x20"}, {4, L"18446744073709551616"},
        {5, L"0"}, {5, L"-1"}, {5, L"18446744073709551616"},
        {6, L"1"}, {6, L""}}) {
        auto values = arguments(pipe); values[index] = replacement;
        CHECK_EQ(invoke(std::move(values)), 65); pipe.checkUnused();
    }
}

TEST_CASE(cli_isolated_worker_publishes_adapter_parse_error_with_successful_transport)
{
    auto config = options();
    config.prepareOwnedCase = [](const fs::path& leaf) {
        return std::make_unique<JobLease>(leaf, JobMode::Valid);
    };
    const auto observed = th::supervise(config); record(config, observed);
    CHECK(observed.childStopped); CHECK(observed.disposition == th::Disposition::Returned);
    CHECK_EQ(observed.childStatus, 0U);
    CHECK(observed.transcript.started); CHECK(observed.transcript.returned);
    CHECK_EQ(observed.transcript.returnedCode, static_cast<unsigned>(M2M_E_PARSE));
    CHECK_EQ(observed.transcript.frames, 2U);
    CHECK(!observed.transcript.boundary); CHECK(!observed.transcript.backendFailure);
    CHECK(!observed.transcript.invalid); CHECK(!observed.supervisorStopRequested);
    CHECK(!fs::exists(config.directory / "out.MST"));
}

TEST_CASE(cli_isolated_worker_malformed_job_is_not_a_returned_conversion)
{
    auto config = options();
    config.prepareOwnedCase = [](const fs::path& leaf) {
        std::ofstream out(leaf / "isolated-save.job", std::ios::binary);
        out << "bad"; out.close(); REQUIRE(!out.fail());
        return std::make_unique<th::PreparedCase>();
    };
    const auto observed = th::supervise(config); record(config, observed);
    CHECK(observed.childStopped); CHECK(observed.disposition == th::Disposition::HarnessFailure);
    CHECK_EQ(observed.childStatus, 66U); CHECK(observed.transcript.started);
    CHECK(!observed.transcript.returned); CHECK(!observed.transcript.boundary);
}

TEST_CASE(cli_isolated_worker_missing_caller_directory_has_no_trustworthy_returned_record)
{
    auto config = options();
    config.prepareOwnedCase = [](const fs::path& leaf) {
        return std::make_unique<JobLease>(leaf, JobMode::MissingDirectory);
    };
    const auto observed = th::supervise(config); record(config, observed);
    CHECK(observed.childStopped); CHECK(observed.disposition == th::Disposition::HarnessFailure);
    CHECK_EQ(observed.childStatus, 66U); CHECK(observed.transcript.started);
    CHECK(!observed.transcript.returned); CHECK(!observed.transcript.boundary);
}

TEST_CASE(cli_isolated_worker_rejects_stale_job_nonce_before_cwd_or_conversion)
{
    auto config = options();
    config.prepareOwnedCase = [](const fs::path& leaf) {
        return std::make_unique<JobLease>(leaf, JobMode::StaleNonce);
    };
    const auto observed = th::supervise(config); record(config, observed);
    CHECK(observed.childStopped); CHECK(observed.disposition == th::Disposition::HarnessFailure);
    CHECK_EQ(observed.childStatus, 66U); CHECK(observed.transcript.started);
    CHECK(observed.transcript.invalid);
    CHECK_EQ(observed.transcript.diagnostic, "v2 EOF without final outcome");
    CHECK(!observed.supervisorStopRequested);
    CHECK(!observed.transcript.returned); CHECK(!observed.transcript.boundary);
    CHECK(!fs::exists(config.directory / "out.MST"));
}

int wmain(int argc, wchar_t** argv)
{
    if (argc > 1 && std::wstring_view(argv[1]) == role)
        return m2m::cli::isolatedSaveWorkerMain(argc, argv);
    return testkit::run_all();
}
