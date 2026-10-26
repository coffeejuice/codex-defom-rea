// ctest-labels: m2m-mdt
// Conditional private-prefix primitive; no public SplitView continuation.
#include <testkit/check.hpp>
#include <memory>
#include <string>
#include <vector>
#include "m2m/common.hpp"
#include "m2m/doe_session_notifications.hpp"
#include "m2m/engine/splitview_placeholder.hpp"
#include "m2m/engine/widget_events.hpp"
#include "m2m/engine/palette_native_ref.hpp"

using namespace m2m;
using namespace m2m::engine;
namespace {
using Stage = SplitViewPlaceholderStage;
using Issue = SplitViewPlaceholderIssue;
using Effect = SplitViewPlaceholderEffect;
constexpr auto qualification = "Isolated actual first-placeholder private call";

template<class F> void refused(F&& fn)
{
    bool caught = false;
    try { fn(); } catch (const Error& error) { caught = error.code == M2M_E_UNSUPPORTED; }
    CHECK(caught);
}

WidgetPrivateConstructionObservation paletteInputs(const std::string& receipt = qualification)
{
    WidgetPrivateConstructionObservation observation;
    observation.qualification = receipt;
    observation.paletteData = {{17}};
    observation.defaultPalette = WidgetPaletteValueObservation{17, 0};
    return observation;
}

WidgetApplicationPaletteSlotObservation slotInputs(const std::shared_ptr<WidgetRuntimeState>& runtime,
    std::optional<bool> present = {}, std::uint32_t word = 0x12345678u)
{
    WidgetApplicationPaletteSlotObservation o;
    o.qualification = runtime->qualification();
    o.moduleSha256 = "c360c971d292a3c84fc59b8e38733a85a111243f1dbc44737d87b6d744725887";
    o.slotRva = 0x9c9690; o.slotPresent = present;
    if (present == true) { o.paletteDataAlias = 17; o.groupResolveWord = word; }
    return o;
}

void registerPaletteData(const std::shared_ptr<WidgetRuntimeState>& runtime,
    const std::shared_ptr<WidgetState>& widget)
{
    auto o = paletteInputs(); o.defaultPalette.reset();
    runtime->observePrivateConstructionInputs(widget, o);
}

struct Fixture {
    std::shared_ptr<WidgetRuntimeState> runtime =
        std::make_shared<WidgetRuntimeState>(true, "Isolated placeholder runtime");
    std::unique_ptr<SplitViewPlaceholderCall> call =
        std::make_unique<SplitViewPlaceholderCall>(runtime, qualification);
    std::shared_ptr<WidgetState> start()
    {
        CHECK(call->resume().stage == Stage::PrivatePalette);
        CHECK(call->report().issue == Issue::CurrentPaletteUnavailable);
        REQUIRE(call->value());
        return call->value()->widgetState();
    }
};

// A separate actual startup-role owner, never an editor substituted for the
// placeholder. Values are explicit isolated inputs, not a recovered history.
struct CountFixture {
    doe::SessionNotificationState notifications;
    std::shared_ptr<MainViewState> main = std::make_shared<MainViewState>();
    std::shared_ptr<ViewManagerState> manager;
    std::shared_ptr<WidgetRuntimeState> runtime =
        std::make_shared<WidgetRuntimeState>(true, "Isolated known historical count");
    CountFixture()
    {
        manager = main->constructManagerChild(notifications);
        main->bindWidgetRuntime(runtime);
        WidgetRuntimeValueObservation values;
        values.mainView = main; values.manager = manager;
        values.qualification = "Isolated existing startup owners";
        values.fontData = {{1, 168, 0}};
        values.paletteData = {{17}};
        values.applicationFont.defaultValue = WidgetFontValueObservation{1, 0};
        values.applicationPalette.defaultValue = WidgetPaletteValueObservation{17, 0};
        values.mainViewValues.font = WidgetFontValueObservation{1, 0};
        runtime->observeStartupValues(values);
        WidgetConstructorTailObservation counts;
        counts.qualification = "Isolated historical current/max admission";
        counts.expectedFontAlias = 1;
        counts.currentWidgets = 174; counts.maximumWidgets = 174;
        runtime->observeConstructorTailValues(main->widgetState(), counts);
        runtime->observeQObjectCount(98, "Isolated separate QObject baseline");
    }
};
}

