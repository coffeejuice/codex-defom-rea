// Isolated typed receiver tests; these do not claim actual Close integration.
// ctest-labels: m2m-mdt; isolated-close-effects
#include <testkit/check.hpp>

#include <limits>
#include <memory>
#include <optional>
#include <string>
#include <utility>

#include "m2m/common.hpp"
#include "m2m/doe_statevar.hpp"
#include "m2m/doe_io.hpp"
#include "m2m/doe_stepdata.hpp"
#include "m2m/doe_session_notifications.hpp"
#include "m2m/engine.hpp"
#include "m2m/engine_auxiliary.hpp"
#include "m2m/engine_scene.hpp"
#include "m2m/engine/close_effects.hpp"
#include "m2m/engine/model_manager.hpp"
#include "m2m/mst/writer.hpp"

using namespace m2m;

namespace {
struct Fixture {
    std::shared_ptr<engine::DbHandle> owner = std::make_shared<engine::DbHandle>();
    engine::GuiModel model{owner.get(), 3};
    Fixture() {
        model.doeStepData = std::make_shared<doe::StepDataState>();
        model.doeStepData->stateVar = std::make_shared<doe::StateVarState>();
    }
    doe::StateVarState& state() { return *model.doeStepData->stateVar; }
};

template<class F> void expectUnsupported(F&& body, const char* detail)
{
    bool refused = false;
    try { body(); }
    catch (const Error& error) {
        refused = error.code == M2M_E_UNSUPPORTED &&
                  std::string(error.what()).find(detail) != std::string::npos;
    }
    CHECK(refused);
}

engine::PtrkGraphRegistrations known(
    std::vector<std::weak_ptr<engine::PtrkGraphManagerState>> receivers = {})
{
    return {"isolated adapter fixture; not an original receiver-population observation",
            std::move(receivers)};
}

engine::PtrkGraphDisplayBinding knownDisplay(Project& project)
{
    engine::projectDisplay(project);
    return {project.display, "isolated actual global Display fixture; not original observation"};
}

void knownPostRegistrations(doe::SessionNotificationState& notifications)
{
    notifications.removeGraphItemsRegistrations.emplace();
    notifications.removeGraphItemsQualification = "isolated explicitly empty registration fixture";
}

engine::PostGraphSceneBinding knownPostScene(Project& project)
{
    engine::projectDisplay(project);
    return {std::make_shared<engine::PostGraphSceneState>(project.display),
            "isolated actual scene/embedded Display fixture; not native construction"};
}

struct RemoveGraphFixture {
    Project cachedProject, primaryProject, secondaryProject;
    std::shared_ptr<engine::DbHandle> model =
        std::make_shared<engine::DbHandle>(engine::DbHandle::Kind::Mo);
    doe::SessionNotificationState notifications;
    engine::PostGraphSceneBinding cached, primary, secondary;
    std::shared_ptr<engine::PostGraphManagerState> manager;
    engine::PostGraphWindowBinding window;
    RemoveGraphFixture() {
        knownPostRegistrations(notifications);
        cached = knownPostScene(cachedProject);
        primary = knownPostScene(primaryProject);
        secondary = knownPostScene(secondaryProject);
        manager = engine::ensurePostGraphManager(model, cached, notifications);
        primary.owner->commitInitialTextItems();
        secondary.owner->commitInitialTextItems();
        window = {true, primary, secondary, "isolated actual current global owners"};
    }
    void qualifyBoth() {
        primary.owner->qualifyCurrentItemPopulation("isolated complete primary text collection");
        secondary.owner->qualifyCurrentItemPopulation("isolated complete secondary text collection");
    }
    void dispatch() { engine::removeGraphItems(notifications, window); }
};

struct HistogramFixture {
    Project project;
    Fixture emitter, selected;
    std::shared_ptr<engine::ModelManager> models = std::make_shared<engine::ModelManager>();
    std::shared_ptr<doe::StudyIo> io = doe::makeRecordingStudyIo();
    std::shared_ptr<doe::SessionNotificationState> notifications =
        std::make_shared<doe::SessionNotificationState>();
    engine::PostGraphSceneBinding scene;
    engine::CloseEffects effects;
    HistogramFixture() {
        project.dataDb = std::make_shared<engine::DbHandle>(engine::DbHandle::Kind::Mo);
        project.dataDb->moProject = &project;
        auto sub = std::make_unique<SubProject>();
        sub->project = &project;
        project.subs.push_back(std::move(sub));
        models->appendCreatedModel(project.dataDb);
        REQUIRE(models->selectOrdinal(0));
        io->bindModelManager(models);
        io->bindSessionNotificationState(notifications);
        knownPostRegistrations(*notifications);
        emitter.state().histogram = std::make_shared<doe::HistogramState>();
        histogram().graphOn = true;
        io->stateVarRuntime().histogramRegistrations.push_back(
            {"signalChangeHistoGraph", "slotChangeHistoGraph", emitter.state().histogram});
        engine::displayShow(project, &selected.model);
        scene = knownPostScene(project);
        effects.setHistogramRegistrations(engine::HistogramRegistrations{
            io, "isolated actual runtime fixture"});
        bind();
    }
    doe::HistogramState& histogram() { return *emitter.state().histogram; }
    void bind() {
        effects.setHistogramGraphBinding(engine::HistogramGraphBinding{
            true, models, scene, "isolated nonnull global window fixture"});
    }
    void dispatch() { effects.changeHistoGraph(emitter.model, emitter.state()); }
    void bindSecondary(engine::PostGraphSceneBinding secondary) {
        auto binding = *effects.histogramGraphBinding();
        binding.secondaryScene = std::move(secondary);
        effects.setHistogramGraphBinding(std::move(binding));
    }
};
}

TEST_CASE(close_effects_isolated_missing_population_is_not_observed_empty)
{
    Fixture f;
    engine::CloseEffects effects;
    f.state().plotApplyInProgress = true;
    f.state().plotVar = 0;
    f.state().plotCompNo = 0;
    f.state().type = 17; // later render effects have not happened
    expectUnsupported([&] { effects.changePtrkGraph(f.model, f.state()); }, "population");
    CHECK(f.state().plotApplyInProgress);
    CHECK_EQ(f.state().type, 17);
    effects.setPtrkGraphRegistrations(engine::PtrkGraphRegistrations{});
    expectUnsupported([&] { effects.changePtrkGraph(f.model, f.state()); }, "population");
    effects.setPtrkGraphRegistrations(known());
    effects.changePtrkGraph(f.model, f.state());
    CHECK_EQ(f.state().type, 17);
    effects.setPtrkGraphRegistrations(std::nullopt);
    expectUnsupported([&] { effects.changePtrkGraph(f.model, f.state()); }, "population");
}

TEST_CASE(close_effects_isolated_off_receiver_uses_its_live_owned_flag)
{
    Fixture f;
    auto receiver = std::make_shared<engine::PtrkGraphManagerState>(f.owner);
    engine::CloseEffects effects;
    effects.setPtrkGraphRegistrations(known({receiver}));
    f.state().plotVar = 101; // off branch returns before reading this field
    f.state().plotCompNo = 2;
    effects.changePtrkGraph(f.model, f.state());
    CHECK(!receiver->graphOn);
    CHECK_EQ(f.state().plotVar, 101);
    CHECK_EQ(f.state().plotCompNo, 2);
    receiver->graphOn = true; // explicit fixture state; no automatic fresh inference
    expectUnsupported([&] { effects.changePtrkGraph(f.model, f.state()); }, "31fab0");
    receiver->graphOn = false;
    effects.changePtrkGraph(f.model, f.state());
}

TEST_CASE(close_effects_isolated_variable_zero_does_not_skip_enabled_graph_prefix)
{
    Fixture f;
    auto receiver = std::make_shared<engine::PtrkGraphManagerState>(f.owner);
    receiver->graphOn = true;
    engine::CloseEffects effects;
    effects.setPtrkGraphRegistrations(known({receiver}));
    f.state().plotVar = 0;
    f.state().plotCompNo = 0;
    f.state().plotApplyInProgress = true;
    f.state().minimum = 23;
    f.state().maximum = 47;
    expectUnsupported([&] { effects.changePtrkGraph(f.model, f.state()); }, "31fab0");
    CHECK(receiver->graphOn);
    CHECK(f.state().plotApplyInProgress);
    CHECK_EQ(f.state().minimum, 23.0);
    CHECK_EQ(f.state().maximum, 47.0);
}

TEST_CASE(close_effects_isolated_expired_receiver_skips_local_delivery_only)
{
    Fixture f;
    engine::CloseEffects effects;
    auto receiver = std::make_shared<engine::PtrkGraphManagerState>(f.owner);
    effects.setPtrkGraphRegistrations(known({receiver}));
    receiver.reset();
    effects.changePtrkGraph(f.model, f.state());
    REQUIRE(effects.ptrkGraphRegistrations().has_value());
    CHECK_EQ(effects.ptrkGraphRegistrations()->receivers.size(), std::size_t{1});
    CHECK(effects.ptrkGraphRegistrations()->receivers.front().expired());
    effects.changePtrkGraph(f.model, f.state()); // persistent registration unchanged
}

