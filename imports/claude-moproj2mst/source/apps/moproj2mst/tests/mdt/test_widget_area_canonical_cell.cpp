// ctest-labels: m2m-mdt
// Declared source observations only; no native constructor/Qt readiness claim.
#include <testkit/check.hpp>
#include "m2m/engine/widget_area_lazy.hpp"
#include "m2m/model.hpp"

using namespace m2m;
using namespace m2m::engine;
namespace {
constexpr const char* q = "synthetic source occurrence, not native readiness";
template<class F> void refused(F&& action)
{
    bool caught = false;
    try { action(); } catch (const Error& e) { caught = e.code == M2M_E_UNSUPPORTED; }
    CHECK(caught);
}
struct Fixture {
    std::shared_ptr<const void> allocation = std::make_shared<const int>(1);
    std::shared_ptr<const void> areaAllocation = std::make_shared<const int>(2);
    std::shared_ptr<WidgetAreaWindowOwner> window = WidgetAreaWindowOwner::create(allocation, q);
    std::shared_ptr<WidgetAreaLazyParent> lazy = WidgetAreaLazyParent::bindWindowOwner(window, q);
    std::shared_ptr<WidgetAreaScrollOwner> area = WidgetAreaScrollOwner::create(areaAllocation, q);
    WidgetAreaLazyObservation input;
    WidgetAreaLazyGetter call;
    Fixture() { input.parentOwner = allocation; input.qualification = q; }
    const WidgetAreaLazyReport& resume() { return call.resume(lazy, input); }
    void nullField() { window->observeInitialArea3c8({}, q); }
    void returnConstructor()
    {
        CHECK(resume().stage == WidgetAreaLazyStage::AllocationPending);
        input.allocationSucceeded = true; input.allocationOwner = areaAllocation;
        CHECK(resume().stage == WidgetAreaLazyStage::ConstructorPending);
        input.constructor = WidgetAreaConstructorObservation::Returned;
        input.constructorResultOwner = areaAllocation;
        CHECK(resume().stage == WidgetAreaLazyStage::ParentCommitPending);
    }
    void store(std::size_t occurrence = 1, const WidgetAreaLazyGetter* frame = nullptr)
    {
        input.parentStoreAt184cb = WidgetAreaParentStoreObservation{
            allocation, (frame ? frame : &call)->sourceInvocationOwner(),
            lazy->fieldSnapshot().revision, occurrence};
    }
};
}

TEST_CASE(widget_area_shared_cell_binding_does_not_replay_historical_constructor_null)
{
    Fixture f;
    CHECK(f.window->canonicalAreaFieldIdentity() == f.lazy->canonicalFieldIdentity());
    CHECK(WidgetAreaLazyParent::bindWindowOwner(f.window, q) == f.lazy);
    CHECK(!f.window->area3c8().known && !f.lazy->fieldKnown());
    CHECK(!f.window->nativeAlive() && !f.window->stack480().known);
    CHECK(f.resume().stage == WidgetAreaLazyStage::FieldRead);
    CHECK(!f.call.report().allocationRequested);
    f.nullField();
    CHECK(f.lazy->fieldKnown() && !f.lazy->fieldSnapshot().nonNull);
    CHECK(f.window->area3c8Field().revision == f.lazy->fieldSnapshot().revision);
    CHECK(!f.window->nativeAlive() && !f.area->impl28().known);
    refused([&] { f.lazy->requireImplRegistration(); });
}

