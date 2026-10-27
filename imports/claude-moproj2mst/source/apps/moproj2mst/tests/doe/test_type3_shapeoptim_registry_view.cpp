// Stable private registry/selection only; no native Qt or public admission.
#include <testkit/check.hpp>

#include "../../src/ops/type3_shapeoptim_registry_view.hpp"
#include "../../src/ops/type3_shapeoptim_initializer.hpp"

using namespace m2m;
using namespace m2m::mst::detail;

namespace {
using Status = Type3ShapeOptimRegistryStatus;
using Graph = Type3ShapeOptimIdentityGraph;
using Id = Type3ShapeOptimOptionalIdentity;

std::vector<Type3ShapeOptimPropertyNode> tree()
{
    return {{"StopCritGroup", {{"MaxSims", {}}, {"MinObjFuncVal", {}}}}};
}

Graph prepared()
{
    Graph graph;
    graph.classes.emplace(1, Type3ShapeOptimIdentityClass{});
    graph.classes.emplace(2, Type3ShapeOptimIdentityClass{});
    auto& state = graph.classes.at(1).state;
    state.value6c = 6; state.value70 = 70; state.value74 = 74;
    state.value78 = 78; state.value7c = 7; state.value80 = 80;
    state.value84 = 84; state.value88 = -0.0; state.value90 = -90.5;
    state.value98 = 98; state.value99 = 99; state.value9a = 100;
    state.value9b = 101; state.value9c = 102; state.value9d = 103;
    state.value9e = 104; state.valuec0 = 10; state.valuec4 = 14;
    state.valuec8 = 18; state.valued0 = 20; state.valued4 = 24;
    state.valued8 = 28; state.valuee8 = 108; state.valuee9 = 109;
    state.listA0 = {5, -6}; state.listA8 = {-0.0, 7.5};
    state.listF0 = {qt::L("seed"), qt::L("other")};
    state.values = {42, 3.25};
    graph.classes.at(2).state = state;
    graph.classes.at(2).state.values = {-99, 0.125};
    // Legacy one-field aggregate remains valid after the appended key.
    graph.operations.emplace(10, Type3ShapeOptimIdentityOperation{Id{20}});
    graph.operations.at(10).classKey = u"ShapeOptim";
    graph.operations.emplace(11, Type3ShapeOptimIdentityOperation{Id{21}, u"ShapeOptim"});
    REQUIRE(prepareType3ShapeOptimConstruction(graph, 1, 20, 30, tree()));
    REQUIRE(prepareType3ShapeOptimConstruction(graph, 2, 21, 31, tree()));
    REQUIRE(prepareType3ShapeOptimLinkedProperty(graph, 1, 21));
    REQUIRE(prepareType3ShapeOptimLinkedProperty(graph, 2, 20));
    REQUIRE(prepareType3ShapeOptimCurrentOperation(graph, 1, 11));
    REQUIRE(prepareType3ShapeOptimCurrentOperation(graph, 2, 11));
    return graph;
}

void checkState(const Type3ShapeOptimPreparedState& a,
                const Type3ShapeOptimPreparedState& b)
{
#define SAME(field) CHECK(a.field == b.field)
    SAME(value6c); SAME(value70); SAME(value74); SAME(value78);
    SAME(value7c); SAME(value80); SAME(value84);
    CHECK_BITS(a.value88, b.value88); CHECK_BITS(a.value90, b.value90);
    SAME(value98); SAME(value99); SAME(value9a); SAME(value9b);
    SAME(value9c); SAME(value9d); SAME(value9e); SAME(listA0);
    REQUIRE(a.listA8.size() == b.listA8.size());
    for (std::size_t i = 0; i < a.listA8.size(); ++i)
        CHECK_BITS(a.listA8[i], b.listA8[i]);
    SAME(valuec0); SAME(valuec4); SAME(valuec8);
    SAME(values.maxSims); CHECK_BITS(a.values.minObjective, b.values.minObjective);
    SAME(valued0); SAME(valued4); SAME(valued8); SAME(valuee8); SAME(valuee9);
    SAME(listF0);
#undef SAME
}

void checkTree(const std::vector<Type3ShapeOptimPropertyNode>& a,
               const std::vector<Type3ShapeOptimPropertyNode>& b)
{
    REQUIRE(a.size() == b.size());
    for (std::size_t i = 0; i < a.size(); ++i) {
        CHECK(a[i].displayName == b[i].displayName);
        checkTree(a[i].children, b[i].children);
    }
}

// Compare every represented field so selection cannot silently reset CP462
// state, relink B, replace P, write data, or add/remove graph entries.
void checkGraph(const Graph& actual, const Graph& expected)
{
    REQUIRE(actual.classes.size() == expected.classes.size());
    for (const auto& entry : expected.classes) {
        REQUIRE(actual.classes.count(entry.first) == 1);
        const auto& a = actual.classes.at(entry.first);
        CHECK(a.currentOperation == entry.second.currentOperation);
        CHECK(a.linkedProperty == entry.second.linkedProperty);
        checkState(a.state, entry.second.state);
    }
    REQUIRE(actual.operations.size() == expected.operations.size());
    for (const auto& entry : expected.operations) {
        REQUIRE(actual.operations.count(entry.first) == 1);
        const auto& a = actual.operations.at(entry.first);
        CHECK(a.property == entry.second.property);
        CHECK(a.classKey == entry.second.classKey);
    }
    REQUIRE(actual.properties.size() == expected.properties.size());
    for (const auto& entry : expected.properties) {
        REQUIRE(actual.properties.count(entry.first) == 1);
        const auto& a = actual.properties.at(entry.first);
        CHECK(a.castSuccess == entry.second.castSuccess);
        CHECK(a.creator == entry.second.creator); CHECK(a.data == entry.second.data);
        checkTree(a.properties, entry.second.properties);
    }
    REQUIRE(actual.data.size() == expected.data.size());
    for (const auto& entry : expected.data) {
        REQUIRE(actual.data.count(entry.first) == 1);
        const auto& a = actual.data.at(entry.first);
        CHECK_EQ(a.maxSims, entry.second.maxSims);
        CHECK_BITS(a.minObjective, entry.second.minObjective);
    }
}

void selected(const Type3ShapeOptimRegistrySelection& result,
              Type3ShapeOptimIdentity classId)
{
    CHECK(result.status == Status::Selected);
    CHECK(result.selectedClass == Id{classId});
}
}  // namespace