TEST_CASE(close_effects_isolated_multiple_receivers_preserve_reached_guard)
{
    Fixture f;
    auto off = std::make_shared<engine::PtrkGraphManagerState>(f.owner);
    auto on = std::make_shared<engine::PtrkGraphManagerState>(f.owner);
    on->graphOn = true;
    engine::CloseEffects effects;
    effects.setPtrkGraphRegistrations(known({off, on, off}));
    expectUnsupported([&] { effects.changePtrkGraph(f.model, f.state()); }, "31fab0");
    CHECK(!off->graphOn);
    CHECK(on->graphOn);
    CHECK_EQ(effects.ptrkGraphRegistrations()->receivers.size(), std::size_t{3});
}

TEST_CASE(close_effects_isolated_rejects_detached_state_and_lost_owner)
{
    Fixture f;
    engine::CloseEffects effects;
    effects.setPtrkGraphRegistrations(known());
    doe::StateVarState detached;
    expectUnsupported([&] { effects.changePtrkGraph(f.model, detached); }, "actual GuiModel");

    // A separately retained manager cannot silently outlive its QObject parent.
    auto otherOwner = std::make_shared<engine::DbHandle>();
    auto receiver = std::make_shared<engine::PtrkGraphManagerState>(otherOwner);
    effects.setPtrkGraphRegistrations(known({receiver}));
    otherOwner.reset();
    expectUnsupported([&] { effects.changePtrkGraph(f.model, f.state()); }, "data-root owner");
}


TEST_CASE(close_effects_isolated_second_hook_defaults_to_guard)
{
    struct FirstOnly : engine::StateVarPlotEffects {
        void changePtrkGraph(engine::GuiModel&, doe::StateVarState&) override {}
    } firstOnly;
    Fixture f;
    expectUnsupported([&] { firstOnly.changeHistoGraph(f.model, f.state()); }, "ChangeHistoGraph");
}

TEST_CASE(close_effects_isolated_histogram_requires_actual_qualified_runtime)
{
    Fixture f;
    engine::CloseEffects effects;
    expectUnsupported([&] { effects.changeHistoGraph(f.model, f.state()); }, "coverage");
    effects.setHistogramRegistrations(engine::HistogramRegistrations{});
    expectUnsupported([&] { effects.changeHistoGraph(f.model, f.state()); }, "coverage");
    auto io = doe::makeRecordingStudyIo();
    effects.setHistogramRegistrations(engine::HistogramRegistrations{io, ""});
    expectUnsupported([&] { effects.changeHistoGraph(f.model, f.state()); }, "coverage");
    effects.setHistogramRegistrations(engine::HistogramRegistrations{io, "isolated observed-empty fixture"});
    effects.changeHistoGraph(f.model, f.state());
    doe::StateVarState detached;
    expectUnsupported([&] { effects.changeHistoGraph(f.model, detached); }, "actual GuiModel");
}

TEST_CASE(close_effects_isolated_histogram_reads_actual_registry_and_live_flags)
{
    Fixture f;
    auto io = doe::makeRecordingStudyIo();
    engine::CloseEffects effects;
    effects.setHistogramRegistrations(engine::HistogramRegistrations{io, "isolated live-registry fixture"});
    f.state().histogram = std::make_shared<doe::HistogramState>();
    // Added AFTER binding; dispatch must not use a cloned registration population.
    io->stateVarRuntime().histogramRegistrations.push_back(
        {"signalChangeHistoGraph", "slotChangeHistoGraph", f.state().histogram});
    auto& histogram = *f.state().histogram;
    histogram.binWidth = 27;
    effects.changeHistoGraph(f.model, f.state());
    histogram.graphOn = true;
    expectUnsupported([&] { effects.changeHistoGraph(f.model, f.state()); }, "+0xb4");
    CHECK(histogram.graphOn);
    CHECK_EQ(histogram.binWidth, 27);
    histogram.graphOn = false;
    effects.changeHistoGraph(f.model, f.state());
}

TEST_CASE(close_effects_isolated_histogram_global_receivers_are_not_just_current_helper)
{
    Fixture f;
    auto io = doe::makeRecordingStudyIo();
    engine::CloseEffects effects;
    effects.setHistogramRegistrations(engine::HistogramRegistrations{io, "isolated global-registry fixture"});
    f.state().histogram = std::make_shared<doe::HistogramState>();
    auto other = std::make_shared<doe::HistogramState>();
    other->graphOn = true;
    auto& records = io->stateVarRuntime().histogramRegistrations;
    records.push_back({"signalHistoGraphOff", "slotHistoGraphOff", other});
    records.push_back({"signalChangeHistoGraph", "slotChangeHistoGraph", f.state().histogram});
    effects.changeHistoGraph(f.model, f.state()); // unrelated key is ignored
    records.push_back({"signalChangeHistoGraph", "slotChangeHistoGraph", other});
    expectUnsupported([&] { effects.changeHistoGraph(f.model, f.state()); }, "+0xb4");
    CHECK(!f.state().histogram->graphOn);
    CHECK(other->graphOn);
}

TEST_CASE(close_effects_isolated_histogram_expiry_preserves_actual_registry)
{
    Fixture f;
    auto io = doe::makeRecordingStudyIo();
    engine::CloseEffects effects;
    effects.setHistogramRegistrations(engine::HistogramRegistrations{io, "isolated lifetime fixture"});
    auto receiver = std::make_shared<doe::HistogramState>();
    auto& records = io->stateVarRuntime().histogramRegistrations;
    records.push_back({"signalChangeHistoGraph", "unrecoveredLiveSlot", receiver});
    expectUnsupported([&] { effects.changeHistoGraph(f.model, f.state()); }, "unsupported slot");
    receiver.reset();
    effects.changeHistoGraph(f.model, f.state()); // null receiver skips slot invocation
    CHECK_EQ(records.size(), std::size_t{1});
    CHECK(records.front().receiver.expired());
}

TEST_CASE(close_effects_isolated_histogram_lease_retains_same_runtime)
{
    Fixture f;
    auto io = doe::makeRecordingStudyIo();
    std::weak_ptr<doe::StudyIo> observed = io;
    auto* runtime = &io->stateVarRuntime();
    engine::CloseEffects effects;
    effects.setHistogramRegistrations(engine::HistogramRegistrations{io, "isolated session lease fixture"});
    io.reset();
    CHECK(!observed.expired());
    CHECK(&effects.histogramRegistrations()->owner->stateVarRuntime() == runtime);
    effects.changeHistoGraph(f.model, f.state());
    effects.setHistogramRegistrations(std::nullopt);
    CHECK(observed.expired());
}

TEST_CASE(close_effects_isolated_enabled_commits_graph_before_model_guard)
{
    Fixture f;
    auto receiver = std::make_shared<engine::PtrkGraphManagerState>(f.owner);
    receiver->graphOn = true;
    engine::CloseEffects effects;
    effects.setPtrkGraphRegistrations(known({receiver}));
    f.state().plotVar = 0;
    f.state().plotApplyInProgress = true;
    f.state().minimum = 23;
    CHECK(receiver->cachedGraph() == nullptr);
    expectUnsupported([&] { effects.changePtrkGraph(f.model, f.state()); }, "model+250");
    REQUIRE(receiver->cachedGraph() != nullptr);
    CHECK(!receiver->cachedGraph()->graphOn); // constructor +f1, separate from+4d0
    CHECK(receiver->graphOn);
    CHECK(f.state().plotApplyInProgress);
    CHECK_EQ(f.state().minimum, 23.0);
}

TEST_CASE(close_effects_isolated_cached_graph_identity_and_flag_survive_repeated_guard)
{
    Fixture f;
    auto receiver = std::make_shared<engine::PtrkGraphManagerState>(f.owner);
    receiver->graphOn = true;
    engine::CloseEffects effects;
    effects.setPtrkGraphRegistrations(known({receiver}));
    expectUnsupported([&] { effects.changePtrkGraph(f.model, f.state()); }, "model+250");
    auto* graph = receiver->cachedGraph();
    REQUIRE(graph != nullptr);
    graph->graphOn = true; // explicit prior graph mutation, not a manager flag
    expectUnsupported([&] { effects.changePtrkGraph(f.model, f.state()); }, "model+250");
    CHECK(receiver->cachedGraph() == graph);
    CHECK(graph->graphOn);
    receiver->graphOn = false;
    effects.changePtrkGraph(f.model, f.state());
    CHECK(receiver->cachedGraph() == graph);
    CHECK(graph->graphOn); // off receiver does not reset an existing graph
}

TEST_CASE(close_effects_isolated_prior_identity_and_owner_guards_precede_allocation)
{
    Fixture f;
    auto receiver = std::make_shared<engine::PtrkGraphManagerState>(f.owner);
    receiver->graphOn = true;
    engine::CloseEffects effects;
    effects.setPtrkGraphRegistrations(known({receiver}));
    doe::StateVarState detached;
    expectUnsupported([&] { effects.changePtrkGraph(f.model, detached); }, "actual GuiModel");
    CHECK(receiver->cachedGraph() == nullptr);
    auto otherOwner = std::make_shared<engine::DbHandle>();
    auto orphan = std::make_shared<engine::PtrkGraphManagerState>(otherOwner);
    orphan->graphOn = true;
    effects.setPtrkGraphRegistrations(known({orphan}));
    otherOwner.reset();
    expectUnsupported([&] { effects.changePtrkGraph(f.model, f.state()); }, "data-root owner");
    CHECK(orphan->cachedGraph() == nullptr);
}

