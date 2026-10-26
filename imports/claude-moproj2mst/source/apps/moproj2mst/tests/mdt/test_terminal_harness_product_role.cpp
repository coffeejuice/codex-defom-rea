// ctest-labels: m2m-mdt
// Product role/scratch transport only. No product core conversion or terminal endpoint runs here.
#include <testkit/check.hpp>
#include "../support/test_paths.hpp"
#include "../../src/engine/terminal_harness_supervisor.hpp"
#include <winioctl.h>
#include <array>
#include <atomic>
#include <charconv>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <memory>
#include <stdexcept>
#include <string_view>
#include <vector>

namespace th = m2m::engine::terminal_harness;
namespace fs = std::filesystem;
namespace {
const fs::path& runRoot()
{
    static const auto path = [] {
        for (unsigned n = 0; n < 1000; ++n) {
            const auto candidate = m2m_test::scratchOutput(m2m_test::scratchRoot() / ("terminal-product-" +
                std::to_string(GetCurrentProcessId()) + "-" + std::to_string(GetTickCount64()) + "-" + std::to_string(n)));
            if (fs::create_directory(candidate)) return fs::canonical(candidate);
        }
        throw std::runtime_error("cannot reserve product-role root");
    }();
    return path;
}
std::vector<std::string>& records() { static std::vector<std::string> values; return values; }
th::Options options()
{
    static std::atomic<unsigned> sequence{0};
    static const auto image = th::ownExecutable();
    static const auto hash = th::executableSha256(image);
    const auto root = runRoot() / "explicit";
    fs::create_directories(root);
    th::Options config;
    config.allowOwnedWorkerTermination = true; config.executable = image; config.expectedSha256 = hash;
    config.workerRole = th::WorkerRole::ProductIsolatedSave;
    config.protocolProfile = th::ProtocolProfile::RemeshV2; config.childCase = th::ChildCase::ProductIsolated;
    config.explicitScratchRoot = fs::canonical(root);
    config.directory = config.explicitScratchRoot / ("case-" + std::to_string(++sequence));
    return config;
}
th::Observation run(const th::Options& config)
{
    auto value = th::supervise(config);
    records().push_back("root=" + config.explicitScratchRoot.string() + ";directory=" + config.directory.string() +
        ";disposition=" + th::name(value.disposition) + ";reason=" + th::name(value.stopReason) +
        ";created=" + std::to_string(value.childCreated) + ";stopped=" + std::to_string(value.childStopped) +
        ";status=" + std::to_string(value.childStatus) + ";elapsed=" + std::to_string(value.elapsedMs) +
        ";before_sha=" + value.sha256Before + ";after_sha=" + value.sha256After + ";diagnostic=" + value.diagnostic);
    return value;
}
struct WithoutScratchEnvironment {
    std::wstring saved;
    bool existed = false;
    WithoutScratchEnvironment() {
        std::array<wchar_t, 32768> bytes{}; SetLastError(ERROR_SUCCESS);
        const auto count = GetEnvironmentVariableW(L"M2M_TEST_SCRATCH", bytes.data(), static_cast<DWORD>(bytes.size()));
        existed = count != 0 || GetLastError() != ERROR_ENVVAR_NOT_FOUND;
        if (count >= bytes.size()) throw std::runtime_error("scratch environment exceeds bound");
        saved.assign(bytes.data(), count);
        if (!SetEnvironmentVariableW(L"M2M_TEST_SCRATCH", nullptr)) throw std::runtime_error("cannot isolate scratch environment");
    }
    ~WithoutScratchEnvironment() { SetEnvironmentVariableW(L"M2M_TEST_SCRATCH", existed ? saved.c_str() : nullptr); }
};
template<class T> bool number(const char* text, T& value)
{
    const std::string_view input(text);
    const auto parsed = std::from_chars(input.data(), input.data() + input.size(), value);
    return !input.empty() && parsed.ec == std::errc{} && parsed.ptr == input.data() + input.size();
}
int productTransportChild(int argc, char** argv)
{
    unsigned raw = 0; std::uintptr_t pipeValue = 0, unrelated = 0; std::uint64_t nonce = 0;
    if (argc != 7 || std::string_view(argv[1]) != "--m2m-isolated-save-worker-v1" ||
        std::string_view(argv[2]) != "--allow-owned-worker-termination" || !number(argv[3], raw) ||
        !number(argv[4], pipeValue) || !number(argv[5], nonce) || !number(argv[6], unrelated) || !nonce || unrelated ||
        !th::validWorkerSelection(th::WorkerRole::ProductIsolatedSave, th::ProtocolProfile::RemeshV2, static_cast<th::ChildCase>(raw)))
        return 65;
    const auto pipe = reinterpret_cast<HANDLE>(pipeValue);
    if (GetFileType(pipe) != FILE_TYPE_PIPE) return 65;
    if (fs::exists("bound-nonce.txt")) {
        std::ifstream input("bound-nonce.txt"); std::uint64_t bound = 0;
        if (!(input >> bound) || bound != nonce) return 65;
    }
    auto send = [&](const th::FrameV2& frame) {
        const auto bytes = th::encodeV2(frame); DWORD count = 0;
        return WriteFile(pipe, bytes.data(), static_cast<DWORD>(bytes.size()), &count, nullptr) && count == bytes.size();
    };
    if (!send({th::Kind::Started, nonce, th::remeshSite, 0, 0, GetCurrentProcessId()})) return 66;
    if (!send({th::Kind::Returned, nonce})) return 66;
    return 0;
}
// A mount-point junction needs no symlink-mode privilege and stays wholly in this owned test root.
class Junction {
    fs::path link_;
public:
    Junction(const fs::path& link, const fs::path& target) : link_(link)
    {
        if (!fs::create_directory(link_)) throw std::runtime_error("cannot reserve junction fixture");
        const auto substitute = L"\\??\\" + fs::canonical(target).wstring();
        const auto printable = fs::canonical(target).wstring();
        struct Header {
            DWORD tag;
            WORD length, reserved, substituteOffset, substituteBytes, printOffset, printBytes;
        };
        static_assert(sizeof(Header) == 16);
        const auto strings = (substitute.size() + printable.size() + 2) * sizeof(wchar_t);
        std::vector<unsigned char> data(sizeof(Header) + strings);
        Header header{IO_REPARSE_TAG_MOUNT_POINT, static_cast<WORD>(8 + strings), 0, 0,
            static_cast<WORD>(substitute.size() * sizeof(wchar_t)),
            static_cast<WORD>((substitute.size() + 1) * sizeof(wchar_t)),
            static_cast<WORD>(printable.size() * sizeof(wchar_t))};
        std::memcpy(data.data(), &header, sizeof(header));
        std::memcpy(data.data() + sizeof(header), substitute.c_str(), (substitute.size() + 1) * sizeof(wchar_t));
        std::memcpy(data.data() + sizeof(header) + header.printOffset, printable.c_str(), (printable.size() + 1) * sizeof(wchar_t));
        HANDLE file = CreateFileW(link_.c_str(), GENERIC_WRITE, 0, nullptr, OPEN_EXISTING,
                                  FILE_FLAG_OPEN_REPARSE_POINT | FILE_FLAG_BACKUP_SEMANTICS, nullptr);
        DWORD count = 0;
        const bool ok = file != INVALID_HANDLE_VALUE && DeviceIoControl(file, FSCTL_SET_REPARSE_POINT,
            data.data(), static_cast<DWORD>(data.size()), nullptr, 0, &count, nullptr);
        if (file != INVALID_HANDLE_VALUE) CloseHandle(file);
        if (!ok) { RemoveDirectoryW(link_.c_str()); throw std::runtime_error("cannot create owned junction fixture"); }
    }
    ~Junction() { RemoveDirectoryW(link_.c_str()); } // remove link only, never recurse through it
};
void finishRun(bool passed)
{
    const auto root = fs::canonical(runRoot()), allowed = fs::canonical(m2m_test::scratchRoot());
    const auto relative = root.lexically_relative(allowed);
    if (relative.empty() || relative.is_absolute() || *relative.begin() == ".." ||
        root.filename().wstring().find(L"terminal-product-") != 0)
        throw std::runtime_error("product-role cleanup containment failed");
    std::ofstream receipt(m2m_test::scratchOutput(allowed / "terminal-product-role-last-run.txt"), std::ios::binary | std::ios::trunc);
    if (!receipt) throw std::runtime_error("cannot retain product-role receipt");
    receipt << "qualification=role-and-scratch-control-only;no-product-conversion\npassed=" << passed << '\n';
    std::size_t total = 0;
    for (const auto& record : records()) {
        if ((total += record.size()) > 65536) throw std::runtime_error("product-role summary exceeds bound");
        receipt << record << '\n';
    }
    receipt.close(); if (receipt.fail()) throw std::runtime_error("cannot flush product-role summary");
    if (passed) fs::remove_all(root); // canonical exclusively created root checked above; junction removed by its owner
    else std::fprintf(stderr, "product-role failure captures: %s\n", root.string().c_str());
}
}

