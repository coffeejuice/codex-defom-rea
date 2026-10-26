// ctest-labels: m2m-mdt
#include <testkit/check.hpp>
#include <algorithm>
#include <limits>
#include <memory>
#include "m2m/engine/paint_device_cleanup.hpp"

using namespace m2m::engine;
namespace {
using Stage = PaintDeviceCleanupStage;
using Issue = PaintDeviceCleanupIssue;
using Effect = PaintDeviceCleanupEffect;
constexpr auto qualification = "Isolated actual QtGui cleanup globals";
struct Fixture {
    std::shared_ptr<PaintDeviceCleanupGlobals> globals = std::make_shared<PaintDeviceCleanupGlobals>(qualification);
    PaintDeviceCleanupCall call{globals};
    PaintDeviceCleanupGlobalObservation observation() const {
        PaintDeviceCleanupGlobalObservation o; o.receiver = globals; o.qualification = qualification;
        o.moduleSha256 = "c360c971d292a3c84fc59b8e38733a85a111243f1dbc44737d87b6d744725887";
        o.pointerRva = 0x9cb858;
        return o;
    }
    void existing(std::optional<std::int32_t> value) {
        auto o = observation(); o.counters = {{31, value}}; o.currentCounterAlias = 31;
        REQUIRE(globals->observe(o));
    }
    std::size_t count(Effect effect) const {
        return static_cast<std::size_t>(std::count(call.effects().begin(), call.effects().end(), effect));
    }
};
}

TEST_CASE(paint_cleanup_missing_or_unqualified_globals_do_not_read_a_pointer)
{
    PaintDeviceCleanupCall missing{nullptr};
    CHECK(missing.resume().issue == Issue::GlobalsUnavailable); CHECK(missing.effects().empty());
    Fixture f;
    CHECK(f.call.resume().issue == Issue::GlobalQualificationUnavailable);
    CHECK(f.call.effects().empty()); CHECK(f.globals->nativeOwnerAlive());
    REQUIRE(f.globals->observe(f.observation()));
    CHECK(f.call.resume().issue == Issue::CounterPointerUnavailable);
    CHECK(f.call.effects().empty()); CHECK(!f.call.counterOwner());
    CHECK(!f.call.report().completeForPublicSave);
}

TEST_CASE(paint_cleanup_known_null_pointer_preserves_lazy_state_guard)
{
    Fixture f; auto o = f.observation(); o.currentCounterAlias = 0;
    REQUIRE(f.globals->observe(o));
    CHECK(f.call.resume().issue == Issue::LazyGlobalUnrecovered);
    CHECK(f.call.report().stage == Stage::FirstGetter); CHECK(f.call.effects().empty());
    CHECK(f.call.resume().issue == Issue::LazyGlobalUnrecovered); CHECK(f.call.effects().empty());
    CHECK(!f.call.counterOwner());
    o = f.observation(); o.counters = {{31, 0}}; o.currentCounterAlias = 31;
    CHECK(!f.globals->observe(o)); CHECK(!f.globals->counterFor(31));
    CHECK(f.call.resume().issue == Issue::LazyGlobalUnrecovered);
}

TEST_CASE(paint_cleanup_existing_zero_counter_returns_in_original_read_order)
{
    Fixture f; f.existing(0);
    const auto owner = f.globals->counterFor(31); REQUIRE(owner);
    CHECK(f.call.resume().stage == Stage::Returned); CHECK(f.call.report().issue == Issue::None);
    const std::vector expected{Effect::FirstGetterReturned, Effect::SecondGetterReturned, Effect::CounterValueRead};
    CHECK(f.call.effects() == expected); CHECK(f.call.counterOwner() == owner);
    CHECK(owner->value() == 0); CHECK(f.globals->counterFor(31) == owner);
    const auto effects = f.call.effects();
    CHECK(f.call.resume().stage == Stage::Returned); CHECK(f.call.effects() == effects);
    CHECK(owner->value() == 0); CHECK(f.globals->nativeOwnerAlive());
}