TEST_CASE(widget_area_shared_cell_return_does_not_commit_without_actual_store_observation)
{
    Fixture f; f.nullField(); f.returnConstructor();
    CHECK(!f.resume().completed);
    CHECK(f.call.report().stage == WidgetAreaLazyStage::ParentCommitPending);
    CHECK(!f.window->area3c8().nonNull && f.lazy->commitCount() == 0u);
    f.store(); CHECK(f.resume().completed);
    CHECK(f.window->area3c8().known && f.window->area3c8().nonNull);
    CHECK(!f.window->area3c8().target); // raw commit is not a typed/native-live receiver
    CHECK(f.window->area3c8Field().allocation == f.areaAllocation);
    CHECK(f.lazy->cachedArea() == f.areaAllocation && f.lazy->commitCount() == 1u);
    f.window->observeInitialArea3c8(f.area, q); // same pointer/generation: bind typed view
    CHECK(f.window->area3c8().target == f.area);
    CHECK(!f.window->area3c8().usableNativeTarget());
    CHECK(!f.area->impl28().known && !f.window->stack480().known);
    WidgetAreaLazyGetter cached;
    CHECK(cached.resume(f.lazy, f.input).cacheHit);
    CHECK(!cached.report().allocationRequested && f.lazy->commitCount() == 1u);
    refused([&] { f.window->observeInitialArea3c8({}, q); });
    refused([&] { f.lazy->observeInitialField({}, q); });
}

TEST_CASE(widget_area_shared_cell_typed_initial_nonnull_is_same_lazy_cache)
{
    Fixture f;
    f.window->observeInitialArea3c8(f.area, q);
    CHECK(f.resume().cacheHit && f.call.report().returnedArea == f.areaAllocation);
    CHECK(f.window->area3c8().target == f.area);
    CHECK(f.lazy->commitCount() == 0u && !f.call.report().constructorRequested);
    const auto foreignAllocation = std::make_shared<const int>(3);
    const auto foreign = WidgetAreaScrollOwner::create(foreignAllocation, q);
    refused([&] { f.window->observeInitialArea3c8(foreign, q); });
    CHECK(f.window->area3c8().target == f.area);
}

TEST_CASE(widget_area_shared_cell_store_rejects_foreign_colliding_revision_and_zero_occurrence)
{
    Fixture f; f.nullField(); f.returnConstructor(); f.store();
    const std::shared_ptr<const void> forged(f.allocation.get(), [](const void*) {});
    f.input.parentStoreAt184cb->parentOwner = forged;
    refused([&] { f.resume(); });
    f.store(); ++f.input.parentStoreAt184cb->expectedFieldRevision;
    refused([&] { f.resume(); });
    f.store(0); refused([&] { f.resume(); });
    CHECK(!f.window->area3c8().nonNull && f.lazy->commitCount() == 0u);
    f.store(); CHECK(f.resume().completed);
    CHECK(f.resume().completed && f.lazy->commitCount() == 1u);
}

TEST_CASE(widget_area_shared_cell_null_allocation_retries_but_does_not_replay_store_occurrence)
{
    Fixture f; f.nullField(); f.resume(); f.input.allocationSucceeded = false;
    CHECK(f.resume().stage == WidgetAreaLazyStage::ParentCommitPending);
    CHECK(!f.resume().completed && !f.call.report().constructorRequested);
    f.store(7); CHECK(f.resume().completed);
    CHECK(f.window->area3c8().known && !f.window->area3c8().nonNull);
    CHECK(f.lazy->commitCount() == 1u);
    WidgetAreaLazyGetter retry;
    CHECK(retry.resume(f.lazy, f.input).stage == WidgetAreaLazyStage::AllocationPending);
    CHECK(retry.resume(f.lazy, f.input).stage == WidgetAreaLazyStage::ParentCommitPending);
    f.store(8); refused([&] { retry.resume(f.lazy, f.input); }); // old invocation identity
    f.store(7, &retry); refused([&] { retry.resume(f.lazy, f.input); }); // consumed occurrence
    f.store(8, &retry); CHECK(retry.resume(f.lazy, f.input).completed);
    CHECK(f.lazy->commitCount() == 2u && !f.window->area3c8().nonNull);
}

