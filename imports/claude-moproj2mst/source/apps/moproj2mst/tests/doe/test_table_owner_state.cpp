// ctest-labels: m2m-doe
// Declared source-stage fixtures only: every opaque int lease is diagnostic,
// never a QWidget/QObject/QTimer or an original runtime observation.
#include <testkit/check.hpp>
#include <initializer_list>
#include <memory>
#include <string>

#include "m2m/common.hpp"
#include "m2m/doe_table_owner_state.hpp"
#include "m2m/doe_session_notifications.hpp"
#include "m2m/engine/conversion_session.hpp"
#include "m2m/model.hpp"
#include "m2m/mst/writer.hpp"

using namespace m2m;
using namespace m2m::doe;
namespace {
using Event = TableOwnerEvent;
using Phase = TableOwnerPhase;
using Field = TableOwnerField;
TableOwnerIdentity identity() { return std::make_shared<const int>(0); }
template<class Work> bool unsupported(const Work& work)
{
    try { work(); }
    catch (const Error& error) { return error.code == M2M_E_UNSUPPORTED; }
    return false;
}
struct Rig {
    engine::DiagramEffectDriver driver;
    engine::ConversionSession session{driver};
    DoeTableOwnerState state{session};
    TableOwnerIdentity table = identity(), timer = identity(), dock = identity();
    Rig() { initProject(session.project(), qt::L("C:/PROBLEM"), qt::L("table_owner")); }
    TableOwnerObservation observation() const
    { return {"declared source-stage adapter fixture; not native execution", table, timer, dock,
              session.sessionNotificationStateOwner()}; }
    void record(Event e) { state.record(session, e, observation()); }
    void initialize()
    { record(Event::InitializeFields); record(Event::PublishDock); }
    void beforeRegistration()
    {
        record(Event::BeginGetter); record(Event::AllocationReturned);
        record(Event::TimerParentRecorded); record(Event::ConstructorPrefixReturned);
    }
    void publish()
    {
        beforeRegistration(); record(Event::RegistrationReturned);
        record(Event::ConstructorReturned); record(Event::PublishCache);
    }
};
void checkUnknownQt(const TableOwnerReport& report)
{
    CHECK(!report.timerActive.has_value()); CHECK(!report.timerStopped.has_value());
    CHECK(!report.deleteWidgetEnabled.has_value()); CHECK(!report.nativeReceiverAlive.has_value());
    CHECK(!report.timerId.has_value());
}
}

TEST_CASE(doe_table_owner_starts_unavailable_and_needs_explicit_zero_stage)
{
    Rig r;
    CHECK(r.state.report().tableField == Field::Unavailable);
    CHECK(r.state.report().dockField == Field::Unavailable);
    CHECK(unsupported([&] { r.record(Event::BeginGetter); }));
    CHECK(unsupported([&] { r.state.requireRefreshDelivery(r.session); }));
    CHECK(r.state.report().receipts.empty());
    auto missing = r.observation(); missing.qualification.clear();
    CHECK(unsupported([&] { r.state.record(r.session, Event::InitializeFields, missing); }));
    r.record(Event::InitializeFields);
    CHECK(r.state.report().tableField == Field::KnownNull);
    CHECK(r.state.report().dockField == Field::KnownNull);
    CHECK(unsupported([&] { r.record(Event::InitializeFields); }));
    r.record(Event::BeginDelete); // native null branch changes neither field
    CHECK(r.state.report().generations.empty());
    checkUnknownQt(r.state.report());
}

TEST_CASE(doe_table_owner_is_one_session_lifetime_binding_across_project_release)
{
    engine::DiagramEffectDriver driver;
    engine::ConversionSession session(driver);
    auto* owner = session.doeTableOwner();
    REQUIRE(owner != nullptr);
    CHECK(owner->report().tableField == Field::Unavailable);
    CHECK(owner->report().dockField == Field::Unavailable);
    CHECK(unsupported([&] { owner->requireRefreshDelivery(session); }));
    session.releaseProjectAfterCloseEffects();
    CHECK(session.doeTableOwner() == owner);
    CHECK(owner->report().tableField == Field::Unavailable);
    CHECK(!owner->report().adapterScopeReleased);
}

