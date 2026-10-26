// ctest-labels: m2m-mdt
// Declared private clicked event; no native-widget or public Session claim.
#include <testkit/check.hpp>
#include <algorithm>
#include <array>
#include <cstring>
#include <memory>
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
    curve.ndim = 1; curve.n[0] = 2; curve.n[1] = curve.n[2] = 0; curve.ncomp = 2;
    def::model::dynary_alloc(&curve);
    REQUIRE(curve.axis[0]); REQUIRE(curve.values);
    curve.axis[0][0] = 10; curve.axis[0][1] = 20;
    for (int i = 0; i < 4; ++i) curve.values[i] = i + 0.25;
    curve.value = 17.5; curve.u54 = 31; curve.u58 = 41; curve.var[0] = 88;
}
void controls(const EnvironmentAcvcofControls& state, int selection)
{
    CHECK_EQ(state.selection, selection);
    CHECK(state.constantInputEnabled == (selection == 0));
    CHECK(state.constantUnitEnabled == (selection == 0));
    CHECK(state.definitionEnabled == (selection != 0));
}
void complete(const EnvironmentReactionReport& report)
{
    CHECK(report.outcome == ApplyResult::Completed);
    CHECK(report.stage == EnvironmentReactionStage::Complete);
    CHECK(report.controlsProjected); CHECK(report.modelUpdated);
}
void preflight(const EnvironmentReactionReport& report)
{
    CHECK(report.outcome == ApplyResult::UnsupportedSnapshot);
    CHECK(report.stage == EnvironmentReactionStage::Preflight);
    CHECK(!report.controlsProjected); CHECK(!report.modelUpdated);
}
void projectedRefusal(const EnvironmentReactionReport& report)
{
    CHECK(report.outcome == ApplyResult::UnsupportedSnapshot);
    CHECK(report.stage == EnvironmentReactionStage::ControlsProjected);
    CHECK(report.controlsProjected); CHECK(!report.modelUpdated);
}
}

TEST_CASE(reaction_clicked_all_choices_update_actual_live_and_temporary_variable_only)
{
    constexpr int variables[]{0, 3, 12, 1};
    for (int choice = 0; choice < 4; ++choice) {
        Fixture f; auto& session = f.open(); auto& page = session.pages.environment;
        auto& live = f.operation.engineModel->simCtl()->acvcof;
        auto& temporary = *session.environmentCurves.curves[3];
        table(live); table(temporary);
        const auto* owner = &temporary; const auto* liveAxis = live.axis[0];
        const auto* liveValues = live.values; const auto* tempAxis = temporary.axis[0];
        const auto* tempValues = temporary.values;
        page.constantValues[3].text = qt::L("UNSAVED INVALID TEXT");
        session.environmentCurves.currentCurveIndex = 1;
        const int reads = f.io.reads;
        complete(f.owner->reactionRateClicked(f.binding(), choice));
        controls(page.acvcofControls, choice);
        REQUIRE(session.environmentCurves.currentCurveIndex);
        CHECK_EQ(*session.environmentCurves.currentCurveIndex, 3);
        CHECK_EQ(live.var[0], variables[choice]); CHECK_EQ(temporary.var[0], variables[choice]);
        CHECK_EQ(live.ndim, choice == 0 ? 0 : 1); CHECK_EQ(temporary.ndim, 1);
        CHECK(page.curves[3].get() == owner); CHECK(session.environmentCurves.curves[3].get() == owner);
        CHECK(live.axis[0] == liveAxis); CHECK(live.values == liveValues);
        CHECK(temporary.axis[0] == tempAxis); CHECK(temporary.values == tempValues);
        CHECK_EQ(live.n[0], 2); CHECK_EQ(temporary.n[0], 2);
        CHECK_EQ(live.ncomp, 2); CHECK_EQ(temporary.ncomp, 2);
        for (int i = 0; i < 4; ++i) {
            CHECK(live.values[i] == i + 0.25); CHECK(temporary.values[i] == i + 0.25);
        }
        CHECK(live.value == 17.5); CHECK(temporary.value == 17.5);
        CHECK_EQ(live.u54, 31); CHECK_EQ(temporary.u54, 31);
        CHECK_EQ(live.u58, 41); CHECK_EQ(temporary.u58, 41);
        CHECK(page.constantValues[3].text == qt::L("UNSAVED INVALID TEXT"));
        CHECK_EQ(f.io.reads, reads); CHECK_EQ(f.io.writes, 0); CHECK_EQ(f.io.removes, 0);
        CHECK_EQ(f.operation.nstartGiven, 0); CHECK_EQ(session.pages.stepCount.steps, 23);
    }
}

