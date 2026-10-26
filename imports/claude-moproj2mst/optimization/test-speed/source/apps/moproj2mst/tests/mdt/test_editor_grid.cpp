// ctest-labels: m2m-mdt
#include <testkit/check.hpp>
#include <array>
#include <memory>
#include <vector>
#include "m2m/engine/simcontrol300_owner.hpp"
#include "m2m/mst/writer.hpp"

using namespace m2m;
using namespace m2m::engine;
using namespace m2m::engine::simcontrol300;

namespace {
struct Fixture {
    Project project;
    Operation operation;
    RemeshState remesh;
    std::shared_ptr<WidgetRuntimeState> runtime =
        std::make_shared<WidgetRuntimeState>(true, "Isolated native editor/grid runtime");
    std::unique_ptr<StagedEditor300> factory;
    EditorSetupState setup;
    Fixture()
    {
        operation.project = &project;
        operation.engineModel = std::make_shared<GuiModel>(nullptr, 3);
        factory = std::make_unique<StagedEditor300>(binding());
        REQUIRE(factory->bindWidgetRuntime(binding(), runtime));
        REQUIRE(factory->bindSetupOwner(setup));
    }
    Editor300Binding binding() { return {project, operation, *operation.engineModel, remesh}; }
    EditorGridContext context()
    { return {factory.get(), "Isolated actual first-grid constructor context", runtime->threadState()}; }
    void propertyReturned()
    {
        WidgetApplicationEventObservation app;
        app.present = true; app.closing = false; app.internalCallbackCount = 0;
        app.filters = std::vector<WidgetApplicationEventObservation::Filter>{
            {WidgetApplicationEventObservation::Filter::Kind::WindowsVistaStyle, true}};
        runtime->observeApplicationEvents(app, "Isolated source-closed property application profile");
        WidgetReceiverEventObservation receiver;
        receiver.sameApplicationThread = true; receiver.layoutPresent = false;
        receiver.objectFilterCount = 0;
        factory->widgetState()->observeEventReceiver(receiver, WidgetEventReceiverKind::QWidget,
            "Isolated actual editor property receiver");
        NativeSetupObservation setupContext;
        setupContext.receiver = factory.get(); setupContext.qualification = "Isolated native setup input";
        setupContext.objectName = qt::String(); setupContext.declarativeHookDataPresent = false;
        setupContext.widgetCreated = false; setupContext.widgetExtra = NativeSetupWidgetExtra::Absent;
        setupContext.geometryEndpoints = std::array<std::int32_t, 4>{0, 0, 639, 479};
        setupContext.metaObject = NativeSetupMetaObject::NativeStatic;
        setupContext.propertyTable = NativeSetupPropertyTable::AbsentExtraData;
        REQUIRE(factory->beginNativeSetupPrefix(binding(), setupContext).stage == NativeSetupPrefixStage::PropertyNotification);
        REQUIRE(factory->continueNativeSetupPropertyNotification(binding()).stage == NativeSetupPrefixStage::PropertySetReturned);
    }
    EditorGridReport resume() { return factory->continueNativeSetupFirstGrid(binding(), context()); }
};
}

TEST_CASE(editor_grid_requires_reached_property_return_before_caller_cleanup_or_allocation)
{
    Fixture f;
    CHECK(f.resume().issue == EditorGridIssue::PropertyNotReturned);
    CHECK(!f.factory->firstGridConstruction()); CHECK(!f.setup.nativePrefix().falseVariantDestroyed);
    CHECK(!f.setup.nativePrefix().report.attempted); CHECK(f.setup.nativePrefix().effects.empty());
    CHECK(f.factory->widgetState()->knownChildSuffix().empty()); CHECK(!f.setup.firstNativeGridUi());
    CHECK(!f.factory->widgetState()->installedLayout());
}

