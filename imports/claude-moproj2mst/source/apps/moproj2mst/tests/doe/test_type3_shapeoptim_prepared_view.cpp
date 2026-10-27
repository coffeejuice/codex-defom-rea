// Prepared normal-path values only; native Qt failure/lifetime and GEO are excluded.
#include <testkit/check.hpp>

#include <limits>

#include "../../src/ops/type3_shapeoptim_prepared_view.hpp"

using namespace m2m;
using namespace m2m::mst::detail;

namespace {

Type3NumConRowValues row(std::int32_t tag, std::int32_t count,
                        std::int32_t id, double value = 0.0,
                        std::int32_t classifier = -123)
{
    return {classifier, tag, count, id, value, qt::L("unused formatted value")};
}

void project(const Type3OptimizerScalarState& source,
             Type3ShapeOptimPreparedState& destination)
{
    projectType3ShapeOptimPreparedView(Type3OptimizerDefOptView(source), destination);
}

}  // namespace

TEST_CASE(type3_shapeoptim_maps_scalars_and_preserves_absent_row_destinations)
{
    Type3OptimizerScalarState source;
    source.value0c = -12; source.value10 = 13; source.value30 = 1055;
    source.value34 = 0; source.value38 = 42; source.value40 = -0.0;
    source.value48 = true;
    Type3ShapeOptimPreparedState state;
    state.value70 = 70; state.value74 = 74; state.value78 = 78;
    state.value7c = -7; state.value80 = -8; state.value84 = -9;
    state.value98 = 2; state.value99 = 3; state.value9a = 4; state.value9b = 5;
    state.value9c = 6; state.value9d = 7; state.value9e = 8;
    state.valuec8 = -800;
    state.listA0 = {9}; state.listA8 = {1.25}; state.listF0 = {qt::L("seed")};
    project(source, state);
    CHECK_EQ(state.valuecc, 42); CHECK_EQ(state.valued8, -12);
    CHECK_EQ(state.valued4, 13); CHECK_EQ(state.valued0, 0);
    CHECK_EQ(state.valuec0, 1055); CHECK_EQ(state.valuec4, 0);
    CHECK_EQ(state.valuec8, -800); CHECK_EQ(state.valuee8, 0);
    CHECK_EQ(state.valuee9, 1); CHECK_BITS(state.valuee0, -0.0);
    CHECK_EQ(state.value70, 70); CHECK_EQ(state.value74, 74); CHECK_EQ(state.value78, 78);
    CHECK_EQ(state.value7c, -7); CHECK_EQ(state.value80, -8); CHECK_EQ(state.value84, -9);
    CHECK_EQ(state.value98, 0); CHECK_EQ(state.value99, 0);
    CHECK_EQ(state.value9a, 0); CHECK_EQ(state.value9b, 0);
    CHECK_EQ(state.value9c, 6); CHECK_EQ(state.value9d, 7); CHECK_EQ(state.value9e, 8);
    CHECK(state.listA0 == std::vector<std::int32_t>{9});
    CHECK(state.listA8 == std::vector<double>{1.25});
    CHECK(state.listF0 == std::vector<qt::String>{qt::L("seed")});
    source.value34 = -1; project(source, state); CHECK_EQ(state.valuee8, 0);
    source.value34 = 1; source.value48 = false;
    project(source, state); CHECK_EQ(state.valuee8, 1); CHECK_EQ(state.valuee9, 0);
}

TEST_CASE(type3_shapeoptim_all_special_tags_share_ordered_overwrites)
{
    Type3OptimizerScalarState source;
    source.numConRows = {row(0x3f2, -4, 11), row(0x3f4, 1, -12),
                         row(0x438, 8, 13), row(0x439, 7, -14)};
    Type3ShapeOptimPreparedState state;
    project(source, state);
    CHECK_EQ(state.value98, 1); CHECK_EQ(state.value99, 1);
    CHECK_EQ(state.value9a, 1); CHECK_EQ(state.value9b, 1);
    CHECK_EQ(state.value74, 13); CHECK_EQ(state.value9d, 1); CHECK_EQ(state.value80, 8);
    CHECK_EQ(state.value78, -14); CHECK_EQ(state.value9e, 1); CHECK_EQ(state.value84, 7);
    CHECK(state.listA0.empty()); CHECK(state.listA8.empty());
    source.numConRows = {row(0x438, 8, 13), row(0x3f4, 1, 0), row(0x3f2, -4, -11)};
    project(source, state);
    CHECK_EQ(state.value74, -11); CHECK_EQ(state.value9d, 0); CHECK_EQ(state.value80, 1);
    CHECK_EQ(state.value9b, 0); CHECK_EQ(state.value78, -14); CHECK_EQ(state.value9e, 1);
}

TEST_CASE(type3_shapeoptim_439_uses_seeded_or_preceding_74_and_not_78)
{
    Type3OptimizerScalarState source;
    source.numConRows = {row(0x439, 0, 999)};
    Type3ShapeOptimPreparedState state;
    state.value74 = -1;
    project(source, state);
    CHECK_EQ(state.value78, 999); CHECK_EQ(state.value9e, 0); CHECK_EQ(state.value84, 1);
    state.value74 = 1;
    source.numConRows = {row(0x439, 1, -999)};
    project(source, state);
    CHECK_EQ(state.value78, -999); CHECK_EQ(state.value9e, 1);
    source.numConRows = {row(0x3f2, 1, 0), row(0x439, 1, 4), row(0x3f2, 1, 8)};
    project(source, state);
    CHECK_EQ(state.value74, 8); CHECK_EQ(state.value9e, 0);
    source.numConRows = {row(0x3f2, 1, 8), row(0x439, 1, -4)};
    project(source, state);
    CHECK_EQ(state.value9e, 1);
}

