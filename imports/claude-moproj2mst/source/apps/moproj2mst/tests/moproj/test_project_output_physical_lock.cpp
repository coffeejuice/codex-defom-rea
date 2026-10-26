#include "../support/test_paths.hpp"
// ctest-labels: m2m-moproj
// Artificially seeded FileDevice-buffer calibration through the private project-output seam.
#include <testkit/check.hpp>

#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <memory>
#include <string>
#include <vector>

#include "m2m/engine.hpp"
#include "m2m/mst/file_device.hpp"
#include "m2m/qt4path.hpp"
#include "../../src/moproj/project_output_device.hpp"

namespace fs = std::filesystem;
using namespace m2m;

namespace {
constexpr DWORD kLockBytes = 1;

std::string readFile(const fs::path& path)
{
    std::ifstream file(path, std::ios::binary);
    return {std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()};
}

struct Observations {
    std::vector<std::string> events;
    std::u16string path;
    std::size_t acceptedBytes = 0;
    std::size_t serializerWriteCalls = 0;
    std::size_t serializerRejectedWrites = 0;
    std::int64_t seededWriteResult = -2;
    std::uintmax_t bytesOnDiskAfterSeed = static_cast<std::uintmax_t>(-1);
    bool opened = false;
    bool lockRequested = false;
    bool lockAcquired = false;
    bool lockWasAcquired = false;
    bool lockHeldAtCloseCallback = false;
    bool callbackWhileOpen = false;
    unsigned closeCallbacks = 0;
    bool closedBeforeError = false;
    bool lockHeldAtError = false;
    bool destroyed = false;
    bool residueCaptured = false;
    bool residueRegular = false;
    std::size_t residueBytes = 0;
    int realError = -1;
    DWORD lockWin32Error = ERROR_SUCCESS;
    HANDLE lockHandle = INVALID_HANDLE_VALUE;
    OVERLAPPED lockRange{};
};

bool acquireOutputRangeLock(Observations& seen)
{
    seen.lockRequested = true;
    seen.lockHandle = CreateFileW(fs::path(seen.path).c_str(), GENERIC_READ | GENERIC_WRITE,
                                  FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_EXISTING,
                                  FILE_ATTRIBUTE_NORMAL, nullptr);
    if (seen.lockHandle == INVALID_HANDLE_VALUE) {
        seen.lockWin32Error = GetLastError();
        return false;
    }
    seen.lockRange = {};
    if (!LockFileEx(seen.lockHandle, LOCKFILE_EXCLUSIVE_LOCK | LOCKFILE_FAIL_IMMEDIATELY,
                    0, kLockBytes, 0, &seen.lockRange)) {
        seen.lockWin32Error = GetLastError();
        CloseHandle(seen.lockHandle);
        seen.lockHandle = INVALID_HANDLE_VALUE;
        return false;
    }
    seen.lockAcquired = true;
    seen.lockWasAcquired = true;
    seen.events.push_back("range-lock.acquired");
    return true;
}

void releaseOutputRangeLock(Observations& seen)
{
    if (seen.lockHandle == INVALID_HANDLE_VALUE) return;
    if (seen.lockAcquired) {
        (void)UnlockFileEx(seen.lockHandle, 0, kLockBytes, 0, &seen.lockRange);
        seen.lockAcquired = false;
    }
    CloseHandle(seen.lockHandle);
    seen.lockHandle = INVALID_HANDLE_VALUE;
    seen.events.push_back("range-lock.released");
}

class SeededBufferOutput final : public moproj::detail::ProjectOutputDevice {
public:
    SeededBufferOutput(const qt::String& path, Observations& seen)
        : path_(path.u16()), file_(path), seen_(seen)
    {
        seen_.path = path_;
        seen_.opened = file_.opened();
        file_.setAboutToClose([this] {
            seen_.events.push_back("aboutToClose");
            ++seen_.closeCallbacks;
            seen_.lockHeldAtCloseCallback = seen_.lockAcquired;
            seen_.callbackWhileOpen = file_.opened();
            notifyAboutToClose();
        });

        // Artificial initial device state for calibrating the saved initial
        // QFile flush result. The ordinary serializer does not seed this byte.
        seen_.seededWriteResult = file_.write("X");
        std::error_code ec;
        seen_.bytesOnDiskAfterSeed = fs::file_size(fs::path(path_), ec);
        seen_.events.push_back("artificial-buffer-seeded");
        if (seen_.seededWriteResult == 1 && !ec && seen_.bytesOnDiskAfterSeed == 0)
            (void)acquireOutputRangeLock(seen_);
    }

    ~SeededBufferOutput() override
    {
        file_.close();
        file_.setAboutToClose({});
        releaseOutputRangeLock(seen_);
        seen_.destroyed = true;
    }

    std::int64_t write(std::string_view bytes) override
    {
        seen_.events.push_back("write");
        ++seen_.serializerWriteCalls;
        const auto accepted = file_.write(bytes);
        if (accepted > 0) seen_.acceptedBytes += static_cast<std::size_t>(accepted);
        if (accepted < 0 || static_cast<std::size_t>(accepted) != bytes.size())
            ++seen_.serializerRejectedWrites;
        return accepted;
    }

    bool flush() override { return file_.flush(); }
    bool opened() const override { return file_.opened(); }

