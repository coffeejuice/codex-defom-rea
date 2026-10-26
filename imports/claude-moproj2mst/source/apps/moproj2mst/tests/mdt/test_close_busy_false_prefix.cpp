// ctest-labels: m2m-mdt close-busy-false-prefix
#include <testkit/check.hpp>

#include <memory>
#include <string>
#include <type_traits>

#include "m2m/common.hpp"
#include "m2m/doe_session_notifications.hpp"
#include "m2m/engine.hpp"
#include "m2m/engine/close_busy_false_prefix.hpp"
#include "m2m/engine/conversion_session.hpp"
#include "m2m/engine/model_manager.hpp"
#include "m2m/mst/writer.hpp"

using namespace m2m;
using namespace m2m::engine;
namespace {
CloseBusyFalseMainObservations mainFacts(const ConversionSession& session, int index, int field270)
{
    CloseBusyFalseMainObservations facts;
    facts.sessionOwner = &session;
    facts.sourceOccurrence = closeBusyFalseSourceOccurrence;
    facts.globalMainPresent = true;
    facts.globalMainQualification = "declared fixture late global main pointer";
    facts.mainIdentity = 77;
    facts.mainBindingQualification = "declared fixture current main and recovered table profile";
    facts.mainProfile = closeBusyFalseMainProfile;
    facts.stackedCurrentIndex = CloseBusyFalseMainField{index, 77, "declared fixture actual main+480 index"};
    facts.mainField270 = CloseBusyFalseMainField{field270, 77, "declared fixture actual same-main+270"};
    return facts;
}
template<class F> bool refuses(F&& body, const char* detail)
{
    try { body(); }
    catch (const Error& error) {
        return error.code == M2M_E_UNSUPPORTED && std::string(error.what()).find(detail) != std::string::npos;
    }
    return false;
}
}

static_assert(!std::is_copy_constructible_v<CloseBusyFalseGlobalOwner>);
static_assert(!std::is_move_constructible_v<CloseBusyFalseGlobalOwner>);

TEST_CASE(close_false_missing_main_retains_early_busy_counter_stores_before_guard)
{
    DiagramEffectDriver effects;
    ConversionSession session(effects);
    CloseBusyFalseGlobalOwner state(session, {9, 8, 7, 0xfedcba98u},
        closeBusyFalseSourceOccurrence, "declared fixture late false entry");
    const auto result = attemptCloseBusyFalsePrefix(session, state, {});
    CHECK(result.stop == CloseBusyFalseStop::MainBusyFactsUnavailable);
    CHECK(result.resetCommittedThisAttempt);
    CHECK(!result.branchCommittedThisAttempt);
    CHECK(!result.selectionEnabledArgument);
    CHECK(state.fields().flag9 == 0);
    CHECK(state.fields().counter143d91230 == 0);
    CHECK(state.fields().fieldA == 8);
    CHECK(state.fields().fieldB == 7);
    CHECK(state.stage() == CloseBusyFalseStage::AwaitMainBusyFacts);
}

TEST_CASE(close_false_qualified_null_main_enables_without_profile_or_field_queries)
{
    DiagramEffectDriver effects;
    ConversionSession session(effects);
    CloseBusyFalseGlobalOwner state(session, {1, 0, 0, 5},
        closeBusyFalseSourceOccurrence, "declared fixture late null main");
    CloseBusyFalseMainObservations facts;
    facts.sessionOwner = &session;
    facts.sourceOccurrence = closeBusyFalseSourceOccurrence;
    facts.globalMainPresent = false;
    facts.globalMainQualification = "declared fixture observed null global main";
    const auto result = attemptCloseBusyFalsePrefix(session, state, facts);
    CHECK(result.stop == CloseBusyFalseStop::TreeEnableSelectionDispatchUnavailable);
    REQUIRE(result.selectionEnabledArgument);
    CHECK(*result.selectionEnabledArgument);
    CHECK(result.branchCommittedThisAttempt);
    CHECK(state.fields().fieldA == 1);
    CHECK(state.fields().fieldB == 1);
}

