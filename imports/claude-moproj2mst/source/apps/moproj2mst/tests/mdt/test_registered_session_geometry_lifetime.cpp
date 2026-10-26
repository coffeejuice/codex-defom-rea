// Adapter ownership and teardown; neither path claims original normal Close.
// ctest-labels: m2m-mdt geometry session
#include "../support/test_paths.hpp"
#include <testkit/check.hpp>

#include <array>
#include <memory>
#include <utility>
#include <vector>

#include <defmodel/geo_kw/geo_kw.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/obj_geo3d.hpp>
#include <defmodel/llobj/object_list.hpp>

#include "m2m/engine.hpp"
#include "m2m/engine/conversion_session.hpp"
#include "m2m/engine/model_manager.hpp"
#include "m2m/engine/mo_model_slots.hpp"
#include "m2m/engine_scene.hpp"
#include "m2m/model.hpp"
#include "m2m/qt4path.hpp"

using namespace m2m;

namespace {
bool hasNonemptySurface(const engine::GuiModel& model)
{
    if (!model.step) return false;
    auto* object = def::model::object_list_find(&model.step->objects, 1);
    auto* surface = object ? def::model::geo_kw::surface(*object, 0) : nullptr;
    return surface && surface->node_count() > 0;
}

struct DeleteObservation {
    bool displayAlive, registryAlive, nonemptyGeometry, priorProjectRestored;
};
struct AdapterUnwind {}; // Deliberate test-stack unwind, not a native query failure.

void registeredGeometryLifetime(bool unwindBeforeExplicitRelease)
{
    const auto key = m2m_test::configuredPath("M2M_CORPUS") /
        "42_vdie" / "Objects" / "Object00001.KEY";
    auto* const priorProject = currentProject();
    std::weak_ptr<engine::DbHandle> childLifetime;
    std::weak_ptr<engine::MoModelSlots> slotsLifetime;
    std::weak_ptr<engine::OperationClassRegistry> registryLifetime;
    std::weak_ptr<engine::Display> displayLifetime;
    std::array<std::weak_ptr<engine::GuiModel>, 2> models;
    std::vector<DeleteObservation> deleted;
    bool unwound = false;
    engine::DiagramEffectDriver driver;
    try {
        engine::ConversionSession session(driver);
        auto& project = session.project();
        session.prepareProjectModel(qt::L("C:/PROBLEM/G102/session.DB"));
        initProject(project, qt::L("C:/PROBLEM/G102"), qt::L("session"));
        session.commitProjectModelConstruction();
        session.commitProjectModelSelection();
        REQUIRE(project.dataDb);
        REQUIRE(session.modelManagerOwner()->currentModel() == project.dataDb.get());
        REQUIRE(project.dataDb->moModelSlotsOwner() == project.modelSlotsOwner());
        childLifetime = project.dataDb;
        slotsLifetime = project.modelSlotsOwner();
        registryLifetime = project.registryOwner();
        displayLifetime = project.display;
        REQUIRE(!displayLifetime.expired());

        for (std::size_t index = 0; index < models.size(); ++index) {
            auto operation = std::make_unique<Operation>();
            operation->project = &project;
            operation->engineModel = std::shared_ptr<engine::GuiModel>(
                new engine::GuiModel(project.dataDb.get(), 3), [&](engine::GuiModel* model) {
                    deleted.push_back({!displayLifetime.expired(), !registryLifetime.expired(),
                        hasNonemptySurface(*model), currentProject() == priorProject});
                    delete model; // Observe the boundary, then run the actual destructor.
                });
            auto& model = *operation->engineModel;
            REQUIRE(model.readKey(qt::String(key.u16string())));
            REQUIRE(hasNonemptySurface(model));
            engine::displayShow(project, &model);
            models[index] = operation->engineModel;
            project.operations.push_back(std::move(operation));
        }
        CHECK(currentProject() == &project);
        CHECK(project.modelSlotsOwner()->operations().size() == 2);
        CHECK(deleted.empty());
        if (unwindBeforeExplicitRelease) throw AdapterUnwind{};

        session.commitNullDiagramBinding();
        session.releaseProjectAfterCloseEffects(); // Adapter storage boundary only.
        CHECK(session.liveProject() == nullptr);
        CHECK(currentProject() == priorProject);
        CHECK(!displayLifetime.expired() && !registryLifetime.expired());
        CHECK(!childLifetime.expired() && !slotsLifetime.expired());
        CHECK(deleted.empty());
        {
            const auto child = childLifetime.lock();
            const auto slots = slotsLifetime.lock();
            REQUIRE(child && slots);
            CHECK(child->moProject == nullptr);
            CHECK(child->moModelSlotsOwner() == slots);
            CHECK(session.modelManagerOwner()->currentModel() == child.get());
            CHECK(!slots->projectStorageAlive());
            CHECK(slots->nextStorageBoundary() == engine::MoModelSlots::Slot::Operations);
            CHECK(slots->operations().size() == 2);
        } // No diagnostic strong lease escapes the Session boundary.
        for (const auto& weakModel : models) {
            const auto model = weakModel.lock();
            REQUIRE(model);
            CHECK(hasNonemptySurface(*model));
        }
    } catch (const AdapterUnwind&) {
        unwound = true;
    }
    CHECK(unwound == unwindBeforeExplicitRelease);
    CHECK(currentProject() == priorProject);
    REQUIRE(deleted.size() == models.size());
    for (const auto& observation : deleted) {
        CHECK(observation.displayAlive && observation.registryAlive);
        CHECK(observation.nonemptyGeometry && observation.priorProjectRestored);
    }
    for (const auto& model : models) CHECK(model.expired());
    CHECK(childLifetime.expired() && slotsLifetime.expired());
    CHECK(displayLifetime.expired() && registryLifetime.expired());
}
} // namespace

TEST_CASE(registered_session_keeps_nonempty_geometry_after_project_storage_release)
{
    registeredGeometryLifetime(false);
}

TEST_CASE(registered_session_adapter_unwind_keeps_display_through_nonempty_model_teardown)
{
    registeredGeometryLifetime(true);
}

int main() { return testkit::run_all(); }
