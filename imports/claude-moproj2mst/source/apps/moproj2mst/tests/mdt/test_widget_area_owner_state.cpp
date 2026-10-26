// ctest-labels: m2m-mdt
// Synthetic typed storage/write witnesses only; no native construction or Qt effects.
#include <testkit/check.hpp>
#include "m2m/engine/widget_area_owner_state.hpp"
#include "m2m/model.hpp"
#include <vector>

using namespace m2m;
using namespace m2m::engine;
namespace {
constexpr const char* q = "synthetic source-point observation; not original runtime admission";
template<class F> void refused(F&& f)
{
    bool caught = false;
    try { f(); } catch (const Error& e) { caught = e.code == M2M_E_UNSUPPORTED; }
    CHECK(caught);
}
struct Fixture {
    std::vector<std::shared_ptr<const void>> allocations;
    template<class T> std::shared_ptr<T> make()
    {
        auto allocation = std::make_shared<const int>(static_cast<int>(allocations.size()));
        allocations.push_back(allocation);
        return T::create(allocation, q);
    }
    std::shared_ptr<WidgetAreaWindowOwner> window;
    std::shared_ptr<WidgetAreaStackOwner> stack;
    std::shared_ptr<WidgetAreaScrollOwner> area;
    std::shared_ptr<WidgetAreaImplOwner> impl;
    std::shared_ptr<WidgetAreaChildOwner> old, incoming;
    Fixture()
    {
        window = make<WidgetAreaWindowOwner>(); stack = make<WidgetAreaStackOwner>();
        area = make<WidgetAreaScrollOwner>(); impl = make<WidgetAreaImplOwner>();
        old = make<WidgetAreaChildOwner>(); incoming = make<WidgetAreaChildOwner>();
    }
    void knownReplacement(bool mode)
    {
        impl->observeNativeAlive(true, q);
        old->observeNativeAlive(true, q); incoming->observeNativeAlive(true, q);
        old->observeKind(WidgetAreaChildKind::Placeholder, q);
        incoming->observeKind(WidgetAreaChildKind::BooleanEditor, q);
        impl->observeInitialCurrent18(old, q);
        impl->observeInitialFlags(mode, std::nullopt, q);
    }
};
}

TEST_CASE(widget_area_owner_state_all_fields_and_native_lifetimes_start_unknown)
{
    Fixture f;
    CHECK(!f.window->nativeAlive() && !f.stack->nativeAlive() && !f.area->nativeAlive());
    CHECK(!f.impl->nativeAlive() && !f.old->nativeAlive());
    CHECK(!f.window->area3c8().known && !f.window->stack480().known);
    CHECK(!f.area->impl28().known && !f.area->scrollContent().known);
    CHECK(!f.impl->area28().known && !f.impl->current18().known);
    CHECK(!f.impl->pending38().known && !f.impl->saved20().known);
    CHECK(!f.impl->deleteMode10() && !f.impl->placeholderEnabled11());
    CHECK(!f.stack->currentIndex());
    CHECK(f.old->kind() == WidgetAreaChildKind::Unknown);
    CHECK(!f.impl->usableNativeOwner());
    refused([&] { f.impl->requireAdoptionEffects(); });
}

TEST_CASE(widget_area_owner_state_window_stack_area_impl_and_content_are_distinct)
{
    Fixture f;
    f.window->observeInitialArea3c8(f.area, q);
    CHECK(!f.window->stack480().known && !f.area->impl28().known);
    f.window->observeInitialStack480(f.stack, q);
    f.area->observeInitialImpl28(f.impl, q);
    f.impl->observeInitialArea28(f.area, q);
    f.area->observeInitialScrollContent(f.old, q);
    CHECK(f.window->area3c8().target == f.area && f.window->stack480().target == f.stack);
    CHECK(f.area->impl28().target == f.impl && f.impl->area28().target == f.area);
    CHECK(f.area->scrollContent().target == f.old && !f.impl->current18().known);
    CHECK(!f.stack->currentIndex()); // linking an area does not select a page
    f.stack->observeInitialCurrentIndex(-1, q);
    CHECK(f.stack->currentIndex() == -1);
    refused([&] { f.stack->observeInitialCurrentIndex(1, q); });
    const auto otherArea = f.make<WidgetAreaScrollOwner>();
    refused([&] { f.window->observeInitialArea3c8(otherArea, q); });
}