TEST_CASE(terminal_product_role_requires_its_dedicated_case_and_v2_profile)
{
    CHECK_EQ(static_cast<unsigned>(th::ChildCase::ProductIsolated), 25u);
    CHECK(th::validWorkerSelection(th::WorkerRole::ProductIsolatedSave, th::ProtocolProfile::RemeshV2, th::ChildCase::ProductIsolated));
    CHECK(!th::validWorkerSelection(th::WorkerRole::ProductIsolatedSave, th::ProtocolProfile::SafetyV1, th::ChildCase::ProductIsolated));
    CHECK(!th::validWorkerSelection(th::WorkerRole::ProductIsolatedSave, th::ProtocolProfile::RemeshV2, th::ChildCase::ConversionNormal));
    CHECK(!th::validWorkerSelection(th::WorkerRole::RemeshConversion, th::ProtocolProfile::RemeshV2, th::ChildCase::ProductIsolated));
    CHECK(!th::validWorkerSelection(th::WorkerRole::SelfTest, th::ProtocolProfile::RemeshV2, th::ChildCase::ProductIsolated));
    CHECK(th::Options{}.explicitScratchRoot.empty()); CHECK(th::Options{}.workerRole == th::WorkerRole::SelfTest);
}

TEST_CASE(terminal_product_explicit_root_works_without_test_environment_and_keeps_lease)
{
    auto config = options(); auto inspected = std::make_shared<bool>(false);
    auto binds = std::make_shared<unsigned>(0);
    struct Lease final : th::PreparedCase {
        std::shared_ptr<bool> flag;
        std::shared_ptr<unsigned> binds;
        fs::path directory;
        Lease(std::shared_ptr<bool> value, std::shared_ptr<unsigned> count, fs::path path)
            : flag(std::move(value)), binds(std::move(count)), directory(std::move(path)) {}
        void bindNonce(std::uint64_t nonce) override {
            ++*binds;
            if (!nonce) throw std::runtime_error("zero nonce");
            std::ofstream file(directory / "bound-nonce.txt"); file << nonce; file.close();
            if (file.fail()) throw std::runtime_error("cannot write bound nonce fixture");
        }
        void inspectStoppedCase(const th::Observation& value) override { *flag = value.childStopped; }
    };
    config.prepareOwnedCase = [inspected, binds](const fs::path& path) { return std::make_unique<Lease>(inspected, binds, path); };
    th::Observation observed;
    { WithoutScratchEnvironment clear; observed = run(config); }
    CHECK(observed.disposition == th::Disposition::Returned); CHECK(observed.childCreated); CHECK(observed.resumed); CHECK(observed.childStopped);
    CHECK(observed.transcript.returned); CHECK(observed.casePrepared); CHECK(observed.caseInspected); CHECK(*inspected);
    CHECK_EQ(*binds, 1u);
    CHECK_EQ(observed.sha256Before, observed.sha256After); CHECK(observed.elapsedMs < 6500);
}

