// ctest-labels: m2m-mdt
// Synthetic source-stage observations; no original widget/runtime capture.
#include <testkit/check.hpp>
#include "m2m/engine/widget_area_lazy.hpp"
#include "m2m/model.hpp"

using namespace m2m;
using namespace m2m::engine;

namespace {
template<class F> void refused(F&& action)
{
    bool caught = false;
    try { action(); } catch (const Error& error) { caught = error.code == M2M_E_UNSUPPORTED; }
    CHECK(caught);
}

struct Fixture {
    std::shared_ptr<const void> nativeParent = std::make_shared<const int>(1);
    std::shared_ptr<const void> nativeAllocation = std::make_shared<const int>(2);
    std::shared_ptr<WidgetAreaLazyParent> parent = WidgetAreaLazyParent::createQualified(
        nativeParent, std::shared_ptr<const void>{}, "synthetic observed parent+3c8=null");
    WidgetAreaLazyObservation input;
    WidgetAreaLazyGetter frame;
    Fixture()
    {
        input.parentOwner = nativeParent;
        input.qualification = "synthetic source-stage witness, not native readiness";
    }
    const WidgetAreaLazyReport& resume() { return frame.resume(parent, input); }
    void observeAllocation()
    {
        input.allocationSucceeded = true;
        input.allocationOwner = nativeAllocation;
    }
    void observeReturn()
    {
        input.constructor = WidgetAreaConstructorObservation::Returned;
        input.constructorResultOwner = nativeAllocation;
    }
};
} // namespace

TEST_CASE(widget_area_lazy_unknown_field_is_not_null_or_an_allocation_request)
{
    Fixture f;
    f.parent = WidgetAreaLazyParent::createQualified(f.nativeParent, std::nullopt,
        "synthetic actual parent, field not observed");
    const auto& unknown = f.resume();
    CHECK(!unknown.completed && unknown.stage == WidgetAreaLazyStage::FieldRead);
    CHECK(!unknown.allocationRequested && !f.parent->fieldKnown());
    f.parent->observeInitialField({}, "synthetic now observed null");
    CHECK(f.parent->fieldKnown() && !f.parent->cachedArea());
    CHECK(f.resume().stage == WidgetAreaLazyStage::AllocationPending);
    CHECK(f.frame.report().allocationRequested);
    CHECK_EQ(f.frame.report().allocationBytes, std::size_t{0x30});
    CHECK_EQ(f.parent->commitCount(), std::size_t{0});
    refused([&] { f.parent->observeInitialField(f.nativeAllocation, "cannot replace field"); });
}

TEST_CASE(widget_area_lazy_cached_hit_has_no_allocation_constructor_or_store)
{
    Fixture f;
    f.parent = WidgetAreaLazyParent::createQualified(f.nativeParent,
        f.nativeAllocation, "synthetic cached nonnull parent field");
    const auto& result = f.resume();
    CHECK(result.completed && result.cacheHit);
    CHECK(result.returnedArea == f.nativeAllocation);
    CHECK(!result.allocationRequested && !result.constructorRequested && !result.constructorReturned);
    CHECK_EQ(f.parent->commitCount(), std::size_t{0});
    CHECK(f.resume().returnedArea == f.nativeAllocation);
    refused([&] { f.parent->requireImplRegistration(); });
}

TEST_CASE(widget_area_lazy_success_commits_only_after_observed_constructor_return)
{
    Fixture f;
    CHECK(f.resume().stage == WidgetAreaLazyStage::AllocationPending);
    f.observeAllocation();
    const auto& allocated = f.resume();
    CHECK(allocated.stage == WidgetAreaLazyStage::ConstructorPending);
    CHECK(allocated.constructorRequested && !allocated.constructorReturned);
    CHECK(allocated.constructorParentNull && allocated.constructorFlag == 1u);
    CHECK(allocated.allocationOwner == f.nativeAllocation);
    CHECK(!f.parent->cachedArea() && f.parent->commitCount() == 0u);
    f.observeReturn();
    CHECK(f.resume().stage == WidgetAreaLazyStage::ParentCommitPending);
    CHECK(f.frame.report().constructorReturned && !f.frame.report().completed);
    CHECK(!f.parent->cachedArea() && f.parent->commitCount() == 0u);
    CHECK(f.resume().completed && f.parent->cachedArea() == f.nativeAllocation);
    CHECK_EQ(f.parent->commitCount(), std::size_t{1});
    CHECK(f.resume().completed && f.parent->commitCount() == 1u);
    WidgetAreaLazyGetter cached;
    CHECK(cached.resume(f.parent, f.input).cacheHit);
    CHECK(!cached.report().allocationRequested && !cached.report().constructorRequested);
    CHECK_EQ(f.parent->commitCount(), std::size_t{1});
    refused([&] { f.parent->requireImplRegistration(); }); // returned is not an impl projection
}

