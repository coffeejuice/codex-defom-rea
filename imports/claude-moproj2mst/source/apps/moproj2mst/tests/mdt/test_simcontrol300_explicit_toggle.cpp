// ctest-labels: m2m-mdt
// Declared private toggle; visibility is a common setter-argument projection.
#include <testkit/check.hpp>
#include <algorithm>
#include <array>
#include <bit>
#include <cstdint>
#include <memory>
#include <optional>
#include <utility>
#include "m2m/engine/simcontrol300_owner.hpp"
#include "m2m/mst/writer.hpp"
#include <defmodel/layout/db_step_data.hpp>

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
std::array<std::uint64_t, 4> parameterBits(const SimControl& sc)
{
    std::array<std::uint64_t, 4> result;
    for (int i = 0; i < 4; ++i) result[i] = std::bit_cast<std::uint64_t>(sc.solmtd_p[i]);
    return result;
}
}

TEST_CASE(explicit_visibility_fill_is_known_hidden_for_all_currently_admitted_solvers)
{
    for (int solver : {0, 1, 2, 4, 5}) {
        Fixture f; f.operation.engineModel->simCtl()->solmtd = solver;
        const auto& form = f.open().pages.iteration;
        CHECK_EQ(form.solver, solver); REQUIRE(form.explicitControlsVisible);
        CHECK(!*form.explicitControlsVisible);
    }
}

TEST_CASE(explicit_toggle_projects_only_visibility_without_reading_or_normalizing_parameters)
{
    Fixture f; auto& session = f.open(); auto& form = session.pages.iteration;
    auto& sc = *f.operation.engineModel->simCtl();
    sc.solmtd = 5; sc.u2e8 = 77;
    sc.solmtd_p[0] = std::bit_cast<double>(std::uint64_t{0x7ff8000000000042});
    sc.solmtd_p[1] = std::bit_cast<double>(std::uint64_t{0x7ff0000000000000});
    sc.solmtd_p[2] = -0.0; sc.solmtd_p[3] = 17.25;
    const auto before = parameterBits(sc); const int reads = f.io.reads;
    form.cgSecond = qt::L("UNSAVED CG TEXT"); form.cgThird = qt::L("ALSO UNSAVED");
    const auto title = session.pages.general.title; const int thermal = form.thermalSolver;
    form.solver = 6;
    CHECK(f.owner->explicitSolverToggled(f.binding(), true) == ApplyResult::Completed);
    REQUIRE(form.explicitControlsVisible); CHECK(*form.explicitControlsVisible);
    CHECK_EQ(form.solver, 6); CHECK_EQ(sc.solmtd, 5); CHECK_EQ(sc.u2e8, 77);
    CHECK(parameterBits(sc) == before);
    form.solver = 4;
    CHECK(f.owner->explicitSolverToggled(f.binding(), false) == ApplyResult::Completed);
    CHECK(!*form.explicitControlsVisible); CHECK(parameterBits(sc) == before);
    CHECK_EQ(sc.solmtd, 5); CHECK_EQ(sc.u2e8, 77); CHECK_EQ(form.thermalSolver, thermal);
    CHECK(form.cgSecond == qt::L("UNSAVED CG TEXT")); CHECK(form.cgThird == qt::L("ALSO UNSAVED"));
    CHECK(session.pages.general.title == title); CHECK_EQ(session.pages.stepCount.steps, 23);
    CHECK_EQ(f.operation.nstartGiven, 0); CHECK_EQ(f.io.reads, reads);
    CHECK_EQ(f.io.writes, 0); CHECK_EQ(f.io.removes, 0);
}

TEST_CASE(explicit_cache_change_is_not_delivery_and_repeated_bool_reasserts_projection)
{
    Fixture f; auto& form = f.open().pages.iteration;
    REQUIRE(form.explicitControlsVisible); CHECK(!*form.explicitControlsVisible);
    form.solver = 6; CHECK(!*form.explicitControlsVisible);
    CHECK(f.owner->explicitSolverToggled(f.binding(), true) == ApplyResult::Completed);
    CHECK(*form.explicitControlsVisible);
    form.explicitControlsVisible.reset();
    CHECK(f.owner->explicitSolverToggled(f.binding(), true) == ApplyResult::Completed);
    REQUIRE(form.explicitControlsVisible); CHECK(*form.explicitControlsVisible);
    form.solver = 1;
    CHECK(f.owner->explicitSolverToggled(f.binding(), false) == ApplyResult::Completed);
    form.explicitControlsVisible = true;
    CHECK(f.owner->explicitSolverToggled(f.binding(), false) == ApplyResult::Completed);
    CHECK(!*form.explicitControlsVisible);
}

