// Prepared graph only: no imported-project admission, Qt guards or lifetimes.
#include <testkit/check.hpp>

#include "../../src/ops/type3_shapeoptim_identity_view.hpp"

using namespace m2m::mst::detail;

namespace {
using Result = Type3ShapeOptimIdentityResult;
using Kind = Type3ShapeOptimPropertyEventKind;
using Events = std::vector<Type3ShapeOptimPropertyEvent>;

std::vector<Type3ShapeOptimPropertyNode> tree()
{
    return {{"StopCritGroup", {{"MaxSims", {}}, {"MinObjFuncVal", {}}}}};
}

Type3ShapeOptimIdentityGraph prepared()
{
    Type3ShapeOptimIdentityGraph graph;
    graph.classes.emplace(1, Type3ShapeOptimIdentityClass{{42, 3.25}, {}, {}});
    graph.classes.emplace(2, Type3ShapeOptimIdentityClass{{-99, 0.125}, {}, {}});
    graph.operations.emplace(10, Type3ShapeOptimIdentityOperation{});
    // These are supplied normal-path postconditions, not native factory calls.
    REQUIRE(prepareType3ShapeOptimConstruction(graph, 1, 20, 30, tree()));
    REQUIRE(prepareType3ShapeOptimLinkedProperty(graph, 1, 20));
    REQUIRE(prepareType3ShapeOptimOperationProperty(graph, 10, 20));
    REQUIRE(prepareType3ShapeOptimCurrentOperation(graph, 1, 10));
    return graph;
}

void checkValues(const Type3ShapeOptimIdentityGraph& graph,
                 Type3ShapeOptimIdentity data, int maxSims, double minObjective)
{
    CHECK_EQ(graph.data.at(data).maxSims, maxSims);
    CHECK_BITS(graph.data.at(data).minObjective, minObjective);
}
}  // namespace

TEST_CASE(type3_identity_construction_defaults_require_explicit_publication)
{
    Type3ShapeOptimIdentityGraph graph;
    graph.classes.emplace(1, Type3ShapeOptimIdentityClass{});
    graph.operations.emplace(10, Type3ShapeOptimIdentityOperation{});
    REQUIRE(prepareType3ShapeOptimConstruction(graph, 1, 20, 30, tree()));
    checkValues(graph, 30, 200, 0.001);
    CHECK(graph.properties.at(20).creator == Type3ShapeOptimOptionalIdentity{1});
    CHECK(graph.properties.at(20).data == Type3ShapeOptimOptionalIdentity{30});
    CHECK(!graph.classes.at(1).linkedProperty);
    CHECK(!graph.classes.at(1).currentOperation);
    CHECK(!graph.operations.at(10).property);
    CHECK(!prepareType3ShapeOptimConstruction(graph, 1, 20, 31, tree()));
    CHECK(!prepareType3ShapeOptimConstruction(graph, 1, 21, 30, tree()));
    CHECK(!prepareType3ShapeOptimConstruction(graph, 999, 21, 31, tree()));
    CHECK_EQ(graph.properties.size(), 1u); CHECK_EQ(graph.data.size(), 1u);
}

TEST_CASE(type3_identity_p_equals_b_only_for_supplied_publication_and_selection)
{
    auto graph = prepared(); Events events;
    CHECK(graph.operations.at(10).property == graph.classes.at(1).linkedProperty);
    CHECK(projectType3ShapeOptimIdentityView(graph, 1, events) == Result::Complete);
    checkValues(graph, 30, 42, 3.25);
    REQUIRE(events.size() == 5);
    CHECK(events[0].kind == Kind::DebugTopDisplayName);
    CHECK(events[1].kind == Kind::SetMaxSims);
    CHECK(events[2].kind == Kind::SetMinObjective);
    CHECK(events[3].kind == Kind::RefreshMaxSims);
    CHECK(events[4].kind == Kind::RefreshMinObjective);
}

