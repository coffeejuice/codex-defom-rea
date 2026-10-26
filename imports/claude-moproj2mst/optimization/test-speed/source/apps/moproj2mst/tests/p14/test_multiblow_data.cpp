// P14: MultiBlowData ctor, setters, list resizing, accessors and the property tree (s1-04 7.3;
// setters from qt_metacall 0x14061ab80, tree 0x140612150). Pure data: no other packet involved.
#include <testkit/check.hpp>

#include "m2m/data/multiblow.hpp"
#include "m2m/mst/writer.hpp"
#include "m2m/props.hpp"
#include "m2m/props_base.hpp"
#include "m2m/model.hpp"
#include "m2m/xml.hpp"

using m2m::qt::L;
using m2m::qt::String;
using m2m::qt::Variant;
namespace d = m2m::data;

static Variant vd(double x)
{
    Variant v;
    v.kind = Variant::Double;
    v.d = x;
    return v;
}
static Variant vi(int x)
{
    Variant v;
    v.kind = Variant::Int;
    v.i = x;
    return v;
}
static Variant vs(const char* s)
{
    Variant v;
    v.kind = Variant::Text;
    v.s = L(s);
    return v;
}

TEST_CASE(ctor_defaults_si_and_english)
{
    d::MultiBlowData m;
    d::initMultiBlowData(m, 1);
    CHECK_BITS(m.heatingTemperature, 0.0);   // 950 then overwritten with 0
    CHECK_BITS(m.roomTemperature, 20.0);
    CHECK_BITS(m.heatingTime, 3600.0);
    CHECK_BITS(m.heatingTimeStep, 20.0);
    CHECK_BITS(m.transferInTime, 10.0);
    CHECK_BITS(m.transferOutTime, 10.0);
    CHECK_BITS(m.transferTimeStep, 1.0);
    CHECK_BITS(m.defaultDwellTime, 1.0);
    CHECK_BITS(m.movDir[1], -1.0);
    CHECK_EQ(int(m.dwell), 1);
    CHECK_EQ(int(m.relativeEnergy), 1);
    CHECK_EQ(int(m.equipment), 1);
    CHECK_EQ(int(m.hammer), 0);
    CHECK_EQ(m.heats, 0);
    CHECK(m.dwellTimeStep == L("0.1 sec"));
    CHECK(m.dwellHeatCoeff == L("1.0 N/sec/mm/C"));
    d::initMultiBlowData(m, 2);
    CHECK_BITS(m.roomTemperature, 68.0);
    CHECK(m.dwellHeatCoeff == L("0.0003 Btu/sec/in^2/F"));
}

TEST_CASE(props_movement_default_by_dimension)
{
    m2m::MultiBlowProps p2, p3;
    d::initMultiBlowProps(p2, 2, 1);
    d::initMultiBlowProps(p3, 3, 1);
    CHECK_BITS(p2.data->movDir[0], 0.0);
    CHECK_BITS(p2.data->movDir[1], -1.0);
    CHECK_BITS(p2.data->movDir[2], 0.0);
    CHECK_BITS(p3.data->movDir[1], 0.0);
    CHECK_BITS(p3.data->movDir[2], -1.0);
}

TEST_CASE(num_heats_and_blows_grow_with_defaults)
{
    d::MultiBlowData m;
    d::initMultiBlowData(m, 1);
    d::mbSetNumHeats(m, 2);
    CHECK_EQ(m.heats, 2);
    CHECK_EQ(int(m.blows.size()), 2);
    CHECK_EQ(m.blows[0], 1);
    CHECK_EQ(m.blows[1], 1);
    CHECK_BITS(m.temperature[1], 100.0);
    d::mbSetNumBlows(m, 1, 3);
    CHECK_EQ(d::mbBlows(m, 2), 3);
    CHECK_BITS(d::mbEnergy(m, 2, 3), 100.0);
    CHECK_BITS(d::mbEfficiency(m, 2, 3), 1.0);
    CHECK_BITS(d::mbDwellTime(m, 2, 3), 1.0);
    CHECK(!d::mbReheat(m, 2, 3));
    CHECK(!d::mbFlip(m, 2, 3));
    d::BlowRotation r = d::mbRotation(m, 2, 3);
    CHECK_EQ(int(r.selfCenter), 1);
    CHECK_BITS(r.angle, 0.0);
    // out of range -> 0 / false, the record defaults
    CHECK_BITS(d::mbEnergy(m, 3, 1), 0.0);
    CHECK_BITS(d::mbEnergy(m, 2, 4), 0.0);
    CHECK_EQ(d::mbBlows(m, 0), 0);
    CHECK_EQ(int(d::mbRotation(m, 0, 1).selfCenter), 1);
}

