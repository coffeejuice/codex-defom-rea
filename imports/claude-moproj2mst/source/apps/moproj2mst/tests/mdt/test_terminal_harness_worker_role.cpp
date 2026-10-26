// ctest-labels: m2m-mdt
// Role/argv transport only; this executable never calls conversion or the remesh service.
#include <testkit/check.hpp>
#include "../support/test_paths.hpp"
#include "../../src/engine/terminal_harness_supervisor.hpp"
#include <atomic>
#include <charconv>
#include <fstream>
#include <memory>
#include <string_view>

namespace th = m2m::engine::terminal_harness;
namespace fs = std::filesystem;
namespace {
const fs::path& runRoot()
{
    static const auto value = [] {
        for (unsigned i = 0; i < 1000; ++i) {
            const auto path = m2m_test::scratchOutput(m2m_test::scratchRoot() / ("terminal-role-" +
                std::to_string(GetCurrentProcessId()) + "-" + std::to_string(GetTickCount64()) + "-" + std::to_string(i)));
            if (fs::create_directory(path)) return path;
        }
        throw std::runtime_error("cannot reserve role test root");
    }();
    return value;
}
std::vector<std::string>& records() { static std::vector<std::string> values; return values; }
th::Options options()
{
    static std::atomic<unsigned> sequence{0};
    static const auto image = th::ownExecutable();
    static const auto hash = th::executableSha256(image);
    th::Options out;
    out.allowOwnedWorkerTermination = true; out.executable = image; out.expectedSha256 = hash;
    out.directory = runRoot() / ("case-" + std::to_string(++sequence));
    out.protocolProfile = th::ProtocolProfile::RemeshV2;
    out.workerRole = th::WorkerRole::RemeshConversion;
    out.childCase = th::ChildCase::ConversionNormal;
    return out;
}
th::Observation run(const th::Options& config)
{
    auto out = th::supervise(config);
    records().push_back("case=" + std::to_string(static_cast<unsigned>(config.childCase)) +
        ";role=" + std::to_string(static_cast<unsigned>(config.workerRole)) +
        ";profile=" + std::to_string(static_cast<unsigned>(config.protocolProfile)) +
        ";disposition=" + th::name(out.disposition) + ";reason=" + th::name(out.stopReason) +
        ";created=" + std::to_string(out.childCreated) + ";stopped=" + std::to_string(out.childStopped) +
        ";status=" + std::to_string(out.childStatus) + ";elapsed=" + std::to_string(out.elapsedMs) +
        ";before_sha=" + out.sha256Before + ";after_sha=" + out.sha256After);
    return out;
}
template<class Integer> bool number(const char* text, Integer& value)
{
    const std::string_view view(text);
    const auto parsed = std::from_chars(view.data(), view.data() + view.size(), value);
    return !view.empty() && parsed.ec == std::errc{} && parsed.ptr == view.data() + view.size();
}
int roleTransportChild(int argc, char** argv)
{
    unsigned mode = 0; std::uintptr_t pipeValue = 0, unrelated = 0; std::uint64_t nonce = 0;
    if (argc != 7 || std::string_view(argv[1]) != "--terminal-remesh-conversion-worker" ||
        std::string_view(argv[2]) != "--allow-owned-worker-termination" || !number(argv[3], mode) ||
        !number(argv[4], pipeValue) || !number(argv[5], nonce) || !number(argv[6], unrelated) || !nonce || unrelated ||
        !th::validWorkerSelection(th::WorkerRole::RemeshConversion, th::ProtocolProfile::RemeshV2,
                                  static_cast<th::ChildCase>(mode)) ||
        mode != static_cast<unsigned>(th::ChildCase::ConversionNormal)) return 65;
    HANDLE pipe = reinterpret_cast<HANDLE>(pipeValue);
    if (GetFileType(pipe) != FILE_TYPE_PIPE) return 65;
    std::ifstream fixture("role-input.txt", std::ios::binary);
    std::string contents; std::getline(fixture, contents);
    if (contents != "ROLE-TRANSPORT-ONLY") return 66;
    auto send = [&](const th::FrameV2& frame) {
        const auto bytes = th::encodeV2(frame); DWORD written = 0;
        return WriteFile(pipe, bytes.data(), static_cast<DWORD>(bytes.size()), &written, nullptr) && written == bytes.size();
    };
    if (!send({th::Kind::Started, nonce, th::remeshSite, 0, 0, GetCurrentProcessId()})) return 66;
    if (!send({th::Kind::Returned, nonce})) return 66;
    return 0;
}
void finishRun(bool passed)
{
    const auto root = fs::canonical(runRoot()), allowed = fs::canonical(m2m_test::scratchRoot());
    const auto relative = root.lexically_relative(allowed);
    if (relative.empty() || relative.is_absolute() || *relative.begin() == ".." ||
        root.filename().wstring().find(L"terminal-role-") != 0)
        throw std::runtime_error("role cleanup containment failed");
    std::ofstream receipt(m2m_test::scratchOutput(allowed / "terminal-role-last-run.txt"), std::ios::binary | std::ios::trunc);
    if (!receipt) throw std::runtime_error("cannot retain role receipt");
    receipt << "qualification=role-argv-transport-only;no-conversion-or-native-handler\npassed=" << passed << '\n';
    std::size_t bytes = 0;
    for (const auto& line : records()) {
        if ((bytes += line.size()) > 65536) throw std::runtime_error("role receipt exceeds bound");
        receipt << line << '\n';
    }
    receipt.close(); if (receipt.fail()) throw std::runtime_error("cannot flush role receipt");
    if (passed) fs::remove_all(root); // canonical exclusive child of allowed scratch, checked above
    else std::fprintf(stderr, "role failure captures: %s\n", root.string().c_str());
}
}

