// MDT L4: SfCoggingProcess, the "Process" editor of a Cogging operation's object tree (vt+0xa8
// 0x140750f20 adds it first; the GUI shows it once the operation is opened). At Save, OB vt+0xd0
// 0x1405f1e20 sends signalUpdateDataInWidgetArea, which the widget area's slotUpdateData
// 0x140dae340 turns into the shown editor's vt+0x1c0: here 0x140789410, the form written back into
// the cogging model (props+0x40) and the engine objects (docs/re/moproj2mst/mdt/L4/spec.md
// section 8; decompiles gui_mo/s1/mdt/L4/cog.c, cacc.c, cacc2.c, cacc3.c).
//
// The form holds text: the fill (vt+0x1b8 0x140788cc0, when the editor is shown after the open)
// writes QString::number(v, 'g', 6) and the apply reads QString::toDouble back; the radio groups'
// buttons carry the values as ids. Nothing changes the model between that open and the Save of a
// conversion, so both run here, in the original's order. Labels and button texts are UI.
#include <cstdint>
#include <string>

#include <defmodel/layout/cls_1401e7b10.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/llobj_data.hpp>

#include "engine_internal.hpp"
#include "m2m/data/cogging.hpp"
#include "m2m/engine.hpp"
#include "m2m/engine_cogging_process.hpp"
#include "m2m/props.hpp"
#include "m2m/qt4.hpp"

