// ctest-labels: m2m-mdt
// Registry observation only; no QApplication getter or placeholder copy runs.
#include <testkit/check.hpp>
#include <memory>
#include <string>
#include <vector>
#include "m2m/common.hpp"
#include "m2m/doe_session_notifications.hpp"
#include "m2m/engine/application_palette_owner.hpp"
#include "m2m/engine/view_manager.hpp"
#include "m2m/engine/palette_native_ref.hpp"

using namespace m2m;
using namespace m2m::engine;
namespace {
using Knowledge = QApplicationPaletteSlotKnowledge;
constexpr auto moduleHash = "c360c971d292a3c84fc59b8e38733a85a111243f1dbc44737d87b6d744725887";
template<class F> void refused(F&& fn)
{
    bool caught = false;
    try { fn(); } catch (const Error& error) { caught = error.code == M2M_E_UNSUPPORTED; }
    CHECK(caught);
}
struct Fixture {
    doe::SessionNotificationState notifications;
    std::shared_ptr<MainViewState> main = std::make_shared<MainViewState>();
    std::shared_ptr<ViewManagerState> manager;
    std::shared_ptr<WidgetRuntimeState> runtime =
        std::make_shared<WidgetRuntimeState>(true, "Isolated current application palette runtime");
    explicit Fixture(bool withDefault = true)
    {
        manager = main->constructManagerChild(notifications);
        main->bindWidgetRuntime(runtime);
        WidgetRuntimeValueObservation values;
        values.mainView = main; values.manager = manager;
        values.qualification = "Isolated existing palette graph";
        values.paletteData = {{17}, {18}};
        values.mainViewValues.palette = WidgetPaletteValueObservation{17, 0};
        if (withDefault) values.applicationPalette.defaultValue = WidgetPaletteValueObservation{17, 0x12345678u};
        values.applicationPalette.classCachePresent = true;
        values.applicationPalette.classes = std::vector<WidgetPaletteClassObservation>{{"QMenu", {18, 7}}};
        runtime->observeStartupValues(values);
    }
    WidgetApplicationPaletteSlotObservation observation() const
    {
        WidgetApplicationPaletteSlotObservation o;
        o.qualification = runtime->qualification(); o.moduleSha256 = moduleHash; o.slotRva = 0x9c9690;
        return o;
    }
    WidgetApplicationPaletteSlotObservation present(std::uint32_t alias = 17, std::uint32_t word = 0x12345678u) const
    {
        auto o = observation(); o.slotPresent = true; o.paletteDataAlias = alias; o.groupResolveWord = word;
        return o;
    }
};
}

TEST_CASE(widget_app_pal_default_and_class_cache_do_not_infer_a_slot_owner)
{
    Fixture f; REQUIRE(f.runtime->applicationPalette());
    CHECK(!f.runtime->applicationPaletteOwner());
    CHECK(!f.runtime->privateConstructionInputs(*f.main->widgetState()));
    f.runtime->observeApplicationPaletteSlot(f.observation());
    const auto owner = f.runtime->applicationPaletteOwner(); REQUIRE(owner);
    CHECK(owner->slotKnowledge() == Knowledge::Unknown); CHECK(!owner->wrapperOwner());
    CHECK(f.runtime->applicationPalette()->groupResolveWord == 0x12345678u);
    CHECK(!f.runtime->privateConstructionInputs(*f.main->widgetState()));
    CHECK(!f.runtime->currentWidgetCount()); CHECK(!f.runtime->maximumWidgetCount());
    CHECK(f.runtime->retainedQObjectConstructionDelta() == 0u);
}

TEST_CASE(widget_app_pal_null_is_distinct_from_unknown_without_lazy_initialization)
{
    Fixture f; f.runtime->observeApplicationPaletteSlot(f.observation());
    const auto owner = f.runtime->applicationPaletteOwner();
    auto o = f.observation(); o.slotPresent = false;
    f.runtime->observeApplicationPaletteSlot(o);
    CHECK(f.runtime->applicationPaletteOwner() == owner);
    CHECK(owner->slotKnowledge() == Knowledge::KnownNull); CHECK(!owner->wrapperOwner());
    f.runtime->observeApplicationPaletteSlot(o);
    f.runtime->observeApplicationPaletteSlot(f.observation());
    CHECK(owner->slotKnowledge() == Knowledge::KnownNull); CHECK(!owner->wrapperOwner());
    refused([&] { f.runtime->observeApplicationPaletteSlot(f.present()); });
    CHECK(owner->slotKnowledge() == Knowledge::KnownNull); CHECK(!owner->wrapperOwner());
}

