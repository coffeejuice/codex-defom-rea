// Default property trees of SfOprDistortion-, SfOprConvert2Dto3D- and SfOprCopyMirrorProperties
// (packet P07; m2m/p07_trees.hpp and the hooks of m2m/props_tree.hpp). Decompiles:
// gui_mo/out_p07/trees.c, tinit.c, dist.c, cc*.c and gui_mo/s1/p13/t1-t3.c; literals read from the
// exe copy. SfOprCycleProperties keeps the base slots (templates 0x1405f8e40, build 0x140cac130:
// nothing), so its tree is empty (in the dispatcher). Pre's builder is P17's preProps.
#include "m2m/data/convert.hpp"
#include "m2m/p07_trees.hpp"
#include "m2m/props_tree.hpp"

namespace m2m::trees {
using namespace m2m::tree;

// @src 0x141365c90  templates: 0x1446f8a60 (vt+0x38 == "SfOprDistortion2D", 0x142e2f630) or
// 0x1446f8a68: group named OB vt+0x38, caption vt+0x40; 0x1446f8a80 NumFixtures (int 0),
// 0x1446f8a88 NumPasses (int 0). Called from the props init 0x141365a70 with the OB.
// @src 0x141366100  build: root at props+0x18, kept in the holder at props+0x38; the root and both
// children get enabled = 1 (0x140370e90, UI only). The base adders follow in makeProps:
// 0x1405f9eb0(false, false, 2, 1, "1.0 sec"), 0x1405fa560("68.00 F", "20.00 C"),
// 0x1405faaa0("0.0 Btu/sec/in^2/F", "0.0 N/sec/mm/C"), 0x1405fb020(0, "0.0 in", "0.0 mm", "0.0 sec").
std::vector<PropNode> distortionPropTree(const qt::String& classKey)
{
    const Template root{classKey, qt::L("group"), vInt(0)};   // 0x140375640 with the class name
    const Template numFixtures = intProp("NumFixtures", 0);   // 0x142e2f660
    const Template numPasses = intProp("NumPasses", 0);       // 0x142e2f688
    std::vector<PropNode> tree;
    PropNode& r = add(tree, root);                           // 0x140375ba0
    add(r.children, numFixtures);                            // 0x140375a70
    add(r.children, numPasses);
    return tree;
}

}  // namespace m2m::trees

namespace m2m::tree {

namespace {

// Data block of the Convert2Dto3D and CopyMirror props (props+0x38, ctor 0x140e2d400): the
// template initialisers take their default values from it (m2m/data/convert.hpp).
using data::ConvertData;

// The common body of 0x1401cdf30 / 0x140e2cbd0 over the templates of 0x1401cd5a0 / 0x140e2c240:
// root group named OB vt+0x38 under props+0x18 (kept at data+0x38), then the eight doubles. Each
// template is a double (0x1403738e0) with 4 decimals and step 0.1; ints of the data are widened.
std::vector<PropNode> convertTree(const char* classKey)
{
    const ConvertData d;
    const Template root{qt::L(classKey), qt::L("group"), vInt(0)};
    const Template kids[] = {
        doubleProp("DTUPAXIS", static_cast<double>(d.upAxis)),       // "Up Axis"
        doubleProp("DTCONTYPE", static_cast<double>(d.conType)),     // "Conversion Type"
        doubleProp("DTREVTYPE", static_cast<double>(d.revType)),     // "Revolve Type"
        doubleProp("DTREVANGLE", d.revAngle),                        // "Revolution angle"
        doubleProp("DTREVSTANGLE", d.revStartAngle),                 // "Start angle"
        doubleProp("DTREVTOL", d.revTol),                            // "Tolerance"
        doubleProp("DTEXTLEN", d.extLen),                            // "Length"
        doubleProp("DTEXTSTLOC", d.extStart),                        // "Start location"
    };
    std::vector<PropNode> tree;
    PropNode& r = add(tree, root);                               // 0x140375ba0
    for (const Template& t : kids) add(r.children, t);           // 0x140375a70
    return tree;
}

}  // namespace

// @src 0x1401cd390  SfOprConvert2Dto3DProperties ctor -> 0x1401cd460: data 0x140e2d400, templates
// 0x1401cd5a0 (globals 0x143d83c00..0x143d83c40, only when vt+0x38 == "SfOprConvert2Dto3D",
// 0x142211630), build 0x1401cdf30 (same test, 0x142211738). No base adders.
// @src 0x1401cd5a0
// @src 0x1401cdf30
std::vector<PropNode> treeConvert2Dto3D()
{
    return convertTree("SfOprConvert2Dto3D");
}

// @src 0x140e2c030  SfOprCopyMirrorProperties ctor -> 0x140e2c100: data 0x140e2d400, templates
// 0x140e2c240 (globals 0x1446d2300..0x1446d2340, vt+0x38 == "SfOprCopyMirror", 0x142ace6c0),
// build 0x140e2cbd0 (0x142ace7c0). Same names and defaults as Convert2Dto3D. No base adders.
// @src 0x140e2c240
// @src 0x140e2cbd0
std::vector<PropNode> treeCopyMirror()
{
    return convertTree("SfOprCopyMirror");
}

}  // namespace m2m::tree
