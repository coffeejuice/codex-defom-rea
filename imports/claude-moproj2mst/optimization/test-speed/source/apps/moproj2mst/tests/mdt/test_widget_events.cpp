// ctest-labels: m2m-mdt
// Isolated shared-runtime seams; no original constructor/queue completion claim.
#include <testkit/check.hpp>
#include <limits>
#include "m2m/engine/widget_events.hpp"
#include "m2m/doe_session_notifications.hpp"
#include "m2m/model.hpp"
using namespace m2m;
using namespace m2m::engine;
namespace {
template<class F> void refused(F&& f) {
    bool caught = false;
    try { f(); } catch (const Error& e) { caught = e.code == M2M_E_UNSUPPORTED; }
    CHECK(caught);
}
WidgetApplicationEventObservation appObservation() {
    WidgetApplicationEventObservation o;
    o.present = true; o.closing = false; o.internalCallbackCount = 0;
    o.filters = std::vector<WidgetApplicationEventObservation::Filter>{
        {WidgetApplicationEventObservation::Filter::Kind::WindowsVistaStyle, true}};
    return o; // gesture state deliberately unknown: raw0xaa bypasses it
}
WidgetReceiverEventObservation receiverObservation() {
    WidgetReceiverEventObservation o;
    o.sameApplicationThread = true; o.objectFilterCount = 0; o.layoutPresent = false;
    return o;
}
struct Fixture {
    std::shared_ptr<MainViewState> owner = std::make_shared<MainViewState>();
    std::shared_ptr<WidgetState> widget = owner->widgetState();
    std::shared_ptr<WidgetRuntimeState> runtime = std::make_shared<WidgetRuntimeState>(true, "isolated event fixture");
    std::unique_ptr<WidgetEventState> event = WidgetEventState::dynamicPropertyChange("sizeGripEnabled");
    WidgetEventDispatch dispatch;
    Fixture() { runtime->bindWidgetComponent(widget); }
    void qualify() {
        runtime->observeApplicationEvents(appObservation(), "isolated application inputs");
        widget->observeEventReceiver(receiverObservation(), WidgetEventReceiverKind::QWidget, "isolated receiver inputs");
    }
    const WidgetDispatchReport& resume() { return dispatch.resume(*runtime, widget, *event); }
};
}

TEST_CASE(widget_event_property_returns_false_with_unknown_depth_and_gestures)
{
    Fixture f; f.qualify();
    auto& r = f.resume();
    CHECK(r.completed && r.originalResult == false);
    CHECK(!f.runtime->threadState()->notifyDepth());
    CHECK(f.runtime->threadState()->retainedNotifyDelta() == 0);
    CHECK(!f.runtime->threadState()->postedEvents().populationKnown());
    CHECK(f.event->accepted() && !f.event->posted() && !f.event->spontaneous());
    CHECK(f.event->nativeAlive());
    CHECK(!f.runtime->hasLiveRegistration(f.owner.get()));
}

TEST_CASE(widget_event_pending_depth_wraps_and_normal_return_restores_once)
{
    Fixture f;
    auto a = appObservation(); a.closing.reset();
    f.runtime->observeApplicationEvents(a, "partial input");
    f.widget->observeEventReceiver(receiverObservation(), WidgetEventReceiverKind::QWidget, "receiver");
    const auto maximum = std::numeric_limits<std::uint32_t>::max();
    f.runtime->threadState()->observeNotifyDepth(maximum, "known depth");
    CHECK(f.resume().stage == WidgetDispatchStage::Application);
    CHECK(f.runtime->threadState()->notifyDepth() == 0u);
    CHECK(f.runtime->threadState()->retainedNotifyDelta() == 1);
    f.resume();
    CHECK(f.runtime->threadState()->retainedNotifyDelta() == 1);
    f.runtime->observeApplicationEvents(appObservation(), "fills unknown closing state");
    CHECK(f.resume().completed);
    CHECK(f.runtime->threadState()->notifyDepth() == maximum);
    CHECK(f.resume().completed);
    CHECK(f.runtime->threadState()->notifyDepth() == maximum);
}

