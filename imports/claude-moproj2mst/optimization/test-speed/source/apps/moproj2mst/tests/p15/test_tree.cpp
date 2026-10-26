// ctest-labels: m2m-p15
// ctest-standalone: src/ops/express_tree.cpp src/ops/props_base.cpp src/core/basic.cpp
// Tier-1 test of the FormingExpress property setters (0x1414645a0 slots, 0x14142d610 setNumHeats,
// 0x14142e260 setNumBlows): list shapes and defaults, the stale +0x128 entries, the scalar map.
#include <testkit/check.hpp>

#include <string>

#include "ops/express.hpp"

using namespace m2m;
using namespace m2m::data;

// QString number helpers the setters use (P04 owns the real ones).
namespace m2m::qt {
double toDouble(const String& s, bool* ok)
{
    std::string a = s.utf8();
    if (ok) *ok = true;
    return std::stod(a);
}
int toInt(const String& s, bool* ok)
{
    if (ok) *ok = true;
    return std::stoi(s.utf8());
}
String number(int v)
{
    std::string a = std::to_string(v);
    return String(std::u16string(a.begin(), a.end()));
}
}  // namespace m2m::qt

namespace m2m {
void Props::postLoad(Project&, Ctx&) {}
void ExpressProps::postLoad(Project&, Ctx&) {}
}  // namespace m2m

namespace {
qt::Variant D(double d)
{
    qt::Variant v;
    v.kind = qt::Variant::Double;
    v.d = d;
    return v;
}
qt::Variant I(int i)
{
    qt::Variant v;
    v.kind = qt::Variant::Int;
    v.i = i;
    return v;
}
qt::Variant T(const char* s)
{
    qt::Variant v;
    v.kind = qt::Variant::Text;
    v.s = qt::L(s);
    return v;
}
}  // namespace

TEST_CASE(num_heats_grow_and_blows)
{
    FormingExpressData d;
    initFormingExpressData(d, 1);
    feSetNumHeats(d, 2);
    CHECK_EQ(d.blows, 2);
    // per heat: steps = 1 after setNumBlows(k, 1); +0x128 got "1" appended first, then [k] = 0
    CHECK_EQ(static_cast<int>(d.steps.size()), 2);
    CHECK_EQ(d.steps[0], 1);
    CHECK_EQ(d.steps[1], 1);
    CHECK_EQ(static_cast<int>(d.stroke.size()), 2);
    CHECK(d.stroke[1].size() == 1 && d.stroke[1][0] == 100.0);
    CHECK(d.l158[0][0] == 1.0);
    CHECK(d.gap[0][0] == 1.0);                     // DW by default
    CHECK(d.drop[0][0] == 0 && d.flip[0][0] == 0);
    CHECK(d.temperature.size() == 2 && d.temperature[1] == 100.0);
    feSetNumBlows(d, 1, 3);
    CHECK_EQ(d.steps[1], 3);
    CHECK_EQ(static_cast<int>(d.gap[1].size()), 3);
    feSetNumBlows(d, 1, 2);
    CHECK_EQ(d.steps[1], 2);
    CHECK_EQ(static_cast<int>(d.stroke[1].size()), 2);
}

TEST_CASE(num_heats_shrink_keeps_stale_steps)
{
    FormingExpressData d;
    initFormingExpressData(d, 1);
    feSetNumHeats(d, 3);
    feSetNumHeats(d, 1);
    CHECK_EQ(d.blows, 1);
    CHECK_EQ(static_cast<int>(d.stroke.size()), 1);
    CHECK_EQ(static_cast<int>(d.temperature.size()), 1);
    CHECK_EQ(static_cast<int>(d.steps.size()), 3);  // never popped
    feSetNumHeats(d, 2);                            // heat 1 again: [1] = 0 then setNumBlows -> 1
    CHECK_EQ(d.steps[1], 1);
    CHECK_EQ(static_cast<int>(d.steps.size()), 4);
}