    void close() override
    {
        if (!file_.opened()) return;
        seen_.events.push_back("close.begin");
        file_.close();
        seen_.events.push_back("close.end");
    }

    int error() const override
    {
        seen_.events.push_back("error.query");
        seen_.closedBeforeError = !file_.opened();
        seen_.lockHeldAtError = seen_.lockAcquired;
        seen_.realError = file_.error();
        return seen_.realError; // no scripted status override
    }

private:
    std::u16string path_;
    mst::FileDevice file_;
    Observations& seen_;
};

class Factory final : public moproj::detail::ProjectOutputFactory {
public:
    explicit Factory(Observations& seen) : seen_(seen) {}

    std::unique_ptr<moproj::detail::ProjectOutputDevice> open(const qt::String& path) override
    {
        ++factoryCalls;
        seen_.events.push_back("factory.open");
        return std::make_unique<SeededBufferOutput>(path, seen_);
    }

    unsigned factoryCalls = 0;

private:
    Observations& seen_;
};

std::size_t eventIndex(const Observations& seen, const char* event)
{
    return static_cast<std::size_t>(std::find(seen.events.begin(), seen.events.end(), event) -
                                    seen.events.begin());
}

struct Run {
    bool result = false;
    Observations seen;
    std::string residue;
};

Run runWriter()
{
    static unsigned serial = 0;
    const auto root = m2m_test::scratchOutput(m2m_test::scratchRoot() /
        ("project-physical-lock-" + std::to_string(GetCurrentProcessId()) + "-" +
         std::to_string(GetTickCount64()) + "-" + std::to_string(++serial)));
    REQUIRE(!fs::exists(root));
    fs::copy(m2m_test::configuredPath("M2M_ORACLE_B") / "133_B_DBSTEP_BOOL3_before",
             root, fs::copy_options::recursive);
    const auto path = root / "COG_P1_B1.moproj";

    Project project;
    ProjectScope scope(project);
    Ctx context;
    initProject(project, qt::String(root.u16string()), qt::L("COG_P1_B1"));
    makeRegistry(project);
    REQUIRE(openProject(project, qt::String(path.u16string()), context));
    REQUIRE(project.openedOk);
    activateSubProject(project, 0, context);
    const auto views = moproj::guiViews(project);

    Run run;
    Factory factory(run.seen);
    engine::FileEffects observation;
    run.result = moproj::detail::saveProjectFileNativeWithFactory(
        project, false, *views, {}, nullptr, &observation, factory);
    CHECK(observation.writes.empty());
    REQUIRE(factory.factoryCalls == 1);
    run.residue = readFile(path); // measured after writer close and lock release
    run.seen.residueCaptured = true;
    run.seen.residueRegular = fs::is_regular_file(path);
    run.seen.residueBytes = run.residue.size();
    return run;
}

void checkCommonLifecycle(const Run& run)
{
    CHECK(run.seen.opened);
    CHECK_EQ(run.seen.closeCallbacks, 1u);
    CHECK(run.seen.callbackWhileOpen);
    CHECK(run.seen.closedBeforeError);
    CHECK(run.seen.destroyed);
    CHECK(run.seen.residueCaptured);
    CHECK(run.seen.residueRegular);
    CHECK_EQ(run.seen.residueBytes, run.residue.size());
    REQUIRE(eventIndex(run.seen, "error.query") < run.seen.events.size());
    CHECK(eventIndex(run.seen, "factory.open") < eventIndex(run.seen, "close.begin"));
    CHECK(eventIndex(run.seen, "close.begin") < eventIndex(run.seen, "aboutToClose"));
    CHECK(eventIndex(run.seen, "aboutToClose") < eventIndex(run.seen, "close.end"));
    CHECK(eventIndex(run.seen, "close.end") < eventIndex(run.seen, "error.query"));
}
} // namespace

TEST_CASE(project_output_seeded_buffer_lock_preserves_physical_postclose_file_error)
{
    const Run locked = runWriter();
    checkCommonLifecycle(locked);
    CHECK(locked.seen.lockRequested);
    CHECK_EQ(locked.seen.seededWriteResult, 1);
    CHECK_EQ(locked.seen.bytesOnDiskAfterSeed, 0u);
    if (!locked.seen.lockWasAcquired) {
        std::fprintf(stdout,
            "[ diagnostic ] seeded-buffer byte lock not acquired (Win32=%lu); "
            "writer=%d postCloseFileError=%d; no physical-error assertion made\n",
            static_cast<unsigned long>(locked.seen.lockWin32Error),
            locked.result ? 1 : 0, locked.seen.realError);
        return;
    }
    CHECK(eventIndex(locked.seen, "artificial-buffer-seeded") <
          eventIndex(locked.seen, "range-lock.acquired"));
    CHECK(eventIndex(locked.seen, "range-lock.acquired") < eventIndex(locked.seen, "write"));
    CHECK(locked.seen.lockHeldAtCloseCallback);
    CHECK(locked.seen.lockHeldAtError);
    CHECK_EQ(locked.seen.realError, 2);
    CHECK(!locked.result);
    CHECK(locked.seen.serializerWriteCalls > 0);
    // Output-device rejection (if any) is recorded separately from QFile error.
    // The ProjectOutputDevice seam does not expose QTextStream's sticky status.
}

int main() { return testkit::run_all(); }
