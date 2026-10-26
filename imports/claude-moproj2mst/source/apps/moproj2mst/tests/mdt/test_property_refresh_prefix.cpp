// ctest-labels: m2m-mdt
// Declared source observations only; no original Qt delivery or Boolean Apply.
#include <testkit/check.hpp>
#include "m2m/engine/property_refresh_prefix.hpp"
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
PropertyRefreshNotifierObservation notifier(std::uint64_t payload = 0)
{
    return {{0x100, 1}, 7, std::uint8_t{0}, payload, "declared notifier observation"};
}
struct Fixture {
    PropertyRefreshSlotOwner owner{{0x200, 2}, {0x228, 2}, {0x300, 3}, {0x400, 4}};
    std::shared_ptr<PropertyRefreshReceiverState> state =
        PropertyRefreshReceiverState::createQualified(owner, 0x500, "declared owner mapping");
    PropertyRefreshNotifierReport planned;
    PropertyRefreshSlotObservation delivery;
    PropertyRefreshClearObservation clear;
    PropertyRefreshSlotPrefix frame;
    explicit Fixture(std::uint64_t payload = 0) : planned(planPropertyRefreshNotifier(notifier(payload)))
    {
        REQUIRE(planned.emission.has_value());
        delivery = {true, owner, {0x100, 1}, 7, 11, payload, "declared actual slot entry"};
    }
    const PropertyRefreshSlotReport& resume() { return frame.resume(state, *planned.emission, delivery, clear); }
    void observeClear(PropertyRefreshClearReturn result = PropertyRefreshClearReturn::Returned)
    {
        REQUIRE(frame.report().browserClear.has_value());
        clear = {*frame.report().browserClear, result, "declared clear return"};
    }
};
} // namespace

TEST_CASE(property_refresh_notifier_unknown_is_not_emitted_or_null)
{
    auto o = notifier(); o.suppression.reset();
    CHECK(planPropertyRefreshNotifier(o).stage == PropertyRefreshNotifierStage::Admission);
    o.suppression = 0; o.payload.reset();
    CHECK(!planPropertyRefreshNotifier(o).emission);
    o.payload = 0; o.notifier.lifetime = 0;
    CHECK(!planPropertyRefreshNotifier(o).emission);
    o = notifier(); o.qualification.clear();
    CHECK(!planPropertyRefreshNotifier(o).emission);
}

TEST_CASE(property_refresh_suppression_returns_without_using_payload)
{
    auto o = notifier(); o.suppression = 2; o.payload.reset();
    auto r = planPropertyRefreshNotifier(o);
    CHECK(r.completed && r.stage == PropertyRefreshNotifierStage::Suppressed);
    CHECK(!r.emission);
    o = notifier(); r = planPropertyRefreshNotifier(o);
    REQUIRE(r.emission.has_value());
    CHECK(!r.completed && r.stage == PropertyRefreshNotifierStage::EmissionPending);
    CHECK_EQ(r.emission->signalLocalIndex, 3u);
    CHECK_EQ(r.emission->payload(), std::uint64_t{0});
}

TEST_CASE(property_refresh_emission_does_not_admit_slot_or_clear)
{
    Fixture f; f.delivery.slotEntryObserved = false;
    CHECK(f.resume().stage == PropertyRefreshSlotStage::Admission);
    CHECK(!f.frame.report().browserClear && !f.frame.report().payloadStored);
    CHECK_EQ(f.state->prefixStoreCount(), std::uint64_t{0});
    f.delivery.slotEntryObserved = true; f.delivery.payload.reset();
    CHECK(f.resume().stage == PropertyRefreshSlotStage::Admission);
    CHECK(!f.frame.report().browserClear);
}

TEST_CASE(property_refresh_null_store_requires_a_later_normal_clear_return)
{
    Fixture f;
    CHECK(f.resume().stage == PropertyRefreshSlotStage::BrowserClearPending);
    CHECK_EQ(*f.state->properties(), std::uint64_t{0x500});
    CHECK(!f.resume().payloadStored);
    f.observeClear();
    CHECK(f.resume().completed && f.frame.report().payloadStored);
    CHECK_EQ(*f.state->properties(), std::uint64_t{0});
    CHECK_EQ(f.state->prefixStoreCount(), std::uint64_t{1});
    CHECK(f.resume().completed);
    CHECK_EQ(f.state->prefixStoreCount(), std::uint64_t{1});
}

TEST_CASE(property_refresh_nonnull_store_stops_before_property_enumeration)
{
    Fixture f(0x600); f.resume(); f.observeClear();
    const auto& r = f.resume();
    CHECK(!r.completed && r.payloadStored && r.stage == PropertyRefreshSlotStage::PropertyListPending);
    CHECK_EQ(*f.state->properties(), std::uint64_t{0x600});
    f.resume(); CHECK_EQ(f.state->prefixStoreCount(), std::uint64_t{1});
}

