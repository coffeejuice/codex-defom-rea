// ctest-labels: m2m-mdt close-main-busy-decision
#include <testkit/check.hpp>

#include <cstdint>
#include <limits>
#include <optional>

#include "m2m/engine/close_main_busy_decision.hpp"

using namespace m2m::engine;

static_assert(noexcept(evaluateCloseMainBusyDecision(std::nullopt, std::nullopt)));
static_assert(noexcept(evaluateCloseMainBusyDecision({})));
static_assert(noexcept(evaluateCloseMainBusyDecision(
    CloseMainBusyPhase::Unknown, std::nullopt, std::nullopt)));

TEST_CASE(close_main_busy_missing_first_read_cannot_use_later_busy_mode)
{
    CHECK(evaluateCloseMainBusyDecision(std::nullopt) == CloseMainBusyDecision::NeedsCurrentIndex);
    CHECK(evaluateCloseMainBusyDecision(std::nullopt, 1u) == CloseMainBusyDecision::NeedsCurrentIndex);
    CHECK(evaluateCloseMainBusyDecision(std::nullopt, 0u) == CloseMainBusyDecision::NeedsCurrentIndex);
}

TEST_CASE(close_main_busy_stack_page_two_short_circuits_missing_mode)
{
    CHECK(evaluateCloseMainBusyDecision(2) == CloseMainBusyDecision::Busy);
    CHECK(evaluateCloseMainBusyDecision(2, 0u) == CloseMainBusyDecision::Busy);
    CHECK(evaluateCloseMainBusyDecision(2, 1u) == CloseMainBusyDecision::Busy);
    CHECK(evaluateCloseMainBusyDecision(2, std::numeric_limits<std::uint32_t>::max()) ==
          CloseMainBusyDecision::Busy);
}

TEST_CASE(close_main_busy_every_non_two_stack_value_still_needs_mode)
{
    const std::int32_t indexes[] = {
        std::numeric_limits<std::int32_t>::min(), -1, 0, 1, 3,
        std::numeric_limits<std::int32_t>::max()
    };
    for (const auto index : indexes)
        CHECK(evaluateCloseMainBusyDecision(index) == CloseMainBusyDecision::NeedsMode270);
}

TEST_CASE(close_main_busy_uses_exact_integer_states_instead_of_boolean_coercion)
{
    struct Case {
        std::int32_t index;
        std::uint32_t mode;
        CloseMainBusyDecision expected;
    };
    const Case cases[] = {
        {-1, 0u, CloseMainBusyDecision::NotBusy},
        {0, 0u, CloseMainBusyDecision::NotBusy},
        {1, 0u, CloseMainBusyDecision::NotBusy},
        {3, 2u, CloseMainBusyDecision::NotBusy},
        {0, 0xffffffffu, CloseMainBusyDecision::NotBusy},
        {0, 0x80000000u, CloseMainBusyDecision::NotBusy},
        {-1, 1u, CloseMainBusyDecision::Busy},
        {0, 1u, CloseMainBusyDecision::Busy},
        {1, 1u, CloseMainBusyDecision::Busy},
        {3, 1u, CloseMainBusyDecision::Busy},
        {std::numeric_limits<std::int32_t>::max(), 0u, CloseMainBusyDecision::NotBusy},
        {std::numeric_limits<std::int32_t>::min(), 1u, CloseMainBusyDecision::Busy}
    };
    for (const auto& value : cases)
        CHECK(evaluateCloseMainBusyDecision(value.index, value.mode) == value.expected);
}

TEST_CASE(close_main_busy_next_attempt_cannot_reuse_previous_not_busy_decision)
{
    // Declared values for separate pure evaluations, not observed Qt owners.
    CHECK(evaluateCloseMainBusyDecision(0, 0u) == CloseMainBusyDecision::NotBusy);
    CHECK(evaluateCloseMainBusyDecision(std::nullopt) == CloseMainBusyDecision::NeedsCurrentIndex);
    CHECK(evaluateCloseMainBusyDecision(0) == CloseMainBusyDecision::NeedsMode270);
    CHECK(evaluateCloseMainBusyDecision(0, 1u) == CloseMainBusyDecision::Busy);
}