TEST_CASE(widget_app_pal_present_binds_one_wrapper_to_same_runtime_shared_data)
{
    Fixture f; const auto data = f.runtime->applicationPalette()->data;
    f.runtime->observeApplicationPaletteSlot(f.present());
    const auto owner = f.runtime->applicationPaletteOwner(); REQUIRE(owner);
    const auto wrapper = owner->wrapperOwner(); REQUIRE(wrapper);
    CHECK(owner->slotKnowledge() == Knowledge::KnownPresent);
    CHECK(wrapper->value.data == data);
    CHECK(!wrapper->value.data.owner_before(data)); CHECK(!data.owner_before(wrapper->value.data));
    CHECK(wrapper->value.groupResolveWord == 0x12345678u);
    CHECK(owner->qualification() == f.runtime->qualification() + "/QtGui4_64.dll/app_pal@9c9690");
    f.runtime->observeApplicationPaletteSlot(f.present());
    f.runtime->observeApplicationPaletteSlot(f.observation());
    CHECK(f.runtime->applicationPaletteOwner() == owner); CHECK(owner->wrapperOwner() == wrapper);
    CHECK(f.main->widgetState()->palette()->groupResolveWord == 0u);
}

TEST_CASE(widget_app_pal_module_and_qualification_preflight_does_not_publish_on_failure)
{
    Fixture f;
    for (int changed = 0; changed != 6; ++changed) {
        auto o = f.present();
        if (changed == 0) o.qualification.clear();
        if (changed == 1) o.qualification = "Other runtime receipt";
        if (changed == 2) o.moduleSha256 = "Other module";
        if (changed == 3) o.slotRva.reset();
        if (changed == 4) o.slotRva = 0;
        if (changed == 5) o.slotRva = 0x9c9698;
        refused([&] { f.runtime->observeApplicationPaletteSlot(o); });
        CHECK(!f.runtime->applicationPaletteOwner());
    }
}

TEST_CASE(widget_app_pal_present_requires_explicit_registered_alias_and_word)
{
    Fixture f;
    for (int changed = 0; changed != 4; ++changed) {
        auto o = f.present();
        if (changed == 0) o.paletteDataAlias.reset();
        if (changed == 1) o.paletteDataAlias = 0;
        if (changed == 2) o.paletteDataAlias = 99;
        if (changed == 3) o.groupResolveWord.reset();
        refused([&] { f.runtime->observeApplicationPaletteSlot(o); });
        CHECK(!f.runtime->applicationPaletteOwner());
    }
    f.runtime->observeApplicationPaletteSlot(f.present(17, 0));
    CHECK(f.runtime->applicationPaletteOwner()->wrapperOwner()->value.groupResolveWord == 0u);
    CHECK(f.runtime->applicationPalette()->groupResolveWord == 0x12345678u);
}

TEST_CASE(widget_app_pal_unknown_and_null_reject_present_payloads_without_partial_owner)
{
    Fixture f;
    for (int changed = 0; changed != 4; ++changed) {
        auto o = f.observation();
        if (changed >= 2) o.slotPresent = false;
        if (changed % 2 == 0) o.paletteDataAlias = 17;
        else o.groupResolveWord = 0;
        refused([&] { f.runtime->observeApplicationPaletteSlot(o); });
        CHECK(!f.runtime->applicationPaletteOwner());
    }
}

TEST_CASE(widget_app_pal_present_conflicts_preserve_exact_owner_and_wrapper)
{
    Fixture f; f.runtime->observeApplicationPaletteSlot(f.present());
    const auto owner = f.runtime->applicationPaletteOwner(); const auto wrapper = owner->wrapperOwner();
    auto null = f.observation(); null.slotPresent = false;
    refused([&] { f.runtime->observeApplicationPaletteSlot(null); });
    refused([&] { f.runtime->observeApplicationPaletteSlot(f.present(17, 7)); });
    refused([&] { f.runtime->observeApplicationPaletteSlot(f.present(18)); });
    CHECK(f.runtime->applicationPaletteOwner() == owner); CHECK(owner->wrapperOwner() == wrapper);
    CHECK(owner->slotKnowledge() == Knowledge::KnownPresent);
    CHECK(wrapper->value.groupResolveWord == 0x12345678u);
}

