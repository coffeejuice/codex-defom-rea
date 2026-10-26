// Packet S: "save current operation" of DEF_GUI_MO_64's File > Save (step 2, before the .moproj
// write), docs/re/moproj2mst/s/spec.md. One function per binary function, in the original's call
// order. UI-only calls (messenger signals, mkpath failure dialog, the property widget) are dropped;
// each drop is named. Qualified native task KEY writers open their real targets
// before calling the ported block writers. Recording mode and remaining writer
// gaps retain scratch capture, explicitly distinguished in FileEffects.
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <cstdint>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iterator>
#include <memory>
#include <string_view>
#include <vector>

#include <defcmd/cmd_p8_01.hpp>
#include <defcmd/dispatch.hpp>
#include <defcmd/key_import.hpp>
#include <defkey/item_writers.hpp>
#include <defmodel/layout/cls_1401e7b10.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/llobj/object_list.hpp>
#include <defmodel/mesh2d/material_ops.hpp>
#include <defmodel/program_info.hpp>
#include <defmodel/window/inter_object_ops.hpp>

#include "../ops/express.hpp"
#include "../doe/output_regions_mkpath_prefix.hpp"
#include "m2m/data/heattransfer.hpp"
#include "m2m/datafiles.hpp"
#include "engine_internal.hpp"
#include "m2m/engine.hpp"
#include "m2m/engine/simctrl_ppv_save_file.hpp"
#include "m2m/engine/simctrl_ppv_text_device.hpp"
#include "m2m/mst/file_device.hpp"
#include "m2m/mst/writer.hpp"
#include "m2m/qt4path.hpp"
#include "m2m/native_directories.hpp"
#include "cogging_save.hpp"
#include "distortion_contacts.hpp"
#include "save_l6.hpp"
#include "../datafiles/cogging_file.hpp"

