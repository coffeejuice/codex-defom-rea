#include "../support/test_paths.hpp"
// Candidate-only conjunction of project/MST failures and ignored disabled-remesh delete failure.
// ctest-labels: m2m-s
#include <testkit/check.hpp>
#include <fstream>
#include <functional>
#include <iterator>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

#include "m2m/convert.hpp"
#include "m2m/convert_details.hpp"
#include "m2m/moproj_write.hpp"
#include "m2m/mst/file_device.hpp"
#include "m2m/qt4path.hpp"
#include "../../src/core/conversion_call_services.hpp"
#include "../../src/core/remesh_terminal_call_service.hpp"
#include "../../src/moproj/project_output_device.hpp"

namespace fs = std::filesystem;
using namespace m2m;

namespace {
std::string read(const fs::path& path)
{
    std::ifstream input(path, std::ios::binary);
    return {std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>()};
}

void write(const fs::path& path, const std::string& contents)
{
    std::ofstream output(path, std::ios::binary | std::ios::trunc);
    REQUIRE(output.is_open());
    output << contents;
    REQUIRE(output.good());
}

struct Handle {
    HANDLE value = INVALID_HANDLE_VALUE;
    Handle(const fs::path& path, DWORD share, DWORD access = GENERIC_READ)
    {
        value = CreateFileW(path.c_str(), access, share, nullptr, OPEN_EXISTING,
                            FILE_ATTRIBUTE_NORMAL, nullptr);
        REQUIRE(value != INVALID_HANDLE_VALUE);
    }
    ~Handle() { if (value != INVALID_HANDLE_VALUE) CloseHandle(value); }
    Handle(const Handle&) = delete;
};

struct Rig {
    fs::path root, project, task, bwz, ini, mst, remesh;

    Rig()
    {
        static unsigned long serial = 0;
        root = m2m_test::scratchOutput(m2m_test::scratchRoot() /
            ("save-remesh-matrix-" + std::to_string(GetCurrentProcessId()) + "-" +
             std::to_string(GetTickCount64()) + "-" + std::to_string(++serial)));
        REQUIRE(!fs::exists(root));
        fs::copy(m2m_test::configuredPath("M2M_ORACLE_B") / "133_B_DBSTEP_BOOL3_before",
                 root, fs::copy_options::recursive);
        project = root / "COG_P1_B1.moproj";
        task = root / "Operations" / "Task00002";
        bwz = task / "Boolean.BWZ";
        ini = task / "DEF_MACHDIST_CTRL2.INI";
        mst = root / "native-output.MST";
        remesh = root / "LOCAL_REMESH.DAT";
    }
};

struct Observation {
    fs::path mst;
    std::vector<std::string> events;
    std::function<void()> aboutToCloseAction;
    int reportedError = 7;
    int closeCount = 0;
    int errorQueries = 0;
};

class DelegatingOutput final : public moproj::detail::ProjectOutputDevice {
public:
    DelegatingOutput(std::unique_ptr<mst::FileDevice> file, Observation& observation)
        : file_(std::move(file)), observation_(observation)
    {
        file_->setAboutToClose([this] {
            observation_.events.emplace_back("aboutToClose");
            if (observation_.aboutToCloseAction) observation_.aboutToCloseAction();
            notifyAboutToClose();
        });
    }

    ~DelegatingOutput() override
    {
        if (file_->opened()) close();
        file_->setAboutToClose({});
    }

    std::int64_t write(std::string_view bytes) override { return file_->write(bytes); }
    bool flush() override { return file_->flush(); }
    void close() override
    {
        if (!file_->opened()) return;
        file_->close();
        ++observation_.closeCount;
        observation_.events.emplace_back("close returned");
    }
    bool opened() const override { return file_->opened(); }
    int error() const override
    {
        ++observation_.errorQueries;
        observation_.events.emplace_back("error query");
        return observation_.reportedError;
    }

private:
    std::unique_ptr<mst::FileDevice> file_;
    Observation& observation_;
};

class OutputFactory final : public moproj::detail::ProjectOutputFactory {
public:
    explicit OutputFactory(Observation& observation) : observation_(observation) {}
    std::unique_ptr<moproj::detail::ProjectOutputDevice> open(const qt::String& path) override
    {
        observation_.events.emplace_back("project output open");
        auto file = std::make_unique<mst::FileDevice>(path);
        REQUIRE(file->opened());
        return std::make_unique<DelegatingOutput>(std::move(file), observation_);
    }

private:
    Observation& observation_;
};

class TerminalSpy final : public detail::RemeshTerminalCallService {
public:
    mutable unsigned profileReads = 0;
    unsigned dispatches = 0;
    unsigned failClosedCalls = 0;