TEST_CASE(reaction_constant_then_variable_preserves_zero_live_dimension_and_table_storage)
{
    Fixture f; auto& session = f.open(); auto& live = f.operation.engineModel->simCtl()->acvcof;
    auto& temporary = *session.environmentCurves.curves[3]; table(live); table(temporary);
    const auto* values = live.values; const auto* temporaryValues = temporary.values;
    constexpr int variables[]{0, 3, 12, 1};
    for (int choice : {0, 1, 2, 3, 0}) {
        complete(f.owner->reactionRateClicked(f.binding(), choice));
        CHECK_EQ(live.ndim, 0); CHECK_EQ(temporary.ndim, 1);
        CHECK_EQ(live.var[0], variables[choice]); CHECK_EQ(temporary.var[0], variables[choice]);
        CHECK(live.values == values); CHECK(temporary.values == temporaryValues);
        CHECK_EQ(live.n[0], 2); CHECK_EQ(temporary.n[0], 2);
    }
}

TEST_CASE(reaction_repeated_explicit_click_rewrites_live_and_temporary_values)
{
    Fixture f; auto& session = f.open(); auto& live = f.operation.engineModel->simCtl()->acvcof;
    auto& temporary = *session.environmentCurves.curves[3];
    complete(f.owner->reactionRateClicked(f.binding(), 2));
    live.var[0] = 99; temporary.var[0] = 98; session.environmentCurves.currentCurveIndex = 0;
    complete(f.owner->reactionRateClicked(f.binding(), 2));
    CHECK_EQ(live.var[0], 12); CHECK_EQ(temporary.var[0], 12);
    CHECK_EQ(*session.environmentCurves.currentCurveIndex, 3);
}

TEST_CASE(reaction_missing_or_replaced_sc_keeps_controls_before_guard_without_curve_store)
{
    for (bool replace : {false, true}) {
        Fixture f; auto& session = f.open(); auto& page = session.pages.environment;
        auto& temporary = *session.environmentCurves.curves[3]; table(temporary);
        temporary.var[0] = 77; temporary.ndim = 4;
        const auto* temporaryAxis = temporary.axis[0];
        const auto* temporaryValues = temporary.values;
        session.environmentCurves.currentCurveIndex = 1;
        auto oldStep = std::move(f.operation.engineModel->step);
        auto& oldLive = oldStep->sim_ctrl.acvcof; table(oldLive);
        oldLive.ndim = 5; oldLive.var[0] = 66;
        const auto* oldAxis = oldLive.axis[0]; const auto* oldValues = oldLive.values;
        if (replace) f.operation.engineModel->step = std::make_unique<def::model::DB_STEP_DATA>(3);
        auto* replacement = f.operation.engineModel->simCtl();
        if (replacement) { table(replacement->acvcof); replacement->acvcof.ndim = 6;
            replacement->acvcof.var[0] = 55; }
        const auto* replacementAxis = replacement ? replacement->acvcof.axis[0] : nullptr;
        const auto* replacementValues = replacement ? replacement->acvcof.values : nullptr;
        const auto report = f.owner->reactionRateClicked(f.binding(), 3);
        projectedRefusal(report); controls(page.acvcofControls, 3);
        CHECK_EQ(*session.environmentCurves.currentCurveIndex, 1);
        CHECK_EQ(temporary.var[0], 77); CHECK_EQ(temporary.ndim, 4);
        CHECK(temporary.axis[0] == temporaryAxis); CHECK(temporary.values == temporaryValues);
        CHECK_EQ(oldLive.var[0], 66); CHECK_EQ(oldLive.ndim, 5);
        CHECK(oldLive.axis[0] == oldAxis); CHECK(oldLive.values == oldValues);
        if (replacement) {
            CHECK_EQ(replacement->acvcof.var[0], 55); CHECK_EQ(replacement->acvcof.ndim, 6);
            CHECK(replacement->acvcof.axis[0] == replacementAxis);
            CHECK(replacement->acvcof.values == replacementValues);
        }
    }
}

