// Default property trees of SfOprDiestressProperties and SfOprDiestressBMProperties (packet P16;
// hooks of include/m2m/props_tree.hpp). Decompiles: gui_mo/out_p16/dstree.c, dsinit.c, dsctor.c,
// blk.c; literals read from the exe copy.
//
// Both props ctors (0x1402b8fb0 / 0x14029edb0) make an equipment block (0x90 bytes, ctor 0x1402a2a20)
// at props+0x38 / props+0x150, run the template initializer and then the builder. The builder reads
// the fresh block: +0x10 = 0 (children not enabled, UI only), +0x14 = 2 (mechanical press),
// +0x60 = 0 (crank press), +0x80 = -1 (step number). Templates are made once (each maker returns
// early when its global is set), so the StepNumber default is the first block's -1.
// The base adders that end both builders are in makeProps (diestress.cpp).
#include "m2m/props_tree.hpp"

namespace m2m::tree {
namespace {

struct EquipmentTemplates {
    Template equipmentType = group("EquipmentType");                 // tr("Equipment Type")
    Template equipmentSpec = group("EquipmentSpec");                 // tr("Equipment Spec")
    // enum "Equipment" (tr "Type"), names Speed, Force, Mechanical, Hammer, Screw; value 1
    Template equipment = other("Equipment", "enum", vInt(1));
    Template equipName = other("EquipName", "QString", vText(qt::L("Undefined")));   // tr "Name"
    Template hammerEnergy = doubleProp("HammerEnergy", 79200.0);       // tr "Energy"
    Template hammerMass = doubleProp("HammerMass", 5000.0);            // tr "Hammer Mass"
    Template hammerEfficiency = doubleProp("HammerEfficiency", 0.9);   // tr "Efficiency"
    Template screwEnergy = doubleProp("ScrewEnergy", 79200.0);         // tr "Energy"
    Template screwEfficiency = doubleProp("ScrewEfficiency", 0.9);     // tr "Efficiency"
    Template momentOfInertia = doubleProp("MomentOfInertia", 4606.46); // tr "Moment of Inertia"
    Template leadScrewPitch = doubleProp("LeadScrewPitch", 21.09);     // tr "Lead Screw Pitch"
    // enum "MechPressType" (tr "Press Type"), names Crank press, Knuckle or Wedge; value 0
    Template mechPressType = other("MechPressType", "enum", vInt(0));
    Template totalStroke = doubleProp("TotalStroke", 10.0);            // tr "Total Stroke"
    Template cyclesSec = doubleProp("CyclesSec", 0.6);                 // tr "Cycles/sec"
    Template connectingRodLength = doubleProp("ConnectingRodLength", 0.9);   // tr "Connecting Rod Length"
    // action "StrokeProfile" (tr "Stroke Profile"), value "Edit..." (0x1403751b0)
    Template strokeProfile = other("StrokeProfile", "action", vText(qt::L("Edit...")));
    Template stepNumber = intProp("StepNumber", -1);                   // tr "Step Number"; block+0x80
};

// Block values the builder reads, as the ctor 0x1402a2a20 leaves them.
struct EquipmentBlock {
    int equipmentType = 2;         // +0x14
    int pressType = 0;             // +0x60
};

// Builders 0x1402bb080 / 0x14029f0a0 with their parts 0x1402bb640 / 0x1402a0ba0 (equipment type)
// and 0x1402bb850 / 0x1402a0dd0 (equipment spec).
std::vector<PropNode> buildEquipmentTree(const String& rootName, const EquipmentBlock& b)
{
    const EquipmentTemplates t;
    std::vector<PropNode> tree;
    add(tree, Template{rootName, qt::L("group"), vInt(0)});   // 0x140375ba0, kept at block+0x00
    {
        PropNode& type = add(tree.back().children, t.equipmentType);   // block+0x20
        add(type.children, t.equipment);
        add(type.children, t.equipName);
    }
    {
        PropNode& spec = add(tree.back().children, t.equipmentSpec);   // block+0x88
        if (b.equipmentType == 2) {
            add(spec.children, t.mechPressType);
            add(spec.children, t.totalStroke);
            add(spec.children, t.cyclesSec);
            if (b.pressType == 0) add(spec.children, t.connectingRodLength);
            else add(spec.children, t.strokeProfile);
        } else if (b.equipmentType == 3) {
            add(spec.children, t.hammerEnergy);
            add(spec.children, t.hammerMass);
            add(spec.children, t.hammerEfficiency);
        } else if (b.equipmentType == 4) {
            add(spec.children, t.screwEnergy);
            add(spec.children, t.screwEfficiency);
            add(spec.children, t.momentOfInertia);
            add(spec.children, t.leadScrewPitch);
        }
    }
    add(tree.back().children, t.stepNumber);
    return tree;
}

}  // namespace

// @src 0x1402bb080 (templates 0x1402b9b60, globals 0x143d90020..0x143d900b0). The root template is
// 0x143d90020 when the class name equals "SfOprFormingExpress2D" (as written in the original),
// else 0x143d90028; either way a group named by the class (OB vt+0x38).
std::vector<PropNode> treeDiestress()
{
    return buildEquipmentTree(qt::L("SfOprDiestress"), EquipmentBlock{});
}

// @src 0x14029f0a0 (templates 0x14029f680, globals 0x143d8e0c0..0x143d8e150): the same tree; root
// 0x143d8e0c0 for "SfOprDiestressBM2D", else 0x143d8e0c8, named by the class.
std::vector<PropNode> treeDiestressBM(const String& classKey)
{
    return buildEquipmentTree(classKey, EquipmentBlock{});
}

}  // namespace m2m::tree
