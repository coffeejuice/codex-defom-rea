#include <testkit/check.hpp>

#include "../../src/ops/type3_shapeoptim_property_tail.hpp"

using namespace m2m::mst::detail;

namespace {
using Kind = Type3ShapeOptimPropertyEventKind;
using Result = Type3ShapeOptimPropertyTailResult;
using Node = Type3ShapeOptimPropertyNode;
using Events = std::vector<Type3ShapeOptimPropertyEvent>;

void checkKinds(const Events& events, const std::vector<Kind>& expected)
{
    REQUIRE(events.size() == expected.size());
    for (std::size_t i = 0; i < expected.size(); ++i)
        CHECK(events[i].kind == expected[i]);
}
}  // namespace

TEST_CASE(type3_property_tail_empty_and_nonmatching_preserve_seeded_data)
{
    const Type3ShapeOptimPropertyValues current{42, -3.25};
    Type3ShapeOptimPropertyValues data{77, 8.5};
    Type3ShapeOptimPropertyCast props{{}, &data};
    Events events;
    CHECK(projectType3ShapeOptimPropertyTail(current, &props, nullptr, events) == Result::Complete);
    CHECK(events.empty());
    props.properties = {{"Other", {{"MaxSims", {}}}}, {"StopCritGroup", {{"unknown", {}}}}};
    CHECK(projectType3ShapeOptimPropertyTail(current, &props, nullptr, events) == Result::Complete);
    checkKinds(events, {Kind::DebugTopDisplayName, Kind::DebugTopDisplayName});
    CHECK_EQ(events[0].displayName, "Other");
    CHECK_EQ(events[1].displayName, "StopCritGroup");
    CHECK_EQ(data.maxSims, 77); CHECK_BITS(data.minObjective, 8.5);
}

TEST_CASE(type3_property_tail_logs_tops_before_ordered_duplicate_writes)
{
    const Type3ShapeOptimPropertyValues current{-42, -3.25};
    Type3ShapeOptimPropertyValues data{7, 8.5};
    Type3ShapeOptimPropertyCast props{{
        {"StopCritGroup", {{"MinObjFuncVal", {}}, {"MaxSims", {}}, {"MaxSims", {}}}},
        {"Other", {}},
        {"StopCritGroup", {{"MaxSims", {}}, {"MinObjFuncVal", {}}}}
    }, &data};
    Events events{{Kind::DebugTopDisplayName, "seed"}};
    CHECK(projectType3ShapeOptimPropertyTail(current, &props, nullptr, events) == Result::Complete);
    checkKinds(events, {Kind::DebugTopDisplayName, Kind::DebugTopDisplayName,
        Kind::SetMinObjective, Kind::SetMaxSims, Kind::SetMaxSims,
        Kind::DebugTopDisplayName, Kind::DebugTopDisplayName,
        Kind::SetMaxSims, Kind::SetMinObjective});
    CHECK_EQ(events[0].displayName, "seed");
    CHECK_EQ(events[1].displayName, "StopCritGroup");
    CHECK_EQ(events[5].displayName, "Other");
    CHECK_EQ(events[6].displayName, "StopCritGroup");
    for (auto i : {3, 4, 7}) CHECK_EQ(events[i].intValue, -42);
    CHECK_BITS(events[2].doubleValue, -3.25); CHECK_BITS(events[8].doubleValue, -3.25);
    CHECK_EQ(data.maxSims, -42); CHECK_BITS(data.minObjective, -3.25);
}

