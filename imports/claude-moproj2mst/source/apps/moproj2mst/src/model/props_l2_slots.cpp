// L2: the qt_static_metacall set and get slots of SfOprHeatTransferProperties,
// SfOprHeatTransferExpressProperties, SfOprHTFurnaceProperties and SfOprPreProperties, which the
// .moproj reader (applyValue, 0x140370600) and writer (getValue, 0x140370200) reach through
// QMetaObject::invokeMethod("set<Name>(SfProperty*,QVariant)" / "get<Name>(SfProperty*)").
// Method names: the moc string tables (file offsets 0x2e145a0, 0x2e79600, 0x28d0b80, 0x2dd8580);
// slot bodies: gui_mo/s1/moproj_get/d1..d5.c. A name the class does not have goes to the base
// (SfOprBaseProperties 0x1405f84b0); SfOprExpressProperties (0x1414031c0) has no slots of its own.
// Dropped UI: the STEPDEF slots also set the enabled flag of the step nodes (0x140370e90, a
// QtProperty state the .moproj does not hold) and emit messenger signal 0x1402cea00.
#include <algorithm>
#include <cmath>

#include "m2m/data/heattransfer.hpp"
#include "m2m/props.hpp"
#include "m2m/props_variant.hpp"

namespace m2m {

namespace {

using qt::Variant;
using pv::vBool;
using pv::vDouble;
using pv::vInt;
using pv::vText;
const auto vToInt = pv::toInt;
const auto vToDouble = pv::toDouble;
const auto vToBool = pv::toBool;
const auto vToString = pv::toString;

// The six step-definition slots shared by HeatTransfer (data at +0x58) and HeatTransferExpress
// (+0x60), same bodies at different offsets
bool stepSet(data::HeatTransferStepData& d, const std::u16string& n, const Variant& v)
{
    if (n == u"STEPDEF") d.stepDef = vToInt(v);                       // toInt, then enabled flags (UI)
    else if (n == u"DTMAX") d.dtMax = vToDouble(v);
    else if (n == u"STPINC") d.stpInc = static_cast<int>(vToDouble(v));   // toDouble, truncated
    else if (n == u"DTEMPMAX") d.dTempMax = vToDouble(v);
    else if (n == u"DTEMPLMT0") d.dTempLmt0 = vToDouble(v);
    else if (n == u"DTEMPLMT1") d.dTempLmt1 = vToDouble(v);
    else return false;
    return true;
}

bool stepGet(const data::HeatTransferStepData& d, const std::u16string& n, Variant& out)
{
    if (n == u"STEPDEF") out = vInt(d.stepDef);                       // enabled flags first (UI)
    else if (n == u"DTMAX") out = vDouble(d.dtMax);
    else if (n == u"STPINC") out = vInt(d.stpInc);
    else if (n == u"DTEMPMAX") out = vDouble(d.dTempMax);
    else if (n == u"DTEMPLMT0") out = vDouble(d.dTempLmt0);
    else if (n == u"DTEMPLMT1") out = vDouble(d.dTempLmt1);
    else return false;
    return true;
}

[[noreturn]] void noData(const char* cls)
{
    throw Error(M2M_E_UNSUPPORTED, std::string("properties: ") + cls + " without its data block");
}

}  // namespace

// ---- SfOprHeatTransferProperties: qt_static_metacall 0x14135a240 ---------------------------------
// @src 0x1413593f0 setSTEPDEF, 0x141355be0 setDTMAX, 0x141355c80 setSTPINC, 0x141355d20 setDTEMPMAX,
// 0x141355dc0 setDTEMPLMT0, 0x141355e60 setDTEMPLMT1 (cases 2, 4 .. 0xc; 0, 1 slotMeshGenerated)
void HeatTransferProps::applyValue(PropNode& node, const std::vector<PropNode*>& ancestors, const Variant& v)
{
    if (!step) noData("SfOprHeatTransferProperties");
    if (!stepSet(*step, node.name.u16(), v)) Props::applyValue(node, ancestors, v);
}

// @src 0x141359500 getSTEPDEF, 0x141355c30 getDTMAX, 0x141355cd0 getSTPINC, 0x141355d70 getDTEMPMAX,
// 0x141355e10 getDTEMPLMT0, 0x141355eb0 getDTEMPLMT1 (cases 3, 5 .. 0xd)
bool HeatTransferProps::getValue(const PropNode& node, const std::vector<PropNode*>&, Variant& out)
{
    if (!step) noData("SfOprHeatTransferProperties");
    if (stepGet(*step, node.name.u16(), out)) return true;
    return base::getValue(*this, node, out);
}

// ---- SfOprHeatTransferExpressProperties: qt_static_metacall 0x1413f5f40 --------------------------
// @src 0x1413edac0 setSTEPDEF, 0x1413eb2b0 setDTMAX, 0x1413eb350 setSTPINC, 0x1413eb3f0 setDTEMPMAX,
// 0x1413eb490 setDTEMPLMT0, 0x1413eb530 setDTEMPLMT1 (cases 3, 5 .. 0xd; 0 slotNumStepsChanged,
// 1, 2 slotMeshGenerated)
void HeatTransferExpressProps::applyValue(PropNode& node, const std::vector<PropNode*>& ancestors, const Variant& v)
{
    if (!step) noData("SfOprHeatTransferExpressProperties");
    if (!stepSet(*step, node.name.u16(), v)) ExpressProps::applyValue(node, ancestors, v);
}

// @src 0x1413edbd0 getSTEPDEF, 0x1413eb300 getDTMAX, 0x1413eb3a0 getSTPINC, 0x1413eb440 getDTEMPMAX,
// 0x1413eb4e0 getDTEMPLMT0, 0x1413eb580 getDTEMPLMT1 (cases 4, 6 .. 0xe)
bool HeatTransferExpressProps::getValue(const PropNode& node, const std::vector<PropNode*>&, Variant& out)
{
    if (!step) noData("SfOprHeatTransferExpressProperties");
    if (stepGet(*step, node.name.u16(), out)) return true;
    return base::getValue(*this, node, out);                          // 0x1414031c0 -> 0x1405f84b0
}

// ---- SfOprHTFurnaceProperties: qt_static_metacall 0x140c6b550 (D = +0xd0) ----------------------
// @src 0x140c673f0 setBVIEWFACTON, 0x140c69f20 setSTEPDEF, 0x140c674b0 setDTMAX, 0x140c67560
// setSTPINC, 0x140c67610 setDTEMPMAX, 0x140c676c0 setDTEMPLMT0, 0x140c67770 setDTEMPLMT1,
// 0x140c67320 setFURNAM, 0x140c69d30 setProcessType (cases 1, 3 .. 0x11; 0 slotNumStepsChanged)
void HTFurnaceProps::applyValue(PropNode& node, const std::vector<PropNode*>& ancestors, const Variant& v)
{
    if (!data) noData("SfOprHTFurnaceProperties");
    data::HTFurnaceData& d = *data;
    const std::u16string& n = node.name.u16();
    if (n == u"BVIEWFACTON") d.viewFactor = vToBool(v);               // D+0xc
    else if (n == u"STEPDEF") d.stepDef = vToInt(v);                  // D+0x10 (enabled flags: UI)
    else if (n == u"DTMAX") d.dtMax = vToDouble(v);                   // D+0x18
    else if (n == u"STPINC") d.stpInc = static_cast<int>(vToDouble(v));   // D+0x14
    else if (n == u"DTEMPMAX") d.dTempMax = vToDouble(v);             // D+0x20
    else if (n == u"DTEMPLMT0") d.dTempLmt0 = vToDouble(v);           // D+0x28
    else if (n == u"DTEMPLMT1") d.dTempLmt1 = vToDouble(v);           // D+0x30
    else if (n == u"FURNAM") d.name = vToString(v);                   // D+0
    else if (n == u"ProcessType") d.i08 = vToInt(v);                  // D+8 (signal 0x1402cea00: UI)
    else Props::applyValue(node, ancestors, v);
}

// @src 0x140c67450 getBVIEWFACTON, 0x140c6a040 getSTEPDEF, 0x140c67500 getDTMAX, 0x140c675c0
// getSTPINC, 0x140c67660 getDTEMPMAX, 0x140c67710 getDTEMPLMT0, 0x140c677c0 getDTEMPLMT1,
// 0x140c673a0 getFURNAM, 0x140c69d90 getProcessType (cases 2, 4 .. 0x12)
bool HTFurnaceProps::getValue(const PropNode& node, const std::vector<PropNode*>&, Variant& out)
{
    if (!data) noData("SfOprHTFurnaceProperties");
    const data::HTFurnaceData& d = *data;
    const std::u16string& n = node.name.u16();
    if (n == u"BVIEWFACTON") out = vBool(d.viewFactor);
    else if (n == u"STEPDEF") out = vInt(d.stepDef);
    else if (n == u"DTMAX") out = vDouble(d.dtMax);
    else if (n == u"STPINC") out = vInt(d.stpInc);
    else if (n == u"DTEMPMAX") out = vDouble(d.dTempMax);
    else if (n == u"DTEMPLMT0") out = vDouble(d.dTempLmt0);
    else if (n == u"DTEMPLMT1") out = vDouble(d.dTempLmt1);
    else if (n == u"FURNAM") out = vText(d.name);
    else if (n == u"ProcessType") out = vInt(d.i08);
    else return base::getValue(*this, node, out);
    return true;
}

// ---- SfOprPreProperties: qt_static_metacall 0x14134c910 (D = +0x38) ----------------------------
// @src 0x14134a250 setDwell, 0x14134a1b0 setNumHeats, 0x14134a2f0 setDouble6, 0x14134a390
// setBoolTest, 0x14134a430 setTimsEnum, 0x14134a4d0 setListTest, 0x14134a610 setLength, 0x14134a6e0
// setTime, 0x14134a7b0 setAngVel, 0x14134a110 setReadOnlyTest (even cases 0 .. 0x12; 0x14
// ButtonTestAction 0x14134c400)
void PreProps::applyValue(PropNode& node, const std::vector<PropNode*>& ancestors, const Variant& v)
{
    if (!data) noData("SfOprPreProperties");
    data::PreData& d = *data;
    const std::u16string& n = node.name.u16();
    if (n == u"Dwell") d.dwell = vToDouble(v);                        // D+0
    else if (n == u"NumHeats") d.numHeats = vToInt(v);                // D+0x10
    else if (n == u"Double6") d.double6 = vToDouble(v);               // D+8
    else if (n == u"BoolTest") d.boolTest = vToBool(v);               // D+0x18
    else if (n == u"TimsEnum") d.timsEnum = vToInt(v);                // D+0x28
    else if (n == u"ListTest") {                                      // D+0x20[node index] (0x140370fd0)
        if (node.index >= 0 && node.index < static_cast<int>(d.listTest.size()))
            d.listTest[static_cast<std::size_t>(node.index)] = vToInt(v);
    } else if (n == u"Length") d.length = vToString(v);               // D+0x30
    else if (n == u"Time") d.time = vToString(v);                     // D+0x38
    else if (n == u"AngVel") d.angVel = vToString(v);                 // D+0x40
    else if (n == u"ReadOnlyTest") d.readOnlyTest = vToInt(v);        // D+0x14
    else Props::applyValue(node, ancestors, v);
}

// @src 0x14134a2a0 getDwell, 0x14134a200 getNumHeats, 0x14134a340 getDouble6, 0x14134a3e0
// getBoolTest, 0x14134a480 getTimsEnum, 0x14134a560 getListTest, 0x14134a690 getLength, 0x14134a760
// getTime, 0x14134a830 getAngVel, 0x14134a160 getReadOnlyTest (odd cases 1 .. 0x13)
bool PreProps::getValue(const PropNode& node, const std::vector<PropNode*>&, Variant& out)
{
    if (!data) noData("SfOprPreProperties");
    const data::PreData& d = *data;
    const std::u16string& n = node.name.u16();
    if (n == u"Dwell") out = vDouble(d.dwell);
    else if (n == u"NumHeats") out = vInt(d.numHeats);
    else if (n == u"Double6") out = vDouble(d.double6);
    else if (n == u"BoolTest") out = vBool(d.boolTest);
    else if (n == u"TimsEnum") out = vInt(d.timsEnum);
    else if (n == u"ListTest")
        out = vInt(node.index >= 0 && node.index < static_cast<int>(d.listTest.size())
                       ? d.listTest[static_cast<std::size_t>(node.index)]
                       : 0);
    else if (n == u"Length") out = vText(d.length);
    else if (n == u"Time") out = vText(d.time);
    else if (n == u"AngVel") out = vText(d.angVel);
    else if (n == u"ReadOnlyTest") out = vInt(d.readOnlyTest);
    else return base::getValue(*this, node, out);
    return true;
}

}  // namespace m2m