TEST_CASE(doe_table_owner_preserves_ctor_registration_cache_and_dock_order)
{
    Rig r; r.initialize();
    const auto notifier = r.session.sessionNotificationStateOwner();
    notifier->flag90 = true;
    CHECK(r.state.report().receipts[1].sourceSite == 0x14001a48fULL);
    r.beforeRegistration();
    CHECK(r.state.report().tableField == Field::KnownNull);
    CHECK(unsupported([&] { r.record(Event::ConstructorReturned); }));
    CHECK(unsupported([&] { r.record(Event::PublishCache); }));
    r.record(Event::RegistrationReturned);
    CHECK(r.state.report().generations[0].registrationReturned);
    CHECK(r.state.report().tableField == Field::KnownNull);
    r.record(Event::ConstructorReturned);
    CHECK(r.state.report().phase == Phase::PublicationPending);
    CHECK(unsupported([&] { r.record(Event::DockInstallReturned); }));
    r.record(Event::PublishCache);
    CHECK(r.state.report().cachedTable == r.table);
    CHECK(!r.state.report().generations[0].dockInstallReturned);
    r.record(Event::DockInstallReturned);
    CHECK(r.state.report().generations[0].dockInstallReturned);
    CHECK(r.state.report().receipts.back().sourceSite == 0x14001a52aULL);
    CHECK(unsupported([&] { r.state.requireRefreshDelivery(r.session); }));
    // The unsuppressed native notifier clears +90 before the unknown key path.
    CHECK(!notifier->flag90 && notifier->attempts.empty() && notifier->emptyKeys.empty());
    checkUnknownQt(r.state.report());
}

TEST_CASE(doe_table_owner_ctor_parent_timer_and_actual_notifier_are_separate_receipts)
{
    Rig r; r.initialize(); r.record(Event::BeginGetter); r.record(Event::AllocationReturned);
    CHECK(unsupported([&] { r.record(Event::ConstructorPrefixReturned); }));
    auto foreign = r.observation(); foreign.table = identity();
    CHECK(unsupported([&] { r.state.record(r.session, Event::TimerParentRecorded, foreign); }));
    foreign = r.observation(); foreign.timer = r.table;
    CHECK(unsupported([&] { r.state.record(r.session, Event::TimerParentRecorded, foreign); }));
    r.record(Event::TimerParentRecorded);
    CHECK(unsupported([&] { r.record(Event::TimerParentRecorded); }));
    r.record(Event::ConstructorPrefixReturned);
    foreign = r.observation(); foreign.notifier = std::make_shared<SessionNotificationState>();
    CHECK(unsupported([&] { r.state.record(r.session, Event::RegistrationReturned, foreign); }));
    CHECK(r.state.report().phase == Phase::RegistrationPending);
    CHECK(!r.state.report().generations[0].registrationReturned);
    r.record(Event::RegistrationReturned);
    // Equal raw address with a different control block is not the allocation.
    foreign = r.observation();
    foreign.table = TableOwnerIdentity(r.table.get(), [](const void*) {});
    CHECK(unsupported([&] { r.state.record(r.session, Event::ConstructorReturned, foreign); }));
    r.record(Event::ConstructorReturned);
    CHECK(unsupported([&] { r.state.record(r.session, Event::PublishCache, foreign); }));
    CHECK(r.state.report().tableField == Field::KnownNull);
    r.record(Event::PublishCache);
    CHECK(r.state.report().generations[0].timer == r.timer);
    checkUnknownQt(r.state.report());
}

TEST_CASE(doe_table_owner_interrupted_constructor_retains_unpublished_prefix)
{
    for (const bool registered : {false, true}) {
        Rig r; r.initialize(); r.beforeRegistration();
        if (registered) r.record(Event::RegistrationReturned);
        r.record(Event::ConstructorInterrupted);
        CHECK(r.state.report().phase == Phase::ConstructorInterrupted);
        CHECK(r.state.report().tableField == Field::KnownNull);
        CHECK(r.state.report().generations[0].allocation == r.table);
        CHECK(r.state.report().generations[0].registrationReturned == registered);
        CHECK(!r.state.report().generations[0].constructorReturned);
        r.record(Event::BeginDelete); // null cache does not destroy pending object
        CHECK(!r.state.report().generations[0].deletingDestructorReturned);
        CHECK(unsupported([&] { r.record(Event::BeginGetter); }));
        CHECK(unsupported([&] { r.record(Event::PublishCache); }));
    }
}

TEST_CASE(doe_table_owner_explicit_null_allocation_still_publishes_and_passes_null_to_dock)
{
    Rig r; r.initialize(); r.record(Event::BeginGetter);
    CHECK(unsupported([&] { r.record(Event::AllocationReturnedNull); }));
    auto null = r.observation(); null.table.reset();
    r.state.record(r.session, Event::AllocationReturnedNull, null);
    CHECK(r.state.report().phase == Phase::NullPublicationPending);
    CHECK(unsupported([&] { r.record(Event::PublishCache); }));
    r.state.record(r.session, Event::PublishCache, null);
    CHECK(r.state.report().tableField == Field::KnownNull);
    CHECK(r.state.report().generations[0].cachePublished);
    CHECK(!r.state.report().generations[0].constructorReturned);
    r.state.record(r.session, Event::DockInstallReturned, null);
    CHECK(r.state.report().generations[0].dockInstallReturned);
    r.record(Event::BeginGetter); // retry after a qualified null return
    CHECK(r.state.report().generations.size() == 2);
    checkUnknownQt(r.state.report());
}

