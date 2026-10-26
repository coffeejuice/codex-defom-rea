// GUI1408d3bc0 initializes UNIT from the startup global before owner/init.
// Source contract: analysis/wave2-save-diffs-300-gui-unit-ctor-contract.json.
#include <testkit/check.hpp>
#include <cstring>
#include <new>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/step/db_step.hpp>
#include <defkey/item_writers.hpp>
#include "m2m/engine.hpp"
#include "m2m/p17.hpp"
#include "m2m/convert.hpp"

using namespace m2m;

TEST_CASE(gui_constructor_overwrites_prior_heap_unit_before_first_environment_init)
{
    REQUIRE(def::model::g_gui_engine_variant);
    using Control = def::model::Cls_1401e7b10;
    for (const int unit : {0, 1, 2}) {
        GuiStartupUnitScope profile(unit);
        alignas(Control) unsigned char storage[sizeof(Control)];
        std::memset(storage, 0xa5, sizeof(storage));
        auto* control = ::new (static_cast<void*>(storage)) Control;
        CHECK_EQ(control->unit, unit);
        CHECK(control->owner == nullptr);
        control->~Control();
        for (const int dim : {2, 3}) {
            engine::GuiModel model(nullptr, dim);
            CHECK_EQ(model.step->sim_ctrl.unit, unit);
            CHECK_BITS(model.step->sim_ctrl.envtmp.value, unit == 2 ? 68.0 : 20.0);
            CHECK_BITS(model.step->sim_ctrl.unte2h, unit == 2 ? 0.107 : 1.0);
        }
    }
    CHECK_EQ(guiStartupUnit(), 0);
}

TEST_CASE(loaded_model_unit_does_not_replace_gui_constructor_global)
{
    GuiStartupUnitScope profile(0);
    engine::GuiModel loaded(nullptr, 3);
    def::key::set_model_unit(*loaded.step, 2, true);
    CHECK_EQ(loaded.step->sim_ctrl.unit, 2);
    CHECK_EQ(guiStartupUnit(), 0);
    engine::GuiModel later(nullptr, 3);
    CHECK_EQ(later.step->sim_ctrl.unit, 0);
    CHECK_BITS(later.step->sim_ctrl.envtmp.value, 20.0);
}

TEST_CASE(conversion_failure_restores_callers_startup_profile)
{
    GuiStartupUnitScope outer(2);
    {
        GuiStartupUnitScope inner(1);
        CHECK_EQ(guiStartupUnit(), 1);
    }
    CHECK_EQ(guiStartupUnit(), 2);
    std::string mst;
    std::u16string output;
    const auto result = convertToBytes(u"", u"", 0, 1252, mst, output, 0);
    CHECK(result.code != M2M_OK);
    CHECK_EQ(guiStartupUnit(), 2);
    engine::GuiModel afterFailure(nullptr, 2);
    CHECK_EQ(afterFailure.step->sim_ctrl.unit, 2);
    CHECK_BITS(afterFailure.step->sim_ctrl.envtmp.value, 68.0);
}

TEST_CASE(def_pre_constructor_retains_its_distinct_store_contract)
{
    struct VariantRestore {
        bool prior = def::model::g_gui_engine_variant;
        ~VariantRestore() { def::model::g_gui_engine_variant = prior; }
    } restore;
    def::model::g_gui_engine_variant = false;
    GuiStartupUnitScope guiProfile(2);
    using Control = def::model::Cls_1401e7b10;
    alignas(Control) unsigned char storage[sizeof(Control)];
    std::memset(storage, 0xa5, sizeof(storage));
    auto* control = ::new (static_cast<void*>(storage)) Control;
    // Inspect object representation, never evaluate the indeterminate int.
    unsigned char unitBytes[sizeof(control->unit)];
    std::memcpy(unitBytes, &control->unit, sizeof(unitBytes));
    for (auto byte : unitBytes) CHECK_EQ(byte, 0xa5);
    control->unit = 1;
    control->~Control();
}

int main() { return testkit::run_all(); }