TEST_CASE(num_heats_shrink_keeps_stale_blow_counts)
{
    d::MultiBlowData m;
    d::initMultiBlowData(m, 1);
    d::mbSetNumHeats(m, 3);
    d::mbSetNumBlows(m, 2, 4);
    d::mbSetNumHeats(m, 1);
    CHECK_EQ(m.heats, 1);
    CHECK_EQ(int(m.energy.size()), 1);
    CHECK_EQ(int(m.temperature.size()), 1);
    CHECK_EQ(int(m.blows.size()), 3);        // +0x148 is not shrunk by 0x1406140c0
    // growing again overwrites the stale entry with 0 and then NumBlows(1)
    d::mbSetNumHeats(m, 2);
    CHECK_EQ(int(m.blows.size()), 4);        // appends 2, then blows[1] = 0 -> 1
    CHECK_EQ(m.blows[1], 1);
    CHECK_EQ(m.blows[3], 2);
    CHECK_EQ(int(m.energy[1].size()), 1);
}

TEST_CASE(num_blows_shrink)
{
    d::MultiBlowData m;
    d::initMultiBlowData(m, 1);
    d::mbSetNumHeats(m, 1);
    d::mbSetNumBlows(m, 0, 4);
    d::mbSetBlow(m, L("BlowEnergy"), 0, 3, vd(55.0));
    d::mbSetNumBlows(m, 0, 2);
    CHECK_EQ(d::mbBlows(m, 1), 2);
    CHECK_EQ(int(m.energy[0].size()), 2);
    CHECK_EQ(int(m.rotAxis[2][0].size()), 2);
}

TEST_CASE(scalar_setters)
{
    d::MultiBlowData m;
    d::initMultiBlowData(m, 1);
    CHECK(d::mbSetScalar(m, L("HeatingTime"), vd(12.5)));
    CHECK_BITS(m.heatingTime, 12.5);
    CHECK(d::mbSetScalar(m, L("TransferInTime"), vd(3.0)));
    CHECK_BITS(m.transferInTime, 3.0);
    CHECK(d::mbSetScalar(m, L("HammerCounterblow"), vd(2.9)));
    CHECK_EQ(m.hammerCounterblow, 2);        // (int)toDouble
    CHECK(d::mbSetScalar(m, L("UseHeating"), vs("true")));
    CHECK_EQ(int(m.heating), 1);
    CHECK(d::mbSetScalar(m, L("UseHeating"), vs("false")));
    CHECK_EQ(int(m.heating), 0);
    CHECK(d::mbSetScalar(m, L("DwellTimeStep"), vs("0.2 sec")));
    CHECK(m.dwellTimeStep == L("0.2 sec"));
    CHECK(d::mbSetScalar(m, L("Equipment"), vs("0")));
    CHECK_EQ(int(m.equipment), 0);
    CHECK_EQ(int(m.hammer), 1);
    CHECK(d::mbSetScalar(m, L("NumHeats"), vi(2)));
    CHECK_EQ(m.heats, 2);
    CHECK(!d::mbSetScalar(m, L("NoSuchProperty"), vd(1.0)));
}

TEST_CASE(heat_ok_and_dwell_on)
{
    d::MultiBlowData m;
    d::initMultiBlowData(m, 1);
    d::mbSetNumHeats(m, 1);
    d::mbSetNumBlows(m, 0, 2);
    CHECK(d::mbDwellOn(m, 1, 1));            // dwell on, dwell time 1.0
    d::mbSetBlow(m, L("BlowDwellTime"), 0, 1, vd(0.0));
    CHECK(!d::mbDwellOn(m, 1, 2));
    CHECK(!d::mbHeatOk(m, 1, 1));            // heating off
    m.heating = 1;
    CHECK(!d::mbHeatOk(m, 1, 1));            // neither reheat nor adaptive
    d::mbSetBlow(m, L("BlowReheat"), 0, 0, vs("true"));
    CHECK(d::mbHeatOk(m, 1, 1));
    m.initTemp = 1;
    CHECK(!d::mbHeatOk(m, 1, 1));            // NDTMP blocks heating
    m.initTemp = 0;
    m.transferInTime = m.heatingTime = m.transferOutTime = 0.0;
    CHECK(!d::mbHeatOk(m, 1, 1));
}

