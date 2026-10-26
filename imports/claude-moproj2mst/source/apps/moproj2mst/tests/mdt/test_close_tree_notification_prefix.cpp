// ctest-labels: m2m-mdt close-tree-notification-prefix
#include <testkit/check.hpp>

#include <memory>
#include <optional>
#include <set>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#include "m2m/common.hpp"
#include "m2m/doe_io.hpp"
#include "m2m/doe_session_notifications.hpp"
#include "m2m/engine.hpp"
#include "m2m/engine/close_busy_prefix.hpp"
#include "m2m/engine/close_tree_notification_prefix.hpp"
#include "m2m/engine/conversion_session.hpp"
#include "m2m/engine/model_manager.hpp"
#include "m2m/model.hpp"
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
struct Fixture {
    DiagramEffectDriver effects;
    ConversionSession session{effects};
    CloseBusyGlobalOwner busy{session, {7, 8, 9, 0xfedcba98u},
        closeBusyTrueSourceOccurrence, "declared accepted true setter fixture"};
    Fixture() { attemptCloseBusyTruePrefix(session, busy); }
};
struct NotificationSnapshot {
    std::set<std::string> emptyKeys;
    std::size_t attempts;
    std::optional<doe::TaskFactoryRegistrationList> taskRegistrations;
    std::optional<std::vector<std::weak_ptr<PostGraphManagerState>>> graphRegistrations;
    std::string graphQualification;
    std::vector<std::pair<std::string, std::size_t>> viewSizes;
    explicit NotificationSnapshot(const doe::SessionNotificationState& n)
        : emptyKeys(n.emptyKeys), attempts(n.attempts.size()),
          taskRegistrations(n.createTaskBaseRegistrations),
          graphRegistrations(n.removeGraphItemsRegistrations),
          graphQualification(n.removeGraphItemsQualification)
    {
        for (const auto& entry : n.viewManagerRegistrations)
            viewSizes.emplace_back(entry.first, entry.second.size());
    }
    void check(const doe::SessionNotificationState& n) const
    {
        CHECK(n.emptyKeys == emptyKeys);
        CHECK(n.attempts.size() == attempts);
        CHECK(n.createTaskBaseRegistrations.has_value() == taskRegistrations.has_value());
        if (n.createTaskBaseRegistrations && taskRegistrations) {
            CHECK(n.createTaskBaseRegistrations->entries.size() == taskRegistrations->entries.size());
            CHECK(n.createTaskBaseRegistrations->qualification == taskRegistrations->qualification);
            CHECK(n.createTaskBaseRegistrations->mapKeyPresent == taskRegistrations->mapKeyPresent);
        }
        CHECK(n.removeGraphItemsRegistrations.has_value() == graphRegistrations.has_value());
        if (n.removeGraphItemsRegistrations && graphRegistrations)
            CHECK(n.removeGraphItemsRegistrations->size() == graphRegistrations->size());
        CHECK(n.removeGraphItemsQualification == graphQualification);
        std::vector<std::pair<std::string, std::size_t>> actual;
        for (const auto& entry : n.viewManagerRegistrations)
            actual.emplace_back(entry.first, entry.second.size());
        CHECK(actual == viewSizes);
    }
};
}

static_assert(!std::is_copy_constructible_v<CloseTreeNotificationPrefixProgress>);
static_assert(!std::is_move_constructible_v<CloseTreeNotificationPrefixProgress>);
static_assert(!CloseTreeNotificationPrefixResult::selectionEnabledArgument);
static_assert(!CloseTreeNotificationPrefixResult::callerArgumentProductionCompleted);
static_assert(!CloseTreeNotificationPrefixResult::busySetterComplete);
static_assert(!CloseTreeNotificationPrefixResult::modelCleanupComplete);
static_assert(!CloseTreeNotificationPrefixResult::completeForPublicClose);

TEST_CASE(close_tree_prefix_constructor_does_not_read_or_mutate_notifier)
{
    Fixture f;
    auto notifier = f.session.sessionNotificationStateOwner();
    notifier->suppressed = true;
    notifier->flag90 = true;
    notifier->emptyKeys.insert("fixture-existing-key");
    const NotificationSnapshot before(*notifier);
    CloseTreeNotificationPrefixProgress progress(f.session, f.busy,
        closeTreeNotificationSourceOccurrence, "declared direct callee entry");
    CHECK(progress.stage() == CloseTreeNotificationPrefixStage::AwaitSuppressionRead);
    CHECK(notifier->suppressed && notifier->flag90);
    before.check(*notifier);
    // Change BEFORE first attempt: the read occurs at attempt, not construction.
    notifier->suppressed = false;
    CHECK(attemptCloseTreeNotificationPrefix(f.session, progress).flag90StoredThisAttempt);
    CHECK(!notifier->flag90);
}