namespace m2m::engine {

using def::model::Cls_1401e7b10;
using qt::L;

namespace {

[[noreturn]] void notPorted(const Operation& op, const char* what)
{
    throw Error(M2M_E_UNSUPPORTED, "save current operation: " + op.classKey.utf8() + " " + what +
                                       " is not ported yet (docs/re/moproj2mst/s/spec.md)");
}

bool keyIn(const String& key, std::initializer_list<const char16_t*> keys)
{
    for (const char16_t* k : keys)
        if (key.u16() == k) return true;
    return false;
}

std::string readAll(const std::filesystem::path& f)
{
    std::ifstream in(f, std::ios::binary);
    return std::string(std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>());
}

// An engine writer's fopen(path) is given a scratch file; its bytes are recorded for `target`.
// A writer that does not create the file (failed open, empty name) records nothing.
void captureKey(const String& target, FileEffects& fx, const std::function<void(const char*)>& write)
{
    const std::filesystem::path tmp = std::filesystem::temp_directory_path() /
                                      ("m2m_s_" + std::to_string(GetCurrentProcessId()) + "_" +
                                       std::to_string(GetCurrentThreadId()) + ".KEY");
    std::error_code ec;
    std::filesystem::remove(tmp, ec);
    write(tmp.string().c_str());
    if (std::filesystem::exists(tmp, ec)) {
        fx.writes.push_back({target, readAll(tmp)});
        std::filesystem::remove(tmp, ec);
    }
}

template<class FileWriter, class StreamWriter>
int writeTaskKey(Project& p, const String& target, FileEffects& fx, int failedOpen,
                 FileWriter fileWriter, StreamWriter streamWriter)
{
    if (!fx.nativeTaskKeys) {
        int result = failedOpen;
        captureKey(target, fx, [&](const char* file) { result = fileWriter(file); });
        return result;
    }
    if (fx.beforeNativeKeyWrite) fx.beforeNativeKeyWrite(target);
    datafiles::detail::CoggingFile file(qt::encodeLocale(target, fx.nativePathCodepage));
    if (!file.get()) return failedOpen;
    p.overlay.erase(qt::cleanPath(target));
    const int result = streamWriter(file.get());
    file.close(); // each recovered wrapper ignores fclose, including failed flush
    return result;
}

// "%1%2.KEY".arg(stem).arg(slot + 1, 5, 10, '0') in the task folder (0x14005cce0 family)
String slotKey(const Operation& op, const char* stem, int slot)
{
    char buf[64];
    std::snprintf(buf, sizeof buf, "%s%05d.KEY", stem, slot + 1);
    return qt::cleanPath(taskDirAbs(op) + qt::kSep + L(buf));
}

// ---- properties -> engine model (props vt+0x60, spec section 3) -----------------------------------

// The SIM_CTRL fields the four base helpers (0x1405fb980, 0x1405fbd10, 0x1405fc060, 0x1405fc5d0)
// read and write, moved through P15's SimCtrlVals.
SimCtrlVals readVals(Cls_1401e7b10* sc)
{
    SimCtrlVals v;
    v.unit = sc->unit;
    v.i164 = sc->stpinc;
    v.i16c = sc->nstep;
    v.dtmax = *at<double>(sc, 0x1d0);
    v.dsmax = *at<double>(sc, 0x1e0);
    v.dtMode = *at<std::int32_t>(sc, 0x240);
    v.d3d0 = *at<double>(sc, 0x3d0);
    v.d4c8 = *at<double>(sc, 0x4c8);
    return v;
}

void writeVals(Cls_1401e7b10* sc, const SimCtrlVals& v)
{
    sc->stpinc = v.i164;
    sc->nstep = v.i16c;
    *at<double>(sc, 0x1d0) = v.dtmax;
    *at<double>(sc, 0x1e0) = v.dsmax;
    *at<std::int32_t>(sc, 0x240) = v.dtMode;
    *at<double>(sc, 0x3d0) = v.d3d0;
    *at<double>(sc, 0x4c8) = v.d4c8;
}

// @src 0x1400543a0  props vt+0x60 of the operation (0x140051230 null: the original crashes)
void propsToEngine(Project& p, Operation& op)
{
    if (!op.props) notPorted(op, "properties (none)");
    // classes whose properties have their own body (MDT L4: ShapeOptim; MDT L5S: Convert2Dto3D, CopyMirror)
    if (mst::OpClass* ob = opBase(op); ob && ob->propsToEngine(p, op)) return;
    const String& key = op.classKey;
    // 0x1405fb610 (returns 0): Base, Boolean, Cycle, DiestressBM, Diestress, Express properties;
    // 0x141352e80 (returns 1): Group
    if (keyIn(key, {u"SfOprBoolean2D", u"SfOprBoolean3D", u"SfOprCycle", u"SfOprDiestressBM2D",
                    u"SfOprDiestressBM3D", u"SfOprDiestress", u"SfOprGroup"}))
        return;
    enum Kind { F80_90_B0_A0, F80_90_A0, F80_90_B0, HT } kind;
    if (keyIn(key, {u"SfOprForming", u"SfOprForming2D", u"SfOprDistortion2D", u"SfOprDistortion3D",
                    u"SfOprFormingExpress", u"SfOprFormingExpress2D", u"SfOprMultiBlow2D",
                    u"SfOprMultiBlow3D"}))
        kind = F80_90_B0_A0;   // 0x140c09ed0, 0x141366650, 0x14142a6f0, 0x140612bf0
    else if (key.u16() == u"SfOprCogging")
        kind = F80_90_A0;      // 0x14075ee60
    else if (keyIn(key, {u"SfOprPre3D", u"SfOprPre2D", u"SfOprReport"}))
        kind = F80_90_B0;      // 0x14134c3c0, 0x1412b1f50
    else if (keyIn(key, {u"SfOprHeatTransfer", u"SfOprHeatTransfer2D", u"SfOprHeatTransferExpress",
                         u"SfOprHeatTransferExpress2D"}))
        kind = HT;             // 0x141357590, 0x1413ed900 (MDT L3S)
    else if (key.u16() == u"SfOprHTFurnace") {
        furnacePropsToEngine(p, op);   // 0x140c69520 (MDT L2, loader_l2.cpp)
        return;
    } else if (keyIn(key, {u"SfOprHeatTreat", u"SfOprHeatTreat2D"})) {
        heatTreatPropsToEngine(p, op); // 0x1411bfa20 (MDT L6S, save_l6.cpp)
        return;
    } else
        notPorted(op, "properties vt+0x60");   // HeatTransferExpress,
                                               // Convert2Dto3D, CopyMirror, DoeOutput, ShapeOptim
    GuiModel* m = opModel(p, op);             // 0x1405f8df0: no model -> the helpers do nothing
    Cls_1401e7b10* sc = m ? m->simCtl() : nullptr;
    if (!sc) return;
    SimCtrlVals v = readVals(sc);
    propsSimCtrl80(*op.props, v);             // 0x1405fb980
    propsSimCtrl90(*op.props, v);             // 0x1405fbd10
    if (kind == F80_90_B0_A0) {
        propsSimCtrlB0(*op.props, v);         // 0x1405fc5d0
        propsSimCtrlA0(*op.props, v);         // 0x1405fc060
    } else if (kind == F80_90_A0 || kind == HT) {
        propsSimCtrlA0(*op.props, v);
    } else {
        propsSimCtrlB0(*op.props, v);
    }
    writeVals(sc, v);
    if (kind == HT && dynamic_cast<HeatTransferExpressProps*>(op.props.get())) {
        // vt+0x70 = 0x1413ed710 with the owner's sim control: the step block +0x60 (MDT L3S)
        auto* hp = static_cast<HeatTransferExpressProps*>(op.props.get());
        if (!hp->step) notPorted(op, "HeatTransferExpress step data (props+0x60)");
        hteStepDataToSimCtl(*hp, sc);
    } else if (kind == HT) {
        // vt+0x70 = 0x1413573a0 with the owner's sim control (0x14005f7e0): the step block +0x58
        auto* hp = dynamic_cast<HeatTransferProps*>(op.props.get());
        if (!hp || !hp->step) notPorted(op, "HeatTransfer step data (props+0x58)");
        const data::HeatTransferStepData& d = *hp->step;
        sc->stpdef = d.stepDef + 1;                     // +0x1c8
        *at<double>(sc, 0x1d0) = d.dtMax;
        *at<double>(sc, 0x270) = d.dTempMax;
        *at<double>(sc, 0x2a0) = d.dTempLmt0;
        *at<double>(sc, 0x2a8) = d.dTempLmt1;
        sc->stpinc = d.stpInc;                          // +0x164
    }
}

}  // namespace (0x140054540 is also called by FormingExpress vt+0xd8, save_l3.cpp)

// ---- 0x140054540: operation and simulation names into the engine (ProcType 5) -----------------

// @src 0x140054540  OPRNAM, CURSIM[1], SIMNAM, CURSIM[0] from the sub-project's first sim entry of
// this operation (0x1400b8540 filters 0x1400b8360 by position)
void storeSimNames(Project& p, Operation& op)
{
    SubProject* sub = activeSubProject(p);                   // 0x140055050
    if (!sub) return;
    const String name = op.name;                             // 0x140050ac0
    const int no = flowNumber(op);                           // 0x140050b20
    const mst::SimEntry* first = nullptr;
    const std::vector<mst::SimEntry> all = mst::simEntries(p, *sub);
    for (const mst::SimEntry& e : all)
        if (e.pos == no) {                                   // 0x140cbc690 (+0x8)
            first = &e;
            break;
        }
    if (!first) return;
    if (!first->named) notPorted(op, "simulation entry name (vt+0x298 of this class)");
    GuiModel* m = opModel(p, op);
    [&](Cls_1401e7b10* sc) -> void {
        if (!sc) return;
        std::strncpy(sc->oprnam, local8(name).c_str(), 0x40);    // 0x141470020
        sc->cursim[1] = no;                                      // +0xe8
        std::strncpy(sc->simnam, local8(first->name).c_str(), 0x50);
        sc->cursim[0] = first->sim;                              // +0xe4 (0x140cbc680, +0xc)
    }(m ? m->simCtl() : nullptr);                              // 0x140c7b2d0
}

// @src 0x1405f22e0  SfOprBase vt+0xd8: ProcType (0x143d766b8+0xc) is 5 after opening a .moproj, so
// 0x140054540(opi) when the class has a current operation. SfOprDiestress vt+0xd8 0x1402b6220 is this
// body; SfOprDiestressBM 0x140299870 overrides saveNames and then runs it (MDT L6S).
void baseSaveNames(mst::OpClass& c, Project& p, Operation& op, FileEffects&)
{
    if (!c.current) return;                                  // 0x1405ef2b0
    storeSimNames(p, op);
}

// The helpers of 0x140057370 below are also used by the class save slots (engine_internal.hpp).
// ---- 0x140057370 helpers --------------------------------------------------------------------------

// @src 0x140051190  mkpath(task folder) and 0x14008c630(project): Operations, Objects, Materials,
// Equipments, Lubricants (+ DieStress of a type-4 sub-project). The "Can not make the directory"
// dialog (0x14081afb0) is UI.
void makeFolders(Project& p, Operation& op, FileEffects& fx)
{
    const auto attempt = [&](const String& target) {
        if (fx.nativeDirectories) {
            const bool success = qt::nativeMkpathAbsolute(target);
            fx.directoryAttempts.push_back({target, success});
            // 81afb0 shows a single-OK dialog on failure and returns true.
            // Its nested event dispatch remains in the session timing queue.
        } else fx.mkdirs.push_back(target);
    };
    attempt(taskDirAbs(op));
    const String dir = projectDir(p);
    for (const char16_t* d : {u"Operations", u"Objects", u"Materials", u"Equipments", u"Lubricants"})
        attempt(qt::cleanPath(dir + qt::kSep + String(d)));   // 0x1400a43e0 .. 0x1400a48a0
    if (SubProject* sub = activeSubProject(p); sub && sub->type == 4)    // 0x140010390
        attempt(qt::cleanPath(dir + qt::kSep + L("DieStress")));   // 0x1400bc160
}

// @src 0x1400597d0  SimCtrl.KEY (flags 0xf), then SimCtrl.PPV when the recovery list is not empty
bool saveSimCtrl(Project& p, Operation& op, FileEffects& fx)
{
    return withOpStep(p, op, [&](def::model::DB_STEP_DATA* step) -> bool {
        if (!step) return false;
        const String path = simCtrlPath(op);                     // 0x140059290
        std::int32_t flags = 0xf;
        int r = 1;
        if (fx.nativeSimCtrl) {
            datafiles::detail::CoggingFile file(qt::encodeLocale(path, fx.nativePathCodepage));
            if (file.get()) {
                // 14089597f: failed open skips model flags and every block writer.
                // The physical file was truncated, so an imported blob is now stale.
                p.overlay.erase(qt::cleanPath(path));
                r = def::key::write_simctrl_key_stream(*step, file.get(), &flags);
                file.close(); // original discards fclose result
            }
        } else {
            captureKey(path, fx, [&](const char* f) { r = def::key::write_simctrl_key_file(*step, f, &flags); });   // 0x140895920
        }
        if (op.recoveryB && !op.recoveryB->empty()) {            // 0x140307410 (isEmpty) in 0x140d4afe0
            if (!fx.nativeSimCtrl)
                throw Error(M2M_E_UNSUPPORTED, "save current operation: SimCtrl.PPV recording-mode ordering is unqualified");
            namespace ppv = simctrl_ppv;
            ppv::OperationSaveInput input;
            input.path = ppv::SavePathQualification::OperationTaskPath;
            input.codec = ppv::SaveCodecQualification::KnownAsciiCompatible;
            input.localeCodepage = fx.nativePathCodepage;
            input.open = ppv::SaveOpenOutcome::Unknown;
            const auto beforeOpen = ppv::writePpvForOperation(op, input);
            if (beforeOpen.status != ppv::OperationSaveStatus::OpenOutcomeUnknown)
                throw Error(M2M_E_UNSUPPORTED, "save current operation: SimCtrl.PPV owner, path or codec is unqualified");
            if (fx.beforeNativeKeyWrite) fx.beforeNativeKeyWrite(beforeOpen.path);
            mst::FileDevice ppvFile(beforeOpen.path);            // QFile WriteOnly|Text, before formatting
            input.open = ppvFile.opened() ? ppv::SaveOpenOutcome::Succeeded : ppv::SaveOpenOutcome::Failed;
            const auto payload = ppv::writePpvForOperation(op, input);
            if (payload.status == ppv::OperationSaveStatus::PayloadReady) {
                std::vector<std::string> lines;
                std::size_t start = 0;
                while (start < payload.bytes.size()) {
                    const auto end = payload.bytes.find("\r\n", start);
                    if (end == std::string::npos) {
                        lines.push_back(payload.bytes.substr(start));
                        break;
                    }
                    lines.push_back(payload.bytes.substr(start, end - start));
                    start = end + 2;
                }
                (void)ppv::writePpvTextSegments(ppvFile, input.localeCodepage, lines);
            }
            // Native caller ignores both failed PPV open and post-open stream status.
        }
        return r == 0;
    });
}

// @src 0x1400655d0  INICTC flags of the contact block from the engine's objects or pairs
void contactFlagsFromEngine(Operation& op, GuiModel& m)
{
    ContactBlock& icb = op.icb;
    icb.flag.fill(0);                                        // 0x140065730(icb, 0)
    [&](def::model::DB_STEP_DATA* step) {
        const std::int32_t pairs = *at<std::int32_t>(&step->inter_obj, 0x10);   // 0x1407d3070
        if (pairs == 0) {
            const int n = m.objectCount();                       // 0x140830180
            for (int i = 0; i < n; ++i) {
                def::model::LLOBJ_DATA* o = m.objectAt(i);       // 0x1408302a0(step, i)
                if (*at<std::int32_t>(o, 0xc) != 7)              // 0x140834fa0
                    icb.flag[static_cast<std::size_t>(*at<std::int32_t>(o, 8) - 1)] = 1;   // 0x140065270
            }
        } else if (icb.enable) {                                 // 0x140065770 (+0x64)
            for (std::int32_t i = 0; i < pairs; ++i) {
                auto* pr = def::model::pair_nth(&step->inter_obj, i + 1);   // 0x1408e6b70
                if (!pr) continue;
                const std::int32_t kind = *at<std::int32_t>(pr, 0x170);
                if (kind == 2) icb.flag[static_cast<std::size_t>(*at<std::int32_t>(pr, 0x16c) - 1)] = 1;
                else if (kind == 1) icb.flag[static_cast<std::size_t>(*at<std::int32_t>(pr, 0x168) - 1)] = 1;
            }
        }
    }(m.step.get());
}

// ---- 0x14005a9b0: system-lubricant contact rebuild (MDT L3S, spec section 4) ---------------------

// @src 0x1405f60c0  SfOprBase vt+0x1a0: IHTC of a default pair by the project unit and the
// operation's sub type (the friction stays as it is)
void baseContactDefaults(Operation& op, double*, double* ihtc, bool flag)
{
    const int unit = op.project ? op.project->unit : 0;     // 0x140054ff0 (0x1400a2180)
    const String sub = op.subType;                          // 0x140050da0
    if (sub == L("Rest on die")) {                          // 0x1424978f8
        if (unit == 2) *ihtc = !flag ? 0.0003 : 0.0002;     // 0x142498298 / 0x1424982a0
        else *ihtc = !flag ? 1.0 : 0.5;                     // 0x1421249e0 / 0x142101640
    } else if (sub == L("Dwell on die")) {                  // 0x142497908
        if (unit == 2) *ihtc = !flag ? 0.0006 : 0.0003;     // 0x142498290 / 0x142498298
        else *ihtc = !flag ? 2.0 : 1.0;                     // 0x142101520 / 0x1421249e0
    } else {
        if (unit == 2) *ihtc = !flag ? 0.004 : 0.002;       // 0x142498288 / 0x142265008
        else *ihtc = !flag ? 11.0 : 5.0;                    // 0x142498280 / 0x1421d3c60
    }
}

// class vt+0x1a0: SfOprExpressBase and its subclasses 0x1413fd4c0, else the base 0x1405f60c0
void classContactDefaults(Project& p, Operation& op, double* frc, double* ihtc, bool flag)
{
    if (keyIn(op.classKey, {u"SfOprHeatTransferExpress", u"SfOprHeatTransferExpress2D", u"SfOprFormingExpress",
                            u"SfOprFormingExpress2D"}))
        expressContactDefaults(p, op, frc, ihtc, flag);
    else
        baseContactDefaults(op, frc, ihtc, flag);
}

// @src 0x1405f5c90  SfOprBase vt+0x198: clears the engine's inter-object list; when exactly one of
// the defined objects is a master candidate (kind not 0/3, or for a kind-5 object its slot
// record's +8 not 0/3) every other object gets a pair with it
std::vector<ContactPair> baseDefaultPairs(Project& p, Operation& op)
{
    std::vector<ContactPair> out;                            // 0x14018f9e0
    GuiModel* m = opModel(p, op);                            // 0x1405ef2e0
    [&](def::model::DB_STEP_DATA* s) -> void {
        if (!s) return;
        s->inter_obj.clear();                                    // 0x1408e6cc0 = 1401fade0
        const int n = m->objectCount();                          // 0x140830180
        int master = 0, cand = 0, defined = 0;
        for (int i = 0; i < n; ++i) {
            def::model::LLOBJ_DATA* o = m->objectAt(i + 1);      // 0x140c7aed0
            if (!o || *at<std::int32_t>(o, 0xc) == 7) continue;  // 0x140834fa0
            ++defined;
            const int kind = *at<std::int32_t>(o, 0xc);
            bool c = !(kind == 0 || kind == 3);                  // 0x140835030
            if (kind == 5) {
                if (ObjEntry* e = objEntryN(op, i + 1)) {         // 0x1400612d0
                    const int f = e->f08;                        // 0x14004d660 (+8)
                    c = !(f == 0 || f == 3);
                }
            }
            if (c) {
                master = *at<std::int32_t>(o, 8);
                ++cand;
            }
        }
        if (defined > 1 && cand == 1 && master > 0) {
            for (int i = 1; i <= n; ++i) {
                def::model::LLOBJ_DATA* o = m->objectAt(i);
                if (!o || *at<std::int32_t>(o, 0xc) == 7) continue;
                if (*at<std::int32_t>(s, 0x1c) == 3 && i == master) continue;   // 0x14035b400
                ContactPair c;                                   // 0x1400657a0
                c.master = i;
                c.slave = master;
                c.lub = 0;
                bool mesh = objHasMesh(o);                       // 0x140834c80
                if (*at<std::int32_t>(o, 0xc) == 5) {
                    const int code = resolvedCode(p, op, *at<std::int32_t>(o, 8) - 1);   // 0x14005fd20
                    if (const Item* it = objItem(op, code)) mesh = it->hasMesh;          // 0x140061360, 0x14004d800
                }
                classContactDefaults(p, op, &c.frc, &c.ihtc, mesh);   // vt+0x1a0
                out.push_back(c);                                // 0x1400ad3b0
            }
        }
    }(m ? m->step.get() : nullptr);
    return out;
}

// class vt+0x198: FormingExpress 0x14141ef60, Cogging / Distortion their own (L1, L6), else the base
std::vector<ContactPair> classDefaultPairs(Project& p, Operation& op)
{
    const String& key = op.classKey;
    if (keyIn(key, {u"SfOprFormingExpress", u"SfOprFormingExpress2D"})) return feDefaultPairs(p, op);
    if (key.u16() == u"SfOprCogging") return cogDefaultPairs(p, op);   // 0x14075ad80 (MDT L1)
    if (keyIn(key, {u"SfOprDistortion2D", u"SfOprDistortion3D"}))
        return distortionDefaultPairs(op);
    return baseDefaultPairs(p, op);
}

// @src 0x140360d30  b = mirror of a: copy, master and slave swapped, kind 2 <-> 1
void mirrorPair(def::model::Cls_1401f46e0* a, def::model::Cls_1401f46e0* b)
{
    if (!a || !b) return;
    def::model::pair_copy(b, a);                             // 0x1408e57e0 = 1401f9900
    *at<std::int32_t>(b, 0x168) = *at<std::int32_t>(a, 0x16c);
    *at<std::int32_t>(b, 0x16c) = *at<std::int32_t>(a, 0x168);
    if (*at<std::int32_t>(a, 0x170) == 2) *at<std::int32_t>(b, 0x170) = 1;
    else if (*at<std::int32_t>(a, 0x170) == 1) *at<std::int32_t>(b, 0x170) = 2;
}

// @src 0x14035e6f0  append a pair (kind 2, master, slave, friction +0x20, IHTC +0x1c0, 3D flag) and
// its mirror; null when an append fails
def::model::Cls_1401f46e0* appendPair(def::model::DB_STEP_DATA* s, const std::int32_t* ms, const std::int32_t* sl,
                                      double frc, double ihtc, std::uint8_t flag)
{
    auto* a = def::model::pair_append(&s->inter_obj);       // 0x1408e6980 = 1401faaa0
    if (!a) return nullptr;
    *at<std::int32_t>(a, 0x170) = 2;
    *at<std::int32_t>(a, 0x168) = *ms;
    *at<std::int32_t>(a, 0x16c) = *sl;
    *at<double>(a, 0x1c0) = ihtc;
    *at<double>(a, 0x20) = frc;
    if (*at<std::int32_t>(s, 0x1c) == 3) *at<std::uint32_t>(a, 0x174) = flag;   // 0x14035b400
    auto* b = def::model::pair_append(&s->inter_obj);
    if (!b) return nullptr;
    mirrorPair(a, b);                                        // 0x140360d30
    return a;
}

// @src 0x14035e540  append a copy of `tmpl` as pair (master, slave) and its mirror
def::model::Cls_1401f46e0* appendPairFrom(def::model::DB_STEP_DATA* s, const std::int32_t* ms,
                                          const std::int32_t* sl, def::model::Cls_1401f46e0* tmpl,
                                          std::uint8_t flag)
{
    auto* a = def::model::pair_append(&s->inter_obj);
    if (!a) return nullptr;
    if (tmpl) def::model::pair_copy(a, tmpl);               // 0x1408e57e0 = 1401f9900
    *at<std::int32_t>(a, 0x170) = 2;
    *at<std::int32_t>(a, 0x168) = *ms;
    *at<std::int32_t>(a, 0x16c) = *sl;
    if (*at<std::int32_t>(s, 0x1c) == 3) *at<std::uint32_t>(a, 0x174) = flag;
    auto* b = def::model::pair_append(&s->inter_obj);
    if (!b) return nullptr;
    mirrorPair(a, b);
    return a;
}

// @src 0x14005a9b0  the engine's inter-object list from the lubricant KEY and the class's default
// pairs
void rebuildSystemContacts(Project& p, Operation& op)
{
    withOpStep(p, op, [&](def::model::DB_STEP_DATA* s) {
        if (!s) return;
        // 0x140065790 (ICB, OPI+0x70) and 0x1400a59b0 (lubricant manager) always exist here
        if (!op.lubSystem) return;                              // 0x140065780: ICB+0x70
        const int lub = op.lubricantId;                         // 0x1400657f0
        def::model::Cls_1401f46e0* defPair = nullptr;
        auto local = std::make_unique<def::model::DB_STEP_DATA>(3);   // 0x140821bf0
        s->inter_obj.clear();                                   // 0x1408e6cc0
        if (lub > 0) {
            const Item* it = p.lubricants.find(lub);            // lubricant manager vt+0x208
            const String file = it ? qt::cleanPath(it->keyPath) : String();
            const std::string lp = local8(file);
            if (def::cmd::key_import_inter_object(local.get(), lp.c_str()))   // 0x140897fd0 = 1401b7530
                defPair = def::model::pair_nth(&local->inter_obj, 1);         // 0x1408e6b70 = 1401fac90
        }
        const std::vector<ContactPair> pairs = classDefaultPairs(p, op);     // ob->vt[0x198]
        for (ContactPair pr : pairs) {                          // Q_FOREACH over a copy, element by value
            if (def::model::LLOBJ_DATA* o = def::model::object_list_find(&s->objects, pr.master))   // 0x140830270
                def::cmd::bccdef_lubricant(o, &pr.master, reinterpret_cast<const char*>(&pr.lub),
                                           at<std::int32_t>(s, 0x1640));    // 0x140875a20 = 140201bc0
            if (!defPair) appendPair(s, &pr.master, &pr.slave, pr.frc, pr.ihtc, pr.f19);   // 0x14035e6f0
            else appendPairFrom(s, &pr.master, &pr.slave, defPair, pr.f19);              // 0x14035e540
        }
    });
}                                                           // 0x140824560 (local)

// @src 0x14005a8a0  InterObject.KEY
bool saveInterObject(Project& p, Operation& op, FileEffects& fx)
{
    GuiModel* m = opModel(p, op);
    return [&](def::model::DB_STEP_DATA* step) -> bool {
        if (!step) return false;
        if (op.lubSystem)                                    // 0x140065780(ICB) = OPI+0xe0
            rebuildSystemContacts(p, op);                    // 0x14005a9b0
        contactFlagsFromEngine(op, *m);                      // 0x1400655d0
        const String path = interObjectPath(op);             // 0x14005a690
        const int r = writeTaskKey(p, path, fx, 1,
            [&](const char* f) { return def::key::write_interobject_key_file(*step, f); },
            [&](std::FILE* f) { return def::key::write_interobject_key_stream(*step, f); }); // 140896030
        return r == 0;
    }(m ? m->step.get() : nullptr);
}

// @src 0x14005c760  object file of slot s (object manager vt+0x208 path, flags 0x3000)
bool saveObject(Project& p, Operation& op, int s, FileEffects& fx)
{
    return withOpStep(p, op, [&](def::model::DB_STEP_DATA* step) -> bool {
        const Item* it = p.objects.find(objId(op, s));           // vt+0x208(code)
        const String path = it ? qt::cleanPath(it->keyPath) : String();
        std::int32_t flags = 0x3000;
        int r = 1;
        if (!path.isEmpty())
            r = writeTaskKey(p, path, fx, 1,
                [&](const char* f) { return def::key::write_object_key_file(*step, f, s + 1, &flags, 0); },
                [&](std::FILE* f) { return def::key::write_object_key_stream(*step, f, s + 1, &flags, 0); }); // 140895f30
        return r == 0;
    });
}

// @src 0x14005d3e0 -> 0x14005d300  MeshSettingsNNNNN.KEY of the slots with MeshSetting
void saveMeshSettings(Project& p, Operation& op, FileEffects& fx)
{
    const int n = objSlots(op);
    for (int s = 0; s < n; ++s) {
        ObjEntry* e = objEntry(op, s);                       // 0x140061250
        if (!e || !e->meshSetting) continue;                 // 0x14004dfc0 (+0x1ba)
        withOpStep(p, op, [&](def::model::DB_STEP_DATA* step) {
            if (!step) return;
            writeTaskKey(p, slotKey(op, "MeshSettings", s), fx, 1, // 14005cf70
                [&](const char* f) { return def::key::write_meshsettings_key_file(*step, f, s + 1); },
                [&](std::FILE* f) { return def::key::write_meshsettings_key_stream(*step, f, s + 1); }); // 140896360
        });
    }
}

// @src 0x14005e480 -> 0x14005e2a0  RzElmCon and BccDef of the slots with DefineBCC and type 5
void saveBcc(Project& p, Operation& op, FileEffects& fx)
{
    const int n = objSlots(op);
    for (int s = 0; s < n; ++s) {
        ObjEntry* e = objEntry(op, s);
        if (!e || !e->defineBCC || e->objectType != 5) continue;   // 0x14004e0c0 (+0x1be), 0x14004d600
        GuiModel* m = opModel(p, op);
        if (!m || !m->step) continue;
        def::model::LLOBJ_DATA* o = m->object(s + 1);        // 0x140c7ae80
        if (!o || !objHasMesh(o)) continue;                  // 0x140834c80
        [&](def::model::DB_STEP_DATA* step) {
            if (!step) return;
            captureKey(slotKey(op, "RzElmCon", s), fx,       // 0x14005cce0
                       [&](const char* f) { def::key::write_rzelmcon_key_file(*step, f, s + 1); });   // 0x140896640
            captureKey(slotKey(op, "BccDef", s), fx,         // 0x14005da90
                       [&](const char* f) { def::key::write_bccdef_key_file(*step, f, s + 1); });     // 0x140896750
        }(m->step.get());
    }
}

// @src 0x14005ef80 -> 0x14005eea0  ObjPropertiesNNNNN.KEY of every slot record
void saveObjProperties(Project& p, Operation& op, FileEffects& fx)
{
    const int n = objSlots(op);
    for (int s = 0; s < n; ++s) {
        if (!objEntry(op, s)) continue;
        withOpStep(p, op, [&](def::model::DB_STEP_DATA* step) {
            if (!step) return;
            writeTaskKey(p, slotKey(op, "ObjProperties", s), fx, 1, // 14005eb10
                [&](const char* f) { return def::key::write_objproperties_key_file(*step, f, s + 1); },
                [&](std::FILE* f) { return def::key::write_objproperties_key_stream(*step, f, s + 1); }); // 140896280
        });
    }
}

// @src 0x140358190(step, obj, file, overwrite = 1)  equipment KEY of an engine object: ".KEY"
// appended unless the upper-cased name ends with it, then absolutePath + sep + baseName + "." +
// suffix with native separators (the "Overwrite File" question is skipped with overwrite)
void saveEquipment(Project& p, def::model::DB_STEP_DATA& step, def::model::LLOBJ_DATA* obj, String file, FileEffects& fx)
{
    file = equipmentKeyTarget(file);
    const std::int32_t no = *at<std::int32_t>(obj, 8);
    writeTaskKey(p, file, fx, 3,
        [&](const char* f) { return def::key::write_equipment_key_file(step, f, no); },
        [&](std::FILE* f) { return def::key::write_equipment_key_stream(step, f, no); }); // 140896840
}

int saveMaterialKey(Project& p, def::model::DB_STEP_DATA& step, int number,
                    const String& path, FileEffects& fx)
{
    def::model::Material* material = nullptr;
    if (fx.nativeTaskKeys) {
        // 140895a3c..a63 resolves the number BEFORE fopen; a missing material
        // never truncates the target. The stream body receives that same object.
        if (path.isEmpty()) return 1;
        material = def::model::material_list_find(&step.materials, number);
        if (!material) return 3;
    }
    return writeTaskKey(p, path, fx, 1,
        [&](const char* f) { return def::key::write_material_key_file(step, f, number, false, nullptr, nullptr); },
        [&](std::FILE* f) { return def::key::write_material_key_stream(step, f, material, false, nullptr, nullptr); });
}

// @src 0x140058cc0  every engine material of type != 2 to Materials/MaterialNNNNN.KEY (material
// manager vt+0x178 by number)
void saveMaterials(Project& p, Operation& op, FileEffects& fx)
{
    std::int32_t n = 0;
    withOpStep(p, op, [&](def::model::DB_STEP_DATA* step) {
        // 0x140c7cae0 returns zero for a present model with no step.
        if (step) n = *at<std::int32_t>(step, 0x1320);  // 0x1408303b0
    });
    for (std::int32_t i = 1; i <= n; ++i) {
        std::int32_t no = 0;
        const bool eligible = withOpStep(p, op, [&](def::model::DB_STEP_DATA* step) -> bool {
            // Null model / later null step remain guarded adapter divergences;
            // no native public Save fault-equivalence is established for them.
            if (!step) return false;
            def::model::Material* mat = def::model::material_list_nth(&step->materials, i);   // 0x140830460
            if (!mat || *at<std::int32_t>(mat, 0x1cc8) == 2) return false;
            no = *at<std::int32_t>(mat, 0);
            return true;
        });
        if (!eligible) continue;
        // Only the copied number survives path generation and the next lookup.
        const String path = materialDefaultFile(p, no);
        withOpStep(p, op, [&](def::model::DB_STEP_DATA* step) {
            if (step) saveMaterialKey(p, *step, no, path, fx); // status deliberately ignored
        });
    }
}


// @src 0x140358190  the file it writes: ".KEY" appended unless the upper-cased name ends with it,
// then convertSeparators(absolutePath + sep + baseName + "." + suffix)
String equipmentKeyTarget(String file)
{
    // toUpper().endsWith(".KEY"): the same as a case-insensitive test for this ASCII suffix
    if (!qt::endsWith(file, L(".KEY"), qt::Case::Insensitive)) file = file + L(".KEY");
    const String name = qt::baseName(file) + L(".") + qt::suffix(file);
    return qt::convertSeparators(qt::absolutePath(file) + qt::kSep + name);
}

// The GUI's version global 0x143dbcbd0 (0x1407fb240 at start-up: "11.0" + the pack.dat suffix),
// which the KEY banner 0x140895560 reads through 0x1407fb770; the engine twin of the banner reads
// def::model::g_version, so the start-up value goes there (the MDT header text without its
// "DEFORM-2D/3D " product).
void setEngineVersion(const std::u16string& deformDir)
{
    const std::string h = datafiles::mdtHeaderText(deformDir);
    const std::string v = h.substr(std::strlen("DEFORM-2D/3D "));
    std::strncpy(def::model::g_version, v.c_str(), sizeof def::model::g_version - 1);
}

// @src 0x140895350 (SfModel wrapper 0x140c7d100)
int saveWholeModelKey(Project& p, def::model::DB_STEP_DATA& step,
                      const String& path, std::uint32_t flags, FileEffects& fx)
{
    // strlen precedes the model query/flag store in the original. Keep its
    // first-NUL behavior after the caller's locale conversion.
    const std::string name = qt::encodeLocale(path, fx.nativePathCodepage);
    if (name.empty() || name.front() == '\0') return 1;
    if (!fx.nativeTaskKeys) {
        int result = 1;
        captureKey(path, fx, [&](const char* file) {
            result = def::cmd::write_key_file(&step, file, &flags);
        });
        return result;
    }
    if (step.u1b5a != 0)
        throw Error(M2M_E_UNSUPPORTED,
            "whole-model KEY: deferred DB-step temporary ownership and unchecked fopen failure remain unrecovered (0x140895350)");
    // The adapter's alias guard precedes original target mutation. Unlike
    // SimCtrl/object writers, this wrapper retains new flags on open failure.
    if (fx.beforeNativeKeyWrite) fx.beforeNativeKeyWrite(path);
    step.u1744 = static_cast<std::int32_t>(flags);
    datafiles::detail::CoggingFile file(name);
    if (!file.get()) return 1;
    p.overlay.erase(qt::cleanPath(path));
    const int result = (flags & 0x80000000u)
        ? def::cmd::write_key_stream_short(&step, file.get())
        : def::cmd::write_key_stream(&step, file.get());
    file.close(); // ignored even after a failed stream write/flush
    return result;
}

// @src 0x140057370
bool syncOperationFiles(Project& p, Operation& op, FileEffects& fx)
{
    GuiModel* m = opModel(p, op);
    if (!m) return false;
    makeFolders(p, op, fx);                                  // 0x140051190
    saveSimCtrl(p, op, fx);                                  // 0x1400597d0
    saveInterObject(p, op, fx);                              // 0x14005a8a0
    const int n = objSlots(op);                              // 0x14005fa80
    bool r = true;
    for (int s = 0; s < n; ++s)
        if (objId(op, s) > 0) r = saveObject(p, op, s, fx);  // 0x140060690, 0x14005c760
    saveMeshSettings(p, op, fx);                             // 0x14005d3e0
    saveBcc(p, op, fx);                                      // 0x14005e480
    saveObjProperties(p, op, fx);                            // 0x14005ef80
    for (int s = 0; s < n; ++s) {
        [&]() {
            def::model::LLOBJ_DATA* eo = m->object(s + 1);       // 0x140057710
            ObjEntry* e = objEntry(op, s);                       // 0x140061250
            if (!e || !eo) return;
            const int eq = e->equipmentId;                       // 0x14004de90 (+0x28)
            if (eq <= 0) return;
            const Item* it = p.equipment.find(eq);               // equipment manager vt+0x208
            saveEquipment(p, *m->step, eo, it ? qt::cleanPath(it->keyPath) : String(), fx);   // 0x140358190
        }();
    }
    saveMaterials(p, op, fx);                                // 0x140058cc0
    if (op.restartGiven) {                                   // 0x140064740
        const String path = restartKeyPath(op, true);        // 0x140064a50(op, 1)
        [&](def::model::DB_STEP_DATA* step) {
            saveWholeModelKey(p, *step, path, 0, fx);        // result ignored; NSTART follows
        }(m->step.get());
    }
    if (startsFromKey(op)) {                                 // 0x14005f230
        if (Cls_1401e7b10* sc = m->simCtl()) {
            op.nstartGiven = 1;                              // 0x14005a2c0
            op.startStepNo = sc->nstart;                     // 0x14005a350 (sc+0x160)
        }
    }
    return r;
}

// @src 0x140052ec0
void saveCurrentOperation(Project& p, Operation& op, Ctx&, FileEffects& fx)
{
    mst::OpClass* ob = opBase(op);                          // 0x140054e90
    if (!ob) return;
    // 0x1405ef9d0: ob->current and its project (the feature test 0x1400aab60 is licence code,
    // @src 0x1400aab60  omitted: licence)
    if (ob->current != &op || !op.project) return;
    // "Save operation - %1" / "Done - save operation" (messenger signals): UI
    const String& key = op.classKey;
    // MDT L7: own vt+0x68 of DoeOutput, Report and ShapeOptim. Their loads (OB vt+0x58) already
    // refuse at the 3D viewer or the report document, so these are reached only through that state.
    if (key.u16() == u"SfOprDoeOutput") {
        auto* sub = activeSubProject(p);
        if (op.project != &p || !sub || !sub->ownedDoeStudy)
            throw Error(M2M_E_UNSUPPORTED, "save current operation: SfOprDoeOutput needs the actual active Type2 Study owner");
        // 0x140e2e752: the directory attempt precedes even the operation-model
        // getter. A failed attempt returns natively only after its warning modal.
        const auto directory = doe::attemptOutputRegionsMkdirPrefix(p, *sub, *sub->ownedDoeStudy, fx);
        if (directory.outcome == doe::OutputRegionsMkdirPrefixOutcome::WarningModalRequired)
            throw Error(M2M_E_UNSUPPORTED, "save current operation: SfOprDoeOutput Control/Regions warning modal return remains unsupported");
        // 0x140e2e76b/773: current OPI -> 05f770, not the later ROIs getter.
        // Native null returns the whole callback before any cleanup or .DS work.
        if (!opModel(p, op)) return;
        throw Error(M2M_E_UNSUPPORTED, "save current operation: SfOprDoeOutput Control/Regions enumeration remains unsupported after operation-model lookup");
    }
    if (key.u16() == u"SfOprReport")
        throw Error(M2M_E_UNSUPPORTED, "save current operation: SfOprReport vt+0x68 0x1412adf90 runs vt+0x80 "
                                       "0x1412aeb30 (report document SP+0x30) and 0x1412af0c0 (viewer models, "
                                       "0x140d0d6f0); neither is reproduced");
    if (key.u16() == u"SfOprShapeOptim") {
        // @src 0x14013d6f0  vt+0x68: vt+0xd0 (the base 0x1405f1e20: props vt+0x60 0x140142d00 copies
        // the props data into OB +0xcc / +0xe0), vt+0x80 0x14013e4f0, vt+0xe8 0x14013c8f0 (MDT L4,
        // src/engine/shapeoptim_load.cpp); no vt+0xd8 and no 0x140057370
        if (ob->updateDataInWidgetArea(p, op, fx)) ob->propsLoad(p, op);
        else propsToEngine(p, op);
        ob->saveAfterFiles(p, op, fx);
        ob->saveOperationFiles(p, op, fx);
        return;
    }
    // OB vt+0x68 = 0x1405ef510 except these classes (spec section 1)
    const bool hte = keyIn(key, {u"SfOprHeatTransferExpress", u"SfOprHeatTransferExpress2D"});
    const bool fe = keyIn(key, {u"SfOprFormingExpress", u"SfOprFormingExpress2D"});
    if (keyIn(key, {u"SfOprReport", u"SfOprDoeOutput"}))
        notPorted(op, "vt+0x68");
    // vt+0x68 of SfOprPre 0x141348e90 / SfOprPre2D 0x141347990: empty (MDT L2)
    if (keyIn(key, {u"SfOprPre3D", u"SfOprPre2D"})) return;
    // Express classes: vt+0x68 = 0x1413f8f60, its own part, then the base 0x1405ef510 (MDT L3S)
    if (hte || fe) expressSaveOperation(p, op);
    // @src 0x1405ef510  SfOprBase vt+0x68
    // vt+0xd0 = 0x1405f1e20 for every class that reaches here except DoeOutput and Report: the
    // property widget index (signalGetPropertyStackWidgetIndex, UI) is 0 without a widget, so
    // 0x1400543a0 (props vt+0x60) runs; with a class editor shown (index != 0, MDT L5S:
    // Convert2Dto3D's Configuration editor) signalUpdateDataInWidgetArea reaches the editor, then
    // 0x140054350 (props vt+0x58)
    if (ob->updateDataInWidgetArea(p, op, fx)) ob->propsLoad(p, op);
    else propsToEngine(p, op);                              // 0x1400543a0
    // 0x1405ef510 calls 0x1400543a0 again after vt+0xd0 returns.
    // A shown editor therefore applies, refreshes properties, then copies them
    // to the model. Index zero runs propsToEngine twice; do not collapse it.
    propsToEngine(p, op);
    // vt+0xd8: base 0x1405f22e0 (ProcType 0x143d766b8+0xc is 5 after opening a .moproj); Cogging
    // 0x14075c260 overrides saveNames (MDT L1B); SfOprHTFurnace vt+0xd8 0x140c65940: the base, then
    // a props dynamic_cast and the sim control, both unused (MDT L2): its base body runs
    if (hte) hteSaveTmax(p, op);                            // 0x1413e6460 / 0x1413e5ba0 (no 0x140054540)
    else if (fe) feSaveSimNames(p, op);                     // 0x14141cf90
    else ob->saveNames(p, op, fx);                          // vt+0xd8 (base 0x1405f22e0)
    // vt+0xe8: base 0x1405f2350 = 0x140057370; classes with their own (MDT L5S) do it themselves;
    // Cycle 0x14134dae0 and Group 0x141352530 are empty (MDT L7): no task files for a cycle or group
    const bool emptyE8 = keyIn(key, {u"SfOprCycle", u"SfOprGroup"});
    if (!emptyE8 && !ob->saveOperationFiles(p, op, fx)) {
        if (hte || fe) expressSyncOperation(p, op, fx);     // vt+0xe8 0x1413f91f0 (calls 0x140057370)
        else syncOperationFiles(p, op, fx);                 // 0x140057370
    }
    // vt+0x80 (after vt+0xe8): empty for Forming 0x140c06770, Forming2D 0x140c04f90, HeatTransfer
    // 0x1413543f0, HeatTransfer2D 0x141353600, Boolean 0x1411df010, MultiBlow(2D) 0x140602770; the
    // others have their own (SfOprBase's slot is _purecall)
    // Cycle 0x14134d780 and Group 0x1413524c0 are empty too (MDT L7); (MDT L3S: HeatTransferExpress
    // 0x1413e64d0, HeatTransferExpress2D 0x1413e5c10 and FormingExpress 0x14141d320 are empty too)
    if (!ob->saveAfterFiles(p, op, fx) &&
        !keyIn(key, {u"SfOprForming", u"SfOprForming2D", u"SfOprHeatTransfer", u"SfOprHeatTransfer2D",
                     u"SfOprBoolean2D", u"SfOprBoolean3D", u"SfOprCycle", u"SfOprGroup", u"SfOprCogging",
                     u"SfOprHTFurnace", u"SfOprHeatTransferExpress", u"SfOprHeatTransferExpress2D",
                     u"SfOprFormingExpress", u"SfOprFormingExpress2D", u"SfOprMultiBlow2D", u"SfOprMultiBlow3D"}))
        notPorted(op, "vt+0x80");   // Cogging 0x1407507a0 (MDT L1), HTFurnace 0x140c659e0 (MDT L2): empty
    // SfOprHeatTreat vt+0x68 0x1411b1320 / 0x1411aed00: after the base above, the schedule's
    // thermal-condition files and HTWZ (MDT L6T, save_l6.cpp)
    if (keyIn(key, {u"SfOprHeatTreat", u"SfOprHeatTreat2D"})) heatTreatSaveSchedule(p, op, fx);
}

}  // namespace m2m::engine