TEST_CASE(close_false_index_two_short_circuits_missing_main270_and_requests_disabled)
{
    DiagramEffectDriver effects;
    ConversionSession session(effects);
    CloseBusyFalseGlobalOwner state(session, {1, 1, 1, 17},
        closeBusyFalseSourceOccurrence, "declared fixture still busy main");
    auto facts = mainFacts(session, 2, 0);
    facts.mainField270.reset();
    const auto result = attemptCloseBusyFalsePrefix(session, state, facts);
    REQUIRE(result.selectionEnabledArgument);
    CHECK(!*result.selectionEnabledArgument);
    CHECK(result.stop == CloseBusyFalseStop::TreeEnableSelectionDispatchUnavailable);
    CHECK(state.fields().flag9 == 0);
    CHECK(state.fields().counter143d91230 == 0);
    CHECK(state.fields().fieldA == 0);
    CHECK(state.fields().fieldB == 0);
}

TEST_CASE(close_false_non_two_index_requires_same_main270_equality_one)
{
    DiagramEffectDriver effects;
    ConversionSession session(effects);
    for (const int value : {1, 0, 2, -1}) {
        CloseBusyFalseGlobalOwner state(session, {1, 6, 7, 18},
            closeBusyFalseSourceOccurrence, "declared fixture raw main270 values");
        const auto result = attemptCloseBusyFalsePrefix(session, state, mainFacts(session, 0, value));
        REQUIRE(result.selectionEnabledArgument);
        CHECK(*result.selectionEnabledArgument == (value != 1));
        CHECK(state.fields().fieldA == (value != 1 ? 1 : 0));
        CHECK(state.fields().fieldB == (value != 1 ? 1 : 0));
    }
}

TEST_CASE(close_false_missing_foreign_or_unqualified_main_facts_keep_branch_stores_pending)
{
    DiagramEffectDriver effects;
    ConversionSession session(effects);
    for (int variant = 0; variant < 12; ++variant) {
        CloseBusyFalseGlobalOwner state(session, {1, 8, 9, 19},
            closeBusyFalseSourceOccurrence, "declared fixture unavailable branch facts");
        auto facts = mainFacts(session, 0, 1);
        switch (variant) {
        case 0: facts.stackedCurrentIndex.reset(); break;
        case 1: facts.mainField270.reset(); break;
        case 2: facts.mainField270->mainIdentity = 88; break;
        case 3: facts.mainProfile = "unresolved different main vtable"; break;
        case 4: facts.stackedCurrentIndex->mainIdentity = 88; break;
        case 5: facts.mainBindingQualification = " \t"; break;
        case 6: facts.sourceOccurrence = closeBusyTrueSourceOccurrence; break;
        case 7: facts.mainField270->qualification.clear(); break;
        case 8: facts.mainIdentity = 0; break;
        case 9: facts.globalMainQualification = " \r\n"; break;
        case 10: facts.globalMainPresent.reset(); break;
        case 11: facts.stackedCurrentIndex->qualification = " \t"; break;
        }
        const auto result = attemptCloseBusyFalsePrefix(session, state, facts);
        CHECK(result.stop == CloseBusyFalseStop::MainBusyFactsUnavailable);
        CHECK(state.fields().flag9 == 0);
        CHECK(state.fields().counter143d91230 == 0);
        CHECK(state.fields().fieldA == 8);
        CHECK(state.fields().fieldB == 9);
        const auto expected = variant == 1 || variant == 2 || variant == 7
            ? CloseBusyFalseStage::AwaitMainField270
            : variant == 0 || variant == 4 || variant == 11
                ? CloseBusyFalseStage::AwaitStackedCurrentIndex
                : CloseBusyFalseStage::AwaitMainBusyFacts;
        CHECK(state.stage() == expected);
    }
}

