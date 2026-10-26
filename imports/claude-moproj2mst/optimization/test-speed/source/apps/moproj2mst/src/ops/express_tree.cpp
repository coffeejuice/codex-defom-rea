// FormingExpress property setters (packet P15): the slots of SfOprFormingExpressProperties that copy
// property values into the data block D (+0x40). Meta-object 0x142ef86e8 (69 methods: set<Name> /
// get<Name>(SfProperty*) pairs and StrokeProfileAction), qt_metacall 0x141465fe0 -> 0x1414645a0.
// Decompiles: gui_mo/s1/p15/m7.c (dispatch), m8.c (slots), m9.c (setNumHeats / setNumBlows).
// The tree refreshes the slots trigger (0x14142a860, 0x14142b000: which Equipment/Spec nodes exist)
// and the tree builder 0x1414269a0 belong to the PropNode hook (see express.hpp); the data effects
// are here.
#include "ops/express.hpp"

#include "m2m/props_base.hpp"

namespace m2m::data {

namespace {

// QVariant conversions of the value the property reader built (s1-01 6.3: double, int or text).
bool vBool(const qt::Variant& v)
{
    switch (v.kind) {
    case qt::Variant::Double: return v.d != 0.0;
    case qt::Variant::Int: return v.i != 0;
    case qt::Variant::Text: {
        const std::u16string& s = v.s.u16();
        if (s.empty() || s == u"0") return false;
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
    case qt::Variant::Double: return static_cast<int>(v.d);
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
    default: return String();
    }
}

template <class T>
void popLast(std::vector<T>& l)
{
    if (!l.empty()) l.pop_back();       // QList::removeAt(count - 1)
}

template <class T>
T* at2(std::vector<std::vector<T>>& l, int k, int j)
{
    // QList::operator[] without a bound check in the original; out of range is ignored here.
    if (k < 0 || k >= static_cast<int>(l.size())) return nullptr;
    auto& in = l[static_cast<size_t>(k)];
    if (j < 0 || j >= static_cast<int>(in.size())) return nullptr;
    return &in[static_cast<size_t>(j)];
}

}  // namespace

// @src 0x14142fac0  FormingExpress data ctor (lists empty, type 2, express off, no blows)
void initFormingExpressData(FormingExpressData& d, int unit)
{
    d = FormingExpressData{};
    if (unit == 2) {
        d.toTemp = 1800.0;         // 0x1424bb078
        d.roomTemp = 68.0;              // 0x1422ad710
    } else {
        d.toTemp = 950.0;          // 0x1424bb070
        d.roomTemp = 20.0;              // 0x1421ce520
    }
    d.dwStep = qt::L("0.1 sec");   // 0x1424bb060
    d.dwIhtc = qt::L(unit == 2 ? "0.0003 Btu/sec/in^2/F" : "1.0 N/sec/mm/C");   // 0x1424bb048 / 0x1424bb038
}

// @src 0x14142e260  setNumBlows of heat k (0-based): remove or add blows of that heat
void feSetNumBlows(FormingExpressData& d, int k, int n)
{
    if (k < 0 || k >= static_cast<int>(d.steps.size())) return;
    const int cur = d.steps[static_cast<size_t>(k)];
    auto row = [&](auto& l) -> auto& { return l[static_cast<size_t>(k)]; };
    if (n < cur) {
        for (int j = n; j < cur; ++j) {             // child node j + 2 removed
            if (k < static_cast<int>(d.stroke.size())) popLast(row(d.stroke));
            if (k < static_cast<int>(d.l158.size())) popLast(row(d.l158));
            if (k < static_cast<int>(d.gap.size())) popLast(row(d.gap));
            if (k < static_cast<int>(d.drop.size())) popLast(row(d.drop));
            if (k < static_cast<int>(d.flip.size())) popLast(row(d.flip));
        }
    } else {
        for (int j = cur; j < n; ++j) {             // "Blow" node with index j and its 5 leaves
            if (k < static_cast<int>(d.stroke.size())) row(d.stroke).push_back(100.0);   // 0x1421de200
            if (k < static_cast<int>(d.l158.size())) row(d.l158).push_back(1.0);         // 0x1421249e0
            if (k < static_cast<int>(d.gap.size())) row(d.gap).push_back(1.0);
            if (k < static_cast<int>(d.drop.size())) row(d.drop).push_back(0);
            if (k < static_cast<int>(d.flip.size())) row(d.flip).push_back(0);
        }
    }
    d.steps[static_cast<size_t>(k)] = n;
}

// @src 0x14142d610  setNumHeats
void feSetNumHeats(FormingExpressData& d, int n)
{
    if (n < d.blows) {
        // One "Heat" node removed per step; each removal drops the last entry of the per-heat
        // lists +0x150..+0x170 and +0x130, but not +0x128 (steps), which keeps stale entries.
        for (int i = n; i < d.blows; ++i) {
            popLast(d.stroke);
            popLast(d.l158);
            popLast(d.gap);
            popLast(d.drop);
            popLast(d.flip);
            popLast(d.temperature);
        }
    } else {
        for (int k = d.blows; k < n; ++k) {         // "Heat" node index k, "Temperature", "NumBlows"
            d.stroke.emplace_back();
            d.l158.emplace_back();
            d.gap.emplace_back();
            d.drop.emplace_back();
            d.flip.emplace_back();
            d.temperature.push_back(100.0);         // 0x1421de200
            d.steps.push_back(1);
            if (k < static_cast<int>(d.steps.size()))
                d.steps[static_cast<size_t>(k)] = 0;
            else
                d.steps.push_back(0);
            feSetNumBlows(d, k, 1);                 // 0x14142e260(this, heat, k, 1)
        }
    }
    d.blows = n;
}

// @src 0x1414261c0  setTemperature of heat k (node index)
void feSetTemperature(FormingExpressData& d, int k, const qt::Variant& v)
{
    if (k > -1 && k < static_cast<int>(d.temperature.size()))
        d.temperature[static_cast<size_t>(k)] = vDouble(v);
}

// @src 0x14142ed40 BlowEnergy, 0x14142eff0 BlowEfficiency, 0x14142f2a0 BlowDwellTime,
// 0x14142f550 BlowReheat, 0x14142f7f0 BlowFlip: list[k][j], k = heat node index, j = blow node index
bool feSetBlow(FormingExpressData& d, const String& name, int k, int j, const qt::Variant& v)
{
    const std::u16string& n = name.u16();
    if (n == u"BlowEnergy") {
        if (double* p = at2(d.stroke, k, j)) *p = vDouble(v);
    } else if (n == u"BlowEfficiency") {
        if (double* p = at2(d.l158, k, j)) *p = vDouble(v);
    } else if (n == u"BlowDwellTime") {
        if (double* p = at2(d.gap, k, j)) *p = vDouble(v);
    } else if (n == u"BlowReheat") {
        if (std::uint8_t* p = at2(d.drop, k, j)) *p = vBool(v) ? 1 : 0;
    } else if (n == u"BlowFlip") {
        if (std::uint8_t* p = at2(d.flip, k, j)) *p = vBool(v) ? 1 : 0;
    } else {
        return false;
    }
    return true;
}

// Scalar setters (even method indices of 0x1414645a0). False for a name that is not one of them.
bool feSetScalar(FormingExpressData& d, const String& name, const qt::Variant& v)
{
    const std::u16string& n = name.u16();
    if (n == u"Equipment") d.type = vInt(v);                        // @src 0x14142a890 (+ tree refresh)
    else if (n == u"EquipName") d.name = vString(v);                // @src 0x141425140
    else if (n == u"HammerEnergy") d.hammerEnergy = vDouble(v);              // @src 0x141425210
    else if (n == u"HammerMass") d.hammerMass = vDouble(v);                // @src 0x1414252b0
    else if (n == u"HammerEfficiency") d.hammerEfficiency = vDouble(v);          // @src 0x141425350
    else if (n == u"ScrewEnergy") d.screwEnergy = vDouble(v);               // @src 0x1414253f0
    else if (n == u"ScrewEfficiency") d.screwEfficiency = vDouble(v);           // @src 0x141425490
    else if (n == u"MomentOfInertia") d.momentOfInertia = vDouble(v);           // @src 0x141425530
    else if (n == u"LeadScrewPitch") d.leadScrewPitch = vDouble(v);            // @src 0x1414255d0
    else if (n == u"MechPressType") d.mechPressType = vInt(v);                // @src 0x14142b030 (+ tree refresh)
    else if (n == u"TotalStroke") d.totalStroke = vDouble(v);               // @src 0x141425670
    else if (n == u"CyclesSec") d.cyclesSec = vDouble(v);                 // @src 0x141425710
    else if (n == u"ConnectingRodLength") d.connectingRodLength = vDouble(v);       // @src 0x1414257b0
    else if (n == u"MovDirX") d.dir[0] = vDouble(v);                // @src 0x141425850
    else if (n == u"MovDirY") d.dir[1] = vDouble(v);                // @src 0x141425900
    else if (n == u"MovDirZ") d.dir[2] = vDouble(v);                // @src 0x1414259b0
    else if (n == u"InitStrain") d.initStrain = vBool(v) ? 1 : 0;   // @src 0x141425a60
    else if (n == u"HeatingTemperature") d.toTemp = vDouble(v);     // @src 0x141425b20
    else if (n == u"RoomTemperature") d.roomTemp = vDouble(v);           // @src 0x141425bd0
    else if (n == u"TransferTime") d.toTime = vDouble(v);           // @src 0x141425c80
    else if (n == u"TransferTimeStep") d.toStep = vDouble(v);       // @src 0x141425d30
    else if (n == u"HeatingTime") d.heatingTime = vDouble(v);               // @src 0x141425de0
    else if (n == u"HeatingTimeStep") d.heatingTimeStep = vDouble(v);           // @src 0x141425e90
    else if (n == u"DwellTimeStep") d.dwStep = vString(v);          // @src 0x141426020
    else if (n == u"DwellHeatCoeff") d.dwIhtc = vString(v);         // @src 0x141425f40
    else if (n == u"UseBlowTable") d.express = vBool(v) ? 1 : 0;    // @src 0x141426100
    else if (n == u"NumHeats") feSetNumHeats(d, vInt(v));           // @src 0x14142ec90
    else return false;
    return true;
}


// ---- property tree (0x1414269a0 templates, 0x141429cc0 builder) -----------------------------------
namespace {
PropNode tnode(const char* name, const char* type, std::vector<PropNode> children = {})
{
    PropNode n;
    n.name = qt::L(name);
    n.typeName = qt::L(type);
    n.children = std::move(children);
    return n;
}
}  // namespace

// @src 0x14142a740  EquipmentType children (templates 0x1446fd898 enum, 0x1446fd8e8 text)
std::vector<PropNode> feEquipmentTypeNodes()
{
    return {tnode("Equipment", "enum"), tnode("EquipName", "QString")};
}

// @src 0x14142a950  EquipmentSpec children by the equipment type D+0x14
std::vector<PropNode> feEquipmentSpecNodes(const FormingExpressData& d)
{
    std::vector<PropNode> c;
    if (d.type == 2) {                              // mechanical press
        c.push_back(tnode("MechPressType", "enum"));        // 0x1446fd928
        c.push_back(tnode("TotalStroke", "double"));        // 0x1446fd930
        c.push_back(tnode("CyclesSec", "double"));          // 0x1446fd938
        if (d.mechPressType == 0)
            c.push_back(tnode("ConnectingRodLength", "double"));   // 0x1446fd940
        else
            c.push_back(tnode("StrokeProfile", "action"));         // 0x1446fd948 (0x1403751b0)
    } else if (d.type == 3) {                       // hammer
        c.push_back(tnode("HammerEnergy", "double"));       // 0x1446fd8f0
        c.push_back(tnode("HammerMass", "double"));         // 0x1446fd8f8
        c.push_back(tnode("HammerEfficiency", "double"));   // 0x1446fd900
    } else if (d.type == 4) {                       // screw press
        c.push_back(tnode("ScrewEnergy", "double"));        // 0x1446fd908
        c.push_back(tnode("ScrewEfficiency", "double"));    // 0x1446fd910
        c.push_back(tnode("MomentOfInertia", "double"));    // 0x1446fd918
        c.push_back(tnode("LeadScrewPitch", "double"));     // 0x1446fd920
    }
    return c;
}

// @src 0x14142d610 / 0x14142e260 (grow branches): "Heat" k (0x1446fd8a8) with Temperature
// (0x1446fd9b8) and NumBlows (0x1446fd8b0), then blow 0; "Blow" j (0x1446fd8b8) with its leaves
// (0x1446fd8c0..0x1446fd8e0). Every node made with index k (heat) or j (blow).
PropNode feBlowNode(int j)
{
    PropNode b = tnode("Blow", "group",
                       {tnode("BlowEnergy", "double"), tnode("BlowEfficiency", "double"),
                        tnode("BlowDwellTime", "double"), tnode("BlowReheat", "bool"), tnode("BlowFlip", "bool")});
    b.index = j;
    for (PropNode& c : b.children) c.index = j;
    return b;
}
PropNode feHeatNode(int k)
{
    PropNode h = tnode("Heat", "group", {tnode("Temperature", "double"), tnode("NumBlows", "int")});
    h.index = k;
    for (PropNode& c : h.children) c.index = k;
    h.children.push_back(feBlowNode(0));
    return h;
}

// @src 0x141429cc0  the tree, in the original's order. Root = the class name template
// (0x1446fd848 3D / 0x1446fd840 2D, named by the class interface vt+0x38, the string compared
// with "SfOprFormingExpress2D"). Heat nodes are children of the root (D+0x00). The base-property
// adders the original calls last (0x1405f9eb0, 0x1405fa560, 0x1405faaa0, 0x1405fb020) and the
// Interobject children (0x1405fad10, 0x1405fa7d0) are SfOprBaseProperties' (P02), not built here.
std::vector<PropNode> formingExpressPropTree(const FormingExpressData& d, bool is2D)
{
    PropNode root = tnode(is2D ? "SfOprFormingExpress2D" : "SfOprFormingExpress", "group");
    auto& c = root.children;
    c.push_back(tnode("EquipmentType", "group", feEquipmentTypeNodes()));      // 0x1446fd850
    c.push_back(tnode("EquipmentSpec", "group", feEquipmentSpecNodes(d)));     // 0x1446fd858
    c.push_back(tnode("MovementSpec", "group",                                 // 0x1446fd860, 0x14142cd90
                      {tnode("MovDirX", "double"), tnode("MovDirY", "double"), tnode("MovDirZ", "double")}));
    c.push_back(tnode("MultiBlow", "group",                                    // 0x1446fd888, 0x14142d490
                      {tnode("UseBlowTable", "bool"), tnode("NumHeats", "int")}));
    for (int k = 0; k < d.blows; ++k) c.push_back(feHeatNode(k));             // setNumHeats(1)
    c.push_back(tnode("Dwell", "group",                                        // 0x1446fd880, 0x14142d3a0
                      {tnode("DwellHeatCoeff", "doubleUnits"), tnode("DwellTimeStep", "doubleUnits")}));
    c.push_back(tnode("Heating", "group",                                      // 0x1446fd870, 0x14142d0b0
                      {tnode("HeatingTemperature", "double"), tnode("RoomTemperature", "double"),
                       tnode("TransferTime", "double"), tnode("HeatingTime", "double"),
                       tnode("TransferTimeStep", "double"), tnode("HeatingTimeStep", "double")}));
    c.push_back(tnode("Init", "group", {tnode("InitStrain", "bool")}));        // 0x1446fd868, 0x14142cf00
    c.push_back(tnode("Interobject", "group"));                               // 0x1446fd890, 0x14142cf80
    return {root};
}

}  // namespace m2m::data

namespace m2m {

namespace {

// Tree side of 0x14142e260 for heat node `heat` (children: Temperature, NumBlows, Blow 0..): the
// data side is feSetNumBlows.
void treeSetNumBlows(PropNode& heat, int cur, int n)
{
    if (n < cur) {
        for (int j = n; j < cur; ++j) {             // child j + 2 (FUN_1403762e0(children, j + 2))
            const size_t at = static_cast<size_t>(n + 2);
            if (at < heat.children.size()) heat.children.erase(heat.children.begin() + static_cast<long>(at));
        }
    } else {
        for (int j = cur; j < n; ++j) heat.children.push_back(data::feBlowNode(j));
    }
}

bool isBlowLeaf(const std::u16string& n)
{
    return n == u"BlowEnergy" || n == u"BlowEfficiency" || n == u"BlowDwellTime" || n == u"BlowReheat" ||
           n == u"BlowFlip";
}

}  // namespace

void FormingExpressProps::applyValue(PropNode& node, const std::vector<PropNode*>& ancestors,
                                     const qt::Variant& v)
{
    if (!fe) return;
    data::FormingExpressData& d = *fe;
    // Copies: growing or rebuilding a children vector below can move `node` (P08: it is an element
    // of its parent's vector); nothing reads `node` after that.
    const std::u16string n = node.name.u16();
    const String name = node.name;
    PropNode* parent = ancestors.empty() ? nullptr : ancestors.back();
    PropNode* grand = ancestors.size() < 2 ? nullptr : ancestors[ancestors.size() - 2];
    if (isBlowLeaf(n)) {                            // 0x14142ed40 ..: parent Blow j, grandparent Heat k
        if (!parent || !grand) return;
        data::feSetBlow(d, name, grand->index, parent->index, v);
        return;
    }
    if (n == u"Temperature") {                      // 0x1414261c0
        data::feSetTemperature(d, node.index, v);
        return;
    }
    if (n == u"NumBlows") {                         // 0x14142eb60: heat = parent, k = node index
        if (!parent) return;
        const int k = node.index;
        const int cur = (k >= 0 && k < static_cast<int>(d.steps.size())) ? d.steps[static_cast<size_t>(k)] : 0;
        int cnt = 0;
        switch (v.kind) {
        case qt::Variant::Double: cnt = static_cast<int>(v.d); break;
        case qt::Variant::Int: cnt = v.i; break;
        case qt::Variant::Text: cnt = qt::toInt(v.s); break;
        default: break;
        }
        treeSetNumBlows(*parent, cur, cnt);
        data::feSetNumBlows(d, k, cnt);
        return;
    }
    if (n == u"NumHeats") {                         // 0x14142ec90 -> 0x14142d610
        int cnt = 0;
        switch (v.kind) {
        case qt::Variant::Double: cnt = static_cast<int>(v.d); break;
        case qt::Variant::Int: cnt = v.i; break;
        case qt::Variant::Text: cnt = qt::toInt(v.s); break;
        default: break;
        }
        // Heat nodes hang under the root (D+0x00), after the nodes already there
        PropNode* root = ancestors.empty() ? nullptr : ancestors.front();
        if (root) {
            auto& ch = root->children;
            if (cnt < d.blows) {
                for (int i = cnt; i < d.blows; ++i) {   // the heat-node list entry at cnt (+0x148)
                    int seen = 0;
                    for (auto it = ch.begin(); it != ch.end(); ++it) {
                        if (it->name.u16() == u"Heat" && seen++ == cnt) {
                            ch.erase(it);
                            break;
                        }
                    }
                }
            } else {
                for (int k = d.blows; k < cnt; ++k) ch.push_back(data::feHeatNode(k));
            }
        }
        data::feSetNumHeats(d, cnt);
        return;
    }
    if (!data::feSetScalar(d, name, v)) {
        base::applyBaseValue(*this, node, ancestors, v);   // SfOprBaseProperties slots (0x1405f7b10)
        return;
    }
    if ((n == u"Equipment" || n == u"MechPressType") && !ancestors.empty()) {
        // 0x14142a860 (EquipmentType children re-made) and 0x14142b000 (EquipmentSpec by type)
        for (PropNode& g : ancestors.front()->children) {
            if (g.name.u16() == u"EquipmentType") g.children = data::feEquipmentTypeNodes();
            else if (g.name.u16() == u"EquipmentSpec") g.children = data::feEquipmentSpecNodes(d);
        }
    }
}

// ---- moproj-ops: the get slots of 0x1414645a0 (D = props+0x40) -----------------------------------

namespace {

qt::Variant gInt(int v)
{
    qt::Variant r;
    r.kind = qt::Variant::Int;
    r.i = v;
    return r;
}
qt::Variant gDouble(double v)
{
    qt::Variant r;
    r.kind = qt::Variant::Double;
    r.d = v;
    return r;
}
qt::Variant gBool(bool v)
{
    qt::Variant r;
    r.kind = qt::Variant::Bool;
    r.i = v ? 1 : 0;
    return r;
}
qt::Variant gText(const String& v)
{
    qt::Variant r;
    r.kind = qt::Variant::Text;
    r.s = v;
    return r;
}

[[noreturn]] void outOfRange(const char* slot)
{
    throw Error(M2M_E_UNSUPPORTED, std::string("project file: FormingExpress ") + slot +
                                       " with an index outside its list (QList::at, undefined in the original)");
}

}  // namespace

bool FormingExpressProps::getValue(const PropNode& node, const std::vector<PropNode*>& ancestors, qt::Variant& out)
{
    const data::FormingExpressData& d = *fe;
    const std::u16string& n = node.name.u16();
    // Blow* slots (0x14142ee80 ..): b = the grandparent's index (the heat), s = the parent's index
    // (the blow); QVariant(int 0) when either parent is missing
    auto blowAt = [&](int& b, int& s) {
        if (ancestors.size() < 2) {
            out = gInt(0);
            return false;
        }
        s = ancestors.back()->index;                                   // 0x140370f40, 0x140370fd0
        b = ancestors[ancestors.size() - 2]->index;
        return true;
    };
    auto atD = [&](const std::vector<std::vector<double>>& l, const char* slot) {
        int b = 0, s = 0;
        if (!blowAt(b, s)) return;
        if (b < 0 || b >= static_cast<int>(l.size()) || s < 0 || s >= static_cast<int>(l[b].size())) outOfRange(slot);
        out = gDouble(l[static_cast<size_t>(b)][static_cast<size_t>(s)]);   // 0x140619390, 0x1401407d0
    };
    auto atB = [&](const std::vector<std::vector<std::uint8_t>>& l, const char* slot) {
        int b = 0, s = 0;
        if (!blowAt(b, s)) return;
        if (b < 0 || b >= static_cast<int>(l.size()) || s < 0 || s >= static_cast<int>(l[b].size())) outOfRange(slot);
        out = gBool(l[static_cast<size_t>(b)][static_cast<size_t>(s)] != 0);   // 0x140619550, 0x14003f7e0
    };
    if (n == u"Equipment") out = gInt(d.type);                        // @src 0x14142a900  D+0x14
    else if (n == u"EquipName") out = gText(d.name);                  // @src 0x1414251c0  D+0x18
    else if (n == u"HammerEnergy") out = gDouble(d.hammerEnergy);     // @src 0x141425260  D+0x28
    else if (n == u"HammerMass") out = gDouble(d.hammerMass);         // @src 0x141425300  D+0x30
    else if (n == u"HammerEfficiency") out = gDouble(d.hammerEfficiency);   // @src 0x1414253a0  D+0x38
    else if (n == u"ScrewEnergy") out = gDouble(d.screwEnergy);       // @src 0x141425440  D+0x40
    else if (n == u"ScrewEfficiency") out = gDouble(d.screwEfficiency);     // @src 0x1414254e0  D+0x48
    else if (n == u"MomentOfInertia") out = gDouble(d.momentOfInertia);     // @src 0x141425580  D+0x50
    else if (n == u"LeadScrewPitch") out = gDouble(d.leadScrewPitch);       // @src 0x141425620  D+0x58
    else if (n == u"MechPressType") out = gInt(d.mechPressType);      // @src 0x14142b0a0  D+0x60
    else if (n == u"TotalStroke") out = gDouble(d.totalStroke);       // @src 0x1414256c0  D+0x68
    else if (n == u"CyclesSec") out = gDouble(d.cyclesSec);           // @src 0x141425760  D+0x70
    else if (n == u"ConnectingRodLength") out = gDouble(d.connectingRodLength);   // @src 0x141425800  D+0x78
    else if (n == u"MovDirX") out = gDouble(d.dir[0]);                // @src 0x1414258a0  D+0x88
    else if (n == u"MovDirY") out = gDouble(d.dir[1]);                // @src 0x141425950  D+0x90
    else if (n == u"MovDirZ") out = gDouble(d.dir[2]);                // @src 0x141425a00  D+0x98
    else if (n == u"InitStrain") out = gBool(d.initStrain != 0);      // @src 0x141425ac0  D+0xa8
    else if (n == u"HeatingTemperature") out = gDouble(d.toTemp);     // @src 0x141425b70  D+0xc0
    else if (n == u"RoomTemperature") out = gDouble(d.roomTemp);      // @src 0x141425c20  D+0xc8
    else if (n == u"TransferTime") out = gDouble(d.toTime);           // @src 0x141425cd0  D+0xd0
    else if (n == u"TransferTimeStep") out = gDouble(d.toStep);       // @src 0x141425d80  D+0xd8
    else if (n == u"HeatingTime") out = gDouble(d.heatingTime);       // @src 0x141425e30  D+0xe0
    else if (n == u"HeatingTimeStep") out = gDouble(d.heatingTimeStep);     // @src 0x141425ee0  D+0xe8
    else if (n == u"DwellTimeStep") out = gText(d.dwStep);            // @src 0x1414260a0  D+0xf8
    else if (n == u"DwellHeatCoeff") out = gText(d.dwIhtc);           // @src 0x141425fc0  D+0x100
    else if (n == u"UseBlowTable") out = gBool(d.express != 0);       // @src 0x141426160  D+0x110
    else if (n == u"NumHeats") out = gInt(d.blows);                   // @src 0x14142ecf0  D+0x120
    else if (n == u"Temperature") {                                   // @src 0x141426260  D+0x130[index]
        const int k = node.index;                                     // 0x140370fd0
        if (-1 < k && k < static_cast<int>(d.temperature.size())) out = gDouble(d.temperature[static_cast<size_t>(k)]);
        else out = gDouble(0.0);
    } else if (n == u"NumBlows") {                                    // @src 0x14142ec20  D+0x128.at(index)
        const int k = node.index;
        if (k < 0 || k >= static_cast<int>(d.steps.size())) outOfRange("getNumBlows");
        out = gInt(d.steps[static_cast<size_t>(k)]);
    } else if (n == u"BlowEnergy") atD(d.stroke, "getBlowEnergy");  // @src 0x14142ee80  D+0x150
    else if (n == u"BlowEfficiency") atD(d.l158, "getBlowEfficiency");   // @src 0x14142f130  D+0x158
    else if (n == u"BlowDwellTime") atD(d.gap, "getBlowDwellTime");  // @src 0x14142f3e0  D+0x160
    else if (n == u"BlowReheat") atB(d.drop, "getBlowReheat");       // @src 0x14142f680  D+0x168
    else if (n == u"BlowFlip") atB(d.flip, "getBlowFlip");           // @src 0x14142f920  D+0x170
    else return base::getValue(*this, node, out);                     // SfOprExpressProperties: none
    return true;
}

}  // namespace m2m