TEST_CASE(paint_cleanup_counter_value_remains_unknown_after_both_borrowed_getters)
{
    Fixture f; f.existing(std::nullopt);
    const auto owner = f.globals->counterFor(31); REQUIRE(owner); CHECK(!owner->value());
    CHECK(f.call.resume().issue == Issue::CounterValueUnavailable);
    CHECK(f.call.report().stage == Stage::CounterComparison);
    CHECK(f.count(Effect::FirstGetterReturned) == 1u); CHECK(f.count(Effect::SecondGetterReturned) == 1u);
    CHECK(f.count(Effect::CounterValueRead) == 0u); CHECK(f.call.counterOwner() == owner);
    const auto effects = f.call.effects();
    CHECK(f.call.resume().issue == Issue::CounterValueUnavailable); CHECK(f.call.effects() == effects);
    f.existing(0); CHECK(f.call.effects() == effects); CHECK(owner->value() == 0);
    CHECK(f.call.resume().stage == Stage::Returned);
    CHECK(f.count(Effect::FirstGetterReturned) == 1u); CHECK(f.count(Effect::SecondGetterReturned) == 1u);
    CHECK(f.count(Effect::CounterValueRead) == 1u); CHECK(f.call.counterOwner() == owner);
}

TEST_CASE(paint_cleanup_unknown_pointer_can_later_bind_without_a_fabricated_null)
{
    Fixture f; auto o = f.observation(); o.counters = {{31, 0}};
    REQUIRE(f.globals->observe(o));
    const auto owner = f.globals->counterFor(31); REQUIRE(owner);
    CHECK(f.call.resume().issue == Issue::CounterPointerUnavailable); CHECK(f.call.effects().empty());
    o = f.observation(); o.currentCounterAlias = 31;
    REQUIRE(f.globals->observe(o)); CHECK(f.call.effects().empty());
    CHECK(f.call.resume().stage == Stage::Returned); CHECK(f.call.counterOwner() == owner);
}

TEST_CASE(paint_cleanup_every_nonzero_counter_value_retains_mutex_list_continuation)
{
    for (std::int32_t n : {-1, 1, std::numeric_limits<std::int32_t>::min(), std::numeric_limits<std::int32_t>::max()}) {
        Fixture f; f.existing(n); const auto owner = f.globals->counterFor(31); REQUIRE(owner);
        CHECK(f.call.resume().issue == Issue::NonzeroCleanupUnrecovered);
        CHECK(f.call.report().stage == Stage::NonzeroContinuation); CHECK(owner->value() == n);
        const auto effects = f.call.effects();
        CHECK(f.call.resume().issue == Issue::NonzeroCleanupUnrecovered); CHECK(f.call.effects() == effects);
        CHECK(f.count(Effect::CounterValueRead) == 1u); CHECK(owner->value() == n);
        auto o = f.observation(); o.counters = {{31, 0}};
        CHECK(!f.globals->observe(o)); CHECK(owner->value() == n);
    }
}

TEST_CASE(paint_cleanup_module_field_and_qualification_are_explicit_observation_requirements)
{
    Fixture f;
    for (int changed = 0; changed != 5; ++changed) {
        auto o = f.observation(); o.counters = {{31, 0}}; o.currentCounterAlias = 31;
        if (changed == 0) o.qualification.clear();
        if (changed == 1) o.qualification = "Other stage";
        if (changed == 2) o.moduleSha256 = "Other DLL";
        if (changed == 3) o.pointerRva = 0;
        if (changed == 4) o.pointerRva.reset();
        CHECK(!f.globals->observe(o)); CHECK(!f.globals->qualified()); CHECK(!f.globals->counterFor(31));
    }
    CHECK(f.call.resume().issue == Issue::GlobalQualificationUnavailable); CHECK(f.call.effects().empty());
}

TEST_CASE(paint_cleanup_foreign_and_rewrapped_global_owners_cannot_receive_observations)
{
    Fixture f, other;
    auto o = f.observation(); o.receiver = other.globals; o.counters = {{31, 0}}; o.currentCounterAlias = 31;
    CHECK(!f.globals->observe(o)); CHECK(!other.globals->qualified());
    std::shared_ptr<PaintDeviceCleanupGlobals> foreign(f.globals.get(), [](PaintDeviceCleanupGlobals*) {});
    o = f.observation(); o.receiver = foreign;
    CHECK(!f.globals->observe(o)); CHECK(!f.globals->qualified());
    o = f.observation(); o.receiver.reset(); CHECK(!f.globals->observe(o));
    CHECK(f.call.effects().empty()); CHECK(f.call.resume().issue == Issue::GlobalQualificationUnavailable);
}

