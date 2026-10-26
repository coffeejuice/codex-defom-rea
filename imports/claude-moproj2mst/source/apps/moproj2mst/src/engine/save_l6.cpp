// MDT L6S: save-current-operation slots of SfOprHeatTreat(2D), SfOprDistortion, SfOprDiestress and
// SfOprDiestressBM(2D/3D) (docs/re/moproj2mst/mdt/L6/spec.md section 7). Packet S's
// saveCurrentOperation (save_current.cpp) calls these where its keyed slot dispatch reaches the
// classes' own bodies. Decompiles: deform-rea-work/gui_mo/s1/mdt/L6/s0.c .. s4.c, d6.c (0x1411bfa20),
// d5.c (0x1411c05a0), s1.c (0x1402a16c0).
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <cstdint>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iterator>
#include <string>
#include <vector>

#include <defcmd/dispatch.hpp>
#include <defmodel/dynary_init.hpp>
#include <defmodel/layout/cls_1401e7b10.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/llobj/object_list.hpp>
#include <defrt/stdlib/legacy_printf.hpp>
#include <defmodel/keyword_registry.hpp>

#include "m2m/data/distortion.hpp"
#include "m2m/data/diestress.hpp"
#include "m2m/mst/emit.hpp"
#include "m2m/mst/stream.hpp"
#include "m2m/mst/file_device.hpp"

#include "cogging_save.hpp"
#include "engine_internal.hpp"
#include "m2m/data/diestressbm.hpp"
#include "m2m/data/heattreat.hpp"
#include "m2m/data/l6_blocks.hpp"
#include "m2m/engine.hpp"
#include "m2m/ht_schedule.hpp"
#include "m2m/props.hpp"
#include "m2m/qt4path.hpp"
#include "save_l6.hpp"
#include "../datafiles/qdom_write.hpp"
#include "m2m/mst/loader_l6.hpp"