TEST_CASE(property_tree_shape)
{
    auto t2 = d::multiBlowPropTree(2);
    REQUIRE(t2.size() == 1);
    CHECK(t2[0].name == L("MultiBlowForging2D"));
    CHECK(d::multiBlowPropTree(3)[0].name == L("MultiBlowForging3D"));   // no conversion: own name
    const auto& c = t2[0].children;
    const char* order[] = {"EquipmentType", "EquipmentSpec", "MovementSpec", "Init", "Heating",
                           "AdaptiveReheat", "Dwell", "Interobject", "MultiBlow", "Heat"};
    REQUIRE(c.size() == 10);
    for (int i = 0; i < 10; ++i) CHECK(c[static_cast<size_t>(i)].name == L(order[i]));
    const auto& heat = c[9].children;
    REQUIRE(heat.size() == 3);
    CHECK(heat[0].name == L("Temperature"));
    CHECK(heat[1].name == L("NumBlows"));
    CHECK(heat[2].name == L("Blow"));
    CHECK_EQ(int(heat[2].children.size()), 13);
    CHECK(c[4].children[3].name == L("TransferInTime"));
    CHECK(c[4].children[4].name == L("HeatingTime"));
}

// Walk a name path in the props tree, collecting the ancestors as P02's applyProperty would.
static m2m::PropNode* walk(m2m::MultiBlowProps& p, std::vector<const char*> names,
                           std::vector<m2m::PropNode*>& anc, int pick = 0)
{
    anc.clear();
    std::vector<m2m::PropNode>* level = &p.tree;
    m2m::PropNode* cur = nullptr;
    for (size_t k = 0; k < names.size(); ++k) {
        cur = nullptr;
        int seen = 0;
        for (auto& n : *level)
            if (n.name == L(names[k]) && (k + 2 != names.size() || seen++ == pick)) {
                cur = &n;
                break;
            }
        if (!cur) return nullptr;
        if (k + 1 < names.size()) {
            anc.push_back(cur);
            level = &cur->children;
        }
    }
    return cur;
}

TEST_CASE(apply_value_by_node_index)
{
    m2m::MultiBlowProps p;
    d::initMultiBlowProps(p, 2, 0);
    p.tree = d::multiBlowPropTree(2);
    d::mbSetNumHeats(*p.data, 1);           // the tree's default NumHeats(1)
    std::vector<m2m::PropNode*> anc;
    m2m::PropNode* n = walk(p, {"MultiBlowForging2D", "MultiBlow", "NumHeats"}, anc);
    REQUIRE(n);
    p.applyValue(*n, anc, vi(2));
    CHECK_EQ(p.data->heats, 2);
    int heats = 0;
    for (auto& c : p.tree[0].children)
        if (c.name == L("Heat")) {
            CHECK_EQ(c.index, heats);
            ++heats;
        }
    CHECK_EQ(heats, 2);
    // second heat: NumBlows 3 -> blows 0..2 under that heat
    m2m::PropNode* heat2 = nullptr;
    for (auto& c : p.tree[0].children)
        if (c.name == L("Heat") && c.index == 1) heat2 = &c;
    REQUIRE(heat2);
    anc = {&p.tree[0], heat2};
    p.applyValue(heat2->children[1], anc, vi(3));
    CHECK_EQ(d::mbBlows(*p.data, 2), 3);
    CHECK_EQ(int(heat2->children.size()), 5);   // Temperature, NumBlows, 3 blows
    CHECK_EQ(heat2->children[4].index, 2);
    p.applyValue(heat2->children[0], anc, vd(1100.0));
    CHECK_BITS(p.data->temperature[1], 1100.0);
    // blow 3 of heat 2: energy
    m2m::PropNode* blow3 = &heat2->children[4];
    anc = {&p.tree[0], heat2, blow3};
    p.applyValue(blow3->children[0], anc, vd(80.0));
    CHECK_BITS(d::mbEnergy(*p.data, 2, 3), 80.0);
    CHECK_BITS(d::mbEnergy(*p.data, 1, 1), 100.0);
    // shrink heats back to 1: data and tree
    n = walk(p, {"MultiBlowForging2D", "MultiBlow", "NumHeats"}, anc);
    REQUIRE(n);
    p.applyValue(*n, anc, vi(1));
    CHECK_EQ(p.data->heats, 1);
    heats = 0;
    for (auto& c : p.tree[0].children) heats += c.name == L("Heat");
    CHECK_EQ(heats, 1);
    // scalar
    n = walk(p, {"MultiBlowForging2D", "Heating", "HeatingTime"}, anc);
    REQUIRE(n);
    p.applyValue(*n, anc, vd(30.0));
    CHECK_BITS(p.data->heatingTime, 30.0);
}

TEST_CASE(make_props_with_base_groups)
{
    auto cls = m2m::mst::makeMultiBlow(false);
    auto pp = cls->makeProps(0);
    auto* p = dynamic_cast<m2m::MultiBlowProps*>(pp.get());
    REQUIRE(p);
    CHECK(p->tree[0].name == L("MultiBlowForging3D"));
    CHECK_EQ(p->data->heats, 1);
    CHECK_EQ(d::mbBlows(*p->data, 1), 1);
    CHECK(m2m::base::testGroup(*p) != nullptr);
    bool friction = false;
    for (auto& g : p->tree[0].children)
        if (g.name == L("Interobject"))
            for (auto& c : g.children) friction = friction || c.name == L("FrictionType");
    CHECK(friction);
    CHECK_BITS(p->data->movDir[2], -1.0);
}

