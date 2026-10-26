// Original-bound diagram admission: 0x14015ef80 -> 0x140161470 -> 0x140074f50.
// See analysis/connector-type-contract.json and connector-admission-original.json
// in the takeover root. The original admission bytes were executed before this
// test was added; its type predicate was controlled, not an original GUI capture.
#include <testkit/check.hpp>

#include <iterator>
#include <string>
#include <vector>

#include "m2m/model.hpp"
#include "m2m/mst/writer.hpp"
#include "m2m/xml.hpp"

using namespace m2m;
using m2m::qt::L;

namespace {

struct Item {
    const char* cls;
    int id;
    int target;
};

void parse(Project& p, const std::vector<Item>& items)
{
    makeRegistry(p);
    p.subs.push_back(makeSubProject(p, 0, 0));
    std::string source = "<Sub_Project Active=\"0\">";
    for (std::size_t i = 0; i < items.size(); ++i) {
        const Item& it = items[i];
        source += "<DiagramItem Operation_Id=\"" + std::to_string(it.id) +
                  "\"><ClassName>" + it.cls + "</ClassName><CellPos Row=\"0\" Col=\"" +
                  std::to_string(i) + "\"/><Connectors><Connections><ItemIndex>" +
                  std::to_string(it.target) + "</ItemIndex></Connections></Connectors></DiagramItem>";
    }
    source += "</Sub_Project>";
    xml::Document doc;
    String error;
    int line = 0, col = 0;
    REQUIRE(xml::setContent(doc, source, &error, &line, &col));
    REQUIRE(doc.root != nullptr);
    Ctx ctx;
    parseSubProject(p, *doc.root, ctx);
}

}  // namespace

TEST_CASE(original_connector_class_matrix)
{
    // Rows are source outputs; columns are target inputs. The converter class
    // has a 2D input and 3D output. Report accepts either dimensionality.
    const char* classes[] = {"SfOprForming2D", "SfOprForming", "SfOprReport", "SfOprConvert2Dto3D"};
    const bool accepts[4][4] = {
        {true, false, true, true},
        {false, true, true, false},
        {true, true, true, true},
        {false, true, true, false},
    };
    for (int source = 0; source < 4; ++source) {
        for (int target = 0; target < 4; ++target) {
            Project p;
            parse(p, {{classes[source], 11, 1}, {classes[target], 22, -1}});
            const auto& items = p.subs[0]->flow.items;
            REQUIRE(items.size() == 2);
            const bool linked = items[0]->next == items[1].get() && items[1]->prev == items[0].get();
            if (linked != accepts[source][target])
                testkit::fail(__FILE__, __LINE__, std::string(classes[source]) + " -> " + classes[target]);
            if (!accepts[source][target]) {
                CHECK(items[0]->next == nullptr);
                CHECK(items[1]->prev == nullptr);
            }
        }
    }
}

TEST_CASE(first_accepted_incoming_connection_is_retained)
{
    Project p;
    // Parse order, not operation ID order, selects the first incoming edge.
    parse(p, {{"SfOprForming", 91, 2}, {"SfOprForming", 7, 2}, {"SfOprForming", 30, -1}});
    const auto& items = p.subs[0]->flow.items;
    CHECK(items[0]->next == items[2].get());
    CHECK(items[1]->next == nullptr);
    CHECK(items[2]->prev == items[0].get());
    const auto order = mstOrder(*p.subs[0]);
    CHECK(order == std::vector<int>({91, 30}));
}

TEST_CASE(rejected_dimension_does_not_reserve_target)
{
    Project p;
    parse(p, {{"SfOprForming2D", 11, 2}, {"SfOprForming", 22, 2}, {"SfOprForming", 33, -1}});
    const auto& items = p.subs[0]->flow.items;
    CHECK(items[0]->next == nullptr);
    CHECK(items[1]->next == items[2].get());
    CHECK(items[2]->prev == items[1].get());
}

TEST_CASE(target_output_does_not_occupy_target_input)
{
    Project p;
    parse(p, {{"SfOprForming", 11, 2}, {"SfOprForming", 22, 0}, {"SfOprForming", 33, -1}});
    const auto& items = p.subs[0]->flow.items;
    CHECK(items[1]->next == items[0].get());
    CHECK(items[0]->prev == items[1].get());
    CHECK(items[0]->next == items[2].get());
    CHECK(mstOrder(*p.subs[0]) == std::vector<int>({22, 11, 33}));
}