TEST_CASE(close_main_busy_unknown_phase_precedes_supplied_widget_values)
{
    CHECK(evaluateCloseMainBusyDecision(CloseMainBusyPhase::Unknown, std::nullopt, std::nullopt) ==
          CloseMainBusyDecision::NeedsMainPhase);
    CHECK(evaluateCloseMainBusyDecision(CloseMainBusyPhase::Unknown, 2, 1u) ==
          CloseMainBusyDecision::NeedsMainPhase);
    CHECK(evaluateCloseMainBusyDecision(static_cast<CloseMainBusyPhase>(99), 0, 0u) ==
          CloseMainBusyDecision::NeedsMainPhase);
    // The original API still has its derived-only interpretation.
    CHECK(evaluateCloseMainBusyDecision({}) == CloseMainBusyDecision::NeedsCurrentIndex);
}

TEST_CASE(close_main_busy_base_construction_needs_no_widget_or_mode_input)
{
    CHECK(evaluateCloseMainBusyDecision(CloseMainBusyPhase::SfMainWindowBaseConstruction,
          std::nullopt, std::nullopt) == CloseMainBusyDecision::NotBusy);
    CHECK(evaluateCloseMainBusyDecision(CloseMainBusyPhase::SfMainWindowBaseConstruction,
          2, 1u) == CloseMainBusyDecision::NotBusy);
}

TEST_CASE(close_main_busy_derived_phase_preserves_order_after_base_evaluation)
{
    CHECK(evaluateCloseMainBusyDecision(CloseMainBusyPhase::SfMainWindowBaseConstruction,
          std::nullopt, std::nullopt) == CloseMainBusyDecision::NotBusy);
    // c99530 installs no table. Its use on an already derived main cannot
    // authorize the base leaf or fabricate the missing actual stack input.
    CHECK(evaluateCloseMainBusyDecision(CloseMainBusyPhase::SfMoMainWindowDerived,
          std::nullopt, 1u) == CloseMainBusyDecision::NeedsCurrentIndex);
    CHECK(evaluateCloseMainBusyDecision(CloseMainBusyPhase::SfMoMainWindowDerived,
          2, std::nullopt) == CloseMainBusyDecision::Busy);
    CHECK(evaluateCloseMainBusyDecision(CloseMainBusyPhase::SfMoMainWindowDerived,
          0, std::nullopt) == CloseMainBusyDecision::NeedsMode270);
    CHECK(evaluateCloseMainBusyDecision(CloseMainBusyPhase::SfMoMainWindowDerived,
          0, 1u) == CloseMainBusyDecision::Busy);
    CHECK(evaluateCloseMainBusyDecision(CloseMainBusyPhase::SfMoMainWindowDerived,
          0, 2u) == CloseMainBusyDecision::NotBusy);
}

TEST_CASE(close_main_busy_later_current_main_cannot_borrow_constructor_main_phase)
{
    // Explicit independent mathematical inputs, not constructed native owners.
    const auto constructorMainPhase = CloseMainBusyPhase::SfMainWindowBaseConstruction;
    const auto currentMainPhaseUnavailable = CloseMainBusyPhase::Unknown;
    CHECK(evaluateCloseMainBusyDecision(constructorMainPhase, std::nullopt, std::nullopt) ==
          CloseMainBusyDecision::NotBusy);
    CHECK(evaluateCloseMainBusyDecision(currentMainPhaseUnavailable, std::nullopt, std::nullopt) ==
          CloseMainBusyDecision::NeedsMainPhase);
    CHECK(evaluateCloseMainBusyDecision(CloseMainBusyPhase::SfMoMainWindowDerived,
          std::nullopt, std::nullopt) == CloseMainBusyDecision::NeedsCurrentIndex);
    CHECK(evaluateCloseMainBusyDecision(CloseMainBusyPhase::SfMoMainWindowDerived,
          2, std::nullopt) == CloseMainBusyDecision::Busy);
    CHECK(evaluateCloseMainBusyDecision(currentMainPhaseUnavailable, 2, 1u) ==
          CloseMainBusyDecision::NeedsMainPhase);
}

int main() { return testkit::run_all(); }
