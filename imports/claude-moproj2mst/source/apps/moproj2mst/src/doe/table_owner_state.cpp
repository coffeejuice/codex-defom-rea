#include "m2m/doe_table_owner_state.hpp"

#include "m2m/common.hpp"
#include "m2m/doe_session_notifications.hpp"
#include "m2m/engine/conversion_session.hpp"
#include "m2m/engine/model_manager.hpp"

namespace m2m::doe {
namespace {
[[noreturn]] void refuse(const char* reason)
{ throw Error(M2M_E_UNSUPPORTED, std::string("DOE table owner: ") + reason); }

template<class A, class B> bool sameOwner(const A& a, const B& b)
{ return a && b && a.get() == b.get() && !a.owner_before(b) && !b.owner_before(a); }

std::uint64_t sourceSite(TableOwnerEvent event)
{
    switch (event) {
    case TableOwnerEvent::InitializeFields: return 0x140019760ULL;
    case TableOwnerEvent::PublishDock: return 0x14001a48fULL;
    case TableOwnerEvent::BeginGetter: return 0x1400196e7ULL;
    case TableOwnerEvent::AllocationReturned:
    case TableOwnerEvent::AllocationReturnedNull: return 0x1400196f6ULL;
    case TableOwnerEvent::TimerParentRecorded: return 0x1400eb826ULL;
    case TableOwnerEvent::ConstructorPrefixReturned: return 0x1400ebc5cULL;
    case TableOwnerEvent::RegistrationReturned: return 0x1400ebc61ULL;
    case TableOwnerEvent::ConstructorReturned: return 0x140019714ULL;
    case TableOwnerEvent::PublishCache: return 0x140019738ULL;
    case TableOwnerEvent::DockInstallReturned: return 0x14001a52aULL;
    case TableOwnerEvent::BeginDelete: return 0x14001979eULL;
    case TableOwnerEvent::DeletingDestructorReturned: return 0x1400197e0ULL;
    case TableOwnerEvent::ClearDeletedCache: return 0x1400197f5ULL;
    // An interruption is an adapter stop, not a recovered native unwind path.
    case TableOwnerEvent::ConstructorInterrupted: return 0;
    }
    return 0;
}
}

struct DoeTableOwnerState::State {
    const engine::ConversionSession* session = nullptr; // comparison only
    std::weak_ptr<engine::ModelManager> manager;
    std::weak_ptr<engine::WidgetRuntimeState> runtime;
    std::weak_ptr<SessionNotificationState> notifier;
    TableOwnerReport report;
    std::optional<std::size_t> pending, cached, getterResult;

