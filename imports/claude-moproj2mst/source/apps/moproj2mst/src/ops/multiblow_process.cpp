// SfMultiBlowProcess: the "Process" editor of a MultiBlow operation's object tree (vt+0xa8 adds it,
// the GUI shows it once the operation is opened). At Save, OB vt+0xd0 0x1405f1e20 finds the
// property stack index != 0 and sends signalUpdateDataInWidgetArea to it: its vt+0x1c0 0x1406370a0
// writes the form back into the MultiBlow data (props+0x38), then 0x140054350 reloads the props
// (MDT L4, docs/re/moproj2mst/mdt/L4/spec.md section 7). Decompiles gui_mo/s1/mdt/L4/proc.c,
// fill.c, acc.c, ui.c (setupUi 0x140638960: the four group boxes are checkable; ctor 0x140636c50
// gives the two radio buttons ids 0 and 1).
//
// The form's widgets hold text: the fill (vt+0x1b8 0x140636f10, when the editor is shown after the
// open) writes QString::number(v, 'g', 6), and the apply reads QString::toDouble back. Nothing
// changes the MultiBlow data between that open and the Save in a conversion, so both run here, in
// the original's order. The unit labels the fill sets (by SC+0xfc) are UI.
#include "m2m/data/multiblow.hpp"
#include "m2m/model.hpp"
#include "m2m/props.hpp"
#include "m2m/qt4.hpp"

namespace m2m {

namespace {

using data::MultiBlowData;

String num6(double v)
{
    return qt::number(v, 'g', 6);           // QString::number(v, 'g', 6)
}

int projectUnit(const Operation* op)        // 0x1405f8dc0: props' operation -> 0x140054ff0
{
    return op && op->project ? op->project->unit : 0;
}

// The form's line edits and check states (ui+0x28 .. +0x1d0), filled from the data
struct ProcessForm {
    String name;                            // ui+0x48
    String energy;                          // ui+0x28
    bool dwell = false;                     // ui+0x88 (group box, checkable)
    String dwellTime, dwellStep, dwellHeat; // ui+0xa0, +0xb0, +0xc0
    bool heating = false;                   // ui+0xf8 (group box, checkable)
    String heatTemp, heatTime, heatStep, transferIn, transferOut, transferStep;   // +0x110 .. +0x178
    bool adaptive = false;                  // ui+0x180 (group box, checkable)
    String trigger, triggerUpper, stop;     // ui+0x1b8, +0x1a8, +0x1c8
    int checkedId = -1;                     // ui+0x1d0 button group (ids 0, 1 from the ctor)
    bool rotation = false;                  // ui+0xe0
    bool initTempBox = false;               // ui+0x118 (group box, checkable): !InitTemp
    bool strain = false;                    // ui+0xd8
};

// @src 0x140636f10  fill (vt+0x1b8): 0x140637100, 0x140637270, 0x1406375a0, 0x140637bd0,
// 0x140637f00, 0x140637ff0
ProcessForm fill(const MultiBlowData& d, const Operation* op)
{
    ProcessForm f;
    // @src 0x140637100  0x14060b980 (+0x10), 0x14060ba10 (+0x40 when !+9, else +0x20)
    f.name = d.equipName;
    f.energy = num6(d.hammer == 0 ? d.screwEnergy : d.hammerEnergy);
    // @src 0x140637270  0x14060c300, 0x14060c350, 0x14060c3a0, 0x14060c4e0 (the unit strings
    // evaluated by 0x1402ede00 in the project unit system: category 0 and 0x20)
    f.dwell = d.dwell != 0;
    f.dwellTime = num6(d.defaultDwellTime);
    bool ok = false;
    f.dwellStep = num6(evalUnitString(d.dwellTimeStep, 0, projectUnit(op), &ok));
    f.dwellHeat = num6(evalUnitString(d.dwellHeatCoeff, 0x20, projectUnit(op), &ok));
    // @src 0x1406375a0  0x14060bef0 .. 0x14060c0f0
    f.heating = d.heating != 0;
    f.heatTemp = num6(d.heatingTemperature);
    f.heatTime = num6(d.heatingTime);
    f.heatStep = num6(d.heatingTimeStep);
    f.transferIn = num6(d.transferInTime);
    f.transferOut = num6(d.transferOutTime);
    f.transferStep = num6(d.transferTimeStep);
    // @src 0x140637bd0  0x14060c140 .. 0x14060c230
    f.adaptive = d.adaptiveReheat != 0;
    f.trigger = num6(d.triggerHeatTemp);
    f.triggerUpper = num6(d.triggerHeatTempUpper);
    f.stop = num6(d.heatStopTemp);
    // @src 0x140637f00  button(1) when +0x88, else button(0), setChecked(true)
    f.checkedId = d.relativeEnergy != 0 ? 1 : 0;
    // @src 0x140637ff0
    f.rotation = d.workpieceRotation != 0;
    f.initTempBox = d.initTemp == 0;
    f.strain = d.initStrain != 0;
    return f;
}

// @src 0x1406370a0  apply (vt+0x1c0, signalUpdateDataInWidgetArea): the six sections in order
void apply(MultiBlowData& d, const ProcessForm& f)
{
    // @src 0x1406371c0  0x14060b9d0, 0x14060ba70 (writes +0x20 and +0x40)
    d.equipName = f.name;
    const double e = qt::toDouble(f.energy);
    d.hammerEnergy = e;
    d.screwEnergy = e;
    // @src 0x140637410  0x14060c320, 0x14060c370, 0x14060c460, 0x14060c5a0
    d.dwell = f.dwell ? 1 : 0;
    d.defaultDwellTime = qt::toDouble(f.dwellTime);
    d.dwellTimeStep = num6(qt::toDouble(f.dwellStep));     // @src 0x14060c460  +0x120
    d.dwellHeatCoeff = num6(qt::toDouble(f.dwellHeat));    // @src 0x14060c5a0  +0x128
    // @src 0x1406378d0  0x14060bf10 .. 0x14060c110
    d.heating = f.heating ? 1 : 0;
    d.heatingTemperature = qt::toDouble(f.heatTemp);
    d.heatingTime = qt::toDouble(f.heatTime);
    d.heatingTimeStep = qt::toDouble(f.heatStep);
    d.transferInTime = qt::toDouble(f.transferIn);
    d.transferOutTime = qt::toDouble(f.transferOut);
    d.transferTimeStep = qt::toDouble(f.transferStep);
    // @src 0x140637d70  0x14060c160 .. 0x14060c250
    d.adaptiveReheat = f.adaptive ? 1 : 0;
    d.triggerHeatTemp = qt::toDouble(f.trigger);
    d.triggerHeatTempUpper = qt::toDouble(f.triggerUpper);
    d.heatStopTemp = qt::toDouble(f.stop);
    // @src 0x140637f90  checkedId() == 1 -> +0x88
    d.relativeEnergy = f.checkedId == 1 ? 1 : 0;
    // @src 0x1406380a0  +0x89, +0x8a = !group box, +0x8b
    d.workpieceRotation = f.rotation ? 1 : 0;
    d.initTemp = f.initTempBox ? 0 : 1;
    d.initStrain = f.strain ? 1 : 0;
}

}  // namespace

// The Process editor's fill then apply on the operation's MultiBlow data; false without one
bool multiBlowProcessUpdate(MultiBlowProps& pr, const Operation& op)
{
    if (!pr.data) return false;             // the ctor's dynamic_cast found no MultiBlow props
    apply(*pr.data, fill(*pr.data, &op));
    return true;
}

}  // namespace m2m