TEST_CASE(close_effects_isolated_first_guard_retains_only_reached_graph_allocation)
{
    Fixture f;
    auto off = std::make_shared<engine::PtrkGraphManagerState>(f.owner);
    auto first = std::make_shared<engine::PtrkGraphManagerState>(f.owner);
    auto later = std::make_shared<engine::PtrkGraphManagerState>(f.owner);
    first->graphOn = later->graphOn = true;
    engine::CloseEffects effects;
    effects.setPtrkGraphRegistrations(known({off, first, later}));
    expectUnsupported([&] { effects.changePtrkGraph(f.model, f.state()); }, "model+250");
    CHECK(off->cachedGraph() == nullptr);
    CHECK(first->cachedGraph() != nullptr);
    CHECK(later->cachedGraph() == nullptr);
}

TEST_CASE(close_effects_isolated_graph_cleanup_preserves_manager_flag_and_recreates_defaults)
{
    Fixture f;
    auto receiver = std::make_shared<engine::PtrkGraphManagerState>(f.owner);
    receiver->graphOn = true;
    engine::CloseEffects effects;
    effects.setPtrkGraphRegistrations(known({receiver}));
    expectUnsupported([&] { effects.changePtrkGraph(f.model, f.state()); }, "model+250");
    REQUIRE(receiver->cachedGraph() != nullptr);
    receiver->cachedGraph()->graphOn = true;
    receiver->clearCachedGraph();
    CHECK(receiver->cachedGraph() == nullptr);
    CHECK(receiver->graphOn);
    receiver->clearCachedGraph(); // native null cache skips deletion
    expectUnsupported([&] { effects.changePtrkGraph(f.model, f.state()); }, "model+250");
    REQUIRE(receiver->cachedGraph() != nullptr);
    CHECK(!receiver->cachedGraph()->graphOn);
    receiver.reset(); // manager owns graph; weak registration survives expiration
    effects.changePtrkGraph(f.model, f.state());
    CHECK(effects.ptrkGraphRegistrations()->receivers.front().expired());
}

TEST_CASE(close_effects_isolated_global_display_missing_and_null_are_distinct)
{
    Project emptyProject;
    Fixture f;
    auto receiver = std::make_shared<engine::PtrkGraphManagerState>(f.owner);
    receiver->graphOn = true;
    engine::CloseEffects effects;
    effects.setPtrkGraphRegistrations(known({receiver}));
    effects.setPtrkGraphDisplayBinding(engine::PtrkGraphDisplayBinding{});
    expectUnsupported([&] { effects.changePtrkGraph(f.model, f.state()); }, "global Display binding");
    REQUIRE(receiver->cachedGraph() != nullptr);
    effects.setPtrkGraphDisplayBinding(engine::PtrkGraphDisplayBinding{
        nullptr, "isolated qualified null global mainwindow/scene"});
    effects.changePtrkGraph(f.model, f.state());
    CHECK(!receiver->cachedGraph()->graphOn);
    receiver->cachedGraph()->graphOn = true;
    effects.changePtrkGraph(f.model, f.state()); // both native null lookups return
    effects.setPtrkGraphDisplayBinding(knownDisplay(emptyProject));
    effects.changePtrkGraph(f.model, f.state()); // live Display with no selected model
    CHECK(f.model.auxiliaryDrawings == nullptr);
    effects.setPtrkGraphDisplayBinding(std::nullopt);
    expectUnsupported([&] { effects.changePtrkGraph(f.model, f.state()); }, "global Display binding");
    CHECK(receiver->cachedGraph()->graphOn);
}

TEST_CASE(close_effects_isolated_new_graph_registers_before_false_graph_return)
{
    Project project; // display outlives the fixture models and their backlinks
    Fixture emitter;
    engine::GuiModel selected{emitter.owner.get(), 3}; // no StateVar
    engine::displayShow(project, &selected);
    auto receiver = std::make_shared<engine::PtrkGraphManagerState>(emitter.owner);
    receiver->graphOn = true;
    engine::CloseEffects effects;
    effects.setPtrkGraphRegistrations(known({receiver}));
    effects.setPtrkGraphDisplayBinding(knownDisplay(project));
    emitter.state().plotVar = 101;
    effects.changePtrkGraph(emitter.model, emitter.state());
    REQUIRE(receiver->cachedGraph() != nullptr);
    REQUIRE(selected.auxiliaryDrawings != nullptr);
    CHECK_EQ(selected.auxiliaryDrawings->entries.size(), std::size_t{1});
    CHECK(selected.auxiliaryDrawings->entries.front() == receiver->cachedGraph());
    CHECK(emitter.model.auxiliaryDrawings == nullptr);
    CHECK(!selected.doeStepData || !selected.doeStepData->stateVar);
    CHECK(!receiver->cachedGraph()->graphOn);
    CHECK_EQ(emitter.state().plotVar, 101);
}

TEST_CASE(close_effects_isolated_cached_graph_follows_actual_guarded_selection)
{
    Project project;
    Fixture first;
    Fixture second;
    engine::displayShow(project, &first.model);
    auto receiver = std::make_shared<engine::PtrkGraphManagerState>(first.owner);
    receiver->graphOn = true;
    engine::CloseEffects effects;
    effects.setPtrkGraphRegistrations(known({receiver}));
    effects.setPtrkGraphDisplayBinding(knownDisplay(project));
    engine::selectDisplayModel(*project.display, &first.model); // actual MAIN commit
    effects.changePtrkGraph(first.model, first.state());
    const auto* graph = receiver->cachedGraph();
    effects.changePtrkGraph(first.model, first.state());
    CHECK_EQ(first.model.auxiliaryDrawings->entries.size(), std::size_t{1});

    engine::displayShow(project, &second.model); // old selected override remains
    effects.changePtrkGraph(first.model, first.state());
    CHECK(second.model.auxiliaryDrawings == nullptr);
    CHECK(engine::displaySelectedModel(*project.display) == &first.model);
    engine::selectDisplayModel(*project.display, &second.model);
    effects.changePtrkGraph(first.model, first.state());
    REQUIRE(second.model.auxiliaryDrawings != nullptr);
    CHECK_EQ(second.model.auxiliaryDrawings->entries.size(), std::size_t{1});
    CHECK(second.model.auxiliaryDrawings->entries.front() == graph);
    CHECK(first.model.auxiliaryDrawings->entries.front() == graph);
    CHECK(receiver->cachedGraph() == graph);
}

TEST_CASE(close_effects_isolated_enabled_graph_reads_selected_zero_variable)
{
    Project project;
    Fixture emitter;
    Fixture selected;
    engine::displayShow(project, &selected.model);
    auto receiver = std::make_shared<engine::PtrkGraphManagerState>(emitter.owner);
    receiver->graphOn = true;
    engine::CloseEffects effects;
    effects.setPtrkGraphRegistrations(known({receiver}));
    effects.setPtrkGraphDisplayBinding(knownDisplay(project));
    emitter.state().plotVar = 101;
    selected.state().plotVar = 0;
    selected.state().minimum = 47;
    effects.changePtrkGraph(emitter.model, emitter.state());
    receiver->cachedGraph()->graphOn = true;
    effects.changePtrkGraph(emitter.model, emitter.state());
    CHECK_EQ(selected.model.auxiliaryDrawings->entries.size(), std::size_t{1});
    CHECK(receiver->cachedGraph()->graphOn);
    CHECK_EQ(selected.state().plotVar, 0);
    CHECK_EQ(selected.state().minimum, 47.0);
    CHECK_EQ(emitter.state().plotVar, 101);
}

TEST_CASE(close_effects_isolated_selected_nonzero_guards_after_registration)
{
    Project project;
    Fixture emitter;
    Fixture selected;
    engine::displayShow(project, &selected.model);
    auto receiver = std::make_shared<engine::PtrkGraphManagerState>(emitter.owner);
    receiver->graphOn = true;
    engine::CloseEffects effects;
    effects.setPtrkGraphRegistrations(known({receiver}));
    effects.setPtrkGraphDisplayBinding(engine::PtrkGraphDisplayBinding{
        nullptr, "isolated prior null global scene"});
    effects.changePtrkGraph(emitter.model, emitter.state());
    receiver->cachedGraph()->graphOn = true;
    effects.setPtrkGraphDisplayBinding(knownDisplay(project));
    emitter.state().plotVar = 0;
    selected.state().plotVar = 101;
    expectUnsupported([&] { effects.changePtrkGraph(emitter.model, emitter.state()); }, "30ad90");
    REQUIRE(selected.model.auxiliaryDrawings != nullptr);
    CHECK(selected.model.auxiliaryDrawings->entries.front() == receiver->cachedGraph());
    CHECK(receiver->graphOn);
    CHECK(receiver->cachedGraph()->graphOn);
    CHECK_EQ(selected.state().plotVar, 101);
    CHECK_EQ(emitter.state().plotVar, 0);
}

