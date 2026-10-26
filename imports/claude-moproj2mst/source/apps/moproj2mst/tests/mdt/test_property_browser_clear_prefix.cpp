// ctest-labels: m2m-mdt
// Declared observations only: no Qt callbacks, clear completion or Boolean Apply.
#include <testkit/check.hpp>
#include <utility>
#include "m2m/engine/property_browser_clear_prefix.hpp"
#include "m2m/common.hpp"

using namespace m2m;
using namespace m2m::engine;
namespace {
template<class F> void refused(F&& action)
{
    bool caught = false;
    try { action(); } catch (const Error& e) { caught = e.code == M2M_E_UNSUPPORTED; }
    CHECK(caught);
}
struct Fixture {
    PropertyRefreshSlotOwner slot{{0x100, 1}, {0x128, 1}, {0x200, 2}, {0x300, 3}};
    std::shared_ptr<PropertyRefreshReceiverState> receiver =
        PropertyRefreshReceiverState::createQualified(slot, 0x777, "declared receiver");
    PropertyRefreshNotifierReport emission = planPropertyRefreshNotifier(
        {{0x50, 5}, 7, std::uint8_t{0}, std::uint64_t{0}, "declared notifier"});
    std::unique_ptr<PropertyRefreshSlotPrefix> parent = std::make_unique<PropertyRefreshSlotPrefix>();
    PropertyRefreshSlotObservation delivery{true, slot, {0x50, 5}, 7, 9, std::uint64_t{0}, "declared slot entry"};
    PropertyRefreshBrowserClearRequest parentRequest;
    std::shared_ptr<PropertyBrowserClearState> state;
    PropertyBrowserSnapshotObservation snapshot;
    PropertyBrowserRemovalObservation removal;
    PropertyBrowserClearPrefix frame;
    explicit Fixture(std::vector<std::uint64_t> entries = {10, 20, 30})
    {
        REQUIRE(emission.emission.has_value());
        const auto& r = parent->resume(receiver, *emission.emission, delivery);
        REQUIRE(r.browserClear.has_value());
        parentRequest = *r.browserClear;
        state = PropertyBrowserClearState::createQualified(parentRequest, {0x400, 4}, "declared B/private owner");
        snapshot = {state->owner(), true, std::move(entries), "declared returned snapshot"};
    }
    const PropertyBrowserClearReport& resume() { return frame.resume(state, snapshot, removal); }
    void observeReturn(PropertyBrowserRemovalReturn result = PropertyBrowserRemovalReturn::Returned)
    {
        REQUIRE(frame.report().removal.has_value());
        removal = {*frame.report().removal, result, "declared actual removal return"};
    }
};
} // namespace

TEST_CASE(property_browser_unknown_snapshot_is_not_empty_or_a_removal)
{
    Fixture f; f.snapshot.copiesReturned = false;
    CHECK(f.resume().stage == PropertyBrowserClearStage::SnapshotPending);
    CHECK(!f.frame.report().snapshotRetained && !f.frame.report().removal);
    f.snapshot.copiesReturned = true; f.snapshot.entries.reset();
    CHECK(f.resume().stage == PropertyBrowserClearStage::SnapshotPending);
    CHECK(!f.frame.report().removal);
}

TEST_CASE(property_browser_empty_snapshot_waits_for_cleanup_without_completing_parent)
{
    Fixture f(std::vector<std::uint64_t>{});
    const auto& r = f.resume();
    CHECK(r.snapshotRetained && r.stage == PropertyBrowserClearStage::CleanupPending);
    CHECK(!r.removal && r.snapshotSize == 0u && r.normalRemovalReturns == 0u);
    CHECK(f.resume().stage == PropertyBrowserClearStage::CleanupPending);
    CHECK(f.parent->report().stage == PropertyRefreshSlotStage::BrowserClearPending);
    CHECK(!f.parent->report().payloadStored);
    CHECK_EQ(*f.receiver->properties(), std::uint64_t{0x777});
}

TEST_CASE(property_browser_snapshot_requests_last_to_first_with_a_return_barrier)
{
    Fixture f;
    for (std::size_t index = 3; index > 0; --index) {
        const auto& r = f.resume();
        REQUIRE(r.removal.has_value());
        CHECK_EQ(r.removal->snapshotIndex, index - 1);
        CHECK_EQ(r.removal->property, std::uint64_t(index * 10));
        CHECK_EQ(r.normalRemovalReturns, std::size_t(3 - index));
        const auto token = r.removal->invocation.lock();
        CHECK(f.resume().removal->invocation.lock() == token);
        f.observeReturn(); f.resume(); f.removal = {};
    }
    CHECK(f.frame.report().stage == PropertyBrowserClearStage::CleanupPending);
    CHECK_EQ(f.frame.report().normalRemovalReturns, std::size_t{3});
    CHECK(!f.frame.report().removal && !f.parent->report().payloadStored);
}

TEST_CASE(property_browser_snapshot_retains_null_duplicates_and_order)
{
    Fixture f({0, 50, 50});
    std::vector<std::uint64_t> seen;
    for (int i = 0; i < 3; ++i) {
        f.resume(); REQUIRE(f.frame.report().removal.has_value());
        seen.push_back(f.frame.report().removal->property);
        f.observeReturn(); f.resume(); f.removal = {};
    }
    const std::vector<std::uint64_t> expected{50, 50, 0};
    CHECK(seen == expected);
    CHECK(f.frame.report().stage == PropertyBrowserClearStage::CleanupPending);
}