TEST_CASE(paint_cleanup_late_bad_pointer_does_not_refine_existing_or_publish_new_counter_data)
{
    Fixture f; auto o = f.observation(); o.counters = {{31, std::nullopt}};
    REQUIRE(f.globals->observe(o)); const auto owner = f.globals->counterFor(31); REQUIRE(owner);
    o = f.observation(); o.counters = {{31, 0}, {32, 0}}; o.currentCounterAlias = 99;
    CHECK(!f.globals->observe(o)); CHECK(!owner->value()); CHECK(!f.globals->counterFor(32));
    CHECK(f.call.resume().issue == Issue::CounterPointerUnavailable); CHECK(f.call.effects().empty());
    o = f.observation(); o.counters = {{31, 1}, {32, 1}}; o.currentCounterAlias = 31;
    REQUIRE(f.globals->observe(o)); CHECK(f.globals->counterFor(31) == owner); CHECK(owner->value() == 1);
}

TEST_CASE(paint_cleanup_duplicate_aliases_and_pointer_replacement_leave_old_state_unchanged)
{
    Fixture f; f.existing(std::nullopt); const auto owner = f.globals->counterFor(31); REQUIRE(owner);
    auto o = f.observation(); o.counters = {{31, 0}, {31, 0}};
    CHECK(!f.globals->observe(o)); CHECK(!owner->value());
    o = f.observation(); o.counters = {{31, 0}, {0, 0}};
    CHECK(!f.globals->observe(o)); CHECK(!owner->value());
    o = f.observation(); o.counters = {{31, 0}, {32, 0}}; o.currentCounterAlias = 32;
    CHECK(!f.globals->observe(o)); CHECK(!owner->value()); CHECK(!f.globals->counterFor(32));
    o = f.observation(); o.currentCounterAlias = 0;
    CHECK(!f.globals->observe(o)); CHECK(f.call.resume().issue == Issue::CounterValueUnavailable);
    CHECK(f.call.counterOwner() == owner);
}

TEST_CASE(paint_cleanup_actual_calls_share_one_global_owner_but_keep_separate_continuations)
{
    Fixture f; f.existing(std::nullopt); PaintDeviceCleanupCall second{f.globals};
    CHECK(f.call.resume().issue == Issue::CounterValueUnavailable);
    CHECK(second.resume().issue == Issue::CounterValueUnavailable);
    CHECK(f.call.counterOwner() == second.counterOwner());
    const auto secondEffects = second.effects();
    f.existing(0); CHECK(second.effects() == secondEffects);
    CHECK(f.call.resume().stage == Stage::Returned);
    CHECK(second.report().stage == Stage::CounterComparison); CHECK(second.effects() == secondEffects);
    CHECK(second.resume().stage == Stage::Returned);
    CHECK(f.count(Effect::CounterValueRead) == 1u);
    CHECK(std::count(second.effects().begin(), second.effects().end(), Effect::CounterValueRead) == 1);
    CHECK(f.globals->counterFor(31)->value() == 0);
}

TEST_CASE(paint_cleanup_same_numeric_alias_in_another_runtime_is_a_different_owner)
{
    Fixture first, second; first.existing(0); second.existing(0);
    CHECK(first.globals->counterFor(31) != second.globals->counterFor(31));
    CHECK(first.call.resume().stage == Stage::Returned); CHECK(second.call.resume().stage == Stage::Returned);
    CHECK(first.call.counterOwner() != second.call.counterOwner());
    first.globals->releaseNativeOwner();
    CHECK(second.globals->nativeOwnerAlive()); CHECK(second.globals->counterFor(31)->value() == 0);
}

TEST_CASE(paint_cleanup_released_runtime_stops_a_pending_call_without_counter_mutation)
{
    Fixture f; f.existing(std::nullopt); const auto owner = f.globals->counterFor(31); REQUIRE(owner);
    CHECK(f.call.resume().issue == Issue::CounterValueUnavailable); const auto effects = f.call.effects();
    f.globals->releaseNativeOwner();
    auto o = f.observation(); o.counters = {{31, 0}};
    CHECK(!f.globals->observe(o)); CHECK(!owner->value()); CHECK(!f.globals->counterFor(31));
    CHECK(f.call.resume().issue == Issue::GlobalsUnavailable); CHECK(f.call.effects() == effects);
}

TEST_CASE(paint_cleanup_diagnostic_counter_lease_cannot_extend_actual_module_lifetime)
{
    std::unique_ptr<PaintDeviceCleanupCall> call;
    std::shared_ptr<const PaintDeviceCleanupCounterState> escaped;
    {
        Fixture f; f.existing(0); escaped = f.globals->counterFor(31);
        call = std::make_unique<PaintDeviceCleanupCall>(f.globals);
    }
    REQUIRE(escaped); CHECK(escaped->value() == 0);
    CHECK(call->resume().issue == Issue::GlobalsUnavailable); CHECK(call->effects().empty());
}

int main() { return testkit::run_all(); }
