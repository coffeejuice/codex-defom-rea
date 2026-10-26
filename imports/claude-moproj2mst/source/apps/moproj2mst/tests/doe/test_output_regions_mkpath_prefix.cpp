#include "../support/test_paths.hpp"

#include <filesystem>
#include <fstream>
#include <string>

#include <testkit/check.hpp>

#include "m2m/common.hpp"
#include "../../src/doe/output_regions_mkpath_prefix.hpp"
#include "m2m/doe_study.hpp"
#include "m2m/engine.hpp"
#include "m2m/model.hpp"
#include "m2m/p17.hpp"
#include "m2m/qt4path.hpp"

using namespace m2m;
namespace fs = std::filesystem;
namespace {
fs::path fresh(const char* name)
{
    static unsigned serial = 0;
    auto path = m2m_test::scratchOutput(m2m_test::scratchRoot() /
        (std::string(name) + "-" + std::to_string(GetCurrentProcessId()) + "-" +
         std::to_string(GetTickCount64()) + "-" + std::to_string(++serial)));
    REQUIRE(!fs::exists(path));
    return path;
}
qt::String stringPath(const fs::path& path) { return qt::String(path.u16string()); }
struct Rig {
    Project project;
    SubProject* sub = nullptr;
    std::shared_ptr<doe::Study> study;
    explicit Rig(const fs::path& root)
    {
        initProject(project, stringPath(root), qt::L("doe-output"));
        sub = activeSubProject(project);
        REQUIRE(sub != nullptr);
        sub->type = 2;
        study = doe::makeStudy(*sub, nullptr);
        REQUIRE(study != nullptr);
        sub->ownedDoeStudy = study;
        sub->doe = study.get();
    }
    qt::String regionsPath() const
    {
        const auto control = mst::doeControlDir(*sub, &study->name);
        return qt::cleanPath(control + qt::kSep + qt::L("Regions"));
    }
};
template<class Work> std::string refused(const Work& work)
{
    try { work(); }
    catch (const Error& error) {
        CHECK_EQ(error.code, M2M_E_UNSUPPORTED);
        return error.what();
    }
    CHECK(false);
    return {};
}
}

TEST_CASE(doe_output_regions_mkdir_uses_actual_type2_study_owner_and_attempts_immediately)
{
    Rig rig(fresh("doe-output-regions-success"));
    engine::FileEffects effects;
    effects.nativeDirectories = true;

    const auto result = doe::attemptOutputRegionsMkdirPrefix(
        rig.project, *rig.sub, *rig.study, effects);

    CHECK(result.outcome == doe::OutputRegionsMkdirPrefixOutcome::AttemptSucceeded);
    CHECK(result.succeeded);
    CHECK(result.path == rig.regionsPath());
    REQUIRE(effects.directoryAttempts.size() == 1);
    CHECK(effects.directoryAttempts.front().path == rig.regionsPath());
    CHECK(effects.directoryAttempts.front().succeeded);
    CHECK(effects.mkdirs.empty() && effects.writes.empty() && effects.deletes.empty());
    CHECK(fs::is_directory(fs::path(result.path.u16())));
    CHECK(rig.study->name == qt::L("DoeStudy0"));
    CHECK(rig.sub->ownedDoeStudy == rig.study && rig.sub->doe == rig.study.get());
}

TEST_CASE(doe_output_regions_mkdir_failure_is_retained_at_warning_modal_boundary)
{
    const auto root = fresh("doe-output-regions-warning");
    Rig rig(root);
    const auto blocker = fs::path(rig.regionsPath().u16());
    fs::create_directories(blocker.parent_path());
    {
        std::ofstream file(blocker, std::ios::binary);
        REQUIRE(file.good());
        file << 'x';
    }
    engine::FileEffects effects;
    effects.nativeDirectories = true;

    const auto result = doe::attemptOutputRegionsMkdirPrefix(
        rig.project, *rig.sub, *rig.study, effects);

    CHECK(result.outcome == doe::OutputRegionsMkdirPrefixOutcome::WarningModalRequired);
    CHECK(!result.succeeded && result.path == rig.regionsPath());
    REQUIRE(effects.directoryAttempts.size() == 1);
    CHECK(effects.directoryAttempts.front().path == rig.regionsPath());
    CHECK(!effects.directoryAttempts.front().succeeded);
    CHECK(effects.mkdirs.empty() && effects.writes.empty() && effects.deletes.empty());
    CHECK(fs::is_regular_file(blocker));
    CHECK(fs::is_directory(blocker.parent_path())); // Earlier mkpath components remain.
    // The one-OK native warning modal and continuation are deliberately not run here.
}

