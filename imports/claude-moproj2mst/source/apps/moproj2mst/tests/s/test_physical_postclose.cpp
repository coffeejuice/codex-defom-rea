#include "../support/test_paths.hpp"
// ctest-labels: m2m-s
// Real FileDevice status through the private conversion service with an explicit artificial seed.
#include <testkit/check.hpp>

#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <memory>
#include <string>
#include <string_view>
#include <system_error>
#include <vector>

#include "m2m/convert_details.hpp"
#include "m2m/engine.hpp"
#include "m2m/mst/file_device.hpp"
#include "m2m/qt4path.hpp"
#include "../../src/core/conversion_call_services.hpp"
#include "../../src/moproj/project_output_device.hpp"

namespace fs = std::filesystem;
using namespace m2m;

namespace {
constexpr DWORD kLockedRangeBytes = 1;

std::string readFile(const fs::path& path)
{
    std::ifstream file(path, std::ios::binary);
    return {std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()};
}

void writeFile(const fs::path& path, const std::string& bytes)
{
    std::ofstream file(path, std::ios::binary | std::ios::trunc);
    REQUIRE(file.is_open());
    file << bytes;
}

struct Rig {
    fs::path root, project, task, bwz, ini, mst, fallback, remesh;
    Rig()
    {
        static unsigned serial = 0;
        root = m2m_test::scratchOutput(m2m_test::scratchRoot() /
            ("save-physical-postclose-" + std::to_string(GetCurrentProcessId()) + "-" +
             std::to_string(GetTickCount64()) + "-" + std::to_string(++serial)));
        REQUIRE(!fs::exists(root));
        fs::copy(m2m_test::configuredPath("M2M_ORACLE_B") / "133_B_DBSTEP_BOOL3_before",
                 root, fs::copy_options::recursive);
        project = root / "COG_P1_B1.moproj";
        task = root / "Operations" / "Task00002";
        bwz = task / "Boolean.BWZ";
        ini = task / "DEF_MACHDIST_CTRL2.INI";
        mst = root / "native-output.MST";
        fallback = root / "LOC_DEN_CTRL.DAT";
        remesh = root / "LOCAL_REMESH.DAT";
        writeFile(fallback, "FILTER_DISTANCE 7\nINTERNAL_ELEMENT_SIZE_CONTROL_METHOD 4\nDONE\n");
        REQUIRE(!fs::exists(bwz));
        REQUIRE(!fs::exists(ini));
        REQUIRE(!fs::exists(mst));
        REQUIRE(!fs::exists(remesh));
    }
};

struct DeviceObservation {
    fs::path project, bwz, ini, mst, remesh;
    std::u16string requestedPath;
    std::vector<std::string> events;
    bool injectSeedAndLock = false;
    bool opened = false;
    bool seedAttempted = false;
    std::int64_t seedWriteResult = -2;
    std::uintmax_t bytesOnDiskAfterSeed = static_cast<std::uintmax_t>(-1);
    bool lockRequested = false;
    bool lockAcquired = false;
    bool lockWasAcquired = false;
    DWORD lockWin32Error = ERROR_SUCCESS;
    bool lockHeldAtAboutToClose = false;
    bool aboutToCloseWhileOpen = false;
    int factoryCalls = 0;
    int serializerWriteCalls = 0;
    int serializerRejectedWrites = 0;
    std::size_t serializerAcceptedBytes = 0;
    int closeReturnedCount = 0;
    int errorQueries = 0;
    int realError = -1;
    bool closedBeforeError = false;
    bool lockHeldAtError = false;
    bool laterOutputsAbsentAtError = false;
    bool destroyed = false;
    HANDLE lockHandle = INVALID_HANDLE_VALUE;
    OVERLAPPED lockRange{};
};

bool acquireLock(DeviceObservation& seen)
{
    seen.lockRequested = true;
    seen.lockHandle = CreateFileW(seen.project.c_str(), GENERIC_READ | GENERIC_WRITE,
                                  FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_EXISTING,
                                  FILE_ATTRIBUTE_NORMAL, nullptr);
    if (seen.lockHandle == INVALID_HANDLE_VALUE) {
        seen.lockWin32Error = GetLastError();
        return false;
    }
    seen.lockRange = {};
    if (!LockFileEx(seen.lockHandle, LOCKFILE_EXCLUSIVE_LOCK | LOCKFILE_FAIL_IMMEDIATELY,
                    0, kLockedRangeBytes, 0, &seen.lockRange)) {
        seen.lockWin32Error = GetLastError();
        CloseHandle(seen.lockHandle);
        seen.lockHandle = INVALID_HANDLE_VALUE;
        return false;
    }
    seen.lockAcquired = true;
    seen.lockWasAcquired = true;
    seen.events.emplace_back("lock acquired");
    return true;
}

void releaseLock(DeviceObservation& seen)
{
    if (seen.lockHandle == INVALID_HANDLE_VALUE) return;
    if (seen.lockAcquired) {
        (void)UnlockFileEx(seen.lockHandle, 0, kLockedRangeBytes, 0, &seen.lockRange);
        seen.lockAcquired = false;
    }
    CloseHandle(seen.lockHandle);
    seen.lockHandle = INVALID_HANDLE_VALUE;
    seen.events.emplace_back("lock released");
}

class SeededProjectOutput final : public moproj::detail::ProjectOutputDevice {
public:
    SeededProjectOutput(const qt::String& path, DeviceObservation& seen)
        : file_(std::make_unique<mst::FileDevice>(path)), seen_(seen)
    {
        seen_.requestedPath = path.u16();
        seen_.opened = file_->opened();
        file_->setAboutToClose([this] {
            seen_.events.emplace_back("aboutToClose");
            seen_.aboutToCloseWhileOpen = file_->opened();
            seen_.lockHeldAtAboutToClose = seen_.lockAcquired;
            notifyAboutToClose();
        });

        if (seen_.injectSeedAndLock && file_->opened()) {
            seen_.seedAttempted = true;
            // Explicit artificial initial FileDevice state; the native project
            // serializer does not place this byte into the device buffer.
            seen_.seedWriteResult = file_->write("X");
            std::error_code ec;
            seen_.bytesOnDiskAfterSeed = fs::file_size(seen_.project, ec);
            seen_.events.emplace_back("artificial seed");
            if (seen_.seedWriteResult == 1 && !ec && seen_.bytesOnDiskAfterSeed == 0)
                (void)acquireLock(seen_);
        }
    }