TEST_CASE(convert_2d_to_3d_bridges_a_valid_chain)
{
    Project p;
    parse(p, {{"SfOprForming2D", 11, 1}, {"SfOprConvert2Dto3D", 22, 2}, {"SfOprForming", 33, -1}});
    CHECK(mstOrder(*p.subs[0]) == std::vector<int>({11, 22, 33}));
}

TEST_CASE(cycle_and_doe_output_have_original_combined_connectors)
{
    // Original init1.c: Cycle 0x14134d2e0 -> flags 0x301b;
    // DoeOutput 0x140e2df30 -> flags 0x20097. Both accept 2D and 3D.
    for (const char* bridge : {"SfOprCycle", "SfOprDoeOutput"}) {
        for (const char* dimension : {"SfOprForming2D", "SfOprForming"}) {
            Project p;
            parse(p, {{dimension, 11, 1}, {bridge, 22, 2}, {dimension, 33, -1},
                      {dimension, 44, 1}});
            const auto& items = p.subs[0]->flow.items;
            CHECK(items[0]->next == items[1].get());
            CHECK(items[1]->prev == items[0].get());
            CHECK(items[1]->next == items[2].get());
            CHECK(items[2]->prev == items[1].get());
            CHECK(items[3]->next == nullptr);
        }
    }
}

TEST_CASE(cogging_connects_to_doe_output_with_original_combined_input)
{
    // Retained B:120_B_DOE sub-project 1 requests operation 3 (Cogging) -> 6 (DoeOutput).
    // Cogging's 0x1a flags provide Control3d; DoeOutput's 0x20097 flags provide Control2d3d.
    Project p;
    parse(p, {{"SfOprCogging", 3, 1}, {"SfOprDoeOutput", 6, -1}});
    const auto& items = p.subs[0]->flow.items;
    REQUIRE(items.size() == 2);
    CHECK(items[0]->next == items[1].get());
    CHECK(items[1]->prev == items[0].get());
}

TEST_CASE(cogging_uses_original_3d_connectors)
{
    // Cogging flags 0x1a resolve to Control3d for both connector directions.
    Project threeDimensional;
    parse(threeDimensional, {{"SfOprForming", 11, 1}, {"SfOprCogging", 22, -1}});
    const auto& accepted = threeDimensional.subs[0]->flow.items;
    REQUIRE(accepted.size() == 2);
    CHECK(accepted[0]->next == accepted[1].get());
    CHECK(accepted[1]->prev == accepted[0].get());

    Project twoDimensional;
    parse(twoDimensional, { {"SfOprForming2D", 33, 1}, {"SfOprCogging", 44, -1} });
    const auto& rejected = twoDimensional.subs[0]->flow.items;
    REQUIRE(rejected.size() == 2);
    CHECK(rejected[0]->next == nullptr);
    CHECK(rejected[1]->prev == nullptr);
}