namespace m2m::engine {
namespace {

using def::model::Cls_1401e7b10;
using def::model::LLOBJ_DATA;
using qt::L;

[[noreturn]] void unsupported(const Operation& op, const std::string& what)
{
    throw Error(M2M_E_UNSUPPORTED, "save current operation: " + op.classKey.utf8() + " " + what);
}

std::string readAll(const std::filesystem::path& f)
{
    std::ifstream in(f, std::ios::binary);
    return std::string(std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>());
}

// An engine writer's fopen(path) is given a scratch file; its bytes are recorded for `target`
// (as packet S's captureKey).
void captureKey(const String& target, FileEffects& fx, const std::function<void(const char*)>& write)
{
    const std::filesystem::path tmp = std::filesystem::temp_directory_path() /
                                      ("m2m_l6s_" + std::to_string(GetCurrentProcessId()) + "_" +
                                       std::to_string(GetCurrentThreadId()) + ".KEY");
    std::error_code ec;
    std::filesystem::remove(tmp, ec);
    write(tmp.string().c_str());
    if (std::filesystem::exists(tmp, ec)) {
        fx.writes.push_back({target, readAll(tmp)});
        std::filesystem::remove(tmp, ec);
    }
}

// @src 0x1402a16c0  DieStressBmWizard.Wzd <- the wizard block (QFile WriteOnly | Text)
void writeBmWizard(const data::DiestressBMData& d, const String& path, FileEffects& fx, bool native = false)
{
    std::unique_ptr<mst::FileDevice> file;
    if (native) {
        file = std::make_unique<mst::FileDevice>(path);
        if (!file->opened()) return;                      // open precedes DOM creation and block reads
    }
    datafiles::DomElement root;                            // createElement("DieStressBmWizard")
    root.name = u"DieStressBmWizard";
    root.setInt(u"nObjTotalNum", d.objTotalNum);
    root.setString(u"qsNewDbName", d.newDbName);
    root.setInt(u"bUseNewDb", static_cast<char>(d.useNewDb));
    for (int k = 0; k < d.objTotalNum; ++k) {
        if (k >= 15) throw Error(M2M_E_UNSUPPORTED, "DieStressBmWizard.Wzd: more than 15 objects (0x1402a16c0 "
                                                    "reads past the wizard block)");
        // "ObjId00" with bytes 5 and 6 replaced: '0' + k/10, '1' + k%10
        const char16_t nm[8] = {u'O', u'b', u'j', u'I', u'd', static_cast<char16_t>(k / 10 + '0'),
                                static_cast<char16_t>(k % 10 + '1'), 0};
        datafiles::DomElement e;
        e.name = nm;
        const auto i = static_cast<std::size_t>(k);
        e.setInt(u"nObjNo", d.objNo[i]);
        e.setInt(u"nRefNo", d.refNo[i]);
        e.setInt(u"bIntepolate", static_cast<char>(d.interpolate[i]));
        e.setDouble(u"dTolerance", d.tolerance[i]);
        root.children.push_back(std::move(e));
    }
    if (file) {
        (void)mst::writeTextMode(*file, datafiles::documentUtf8(u"DieStressBmWizard", root));
        file->close();                                    // writer ignores write and close status
    } else fx.writes.push_back({path, datafiles::documentBytes(u"DieStressBmWizard", root)});
}

// @src 0x14085fdd0  free the tables of an object's movement block (+0x2160)
void movementClear(std::byte* mv)
{
    static const std::size_t tables[] = {0xe8, 0x70, 0x208, 0x2a0, 0x3b0, 0x448, 0x660, 0x6d8};
    for (std::size_t off : tables) def::model::dynary_free_arrays(at<def::model::DYNARY>(mv, off));   // 0x14089b1f0
    *at<std::uint8_t>(mv, 0x8b8) = 0;
    static const std::size_t tables2[] = {0x8c0, 0x968, 0x9e0, 0x790, 0x808};
    for (std::size_t off : tables2) def::model::dynary_free_arrays(at<def::model::DYNARY>(mv, off));
}

// @src 0x14084e770 (= DEF_PRE 14017a550)  movement reset of an object and of its master object
// (+0x2360)
void objectMovementReset(LLOBJ_DATA* o)
{
    const int master = *at<std::int32_t>(o, 0x2360);
    if (master > 0) {
        auto* owner = *at<def::model::DB_STEP_DATA*>(o, 0x3a98);
        LLOBJ_DATA* q = owner ? def::model::object_list_find(&owner->objects, master) : nullptr;   // 0x140830270
        if (q) {
            movementClear(at<std::byte>(q, 0x2160));
            movementDefaults(at<std::byte>(q, 0x2160));      // 0x14085f6d0
        }
    }
    movementClear(at<std::byte>(o, 0x2160));
    movementDefaults(at<std::byte>(o, 0x2160));
}

// @src 0x140840bc0 (= DEF_PRE 14016c9a0)  node values of node set +0x8a0 slot +0x10 cleared
// (width data+0x15fc per node)
void objectNodeVar1Clear(LLOBJ_DATA* o)
{
    auto* owner = *at<def::model::DB_STEP_DATA*>(o, 0x3a98);
    const int w = owner->u15f8[1];                         // data+0x15fc
    const int n = *at<std::int32_t>(o, 0x880);
    double* v = *at<double*>(o, 0x8b0);
    if (n > 0 && w > 0 && !v)
        throw Error(M2M_E_UNSUPPORTED, "node values (+0x8b0) of a rigid object are not allocated (0x140840bc0 "
                                       "writes through the null pointer)");
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < w; ++j) v[i * w + j] = 0.0;
}

}  // namespace

// @src 0x1411c05a0  HeatTreat props vt+0x70: sim control <- block +0x58
void heatTreatPropsToSimCtl(Project& p, HeatTreatProps& hp, Cls_1401e7b10* sc)
{
    if (!sc) return;
    if (hp.data && !hp.data->schedule.engineDefaults) ht::engineDefaults(p, hp.data->schedule);
    const data::HTPropsBlock& b = *hp.block;
    *at<std::int32_t>(sc, 0x108) = b.b0 ? 1 : 0;           // 0x1408e0000
    *at<std::int32_t>(sc, 0x110) = b.b1 ? 1 : 0;           // 0x1408e0040
    *at<std::int32_t>(sc, 0x10c) = b.b2 ? 1 : 0;           // 0x1408e0020
    *at<std::int32_t>(sc, 0x1c8) = b.stpdef == 0 ? 1 : 3;
    *at<double>(sc, 0x1d0) = b.stpdef == 0 ? b.dtmax : b.dtpmax2;
    *at<double>(sc, 0x270) = b.dtpmax1;
    *at<double>(sc, 0x2a0) = b.dtpmax2;
    *at<double>(sc, 0x2a8) = b.f28;
    *at<std::int32_t>(sc, 0x164) = b.stepIncr;
}