TEST_CASE(type3_property_tail_matches_exact_prepared_names_and_never_descends)
{
    Type3ShapeOptimPropertyValues data{77, 8.5};
    Type3ShapeOptimPropertyCast props{{
        {"stopCritGroup", {{"MaxSims", {}}}},
        {"StopCritGroup1", {{"MaxSims", {}}}},
        {"StopCritGroup", {{"maxSims", {}}, {"MaxSims1", {}},
            {"minObjFuncVal", {}}, {"MinObjFuncVal1", {}},
            {"Nested", {{"MaxSims", {}}, {"MinObjFuncVal", {}}}}}}
    }, &data};
    Events events;
    CHECK(projectType3ShapeOptimPropertyTail({42, -3.25}, &props, nullptr, events) == Result::Complete);
    checkKinds(events, {Kind::DebugTopDisplayName, Kind::DebugTopDisplayName, Kind::DebugTopDisplayName});
    CHECK_EQ(data.maxSims, 77); CHECK_BITS(data.minObjective, 8.5);
}

TEST_CASE(type3_property_tail_double_keeps_fraction_and_negative_zero)
{
    Type3ShapeOptimPropertyValues data{77, 8.5};
    Type3ShapeOptimPropertyCast props{{{"StopCritGroup", {{"MinObjFuncVal", {}}}}}, &data};
    for (double value : {-0.0, -3.25, 0.125}) {
        Events events;
        CHECK(projectType3ShapeOptimPropertyTail({42, value}, &props, nullptr, events) == Result::Complete);
        CHECK_EQ(data.maxSims, 77); CHECK_BITS(data.minObjective, value);
        REQUIRE(events.size() == 2); CHECK_BITS(events[1].doubleValue, value);
    }
}

TEST_CASE(type3_property_tail_refresh_has_independent_source_and_distinct_or_shared_sink)
{
    const Type3ShapeOptimPropertyValues current{42, -3.25}, linked{-99, 0.125};
    Type3ShapeOptimPropertyValues propertyData{7, 8.5}, refreshData{8, 9.5};
    Type3ShapeOptimPropertyCast props{{{"StopCritGroup", {{"MaxSims", {}}, {"MinObjFuncVal", {}}}}}, &propertyData};
    Type3ShapeOptimPropertyRefresh refresh{&linked, &refreshData};
    Events events;
    CHECK(projectType3ShapeOptimPropertyTail(current, &props, &refresh, events) == Result::Complete);
    checkKinds(events, {Kind::DebugTopDisplayName, Kind::SetMaxSims, Kind::SetMinObjective,
        Kind::RefreshMaxSims, Kind::RefreshMinObjective});
    CHECK_EQ(propertyData.maxSims, 42); CHECK_BITS(propertyData.minObjective, -3.25);
    CHECK_EQ(refreshData.maxSims, -99); CHECK_BITS(refreshData.minObjective, 0.125);
    CHECK_EQ(events[3].intValue, -99); CHECK_BITS(events[4].doubleValue, 0.125);
    events.clear(); refresh.data = &propertyData;
    CHECK(projectType3ShapeOptimPropertyTail(current, &props, &refresh, events) == Result::Complete);
    checkKinds(events, {Kind::DebugTopDisplayName, Kind::SetMaxSims, Kind::SetMinObjective,
        Kind::RefreshMaxSims, Kind::RefreshMinObjective});
    CHECK_EQ(propertyData.maxSims, -99); CHECK_BITS(propertyData.minObjective, 0.125);
    CHECK_EQ(events[1].intValue, 42); CHECK_BITS(events[2].doubleValue, -3.25);
}

TEST_CASE(type3_property_tail_absent_cast_still_refreshes_and_null_class_writes_nothing)
{
    const Type3ShapeOptimPropertyValues linked{-99, -0.0};
    Type3ShapeOptimPropertyValues data{7, 8.5};
    Type3ShapeOptimPropertyRefresh refresh{&linked, &data};
    Events events;
    CHECK(projectType3ShapeOptimPropertyTail({42, 3.25}, nullptr, &refresh, events) == Result::Complete);
    checkKinds(events, {Kind::RefreshMaxSims, Kind::RefreshMinObjective});
    CHECK_EQ(data.maxSims, -99); CHECK_BITS(data.minObjective, -0.0);
    events.clear(); refresh.linkedClass = nullptr; refresh.data = nullptr;
    CHECK(projectType3ShapeOptimPropertyTail({42, 3.25}, nullptr, &refresh, events) == Result::Complete);
    CHECK(events.empty());
    CHECK(projectType3ShapeOptimPropertyTail({42, 3.25}, nullptr, nullptr, events) == Result::Complete);
    CHECK(events.empty());
}

