// ctest-labels: m2m-mdt
// Synthetic binder contracts only; no original readiness/timing acceptance.
#include <testkit/check.hpp>
#include <type_traits>
#include "m2m/engine/boolean_trace_due.hpp"

using namespace m2m::engine;
namespace {
struct Fixture {
    DiagramLifecycleEvents events;
    BooleanTraceOwnerScope scope{events};
    DiagramLifecycleEvents::ReceiverId receiver = events.createReceiver();
    DiagramLifecycleEvents::TimerId first = events.modelAttached(receiver, true);
    DiagramLifecycleEvents::TimerId second = events.modelAttached(receiver, true);
    BooleanTraceBindingContext context;
    BooleanPumpTrace trace;
    Fixture() {
        context.logicalOwner = &events;
        context.ownerScope = &scope;
        context.expectedIdentity = {std::string(64, 'a'), std::string(64, 'b'), std::string(64, 'c'),
            std::string(64, 'd'), std::string(64, 'e'), "synthetic action contract", "synthetic session1"};
        context.expectedPoint = {"140098129", 1};
        context.expectedNativeThread = 88;
        for (unsigned i = 0; i != 2; ++i) {
            BooleanTraceRegistration m;
            m.native = {0x1000, 1, 0x2000 + i * 0x100, 1, 1 + i};
            m.nativeTimerId = 11 + static_cast<int>(i); m.requestedIntervalMs = 1000; m.nativeThread = 88;
            m.logicalReceiver = receiver; m.logicalTimer = i ? second : first;
            m.evidence = "synthetic registration contract";
            context.registrations.push_back(m);
        }
        trace.identity = context.expectedIdentity; trace.point = context.expectedPoint;
        trace.nativeThread = 88; trace.enterOrdinal = 100; trace.returnOrdinal = 200;
        trace.complete = trace.balanced = trace.registrationsComplete = true;
        trace.evidence = "synthetic complete interval, not an original observation";
        trace.deliveries = {delivery(1, 110, 120), delivery(0, 130, 140)};
    }
    BooleanTraceDelivery delivery(std::size_t i, std::uint64_t enter, std::uint64_t leave) const {
        BooleanTraceDelivery d;
        d.native = context.registrations[i].native; d.nativeTimerId = context.registrations[i].nativeTimerId;
        d.nativeThread = 88; d.enterOrdinal = enter; d.returnOrdinal = leave;
        d.timerEventObserved = d.modelUpdatedObserved = d.matchedCallbackReceiver = d.balanced = true;
        d.evidence = "synthetic timerEvent/modelUpdated contract";
        return d;
    }
    BooleanTraceDueResult map() const { return bindBooleanTraceDue(events, context, trace); }
    BooleanTraceDueValidation bind() const {
        auto result = map();
        if (!result.binding) return {std::nullopt, result.issue};
        return validateBooleanTraceDueForOwner(events, scope, *result.binding);
    }
};
void rejected(const BooleanTraceDueResult& r) { CHECK(!r.binding); CHECK(!r.issue.empty()); }
void rejected(const BooleanTraceDueValidation& r) { CHECK(!r.observation); CHECK(!r.issue.empty()); }
static_assert(!std::is_convertible_v<BooleanBoundTraceDue, DueObservation>);
static_assert(!std::is_copy_constructible_v<BooleanTraceOwnerScope>);
static_assert(!std::is_move_constructible_v<BooleanTraceOwnerScope>);
}

TEST_CASE(boolean_trace_due_preserves_observed_order_and_never_consumes_owner_timers)
{
    Fixture f;
    const auto r = f.bind(); REQUIRE(r.observation); CHECK(r.issue.empty());
    CHECK(r.observation->origin == DueObservation::Origin::OriginalTrace);
    CHECK(r.observation->point.sourceSite == "140098129" && r.observation->point.occurrence == 1);
    CHECK(r.observation->dueInOrder == std::vector<DiagramLifecycleEvents::TimerId>({f.second, f.first}));
    CHECK(r.observation->qualification.find(f.trace.evidence) != std::string::npos);
    REQUIRE(f.events.pending().size() == 2);
    CHECK(f.events.pending()[0].id == f.first && f.events.pending()[1].id == f.second);
    const auto repeated = f.bind(); REQUIRE(repeated.observation);
    CHECK(repeated.observation->dueInOrder == r.observation->dueInOrder);
    unsigned callbacks = 0;
    for (auto id : r.observation->dueInOrder)
        CHECK(f.events.deliver(id, [&](auto receiver) { CHECK(receiver == f.receiver); ++callbacks; }) ==
            DiagramLifecycleEvents::Delivery::Delivered);
    CHECK(callbacks == 2 && f.events.pending().empty());
    rejected(f.bind()); // previously consumed IDs cannot bind to pending state
}

