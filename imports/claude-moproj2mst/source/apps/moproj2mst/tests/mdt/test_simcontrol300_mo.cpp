// ctest-labels: m2m-mdt
#include <testkit/check.hpp>
#include <algorithm>
#include "m2m/engine/simcontrol300_lifecycle.hpp"
#include "m2m/engine.hpp"
#include "m2m/mst/writer.hpp" // Complete Project.registry's OpClass at destruction.
#include <defmodel/layout/db_step_data.hpp>

using namespace m2m;
using namespace m2m::engine::simcontrol300;
namespace {
struct Fixture {
    Project project;
    Operation operation;
    FlowItem* row;
    FlowItem* predecessor;
    Fixture()
    {
        project.subs.push_back(std::make_unique<SubProject>());
        project.subs[0]->project = &project;
        project.activeSub = 0;
        auto previous = std::make_unique<FlowItem>(); previous->opId = 2;
        predecessor = previous.get();
        auto current = std::make_unique<FlowItem>(); current->opId = 7;
        row = current.get();
        auto& items = project.subs[0]->flow.items;
        items.push_back(std::move(previous)); items.push_back(std::move(current));
        operation.project = &project; operation.id = 7;
    }
    void nonHead() { row->prev = predecessor; predecessor->next = row; }
};
StepCountContext moStep(Operation* operation, bool classSupports)
{
    struct StartClass final : mst::OpClass {
        StartClass(qt::String key, bool value) : OpClass(std::move(key)), value(value) {}
        bool value;
        bool writesDimNstart() const override { return value; }
    };
    if (operation) {
        REQUIRE(operation->project != nullptr);
        const auto key = qt::L(classSupports ? "TestStartClass" : "TestNoStartClass");
        auto& registry = operation->project->registry;
        if (!registry.contains(key)) registry[key] = std::make_unique<StartClass>(key, classSupports);
        operation->classKey = key;
        operation->opClass = registry[key].get();
        REQUIRE(opBase(*operation) != nullptr); // opBase reads registry, not the cached opClass field.
        REQUIRE(opBase(*operation)->writesDimNstart() == classSupports);
    }
    return {5, {2, 7}, MoStepCountContext{operation}};
}
struct NoFiles final : Extra3DIo {
    NoFiles() : Extra3DIo(1252) {}
    int removes = 0;
    int nbcReads = 0;
    bool readDoubles(const qt::String& path, std::span<double> output) override
    {
        std::fill(output.begin(), output.end(), 0.0);
        if (path == qt::L("NBC.DAT")) ++nbcReads;
        return false;
    }
    bool readInts(const qt::String&, std::span<int> output) override
    { std::fill(output.begin(), output.end(), 0); return false; }
    bool readSprings(std::vector<SpringRecord>& output) override { output.clear(); return false; }
    bool removeIfReadable(const qt::String&) override { ++removes; return true; }
    void nbcRemoveWarning() override {}
    std::unique_ptr<mst::OutputDevice> openWrite(const qt::String&) override { return nullptr; }
};
MoInitialObservations observedFeatures()
{
    MoInitialObservations observations;
    observations.generalFills[0] = MoFeatureAvailability{1, std::nullopt, {1, 1, 1}};
    observations.generalFills[1] = MoFeatureAvailability{1, std::nullopt, {1, 1, 1}};
    observations.startupHeating = {0, 1};
    return observations;
}
}

TEST_CASE(mo_pressure_features_keep_unknown_distinct_and_combine_both_startup_dimensions)
{
    CHECK(!pressureFieldsForStartupHeating({}));
    CHECK(!pressureFieldsForStartupHeating({0, std::nullopt}));
    CHECK(!pressureFieldsForStartupHeating({std::nullopt, 0}));
    const auto disabled = pressureFieldsForStartupHeating({0, 0}); REQUIRE(disabled);
    CHECK(!*disabled);
    for (const StartupHeatingFeatures features : {StartupHeatingFeatures{1, 0},
             StartupHeatingFeatures{0, 1}, StartupHeatingFeatures{255, std::nullopt},
             StartupHeatingFeatures{std::nullopt, 128}}) {
        const auto enabled = pressureFieldsForStartupHeating(features); REQUIRE(enabled);
        CHECK(*enabled);
    }
}