TEST_CASE(splitview_placeholder_does_not_start_from_construction_or_report_inspection)
{
    Fixture f;
    CHECK(f.call->report().stage == Stage::NotStarted);
    CHECK(f.call->report().issue == Issue::None);
    CHECK(!f.call->report().attempted);
    CHECK(!f.call->value()); CHECK(f.call->effects().empty());
    CHECK(!f.call->report().qwidgetReturned);
    CHECK(!f.call->report().completeForPublicSave);
    CHECK(!f.runtime->currentWidgetCount()); CHECK(!f.runtime->maximumWidgetCount());
    CHECK(!f.runtime->qobjectCount());
    CHECK(f.runtime->retainedQObjectConstructionDelta() == 0u);
}

TEST_CASE(splitview_placeholder_prefix_owns_one_pre_qobject_widget_and_stops_before_palette)
{
    Fixture f; const auto runtimeThread = f.runtime->threadState();
    const auto widget = f.start();
    CHECK(widget->nativeIdentity() == f.call->value());
    CHECK(f.call->value()->nativeOwnerAlive());
    CHECK(widget->runtimeOwner() == f.runtime);
    CHECK(!widget->threadState()); CHECK(f.runtime->threadState() == runtimeThread);
    CHECK(!widget->parentWidget()); CHECK(!widget->parentOwnsChild());
    REQUIRE(widget->objectName()); CHECK(*widget->objectName() == qt::String());
    CHECK(!widget->childPrefixUnrecovered()); CHECK(widget->knownChildSuffix().empty());
    REQUIRE(widget->pendingChildren()); CHECK(widget->pendingChildren()->empty());
    CHECK(widget->postedEventCount() == 0u);
    CHECK(widget->receivesChildEvents() == true);
    const auto& properties = widget->dynamicBooleanProperties();
    CHECK(properties.extraDataAllocated == false);
    REQUIRE(properties.names); REQUIRE(properties.values);
    CHECK(properties.names->empty()); CHECK(properties.values->empty());
    CHECK(widget->layoutPresence() == false); CHECK(!widget->installedLayout());
    CHECK(!widget->font()); CHECK(!widget->palette()); CHECK(!widget->locale());
    CHECK(!widget->opaque()); CHECK(!widget->opaqueChildrenDirty());
    CHECK(!widget->attributesLow()); CHECK(!widget->extendedAttributes());
    CHECK(!widget->windowFlags()); CHECK(!widget->geometryEndpoints());
    CHECK(!widget->widgetDataAssigned());
    CHECK(!widget->focusNextKnown()); CHECK(!widget->focusPreviousKnown());
    CHECK(!f.runtime->hasLiveRegistration(widget->nativeIdentity()));
    CHECK(!f.runtime->threadState()->postedEvents().populationKnown());
    CHECK(!f.runtime->currentWidgetCount()); CHECK(!f.runtime->maximumWidgetCount());
    CHECK(f.runtime->retainedQObjectConstructionDelta() == 0u);
    const std::vector<Effect> expected{Effect::PlaceholderStorageAllocated,
        Effect::WidgetPrivateAllocated, Effect::QObjectPrivateConstructed, Effect::WidgetPrivatePrefix};
    CHECK(f.call->effects() == expected);
}

TEST_CASE(splitview_placeholder_retry_keeps_actual_value_and_prefix_without_reinitialization)
{
    Fixture f; const auto widget = f.start(); const auto* value = f.call->value();
    const auto effects = f.call->effects();
    for (int retry = 0; retry != 3; ++retry) {
        CHECK(f.call->resume().issue == Issue::CurrentPaletteUnavailable);
        CHECK(f.call->value() == value); CHECK(f.call->value()->widgetState() == widget);
        CHECK(f.call->effects() == effects); CHECK(!widget->threadState());
        CHECK(widget->postedEventCount() == 0u);
    }
}