TEST_CASE(close_effects_isolated_selected_missing_state_guards_at_constructor)
{
    Project project;
    Fixture emitter;
    engine::GuiModel selected{emitter.owner.get(), 3};
    engine::displayShow(project, &selected);
    auto receiver = std::make_shared<engine::PtrkGraphManagerState>(emitter.owner);
    receiver->graphOn = true;
    engine::CloseEffects effects;
    effects.setPtrkGraphRegistrations(known({receiver}));
    effects.setPtrkGraphDisplayBinding(knownDisplay(project));
    effects.changePtrkGraph(emitter.model, emitter.state());
    receiver->cachedGraph()->graphOn = true;
    expectUnsupported([&] { effects.changePtrkGraph(emitter.model, emitter.state()); }, "c905b0");
    REQUIRE(selected.auxiliaryDrawings != nullptr);
    CHECK_EQ(selected.auxiliaryDrawings->entries.size(), std::size_t{1});
    CHECK(!selected.doeStepData || !selected.doeStepData->stateVar);
    CHECK(receiver->cachedGraph()->graphOn);
}

TEST_CASE(close_effects_isolated_manager_off_skips_display_and_registry)
{
    Project project;
    Fixture f;
    engine::displayShow(project, &f.model);
    auto receiver = std::make_shared<engine::PtrkGraphManagerState>(f.owner);
    engine::CloseEffects effects;
    effects.setPtrkGraphRegistrations(known({receiver}));
    effects.setPtrkGraphDisplayBinding(knownDisplay(project));
    effects.changePtrkGraph(f.model, f.state());
    effects.setPtrkGraphDisplayBinding(std::nullopt);
    effects.changePtrkGraph(f.model, f.state());
    CHECK(receiver->cachedGraph() == nullptr);
    CHECK(f.model.auxiliaryDrawings == nullptr);
}

TEST_CASE(close_effects_isolated_registration_preserves_receiver_failure_order)
{
    Project project;
    Fixture f;
    engine::displayShow(project, &f.model);
    auto first = std::make_shared<engine::PtrkGraphManagerState>(f.owner);
    auto second = std::make_shared<engine::PtrkGraphManagerState>(f.owner);
    auto later = std::make_shared<engine::PtrkGraphManagerState>(f.owner);
    first->graphOn = second->graphOn = later->graphOn = true;
    engine::CloseEffects effects;
    effects.setPtrkGraphRegistrations(known({second}));
    effects.setPtrkGraphDisplayBinding(engine::PtrkGraphDisplayBinding{
        nullptr, "isolated earlier null global scene"});
    effects.changePtrkGraph(f.model, f.state());
    second->cachedGraph()->graphOn = true;
    effects.setPtrkGraphRegistrations(known({first, second, later}));
    effects.setPtrkGraphDisplayBinding(knownDisplay(project));
    f.state().plotVar = 101;
    expectUnsupported([&] { effects.changePtrkGraph(f.model, f.state()); }, "30ad90");
    REQUIRE(f.model.auxiliaryDrawings != nullptr);
    REQUIRE(f.model.auxiliaryDrawings->entries.size() == std::size_t{2});
    CHECK(f.model.auxiliaryDrawings->entries[0] == first->cachedGraph());
    CHECK(f.model.auxiliaryDrawings->entries[1] == second->cachedGraph());
    CHECK(!first->cachedGraph()->graphOn);
    CHECK(later->cachedGraph() == nullptr);
}

TEST_CASE(close_effects_isolated_graph_delete_preserves_raw_model_membership)
{
    Project project;
    Fixture f;
    engine::displayShow(project, &f.model);
    auto receiver = std::make_shared<engine::PtrkGraphManagerState>(f.owner);
    receiver->graphOn = true;
    engine::CloseEffects effects;
    effects.setPtrkGraphRegistrations(known({receiver}));
    effects.setPtrkGraphDisplayBinding(knownDisplay(project));
    effects.changePtrkGraph(f.model, f.state());
    const void* identity = receiver->cachedGraph();
    receiver->clearCachedGraph();
    CHECK(receiver->cachedGraph() == nullptr);
    REQUIRE(f.model.auxiliaryDrawings != nullptr);
    CHECK_EQ(f.model.auxiliaryDrawings->entries.size(), std::size_t{1});
    CHECK(f.model.auxiliaryDrawings->entries.front() == identity); // never dereference
    f.model.reset();
    REQUIRE(f.model.auxiliaryDrawings != nullptr);
    CHECK(f.model.auxiliaryDrawings->entries.front() == identity);
}

TEST_CASE(close_effects_isolated_post_manager_owns_factory_and_registers_in_shared_notifier)
{
    Project project;
    auto model = std::make_shared<engine::DbHandle>(engine::DbHandle::Kind::Mo);
    doe::SessionNotificationState notifications;
    knownPostRegistrations(notifications);
    notifications.suppressed = true;
    notifications.flag90 = true;
    const auto scene = knownPostScene(project);
    auto manager = engine::ensurePostGraphManager(model, scene, notifications);
    REQUIRE(manager != nullptr);
    CHECK(model->postGraphManager == manager);
    CHECK(manager->modelOwner().lock() == model);
    CHECK(manager->sceneOwner().lock() == scene.owner);
    CHECK(scene.owner->displayOwner() == project.display);
    REQUIRE(manager->factory() != nullptr);
    CHECK(manager->factory()->kind() == engine::PostGraphFactoryState::Kind::Post);
    CHECK(manager->factory()->manager() == manager.get());
    CHECK(manager->factory()->modelOwner().lock() == model);
    CHECK_EQ(manager->field30, 10);
    CHECK_EQ(manager->field34, 10);
    CHECK(!manager->flag38);
    CHECK(manager->graphItems().empty());
    REQUIRE(notifications.removeGraphItemsRegistrations->size() == std::size_t{1});
    CHECK(notifications.removeGraphItemsRegistrations->front().lock() == manager);
    CHECK(notifications.suppressed); // registration does not dispatch or alter gates
    CHECK(notifications.flag90);
    CHECK(notifications.attempts.empty());
}

TEST_CASE(close_effects_isolated_post_manager_cache_precedes_constructor_context)
{
    Project project;
    auto model = std::make_shared<engine::DbHandle>(engine::DbHandle::Kind::Mo);
    doe::SessionNotificationState notifications;
    knownPostRegistrations(notifications);
    const auto scene = knownPostScene(project);
    auto manager = engine::ensurePostGraphManager(model, scene, notifications);
    const auto* factory = manager->factory();
    manager->field30 = 27;
    manager->flag38 = true;
    doe::SessionNotificationState unavailable;
    auto cached = engine::ensurePostGraphManager(model, std::nullopt, unavailable);
    CHECK(cached == manager);
    CHECK(cached->factory() == factory);
    CHECK_EQ(cached->field30, 27);
    CHECK(cached->flag38);
    CHECK(!unavailable.removeGraphItemsRegistrations);
    CHECK_EQ(notifications.removeGraphItemsRegistrations->size(), std::size_t{1});
}

TEST_CASE(close_effects_isolated_post_manager_scene_absence_differs_from_unavailable)
{
    auto model = std::make_shared<engine::DbHandle>(engine::DbHandle::Kind::Mo);
    doe::SessionNotificationState notifications;
    knownPostRegistrations(notifications);
    expectUnsupported([&] { engine::ensurePostGraphManager(model, std::nullopt, notifications); },
                      "scene binding");
    expectUnsupported([&] { engine::ensurePostGraphManager(model,
        engine::PostGraphSceneBinding{}, notifications); }, "scene binding");
    CHECK(!model->postGraphManager);
    CHECK(notifications.removeGraphItemsRegistrations->empty());
    const engine::PostGraphSceneBinding absent{nullptr,
        "isolated nonnull mainwindow with observed null scene result"};
    auto manager = engine::ensurePostGraphManager(model, absent, notifications);
    REQUIRE(manager != nullptr);
    CHECK(manager->sceneOwner().expired());
    CHECK(manager->factory() != nullptr);
    CHECK_EQ(notifications.removeGraphItemsRegistrations->size(), std::size_t{1});
    expectUnsupported([&] { engine::PostGraphSceneState invalid{nullptr}; }, "embedded Display");
}

TEST_CASE(close_effects_isolated_post_manager_preserves_unknown_registration_guard)
{
    auto model = std::make_shared<engine::DbHandle>(engine::DbHandle::Kind::Mo);
    const engine::PostGraphSceneBinding scene{nullptr, "isolated qualified null scene"};
    doe::SessionNotificationState notifications;
    expectUnsupported([&] { engine::ensurePostGraphManager(model, scene, notifications); },
                      "registration population");
    CHECK(!model->postGraphManager); // cdd8e0 has not committed constructor result
    CHECK(!notifications.removeGraphItemsRegistrations);
    notifications.removeGraphItemsRegistrations.emplace();
    expectUnsupported([&] { engine::ensurePostGraphManager(model, scene, notifications); },
                      "registration population");
    CHECK(!model->postGraphManager);
    CHECK(notifications.removeGraphItemsRegistrations->empty());
    notifications.removeGraphItemsQualification = "isolated explicit empty population";
    CHECK(engine::ensurePostGraphManager(model, scene, notifications) != nullptr);
}

