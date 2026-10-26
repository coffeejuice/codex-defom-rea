// ctest-labels: m2m-mdt close-busy-prefix
#include <testkit/check.hpp>

#include <memory>
#include <string>
#include <type_traits>

#include "m2m/common.hpp"
#include "m2m/doe_session_notifications.hpp"
#include "m2m/engine.hpp"
#include "m2m/engine/close_busy_prefix.hpp"
#include "m2m/engine/close_entry_prefix.hpp"
#include "m2m/engine/conversion_session.hpp"
#include "m2m/engine/model_manager.hpp"
#include "m2m/mst/writer.hpp"

using namespace m2m;
using namespace m2m::engine;
namespace {
template<class F> bool refuses(F&& body, const char* detail)
{
    try { body(); }
    catch (const Error& error) {
        return error.code == M2M_E_UNSUPPORTED &&
            std::string(error.what()).find(detail) != std::string::npos;
    }
    return false;
}
}

static_assert(!std::is_copy_constructible_v<CloseBusyGlobalOwner>);
static_assert(!std::is_move_constructible_v<CloseBusyGlobalOwner>);
static_assert(!std::is_default_constructible_v<CloseBusyNativeFields>);

TEST_CASE(close_busy_true_prefix_commits_three_bytes_preserving_nondefault_counter)
{
    DiagramEffectDriver effects;
    ConversionSession session(effects);
    CloseBusyGlobalOwner state(session, {6, 8, 9, 0xfedcba98u},
        closeBusyTrueSourceOccurrence, "declared fixture at accepted true setter entry");
    CHECK(state.stage() == CloseBusyPrefixStage::BeforeTrueStore);
    CHECK(state.fields().flag9 == 6);
    CHECK(state.fields().fieldA == 8);
    CHECK(state.fields().fieldB == 9);
    const auto result = attemptCloseBusyTruePrefix(session, state);
    CHECK(result.stop == CloseBusyPrefixStop::TreeEnableSelectionDispatchUnavailable);
    CHECK(result.prefixCommittedThisAttempt);
    CHECK(!result.selectionEnabledArgument);
    CHECK(state.fields().flag9 == 1);
    CHECK(state.fields().fieldA == 0);
    CHECK(state.fields().fieldB == 0);
    CHECK(state.fields().counter143d91230 == 0xfedcba98u);
    CHECK(state.stage() == CloseBusyPrefixStage::BeforeTreeEnableSelectionDispatch);
}

TEST_CASE(close_busy_true_prefix_retry_retains_pending_notifier_without_replaying_stores)
{
    DiagramEffectDriver effects;
    ConversionSession session(effects);
    CloseBusyGlobalOwner state(session, {0, 1, 1, 17},
        closeBusyTrueSourceOccurrence, "declared fixture true entry");
    CHECK(attemptCloseBusyTruePrefix(session, state).prefixCommittedThisAttempt);
    const auto second = attemptCloseBusyTruePrefix(session, state);
    const auto third = attemptCloseBusyTruePrefix(session, state);
    CHECK(!second.prefixCommittedThisAttempt);
    CHECK(!third.prefixCommittedThisAttempt);
    CHECK(second.stop == CloseBusyPrefixStop::TreeEnableSelectionDispatchUnavailable);
    CHECK(third.stop == second.stop);
    CHECK(std::string(third.nextSourceSite) == second.nextSourceSite);
    CHECK(!third.selectionEnabledArgument);
    CHECK(state.fields().counter143d91230 == 17);
}

TEST_CASE(close_busy_true_prefix_does_not_assume_or_execute_tree_notifier_population)
{
    DiagramEffectDriver effects;
    ConversionSession session(effects);
    auto notifications = session.sessionNotificationStateOwner();
    REQUIRE(notifications);
    notifications->flag90 = true;
    notifications->emptyKeys.insert("fixture-existing-key");
    const auto keys = notifications->emptyKeys;
    const auto attempts = notifications->attempts.size();
    CloseBusyGlobalOwner state(session, {0, 0, 0, 77},
        closeBusyTrueSourceOccurrence, "declared fixture unsuppressed true entry");
    const auto result = attemptCloseBusyTruePrefix(session, state);
    CHECK(result.stop == CloseBusyPrefixStop::TreeEnableSelectionDispatchUnavailable);
    CHECK(notifications->flag90);
    CHECK(notifications->emptyKeys == keys);
    CHECK(notifications->attempts.size() == attempts);
    CHECK(notifications->emptyKeys.count("2signalTreeEnableSelection()") == 0);
}

