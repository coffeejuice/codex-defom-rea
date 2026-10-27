#include <testkit/check.hpp>

#include "../../src/ops/type3_optimizer_defopt_view.hpp"

using namespace m2m;
using namespace m2m::mst::detail;

TEST_CASE(type3_defopt_view_maps_default_and_changed_scalars)
{
    Type3OptimizerScalarState state;
    Type3OptimizerDefOptView view(state);
    CHECK(view.value0c() == 1); CHECK(view.value10() == 1);
    CHECK(view.value30() == 0x3fd); CHECK(view.value34() == -1);
    CHECK(view.value38() == 200); CHECK(view.value40() == 0.001); CHECK(!view.value48());
    state.value0c = 9; state.value10 = 8; state.value30 = 7; state.value34 = 6;
    state.value38 = 5; state.value40 = 0.25; state.value48 = true;
    CHECK(view.value0c() == 9); CHECK(view.value10() == 8);
    CHECK(view.value30() == 7); CHECK(view.value34() == 6);
    CHECK(view.value38() == 5); CHECK(view.value40() == 0.25); CHECK(view.value48());
}

TEST_CASE(type3_defopt_view_preserves_classifier_tag_and_signed_row_bounds)
{
    Type3OptimizerScalarState state;
    const auto formatted = qt::L("row");
    appendType3NumConPreparedRow(state, 0x439, 17, 2.5, formatted, 3, 4);
    Type3OptimizerDefOptView view(state);
    CHECK(view.numConCount() == 1);
    CHECK(view.numConAt(-1) == nullptr); CHECK(view.numConAt(1) == nullptr);
    const auto* row = view.numConAt(0);
    REQUIRE(row != nullptr);
    CHECK(row->value00 == 17); CHECK(row->value04 == 0x439);
    CHECK(row->value08 == 3); CHECK(row->value0c == 4);
    CHECK(row->value18 == 2.5); CHECK(row->value20 == formatted);
}

TEST_CASE(type3_defopt_view_keeps_region_order_and_invalid_name_fallback)
{
    Type3OptimizerScalarState state;
    appendType3RegionPreparedRow(state, qt::L("first"));
    appendType3RegionPreparedRow(state, qt::L("second"));
    Type3OptimizerDefOptView view(state);
    CHECK(view.regionCount() == 2);
    CHECK(view.regionNameAt(0) == qt::L("first"));
    CHECK(view.regionNameAt(1) == qt::L("second"));
    CHECK(view.regionNameAt(-1) == qt::L("Selected Object"));
    CHECK(view.regionNameAt(2) == qt::L("Selected Object"));
}

int main() { return testkit::run_all(); }