TEST_CASE(type3_shapeoptim_3f4_commits_shared_fields_without_another_special_row)
{
    Type3OptimizerScalarState source;
    source.numConRows = {row(0x3f4, -2, 27)};
    Type3ShapeOptimPreparedState state;
    state.value74 = -77;
    state.value80 = -9;
    state.value9d = 9;
    project(source, state);
    CHECK_EQ(state.value99, 1);
    CHECK_EQ(state.value74, 27);
    CHECK_EQ(state.value9d, 1);
    CHECK_EQ(state.value80, 1);
}

TEST_CASE(type3_shapeoptim_fallback_uses_tag_and_preserves_append_order_and_double_bits)
{
    Type3OptimizerScalarState source;
    source.numConRows = {row(77, -9, 10, -0.0, 0x3f2),
                         row(77, 1, -20, -3.25, 0x439),
                         row(-7, 6, 0, 9.5, 0x438)};
    Type3ShapeOptimPreparedState state;
    state.listA0 = {88}; state.listA8 = {4.0};
    project(source, state);
    CHECK(state.listA0 == (std::vector<std::int32_t>{88, 77, 77, -7}));
    REQUIRE(state.listA8.size() == 4);
    CHECK_BITS(state.listA8[0], 4.0); CHECK_BITS(state.listA8[1], -0.0);
    CHECK_BITS(state.listA8[2], -3.25); CHECK_BITS(state.listA8[3], 9.5);
    CHECK_EQ(state.value70, 0); CHECK_EQ(state.value9c, 0); CHECK_EQ(state.value7c, 6);
    CHECK_EQ(state.value98, 0); CHECK_EQ(state.value99, 0);
    CHECK_EQ(state.value9a, 0); CHECK_EQ(state.value9b, 0);
    source.numConRows = {row(0x3f2, 3, 42, 123.0, -7)};
    project(source, state);
    CHECK_EQ(state.value98, 1); CHECK_EQ(state.value74, 42);
    CHECK_EQ(state.listA0.size(), std::size_t(4));
    source.numConRows = {row(77, 0, -20, -3.25)};
    project(source, state);
    CHECK_EQ(state.value70, -20); CHECK_EQ(state.value9c, 0); CHECK_EQ(state.value7c, 1);
}

TEST_CASE(type3_shapeoptim_repeated_calls_append_names_and_fallbacks_without_clearing)
{
    Type3OptimizerScalarState source;
    source.numConRows = {row(5, 1, 1, 1.5), row(5, 1, 2, 2.5)};
    source.regionRows = {{9, qt::L("same")}, {1, qt::L("other")}, {9, qt::L("same")}};
    Type3ShapeOptimPreparedState state;
    state.listA0 = {6}; state.listA8 = {0.5}; state.listF0 = {qt::L("seed")};
    project(source, state);
    project(source, state);
    CHECK(state.listA0 == (std::vector<std::int32_t>{6, 5, 5, 5, 5}));
    CHECK(state.listA8 == (std::vector<double>{0.5, 1.5, 2.5, 1.5, 2.5}));
    CHECK(state.listF0 == (std::vector<qt::String>{qt::L("seed"), qt::L("same"),
        qt::L("other"), qt::L("same"), qt::L("same"), qt::L("other"), qt::L("same")}));
}

TEST_CASE(type3_shapeoptim_clamps_signed_counts_for_each_destination)
{
    const std::int32_t counts[] = {(std::numeric_limits<std::int32_t>::min)(), -1, 0, 1, 2,
                                   (std::numeric_limits<std::int32_t>::max)()};
    for (auto count : counts) {
        Type3OptimizerScalarState source;
        source.numConRows = {row(0x3f2, count, 1), row(0x3f4, count, 1),
                             row(0x438, count, 1), row(0x439, count, 1), row(3, count, 1)};
        Type3ShapeOptimPreparedState state;
        project(source, state);
        const auto expected = count < 1 ? 1 : count;
        CHECK_EQ(state.value7c, expected); CHECK_EQ(state.value80, expected);
        CHECK_EQ(state.value84, expected); CHECK_EQ(state.value9c, 1);
    }
}

TEST_CASE(type3_shapeoptim_c4_uses_signed_remainders_and_threshold_order)
{
    struct Example { std::int32_t input; std::int32_t expected; };
    const Example examples[] = {
        {-2147483647 - 1, -54}, {-1057, -36}, {-1022, -1}, {-1021, 0}, {-4, -4},
        {0, 0}, {1, 1}, {2, 2}, {3, 3}, {1021, 0}, {1022, 1}, {1023, 2},
        {1024, 4}, {1054, 4}, {1055, 0}, {1056, 0}, {1057, 0}, {1058, 8},
        {9999, 9}, {10000, 0}, {10001, 1}, {10099, 99}, {10210, 10},
        {(std::numeric_limits<std::int32_t>::max)(), 47}
    };
    for (const auto& example : examples) {
        Type3OptimizerScalarState source;
        source.value30 = example.input;
        Type3ShapeOptimPreparedState state;
        project(source, state);
        CHECK_EQ(state.valuec0, example.input); CHECK_EQ(state.valuec4, example.expected);
    }
}

int main() { return testkit::run_all(); }

