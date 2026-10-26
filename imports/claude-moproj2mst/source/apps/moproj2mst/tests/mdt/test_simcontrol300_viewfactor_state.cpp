// ctest-labels: m2m-mdt
// Explicit private checkbox event; no native QWidget/dialog/Session claim.
#include <testkit/check.hpp>
#include <algorithm>
#include <memory>
#include <optional>
#include <utility>
#include "m2m/engine/simcontrol300_owner.hpp"
#include "m2m/mst/writer.hpp"
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
        sc.vfactr_mode = 0; sc.vfactr.value = 17.5; sc.vfactr_n = 7;
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
        auto* pages = owner->pageState(); REQUIRE(pages); REQUIRE(pages->viewFactorCurve);
        return *pages;
    }
};
}

TEST_CASE(viewfactor_state_only_projects_button_and_preserves_actual_model_and_owner)
{
    for (int state : {0, 2}) {
        Fixture f; auto& session = f.open(); auto& form = session.pages.environment;
        auto& sc = *f.operation.engineModel->simCtl(); const auto* owner = session.viewFactorCurve.get();
        const int reads = f.io.reads; const auto curveIndex = session.environmentCurves.currentCurveIndex;
        sc.vfactr_mode = 9; sc.vfactr.value = 99; sc.vfactr_n = 88;
        form.viewFactorChecked = state != 0; form.viewFactorDefinitionEnabled = state == 0;
        form.scalars[2] = {qt::L("UNSAVED INVALID"), ValidatorState::Invalid};
        CHECK(f.owner->viewFactorStateChanged(f.binding(), state) == ApplyResult::Completed);
        CHECK(form.viewFactorDefinitionEnabled == (state != 0)); CHECK(form.viewFactorChecked == (state != 0));
        CHECK_EQ(sc.vfactr_mode, 9); CHECK(sc.vfactr.value == 99); CHECK_EQ(sc.vfactr_n, 88);
        CHECK(session.viewFactorCurve.get() == owner); CHECK(owner->value == 17.5);
        CHECK_EQ(session.viewFactorDefinition, 7); CHECK(session.environmentCurves.currentCurveIndex == curveIndex);
        CHECK(form.scalars[2].text == qt::L("UNSAVED INVALID")); CHECK_EQ(session.pages.stepCount.steps, 23);
        CHECK_EQ(f.operation.nstartGiven, 0); CHECK_EQ(f.io.reads, reads);
        CHECK_EQ(f.io.writes, 0); CHECK_EQ(f.io.removes, 0);
    }
}

TEST_CASE(viewfactor_cached_assignment_is_not_delivery_and_same_state_reasserts_enablement)
{
    Fixture f; auto& form = f.open().pages.environment;
    REQUIRE(!form.viewFactorChecked); REQUIRE(!form.viewFactorDefinitionEnabled);
    form.viewFactorChecked = true;
    CHECK(!form.viewFactorDefinitionEnabled);
    CHECK(f.owner->viewFactorStateChanged(f.binding(), 2) == ApplyResult::Completed);
    CHECK(form.viewFactorDefinitionEnabled);
    form.viewFactorDefinitionEnabled = false;
    CHECK(f.owner->viewFactorStateChanged(f.binding(), 2) == ApplyResult::Completed);
    CHECK(form.viewFactorDefinitionEnabled);
}

TEST_CASE(viewfactor_state_has_no_current_sc_dimension_or_curve_payload_prerequisite)
{
    for (bool replace : {false, true}) {
        Fixture f; auto& session = f.open(); auto& form = session.pages.environment;
        form.viewFactorChecked = true; session.viewFactorCurve.reset();
        auto oldStep = std::move(f.operation.engineModel->step);
        oldStep->sim_ctrl.vfactr_mode = 9;
        if (replace) f.operation.engineModel->step = std::make_unique<def::model::DB_STEP_DATA>(3);
        auto* replacement = f.operation.engineModel->simCtl();
        if (replacement) replacement->vfactr_mode = 8;
        CHECK(f.owner->viewFactorStateChanged(f.binding(), 2) == ApplyResult::Completed);
        CHECK(form.viewFactorDefinitionEnabled); CHECK(!session.viewFactorCurve);
        CHECK_EQ(oldStep->sim_ctrl.vfactr_mode, 9);
        if (replacement) CHECK_EQ(replacement->vfactr_mode, 8);
    }
}

TEST_CASE(viewfactor_uncheck_defers_mode_store_and_preserves_live_curve_during_apply)
{
    Fixture f; auto& session = f.open(); auto& form = session.pages.environment;
    auto& sc = *f.operation.engineModel->simCtl();
    sc.vfactr_mode = 9; sc.vfactr.value = 99; sc.vfactr_n = 88;
    form.viewFactorChecked = false; form.viewFactorDefinitionEnabled = true;
    CHECK(f.owner->viewFactorStateChanged(f.binding(), 0) == ApplyResult::Completed);
    CHECK(!form.viewFactorDefinitionEnabled); CHECK_EQ(sc.vfactr_mode, 9);
    CHECK(applyEnvironment3D(sc, form, std::nullopt, &session.environmentCurves) == ApplyResult::Completed);
    CHECK_EQ(sc.vfactr_mode, 0); CHECK(sc.vfactr.value == 99); CHECK_EQ(sc.vfactr_n, 88);
    CHECK(session.viewFactorCurve->value == 17.5); CHECK_EQ(session.viewFactorDefinition, 7);
}

