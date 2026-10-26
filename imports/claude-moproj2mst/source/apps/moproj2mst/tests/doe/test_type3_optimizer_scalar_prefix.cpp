// ctest-labels: m2m-doe
#include <testkit/check.hpp>

#include <array>
#include <initializer_list>
#include <limits>

#include <vector>

#include "m2m/common.hpp"
#include "../../src/ops/type3_optimizer_scalar_state.hpp"

using namespace m2m;
using namespace m2m::mst::detail;

namespace {
std::vector<qt::String> fields(std::initializer_list<const char*> values)
{
    std::vector<qt::String> result;
    result.reserve(values.size());
    for (const char* value : values) result.push_back(qt::L(value));
    return result;
}

void checkDefaults(const Type3OptimizerScalarState& state)
{
    CHECK_EQ(state.value0c, 1);
    CHECK_EQ(state.value10, 1);
    CHECK_EQ(state.value30, 0x3fd);
    CHECK_EQ(state.value34, -1);
    CHECK_EQ(state.value38, 200);
    CHECK_EQ(state.value3c, 0);
    CHECK_BITS(state.value40, 0.001);
    CHECK_EQ(state.value48, false);
}
}  // namespace

TEST_CASE(type3_optimizer_scalar_constructor_defaults)
{
    checkDefaults(Type3OptimizerScalarState{});
}

TEST_CASE(type3_line_tokenizer_skips_empty_ascii_whitespace_fields)
{
    std::vector<qt::String> tokens{qt::L("stale")};
    CHECK(tokenizeType3OptimizerLine(qt::L(" \tOPR_NO\r\n17 \v"), tokens) == Type3LineTokenizeResult::Tokenized);
    CHECK_EQ(tokens.size(), 2u);
    CHECK(tokens[0] == qt::L("OPR_NO"));
    CHECK(tokens[1] == qt::L("17"));
}

TEST_CASE(type3_line_tokenizer_preserves_shipped_unicode_whitespace_boundaries)
{
    std::vector<qt::String> tokens;
    const qt::String input(std::u16string{u'A', 0x0085, u'B', u' ', u'C', 0x180e, u'D', 0x2028, u'E', 0x2029});
    CHECK(tokenizeType3OptimizerLine(input, tokens) == Type3LineTokenizeResult::Tokenized);
    CHECK_EQ(tokens.size(), 4u);
    CHECK(tokens[0] == qt::String(std::u16string{u'A', 0x0085, u'B'}));
    CHECK(tokens[1] == qt::L("C"));
    CHECK(tokens[2] == qt::L("D"));
    CHECK(tokens[3] == qt::L("E"));
}

TEST_CASE(type3_line_tokenizer_empty_lines_produce_no_fields)
{
    std::vector<qt::String> tokens{qt::L("stale")};
    CHECK(tokenizeType3OptimizerLine(qt::String{}, tokens) == Type3LineTokenizeResult::Tokenized);
    CHECK(tokens.empty());
    const qt::String allWhitespace(std::u16string{u' ', u'\t', 0x3000, u'\r'});
    CHECK(tokenizeType3OptimizerLine(allWhitespace, tokens) == Type3LineTokenizeResult::Tokenized);
    CHECK(tokens.empty());
}

TEST_CASE(type3_line_tokenizer_rejects_surrogate_profile_and_clears_partial_output)
{
    std::vector<qt::String> tokens{qt::L("stale")};
    const qt::String supplementary(std::u16string{u'o', u'k', u' ', 0xd83d, 0xdca9, u' ', u'x'});
    CHECK(tokenizeType3OptimizerLine(supplementary, tokens) == Type3LineTokenizeResult::UnsupportedSurrogateInput);
    CHECK(tokens.empty());
}