TEST_CASE(editor_grid_validates_actual_factory_binding_and_qualification_before_effects)
{
    Fixture f; f.propertyReturned();
    StagedEditor300 unrelated(f.binding());
    auto context = f.context(); context.receiver = &unrelated;
    CHECK(f.factory->continueNativeSetupFirstGrid(f.binding(), context).issue == EditorGridIssue::IdentityMismatch);
    context = f.context(); context.qualification.clear();
    CHECK(f.factory->continueNativeSetupFirstGrid(f.binding(), context).issue == EditorGridIssue::MissingQualification);
    auto different = std::make_shared<GuiModel>(nullptr, 3);
    CHECK(f.factory->continueNativeSetupFirstGrid({f.project, f.operation, *different, f.remesh}, f.context()).issue ==
        EditorGridIssue::IdentityMismatch);
    CHECK(!f.factory->firstGridConstruction()); CHECK(!f.setup.nativePrefix().falseVariantDestroyed);
    CHECK(f.setup.nativePrefix().effects.back() == NativeSetupPrefixEffect::DynamicPropertySetterReturned);
}

TEST_CASE(editor_grid_keeps_source_allocation_before_unknown_thread_without_parenting)
{
    Fixture f; f.propertyReturned();
    auto context = f.context(); context.currentThread.reset();
    const auto result = f.factory->continueNativeSetupFirstGrid(f.binding(), context);
    CHECK(result.issue == EditorGridIssue::CurrentThreadUnavailable);
    CHECK(result.stage == EditorGridStage::ThreadSelection); CHECK(result.attempted);
    CHECK(f.setup.nativePrefix().falseVariantConstructed); CHECK(f.setup.nativePrefix().falseVariantDestroyed);
    CHECK(f.setup.nativePrefix().effects.back() == NativeSetupPrefixEffect::CallerFalseVariantDestroyed);
    const auto* frame = f.factory->firstGridConstruction(); REQUIRE(frame);
    const auto grid = frame->pendingAllocation(); REQUIRE(grid);
    CHECK(frame->actualGrid() == grid); CHECK(!frame->ownedChild());
    CHECK(grid->nativeOwnerAlive()); CHECK(!grid->parentWidget()); CHECK(!grid->threadState());
    CHECK(grid->constructionProfile() == GridConstructionProfile::ParentedPending);
    CHECK(grid->rowCount() == 0); CHECK(grid->columnCount() == 0);
    CHECK(grid->layoutFlags() == 6u); CHECK(grid->layoutFlagsKnownMask() == 0x0fu);
    CHECK(grid->dirtyFlags() == 4u); CHECK(grid->dirtyFlagsKnownMask() == 0x17u);
    CHECK(grid->cachedSizeValue() == -1); CHECK(!grid->isTopLevelLayout()); CHECK(grid->items().empty());
    const std::array<int, 4> margins{-1, -1, -1, -1}, rectangle{0, 0, -1, -1};
    CHECK(grid->contentsMargins() == margins); CHECK(grid->geometryEndpoints() == rectangle);
    CHECK(f.factory->widgetState()->knownChildSuffix().empty()); CHECK(!f.factory->widgetState()->installedLayout());
    CHECK(!f.setup.firstNativeGridUi()); CHECK(!result.constructorReturned); CHECK(!result.completeForNativeSetup);
    const std::vector<EditorGridEffect> expected{EditorGridEffect::AllocationAndPrivateConstruction,
        EditorGridEffect::QObjectBaseInstalled};
    CHECK(frame->effects() == expected);
}

TEST_CASE(editor_grid_thread_mismatch_keeps_pending_allocation_before_parent_stores)
{
    Fixture f; f.propertyReturned();
    auto otherRuntime = std::make_shared<WidgetRuntimeState>(true, "Different actual thread fixture");
    auto context = f.context(); context.currentThread = otherRuntime->threadState();
    const auto result = f.factory->continueNativeSetupFirstGrid(f.binding(), context);
    CHECK(result.issue == EditorGridIssue::ParentThreadMismatch);
    const auto grid = f.factory->firstGridConstruction()->pendingAllocation(); REQUIRE(grid);
    CHECK(!grid->threadState()); CHECK(!grid->parentWidget());
    CHECK(f.factory->widgetState()->knownChildSuffix().empty());
    CHECK(!f.factory->widgetState()->installedLayout()); CHECK(!f.setup.firstNativeGridUi());
    const auto retained = grid.get();
    CHECK(f.resume().issue == EditorGridIssue::UnknownReceiveChildEvents);
    CHECK(f.factory->firstGridConstruction()->ownedChild().get() == retained);
}