TEST_CASE(close_effects_isolated_post_manager_rejects_missing_and_base_model_factory)
{
    auto base = std::make_shared<engine::DbHandle>(); // type4/base factory differs
    doe::SessionNotificationState notifications;
    knownPostRegistrations(notifications);
    const engine::PostGraphSceneBinding scene{nullptr, "isolated qualified null scene"};
    expectUnsupported([&] { engine::ensurePostGraphManager(nullptr, scene, notifications); },
                      "actual model/DB child");
    expectUnsupported([&] { engine::ensurePostGraphManager(base, scene, notifications); }, "cdd930");
    CHECK(!base->postGraphManager);
    CHECK(notifications.removeGraphItemsRegistrations->empty());
}

TEST_CASE(close_effects_isolated_post_manager_weak_scene_and_retained_expired_registration)
{
    Project project;
    auto model = std::make_shared<engine::DbHandle>(engine::DbHandle::Kind::Mo);
    doe::SessionNotificationState notifications;
    knownPostRegistrations(notifications);
    auto scene = knownPostScene(project);
    auto manager = engine::ensurePostGraphManager(model, scene, notifications);
    std::weak_ptr<engine::PostGraphSceneState> sceneLifetime = scene.owner;
    scene.owner.reset();
    CHECK(sceneLifetime.expired()); // manager+28 does not own the scene
    CHECK(manager->sceneOwner().expired());
    std::weak_ptr<engine::PostGraphManagerState> lifetime = manager;
    manager.reset();
    CHECK(!lifetime.expired()); // actual model cache owns the manager
    model->postGraphManager.reset(); // empty manager destroys its factory
    CHECK(lifetime.expired());
    REQUIRE(notifications.removeGraphItemsRegistrations->size() == std::size_t{1});
    CHECK(notifications.removeGraphItemsRegistrations->front().expired());
    auto replacement = engine::ensurePostGraphManager(model,
        engine::PostGraphSceneBinding{nullptr, "isolated qualified null scene"}, notifications);
    CHECK(replacement != nullptr);
    CHECK_EQ(notifications.removeGraphItemsRegistrations->size(), std::size_t{2});
    CHECK(notifications.removeGraphItemsRegistrations->front().expired());
}

TEST_CASE(close_effects_isolated_post_manager_cache_cannot_migrate_between_model_owners)
{
    auto first = std::make_shared<engine::DbHandle>(engine::DbHandle::Kind::Mo);
    auto other = std::make_shared<engine::DbHandle>(engine::DbHandle::Kind::Mo);
    doe::SessionNotificationState notifications;
    knownPostRegistrations(notifications);
    auto manager = engine::ensurePostGraphManager(first,
        engine::PostGraphSceneBinding{nullptr, "isolated qualified null scene"}, notifications);
    other->postGraphManager = manager; // invalid adapter wiring, not a native mutator
    expectUnsupported([&] { engine::ensurePostGraphManager(other, std::nullopt, notifications); },
                      "different model/DB child");
    CHECK(first->postGraphManager == manager);
    CHECK_EQ(notifications.removeGraphItemsRegistrations->size(), std::size_t{1});
}

TEST_CASE(close_effects_isolated_remove_graph_items_suppression_precedes_unknown_population)
{
    doe::SessionNotificationState notifications;
    notifications.suppressed = true;
    notifications.flag90 = true;
    engine::removeGraphItems(notifications);
    CHECK(notifications.flag90);
    CHECK(!notifications.removeGraphItemsRegistrations);
    CHECK(notifications.emptyKeys.empty());
    CHECK(notifications.attempts.empty());
}

TEST_CASE(close_effects_isolated_remove_graph_items_unknown_population_preserves_prefix)
{
    doe::SessionNotificationState notifications;
    notifications.flag90 = true;
    expectUnsupported([&] { engine::removeGraphItems(notifications); }, "dispatch population");
    CHECK(!notifications.flag90);
    CHECK(notifications.emptyKeys.empty());
    notifications.removeGraphItemsRegistrations.emplace();
    notifications.flag90 = true;
    expectUnsupported([&] { engine::removeGraphItems(notifications); }, "dispatch population");
    CHECK(!notifications.flag90);
    CHECK(notifications.emptyKeys.empty());
}

TEST_CASE(close_effects_isolated_remove_graph_items_qualified_empty_key_is_persistent)
{
    doe::SessionNotificationState notifications;
    knownPostRegistrations(notifications);
    notifications.emptyKeys.insert("2signalOpenSession()");
    notifications.flag90 = true;
    engine::removeGraphItems(notifications);
    CHECK(!notifications.flag90);
    CHECK_EQ(notifications.emptyKeys.size(), std::size_t{2});
    CHECK(notifications.emptyKeys.count("2signalRemoveGraphItems()") == 1);
    notifications.flag90 = true;
    engine::removeGraphItems(notifications);
    CHECK(!notifications.flag90);
    CHECK_EQ(notifications.emptyKeys.size(), std::size_t{2});
    CHECK(notifications.removeGraphItemsRegistrations->empty());
}

TEST_CASE(close_effects_isolated_remove_graph_items_expired_registration_is_not_erased)
{
    auto model = std::make_shared<engine::DbHandle>(engine::DbHandle::Kind::Mo);
    doe::SessionNotificationState notifications;
    knownPostRegistrations(notifications);
    auto receiver = engine::ensurePostGraphManager(model,
        engine::PostGraphSceneBinding{nullptr, "isolated qualified null scene"}, notifications);
    receiver.reset();
    model.reset();
    REQUIRE(notifications.removeGraphItemsRegistrations->front().expired());
    notifications.flag90 = true;
    engine::removeGraphItems(notifications);
    CHECK(!notifications.flag90);
    CHECK_EQ(notifications.removeGraphItemsRegistrations->size(), std::size_t{1});
    CHECK(notifications.emptyKeys.empty()); // registered key was not absent
}

TEST_CASE(close_effects_isolated_remove_graph_items_live_empty_manager_keeps_scene_guard)
{
    auto model = std::make_shared<engine::DbHandle>(engine::DbHandle::Kind::Mo);
    doe::SessionNotificationState notifications;
    knownPostRegistrations(notifications);
    auto receiver = engine::ensurePostGraphManager(model,
        engine::PostGraphSceneBinding{nullptr, "isolated earlier null scene"}, notifications);
    receiver->field30 = 28;
    receiver->flag38 = true;
    const auto* factory = receiver->factory();
    notifications.flag90 = true;
    expectUnsupported([&] { engine::removeGraphItems(notifications); }, "c0cfe0");
    CHECK(!notifications.flag90);
    CHECK(receiver->factory() == factory);
    CHECK(receiver->graphItems().empty());
    CHECK_EQ(receiver->field30, 28);
    CHECK(receiver->flag38);
    CHECK(model->postGraphManager == receiver);
    CHECK_EQ(notifications.removeGraphItemsRegistrations->size(), std::size_t{1});
    CHECK(notifications.emptyKeys.empty());
    notifications.suppressed = true;
    notifications.flag90 = true;
    engine::removeGraphItems(notifications);
    CHECK(notifications.flag90);
}

TEST_CASE(close_effects_isolated_remove_graph_items_rejects_retained_orphan_receiver)
{
    auto model = std::make_shared<engine::DbHandle>(engine::DbHandle::Kind::Mo);
    doe::SessionNotificationState notifications;
    knownPostRegistrations(notifications);
    auto receiver = engine::ensurePostGraphManager(model,
        engine::PostGraphSceneBinding{nullptr, "isolated qualified null scene"}, notifications);
    model->postGraphManager.reset();
    expectUnsupported([&] { engine::removeGraphItems(notifications); }, "cached manager");
    model.reset();
    expectUnsupported([&] { engine::removeGraphItems(notifications); }, "lost its model owner");
    CHECK_EQ(notifications.removeGraphItemsRegistrations->size(), std::size_t{1});
    CHECK(receiver->factory() != nullptr);
}

TEST_CASE(close_effects_isolated_histogram_absent_global_returns_before_owner_lookup)
{
    HistogramFixture f;
    f.effects.setHistogramGraphBinding(engine::HistogramGraphBinding{
        false, nullptr, std::nullopt, "isolated observed absent global mainwindow"});
    f.dispatch();
    CHECK(f.histogram().graphOn);
    CHECK(!f.project.dataDb->postGraphManager);
    CHECK(f.notifications->removeGraphItemsRegistrations->empty());
}