TEST_CASE(splitview_placeholder_correct_palette_observation_does_not_execute_palette_copy)
{
    Fixture f; const auto widget = f.start(); const auto effects = f.call->effects();
    f.runtime->observePrivateConstructionInputs(widget, paletteInputs());
    const auto* inputs = f.runtime->privateConstructionInputs(*widget);
    REQUIRE(inputs); REQUIRE(inputs->defaultPalette);
    CHECK(f.call->report().issue == Issue::CurrentPaletteUnavailable);
    CHECK(f.call->effects() == effects); CHECK(!widget->palette());
    CHECK(f.call->resume().issue == Issue::PaletteCopyUnrecovered);
    CHECK(f.call->report().stage == Stage::PrivatePalette);
    CHECK(!widget->palette()); CHECK(!widget->font()); CHECK(!widget->locale());
    CHECK(!widget->threadState()); CHECK(!widget->widgetDataAssigned());
    CHECK(f.call->effects() == effects);
    CHECK(f.call->resume().issue == Issue::PaletteCopyUnrecovered);
    CHECK(f.call->effects() == effects);
}

TEST_CASE(splitview_placeholder_does_not_consume_global_startup_defaults_or_stale_count)
{
    CountFixture f;
    SplitViewPlaceholderCall call(f.runtime, qualification);
    CHECK(call.resume().issue == Issue::CurrentPaletteUnavailable);
    REQUIRE(call.value()); const auto widget = call.value()->widgetState();
    CHECK(!f.runtime->privateConstructionInputs(*widget)); CHECK(!widget->palette());
    CHECK(f.runtime->currentWidgetCount() == 174u);
    CHECK(f.runtime->maximumWidgetCount() == 174u);
    CHECK(f.runtime->qobjectCount() == 98u);
    CHECK(f.runtime->retainedQObjectConstructionDelta() == 0u);
    const auto effects = call.effects();
    f.runtime->observePrivateConstructionInputs(widget, paletteInputs());
    CHECK(call.resume().issue == Issue::PaletteCopyUnrecovered);
    CHECK(f.runtime->currentWidgetCount() == 174u);
    CHECK(f.runtime->maximumWidgetCount() == 174u);
    CHECK(f.runtime->qobjectCount() == 98u);
    CHECK(!f.runtime->hasLiveRegistration(widget->nativeIdentity()));
    CHECK(call.effects() == effects);
}

TEST_CASE(splitview_placeholder_missing_runtime_or_qualification_has_no_native_prefix)
{
    auto runtime = std::make_shared<WidgetRuntimeState>(true, "Isolated runtime");
    SplitViewPlaceholderCall noRuntime({}, qualification);
    CHECK(noRuntime.resume().issue == Issue::RuntimeUnavailable);
    SplitViewPlaceholderCall noReceipt(runtime, "");
    CHECK(noReceipt.resume().issue == Issue::MissingQualification);
    auto unqualified = std::make_shared<WidgetRuntimeState>(true, "");
    SplitViewPlaceholderCall noRuntimeReceipt(unqualified, qualification);
    CHECK(noRuntimeReceipt.resume().issue == Issue::RuntimeQualificationUnavailable);
    for (const auto* call : {&noRuntime, &noReceipt, &noRuntimeReceipt}) {
        CHECK(!call->report().attempted); CHECK(call->report().stage == Stage::NotStarted);
        CHECK(!call->value()); CHECK(call->effects().empty());
    }
}

TEST_CASE(splitview_placeholder_rejects_equal_raw_runtime_with_foreign_control_block)
{
    auto runtime = std::make_shared<WidgetRuntimeState>(true, "Isolated canonical runtime");
    std::shared_ptr<WidgetRuntimeState> foreign(runtime.get(), [](WidgetRuntimeState*) {});
    SplitViewPlaceholderCall call(foreign, qualification);
    CHECK(call.resume().issue == Issue::RuntimeOwnerMismatch);
    CHECK(!call.report().attempted); CHECK(!call.value()); CHECK(call.effects().empty());
    CHECK(!runtime->currentWidgetCount());
    runtime.reset(); // The rejected foreign lease must never be dereferenced later.
    CHECK(call.resume().issue == Issue::RuntimeOwnerMismatch);
    CHECK(!call.value()); CHECK(call.effects().empty());
}