static m2m::String rootOf(m2m::mst::OpClass& c)
{
    return c.makeProps(0)->tree[0].name;
}

TEST_CASE(root_name_first_wins_per_conversion)
{
    auto c2 = m2m::mst::makeMultiBlow(true);
    auto c3 = m2m::mst::makeMultiBlow(false);
    {   // 3D first: both get the 3D name (a fresh GUI)
        m2m::Project p;
        m2m::ProjectScope s(p);
        CHECK(rootOf(*c3) == L("MultiBlowForging3D"));
        CHECK(rootOf(*c2) == L("MultiBlowForging3D"));
    }
    {   // 2D first
        m2m::Project p;
        m2m::ProjectScope s(p);
        CHECK(rootOf(*c2) == L("MultiBlowForging2D"));
        CHECK(rootOf(*c3) == L("MultiBlowForging2D"));
    }
    {   // a 3D-only file saved by a 2D-first session (75_B_MB3): a fresh GUI still names the root
        // after its own first props, and the saved root name is reported by savedMultiBlowRoot
        m2m::Project p;
        p.dom = std::make_shared<m2m::xml::Document>();
        m2m::String err;
        int line = 0, col = 0;
        REQUIRE(m2m::xml::setContent(*p.dom,
                                     "<Project><Operation_List><Operation><Id>3</Id><Properties Number=\"1\">"
                                     "<Property><Name>MultiBlowForging2D</Name></Property></Properties>"
                                     "</Operation></Operation_List></Project>",
                                     &err, &line, &col));
        m2m::ProjectScope s(p);
        CHECK(rootOf(*c3) == L("MultiBlowForging3D"));
        CHECK(d::savedMultiBlowRoot(p, 3) == L("MultiBlowForging2D"));
        CHECK(d::savedMultiBlowRoot(p, 4).isNull());
    }
    {   // no MultiBlow in the file: the first props' own name; the state stays unset until then
        m2m::Project p;
        p.dom = std::make_shared<m2m::xml::Document>();
        m2m::String err;
        int line = 0, col = 0;
        REQUIRE(m2m::xml::setContent(*p.dom, "<Project><Name>Forging</Name></Project>", &err, &line, &col));
        m2m::ProjectScope s(p);
        CHECK(!p.multiBlowRootSet);
        CHECK(rootOf(*c3) == L("MultiBlowForging3D"));
        CHECK(p.multiBlowRootSet);
    }
    CHECK(m2m::currentProject() == nullptr);
}

// ParseProperties 0x140094f70 calls (props+0x10)->vt[+0x8] = 0x14060b950 before the saved values:
// relative energy (ctor 1) and adaptive reheat end up false unless the file sets them (oracle B
// 75F_B_MB3: Properties Number="0" -> DOESCL 100; 92_B_MB2REL: UseRelativeEnergy true kept).
static void parse(m2m::Operation& op, const char* xml)
{
    m2m::xml::Document doc;
    m2m::String err;
    int line = 0, col = 0;
    REQUIRE(m2m::xml::setContent(doc, xml, &err, &line, &col));
    m2m::Project p;
    m2m::ProjectScope s(p);
    m2m::Ctx ctx;
    m2m::parseProperties(p, *doc.root, op, ctx);
}

TEST_CASE(parse_properties_clears_relative_energy_first)
{
    auto cls = m2m::mst::makeMultiBlow(true);
    m2m::Operation op;
    {
        m2m::Project p;
        m2m::ProjectScope s(p);
        op.props = cls->makeProps(0);
    }
    auto* mb = dynamic_cast<m2m::MultiBlowProps*>(op.props.get());
    REQUIRE(mb);
    CHECK_EQ(int(mb->data->relativeEnergy), 1);   // ctor 0x140618d20
    mb->data->adaptiveReheat = 1;
    parse(op, "<Properties Number=\"0\"/>");
    CHECK_EQ(int(mb->data->relativeEnergy), 0);
    CHECK_EQ(int(mb->data->adaptiveReheat), 0);
    CHECK_EQ(int(mb->data->dwell), 1);            // other ctor values stay
    parse(op, "<Properties Number=\"1\"><Property><Name>MultiBlowForging2D</Name><Property>"
              "<Name>MultiBlow</Name><Property><Name>UseRelativeEnergy</Name><TypeName>bool</TypeName>"
              "<Value>true</Value></Property></Property></Property></Properties>");
    CHECK_EQ(int(mb->data->relativeEnergy), 1);
}

int main()
{
    return testkit::run_all();
}
