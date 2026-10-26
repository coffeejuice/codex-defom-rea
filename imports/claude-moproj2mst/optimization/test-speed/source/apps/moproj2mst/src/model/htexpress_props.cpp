// Default properties of SfOprHeatTransferExpress / 2D (OB vt+0x98 0x1413e66b0 / 0x1413e5de0 ->
// SfOprHeatTransferExpressProperties ctor 0x1413eb5d0), moved P11 -> P01 (coordinator 2026-10-22).
// Decompiles gui_mo/out_p11/ht_d0.c, ht_d1.c (0x1413eb740, 0x1413eb8b0, 0x1413ec360, 0x141359a60,
// 0x1413ed900); string literals read from the exe copy. The corpus check: the Properties of the
// SfOprAirConvect2D operation of 46_MO2_Boolean_Oprations name exactly these nodes, in this order.
#include "m2m/data/common.hpp"
#include "m2m/data/heattransfer.hpp"
#include "m2m/p17.hpp"
#include "m2m/props_base.hpp"
#include "m2m/props_tree.hpp"

namespace m2m::tree {

namespace {

// @src 0x141359a60  step-definition data (new 0x38 at props+0x60): +0 = 1, +4 = 10 (STPINC),
// +8 = 0.001 (DTMAX), +0x10 = 2.0 (DTEMPMAX), +0x18 = +8 (DTEMPLMT0), +0x20 = 10.0 (DTEMPLMT1);
// +0x28 the StepDefinition group list
struct StepDefData {
    int f0 = 1;
    int stpinc = 10;
    double dtmax = 0.001;          // 0x1421b5050
    double dtempmax = 2.0;         // 0x142101520
    double dtemplmt0 = 0.001;
    double dtemplmt1 = 10.0;       // 0x142142078
};

// @src 0x1413eb8b0  the templates of the class (globals 0x1446fa6b0 .. 0x1446fa760). `dim` 2
// gives the 2D root. The enum STEPDEF lists "User", "System", "Auto" with default 1 (0x1403747c0).
struct Templates {
    Template root, stepDef, stepdefEnum, dtmax, stpinc, dtempmax, dtemplmt0, dtemplmt1;
};

Templates makeTemplates(int dim, const StepDefData& d)
{
    Templates t;
    t.root = group(dim == 2 ? "HeatTrasnferExpress2D" /*0x142e6e270*/ : "HeatTrasnferExpress3D" /*0x142e6e2a8*/);
    t.stepDef = group("StepDefinition");                        // 0x142e6e2d0
    t.stepdefEnum = other("STEPDEF", "enum", vInt(1));          // 0x142e6e308 (User/System/Auto)
    t.dtmax = doubleProp("DTMAX", d.dtmax);                     // 0x142e6e320
    t.stpinc = intProp("STPINC", d.stpinc);                     // 0x142e6e338
    t.dtempmax = doubleProp("DTEMPMAX", d.dtempmax);            // 0x142e6e358
    t.dtemplmt0 = doubleProp("DTEMPLMT0", d.dtemplmt0);         // 0x142e6e380
    t.dtemplmt1 = doubleProp("DTEMPLMT1", d.dtemplmt1);         // 0x142e6e3a8
    return t;
}

}  // namespace

// @src 0x1413eb5d0 -> 0x1413eb740 -> 0x1413eb8b0 + 0x1413ec360 (subtype 0: the vt+0x98 slots pass 0,
// so 0x1413ec360 takes its final branch)
std::shared_ptr<Props> heatTransferExpressProps(const String& classKey, int guiUnit)
{
    auto p = std::make_shared<HeatTransferExpressProps>();   // base ctor 0x141401ee0 (SfOprExpressProperties)
    data::initCommonBlock(*p->common, guiUnit);       // 0x1405f8ca0 -> 0x1405fd250
    // dim: 3, or 2 when OB->vt[0x58]() & 1 (the 2D class)
    const int dim = qt::endsWith(classKey, qt::L("2D")) ? 2 : 3;
    const StepDefData d;                              // 0x1413eb740: new 0x38, 0x141359a60
    p->step = std::make_shared<data::HeatTransferStepData>();   // the same object at +0x60 (MDT L3)
    const Templates t = makeTemplates(dim, d);        // 0x1413eb8b0
    // 0x1413ec360
    add(p->tree, t.root);                             // 0x140375ba0(props+0x18 tree, template 0x1446fa720)
    base::addHeatingTime(*p, &p->tree.back(), qt::L("3600.0 sec") /*0x142e6e600*/,
                         qt::L("Heating Time") /*0x142e6e5f0*/);            // 0x1405fa2c0 under the root
    base::addEnvTemp(*p, guiUnit, qt::L("2250 F") /*0x142e6e614*/, qt::L("1200 C") /*0x142e6e60c*/);  // 0x1405fa560
    base::addConvection(*p, guiUnit, qt::L("7.7e-06 Btu/sec/in^2/F") /*0x142e6e638*/,
                        qt::L("0.0226611 N/sec/mm/C") /*0x142e6e620*/);    // 0x1405faaa0
    // vt+0x60 = 0x1413ed900: copies NumSteps / EnvTemp / Convection into the engine model's SimCtrl
    // (0x1405fb980, 0x1405fbd10, 0x1405fc060) and calls vt+0x70 with it; no tree change, and the
    // engine model is not preloaded in the converter (plan 8, engine model). Classes without an
    // override keep the props base slot 0x1405fb610 (returns 0).
    PropNode& sd = add(p->tree, t.stepDef);           // 0x140375ba0, kept in data+0x28
    add(sd.children, t.stepdefEnum);                  // 0x140375a70 (template 0x1446fa738)
    add(sd.children, t.dtmax);                        // 0x1446fa740 (props+0x40)
    add(sd.children, t.dtempmax);                     // 0x1446fa750 (props+0x48)
    add(sd.children, t.dtemplmt0);                    // 0x1446fa758 (props+0x50)
    add(sd.children, t.dtemplmt1);                    // 0x1446fa760 (props+0x58)
    add(sd.children, t.stpinc);                       // 0x1446fa748
    // 0x1413eb740 then calls vt+0x158, +0x148, +0x140 and connects two signals (enable/visibility of
    // the step nodes in the browser; not followed)
    return p;
}

}  // namespace m2m::tree