TEST_CASE(type3_identity_later_link_replacement_preserves_operation_property)
{
    auto graph = prepared(); Events events;
    REQUIRE(prepareType3ShapeOptimConstruction(graph, 2, 21, 31, tree()));
    REQUIRE(prepareType3ShapeOptimLinkedProperty(graph, 1, 21));
    CHECK(graph.operations.at(10).property == Type3ShapeOptimOptionalIdentity{20});
    CHECK(graph.classes.at(1).currentOperation == Type3ShapeOptimOptionalIdentity{10});
    CHECK(projectType3ShapeOptimIdentityView(graph, 1, events) == Result::Complete);
    // P receives current OB; B receives its different creator, independent of P.
    checkValues(graph, 30, 42, 3.25); checkValues(graph, 31, -99, 0.125);
    REQUIRE(events.size() == 5); CHECK_EQ(events[3].intValue, -99);
}

TEST_CASE(type3_identity_link_reset_keeps_current_p_and_its_effects)
{
    auto graph = prepared(); Events events;
    REQUIRE(prepareType3ShapeOptimLinkedProperty(graph, 1, std::nullopt));
    CHECK(projectType3ShapeOptimIdentityView(graph, 1, events) == Result::Complete);
    checkValues(graph, 30, 42, 3.25); CHECK_EQ(events.size(), 3u);
    CHECK(graph.operations.at(10).property == Type3ShapeOptimOptionalIdentity{20});
    CHECK_EQ(graph.properties.size(), 1u); CHECK_EQ(graph.data.size(), 1u);
}

TEST_CASE(type3_identity_distinct_properties_can_share_data_identity)
{
    auto graph = prepared(); Events events;
    REQUIRE(prepareType3ShapeOptimConstruction(graph, 2, 21, 31, tree()));
    graph.properties.at(21).data = 30;
    REQUIRE(prepareType3ShapeOptimLinkedProperty(graph, 1, 21));
    CHECK(projectType3ShapeOptimIdentityView(graph, 1, events) == Result::Complete);
    checkValues(graph, 30, -99, 0.125); checkValues(graph, 31, 200, 0.001);
    REQUIRE(events.size() == 5);
    CHECK_EQ(events[1].intValue, 42); CHECK_EQ(events[3].intValue, -99);
}

TEST_CASE(type3_identity_failed_cast_skips_p_but_still_refreshes_b)
{
    auto graph = prepared(); Events events;
    graph.properties.at(20).castSuccess = false;
    CHECK(projectType3ShapeOptimIdentityView(graph, 1, events) == Result::Complete);
    checkValues(graph, 30, 42, 3.25);
    REQUIRE(events.size() == 2);
    CHECK(events[0].kind == Kind::RefreshMaxSims);
    CHECK(events[1].kind == Kind::RefreshMinObjective);
}

TEST_CASE(type3_identity_absent_current_operation_and_null_creator_are_independent)
{
    auto graph = prepared(); Events events;
    REQUIRE(prepareType3ShapeOptimCurrentOperation(graph, 1, std::nullopt));
    CHECK(projectType3ShapeOptimIdentityView(graph, 1, events) == Result::Complete);
    CHECK_EQ(events.size(), 2u); checkValues(graph, 30, 42, 3.25);
    graph.properties.at(20).creator.reset(); graph.properties.at(20).data.reset();
    events.clear();
    CHECK(projectType3ShapeOptimIdentityView(graph, 1, events) == Result::Complete);
    CHECK(events.empty());
    REQUIRE(prepareType3ShapeOptimCurrentOperation(graph, 1, 10));
    REQUIRE(prepareType3ShapeOptimOperationProperty(graph, 10, std::nullopt));
    CHECK(projectType3ShapeOptimIdentityView(graph, 1, events) == Result::Complete);
    CHECK(events.empty());
}

