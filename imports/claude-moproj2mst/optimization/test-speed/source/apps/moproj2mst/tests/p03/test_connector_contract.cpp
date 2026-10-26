// Original-bound diagram admission: 0x14015ef80 -> 0x140161470 -> 0x140074f50.
// See analysis/connector-type-contract.json and connector-admission-original.json
// in the takeover root. The original admission bytes were executed before this
// test was added; its type predicate was controlled, not an original GUI capture.
#include <testkit/check.hpp>

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