TEST_CASE(type3_optimizer_scalar_keys_map_to_recovered_fields)
{
    Type3OptimizerScalarState state;
    CHECK(applyType3OptimizerScalarLine(state, fields({"OPR_NO", "17"})) == Type3ScalarLineResult::Applied);
    CHECK_EQ(state.value38, 17);
    CHECK_EQ(state.value0c, 1);
    CHECK(applyType3OptimizerScalarLine(state, fields({"KRG_MAX", "9"})) == Type3ScalarLineResult::Applied);
    CHECK_EQ(state.value0c, 9);
    CHECK_EQ(state.value10, 1);
    CHECK(applyType3OptimizerScalarLine(state, fields({"TYP_OBJ", "10", "11", "12"})) == Type3ScalarLineResult::Applied);
    CHECK_EQ(state.value30, 10);
    CHECK_EQ(state.value10, 11);
    CHECK_EQ(state.value34, 12);
    CHECK_EQ(state.value3c, 0);
    CHECK(applyType3OptimizerScalarLine(state, fields({"EPS_OBJ", "0.125"})) == Type3ScalarLineResult::Applied);
    CHECK_BITS(state.value40, 0.125);
}

TEST_CASE(db_purge_maps_missing_and_numeric_token_values)
{
    Type3OptimizerScalarState state;
    state.value48 = true;
    CHECK(applyType3OptimizerScalarLine(state, fields({"DB_PURGE"})) == Type3ScalarLineResult::Applied);
    CHECK_EQ(state.value48, false);
    CHECK(applyType3OptimizerScalarLine(state, fields({"DB_PURGE", "1"})) == Type3ScalarLineResult::Applied);
    CHECK_EQ(state.value48, false);
    CHECK(applyType3OptimizerScalarLine(state, fields({"DB_PURGE", "0"})) == Type3ScalarLineResult::Applied);
    CHECK_EQ(state.value48, true);
}

TEST_CASE(later_scalar_lines_overwrite_earlier_values)
{
    Type3OptimizerScalarState state;
    CHECK(applyType3OptimizerScalarLine(state, fields({"KRG_MAX", "3"})) == Type3ScalarLineResult::Applied);
    CHECK(applyType3OptimizerScalarLine(state, fields({"KRG_MAX", "8"})) == Type3ScalarLineResult::Applied);
    CHECK_EQ(state.value0c, 8);
}

TEST_CASE(skipped_and_unknown_keys_do_not_alter_scalar_state)
{
    Type3OptimizerScalarState state;
    const Type3OptimizerScalarState before = state;
    for (const char* key : {"DEF_OPT", "TYP_OPT", "GRD_TYP", "DOE_PAR", "OTHER"}) {
        CHECK(applyType3OptimizerScalarLine(state, fields({key, "77"})) == Type3ScalarLineResult::Ignored);
        checkDefaults(state);
    }
    CHECK(applyType3OptimizerScalarLine(state, {}) == Type3ScalarLineResult::Ignored);
    CHECK_EQ(state.value0c, before.value0c);
}

TEST_CASE(enddata_returns_terminal_control_without_scalar_writes)
{
    Type3OptimizerScalarState state;
    const Type3OptimizerScalarState before = state;
    CHECK(applyType3OptimizerScalarLine(state, fields({"ENDDATA"})) == Type3ScalarLineResult::Terminate);
    CHECK_EQ(state.value0c, before.value0c);
    CHECK_EQ(state.value10, before.value10);
    CHECK_EQ(state.value30, before.value30);
    CHECK_EQ(state.value34, before.value34);
    CHECK_EQ(state.value38, before.value38);
    CHECK_EQ(state.value3c, before.value3c);
    CHECK_BITS(state.value40, before.value40);
    CHECK_EQ(state.value48, before.value48);
}

TEST_CASE(optimizer_row_bodies_are_recognized_but_not_mutated)
{
    Type3OptimizerScalarState state;
    CHECK(applyType3OptimizerScalarLine(state, fields({"NUM_CON", "2"})) == Type3ScalarLineResult::RowBodyNotImplemented);
    CHECK(applyType3OptimizerScalarLine(state, fields({"REGIONS", "1"})) == Type3ScalarLineResult::RowBodyNotImplemented);
    checkDefaults(state);
}

