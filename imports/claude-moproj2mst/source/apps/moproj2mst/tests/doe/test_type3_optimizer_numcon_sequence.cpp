// ctest-labels: m2m-doe
#include <testkit/check.hpp>

#include <vector>

#include "m2m/common.hpp"
#include "../../src/ops/type3_optimizer_scalar_sequence.hpp"

using namespace m2m;
using namespace m2m::mst::detail;

TEST_CASE(type3_numcon_sequence_appends_complete_row_with_empty_native_string)
{
    Type3OptimizerScalarState state;
    const std::vector<qt::String> lines{
        qt::L("NUM_CON 1"),
        qt::L("ignored 1021 33 2.5 44"),
        qt::L("ENDDATA"),
        qt::L("KRG_MAX 12"),
    };

    const auto result = applyType3OptimizerScalarLines(state, lines);
    CHECK(result.stop == Type3ScalarSequenceStop::EndData);
    CHECK_EQ(result.nextLineIndex, 3u);
    REQUIRE(state.numConRows.size() == 1u);
    const auto& row = state.numConRows[0];
    CHECK_EQ(row.value00, 1);
    CHECK_EQ(row.value04, 1021);
    CHECK_EQ(row.value08, 33);
    CHECK_EQ(row.value0c, 44);
    CHECK_BITS(row.value18, 2.5);
    CHECK(row.value20.isEmpty());
    CHECK_EQ(state.value0c, 1);
}

TEST_CASE(type3_numcon_sequence_keeps_complete_rows_when_input_ends_early)
{
    Type3OptimizerScalarState state;
    const std::vector<qt::String> lines{
        qt::L("NUM_CON 3"),
        qt::L("row-a 1021 11 1.25 21"),
        qt::L("row-b 1022 12 2.5 22"),
    };

    const auto result = applyType3OptimizerScalarLines(state, lines);
    CHECK(result.stop == Type3ScalarSequenceStop::AllLinesConsumed);
    CHECK_EQ(result.nextLineIndex, lines.size());
    REQUIRE(state.numConRows.size() == 2u);
    CHECK_EQ(state.numConRows[0].value04, 1021);
    CHECK_EQ(state.numConRows[1].value04, 1022);
    CHECK(state.numConRows[0].value20.isEmpty());
    CHECK(state.numConRows[1].value20.isEmpty());
}

TEST_CASE(type3_numcon_sequence_preserves_prior_scalar_and_rows_at_bad_later_row)
{
    Type3OptimizerScalarState state;
    const std::vector<qt::String> lines{
        qt::L("OPR_NO 7"),
        qt::L("NUM_CON 2"),
        qt::L("row-a 1021 11 1.25 21"),
        qt::L("row-b 1022 bad 2.5 22"),
        qt::L("ENDDATA"),
    };

    const auto result = applyType3OptimizerScalarLines(state, lines);
    CHECK(result.stop == Type3ScalarSequenceStop::UnsupportedRowBody);
    CHECK_EQ(result.nextLineIndex, 3u);
    CHECK_EQ(state.value38, 7);
    REQUIRE(state.numConRows.size() == 1u);
    CHECK_EQ(state.numConRows[0].value04, 1021);
    CHECK(state.numConRows[0].value20.isEmpty());
}

TEST_CASE(type3_numcon_sequence_keeps_regions_guarded_after_prior_row)
{
    Type3OptimizerScalarState state;
    const std::vector<qt::String> lines{
        qt::L("NUM_CON 1"),
        qt::L("row-a 1021 11 1.25 21"),
        qt::L("REGIONS 1"),
        qt::L("region-value"),
    };

    const auto result = applyType3OptimizerScalarLines(state, lines);
    CHECK(result.stop == Type3ScalarSequenceStop::UnsupportedRowBody);
    CHECK_EQ(result.nextLineIndex, 2u);
    REQUIRE(state.numConRows.size() == 1u);
    CHECK(state.numConRows[0].value20.isEmpty());
    CHECK(state.regionRows.empty());
}

int main() { return testkit::run_all(); }