    engine::simcontrol300::detail::RemeshTerminalProfile profile() const noexcept override
    {
        ++profileReads;
        return engine::simcontrol300::detail::RemeshTerminalProfile::FreshDefault;
    }
    engine::simcontrol300::detail::RemeshTerminalDispatchFailure dispatch(
        const engine::simcontrol300::detail::RemeshTerminalRequest&,
        const engine::SaveCallState&) noexcept override
    {
        ++dispatches;
        return engine::simcontrol300::detail::RemeshTerminalDispatchFailure::EndpointReturned;
    }
    void failClosed(const engine::simcontrol300::detail::RemeshTerminalRequest&,
                    engine::simcontrol300::detail::RemeshTerminalDispatchFailure,
                    const engine::SaveCallState&) noexcept override
    {
        ++failClosedCalls;
    }
};
} // namespace

TEST_CASE(save_project_mst_failures_do_not_turn_ignored_disabled_remesh_delete_into_status)
{
    Rig files;
    const std::string remeshSentinel = "DONE\n"; // Unmarked primary with no enabled fallback.
    const std::string mstSentinel = "existing MST retained by write-open denial\n";
    write(files.remesh, remeshSentinel);

    // Permit Open's read but deny DELETE. The handle is scoped to this disposable fixture.
    Handle denyRemeshDelete(files.remesh, FILE_SHARE_READ | FILE_SHARE_WRITE);
    Observation observation;
    observation.mst = files.mst;
    std::unique_ptr<Handle> denyMstWrite;
    observation.aboutToCloseAction = [&] {
        // This callback runs after project/input Open, immediately before the
        // native project writer closes. The lock cannot preempt Open or step 2.
        write(files.mst, mstSentinel);
        denyMstWrite = std::make_unique<Handle>(files.mst, FILE_SHARE_READ);
    };

    OutputFactory factory(observation);
    TerminalSpy terminal;
    detail::ConversionCallServices services{&factory, &terminal};
    ConversionDetails details;
    const Result result = detail::convertDetailedWithServices(
        files.project.u16string(), files.mst.u16string(), M2M_FLAG_WRITE_SAVE, 1252,
        details, services);
    denyMstWrite.reset();

    CHECK((observation.events == std::vector<std::string>{
        "project output open", "aboutToClose", "close returned", "error query"}));
    CHECK_EQ(observation.closeCount, 1);
    CHECK_EQ(observation.errorQueries, 1);
    REQUIRE(details.saveCall.has_value());
    CHECK(details.saveCall->projectWriteAttempted);
    CHECK(details.saveCall->projectWriteReturned);
    CHECK(!details.saveCall->projectWriteSucceeded); // Declared post-close error 7.
    CHECK(details.saveCall->mstWriteAttempted);
    CHECK(details.saveCall->mstWriteReturned);
    CHECK(!details.saveCall->mstWriteSucceeded); // Real disposable write-open denial.

    CHECK(read(files.bwz).find("WZD_BOOLEAN_CUTTER_OBJECT_NO") != std::string::npos);
    CHECK(read(files.ini).find("WZD_MACHDIST_OPERATION_BOOLEAN") != std::string::npos);
    CHECK_EQ(read(files.mst), mstSentinel);
    CHECK_EQ(read(files.remesh), remeshSentinel); // Disabled QFile::remove denial is ignored.

    CHECK_EQ(result.code, M2M_E_WRITE);
    const std::string prefix = "cannot open MST ";
    CHECK(result.message.find(prefix) == 0);
    CHECK_EQ(result.message, prefix + qt::String(files.mst.u16string()).utf8());
    REQUIRE(details.returnedCode.has_value());
    CHECK_EQ(*details.returnedCode, M2M_E_WRITE);
    CHECK_EQ(terminal.profileReads, 0);
    CHECK_EQ(terminal.dispatches, 0);
    CHECK_EQ(terminal.failClosedCalls, 0);
    // These are adapter status and scripted/device observations, not the
    // original GUI Save's earlier check/status local or a fresh original run.
}

