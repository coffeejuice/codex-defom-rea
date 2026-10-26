// MultiBlowData: ctor, property setters and the accessors of the MultiBlow writer (P14).
// s1-04 7.3; setters from qt_metacall 0x14061ab80 (decompiles in gui_mo/out_p14/a3.c, a4.c).
#include <cstdint>
#include <cstring>
#include <string>

#include "m2m/data/multiblow.hpp"
#include "m2m/engine_multiblow.hpp"
#include "m2m/model.hpp"
#include "m2m/props_base.hpp"
#include "m2m/xml.hpp"

namespace m2m::data {

namespace {

// QVariant::toBool / toDouble / toInt / toString of the variant the property reader built
// (s1-01 6.3: double, int or the Value text). Qt 4.8 rules for a string variant: toBool is false
// for "", "0" and "false" (case-insensitive); toDouble/toInt parse with QString::toDouble/toInt.
bool vBool(const qt::Variant& v)
{
    switch (v.kind) {
    case qt::Variant::Double: return v.d != 0.0;
    case qt::Variant::Int: return v.i != 0;
    case qt::Variant::Text: {
        const std::u16string& s = v.s.u16();
        if (s.empty()) return false;
        if (s == u"0") return false;
        if (s.size() == 5) {
            std::u16string l;
            for (char16_t c : s) l += (c >= u'A' && c <= u'Z') ? char16_t(c - u'A' + u'a') : c;
            if (l == u"false") return false;
        }
        return true;
    }
    default: return false;
    }
}

double vDouble(const qt::Variant& v)
{
    switch (v.kind) {
    case qt::Variant::Double: return v.d;
    case qt::Variant::Int: return static_cast<double>(v.i);
    case qt::Variant::Text: return qt::toDouble(v.s);
    default: return 0.0;
    }
}

int vInt(const qt::Variant& v)
{
    switch (v.kind) {
    case qt::Variant::Double: return static_cast<int>(v.d);   // QVariant(double).toInt() truncates
    case qt::Variant::Int: return v.i;
    case qt::Variant::Text: return qt::toInt(v.s);
    default: return 0;
    }
}

String vString(const qt::Variant& v)
{
    switch (v.kind) {
    case qt::Variant::Text: return v.s;
    case qt::Variant::Int: return qt::number(v.i);
    case qt::Variant::Double:   // not produced for the two QString setters (TypeName doubleUnits)
    default: return String();
    }
}

template <class T>
T at2(const std::vector<std::vector<T>>& l, int i, int j)
{
    if (i < 1 || i > static_cast<int>(l.size())) return T{};
    const auto& in = l[static_cast<size_t>(i - 1)];
    if (j < 1 || j > static_cast<int>(in.size())) return T{};
    return in[static_cast<size_t>(j - 1)];
}

template <class T>
void popLast(std::vector<T>& l)
{
    if (!l.empty()) l.pop_back();   // QList::removeAt(count - 1); never called on an empty list
}

}  // namespace

// @src 0x140618d20
void initMultiBlowData(MultiBlowData& d, int unit)
{
    d = MultiBlowData{};
    d.f08 = 0;
    d.hammer = 0;
    d.equipment = 1;
    d.hammerEnergy = d.hammerMass = d.hammerEfficiency = 0.0;
    d.hammerCounterblow = 0;
    d.screwEnergy = d.screwEfficiency = d.momentOfInertia = d.leadScrewPitch = 0.0;
    d.movDir[0] = 0.0;
    d.movDir[1] = -1.0;                       // 0x1421d3c90
    d.movDir[2] = 0.0;
    d.workpieceRotation = d.initTemp = d.initStrain = d.efficiencyFunction = 0;
    d.heating = 0;
    if (unit == 2) {
        d.heatingTemperature = 1800.0;        // 0x1424bb078
        d.roomTemperature = 68.0;             // 0x1422ad710
    } else {
        d.heatingTemperature = 950.0;         // 0x1424bb070
        d.roomTemperature = 20.0;             // 0x1421ce520
    }
    d.heatingTemperature = 0.0;               // overwritten right after (as in the original)
    d.heatingTime = 3600.0;                   // 0x1424bb068
    d.heatingTimeStep = 20.0;                 // 0x1421ce520
    d.transferInTime = 10.0;                  // 0x142142078
    d.transferOutTime = 10.0;
    d.transferTimeStep = 1.0;                 // 0x1421249e0
    d.adaptiveReheat = 0;
    d.triggerHeatTemp = d.triggerHeatTempUpper = d.heatStopTemp = 0.0;
    d.dwell = 1;
    d.defaultDwellTime = 1.0;
    d.dwellTimeStep = qt::L("0.1 sec");                                        // 0x1424bb060
    d.dwellHeatCoeff = qt::L(unit == 2 ? "0.0003 Btu/sec/in^2/F" : "1.0 N/sec/mm/C");   // 0x1424bb048 / 0x1424bb038
    d.relativeEnergy = 1;
    d.heats = 0;
}

// @src 0x14060b500 / 0x14060b5d0 / 0x14060e2c0 (data part)
void initMultiBlowProps(MultiBlowProps& p, int dim, int unit)
{
    p.data = std::make_shared<MultiBlowData>();
    initMultiBlowData(*p.data, unit);
    MultiBlowData& d = *p.data;
    if (dim == 2) {
        d.movDir[0] = 0.0;
        d.movDir[1] = -1.0;
        d.movDir[2] = 0.0;
    } else {
        d.movDir[0] = 0.0;
        d.movDir[1] = 0.0;
        d.movDir[2] = -1.0;
    }
}

// @src 0x140612de0
void mbSetEquipment(MultiBlowData& d, const qt::Variant& v)
{
    d.equipment = vBool(v) ? 1 : 0;
    d.hammer = d.equipment == 0 ? 1 : 0;
}

bool mbSetScalar(MultiBlowData& d, const String& name, const qt::Variant& v)
{
    const std::u16string& n = name.u16();
    if (n == u"Equipment") mbSetEquipment(d, v);
    else if (n == u"EquipName") d.equipName = vString(v);                 // @src 0x140609d60
    else if (n == u"HammerEnergy") d.hammerEnergy = vDouble(v);           // @src 0x140609e30
    else if (n == u"HammerMass") d.hammerMass = vDouble(v);               // @src 0x140609ed0
    else if (n == u"HammerEfficiency") d.hammerEfficiency = vDouble(v);   // @src 0x140609f70
    else if (n == u"HammerCounterblow") d.hammerCounterblow = static_cast<int>(vDouble(v));   // @src 0x14060a010
    else if (n == u"ScrewEnergy") d.screwEnergy = vDouble(v);             // @src 0x14060a0b0
    else if (n == u"ScrewEfficiency") d.screwEfficiency = vDouble(v);     // @src 0x14060a150
    else if (n == u"MomentOfInertia") d.momentOfInertia = vDouble(v);     // @src 0x14060a1f0
    else if (n == u"LeadScrewPitch") d.leadScrewPitch = vDouble(v);       // @src 0x14060a290
    else if (n == u"MovDirX") d.movDir[0] = vDouble(v);                   // @src 0x14060a330
    else if (n == u"MovDirY") d.movDir[1] = vDouble(v);                   // @src 0x14060a3d0
    else if (n == u"MovDirZ") d.movDir[2] = vDouble(v);                   // @src 0x14060a470
    else if (n == u"UseWorkpieceRotation") d.workpieceRotation = vBool(v);   // @src 0x14060a510
    else if (n == u"InitTemp") d.initTemp = vBool(v);                     // @src 0x1406189b0 (node enabling not ported)
    else if (n == u"InitStrain") d.initStrain = vBool(v);                 // @src 0x14060a5d0
    else if (n == u"UseEfficiencyFunctionData") d.efficiencyFunction = vBool(v);   // @src 0x14060a690
    else if (n == u"UseHeating") d.heating = vBool(v);                    // @src 0x14060a750
    else if (n == u"HeatingTemperature") d.heatingTemperature = vDouble(v);   // @src 0x14060a810
    else if (n == u"RoomTemperature") d.roomTemperature = vDouble(v);     // @src 0x14060a8c0
    else if (n == u"TransferInTime") d.transferInTime = vDouble(v);       // @src 0x14060aad0
    else if (n == u"HeatingTime") d.heatingTime = vDouble(v);             // @src 0x14060a970
    else if (n == u"TransferOutTime") d.transferOutTime = vDouble(v);     // @src 0x14060ab80
    else if (n == u"TransferTimeStep") d.transferTimeStep = vDouble(v);   // @src 0x14060ac30
    else if (n == u"HeatingTimeStep") d.heatingTimeStep = vDouble(v);     // @src 0x14060aa20
    else if (n == u"UseAdaptiveReheat") d.adaptiveReheat = vBool(v);      // @src 0x14060ace0
    else if (n == u"TriggerHeatTemp") d.triggerHeatTemp = vDouble(v);     // @src 0x14060ada0
    else if (n == u"TriggerHeatTempUpperLimit") d.triggerHeatTempUpper = vDouble(v);   // @src 0x14060ae50
    else if (n == u"HeatStopTemp") d.heatStopTemp = vDouble(v);           // @src 0x14060af00
    else if (n == u"UseDwell") d.dwell = vBool(v);                        // @src 0x14060afb0
    else if (n == u"DefaultDwellTime") d.defaultDwellTime = vDouble(v);   // @src 0x14060b070
    else if (n == u"DwellTimeStep") d.dwellTimeStep = vString(v);         // @src 0x14060b120
    else if (n == u"DwellHeatCoeff") d.dwellHeatCoeff = vString(v);       // @src 0x14060b200
    else if (n == u"UseRelativeEnergy") d.relativeEnergy = vBool(v);      // @src 0x14060b2e0
    else if (n == u"NumHeats") mbSetNumHeats(d, vInt(v));                 // @src 0x140616640
    else return false;
    return true;
}

// @src 0x1406140c0
void mbSetNumHeats(MultiBlowData& d, int n)
{
    if (n < d.heats) {
        // One heat node removed per step; each removal drops the last entry of every per-heat
        // list except +0x148 (blows), which keeps its stale entries.
        for (int k = n; k < d.heats; ++k) {
            popLast(d.energy);
            popLast(d.efficiency);
            popLast(d.dwellTime);
            popLast(d.reheat);
            popLast(d.flip);
            popLast(d.temperature);
            popLast(d.rotAngle);
            popLast(d.rotSelfCenter);
            for (auto& l : d.rotCenter) popLast(l);
            for (auto& l : d.rotAxis) popLast(l);
        }
    } else {
        for (int k = d.heats; k < n; ++k) {
            d.energy.emplace_back();
            d.efficiency.emplace_back();
            d.dwellTime.emplace_back();
            d.reheat.emplace_back();
            d.flip.emplace_back();
            d.rotAngle.emplace_back();
            d.rotSelfCenter.emplace_back();
            for (auto& l : d.rotCenter) l.emplace_back();
            for (auto& l : d.rotAxis) l.emplace_back();
            d.temperature.push_back(100.0);            // 0x1421de200
            d.blows.push_back(2);
            if (k < static_cast<int>(d.blows.size())) d.blows[static_cast<size_t>(k)] = 0;
            else d.blows.push_back(0);
            mbSetNumBlows(d, k, 1);
        }
    }
    d.heats = n;
}

// @src 0x140615190
void mbSetNumBlows(MultiBlowData& d, int k, int n)
{
    const size_t K = static_cast<size_t>(k);
    if (n < d.blows[K]) {
        for (int b = n; b < d.blows[K]; ++b) {
            popLast(d.energy[K]);
            popLast(d.efficiency[K]);
            popLast(d.dwellTime[K]);
            popLast(d.reheat[K]);
            popLast(d.flip[K]);
            popLast(d.rotAngle[K]);
            popLast(d.rotSelfCenter[K]);
            for (auto& l : d.rotCenter) popLast(l[K]);
            for (auto& l : d.rotAxis) popLast(l[K]);
        }
    } else {
        for (int b = d.blows[K]; b < n; ++b) {
            d.energy[K].push_back(100.0);           // 0x1421de200
            d.efficiency[K].push_back(1.0);         // 0x1421249e0
            d.dwellTime[K].push_back(1.0);
            d.reheat[K].push_back(0);
            d.flip[K].push_back(0);
            d.rotAngle[K].push_back(0.0);
            d.rotSelfCenter[K].push_back(1);
            for (auto& l : d.rotCenter) l[K].push_back(0.0);
            for (auto& l : d.rotAxis) l[K].push_back(0.0);
        }
    }
    d.blows[K] = n;
}

// @src 0x14060b3a0
void mbSetTemperature(MultiBlowData& d, int k, const qt::Variant& v)
{
    if (k > -1 && k < static_cast<int>(d.temperature.size()))
        d.temperature[static_cast<size_t>(k)] = vDouble(v);
}

bool mbSetBlow(MultiBlowData& d, const String& name, int k, int b, const qt::Variant& v)
{
    const std::u16string& n = name.u16();
    const size_t K = static_cast<size_t>(k), B = static_cast<size_t>(b);
    if (n == u"NumBlows") {                                  // @src 0x140616510
        mbSetNumBlows(d, k, vInt(v));
        return true;
    }
    auto setD = [&](std::vector<std::vector<double>>& l) { l[K][B] = vDouble(v); };
    auto setB = [&](std::vector<std::vector<std::uint8_t>>& l) { l[K][B] = vBool(v) ? 1 : 0; };
    if (n == u"BlowEnergy") setD(d.energy);                  // @src 0x1406166f0
    else if (n == u"BlowEfficiency") setD(d.efficiency);     // @src 0x1406169a0
    else if (n == u"BlowDwellTime") setD(d.dwellTime);       // @src 0x140616c50
    else if (n == u"BlowReheat") setB(d.reheat);             // @src 0x140616f00
    else if (n == u"BlowFlip") setB(d.flip);                 // @src 0x1406171a0
    else if (n == u"BlowRotAngle") setD(d.rotAngle);         // @src 0x140617440
    else if (n == u"BlowRotSelfCenter") setB(d.rotSelfCenter);   // @src 0x1406176f0
    else if (n == u"BlowRotCenterX") setD(d.rotCenter[0]);   // @src 0x140617990
    else if (n == u"BlowRotCenterY") setD(d.rotCenter[1]);   // @src 0x140617c40
    else if (n == u"BlowRotCenterZ") setD(d.rotCenter[2]);   // @src 0x140617ef0
    else if (n == u"BlowRotAxisX") setD(d.rotAxis[0]);       // @src 0x1406181a0
    else if (n == u"BlowRotAxisY") setD(d.rotAxis[1]);       // @src 0x140618450
    else if (n == u"BlowRotAxisZ") setD(d.rotAxis[2]);       // @src 0x140618700
    else return false;
    return true;
}

// ---- property tree --------------------------------------------------------------------------------
namespace detail {

// Template kinds by the registering function: 0x140375640 group, 0x140373400 int, 0x1403747c0 enum,
// 0x140374ce0 text, 0x1403738e0 double, 0x140374300 bool, 0x140373de0 doubleUnits. Only "group"
// was read as a literal (0x14232788c); the others are the TypeName spellings of the corpus files.
PropNode node(const char* name, const char* type, std::vector<PropNode> children = {})
{
    PropNode n;
    n.name = qt::L(name);
    n.typeName = qt::L(type);
    n.children = std::move(children);
    return n;
}

// @src 0x140615190 (grow branch): blow b (template 0x143d9ef80) and its 13 children
// (0x143d9ef88..0x143d9eff0), all created with index b (0x140375a70 param 5, SfProperty+0x48)
PropNode blowNode(int b)
{
    PropNode n = node("Blow", "group",
                      {node("BlowEnergy", "double"), node("BlowEfficiency", "double"),
                       node("BlowDwellTime", "double"), node("BlowReheat", "bool"), node("BlowFlip", "bool"),
                       node("BlowRotAngle", "double"), node("BlowRotSelfCenter", "bool"),
                       node("BlowRotCenterX", "double"), node("BlowRotCenterY", "double"),
                       node("BlowRotCenterZ", "double"), node("BlowRotAxisX", "double"),
                       node("BlowRotAxisY", "double"), node("BlowRotAxisZ", "double")});
    n.index = b;
    for (PropNode& c : n.children) c.index = b;
    return n;
}

// @src 0x1406140c0 (grow branch): heat k (0x143d9ef70) with Temperature (0x143d9f0f8), NumBlows
// (0x143d9ef78), all index k, then NumBlows(k, 1) adds blow 0
PropNode heatNode(int k)
{
    PropNode n = node("Heat", "group", {node("Temperature", "double"), node("NumBlows", "int")});
    n.index = k;
    for (PropNode& c : n.children) c.index = k;
    n.children.push_back(blowNode(0));
    return n;
}

}  // namespace detail

using detail::node;
using detail::heatNode;

// One conversion = one fresh GUI process: the template starts null and the first MultiBlow props
// made (operations are created in document order at open) fills it with its own name.
String multiBlowRootName(int dim)
{
    const String own = qt::L(dim == 2 ? "MultiBlowForging2D" : "MultiBlowForging3D");   // 0x14060e2c0
    Project* p = currentProject();
    if (!p) return own;
    if (!p->multiBlowRootSet) {                                   // 0x140375640: only while null
        p->multiBlowRoot = own;
        p->multiBlowRootSet = true;
    }
    return p->multiBlowRoot;
}

namespace {
const xml::Node* child(const xml::Node& n, const char16_t* tag)
{
    for (const xml::Node* c : n.childElements())
        if (c->tag.u16() == tag) return c;
    return nullptr;
}
const xml::Node* findOperation(const xml::Node& n, int id)
{
    if (n.kind == xml::Node::Element && n.tag.u16() == u"Operation") {
        const xml::Node* i = child(n, u"Id");
        if (i && qt::toInt(i->text()) == id) return &n;
    }
    for (const auto& c : n.children)
        if (c)
            if (const xml::Node* r = findOperation(*c, id)) return r;
    return nullptr;
}
}  // namespace

String savedMultiBlowRoot(const Project& p, int opId)
{
    if (!p.dom || !p.dom->root) return String();
    const xml::Node* op = findOperation(*p.dom->root, opId);
    const xml::Node* props = op ? child(*op, u"Properties") : nullptr;
    if (!props) return String();
    for (const xml::Node* pr : props->childElements(qt::L("Property"))) {
        const xml::Node* nm = child(*pr, u"Name");
        if (nm && qt::startsWith(nm->text(), qt::L("MultiBlowForging"))) return nm->text();
    }
    return String();
}

// @src 0x140612150
void equipmentSpecFill(PropNode& spec, const MultiBlowData& d)
{
    if (d.hammer != 0) {
        spec.children.push_back(node("HammerEnergy", "double"));        // 0x143d9f000
        spec.children.push_back(node("HammerMass", "double"));          // 0x143d9f008
        spec.children.push_back(node("HammerEfficiency", "double"));    // 0x143d9f010
        spec.children.push_back(node("HammerCounterblow", "double"));   // 0x143d9f018
    }
    if (d.equipment != 0) {
        spec.children.push_back(node("ScrewEnergy", "double"));         // 0x143d9f020
        spec.children.push_back(node("ScrewEfficiency", "double"));     // 0x143d9f028
        spec.children.push_back(node("MomentOfInertia", "double"));     // 0x143d9f030
        spec.children.push_back(node("LeadScrewPitch", "double"));      // 0x143d9f038
    }
}

void equipmentSpecRebuild(PropNode& spec, const MultiBlowData& d)
{
    spec.children.clear();                                              // 0x140613280
    equipmentSpecFill(spec, d);                                         // 0x140612e80
}

std::vector<PropNode> multiBlowPropTree(int dim, const MultiBlowData& d)
{
    PropNode root = node("", "group");
    root.name = multiBlowRootName(dim);   // 0x143d9ef10
    auto& c = root.children;
    // 0x140612c40
    c.push_back(node("EquipmentType", "group", {node("Equipment", "enum"), node("EquipName", "QString")}));
    // 0x140612e80: the hammer or the screw fields by the MBD's state; setEquipment 0x140612de0
    // rebuilds them (0x1406132b0, MultiBlowProps::applyValue)
    c.push_back(node("EquipmentSpec", "group"));
    equipmentSpecFill(c.back(), d);
    // 0x1406132e0
    c.push_back(node("MovementSpec", "group",
                     {node("MovDirX", "double"), node("MovDirY", "double"), node("MovDirZ", "double")}));
    // 0x140613450
    c.push_back(node("Init", "group",
                     {node("UseWorkpieceRotation", "bool"), node("InitTemp", "bool"),
                      node("InitStrain", "bool"), node("UseEfficiencyFunctionData", "bool")}));
    // 0x140613770 (this order: TransferInTime before HeatingTime)
    c.push_back(node("Heating", "group",
                     {node("UseHeating", "bool"), node("HeatingTemperature", "double"),
                      node("RoomTemperature", "double"), node("TransferInTime", "double"),
                      node("HeatingTime", "double"), node("TransferOutTime", "double"),
                      node("TransferTimeStep", "double"), node("HeatingTimeStep", "double")}));
    // 0x140613b60
    c.push_back(node("AdaptiveReheat", "group",
                     {node("UseAdaptiveReheat", "bool"), node("TriggerHeatTemp", "double"),
                      node("TriggerHeatTempUpperLimit", "double"), node("HeatStopTemp", "double")}));
    // 0x140613d50
    c.push_back(node("Dwell", "group",
                     {node("UseDwell", "bool"), node("DefaultDwellTime", "double"),
                      node("DwellTimeStep", "doubleUnits"), node("DwellHeatCoeff", "doubleUnits")}));
    // 0x140613640: children from the base adders (P02)
    c.push_back(node("Interobject", "group"));
    // 0x140613f40, then NumHeats(1)
    c.push_back(node("MultiBlow", "group", {node("UseRelativeEnergy", "bool"), node("NumHeats", "int")}));
    c.push_back(heatNode(0));
    return {root};
}

}  // namespace m2m::data