TEST_CASE(boolean_trace_due_unavailable_interval_is_distinct_from_complete_empty)
{
    Fixture f; f.trace.deliveries.clear(); f.trace.complete = false;
    rejected(f.bind()); CHECK(f.events.pending().size() == 2);
    f.trace.complete = true;
    auto r = f.bind(); REQUIRE(r.observation); CHECK(r.observation->dueInOrder.empty());
    CHECK(r.observation->origin == DueObservation::Origin::OriginalTrace);
    CHECK(f.events.pending().size() == 2);
    f.context.registrations.clear(); // complete interval can also have no selected registrations
    REQUIRE(f.bind().observation);
}

TEST_CASE(boolean_trace_due_rejects_incomplete_truncated_or_error_coverage)
{
    Fixture f;
    f.trace.balanced = false; rejected(f.bind()); f.trace.balanced = true;
    f.trace.registrationsComplete = false; rejected(f.bind()); f.trace.registrationsComplete = true;
    f.trace.truncated = true; rejected(f.bind()); f.trace.truncated = false;
    f.trace.readErrors = 1; rejected(f.bind()); f.trace.readErrors = 0;
    f.trace.observerErrors = 1; rejected(f.bind()); f.trace.observerErrors = 0;
    f.trace.unmatchedFrames = 1; rejected(f.bind()); f.trace.unmatchedFrames = 0;
    f.trace.evidence.clear(); rejected(f.bind());
    CHECK(f.events.pending().size() == 2);
}

TEST_CASE(boolean_trace_due_rejects_profile_module_session_and_pump_mismatch)
{
    Fixture f;
    auto identity = f.trace.identity;
    f.trace.identity.fixtureSha256 = std::string(64, 'f'); rejected(f.bind()); f.trace.identity = identity;
    f.trace.identity.qtCoreSha256 = "not a digest"; rejected(f.bind()); f.trace.identity = identity;
    f.trace.identity.actionProfile += " other"; rejected(f.bind()); f.trace.identity = identity;
    f.trace.identity.nativeSession += " other"; rejected(f.bind()); f.trace.identity = identity;
    ++f.trace.point.occurrence; rejected(f.bind()); --f.trace.point.occurrence;
    f.trace.point.sourceSite = "another pump"; rejected(f.bind()); f.trace.point = f.context.expectedPoint;
    ++f.trace.nativeThread; rejected(f.bind()); --f.trace.nativeThread;
    f.trace.returnOrdinal = f.trace.enterOrdinal; rejected(f.bind());
}

TEST_CASE(boolean_trace_due_rejects_other_owner_even_when_logical_ids_collide)
{
    Fixture f;
    DiagramLifecycleEvents other;
    const auto receiver = other.createReceiver(); other.modelAttached(receiver, true); other.modelAttached(receiver, true);
    CHECK(other.pending()[0].id == f.first);
    rejected(bindBooleanTraceDue(other, f.context, f.trace));
    CHECK(f.events.pending().size() == 2 && other.pending().size() == 2);
}

TEST_CASE(boolean_trace_due_rejects_ambiguous_or_unregistered_mapping)
{
    Fixture f;
    const auto original = f.context.registrations;
    f.context.registrations[1].logicalTimer = f.first; rejected(f.bind()); f.context.registrations = original;
    f.context.registrations[1].native = f.context.registrations[0].native;
    rejected(f.bind()); f.context.registrations = original;
    f.context.registrations[0].logicalTimer = 900; rejected(f.bind()); f.context.registrations = original;
    f.context.registrations[0].nativeTimerId = 0; rejected(f.bind()); f.context.registrations = original;
    f.context.registrations[0].requestedIntervalMs = 999; rejected(f.bind()); f.context.registrations = original;
    f.context.registrations[0].nativeThread = 89; rejected(f.bind()); f.context.registrations = original;
    f.context.registrations[0].evidence.clear(); rejected(f.bind()); f.context.registrations = original;
    f.context.registrations[0].native.receiverLifetime = 0; rejected(f.bind());
}

TEST_CASE(boolean_trace_due_timer_address_reuse_requires_a_distinct_lifetime)
{
    Fixture f;
    f.context.registrations[1].native.timerAddress = f.context.registrations[0].native.timerAddress;
    f.trace.deliveries[0] = f.delivery(1, 110, 120);
    rejected(f.bind()); // same address/generation cannot denote two timers
    f.context.registrations[1].native.timerLifetime = 2;
    f.trace.deliveries[0] = f.delivery(1, 110, 120);
    REQUIRE(f.bind().observation);
    f.trace.deliveries[0].native.timerLifetime = 1;
    rejected(f.bind()); // cannot use the reused address with a stale key
}

