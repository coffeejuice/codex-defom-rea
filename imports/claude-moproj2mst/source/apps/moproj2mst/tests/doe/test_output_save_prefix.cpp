// ctest-labels: m2m-doe
#include "../support/test_paths.hpp"

#include <filesystem>
#include <fstream>
#include <string>

#include <testkit/check.hpp>

#include "m2m/common.hpp"
#include "m2m/doe_study.hpp"
#include "m2m/engine.hpp"
#include "m2m/model.hpp"
#include "m2m/moproj_write.hpp"
#include "m2m/mst/writer.hpp"
#include "m2m/p17.hpp"
#include "m2m/qt4path.hpp"

using namespace m2m;
namespace fs = std::filesystem;
namespace {
struct Scratch {
    fs::path path;
    Scratch()
    {
        static unsigned serial = 0;
        path = m2m_test::scratchOutput(m2m_test::scratchRoot() /
            ("doe-output-save-" + std::to_string(GetCurrentProcessId()) + "-" +
             std::to_string(GetTickCount64()) + "-" + std::to_string(++serial)));
        REQUIRE(!fs::exists(path));
    }
    ~Scratch() { std::error_code error; fs::remove_all(path, error); }
};
struct UnreachedViews final : moproj::DiagramViews {
    unsigned calls = 0;
    void viewCentre(const SubProject&, double& x, double& y) override { ++calls; x = y = 0; }
    xml::NodePtr dispView(const SubProject&) override { ++calls; return {}; }
    bool expanded(const SubProject&, int) override { ++calls; return false; }
    moproj::RectF rect(const SubProject&, int) override { ++calls; return {}; }
    std::vector<moproj::ExpansionItem> expansion(const SubProject&, int) override { ++calls; return {}; }
};
struct Rig {
    Scratch scratch; // Destroyed after Project and its model/Study owners.
    Project project;
    SubProject* sub = nullptr;
    Operation* operation = nullptr;
    Ctx context;
    engine::SaveEffects saved;
    engine::FileEffects opened;
    UnreachedViews views;
    Rig()
    {
        initProject(project, qt::String(scratch.path.u16string()), qt::L("output-save"));
        makeRegistry(project);
        sub = activeSubProject(project);
        REQUIRE(sub != nullptr);
        sub->type = 2;
        sub->ownedDoeStudy = doe::makeStudy(*sub, nullptr);
        REQUIRE(sub->ownedDoeStudy != nullptr);
        sub->doe = sub->ownedDoeStudy.get();
        operation = createOperation(project, qt::L("Output"), qt::L("SfOprDoeOutput"), {}, 71);
        REQUIRE(operation != nullptr);
        REQUIRE(opBase(*operation) != nullptr);
        opBase(*operation)->current = operation;
        sub->flow.items.clear();
        auto item = std::make_unique<FlowItem>();
        item->opId = operation->id;
        item->className = operation->classKey;
        sub->flow.items.push_back(std::move(item));
        sub->flow.activeItem = 0;
        project.openedOk = true;
        saved.step2.nativeDirectories = true;
    }
    fs::path regions() const
    {
        return fs::path(qt::cleanPath(mst::doeControlDir(*sub, &sub->ownedDoeStudy->name) +
                                     qt::kSep + qt::L("Regions")).u16());
    }
    std::string saveRefusal()
    {
        try { (void)engine::saveProject(project, context, views, opened, saved); }
        catch (const Error& error) {
            CHECK_EQ(error.code, M2M_E_UNSUPPORTED);
            return error.what();
        }
        CHECK(false);
        return {};
    }
    void checkNoLaterSaveEffects()
    {
        CHECK(saved.current == operation);
        CHECK(saved.call.validationComplete);
        CHECK(!saved.call.projectWriteAttempted && !saved.call.projectWriteReturned);
        CHECK(!saved.call.mstWriteAttempted);
        CHECK(saved.step2.writes.empty() && saved.step2.deletes.empty() && saved.step2.mkdirs.empty());
        CHECK(saved.step2.removalAttempts.empty() && saved.step2.renames.empty());
        CHECK(saved.project.writes.empty() && !saved.project.xmlJournal);
        CHECK_EQ(views.calls, 0u);
        CHECK(!sub->ownedDoeStudy->outputDocument);
    }
};
void write(const fs::path& path, const char* bytes)
{
    fs::create_directories(path.parent_path());
    std::ofstream file(path, std::ios::binary);
    REQUIRE(file.good());
    file << bytes;
}
const char* cleanupGuard = "Control/Regions enumeration remains unsupported after operation-model lookup";
const char* modalGuard = "Control/Regions warning modal return remains unsupported";
}