// @src 0x1411bfa20  HeatTreat props vt+0x60: vt+0x70, then the schedule <- block lists
void heatTreatPropsToEngine(Project& p, Operation& op)
{
    auto* hp = dynamic_cast<HeatTreatProps*>(op.props.get());
    if (!hp || !hp->data || !hp->block) unsupported(op, "HeatTreat properties without their blocks (+0x58, +0x68)");
    if (hp->owner)                                          // 0x1405f8d90
        if (GuiModel* m = opModel(p, *hp->owner))           // 0x14005f7e0
            if (Cls_1401e7b10* sc = m->simCtl()) heatTreatPropsToSimCtl(p, *hp, sc);
    data::HTSchedule& s = hp->data->schedule;
    if (!s.engineDefaults) ht::engineDefaults(p, s);        // 0x14172d790 / 0x1411b3540
    const data::HTPropsBlock& b = *hp->block;
    const int n = b.stepCount;                              // block+4
    for (int i = 0; i < n; ++i) {
        if (!(!s.steps.empty() && i > -1 && i < static_cast<int>(s.steps.size()))) continue;   // 0x1411b0af0
        data::HTStep& st = *s.steps[static_cast<std::size_t>(i)];
        int mi = st.media;
        if (!(!s.media.empty() && mi > -1 && mi < static_cast<int>(s.media.size()))) {   // 0x1411c2920
            mi = 0;
            st.media = 0;
        }
        if (static_cast<std::size_t>(i) >= b.time.size() || static_cast<std::size_t>(i) >= b.media.size() ||
            static_cast<std::size_t>(i) >= b.mediaFlag.size() || static_cast<std::size_t>(i) >= b.t3.size())
            unsupported(op, "HeatTreat property block shorter than its step count (QList::at out of range)");
        data::HTMedia* md = (!s.media.empty() && mi > -1 && mi < static_cast<int>(s.media.size()))
                                ? s.media[static_cast<std::size_t>(mi)].get()
                                : (s.media.push_back(std::make_unique<data::HTMedia>()), s.media.back().get());   // 0x1411c2860
        const double time = b.time[static_cast<std::size_t>(i)];   // +0xa0
        const qt::String name = b.media[static_cast<std::size_t>(i)];   // +0xa8
        const bool flag = b.mediaFlag[static_cast<std::size_t>(i)];     // +0xd0
        if (time >= 0.0) st.time = time;                    // 0x1411c2b30
        md->name = name;                                    // 0x140dfbd00
        if (st.t20.ndim == 0) {                             // 0x14089b800(step+0x20)
            st.t3 = b.t3[static_cast<std::size_t>(i)];      // 0x1411c2b60
            st.t20.value = st.t3;
        }
        if (b.b1 && st.t98.ndim == 0) {                     // 0x1411c24c0 table
            if (static_cast<std::size_t>(i) >= b.t4.size())
                unsupported(op, "HeatTreat property block without t4 for a step (QList::at out of range)");
            const double t4 = b.t4[static_cast<std::size_t>(i)];
            if (t4 >= 0.0) st.t4 = t4;                      // 0x1411c2b90
            st.t98.value = t4;
        }
        md->radiation = flag;                               // 0x1411c2b10
    }
}

// @src 0x1402b5be0  SfOprDiestress vt+0xe8: the model as <sub-project DB base>.KEY (0x140c7d100)
void diestressSaveKey(Project& p, Operation& op, FileEffects& fx)
{
    GuiModel* m = opModel(p, op);
    if (!m) return;
    SubProject* sp = activeSubProject(p);
    const String db = sp ? subProjectDbPath(p, *sp, true) : String();   // vt+0xf0 = 0x1402b6240(self, s, 1)
    const String key = app::changeSuffix(db, L(".KEY"));               // 0x1402b6310 -> 0x140818d50
    [&](def::model::DB_STEP_DATA* step) -> void {
        if (!step) return;
        captureKey(key, fx, [&](const char* f) {                       // 0x140895350 = 1401c5810
            const std::uint32_t flags = 0;
            def::cmd::write_key_file(step, f, &flags);
        });
    }(m->step.get());
}