TEST_CASE(viewfactor_check_defers_actual_retained_curve_copy_until_apply)
{
    Fixture f; auto& session = f.open(); auto& form = session.pages.environment;
    auto& model = *f.operation.engineModel; auto& sc = *model.simCtl();
    sc.vfactr_mode = 9; sc.vfactr.value = 99; sc.vfactr_n = 88;
    form.viewFactorChecked = true;
    CHECK(f.owner->viewFactorStateChanged(f.binding(), 2) == ApplyResult::Completed);
    CHECK(form.viewFactorDefinitionEnabled); CHECK_EQ(sc.vfactr_mode, 9);
    CHECK(sc.vfactr.value == 99); CHECK_EQ(sc.vfactr_n, 88);
    const EnvironmentViewFactorState actual{model.step->dimension,
        session.viewFactorCurve.get(), session.viewFactorDefinition};
    CHECK(applyEnvironment3D(sc, form, actual, &session.environmentCurves) == ApplyResult::Completed);
    CHECK_EQ(sc.vfactr_mode, 1); CHECK(sc.vfactr.value == 17.5); CHECK_EQ(sc.vfactr_n, 7);
    CHECK(session.viewFactorCurve->value == 17.5);
}

TEST_CASE(viewfactor_missing_apply_context_retains_reached_scalar_and_mode_stores)
{
    Fixture f; auto& session = f.open(); auto& form = session.pages.environment;
    auto& sc = *f.operation.engineModel->simCtl();
    sc.ehrate = 9; sc.vfactr_mode = 9; sc.vfactr.value = 99; sc.vfactr_n = 88;
    form.scalars[2] = {qt::L("41"), ValidatorState::Acceptable}; form.viewFactorChecked = true;
    CHECK(f.owner->viewFactorStateChanged(f.binding(), 2) == ApplyResult::Completed);
    CHECK(sc.ehrate == 9); CHECK_EQ(sc.vfactr_mode, 9);
    CHECK(applyEnvironment3D(sc, form, std::nullopt, &session.environmentCurves) == ApplyResult::UnsupportedSnapshot);
    CHECK(sc.ehrate == 41); CHECK_EQ(sc.vfactr_mode, 1);
    CHECK(sc.vfactr.value == 99); CHECK_EQ(sc.vfactr_n, 88);
    CHECK(form.viewFactorDefinitionEnabled); CHECK(session.viewFactorCurve->value == 17.5);
}

TEST_CASE(viewfactor_excluded_state_mismatched_cache_and_binding_guard_before_projection)
{
    Fixture f; auto& form = f.open().pages.environment;
    form.viewFactorChecked = true; form.viewFactorDefinitionEnabled = false;
    for (int state : {-1, 1, 3}) {
        CHECK(f.owner->viewFactorStateChanged(f.binding(), state) == ApplyResult::UnsupportedSnapshot);
        CHECK(!form.viewFactorDefinitionEnabled);
    }
    CHECK(f.owner->viewFactorStateChanged(f.binding(), 0) == ApplyResult::UnsupportedSnapshot);
    form.viewFactorChecked = false;
    CHECK(f.owner->viewFactorStateChanged(f.binding(), 2) == ApplyResult::UnsupportedSnapshot);
    Project other;
    CHECK(f.owner->viewFactorStateChanged({other, f.operation, *f.operation.engineModel, f.remesh}, 0)
        == ApplyResult::UnsupportedSnapshot);
    CHECK(!form.viewFactorDefinitionEnabled);
}

TEST_CASE(viewfactor_unconstructed_partial_and_expired_opi_cannot_bypass_public_guard)
{
    Fixture f;
    CHECK(f.owner->viewFactorStateChanged(f.binding(), 0) == ApplyResult::UnsupportedSnapshot);
    CHECK(!f.owner->partialPageState());
    auto context = f.context(); context.moObservations.reset();
    f.owner->constructPageState(f.binding(), f.io, context);
    CHECK(f.owner->viewFactorStateChanged(f.binding(), 0) == ApplyResult::UnsupportedSnapshot);
    CHECK(!f.owner->pageState());
    Fixture live; auto& form = live.open().pages.environment;
    form.viewFactorChecked = true; form.viewFactorDefinitionEnabled = false;
    REQUIRE(live.operation.commitOperationInfoQObjectBoundary(
        "declared private view-factor fixture OPI boundary; not native destruction"));
    CHECK(live.owner->viewFactorStateChanged(live.binding(), 2) == ApplyResult::UnsupportedSnapshot);
    CHECK(!form.viewFactorDefinitionEnabled); CHECK(!StagedEditor300::completeForPublicSave);
}

int main() { return testkit::run_all(); }