TEST_CASE(widget_app_pal_rejects_default_owner_substitution_but_does_not_create_a_default)
{
    Fixture f; refused([&] { f.runtime->observeApplicationPaletteSlot(f.present(18)); });
    CHECK(!f.runtime->applicationPaletteOwner());
    Fixture noDefault(false); CHECK(!noDefault.runtime->applicationPalette());
    noDefault.runtime->observeApplicationPaletteSlot(noDefault.present(18, 7));
    CHECK(noDefault.runtime->applicationPaletteOwner()->wrapperOwner()->value.data->observationAlias() == 18u);
    CHECK(!noDefault.runtime->applicationPalette());
}

TEST_CASE(widget_app_pal_equal_local_aliases_in_two_runtimes_remain_distinct)
{
    Fixture first, second;
    first.runtime->observeApplicationPaletteSlot(first.present());
    second.runtime->observeApplicationPaletteSlot(second.present());
    const auto a = first.runtime->applicationPaletteOwner(); const auto b = second.runtime->applicationPaletteOwner();
    CHECK(a != b); CHECK(a->wrapperOwner() != b->wrapperOwner());
    CHECK(a->wrapperOwner()->value.data != b->wrapperOwner()->value.data);
    CHECK(a->wrapperOwner()->value.data->observationAlias() == b->wrapperOwner()->value.data->observationAlias());
}

TEST_CASE(widget_app_pal_needs_existing_shared_registry_and_shared_runtime_owner)
{
    auto runtime = std::make_shared<WidgetRuntimeState>(true, "Empty registry runtime");
    WidgetApplicationPaletteSlotObservation o;
    o.qualification = runtime->qualification(); o.moduleSha256 = moduleHash; o.slotRva = 0x9c9690;
    refused([&] { runtime->observeApplicationPaletteSlot(o); });
    CHECK(!runtime->applicationPaletteOwner());
    WidgetRuntimeState unshared(true, "Unshared runtime"); o.qualification = unshared.qualification();
    refused([&] { unshared.observeApplicationPaletteSlot(o); });
    CHECK(!unshared.applicationPaletteOwner());
}

TEST_CASE(widget_app_pal_diagnostic_owner_does_not_hold_runtime_or_claim_a_native_read)
{
    std::shared_ptr<const QApplicationPaletteOwnerState> diagnostic;
    std::weak_ptr<WidgetRuntimeState> weakRuntime;
    {
        Fixture f; weakRuntime = f.runtime;
        f.runtime->observeApplicationPaletteSlot(f.present()); diagnostic = f.runtime->applicationPaletteOwner();
        CHECK(!f.runtime->privateConstructionInputs(*f.main->widgetState()));
        CHECK(!f.runtime->currentWidgetCount()); CHECK(!f.runtime->maximumWidgetCount());
        CHECK(f.runtime->retainedQObjectConstructionDelta() == 0u);
    }
    CHECK(weakRuntime.expired()); REQUIRE(diagnostic);
    // Const historical diagnostics have no mutable read/provider operation.
    CHECK(diagnostic->slotKnowledge() == Knowledge::KnownPresent);
    CHECK(diagnostic->wrapperOwner()->value.groupResolveWord == 0x12345678u);
}

TEST_CASE(widget_app_pal_bound_read_preserves_unknown_and_null_without_provider)
{
    Fixture f; f.runtime->observeApplicationPaletteSlot(f.observation());
    const auto owner = f.runtime->applicationPaletteOwner();
    auto read = f.runtime->readApplicationPaletteFor(f.main->widgetState(), owner);
    CHECK(read.issue == QApplicationPaletteReadIssue::SlotStateUnknown);
    CHECK(!read.output); CHECK(read.effects.empty()); CHECK(!owner->wrapperOwner());
    CHECK(!read.nativeReference()); CHECK(!f.runtime->applicationPalette()->data->nativeReferenceState());
    auto null = f.observation(); null.slotPresent = false;
    f.runtime->observeApplicationPaletteSlot(null);
    read = f.runtime->readApplicationPaletteFor(f.main->widgetState(), owner);
    CHECK(read.issue == QApplicationPaletteReadIssue::InitializerUnavailable);
    CHECK(!read.output); CHECK(read.effects.empty());
    CHECK(owner->slotKnowledge() == Knowledge::KnownNull); CHECK(!owner->wrapperOwner());
    CHECK(!read.nativeReference()); CHECK(!f.runtime->applicationPalette()->data->nativeReferenceState());
}

