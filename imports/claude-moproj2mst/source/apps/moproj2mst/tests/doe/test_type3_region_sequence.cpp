// ctest-labels: m2m-doe
#include <testkit/check.hpp>

#include <array>

#include "m2m/common.hpp"
#include "../../src/ops/type3_region_sequence.hpp"

using namespace m2m;
using namespace m2m::mst::detail;

TEST_CASE(type3_region_sequence_appends_raw_rows_in_order)
{
    Type3OptimizerScalarState state;
    const std::array<qt::String, 3> rows{
        qt::L("discard#folder/first.ext"),
        qt::L("root\\second.part.opt"),
        qt::L("ignored-extra"),
    };

    const auto result = appendType3RegionRows(state, 2, rows);
    CHECK(result.stop == Type3RegionSequenceStop::CountSatisfied);
    CHECK_EQ(result.rowsConsumed, 2u);
    REQUIRE(state.regionRows.size() == 2u);
    CHECK_EQ(state.regionRows[0].value00, 0);
    CHECK(state.regionRows[0].value08 == qt::L("first"));
    CHECK_EQ(state.regionRows[1].value00, 0);
    CHECK(state.regionRows[1].value08 == qt::L("second.part"));
}

TEST_CASE(type3_region_sequence_zero_and_negative_counts_consume_nothing)
{
    Type3OptimizerScalarState state;
    const std::array<qt::String, 1> rows{qt::L("path/row.ext")};

    const auto zero = appendType3RegionRows(state, 0, rows);
    CHECK(zero.stop == Type3RegionSequenceStop::CountSatisfied);
    CHECK_EQ(zero.rowsConsumed, 0u);
    const auto negative = appendType3RegionRows(state, -1, rows);
    CHECK(negative.stop == Type3RegionSequenceStop::CountSatisfied);
    CHECK_EQ(negative.rowsConsumed, 0u);
    CHECK(state.regionRows.empty());
}

TEST_CASE(type3_region_sequence_preserves_rows_when_supplied_input_ends_early)
{
    Type3OptimizerScalarState state;
    state.value38 = 19;  // Earlier top-level scalar effect is retained.
    const std::array<qt::String, 2> rows{
        qt::L("a/one.dat"),
        qt::L("b/two.dat"),
    };

    const auto result = appendType3RegionRows(state, 3, rows);
    CHECK(result.stop == Type3RegionSequenceStop::InputEnded);
    CHECK_EQ(result.rowsConsumed, 2u);
    CHECK_EQ(state.value38, 19);
    REQUIRE(state.regionRows.size() == 2u);
    CHECK(state.regionRows[0].value08 == qt::L("one"));
    CHECK(state.regionRows[1].value08 == qt::L("two"));
}

TEST_CASE(type3_region_sequence_leaves_following_raw_lines_for_top_level_dispatch)
{
    Type3OptimizerScalarState state;
    const std::array<qt::String, 2> rows{
        qt::L("a/one.dat"),
        qt::L("ENDDATA"),
    };

    const auto result = appendType3RegionRows(state, 1, rows);
    CHECK(result.stop == Type3RegionSequenceStop::CountSatisfied);
    CHECK_EQ(result.rowsConsumed, 1u);
    REQUIRE(state.regionRows.size() == 1u);
    CHECK(state.regionRows[0].value08 == qt::L("one"));
    CHECK(rows[result.rowsConsumed] == qt::L("ENDDATA"));
}

int main() { return testkit::run_all(); }
