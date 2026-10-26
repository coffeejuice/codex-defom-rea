// m2m/data/heattransfer.hpp: data payloads of SfOprHeatTransferProperties and
// SfOprHTFurnaceProperties (packet MDT L2). Member defaults are the binary's data ctors; the load
// slots (src/engine/loader_l2.cpp) fill them from the engine model's sim control.
#pragma once

#include <vector>

#include "m2m/qt4.hpp"

namespace m2m::data {

// SfOprHeatTransferProperties+0x58 (new, ctor 0x141359a60, owner P01: htexpress_props.cpp keeps
// the same defaults in its StepDefData). The tree nodes of 0x1413561c0 take
// their defaults from it (STEPDEF, STPINC, DTMAX, DTEMPMAX, DTEMPLMT0, DTEMPLMT1).
struct HeatTransferStepData {
    int stepDef = 1;               // +0x00 STEPDEF enum index (SC STPDEF - 1 after a load)
    int stpInc = 10;               // +0x04 STPINC
    double dtMax = 0.001;          // +0x08 DTMAX (0x1421b5050)
    double dTempMax = 2.0;         // +0x10 DTEMPMAX (0x142101520)
    double dTempLmt0 = 0.001;      // +0x18 DTEMPLMT0 (= +0x08)
    double dTempLmt1 = 10.0;       // +0x20 DTEMPLMT1 (0x142142078)
    // +0x28 (0x14015d170: null pointer) and +0x30 (0x14018f9e0) are not read on the load path.
};

// SfOprHTFurnaceProperties+0xd0 (new(0x40), ctor 0x140c6a190)
struct HTFurnaceData {
    qt::String name = qt::String(u"HT Furnace");   // +0x00 FURNAM (0x1428caf08)
    int i08 = 0;                   // +0x08
    bool viewFactor = false;       // +0x0c BVIEWFACTON
    int stepDef = 1;               // +0x10 STEPDEF enum index
    int stpInc = 10;               // +0x14 STPINC
    double dtMax = 0.001;          // +0x18 DTMAX (0x1421b5050)
    double dTempMax = 2.0;         // +0x20 DTEMPMAX (0x142101520)
    double dTempLmt0 = 0.001;      // +0x28 DTEMPLMT0 (= +0x18)
    double dTempLmt1 = 10.0;       // +0x30 DTEMPLMT1 (0x142142078)
    // +0x38: null pointer (0x14015d170), not read on the load path
};

}  // namespace m2m::data

namespace m2m::data {

// SfOprPreProperties+0x38 (new(0x48), ctor 0x14134c570): the fields behind its test property tree
struct PreData {
    double dwell = 5.0;                     // +0x00 Dwell (0x1421d3c60)
    double double6 = 0.12345678;            // +0x08 Double6 (0x142dd57c8)
    int numHeats = 2;                       // +0x10 NumHeats
    int readOnlyTest = 5;                   // +0x14 ReadOnlyTest
    bool boolTest = false;                  // +0x18 BoolTest
    std::vector<int> listTest{5, 10, 15, 20};   // +0x20 ListTest (QList<int>)
    int timsEnum = 1;                       // +0x28 TimsEnum
    qt::String length = qt::String(u"50.00 mm");    // +0x30 Length (0x142dd57b8)
    qt::String time = qt::String(u"5.00 sec");      // +0x38 Time (0x142dd57a8)
    qt::String angVel = qt::String(u"20.00 RPM");   // +0x40 AngVel (0x142dd5798)
};

}  // namespace m2m::data

namespace m2m::data {

// SfOprDoeOutputProperties+0x38 (new(8), ctor 0x140e363e0)
struct DoeOutputData {
    int selecedOpr = 1;                     // +0 SelecedOpr (sic)
    int selectedObj = 1;                    // +4 SelectedObj
};

// SfOprShapeOptimProperties+0x38 (new(0x18), ctor 0x140142d90)
struct ShapeOptimData {
    int maxSims = 200;                      // +0x08 MaxSims (+0: a null pointer, 0x14015d170)
    double minObjFuncVal = 0.001;           // +0x10 MinObjFuncVal (0x1421b5050)
};

}  // namespace m2m::data