TEST_CASE(property_browser_rejects_snapshot_replacement_and_foreign_private_owner)
{
    Fixture f; f.snapshot.owner.browserPrivate.lifetime++;
    refused([&] { f.resume(); });
    CHECK(!f.frame.report().snapshotRetained);
    f.snapshot.owner = f.state->owner(); f.resume();
    f.snapshot.entries = std::vector<std::uint64_t>{99};
    refused([&] { f.resume(); });
    CHECK_EQ(f.frame.report().removal->property, std::uint64_t{30});
    CHECK_EQ(f.frame.report().normalRemovalReturns, std::size_t{0});
}

TEST_CASE(property_browser_early_foreign_and_repeated_returns_cannot_advance)
{
    Fixture early; early.removal.result = PropertyBrowserRemovalReturn::Returned;
    refused([&] { early.resume(); });
    CHECK(!early.frame.report().snapshotRetained);
    Fixture a, b; a.resume(); b.resume(); a.observeReturn(); b.observeReturn();
    a.removal.request.invocation = b.removal.request.invocation;
    refused([&] { a.resume(); });
    a.observeReturn(); a.removal.request.owner.slot.browser.lifetime++;
    refused([&] { a.resume(); });
    a.observeReturn(); a.removal.request.snapshotIndex--;
    refused([&] { a.resume(); });
    CHECK_EQ(a.frame.report().normalRemovalReturns, std::size_t{0});
    a.observeReturn(); a.resume();
    CHECK_EQ(a.frame.report().normalRemovalReturns, std::size_t{1});
    refused([&] { a.resume(); }); // old return is not the newly pending request
    CHECK_EQ(a.frame.report().normalRemovalReturns, std::size_t{1});
}

TEST_CASE(property_browser_interruption_prevents_cursor_advance_and_cleanup)
{
    Fixture f; f.resume(); f.observeReturn(PropertyBrowserRemovalReturn::Interrupted);
    CHECK(f.resume().stage == PropertyBrowserClearStage::RemovalInterrupted);
    CHECK_EQ(f.frame.report().normalRemovalReturns, std::size_t{0});
    CHECK(f.resume().stage == PropertyBrowserClearStage::RemovalInterrupted);
    f.observeReturn(); refused([&] { f.resume(); });
}

TEST_CASE(property_browser_parent_or_local_scope_expiry_prevents_progress)
{
    Fixture parentExpired; parentExpired.resume(); parentExpired.observeReturn(); parentExpired.parent.reset();
    refused([&] { parentExpired.resume(); });
    CHECK_EQ(parentExpired.frame.report().normalRemovalReturns, std::size_t{0});
    Fixture local; local.resume(); local.observeReturn(); local.state->releaseAdmissionScope();
    refused([&] { local.resume(); });
    CHECK_EQ(local.frame.report().normalRemovalReturns, std::size_t{0});
}

TEST_CASE(property_browser_owner_scope_cannot_be_rebound_or_replayed)
{
    Fixture f; f.resume();
    auto otherState = PropertyBrowserClearState::createQualified(f.parentRequest, {0x400, 4}, "colliding source numbers");
    refused([&] { f.frame.resume(otherState, f.snapshot); });
    PropertyBrowserClearPrefix otherFrame;
    refused([&] { otherFrame.resume(f.state, f.snapshot); });
    refused([&] { f.frame.resume({}, f.snapshot); });
    CHECK_EQ(f.frame.report().normalRemovalReturns, std::size_t{0});
}

TEST_CASE(property_browser_snapshot_is_not_replaced_by_current_list_changes)
{
    Fixture f; f.resume();
    // Later current-list changes do not replace the already copied snapshot.
    // Callback/list mutation itself is outside this prefix.
    f.snapshot.copiesReturned = false; f.snapshot.entries.reset();
    f.observeReturn(); f.resume();
    REQUIRE(f.frame.report().removal.has_value());
    CHECK_EQ(f.frame.report().removal->property, std::uint64_t{20});
    CHECK_EQ(f.frame.report().snapshotSize, std::size_t{3});
}

TEST_CASE(property_browser_exhausted_snapshot_does_not_accept_a_cleanup_as_removal_return)
{
    Fixture f({1}); f.resume(); f.observeReturn(); f.resume();
    CHECK(f.frame.report().stage == PropertyBrowserClearStage::CleanupPending);
    refused([&] { f.resume(); });
    CHECK_EQ(f.frame.report().normalRemovalReturns, std::size_t{1});
    CHECK(!f.parent->report().payloadStored);
}

TEST_CASE(property_browser_unqualified_parent_private_owner_or_snapshot_is_rejected)
{
    Fixture f;
    auto invalidParent = f.parentRequest; invalidParent.invocation.reset();
    refused([&] { PropertyBrowserClearState::createQualified(invalidParent, {0x400, 4}, "declared"); });
    refused([&] { PropertyBrowserClearState::createQualified(f.parentRequest, {0x400, 0}, "declared"); });
    refused([&] { PropertyBrowserClearState::createQualified(f.parentRequest, {0x400, 4}, ""); });
    f.snapshot.qualification.clear();
    refused([&] { f.resume(); });
    CHECK(!f.frame.report().snapshotRetained && !f.frame.report().removal);
}

int main() { return testkit::run_all(); }
