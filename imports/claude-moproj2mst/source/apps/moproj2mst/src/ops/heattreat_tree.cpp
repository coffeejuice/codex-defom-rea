// SfOprHeatTreat / SfOprHeatTreat2D default property tree (packet P15, moved from P16 by the
// coordinator). Props ctor 0x1411bd570 -> 0x1411bd6a0: control block +0x58 (ctor 0x1411c2ec0), schedule
// +0x68, templates 0x1411bd890 (globals 0x1446e27e0..0x1446e2898), tree 0x1411bf050. Both classes'
// vt+0x98 (0x1411b2310, 0x1411afcf0) pass 0, so the root is "HTWizard" and the Mode group is built.
// No base adders are called. Decompiles: gui_mo/s1/p15/h1.c .. h5.c.
#include "m2m/props_tree.hpp"

namespace m2m::tree {

// @src 0x1411bd570 -> 0x1411bd890 (templates) + 0x1411bf050 (tree). Defaults: the template values,
// the bools 0 and the StepDefinition values from the control block ctor 0x1411c2ec0 (+0x8 STEPDEF 1,
// +0xc STPINC 10, +0x10 DTMAX 0.001, +0x18 DTEMPMAX 2.0, +0x20 DTEMPLMT0 = DTMAX, +0x28 DTEMPLMT1 10.0).
// The schedule children (Process per medium: TMAX, MEDNAM, ENVTMP, CNVCOF, ENVATM, ACVCOF, ENVRAD;
// templates 0x1446e2860..0x1446e2898) are added by setNO_MEDIA (0x1411c20e0 -> 0x1411c0c30), not here.
std::vector<PropNode> treeHeatTreat(const String&)
{
    std::vector<PropNode> roots;
    // 0x1446e27e0 = 0x1446e27e8 (param 0): "HTWizard"
    PropNode& root = add(roots, group("HTWizard"));
    {
        PropNode& mode = add(root.children, group("Mode"));                       // 0x1446e27f8
        add(mode.children, other("PhaseTransformation", "bool", vInt(0)));       // 0x1446e2810
        add(mode.children, other("Diffusion", "bool", vInt(0)));                 // 0x1446e2808
        add(mode.children, other("Deformation", "bool", vInt(0)));               // 0x1446e2800
    }
    add(root.children, group("Thermal Schedule"));                               // 0x1446e2818 (+ vt+0x60 sync)
    PropNode& step = add(root.children, group("StepDefinition"));               // 0x1446e2828
    add(step.children, other("STEPDEF", "enum", vInt(1)));                       // 0x1446e2830
    add(step.children, doubleProp("DTMAX", 0.001));                              // 0x1446e2838
    add(step.children, doubleProp("DTEMPMAX", 2.0));                             // 0x1446e2848
    add(step.children, doubleProp("DTEMPLMT0", 0.001));                          // 0x1446e2850
    add(step.children, doubleProp("DTEMPLMT1", 10.0));                           // 0x1446e2858
    add(step.children, intProp("STPINC", 10));                                   // 0x1446e2840
    return roots;
}

}  // namespace m2m::tree