TEST_CASE(close_false_retry_latches_completed_index_before_unavailable_main270)
{
    DiagramEffectDriver effects;
    ConversionSession session(effects);
    CloseBusyFalseGlobalOwner state(session, {1, 8, 9, 25},
        closeBusyFalseSourceOccurrence, "declared fixture partial busy query");
    auto facts = mainFacts(session, 0, 0);
    facts.mainField270.reset();
    CHECK(attemptCloseBusyFalsePrefix(session, state, facts).stop == CloseBusyFalseStop::MainBusyFactsUnavailable);
    CHECK(state.stage() == CloseBusyFalseStage::AwaitMainField270);
    REQUIRE(state.observedStackedIndex());
    CHECK(*state.observedStackedIndex() == 0);
    facts = mainFacts(session, 2, 0); // later index is not the earlier native query
    const auto result = attemptCloseBusyFalsePrefix(session, state, facts);
    REQUIRE(result.selectionEnabledArgument);
    CHECK(*result.selectionEnabledArgument); // uses latched0 then main270=0
    CHECK(*state.observedStackedIndex() == 0);
    CHECK(!result.resetCommittedThisAttempt);
}

TEST_CASE(close_false_retry_retains_resolved_main_identity_before_index_read)
{
    DiagramEffectDriver effects;
    ConversionSession session(effects);
    CloseBusyFalseGlobalOwner state(session, {1, 8, 9, 26},
        closeBusyFalseSourceOccurrence, "declared fixture latched main pointer");
    auto facts = mainFacts(session, 0, 0);
    facts.stackedCurrentIndex.reset();
    attemptCloseBusyFalsePrefix(session, state, facts);
    CHECK(state.stage() == CloseBusyFalseStage::AwaitStackedCurrentIndex);
    CHECK(state.observedMainIdentity() == 77);
    auto otherMain = mainFacts(session, 0, 0);
    otherMain.mainIdentity = 88;
    otherMain.stackedCurrentIndex->mainIdentity = 88;
    otherMain.mainField270->mainIdentity = 88;
    CHECK(attemptCloseBusyFalsePrefix(session, state, otherMain).stop == CloseBusyFalseStop::MainBusyFactsUnavailable);
    CHECK(!state.observedStackedIndex());
    facts = mainFacts(session, 0, 0);
    facts.globalMainPresent = false; // global lookup has already completed
    const auto result = attemptCloseBusyFalsePrefix(session, state, facts);
    REQUIRE(result.selectionEnabledArgument);
    CHECK(*result.selectionEnabledArgument);
    CHECK(state.observedMainIdentity() == 77);
}

TEST_CASE(close_false_retry_preserves_partial_progress_and_latches_resolved_branch)
{
    DiagramEffectDriver effects;
    ConversionSession session(effects);
    CloseBusyFalseGlobalOwner state(session, {1, 8, 9, 20},
        closeBusyFalseSourceOccurrence, "declared fixture staged retry");
    CHECK(attemptCloseBusyFalsePrefix(session, state, {}).resetCommittedThisAttempt);
    const auto unavailable = attemptCloseBusyFalsePrefix(session, state, {});
    CHECK(!unavailable.resetCommittedThisAttempt);
    CHECK(!unavailable.branchCommittedThisAttempt);
    const auto branch = attemptCloseBusyFalsePrefix(session, state, mainFacts(session, 0, 0));
    CHECK(!branch.resetCommittedThisAttempt);
    CHECK(branch.branchCommittedThisAttempt);
    REQUIRE(branch.selectionEnabledArgument);
    CHECK(*branch.selectionEnabledArgument);
    const auto retry = attemptCloseBusyFalsePrefix(session, state, mainFacts(session, 2, 1));
    CHECK(!retry.resetCommittedThisAttempt);
    CHECK(!retry.branchCommittedThisAttempt);
    CHECK(retry.selectionEnabledArgument == branch.selectionEnabledArgument);
    CHECK(std::string(retry.nextSourceSite) == branch.nextSourceSite);
    CHECK(state.fields().fieldA == 1);
}