TEST_CASE(splitview_placeholder_runtime_loss_before_resume_creates_no_value)
{
    auto runtime = std::make_shared<WidgetRuntimeState>(true, "Isolated expiring runtime");
    SplitViewPlaceholderCall call(runtime, qualification); runtime.reset();
    CHECK(call.resume().issue == Issue::RuntimeUnavailable);
    CHECK(!call.report().attempted); CHECK(!call.value()); CHECK(call.effects().empty());
}

TEST_CASE(splitview_placeholder_runtime_loss_after_prefix_preserves_cursor_without_replay)
{
    Fixture f; const auto widget = f.start(); const auto* value = f.call->value();
    const auto effects = f.call->effects(); f.runtime.reset();
    CHECK(!widget->runtimeOwner());
    CHECK(f.call->resume().issue == Issue::RuntimeUnavailable);
    CHECK(f.call->report().stage == Stage::PrivatePalette); CHECK(f.call->report().attempted);
    CHECK(f.call->value() == value); CHECK(f.call->effects() == effects);
    CHECK(f.call->resume().issue == Issue::RuntimeUnavailable);
    CHECK(f.call->effects() == effects);
    f.call.reset(); CHECK(!widget->nativeOwnerAlive());
}

TEST_CASE(splitview_placeholder_foreign_receiver_observations_do_not_authorize_this_call)
{
    Fixture f, other; const auto widget = f.start(); const auto otherWidget = other.start();
    refused([&] { f.runtime->observePrivateConstructionInputs(otherWidget, paletteInputs()); });
    std::shared_ptr<WidgetState> foreign(widget.get(), [](WidgetState*) {});
    refused([&] { f.runtime->observePrivateConstructionInputs(foreign, paletteInputs()); });
    CHECK(!f.runtime->privateConstructionInputs(*widget));
    CHECK(f.call->resume().issue == Issue::CurrentPaletteUnavailable);
}

TEST_CASE(splitview_placeholder_another_same_runtime_receiver_cannot_supply_its_palette)
{
    Fixture f; const auto first = f.start();
    SplitViewPlaceholderCall second(f.runtime, qualification);
    CHECK(second.resume().issue == Issue::CurrentPaletteUnavailable);
    REQUIRE(second.value()); const auto otherWidget = second.value()->widgetState();
    CHECK(otherWidget != first); CHECK(otherWidget->nativeIdentity() != first->nativeIdentity());
    f.runtime->observePrivateConstructionInputs(otherWidget, paletteInputs());
    CHECK(f.call->resume().issue == Issue::CurrentPaletteUnavailable);
    CHECK(second.resume().issue == Issue::PaletteCopyUnrecovered);
    CHECK(!first->palette()); CHECK(!otherWidget->palette());
}

TEST_CASE(splitview_placeholder_abandonment_invalidates_only_its_own_diagnostic_widget)
{
    Fixture f; const auto first = f.start();
    SplitViewPlaceholderCall second(f.runtime, qualification);
    CHECK(second.resume().issue == Issue::CurrentPaletteUnavailable);
    REQUIRE(second.value()); const auto secondWidget = second.value()->widgetState();
    const auto effects = second.effects();
    f.call.reset();
    CHECK(!first->nativeOwnerAlive()); CHECK(secondWidget->nativeOwnerAlive());
    CHECK(first->runtimeOwner() == f.runtime); CHECK(!first->threadState());
    refused([&] { f.runtime->observePrivateConstructionInputs(first, paletteInputs()); });
    CHECK(second.resume().issue == Issue::CurrentPaletteUnavailable);
    CHECK(second.effects() == effects); CHECK(!second.report().qwidgetReturned);
    CHECK(!f.runtime->currentWidgetCount()); CHECK(!f.runtime->maximumWidgetCount());
    CHECK(f.runtime->retainedQObjectConstructionDelta() == 0u);
}

