// ctest-labels: m2m-mdt mdt-l6 m2m-mdt-l7
#include <testkit/check.hpp>
#include <functional>
#include <string>
#include <memory>
#include "m2m/common.hpp"
#include "m2m/doe_io.hpp"
#include "m2m/doe_session_notifications.hpp"
#include "m2m/doe_statevar.hpp"
#include "m2m/doe_stepdata.hpp"
#include "m2m/engine.hpp"
#include "m2m/engine/close_effects.hpp"
using namespace m2m;
namespace {
doe::StateVarConstructorContext profile()
{
    doe::StateVarConstructorContext context;
    context.profileIdentity = "declared apply-seam unit profile; not original capture";
    context.globals.emplace();
    context.deflectionScaleType.observation = doe::SettingObservation::Absent;
    context.deflectionScale.observation = doe::SettingObservation::Absent;
    context.deflectionLineThickness.observation = doe::SettingObservation::Absent;
    context.showDeflectionReference.observation = doe::SettingObservation::Absent;
    context.transparentDeflectionReference.observation = doe::SettingObservation::Absent;
    context.translatedNoneTitle = qt::L("Explicit apply None translation");
    return context;
}
bool refuses(const auto& action, const char* text)
{
    try { action(); }
    catch (const Error& e) { return e.code == M2M_E_UNSUPPORTED && std::string(e.what()).find(text) != std::string::npos; }
    return false;
}
struct Probe final : engine::StateVarPlotEffects {
    std::function<void(engine::GuiModel&, doe::StateVarState&)> first, second;
    void changePtrkGraph(engine::GuiModel& m, doe::StateVarState& s) override {
        if (!first) throw Error(M2M_E_UNSUPPORTED, "unexpected first probe");
        first(m, s);
    }
    void changeHistoGraph(engine::GuiModel& m, doe::StateVarState& s) override {
        if (!second) throw Error(M2M_E_UNSUPPORTED, "unexpected second probe");
        second(m, s);
    }
};
} // namespace

TEST_CASE(doe_statevar_explicit_apply_notify_false_ignores_effects_and_does_not_reset_histogram)
{
    engine::GuiModel model(nullptr, 2);
    auto io = doe::makeRecordingStudyIo();
    auto context = profile();
    Probe effects;
    doe::applyStateVarSelection(model, 0, 13, false, &context, io.get(), &effects);
    REQUIRE(model.doeStepData && model.doeStepData->stateVar);
    const auto state = model.doeStepData->stateVar;
    CHECK(state->plotVar == 0 && state->plotCompNo == 13 && state->type == 2);
    CHECK(state->plotTitle == *context.translatedNoneTitle && state->maximum == 1);
    CHECK(!state->histogram && !state->plotApplyInProgress && !state->postparsePending);
    CHECK(!model.doeStepData->propertyTableCreated() && !model.viewer);
    CHECK(io->observedSessionNotificationState() == nullptr);
}

TEST_CASE(doe_statevar_explicit_apply_missing_first_dispatch_retains_stores_before_type_work)
{
    engine::GuiModel model(nullptr, 2);
    auto io = doe::makeRecordingStudyIo();
    auto context = profile();
    io->sessionNotificationState().flag90 = true;
    CHECK(refuses([&] { doe::applyStateVarSelection(model, 0, 17, true, &context, io.get()); },
        "signalChangePtrkGraph"));
    REQUIRE(model.doeStepData && model.doeStepData->stateVar);
    const auto state = model.doeStepData->stateVar;
    CHECK(state->plotVar == 0 && state->plotCompNo == 17 && state->type == 1);
    CHECK(state->cachedVariable == -1 && !state->cachedDataPresent);
    CHECK(state->plotApplyInProgress && state->postparsePending && !state->histogram);
    CHECK(state->plotTitle.isNull() && state->maximum == 0);
    CHECK(!io->sessionNotificationState().flag90);
}

TEST_CASE(doe_statevar_first_typed_failure_observes_actual_state_after_cache_clear)
{
    engine::GuiModel model(nullptr, 2);
    auto io = doe::makeRecordingStudyIo();
    auto context = profile();
    doe::applyStateVarSelection(model, 0, 0, false, &context, io.get());
    auto state = model.doeStepData->stateVar;
    state->plotVar = 1000;
    state->type = 77;
    state->cachedVariable = 9;
    state->cachedDataPresent = true;
    state->minimum = -4;
    state->maximum = 8;
    Probe effects;
    effects.first = [&](auto& owner, auto& actual) {
        CHECK(&owner == &model && &actual == state.get());
        CHECK(actual.plotVar == 0 && actual.plotCompNo == 7);
        CHECK(!actual.cachedDataPresent && actual.cachedVariable == -1);
        CHECK(actual.type == 77 && actual.minimum == -4 && actual.maximum == 8);
        throw Error(M2M_E_UNSUPPORTED, "declared first hook failure");
    };
    CHECK(refuses([&] { doe::applyStateVarSelection(model, 0, 7, true, &context, io.get(), &effects); },
        "declared first hook failure"));
    CHECK(state->plotApplyInProgress && state->type == 77 && state->maximum == 8);
}

