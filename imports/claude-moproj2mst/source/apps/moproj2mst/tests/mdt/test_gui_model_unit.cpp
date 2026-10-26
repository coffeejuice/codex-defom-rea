#include <testkit/check.hpp>

#include <defkey/item_writers.hpp>
#include <defmodel/layout/db_step_data.hpp>

#include "m2m/engine.hpp"
#include "m2m/mst/writer.hpp"
#include "m2m/p17.hpp"

namespace m2m::mst {
void installLoaderSlots();
}

using m2m::engine::GuiModel;

TEST_CASE(gui_model_unit_command_matches_native_changed_unit)
{
    m2m::GuiStartupUnitScope startup(0);
    GuiModel actual(nullptr, 3);
    GuiModel nativeBaseline(nullptr, 3);

    actual.setModelUnit(2, true);
    def::key::set_model_unit(*nativeBaseline.step, 2, true);

    CHECK_EQ(actual.step->sim_ctrl.unit, nativeBaseline.step->sim_ctrl.unit);
    CHECK_BITS(actual.step->sim_ctrl.envtmp.value, nativeBaseline.step->sim_ctrl.envtmp.value);
    CHECK_BITS(actual.step->sim_ctrl.unte2h, nativeBaseline.step->sim_ctrl.unte2h);
    CHECK_BITS(actual.step->sim_ctrl.cnvcof.value, nativeBaseline.step->sim_ctrl.cnvcof.value);
}

TEST_CASE(gui_model_unit_command_matches_native_equal_unit_noop)
{
    m2m::GuiStartupUnitScope startup(0);
    GuiModel actual(nullptr, 3);
    GuiModel nativeBaseline(nullptr, 3);
    actual.step->sim_ctrl.envtmp.value = 321.0;
    nativeBaseline.step->sim_ctrl.envtmp.value = 321.0;

    actual.setModelUnit(0, true);
    def::key::set_model_unit(*nativeBaseline.step, 0, true);

    CHECK_EQ(actual.step->sim_ctrl.unit, 0);
    CHECK_BITS(actual.step->sim_ctrl.envtmp.value, 321.0);
    CHECK_BITS(actual.step->sim_ctrl.envtmp.value, nativeBaseline.step->sim_ctrl.envtmp.value);
}

TEST_CASE(loader_new_model_uses_project_unit_after_gui_initialization)
{
    m2m::GuiStartupUnitScope startup(0);
    m2m::mst::installLoaderSlots();
    REQUIRE(m2m::mst::g_loaderSlots.newModel != nullptr);

    m2m::Project changedProject;
    changedProject.unit = 2;
    const auto changed = m2m::mst::g_loaderSlots.newModel(changedProject, 3);
    REQUIRE(changed != nullptr);
    CHECK_EQ(changed->step->sim_ctrl.unit, 2);
    CHECK_BITS(changed->step->sim_ctrl.envtmp.value, 68.0);
    CHECK_BITS(changed->step->sim_ctrl.unte2h, 0.107);

    m2m::Project equalProject;
    equalProject.unit = 0;
    const auto equal = m2m::mst::g_loaderSlots.newModel(equalProject, 3);
    REQUIRE(equal != nullptr);
    CHECK_EQ(equal->step->sim_ctrl.unit, 0);
    CHECK_BITS(equal->step->sim_ctrl.envtmp.value, 20.0);
}

int main() { return testkit::run_all(); }
