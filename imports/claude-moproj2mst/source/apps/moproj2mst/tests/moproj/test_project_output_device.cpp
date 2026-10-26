#include "../support/test_paths.hpp"
// ctest-labels: m2m-moproj
// A declared post-close status exercises the writer's Boolean rule. Real
// FileDevice owns flushing/close; this is not a physical flush-fault oracle.
#include <testkit/check.hpp>

#include <algorithm>
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

std::string readFile(const fs::path& path)
{
    std::ifstream file(path, std::ios::binary);
    return {std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()};
}

struct Observations {
    std::vector<std::string> events;
    std::u16string path;
    unsigned factoryCalls = 0, closeCallbacks = 0, errorQueries = 0;
    std::size_t acceptedBytes = 0;
    bool opened = false, callbackWhileOpen = false;
    bool closedBeforeError = false, destroyed = false;
    int realError = -1, reportedError = -1;
};

class DelegatingOutput final : public moproj::detail::ProjectOutputDevice {
public:
    DelegatingOutput(const qt::String& path, Observations& seen, int reportedError)
        : file_(path), seen_(seen), reportedError_(reportedError)
    {
        seen_.opened = file_.opened();
        file_.setAboutToClose([this] {
            seen_.events.push_back("aboutToClose");
            ++seen_.closeCallbacks;
            seen_.callbackWhileOpen = file_.opened();
            notifyAboutToClose(); // forward only when real FileDevice emits it
        });
    }
    ~DelegatingOutput() override
    {
        file_.close();
        file_.setAboutToClose({});
        seen_.destroyed = true;
    }
    std::int64_t write(std::string_view bytes) override
    {
        seen_.events.push_back("write");
        const auto accepted = file_.write(bytes);
        if (accepted > 0) seen_.acceptedBytes += static_cast<std::size_t>(accepted);
        return accepted;
    }
    bool flush() override { return file_.flush(); }
    bool opened() const override { return file_.opened(); }
    void close() override
    {
        if (!file_.opened()) return;
        seen_.events.push_back("close.begin");
        file_.close(); // retains saved-initial-flush / callback / handle-close order
        seen_.events.push_back("close.end");
    }
    int error() const override
    {
        seen_.events.push_back("error");
        ++seen_.errorQueries;
        seen_.closedBeforeError = !file_.opened();
        seen_.realError = file_.error();
        // Negative means real status control. Seven is only a declared status,
        // with no claim that the delegated physical device actually failed.
        seen_.reportedError = reportedError_ < 0 ? seen_.realError : reportedError_;
        return seen_.reportedError;
    }
private:
    mst::FileDevice file_;
    Observations& seen_;
    int reportedError_;
};

class Factory final : public moproj::detail::ProjectOutputFactory {
public:
    Factory(Observations& seen, int reportedError) : seen_(seen), reportedError_(reportedError) {}
    std::unique_ptr<moproj::detail::ProjectOutputDevice> open(const qt::String& path) override
    {
        ++seen_.factoryCalls;
        seen_.path = path.u16();
        seen_.events.push_back("open");
        return std::make_unique<DelegatingOutput>(path, seen_, reportedError_);
    }
private:
    Observations& seen_;
    int reportedError_;
};

std::size_t eventIndex(const Observations& seen, const char* name)
{
    return static_cast<std::size_t>(std::find(seen.events.begin(), seen.events.end(), name) -
                                    seen.events.begin());
}

void checkWriter(int declaredError)
{
    static unsigned serial = 0;
    const auto root = m2m_test::scratchOutput(m2m_test::scratchRoot() /
        ("project-device-" + std::to_string(GetCurrentProcessId()) + "-" +
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
    Observations seen;
    Factory factory(seen, declaredError);
    engine::FileEffects observation;
    const bool result = moproj::detail::saveProjectFileNativeWithFactory(
        project, false, *views, {}, nullptr, &observation, factory);

    CHECK(result == (declaredError < 0));
    CHECK_EQ(seen.factoryCalls, 1u);
    CHECK(seen.path == project.path.u16());
    CHECK(seen.path == qt::cleanPath(qt::String(path.u16string())).u16());
    CHECK(seen.opened);
    CHECK(seen.acceptedBytes > 0);
    CHECK_EQ(seen.closeCallbacks, 1u);
    CHECK(seen.callbackWhileOpen);
    CHECK(seen.closedBeforeError);
    CHECK_EQ(seen.errorQueries, 1u);
    CHECK_EQ(seen.realError, 0);
    CHECK_EQ(seen.reportedError, declaredError < 0 ? 0 : declaredError);
    CHECK(seen.destroyed); // observation storage survives the per-call device
    REQUIRE(eventIndex(seen, "error") < seen.events.size());
    CHECK(eventIndex(seen, "open") < eventIndex(seen, "close.begin"));
    CHECK(eventIndex(seen, "close.begin") < eventIndex(seen, "aboutToClose"));
    CHECK(eventIndex(seen, "aboutToClose") < eventIndex(seen, "close.end"));
    CHECK(eventIndex(seen, "close.end") < eventIndex(seen, "error"));
    // Both runs really serialize/close. The injected Boolean must not be
    // mistaken for a prediction of partial file bytes from a physical fault.
    REQUIRE(project.dom);
    const auto bytes = readFile(path);
    CHECK(!bytes.empty());
    CHECK_EQ(bytes, qt::String(xml::toString(*project.dom, 1)).utf8());
    CHECK(observation.writes.empty()); // native output was not recorded/replayed
}

} // namespace

TEST_CASE(project_output_declared_postclose_error_controls_writer_result)
{
    checkWriter(-1); // unmodified real post-close error control
    checkWriter(7);  // declared nonzero post-close error, ordinary false return
}

int main() { return testkit::run_all(); }