TEST_CASE(splitview_placeholder_does_not_restore_a_mutated_native_thread_on_retry)
{
    Fixture f; const auto widget = f.start(); const auto effects = f.call->effects();
    f.runtime->bindWidgetComponent(widget); // Host affinity operation, not this native prefix.
    REQUIRE(widget->threadState());
    CHECK(f.call->resume().issue == Issue::PrefixStateChanged);
    CHECK(widget->threadState() == f.runtime->threadState());
    CHECK(f.call->effects() == effects); CHECK(widget->postedEventCount() == 0u);
}

TEST_CASE(splitview_placeholder_private_input_qualification_is_not_replaceable)
{
    Fixture f; const auto widget = f.start(); const auto effects = f.call->effects();
    f.runtime->observePrivateConstructionInputs(widget, paletteInputs("A different native private call"));
    CHECK(f.call->resume().issue == Issue::PrivateInputsQualificationMismatch);
    CHECK(f.call->effects() == effects); CHECK(!widget->palette());
    refused([&] { f.runtime->observePrivateConstructionInputs(widget, paletteInputs()); });
    CHECK(f.call->resume().issue == Issue::PrivateInputsQualificationMismatch);
}

TEST_CASE(splitview_placeholder_unknown_getter_late_present_retains_one_same_owner_output)
{
    Fixture f; const auto widget = f.start(); registerPaletteData(f.runtime, widget);
    f.runtime->observeApplicationPaletteSlot(slotInputs(f.runtime));
    const auto owner = f.runtime->applicationPaletteOwner(); const auto prefix = f.call->effects();
    CHECK(f.call->report().stage == Stage::PrivatePalette); CHECK(!f.call->applicationPaletteRead());
    CHECK(f.call->resume().issue == Issue::ApplicationPaletteGetterPending);
    CHECK(f.call->report().stage == Stage::ApplicationPaletteGetter);
    CHECK(f.call->applicationPaletteOwner().lock() == owner);
    REQUIRE(f.call->applicationPaletteRead());
    CHECK(f.call->applicationPaletteRead()->issue == QApplicationPaletteReadIssue::SlotStateUnknown);
    CHECK(!f.call->applicationPaletteRead()->output); CHECK(f.call->effects() == prefix);
    f.runtime->observeApplicationPaletteSlot(slotInputs(f.runtime, true));
    CHECK(f.call->report().issue == Issue::ApplicationPaletteGetterPending);
    CHECK(f.call->applicationPaletteRead()->issue == QApplicationPaletteReadIssue::SlotStateUnknown);
    CHECK(f.call->resume().issue == Issue::PaletteRefUnavailable);
    CHECK(f.call->report().stage == Stage::PaletteTemporaryDeref);
    const auto* read = f.call->applicationPaletteRead(); REQUIRE(read); REQUIRE(read->output);
    CHECK(read->output->data == owner->wrapperOwner()->value.data);
    CHECK(read->output->groupResolveWord == 0x12345678u);
    REQUIRE(read->effects.size() == 1u); CHECK(read->effects[0] == QApplicationPaletteEffect::OutputCopied);
    const auto effects = f.call->effects(); REQUIRE(effects.size() == 6u);
    CHECK(effects[4] == Effect::ApplicationPaletteGetterReturned);
    CHECK(effects.back() == Effect::PrivatePaletteDataStored);
    for (int retry = 0; retry != 3; ++retry) {
        CHECK(f.call->resume().issue == Issue::PaletteRefUnavailable);
        CHECK(f.call->applicationPaletteRead() == read); CHECK(f.call->effects() == effects);
        CHECK(read->effects.size() == 1u);
    }
    CHECK(!widget->palette()); CHECK(!widget->font()); CHECK(!widget->locale());
    CHECK(!widget->threadState()); CHECK(!widget->widgetDataAssigned());
    CHECK(!f.runtime->currentWidgetCount()); CHECK(f.runtime->retainedQObjectConstructionDelta() == 0u);
}