// @src 0x140299870  SfOprDiestressBM vt+0xd8, the part before the base 0x1405f22e0: movement and
// node values of every rigid (kind 3) object reset
void diestressBMBeforeSimNames(Project& p, Operation& op)
{
    GuiModel* m = opModel(p, op);
    if (!m) return;
    const int n = m->objectCount();
    for (int i = 0; i < n; ++i) {
        LLOBJ_DATA* o = m->objectAt(i + 1);                 // 0x140c7aed0
        if (o && *at<std::int32_t>(o, 0xc) == 3) {
            objectMovementReset(o);                         // 0x14084e770
            objectNodeVar1Clear(o);                         // 0x140840bc0
        }
    }
}

// @src 0x140299b60  SfOprDiestressBM vt+0x80: wizard block <- model (0x1402a2910), then the file
void diestressBMSaveWizard(Project& p, Operation& op, FileEffects& fx)
{
    auto* bp = dynamic_cast<DiestressBMProps*>(op.props.get());   // 0x1405ef320(self, 0), TD 0x1435fb1a8
    if (!bp) return;
    if (!bp->data) unsupported(op, "DiestressBM properties without their wizard block (props+0x38)");
    data::DiestressBMData& d = *bp->data;
    // 0x1402a2910(data, opi)
    if (GuiModel* m = opModel(p, op)) {
        d.objTotalNum = m->objectCount();
        for (int i = 0; i < d.objTotalNum; ++i) {
            if (i >= 15) unsupported(op, "DiestressBM wizard block of more than 15 objects (0x1402a2910 writes past it)");
            LLOBJ_DATA* o = m->objectAt(i + 1);
            if (!o) unsupported(op, "object position " + std::to_string(i + 1) + " is empty (0x1402a2910)");
            d.objNo[static_cast<std::size_t>(i)] = *at<std::int32_t>(o, 8);
        }
    }
    if (d.newDbName.size() < 2) d.newDbName = L("DieStressDb.DB");   // @0x142289b30
    // 0x14029eef0(props): convertSeparators(taskDirAbs(owner) + sep + "DieStressBmWizard.Wzd")
    const String dir = bp->owner ? taskDirAbs(*bp->owner) : String();
    const String path = qt::convertSeparators(dir + qt::kSep + L("DieStressBmWizard.Wzd"));
    writeBmWizard(d, path, fx);                             // 0x1402a16c0
}

void installL6SaveSlots()
{
    mst::L6Slots& t = mst::g_l6Slots;
    t.loadPatternInit = [](Project& p, Operation& op, LoadPatternForm& f) {
        if (GuiModel* m = opModel(p, op)) loadPatternFormInit(*m, f);
    };
    t.loadPatternUpdate = [](Project& p, Operation& op, const LoadPatternForm& f) {
        if (GuiModel* m = opModel(p, op)) loadPatternUpdate(*m, f);
    };
    t.diestressSaveFiles = [](mst::OpClass&, Project& p, Operation& op, FileEffects& fx) {
        diestressSaveKey(p, op, fx);
        return true;
    };
    t.diestressBMSaveNames = [](mst::OpClass& c, Project& p, Operation& op, FileEffects& fx) {
        diestressBMBeforeSimNames(p, op);
        c.mst::OpClass::saveNames(p, op, fx);           // 0x1405f22e0
    };
    t.diestressBMSaveAfter = [](mst::OpClass&, Project& p, Operation& op, FileEffects& fx) {
        diestressBMSaveWizard(p, op, fx);
        return true;
    };
}


// ---- props vt+0xb8: the data files of File > Save step 3 (after the .moproj) -------------------