TEST_CASE(mo_feature_observations_require_only_queries_reached_by_original_short_circuit)
{
    Fixture f;
    CHECK(!generalContextForMoAvailability(f.project, 3, {}));
    MoFeatureAvailability observations{1, std::nullopt, {1, 1, 1}};
    REQUIRE(generalContextForMoAvailability(f.project, 3, observations));
    CHECK(!generalContextForMoAvailability(f.project, 2, observations));
    observations.microstructure3D = 0;
    CHECK(!generalContextForMoAvailability(f.project, 3, observations));
    observations.wizardHeatTreat3D = -1;
    auto disabled = generalContextForMoAvailability(f.project, 3, observations); REQUIRE(disabled);
    CHECK(!disabled->processEnabled[2]);
    observations.wizardHeatTreat3D = 7;
    auto enabled = generalContextForMoAvailability(f.project, 3, observations); REQUIRE(enabled);
    CHECK(enabled->processEnabled[2]);
    for (auto& result : observations.heating) {
        result.reset();
        CHECK(!generalContextForMoAvailability(f.project, 3, observations));
        result = 1;
    }
}

TEST_CASE(mo_feature_observations_preserve_three_distinct_heating_calls_and_remembered_controls)
{
    Fixture f; engine::GuiModel model(nullptr, 3); auto& sc = *model.simCtl(); sc.unit = 1;
    for (int& flag : sc.trans) flag = 1;
    sc.htmthd = 1;
    MoFeatureAvailability observations{-2, 9, {4, 0, -7}};
    auto context = generalContextForMoAvailability(f.project, 3, observations); REQUIRE(context);
    CHECK((context->processEnabled == std::array{true, true, true, false, false, true}));
    CHECK_EQ(context->applicationMode, 5); CHECK(context->moProject == &f.project);
    const auto form = fillGeneral(sc, *context); REQUIRE(form);
    // A later call has its own observations; it cannot change the earlier form.
    observations = {0, 0, {0, 5, 6}};
    const auto later = generalContextForMoAvailability(f.project, 3, observations); REQUIRE(later);
    CHECK((later->processEnabled == std::array{true, true, false, true, true, false}));
    for (int& flag : sc.trans) flag = 99;
    sc.htmthd = 99;
    REQUIRE(applyGeneral(sc, *form));
    CHECK_EQ(sc.trans[0], 1); CHECK_EQ(sc.trans[1], 1); CHECK_EQ(sc.trans[2], 1);
    CHECK_EQ(sc.trans[3], 0); CHECK_EQ(sc.trans[4], 0); CHECK_EQ(sc.htmthd, 1);
}

TEST_CASE(mo_general_hides_mesh_store_and_uses_final_source_qualified_enable_dependencies)
{
    Fixture f; engine::GuiModel model(nullptr, 3); auto& sc = *model.simCtl();
    sc.unit = 1; // Bare GuiModel init does not assign UNIT; this fixture supplies the loaded SI state.
    sc.meshno = 30000;
    for (int& flag : sc.trans) flag = 1;
    sc.trans[2] = 2; sc.htmthd = 3;
    const auto context = generalContextForMo(f.project, 3, {false, true});
    CHECK((context.processEnabled == std::array{true, true, false, true, true, true}));
    const auto form = fillGeneral(sc, context); REQUIRE(form);
    CHECK(!form->meshNumberEnabled); CHECK_EQ(form->meshNumber, 1);
    sc.meshno = 77; for (int& flag : sc.trans) flag = 99;
    REQUIRE(applyGeneral(sc, *form));
    CHECK_EQ(sc.meshno, 77); CHECK_EQ(sc.trans[0], 1); CHECK_EQ(sc.trans[1], 1);
    CHECK_EQ(sc.trans[2], 0); CHECK_EQ(sc.trans[3], 1); CHECK_EQ(sc.trans[4], 1);
    CHECK_EQ(sc.htmthd, 1); // This isolated General form has not received Environment's combo overwrite.
    const auto disabled = generalContextForMo(f.project, 3, {true, false});
    CHECK((disabled.processEnabled == std::array{true, true, true, false, false, false}));
}