TEST_CASE(widget_event_internal_callback_guard_precedes_depth_increment)
{
    Fixture f;
    auto a = appObservation(); a.internalCallbackCount.reset();
    f.runtime->observeApplicationEvents(a, "unknown internal population");
    f.widget->observeEventReceiver(receiverObservation(), WidgetEventReceiverKind::QWidget, "receiver");
    CHECK(f.resume().stage == WidgetDispatchStage::InternalCallbacks);
    CHECK(f.runtime->threadState()->retainedNotifyDelta() == 0);
    f.runtime->observeApplicationEvents(appObservation(), "qualified empty internal registry");
    CHECK(f.resume().completed);
}

TEST_CASE(widget_event_unknown_object_filters_preserve_pending_call_and_same_event)
{
    Fixture f;
    f.runtime->observeApplicationEvents(appObservation(), "application");
    auto o = receiverObservation(); o.objectFilterCount.reset();
    f.widget->observeEventReceiver(o, WidgetEventReceiverKind::QWidget, "partial receiver");
    CHECK(f.resume().stage == WidgetDispatchStage::ReceiverFilters);
    CHECK(f.runtime->threadState()->retainedNotifyDelta() == 1);
    auto replacement = WidgetEventState::dynamicPropertyChange("sizeGripEnabled");
    refused([&] { f.dispatch.resume(*f.runtime, f.widget, *replacement); });
    refused([&] { f.event->destroyStackEvent(); });
    f.widget->observeEventReceiver(receiverObservation(), WidgetEventReceiverKind::QWidget, "fills filter population");
    CHECK(f.resume().completed);
    CHECK(f.runtime->threadState()->retainedNotifyDelta() == 0);
}

TEST_CASE(widget_event_filter_affinity_failure_retains_diagnostic_boundary)
{
    Fixture f;
    auto a = appObservation(); a.filters->front().sameApplicationThread = false;
    f.runtime->observeApplicationEvents(a, "foreign filter fixture");
    f.widget->observeEventReceiver(receiverObservation(), WidgetEventReceiverKind::QWidget, "receiver");
    CHECK(f.resume().stage == WidgetDispatchStage::ApplicationFilters);
    CHECK(!f.dispatch.report().completed && !f.dispatch.report().originalResult);
    CHECK(f.runtime->threadState()->retainedNotifyDelta() == 1);
    refused([&] { f.runtime->observeApplicationEvents(appObservation(), "cannot overwrite affinity"); });
}

TEST_CASE(widget_event_receiver_type_can_be_qualified_without_replaying_depth)
{
    Fixture f;
    f.runtime->observeApplicationEvents(appObservation(), "application");
    f.widget->observeEventReceiver(receiverObservation(), WidgetEventReceiverKind::Unknown, "unknown receiver binding");
    CHECK(f.resume().stage == WidgetDispatchStage::Receiver);
    CHECK(f.runtime->threadState()->retainedNotifyDelta() == 1);
    f.widget->observeEventReceiver({}, WidgetEventReceiverKind::QWidget, "source-bound virtual destination");
    CHECK(f.resume().completed);
    CHECK(f.runtime->threadState()->retainedNotifyDelta() == 0);
}

TEST_CASE(widget_event_custom_dpi_does_not_use_property_no_effect_leaf)
{
    Fixture f; f.qualify();
    f.event = WidgetEventState::dynamicPropertyChange("_q_customDpiX");
    CHECK(f.resume().stage == WidgetDispatchStage::Receiver);
    CHECK(!f.dispatch.report().completed);
    CHECK(f.runtime->threadState()->retainedNotifyDelta() == 1);
}

TEST_CASE(widget_event_closing_application_returns_before_receiver_dependencies)
{
    Fixture f;
    WidgetApplicationEventObservation a;
    a.present = true; a.closing = true; a.internalCallbackCount = 0;
    f.runtime->observeApplicationEvents(a, "closing application");
    CHECK(f.resume().completed);
    CHECK(f.dispatch.report().originalResult == false);
    CHECK(f.runtime->threadState()->retainedNotifyDelta() == 0);
}

TEST_CASE(widget_event_observations_do_not_replace_current_application_inputs)
{
    Fixture f; f.qualify();
    auto conflict = appObservation(); conflict.internalCallbackCount = 1; conflict.closing = true;
    refused([&] { f.runtime->observeApplicationEvents(conflict, "stale inputs"); });
    CHECK(f.runtime->applicationEvents().closing == false);
    CHECK(f.runtime->applicationEvents().internalCallbackCount == 0u);
    CHECK(f.resume().completed);
}