TEST_CASE(boolean_trace_due_rejects_duplicate_crossing_and_unbalanced_deliveries)
{
    Fixture f;
    const auto original = f.trace.deliveries;
    f.trace.deliveries[1] = f.delivery(1, 130, 140); rejected(f.bind()); f.trace.deliveries = original;
    f.trace.deliveries[0].returnOrdinal = 135; rejected(f.bind()); f.trace.deliveries = original;
    f.trace.deliveries[0].returnOrdinal = 150; // nesting cannot be flattened into a sequential pump
    rejected(f.bind());
    f.trace.deliveries = original;
    f.trace.deliveries[0].balanced = false; rejected(f.bind()); f.trace.deliveries = original;
    f.trace.deliveries[0].timerEventObserved = false; rejected(f.bind()); f.trace.deliveries = original;
    f.trace.deliveries[0].modelUpdatedObserved = false; rejected(f.bind()); f.trace.deliveries = original;
    f.trace.deliveries[0].matchedCallbackReceiver = false; rejected(f.bind()); f.trace.deliveries = original;
    f.trace.deliveries[0].enterOrdinal = f.trace.enterOrdinal; rejected(f.bind());
}

TEST_CASE(boolean_trace_due_rejects_delivery_from_other_thread_id_or_timer_generation)
{
    Fixture f;
    const auto original = f.trace.deliveries;
    ++f.trace.deliveries[0].nativeThread; rejected(f.bind()); f.trace.deliveries = original;
    ++f.trace.deliveries[0].nativeTimerId; rejected(f.bind()); f.trace.deliveries = original;
    ++f.trace.deliveries[0].native.registrationOrdinal; rejected(f.bind()); f.trace.deliveries = original;
    f.trace.deliveries[0].returnOrdinal = f.trace.returnOrdinal; rejected(f.bind());
    CHECK(f.events.pending().size() == 2);
}

TEST_CASE(boolean_trace_due_diagnostic_bounds_fail_closed_without_owner_mutation)
{
    Fixture f;
    f.context.registrations.resize(1025); rejected(f.bind());
    CHECK(f.events.pending().size() == 2);
    f.context.registrations.resize(2); f.trace.deliveries.resize(4097); rejected(f.bind());
    CHECK(f.events.pending().size() == 2);
}


TEST_CASE(boolean_trace_due_bound_receipt_rejects_another_owner_with_colliding_pending_ids)
{
    Fixture f;
    auto result = f.map(); REQUIRE(result.binding);
    DiagramLifecycleEvents other;
    const auto receiver = other.createReceiver();
    const auto first = other.modelAttached(receiver, true);
    const auto second = other.modelAttached(receiver, true);
    CHECK(first == f.first && second == f.second);
    BooleanTraceOwnerScope otherScope(other);
    rejected(validateBooleanTraceDueForOwner(other, otherScope, *result.binding));
    rejected(validateBooleanTraceDueForOwner(other, f.scope, *result.binding));
    rejected(validateBooleanTraceDueForOwner(f.events, otherScope, *result.binding));
    CHECK(f.events.pending().size() == 2 && other.pending().size() == 2);
    REQUIRE(validateBooleanTraceDueForOwner(f.events, f.scope, *result.binding).observation);
}

TEST_CASE(boolean_trace_due_rebinding_admission_scope_cannot_reuse_an_old_bound_receipt)
{
    Fixture f; auto old = f.map(); REQUIRE(old.binding);
    BooleanTraceOwnerScope replacement(f.events);
    rejected(validateBooleanTraceDueForOwner(f.events, replacement, *old.binding));
    f.context.ownerScope = &replacement;
    auto fresh = f.map(); REQUIRE(fresh.binding);
    REQUIRE(validateBooleanTraceDueForOwner(f.events, replacement, *fresh.binding).observation);
    rejected(validateBooleanTraceDueForOwner(f.events, f.scope, *fresh.binding));
    CHECK(f.events.pending().size() == 2);
    f.context.ownerScope = nullptr; rejected(f.map());
}

TEST_CASE(boolean_trace_due_escaped_receipt_does_not_keep_an_admission_scope_alive)
{
    Fixture f;
    std::optional<BooleanBoundTraceDue> escaped;
    {
        BooleanTraceOwnerScope temporary(f.events);
        f.context.ownerScope = &temporary;
        auto result = f.map(); REQUIRE(result.binding);
        escaped = *result.binding;
        REQUIRE(validateBooleanTraceDueForOwner(f.events, temporary, *escaped).observation);
    }
    f.context.ownerScope = &f.scope;
    rejected(validateBooleanTraceDueForOwner(f.events, f.scope, *escaped));
    CHECK(f.events.pending().size() == 2);
}

TEST_CASE(boolean_trace_due_revalidates_pending_registrations_before_exposing_raw_observation)
{
    Fixture f; auto bound = f.map(); REQUIRE(bound.binding);
    REQUIRE(validateBooleanTraceDueForOwner(f.events, f.scope, *bound.binding).observation);
    unsigned calls = 0;
    CHECK(f.events.deliver(f.first, [&](auto) { ++calls; }) == DiagramLifecycleEvents::Delivery::Delivered);
    rejected(validateBooleanTraceDueForOwner(f.events, f.scope, *bound.binding));
    CHECK(calls == 1 && f.events.pending().size() == 1);
    CHECK(f.events.pending()[0].id == f.second);
}

int main() { return testkit::run_all(); }