TEST_CASE(close_tree_prefix_requires_reached_boundary_and_exact_declared_entry)
{
    DiagramEffectDriver effects;
    ConversionSession session(effects);
    auto notifier = session.sessionNotificationStateOwner();
    notifier->flag90 = true;
    CloseBusyGlobalOwner busy(session, {7, 8, 9, 12}, closeBusyTrueSourceOccurrence, "fixture");
    CHECK(refuses([&] { CloseTreeNotificationPrefixProgress progress(session, busy,
        closeTreeNotificationSourceOccurrence, "fixture unstarted"); }, "already-reached"));
    CHECK(busy.stage() == CloseBusyPrefixStage::BeforeTrueStore);
    CHECK(busy.fields().flag9 == 7 && busy.fields().fieldA == 8 && busy.fields().fieldB == 9);
    CHECK(busy.fields().counter143d91230 == 12 && notifier->flag90);
    attemptCloseBusyTruePrefix(session, busy);
    CHECK(refuses([&] { CloseTreeNotificationPrefixProgress progress(session, busy,
        "0x140029c9b:false setter", "fixture wrong enclosing occurrence"); }, "occurrence/qualification"));
    CHECK(refuses([&] { CloseTreeNotificationPrefixProgress progress(session, busy,
        closeTreeNotificationSourceOccurrence, " \t\r\n"); }, "occurrence/qualification"));
    CHECK(notifier->flag90);
}

TEST_CASE(close_tree_prefix_suppressed_callee_preserves_actual_shared_notification_state)
{
    Fixture f;
    auto notifier = f.session.sessionNotificationStateOwner();
    notifier->suppressed = true;
    notifier->flag90 = true;
    notifier->emptyKeys.insert("fixture-existing-key");
    const NotificationSnapshot before(*notifier);
    CloseTreeNotificationPrefixProgress progress(f.session, f.busy,
        closeTreeNotificationSourceOccurrence, "declared suppressed direct callee");
    const auto result = attemptCloseTreeNotificationPrefix(f.session, progress);
    CHECK(result.stop == CloseTreeNotificationPrefixStop::SuppressedCalleeReturnReached);
    CHECK(result.suppressionReadThisAttempt && !result.flag90StoredThisAttempt);
    CHECK(progress.stage() == CloseTreeNotificationPrefixStage::SuppressedCalleeReturn);
    CHECK(std::string(result.nextSourceSite).find("0x1402cf72e") == 0);
    CHECK(notifier->suppressed && notifier->flag90);
    before.check(*notifier);
}

TEST_CASE(close_tree_prefix_unsuppressed_store_precedes_unknown_key_and_population)
{
    Fixture f;
    auto notifier = f.session.sessionNotificationStateOwner();
    auto io = doe::makeRecordingStudyIo();
    io->bindSessionNotificationState(notifier);
    notifier->suppressed = false;
    notifier->flag90 = true;
    notifier->emptyKeys.insert("fixture-existing-key");
    const NotificationSnapshot before(*notifier);
    CloseTreeNotificationPrefixProgress progress(f.session, f.busy,
        closeTreeNotificationSourceOccurrence, "declared unsuppressed direct callee");
    const auto result = attemptCloseTreeNotificationPrefix(f.session, progress);
    CHECK(result.stop == CloseTreeNotificationPrefixStop::KeyQStringConstructionUnavailable);
    CHECK(result.suppressionReadThisAttempt && result.flag90StoredThisAttempt);
    CHECK(progress.stage() == CloseTreeNotificationPrefixStage::BeforeKeyQStringConstruction);
    CHECK(std::string(result.nextSourceSite).find("0x1402cf41f") == 0);
    CHECK(io->observedSessionNotificationState() == notifier.get());
    CHECK(!io->sessionNotificationState().flag90);
    CHECK(notifier->emptyKeys.count("2signalTreeEnableSelection()") == 0);
    before.check(*notifier);
}

TEST_CASE(close_tree_prefix_retries_never_reread_suppression_or_replay_flag_store)
{
    Fixture f;
    auto notifier = f.session.sessionNotificationStateOwner();
    for (bool initiallySuppressed : {false, true}) {
        notifier->suppressed = initiallySuppressed;
        notifier->flag90 = true;
        CloseTreeNotificationPrefixProgress progress(f.session, f.busy,
            closeTreeNotificationSourceOccurrence, "declared separate direct callee fixture");
        const auto first = attemptCloseTreeNotificationPrefix(f.session, progress);
        const auto stage = progress.stage();
        notifier->suppressed = !initiallySuppressed;
        notifier->flag90 = true; // later effect must survive retry
        const auto retry = attemptCloseTreeNotificationPrefix(f.session, progress);
        CHECK(retry.stop == first.stop && progress.stage() == stage);
        CHECK(!retry.suppressionReadThisAttempt && !retry.flag90StoredThisAttempt);
        CHECK(notifier->flag90 && notifier->suppressed == !initiallySuppressed);
    }
}