namespace m2m::engine {

using def::model::LLOBJ_DATA;

namespace {

[[noreturn]] void unsupported(const std::string& what)
{
    throw Error(M2M_E_UNSUPPORTED, "save: " + what);
}

String num6(double v)
{
    return qt::number(v, 'g', 6);           // QString::number(v, 'g', 6)
}

// 0x1401b2470  primitive parameter k (0 <= k < 20: LLOBJ_DATA +0x3ab0 + 8k)
void primParam(LLOBJ_DATA* o, int k, double v)
{
    if (k >= 0 && k < 20) *at<double>(o, 0x3ab0 + 8 * static_cast<std::size_t>(k)) = v;
}

// The form (ui = widget+0x48): radio groups by button id, check states, line-edit texts
struct ProcessForm {
    int process = 0;                        // group ui+0x1a0 (id = Process_Type)
    String swaging;                         // ui+0x38 (filled only for process 1)
    int temperature = 0;                    // group ui+0x1a8 (id = Temperaure_Type)
    bool heatTransfer = false;              // group box ui+0x148
    String cycle, decompression, dwell;     // ui+0x160, +0x178, +0x188
    bool remainingDwell = false;            // radio ui+0x198 (ui+0x180 is its partner)
    int dies = 0;                           // group ui+0x1b8: 0 = 2 dies, 1 = 4 dies
    bool differentDies = false;             // ui+0xa0
    bool rotSymmetry = false;               // ui+0xb8
    bool mandrel = false;                   // ui+0xb0
    bool manip = false;                     // ui+0xa8
    bool adaptive = false;                  // group box ui+0xc0
    int checking = 0;                       // group ui+0x1b0 (id = Checking_Method)
    String range0, range1, heatTime;        // ui+0xf0, +0x138, +0x128
};

// @src 0x140788cc0  fill (vt+0x1b8)
ProcessForm fill(const data::CogModel& M)
{
    ProcessForm f;
    f.process = M.processType;                          // 0x1413ee240, button(id)->setChecked
    if (f.process == 1) f.swaging = num6(M.swagingStroke);   // 0x14075dd30
    // ui+0x40 label: unit name 1 of the project (0x140917de0), UI
    f.temperature = M.temperatureType;                  // 0x14142ff20
    f.heatTransfer = M.heatTransfer != 0;               // 0x14075dc20
    f.cycle = num6(M.cycleTime);                        // 0x14075dd10
    f.decompression = num6(M.decompressionTime);        // 0x14075dc80
    f.dwell = num6(M.dwellTime);                        // 0x14075dc60
    f.remainingDwell = M.remainingCycleTimeDwell != 0;  // 0x14075dc40: ui+0x198 checked, else ui+0x180
    // 0x140765b00: 4 dies when the die block's flag +0x14a8 is 1 (id 1, "4 Dies" text), else 2 (id 0)
    f.dies = M.cog.die.four == 1 ? 1 : 0;
    f.differentDies = (M.useDifferentDies & 0xff) != 0; // 0x14075dc10 (byte +0x14); its text is UI
    f.rotSymmetry = M.rotationalSymmetry != 0;          // 0x14075dca0 (+0x15)
    f.mandrel = M.useMandrel != 0;                      // 0x14075dc00 (+0x08)
    f.manip = M.cog.useManip != 0;                      // 0x140765960 -> 0x14076b060 (cog+0xc8)
    // 0x140765a10: +0x4cc (returned), +0x4d0, +0x4d8, +0x4e0, +0x4e8
    f.adaptive = M.useAdaptiveReheat != 0;
    f.checking = M.checkingMethod;
    f.range0 = num6(M.tempRange[0]);
    f.range1 = num6(M.tempRange[1]);
    f.heatTime = num6(M.adaptiveHeatingTime);
    return f;
}

// @src 0x140765f30  die objects 2, 3, 8, 9 by index
int dieObject(int i)
{
    if (i == 0) return 2;
    if (i == 1) return 3;
    if (i == 2) return 8;
    if (i == 3) return 9;
    return 0;
}

// @src 0x140765fa0  manipulator objects 4 .. 7 by index
int manipObject(int i)
{
    if (i == 0) return 4;
    if (i == 1) return 5;
    if (i == 2) return 6;
    if (i == 3) return 7;
    return 0;
}

// @src 0x140766e80  RotationalSymmetry: +0x15, then on engine object 1 (if any) primitive parameter 4
// = the billet's inner diameter (dia / 2.0 when 0) when set, and parameter 6 = the flag
void setRotationalSymmetry(Project& p, Operation& op, data::CogModel& M, bool v)
{
    M.rotationalSymmetry = v ? 1 : 0;
    GuiModel* m = opModel(p, op);           // 0x1405f8d90(+0x548) -> 0x14005f770
    LLOBJ_DATA* o1 = m ? m->object(1) : nullptr;   // 0x140c7ae80
    if (!o1) return;
    data::CogBillet& b = M.cog.billet;      // 0x14075dcb0 -> 0x14044b5d0 (cog+0xd8)
    if (v) {
        if (b.inDia == 0.0) b.inDia = b.dia / 2.0;   // 0x1421ad1d0, 0x142101520
        primParam(o1, 4, b.inDia);
    }
    primParam(o1, 6, v ? 1.0 : 0.0);
}

// The apply's mesh deletion: engine object no loses its mesh (0x140873be0); the viewer sync
// 0x140c80280 is packet V
void deleteMesh(GuiModel& m, int no)
{
    LLOBJ_DATA* o = m.object(no);           // 0x140057710
    if (o && objHasMesh(o)) objDeleteMesh(o);   // 0x140834c80, 0x140873be0
}

// @src 0x140789410  apply (vt+0x1c0)
void apply(Project& p, Operation& op, data::CogModel& M, const ProcessForm& f)
{
    GuiModel* m = opModel(p, op);
    M.processType = f.process;                          // 0x140d33b00
    LLOBJ_DATA* o2 = m ? m->object(2) : nullptr;        // 0x140057710(op, 2)
    if (f.process == 1) {
        const double v = qt::toDouble(f.swaging);
        M.swagingStroke = v;                            // 0x140790610
        if (!o2) unsupported("Cogging Process editor: no engine object 2 (the original writes through null)");
        *at<double>(o2, 0x2478) = v;
    }
    // @src 0x140765b90  Temperaure_Type: +0x04, the owner's sim control TRANS 1 (+0x104) = (t != 0),
    // heat transfer (+0x504) = (t != 0)
    M.temperatureType = f.temperature;
    if (def::model::Cls_1401e7b10* sc = m ? m->simCtl() : nullptr) sc->trans[0] = f.temperature != 0 ? 1 : 0;
    M.heatTransfer = f.temperature != 0 ? 1 : 0;        // 0x14075dd90
    M.heatTransfer = f.heatTransfer ? 1 : 0;            // 0x14075dd90 (group box)
    M.cycleTime = qt::toDouble(f.cycle);                // 0x140790650
    M.decompressionTime = qt::toDouble(f.decompression);   // 0x140790690
    M.dwellTime = qt::toDouble(f.dwell);                // 0x140790670
    M.remainingCycleTimeDwell = f.remainingDwell ? 1 : 0;  // 0x140790630
    if (f.heatTransfer) {
        if (!o2) unsupported("Cogging Process editor: no engine object 2 (the original writes through null)");
        *at<double>(o2, 0x2480) = 1.0 / qt::toDouble(f.cycle);   // 0x1421249e0
    }
    // @src 0x140765a90  number of dies: +0x10, and the die block's flag +0x14a8 = (n == 4)
    const int n = f.dies == 1 ? 4 : 2;
    M.f10 = n;
    M.cog.die.four = n == 4 ? 1 : 0;
    M.useDifferentDies = (M.useDifferentDies & ~0xff) | (f.differentDies ? 1 : 0);   // 0x1407906b0 (byte +0x14)
    setRotationalSymmetry(p, op, M, f.rotSymmetry);     // 0x140766e80
    M.useMandrel = f.mandrel ? 1 : 0;                   // 0x1411c2b10 (byte +0x08)
    M.cog.useManip = f.manip ? 1 : 0;                   // 0x140765930 -> 0x14076b080
    // @src 0x140765990  +0x4cc, +0x4d0, +0x4d8, +0x4e0, +0x4e8
    M.useAdaptiveReheat = f.adaptive ? 1 : 0;
    M.checkingMethod = f.checking;
    M.tempRange[0] = qt::toDouble(f.range0);
    M.tempRange[1] = qt::toDouble(f.range1);
    M.adaptiveHeatingTime = qt::toDouble(f.heatTime);
    if (f.temperature < 2 && m) {
        for (int i = 0; i < n; ++i) deleteMesh(*m, dieObject(i));
        if (M.cog.useManip != 0)                        // 0x140765960
            for (int i = 0; i < 4; ++i) deleteMesh(*m, manipObject(i));
    }
}

}  // namespace

bool cogProcessUpdate(Project& p, Operation& op)
{
    // 0x140130af0: the editor's operation; props (0x140051230) as SfOprCoggingProperties, model +0x40
    auto* cp = dynamic_cast<CoggingProps*>(op.props.get());
    if (!cp || !cp->model) return true;     // the editor is shown; the original does nothing
    apply(p, op, *cp->model, fill(*cp->model));
    return true;
}

}  // namespace m2m::engine