TEST_CASE(widget_app_pal_bound_present_read_has_one_copy_and_no_private_input_dependency)
{
    Fixture f; f.runtime->observeApplicationPaletteSlot(f.present());
    const auto owner = f.runtime->applicationPaletteOwner(); const auto wrapper = owner->wrapperOwner();
    CHECK(!f.runtime->privateConstructionInputs(*f.main->widgetState()));
    const auto read = f.runtime->readApplicationPaletteFor(f.main->widgetState(), owner);
    CHECK(read.issue == QApplicationPaletteReadIssue::None); REQUIRE(read.output);
    CHECK(read.output->data == wrapper->value.data);
    CHECK(!read.output->data.owner_before(wrapper->value.data));
    CHECK(!wrapper->value.data.owner_before(read.output->data));
    CHECK(read.output->groupResolveWord == 0x12345678u);
    REQUIRE(read.effects.size() == 1u); CHECK(read.effects[0] == QApplicationPaletteEffect::OutputCopied);
    CHECK(owner->wrapperOwner() == wrapper);
    CHECK(!f.runtime->privateConstructionInputs(*f.main->widgetState()));
    CHECK(f.main->widgetState()->palette()->groupResolveWord == 0u);
    CHECK(!f.runtime->currentWidgetCount()); CHECK(f.runtime->retainedQObjectConstructionDelta() == 0u);
}

TEST_CASE(widget_app_pal_bound_read_rejects_missing_foreign_copied_and_rewrapped_owners)
{
    Fixture f, other; f.runtime->observeApplicationPaletteSlot(f.present());
    other.runtime->observeApplicationPaletteSlot(other.present());
    const auto owner = f.runtime->applicationPaletteOwner(); const auto wrapper = owner->wrapperOwner();
    auto copy = std::make_shared<QApplicationPaletteOwnerState>(*owner);
    std::shared_ptr<const QApplicationPaletteOwnerState> rewrapped(owner.get(), [](const QApplicationPaletteOwnerState*) {});
    refused([&] { f.runtime->readApplicationPaletteFor(f.main->widgetState(), {}); });
    refused([&] { f.runtime->readApplicationPaletteFor(f.main->widgetState(), other.runtime->applicationPaletteOwner()); });
    refused([&] { f.runtime->readApplicationPaletteFor(f.main->widgetState(), copy); });
    refused([&] { f.runtime->readApplicationPaletteFor(f.main->widgetState(), rewrapped); });
    Fixture absent;
    refused([&] { absent.runtime->readApplicationPaletteFor(absent.main->widgetState(), owner); });
    CHECK(!absent.runtime->applicationPaletteOwner()); CHECK(owner->wrapperOwner() == wrapper);
}

TEST_CASE(widget_app_pal_bound_read_requires_same_live_receiver_control_block_and_runtime)
{
    Fixture f, other; f.runtime->observeApplicationPaletteSlot(f.present());
    const auto owner = f.runtime->applicationPaletteOwner(); const auto widget = f.main->widgetState();
    std::shared_ptr<WidgetState> rewrapped(widget.get(), [](WidgetState*) {});
    refused([&] { f.runtime->readApplicationPaletteFor({}, owner); });
    refused([&] { f.runtime->readApplicationPaletteFor(rewrapped, owner); });
    refused([&] { f.runtime->readApplicationPaletteFor(other.main->widgetState(), owner); });
    refused([&] { other.runtime->readApplicationPaletteFor(widget, owner); });
    f.main->releaseNativeOwner(); CHECK(!widget->nativeOwnerAlive());
    refused([&] { f.runtime->readApplicationPaletteFor(widget, owner); });
    CHECK(owner->slotKnowledge() == Knowledge::KnownPresent);
}