TEST_CASE(close_tree_prefix_rejects_foreign_live_session_before_effects)
{
    Fixture first, second;
    auto one = first.session.sessionNotificationStateOwner();
    auto two = second.session.sessionNotificationStateOwner();
    one->flag90 = two->flag90 = true;
    CHECK(refuses([&] { CloseTreeNotificationPrefixProgress foreign(second.session, first.busy,
        closeTreeNotificationSourceOccurrence, "fixture foreign prerequisite"); }, "identity"));
    CloseTreeNotificationPrefixProgress progress(first.session, first.busy,
        closeTreeNotificationSourceOccurrence, "fixture first owner");
    CHECK(refuses([&] { attemptCloseTreeNotificationPrefix(second.session, progress); }, "identity"));
    CHECK(progress.stage() == CloseTreeNotificationPrefixStage::AwaitSuppressionRead);
    CHECK(one->flag90 && two->flag90);
}

TEST_CASE(close_tree_prefix_does_not_retain_busy_object_or_authorize_dead_session)
{
    std::unique_ptr<CloseTreeNotificationPrefixProgress> progress;
    std::shared_ptr<ModelManager> managerLease;
    std::shared_ptr<doe::SessionNotificationState> notifierLease;
    {
        DiagramEffectDriver effects;
        ConversionSession session(effects);
        managerLease = session.modelManagerOwner();
        notifierLease = session.sessionNotificationStateOwner();
        notifierLease->suppressed = false;
        notifierLease->flag90 = true;
        {
            CloseBusyGlobalOwner busy(session, {3, 4, 5, 6}, closeBusyTrueSourceOccurrence, "fixture");
            attemptCloseBusyTruePrefix(session, busy);
            progress = std::make_unique<CloseTreeNotificationPrefixProgress>(session, busy,
                closeTreeNotificationSourceOccurrence, "fixture escaped progress");
        }
        CHECK(attemptCloseTreeNotificationPrefix(session, *progress).flag90StoredThisAttempt);
    }
    notifierLease->flag90 = true;
    Fixture replacement;
    REQUIRE(replacement.session.modelManagerOwner() != managerLease);
    REQUIRE(replacement.session.sessionNotificationStateOwner() != notifierLease);
    replacement.session.sessionNotificationStateOwner()->flag90 = true;
    CHECK(refuses([&] { attemptCloseTreeNotificationPrefix(replacement.session, *progress); }, "identity"));
    CHECK(notifierLease->flag90 && replacement.session.sessionNotificationStateOwner()->flag90);
    CHECK(progress->stage() == CloseTreeNotificationPrefixStage::BeforeKeyQStringConstruction);
}

TEST_CASE(close_tree_prefix_preserves_actual_project_current_opi_and_pending_timers)
{
    Fixture f;
    f.session.prepareProjectModel(qt::L("C:/PROBLEM/C88/fixture.DB"));
    f.session.commitProjectModelConstruction();
    f.session.commitProjectModelSelection();
    auto manager = f.session.modelManagerOwner();
    auto model = f.session.project().dataDb;
    auto* project = f.session.liveProject();
    auto sub = std::make_unique<SubProject>();
    sub->project = project;
    auto* attached = sub.get();
    project->subs.push_back(std::move(sub));
    f.session.commitNonNullDiagramBinding(*attached);
    f.session.commitNonNullDiagramBinding(*attached);
    const auto pending = f.session.pending();
    REQUIRE(pending.size() == 2);
    const auto connections = manager->connections().size();
    Operation operation;
    mst::OpClass cls(qt::L("C88Probe"));
    cls.current = &operation;
    const auto guard = operation.operationInfoIdentity().guard();
    auto notifier = f.session.sessionNotificationStateOwner();
    notifier->flag90 = true;
    CloseTreeNotificationPrefixProgress progress(f.session, f.busy,
        closeTreeNotificationSourceOccurrence, "declared actual-owner preservation fixture");
    REQUIRE(attemptCloseTreeNotificationPrefix(f.session, progress).flag90StoredThisAttempt);
    CHECK(f.session.liveProject() == project && f.session.project().dataDb == model);
    CHECK(model->moProject == project && manager->currentModel() == model.get());
    CHECK(manager->size() == 1 && manager->connections().size() == connections);
    REQUIRE(f.session.pending().size() == pending.size());
    for (std::size_t i = 0; i < pending.size(); ++i) {
        CHECK(f.session.pending()[i].id == pending[i].id);
        CHECK(f.session.pending()[i].receiver == pending[i].receiver);
        CHECK(f.session.pending()[i].requestedIntervalMs == pending[i].requestedIntervalMs);
    }
    CHECK(cls.current == &operation && guard.get() == &operation);
    CHECK(guard.expiry() == OperationInfoExpiry::Live);
    CHECK(f.busy.fields().flag9 == 1 && f.busy.fields().fieldA == 0 && f.busy.fields().fieldB == 0);
    CHECK(f.busy.fields().counter143d91230 == 0xfedcba98u);
}

int main() { return testkit::run_all(); }
