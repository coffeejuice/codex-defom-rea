// m2m/data/common.hpp: the common properties block of SfOprBaseProperties (PROPS+0x30).
// Layout from the readers 0x1405fb720 / 0x1405fb980 / 0x1405fbd10 / 0x1405fc060 / 0x1405fc5d0 /
// 0x1405fc890 / 0x1405fc8e0 (s1-05 5.5) and the base slots; names are the slot names. Owner P15:
// the adders that set the defaults and the set* slots are in m2m/props_base.hpp.
#pragma once

#include "m2m/qt4.hpp"

namespace m2m::data {
using qt::String;

// Member defaults = the ctor 0x1405fd250 with a GUI unit other than 2 (SI); English: initCommonBlock.
// +0x48 "Operation Heating Time" label and the +0x50/+0x58/+0x60 node pointers are UI/tree state.
struct CommonBlock {
    int numSteps = 100;             // +0x00 NumSteps -> SimCtrl+0x16c (0x1405fb980)
    int stepIncr = 10;              // +0x04 StepIncr -> SimCtrl+0x164
    int dtMode = 0;                // +0x08 StepBy -> SimCtrl+0x240: 0 DSMAX, else DTMAX (0x1405fc5d0)
    int frictionType = 0;          // +0x0c FrictionType
    double friction = 0.0;         // +0x10 FrictionCoef: FRCFAC value (0x1405fc890, no unit conversion)
    String heatingTime = String(u"15.0 sec");   // +0x18 HeatingTime (base slot 0x1405fd180)
    String envTemp = String(u"20 C");           // +0x20 EnvTemp, category 9 -> SimCtrl+0x3d0 (0x1405fbd10)
    String ihtc = String(u"11.0 N/sec/mm/C");   // +0x28 HeatTransCoef, category 0x20: IHTCOF (0x1405fc8e0)
    String convection = String(u"0.02 N/sec/mm/C");   // +0x30 ConvectionCoef, category 0x20 -> SimCtrl+0x4c8 (0x1405fc060)
    String dsmax = String(u"0.0 mm");          // +0x38 DistPerStep, category 1 -> SimCtrl+0x1e0 (mode 0)
    String dtmax = String(u"0.0 sec");         // +0x40 TimePerStep, category 0 -> SimCtrl+0x1d0
};

// @src 0x1405fd250  ctor defaults for a GUI unit (2 = English: "68 F", "0.004 Btu/sec/in^2/F",
// "7.7e-06 Btu/sec/in^2/F"); src/ops/props_base.cpp
void initCommonBlock(CommonBlock& b, int unit);

}  // namespace m2m::data