TEST_CASE(editor_grid_same_thread_transfers_the_same_record_before_child_event_predicate)
{
    Fixture f; f.propertyReturned();
    auto context = f.context(); context.currentThread.reset();
    f.factory->continueNativeSetupFirstGrid(f.binding(), context);
    const auto allocation = f.factory->firstGridConstruction()->pendingAllocation(); REQUIRE(allocation);
    const auto result = f.resume();
    CHECK(result.stage == EditorGridStage::ChildEventPredicate);
    CHECK(result.issue == EditorGridIssue::UnknownReceiveChildEvents);
    const auto* frame = f.factory->firstGridConstruction();
    CHECK(!frame->pendingAllocation()); CHECK(frame->ownedChild() == allocation);
    CHECK(allocation->parentWidget() == f.factory->widgetState());
    CHECK(allocation->threadState() == f.runtime->threadState());
    REQUIRE(f.factory->widgetState()->knownChildSuffix().size() == 1);
    CHECK(f.factory->widgetState()->childPrefixUnrecovered());
    CHECK(f.factory->widgetState()->knownChildSuffix().front() == allocation.get());
    CHECK(allocation->rowCount() == 0); CHECK(allocation->columnCount() == 0); CHECK(allocation->layoutFlags() == 6u);
    CHECK(!f.factory->widgetState()->installedLayout()); CHECK(f.factory->widgetState()->layoutPresence() == false);
    CHECK(!f.setup.firstNativeGridUi()); CHECK(f.runtime->threadState()->retainedNotifyDelta() == 0);
    const std::vector<EditorGridEffect> expected{EditorGridEffect::AllocationAndPrivateConstruction,
        EditorGridEffect::QObjectBaseInstalled, EditorGridEffect::ThreadDataRetained,
        EditorGridEffect::ParentAssigned, EditorGridEffect::ChildMembershipAppended};
    CHECK(frame->effects() == expected);
}

TEST_CASE(editor_grid_receive_enabled_retains_child_event_at_unknown_receiver_virtual)
{
    Fixture f; f.propertyReturned(); f.factory->widgetState()->observeReceivesChildEvents(true);
    const auto result = f.resume();
    CHECK(result.stage == EditorGridStage::ChildAddedNotification);
    CHECK(result.issue == EditorGridIssue::ChildAddedNotificationPending);
    REQUIRE(f.factory->firstGridConstruction()->ownedChild());
    CHECK(f.factory->firstGridConstruction()->ownedChild()->rowCount() == 0);
    const auto* event = f.factory->firstGridConstruction()->childEvent(); REQUIRE(event);
    CHECK(event->nativeAlive()); CHECK(event->rawType() == 0x44);
    CHECK(event->child() == f.factory->firstGridConstruction()->ownedChild());
    CHECK(f.runtime->threadState()->retainedNotifyDelta() == 1);
    CHECK(!f.runtime->threadState()->postedEvents().populationKnown());
    CHECK(!f.factory->widgetState()->installedLayout()); CHECK(!f.setup.firstNativeGridUi());
    CHECK(f.setup.nativePrefix().notification().completed); // only earlier raw0xaa
    CHECK(!StagedEditor300::completeForPublicSave);
}