TEST_CASE(close_effects_isolated_histogram_requires_same_live_manager_and_notifier)
{
    HistogramFixture f;
    auto other = std::make_shared<engine::ModelManager>();
    f.effects.setHistogramGraphBinding(engine::HistogramGraphBinding{
        true, other, f.scene, "isolated wrong manager fixture"});
    expectUnsupported([&] { f.dispatch(); }, "StudyIo-bound owner");
    CHECK(f.histogram().graphOn);
    CHECK(!f.project.dataDb->postGraphManager);
    auto missingNotifierIo = doe::makeRecordingStudyIo();
    missingNotifierIo->bindModelManager(f.models);
    missingNotifierIo->stateVarRuntime().histogramRegistrations =
        f.io->stateVarRuntime().histogramRegistrations;
    f.effects.setHistogramRegistrations(engine::HistogramRegistrations{
        missingNotifierIo, "isolated actual runtime without notifier"});
    f.bind();
    expectUnsupported([&] { f.dispatch(); }, "shared notifier owner");
    CHECK(missingNotifierIo->observedSessionNotificationState() == nullptr);
    CHECK(!f.project.dataDb->postGraphManager);
}

TEST_CASE(close_effects_isolated_histogram_null_current_is_not_success)
{
    HistogramFixture f;
    f.models->clear();
    expectUnsupported([&] { f.dispatch(); }, "null current model");
    CHECK(f.histogram().graphOn);
    CHECK(!f.project.dataDb->postGraphManager);
    CHECK(f.notifications->removeGraphItemsRegistrations->empty());
}

TEST_CASE(close_effects_isolated_histogram_manager_commit_precedes_missing_selected_state)
{
    HistogramFixture f;
    f.selected.model.doeStepData->stateVar.reset();
    expectUnsupported([&] { f.dispatch(); }, "c905b0");
    REQUIRE(f.project.dataDb->postGraphManager != nullptr);
    CHECK_EQ(f.notifications->removeGraphItemsRegistrations->size(), std::size_t{1});
    CHECK(f.histogram().graphOn);
    CHECK(!f.selected.model.doeStepData->stateVar);
}

TEST_CASE(close_effects_isolated_histogram_reads_global_selected_variable_before_clear)
{
    HistogramFixture f;
    f.emitter.state().plotVar = 0;
    f.selected.state().plotVar = 19;
    expectUnsupported([&] { f.dispatch(); }, "virtual+78");
    REQUIRE(f.project.dataDb->postGraphManager != nullptr);
    CHECK(f.histogram().graphOn);
    auto manager = f.project.dataDb->postGraphManager;
    f.emitter.state().plotVar = 99;
    f.selected.state().plotVar = 0;
    f.histogram().binWidth = 23;
    expectUnsupported([&] { f.dispatch(); }, "c0d7e0");
    CHECK(!f.histogram().graphOn);
    CHECK_EQ(f.histogram().binWidth, 23);
    CHECK(f.project.dataDb->postGraphManager == manager);
    CHECK_EQ(f.notifications->removeGraphItemsRegistrations->size(), std::size_t{1});
}

TEST_CASE(close_effects_isolated_histogram_null_scene_is_distinct_from_unknown_scene)
{
    HistogramFixture f;
    f.effects.setHistogramGraphBinding(engine::HistogramGraphBinding{
        true, f.models, std::nullopt, "isolated known window with unresolved scene"});
    expectUnsupported([&] { f.dispatch(); }, "045260");
    CHECK(!f.project.dataDb->postGraphManager);
    CHECK(f.histogram().graphOn);
    f.scene = {nullptr, "isolated known null primary scene"};
    f.bind();
    expectUnsupported([&] { f.dispatch(); }, "c0d7e0");
    CHECK(!f.histogram().graphOn);
    REQUIRE(f.project.dataDb->postGraphManager != nullptr);
    CHECK(f.project.dataDb->postGraphManager->sceneOwner().expired());
}

TEST_CASE(close_effects_isolated_histogram_cached_manager_precedes_unknown_selected_scene)
{
    HistogramFixture f;
    auto manager = engine::ensurePostGraphManager(f.project.dataDb, f.scene, *f.notifications);
    f.effects.setHistogramGraphBinding(engine::HistogramGraphBinding{
        true, f.models, std::nullopt, "isolated known window with unresolved current scene"});
    expectUnsupported([&] { f.dispatch(); }, "414030");
    CHECK(f.histogram().graphOn);
    CHECK(f.project.dataDb->postGraphManager == manager);
    CHECK_EQ(f.notifications->removeGraphItemsRegistrations->size(), std::size_t{1});
}

TEST_CASE(close_effects_isolated_histogram_uses_live_current_model_and_preserves_type4_guard)
{
    HistogramFixture f;
    auto base = std::make_shared<engine::DbHandle>();
    f.project.subs[0]->type = 4;
    f.project.subs[0]->ownDb = base;
    expectUnsupported([&] { f.dispatch(); }, "cdd930");
    CHECK(!base->postGraphManager);
    CHECK(!f.project.dataDb->postGraphManager);
    CHECK(f.histogram().graphOn);
    f.project.subs[0]->type = 0;
    expectUnsupported([&] { f.dispatch(); }, "c0d7e0");
    CHECK(f.project.dataDb->postGraphManager != nullptr);
    CHECK(!base->postGraphManager);
    CHECK(!f.histogram().graphOn);
}

TEST_CASE(close_effects_isolated_histogram_failed_receiver_stops_later_receiver_mutation)
{
    HistogramFixture f;
    auto later = std::make_shared<doe::HistogramState>();
    later->graphOn = true;
    f.io->stateVarRuntime().histogramRegistrations.push_back(
        {"signalChangeHistoGraph", "slotChangeHistoGraph", later});
    expectUnsupported([&] { f.dispatch(); }, "c0d7e0");
    CHECK(!f.histogram().graphOn);
    CHECK(later->graphOn);
    f.effects.setHistogramGraphBinding(std::nullopt);
    expectUnsupported([&] { f.dispatch(); }, "+0xb4");
    CHECK(!f.histogram().graphOn);
    CHECK(later->graphOn);
}

TEST_CASE(close_effects_isolated_scene_identity_binding_does_not_construct_items)
{
    Project project;
    auto scene = knownPostScene(project);
    CHECK(scene.owner->items().empty());
    CHECK(scene.owner->databaseNameItem() == nullptr);
    CHECK(scene.owner->stepNumberItem() == nullptr);
    expectUnsupported([&] { scene.owner->hasHistogramChart(); }, "population coverage");
    expectUnsupported([&] {
        scene.owner->qualifyCurrentItemPopulation("invalid missing constructor fixture");
    }, "initial text construction");
    CHECK(scene.owner->itemPopulationQualification().empty());
}

TEST_CASE(close_effects_isolated_scene_text_prefix_owns_two_distinct_hidden_objects)
{
    Project project;
    auto scene = knownPostScene(project);
    scene.owner->commitInitialTextItems();
    auto* database = scene.owner->databaseNameItem();
    auto* step = scene.owner->stepNumberItem();
    REQUIRE(database != nullptr && step != nullptr);
    CHECK(database != step);
    CHECK(database->textTemplate == qt::L("$DBNAME"));
    CHECK(database->displayedText == database->textTemplate);
    CHECK(step->textTemplate == qt::L("Step $STEPNO"));
    CHECK(step->displayedText == step->textTemplate);
    CHECK(database->hidden && step->hidden);
    CHECK(database->displayOwner().lock() == project.display);
    CHECK(step->displayOwner().lock() == project.display);
    REQUIRE(scene.owner->items().size() == std::size_t{2});
    CHECK(scene.owner->items()[0].get() == database);
    CHECK(scene.owner->items()[1].get() == step);
    CHECK(scene.owner->itemPopulationQualification().empty());
    expectUnsupported([&] { scene.owner->commitInitialTextItems(); }, "one-shot");
    CHECK(scene.owner->databaseNameItem() == database);
    CHECK(scene.owner->stepNumberItem() == step);
    CHECK_EQ(scene.owner->items().size(), std::size_t{2});
}

TEST_CASE(close_effects_isolated_scene_query_uses_concrete_types_and_preserves_text)
{
    Project project;
    auto scene = knownPostScene(project);
    scene.owner->commitInitialTextItems();
    expectUnsupported([&] { scene.owner->hasHistogramChart(); }, "population coverage");
    scene.owner->qualifyCurrentItemPopulation("isolated complete text-only scene fixture");
    CHECK(!scene.owner->hasHistogramChart());
    auto* database = scene.owner->databaseNameItem();
    auto* step = scene.owner->stepNumberItem();
    database->textTemplate = qt::L("SfHistoChart");
    database->displayedText = qt::L("SfHistoChart");
    database->hidden = false;
    CHECK(!scene.owner->hasHistogramChart()); // text is not SfGraphItem RTTI
    CHECK(scene.owner->items()[0].get() == database);
    CHECK(scene.owner->items()[1].get() == step);
    CHECK(!database->hidden && step->hidden);
    scene.owner->invalidateItemPopulation();
    expectUnsupported([&] { scene.owner->hasHistogramChart(); }, "population coverage");
    CHECK(database->textTemplate == qt::L("SfHistoChart"));
    CHECK_EQ(scene.owner->items().size(), std::size_t{2});
    scene.owner->qualifyCurrentItemPopulation("isolated renewed current coverage");
    CHECK(!scene.owner->hasHistogramChart());
    CHECK(scene.owner->databaseNameItem() == database);
}