    bool matches(engine::ConversionSession& actual) const
    {
        const auto m = manager.lock();
        return !report.adapterScopeReleased && session == &actual && m &&
            m->ownerSession() == &actual && sameOwner(m, actual.modelManagerOwner()) &&
            sameOwner(runtime.lock(), actual.widgetRuntimeOwner()) &&
            sameOwner(notifier.lock(), actual.sessionNotificationStateOwner());
    }
};

DoeTableOwnerState::DoeTableOwnerState(engine::ConversionSession& session)
    : state_(std::make_unique<State>())
{
    state_->session = &session;
    state_->manager = session.modelManagerOwner();
    state_->runtime = session.widgetRuntimeOwner();
    state_->notifier = session.sessionNotificationStateOwner();
    if (!state_->matches(session)) refuse("actual Session/manager/notifier/runtime binding is unavailable");
}
DoeTableOwnerState::~DoeTableOwnerState() = default;
const TableOwnerReport& DoeTableOwnerState::report() const noexcept { return state_->report; }
void DoeTableOwnerState::releaseAdmissionScope() noexcept { state_->report.adapterScopeReleased = true; }

void DoeTableOwnerState::record(engine::ConversionSession& session, TableOwnerEvent event,
                               const TableOwnerObservation& observation)
{
    auto& s = *state_;
    auto& r = s.report;
    if (!s.matches(session)) refuse("actual Session admission scope is unavailable");
    if (observation.qualification.empty()) refuse("source-stage qualification is unavailable");
    auto require = [](bool condition, const char* reason) { if (!condition) refuse(reason); };
    auto phase = [&](TableOwnerPhase expected) { require(r.phase == expected, "source stage is out of order"); };
    auto generation = [&]() -> TableOwnerGeneration& {
        require(s.pending.has_value(), "pending allocation generation is unavailable");
        return r.generations[*s.pending];
    };
    std::uint64_t receiptGeneration = 0;
    switch (event) {
    case TableOwnerEvent::InitializeFields:
        require(r.tableField == TableOwnerField::Unavailable && r.dockField == TableOwnerField::Unavailable,
                "field initialization cannot reset known or partial state");
        r.tableField = r.dockField = TableOwnerField::KnownNull;
        break;
    case TableOwnerEvent::PublishDock:
        require(r.dockField == TableOwnerField::KnownNull && observation.dock,
                "dock constructor return identity is unavailable or already published");
        r.dock = observation.dock;
        r.dockField = TableOwnerField::Cached;
        break;
    case TableOwnerEvent::BeginGetter:
        require(r.tableField != TableOwnerField::Unavailable, "main-window +458 is unavailable");
        if (r.tableField == TableOwnerField::Cached) {
            phase(TableOwnerPhase::Published);
            s.getterResult = s.cached;
            receiptGeneration = r.cachedGeneration.value(); // cache hit: no new generation
            break;
        }
        phase(TableOwnerPhase::Idle);
        s.getterResult.reset();
        r.generations.push_back({});
        s.pending = r.generations.size() - 1;
        r.generations.back().id = r.generations.size();
        r.phase = TableOwnerPhase::AllocationPending;
        receiptGeneration = r.generations.back().id;
        break;
    case TableOwnerEvent::AllocationReturned:
        phase(TableOwnerPhase::AllocationPending);
        require(static_cast<bool>(observation.table), "allocation return identity is unavailable");
        generation().allocation = observation.table;
        r.phase = TableOwnerPhase::ConstructorPending;
        break;
    case TableOwnerEvent::AllocationReturnedNull:
        phase(TableOwnerPhase::AllocationPending);
        require(!observation.table, "null allocation observation has a nonnull identity");
        generation().allocationReturnedNull = true;
        r.phase = TableOwnerPhase::NullPublicationPending;
        break;
    case TableOwnerEvent::TimerParentRecorded:
        phase(TableOwnerPhase::ConstructorPending);
        require(!generation().timerParentRecorded && observation.timer &&
                    sameOwner(generation().allocation, observation.table),
                "QTimer constructor parent/identity receipt is unavailable or duplicated");
        require(observation.timer.get() != observation.table.get(), "timer identity aliases its table parent");
        generation().timer = observation.timer;
        generation().timerParentRecorded = true;
        break;
    case TableOwnerEvent::ConstructorPrefixReturned:
        phase(TableOwnerPhase::ConstructorPending);
        require(generation().timerParentRecorded, "preceding QTimer constructor receipt is unavailable");
        require(sameOwner(generation().allocation, observation.table), "constructor prefix owner differs");
        generation().constructorPrefixReturned = true;
        r.phase = TableOwnerPhase::RegistrationPending;
        break;
    case TableOwnerEvent::RegistrationReturned:
        phase(TableOwnerPhase::RegistrationPending);
        require(sameOwner(generation().allocation, observation.table) &&
                    sameOwner(s.notifier.lock(), observation.notifier),
                "actual table/global notifier registration receipt differs");
        generation().registrationReturned = true;
        r.phase = TableOwnerPhase::ConstructorReturnPending;
        break;
    case TableOwnerEvent::ConstructorReturned:
        phase(TableOwnerPhase::ConstructorReturnPending);
        require(sameOwner(generation().allocation, observation.table), "constructor result owner differs from allocation");
        generation().constructorResult = observation.table;
        generation().constructorReturned = true;
        r.phase = TableOwnerPhase::PublicationPending;
        break;
    case TableOwnerEvent::ConstructorInterrupted:
        require(r.phase == TableOwnerPhase::ConstructorPending || r.phase == TableOwnerPhase::RegistrationPending ||
                    r.phase == TableOwnerPhase::ConstructorReturnPending,
                "no in-flight constructor exists to interrupt");
        generation().constructorInterrupted = true;
        r.phase = TableOwnerPhase::ConstructorInterrupted;
        break;
    case TableOwnerEvent::PublishCache: {
        require(r.phase == TableOwnerPhase::PublicationPending || r.phase == TableOwnerPhase::NullPublicationPending,
                "constructor/allocation result has not returned for publication");
        auto& g = generation();
        receiptGeneration = g.id;
        // Validate before exposing the result or changing any metadata.
        if (r.phase == TableOwnerPhase::NullPublicationPending)
            require(!observation.table, "null allocation cannot publish a nonnull table");
        else
            require(sameOwner(g.constructorResult, observation.table), "cache publication differs from constructor result");
        s.getterResult = s.pending;
        if (r.phase == TableOwnerPhase::NullPublicationPending) {
            g.cachePublished = true;
            s.pending.reset();
            r.phase = TableOwnerPhase::Idle;
        } else {
            g.cachePublished = true;
            r.cachedTable = observation.table;
            r.cachedGeneration = g.id;
            s.cached = s.pending;
            s.pending.reset();
            r.tableField = TableOwnerField::Cached;
            r.phase = TableOwnerPhase::Published;
        }
        break;
    }
    case TableOwnerEvent::DockInstallReturned:
        require(r.phase == TableOwnerPhase::Published ||
                    (r.phase == TableOwnerPhase::Idle && r.tableField == TableOwnerField::KnownNull),
                "getter has not returned before dock installation");
        require(s.getterResult && sameOwner(r.dock, observation.dock) &&
                    ((r.cachedTable && sameOwner(r.cachedTable, observation.table)) ||
                     (!r.cachedTable && !observation.table && r.generations[*s.getterResult].allocationReturnedNull)),
                "dock installation differs from published dock/table");
        require(!r.generations[*s.getterResult].dockInstallReturned, "dock installation return already recorded");
        r.generations[*s.getterResult].dockInstallReturned = true;
        receiptGeneration = r.generations[*s.getterResult].id;
        break;
    case TableOwnerEvent::BeginDelete:
        require(r.tableField != TableOwnerField::Unavailable, "main-window +458 is unavailable");
        if (r.tableField == TableOwnerField::KnownNull) break; // source helper's null early return
        phase(TableOwnerPhase::Published);
        require(sameOwner(r.cachedTable, observation.table), "delete receiver differs from cached table");
        receiptGeneration = r.cachedGeneration.value();
        r.phase = TableOwnerPhase::Deleting;
        break;
    case TableOwnerEvent::DeletingDestructorReturned:
        phase(TableOwnerPhase::Deleting);
        require(sameOwner(r.cachedTable, observation.table), "deleting destructor return owner differs");
        r.generations[*s.cached].deletingDestructorReturned = true;
        receiptGeneration = r.cachedGeneration.value();
        r.phase = TableOwnerPhase::DestructorReturned;
        break;
    case TableOwnerEvent::ClearDeletedCache:
        phase(TableOwnerPhase::DestructorReturned);
        receiptGeneration = r.cachedGeneration.value();
        r.generations[*s.cached].cacheCleared = true;
        r.cachedTable.reset();
        r.cachedGeneration.reset();
        s.cached.reset();
        s.getterResult.reset();
        r.tableField = TableOwnerField::KnownNull;
        r.phase = TableOwnerPhase::Idle;
        break;
    default: refuse("unrecognized source event");
    }
    if (!receiptGeneration && s.pending) receiptGeneration = r.generations[*s.pending].id;
    r.receipts.push_back({event, sourceSite(event), receiptGeneration, observation.qualification});
}

RefreshNotifierPrefixOutcome DoeTableOwnerState::enterRefreshNotifierPrefix(
    engine::ConversionSession& session) const
{
    if (!state_->matches(session)) refuse("actual Session admission scope is unavailable");
    const auto notifier = state_->notifier.lock();
    if (!notifier) refuse("actual Session notifier owner is unavailable");
    // 2cf3f1: suppression returns before +90, key construction, or registration lookup.
    if (notifier->suppressed) return RefreshNotifierPrefixOutcome::SuppressedCalleeReturned;
    // 2cf408: the unsuppressed dispatcher clears +90 before key/map/receiver work.
    notifier->flag90 = false;
    return RefreshNotifierPrefixOutcome::UnsuppressedDispatchRequired;
}

[[noreturn]] void DoeTableOwnerState::requireRefreshDelivery(engine::ConversionSession& session) const
{
    (void)enterRefreshNotifierPrefix(session);
    // Both branches still stop: neither key construction/delivery nor the
    // public continuation after a suppressed callee return is implemented here.
    if (state_->report.tableField == TableOwnerField::Unavailable) refuse("main-window +458 is unavailable");
    refuse("signalDoeTableRefresh() timer/UI receiver delivery remains unsupported");
}
} // namespace m2m::doe