TEST_CASE(mo_general_reads_active_subproject_override_at_save)
{
    Fixture f; engine::GuiModel model(nullptr, 3); auto& sc = *model.simCtl();
    sc.unit = 1;
    for (int& flag : sc.trans) flag = 1;
    sc.htmthd = 1;
    const auto form = fillGeneral(sc, generalContextForMo(f.project, 3, {true, true})); REQUIRE(form);
    for (int i = 0; i < 5; ++i) sc.trans[i] = 7 + i;
    sc.htmthd = 0; f.project.subs[0]->type = 2;
    REQUIRE(applyGeneral(sc, *form));
    for (int i = 0; i < 5; ++i) CHECK_EQ(sc.trans[i], 7 + i);
    CHECK_EQ(sc.htmthd, 0);
    f.project.subs[0]->type = 3; sc.trans[0] = 33;
    REQUIRE(applyGeneral(sc, *form)); CHECK_EQ(sc.trans[0], 33);
    f.project.subs[0]->type = 0;
    REQUIRE(applyGeneral(sc, *form));
    for (int flag : sc.trans) CHECK_EQ(flag, 1);
    CHECK_EQ(sc.htmthd, 1);
}

TEST_CASE(mo_step_count_head_sets_actual_operation_start_from_remembered_spin)
{
    Fixture f; REQUIRE(startsFromKey(f.operation));
    engine::GuiModel model(nullptr, 3); auto& sc = *model.simCtl();
    sc.nstart = 7; sc.nstep = 23; sc.stpinc = 3; sc.pdie1 = 7;
    f.operation.nstartGiven = 0; f.operation.startStepNo = 333;
    const auto form = fillStepCount(sc, moStep(&f.operation, true)); REQUIRE(form);
    CHECK_EQ(form->start, -8); CHECK(!form->nstartChecked); CHECK_EQ(form->primaryDie, 7);
    sc.nstart = 99; f.operation.startStepNo = 888;
    applyStepCount(sc, *form);
    CHECK_EQ(sc.nstart, -8); CHECK_EQ(f.operation.nstartGiven, 1); CHECK_EQ(f.operation.startStepNo, -8);
    CHECK_EQ(sc.nstep, 23); CHECK_EQ(sc.stpinc, 3);
}

TEST_CASE(mo_step_count_selected_positive_override_negates_sc_source_not_override)
{
    Fixture f; f.nonHead(); REQUIRE(!startsFromKey(f.operation));
    engine::GuiModel model(nullptr, 3); auto& sc = *model.simCtl();
    sc.nstart = -7; f.operation.nstartGiven = 1; f.operation.startStepNo = 10;
    const auto form = fillStepCount(sc, moStep(&f.operation, true)); REQUIRE(form);
    CHECK_EQ(form->start, 6); CHECK(form->nstartChecked); // -(SC.nstart+1), not -(10+1).
    sc.nstart = 99; f.operation.nstartGiven = 0; f.operation.startStepNo = 999;
    applyStepCount(sc, *form);
    CHECK_EQ(sc.nstart, 6); CHECK_EQ(f.operation.nstartGiven, 1); CHECK_EQ(f.operation.startStepNo, 6);
    sc.nstart = 7; f.operation.startStepNo = -88;
    const auto negative = fillStepCount(sc, moStep(&f.operation, true)); REQUIRE(negative);
    CHECK_EQ(negative->start, -88);
}

TEST_CASE(mo_step_count_unchecked_or_unsupported_class_preserves_unwritten_start_value)
{
    Fixture f; f.nonHead(); engine::GuiModel model(nullptr, 3); auto& sc = *model.simCtl();
    sc.nstart = 7; f.operation.nstartGiven = 0; f.operation.startStepNo = -88;
    const auto unchecked = fillStepCount(sc, moStep(&f.operation, true)); REQUIRE(unchecked);
    CHECK_EQ(unchecked->start, -8); CHECK(!unchecked->nstartChecked);
    f.operation.nstartGiven = 1; f.operation.startStepNo = 999;
    applyStepCount(sc, *unchecked);
    CHECK_EQ(f.operation.nstartGiven, 0); CHECK_EQ(f.operation.startStepNo, 999);
    sc.nstart = 5; f.operation.nstartGiven = 1;
    const auto unsupportedClass = fillStepCount(sc, moStep(&f.operation, false)); REQUIRE(unsupportedClass);
    CHECK_EQ(unsupportedClass->start, -6); CHECK(!unsupportedClass->nstartChecked);
    applyStepCount(sc, *unsupportedClass);
    CHECK_EQ(f.operation.nstartGiven, 0); CHECK_EQ(f.operation.startStepNo, 999);
    f.operation.classKey = qt::L("AbsentClass"); f.operation.opClass = nullptr;
    f.operation.nstartGiven = 1; sc.nstart = 4;
    const auto missingClass = fillStepCount(sc, {5, {}, MoStepCountContext{&f.operation}});
    REQUIRE(missingClass); CHECK_EQ(missingClass->start, -5);
    applyStepCount(sc, *missingClass); CHECK_EQ(f.operation.nstartGiven, 0);
    CHECK_EQ(f.operation.startStepNo, 999);
    sc.nstart = 3;
    const auto absentOperation = fillStepCount(sc, moStep(nullptr, false)); REQUIRE(absentOperation);
    applyStepCount(sc, *absentOperation); CHECK_EQ(sc.nstart, -4);
    CHECK(!fillStepCount(sc, {5, {}})); // Missing explicit MO binding still refuses.
}

