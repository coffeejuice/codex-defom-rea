// SfOprHeatTransfer / 2D / Common default properties (packet P11). Decompiles: gui_mo/out_p10/ptree.c
// (0x1413561c0, 0x141356c60), out_p11/ht_d1.c .. ht_d4.c (0x141356060, vt+0x60/+0x100/+0x108/+0x120).
#include "m2m/data/common.hpp"
#include "m2m/data/heattransfer.hpp"
#include "m2m/props_base.hpp"
#include "m2m/props_tree.hpp"
#include "m2m/p17.hpp"

namespace m2m::tree {

// vt+0x98 0x1413549f0 / 0x141353cf0 / 0x141355430 -> props ctor 0x141355f00(dim): base ctor 0x1405f8bb0
// (CommonBlock 0x1405fd250), step data 0x141359a60 (props+0x58: 1, 10, 0.001, 2.0, 0.001, 10.0), then
// 0x141356060: templates 0x1413561c0(dim), builder 0x141356c60, vt+0x120 / +0x108 (owner sync: no owner
// yet, no effect), vt+0x100 0x141357600.
std::shared_ptr<Props> heatTransferProps(const String& classKey, int guiUnit)
{
    auto p = std::make_shared<HeatTransferProps>();
    p->step = std::make_shared<data::HeatTransferStepData>();   // 0x141359a60 (MDT L2 payload)
    data::initCommonBlock(*p->common, guiUnit);
    const bool is2D = classKey.u16() == u"SfOprHeatTransfer2D";   // dim 2 when vbase->v58() & 1

    // 0x141356c60: class root, template 0x1446f72d0 = 0x1446f72c0 (dim 2) or 0x1446f72c8
    PropNode& root = add(p->tree, group(is2D ? "HeatTrasnfer2D" : "HeatTrasnfer3D"));   // 0x142e09de8 / e10
    base::addHeatingTime(*p, &root, qt::L("15.0 sec"), qt::L("Transfer Time"));        // 0x1405fa2c0
    base::addEnvTemp(*p, guiUnit, qt::L("68 F"), qt::L("20 C"));                       // 0x1405fa560
    base::addConvection(*p, guiUnit, qt::L("7.7e-06 Btu/sec/in^2/F"),                 // 0x1405faaa0
                        qt::L("0.0226611 N/sec/mm/C"));
    // vt+0x60 0x141357590: copies CommonBlock into the owner's SimCtrl data (0x1405fb980 / fbd10 /
    // fc060); no owner while the ctor runs, so nothing.
    // StepDefinition group 0x1446f72e0 (top level), children from the templates of 0x1413561c0, whose
    // defaults come from the step data at props+0x58.
    PropNode& sd = add(p->tree, group("StepDefinition"));                              // 0x142e09e30
    add(sd.children, other("STEPDEF", "enum", vInt(1)));       // 0x1446f72e8, 0x1403747c0: User/System/Auto
    add(sd.children, doubleProp("DTMAX", 0.001));             // 0x1446f72f0 (+8)
    add(sd.children, doubleProp("DTEMPMAX", 2.0));            // 0x1446f7300 (+0x10)
    add(sd.children, doubleProp("DTEMPLMT0", 0.001));         // 0x1446f7308 (+0x18)
    add(sd.children, doubleProp("DTEMPLMT1", 10.0));          // 0x1446f7310 (+0x20)
    add(sd.children, intProp("STPINC", 10));                  // 0x1446f72f8 (+4)

    // vt+0x100 0x141357600 -> 0x1405fc9a0: CommonBlock+0x28 = QString::number(unit == 2 ? 0.0003 : 1.0)
    p->common->ihtc = qt::number(guiUnit == 2 ? 0.0003 : 1.0, 'g', 6);   // 0x142498298 / 0x1421249e0
    return p;
}

}  // namespace m2m::tree
