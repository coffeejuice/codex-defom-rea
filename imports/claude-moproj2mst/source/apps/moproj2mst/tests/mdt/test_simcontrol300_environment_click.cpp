// ctest-labels: m2m-mdt
// Bound private control projection; no physical QWidget/full Session claim.
#include <testkit/check.hpp>
#include <algorithm>
#include <array>
#include <memory>
#include <optional>
#include <utility>
#include "m2m/engine/simcontrol300_owner.hpp"
#include "m2m/mst/writer.hpp"
#include <defmodel/dynary_init.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/dynary.hpp>

using namespace m2m;
using namespace m2m::engine::simcontrol300;
namespace {
struct Io final : Extra3DIo {
    Io() : Extra3DIo(1252) {}
    int reads = 0, writes = 0, removes = 0;
    bool readDoubles(const qt::String&, std::span<double> values) override
    { ++reads; std::fill(values.begin(), values.end(), 0.0); return false; }
    bool readInts(const qt::String&, std::span<int> values) override
    { ++reads; std::fill(values.begin(), values.end(), 0); return false; }
    bool readSprings(std::vector<SpringRecord>& values) override
    { ++reads; values.clear(); return false; }
    bool removeIfReadable(const qt::String&) override { ++removes; return true; }
    void nbcRemoveWarning() override {}
    std::unique_ptr<mst::OutputDevice> openWrite(const qt::String&) override
    { ++writes; return nullptr; }
};
struct Fixture {
    Project project; Operation operation; RemeshState remesh; Io io;
    std::unique_ptr<StagedEditor300> owner;
    Fixture()
    {
        operation.project = &project;
        operation.engineModel = std::make_shared<engine::GuiModel>(nullptr, 3);
        auto& sc = *operation.engineModel->simCtl();
        sc.unit = 1; sc.nstart = 7; sc.nstep = 23;
        sc.trans[0] = sc.trans[1] = 1;
        owner = std::make_unique<StagedEditor300>(binding());
    }
    Editor300Binding binding() { return {project, operation, *operation.engineModel, remesh}; }
    InitialEditorContext context()
    {
        MoInitialObservations features;
        features.generalFills[0] = MoFeatureAvailability{1, std::nullopt, {1, 1, 1}};
        features.generalFills[1] = MoFeatureAvailability{1, std::nullopt, {1, 1, 1}};
        features.startupHeating = {0, 1};
        return {generalContextForMo(project, 3, {true, true}),
            {5, {}, MoStepCountContext{&operation}}, NumericLocale::KnownC, true, features};
    }
    InitialEditorSession& open()
    {
        owner->constructPageState(binding(), io, context());
        auto* pages = owner->pageState(); REQUIRE(pages); return *pages;
    }
};
void table(def::model::DYNARY& curve)
{
    def::model::dynary_free_arrays(&curve);
    curve.ndim = 1; curve.n[0] = 2; curve.n[1] = curve.n[2] = 0; curve.ncomp = 1;
    def::model::dynary_alloc(&curve); REQUIRE(curve.axis[0]); REQUIRE(curve.values);
    curve.axis[0][0] = 10; curve.axis[0][1] = 20;
    curve.values[0] = 1.25; curve.values[1] = 2.5;
    curve.var[0] = 3; curve.value = 17.5;
}
void controls(const EnvironmentCurveControls& value, bool constant)
{
    CHECK(value.constantChecked == constant); CHECK(value.tableChecked == !constant);
    CHECK(value.constantInputEnabled == constant); CHECK(value.constantUnitEnabled == constant);
    CHECK(value.definitionEnabled == !constant);
}
std::array<def::model::DYNARY*, 3> curves(SimControl& sc)
{ return {&sc.envtmp, &sc.cnvcof, &sc.envatm}; }
}