TEST_CASE(doe_statevar_second_typed_failure_follows_range_prefix_and_separate_flag_reset)
{
    engine::GuiModel model(nullptr, 2);
    auto io = doe::makeRecordingStudyIo();
    auto context = profile();
    Probe effects;
    effects.first = [&](auto&, auto& actual) {
        CHECK(actual.type == 1 && actual.maximum == 0);
        actual.rangeMin = 41; //declared probe effect, not an original receiver claim
        io->sessionNotificationState().flag90 = true;
    };
    effects.second = [&](auto& owner, auto& actual) {
        CHECK(&owner == &model && &actual == model.doeStepData->stateVar.get());
        CHECK(actual.type == 2 && actual.minimum == 0 && actual.maximum == 1 && actual.rangeMin == 41);
        CHECK(actual.plotTitle == *context.translatedNoneTitle);
        CHECK(!io->sessionNotificationState().flag90);
        throw Error(M2M_E_UNSUPPORTED, "declared second hook failure");
    };
    CHECK(refuses([&] { doe::applyStateVarSelection(model, 0, 0, true, &context, io.get(), &effects); },
        "declared second hook failure"));
    CHECK(model.doeStepData->stateVar->plotApplyInProgress);
    CHECK(io->stateVarRuntime().legends.empty());
}

TEST_CASE(doe_statevar_typed_receiver_mutations_survive_and_normal_return_clears_apply_marker)
{
    engine::GuiModel model(nullptr, 2);
    auto io = doe::makeRecordingStudyIo();
    auto context = profile();
    Probe effects;
    effects.first = [](auto&, auto& actual) { actual.rangeMin = 6; };
    effects.second = [](auto&, auto& actual) {
        CHECK(actual.rangeMin == 6 && actual.type == 2 && actual.maximum == 1);
        actual.rangeMax = 9;
    };
    doe::applyStateVarSelection(model, 0, 0, true, &context, io.get(), &effects);
    const auto state = model.doeStepData->stateVar;
    CHECK(state->rangeMin == 6 && state->rangeMax == 9);
    CHECK(!state->plotApplyInProgress && !state->postparsePending);
    CHECK(io->reads().empty() && io->textWrites().empty());
}

TEST_CASE(doe_statevar_live_shared_suppression_is_rechecked_for_each_notify)
{
    for (const bool initiallySuppressed : {false, true}) {
        engine::GuiModel model(nullptr, 2);
        auto io = doe::makeRecordingStudyIo();
        auto context = profile();
        auto shared = std::make_shared<doe::SessionNotificationState>();
        shared->suppressed = initiallySuppressed;
        shared->flag90 = true;
        io->bindSessionNotificationState(shared);
        Probe effects;
        effects.first = [&](auto&, auto&) { shared->suppressed = true; shared->flag90 = true; };
        //If initially suppressed, null dispatcher is valid: the source returns
        //before receiver lookup. Otherwise first effect suppresses the second.
        doe::applyStateVarSelection(model, 0, 0, true, &context, io.get(),
            initiallySuppressed ? nullptr : &effects);
        CHECK(shared->suppressed && shared->flag90);
        CHECK(!model.doeStepData->stateVar->plotApplyInProgress);
    }
}

TEST_CASE(doe_statevar_explicit_apply_view_guard_precedes_argument_stores_or_notifier_state)
{
    engine::GuiModel model(nullptr, 2);
    auto io = doe::makeRecordingStudyIo();
    auto context = profile();
    doe::applyStateVarSelection(model, 0, 3, false, &context, io.get());
    const auto state = model.doeStepData->stateVar;
    state->plotVar = 1000;
    state->cachedVariable = 9;
    state->cachedDataPresent = true;
    float minimum[3]{}, maximum[3]{};
    CHECK(engine::viewerBox(model, 1, minimum, maximum));
    CHECK(refuses([&] { doe::applyStateVarSelection(model, 0, 17, true, &context, io.get()); }, "view normalization"));
    CHECK(state->plotVar == 1000 && state->plotCompNo == 3);
    CHECK(state->cachedVariable == 9 && state->cachedDataPresent && state->plotApplyInProgress);
    CHECK(io->observedSessionNotificationState() == nullptr);
}

int main() { return testkit::run_all(); }