TEST_CASE(type3_registry_operation_key_selects_class_and_repeated_selection_is_stable)
{
    auto graph = prepared(); auto expected = graph;
    const Type3ShapeOptimPreparedRegistry registry{{u"ShapeOptim", 1}, {u"other", 2}};
    const auto originalRegistry = registry;
    expected.classes.at(1).currentOperation = 10;
    selected(selectType3ShapeOptimRegistryOperation(graph, registry, 10), 1);
    checkGraph(graph, expected);
    selected(selectType3ShapeOptimRegistryOperation(graph, registry, 10), 1);
    checkGraph(graph, expected);
    // Changing the operation-owned key changes the selected class. No caller
    // class parameter can accidentally bypass the lookup.
    graph.operations.at(10).classKey = u"other";
    expected.operations.at(10).classKey = u"other";
    expected.classes.at(2).currentOperation = 10;
    selected(selectType3ShapeOptimRegistryOperation(graph, registry, 10), 2);
    checkGraph(graph, expected); CHECK(registry == originalRegistry);
}

TEST_CASE(type3_registry_two_keys_can_select_one_class_with_exact_prepared_utf16_keys)
{
    auto graph = prepared(); auto expected = graph;
    const std::u16string key = u"ShapeOptim\u03a9";
    const Type3ShapeOptimPreparedRegistry registry{{u"ShapeOptim", 1}, {key, 1}};
    selected(selectType3ShapeOptimRegistryOperation(graph, registry, 10), 1);
    graph.operations.at(11).classKey = key;
    expected.operations.at(11).classKey = key;
    selected(selectType3ShapeOptimRegistryOperation(graph, registry, 11), 1);
    expected.classes.at(1).currentOperation = 11;
    checkGraph(graph, expected);
    graph.operations.at(11).classKey = u"shapeoptim\u03a9";
    expected.operations.at(11).classKey = graph.operations.at(11).classKey;
    const auto miss = selectType3ShapeOptimRegistryOperation(graph, registry, 11);
    CHECK(miss.status == Status::MissingClassKey); CHECK(!miss.selectedClass);
    checkGraph(graph, expected);
}