TEST_CASE(widget_area_owner_state_delete_branch_preserves_pending_old_and_separate_scroll_content)
{
    Fixture f; f.knownReplacement(true);
    f.area->observeInitialScrollContent(f.old, q);
    f.impl->observePendingCopyAtDadcc3(f.old, q);
    CHECK(f.impl->current18().target == f.old && f.impl->pending38().target == f.old);
    f.impl->observeCurrentClearAtDadfec(f.old, q);
    CHECK(f.impl->current18().known && !f.impl->current18().nonNull);
    CHECK(f.impl->pending38().target == f.old && f.old->nativeAlive() == true);
    f.impl->observeCurrentStoreAtDae213(f.incoming, f.old, q);
    CHECK(f.impl->current18().target == f.incoming);
    CHECK(f.area->scrollContent().target == f.old); // no fabricated takeWidget/setWidget
    CHECK(!f.impl->saved20().known && !f.stack->currentIndex());
    refused([&] { f.impl->requireAdoptionEffects(); });
}

TEST_CASE(widget_area_owner_state_hide_only_branch_keeps_old_until_post_enter_site_store)
{
    Fixture f; f.knownReplacement(false);
    refused([&] { f.impl->observePendingCopyAtDadcc3(f.old, q); });
    refused([&] { f.impl->observeCurrentClearAtDadfec(f.old, q); });
    CHECK(f.impl->current18().target == f.old);
    CHECK(!f.impl->pending38().known);
    f.impl->observeCurrentStoreAtDae213(f.incoming, f.old, q);
    CHECK(f.impl->current18().target == f.incoming && !f.impl->pending38().known);
    CHECK(f.old->usableNativeOwner()); // hidden/current loss is not destruction
}

TEST_CASE(widget_area_owner_state_delete_branch_store_requires_actual_copy_and_clear_observations)
{
    Fixture f; f.knownReplacement(true);
    refused([&] { f.impl->observeCurrentStoreAtDae213(f.incoming, f.old, q); });
    refused([&] { f.impl->observeCurrentClearAtDadfec(f.old, q); });
    f.impl->observePendingCopyAtDadcc3(f.old, q);
    refused([&] { f.impl->observeCurrentStoreAtDae213(f.incoming, f.old, q); });
    CHECK(f.impl->current18().target == f.old);
    f.impl->observeCurrentClearAtDadfec(f.old, q);
    f.impl->observeCurrentStoreAtDae213(f.incoming, f.old, q);
    CHECK(f.impl->current18().target == f.incoming);
}

TEST_CASE(widget_area_owner_state_null_old_profile_cannot_hide_a_placeholder)
{
    Fixture f; f.knownReplacement(false);
    refused([&] { f.impl->observeCurrentStoreAtDae213(f.incoming, {}, q); });
    CHECK(f.impl->current18().target == f.old);
    Fixture empty;
    empty.impl->observeNativeAlive(true, q);
    empty.incoming->observeNativeAlive(true, q);
    empty.incoming->observeKind(WidgetAreaChildKind::BooleanEditor, q);
    empty.impl->observeInitialFlags(false, std::nullopt, q);
    refused([&] { empty.impl->observeCurrentStoreAtDae213(empty.incoming, {}, q); });
    empty.impl->observeInitialCurrent18({}, q);
    empty.impl->observeCurrentStoreAtDae213(empty.incoming, {}, q);
    CHECK(empty.impl->current18().target == empty.incoming);
    CHECK(!empty.impl->pending38().known && !empty.impl->placeholderEnabled11());
}

TEST_CASE(widget_area_owner_state_unknown_mode_kind_or_native_liveness_never_selects_a_branch)
{
    Fixture f;
    f.impl->observeInitialCurrent18(f.old, q);
    refused([&] { f.impl->observePendingCopyAtDadcc3(f.old, q); });
    f.old->observeNativeAlive(true, q);
    f.impl->observeNativeAlive(true, q);
    f.impl->observeInitialFlags(true, std::nullopt, q);
    refused([&] { f.impl->observePendingCopyAtDadcc3(f.old, q); }); // old kind unavailable
    CHECK(!f.impl->pending38().known);
    f.old->observeKind(WidgetAreaChildKind::Placeholder, q);
    f.impl->observePendingCopyAtDadcc3(f.old, q);
    CHECK(f.impl->pending38().target == f.old);
    refused([&] { f.old->observeKind(WidgetAreaChildKind::BooleanEditor, q); });
    refused([&] { f.impl->observeInitialFlags(false, true, q); });
    CHECK(!f.impl->placeholderEnabled11()); // failed observation must not partially merge flags
}