TEST_CASE(widget_area_shared_cell_interruption_preserves_null_and_nested_guard)
{
    Fixture f; f.nullField(); f.resume();
    WidgetAreaLazyGetter nested;
    refused([&] { nested.resume(WidgetAreaLazyParent::bindWindowOwner(f.window, q), f.input); });
    f.input.allocationSucceeded = true; f.input.allocationOwner = f.areaAllocation; f.resume();
    f.input.constructor = WidgetAreaConstructorObservation::Interrupted;
    CHECK(f.resume().stage == WidgetAreaLazyStage::ConstructorInterrupted);
    f.store(); CHECK(!f.resume().completed);
    CHECK(!f.window->area3c8().nonNull && f.lazy->commitCount() == 0u);
}

TEST_CASE(widget_area_shared_cell_expired_raw_and_typed_bindings_are_not_null)
{
    Fixture f;
    f.lazy->observeInitialField(f.areaAllocation, q);
    f.area.reset(); f.areaAllocation.reset();
    CHECK(f.lazy->fieldSnapshot().known && f.lazy->fieldSnapshot().nonNull);
    CHECK(f.lazy->fieldSnapshot().pointerIdentity != nullptr);
    CHECK(!f.resume().completed && !f.call.report().allocationRequested);
    CHECK(f.window->area3c8().nonNull && !f.window->area3c8().target);

    Fixture typed; typed.window->observeInitialArea3c8(typed.area, q); typed.area.reset();
    CHECK(typed.window->area3c8().nonNull && !typed.window->area3c8().target);
    CHECK(!typed.resume().completed && !typed.call.report().allocationRequested);
    const auto replacement = WidgetAreaScrollOwner::create(typed.areaAllocation, q);
    refused([&] { typed.window->observeInitialArea3c8(replacement, q); });
}

TEST_CASE(widget_area_shared_cell_rejects_colliding_typed_allocation_and_forged_window_record)
{
    Fixture f; f.lazy->observeInitialField(f.areaAllocation, q);
    const std::shared_ptr<const void> forgedAllocation(f.areaAllocation.get(), [](const void*) {});
    const auto wrong = WidgetAreaScrollOwner::create(forgedAllocation, q);
    refused([&] { f.window->observeInitialArea3c8(wrong, q); });
    CHECK(!f.window->area3c8().target);
    const std::shared_ptr<WidgetAreaWindowOwner> forgedWindow(f.window.get(), [](WidgetAreaWindowOwner*) {});
    refused([&] { WidgetAreaLazyParent::bindWindowOwner(forgedWindow, q); });
    CHECK(f.resume().cacheHit && f.call.report().returnedArea == f.areaAllocation);
}

TEST_CASE(widget_area_shared_cell_revocation_or_dead_parent_never_clears_field)
{
    Fixture f; f.nullField(); f.lazy->releaseAdmissionScope();
    refused([&] { f.resume(); });
    refused([&] { f.window->observeInitialArea3c8({}, q); });
    CHECK(f.window->area3c8().known && !f.window->area3c8().nonNull);
    Fixture owner; owner.nullField(); owner.window->releaseAdmissionScope();
    refused([&] { owner.resume(); });
    Fixture dead; dead.nullField(); dead.window->observeNativeAlive(false, q);
    refused([&] { dead.resume(); });
    CHECK(dead.window->area3c8().known && !dead.window->area3c8().nonNull);
}

TEST_CASE(widget_area_shared_cell_abandoned_adapter_releases_active_frame_without_native_clear)
{
    Fixture f; f.nullField();
    {
        WidgetAreaLazyGetter abandoned;
        CHECK(abandoned.resume(f.lazy, f.input).stage == WidgetAreaLazyStage::AllocationPending);
        f.lazy.reset(); // window still owns canonical cell, not this adapter wrapper
    }
    f.lazy = WidgetAreaLazyParent::bindWindowOwner(f.window, q);
    CHECK(f.resume().stage == WidgetAreaLazyStage::AllocationPending);
    CHECK(f.window->area3c8().known && !f.window->area3c8().nonNull);
    CHECK(f.lazy->commitCount() == 0u);
}

int main() { return testkit::run_all(); }
