// ctest-labels: m2m-mdt
#include <testkit/check.hpp>
#include <string>
#include <vector>
#include "m2m/engine/lifecycle_events.hpp"

using Events = m2m::engine::DiagramLifecycleEvents;
using Delivery = Events::Delivery;

TEST_CASE(diagram_attachment_requests_two_distinct_timers_without_automatic_delivery)
{
    Events events;
    const auto receiver = events.createReceiver();
    const auto first = events.modelAttached(receiver, true);  // 0a7b90
    const auto second = events.modelAttached(receiver, true); // 038380, same model
    REQUIRE(first != 0);
    CHECK(first != second);
    REQUIRE(events.pending().size() == 2u);
    CHECK_EQ(events.pending()[0].requestedIntervalMs, 1000u);
    CHECK_EQ(events.pending()[1].requestedIntervalMs, 1000u);
    CHECK_EQ(events.pending()[0].receiver, receiver);
    unsigned calls = 0;
    const auto callback = [&](Events::ReceiverId id) { CHECK_EQ(id, receiver); ++calls; };
    CHECK_EQ(calls, 0u); // Immediate adapter Save supplies no due timer input.
    CHECK(events.deliver(first, callback) == Delivery::Delivered);
    CHECK_EQ(calls, 1u);
    REQUIRE(events.pending().size() == 1u);
    CHECK_EQ(events.pending()[0].id, second);
    CHECK(events.deliver(first, callback) == Delivery::NotPending);
    CHECK(events.deliver(0, callback) == Delivery::NotPending);
    CHECK_EQ(calls, 1u);
}

TEST_CASE(diagram_null_rebind_retains_timer_and_callback_reads_current_attachment)
{
    Events events;
    const auto receiver = events.createReceiver();
    const auto first = events.modelAttached(receiver, true);
    int model = 1, item = 4;
    unsigned fits = 0;
    std::vector<int> selections;
    const auto callback = [&](Events::ReceiverId) {
        ++fits; // 183160 fits before checking current model/item.
        if (model != 0 && item >= 0) selections.push_back(item);
    };
    model = 0;
    CHECK_EQ(events.modelAttached(receiver, false), 0u);
    REQUIRE(events.pending().size() == 1u);
    CHECK(events.deliver(first, callback) == Delivery::Delivered);
    CHECK_EQ(fits, 1u);
    CHECK(selections.empty());
    model = 2;
    const auto second = events.modelAttached(receiver, true);
    item = 9; // Resolve at delivery; never use the item present at scheduling.
    CHECK(events.deliver(second, callback) == Delivery::Delivered);
    REQUIRE(selections.size() == 1u);
    CHECK_EQ(selections[0], 9);
}

TEST_CASE(diagram_receiver_destruction_disconnects_without_cancelling_timer)
{
    Events events;
    const auto dead = events.createReceiver();
    const auto timer = events.modelAttached(dead, true);
    events.destroyReceiver(dead);
    REQUIRE(events.pending().size() == 1u);
    const auto replacement = events.createReceiver();
    CHECK(replacement != dead); // New lifetime cannot revive old connections.
    const auto liveTimer = events.modelAttached(replacement, true);
    unsigned calls = 0;
    const auto callback = [&](Events::ReceiverId id) { CHECK_EQ(id, replacement); ++calls; };
    CHECK(events.deliver(timer, callback) == Delivery::Disconnected);
    CHECK_EQ(calls, 0u);
    CHECK(events.deliver(liveTimer, callback) == Delivery::Delivered);
    CHECK_EQ(calls, 1u);
    CHECK_EQ(events.modelAttached(dead, true), 0u);
}

TEST_CASE(diagram_dispatcher_destruction_removes_timers_and_prevents_later_registration)
{
    Events events;
    const auto receiver = events.createReceiver();
    const auto old = events.modelAttached(receiver, true);
    events.dispatcherDestroyed();
    CHECK(events.pending().empty());
    unsigned calls = 0;
    const auto callback = [&](Events::ReceiverId) { ++calls; };
    CHECK(events.deliver(old, callback) == Delivery::NotPending);
    CHECK_EQ(events.modelAttached(receiver, true), 0u);
    const auto laterReceiver = events.createReceiver(); // QObject can exist without dispatcher.
    CHECK_EQ(events.modelAttached(laterReceiver, true), 0u);
    events.dispatcherDestroyed(); // Repeated teardown does not restore scheduling.
    CHECK(events.pending().empty());
    CHECK_EQ(calls, 0u);
}

TEST_CASE(diagram_delivery_consumes_before_reentrant_callback)
{
    Events events;
    const auto receiver = events.createReceiver();
    const auto first = events.modelAttached(receiver, true);
    const auto second = events.modelAttached(receiver, true);
    Events::TimerId added = 0;
    unsigned calls = 0;
    const auto callback = [&](Events::ReceiverId id) {
        ++calls;
        CHECK(events.deliver(first, [](Events::ReceiverId) {}) == Delivery::NotPending);
        added = events.modelAttached(id, true);
        events.destroyReceiver(id);
    };
    CHECK(events.deliver(first, callback) == Delivery::Delivered);
    REQUIRE(events.pending().size() == 2u);
    CHECK_EQ(events.pending()[0].id, second);
    CHECK_EQ(events.pending()[1].id, added);
    CHECK(events.deliver(second, callback) == Delivery::Disconnected);
    CHECK(events.deliver(added, callback) == Delivery::Disconnected);
    CHECK_EQ(calls, 1u);
}

TEST_CASE(diagram_due_events_use_replacement_selection_callback_with_editor_refill)
{
    Events events;
    const auto receiver = events.createReceiver();
    const auto first = events.modelAttached(receiver, true);
    const auto second = events.modelAttached(receiver, true);
    // Callback-order fixture, not a replacement for the real Boolean hooks.
    // A first AddNew selection becomes an existing-object selection on refill.
    int object = 0, additions = 0, shownSelection = 0;
    std::vector<std::string> order;
    const auto replaceSelection = [&](Events::ReceiverId) {
        order.push_back("apply");
        if (shownSelection == 0) object = ++additions;
        order.push_back("leave");
        order.push_back("factory-fill");
        shownSelection = object;
        order.push_back("show");
    };
    CHECK(events.deliver(first, replaceSelection) == Delivery::Delivered);
    CHECK(events.deliver(second, replaceSelection) == Delivery::Delivered);
    CHECK_EQ(additions, 1);
    CHECK(order == std::vector<std::string>({"apply", "leave", "factory-fill", "show",
                                            "apply", "leave", "factory-fill", "show"}));
}

int main() { return testkit::run_all(); }