TEST_CASE(splitview_placeholder_null_getter_never_initializes_from_an_existing_default)
{
    CountFixture f; auto null = slotInputs(f.runtime, false);
    f.runtime->observeApplicationPaletteSlot(null);
    SplitViewPlaceholderCall call(f.runtime, qualification);
    CHECK(call.resume().issue == Issue::ApplicationPaletteGetterPending);
    CHECK(call.report().stage == Stage::ApplicationPaletteGetter);
    REQUIRE(call.applicationPaletteRead());
    CHECK(call.applicationPaletteRead()->issue == QApplicationPaletteReadIssue::InitializerUnavailable);
    CHECK(!call.applicationPaletteRead()->output); CHECK(call.applicationPaletteRead()->effects.empty());
    CHECK(f.runtime->applicationPaletteOwner()->slotKnowledge() == QApplicationPaletteSlotKnowledge::KnownNull);
    CHECK(!f.runtime->applicationPaletteOwner()->wrapperOwner());
    const auto effects = call.effects();
    CHECK(call.resume().issue == Issue::ApplicationPaletteGetterPending); CHECK(call.effects() == effects);
    CHECK(f.runtime->currentWidgetCount() == 174u); CHECK(f.runtime->maximumWidgetCount() == 174u);
    CHECK(f.runtime->qobjectCount() == 98u);
}

TEST_CASE(splitview_placeholder_getter_needs_no_completed_private_palette_and_keeps_counters)
{
    CountFixture f; f.runtime->observeApplicationPaletteSlot(slotInputs(f.runtime, true, 0xfedcba98u));
    SplitViewPlaceholderCall call(f.runtime, qualification);
    CHECK(call.resume().issue == Issue::PaletteRefUnavailable);
    CHECK(call.report().stage == Stage::PaletteTemporaryDeref);
    REQUIRE(call.value()); const auto widget = call.value()->widgetState();
    CHECK(!f.runtime->privateConstructionInputs(*widget)); CHECK(!widget->palette());
    REQUIRE(call.applicationPaletteRead()); REQUIRE(call.applicationPaletteRead()->output);
    CHECK(call.applicationPaletteRead()->output->groupResolveWord == 0xfedcba98u);
    CHECK(f.runtime->applicationPaletteOwner()->wrapperOwner()->value.groupResolveWord == 0xfedcba98u);
    CHECK(!widget->font()); CHECK(!widget->locale()); CHECK(!widget->threadState());
    CHECK(!widget->widgetDataAssigned()); CHECK(!widget->extendedAttributes());
    CHECK(!f.runtime->hasLiveRegistration(widget->nativeIdentity()));
    CHECK(f.runtime->currentWidgetCount() == 174u); CHECK(f.runtime->maximumWidgetCount() == 174u);
    CHECK(f.runtime->qobjectCount() == 98u); CHECK(!call.report().qwidgetReturned);
}

TEST_CASE(splitview_placeholder_completed_getter_output_survives_runtime_loss_as_history)
{
    Fixture f; const auto widget = f.start(); registerPaletteData(f.runtime, widget);
    f.runtime->observeApplicationPaletteSlot(slotInputs(f.runtime, true));
    CHECK(f.call->resume().issue == Issue::PaletteRefUnavailable);
    const auto owner = f.runtime->applicationPaletteOwner();
    const auto* read = f.call->applicationPaletteRead(); REQUIRE(read); REQUIRE(read->output);
    const auto data = read->output->data; const auto effects = f.call->effects();
    f.runtime.reset(); CHECK(!widget->runtimeOwner());
    CHECK(f.call->resume().issue == Issue::RuntimeUnavailable);
    CHECK(f.call->report().stage == Stage::PaletteTemporaryDeref);
    CHECK(f.call->applicationPaletteRead() == read); CHECK(read->output->data == data);
    CHECK(f.call->effects() == effects); CHECK(!widget->palette());
    CHECK(owner->wrapperOwner()->value.data == data); // Diagnostic owner is no runtime grant.
    f.call.reset(); CHECK(!widget->nativeOwnerAlive());
}