TEST_CASE(output_save_successful_mkdir_reaches_actual_existing_model_then_cleanup_guard)
{
    Rig rig;
    rig.operation->engineModel = std::make_shared<engine::GuiModel>(nullptr, 3);
    CHECK(rig.operation->hasEngineModel());
    const auto model = rig.operation->engineModel;
    const auto sentinel = rig.regions() / "retained.txt";
    write(sentinel, "not enumerated or removed");

    CHECK(rig.saveRefusal().find(cleanupGuard) != std::string::npos);

    REQUIRE(rig.saved.step2.directoryAttempts.size() == 1);
    const auto& attempt = rig.saved.step2.directoryAttempts.front();
    CHECK(attempt.succeeded && attempt.path == qt::String(rig.regions().u16string()));
    CHECK(rig.operation->engineModel == model);
    CHECK(fs::is_regular_file(sentinel));
    rig.checkNoLaterSaveEffects();
}

TEST_CASE(output_save_failed_mkdir_preserves_attempt_and_stops_before_model_getter)
{
    Rig rig;
    const auto blocker = rig.regions();
    write(blocker, "blocking file");
    REQUIRE(!rig.operation->engineModel);
    CHECK(!rig.operation->hasEngineModel());

    CHECK(rig.saveRefusal().find(modalGuard) != std::string::npos);

    REQUIRE(rig.saved.step2.directoryAttempts.size() == 1);
    CHECK(!rig.saved.step2.directoryAttempts.front().succeeded);
    CHECK(rig.saved.step2.directoryAttempts.front().path == qt::String(blocker.u16string()));
    CHECK(!rig.operation->engineModel);
    CHECK(fs::is_regular_file(blocker));
    rig.checkNoLaterSaveEffects();
}

TEST_CASE(output_save_successful_mkdir_runs_actual_lazy_operation_model_producer)
{
    Rig rig;
    REQUIRE(!rig.operation->engineModel);
    CHECK(!rig.operation->hasEngineModel());

    CHECK(rig.saveRefusal().find(cleanupGuard) != std::string::npos);

    REQUIRE(rig.operation->engineModel != nullptr);
    CHECK(rig.operation->hasEngineModel());
    CHECK_EQ(rig.operation->engineModel->f25c, 0); // Later ROIs getter was not invoked.
    REQUIRE(rig.saved.step2.directoryAttempts.size() == 1);
    CHECK(rig.saved.step2.directoryAttempts.front().succeeded);
    CHECK(fs::is_directory(rig.regions()));
    rig.checkNoLaterSaveEffects();
}

TEST_CASE(output_save_owner_and_non_immediate_guards_precede_directory_and_model_effects)
{
    Rig rig;
    auto* study = rig.sub->ownedDoeStudy.get();
    study->sub = nullptr;
    CHECK(rig.saveRefusal().find("actual active Type2 Project/SubProject/Study owner is unavailable") != std::string::npos);
    CHECK(rig.saved.step2.directoryAttempts.empty() && !rig.operation->engineModel);
    CHECK(!fs::exists(rig.regions()));

    study->sub = rig.sub;
    rig.saved.step2.nativeDirectories = false;
    CHECK(rig.saveRefusal().find("immediate native directory profile is unavailable") != std::string::npos);
    CHECK(rig.saved.step2.directoryAttempts.empty() && !rig.operation->engineModel);
    CHECK(!fs::exists(rig.regions()));
    rig.checkNoLaterSaveEffects();
}

int main() { return testkit::run_all(); }