namespace {

// @src 0x1402b9050  SfOprDiestressProperties vt+0xb8: DieStress.Wzd (QFile WriteOnly | Text)
void diestressWizardSave(DiestressProps& dp, FileEffects& fx, Ctx& ctx)
{
    const String dir = dp.owner ? taskDirAbs(*dp.owner) : String();   // 0x1405f8d90 -> 0x140050df0
    const String path = qt::convertSeparators(dir + qt::kSep + L("DieStress.Wzd"));   // @0x1422b7f60
    std::unique_ptr<mst::FileDevice> file;
    if (ctx.nativePropertyFiles) {
        file = std::make_unique<mst::FileDevice>(path);
        if (!file->opened()) return;                      // 0x1402b9191: skip DOM/data on open failure
    }
    if (!dp.data) throw Error(M2M_E_UNSUPPORTED, "Diestress properties without their data (props+0x38)");
    datafiles::DomElement root;                             // QDomDocument("DieStressWizard")
    root.name = u"DieStressWizard";
    root.setInt(u"nStepNumber", dp.data->stepNumber);       // *(PROPS+0x38)+0x80
    if (file) {
        (void)mst::writeTextMode(*file, datafiles::documentUtf8(u"DieStressWizard", root));
        file->close();
    } else fx.writes.push_back({path, datafiles::documentBytes(u"DieStressWizard", root)});
}

// QTextStream on a QFile opened WriteOnly (no Text flag): "\n" as is; every endl flushes
using mst::endl;

// @src 0x1411638a0  BEGIN_FREE_DIST_BCC_INFO .. END_FREE_DIST_BCC_INFO
void writeFreeDistBcc(mst::Stream& ts, const data::FreeDistBcc& fd)
{
    ts << "BEGIN_FREE_DIST_BCC_INFO" << endl;
    ts << "BCC_INIT    \t" << static_cast<int>(fd.f91) << endl;            // +0x91
    ts << "FIXEDNODE    \t" << fd.fixedNode << endl;                       // 0x141344d40
    ts << "FIXEDCOORD    ";
    for (int k = 0; k < 3; ++k) ts << "\t" << fd.fixedCoord[k];
    ts << endl;
    ts << "FIXED_Z_COORD    ";
    for (int k = 0; k < 3; ++k) ts << "\t" << fd.fixedZ[k];
    ts << endl;
    ts << "FIXED_X_COORD    ";
    for (int k = 0; k < 3; ++k) ts << "\t" << fd.fixedX[k];
    ts << endl;
    ts << "FIXED_Y_COORD    ";
    for (int k = 0; k < 3; ++k) ts << "\t" << fd.fixedY[k];
    ts << endl;
    ts << "AUTO    \t" << static_cast<int>(fd.autoFix) << endl;            // 0x1411634e0
    ts << "FIXEDZNODE    \t" << fd.fixedZNode << endl;
    ts << "FIXEDZANGLE    \t" << fd.fixedZAngle << endl;
    ts << "FIXEDXNODE    \t" << fd.fixedXNode << endl;
    ts << "FIXEDXANGLE    \t" << fd.fixedXAngle << endl;
    ts << "FIXEDYNODE    \t" << fd.fixedYNode << endl;
    ts << "FIXEDYANGLE    \t" << fd.fixedYAngle << endl;
    ts << "END_FREE_DIST_BCC_INFO" << endl;
}

// @src 0x141367b80  MachDistortion.MDWZ (QFile WriteOnly, QTextStream)
void writeMdwz(const data::DistortionData& d, const String& dir, FileEffects& fx, Ctx& ctx)
{
    const String path = qt::convertSeparators(dir + qt::kSep + L("MachDistortion.MDWZ"));   // 0x1413667b0
    std::unique_ptr<mst::FileDevice> file;
    if (ctx.nativePropertyFiles) {
        file = std::make_unique<mst::FileDevice>(path);
        if (!file->opened()) return;                     // caller still attempts INI files
    }
    mst::Stream ts(ctx.codepage, file.get());
    ts << "WZD_MACHDIST_WORKPIECE_OBJECT_NO" << endl << "  * OBJECT # FOR WORKPIECE " << endl << d.workpiece << endl
       << endl;
    ts << "WZD_MACHDIST_NUM_FIXTURES" << endl << "  * NUMBER OF FIXTURES AND THEIR OBJECT #" << endl
       << d.fixtureCount << endl << endl;
    for (int i = 0; i < d.fixtureCount; ++i) {
        if (i >= 100) throw Error(M2M_E_UNSUPPORTED, "MachDistortion.MDWZ: more than 100 fixtures");
        ts << d.fixtures[static_cast<std::size_t>(i)] << endl;
    }
    ts << "WZD_MACHDIST_NUM_PASSES" << endl << "  * NUMBER OF PASSES AND THEIR OBJECT #" << endl << d.passCount << endl
       << endl;
    for (int i = 0; i < d.passCount; ++i) {
        if (i >= 100) throw Error(M2M_E_UNSUPPORTED, "MachDistortion.MDWZ: more than 100 passes");
        ts << d.passObjects[static_cast<std::size_t>(i)] << endl;
    }
    writeFreeDistBcc(ts, d.freeDist);                       // +0x18
    ts << " INIT_VEL_BCC" << "\t" << static_cast<int>(d.initVelBcc) << endl;   // 0x14004ef60: +0x10
    ts << "DONE" << endl << endl;
    if (file) file->close();                             // successful open returns1, no status query
    else fx.writes.push_back({path, ts.bytes()});
}

// @src 0x141367940  OBJPOS records of the operation for object no (3D 0x140065f80, 2D 0x140065af0)
void writeObjposOf(Operation& op, int no, mst::Stream& ts, Ctx& ctx)
{
    mst::State st;
    mst::W w{ts, st, ctx};
    const int n = static_cast<int>(op.objpos.size());      // 0x140062070
    for (int i = 0; i < n; ++i) {
        const ObjPos* r = op.objpos[static_cast<std::size_t>(i)].get();   // 0x1400620a0
        if (!r || r->object != no) continue;
        if (!is2D(op)) mst::emitObjposLine3D(w, *r);       // 0x14005f150
        else mst::emitObjposLine2D(w, *r);
    }
}

// @src 0x141366d60  DEF_MACHDIST_CTRL2.INI_<i> for i = 0 .. passes + 1 (QFile WriteOnly)
void writeMachDistIni(data::DistortionData& d, Operation& op, const String& dir, FileEffects& fx, Ctx& ctx)
{
    // the project path and its QDir(..).cdUp(), 0x1400a58f0 and 0x140050bc0: computed, unused
    for (int i = 0; i <= d.passCount + 1; ++i) {
        const String path = qt::convertSeparators(qt::cleanPath(dir + qt::kSep + L("DEF_MACHDIST_CTRL2.INI_") +
                                                                qt::number(i)));   // 0x141366a00 / 0x141366c30
        std::unique_ptr<mst::FileDevice> file;
        if (ctx.nativePropertyFiles) {
            file = std::make_unique<mst::FileDevice>(path);
            if (!file->opened()) continue;              // failed open skips ALL per-file state changes
        }
        mst::Stream ts(ctx.codepage, file.get());
        char nm[32];
        std::snprintf(nm, sizeof nm, "Step%d.KEY", i);      // 0x141366850
        const String in = qt::cleanPath(op.taskDir + qt::kSep + L(nm));   // 0x140051110: OPI+0x130
        ts << " * Keyword file which contains the workpiece " << endl << def::model::keyword_name(3) << endl;
        if (in.isEmpty()) ts << "(null)";
        //0x1413671e5 uses QString::operator const char*, then the const-char
        //stream overload. Under the adapter's null CString-codec profile this
        //loses code units above Latin1 before the output locale encoding, and
        //the char pointer truncates at NUL. The output filename below is QString.
        else ts << qt::toAscii(in).c_str();
        ts << endl << endl;
        ts << " * Workpiece object #" << endl << "WZD_MACHDIST_WORKPIECE_OBJECT_NO" << "\t" << d.workpiece << endl
           << endl;
        if (d.fixtureCount > 0) {
            ts << " * Number of fixtures and their keyword file name" << endl << "WZD_MACHDIST_NUM_FIXTURES" << "\t"
               << d.fixtureCount << endl << endl;
            for (int k = 1; k <= d.fixtureCount; ++k)
                ts << "WZD_MACHDIST_FIXTURE_READ_DB" << "\t" << d.fixtures[static_cast<std::size_t>(k - 1)] << endl
                   << endl;
        }
        if (i == 0) {
            ts << " * Keep Original Workpiece operation" << endl << "WZD_MACHDIST_OPERATION_KEEP_WORKPIECE_ONLY" << endl
               << endl;
        } else if (i == d.passCount + 1) {
            ts << " * Unload operation" << endl << "WZD_MACHDIST_OPERATION_UNLOAD" << endl << endl;
        } else {
            const int obj = d.passObjects[static_cast<std::size_t>(i - 1)];
            ts << "WZD_MACHDIST_PASS_OBJNO" << "\t" << obj << endl << endl;
            if (0 < i && i <= d.passCount) writeObjposOf(op, obj, ts, ctx);
            ts << " * Boolean operation" << endl << "WZD_MACHDIST_OPERATION_BOOLEAN" << endl << endl;
        }
        if (i == 1) {
            writeObjposOf(op, d.workpiece, ts, ctx);
            for (int k = 1; k <= d.fixtureCount; ++k)
                writeObjposOf(op, d.fixtures[static_cast<std::size_t>(k - 1)], ts, ctx);
        }
        const mst::OpClass* c = opBase(op);
        if (!c || c->dim() == 3) {                          // 0x14005f1c0
            if (i == 0) d.freeDist.f91 = 1;                 // 0x1411e2450
            writeFreeDistBcc(ts, d.freeDist);
            d.freeDist.f91 = 0;
        }
        char on[40];
        std::snprintf(on, sizeof on, "DEF_MACHDIST_OUT%d.KEY", i);   // 0x1413668e0
        const String out = qt::cleanPath(op.taskDir + qt::kSep + L(on));
        ts << " * Output file name " << endl << "WZD_MACHDIST_OUTPUT_FILENAME" << endl << out << endl << endl;
        if (file) file->close();                         // stream failure does not stop loop/body state
        else fx.writes.push_back({path, ts.bytes()});
    }
}

}  // namespace