TEST_CASE(widget_event_native_owner_death_invalidates_escaped_component)
{
    Fixture f; f.qualify();
    f.owner.reset();
    CHECK(!f.widget->nativeOwnerAlive());
    CHECK(!f.widget->threadState());
    refused([&] { f.resume(); });
    CHECK(f.runtime->threadState()->retainedNotifyDelta() == 0);
}

TEST_CASE(widget_event_component_binding_shares_thread_without_registering_or_rebinding)
{
    Fixture f;
    auto other = std::make_shared<MainViewState>();
    f.runtime->bindWidgetComponent(other->widgetState());
    CHECK(other->widgetState()->threadState() == f.widget->threadState());
    CHECK(!f.runtime->hasLiveRegistration(other.get()));
    auto foreign = std::make_shared<WidgetRuntimeState>(true, "foreign runtime");
    refused([&] { foreign->bindWidgetComponent(f.widget); });
    CHECK(f.widget->runtimeOwner() == f.runtime);
}

TEST_CASE(widget_event_normal_stack_destruction_releases_payload_without_posting)
{
    Fixture f; f.qualify();
    CHECK(f.resume().completed);
    f.event->destroyStackEvent();
    CHECK(!f.event->nativeAlive() && f.event->propertyName().empty());
    CHECK(!f.event->posted());
    refused([&] { f.event->destroyStackEvent(); });
    refused([&] { f.resume(); });
    CHECK(f.runtime->threadState()->retainedNotifyDelta() == 0);
}

TEST_CASE(widget_event_expired_runtime_cannot_be_replaced_on_same_native_component)
{
    Fixture f;
    f.runtime.reset();
    auto replacement = std::make_shared<WidgetRuntimeState>(true, "replacement runtime");
    refused([&] { replacement->bindWidgetComponent(f.widget); });
    CHECK(!f.widget->runtimeOwner());
}

TEST_CASE(widget_event_abandoned_pending_frame_does_not_fake_native_unwind)
{
    Fixture f;
    auto app = appObservation(); app.closing.reset();
    f.runtime->observeApplicationEvents(app, "partial application");
    {
        WidgetEventDispatch pending;
        CHECK(pending.resume(*f.runtime, f.widget, *f.event).stage == WidgetDispatchStage::Application);
    }
    CHECK(f.runtime->threadState()->retainedNotifyDelta() == 1);
    refused([&] { f.event->destroyStackEvent(); });
    WidgetEventDispatch replacement;
    refused([&] { replacement.resume(*f.runtime, f.widget, *f.event); });
}

TEST_CASE(widget_event_actual_installed_layout_overrides_earlier_null_observation)
{
    doe::SessionNotificationState notifications;
    auto owner = std::make_shared<MainViewState>();
    auto manager = owner->constructManagerChild(notifications);
    auto runtime = std::make_shared<WidgetRuntimeState>(true, "actual layout receiver fixture");
    owner->bindWidgetRuntime(runtime);
    runtime->observeApplicationEvents(appObservation(), "application");
    manager->widgetState()->observeEventReceiver(receiverObservation(), WidgetEventReceiverKind::QWidget, "before layout install");
    refused([&] { manager->ensureGridLayout(); });
    auto event = WidgetEventState::dynamicPropertyChange("sizeGripEnabled");
    WidgetEventDispatch dispatch;
    CHECK(dispatch.resume(*runtime, manager->widgetState(), *event).stage == WidgetDispatchStage::ReceiverLayout);
    CHECK(!dispatch.report().completed);
    CHECK(runtime->threadState()->retainedNotifyDelta() == 1);
    CHECK(manager->widgetState()->installedLayout() == manager->installedWidgetLayout());
}

TEST_CASE(widget_event_create_retains_depth_until_actual_gesture_input_is_known)
{
    Fixture f; f.qualify();
    f.event = WidgetEventState::create();
    CHECK(f.resume().stage == WidgetDispatchStage::ApplicationGestures);
    CHECK(f.runtime->threadState()->retainedNotifyDelta() == 1);
    CHECK(f.event->nativeAlive() && !f.event->posted());
    f.resume();
    CHECK(f.runtime->threadState()->retainedNotifyDelta() == 1);
    auto observation = appObservation(); observation.gestureManagerPresent = false;
    f.runtime->observeApplicationEvents(observation, "known null gesture manager");
    CHECK(f.resume().completed && f.dispatch.report().originalResult == false);
    CHECK(f.runtime->threadState()->retainedNotifyDelta() == 0);
    CHECK(f.event->nativeAlive());
    f.event->destroyStackEvent();
    CHECK(!f.event->nativeAlive());
}

