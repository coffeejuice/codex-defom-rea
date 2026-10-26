// P17: default property trees (OB->vt[0x98]) of Forming(2D), Pre(2D/3D), HTFurnace and ShapeOptim,
// and the Forming property slots that reshape the tree. None of these values reaches the MST; the
// trees exist so that the reader (0x1400952c0) finds the saved <Property> names as the original does.
// Template names and kinds: the template registrations 0x140c08380 (Forming), 0x14134a9e0 (Pre),
// 0x140c67f60 (HTFurnace), 0x140142800 (ShapeOptim), read from their disassembly (gui_mo/s1/p17/d2.txt,
// d1.txt); tree builders b2.c/b3.c/b5.c; slots b9.c/b10.c. Template kinds by registering function as
// in multiblow_props.cpp; "QString" is the corpus TypeName of the text kind (0x140374ce0).
#include "m2m/p17.hpp"

#include <memory>

#include <defmodel/layout/cls_1403753f0.hpp>

#include "m2m/data/common.hpp"
#include "m2m/data/heattransfer.hpp"
#include "m2m/props_base.hpp"
#include "m2m/props_forming.hpp"
#include "m2m/props_tree.hpp"

namespace m2m {

namespace {

PropNode node(const char* name, const char* type, std::vector<PropNode> children = {})
{
    PropNode n;
    n.name = qt::L(name);
    n.typeName = qt::L(type);
    n.children = std::move(children);
    return n;
}

int vInt(const qt::Variant& v)     // QVariant::toInt of the reader's value
{
    switch (v.kind) {
    case qt::Variant::Double: return static_cast<int>(v.d);
    case qt::Variant::Int: return v.i;
    case qt::Variant::Text: return qt::toInt(v.s);
    default: return 0;
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

qt::Variant vText(const qt::String& s)
{
    qt::Variant v;
    v.kind = qt::Variant::Text;
    v.s = s;
    return v;
}

PropNode* child(std::vector<PropNode>& list, const char16_t* name)
{
    for (PropNode& n : list)
        if (n.name.u16() == name) return &n;
    return nullptr;
}

// ---- Forming ------------------------------------------------------------------------------------

// FormingData, FormingProps: m2m/props_forming.hpp (moved there by MDT L3)

// @src 0x140c0a270  EquipmentSpec children by the equipment type D+0x14
std::vector<PropNode> formingSpecNodes(const FormingData& d)
{
    std::vector<PropNode> c;
    if (d.type == 2) {
        c.push_back(node("MechPressType", "enum"));               // 0x1446a7ec8
        c.push_back(node("TotalStroke", "double"));               // 0x1446a7ed0
        c.push_back(node("CyclesSec", "double"));                 // 0x1446a7ed8
        if (d.mechPressType == 0)
            c.push_back(node("ConnectingRodLength", "double"));   // 0x1446a7ee0
        else
            c.push_back(node("StrokeProfile", "action"));         // 0x1446a7ee8 (0x1403751b0)
    } else if (d.type == 3) {
        c.push_back(node("HammerEnergy", "double"));              // 0x1446a7e90
        c.push_back(node("HammerMass", "double"));                // 0x1446a7e98
        c.push_back(node("HammerEfficiency", "double"));          // 0x1446a7ea0
    } else if (d.type == 4) {
        c.push_back(node("ScrewEnergy", "double"));               // 0x1446a7ea8
        c.push_back(node("ScrewEfficiency", "double"));           // 0x1446a7eb0
        c.push_back(node("MomentOfInertia", "double"));           // 0x1446a7eb8
        c.push_back(node("LeadScrewPitch", "double"));            // 0x1446a7ec0
    }
    return c;
}

}  // namespace

void FormingProps::applyValue(PropNode& n, const std::vector<PropNode*>& anc, const qt::Variant& v)
{
    const std::u16string& s = n.name.u16();
    auto rebuild = [&] {
        if (tree.empty()) return;
        if (PropNode* spec = child(tree[0].children, u"EquipmentSpec")) spec->children = formingSpecNodes(d);
    };
    if (s == u"Equipment") {                   // @src 0x140c0a070 (also SimCtrl+0x68 of the engine model)
        d.type = vInt(v);
        rebuild();
    } else if (s == u"MechPressType") {        // @src 0x140c0a950
        d.mechPressType = vInt(v);
        rebuild();
    } else if (s == u"EquipName") {            // @src 0x140c075f0
        d.name = v.kind == qt::Variant::Text ? v.s : String();
    } else if (s == u"HammerEnergy") {         // @src 0x140c076c0  QVariant::toDouble -> D+0x28
        d.hammerEnergy = vDouble(v);
    } else if (s == u"HammerMass") {           // D+0x30 (the set slots 0x140c07760 .. 0x140c07c60 store
        d.hammerMass = vDouble(v);             // toDouble into one field each; moproj-ops)
    } else if (s == u"HammerEfficiency") {     // D+0x38
        d.hammerEfficiency = vDouble(v);
    } else if (s == u"ScrewEnergy") {          // D+0x40
        d.screwEnergy = vDouble(v);
    } else if (s == u"ScrewEfficiency") {      // D+0x48
        d.screwEfficiency = vDouble(v);
    } else if (s == u"MomentOfInertia") {      // D+0x50
        d.momentOfInertia = vDouble(v);
    } else if (s == u"LeadScrewPitch") {       // D+0x58
        d.leadScrewPitch = vDouble(v);
    } else if (s == u"TotalStroke") {          // D+0x68
        d.totalStroke = vDouble(v);
    } else if (s == u"CyclesSec") {            // D+0x70
        d.cyclesSec = vDouble(v);
    } else if (s == u"ConnectingRodLength") {  // @src 0x140c07c60  D+0x78
        d.connectingRodLength = vDouble(v);
    } else {
        Props::applyValue(n, anc, v);          // base slots (P15)
    }
}

// @src 0x140c0ad80  get cases: 0x140c0a220 Equipment (D+0x14), 0x140c07670 EquipName (D+0x18),
// 0x140c07710 .. 0x140c07ad0 the hammer and screw doubles (D+0x28 .. D+0x58), 0x140c0a9c0
// MechPressType (D+0x60), 0x140c07b70 / 0x140c07c10 / 0x140c07cb0 the press doubles (D+0x68 ..
// D+0x78); other names go to the base metaobject (moproj-ops)
bool FormingProps::getValue(const PropNode& n, const std::vector<PropNode*>&, qt::Variant& out)
{
    const std::u16string& s = n.name.u16();
    auto num = [&](double x) { out.kind = qt::Variant::Double; out.d = x; };
    if (s == u"Equipment") { out.kind = qt::Variant::Int; out.i = d.type; }
    else if (s == u"EquipName") { out.kind = qt::Variant::Text; out.s = d.name; }
    else if (s == u"HammerEnergy") num(d.hammerEnergy);
    else if (s == u"HammerMass") num(d.hammerMass);
    else if (s == u"HammerEfficiency") num(d.hammerEfficiency);
    else if (s == u"ScrewEnergy") num(d.screwEnergy);
    else if (s == u"ScrewEfficiency") num(d.screwEfficiency);
    else if (s == u"MomentOfInertia") num(d.momentOfInertia);
    else if (s == u"LeadScrewPitch") num(d.leadScrewPitch);
    else if (s == u"MechPressType") { out.kind = qt::Variant::Int; out.i = d.mechPressType; }
    else if (s == u"TotalStroke") num(d.totalStroke);
    else if (s == u"CyclesSec") num(d.cyclesSec);
    else if (s == u"ConnectingRodLength") num(d.connectingRodLength);
    else return base::getValue(*this, n, out);
    return true;
}

// ---- added by MDT L3: the props load 0x140c09da0 re-makes both groups (0x140c07eb0) ------------

// @src 0x140c0a040
void formingRemakeEquipmentType(FormingProps& p)
{
    if (p.tree.empty()) return;
    if (PropNode* g = child(p.tree[0].children, u"EquipmentType"))
        g->children = {node("Equipment", "enum"), node("EquipName", "QString")};   // 0x140c0a010, 0x140c09f20
}

// @src 0x140c0a920
void formingRemakeEquipmentSpec(FormingProps& p)
{
    if (p.tree.empty()) return;
    if (PropNode* g = child(p.tree[0].children, u"EquipmentSpec"))
        g->children = formingSpecNodes(p.d);                                    // 0x140c0a8e0, 0x140c0a270
}

namespace base {

// @src 0x1405fa2c0  HeatingTime (template 0x143d9dcf0) under `parent`, or under TestGroup (made when
// missing) when parent is null; B+0x18 = value; B+0x48 = label (display text, not modelled)
void addHeatingTime(Props& p, PropNode* parent, const qt::String& value, const qt::String& /*label*/)
{
    if (!parent) {
        if (!testGroup(p)) {
            PropNode g = node("TestGroup", "group");               // 0x143d9dcd0
            p.tree.push_back(std::move(g));
        }
        parent = testGroup(p);
    }
    if (!p.common) p.common = std::make_shared<data::CommonBlock>();
    p.common->heatingTime = value;
    PropNode h = node("HeatingTime", "doubleUnits");
    h.value = vText(value);
    parent->children.push_back(std::move(h));
}

}  // namespace base

// @src 0x140c070b0 / 0x140c059f0 -> 0x140c07d00 -> 0x140c07dd0 -> 0x140c09850
std::shared_ptr<Props> formingProps(bool is2D, int guiUnit)
{
    auto p = std::make_shared<FormingProps>();
    data::initCommonBlock(*p->common, guiUnit);       // base ctor 0x1405f8ca0 -> 0x1405fd250
    p->is2D = is2D;
    // root: 0x1446a7e60 "SfOprForming2D" when the class name is "SfOprForming2D", else 0x1446a7e68
    // (named after the class, "SfOprForming")
    PropNode root = node(is2D ? "SfOprForming2D" : "SfOprForming", "group");
    // @src 0x140c09f20
    root.children.push_back(node("EquipmentType", "group",
                                 {node("Equipment", "enum"), node("EquipName", "QString")}));
    root.children.push_back(node("EquipmentSpec", "group", formingSpecNodes(p->d)));
    p->tree.push_back(std::move(root));
    // D+0x10 (disabled) is 0 after the ctor, so nothing is disabled
    base::addStepCounts(*p, false, false, 100, 10, qt::L("0.0 sec"));          // 0x1428136b0
    base::addEnvTemp(*p, guiUnit, qt::L("68.00 F"), qt::L("20.00 C"));          // 0x1428136c0 / b8
    base::addConvection(*p, guiUnit, qt::L("0.0 Btu/sec/in^2/F"), qt::L("0.0 N/sec/mm/C"));
    base::addStepBy(*p, guiUnit, 0, qt::L("0.0 in"), qt::L("0.0 mm"), qt::L("0.0 sec"));
    return p;
}

// @src 0x141349c50 / 0x141348810 -> 0x14134a880 -> 0x14134a940 (data 0x14134c570: list 5, 10, 15, 20)
// -> second vtable +0x20 = 0x14134b9c0 (Tim's test tree)
std::shared_ptr<Props> preProps(int guiUnit)
{
    auto p = std::make_shared<PreProps>();
    p->data = std::make_shared<data::PreData>();               // 0x14134a940: new(0x48), 0x14134c570
    data::initCommonBlock(*p->common, guiUnit);       // base ctor 0x1405f8ca0 -> 0x1405fd250
    PropNode root = node("TimsTestOpr", "group");                  // 0x1446f5140
    PropNode tg = node("TestGroup", "group",                       // 0x1446f5148
                       {node("Dwell", "double"), node("NumHeats", "int"),           // 5168, 5160
                        node("BoolTest", "bool"), node("ReadOnlyTest", "int")});    // 5178, 5158
    root.children.push_back(std::move(tg));
    root.children.push_back(node("TimsEnum", "enum"));             // 0x1446f5180
    root.children.push_back(node("Double6", "double"));            // 0x1446f5170
    root.children.push_back(node("ButtonTest", "action"));         // 0x1446f5150
    for (int i = 0; i < 4; ++i) {                                  // 0x1446f5188, index i
        PropNode l = node("ListTest", "int");
        l.index = i;
        root.children.push_back(std::move(l));
    }
    root.children.push_back(node("Length", "doubleUnits"));        // 0x1446f5190
    root.children.push_back(node("Time", "doubleUnits"));          // 0x1446f5198
    root.children.push_back(node("AngVel", "doubleUnits"));        // 0x1446f51a0
    p->tree.push_back(std::move(root));
    base::addStepCounts(*p, false, false, 100, 10, qt::L("0.0 sec"));          // 0x142dd5310
    base::addHeatingTime(*p, nullptr, qt::L("20.00 sec"), qt::L("Tims Heating Time"));
    base::addEnvTemp(*p, guiUnit, qt::L("68.00 F"), qt::L("20.00 C"));
    base::addConvection(*p, guiUnit, qt::L("0.0 Btu/sec/in^2/F"), qt::L("0.0 N/sec/mm/C"));
    base::addStepBy(*p, guiUnit, 0, qt::L("0.0 in"), qt::L("0.0 mm"), qt::L("0.0 sec"));
    return p;
}

// @src 0x140c663d0 -> 0x140c67820 -> 0x140c67980 (data 0x140c6a190: name "HT Furnace", STEPDEF 1,
// STPINC 10) -> second vtable +0x20 = 0x140c68ab0. The parents of FURNAM, HeatingTime and BVIEWFACTON
// (the root) and of the StepDefinition leaves are inferred from the HeatTransfer trees of the corpus.
std::shared_ptr<Props> htFurnaceProps(int guiUnit)
{
    auto p = std::make_shared<HTFurnaceProps>();
    p->furnace = std::make_shared<def::model::Cls_1403753f0>();   // +0x38 (MDT L2 payloads)
    p->data = std::make_shared<data::HTFurnaceData>();             // +0xd0 (0x140c67980)
    data::initCommonBlock(*p->common, guiUnit);       // base ctor 0x1405f8ca0 -> 0x1405fd250
    p->tree.push_back(node("HTBatchFurnace3D", "group", {node("FURNAM", "QString")}));   // afa20, afa28
    base::addHeatingTime(*p, &p->tree.back(), qt::L("3600.0 sec"), qt::L("Heating Time"));   // 0x1405fa2c0 (0x1428caa78, 0x1428caa68)
    p->tree.back().children.push_back(node("BVIEWFACTON", "bool"));                     // afa40
    p->tree.push_back(node("StepDefinition", "group",                                   // afa30
                           {node("STEPDEF", "enum"), node("DTMAX", "double"),           // afa38, afa48
                            node("DTEMPMAX", "double"), node("DTEMPLMT0", "double"),    // afa58, afa60
                            node("DTEMPLMT1", "double"), node("STPINC", "int")}));      // afa68, afa50
    return p;
}

// @src 0x14013be10 -> 0x140142620 -> 0x140142720 (data 0x140142d90: MaxSims 200) -> 0x140142b00
std::shared_ptr<Props> shapeOptimProps(int guiUnit)
{
    auto p = std::make_shared<ShapeOptimProps>();
    p->data = std::make_shared<data::ShapeOptimData>();        // 0x140142720: new(0x18), 0x140142d90
    data::initCommonBlock(*p->common, guiUnit);       // base ctor 0x1405f8ca0 -> 0x1405fd250
    p->tree.push_back(node("StopCritGroup", "group",                  // 0x143d80f70
                           {node("MaxSims", "int"), node("MinObjFuncVal", "double")}));  // f78, f80
    return p;
}

}  // namespace m2m

// ---- DoeOutput and Report (props ctors 0x140e35ec0 / 0x1412b1a90; P11 dispatcher) --------------
namespace m2m {

// @src 0x1412b1a90 -> 0x1412b1b50 (data 0x141377790) -> second vtable +0x18 0x1412b1bf0 (no
// templates), +0x20 0x1412b1c00: only the base adders (literals 0x142d6fb18 .. 0x142d6fb90)
std::shared_ptr<Props> reportProps(int guiUnit)
{
    auto p = std::make_shared<BaseSlotsProps>();                 // no get slots of its own (L2)
    data::initCommonBlock(*p->common, guiUnit);       // base ctor 0x1405f8ca0 -> 0x1405fd250
    base::addStepCounts(*p, false, false, 100, 10, qt::L("0.0 sec"));
    base::addHeatingTime(*p, nullptr, qt::L("20.00 sec"), qt::L("Tims Heating Time"));
    base::addEnvTemp(*p, guiUnit, qt::L("68.00 F"), qt::L("20.00 C"));
    base::addConvection(*p, guiUnit, qt::L("0.0 Btu/sec/in^2/F"), qt::L("0.0 N/sec/mm/C"));
    base::addStepBy(*p, guiUnit, 0, qt::L("0.0 in"), qt::L("0.0 mm"), qt::L("0.0 sec"));
    return p;
}

namespace tree {

// @src 0x140e35ec0 -> 0x140e35f90 (data 0x140e363e0: 1, 1) -> templates @src 0x140e36030
// (0x1446d2e10 "SelecedOpr" sic, 0x1446d2e18 "SelectedObj", int 1) -> builder @src 0x140e361f0
// (both top-level)
std::vector<PropNode> treeDoeOutput()
{
    std::vector<PropNode> t;
    add(t, intProp("SelecedOpr", 1));
    add(t, intProp("SelectedObj", 1));
    return t;
}

// SfOprReport's tree is made only by the base adders, whose defaults depend on the GUI unit:
// the dispatcher should take reportProps(guiUnit); this unit-less form uses SI (1).
std::vector<PropNode> treeReport()
{
    return reportProps(1)->tree;
}

}  // namespace tree
}  // namespace m2m
