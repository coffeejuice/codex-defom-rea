// CP226: model-owned dimension command preserves the native setter boundary.
#include <cstdint>
#include <vector>

#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/llobj_ctor/llobj_ctor.hpp>
#include <testkit/check.hpp>

#include "m2m/engine.hpp"

using namespace m2m::engine;

TEST_CASE(gui_model_equal_dimension_preserves_native_and_wrapper_state)
{
    for (const int dimension : {2, 3}) {
        GuiModel model(nullptr, dimension);
        auto* const address = model.step.get();
        auto* const object = def::model::llobj_ctor::get_or_create(address, 1);
        REQUIRE(object);
        REQUIRE(model.objectCount() == 1);
        model.step->sim_ctrl.nstep = 47;
        model.state100 = 19;
        const std::vector<int> mirrorPlanes{2, 5};
        model.viewerMirrorPlanes = mirrorPlanes;
        const auto before = model.stepIdentity();

        model.setDimension(dimension);

        CHECK(model.step.get() == address);
        CHECK_EQ(model.dimension(), dimension);
        CHECK_EQ(model.objectCount(), 1);
        CHECK(model.object(1) == object);
        CHECK_EQ(model.step->sim_ctrl.nstep, 47);
        CHECK_EQ(model.state100, 19);
        CHECK(model.viewerMirrorPlanes == mirrorPlanes);
        // CP222 observes constructor/reset/destruction only; this grants no storage access.
        CHECK(before == model.stepIdentity());
        CHECK(model.isCurrentStepIdentity(before));
    }
}

TEST_CASE(gui_model_changed_dimension_uses_native_defaults_without_gui_reset)
{
    for (const int dimension : {2, 3}) {
        const int targetDimension = dimension == 2 ? 3 : 2;
        GuiModel model(nullptr, dimension);
        auto* const address = model.step.get();
        REQUIRE(def::model::llobj_ctor::get_or_create(address, 1));
        REQUIRE(model.objectCount() == 1);
        model.step->sim_ctrl.nstep = 47;
        model.state100 = 19;
        const std::vector<int> mirrorPlanes{2, 5};
        model.viewerMirrorPlanes = mirrorPlanes;
        const auto before = model.stepIdentity();

        // Qualify native initialization separately from GuiModel::reset's GUI defaults.
        def::model::DB_STEP_DATA native(dimension);
        REQUIRE(def::model::llobj_ctor::get_or_create(&native, 1));
        native.sim_ctrl.nstep = 47;
        const std::int32_t nativeDimension = targetDimension;
        native.set_dimension(&nativeDimension);
        CHECK_EQ(native.sim_ctrl.nstep, 1);
        CHECK_EQ(native.u1378, 0);

        model.setDimension(targetDimension);

        CHECK(model.step.get() == address);
        CHECK_EQ(model.dimension(), targetDimension);
        CHECK_EQ(model.objectCount(), 0);
        CHECK(model.object(1) == nullptr); // Query fresh state; the old object was destroyed.
        CHECK_EQ(def::model::llobj_ctor::list_count(&model.step->objects), 0);
        CHECK_EQ(model.step->sim_ctrl.nstep, native.sim_ctrl.nstep);
        CHECK_EQ(model.state100, 19);
        CHECK(model.viewerMirrorPlanes == mirrorPlanes);
        // Intentional staging limit: a current observation does not authorize old aliases.
        CHECK(before == model.stepIdentity());
        CHECK(model.isCurrentStepIdentity(before));
    }
}

int main() { return testkit::run_all(); }
