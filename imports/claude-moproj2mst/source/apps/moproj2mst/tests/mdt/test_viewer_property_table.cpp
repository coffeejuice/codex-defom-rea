// ctest-labels: m2m-mdt mdt-viewer doe
#include <testkit/check.hpp>
#include <cstdint>
#include <cstring>
#include <string>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include "m2m/common.hpp"
#include "m2m/doe_property_table.hpp"
#include "m2m/engine.hpp"
#include "m2m/engine_scene.hpp"

using namespace m2m;
namespace {
bool refuses(const auto& action, const char* fragment)
{
    try { action(); }
    catch (const Error& error) {
        return error.code == M2M_E_UNSUPPORTED && std::string(error.what()).find(fragment) != std::string::npos;
    }
    return false;
}
engine::LLOBJ_DATA* bound(engine::GuiModel& model, int number)
{
    auto* object = model.objectWrap(number, true, false);
    REQUIRE(object);
    engine::viewerSync(model, object, true);
    return object;
}
void kind(engine::LLOBJ_DATA& object, std::int32_t value)
{
    std::memcpy(reinterpret_cast<char*>(&object) + 0x0c, &value, sizeof value);
}
}

TEST_CASE(property_table_unbound_views_skip_unrecovered_cells_after_current_refresh)
{
    engine::GuiModel model(nullptr, 2);
    float minimum[3]{}, maximum[3]{};
    CHECK(engine::viewerBox(model, 1, minimum, maximum));
    doe::PropertyTableState table(model);
    table.markPopulationUnrecovered();
    model.step->u1b4c = 7;
    engine::finalizeStepDataPropertyTable(model, table);
    CHECK(model.step->u1b4c == 0);
    CHECK(table.queryCount() == 0);
    CHECK(engine::viewerItemCount(model) == 1);
    CHECK(!engine::viewerDisplayOptionSnapshot(model, 0));
}

TEST_CASE(property_table_kind7_bound_view_skips_unrecovered_cells)
{
    engine::GuiModel model(nullptr, 2);
    auto* object = bound(model, 1);
    kind(*object, 7);
    doe::PropertyTableState table(model);
    table.markPopulationUnrecovered();
    engine::finalizeStepDataPropertyTable(model, table);
    CHECK(table.queryCount() == 0);
    CHECK(engine::viewerDisplayOptionSnapshot(model, 0).has_value());
}

TEST_CASE(property_table_rows_count_only_eligible_views_and_empty_cells_keep_flags)
{
    engine::GuiModel model(nullptr, 2);
    bound(model, 1);
    auto* excluded = bound(model, 2);
    bound(model, 3);
    kind(*excluded, 7);
    const auto before = engine::viewerDisplayOptionSnapshot(model, 0);
    REQUIRE(before.has_value());
    doe::PropertyTableState table(model);
    engine::finalizeStepDataPropertyTable(model, table);
    CHECK(table.queryCount() == 12);
    REQUIRE(table.queryPrefix().size() == 12);
    for (int index = 0; index < 12; ++index) {
        CHECK(table.queryPrefix()[index].row == index / 6);
        CHECK(table.queryPrefix()[index].column == index % 6 + 1);
    }
    const auto after = engine::viewerDisplayOptionSnapshot(model, 0);
    REQUIRE(after.has_value());
    CHECK(after->flags == before->flags && after->mode == before->mode);
    CHECK(after->cachedMesh == before->cachedMesh && after->cachedGeometry == before->cachedGeometry);
    CHECK(!table.queryReportTruncated());
    CHECK(table.owner() == &model);
}

TEST_CASE(property_table_unrecovered_population_refuses_at_first_cell_after_refresh)
{
    engine::GuiModel model(nullptr, 2);
    bound(model, 1);
    doe::PropertyTableState table(model);
    table.markPopulationUnrecovered();
    model.step->u1b4c = 999;
    CHECK(refuses([&] { engine::finalizeStepDataPropertyTable(model, table); }, "property"));
    CHECK(model.step->u1b4c == 0); // Out-of-range reset does not immediately repick object1.
    REQUIRE(table.queryCount() == 1 && table.queryPrefix().size() == 1);
    CHECK(table.queryPrefix()[0].row == 0 && table.queryPrefix()[0].column == 1);
    CHECK(engine::viewerItemCount(model) == 1 && model.objectCount() == 1);
}

TEST_CASE(property_table_foreign_owner_rejects_adapter_misuse_before_effects)
{
    engine::GuiModel first(nullptr, 2), second(nullptr, 2);
    doe::PropertyTableState table(first);
    second.step->u1b4c = 999;
    CHECK(refuses([&] { engine::finalizeStepDataPropertyTable(second, table); }, "different actual model"));
    CHECK(second.step->u1b4c == 999 && table.queryCount() == 0);
    CHECK(!second.viewer && !first.viewer);
}

TEST_CASE(property_table_missing_step_skips_refresh_without_creating_model_or_viewer)
{
    engine::GuiModel model(nullptr, 2);
    doe::PropertyTableState table(model);
    table.markPopulationUnrecovered();
    model.step.reset();
    REQUIRE(!model.step && !model.viewer);
    engine::finalizeStepDataPropertyTable(model, table);
    CHECK(!model.step && !model.viewer);
    CHECK(table.owner() == &model);
    CHECK(table.queryCount() == 0 && table.queryPrefix().empty());
}

int main() { return testkit::run_all(); }