// @src 0x141365c30 -> 0x141367a30  SfOprDistortionProperties vt+0xb8
// @src 0x1402b9050  SfOprDiestressProperties vt+0xb8
// @src 0x14029ee50  SfOprDiestressBMProperties vt+0xb8: 0x1402a16c0(+0x38, 0x14029eef0(props))
// @src 0x1411c0910  SfOprHeatTreatProperties vt+0xb8: 0x1411b3b20(+0x68)
bool l6PropsSave(Project& p, Operation& op, FileEffects& fx, Ctx& ctx)
{
    Props* pr = op.props.get();
    if (auto* dp = dynamic_cast<DiestressProps*>(pr)) {
        diestressWizardSave(*dp, fx, ctx);
        return true;
    }
    if (auto* bp = dynamic_cast<DiestressBMProps*>(pr)) {
        if (!bp->data) unsupported(op, "DiestressBM properties without their wizard block (props+0x38)");
        const String dir = bp->owner ? taskDirAbs(*bp->owner) : String();
        writeBmWizard(*bp->data, qt::convertSeparators(dir + qt::kSep + L("DieStressBmWizard.Wzd")), fx,
                      ctx.nativePropertyFiles);
        return true;
    }
    if (auto* sp = dynamic_cast<DistortionProps*>(pr)) {
        if (!sp->data) unsupported(op, "Distortion properties without their pass data (props+0x40)");
        if (!sp->owner) return true;                        // +0x5d8 owner: -1
        const String dir = taskDirAbs(*sp->owner);
        writeMdwz(*sp->data, dir, fx, ctx);                 // 0x141367b80
        writeMachDistIni(*sp->data, *sp->owner, dir, fx, ctx);   // 0x141366d60
        return true;
    }
    if (auto* hp = dynamic_cast<HeatTreatProps*>(pr)) {
        if (!hp->data) unsupported(op, "HeatTreat properties without their schedule (props+0x68)");
        heatTreatWriteSchedule(p, hp->data->schedule, fx, &ctx);   // 0x1411b3b20
        return true;
    }
    return false;
}

}  // namespace m2m::engine