TEST_CASE(splitview_placeholder_two_getter_frames_share_module_but_not_temporary_lifetimes)
{
    Fixture f; const auto firstWidget = f.start(); registerPaletteData(f.runtime, firstWidget);
    f.runtime->observeApplicationPaletteSlot(slotInputs(f.runtime, true));
    CHECK(f.call->resume().issue == Issue::PaletteRefUnavailable);
    SplitViewPlaceholderCall second(f.runtime, qualification);
    CHECK(second.resume().issue == Issue::PaletteRefUnavailable);
    REQUIRE(second.value()); const auto secondWidget = second.value()->widgetState();
    const auto* firstRead = f.call->applicationPaletteRead(); const auto* secondRead = second.applicationPaletteRead();
    REQUIRE(firstRead); REQUIRE(secondRead); REQUIRE(firstRead->output); REQUIRE(secondRead->output);
    CHECK(firstRead != secondRead); CHECK(firstWidget != secondWidget);
    CHECK(firstRead->output->data == secondRead->output->data);
    CHECK(f.call->applicationPaletteOwner().lock() == second.applicationPaletteOwner().lock());
    const auto effects = second.effects(); f.call.reset();
    CHECK(!firstWidget->nativeOwnerAlive()); CHECK(secondWidget->nativeOwnerAlive());
    CHECK(second.resume().issue == Issue::PaletteRefUnavailable);
    CHECK(second.applicationPaletteRead() == secondRead); CHECK(second.effects() == effects);
    CHECK(secondRead->effects.size() == 1u); CHECK(!secondWidget->palette());
}

TEST_CASE(splitview_placeholder_wrong_private_receipt_does_not_invoke_registered_getter)
{
    Fixture f; const auto widget = f.start();
    f.runtime->observePrivateConstructionInputs(widget, paletteInputs("Other constructor context"));
    f.runtime->observeApplicationPaletteSlot(slotInputs(f.runtime, true));
    const auto effects = f.call->effects();
    CHECK(f.call->resume().issue == Issue::PrivateInputsQualificationMismatch);
    CHECK(f.call->report().stage == Stage::PrivatePalette);
    CHECK(!f.call->applicationPaletteRead()); CHECK(f.call->applicationPaletteOwner().expired());
    CHECK(f.call->effects() == effects); CHECK(!widget->palette());
}

TEST_CASE(splitview_placeholder_pending_getter_loses_runtime_without_rebinding_owner)
{
    Fixture f; const auto widget = f.start(); registerPaletteData(f.runtime, widget);
    f.runtime->observeApplicationPaletteSlot(slotInputs(f.runtime));
    CHECK(f.call->resume().issue == Issue::ApplicationPaletteGetterPending);
    const auto* read = f.call->applicationPaletteRead(); REQUIRE(read);
    const auto effects = f.call->effects(); f.runtime.reset();
    CHECK(f.call->applicationPaletteOwner().expired());
    CHECK(f.call->resume().issue == Issue::RuntimeUnavailable);
    CHECK(f.call->report().stage == Stage::ApplicationPaletteGetter);
    CHECK(f.call->applicationPaletteRead() == read); CHECK(!read->output);
    CHECK(f.call->effects() == effects); CHECK(!widget->palette());
}

TEST_CASE(splitview_placeholder_partial_private_slot_keeps_word_unknown_and_getter_reference_held)
{
    Fixture f; const auto widget = f.start(); registerPaletteData(f.runtime, widget);
    CHECK(!widget->paletteConstructionSlot());
    f.runtime->observeApplicationPaletteSlot(slotInputs(f.runtime, true));
    CHECK(f.call->resume().issue == Issue::PaletteRefUnavailable);
    CHECK(f.call->report().stage == Stage::PaletteTemporaryDeref);
    REQUIRE(widget->paletteConstructionSlot()); REQUIRE(f.call->applicationPaletteRead());
    const auto* read = f.call->applicationPaletteRead(); REQUIRE(read->output);
    CHECK(widget->paletteConstructionSlot()->data == read->output->data);
    CHECK(!widget->paletteConstructionSlot()->groupResolveWord); CHECK(!widget->palette());
    const auto token = read->nativeReference(); REQUIRE(token);
    const auto refs = read->output->data->nativeReferenceState(); REQUIRE(refs);
    CHECK(token->stateOwner().lock() == refs); CHECK(token->report().stage == PaletteNativeValueStage::Held);
    CHECK(!token->releaseReceipt()); CHECK(!refs->currentBits());
    CHECK(refs->refMutations() == 1u); CHECK(refs->derefMutations() == 0u);
    const auto effects = f.call->effects(); auto copiedRead = *read;
    CHECK(copiedRead.nativeReference() == token);
    CHECK(f.call->resume().issue == Issue::PaletteRefUnavailable); CHECK(f.call->effects() == effects);
    CHECK(refs->mutationSequence() == 1u); CHECK(!widget->font()); CHECK(!widget->locale());
}

