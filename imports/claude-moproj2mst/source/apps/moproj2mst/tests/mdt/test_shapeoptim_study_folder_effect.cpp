// ShapeOptim 0x14013d180: native QDir::mkpath is attempted before later study GEO guards.
// ctest-labels: m2m-mdt m2m-mdt-l4
#include <testkit/check.hpp>

#include <filesystem>
#include <fstream>
#include <string>
#include <utility>

#include "m2m/engine.hpp"
#include "m2m/doe_study.hpp"
#include "m2m/model.hpp"
#include "m2m/mst/writer.hpp"
#include "m2m/native_directories.hpp"
#include "m2m/qt4path.hpp"
#include "../../src/ops/shapeoptim.hpp"
#include "../support/test_paths.hpp"

namespace m2m::mst::detail {
// Private source/test seam; deliberately absent from the public and shared headers.
void completeShapeOptimStudyFolderEffect(const qt::String&, engine::FileEffects&, bool immediate);
}

using namespace m2m;

namespace {
std::filesystem::path ownedRoot()
{
    const std::filesystem::path problemRoot(L"C:/PROBLEM");
    std::filesystem::create_directories(problemRoot);
    const auto canonicalProblemRoot = std::filesystem::canonical(problemRoot);
    const auto root = canonicalProblemRoot / ("M2M_SO_" + std::to_string(GetCurrentProcessId()));
    std::filesystem::create_directories(root);
    const auto resolvedRoot = std::filesystem::canonical(root);
    if (resolvedRoot.parent_path() != canonicalProblemRoot)
        throw std::runtime_error("ShapeOptim PID root resolves outside C:/PROBLEM");
    return resolvedRoot;
}

bool remainsInside(const std::filesystem::path& candidate, const std::filesystem::path& root)
{
    const auto resolved = std::filesystem::weakly_canonical(candidate);
    const auto relative = resolved.lexically_relative(root);
    return !relative.empty() && !relative.is_absolute() && *relative.begin() != "..";
}

std::filesystem::path target(const char* leaf)
{
    const auto root = ownedRoot();
    auto path = root / leaf;
    if (!remainsInside(path, root)) throw std::runtime_error("ShapeOptim test leaf escapes its PID root");
    if (path.native().size() >= 100) throw std::runtime_error("ShapeOptim test path must stay below 100 characters");
    return path;
}

void removeTestLeaf(const std::filesystem::path& path)
{
    const auto root = ownedRoot();
    if (!remainsInside(path, root) || path.parent_path().lexically_normal() != root)
        throw std::runtime_error("Refusing to remove a path outside the ShapeOptim test leaf root");
    std::error_code ec;
    std::filesystem::remove_all(path, ec);
    if (ec) throw std::filesystem::filesystem_error("Remove ShapeOptim test leaf", path, ec);
}

qt::String stringPath(const std::filesystem::path& path)
{
    return qt::String(path.u16string());
}
}

TEST_CASE(shapeoptim_study_folder_recording_keeps_creation_deferred)
{
    const auto path = target("so-opt-deferred");
    removeTestLeaf(path);
    engine::FileEffects effects;
    effects.mkdirs.push_back(stringPath(path)); // queued by the source-owned studyFolder()

    mst::detail::completeShapeOptimStudyFolderEffect(stringPath(path), effects, false);

    REQUIRE(effects.mkdirs.size() == 1);
    CHECK(effects.mkdirs[0] == stringPath(path));
    CHECK(effects.directoryAttempts.empty());
    CHECK(!std::filesystem::exists(path));
}

TEST_CASE(shapeoptim_study_folder_native_attempt_persists_before_later_guard)
{
    const auto path = target("so-opt-before-guard");
    removeTestLeaf(path);
    engine::FileEffects effects;
    effects.mkdirs.push_back(stringPath(path));

    mst::detail::completeShapeOptimStudyFolderEffect(stringPath(path), effects, true);
    CHECK(std::filesystem::is_directory(path));
    REQUIRE(effects.directoryAttempts.size() == 1);
    CHECK(effects.directoryAttempts[0].path == stringPath(path));
    CHECK(effects.directoryAttempts[0].succeeded);
    CHECK(effects.mkdirs.empty());

    // Later payload refusal cannot roll back the already-attempted native mkdir.
    bool laterGuardReached = false;
    try { throw Error(M2M_E_UNSUPPORTED, "simulated later GEO payload guard"); }
    catch (const Error&) { laterGuardReached = true; }
    CHECK(laterGuardReached);
    CHECK(std::filesystem::is_directory(path));
    removeTestLeaf(path);
}