TEST_CASE(doe_table_owner_cache_hit_and_dock_refusal_retain_published_receiver)
{
    Rig r; r.initialize(); r.publish();
    auto wrongDock = r.observation(); wrongDock.dock = identity();
    CHECK(unsupported([&] { r.state.record(r.session, Event::DockInstallReturned, wrongDock); }));
    CHECK(r.state.report().cachedTable == r.table);
    CHECK(r.state.report().generations[0].registrationReturned);
    r.record(Event::BeginGetter);
    CHECK(r.state.report().generations.size() == 1);
    CHECK(r.state.report().receipts.back().generation == 1);
    CHECK(unsupported([&] { r.record(Event::AllocationReturned); }));
    r.record(Event::DockInstallReturned);
    CHECK(unsupported([&] { r.record(Event::DockInstallReturned); }));
}

TEST_CASE(doe_table_owner_delete_returns_before_clear_and_keeps_dock_and_history)
{
    Rig r; r.initialize(); r.publish(); r.record(Event::DockInstallReturned);
    CHECK(unsupported([&] { r.record(Event::ClearDeletedCache); }));
    auto foreign = r.observation(); foreign.table = identity();
    CHECK(unsupported([&] { r.state.record(r.session, Event::BeginDelete, foreign); }));
    CHECK(r.state.report().phase == Phase::Published);
    r.record(Event::BeginDelete);
    CHECK(r.state.report().cachedTable == r.table);
    CHECK(unsupported([&] { r.record(Event::ClearDeletedCache); }));
    CHECK(unsupported([&] { r.record(Event::BeginGetter); }));
    r.record(Event::DeletingDestructorReturned);
    CHECK(r.state.report().cachedTable == r.table);
    CHECK(r.state.report().generations[0].deletingDestructorReturned);
    CHECK(unsupported([&] { r.record(Event::BeginGetter); }));
    r.record(Event::ClearDeletedCache);
    CHECK(r.state.report().tableField == Field::KnownNull);
    CHECK(!r.state.report().cachedTable);
    CHECK(r.state.report().dock == r.dock && r.state.report().dockField == Field::Cached);
    CHECK(r.state.report().generations[0].cacheCleared);
    r.publish(); // same token/address is a distinct new allocation generation
    CHECK(r.state.report().generations.size() == 2);
    CHECK(r.state.report().cachedGeneration.value() == 2);
    CHECK(r.state.report().generations[0].cacheCleared);
    CHECK(!r.state.report().generations[1].cacheCleared);
    checkUnknownQt(r.state.report());
}

TEST_CASE(doe_table_owner_getter_is_independent_of_dock_and_project_lifetime)
{
    Rig r; r.record(Event::InitializeFields); r.publish();
    CHECK(r.state.report().dockField == Field::KnownNull);
    CHECK(unsupported([&] { r.record(Event::DockInstallReturned); }));
    r.session.releaseProjectAfterCloseEffects();
    CHECK(!r.session.liveProject());
    r.record(Event::BeginGetter);
    CHECK(r.state.report().cachedTable == r.table);
    CHECK(unsupported([&] { r.state.requireRefreshDelivery(r.session); }));
    r.record(Event::PublishDock); r.record(Event::DockInstallReturned);
    CHECK(r.state.report().generations[0].dockInstallReturned);
}

TEST_CASE(doe_table_owner_rejects_foreign_session_and_scope_release_is_not_native_delete)
{
    Rig r; r.initialize(); r.publish();
    Rig foreign;
    CHECK(unsupported([&] { r.state.record(foreign.session, Event::BeginGetter, r.observation()); }));
    CHECK(unsupported([&] { r.state.requireRefreshDelivery(foreign.session); }));
    r.state.releaseAdmissionScope();
    CHECK(r.state.report().adapterScopeReleased);
    CHECK(r.state.report().cachedTable == r.table);
    CHECK(!r.state.report().generations[0].deletingDestructorReturned);
    CHECK(!r.state.report().generations[0].cacheCleared);
    CHECK(unsupported([&] { r.record(Event::BeginDelete); }));
    CHECK(unsupported([&] { r.state.requireRefreshDelivery(r.session); }));
    checkUnknownQt(r.state.report());
}

int main() { return testkit::run_all(); }
