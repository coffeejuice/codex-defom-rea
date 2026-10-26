// P17: default property trees of Forming, Pre, HTFurnace, ShapeOptim (OB vt+0x98).
// The Forming trees are checked against the <Properties> of corpus projects (DB_deconstruction_01,
// MO2_Boolean_Oprations, Cogging_Multiple_Ops_Lab2).
#include <string>

#include <testkit/check.hpp>

#include "m2m/p17.hpp"
#include "m2m/props_tree.hpp"

using m2m::PropNode;

static std::string dump(const std::vector<PropNode>& l, int depth = 0)
{
    std::string s;
    for (const PropNode& n : l) {
        s += std::string(static_cast<size_t>(depth) * 2, ' ') + n.name.utf8() + ":" + n.typeName.utf8() + "\n";
        s += dump(n.children, depth + 1);
    }
    return s;
}

static PropNode* find(std::vector<PropNode>& l, const char16_t* name)
{
    for (PropNode& n : l)
        if (n.name.u16() == name) return &n;
    return nullptr;
}

TEST_CASE(forming_default_tree)
{
    auto p = m2m::formingProps(true, 1);
    const std::string exp =
        "SfOprForming2D:group\n"
        "  EquipmentType:group\n"
        "    Equipment:enum\n"
        "    EquipName:QString\n"
        "  EquipmentSpec:group\n"
        "    MechPressType:enum\n"
        "    TotalStroke:double\n"
        "    CyclesSec:double\n"
        "    ConnectingRodLength:double\n";
    const std::string got = dump(p->tree);
    CHECK_EQ(got.substr(0, exp.size()), exp);
    CHECK(got.find("TestGroup:group\n  NumSteps:int\n  StepIncr:int\n  EnvTemp:doubleUnits\n"
                   "  ConvectionCoef:doubleUnits\n  StepBy:enum\n") != std::string::npos);
}

TEST_CASE(forming_equipment_rebuilds_spec)
{
    auto p = m2m::formingProps(false, 1);
    PropNode* root = &p->tree[0];
    PropNode* eq = find(find(root->children, u"EquipmentType")->children, u"Equipment");
    std::vector<PropNode*> anc{root};
    m2m::qt::Variant v;
    v.kind = m2m::qt::Variant::Int;
    v.i = 0;
    p->applyValue(*eq, anc, v);
    CHECK(find(root->children, u"EquipmentSpec")->children.empty());   // DB_deconstruction_01
    v.i = 3;
    p->applyValue(*eq, anc, v);
    CHECK_EQ(dump(find(root->children, u"EquipmentSpec")->children),
             std::string("HammerEnergy:double\nHammerMass:double\nHammerEfficiency:double\n"));
    v.i = 2;
    p->applyValue(*eq, anc, v);
    PropNode* spec = find(root->children, u"EquipmentSpec");
    PropNode* mpt = find(spec->children, u"MechPressType");
    v.i = 1;
    p->applyValue(*mpt, anc, v);
    spec = find(root->children, u"EquipmentSpec");
    CHECK(find(spec->children, u"StrokeProfile") != nullptr);
    CHECK(find(spec->children, u"ConnectingRodLength") == nullptr);
}

TEST_CASE(other_trees)
{
    CHECK_EQ(dump(m2m::shapeOptimProps()->tree),
             std::string("StopCritGroup:group\n  MaxSims:int\n  MinObjFuncVal:double\n"));
    const std::string f = dump(m2m::htFurnaceProps(1)->tree);
    CHECK_EQ(f, std::string("HTBatchFurnace3D:group\n  FURNAM:QString\n  HeatingTime:doubleUnits\n"
                            "  BVIEWFACTON:bool\nStepDefinition:group\n  STEPDEF:enum\n  DTMAX:double\n"
                            "  DTEMPMAX:double\n  DTEMPLMT0:double\n  DTEMPLMT1:double\n  STPINC:int\n"));
    auto pre = m2m::preProps(1);
    CHECK_EQ(pre->tree[0].name.utf8(), std::string("TimsTestOpr"));
    CHECK(find(pre->tree, u"TestGroup") != nullptr);
    CHECK(find(find(pre->tree, u"TestGroup")->children, u"HeatingTime") != nullptr);
}

TEST_CASE(doe_output_and_report)
{
    // corpus BEND_2D: SelecedOpr:int, SelectedObj:int
    CHECK_EQ(dump(m2m::tree::treeDoeOutput()), std::string("SelecedOpr:int\nSelectedObj:int\n"));
    CHECK_EQ(m2m::tree::treeDoeOutput()[0].value.i, 1);
    auto r = m2m::reportProps(2);
    CHECK_EQ(r->tree.size(), size_t(1));
    CHECK_EQ(r->tree[0].name.utf8(), std::string("TestGroup"));
}

int main() { return testkit::run_all(); }