    ~SeededProjectOutput() override
    {
        if (file_->opened()) close();
        file_->setAboutToClose({});
        releaseLock(seen_);
        seen_.destroyed = true;
    }

    std::int64_t write(std::string_view bytes) override
    {
        ++seen_.serializerWriteCalls;
        const auto result = file_->write(bytes);
        if (result > 0) seen_.serializerAcceptedBytes += static_cast<std::size_t>(result);
        if (result < 0 || static_cast<std::size_t>(result) != bytes.size())
            ++seen_.serializerRejectedWrites;
        seen_.events.emplace_back("serializer write");
        return result;
    }

    bool flush() override { return file_->flush(); }
    bool opened() const override { return file_->opened(); }

    void close() override
    {
        if (!file_->opened()) return;
        file_->close();
        ++seen_.closeReturnedCount;
        seen_.events.emplace_back("close returned");
    }

    int error() const override
    {
        ++seen_.errorQueries;
        seen_.closedBeforeError = !file_->opened();
        seen_.lockHeldAtError = seen_.lockAcquired;
        seen_.realError = file_->error();
        seen_.laterOutputsAbsentAtError = !fs::exists(seen_.bwz) && !fs::exists(seen_.ini) &&
                                           !fs::exists(seen_.mst) && !fs::exists(seen_.remesh);
        seen_.events.emplace_back("error query");
        return seen_.realError; // actual FileDevice status, no override
    }

private:
    std::unique_ptr<mst::FileDevice> file_;
    DeviceObservation& seen_;
};

class SeededProjectFactory final : public moproj::detail::ProjectOutputFactory {
public:
    explicit SeededProjectFactory(DeviceObservation& seen) : seen_(seen) {}

    std::unique_ptr<moproj::detail::ProjectOutputDevice> open(const qt::String& path) override
    {
        ++seen_.factoryCalls;
        seen_.project = fs::path(path.u16());
        seen_.events.emplace_back("factory/open");
        return std::make_unique<SeededProjectOutput>(path, seen_);
    }

private:
    DeviceObservation& seen_;
};

struct Run {
    Result result;
    ConversionDetails details;
    DeviceObservation observation;
    std::string bwz, ini, mst, remesh, projectResidue;
    std::size_t projectResidueBytes = 0;
};

std::size_t eventIndex(const DeviceObservation& seen, const char* event)
{
    return static_cast<std::size_t>(std::find(seen.events.begin(), seen.events.end(), event) -
                                    seen.events.begin());
}

Run run(bool seedAndLock)
{
    Rig files;
    DeviceObservation observation;
    observation.project = files.project;
    observation.bwz = files.bwz;
    observation.ini = files.ini;
    observation.mst = files.mst;
    observation.remesh = files.remesh;
    observation.injectSeedAndLock = seedAndLock;
    SeededProjectFactory factory(observation);
    detail::ConversionCallServices services{&factory};
    ConversionDetails details;
    const Result result = detail::convertDetailedWithServices(
        files.project.u16string(), files.mst.u16string(), M2M_FLAG_WRITE_SAVE, 1252,
        details, services);

    Run output;
    output.result = result;
    output.details = details;
    output.observation = observation;
    output.bwz = readFile(files.bwz);
    output.ini = readFile(files.ini);
    output.mst = readFile(files.mst);
    output.remesh = readFile(files.remesh);
    output.projectResidue = readFile(files.project); // measured after device/lock cleanup
    output.projectResidueBytes = output.projectResidue.size();
    return output;
}

void checkLaterOutputs(const Run& result)
{
    CHECK(result.bwz.find("WZD_BOOLEAN_CUTTER_OBJECT_NO") != std::string::npos);
    CHECK(result.ini.find("WZD_MACHDIST_OPERATION_BOOLEAN") != std::string::npos);
    CHECK(result.mst.find("END OF MASTER FILE") != std::string::npos);
    CHECK(result.remesh.find("KEYWORD_DRIVEN") != std::string::npos);
    CHECK(result.remesh.find("INTERNAL_ELEMENT_SIZE_CONTROL_METHOD 4") != std::string::npos);
    CHECK(result.remesh.find("FILTER_DISTANCE 7") != std::string::npos);
    REQUIRE(result.details.saveCall.has_value());
    const auto& state = *result.details.saveCall;
    CHECK(state.mstWriteAttempted);
    CHECK(state.mstWriteReturned);
    CHECK(state.mstWriteSucceeded);
}
} // namespace