TEST_CASE(close_effects_isolated_histogram_secondary_text_no_match_completes)
{
    HistogramFixture f;
    Project secondaryProject;
    auto secondary = knownPostScene(secondaryProject);
    secondary.owner->commitInitialTextItems();
    secondary.owner->qualifyCurrentItemPopulation("isolated complete secondary text-only fixture");
    auto* database = secondary.owner->databaseNameItem();
    auto* step = secondary.owner->stepNumberItem();
    f.bindSecondary(secondary);
    f.dispatch();
    CHECK(!f.histogram().graphOn);
    REQUIRE(f.project.dataDb->postGraphManager != nullptr);
    CHECK(f.project.dataDb->postGraphManager->sceneOwner().lock() == f.scene.owner);
    CHECK(f.scene.owner != secondary.owner);
    CHECK(f.scene.owner->items().empty()); // primary population is not substituted
    CHECK(secondary.owner->items()[0].get() == database);
    CHECK(secondary.owner->items()[1].get() == step);
    CHECK(database->hidden && step->hidden);
    CHECK_EQ(f.notifications->removeGraphItemsRegistrations->size(), std::size_t{1});
    CHECK(f.notifications->attempts.empty()); // no synthetic Hide notification
}

TEST_CASE(close_effects_isolated_histogram_unknown_secondary_coverage_preserves_clear)
{
    HistogramFixture f;
    Project secondaryProject;
    auto secondary = knownPostScene(secondaryProject);
    secondary.owner->commitInitialTextItems();
    f.bindSecondary(secondary);
    expectUnsupported([&] { f.dispatch(); }, "population coverage");
    CHECK(!f.histogram().graphOn);
    CHECK(f.project.dataDb->postGraphManager != nullptr);
    CHECK_EQ(secondary.owner->items().size(), std::size_t{2});
    CHECK(secondary.owner->databaseNameItem()->hidden);
    f.histogram().graphOn = true;
    secondary.owner->qualifyCurrentItemPopulation("isolated subsequent source coverage");
    f.dispatch();
    CHECK(!f.histogram().graphOn);
}

TEST_CASE(close_effects_isolated_histogram_null_secondary_is_not_empty_population)
{
    HistogramFixture f;
    f.bindSecondary({nullptr, "isolated observed null secondary scene"});
    expectUnsupported([&] { f.dispatch(); }, "items() dereference");
    CHECK(!f.histogram().graphOn);
    CHECK(f.project.dataDb->postGraphManager != nullptr);
}

TEST_CASE(close_effects_isolated_histogram_no_match_allows_later_actual_receivers)
{
    HistogramFixture f;
    Project secondaryProject;
    auto secondary = knownPostScene(secondaryProject);
    secondary.owner->commitInitialTextItems();
    secondary.owner->qualifyCurrentItemPopulation("isolated complete secondary text-only fixture");
    f.bindSecondary(secondary);
    auto later = std::make_shared<doe::HistogramState>();
    later->graphOn = true;
    later->binWidth = 31;
    f.io->stateVarRuntime().histogramRegistrations.push_back(
        {"signalChangeHistoGraph", "slotChangeHistoGraph", later});
    f.dispatch();
    CHECK(!f.histogram().graphOn && !later->graphOn);
    CHECK_EQ(later->binWidth, 31);
    CHECK_EQ(f.notifications->removeGraphItemsRegistrations->size(), std::size_t{1});
    CHECK_EQ(secondary.owner->items().size(), std::size_t{2});
}

TEST_CASE(close_effects_isolated_histogram_uses_rebound_secondary_not_cached_scene)
{
    HistogramFixture f;
    Project secondaryProject, replacementProject;
    auto secondary = knownPostScene(secondaryProject);
    secondary.owner->commitInitialTextItems();
    secondary.owner->qualifyCurrentItemPopulation("isolated complete original secondary");
    f.bindSecondary(secondary);
    f.dispatch();
    auto manager = f.project.dataDb->postGraphManager;
    auto replacement = knownPostScene(replacementProject);
    replacement.owner->commitInitialTextItems(); // unknown complete population
    f.bindSecondary(replacement);
    f.histogram().graphOn = true;
    expectUnsupported([&] { f.dispatch(); }, "population coverage");
    CHECK(!f.histogram().graphOn);
    CHECK(f.project.dataDb->postGraphManager == manager);
    CHECK(manager->sceneOwner().lock() == f.scene.owner);
    CHECK_EQ(secondary.owner->items().size(), std::size_t{2});
    CHECK_EQ(replacement.owner->items().size(), std::size_t{2});
}

TEST_CASE(close_effects_isolated_remove_current_scenes_preserves_text_until_hide)
{
    RemoveGraphFixture f;
    f.qualifyBoth();
    auto* primaryText = f.primary.owner->databaseNameItem();
    auto* secondaryText = f.secondary.owner->stepNumberItem();
    primaryText->hidden = false;
    primaryText->displayedText = qt::L("SfGraphHistogramItem");
    secondaryText->textTemplate = qt::L("SfChartPtrkItem");
    f.manager->field30 = 72;
    f.manager->flag38 = true;
    f.notifications.flag90 = true;
    expectUnsupported([&] { f.dispatch(); }, "unconditional HideSecondary");
    CHECK(!f.notifications.flag90);
    CHECK(f.primary.owner->databaseNameItem() == primaryText);
    CHECK(f.secondary.owner->stepNumberItem() == secondaryText);
    CHECK_EQ(f.primary.owner->items().size(), std::size_t{2});
    CHECK_EQ(f.secondary.owner->items().size(), std::size_t{2});
    CHECK(!primaryText->hidden);
    CHECK(secondaryText->hidden);
    CHECK(primaryText->displayedText == qt::L("SfGraphHistogramItem"));
    CHECK(secondaryText->textTemplate == qt::L("SfChartPtrkItem"));
    CHECK(primaryText->displayOwner().lock() == f.primaryProject.display);
    CHECK(secondaryText->displayOwner().lock() == f.secondaryProject.display);
    CHECK(f.manager->sceneOwner().lock() == f.cached.owner);
    CHECK(f.cached.owner->items().empty()); // constructor cache was not queried
    CHECK_EQ(f.manager->field30, 72);
    CHECK(f.manager->flag38);
    CHECK(f.manager->graphItems().empty());
    CHECK(f.notifications.emptyKeys.empty());
    CHECK(f.notifications.attempts.empty());
}

TEST_CASE(close_effects_isolated_remove_resolves_both_scenes_before_item_traversal)
{
    RemoveGraphFixture f; // both actual collections have unavailable coverage
    f.window.secondaryScene.reset();
    expectUnsupported([&] { f.dispatch(); }, "current secondary scene binding");
    f.window.primaryScene.reset();
    expectUnsupported([&] { f.dispatch(); }, "current primary scene binding");
    f.window.primaryScene = f.primary;
    f.window.secondaryScene = f.secondary;
    expectUnsupported([&] { f.dispatch(); }, "c0c650 current item population");
    CHECK_EQ(f.primary.owner->items().size(), std::size_t{2});
    CHECK_EQ(f.secondary.owner->items().size(), std::size_t{2});
}

TEST_CASE(close_effects_isolated_remove_requires_each_current_scene_population)
{
    RemoveGraphFixture f;
    expectUnsupported([&] { f.dispatch(); }, "c0c650 current item population");
    f.primary.owner->qualifyCurrentItemPopulation("isolated primary only");
    expectUnsupported([&] { f.dispatch(); }, "c0c650 current item population");
    f.secondary.owner->qualifyCurrentItemPopulation("isolated secondary now covered");
    expectUnsupported([&] { f.dispatch(); }, "unconditional HideSecondary");
    f.primary.owner->invalidateItemPopulation();
    expectUnsupported([&] { f.dispatch(); }, "c0c650 current item population");
    CHECK_EQ(f.primary.owner->items().size(), std::size_t{2});
    CHECK_EQ(f.secondary.owner->items().size(), std::size_t{2});
}

TEST_CASE(close_effects_isolated_remove_null_scenes_still_reaches_hide)
{
    RemoveGraphFixture f;
    const engine::PostGraphSceneBinding absent{nullptr, "isolated qualified null scene"};
    f.window.primaryScene = absent;
    f.window.secondaryScene = absent;
    expectUnsupported([&] { f.dispatch(); }, "unconditional HideSecondary");
    f.window.primaryScene = f.primary;
    expectUnsupported([&] { f.dispatch(); }, "c0c650 current item population");
    f.window.primaryScene = absent;
    f.window.secondaryScene = f.secondary;
    f.secondary.owner->qualifyCurrentItemPopulation("isolated secondary text collection");
    expectUnsupported([&] { f.dispatch(); }, "unconditional HideSecondary");
    CHECK(f.primary.owner->itemPopulationQualification().empty());
    CHECK_EQ(f.primary.owner->items().size(), std::size_t{2});
}

TEST_CASE(close_effects_isolated_remove_missing_and_absent_global_are_distinct)
{
    RemoveGraphFixture f;
    expectUnsupported([&] { engine::removeGraphItems(f.notifications, std::nullopt); },
                      "current global mainwindow binding");
    f.window.qualification.clear();
    expectUnsupported([&] { f.dispatch(); }, "current global mainwindow binding");
    f.window.qualification = "isolated explicitly absent global mainwindow";
    f.window.mainWindowPresent = false;
    expectUnsupported([&] { f.dispatch(); }, "null global mainwindow would be dereferenced");
    CHECK(f.model->postGraphManager == f.manager);
    CHECK(f.notifications.removeGraphItemsRegistrations->front().lock() == f.manager);
}