TEST_CASE(num_con_prepared_rows_preserve_native_argument_order_and_return_index)
{
    Type3OptimizerScalarState state;
    const qt::String formatted = qt::L("mapped-value");
    CHECK_EQ(appendType3NumConPreparedRow(state, 11, 21, 3.5, formatted, 12, 14), 0);
    CHECK_EQ(appendType3NumConPreparedRow(state, 31, 41, 5.5, qt::L("next-value"), 32, 34), 1);
    CHECK_EQ(state.numConRows.size(), 2u);
    const Type3NumConRowValues& first = state.numConRows[0];
    CHECK_EQ(first.value00, 21); // classifier result
    CHECK_EQ(first.value04, 11); // parser field 1
    CHECK_EQ(first.value08, 12); // parser field 2
    CHECK_EQ(first.value0c, 14); // parser field 4
    CHECK_BITS(first.value18, 3.5); // parser field 3
    CHECK(first.value20 == formatted);
    const Type3NumConRowValues& second = state.numConRows[1];
    CHECK_EQ(second.value00, 41);
    CHECK_EQ(second.value04, 31);
    CHECK_EQ(second.value08, 32);
    CHECK_EQ(second.value0c, 34);
    CHECK_BITS(second.value18, 5.5);
    CHECK(second.value20 == qt::L("next-value"));
    checkDefaults(state);
}

TEST_CASE(regions_prepared_row_uses_zero_and_returns_size_minus_one)
{
    Type3OptimizerScalarState state;
    const qt::String firstValue = qt::L("region-a");
    CHECK_EQ(appendType3RegionPreparedRow(state, firstValue), 0);
    CHECK_EQ(appendType3RegionPreparedRow(state, qt::L("region-b")), 1);
    CHECK_EQ(state.regionRows.size(), 2u);
    CHECK_EQ(state.regionRows[0].value00, 0);
    CHECK(state.regionRows[0].value08 == firstValue);
    CHECK_EQ(state.regionRows[1].value00, 0);
    CHECK(state.regionRows[1].value08 == qt::L("region-b"));
    checkDefaults(state);
}

TEST_CASE(type3_row_tag_classifier_exhaustively_matches_embedded_window)
{
    constexpr std::array<std::int32_t, 93> expected{{
        0, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, 1, 1, 1, -1, -1,
        -1, -1, -1, -1, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1, 1, 1,
        -1, -1, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, 0, 2, -1, -1, -1, -1, -1, -1, -1, -1,
        1, 1, 1, -1, -1, -1, -1, -1, -1, -1, 1, 1, 1,
    }};
    for (std::size_t index = 0; index < expected.size(); ++index)
        CHECK_EQ(classifyType3OptimizerRowTag(static_cast<std::int32_t>(0x3f2 + index)), expected[index]);
}

TEST_CASE(type3_row_tag_classifier_returns_default_outside_table_window)
{
    CHECK_EQ(classifyType3OptimizerRowTag(std::numeric_limits<std::int32_t>::min()), -1);
    CHECK_EQ(classifyType3OptimizerRowTag(-1), -1);
    CHECK_EQ(classifyType3OptimizerRowTag(0x3f1), -1);
    CHECK_EQ(classifyType3OptimizerRowTag(0x44f), -1);
    CHECK_EQ(classifyType3OptimizerRowTag(std::numeric_limits<std::int32_t>::max()), -1);
}

TEST_CASE(undersized_scalar_records_are_rejected_without_partial_writes)
{
    Type3OptimizerScalarState state;
    CHECK(applyType3OptimizerScalarLine(state, fields({"OPR_NO"})) == Type3ScalarLineResult::RowBodyNotImplemented);
    CHECK(applyType3OptimizerScalarLine(state, fields({"KRG_MAX"})) == Type3ScalarLineResult::RowBodyNotImplemented);
    CHECK(applyType3OptimizerScalarLine(state, fields({"EPS_OBJ"})) == Type3ScalarLineResult::RowBodyNotImplemented);
    CHECK(applyType3OptimizerScalarLine(state, fields({"TYP_OBJ", "5", "6"})) == Type3ScalarLineResult::RowBodyNotImplemented);
    checkDefaults(state);
}

int main() { return testkit::run_all(); }
