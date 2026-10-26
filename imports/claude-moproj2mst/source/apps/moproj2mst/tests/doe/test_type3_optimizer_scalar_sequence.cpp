// ctest-labels: m2m-doe
#include <testkit/check.hpp>

#include <vector>

#include "m2m/common.hpp"
#include "../../src/ops/type3_optimizer_scalar_sequence.hpp"

using namespace m2m;
using namespace m2m::mst::detail;

TEST_CASE(type3_scalar_sequence_applies_lines_in_order_and_ignores_skipped_keys)
{
    Type3OptimizerScalarState state;
    const std::vector<qt::String> lines{
        qt::L(" \tKRG_MAX 4"),
        qt::L("UNKNOWN key"),
        qt::L("DEF_OPT 99"),
        qt::L("KRG_MAX 9"),
        qt::L("OPR_NO 17"),
    };

    const auto result = applyType3OptimizerScalarLines(state, lines);
    CHECK(result.stop == Type3ScalarSequenceStop::AllLinesConsumed);
    CHECK_EQ(result.nextLineIndex, lines.size());
    CHECK_EQ(state.value0c, 9);
    CHECK_EQ(state.value38, 17);
    CHECK_EQ(state.value10, 1);
    CHECK_EQ(state.value30, 0x3fd);
}

TEST_CASE(type3_scalar_sequence_stops_after_enddata_before_later_mutation)
{
    Type3OptimizerScalarState state;
    const std::vector<qt::String> lines{
        qt::L("KRG_MAX 6"),
        qt::L("ENDDATA"),
        qt::L("KRG_MAX 12"),
    };

    const auto result = applyType3OptimizerScalarLines(state, lines);
    CHECK(result.stop == Type3ScalarSequenceStop::EndData);
    CHECK_EQ(result.nextLineIndex, 2u);
    CHECK_EQ(state.value0c, 6);
}

TEST_CASE(type3_scalar_sequence_keeps_prior_mutation_when_regions_is_guarded)
{
    Type3OptimizerScalarState state;
    const std::vector<qt::String> lines{
        qt::L("OPR_NO 23"),
        qt::L("REGIONS 1"),
        qt::L("KRG_MAX 99"),
    };

    const auto result = applyType3OptimizerScalarLines(state, lines);
    CHECK(result.stop == Type3ScalarSequenceStop::UnsupportedRowBody);
    CHECK_EQ(result.nextLineIndex, 1u);
    CHECK_EQ(state.value38, 23);
    CHECK_EQ(state.value0c, 1);
    CHECK(state.regionRows.empty());
}

TEST_CASE(type3_scalar_sequence_reports_surrogate_guard_after_prior_effect)
{
    Type3OptimizerScalarState state;
    const qt::String supplementary(std::u16string{u'K', u'R', u'G', u'_', u'M', u'A', u'X', u' ', 0xd83d, 0xdca9});
    const std::vector<qt::String> lines{qt::L("KRG_MAX 8"), supplementary};

    const auto result = applyType3OptimizerScalarLines(state, lines);
    CHECK(result.stop == Type3ScalarSequenceStop::UnsupportedSurrogateLine);
    CHECK_EQ(result.nextLineIndex, 1u);
    CHECK_EQ(state.value0c, 8);
}

int main() { return testkit::run_all(); }