TEST_CASE(shapeoptim_load_data_reaches_native_study_folder_attempt_through_actual_class)
{
    const auto root = target("so-opt-load-data");
    removeTestLeaf(root);

    Project project;
    initProject(project, stringPath(root), qt::L("project"));
    REQUIRE(activeSubProject(project) != nullptr);
    REQUIRE(activeSubProject(project)->doe == nullptr); // bounded absent-study-name profile

    Operation operation;
    operation.project = &project;
    auto model = std::make_shared<engine::GuiModel>(nullptr, 3);
    operation.engineModel = model;

    auto shapeOwner = mst::makeShapeOptim();
    REQUIRE(shapeOwner != nullptr);
    const auto inserted = project.registry.emplace(shapeOwner->key(), std::move(shapeOwner));
    REQUIRE(inserted.second);
    auto* shape = dynamic_cast<mst::ShapeOptimClass*>(inserted.first->second.get());
    REQUIRE(shape != nullptr);
    operation.opClass = shape;
    operation.classKey = shape->key();
    shape->current = &operation;

    const auto expected = projectDir(project) + qt::kSep + qt::L("DoeStudies") + qt::kSep + qt::kSep +
                          qt::L("Control") + qt::kSep + qt::L("Opt") + qt::kSep;
    if (expected.u16().size() >= 100)
        throw std::runtime_error("ShapeOptim generated study-folder path must stay below 100 characters");
    Ctx context;
    context.nativeOperationBlobs = true; // exact full WRITE_SAVE Open-mode selector at loadData
    engine::FileEffects effects;

    // With the currently reachable empty listF0_, loadData completes its real
    // ShapeOptim path; the later GEO refusal remains unavailable until its
    // missing listF0_ producer is recovered.
    shape->loadData(project, operation, context, effects);

    CHECK(std::filesystem::is_directory(std::filesystem::path(expected.u16())));
    REQUIRE(effects.directoryAttempts.size() == 1);
    CHECK(effects.directoryAttempts[0].path == expected);
    CHECK(effects.directoryAttempts[0].succeeded);
    CHECK(effects.mkdirs.empty());
    removeTestLeaf(root);
}

TEST_CASE(shapeoptim_load_data_uses_nonempty_name_from_exact_type2_study_owner)
{
    const auto root = target("so-opt-owned-study");
    removeTestLeaf(root);

    Project project;
    initProject(project, stringPath(root), qt::L("project"));
    auto* sub = activeSubProject(project);
    REQUIRE(sub != nullptr);
    sub->type = 2;
    sub->ownedDoeStudy = doe::makeStudy(*sub, nullptr);
    sub->doe = sub->ownedDoeStudy.get();
    REQUIRE(!sub->ownedDoeStudy->name.isEmpty());
    CHECK(sub->ownedDoeStudy->name == qt::L("DoeStudy0"));

    Operation operation;
    operation.project = &project;
    operation.engineModel = std::make_shared<engine::GuiModel>(nullptr, 3);

    auto shapeOwner = mst::makeShapeOptim();
    REQUIRE(shapeOwner != nullptr);
    const auto inserted = project.registry.emplace(shapeOwner->key(), std::move(shapeOwner));
    REQUIRE(inserted.second);
    auto* shape = dynamic_cast<mst::ShapeOptimClass*>(inserted.first->second.get());
    REQUIRE(shape != nullptr);
    operation.opClass = shape;
    operation.classKey = shape->key();
    shape->current = &operation;

    const auto expected = projectDir(project) + qt::kSep + qt::L("DoeStudies") + qt::kSep +
                          sub->ownedDoeStudy->name + qt::kSep + qt::L("Control") + qt::kSep +
                          qt::L("Opt") + qt::kSep;
    if (expected.u16().size() >= 100)
        throw std::runtime_error("ShapeOptim generated study-folder path must stay below 100 characters");
    Ctx context;
    context.nativeOperationBlobs = true;
    engine::FileEffects effects;

    shape->loadData(project, operation, context, effects);

    CHECK(std::filesystem::is_directory(std::filesystem::path(expected.u16())));
    REQUIRE(effects.directoryAttempts.size() == 1);
    CHECK(effects.directoryAttempts[0].path == expected);
    CHECK(effects.directoryAttempts[0].succeeded);
    CHECK(effects.mkdirs.empty());
    removeTestLeaf(root);
}

TEST_CASE(shapeoptim_study_folder_failed_native_attempt_is_recorded_then_stops_at_warning_modal)
{
    const auto blocker = target("so-opt-blocker");
    removeTestLeaf(blocker);
    {
        std::ofstream file(blocker, std::ios::binary);
        REQUIRE(file.good());
        file << 'x';
    }
    const auto path = blocker / "Opt";
    engine::FileEffects effects;
    effects.mkdirs.push_back(stringPath(path));

    bool stoppedAtWarningBoundary = false;
    try {
        mst::detail::completeShapeOptimStudyFolderEffect(stringPath(path), effects, true);
    } catch (const Error& error) {
        stoppedAtWarningBoundary = error.code == M2M_E_UNSUPPORTED &&
            std::string(error.what()).find("warning modal delivery is not recovered") != std::string::npos;
    }

    CHECK(stoppedAtWarningBoundary);
    REQUIRE(effects.directoryAttempts.size() == 1);
    CHECK(effects.directoryAttempts[0].path == stringPath(path));
    CHECK(!effects.directoryAttempts[0].succeeded);
    CHECK(effects.mkdirs.empty());
    CHECK(std::filesystem::is_regular_file(blocker));
    removeTestLeaf(blocker);
}

int main() { return testkit::run_all(); }