TEST_CASE(widget_app_pal_getter_tokens_share_one_data_state_without_host_copy_retain)
{
    Fixture f; f.runtime->observeApplicationPaletteSlot(f.present());
    const auto owner = f.runtime->applicationPaletteOwner(); const auto data = owner->wrapperOwner()->value.data;
    CHECK(!data->nativeReferenceState());
    const auto first = f.runtime->readApplicationPaletteFor(f.main->widgetState(), owner);
    const auto token = first.nativeReference(); REQUIRE(token);
    const auto refs = data->nativeReferenceState(); REQUIRE(refs);
    CHECK(token->stateOwner().lock() == refs); CHECK(token->kind() == PaletteNativeValueKind::GetterTemporary);
    CHECK(token->report().stage == PaletteNativeValueStage::Held); REQUIRE(token->retainReceipt());
    CHECK(token->retainReceipt()->sequence == 1u); CHECK(!token->retainReceipt()->afterBits);
    CHECK(!refs->currentBits()); CHECK(refs->refMutations() == 1u); CHECK(refs->mutationSequence() == 1u);
    CHECK(refs->derefMutations() == 0u); CHECK(refs->nativeValueBalance() == 1u);
    auto diagnosticCopy = first;
    CHECK(diagnosticCopy.nativeReference() == token); CHECK(refs->mutationSequence() == 1u);
    const auto second = f.runtime->readApplicationPaletteFor(f.manager->widgetState(), owner);
    REQUIRE(second.nativeReference()); CHECK(second.nativeReference() != token);
    CHECK(second.nativeReference()->stateOwner().lock() == refs);
    CHECK(data->nativeReferenceState() == refs); CHECK(refs->refMutations() == 2u);
    CHECK(refs->mutationSequence() == 2u); CHECK(refs->nativeValueBalance() == 2u);
    CHECK(!refs->currentBits()); CHECK(!f.runtime->currentWidgetCount());
}

TEST_CASE(widget_app_pal_ref_states_with_equal_aliases_in_different_runtimes_are_distinct)
{
    Fixture first, second;
    first.runtime->observeApplicationPaletteSlot(first.present());
    second.runtime->observeApplicationPaletteSlot(second.present());
    const auto a = first.runtime->readApplicationPaletteFor(first.main->widgetState(), first.runtime->applicationPaletteOwner());
    const auto b = second.runtime->readApplicationPaletteFor(second.main->widgetState(), second.runtime->applicationPaletteOwner());
    REQUIRE(a.nativeReference()); REQUIRE(b.nativeReference());
    const auto stateA = a.nativeReference()->stateOwner().lock(); const auto stateB = b.nativeReference()->stateOwner().lock();
    REQUIRE(stateA); REQUIRE(stateB); CHECK(stateA != stateB);
    CHECK(a.output->data != b.output->data);
    CHECK(a.output->data->observationAlias() == b.output->data->observationAlias());
    CHECK(stateA->mutationSequence() == 1u); CHECK(stateB->mutationSequence() == 1u);
}

TEST_CASE(widget_app_pal_runtime_loss_invalidates_reference_callability_without_deref)
{
    std::shared_ptr<const PaletteNativeValueRef> token;
    std::shared_ptr<const PaletteNativeRefState> refs;
    std::weak_ptr<WidgetRuntimeState> weakRuntime;
    {
        Fixture f; weakRuntime = f.runtime; f.runtime->observeApplicationPaletteSlot(f.present());
        const auto result = f.runtime->readApplicationPaletteFor(f.main->widgetState(), f.runtime->applicationPaletteOwner());
        token = result.nativeReference(); REQUIRE(token); refs = result.output->data->nativeReferenceState(); REQUIRE(refs);
        CHECK(refs->nativeOwnerAlive()); CHECK(refs->mutationSequence() == 1u);
    }
    CHECK(weakRuntime.expired()); CHECK(!refs->nativeOwnerAlive());
    CHECK(refs->refMutations() == 1u); CHECK(refs->derefMutations() == 0u);
    CHECK(refs->nativeValueBalance() == 1u); CHECK(!refs->currentBits());
    CHECK(token->report().stage == PaletteNativeValueStage::Held); CHECK(!token->releaseReceipt());
}

TEST_CASE(widget_app_pal_withdrawn_data_ref_origin_cannot_be_replaced_or_retained_again)
{
    Fixture f; f.runtime->observeApplicationPaletteSlot(f.present());
    const auto owner = f.runtime->applicationPaletteOwner();
    const auto result = f.runtime->readApplicationPaletteFor(f.main->widgetState(), owner);
    REQUIRE(result.nativeReference()); const auto refs = result.nativeReference()->stateOwner().lock(); REQUIRE(refs);
    refs->releaseNativeOwner(); // Explicit isolated withdrawal, not native QPalette destruction.
    refused([&] { f.runtime->readApplicationPaletteFor(f.main->widgetState(), owner); });
    CHECK(owner->wrapperOwner()->value.data->nativeReferenceState() == refs);
    CHECK(refs->refMutations() == 1u); CHECK(refs->derefMutations() == 0u);
    CHECK(refs->mutationSequence() == 1u); CHECK(!refs->currentBits());
}

int main() { return testkit::run_all(); }