TEST_CASE(save_project_and_mst_failures_do_not_preempt_enabled_remesh_failure)
{
    Rig files;
    const std::string remeshSentinel = "DONE\n";
    const std::string fallbackSeed = "FILTER_DISTANCE 7\nDONE\n";
    const std::string mstSentinel = "existing MST retained by write-open denial\n";
    write(files.remesh, remeshSentinel);
    write(files.root / "LOC_DEN_CTRL.DAT", fallbackSeed);

    // Keep a read-compatible lock for the entire conversion. First prove the
    // same lock really denies a write-open, before relying on retained bytes.
    Handle denyRemeshWrite(files.remesh, FILE_SHARE_READ);
    HANDLE remeshProbe = CreateFileW(files.remesh.c_str(), GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
    const DWORD remeshProbeError = GetLastError();
    CHECK(remeshProbe == INVALID_HANDLE_VALUE);
    CHECK_EQ(remeshProbeError, static_cast<DWORD>(ERROR_SHARING_VIOLATION));
    if (remeshProbe != INVALID_HANDLE_VALUE) CloseHandle(remeshProbe);

    Observation observation;
    observation.mst = files.mst;
    std::unique_ptr<Handle> denyMstWrite;
    observation.aboutToCloseAction = [&] {
        // Conversion input Open and Save-current have completed; this output
        // device open begins the project writer stage. Establish MST denial
        // before the writer closes and the scripted post-close error.
        write(files.mst, mstSentinel);
        denyMstWrite = std::make_unique<Handle>(files.mst, FILE_SHARE_READ);
    };

    OutputFactory factory(observation);
    detail::ConversionCallServices services{&factory};
    ConversionDetails details;
    const Result result = detail::convertDetailedWithServices(
        files.project.u16string(), files.mst.u16string(), M2M_FLAG_WRITE_SAVE, 1252,
        details, services);
    denyMstWrite.reset();

    CHECK((observation.events == std::vector<std::string>{
        "project output open", "aboutToClose", "close returned", "error query"}));
    CHECK_EQ(observation.closeCount, 1);
    CHECK_EQ(observation.errorQueries, 1);
    REQUIRE(details.saveCall.has_value());
    CHECK(details.saveCall->projectWriteAttempted);
    CHECK(details.saveCall->projectWriteReturned);
    CHECK(!details.saveCall->projectWriteSucceeded);
    CHECK(details.saveCall->mstWriteAttempted);
    CHECK(details.saveCall->mstWriteReturned);
    CHECK(!details.saveCall->mstWriteSucceeded);

    CHECK(read(files.bwz).find("WZD_BOOLEAN_CUTTER_OBJECT_NO") != std::string::npos);
    CHECK(read(files.ini).find("WZD_MACHDIST_OPERATION_BOOLEAN") != std::string::npos);
    CHECK_EQ(read(files.mst), mstSentinel);
    CHECK_EQ(read(files.remesh), remeshSentinel);
    CHECK_EQ(read(files.root / "LOC_DEN_CTRL.DAT"), fallbackSeed);

    CHECK_EQ(result.code, M2M_E_UNSUPPORTED);
    CHECK_EQ(result.message,
        std::string("Save remesh reaches an unrecovered writer or invalid-parameter boundary"));
    REQUIRE(details.returnedCode.has_value());
    CHECK_EQ(*details.returnedCode, M2M_E_UNSUPPORTED);
}

int main()
{
    return testkit::run_all();
}