TEST_CASE(environment_clicked_all_six_choices_project_only_addressed_controls)
{
    for (int index = 0; index < 3; ++index) for (int selection : {0, 1}) {
        Fixture f; auto& session = f.open(); auto& form = session.pages.environment;
        auto live = curves(*f.operation.engineModel->simCtl());
        auto& temporary = *session.environmentCurves.curves[index];
        table(*live[index]); table(temporary);
        const auto* owner = &temporary; const auto* axis = temporary.axis[0];
        const auto* values = temporary.values; const auto* liveValues = live[index]->values;
        form.constantValues[index] = {qt::L("UNSAVED BAD TEXT"), ValidatorState::Invalid};
        session.environmentCurves.currentCurveIndex = 2;
        const int reads = f.io.reads;
        CHECK(f.owner->environmentCurveClicked(f.binding(), index, selection) == ApplyResult::Completed);
        for (int other = 0; other < 3; ++other)
            controls(form.curveControls[other], other == index ? selection == 0 : true);
        CHECK_EQ(form.acvcofControls.selection, 0); CHECK_EQ(*session.environmentCurves.currentCurveIndex, 2);
        CHECK_EQ(temporary.ndim, 1); CHECK_EQ(live[index]->ndim, 1);
        CHECK_EQ(temporary.n[0], 2); CHECK_EQ(live[index]->n[0], 2);
        CHECK_EQ(temporary.var[0], 3); CHECK_EQ(live[index]->var[0], 3);
        CHECK(temporary.value == 17.5); CHECK(live[index]->value == 17.5);
        CHECK(temporary.axis[0] == axis); CHECK(temporary.values == values);
        CHECK(live[index]->values == liveValues); CHECK(temporary.values[1] == 2.5);
        CHECK(form.curves[index].get() == owner); CHECK(session.environmentCurves.curves[index].get() == owner);
        CHECK(form.constantValues[index].text == qt::L("UNSAVED BAD TEXT"));
        CHECK(form.constantValues[index].state == ValidatorState::Invalid);
        CHECK_EQ(session.pages.stepCount.steps, 23); CHECK_EQ(f.operation.nstartGiven, 0);
        CHECK_EQ(f.io.reads, reads); CHECK_EQ(f.io.writes, 0); CHECK_EQ(f.io.removes, 0);
    }
}

TEST_CASE(environment_clicked_reselection_normalizes_controls_without_refill)
{
    Fixture f; auto& form = f.open().pages.environment;
    form.constantValues[0].text = qt::L("REMEMBERED EDIT");
    for (int selection : {0, 1, 0}) {
        CHECK(f.owner->environmentCurveClicked(f.binding(), 0, selection) == ApplyResult::Completed);
        controls(form.curveControls[0], selection == 0);
    }
    form.curveControls[0] = {true, true, false, false, true};
    CHECK(f.owner->environmentCurveClicked(f.binding(), 0, 0) == ApplyResult::Completed);
    controls(form.curveControls[0], true);
    CHECK(form.constantValues[0].text == qt::L("REMEMBERED EDIT"));
}

TEST_CASE(environment_clicked_requires_no_current_sc_or_curve_payload)
{
    for (bool replace : {false, true}) {
        Fixture f; auto& session = f.open();
        auto oldStep = std::move(f.operation.engineModel->step); oldStep->sim_ctrl.envtmp.var[0] = 99;
        if (replace) f.operation.engineModel->step = std::make_unique<def::model::DB_STEP_DATA>(3);
        auto* replacement = f.operation.engineModel->simCtl();
        if (replacement) replacement->envtmp.var[0] = 98;
        // No curve owner is consumed by these control-only native callbacks.
        session.pages.environment.curves[0].reset(); session.environmentCurves.curves[0].reset();
        CHECK(f.owner->environmentCurveClicked(f.binding(), 0, 1) == ApplyResult::Completed);
        controls(session.pages.environment.curveControls[0], false);
        CHECK_EQ(oldStep->sim_ctrl.envtmp.var[0], 99);
        if (replacement) CHECK_EQ(replacement->envtmp.var[0], 98);
    }
}