TEST_CASE(type3_identity_null_p_data_refuses_only_at_matching_write)
{
    auto graph = prepared(); Events events;
    graph.properties.at(20).data.reset();
    REQUIRE(prepareType3ShapeOptimLinkedProperty(graph, 1, std::nullopt));
    graph.properties.at(20).properties = {{"Other", {}}, {"StopCritGroup", {{"Unused", {}}}}};
    CHECK(projectType3ShapeOptimIdentityView(graph, 1, events) == Result::Complete);
    CHECK_EQ(events.size(), 2u);
    for (const char* match : {"MaxSims", "MinObjFuncVal"}) {
        graph.properties.at(20).properties[1].children.push_back({match, {}});
        graph.properties.at(20).properties.push_back({"Unvisited", {}});
        events.clear();
        CHECK(projectType3ShapeOptimIdentityView(graph, 1, events) == Result::MissingPropertyData);
        REQUIRE(events.size() == 2);
        CHECK(events[0].kind == Kind::DebugTopDisplayName);
        CHECK(events[1].kind == Kind::DebugTopDisplayName);
        graph.properties.at(20).properties[1].children.pop_back();
        graph.properties.at(20).properties.pop_back();
    }
    checkValues(graph, 30, 200, 0.001);
}

TEST_CASE(type3_identity_null_b_data_preserves_completed_p_effects)
{
    auto graph = prepared(); Events events;
    REQUIRE(prepareType3ShapeOptimConstruction(graph, 2, 21, 31, tree()));
    graph.properties.at(21).data.reset();
    REQUIRE(prepareType3ShapeOptimLinkedProperty(graph, 1, 21));
    CHECK(projectType3ShapeOptimIdentityView(graph, 1, events) == Result::MissingRefreshData);
    checkValues(graph, 30, 42, 3.25); checkValues(graph, 31, 200, 0.001);
    REQUIRE(events.size() == 3); CHECK(events[2].kind == Kind::SetMinObjective);
}

TEST_CASE(type3_identity_unknown_ids_refuse_before_tail_effects)
{
    for (int malformed = 0; malformed != 8; ++malformed) {
        auto graph = prepared(); Events events{{Kind::DebugTopDisplayName, "seed"}};
        auto selected = Type3ShapeOptimIdentity{1};
        switch (malformed) {
        case 0: selected = 999; break;
        case 1: graph.classes.at(1).currentOperation = 999; break;
        case 2: graph.operations.at(10).property = 999; break;
        case 3: graph.classes.at(1).linkedProperty = 999; break;
        case 4: graph.properties.at(20).creator = 999; break;
        case 5: graph.properties.at(20).data = 999; break;
        case 6:
        case 7:
            REQUIRE(prepareType3ShapeOptimConstruction(graph, 2, 21, 31, tree()));
            REQUIRE(prepareType3ShapeOptimLinkedProperty(graph, 1, 21));
            if (malformed == 6) graph.properties.at(21).creator = 999;
            else graph.properties.at(21).data = 999;
            break;
        }
        CHECK(projectType3ShapeOptimIdentityView(graph, selected, events) == Result::InvalidPreparedIdentity);
        REQUIRE(events.size() == 1); CHECK(events[0].displayName == "seed");
        checkValues(graph, 30, 200, 0.001);
    }
}

TEST_CASE(type3_identity_invalid_prepared_publication_does_not_change_selection)
{
    auto graph = prepared();
    CHECK(!prepareType3ShapeOptimLinkedProperty(graph, 1, 999));
    CHECK(!prepareType3ShapeOptimLinkedProperty(graph, 999, 20));
    CHECK(!prepareType3ShapeOptimOperationProperty(graph, 10, 999));
    CHECK(!prepareType3ShapeOptimOperationProperty(graph, 999, 20));
    CHECK(!prepareType3ShapeOptimCurrentOperation(graph, 1, 999));
    CHECK(!prepareType3ShapeOptimCurrentOperation(graph, 999, 10));
    CHECK(graph.classes.at(1).linkedProperty == Type3ShapeOptimOptionalIdentity{20});
    CHECK(graph.classes.at(1).currentOperation == Type3ShapeOptimOptionalIdentity{10});
    CHECK(graph.operations.at(10).property == Type3ShapeOptimOptionalIdentity{20});
}

int main() { return testkit::run_all(); }