TEST_CASE(terminal_worker_role_defaults_and_finite_role_profile_case_matrix)
{
    const th::Options defaults;
    CHECK(defaults.workerRole == th::WorkerRole::SelfTest);
    CHECK(defaults.protocolProfile == th::ProtocolProfile::SafetyV1);
    CHECK(th::validWorkerSelection(defaults.workerRole, defaults.protocolProfile, defaults.childCase));
    CHECK_EQ(static_cast<unsigned>(th::ChildCase::PreparedFileProbe), 16u);
    CHECK_EQ(static_cast<unsigned>(th::ChildCase::ConversionNormal), 17u);
    CHECK_EQ(static_cast<unsigned>(th::ChildCase::ConversionEndpointReturned), 24u);
    for (unsigned value = 17; value <= 24; ++value) {
        const auto mode = static_cast<th::ChildCase>(value);
        CHECK(th::validWorkerSelection(th::WorkerRole::RemeshConversion, th::ProtocolProfile::RemeshV2, mode));
        CHECK(!th::validWorkerSelection(th::WorkerRole::RemeshConversion, th::ProtocolProfile::SafetyV1, mode));
        CHECK(!th::validWorkerSelection(th::WorkerRole::SelfTest, th::ProtocolProfile::SafetyV1, mode));
        CHECK(!th::validWorkerSelection(th::WorkerRole::SelfTest, th::ProtocolProfile::RemeshV2, mode));
    }
    CHECK(!th::validWorkerSelection(static_cast<th::WorkerRole>(99), th::ProtocolProfile::RemeshV2, th::ChildCase::ConversionNormal));
    CHECK(!th::validWorkerSelection(th::WorkerRole::RemeshConversion, static_cast<th::ProtocolProfile>(99), th::ChildCase::ConversionNormal));
    CHECK(!th::validWorkerSelection(th::WorkerRole::RemeshConversion, th::ProtocolProfile::RemeshV2, static_cast<th::ChildCase>(25)));
    CHECK(!th::validWorkerSelection(th::WorkerRole::RemeshConversion, th::ProtocolProfile::RemeshV2, th::ChildCase::Returned));
    CHECK(!th::validWorkerSelection(th::WorkerRole::SelfTest, th::ProtocolProfile::SafetyV1, th::ChildCase::PreparedFileProbe));
    CHECK(!th::validWorkerSelection(th::WorkerRole::SelfTest, th::ProtocolProfile::RemeshV2, th::ChildCase::Terminal));
}