TEST_CASE(mo_step_count_reselects_live_starts_from_key_but_remembers_checkbox)
{
    Fixture f; f.nonHead(); engine::GuiModel model(nullptr, 3); auto& sc = *model.simCtl();
    sc.nstart = 7; f.operation.nstartGiven = 0;
    const auto initial = fillStepCount(sc, moStep(&f.operation, true)); REQUIRE(initial);
    f.operation.restartGiven = 1; // Same source predicate as original startsFromKey.
    applyStepCount(sc, *initial);
    CHECK_EQ(f.operation.nstartGiven, 1); CHECK_EQ(f.operation.startStepNo, -8);
    const auto restart = fillStepCount(sc, moStep(&f.operation, true)); REQUIRE(restart);
    CHECK(!restart->nstartChecked);
    f.operation.restartGiven = 0; f.operation.startStepNo = 777;
    applyStepCount(sc, *restart);
    CHECK_EQ(f.operation.nstartGiven, 0); CHECK_EQ(f.operation.startStepNo, 777);
}

TEST_CASE(mo_persistent_session_composes_real_mode5_operation_and_shared_control_state)
{
    Fixture f; engine::GuiModel model(nullptr, 3); auto& sc = *model.simCtl();
    sc.unit = 1;
    sc.nstart = 7; sc.nstep = 23; sc.meshno = 30000; sc.htmthd = 3;
    RemeshState remesh; NoFiles io;
    const InitialEditorContext context{generalContextForMo(f.project, 3, {true, true}),
        moStep(&f.operation, true), NumericLocale::KnownC, true, observedFeatures()};
    auto session = openInitialEditor(model, remesh, io, context); REQUIRE(session);
    CHECK_EQ(f.operation.nstartGiven, 0); // Open has filled controls, not applied them.
    sc.nstart = 99; sc.nstep = 999; sc.meshno = 77;
    std::vector<RecoveryEntry> empty;
    const auto report = applyInitialEditorPageState(*session, model, remesh, io, &empty, &empty);
    CHECK(report.outcome == ApplyResult::Completed);
    CHECK_EQ(sc.nstart, -8); CHECK_EQ(sc.nstep, 23); CHECK_EQ(sc.meshno, 77);
    CHECK_EQ(f.operation.nstartGiven, 1); CHECK_EQ(f.operation.startStepNo, -8);
    CHECK_EQ(sc.htmthd, 3); CHECK_EQ(io.removes, 7);
    CHECK(!InitialEditorSession::completeForPublicSave);
}