TEST_CASE(environment_table_click_defers_to_apply_and_skips_invalid_constant_text)
{
    for (int index = 0; index < 3; ++index) {
        Fixture f; auto& session = f.open(); auto& form = session.pages.environment;
        auto& sc = *f.operation.engineModel->simCtl(); auto live = curves(sc);
        auto& temporary = *session.environmentCurves.curves[index]; table(temporary);
        form.constantValues[index] = {qt::L("INVALID"), ValidatorState::Invalid};
        live[index]->value = 99;
        CHECK(f.owner->environmentCurveClicked(f.binding(), index, 1) == ApplyResult::Completed);
        CHECK_EQ(live[index]->ndim, 0); CHECK(live[index]->value == 99);
        CHECK(applyEnvironment3D(sc, form, std::nullopt, &session.environmentCurves) == ApplyResult::Completed);
        CHECK_EQ(live[index]->ndim, 1); CHECK_EQ(live[index]->n[0], 2);
        CHECK(live[index]->value == 17.5); REQUIRE(live[index]->values);
        CHECK(live[index]->values[0] == 1.25); CHECK(live[index]->values[1] == 2.5);
        CHECK(live[index]->values != temporary.values); CHECK_EQ(temporary.ndim, 1);
    }
}

TEST_CASE(environment_constant_click_defers_dimension_and_invalid_value_handling_until_apply)
{
    for (int index = 0; index < 3; ++index) {
        Fixture f; auto& session = f.open(); auto& form = session.pages.environment;
        auto& sc = *f.operation.engineModel->simCtl(); auto live = curves(sc);
        auto& temporary = *session.environmentCurves.curves[index]; table(temporary);
        const auto* values = temporary.values;
        form.constantValues[index] = {qt::L("INVALID"), ValidatorState::Invalid};
        CHECK(f.owner->environmentCurveClicked(f.binding(), index, 0) == ApplyResult::Completed);
        CHECK_EQ(temporary.ndim, 1); CHECK(temporary.values == values);
        CHECK(applyEnvironment3D(sc, form, std::nullopt, &session.environmentCurves) == ApplyResult::Completed);
        CHECK_EQ(temporary.ndim, 0); CHECK(temporary.value == 17.5);
        CHECK(temporary.values == values); CHECK_EQ(temporary.n[0], 2);
        CHECK_EQ(live[index]->ndim, 0); CHECK(live[index]->value == 17.5);
    }
}

TEST_CASE(environment_clicked_invalid_selection_index_or_binding_refuses_before_projection)
{
    Fixture f; auto& form = f.open().pages.environment;
    for (const auto pair : {std::pair{-1, 0}, std::pair{3, 0}, std::pair{0, -1}, std::pair{0, 2}}) {
        CHECK(f.owner->environmentCurveClicked(f.binding(), pair.first, pair.second) == ApplyResult::UnsupportedSnapshot);
        for (const auto& value : form.curveControls) controls(value, true);
    }
    Project otherProject;
    CHECK(f.owner->environmentCurveClicked({otherProject, f.operation, *f.operation.engineModel, f.remesh}, 0, 1)
        == ApplyResult::UnsupportedSnapshot);
    for (const auto& value : form.curveControls) controls(value, true);
}

TEST_CASE(environment_clicked_unconstructed_and_partial_owner_cannot_create_pages)
{
    Fixture f;
    CHECK(f.owner->environmentCurveClicked(f.binding(), 0, 1) == ApplyResult::UnsupportedSnapshot);
    CHECK(!f.owner->partialPageState());
    auto context = f.context(); context.moObservations.reset();
    f.owner->constructPageState(f.binding(), f.io, context);
    CHECK(f.owner->environmentCurveClicked(f.binding(), 0, 1) == ApplyResult::UnsupportedSnapshot);
    CHECK(!f.owner->pageState()); CHECK(!StagedEditor300::completeForPublicSave);
}

TEST_CASE(environment_clicked_expired_opi_retains_completed_control_values)
{
    Fixture f; auto& session = f.open();
    REQUIRE(f.operation.commitOperationInfoQObjectBoundary(
        "declared private Environment fixture OPI boundary; not native destruction"));
    CHECK(f.owner->environmentCurveClicked(f.binding(), 2, 1) == ApplyResult::UnsupportedSnapshot);
    for (const auto& value : session.pages.environment.curveControls) controls(value, true);
    CHECK(!f.owner->pageState()); CHECK(!StagedEditor300::completeForPublicSave);
}

int main() { return testkit::run_all(); }