TEST_CASE(terminal_worker_invalid_selection_is_rejected_before_preparation_or_spawn)
{
    for (unsigned fault = 0; fault < 5; ++fault) {
        auto config = options(); bool prepared = false;
        config.prepareOwnedCase = [&prepared](const fs::path&) -> std::unique_ptr<th::PreparedCase> {
            prepared = true; return std::make_unique<th::PreparedCase>();
        };
        if (fault == 0) config.workerRole = th::WorkerRole::SelfTest;
        if (fault == 1) config.protocolProfile = th::ProtocolProfile::SafetyV1;
        if (fault == 2) config.workerRole = static_cast<th::WorkerRole>(99);
        if (fault == 3) config.protocolProfile = static_cast<th::ProtocolProfile>(99);
        if (fault == 4) config.childCase = static_cast<th::ChildCase>(25);
        const auto observed = run(config);
        CHECK(observed.disposition == th::Disposition::HarnessFailure); CHECK(!observed.childCreated);
        CHECK(!prepared); CHECK(!fs::exists(config.directory));
    }
}

TEST_CASE(terminal_conversion_worker_role_argv_reaches_only_explicit_transport_child)
{
    auto config = options();
    struct State { bool inspected = false, released = false; };
    auto state = std::make_shared<State>();
    struct Lease final : th::PreparedCase {
        std::shared_ptr<State> state;
        explicit Lease(std::shared_ptr<State> value) : state(std::move(value)) {}
        ~Lease() override { state->released = true; }
        void inspectStoppedCase(const th::Observation& observed) override { state->inspected = observed.childStopped; }
    };
    config.prepareOwnedCase = [state](const fs::path& directory) -> std::unique_ptr<th::PreparedCase> {
        std::ofstream input(directory / "role-input.txt", std::ios::binary); input << "ROLE-TRANSPORT-ONLY\n";
        if (!input) throw std::runtime_error("cannot seed role transport fixture");
        return std::make_unique<Lease>(state);
    };
    const auto observed = run(config);
    CHECK(observed.childCreated); CHECK(observed.resumed); CHECK(observed.childStopped);
    CHECK(observed.disposition == th::Disposition::Returned); CHECK_EQ(observed.childStatus, 0u);
    CHECK(observed.transcript.profile == th::ProtocolProfile::RemeshV2); CHECK(observed.transcript.returned);
    CHECK(!observed.transcript.boundary); CHECK(!observed.transcript.save);
    CHECK(observed.casePrepared); CHECK(observed.caseInspected); CHECK(state->inspected); CHECK(state->released);
    CHECK_EQ(observed.sha256Before, observed.sha256After); CHECK(observed.elapsedMs < 6500);
}

TEST_CASE(terminal_conversion_worker_role_still_requires_parent_opt_in)
{
    auto config = options(); config.allowOwnedWorkerTermination = false;
    const auto observed = run(config);
    CHECK(observed.disposition == th::Disposition::HarnessFailure); CHECK(!observed.childCreated);
    CHECK(!fs::exists(config.directory));
}

TEST_CASE(terminal_existing_child_main_rejects_conversion_worker_role_and_case)
{
    char image[] = "role-test"; char role[] = "--terminal-remesh-conversion-worker";
    char opt[] = "--allow-owned-worker-termination"; char mode[] = "17";
    char pipe[] = "0"; char nonce[] = "1"; char unrelated[] = "0";
    char* args[]{image, role, opt, mode, pipe, nonce, unrelated};
    CHECK_EQ(th::childMain(7, args), 65); // rejected before pipe access or any endpoint
    char selfRole[] = "--terminal-harness-child"; args[1] = selfRole;
    CHECK_EQ(th::childMain(7, args), 65); // conversion case is outside supplied self-test main
}

int main(int argc, char** argv)
{
    if (argc > 1 && std::string_view(argv[1]) == "--terminal-remesh-conversion-worker") return roleTransportChild(argc, argv);
    if (argc != 1) return th::childMain(argc, argv);
    const int result = testkit::run_all();
    try { finishRun(result == 0); }
    catch (const std::exception& error) { std::fprintf(stderr, "role receipt/cleanup failed: %s\n", error.what()); return result ? result : 1; }
    return result;
}