TEST_CASE(mo_composer_remembers_independent_refill_observations_and_two_dimensional_pressure)
{
    Fixture f; engine::GuiModel model(nullptr, 3); auto& sc = *model.simCtl(); sc.unit = 1;
    for (int& flag : sc.trans) flag = 1;
    sc.htmthd = 1; sc.envmpr = 1.23456789; sc.envmpt = 2.34567891;
    auto observations = observedFeatures();
    observations.generalFills[1] = MoFeatureAvailability{0, -1, {0, 0, 0}};
    observations.startupHeating = {1, 0}; //2D-only startup feature still fills3D pressure.
    InitialEditorContext context{generalContextForMo(f.project, 3, {true, true}),
        moStep(&f.operation, true), NumericLocale::KnownC, false, observations};
    RemeshState remesh; NoFiles io;
    auto session = openInitialEditor(model, remesh, io, context); REQUIRE(session);
    CHECK((session->pages.general.processEnabled == std::array{true, true, false, false, false, false}));
    CHECK(session->pages.environment.scalars[0].state == ValidatorState::Acceptable);
    CHECK(session->pages.environment.scalars[0].text == qt::L("1.23457"));
    CHECK_EQ(io.nbcReads, 1);
    context.moObservations.reset(); // Save must use remembered controls, not re-query.
    for (int& flag : sc.trans) flag = 99;
    sc.htmthd = 99; sc.envmpr = sc.envmpt = 99.0;
    std::vector<RecoveryEntry> empty;
    const auto report = applyInitialEditorPageState(*session, model, remesh, io, &empty, &empty);
    CHECK(report.outcome == ApplyResult::Completed);
    CHECK_EQ(sc.trans[0], 1); CHECK_EQ(sc.trans[1], 1);
    CHECK_EQ(sc.trans[2], 0); CHECK_EQ(sc.trans[3], 0); CHECK_EQ(sc.trans[4], 0); CHECK_EQ(sc.htmthd, 0);
    CHECK(sc.envmpr == 1.23457); CHECK(sc.envmpt == 2.34568);
}

TEST_CASE(mo_composer_observed_zero_pressure_is_supported_and_keeps_fresh_empty_controls)
{
    Fixture f; engine::GuiModel model(nullptr, 3); auto& sc = *model.simCtl(); sc.unit = 1;
    sc.envmpr = 12.3456789; sc.envmpt = 23.4567891;
    auto observations = observedFeatures(); observations.startupHeating = {0, 0};
    InitialEditorContext context{generalContextForMo(f.project, 3, {true, true}),
        moStep(&f.operation, true), NumericLocale::KnownC, true, observations};
    RemeshState remesh; NoFiles io;
    auto session = openInitialEditor(model, remesh, io, context); REQUIRE(session);
    CHECK(session->pages.environment.scalars[0].text.isEmpty());
    CHECK(session->pages.environment.scalars[0].state == ValidatorState::Intermediate);
    CHECK(session->pages.environment.scalars[1].text.isEmpty());
    CHECK(sc.envmpr == 12.3456789); CHECK(sc.envmpt == 23.4567891);
    std::vector<RecoveryEntry> empty;
    CHECK(applyInitialEditorPageState(*session, model, remesh, io, &empty, &empty).outcome == ApplyResult::Completed);
    CHECK(sc.envmpr == 0.0); CHECK(sc.envmpt == 0.0);
}

TEST_CASE(mo_composer_refuses_missing_observations_at_their_actual_constructor_stage)
{
    Fixture f; engine::GuiModel model(nullptr, 3); auto& sc = *model.simCtl(); sc.unit = 1;
    sc.smode = 99; sc.trans[0] = sc.trans[1] = 1;
    InitialEditorContext context{generalContextForMo(f.project, 3, {true, true}),
        moStep(&f.operation, true), NumericLocale::KnownC, true};
    RemeshState remesh; NoFiles io;
    CHECK(!openInitialEditor(model, remesh, io, context)); CHECK_EQ(sc.smode, 99); CHECK_EQ(io.nbcReads, 0);
    context.moObservations = observedFeatures();
    context.moObservations->generalFills[0].reset();
    CHECK(!openInitialEditor(model, remesh, io, context)); CHECK_EQ(sc.smode, 99);
    context.moObservations = observedFeatures();
    context.moObservations->startupHeating = {};
    CHECK(!openInitialEditor(model, remesh, io, context));
    CHECK_EQ(sc.smode, 3); CHECK_EQ(io.nbcReads, 0); // General ran; Environment refused beforeExtra3D.
    context.moObservations = observedFeatures();
    context.moObservations->generalFills[1].reset();
    CHECK(!openInitialEditor(model, remesh, io, context));
    CHECK_EQ(sc.smode, 3); CHECK_EQ(io.nbcReads, 1); // Initial pageI/O precedes backed-up refresh refusal.
    CHECK_EQ(io.removes, 0); // No Save page effects occurred.
}

int main() { return testkit::run_all(); }