TEST_CASE(terminal_product_explicit_root_is_forbidden_for_older_roles_before_prepare)
{
    for (bool conversion : {false, true}) {
        auto config = options(); bool prepared = false;
        config.workerRole = conversion ? th::WorkerRole::RemeshConversion : th::WorkerRole::SelfTest;
        config.protocolProfile = conversion ? th::ProtocolProfile::RemeshV2 : th::ProtocolProfile::SafetyV1;
        config.childCase = conversion ? th::ChildCase::ConversionNormal : th::ChildCase::Returned;
        config.prepareOwnedCase = [&prepared](const fs::path&) -> std::unique_ptr<th::PreparedCase> {
            prepared = true; return std::make_unique<th::PreparedCase>();
        };
        const auto observed = run(config);
        CHECK(observed.disposition == th::Disposition::HarnessFailure); CHECK(!observed.childCreated); CHECK(!prepared);
        CHECK(!fs::exists(config.directory));
    }
}

TEST_CASE(terminal_product_explicit_root_and_direct_leaf_fail_closed_on_invalid_paths)
{
    const auto file = runRoot() / "regular-file"; { std::ofstream out(file); out << "KEEP"; }
    for (unsigned fault = 0; fault < 7; ++fault) {
        auto config = options();
        if (fault == 0) config.explicitScratchRoot = "relative-root";
        if (fault == 1) config.explicitScratchRoot = runRoot() / "missing-root";
        if (fault == 2) config.explicitScratchRoot = file;
        if (fault == 3) config.explicitScratchRoot = runRoot() / "explicit" / ".." / "explicit";
        if (fault == 4) config.directory = config.explicitScratchRoot / "missing-parent" / "leaf";
        if (fault == 5) config.directory = runRoot() / "not-a-direct-child";
        if (fault == 6) fs::create_directory(config.directory);
        const auto observed = run(config);
        CHECK(observed.disposition == th::Disposition::HarnessFailure); CHECK(!observed.childCreated);
        CHECK(!observed.casePrepared);
    }
}