TEST_CASE(property_refresh_rejects_foreign_delivery_and_rebinding)
{
    Fixture f; f.delivery.owner.receiver.lifetime++;
    refused([&] { f.resume(); });
    CHECK(!f.frame.report().browserClear && !f.frame.report().payloadStored);
    f.delivery.owner = f.owner; f.resume();
    auto colliding = PropertyRefreshReceiverState::createQualified(f.owner, 0x500, "other admission scope");
    refused([&] { f.frame.resume(colliding, *f.planned.emission, f.delivery); });
    auto sameNumbers = planPropertyRefreshNotifier(notifier());
    refused([&] { f.frame.resume(f.state, *sameNumbers.emission, f.delivery); });
    CHECK_EQ(f.state->prefixStoreCount(), std::uint64_t{0});
}

TEST_CASE(property_refresh_rejects_wrong_or_early_clear_completion)
{
    Fixture early;
    early.clear.result = PropertyRefreshClearReturn::Returned;
    refused([&] { early.resume(); });
    CHECK(!early.frame.report().browserClear);
    Fixture a, b; b.delivery.deliveryOrdinal = 12;
    a.resume(); b.resume(); a.observeClear(); b.observeClear();
    a.clear.request.invocation = b.clear.request.invocation;
    refused([&] { a.resume(); });
    a.observeClear(); a.clear.request.owner.browser.lifetime++;
    refused([&] { a.resume(); });
    a.observeClear(); a.clear.request.deliveryOrdinal++;
    refused([&] { a.resume(); });
    CHECK_EQ(a.state->prefixStoreCount(), std::uint64_t{0});
}

TEST_CASE(property_refresh_expired_receiver_or_interrupted_clear_cannot_store)
{
    Fixture expired; expired.resume(); expired.observeClear(); expired.state->releaseAdmissionScope();
    refused([&] { expired.resume(); });
    CHECK_EQ(expired.state->prefixStoreCount(), std::uint64_t{0});
    Fixture interrupted; interrupted.resume(); interrupted.observeClear(PropertyRefreshClearReturn::Interrupted);
    CHECK(interrupted.resume().stage == PropertyRefreshSlotStage::BrowserClearInterrupted);
    CHECK(!interrupted.frame.report().payloadStored);
    interrupted.observeClear(); refused([&] { interrupted.resume(); });
    CHECK_EQ(interrupted.state->prefixStoreCount(), std::uint64_t{0});
}

TEST_CASE(property_refresh_external_change_during_clear_does_not_replace_later_native_store)
{
    Fixture f(0x600); f.resume();
    f.state->observeProperties(0x700, "declared external change during clear, callback unmodeled");
    CHECK_EQ(*f.state->properties(), std::uint64_t{0x700});
    CHECK_EQ(f.state->prefixStoreCount(), std::uint64_t{0});
    f.observeClear(); f.resume();
    CHECK_EQ(*f.state->properties(), std::uint64_t{0x600});
    CHECK_EQ(f.state->prefixStoreCount(), std::uint64_t{1});
}

TEST_CASE(property_refresh_one_delivery_ordinal_cannot_be_replayed_by_another_frame)
{
    Fixture f; f.resume();
    PropertyRefreshSlotPrefix other;
    refused([&] { other.resume(f.state, *f.planned.emission, f.delivery); });
    f.delivery.deliveryOrdinal = 12;
    CHECK(other.resume(f.state, *f.planned.emission, f.delivery).stage == PropertyRefreshSlotStage::BrowserClearPending);
    CHECK_EQ(f.state->prefixStoreCount(), std::uint64_t{0});
}

TEST_CASE(property_refresh_unknown_receiver_field_is_not_null_until_clear_return)
{
    Fixture f;
    f.state = PropertyRefreshReceiverState::createQualified(f.owner, std::nullopt, "declared unknown cell");
    CHECK(!f.state->properties());
    f.resume(); CHECK(!f.state->properties());
    f.observeClear(); f.resume();
    REQUIRE(f.state->properties().has_value());
    CHECK_EQ(*f.state->properties(), std::uint64_t{0});
    refused([&] { f.frame.resume({}, *f.planned.emission, f.delivery); });
}

TEST_CASE(property_refresh_wrong_payload_and_missing_clear_identity_are_rejected)
{
    Fixture f(0x600); f.delivery.payload = 0x700;
    refused([&] { f.resume(); });
    f.delivery.payload = 0x600; f.resume(); f.observeClear();
    f.clear.request.invocation.reset();
    refused([&] { f.resume(); });
    CHECK_EQ(f.state->prefixStoreCount(), std::uint64_t{0});
    f.observeClear(); f.delivery.payload.reset();
    CHECK(f.resume().payloadStored); // prior admitted slot payload is retained
    CHECK_EQ(*f.state->properties(), std::uint64_t{0x600});
}

int main() { return testkit::run_all(); }
