// ctest-labels: m2m-mdt
#include "../support/test_paths.hpp"
#include <testkit/check.hpp>
#include <cstdlib>
#include <filesystem>
#include <string>

#include "m2m/engine.hpp"
#include "m2m/model.hpp"
#include "m2m/mst/writer.hpp"
#include "m2m/qt4path.hpp"

using namespace m2m;
namespace fs = std::filesystem;

TEST_CASE(root_only_shared_ppv_save_preserves_native_local_empty_gate)
{
    const char* configured = std::getenv("M2M_ORACLE_B");
    const fs::path oracle = configured ? fs::path(configured) : m2m_test::configuredPath("M2M_ORACLE_B");
    const fs::path input = oracle / "93_B_MB3ACT_before" / "Gear_blank.moproj";
    REQUIRE(fs::is_regular_file(input));

    const qt::String file(input.u16string());
    const qt::String db = app::changeSuffix(file, qt::L(".DB"));
    Project project;
    ProjectScope scope(project);
    Ctx context;
    initProject(project, qt::absolutePath(db), qt::completeBaseName(db));
    makeRegistry(project);
    REQUIRE(openProject(project, file, context) && project.openedOk);
    activateSubProject(project, 0, context);

    engine::FileEffects opened;
    Operation* operation = engine::openActiveOperation(project, context, opened);
    REQUIRE(operation != nullptr);
    REQUIRE(operation->recoveryB != nullptr);
    REQUIRE(project.recoveryA != nullptr);
    operation->recoveryB->clear();
    project.recoveryA->push_back({4, 19.0, 5, 1.25, 2.5});
    REQUIRE(operation->recoveryB->empty());
    REQUIRE(!project.recoveryA->empty());

    engine::FileEffects saved;
    int errorCode = 0;
    std::string errorText;
    bool completed = false;
    try {
        engine::saveCurrentOperation(project, *operation, context, saved);
        completed = true;
    } catch (const Error& error) {
        errorCode = error.code;
        errorText = error.what();
    }

    // Native 0x140d4afe0 gates on the local list before QFile open. With the
    // local list empty, the PPV helper returns false before touching the file;
    // 0x1400597d0 ignores that result and returns the KEY writer result.
    CHECK(completed);
    CHECK_EQ(errorCode, 0);
    CHECK(errorText.empty());
    bool keyWritten = false;
    bool ppvWritten = false;
    for (const auto& write : saved.writes) {
        const std::string path = write.path.utf8();
        keyWritten = keyWritten || path.find("SimCtrl.KEY") != std::string::npos;
        ppvWritten = ppvWritten || path.find("SimCtrl.PPV") != std::string::npos;
    }
    CHECK(keyWritten);
    CHECK(!ppvWritten);
}

int main() { return testkit::run_all(); }
