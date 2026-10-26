// ctest-labels: m2m-mdt close-status-prefix
#include <testkit/check.hpp>

#include <memory>
#include <string>
#include <type_traits>

#include "m2m/common.hpp"
#include "m2m/doe_session_notifications.hpp"
#include "m2m/engine.hpp"
#include "m2m/engine/close_status_prefix.hpp"
#include "m2m/engine/conversion_session.hpp"
#include "m2m/engine/model_manager.hpp"
#include "m2m/mst/writer.hpp"

using namespace m2m;
using namespace m2m::engine;
namespace {
CloseStatusMainObservation observedMain(const ConversionSession& session, bool present)
{
    return {&session, closeStatusSourceOccurrence, present,
            "declared fixture fresh global main lookup AFTER false-setter receivers"};
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

static_assert(!std::is_copy_constructible_v<CloseStatusPrefixProgress>);
static_assert(!std::is_move_constructible_v<CloseStatusPrefixProgress>);

TEST_CASE(close_status_fresh_qualified_null_skips_status_work_then_guards_caller_release)
{
    DiagramEffectDriver effects;
    ConversionSession session(effects);
    REQUIRE(session.mainViewOwner() != nullptr); // candidate view does not prove native global alias
    CloseStatusPrefixProgress progress(session, closeStatusSourceOccurrence,
        "declared fixture reached normal status invocation");
    CHECK(progress.stage() == CloseStatusPrefixStage::AwaitMainLookup);
    CHECK(!progress.observedMainPresent());
    const auto result = attemptCloseStatusPrefix(session, progress, observedMain(session, false));
    CHECK(result.stop == CloseStatusPrefixStop::NullMainSkipBeforeCallerQStringRelease);
    CHECK(result.lookupCommittedThisAttempt);
    REQUIRE(result.observedMainPresent);
    CHECK(!*result.observedMainPresent);
    CHECK(progress.stage() == CloseStatusPrefixStage::NullSkipBeforeCallerQStringRelease);
    CHECK(std::string(result.nextSourceSite).find("QString release/public-return boundary") != std::string::npos);
    CHECK(session.liveProject() != nullptr);
}

TEST_CASE(close_status_missing_unqualified_stale_or_foreign_facts_do_not_consume_lookup)
{
    DiagramEffectDriver effectsA, effectsB;
    ConversionSession session(effectsA), other(effectsB);
    for (int variant = 0; variant < 4; ++variant) {
        CloseStatusPrefixProgress progress(session, closeStatusSourceOccurrence,
            "declared fixture pending status lookup");
        auto observation = observedMain(session, false);
        switch (variant) {
        case 0: observation.globalMainPresent.reset(); break;
        case 1: observation.qualification = " \r\n"; break;
        case 2: observation.sourceOccurrence = "0x140029c9b:0x1402ca9f0(false)"; break;
        case 3: observation.sessionOwner = &other; break;
        }
        const auto result = attemptCloseStatusPrefix(session, progress, observation);
        CHECK(result.stop == CloseStatusPrefixStop::MainLookupUnavailable);
        CHECK(!result.lookupCommittedThisAttempt);
        CHECK(!result.observedMainPresent);
        CHECK(progress.stage() == CloseStatusPrefixStage::AwaitMainLookup);
    }
}

TEST_CASE(close_status_nonnull_lookup_latches_guard_and_cannot_switch_to_later_null)
{
    DiagramEffectDriver effects;
    ConversionSession session(effects);
    CloseStatusPrefixProgress progress(session, closeStatusSourceOccurrence,
        "declared fixture nonnull current main");
    const auto first = attemptCloseStatusPrefix(session, progress, observedMain(session, true));
    CHECK(first.stop == CloseStatusPrefixStop::NonNullStatusBarEffectsUnavailable);
    CHECK(first.lookupCommittedThisAttempt);
    REQUIRE(first.observedMainPresent);
    CHECK(*first.observedMainPresent);
    const auto retry = attemptCloseStatusPrefix(session, progress, observedMain(session, false));
    CHECK(retry.stop == CloseStatusPrefixStop::NonNullStatusBarEffectsUnavailable);
    CHECK(!retry.lookupCommittedThisAttempt);
    REQUIRE(retry.observedMainPresent);
    CHECK(*retry.observedMainPresent);
    CHECK(std::string(retry.nextSourceSite) == first.nextSourceSite);
}

TEST_CASE(close_status_null_skip_retains_completed_outcome_without_requery_or_release)
{
    DiagramEffectDriver effects;
    ConversionSession session(effects);
    CloseStatusPrefixProgress progress(session, closeStatusSourceOccurrence,
        "declared fixture null current main");
    attemptCloseStatusPrefix(session, progress, observedMain(session, false));
    const auto retry = attemptCloseStatusPrefix(session, progress, observedMain(session, true));
    CHECK(retry.stop == CloseStatusPrefixStop::NullMainSkipBeforeCallerQStringRelease);
    CHECK(!retry.lookupCommittedThisAttempt);
    REQUIRE(retry.observedMainPresent);
    CHECK(!*retry.observedMainPresent);
    CHECK(session.liveProject() != nullptr);
}

TEST_CASE(close_status_preserves_actual_project_model_notifier_pending_and_opi)
{
    DiagramEffectDriver effects;
    ConversionSession session(effects);
    session.prepareProjectModel(qt::L("C:/PROBLEM/CloseStatus/fixture.DB"));
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
    mst::OpClass cls(qt::L("StatusPrefixProbe"));
    cls.current = &operation;
    const auto guard = operation.operationInfoIdentity().guard();
    for (bool present : {false, true}) {
        CloseStatusPrefixProgress progress(session, closeStatusSourceOccurrence,
            "declared fixture isolated status state preservation");
        const auto result = attemptCloseStatusPrefix(session, progress, observedMain(session, present));
        CHECK(result.stop == (present ? CloseStatusPrefixStop::NonNullStatusBarEffectsUnavailable
            : CloseStatusPrefixStop::NullMainSkipBeforeCallerQStringRelease));
    }
    CHECK(manager->size() == 1);
    CHECK(manager->currentModel() == nullptr);
    CHECK(manager->connections().size() == connections);
    CHECK(session.project().dataDb == model);
    CHECK(session.liveProject() == project);
    CHECK(model->moProject == nullptr);
    CHECK(session.pending().size() == pending);
    CHECK(notifications->flag90);
    CHECK(notifications->suppressed);
    CHECK(notifications->emptyKeys == keys);
    CHECK(notifications->attempts.size() == attempts);
    CHECK(cls.current == &operation);
    CHECK(guard.expiry() == OperationInfoExpiry::Live);
}

TEST_CASE(close_status_requires_same_actual_live_session_before_consuming_lookup)
{
    DiagramEffectDriver effectsA, effectsB;
    ConversionSession sessionA(effectsA), sessionB(effectsB);
    CloseStatusPrefixProgress progress(sessionA, closeStatusSourceOccurrence,
        "declared fixture session A status progress");
    CHECK(refuses([&] { attemptCloseStatusPrefix(sessionB, progress, observedMain(sessionB, false)); }, "identity"));
    CHECK(progress.stage() == CloseStatusPrefixStage::AwaitMainLookup);
    CHECK(!progress.observedMainPresent());
}

TEST_CASE(close_status_constructor_requires_exact_status_occurrence_and_qualification)
{
    DiagramEffectDriver effects;
    ConversionSession session(effects);
    CHECK(refuses([&] { CloseStatusPrefixProgress progress(session,
        "0x140029c9b:0x1402ca9f0(false)", "declared fixture earlier false setter"); }, "occurrence"));
    CHECK(refuses([&] { CloseStatusPrefixProgress progress(session,
        closeStatusSourceOccurrence, " \t\r\n"); }, "qualification"));
}

TEST_CASE(close_status_dead_session_progress_does_not_bind_a_replacement_session)
{
    std::unique_ptr<CloseStatusPrefixProgress> progress;
    std::shared_ptr<ModelManager> retainedManager;
    {
        DiagramEffectDriver effects;
        ConversionSession original(effects);
        retainedManager = original.modelManagerOwner();
        progress = std::make_unique<CloseStatusPrefixProgress>(original,
            closeStatusSourceOccurrence, "declared fixture original status progress");
    }
    DiagramEffectDriver effects;
    ConversionSession replacement(effects);
    REQUIRE(retainedManager != replacement.modelManagerOwner());
    CHECK(refuses([&] { attemptCloseStatusPrefix(replacement, *progress,
        observedMain(replacement, false)); }, "identity"));
    CHECK(progress->stage() == CloseStatusPrefixStage::AwaitMainLookup);
}

int main() { return testkit::run_all(); }