TEST_CASE(reaction_invalid_choice_or_binding_refuses_before_control_projection)
{
    Fixture f; auto& session = f.open(); Project other;
    for (int choice : {-1, 4}) {
        preflight(f.owner->reactionRateClicked(f.binding(), choice));
        controls(session.pages.environment.acvcofControls, 0);
    }
    preflight(f.owner->reactionRateClicked({other, f.operation, *f.operation.engineModel, f.remesh}, 1));
    controls(session.pages.environment.acvcofControls, 0);
}

TEST_CASE(reaction_changed_temporary_or_page_alias_refuses_before_control_projection)
{
    for (int variant = 0; variant < 3; ++variant) {
        Fixture f; auto& session = f.open();
        if (variant == 0) session.environmentCurves.curves[3].reset();
        if (variant == 1) session.pages.environment.curves[3] = session.environmentCurves.curves[0];
        if (variant == 2) {
            session.environmentCurves.curves[3] = session.environmentCurves.curves[0];
            session.pages.environment.curves[3] = session.environmentCurves.curves[3];
        }
        preflight(f.owner->reactionRateClicked(f.binding(), 1));
        controls(session.pages.environment.acvcofControls, 0);
    }
}

TEST_CASE(reaction_live_curve_alias_guard_retains_only_prior_controls)
{
    Fixture f; auto& session = f.open(); auto model = f.operation.engineModel;
    auto& live = model->simCtl()->acvcof; table(live);
    live.ndim = 5; live.var[0] = 99;
    const auto* axis = live.axis[0]; const auto* values = live.values;
    // Alias the actual model lease, never create a second deleting owner.
    session.environmentCurves.curves[3] = std::shared_ptr<def::model::DYNARY>(model, &live);
    session.pages.environment.curves[3] = session.environmentCurves.curves[3];
    session.environmentCurves.currentCurveIndex = 1;
    const auto report = f.owner->reactionRateClicked(f.binding(), 2);
    projectedRefusal(report);
    controls(session.pages.environment.acvcofControls, 2);
    CHECK_EQ(live.var[0], 99); CHECK_EQ(live.ndim, 5);
    CHECK(live.axis[0] == axis); CHECK(live.values == values);
    CHECK_EQ(*session.environmentCurves.currentCurveIndex, 1);
}

TEST_CASE(reaction_partial_unconstructed_or_expired_opi_cannot_advance_public_factory)
{
    Fixture f; preflight(f.owner->reactionRateClicked(f.binding(), 1)); CHECK(!f.owner->partialPageState());
    auto context = f.context(); context.moObservations.reset();
    f.owner->constructPageState(f.binding(), f.io, context);
    preflight(f.owner->reactionRateClicked(f.binding(), 1)); CHECK(!f.owner->pageState());
    Fixture live; auto& session = live.open();
    REQUIRE(live.operation.commitOperationInfoQObjectBoundary(
        "declared private reaction fixture OPI boundary; not native destruction"));
    preflight(live.owner->reactionRateClicked(live.binding(), 2));
    controls(session.pages.environment.acvcofControls, 0);
    CHECK(!StagedEditor300::completeForPublicSave);
}

int main() { return testkit::run_all(); }