TEST_CASE(doe_output_regions_mkdir_owner_and_profile_guards_precede_filesystem_effect)
{
    Rig rig(fresh("doe-output-regions-owner"));
    Rig foreign(fresh("doe-output-regions-foreign"));
    engine::FileEffects effects;
    effects.nativeDirectories = true;
    const auto sentinel = qt::L("C:/existing/planned-path");
    effects.mkdirs.push_back(sentinel);

    CHECK(refused([&] {
        (void)doe::attemptOutputRegionsMkdirPrefix(rig.project, *foreign.sub, *foreign.study, effects);
    }).find("actual active Type2 Project/SubProject/Study owner is unavailable") != std::string::npos);
    CHECK(effects.directoryAttempts.empty() && effects.mkdirs.size() == 1 && effects.mkdirs.front() == sentinel);

    rig.sub->type = 3;
    CHECK(refused([&] {
        (void)doe::attemptOutputRegionsMkdirPrefix(rig.project, *rig.sub, *rig.study, effects);
    }).find("actual active Type2 Project/SubProject/Study owner is unavailable") != std::string::npos);
    CHECK(effects.directoryAttempts.empty() && effects.mkdirs.size() == 1 && effects.mkdirs.front() == sentinel);

    rig.sub->type = 2;
    effects.nativeDirectories = false;
    CHECK(refused([&] {
        (void)doe::attemptOutputRegionsMkdirPrefix(rig.project, *rig.sub, *rig.study, effects);
    }).find("immediate native directory profile is unavailable") != std::string::npos);
    CHECK(effects.directoryAttempts.empty() && effects.mkdirs.size() == 1 && effects.mkdirs.front() == sentinel);
}

TEST_CASE(doe_output_regions_rejects_relative_control_path_before_directory_attempt)
{
    Rig rig(fresh("doe-output-regions-relative"));
    // projectDir(empty) stays empty, so doeControlDir produces a relative
    // DoeStudies/.../Control path and the candidate adapter must refuse it.
    rig.project.path = qt::String();
    engine::FileEffects effects;
    effects.nativeDirectories = true;

    CHECK(refused([&] {
        (void)doe::attemptOutputRegionsMkdirPrefix(rig.project, *rig.sub, *rig.study, effects);
    }).find("absolute Control/Regions path required") != std::string::npos);
    CHECK(effects.directoryAttempts.empty());
    CHECK(effects.mkdirs.empty() && effects.writes.empty() && effects.deletes.empty());
}

TEST_CASE(doe_output_regions_rejects_empty_or_mismatched_study_backlinks_before_attempt)
{
    Rig rig(fresh("doe-output-regions-backlink"));
    engine::FileEffects effects;
    effects.nativeDirectories = true;

    rig.study->sub = nullptr;
    CHECK(refused([&] {
        (void)doe::attemptOutputRegionsMkdirPrefix(rig.project, *rig.sub, *rig.study, effects);
    }).find("actual active Type2 Project/SubProject/Study owner is unavailable") != std::string::npos);
    CHECK(effects.directoryAttempts.empty());
    rig.study->sub = rig.sub;

    rig.sub->doe = nullptr;
    CHECK(refused([&] {
        (void)doe::attemptOutputRegionsMkdirPrefix(rig.project, *rig.sub, *rig.study, effects);
    }).find("actual active Type2 Project/SubProject/Study owner is unavailable") != std::string::npos);
    CHECK(effects.directoryAttempts.empty());
    rig.sub->doe = rig.study.get();

    rig.study->name = qt::String();
    CHECK(refused([&] {
        (void)doe::attemptOutputRegionsMkdirPrefix(rig.project, *rig.sub, *rig.study, effects);
    }).find("actual active Type2 Project/SubProject/Study owner is unavailable") != std::string::npos);
    CHECK(effects.directoryAttempts.empty());
    CHECK(effects.mkdirs.empty() && effects.writes.empty() && effects.deletes.empty());
}
int main() { return testkit::run_all(); }