namespace {
struct ChildFixture {
    doe::SessionNotificationState notifications;
    std::shared_ptr<MainViewState> owner = std::make_shared<MainViewState>();
    std::shared_ptr<ViewManagerState> manager = owner->constructManagerChild(notifications);
    std::shared_ptr<WidgetRuntimeState> runtime = std::make_shared<WidgetRuntimeState>(true, "actual child fixture");
    std::shared_ptr<GridLayoutState> grid;
    ChildFixture() {
        owner->bindWidgetRuntime(runtime);
        runtime->observeApplicationEvents(appObservation(), "same actual application");
        manager->widgetState()->observeEventReceiver(receiverObservation(), WidgetEventReceiverKind::QWidget, "actual receiver");
        refused([&] { manager->ensureGridLayout(); });
        grid = manager->installedWidgetLayout();
    }
};
}

TEST_CASE(widget_event_child_added_uses_actual_grid_and_returns_true_after_leaf)
{
    ChildFixture f;
    auto event = WidgetEventState::childAdded(f.grid);
    WidgetEventDispatch dispatch;
    CHECK(event->child() == f.grid);
    CHECK(event->rawType() == 0x44 && event->accepted() && !event->spontaneous());
    CHECK(dispatch.resume(*f.runtime, f.manager->widgetState(), *event).stage == WidgetDispatchStage::Receiver);
    CHECK(f.runtime->threadState()->retainedNotifyDelta() == 1);
    CHECK(event->nativeAlive());
    WidgetReceiverEventObservation observation;
    observation.childEventHandler = WidgetChildEventHandler::QObjectLeaf;
    f.manager->widgetState()->observeEventReceiver(observation, WidgetEventReceiverKind::QWidget, "source-bound childEvent leaf");
    CHECK(dispatch.resume(*f.runtime, f.manager->widgetState(), *event).completed);
    CHECK(dispatch.report().originalResult == true);
    CHECK(f.runtime->threadState()->retainedNotifyDelta() == 0);
    CHECK(event->nativeAlive() && event->child() == f.grid);
    event->destroyStackEvent();
    CHECK(!event->nativeAlive() && !event->child());
    CHECK(f.grid->nativeOwnerAlive());
    CHECK(f.grid->parentWidget() == f.manager->widgetState());
}

TEST_CASE(widget_event_child_added_rejects_foreign_child_and_receiver_owners)
{
    ChildFixture f;
    auto foreignChild = std::shared_ptr<GridLayoutState>(f.grid.get(), [](GridLayoutState*) {});
    refused([&] { WidgetEventState::childAdded(foreignChild); });
    auto event = WidgetEventState::childAdded(f.grid);
    auto foreignReceiver = std::shared_ptr<WidgetState>(f.manager->widgetState().get(), [](WidgetState*) {});
    WidgetEventDispatch dispatch;
    refused([&] { dispatch.resume(*f.runtime, foreignReceiver, *event); });
    refused([&] { dispatch.resume(*f.runtime, f.owner->widgetState(), *event); });
    CHECK(f.runtime->threadState()->retainedNotifyDelta() == 0);
    CHECK(event->nativeAlive());
}

TEST_CASE(widget_event_child_added_payload_does_not_retain_native_child_liveness)
{
    ChildFixture f;
    auto event = WidgetEventState::childAdded(f.grid);
    f.owner->releaseNativeOwner();
    CHECK(!f.grid->nativeOwnerAlive());
    WidgetEventDispatch dispatch;
    refused([&] { dispatch.resume(*f.runtime, f.manager->widgetState(), *event); });
    CHECK(f.runtime->threadState()->retainedNotifyDelta() == 0);
    event->destroyStackEvent();
    CHECK(!event->nativeAlive());
}

TEST_CASE(widget_event_component_binding_rejects_a_foreign_initial_control_block)
{
    auto owner = std::make_shared<MainViewState>();
    auto actual = owner->widgetState();
    auto foreign = std::shared_ptr<WidgetState>(actual.get(), [](WidgetState*) {});
    auto runtime = std::make_shared<WidgetRuntimeState>(true, "initial owner check");
    refused([&] { runtime->bindWidgetComponent(foreign); });
    CHECK(!actual->runtimeOwner());
    runtime->bindWidgetComponent(actual);
    CHECK(actual->runtimeOwner() == runtime);
}

int main() { return testkit::run_all(); }