TEST_CASE(splitview_placeholder_known_primitive_snapshot_still_does_not_deref_or_initialize_word)
{
    Fixture f; const auto widget = f.start(); registerPaletteData(f.runtime, widget);
    f.runtime->observeApplicationPaletteSlot(slotInputs(f.runtime, true));
    CHECK(f.call->resume().issue == Issue::PaletteRefUnavailable);
    const auto token = f.call->applicationPaletteRead()->nativeReference(); REQUIRE(token);
    const auto refs = token->stateOwner().lock(); REQUIRE(refs);
    // Isolated primitive snapshot, not a new runtime/native producer admission.
    REQUIRE(refs->observeCurrent(3, refs->mutationSequence(), refs->qualification()));
    const auto effects = f.call->effects();
    CHECK(f.call->resume().issue == Issue::PaletteTemporaryDerefUnrecovered);
    CHECK(refs->currentBits() == 3u); CHECK(refs->mutationSequence() == 1u);
    CHECK(refs->derefMutations() == 0u); CHECK(token->report().stage == PaletteNativeValueStage::Held);
    CHECK(f.call->effects() == effects); CHECK(!widget->palette());
    REQUIRE(widget->paletteConstructionSlot()); CHECK(!widget->paletteConstructionSlot()->groupResolveWord);
}

TEST_CASE(splitview_placeholder_reference_origin_loss_preserves_committed_pointer_and_receipts)
{
    Fixture f; const auto widget = f.start(); registerPaletteData(f.runtime, widget);
    f.runtime->observeApplicationPaletteSlot(slotInputs(f.runtime, true));
    CHECK(f.call->resume().issue == Issue::PaletteRefUnavailable);
    const auto token = f.call->applicationPaletteRead()->nativeReference(); REQUIRE(token);
    const auto refs = token->stateOwner().lock(); REQUIRE(refs);
    const auto effects = f.call->effects(); const auto data = widget->paletteConstructionSlot()->data;
    refs->releaseNativeOwner();
    CHECK(f.call->resume().issue == Issue::PaletteNativeReferenceUnavailable);
    CHECK(widget->paletteConstructionSlot()->data == data); CHECK(!widget->paletteConstructionSlot()->groupResolveWord);
    CHECK(f.call->effects() == effects); CHECK(refs->derefMutations() == 0u);
    CHECK(token->report().stage == PaletteNativeValueStage::Held); CHECK(!widget->palette());
}

TEST_CASE(splitview_placeholder_abandonment_drops_metadata_without_native_reference_rollback)
{
    Fixture f; const auto widget = f.start(); registerPaletteData(f.runtime, widget);
    f.runtime->observeApplicationPaletteSlot(slotInputs(f.runtime, true));
    CHECK(f.call->resume().issue == Issue::PaletteRefUnavailable);
    const auto copiedRead = *f.call->applicationPaletteRead(); const auto token = copiedRead.nativeReference(); REQUIRE(token);
    const auto refs = token->stateOwner().lock(); REQUIRE(refs);
    f.call.reset();
    CHECK(!widget->nativeOwnerAlive()); CHECK(!widget->paletteConstructionSlot());
    CHECK(token->report().stage == PaletteNativeValueStage::Held); CHECK(!token->releaseReceipt());
    CHECK(refs->refMutations() == 1u); CHECK(refs->derefMutations() == 0u);
    CHECK(refs->nativeValueBalance() == 1u); CHECK(refs->mutationSequence() == 1u);
    CHECK(!f.runtime->currentWidgetCount()); CHECK(f.runtime->retainedQObjectConstructionDelta() == 0u);
}

int main() { return testkit::run_all(); }