TEST_CASE(close_effects_isolated_remove_rebinding_reads_actual_new_scene)
{
    RemoveGraphFixture f;
    f.qualifyBoth();
    expectUnsupported([&] { f.dispatch(); }, "unconditional HideSecondary");
    Project replacementProject;
    auto replacement = knownPostScene(replacementProject);
    replacement.owner->commitInitialTextItems();
    f.window.secondaryScene = replacement;
    expectUnsupported([&] { f.dispatch(); }, "c0c650 current item population");
    replacement.owner->qualifyCurrentItemPopulation("isolated replacement collection");
    expectUnsupported([&] { f.dispatch(); }, "unconditional HideSecondary");
    CHECK(f.manager->sceneOwner().lock() == f.cached.owner);
    CHECK_EQ(f.secondary.owner->items().size(), std::size_t{2});
    CHECK_EQ(replacement.owner->items().size(), std::size_t{2});
    CHECK(replacement.owner->databaseNameItem()->displayOwner().lock() == replacementProject.display);
}

TEST_CASE(close_effects_isolated_remove_skipped_receivers_do_not_require_global_context)
{
    RemoveGraphFixture f;
    f.notifications.suppressed = true;
    f.notifications.flag90 = true;
    engine::removeGraphItems(f.notifications, std::nullopt);
    CHECK(f.notifications.flag90);
    f.notifications.suppressed = false;
    f.model->postGraphManager.reset();
    f.manager.reset();
    REQUIRE(f.notifications.removeGraphItemsRegistrations->front().expired());
    engine::removeGraphItems(f.notifications, std::nullopt);
    CHECK(!f.notifications.flag90);
    CHECK_EQ(f.notifications.removeGraphItemsRegistrations->size(), std::size_t{1});
    CHECK(f.notifications.emptyKeys.empty());
    doe::SessionNotificationState empty;
    knownPostRegistrations(empty);
    engine::removeGraphItems(empty, std::nullopt);
    CHECK(empty.emptyKeys.count("2signalRemoveGraphItems()") == 1);
}

TEST_CASE(close_effects_isolated_selection_initial_depth_and_coverage_are_unknown)
{
    Project project;
    auto scene = knownPostScene(project).owner;
    scene->commitInitialTextItems();
    scene->qualifyCurrentItemPopulation("isolated complete hidden text population");
    CHECK(scene->currentSelectedItems().empty());
    CHECK(scene->selectionQualification().empty());
    CHECK(!scene->selectionChangeDepth());
    expectUnsupported([&] { scene->clearSelection(); }, "depth is unavailable");
    CHECK(!scene->clearSelectionPending());
    CHECK(!scene->selectionChangeDepth());
    CHECK_EQ(scene->items().size(), std::size_t{2});
}

TEST_CASE(close_effects_isolated_clear_empty_selection_preserves_actual_scene_items)
{
    Project project;
    auto scene = knownPostScene(project).owner;
    scene->commitInitialTextItems();
    auto* first = scene->databaseNameItem();
    auto* second = scene->stepNumberItem();
    first->hidden = false;
    first->displayedText = qt::L("current database label");
    scene->observeCurrentSelection({}, 0, "isolated complete current empty selection");
    scene->clearSelection();
    scene->clearSelection(); // a completed invocation can be called again
    REQUIRE(scene->selectionChangeDepth());
    CHECK_EQ(*scene->selectionChangeDepth(), std::uint32_t{0});
    CHECK(!scene->clearSelectionPending());
    CHECK(scene->currentSelectedItems().empty());
    CHECK(scene->itemPopulationQualification().empty()); // independent coverage
    CHECK(scene->databaseNameItem() == first);
    CHECK(scene->stepNumberItem() == second);
    CHECK_EQ(scene->items().size(), std::size_t{2});
    CHECK(!first->hidden);
    CHECK(second->hidden);
    CHECK(first->displayedText == qt::L("current database label"));
    CHECK(first->displayOwner().lock() == project.display);
}

TEST_CASE(close_effects_isolated_clear_empty_selection_preserves_nested_depth_bits)
{
    Project project;
    auto scene = knownPostScene(project).owner;
    for (const auto depth : {std::uint32_t{1}, std::uint32_t{7},
                            std::numeric_limits<std::uint32_t>::max()}) {
        scene->observeCurrentSelection({}, depth, "isolated complete current empty selection");
        scene->clearSelection();
        REQUIRE(scene->selectionChangeDepth());
        CHECK_EQ(*scene->selectionChangeDepth(), depth);
        CHECK(!scene->clearSelectionPending());
        CHECK(scene->currentSelectedItems().empty());
    }
}

TEST_CASE(close_effects_isolated_clear_unknown_selection_retains_single_depth_prefix)
{
    Project project;
    auto scene = knownPostScene(project).owner;
    scene->observeCurrentSelection({}, 6, ""); // depth known, selection coverage unknown
    expectUnsupported([&] { scene->clearSelection(); }, "coverage is unavailable");
    REQUIRE(scene->selectionChangeDepth());
    CHECK_EQ(*scene->selectionChangeDepth(), std::uint32_t{7});
    CHECK(scene->clearSelectionPending());
    expectUnsupported([&] { scene->clearSelection(); }, "continuation is pending");
    expectUnsupported([&] {
        scene->observeCurrentSelection({}, 6, "cannot overwrite suspended prefix");
    }, "continuation is pending");
    CHECK_EQ(*scene->selectionChangeDepth(), std::uint32_t{7});
    CHECK(scene->selectionQualification().empty());
}

TEST_CASE(close_effects_isolated_clear_nonempty_selection_stops_before_item_callback)
{
    Project project;
    auto scene = knownPostScene(project).owner;
    scene->commitInitialTextItems();
    auto* item = scene->databaseNameItem();
    item->hidden = false;
    item->displayedText = qt::L("selected text");
    scene->observeCurrentSelection({item, item}, 3, "isolated actual selected identity");
    CHECK_EQ(scene->currentSelectedItems().size(), std::size_t{1});
    expectUnsupported([&] { scene->clearSelection(); }, "setSelected callbacks");
    REQUIRE(scene->selectionChangeDepth());
    CHECK_EQ(*scene->selectionChangeDepth(), std::uint32_t{4});
    CHECK(scene->clearSelectionPending());
    CHECK(scene->currentSelectedItems().count(item) == 1);
    CHECK(scene->databaseNameItem() == item);
    CHECK(!item->hidden);
    CHECK(item->displayedText == qt::L("selected text"));
    expectUnsupported([&] { scene->clearSelection(); }, "continuation is pending");
    CHECK_EQ(*scene->selectionChangeDepth(), std::uint32_t{4});
}

TEST_CASE(close_effects_isolated_selection_invalidation_keeps_identities_and_depth)
{
    Project project;
    auto scene = knownPostScene(project).owner;
    scene->commitInitialTextItems();
    scene->qualifyCurrentItemPopulation("isolated complete item collection");
    auto* item = scene->stepNumberItem();
    scene->observeCurrentSelection({item}, 2, "isolated complete selected collection");
    scene->invalidateItemPopulation();
    CHECK(!scene->selectionQualification().empty());
    scene->invalidateCurrentSelection();
    CHECK(scene->selectionQualification().empty());
    CHECK(scene->currentSelectedItems().count(item) == 1);
    REQUIRE(scene->selectionChangeDepth());
    CHECK_EQ(*scene->selectionChangeDepth(), std::uint32_t{2});
    expectUnsupported([&] { scene->clearSelection(); }, "coverage is unavailable");
    CHECK(scene->currentSelectedItems().count(item) == 1);
    CHECK_EQ(*scene->selectionChangeDepth(), std::uint32_t{3});
}

TEST_CASE(close_effects_isolated_selection_binds_only_actual_scene_owned_identities)
{
    Project firstProject, secondProject;
    auto first = knownPostScene(firstProject).owner;
    auto second = knownPostScene(secondProject).owner;
    first->commitInitialTextItems();
    second->commitInitialTextItems();
    auto* selected = first->databaseNameItem();
    first->observeCurrentSelection({selected}, 5, "isolated first selection");
    second->observeCurrentSelection({}, 0, "isolated second selection");
    expectUnsupported([&] {
        second->observeCurrentSelection({selected}, 8, "foreign item");
    }, "not represented in this actual scene");
    expectUnsupported([&] {
        second->observeCurrentSelection({nullptr}, 8, "null item");
    }, "not represented in this actual scene");
    second->clearSelection();
    CHECK(first->currentSelectedItems().count(selected) == 1);
    CHECK_EQ(*first->selectionChangeDepth(), std::uint32_t{5});
    CHECK(!first->clearSelectionPending());
    CHECK(second->currentSelectedItems().empty());
    CHECK_EQ(*second->selectionChangeDepth(), std::uint32_t{0});
    CHECK(second->selectionQualification() == "isolated second selection");
}

int main() { return testkit::run_all(); }
