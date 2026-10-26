// ctest-labels: m2m-doe
#include <testkit/check.hpp>

#include <array>
#include <vector>

#include "../../src/doe/frcrot_pair_prefix.hpp"

using namespace m2m::doe;

TEST_CASE(frcrot_mode_failure_keeps_forward_pair_without_table_or_reverse_effects)
{
    FrcrotPairPrefixInput input;
    input.master = 3;
    input.slave = 7;
    input.modeLineSucceeded = false;
    const auto result = applyFrcrotPairPrefix(input);
    const std::vector<FrcrotPairIdentity> expected{{3, 7}};
    CHECK(result.pairs == expected);
    CHECK(result.status == FrcrotPairPrefixStatus::ModeLineFailure);
    CHECK(!result.forwardTable.resetReached);
    const std::vector<FrcrotPairEvent> expectedEvents{
        FrcrotPairEvent::ForwardAppended, FrcrotPairEvent::ModeLineFailed};
    CHECK(result.events == expectedEvents);
}

TEST_CASE(frcrot_existing_forward_pair_is_not_duplicated_before_mode_failure)
{
    FrcrotPairPrefixInput input;
    input.existingPairs = {{9, 2}, {3, 7}};
    input.master = 3;
    input.slave = 7;
    const auto result = applyFrcrotPairPrefix(input);
    CHECK(result.pairs == input.existingPairs);
    CHECK(result.events.front() == FrcrotPairEvent::ForwardFound);
    CHECK(result.status == FrcrotPairPrefixStatus::ModeLineFailure);
}

TEST_CASE(frcrot_mode_zero_field_prefix_survives_table_reader_failure_and_mirror_tail)
{
    FrcrotPairPrefixInput input;
    input.master = 3;
    input.slave = 7;
    input.modeLineSucceeded = true;
    input.mode = 0;
    input.constantValue = 12.5;
    input.tableReadSucceeded = false;
    const auto result = applyFrcrotPairPrefix(input);
    const std::vector<FrcrotPairIdentity> expected{{3, 7}, {7, 3}};
    CHECK(result.pairs == expected);
    CHECK(result.forwardTable.resetReached);
    CHECK(result.forwardTable.dimensionWrite == 0);
    CHECK(result.forwardTable.constantWrite == 12.5);
    CHECK(!result.forwardTable.integerValueWrite.has_value());
    CHECK(!result.forwardTable.codeWrite.has_value());
    CHECK(result.status == FrcrotPairPrefixStatus::TableReadFailure);
    CHECK(result.events[6] == FrcrotPairEvent::ReverseAppended);
    CHECK(result.events.back() == FrcrotPairEvent::MirrorCopyReached);
}

int main() { return testkit::run_all(); }

TEST_CASE(frcrot_recognized_variable_modes_store_distinct_codes_and_integer_values)
{
    for (const auto [mode, code] : {std::array<int, 2>{1, 1}, std::array<int, 2>{2, 4}}) {
        FrcrotPairPrefixInput input;
        input.master = 1;
        input.slave = 2;
        input.modeLineSucceeded = true;
        input.mode = mode;
        input.integerValue = -8;
        input.tableReadSucceeded = true;
        const auto result = applyFrcrotPairPrefix(input);
        CHECK(result.forwardTable.resetReached);
        CHECK(result.forwardTable.dimensionWrite == 1);
        CHECK(result.forwardTable.codeWrite == code);
        CHECK(result.forwardTable.integerValueWrite == -8);
        CHECK(result.status == FrcrotPairPrefixStatus::TableReadSuccess);
    }
}

TEST_CASE(frcrot_other_mode_retains_reset_only_then_still_reaches_common_tail)
{
    FrcrotPairPrefixInput input;
    input.master = 1;
    input.slave = 1;
    input.modeLineSucceeded = true;
    input.mode = 9;
    input.tableReadSucceeded = true;
    const auto result = applyFrcrotPairPrefix(input);
    CHECK(result.forwardTable.resetReached);
    CHECK(!result.forwardTable.dimensionWrite.has_value());
    CHECK(!result.forwardTable.codeWrite.has_value());
    CHECK(!result.forwardTable.constantWrite.has_value());
    CHECK(!result.forwardTable.integerValueWrite.has_value());
    CHECK(result.events.back() == FrcrotPairEvent::SelfPairSkippedReverse);
    CHECK(result.status == FrcrotPairPrefixStatus::TableReadSuccess);
}

TEST_CASE(frcrot_existing_reverse_pair_preserves_input_order_and_is_not_duplicated)
{
    FrcrotPairPrefixInput input;
    input.existingPairs = {{8, 9}, {7, 3}, {1, 5}};
    input.master = 3;
    input.slave = 7;
    input.modeLineSucceeded = true;
    input.mode = 2;
    input.integerValue = 4;
    input.tableReadSucceeded = true;
    const auto result = applyFrcrotPairPrefix(input);
    const std::vector<FrcrotPairIdentity> expected{{8, 9}, {7, 3}, {1, 5}, {3, 7}};
    CHECK(result.pairs == expected);
    CHECK(result.events[0] == FrcrotPairEvent::ForwardAppended);
    CHECK(result.events[7] == FrcrotPairEvent::ReverseFound);
    CHECK(result.events.back() == FrcrotPairEvent::MirrorCopyReached);
}