namespace m2m {

namespace {
thread_local Project* tlsProject = nullptr;
}

Project* currentProject() { return tlsProject; }

ProjectScope::ProjectScope(Project& p) : prev_(tlsProject) { tlsProject = &p; }

ProjectScope::~ProjectScope() { tlsProject = prev_; }

// @src 0x14060b950  (props+0x10)->vt[+0x8], from ParseProperties 0x140094f70 before the saved
// values apply: a project opened in the GUI starts with absolute energy (DOESCL <energy>, MOVCTL
// <die> 1) unless it saved UseRelativeEnergy true (oracle B 75F_B_MB3, 92_B_MB2REL).
void MultiBlowProps::beforeParse()
{
    if (!data) return;
    data->relativeEnergy = 0;
    data->adaptiveReheat = 0;
}

// @src 0x14061ab80 -> the set* slot of the node (0x140370600 invokes it with the QVariant).
// Indexed slots read the node's own index (0x140370fd0) and its parents' (0x140370f40); heat
// nodes are children of the root (MBD+0x00), blow nodes children of their heat. NumHeats and
// NumBlows add or remove those children (0x1406140c0 / 0x140615190) as well as the data.
void MultiBlowProps::applyValue(PropNode& n, const std::vector<PropNode*>& anc, const qt::Variant& v)
{
    // Nodes outside the class root (TestGroup) and the Interobject children belong to the base
    // properties' slots (0x1405f7b10, P15).
    PropNode* parent = anc.empty() ? nullptr : anc.back();
    const bool inRoot = !anc.empty() && qt::startsWith(anc.front()->name, qt::L("MultiBlowForging"));
    if (!inRoot || (parent && parent->name.u16() == u"Interobject")) {
        base::applyBaseValue(*this, n, anc, v);
        return;
    }
    if (!data) return;
    data::MultiBlowData& d = *data;
    const std::u16string& name = n.name.u16();
    PropNode* grand = anc.size() < 2 ? nullptr : anc[anc.size() - 2];
    auto heatInRange = [&](int k) { return k >= 0 && k < static_cast<int>(d.energy.size()); };

    if (name == u"NumHeats") {                                           // @src 0x140616640
        const int before = d.heats;
        const int want = qt::Variant::Int == v.kind ? v.i
                         : v.kind == qt::Variant::Double ? static_cast<int>(v.d)
                         : v.kind == qt::Variant::Text ? qt::toInt(v.s) : 0;
        data::mbSetNumHeats(d, want);
        if (!anc.empty()) {
            std::vector<PropNode>& roots = anc.front()->children;
            if (want < before) {
                for (auto it = roots.begin(); it != roots.end();) {
                    if (it->name.u16() == u"Heat" && it->index >= want) it = roots.erase(it);
                    else ++it;
                }
            } else {
                for (int k = before; k < want; ++k) roots.push_back(data::detail::heatNode(k));
            }
        }
        return;
    }
    if (name == u"NumBlows") {                                           // @src 0x140616510
        if (!parent) return;                                             // node's parent: the heat
        const int k = n.index;
        if (!heatInRange(k)) return;
        const int before = d.blows[static_cast<size_t>(k)];
        const int want = v.kind == qt::Variant::Int ? v.i
                         : v.kind == qt::Variant::Double ? static_cast<int>(v.d)
                         : v.kind == qt::Variant::Text ? qt::toInt(v.s) : 0;
        data::mbSetNumBlows(d, k, want);
        std::vector<PropNode>& ch = parent->children;
        if (want < before) {
            for (auto it = ch.begin(); it != ch.end();) {
                if (it->name.u16() == u"Blow" && it->index >= want) it = ch.erase(it);
                else ++it;
            }
        } else {
            for (int b = before; b < want; ++b) ch.push_back(data::detail::blowNode(b));
        }
        return;
    }
    if (name == u"Temperature") {                                        // @src 0x14060b3a0
        data::mbSetTemperature(d, n.index, v);
        return;
    }
    if (parent && parent->name.u16() == u"Blow") {                       // per-blow slots
        if (!grand) return;
        const int b = parent->index, k = grand->index;
        if (!heatInRange(k) || b < 0 || b >= static_cast<int>(d.energy[static_cast<size_t>(k)].size())) return;
        data::mbSetBlow(d, n.name, k, b, v);
        return;
    }
    if (name == u"Equipment") {                                          // 0x140612de0
        data::mbSetEquipment(d, v);
        // 0x1406132b0: the EquipmentSpec group (MBD+0x60) gets the set of the new state; the UI
        // signal 0x1402cea00 is dropped
        for (PropNode& g : anc.front()->children)
            if (g.name.u16() == u"EquipmentSpec") data::equipmentSpecRebuild(g, d);
        return;
    }
    data::mbSetScalar(d, n.name, v);
}

// @src 0x14060b6b0  the die's movement (LLOBJ_DATA+0x2160, the curve block of 0x14018b220) into the
// MultiBlow data: equipment name, hammer (type +0x68 == 2) or screw (3) fields, the direction
bool multiBlowEquipmentFromMovement(MultiBlowProps& pr, const std::uint8_t* mv)
{
    if (!pr.data) throw Error(M2M_E_UNSUPPORTED, "MultiBlow 0x14060b6b0: props without data (+0x38)");
    data::MultiBlowData& d = *pr.data;
    auto i32 = [mv](std::size_t o) { std::int32_t v; std::memcpy(&v, mv + o, sizeof v); return v; };
    auto f64 = [mv](std::size_t o) { double v; std::memcpy(&v, mv + o, sizeof v); return v; };
    bool set = false;
    const std::uint8_t* curve = nullptr;
    d.equipName = String::latin1(reinterpret_cast<const char*>(mv));   // QString(const char*) (fromAscii): +0x10
    if (i32(0x68) == 2) {                                       // hammer
        d.hammer = 1;                                           // +0x09
        d.equipment = 0;                                        // +0x0a
        d.hammerEnergy = f64(0x1f0);                            // +0x20
        d.hammerMass = f64(0x1e8);                              // +0x28
        d.hammerEfficiency = f64(0x1f8);                        // +0x30
        d.hammerCounterblow = i32(0x200);                       // +0x38
        const double e = f64(0x1f0);
        d.hammerEnergy = e;                                     // 0x14060ba70: +0x20 and +0x40
        d.screwEnergy = e;
        curve = mv + 0x208;
        set = true;
    } else if (i32(0x68) == 3) {                                // screw
        d.hammer = 0;
        d.equipment = 1;
        d.screwEnergy = f64(0x280);                             // +0x40
        d.screwEfficiency = f64(0x288);                         // +0x48
        d.momentOfInertia = f64(0x290);                         // +0x50
        d.leadScrewPitch = f64(0x298);                          // +0x58
        const double e = f64(0x280);
        d.hammerEnergy = e;                                     // 0x14060ba70
        d.screwEnergy = e;
        curve = mv + 0x2a0;
        set = true;
    }
    d.movDir[0] = f64(0x50);                                    // +0x68
    d.movDir[1] = f64(0x58);                                    // +0x70
    d.movDir[2] = f64(0x60);                                    // +0x78
    if (set) {
        // 0x140612d60: the EquipmentType group's children (MBD+0x18) removed (0x140612d30) and made
        // again from their templates (0x140612c40); 0x1406132b0: EquipmentSpec likewise
        for (PropNode& root : pr.tree)
            for (PropNode& g : root.children) {
                if (g.name.u16() == u"EquipmentType") {
                    g.children.clear();
                    g.children.push_back(data::detail::node("Equipment", "enum"));     // 0x143d9ef68
                    g.children.push_back(data::detail::node("EquipName", "QString"));  // 0x143d9eff8
                }
                if (g.name.u16() == u"EquipmentSpec") data::equipmentSpecRebuild(g, d);
            }
        // 0x14060bda0: UseEfficiencyFunctionData = the curve's point count (+4) is not 0
        std::int32_t n = 0;
        std::memcpy(&n, curve + 4, sizeof n);
        d.efficiencyFunction = n != 0 ? 1 : 0;                  // +0x8c
    }
    return set;
}

}  // namespace m2m