TEST_CASE(widget_area_owner_state_canonical_aliases_and_old_current_identity_are_required)
{
    Fixture f;
    const std::shared_ptr<WidgetAreaChildOwner> forged(f.old.get(), [](WidgetAreaChildOwner*) {});
    refused([&] { f.impl->observeInitialCurrent18(forged, q); });
    CHECK(!f.impl->current18().known);
    f.knownReplacement(true);
    refused([&] { f.impl->observePendingCopyAtDadcc3(forged, q); });
    const auto unrelated = f.make<WidgetAreaChildOwner>();
    unrelated->observeNativeAlive(true, q); unrelated->observeKind(WidgetAreaChildKind::Placeholder, q);
    refused([&] { f.impl->observePendingCopyAtDadcc3(unrelated, q); });
    CHECK(!f.impl->pending38().known);
    refused([&] { f.impl->observeCurrentStoreAtDae213(f.old, f.old, q); }); // same-current path skips write
}

TEST_CASE(widget_area_owner_state_unknown_mode_and_impl_liveness_are_independent_guards)
{
    Fixture mode;
    mode.impl->observeNativeAlive(true, q); mode.old->observeNativeAlive(true, q);
    mode.old->observeKind(WidgetAreaChildKind::Placeholder, q);
    mode.impl->observeInitialCurrent18(mode.old, q);
    refused([&] { mode.impl->observePendingCopyAtDadcc3(mode.old, q); });
    CHECK(!mode.impl->pending38().known);
    mode.impl->observeInitialFlags(true, std::nullopt, q);
    mode.impl->observePendingCopyAtDadcc3(mode.old, q);

    Fixture lifetime;
    lifetime.old->observeNativeAlive(true, q);
    lifetime.old->observeKind(WidgetAreaChildKind::Placeholder, q);
    lifetime.impl->observeInitialCurrent18(lifetime.old, q);
    lifetime.impl->observeInitialFlags(true, std::nullopt, q);
    refused([&] { lifetime.impl->observePendingCopyAtDadcc3(lifetime.old, q); });
    CHECK(!lifetime.impl->pending38().known);
    lifetime.impl->observeNativeAlive(true, q);
    lifetime.impl->observePendingCopyAtDadcc3(lifetime.old, q);
}

TEST_CASE(widget_area_owner_state_dead_or_revoked_native_owner_is_not_resurrected_or_auto_cleared)
{
    Fixture f; f.knownReplacement(true);
    f.old->observeNativeAlive(false, q);
    refused([&] { f.old->observeNativeAlive(true, q); });
    CHECK(f.impl->current18().known && f.impl->current18().nonNull);
    CHECK(!f.impl->current18().usableNativeTarget());
    refused([&] { f.impl->observePendingCopyAtDadcc3(f.old, q); });
    f.impl->releaseAdmissionScope();
    refused([&] { f.impl->observeInitialSaved20({}, q); });
    CHECK(!f.impl->saved20().known);
}

TEST_CASE(widget_area_owner_state_owner_expiry_remains_distinct_from_known_null)
{
    Fixture f; f.knownReplacement(false);
    f.allocations.clear(); // allocation admission gone; diagnostic node leases remain
    CHECK(!f.old->observationScopeAvailable() && !f.old->usableNativeOwner());
    CHECK(f.impl->current18().known && f.impl->current18().nonNull);
    CHECK(f.impl->current18().target == f.old && !f.impl->current18().usableNativeTarget());
    refused([&] { f.impl->observeCurrentStoreAtDae213(f.incoming, f.old, q); });

    Fixture missing;
    missing.impl->observeInitialCurrent18(missing.old, q);
    missing.old.reset();
    const auto unavailable = missing.impl->current18();
    CHECK(unavailable.known && unavailable.nonNull && !unavailable.target);
    CHECK(!unavailable.usableNativeTarget());
}

TEST_CASE(widget_area_owner_state_pending_is_one_guard_and_saved_remains_independent)
{
    Fixture f; f.knownReplacement(true);
    const auto earlier = f.make<WidgetAreaChildOwner>();
    earlier->observeKind(WidgetAreaChildKind::Other, q); earlier->observeNativeAlive(true, q);
    f.impl->observeInitialSaved20(earlier, q);
    f.impl->observeInitialPending38(earlier, q);
    f.impl->observePendingCopyAtDadcc3(f.old, q);
    CHECK(f.impl->pending38().target == f.old && f.impl->saved20().target == earlier);
    CHECK(earlier->usableNativeOwner()); // overwriting a guard never deletes its old target
    refused([&] { f.impl->requireAdoptionEffects(); });
}

int main() { return testkit::run_all(); }
