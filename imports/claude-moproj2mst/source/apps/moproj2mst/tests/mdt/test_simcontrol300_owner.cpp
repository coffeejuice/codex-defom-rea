// ctest-labels: m2m-mdt
#include <testkit/check.hpp>
#include <algorithm>
#include <cstring>
#include <functional>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include "m2m/engine/simcontrol300_owner.hpp"
#include "m2m/mst/writer.hpp"
#include <defmodel/layout/db_step_data.hpp>

using namespace m2m;
using namespace m2m::engine::simcontrol300;
namespace {
struct Io final : Extra3DIo {
    Io() : Extra3DIo(1252) {}
    int reads = 0, removes = 0;
    std::function<void()> duringRead;
    bool readDoubles(const qt::String&, std::span<double> output) override
    {
        ++reads;
        std::fill(output.begin(), output.end(), 0.0);
        if (duringRead) { auto call = std::move(duringRead); duringRead = {}; call(); }
        return false;
    }
    bool readInts(const qt::String&, std::span<int> output) override
    { ++reads; std::fill(output.begin(), output.end(), 0); return false; }
    bool readSprings(std::vector<SpringRecord>& output) override
    { ++reads; output.clear(); return false; }
    bool removeIfReadable(const qt::String&) override { ++removes; return true; }
    void nbcRemoveWarning() override {}
    std::unique_ptr<mst::OutputDevice> openWrite(const qt::String&) override { return nullptr; }
};
struct Fixture {
    Project project;
    Operation operation;
    RemeshState remesh;
    Fixture()
    {
        operation.project = &project;
        operation.engineModel = std::make_shared<engine::GuiModel>(nullptr, 3);
        auto& sc = *operation.engineModel->simCtl();
        sc.unit = 1; sc.nstart = 7; sc.nstep = 23;
        sc.smode = 99; sc.trans[0] = sc.trans[1] = 1;
        std::strcpy(sc.title, "ORIGINAL TITLE");
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
};
template <std::size_t N> bool all(const std::array<bool, N>& values)
{ return std::all_of(values.begin(), values.end(), [](bool value) { return value; }); }
template <std::size_t N> bool none(const std::array<bool, N>& values)
{ return std::none_of(values.begin(), values.end(), [](bool value) { return value; }); }
}

TEST_CASE(editor300_factory_record_requires_real_binding_and_does_not_invent_context)
{
    Fixture f; auto& sc = *f.operation.engineModel->simCtl(); sc.solmtd = 2; sc.u2e8 = 0;
    StagedEditor300 owner(f.binding());
    CHECK(owner.identityMatches(f.binding()));
    CHECK(owner.construction().state == Editor300ConstructorState::AwaitingContext);
    CHECK(!owner.construction().pages); CHECK(!owner.pageState()); CHECK(!owner.partialPageState());
    CHECK_EQ(sc.smode, 99); CHECK_EQ(sc.u2e8, 0);
    CHECK(!StagedEditor300::completeForPublicSave);
    CHECK(!Editor300ConstructionReport::completeForPublicSave);
    Project otherProject; Operation otherOperation; RemeshState otherRemesh;
    engine::GuiModel otherModel(nullptr, 3);
    CHECK(!owner.identityMatches({otherProject, f.operation, *f.operation.engineModel, f.remesh}));
    CHECK(!owner.identityMatches({f.project, otherOperation, *f.operation.engineModel, f.remesh}));
    CHECK(!owner.identityMatches({f.project, f.operation, otherModel, f.remesh}));
    CHECK(!owner.identityMatches({f.project, f.operation, *f.operation.engineModel, otherRemesh}));
    StagedEditor300 mismatch({otherProject, f.operation, *f.operation.engineModel, f.remesh});
    CHECK(mismatch.construction().state == Editor300ConstructorState::InvalidBinding);
    CHECK(mismatch.construction().bindingIssue == Editor300BindingIssue::IdentityMismatch);
    CHECK(!mismatch.partialPageState());
}

TEST_CASE(editor300_constructor_rejects_substituted_context_before_model_or_file_effects)
{
    for (int replacement = 0; replacement < 4; ++replacement) {
        Fixture f; StagedEditor300 owner(f.binding()); Io io;
        Project otherProject; Operation otherOperation;
        auto context = f.context();
        if (replacement == 0) context.general.applicationMode = 1;
        if (replacement == 1) context.general.moProject = &otherProject;
        if (replacement == 2) context.stepCount.mo->operation = &otherOperation;
        if (replacement == 3) context.stepCount.mo.reset();
        const auto& report = owner.constructPageState(f.binding(), io, context);
        CHECK(report.state == Editor300ConstructorState::PageRefused);
        CHECK(report.bindingIssue == Editor300BindingIssue::ContextMismatch);
        CHECK(!report.pages); CHECK(!owner.partialPageState());
        CHECK_EQ(f.operation.engineModel->simCtl()->smode, 99); CHECK_EQ(io.reads, 0);
    }
}

TEST_CASE(editor300_missing_initial_observation_keeps_reached_guard_and_forbids_replay)
{
    Fixture f; StagedEditor300 owner(f.binding()); Io io;
    auto context = f.context(); context.moObservations.reset();
    const auto& report = owner.constructPageState(f.binding(), io, context);
    CHECK(report.state == Editor300ConstructorState::PageRefused); REQUIRE(report.pages);
    CHECK(report.pages->stage == InitialOpenStage::InitialFill);
    CHECK(report.pages->issue == InitialOpenIssue::MissingMoObservation);
    REQUIRE(report.pages->reachedPage); CHECK(*report.pages->reachedPage == Page::General);
    CHECK(none(report.pages->initialPages)); REQUIRE(owner.partialPageState());
    CHECK(owner.partialPageState()->sourceRemesh == &f.remesh);
    CHECK(!owner.partialPageState()->pageStateComplete); CHECK(!owner.pageState());
    CHECK_EQ(io.reads, 0); CHECK_EQ(f.operation.engineModel->simCtl()->smode, 99);
    owner.constructPageState(f.binding(), io, f.context());
    CHECK(report.repeatedConstructionRejected);
    CHECK(report.pages->issue == InitialOpenIssue::MissingMoObservation);
    CHECK(report.bindingIssue == Editor300BindingIssue::None);
    CHECK_EQ(io.reads, 0); CHECK_EQ(f.operation.engineModel->simCtl()->smode, 99);
}

TEST_CASE(editor300_environment_refusal_retains_earlier_iteration_and_widget_state)
{
    Fixture f; auto& sc = *f.operation.engineModel->simCtl(); sc.solmtd = 2; sc.u2e8 = 0;
    StagedEditor300 owner(f.binding()); Io io;
    auto context = f.context(); context.moObservations->startupHeating = {};
    const auto& report = owner.constructPageState(f.binding(), io, context);
    REQUIRE(report.pages); CHECK(report.state == Editor300ConstructorState::PageRefused);
    REQUIRE(report.pages->reachedPage); CHECK(*report.pages->reachedPage == Page::Environment);
    CHECK(report.pages->issue == InitialOpenIssue::MissingMoObservation);
    for (int i = 0; i < 6; ++i) CHECK(report.pages->initialPages[i]);
    for (int i = 6; i < 9; ++i) CHECK(!report.pages->initialPages[i]);
    CHECK(!report.pages->temporariesCopied); CHECK(!report.pages->backupCreated);
    REQUIRE(owner.partialPageState());
    CHECK(owner.partialPageState()->pages.general.title == qt::L("ORIGINAL TITLE"));
    CHECK_EQ(owner.partialPageState()->pages.iteration.solver, 2);
    CHECK_EQ(sc.u2e8, 4); CHECK(sc.solmtd_p[0] == 2.0); CHECK_EQ(sc.smode, 3);
    CHECK_EQ(io.reads, 0); CHECK(!owner.pageState());
}

TEST_CASE(editor300_second_observation_refusal_preserves_backup_and_full_initial_effects)
{
    Fixture f; StagedEditor300 owner(f.binding()); Io io;
    auto context = f.context(); context.moObservations->generalFills[1].reset();
    auto& sc = *f.operation.engineModel->simCtl();
    io.duringRead = [&] { sc.nstep = 99; };
    const auto& report = owner.constructPageState(f.binding(), io, context);
    REQUIRE(report.pages); CHECK(report.state == Editor300ConstructorState::PageRefused);
    CHECK(report.pages->stage == InitialOpenStage::BackedRefresh);
    REQUIRE(report.pages->reachedPage); CHECK(*report.pages->reachedPage == Page::General);
    CHECK(all(report.pages->initialPages)); CHECK(all(report.pages->repeatPages));
    CHECK(none(report.pages->refreshedPages)); CHECK(report.pages->temporariesCopied);
    CHECK(report.pages->backupCreated); CHECK(report.pages->backupRestored);
    CHECK(!report.pages->extra3DRefreshed);
    REQUIRE(owner.partialPageState()); REQUIRE(owner.partialPageState()->viewFactorCurve);
    CHECK_EQ(owner.partialPageState()->pages.stepCount.steps, 23); CHECK_EQ(sc.nstep, 99);
    CHECK(io.reads > 0); CHECK_EQ(io.removes, 0); CHECK(!owner.pageState());
}

TEST_CASE(editor300_completed_pages_remain_owned_but_do_not_complete_native_constructor)
{
    Fixture f; StagedEditor300 owner(f.binding()); Io io;
    const auto& report = owner.constructPageState(f.binding(), io, f.context());
    CHECK(report.state == Editor300ConstructorState::AwaitingNativeBoundaries); REQUIRE(report.pages);
    CHECK(report.pages->complete()); CHECK(all(report.pages->initialPages));
    CHECK(all(report.pages->repeatPages)); CHECK(all(report.pages->refreshedPages));
    CHECK(report.pages->backupCreated && report.pages->backupRestored && report.pages->extra3DRefreshed);
    auto* pages = owner.pageState(); REQUIRE(pages);
    const auto* curveOwner = pages->environmentCurves.curves[0].get();
    CHECK(pages->pages.environment.curves[0].get() == curveOwner);
    auto& sc = *f.operation.engineModel->simCtl();
    sc.nstep = 999; std::strcpy(sc.title, "LATER TITLE");
    const int reads = io.reads;
    owner.constructPageState(f.binding(), io, f.context());
    CHECK(report.repeatedConstructionRejected); CHECK_EQ(io.reads, reads);
    CHECK(owner.pageState() == pages); CHECK(pages->environmentCurves.curves[0].get() == curveOwner);
    std::vector<RecoveryEntry> empty;
    const auto applied = applyInitialEditorPageState(*pages, *f.operation.engineModel, f.remesh,
        io, &empty, &empty);
    CHECK(applied.outcome == ApplyResult::Completed);
    CHECK_EQ(sc.nstep, 23); CHECK(std::strcmp(sc.title, "ORIGINAL TITLE") == 0);
    CHECK(!InitialApplyReport::completeForPublicSave);
}

TEST_CASE(editor300_borrowed_model_guard_expires_without_extending_operation_model_lifetime)
{
    Fixture f; StagedEditor300 owner(f.binding()); Io io;
    owner.constructPageState(f.binding(), io, f.context()); REQUIRE(owner.pageState());
    std::weak_ptr<engine::GuiModel> old = f.operation.engineModel;
    f.operation.engineModel.reset();
    CHECK(old.expired()); CHECK(!owner.pageState()); REQUIRE(owner.partialPageState());
    f.operation.engineModel = std::make_shared<engine::GuiModel>(nullptr, 3);
    CHECK(!owner.identityMatches(f.binding()));
    CHECK(owner.construction().state == Editor300ConstructorState::AwaitingNativeBoundaries);
}

TEST_CASE(editor300_opi_qobject_expiry_refuses_continuation_with_live_model)
{
    Fixture f; StagedEditor300 owner(f.binding()); Io io;
    const auto model = f.operation.engineModel;
    const auto identity = f.operation.operationInfoIdentity().guard();
    const auto widget = owner.widgetState();
    REQUIRE(owner.identityMatches(f.binding()));
    // Declared identity-boundary fixture only: no native payload/Close replay.
    REQUIRE(f.operation.commitOperationInfoQObjectBoundary(
        "declared Editor300 fixture: OPI QObject boundary; not accepted native Close"));
    CHECK(identity.expiry() == engine::OperationInfoExpiry::QObjectBoundary);
    CHECK(!identity.get());
    CHECK(f.operation.engineModel == model);
    CHECK(!owner.identityMatches(f.binding()));
    EditorBaseContext base; base.receiver = &owner;
    CHECK(owner.continueNativeBase(f.binding(), base).issue == EditorBaseIssue::IdentityMismatch);
    EditorSetupState setup;
    CHECK(!owner.bindSetupOwner(setup));
    CHECK(!owner.setupWasBound());
    const auto& report = owner.constructPageState(f.binding(), io, f.context());
    CHECK(report.state == Editor300ConstructorState::InvalidBinding);
    CHECK(report.bindingIssue == Editor300BindingIssue::IdentityMismatch);
    CHECK(!owner.partialPageState());
    CHECK_EQ(io.reads, 0); CHECK_EQ(io.removes, 0);
    CHECK_EQ(model->simCtl()->smode, 99);
    CHECK(widget->nativeOwnerAlive());
    CHECK(!StagedEditor300::completeForPublicSave);
}

TEST_CASE(editor300_opi_expiry_hides_live_pages_but_retains_partial_diagnostics)
{
    Fixture f; StagedEditor300 owner(f.binding()); Io io;
    owner.constructPageState(f.binding(), io, f.context());
    REQUIRE(owner.pageState());
    const auto* const partial = owner.partialPageState();
    const auto model = f.operation.engineModel;
    const auto widget = owner.widgetState();
    const auto reads = io.reads, removes = io.removes;
    REQUIRE(f.operation.commitOperationInfoQObjectBoundary(
        "declared retained-page fixture: OPI QObject boundary; not native destruction"));
    CHECK(f.operation.engineModel == model);
    CHECK(!owner.identityMatches(f.binding()));
    CHECK(!owner.pageState());
    CHECK(owner.partialPageState() == partial);
    CHECK(owner.construction().state == Editor300ConstructorState::AwaitingNativeBoundaries);
    CHECK_EQ(io.reads, reads); CHECK_EQ(io.removes, removes);
    CHECK(widget->nativeOwnerAlive());
}

TEST_CASE(editor300_opi_reused_address_cannot_replace_original_generation)
{
    Project project; RemeshState remesh;
    const auto model = std::make_shared<engine::GuiModel>(nullptr, 3);
    std::optional<Operation> operation(std::in_place);
    operation->project = &project; operation->engineModel = model;
    const auto binding = [&]() -> Editor300Binding { return {project, *operation, *model, remesh}; };
    StagedEditor300 owner(binding());
    const auto oldIdentity = operation->operationInfoIdentity().guard();
    auto* const operationAddress = &*operation;
    REQUIRE(owner.identityMatches(binding()));
    // Optional reuses its contained storage and remains disengaged if the new
    // constructor throws. Retaining this model makes every raw binding match.
    operation.reset();
    operation.emplace();
    operation->project = &project; operation->engineModel = model;
    CHECK(&*operation == operationAddress);
    CHECK(oldIdentity.expiry() == engine::OperationInfoExpiry::AdapterIdentityLifetime);
    CHECK(!oldIdentity.get());
    CHECK(operation->operationInfoIdentity().get() == operationAddress);
    CHECK(!owner.identityMatches(binding()));
    EditorBaseContext base; base.receiver = &owner;
    CHECK(owner.continueNativeBase(binding(), base).issue == EditorBaseIssue::IdentityMismatch);
    CHECK(!owner.partialPageState());
    StagedEditor300 replacement(binding());
    CHECK(replacement.identityMatches(binding()));
}

TEST_CASE(editor300_diagnostic_observer_destruction_does_not_expire_opi)
{
    Fixture f;
    const auto identity = f.operation.operationInfoIdentity().guard();
    {
        StagedEditor300 owner(f.binding());
        CHECK(owner.identityMatches(f.binding()));
        CHECK(identity.expiry() == engine::OperationInfoExpiry::Live);
    }
    CHECK(identity.get() == &f.operation);
    CHECK(identity.expiry() == engine::OperationInfoExpiry::Live);
}

TEST_CASE(editor300_adapter_exception_preserves_reached_page_and_propagates_original_exception)
{
    Fixture f; StagedEditor300 owner(f.binding()); Io io;
    io.duringRead = [] { throw std::runtime_error("unrecovered reader branch"); };
    bool caught = false;
    try { owner.constructPageState(f.binding(), io, f.context()); }
    catch (const std::runtime_error& error) {
        caught = std::string(error.what()) == "unrecovered reader branch";
    }
    CHECK(caught); const auto& report = owner.construction();
    CHECK(report.state == Editor300ConstructorState::PageRefused); CHECK(report.exceptionInterrupted);
    REQUIRE(report.pages); CHECK(report.pages->stage == InitialOpenStage::InitialFill);
    REQUIRE(report.pages->reachedPage); CHECK(*report.pages->reachedPage == Page::Extra3D);
    for (int i = 0; i < 8; ++i) CHECK(report.pages->initialPages[i]);
    CHECK(!report.pages->initialPages[8]); CHECK(!report.pages->temporariesCopied);
    REQUIRE(owner.partialPageState()); CHECK(!owner.pageState());
    const int reads = io.reads; owner.constructPageState(f.binding(), io, f.context());
    CHECK_EQ(io.reads, reads); CHECK(report.repeatedConstructionRejected);
}

TEST_CASE(editor300_partial_lifecycle_owner_cannot_be_applied_or_reconstructed_again)
{
    Fixture f; Io io; InitialEditorSession session{}; InitialOpenReport report;
    auto context = f.context(); context.moObservations->startupHeating = {};
    openInitialEditorInto(session, *f.operation.engineModel, f.remesh, io, context, report);
    CHECK(!report.complete()); CHECK(!session.pageStateComplete);
    auto& sc = *f.operation.engineModel->simCtl(); sc.nstep = 456;
    std::vector<RecoveryEntry> empty;
    const auto applied = applyInitialEditorPageState(session, *f.operation.engineModel, f.remesh,
        io, &empty, &empty);
    CHECK(applied.outcome == ApplyResult::UnsupportedSnapshot);
    for (const auto& page : applied.pages) CHECK(!page);
    CHECK_EQ(sc.nstep, 456); CHECK_EQ(io.removes, 0);
    InitialOpenReport retry;
    openInitialEditorInto(session, *f.operation.engineModel, f.remesh, io, f.context(), retry);
    CHECK(retry.issue == InitialOpenIssue::AlreadyStarted); CHECK_EQ(io.reads, 0);
    CHECK_EQ(sc.nstep, 456);
}

int main() { return testkit::run_all(); }