TEST_CASE(editor_grid_receive_disabled_skips_event_and_commits_global_qobject_count_once)
{
    Fixture f; f.propertyReturned(); f.factory->widgetState()->observeReceivesChildEvents(false);
    const auto result = f.resume();
    CHECK(result.stage == EditorGridStage::LayoutConstruction);
    CHECK(result.issue == EditorGridIssue::LayoutConstructionUnrecovered);
    CHECK(f.factory->firstGridConstruction()->ownedChild()->parentWidget() == f.factory->widgetState());
    CHECK(f.runtime->threadState()->retainedNotifyDelta() == 0);
    CHECK(!f.factory->firstGridConstruction()->childEvent());
    CHECK(f.runtime->retainedQObjectConstructionDelta() == 1u);
    CHECK(f.resume().issue == EditorGridIssue::LayoutConstructionUnrecovered);
    CHECK(f.runtime->retainedQObjectConstructionDelta() == 1u);
    CHECK(!result.constructorReturned); CHECK(!f.factory->widgetState()->installedLayout());
    CHECK(!f.setup.firstNativeGridUi());
}

TEST_CASE(editor_grid_repeated_guards_preserve_allocation_membership_and_caller_dtor_once)
{
    Fixture f; f.propertyReturned();
    auto context = f.context(); context.currentThread.reset();
    f.factory->continueNativeSetupFirstGrid(f.binding(), context);
    const auto first = f.factory->firstGridConstruction()->actualGrid();
    const auto setupEffects = f.setup.nativePrefix().effects;
    for (int i = 0; i != 2; ++i) {
        CHECK(f.factory->continueNativeSetupFirstGrid(f.binding(), context).issue == EditorGridIssue::CurrentThreadUnavailable);
        CHECK(f.factory->firstGridConstruction()->actualGrid() == first);
        CHECK(f.setup.nativePrefix().effects == setupEffects);
    }
    CHECK(f.resume().issue == EditorGridIssue::UnknownReceiveChildEvents);
    const auto effects = f.factory->firstGridConstruction()->effects();
    CHECK(f.resume().issue == EditorGridIssue::UnknownReceiveChildEvents);
    f.factory->widgetState()->observeReceivesChildEvents(true);
    CHECK(f.resume().issue == EditorGridIssue::ChildAddedNotificationPending);
    const auto withEvent = f.factory->firstGridConstruction()->effects();
    CHECK(withEvent.size() == effects.size() + 1);
    CHECK(f.resume().issue == EditorGridIssue::ChildAddedNotificationPending);
    CHECK(f.factory->firstGridConstruction()->actualGrid() == first);
    CHECK(f.factory->firstGridConstruction()->effects() == withEvent);
    CHECK(f.factory->widgetState()->knownChildSuffix().size() == 1);
    CHECK(f.setup.nativePrefix().effects == setupEffects);
}

TEST_CASE(editor_grid_cannot_substitute_thread_after_committed_membership)
{
    Fixture f; f.propertyReturned(); CHECK(f.resume().issue == EditorGridIssue::UnknownReceiveChildEvents);
    const auto child = f.factory->firstGridConstruction()->ownedChild();
    auto other = std::make_shared<WidgetRuntimeState>(true, "Replacement thread fixture");
    auto context = f.context(); context.currentThread = other->threadState();
    CHECK(f.factory->continueNativeSetupFirstGrid(f.binding(), context).issue == EditorGridIssue::CurrentThreadChanged);
    CHECK(child->threadState() == f.runtime->threadState());
    CHECK(f.factory->widgetState()->knownChildSuffix().size() == 1);
    CHECK(f.resume().issue == EditorGridIssue::UnknownReceiveChildEvents);
}