TEST_CASE(close_busy_true_prefix_suppression_is_a_later_dependency_not_an_escape)
{
    DiagramEffectDriver effects;
    ConversionSession session(effects);
    auto notifications = session.sessionNotificationStateOwner();
    notifications->suppressed = true;
    notifications->flag90 = true;
    CloseBusyGlobalOwner state(session, {0, 2, 3, 41},
        closeBusyTrueSourceOccurrence, "declared fixture suppressed true entry");
    CHECK(attemptCloseBusyTruePrefix(session, state).stop ==
          CloseBusyPrefixStop::TreeEnableSelectionDispatchUnavailable);
    CHECK(notifications->suppressed);
    CHECK(notifications->flag90);
    CHECK(state.fields().flag9 == 1);
}

TEST_CASE(close_busy_true_prefix_preserves_actual_project_manager_pending_and_current_opi)
{
    DiagramEffectDriver effects;
    ConversionSession session(effects);
    session.prepareProjectModel(qt::L("C:/PROBLEM/CloseBusy/fixture.DB"));
    auto manager = session.modelManagerOwner();
    auto model = session.project().dataDb;
    auto* project = session.liveProject();
    const auto connections = manager->connections().size();
    const auto pending = session.pending().size();
    Operation operation;
    mst::OpClass cls(qt::L("BusyProbe"));
    cls.current = &operation;
    const auto guard = operation.operationInfoIdentity().guard();
    CloseEntryBusyObservations facts{&session, closeEntrySourceOccurrence,
        CloseEntryByteObservation{0, "declared fixture global+9"},
        CloseEntryBoolObservation{false, "declared fixture null global window"}, {}};
    REQUIRE(attemptCloseEntryPrefix(session, facts).stop ==
            CloseEntryPrefixStop::BeforeBusyTransitionEffects);
    CloseBusyGlobalOwner state(session, {0, 1, 1, 5},
        closeBusyTrueSourceOccurrence, "declared fixture after all child guards allowed");
    attemptCloseBusyTruePrefix(session, state);
    CHECK(facts.globalFlag9->value == 0); // observations are not mutable storage
    CHECK(manager->size() == 1);
    CHECK(manager->currentModel() == nullptr);
    CHECK(manager->connections().size() == connections);
    CHECK(session.pending().size() == pending);
    CHECK(session.liveProject() == project);
    CHECK(session.project().dataDb == model);
    CHECK(model->moProject == nullptr);
    CHECK(cls.current == &operation);
    CHECK(guard.get() == &operation);
    CHECK(guard.expiry() == OperationInfoExpiry::Live);
}

TEST_CASE(close_busy_true_prefix_rejects_foreign_live_session_before_any_store)
{
    DiagramEffectDriver effectsA, effectsB;
    ConversionSession sessionA(effectsA), sessionB(effectsB);
    CloseBusyGlobalOwner state(sessionA, {4, 5, 6, 7},
        closeBusyTrueSourceOccurrence, "declared fixture bound to session A");
    CHECK(refuses([&] { attemptCloseBusyTruePrefix(sessionB, state); }, "identity"));
    CHECK(state.stage() == CloseBusyPrefixStage::BeforeTrueStore);
    CHECK(state.fields().flag9 == 4);
    CHECK(state.fields().fieldA == 5);
    CHECK(state.fields().fieldB == 6);
    CHECK(state.fields().counter143d91230 == 7);
}

TEST_CASE(close_busy_true_prefix_requires_exact_source_and_nonblank_qualification)
{
    DiagramEffectDriver effects;
    ConversionSession session(effects);
    CHECK(refuses([&] { CloseBusyGlobalOwner state(session, {0, 1, 1, 8},
        "0x140029c9b:0x1402ca9f0(false)", "declared fixture wrong false setter"); }, "occurrence"));
    CHECK(refuses([&] { CloseBusyGlobalOwner state(session, {0, 1, 1, 8},
        closeBusyTrueSourceOccurrence, " \t\r\n"); }, "qualification"));
}

TEST_CASE(close_busy_true_prefix_cannot_rebind_after_original_session_dies)
{
    std::unique_ptr<CloseBusyGlobalOwner> state;
    std::shared_ptr<ModelManager> retainedManager;
    {
        DiagramEffectDriver originalEffects;
        ConversionSession original(originalEffects);
        retainedManager = original.modelManagerOwner();
        state = std::make_unique<CloseBusyGlobalOwner>(original,
            CloseBusyNativeFields{3, 4, 5, 6}, closeBusyTrueSourceOccurrence,
            "declared fixture original session owner");
    }
    // An external manager lease must not authorize the dead Session's state
    // for another live Session, even though that lease still locks successfully.
    DiagramEffectDriver replacementEffects;
    ConversionSession replacement(replacementEffects);
    REQUIRE(retainedManager != replacement.modelManagerOwner());
    CHECK(refuses([&] { attemptCloseBusyTruePrefix(replacement, *state); }, "identity"));
    CHECK(state->stage() == CloseBusyPrefixStage::BeforeTrueStore);
    CHECK(state->fields().flag9 == 3);
    CHECK(state->fields().counter143d91230 == 6);
}

int main() { return testkit::run_all(); }