TEST_CASE(blow_setters_and_scalars)
{
    FormingExpressData d;
    initFormingExpressData(d, 1);
    CHECK(feSetScalar(d, qt::L("NumHeats"), I(1)));
    feSetNumBlows(d, 0, 2);
    CHECK(feSetBlow(d, qt::L("BlowEnergy"), 0, 1, D(55.0)));
    CHECK(feSetBlow(d, qt::L("BlowDwellTime"), 0, 0, T("0")));
    CHECK(feSetBlow(d, qt::L("BlowReheat"), 0, 1, T("true")));
    CHECK(feSetBlow(d, qt::L("BlowFlip"), 0, 1, T("False")));
    CHECK(feSetBlow(d, qt::L("BlowFlip"), 5, 9, D(1.0)));   // out of range: ignored
    CHECK(!feSetBlow(d, qt::L("Temperature"), 0, 0, D(1.0)));
    CHECK(d.stroke[0][1] == 55.0);
    CHECK(d.gap[0][0] == 0.0);
    CHECK(d.drop[0][1] == 1);
    CHECK(d.flip[0][1] == 0);
    feSetTemperature(d, 0, D(1100.0));
    feSetTemperature(d, 3, D(1.0));
    CHECK(d.temperature[0] == 1100.0);
    CHECK(feSetScalar(d, qt::L("Equipment"), T("3")));
    CHECK(feSetScalar(d, qt::L("UseBlowTable"), T("true")));
    CHECK(feSetScalar(d, qt::L("TransferTime"), D(12.5)));
    CHECK(feSetScalar(d, qt::L("DwellHeatCoeff"), T("2 N/sec/mm/C")));
    CHECK(feSetScalar(d, qt::L("MovDirY"), D(-1.0)));
    CHECK(!feSetScalar(d, qt::L("StrokeProfile"), D(0.0)));
    CHECK_EQ(d.type, 3);
    CHECK(d.express == 1);
    CHECK(d.toTime == 12.5);
    CHECK(d.dwIhtc.utf8() == "2 N/sec/mm/C");
    CHECK(d.dir[1] == -1.0);
}

TEST_CASE(apply_value_tree)
{
    FormingExpressProps p;
    initFormingExpressData(*p.fe, 0);
    PropNode group;
    group.name = qt::L("MultiBlow");
    PropNode nh;
    nh.name = qt::L("NumHeats");
    group.children.push_back(nh);
    std::vector<PropNode*> anc{&group};
    p.applyValue(group.children[0], anc, I(2));
    CHECK_EQ(p.fe->blows, 2);
    REQUIRE(group.children.size() == 3);
    PropNode& heat1 = group.children[2];
    CHECK(heat1.name.utf8() == "Heat" && heat1.index == 1);
    REQUIRE(heat1.children.size() == 3);
    // NumBlows of heat 1 -> 3 blows: tree and data
    std::vector<PropNode*> a2{&group, &heat1};
    p.applyValue(heat1.children[1], a2, T("3"));
    CHECK_EQ(p.fe->steps[1], 3);
    REQUIRE(heat1.children.size() == 5);
    PropNode& blow2 = heat1.children[4];
    CHECK(blow2.index == 2);
    std::vector<PropNode*> a3{&group, &heat1, &blow2};
    p.applyValue(blow2.children[2], a3, D(0.0));       // BlowDwellTime of heat 1, blow 2
    CHECK(p.fe->gap[1][2] == 0.0);
    CHECK(p.fe->gap[1][1] == 1.0);
    p.applyValue(heat1.children[0], a2, D(1250.0));    // Temperature of heat 1
    CHECK(p.fe->temperature[1] == 1250.0);
    p.applyValue(heat1.children[1], a2, I(1));         // back to one blow
    CHECK_EQ(static_cast<int>(heat1.children.size()), 3);
    CHECK_EQ(static_cast<int>(p.fe->gap[1].size()), 1);
    PropNode ub;
    ub.name = qt::L("UseBlowTable");
    p.applyValue(ub, anc, T("true"));
    CHECK(p.fe->express == 1);
    p.applyValue(group.children[0], anc, I(1));        // NumHeats 1: heat 1 removed
    CHECK_EQ(static_cast<int>(group.children.size()), 2);
    CHECK_EQ(p.fe->blows, 1);
}

TEST_CASE(default_tree_and_equipment_rebuild)
{
    FormingExpressData d;
    initFormingExpressData(d, 1);
    feSetNumHeats(d, 1);
    auto t = formingExpressPropTree(d, true);
    REQUIRE(t.size() == 1);
    PropNode& root = t[0];
    CHECK(root.name.utf8() == "SfOprFormingExpress2D");
    std::string names;
    for (auto& c : root.children) names += c.name.utf8() + ",";
    CHECK(names == "EquipmentType,EquipmentSpec,MovementSpec,MultiBlow,Heat,Dwell,Heating,Init,Interobject,");
    CHECK(root.children[1].children.size() == 4);   // type 2, MechPressType 0: ..., ConnectingRodLength
    CHECK(root.children[1].children[3].name.utf8() == "ConnectingRodLength");
    PropNode& heat = root.children[4];
    CHECK(heat.index == 0 && heat.children.size() == 3 && heat.children[2].name.utf8() == "Blow");
    // Equipment = 3 (hammer) rebuilds EquipmentSpec
    FormingExpressProps p;
    *p.fe = d;
    p.tree = t;
    std::vector<PropNode*> anc{&p.tree[0], &p.tree[0].children[0]};
    p.applyValue(p.tree[0].children[0].children[0], anc, I(3));
    CHECK_EQ(p.fe->type, 3);
    REQUIRE(p.tree[0].children[1].children.size() == 3);
    CHECK(p.tree[0].children[1].children[0].name.utf8() == "HammerEnergy");
}

int main() { return testkit::run_all(); }