TEST_CASE(editor_grid_rejects_same_address_thread_with_foreign_lifetime_control_block)
{
    Fixture f; f.propertyReturned();
    const auto actual = f.runtime->threadState();
    const std::shared_ptr<WidgetThreadState> foreign(actual.get(), [](WidgetThreadState*) {});
    REQUIRE(foreign.get() == actual.get());
    REQUIRE(foreign.owner_before(actual) || actual.owner_before(foreign));
    auto context = f.context(); context.currentThread = foreign;
    CHECK(f.factory->continueNativeSetupFirstGrid(f.binding(), context).issue == EditorGridIssue::ParentThreadMismatch);
    const auto allocation = f.factory->firstGridConstruction()->pendingAllocation(); REQUIRE(allocation);
    CHECK(!allocation->threadState()); CHECK(!allocation->parentWidget());
    CHECK(f.factory->widgetState()->knownChildSuffix().empty());
    CHECK(f.resume().issue == EditorGridIssue::UnknownReceiveChildEvents);
    const auto child = f.factory->firstGridConstruction()->ownedChild(); REQUIRE(child);
    CHECK(child == allocation);
    const auto effects = f.factory->firstGridConstruction()->effects();
    CHECK(f.factory->continueNativeSetupFirstGrid(f.binding(), context).issue == EditorGridIssue::CurrentThreadChanged);
    const auto retained = child->threadState();
    CHECK(retained.get() == actual.get());
    CHECK(!retained.owner_before(actual)); CHECK(!actual.owner_before(retained));
    CHECK(f.factory->firstGridConstruction()->effects() == effects);
    CHECK(f.factory->widgetState()->knownChildSuffix().size() == 1);
    CHECK(f.resume().issue == EditorGridIssue::UnknownReceiveChildEvents);
}

TEST_CASE(editor_grid_pending_and_parented_leases_are_invalidated_with_actual_editor)
{
    for (bool parented : {false, true}) {
        Fixture f; f.propertyReturned();
        auto context = f.context(); if (!parented) context.currentThread.reset();
        f.factory->continueNativeSetupFirstGrid(f.binding(), context);
        const auto grid = f.factory->firstGridConstruction()->actualGrid(); REQUIRE(grid);
        const auto widget = f.factory->widgetState(); REQUIRE(grid->nativeOwnerAlive());
        f.factory.reset();
        CHECK(!grid->nativeOwnerAlive()); CHECK(!widget->nativeOwnerAlive());
        CHECK(!grid->parentWidget()); CHECK(!grid->threadState());
        CHECK(!f.setup.firstNativeGridUi());
    }
}

TEST_CASE(editor_grid_expired_model_rejects_continuation_without_losing_owned_child)
{
    Fixture f; f.propertyReturned(); f.resume();
    const auto grid = f.factory->firstGridConstruction()->ownedChild(); REQUIRE(grid);
    const auto effects = f.factory->firstGridConstruction()->effects();
    f.operation.engineModel.reset();
    auto different = std::make_shared<GuiModel>(nullptr, 3);
    CHECK(f.factory->continueNativeSetupFirstGrid({f.project, f.operation, *different, f.remesh}, f.context()).issue ==
        EditorGridIssue::IdentityMismatch);
    CHECK(f.factory->firstGridConstruction()->ownedChild() == grid); CHECK(grid->nativeOwnerAlive());
    CHECK(f.factory->firstGridConstruction()->effects() == effects);
    CHECK(f.factory->widgetState()->knownChildSuffix().size() == 1);
}

TEST_CASE(editor_grid_true_child_notification_keeps_event_alive_until_post_parent_context)
{
    Fixture f; f.propertyReturned(); f.factory->widgetState()->observeReceivesChildEvents(true);
    WidgetReceiverEventObservation receiver; receiver.childEventHandler = WidgetChildEventHandler::QObjectLeaf;
    f.factory->widgetState()->observeEventReceiver(receiver, WidgetEventReceiverKind::QWidget,
        "Isolated source-bound QObject childEvent leaf");
    f.runtime->observeQObjectCount(1892, "Isolated original pre-grid count");
    CHECK(f.resume().issue == EditorGridIssue::DeclarativeContextUnavailable);
    const auto* frame = f.factory->firstGridConstruction(); const auto* event = frame->childEvent();
    REQUIRE(event); CHECK(event->nativeAlive()); CHECK(!event->posted());
    CHECK(event->child() == frame->ownedChild());
    CHECK(frame->childNotification().completed); CHECK(frame->childNotification().originalResult == true);
    CHECK(f.setup.nativePrefix().notification().originalResult == false);
    CHECK(f.runtime->threadState()->retainedNotifyDelta() == 0);
    CHECK(f.runtime->qobjectCount() == 1892u);
    const auto effects = frame->effects();
    CHECK(f.resume().issue == EditorGridIssue::DeclarativeContextUnavailable);
    CHECK(frame->effects() == effects); CHECK(frame->childEvent() == event); CHECK(event->nativeAlive());
    WidgetApplicationEventObservation app; app.declarativeChildEventsEnabled = false;
    f.runtime->observeApplicationEvents(app, "Isolated post-parent declarative flag false");
    CHECK(f.resume().issue == EditorGridIssue::LayoutConstructionUnrecovered);
    CHECK(!event->nativeAlive()); CHECK(frame->ownedChild()->nativeOwnerAlive());
    CHECK(f.runtime->qobjectCount() == 1893u);
    REQUIRE(frame->effects().size() >= 2);
    CHECK(frame->effects()[frame->effects().size() - 2] == EditorGridEffect::ChildEventDestroyed);
    CHECK(frame->effects().back() == EditorGridEffect::QObjectCountIncremented);
    CHECK(frame->ownedChild()->rowCount() == 0); CHECK(frame->ownedChild()->layoutFlags() == 6u);
    CHECK(!f.factory->widgetState()->installedLayout()); CHECK(!f.setup.firstNativeGridUi());
}

