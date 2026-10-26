// ctest-labels: m2m-mdt mdt-l6 m2m-mdt-l7
#include <testkit/check.hpp>

#include "m2m/common.hpp"
#include "m2m/doe_property_table.hpp"
#include "m2m/doe_statevar.hpp"
#include "m2m/doe_stepdata.hpp"
#include "m2m/engine.hpp"

using namespace m2m;

namespace {
bool refuses(const auto& action)
{
    try { action(); }
    catch (const Error& error) { return error.code == M2M_E_UNSUPPORTED; }
    return false;
}
} // namespace

TEST_CASE(doe_property_table_constructor_empty_queries_keep_actual_owner)
{
    engine::GuiModel model(nullptr, 2);
    doe::PropertyTableState table(model);
    CHECK(table.owner() == &model && table.constructorEmpty());
    CHECK(table.queryPrefix().empty() && table.queryCount() == 0 && !table.lastQuery());
    for (int column = 1; column <= 6; ++column) CHECK(table.itemIsAbsent(0, column));
    CHECK(table.itemIsAbsent(17, 0));
    CHECK(table.constructorEmpty() && table.owner() == &model);
    REQUIRE(table.queryPrefix().size() == 7);
    for (int column = 1; column <= 6; ++column) {
        CHECK(table.queryPrefix()[column - 1].row == 0);
        CHECK(table.queryPrefix()[column - 1].column == column);
    }
    CHECK(table.queryCount() == 7 && !table.queryReportTruncated());
    REQUIRE(table.lastQuery());
    CHECK(table.lastQuery()->row == 17 && table.lastQuery()->column == 0);
}

TEST_CASE(doe_property_table_population_guards_at_query_without_replacing_owner)
{
    engine::GuiModel model(nullptr, 2);
    doe::PropertyTableState table(model);
    CHECK(table.itemIsAbsent(0, 1));
    table.markPopulationUnrecovered();
    CHECK(!table.constructorEmpty());
    CHECK(refuses([&] { (void)table.itemIsAbsent(0, 2); }));
    CHECK(table.owner() == &model && !table.constructorEmpty());
    CHECK(table.queryCount() == 2);
    REQUIRE(table.lastQuery());
    CHECK(table.lastQuery()->row == 0 && table.lastQuery()->column == 2);
}

TEST_CASE(doe_property_table_negative_indices_return_null_before_population_consumer)
{
    engine::GuiModel model(nullptr, 2);
    doe::PropertyTableState table(model);
    table.markPopulationUnrecovered();
    CHECK(table.itemIsAbsent(-1, 1));
    CHECK(table.itemIsAbsent(0, -1));
    CHECK(!table.constructorEmpty() && table.queryCount() == 2);
    CHECK(refuses([&] { (void)table.itemIsAbsent(0, 0); }));
}

TEST_CASE(doe_property_table_query_diagnostics_truncate_without_semantic_refusal)
{
    engine::GuiModel model(nullptr, 2);
    doe::PropertyTableState table(model);
    for (int row = 0; row < 300; ++row) CHECK(table.itemIsAbsent(row, 6));
    CHECK(table.queryPrefix().size() == 256 && table.queryCount() == 300);
    CHECK(table.queryReportTruncated() && table.constructorEmpty());
    REQUIRE(table.lastQuery());
    CHECK(table.lastQuery()->row == 299 && table.lastQuery()->column == 6);
    CHECK(table.queryPrefix().back().row == 255);
}

TEST_CASE(doe_property_table_reset_deletes_actual_table_and_retains_separate_statevar)
{
    engine::GuiModel model(nullptr, 2);
    doe::StepDataState state;
    CHECK(!state.propertyTableCreated());
    state.propertyTable = std::make_unique<doe::PropertyTableState>(model);
    //This test-created value checks ownership separation, not native ctor effects.
    const auto stateVar = std::make_shared<doe::StateVarState>();
    state.stateVar = stateVar;
    CHECK(state.propertyTableCreated() && state.propertyTable->owner() == &model);
    state.resetPropertyTable();
    CHECK(!state.propertyTableCreated() && !state.propertyTable && state.stateVar == stateVar);
    state.resetPropertyTable();
    CHECK(!state.propertyTableCreated() && state.stateVar == stateVar);
}

int main() { return testkit::run_all(); }