namespace m2m::data {

// ---- accessors -----------------------------------------------------------------------------------
// @src 0x14060c6f0
int mbHeats(const MultiBlowData& d) { return d.heats; }

// @src 0x14060c710
int mbBlows(const MultiBlowData& d, int i)
{
    if (i < 1 || i > static_cast<int>(d.blows.size())) return 0;
    return d.blows[static_cast<size_t>(i - 1)];
}

// @src 0x14060c810
double mbEnergy(const MultiBlowData& d, int i, int j) { return at2(d.energy, i, j); }
// @src 0x14060cb20
double mbEfficiency(const MultiBlowData& d, int i, int j) { return at2(d.efficiency, i, j); }
// @src 0x14060ccc0
double mbDwellTime(const MultiBlowData& d, int i, int j) { return at2(d.dwellTime, i, j); }
// @src 0x14060ce60
bool mbReheat(const MultiBlowData& d, int i, int j) { return at2(d.reheat, i, j) != 0; }
// @src 0x14060cfc0
bool mbFlip(const MultiBlowData& d, int i, int j) { return at2(d.flip, i, j) != 0; }

// @src 0x14060d150  each field read only when (i, j) is in range of its own list
BlowRotation mbRotation(const MultiBlowData& d, int i, int j)
{
    BlowRotation r;   // 0x140609140
    auto in = [&](const auto& l) {
        return i > 0 && i <= static_cast<int>(l.size()) && j > 0 &&
               j <= static_cast<int>(l[static_cast<size_t>(i - 1)].size());
    };
    auto get = [&](const auto& l) { return l[static_cast<size_t>(i - 1)][static_cast<size_t>(j - 1)]; };
    if (in(d.rotAngle)) r.angle = get(d.rotAngle);
    if (in(d.rotSelfCenter)) r.selfCenter = get(d.rotSelfCenter);
    for (int k = 0; k < 3; ++k)
        if (in(d.rotCenter[k])) r.c[k] = get(d.rotCenter[k]);
    for (int k = 0; k < 3; ++k)
        if (in(d.rotAxis[k])) r.a[k] = get(d.rotAxis[k]);
    return r;
}

// @src 0x14060bdd0
bool mbHeatOk(const MultiBlowData& d, int i, int j)
{
    const bool heat = d.heating != 0;
    const bool re = mbReheat(d, i, j);
    const bool adaptive = d.adaptiveReheat != 0;
    const bool ndtmp = d.initTemp != 0;
    const bool any = 0.0 < d.transferInTime || 0.0 < d.heatingTime || 0.0 < d.transferOutTime;
    return !(!heat || (!re && !adaptive) || !any || ndtmp);
}

// @src 0x14060c280
bool mbDwellOn(const MultiBlowData& d, int i, int j)
{
    const bool on = d.dwell != 0;
    const double t = mbDwellTime(d, i, j);
    return !(!on || t <= 0.0);
}

}  // namespace m2m::data