TEST_CASE(widget_area_lazy_unresolved_constructor_never_commits_or_registers)
{
    Fixture f;
    f.resume(); f.observeAllocation(); f.resume();
    for (int i = 0; i != 3; ++i) {
        CHECK(f.resume().stage == WidgetAreaLazyStage::ConstructorPending);
        CHECK(!f.parent->cachedArea() && !f.frame.report().completed);
        CHECK(!f.frame.report().constructorReturned);
        CHECK_EQ(f.parent->commitCount(), std::size_t{0});
        refused([&] { f.parent->requireImplRegistration(); });
    }
}

TEST_CASE(widget_area_lazy_interrupted_constructor_keeps_allocation_without_parent_commit)
{
    Fixture f;
    f.resume(); f.observeAllocation(); f.resume();
    f.input.constructor = WidgetAreaConstructorObservation::Interrupted;
    CHECK(f.resume().stage == WidgetAreaLazyStage::ConstructorInterrupted);
    CHECK(!f.frame.report().completed && !f.frame.report().constructorReturned);
    CHECK(f.frame.report().allocationOwner == f.nativeAllocation);
    CHECK(!f.parent->cachedArea() && f.parent->commitCount() == 0u);
    f.observeReturn(); // an interrupted invocation cannot become a returned one
    CHECK(f.resume().stage == WidgetAreaLazyStage::ConstructorInterrupted);
    CHECK(!f.parent->cachedArea() && f.parent->commitCount() == 0u);
}

TEST_CASE(widget_area_lazy_null_allocator_result_commits_null_and_later_call_retries)
{
    Fixture f;
    f.resume();
    CHECK(f.resume().stage == WidgetAreaLazyStage::AllocationPending); // unavailable is not failure
    f.input.allocationSucceeded = false;
    CHECK(f.resume().stage == WidgetAreaLazyStage::ParentCommitPending);
    CHECK(!f.frame.report().constructorRequested && !f.parent->cachedArea());
    CHECK(f.resume().completed && !f.frame.report().returnedArea);
    CHECK(f.parent->fieldKnown() && !f.parent->cachedArea());
    CHECK_EQ(f.parent->commitCount(), std::size_t{1});
    WidgetAreaLazyGetter retry;
    CHECK(retry.resume(f.parent, f.input).stage == WidgetAreaLazyStage::AllocationPending);
    CHECK(retry.report().allocationRequested && !retry.report().cacheHit);
}

TEST_CASE(widget_area_lazy_rejects_wrong_parent_and_same_address_different_lifetimes)
{
    Fixture f;
    const std::shared_ptr<const void> forgedOwner(f.nativeParent.get(), [](const void*) {});
    f.input.parentOwner = forgedOwner;
    refused([&] { f.resume(); });
    f.input.parentOwner = f.nativeParent;
    CHECK(f.resume().stage == WidgetAreaLazyStage::AllocationPending);
    Fixture other;
    refused([&] { f.frame.resume(other.parent, other.input); });
    const std::shared_ptr<WidgetAreaLazyParent> forgedState(f.parent.get(), [](WidgetAreaLazyParent*) {});
    refused([&] { f.frame.resume(forgedState, f.input); });
    CHECK(!f.parent->cachedArea() && f.parent->commitCount() == 0u);
}

TEST_CASE(widget_area_lazy_constructor_return_requires_canonical_allocation_identity)
{
    Fixture f;
    f.resume(); f.observeAllocation(); f.resume();
    f.observeReturn();
    const std::shared_ptr<const void> forged(f.nativeAllocation.get(), [](const void*) {});
    f.input.constructorResultOwner = forged;
    refused([&] { f.resume(); });
    CHECK(!f.parent->cachedArea() && f.parent->commitCount() == 0u);
    f.observeReturn();
    CHECK(f.resume().stage == WidgetAreaLazyStage::ParentCommitPending);
    CHECK(f.resume().completed);
}

TEST_CASE(widget_area_lazy_null_allocation_rejects_contradictory_constructor_effects)
{
    Fixture f;
    f.resume();
    f.input.allocationSucceeded = false;
    f.observeReturn();
    refused([&] { f.resume(); });
    CHECK(!f.parent->cachedArea() && f.parent->commitCount() == 0u);
    CHECK(!f.frame.report().constructorRequested);
}

TEST_CASE(widget_area_lazy_nested_construction_and_expired_admission_remain_guarded)
{
    Fixture f;
    f.resume();
    WidgetAreaLazyGetter nested;
    refused([&] { nested.resume(f.parent, f.input); });
    f.parent->releaseAdmissionScope();
    f.observeAllocation();
    refused([&] { f.resume(); });
    CHECK(!f.parent->cachedArea() && f.parent->commitCount() == 0u);

    Fixture expired;
    expired.nativeParent.reset();
    refused([&] { expired.resume(); });
    CHECK(!expired.frame.report().allocationRequested);
}

int main() { return testkit::run_all(); }