TEST_CASE(type3_registry_missing_null_and_invalid_identity_leave_graph_and_registry_unchanged)
{
    const Type3ShapeOptimPreparedRegistry registry{
        {u"null", std::nullopt}, {u"bad", 999}, {u"ShapeOptim", 1}};
    const auto originalRegistry = registry;
    for (const auto& key : {u"missing", u"null", u"bad"}) {
        auto graph = prepared(); graph.operations.at(10).classKey = key;
        const auto before = graph;
        const auto result = selectType3ShapeOptimRegistryOperation(graph, registry, 10);
        const auto wanted = graph.operations.at(10).classKey == u"missing"
            ? Status::MissingClassKey : graph.operations.at(10).classKey == u"null"
            ? Status::NullClass : Status::InvalidPreparedIdentity;
        CHECK(result.status == wanted); CHECK(!result.selectedClass);
        checkGraph(graph, before); CHECK(registry == originalRegistry);
    }
    auto graph = prepared(); const auto before = graph;
    const auto invalid = selectType3ShapeOptimRegistryOperation(graph, registry, 999);
    CHECK(invalid.status == Status::InvalidPreparedIdentity);
    CHECK(!invalid.selectedClass); checkGraph(graph, before);
    CHECK(registry == originalRegistry);
}

TEST_CASE(type3_registry_two_operations_same_class_keep_p_distinct_from_linked_b)
{
    auto graph = prepared(); auto expected = graph;
    const Type3ShapeOptimPreparedRegistry registry{{u"ShapeOptim", 1}};
    selected(selectType3ShapeOptimRegistryOperation(graph, registry, 11), 1);
    checkGraph(graph, expected);
    selected(selectType3ShapeOptimRegistryOperation(graph, registry, 10), 1);
    expected.classes.at(1).currentOperation = 10;
    checkGraph(graph, expected);
    CHECK(graph.operations.at(10).property != graph.classes.at(1).linkedProperty);
    // CP459 tail sees P=20 and B=21, whose creator is a different class.
    std::vector<Type3ShapeOptimPropertyEvent> events;
    CHECK(projectType3ShapeOptimIdentityView(graph, 1, events)
          == Type3ShapeOptimIdentityResult::Complete);
    REQUIRE(events.size() == 5);
    CHECK_EQ(events[1].intValue, 42); CHECK_BITS(events[2].doubleValue, 3.25);
    CHECK_EQ(events[3].intValue, -99); CHECK_BITS(events[4].doubleValue, 0.125);
    expected.data.at(30) = {42, 3.25}; expected.data.at(31) = {-99, 0.125};
    checkGraph(graph, expected);
}

TEST_CASE(type3_registry_selection_preserves_cp462_initialized_and_projected_state)
{
    auto graph = prepared();
    std::vector<Type3ShapeOptimInitEvent> initEvents;
    REQUIRE(initializeType3ShapeOptimPreparedState(graph.classes.at(1).state, initEvents)
            == Type3ShapeOptimInitResult::Complete);
    Type3OptimizerScalarState source;
    source.value38 = 57; source.value40 = 6.25;
    source.numConRows = {{1, 2, 3, 4, 5.5, qt::L("row")}};
    source.regionRows = {{6, qt::L("region")}};
    projectType3ShapeOptimPreparedView(Type3OptimizerDefOptView(source), graph.classes.at(1).state);
    auto expected = graph; expected.classes.at(1).currentOperation = 10;
    const Type3ShapeOptimPreparedRegistry registry{{u"ShapeOptim", 1}};
    selected(selectType3ShapeOptimRegistryOperation(graph, registry, 10), 1);
    checkGraph(graph, expected);
    CHECK_EQ(initEvents.size(), 26u);
}

int main() { return testkit::run_all(); }