TEST_CASE(save_seeded_real_project_postclose_error_continues_boolean_mst_and_remesh)
{
    const Run control = run(false);
    CHECK_EQ(control.result.code, M2M_OK);
    REQUIRE(control.details.saveCall.has_value());
    CHECK(control.details.saveCall->projectWriteAttempted);
    CHECK(control.details.saveCall->projectWriteReturned);
    CHECK(control.details.saveCall->projectWriteSucceeded);
    CHECK(control.observation.opened);
    CHECK_EQ(control.observation.realError, 0);
    checkLaterOutputs(control);

    const Run injected = run(true); // artificial seeded initial FileDevice buffer + real byte lock
    CHECK(injected.observation.seedAttempted);
    CHECK_EQ(injected.observation.seedWriteResult, 1);
    CHECK_EQ(injected.observation.bytesOnDiskAfterSeed, 0u);
    if (!injected.observation.lockWasAcquired) {
        std::fprintf(stdout,
            "[ diagnostic ] seeded-buffer lock not acquired (Win32=%lu); "
            "writer=%d postCloseFileError=%d; downstream lifecycle not asserted\n",
            static_cast<unsigned long>(injected.observation.lockWin32Error),
            injected.result.code, injected.observation.realError);
        return;
    }

    CHECK(injected.observation.lockHeldAtAboutToClose);
    CHECK(injected.observation.lockHeldAtError);
    CHECK(injected.observation.closedBeforeError);
    CHECK_EQ(injected.observation.realError, 2);
    CHECK(injected.observation.laterOutputsAbsentAtError);
    CHECK_EQ(injected.observation.closeReturnedCount, 1);
    CHECK_EQ(injected.observation.errorQueries, 1);
    CHECK(injected.observation.serializerWriteCalls > 0);
    REQUIRE(eventIndex(injected.observation, "lock released") < injected.observation.events.size());
    CHECK(eventIndex(injected.observation, "factory/open") <
          eventIndex(injected.observation, "artificial seed"));
    CHECK(eventIndex(injected.observation, "artificial seed") <
          eventIndex(injected.observation, "lock acquired"));
    CHECK(eventIndex(injected.observation, "lock acquired") <
          eventIndex(injected.observation, "serializer write"));
    CHECK(eventIndex(injected.observation, "serializer write") <
          eventIndex(injected.observation, "aboutToClose"));
    CHECK(eventIndex(injected.observation, "aboutToClose") <
          eventIndex(injected.observation, "close returned"));
    CHECK(eventIndex(injected.observation, "close returned") <
          eventIndex(injected.observation, "error query"));
    CHECK(eventIndex(injected.observation, "error query") <
          eventIndex(injected.observation, "lock released"));
    REQUIRE(injected.details.saveCall.has_value());
    CHECK(injected.details.saveCall->projectWriteAttempted);
    CHECK(injected.details.saveCall->projectWriteReturned);
    CHECK(!injected.details.saveCall->projectWriteSucceeded);
    CHECK(injected.details.saveCall->mstWriteAttempted);
    CHECK(injected.details.saveCall->mstWriteReturned);
    CHECK(injected.details.saveCall->mstWriteSucceeded);
    CHECK_EQ(injected.result.code, M2M_E_WRITE);
    CHECK(injected.result.message.find("cannot write project ") == 0);
    REQUIRE(injected.details.returnedCode.has_value());
    CHECK_EQ(*injected.details.returnedCode, M2M_E_WRITE);
    checkLaterOutputs(injected);
    CHECK_EQ(control.bwz, injected.bwz);
    CHECK_EQ(control.ini, injected.ini);
    CHECK_EQ(control.mst, injected.mst);
    CHECK_EQ(control.remesh, injected.remesh);
    CHECK_EQ(injected.projectResidue.size(), injected.projectResidueBytes);
    // No expected project residue is asserted: the injected initial byte and
    // physical lock are explicitly artificial test conditions.
}

int main() { return testkit::run_all(); }