TEST_CASE(close_false_owner_and_evidence_session_mismatches_have_distinct_boundaries)
{
    DiagramEffectDriver effectsA, effectsB;
    ConversionSession sessionA(effectsA), sessionB(effectsB);
    CloseBusyFalseGlobalOwner state(sessionA, {3, 4, 5, 21},
        closeBusyFalseSourceOccurrence, "declared fixture session A state");
    CHECK(refuses([&] { attemptCloseBusyFalsePrefix(sessionB, state, {}); }, "identity"));
    CHECK(state.stage() == CloseBusyFalseStage::BeforeResetStores);
    CHECK(state.fields().flag9 == 3);
    const auto result = attemptCloseBusyFalsePrefix(sessionA, state, mainFacts(sessionB, 0, 0));
    CHECK(result.stop == CloseBusyFalseStop::MainBusyFactsUnavailable);
    CHECK(state.fields().flag9 == 0);
    CHECK(state.fields().counter143d91230 == 0);
    CHECK(state.fields().fieldA == 4);
}

TEST_CASE(close_false_requires_late_entry_and_nonblank_owner_qualification)
{
    DiagramEffectDriver effects;
    ConversionSession session(effects);
    CHECK(refuses([&] { CloseBusyFalseGlobalOwner state(session, {1, 2, 3, 22},
        closeBusyTrueSourceOccurrence, "declared fixture wrong true entry"); }, "occurrence"));
    CHECK(refuses([&] { CloseBusyFalseGlobalOwner state(session, {1, 2, 3, 22},
        closeBusyFalseSourceOccurrence, " \r\n"); }, "qualification"));
}

TEST_CASE(close_false_prefix_preserves_notifier_actual_model_project_timers_and_opi)
{
    DiagramEffectDriver effects;
    ConversionSession session(effects);
    session.prepareProjectModel(qt::L("C:/PROBLEM/CloseFalse/fixture.DB"));
    auto manager = session.modelManagerOwner();
    auto model = session.project().dataDb;
    auto* project = session.liveProject();
    const auto pending = session.pending().size();
    const auto connections = manager->connections().size();
    auto notifications = session.sessionNotificationStateOwner();
    notifications->flag90 = true;
    notifications->suppressed = true;
    const auto keys = notifications->emptyKeys;
    const auto attempts = notifications->attempts.size();
    Operation operation;
    mst::OpClass cls(qt::L("FalsePrefixProbe"));
    cls.current = &operation;
    const auto guard = operation.operationInfoIdentity().guard();
    CloseBusyFalseGlobalOwner state(session, {1, 0, 0, 23},
        closeBusyFalseSourceOccurrence, "declared fixture isolated late false prefix");
    CHECK(attemptCloseBusyFalsePrefix(session, state, mainFacts(session, 2, 0)).stop ==
          CloseBusyFalseStop::TreeEnableSelectionDispatchUnavailable);
    CHECK(manager->size() == 1);
    CHECK(manager->currentModel() == nullptr);
    CHECK(manager->connections().size() == connections);
    CHECK(session.liveProject() == project);
    CHECK(session.project().dataDb == model);
    CHECK(model->moProject == nullptr);
    CHECK(session.pending().size() == pending);
    CHECK(notifications->flag90);
    CHECK(notifications->suppressed);
    CHECK(notifications->emptyKeys == keys);
    CHECK(notifications->attempts.size() == attempts);
    CHECK(cls.current == &operation);
    CHECK(guard.expiry() == OperationInfoExpiry::Live);
}

TEST_CASE(close_false_state_cannot_authorize_new_session_after_original_dies)
{
    std::unique_ptr<CloseBusyFalseGlobalOwner> state;
    std::shared_ptr<ModelManager> retainedManager;
    {
        DiagramEffectDriver effects;
        ConversionSession original(effects);
        retainedManager = original.modelManagerOwner();
        state = std::make_unique<CloseBusyFalseGlobalOwner>(original,
            CloseBusyNativeFields{1, 2, 3, 24}, closeBusyFalseSourceOccurrence,
            "declared fixture original Session");
    }
    DiagramEffectDriver effects;
    ConversionSession replacement(effects);
    REQUIRE(retainedManager != replacement.modelManagerOwner());
    CHECK(refuses([&] { attemptCloseBusyFalsePrefix(replacement, *state, {}); }, "identity"));
    CHECK(state->fields().flag9 == 1);
    CHECK(state->fields().counter143d91230 == 24);
}

int main() { return testkit::run_all(); }