TEST_CASE(all_registered_connector_flags_and_roles_match_original)
{
    // Expectations are transcribed from the retained original init/constructor
    // contracts (see classinit.cpp @src references and connector-type-contract.json),
    // rather than calculated from the candidate's flag decoder.
    enum class Role { TwoD, ThreeD, Both };
    struct Expected {
        const char* key;
        int flags;
        Role input;
        Role output;
    };
    constexpr Expected expected[] = {
        {"SfOprForming", 0x1a, Role::ThreeD, Role::ThreeD},
        {"SfOprForming2D", 0x19, Role::TwoD, Role::TwoD},
        {"SfOprHeatTransfer", 0x1a, Role::ThreeD, Role::ThreeD},
        {"SfOprHeatTransfer2D", 0x19, Role::TwoD, Role::TwoD},
        {"SfOprMultiBlow3D", 0x2a, Role::ThreeD, Role::ThreeD},
        {"SfOprMultiBlow2D", 0x29, Role::TwoD, Role::TwoD},
        {"SfOprFormingExpress", 0x16, Role::ThreeD, Role::ThreeD},
        {"SfOprFormingExpress2D", 0x15, Role::TwoD, Role::TwoD},
        {"SfOprDistortion3D", 0x1a, Role::ThreeD, Role::ThreeD},
        {"SfOprDistortion2D", 0x19, Role::TwoD, Role::TwoD},
        {"SfOprHeatTransferExpress", 0x16, Role::ThreeD, Role::ThreeD},
        {"SfOprHeatTransferExpress2D", 0x15, Role::TwoD, Role::TwoD},
        {"SfOprHTFurnace", 0x1a, Role::ThreeD, Role::ThreeD},
        {"SfOprHeatTreat", 0x2a, Role::ThreeD, Role::ThreeD},
        {"SfOprHeatTreat2D", 0x29, Role::TwoD, Role::TwoD},
        {"SfOprCogging", 0x1a, Role::ThreeD, Role::ThreeD},
        {"SfOprPre3D", 0x5a, Role::ThreeD, Role::ThreeD},
        {"SfOprPre2D", 0x59, Role::TwoD, Role::TwoD},
        {"SfOprReport", 0x9b, Role::Both, Role::Both},
        {"SfOprShapeOptim", 0x40097, Role::Both, Role::Both},
        {"SfOprDoeOutput", 0x20097, Role::Both, Role::Both},
        {"SfOprConvert2Dto3D", 0x1100009d, Role::TwoD, Role::ThreeD},
        {"SfOprBoolean2D", 0x1000099, Role::TwoD, Role::TwoD},
        {"SfOprBoolean3D", 0x100009a, Role::ThreeD, Role::ThreeD},
        {"SfOprCycle", 0x301b, Role::Both, Role::Both},
        {"SfOprDiestress", 0xdf, Role::Both, Role::Both},
        {"SfOprDiestressBM2D", 0x4000009d, Role::TwoD, Role::TwoD},
        {"SfOprDiestressBM3D", 0x4000009e, Role::ThreeD, Role::ThreeD},
    };
    constexpr bool accepts[3][3] = {
        {true, false, true},
        {false, true, true},
        {true, true, true},
    };
    constexpr const char* representative[] = {
        "SfOprForming2D", "SfOprForming", "SfOprReport"
    };
    constexpr auto roleIndex = [](Role role) {
        return role == Role::TwoD ? 0 : role == Role::ThreeD ? 1 : 2;
    };
    constexpr auto roleName = [](Role role) -> const char* {
        return role == Role::TwoD ? "Control2d" :
               role == Role::ThreeD ? "Control3d" : "Control2d3d";
    };

    Project registryProject;
    makeRegistry(registryProject);
    REQUIRE(registryProject.registry.size() == std::size(expected));
    for (const Expected& item : expected) {
        const auto it = registryProject.registry.find(String::latin1(item.key));
        REQUIRE(it != registryProject.registry.end());
        REQUIRE(it->second != nullptr);
        CHECK_EQ(it->second->dimFlags, item.flags);
    }

    // Verify both input and output role resolution through the real parser.
    // The fixed matrix comes from the retained original connector contract:
    // rows are source outputs and columns are target inputs.
    for (const Expected& item : expected) {
        for (int role = 0; role != 3; ++role) {
            Project asSource;
            parse(asSource, {{item.key, 11, 1}, {representative[role], 22, -1}});
            const auto& sourceItems = asSource.subs[0]->flow.items;
            REQUIRE(sourceItems.size() == 2u);
            const bool sourceLinked = sourceItems[0]->next == sourceItems[1].get();
            const bool sourceExpected = accepts[roleIndex(item.output)][role];
            if (sourceLinked != sourceExpected)
                testkit::fail(__FILE__, __LINE__, std::string(item.key) + " output " +
                              roleName(item.output) + " -> target input " +
                              roleName(role == 0 ? Role::TwoD : role == 1 ? Role::ThreeD : Role::Both));

            Project asTarget;
            parse(asTarget, {{representative[role], 11, 1}, {item.key, 22, -1}});
            const auto& targetItems = asTarget.subs[0]->flow.items;
            REQUIRE(targetItems.size() == 2u);
            const bool targetLinked = targetItems[0]->next == targetItems[1].get();
            const bool targetExpected = accepts[role][roleIndex(item.input)];
            if (targetLinked != targetExpected)
                testkit::fail(__FILE__, __LINE__, std::string(representative[role]) + " output " +
                              roleName(role == 0 ? Role::TwoD : role == 1 ? Role::ThreeD : Role::Both) +
                              " -> " + item.key + " input " + roleName(item.input));
        }
    }
}

TEST_CASE(self_connection_uses_the_same_type_admission)
{
    Project ordinary;
    parse(ordinary, {{"SfOprForming", 11, 0}});
    const auto* item = ordinary.subs[0]->flow.items[0].get();
    CHECK(item->next == item);
    CHECK(item->prev == item);
    // Admission only: traversal of a cyclic graph is a separate contract.
    Project asymmetric;
    parse(asymmetric, {{"SfOprConvert2Dto3D", 22, 0}});
    CHECK(asymmetric.subs[0]->flow.items[0]->next == nullptr);
    CHECK(asymmetric.subs[0]->flow.items[0]->prev == nullptr);
}

int main()
{
    return testkit::run_all();
}
