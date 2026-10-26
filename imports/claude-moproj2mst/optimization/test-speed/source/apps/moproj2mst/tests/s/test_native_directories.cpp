#include "../support/test_paths.hpp"
#include <fstream>
#include <testkit/check.hpp>
#include "m2m/native_directories.hpp"
#include "m2m/qt4path.hpp"
#include "m2m/mst/writer.hpp"
#include "../../src/engine/engine_internal.hpp"

using namespace m2m;
namespace fs = std::filesystem;
namespace {
fs::path fresh(const char* name) {
    static unsigned serial = 0;
    const auto p = m2m_test::scratchOutput(m2m_test::scratchRoot() /
        (std::string(name) + "-" + std::to_string(GetCurrentProcessId()) + "-" +
         std::to_string(GetTickCount64()) + "-" + std::to_string(++serial)));
    REQUIRE(!fs::exists(p));
    return p;
}
String str(const fs::path& p) { return String(p.u16string()); }
}

TEST_CASE(native_mkpath_creates_components_and_accepts_existing_directories)
{
    const auto root = fresh("mkpath");
    const auto path = root / L"\x03b1" / "leaf";
    CHECK(qt::nativeMkpathAbsolute(str(path)));
    CHECK(fs::is_directory(path));
    CHECK(qt::nativeMkpathAbsolute(str(path)));
    std::ofstream(root / "file") << "retained";
    CHECK(!qt::nativeMkpathAbsolute(str(root / "file" / "child")));
    CHECK(fs::is_regular_file(root / "file"));
}

TEST_CASE(native_mkpath_retains_created_prefix_after_later_failure)
{
    const auto root = fresh("mkpath-partial");
    const auto prefix = root / "retained";
    CHECK(!qt::nativeMkpathAbsolute(str(prefix / std::string(300, 'x') / "tail")));
    CHECK(fs::is_directory(prefix));
}

TEST_CASE(native_save_directory_failures_do_not_skip_later_attempts)
{
    const auto root = fresh("mkpath-save");
    fs::create_directories(root / "Operations");
    std::ofstream(root / "Operations" / "Task00001") << "blocking-file";
    Project p; p.path = str(root / "project.moproj");
    Operation op; op.project = &p; op.taskDir = qt::L("Operations/Task00001");
    engine::FileEffects effects; effects.nativeDirectories = true;
    engine::makeFolders(p, op, effects);
    REQUIRE(effects.directoryAttempts.size() == 6);
    CHECK(!effects.directoryAttempts.front().succeeded);
    CHECK(effects.mkdirs.empty()); // failed call will not be retried by applyEffects
    const char* names[] = {"Operations", "Objects", "Materials", "Equipments", "Lubricants"};
    for (std::size_t i = 0; i < 5; ++i) {
        CHECK(effects.directoryAttempts[i + 1].succeeded);
        CHECK(effects.directoryAttempts[i + 1].path == qt::cleanPath(str(root / names[i])));
        CHECK(fs::is_directory(root / names[i]));
    }
}

TEST_CASE(recorded_directory_profile_remains_without_physical_effects)
{
    const auto root = fresh("mkpath-record");
    Project p; p.path = str(root / "project.moproj");
    Operation op; op.project = &p; op.taskDir = qt::L("Operations/Task00001");
    engine::FileEffects effects;
    engine::makeFolders(p, op, effects);
    CHECK_EQ(effects.mkdirs.size(), std::size_t(6));
    CHECK(effects.directoryAttempts.empty());
    CHECK(!fs::exists(root));
    CHECK(!qt::nativeMkpathAbsolute(qt::L("")));
}

int main() { return testkit::run_all(); }