TEST_CASE(editor_grid_declarative_true_retains_live_event_after_closed_notification)
{
    Fixture f; f.propertyReturned(); f.factory->widgetState()->observeReceivesChildEvents(true);
    WidgetReceiverEventObservation receiver; receiver.childEventHandler = WidgetChildEventHandler::QObjectLeaf;
    f.factory->widgetState()->observeEventReceiver(receiver, WidgetEventReceiverKind::QWidget,
        "Isolated childEvent leaf");
    WidgetApplicationEventObservation app; app.declarativeChildEventsEnabled = true;
    f.runtime->observeApplicationEvents(app, "Isolated declarative-enabled process");
    CHECK(f.resume().issue == EditorGridIssue::DeclarativeChildrenUnrecovered);
    const auto* frame = f.factory->firstGridConstruction();
    CHECK(frame->childNotification().originalResult == true); CHECK(frame->childEvent()->nativeAlive());
    CHECK(f.runtime->retainedQObjectConstructionDelta() == 0u);
    CHECK(f.runtime->threadState()->retainedNotifyDelta() == 0);
    CHECK(f.resume().issue == EditorGridIssue::DeclarativeChildrenUnrecovered);
    CHECK(f.runtime->retainedQObjectConstructionDelta() == 0u);
}

TEST_CASE(editor_grid_pending_receiver_resume_uses_same_event_and_returns_depth_before_counter)
{
    Fixture f; f.propertyReturned(); f.factory->widgetState()->observeReceivesChildEvents(true);
    CHECK(f.resume().issue == EditorGridIssue::ChildAddedNotificationPending);
    const auto* event = f.factory->firstGridConstruction()->childEvent(); REQUIRE(event);
    CHECK(f.runtime->threadState()->retainedNotifyDelta() == 1);
    WidgetReceiverEventObservation receiver; receiver.childEventHandler = WidgetChildEventHandler::QObjectLeaf;
    f.factory->widgetState()->observeEventReceiver(receiver, WidgetEventReceiverKind::QWidget,
        "Resolved exact existing receiver virtual");
    WidgetApplicationEventObservation app; app.declarativeChildEventsEnabled = false;
    f.runtime->observeApplicationEvents(app, "Resolved actual process declarative flag");
    CHECK(f.resume().issue == EditorGridIssue::LayoutConstructionUnrecovered);
    CHECK(f.factory->firstGridConstruction()->childEvent() == event); CHECK(!event->nativeAlive());
    CHECK(f.factory->firstGridConstruction()->childNotification().originalResult == true);
    CHECK(f.runtime->threadState()->retainedNotifyDelta() == 0);
    CHECK(f.runtime->retainedQObjectConstructionDelta() == 1u);
    CHECK(f.factory->widgetState()->knownChildSuffix().size() == 1);
}

int main() { return testkit::run_all(); }
