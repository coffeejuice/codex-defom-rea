// ctest-labels: m2m-mdt
// Private bound page projection; not a native-widget or public Session test.
#include <testkit/check.hpp>
#include <algorithm>
#include <limits>
#include <memory>
#include <optional>
#include <utility>
#include "m2m/engine/simcontrol300_owner.hpp"
#include "m2m/mst/writer.hpp"
#include <defmodel/layout/db_step_data.hpp>

using namespace m2m;
using namespace m2m::engine::simcontrol300;
namespace {
struct NoFiles final : Extra3DIo {
    NoFiles() : Extra3DIo(1252) {}
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
struct StartClass final : mst::OpClass {
    StartClass(qt::String key, bool writes) : OpClass(std::move(key)), writes(writes) {}
    bool writes;
    bool writesDimNstart() const override { return writes; }
};
struct Fixture {
    Project project;
    Operation operation;
    RemeshState remesh;
    NoFiles io;
    std::unique_ptr<StagedEditor300> owner;
    Fixture()
    {
        project.subs.push_back(std::make_unique<SubProject>());
        auto& sub = *project.subs[0]; sub.project = &project; project.activeSub = 0;
        auto previous = std::make_unique<FlowItem>(); previous->opId = 2;
        auto current = std::make_unique<FlowItem>(); current->opId = 7;
        previous->next = current.get(); current->prev = previous.get();
        sub.flow.items.push_back(std::move(previous)); sub.flow.items.push_back(std::move(current));
        operation.project = &project; operation.id = 7;
        operation.engineModel = std::make_shared<engine::GuiModel>(nullptr, 3);
        setClass(true);
        auto& sc = *operation.engineModel->simCtl();
        sc.unit = 1; sc.nstart = -7; sc.nstep = 23; sc.stpinc = 3; sc.pdie1 = 7;
        sc.trans[0] = sc.trans[1] = 1;
        operation.startStepNo = 10;
        owner = std::make_unique<StagedEditor300>(binding());
    }
    void setClass(bool writes)
    {
        const auto key = qt::L(writes ? "ToggleStartClass" : "ToggleNoStartClass");
        if (!project.registry.contains(key))
            project.registry[key] = std::make_unique<StartClass>(key, writes);
        operation.classKey = key; operation.opClass = project.registry[key].get();
    }
    Editor300Binding binding() { return {project, operation, *operation.engineModel, remesh}; }
    StepCountContext stepContext() { return {5, {2, 7}, MoStepCountContext{&operation}}; }
    InitialEditorContext context()
    {
        MoInitialObservations features;
        features.generalFills[0] = MoFeatureAvailability{1, std::nullopt, {1, 1, 1}};
        features.generalFills[1] = MoFeatureAvailability{1, std::nullopt, {1, 1, 1}};
        features.startupHeating = {0, 1};
        return {generalContextForMo(project, 3, {true, true}), stepContext(),
            NumericLocale::KnownC, true, features};
    }
    InitialEditorSession& open()
    {
        REQUIRE(!startsFromKey(operation));
        owner->constructPageState(binding(), io, context());
        auto* pages = owner->pageState(); REQUIRE(pages); return *pages;
    }
};
void completed(const StepCountToggleReport& report)
{
    CHECK(report.outcome == ApplyResult::Completed);
    CHECK(report.stage == StepCountToggleStage::Complete);
    CHECK(report.operationFlagStored); CHECK(report.pageRefilled);
}
void refusedBeforeStore(const StepCountToggleReport& report)
{
    CHECK(report.outcome == ApplyResult::UnsupportedSnapshot);
    CHECK(report.stage == StepCountToggleStage::Preflight);
    CHECK(!report.operationFlagStored); CHECK(!report.pageRefilled);
}
StepCountPage sentinel(Operation& operation)
{
    return {101, 102, 103, 104, MoStepCountContext{&operation}, true};
}
void unchanged(const StepCountPage& page, const Operation& operation)
{
    CHECK_EQ(page.start, 101); CHECK_EQ(page.steps, 102);
    CHECK_EQ(page.increment, 103); CHECK_EQ(page.primaryDie, 104);
    REQUIRE(page.mo); CHECK(page.mo->operation == &operation);
    CHECK(page.nstartChecked);
}
}

TEST_CASE(nstart_model_refill_matches_current_control_and_publishes_only_complete_page)
{
    Fixture f; auto& model = *f.operation.engineModel;
    auto* current = model.simCtl(); REQUIRE(current);
    auto page = sentinel(f.operation);
    auto context = f.stepContext();
    CHECK(!model.refillEditorStepCountIfControlMatches(nullptr, context, false, page));
    unchanged(page, f.operation);
    auto oldStep = std::move(model.step);
    model.step = std::make_unique<def::model::DB_STEP_DATA>(3);
    CHECK(!model.refillEditorStepCountIfControlMatches(current, context, false, page));
    unchanged(page, f.operation);
    model.step.reset();
    CHECK(!model.refillEditorStepCountIfControlMatches(nullptr, context, false, page));
    CHECK(!model.refillEditorStepCountIfControlMatches(current, context, false, page));
    unchanged(page, f.operation);
    model.step = std::move(oldStep);
    context.applicationMode = 0;
    CHECK(!model.refillEditorStepCountIfControlMatches(current, context, false, page));
    unchanged(page, f.operation);
    context = f.stepContext();
    f.operation.nstartGiven = 1;
    CHECK(model.refillEditorStepCountIfControlMatches(current, context, true, page));
    CHECK_EQ(page.start, 6); CHECK_EQ(page.steps, 23);
    CHECK_EQ(page.increment, 3); CHECK_EQ(page.primaryDie, 7);
    REQUIRE(page.mo); CHECK(page.mo->operation == &f.operation);
    CHECK(page.nstartChecked);
    CHECK_EQ(current->nstart, -7); CHECK_EQ(current->nstep, 23);
    CHECK_EQ(current->stpinc, 3); CHECK_EQ(current->pdie1, 7);
    CHECK_EQ(f.operation.startStepNo, 10);
    CHECK_EQ(f.operation.nstartGiven, 1);
}

TEST_CASE(nstart_model_refill_preserves_prior_checkbox_for_key_and_null_operation)
{
    Fixture f; auto& model = *f.operation.engineModel;
    auto* current = model.simCtl(); REQUIRE(current);
    auto context = f.stepContext();
    auto page = sentinel(f.operation);
    f.operation.restartGiven = 1; REQUIRE(startsFromKey(f.operation));
    for (bool prior : {false, true}) {
        CHECK(model.refillEditorStepCountIfControlMatches(current, context, prior, page));
        CHECK_EQ(page.nstartChecked, prior); CHECK_EQ(page.start, -7);
    }
    context.mo->operation = nullptr;
    for (bool prior : {false, true}) {
        CHECK(model.refillEditorStepCountIfControlMatches(current, context, prior, page));
        CHECK_EQ(page.nstartChecked, prior); CHECK_EQ(page.start, -7);
    }
    context = f.stepContext();
    f.operation.restartGiven = 0; REQUIRE(!startsFromKey(f.operation));
    f.operation.nstartGiven = 1;
    f.setClass(true);
    for (bool prior : {false, true}) {
        CHECK(model.refillEditorStepCountIfControlMatches(current, context, prior, page));
        CHECK(page.nstartChecked); CHECK_EQ(page.start, 6);
    }
    f.setClass(false);
    CHECK(model.refillEditorStepCountIfControlMatches(current, context, true, page));
    CHECK(!page.nstartChecked); CHECK_EQ(page.start, -7);
}

TEST_CASE(nstart_toggle_stores_given_then_refills_all_current_controls_without_save)
{
    Fixture f; auto& session = f.open(); auto& page = session.pages.stepCount;
    auto& sc = *f.operation.engineModel->simCtl();
    page.nstartChecked = true; page.start = 500; page.steps = 501;
    page.increment = 502; page.primaryDie = 2;
    const int reads = f.io.reads; const auto title = session.pages.general.title;
    completed(f.owner->toggleNstart(f.binding(), f.stepContext(), true));
    CHECK_EQ(f.operation.nstartGiven, 1); CHECK_EQ(f.operation.startStepNo, 10);
    CHECK_EQ(page.start, 6); CHECK_EQ(page.steps, 23); CHECK_EQ(page.increment, 3);
    CHECK_EQ(page.primaryDie, 7); CHECK(page.nstartChecked);
    CHECK_EQ(sc.nstart, -7); CHECK_EQ(sc.nstep, 23); CHECK_EQ(sc.stpinc, 3); CHECK_EQ(sc.pdie1, 7);
    page.nstartChecked = false;
    completed(f.owner->toggleNstart(f.binding(), f.stepContext(), false));
    CHECK_EQ(f.operation.nstartGiven, 0); CHECK_EQ(page.start, -7); CHECK(!page.nstartChecked);
    CHECK_EQ(f.operation.startStepNo, 10); CHECK(session.pages.general.title == title);
    CHECK_EQ(f.io.reads, reads); CHECK_EQ(f.io.writes, 0); CHECK_EQ(f.io.removes, 0);
    CHECK(!f.owner->completeForPublicSave);
    CHECK(f.owner->construction().state == Editor300ConstructorState::AwaitingNativeBoundaries);
}

TEST_CASE(nstart_repeated_delivered_value_refills_live_sc_wrap_clamps_and_primary_profile)
{
    Fixture f; auto& page = f.open().pages.stepCount; page.nstartChecked = true;
    completed(f.owner->toggleNstart(f.binding(), f.stepContext(), true));
    auto& sc = *f.operation.engineModel->simCtl();
    sc.nstart = (std::numeric_limits<int>::max)(); sc.nstep = -1; sc.stpinc = 0;
    auto current = f.stepContext(); current.primaryDieObjects = {2};
    completed(f.owner->toggleNstart(f.binding(), current, true));
    CHECK_EQ(page.start, -999999); CHECK_EQ(page.steps, 0); CHECK_EQ(page.increment, 1);
    CHECK_EQ(page.primaryDie, 0); CHECK_EQ(sc.nstart, (std::numeric_limits<int>::max)());
    sc.nstart = (std::numeric_limits<int>::min)(); sc.nstep = 2000000; sc.stpinc = 2000000;
    completed(f.owner->toggleNstart(f.binding(), current, true));
    CHECK_EQ(page.start, 999999); CHECK_EQ(page.steps, 999999); CHECK_EQ(page.increment, 999999);
}

TEST_CASE(nstart_hidden_key_checkbox_retains_event_history_but_fresh_fill_stays_unchecked)
{
    Fixture f; auto& page = f.open().pages.stepCount;
    f.operation.restartGiven = 1; REQUIRE(startsFromKey(f.operation));
    page.nstartChecked = true;
    completed(f.owner->toggleNstart(f.binding(), f.stepContext(), true));
    CHECK(page.nstartChecked); CHECK_EQ(page.start, -7); CHECK_EQ(f.operation.nstartGiven, 1);
    const auto fresh = fillStepCount(*f.operation.engineModel->simCtl(), f.stepContext());
    REQUIRE(fresh); CHECK(!fresh->nstartChecked);
    page.nstartChecked = false;
    completed(f.owner->toggleNstart(f.binding(), f.stepContext(), false));
    CHECK(!page.nstartChecked); CHECK_EQ(f.operation.nstartGiven, 0);
    CHECK_EQ(f.operation.startStepNo, 10);
}

TEST_CASE(nstart_nonwriting_or_missing_class_clears_checkbox_without_reversing_flag_store)
{
    for (bool missing : {false, true}) {
        Fixture f; auto& page = f.open().pages.stepCount;
        if (missing) { f.operation.classKey = qt::L("AbsentToggleClass"); f.operation.opClass = nullptr; }
        else f.setClass(false);
        page.nstartChecked = true;
        completed(f.owner->toggleNstart(f.binding(), f.stepContext(), true));
        CHECK_EQ(f.operation.nstartGiven, 1); CHECK(!page.nstartChecked); CHECK_EQ(page.start, -7);
        CHECK_EQ(f.operation.startStepNo, 10);
        CHECK_EQ(f.operation.engineModel->simCtl()->nstart, -7);
    }
}

TEST_CASE(nstart_missing_current_sc_retains_ordered_flag_store_without_stale_page_refill)
{
    Fixture f; auto& page = f.open().pages.stepCount;
    page.nstartChecked = true; page.start = 800; page.steps = 801;
    f.operation.engineModel->step.reset();
    const auto report = f.owner->toggleNstart(f.binding(), f.stepContext(), true);
    CHECK(report.outcome == ApplyResult::UnsupportedSnapshot);
    CHECK(report.stage == StepCountToggleStage::OperationFlagStored);
    CHECK(report.operationFlagStored); CHECK(!report.pageRefilled);
    CHECK_EQ(f.operation.nstartGiven, 1); CHECK_EQ(f.operation.startStepNo, 10);
    CHECK_EQ(page.start, 800); CHECK_EQ(page.steps, 801); CHECK(page.nstartChecked);
    CHECK(!f.owner->pageState());
}

TEST_CASE(nstart_replaced_current_sc_is_not_substituted_after_flag_store)
{
    Fixture f; auto& session = f.open(); auto& page = session.pages.stepCount;
    page.nstartChecked = true; page.start = 700;
    auto oldStep = std::move(f.operation.engineModel->step);
    f.operation.engineModel->step = std::make_unique<def::model::DB_STEP_DATA>(3);
    REQUIRE(f.operation.engineModel->simCtl() != session.sourceControl);
    const auto report = f.owner->toggleNstart(f.binding(), f.stepContext(), true);
    CHECK(report.stage == StepCountToggleStage::OperationFlagStored);
    CHECK(report.outcome == ApplyResult::UnsupportedSnapshot); CHECK(!report.pageRefilled);
    CHECK_EQ(f.operation.nstartGiven, 1); CHECK_EQ(page.start, 700);
    CHECK_EQ(oldStep->sim_ctrl.nstart, -7);
}

TEST_CASE(nstart_context_event_and_owner_mismatch_refuse_before_any_flag_store)
{
    for (int variant = 0; variant < 5; ++variant) {
        Fixture f; auto& page = f.open().pages.stepCount; page.nstartChecked = true;
        auto context = f.stepContext(); Operation other; Project otherProject;
        if (variant == 0) context.applicationMode = 1;
        if (variant == 1) context.mo.reset();
        if (variant == 2) context.mo->operation = &other;
        if (variant == 3) page.nstartChecked = false;
        auto binding = f.binding();
        const auto report = variant == 4
            ? f.owner->toggleNstart({otherProject, f.operation, *f.operation.engineModel, f.remesh}, context, true)
            : f.owner->toggleNstart(binding, context, true);
        refusedBeforeStore(report); CHECK_EQ(f.operation.nstartGiven, 0);
        CHECK_EQ(f.operation.engineModel->simCtl()->nstart, -7);
    }
}

TEST_CASE(nstart_unconstructed_partial_and_replaced_opi_owners_cannot_create_page_state)
{
    Fixture f;
    refusedBeforeStore(f.owner->toggleNstart(f.binding(), f.stepContext(), true));
    CHECK(!f.owner->partialPageState()); CHECK_EQ(f.operation.nstartGiven, 0);
    auto context = f.context(); context.moObservations.reset();
    f.owner->constructPageState(f.binding(), f.io, context);
    refusedBeforeStore(f.owner->toggleNstart(f.binding(), f.stepContext(), true));
    CHECK(!f.owner->pageState()); CHECK_EQ(f.operation.nstartGiven, 0);

    Project project; RemeshState remesh;
    auto model = std::make_shared<engine::GuiModel>(nullptr, 3);
    std::optional<Operation> operation(std::in_place);
    operation->project = &project; operation->engineModel = model;
    const auto binding = [&]() -> Editor300Binding { return {project, *operation, *model, remesh}; };
    StagedEditor300 owner(binding()); auto* address = &*operation;
    operation.reset(); operation.emplace(); operation->project = &project; operation->engineModel = model;
    CHECK(&*operation == address);
    refusedBeforeStore(owner.toggleNstart(binding(), {5, {}, MoStepCountContext{&*operation}}, true));
    CHECK_EQ(operation->nstartGiven, 0); CHECK(!owner.partialPageState());
    CHECK(!StagedEditor300::completeForPublicSave);
}

int main() { return testkit::run_all(); }