TEST_CASE(terminal_product_explicit_root_rejects_reparse_root_and_ancestor)
{
    const auto target = runRoot() / "junction-target"; fs::create_directories(target / "child");
    const auto link = runRoot() / "junction"; Junction junction(link, target);
    const auto attributes = GetFileAttributesW(link.c_str());
    REQUIRE(attributes != INVALID_FILE_ATTRIBUTES);
    REQUIRE((attributes & FILE_ATTRIBUTE_REPARSE_POINT) != 0);
    for (bool ancestor : {false, true}) {
        auto config = options();
        config.explicitScratchRoot = ancestor ? link / "child" : link;
        config.directory = config.explicitScratchRoot / "denied-leaf";
        const auto observed = run(config);
        CHECK(observed.disposition == th::Disposition::HarnessFailure); CHECK(!observed.childCreated);
        CHECK(!fs::exists(target / (ancestor ? "child/denied-leaf" : "denied-leaf")));
    }
}

TEST_CASE(terminal_product_empty_root_retains_legacy_scratch_environment_policy)
{
    auto config = options(); config.explicitScratchRoot.clear();
    auto observed = run(config);
    CHECK(observed.disposition == th::Disposition::Returned); CHECK(observed.childStopped);
    config = options(); config.explicitScratchRoot.clear();
    { WithoutScratchEnvironment clear; observed = run(config); }
    CHECK(observed.disposition == th::Disposition::HarnessFailure); CHECK(!observed.childCreated);
}

TEST_CASE(terminal_product_nonce_binding_failure_is_sticky_and_releases_unlaunched_lease)
{
    auto config = options(); bool destroyed = false;
    struct Lease final : th::PreparedCase {
        bool& destroyed;
        explicit Lease(bool& value) : destroyed(value) {}
        ~Lease() override { destroyed = true; }
        void bindNonce(std::uint64_t) override { throw std::runtime_error("declared binding failure"); }
    };
    config.prepareOwnedCase = [&destroyed](const fs::path&) { return std::make_unique<Lease>(destroyed); };
    const auto observed = run(config);
    CHECK(observed.disposition == th::Disposition::HarnessFailure);
    CHECK(observed.stopReason == th::StopReason::Preparation);
    CHECK(observed.casePrepared); CHECK(!observed.childCreated); CHECK(!observed.caseInspected); CHECK(destroyed);
}

TEST_CASE(terminal_product_nonce_binding_deadline_prevents_child_creation)
{
    auto config = options(); config.deadlineMs = 50;
    struct Lease final : th::PreparedCase { void bindNonce(std::uint64_t) override { Sleep(70); } };
    config.prepareOwnedCase = [](const fs::path&) { return std::make_unique<Lease>(); };
    const auto observed = run(config);
    CHECK(observed.disposition == th::Disposition::Timeout);
    CHECK(observed.stopReason == th::StopReason::Timeout);
    CHECK(observed.casePrepared); CHECK(!observed.childCreated); CHECK(observed.elapsedMs < 2500);
}

int main(int argc, char** argv)
{
    if (argc > 1 && std::string_view(argv[1]) == "--m2m-isolated-save-worker-v1") return productTransportChild(argc, argv);
    if (argc != 1) return th::childMain(argc, argv);
    const int result = testkit::run_all();
    try { finishRun(result == 0); }
    catch (const std::exception& error) { std::fprintf(stderr, "product-role receipt/cleanup failed: %s\n", error.what()); return result ? result : 1; }
    return result;
}