TEST_CASE(type3_property_tail_linked_source_alias_reads_after_property_writes)
{
    Type3ShapeOptimPropertyValues propertyData{7, 8.5}, refreshData{8, 9.5};
    Type3ShapeOptimPropertyCast props{{{"StopCritGroup", {{"MaxSims", {}}, {"MinObjFuncVal", {}}}}}, &propertyData};
    Type3ShapeOptimPropertyRefresh refresh{&propertyData, &refreshData};
    Events events;
    CHECK(projectType3ShapeOptimPropertyTail({42, -3.25}, &props, &refresh, events) == Result::Complete);
    checkKinds(events, {Kind::DebugTopDisplayName, Kind::SetMaxSims, Kind::SetMinObjective,
        Kind::RefreshMaxSims, Kind::RefreshMinObjective});
    CHECK_EQ(propertyData.maxSims, 42); CHECK_BITS(propertyData.minObjective, -3.25);
    CHECK_EQ(refreshData.maxSims, 42); CHECK_BITS(refreshData.minObjective, -3.25);
    CHECK_EQ(events[3].intValue, 42); CHECK_BITS(events[4].doubleValue, -3.25);
}

TEST_CASE(type3_property_tail_missing_property_sink_refuses_only_when_write_is_needed)
{
    Type3ShapeOptimPropertyCast props{{{"Other", {}}, {"StopCritGroup", {{"unknown", {}}}}}, nullptr};
    const Type3ShapeOptimPropertyValues linked{-99, 0.125};
    Type3ShapeOptimPropertyValues refreshData{7, 8.5};
    Type3ShapeOptimPropertyRefresh refresh{&linked, &refreshData};
    Events events;
    CHECK(projectType3ShapeOptimPropertyTail({42, 3.25}, &props, &refresh, events) == Result::Complete);
    checkKinds(events, {Kind::DebugTopDisplayName, Kind::DebugTopDisplayName,
        Kind::RefreshMaxSims, Kind::RefreshMinObjective});
    for (const char* match : {"MaxSims", "MinObjFuncVal"}) {
        props.properties[1].children.push_back({match, {}});
        props.properties.push_back({"Unvisited", {}});
        refreshData = {7, 8.5}; events.clear();
        CHECK(projectType3ShapeOptimPropertyTail({42, 3.25}, &props, &refresh, events) == Result::MissingPropertyData);
        checkKinds(events, {Kind::DebugTopDisplayName, Kind::DebugTopDisplayName});
        CHECK_EQ(refreshData.maxSims, 7); CHECK_BITS(refreshData.minObjective, 8.5);
        props.properties[1].children.pop_back(); props.properties.pop_back();
    }
}

TEST_CASE(type3_property_tail_missing_refresh_sink_preserves_completed_property_effects)
{
    const Type3ShapeOptimPropertyValues linked{-99, 0.125};
    Type3ShapeOptimPropertyValues data{7, 8.5};
    Type3ShapeOptimPropertyCast props{{{"StopCritGroup", {{"MaxSims", {}}, {"MinObjFuncVal", {}}}}}, &data};
    const Type3ShapeOptimPropertyRefresh refresh{&linked, nullptr};
    Events events;
    CHECK(projectType3ShapeOptimPropertyTail({42, 3.25}, &props, &refresh, events) == Result::MissingRefreshData);
    checkKinds(events, {Kind::DebugTopDisplayName, Kind::SetMaxSims, Kind::SetMinObjective});
    CHECK_EQ(data.maxSims, 42); CHECK_BITS(data.minObjective, 3.25);
}

int main() { return testkit::run_all(); }