TEST_CASE(explicit_toggle_does_not_require_current_model_step_or_parameter_owner)
{
    for (bool replace : {false, true}) {
        Fixture f; auto& form = f.open().pages.iteration; form.solver = 6;
        auto oldStep = std::move(f.operation.engineModel->step); oldStep->sim_ctrl.u2e8 = 77;
        if (replace) f.operation.engineModel->step = std::make_unique<def::model::DB_STEP_DATA>(3);
        auto* replacement = f.operation.engineModel->simCtl();
        if (replacement) replacement->u2e8 = 88;
        CHECK(f.owner->explicitSolverToggled(f.binding(), true) == ApplyResult::Completed);
        REQUIRE(form.explicitControlsVisible); CHECK(*form.explicitControlsVisible);
        CHECK_EQ(oldStep->sim_ctrl.u2e8, 77);
        if (replacement) CHECK_EQ(replacement->u2e8, 88);
    }
}

TEST_CASE(explicit_false_toggle_accepts_each_qualified_nonexplicit_selected_radio)
{
    Fixture f; auto& form = f.open().pages.iteration; auto& sc = *f.operation.engineModel->simCtl();
    sc.solmtd = 6; sc.u2e8 = 77;
    for (int solver : {0, 1, 2, 4, 5}) {
        form.solver = solver; form.explicitControlsVisible = true;
        CHECK(f.owner->explicitSolverToggled(f.binding(), false) == ApplyResult::Completed);
        REQUIRE(form.explicitControlsVisible); CHECK(!*form.explicitControlsVisible);
        CHECK_EQ(sc.solmtd, 6); CHECK_EQ(sc.u2e8, 77);
    }
}

TEST_CASE(explicit_mismatched_radio_or_binding_refuses_before_visibility_projection)
{
    Fixture f; auto& form = f.open().pages.iteration;
    const std::array<std::pair<int, bool>, 6> rejected{
        std::pair{0, true}, std::pair{6, false}, std::pair{3, false},
        std::pair{3, true}, std::pair{7, false}, std::pair{-1, false}};
    for (const auto choice : rejected) {
        form.solver = choice.first; form.explicitControlsVisible.reset();
        CHECK(f.owner->explicitSolverToggled(f.binding(), choice.second) == ApplyResult::UnsupportedSnapshot);
        CHECK(!form.explicitControlsVisible);
    }
    form.solver = 6; Project other;
    CHECK(f.owner->explicitSolverToggled({other, f.operation, *f.operation.engineModel, f.remesh}, true)
        == ApplyResult::UnsupportedSnapshot);
    CHECK(!form.explicitControlsVisible);
}

TEST_CASE(explicit_visibility_does_not_enable_unknown_solver_six_fill_or_apply)
{
    Fixture f; auto& form = f.open().pages.iteration; auto& sc = *f.operation.engineModel->simCtl();
    sc.solmtd = 5; sc.u2e8 = 77;
    for (int i = 0; i < 4; ++i) sc.solmtd_p[i] = i + 0.25;
    const auto before = parameterBits(sc);
    form.solver = 6;
    CHECK(f.owner->explicitSolverToggled(f.binding(), true) == ApplyResult::Completed);
    CHECK(applyIteration3D(sc, form) == ApplyResult::UnsupportedSnapshot);
    CHECK_EQ(sc.solmtd, 5); CHECK_EQ(sc.u2e8, 77); CHECK(parameterBits(sc) == before);
    sc.solmtd = 6;
    CHECK(!fillIteration3D(sc, 3, NumericLocale::KnownC));
    CHECK_EQ(sc.solmtd, 6); CHECK_EQ(sc.u2e8, 77); CHECK(parameterBits(sc) == before);
    CHECK(!StagedEditor300::completeForPublicSave);
}

TEST_CASE(explicit_unconstructed_partial_and_expired_opi_remain_guarded)
{
    Fixture f;
    CHECK(f.owner->explicitSolverToggled(f.binding(), false) == ApplyResult::UnsupportedSnapshot);
    CHECK(!f.owner->partialPageState());
    auto context = f.context(); context.moObservations.reset();
    f.owner->constructPageState(f.binding(), f.io, context);
    CHECK(f.owner->explicitSolverToggled(f.binding(), false) == ApplyResult::UnsupportedSnapshot);
    CHECK(!f.owner->pageState());
    Fixture live; auto& form = live.open().pages.iteration;
    form.solver = 6; form.explicitControlsVisible.reset();
    REQUIRE(live.operation.commitOperationInfoQObjectBoundary(
        "declared private explicit-toggle fixture OPI boundary; not native destruction"));
    CHECK(live.owner->explicitSolverToggled(live.binding(), true) == ApplyResult::UnsupportedSnapshot);
    CHECK(!form.explicitControlsVisible); CHECK(!StagedEditor300::completeForPublicSave);
}

int main() { return testkit::run_all(); }
