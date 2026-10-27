// MDT L5: operation-loader slots of SfOprBoolean, SfOprConvert2Dto3D and SfOprCopyMirror
// (docs/re/moproj2mst/mdt/L5/spec_L5.md; decompiles deform-rea-work/gui_mo/s1/mdt/L5/d*.c, a*.asm).
// One function per binary function, in the original's call order. Engine calls go to the DEF_PRE
// port (GUI twin = DEF_PRE address in the comments). Viewer-only calls (the GL scene of the main
// window, its second model, the viewer items) are dropped and named; the one viewer value engine code
// reads (CopyMirror's plane count) is kept in GuiModel::viewerMirrorPlanes.
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <cstdint>
#include <cstdio>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iterator>
#include <map>
#include <memory>

#include <defcmd/cnvt3d.hpp>
#include <defcmd/dispatch.hpp>
#include <defcmd/interp.hpp>
#include <defcmd/key_file.hpp>
#include <defcmd/key_import.hpp>
#include <defcmd/objpos_centre.hpp>
#include <defkey/item_writers.hpp>
#include <defkey/key_writer.hpp>
#include <defkey/kw_g2mg.hpp>
#include <defmodel/geo_kw/misc_model.hpp>
#include <defmodel/gui_twins.hpp>
#include <defmodel/layout/cls_1401e7b10.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/llobj_ctor/llobj_copy.hpp>
#include <defmodel/node_access.hpp>
#include <defrt/stdlib/libm.hpp>

#include "engine_internal.hpp"
#include "m2m/data/boolean.hpp"
#include "m2m/data/convert.hpp"
#include "m2m/datafiles.hpp"
#include "m2m/engine.hpp"
#include "m2m/engine_scene.hpp"
#include "m2m/mst/loader_l5.hpp"
#include "m2m/mst/writer.hpp"
#include "m2m/props.hpp"
#include "m2m/qt4path.hpp"

namespace m2m::mst {

bool booleanEditorFill(BooleanObjectEditor&, Project&, Operation&);
bool booleanEditorApply(BooleanObjectEditor&, Project&, Operation&, engine::FileEffects&);
void booleanSideFiles(Project&, Operation&, const data::BooleanData&, Ctx&, engine::FileEffects&);

using def::model::Cls_1401e7b10;
using def::model::DB_STEP_DATA;
using def::model::LLOBJ_DATA;
using engine::at;
using engine::FileEffects;
using engine::GuiModel;
using qt::L;

namespace {

[[noreturn]] void unsupported(const std::string& what)
{
    throw Error(M2M_E_UNSUPPORTED, "operation load: " + what);
}

// 0x1405ef2e0(self) / 0x14005f770(0x1405ef2b0(self)): the model of the class's current operation
GuiModel* curModel(OpClass& c, Project& p)
{
    return c.current ? engine::opModel(p, *c.current) : nullptr;
}

// 0x140834fa0 = 140160f10: object kind (+0xc) is not 7
bool kindNot7(const LLOBJ_DATA* o)
{
    return *at<std::int32_t>(o, 0xc) != 7;
}

// QFile::exists with the files this load has already removed or written (FileEffects) taken into
// account; the original removes and writes them on the spot.
bool existsFx(const String& path, const FileEffects& fx)
{
    for (const String& d : fx.deletes)
        if (d == path) return false;
    for (const FileEffects::Write& w : fx.writes)
        if (w.path == path) return true;
    return qt::fileExists(path);
}

// QFile::remove(path)
void removeFx(const String& path, FileEffects& fx)
{
    if (existsFx(path, fx)) fx.deletes.push_back(path);
}

// ---- SfOprBase setters on the current operation's sim control (spec_C section 2.1) -------------

// 0x1405ef6f0 / 0x1405ef750 (TRANS 2 / TRANS 1): L2's engine::setTrans2 / setTrans1

// @src 0x1405ef870  SC+0x16c (NSTEP), +0x164 (STPINC), +0x160 (NSTART), +0x1d0 (DTMAX 1)
void setSteps(OpClass& c, Project& p, std::int32_t nstep, std::int32_t stpinc, std::int32_t nstart,
              double dtmax)
{
    GuiModel* m = curModel(c, p);
    if (Cls_1401e7b10* sc = m ? m->simCtl() : nullptr) {
        sc->nstep = nstep;
        sc->stpinc = stpinc;
        sc->nstart = nstart;
        sc->dtmax1 = dtmax;
    }
}

// @src 0x1405ef900  SC+0x2e0 (ITRMTH) = v
void setItrmth(OpClass& c, Project& p, std::int32_t v)
{
    GuiModel* m = curModel(c, p);
    if (Cls_1401e7b10* sc = m ? m->simCtl() : nullptr) sc->itrmth = v;
}

// ---- SfOprBoolean -------------------------------------------------------------------------------

// @src 0x1411def50  vt+0xb8
void booleanPrologue(OpClass& c, Project& p, Operation& op)
{
    baseLoadPrologue(c, p, op);                           // 0x1405f1bb0
    engine::setTrans2(p, op, true);                       // 0x1405ef6f0
    engine::setTrans1(p, op, false);                      // 0x1405ef750
    setSteps(c, p, 0, 1, -1, 1.0);                         // 0x1421249e0 = 1.0
    setItrmth(c, p, 1);
    GuiModel* m = curModel(c, p);                          // 0x14005f770(0x1405ef2b0(self))
    if (Cls_1401e7b10* sc = m ? m->simCtl() : nullptr) sc->solmtd = 1;   // SC+0x2e4
}

// @src 0x140058aa0  kind of slot s's engine object and of its slot record
bool setSlotKind(Project& p, Operation& op, int s, std::int32_t kind)
{
    if (!engine::opModel(p, op)) return false;             // 0x14005f770
    LLOBJ_DATA* o = engine::slotObject(p, op, s);          // 0x140057690
    if (!o) return false;
    *at<std::int32_t>(o, 0xc) = kind;
    ObjEntry* e = objEntry(op, s);                         // 0x140061250
    if (!e) return false;
    e->objectType = kind;                                  // 0x14004d580: +4
    return true;
}

// @src 0x1411df020  vt+0xc0 (SfOprBase's 0x1405f1c00 is not called)
void booleanPostLoad(OpClass& c, Project& p, Operation&, FileEffects& fx)
{
    Operation* opi = c.current;                           // 0x1405ef2b0
    if (!opi) return;
    Props* pr = opi->props.get();                         // 0x140051230
    auto* bp = dynamic_cast<BooleanProps*>(pr);           // 0x14146f8fc (SfOprBooleanProperties)
    if (!bp || !bp->data) return;                         // 0x140025010: props+0x38
    data::BooleanData& bd = *bp->data;
    const int wp = bd.workpiece;                          // 0x14142ff20: +4
    (void)bd.cutter;                                      // 0x140cbc690: +8, result unused
    const int n = static_cast<int>(opi->objs.size());     // 0x140061220: OPI+0x50 count
    if (n == 0) {
        engine::addObject(p, *opi, fx);                   // 0x140057ba0 (MDT L7)
        setSlotKind(p, *opi, 0, 5);                       // 0x140058aa0
        return;
    }
    GuiModel* m = engine::opModel(p, *opi);               // 0x140057710
    LLOBJ_DATA* o = m ? m->object(wp) : nullptr;
    if (o && *at<std::int32_t>(o, 0xc) == 7) {
        for (int i = 0; i < n; ++i) {
            LLOBJ_DATA* oi = engine::slotObject(p, *opi, i);   // 0x140057690
            if (oi && *at<std::int32_t>(oi, 0xc) == 5) bd.workpiece = *at<std::int32_t>(oi, 8);   // 0x1413fd9d0
        }
    }
}

// ---- SfOprConvert2Dto3D -------------------------------------------------------------------------

// @src 0x1401c2970  vt+0xb8
void c23Prologue(OpClass& c, Project& p, Operation& op)
{
    baseLoadPrologue(c, p, op);                           // 0x1405f1bb0
    engine::setTrans2(p, op, true);                       // 0x1405ef6f0
    engine::setTrans1(p, op, true);                       // 0x1405ef750
}

// Common body of 0x1401c6110 and 0x1401c65f0: cleanPath(dir + sep + "%1%2.KEY".arg(stem).arg(no,
// 5, 10, '0')), dir = the current operation's task folder or "" (literals 0x1421ff716 / 0x1421ff717)
String classSlotFile(OpClass& c, const char* stem, int no)
{
    String dir = L("");
    if (Operation* cur = c.current) dir = taskDirAbs(*cur);   // 0x140050df0
    const String name = qt::arg(qt::arg(L("%1%2.KEY"), L(stem), 0, u' '), no, 5, 10, u'0');
    return qt::cleanPath(dir + qt::kSep + name);
}

// @src 0x1401c6110  MeshSettings2D%05d.KEY (mode 2) / MeshSettings3D%05d.KEY (other modes)
String meshSettingsFile(OpClass& c, int no, int mode)
{
    return classSlotFile(c, mode == 2 ? "MeshSettings2D" : "MeshSettings3D", no);
}

// @src 0x1401c65f0  GeoConvt23Data%05d.KEY (the mode argument is not used)
String geoConvtFile(OpClass& c, int no)
{
    return classSlotFile(c, "GeoConvt23Data", no);
}

// @src 0x1401c7370  the class's 3D work model (+0x138): a 2D one is dropped; a new one gets the
// operation's database and a copy of its sim control, TRANS 1 and 2 = 1
GuiModel* workModel(OpClass& c, Convert2Dto3DState& st, Project& p)
{
    if (st.work && st.work->step && *at<std::int32_t>(st.work->step.get(), 0x1c) == 2)   // 0x140c7c0e0
        st.work.reset();                                   // vt+0x18(1)
    if (!st.work) {
        st.work = std::make_shared<GuiModel>(nullptr, 3);  // 0x140c7a6b0(mem, 3): no database
        if (GuiModel* m = curModel(c, p)) {
            st.work->db = m->db;                           // 0x140c7aa80(work, 0x140c7ac30(m)), as L0's ctor
            Cls_1401e7b10* d = st.work->simCtl();
            Cls_1401e7b10* s = m->simCtl();
            if (d) {
                if (s) def::cmd::sim_ctrl_copy(d, s);      // 0x1408df7b0 = 1401f3660
                d->trans[1] = 1;                           // +0x108
                d->trans[0] = 1;                           // +0x104
            }
        }
    }
    return st.work.get();
}

// @src 0x1401c70d0  a node whose BC code (o+0x990, row width u15f8[0x12]) is 1 in some direction
// with |state variable 2| (1401809c0, row width u15f8[2]) <= 1e-6 in that direction
bool zeroOnFixedNode(DB_STEP_DATA* step, LLOBJ_DATA* o)
{
    if (step == nullptr) return false;
    const std::int32_t w = step->u15f8[0x12];             // +0x1640
    const std::int32_t w2 = step->u15f8[2];               // +0x1600
    const std::int32_t* bc = *at<std::int32_t*>(o, 0x990);
    for (std::int32_t i = 0; i < *at<std::int32_t>(o, 0x880); i = i + 1) {
        for (std::int32_t k = w - 1; -1 < k; k = k - 1) {
            if (bc[i * w + k] == 1) {
                const double* v = def::model::fun_1401809c0(at<def::model::Cls_14017ef20>(o, 0x8a0));
                if (def::rt::libm::ps_fabs(v[i * w2 + k]) <= 1e-6) return true;   // 0x141470a78, 0x142157420
            }
        }
    }
    return false;
}

// @src 0x1401c71f0  GEO23 revolve type (o+0x2c08): 0 when such a node exists, else 1
bool setRevolveType(DB_STEP_DATA* step, LLOBJ_DATA* o)
{
    if (step == nullptr || o == nullptr) return false;
    *at<std::int32_t>(o, 0x2c08) = zeroOnFixedNode(step, o) ? 0 : 1;
    return true;
}

// @src 0x1401c29d0  vt+0xc0
void c23PostLoad(OpClass& c, Convert2Dto3DState& st, const String& tempStep, Project& p, Operation& op,
                 Ctx& ctx, FileEffects& fx)
{
    basePostLoadBody(c, p, op, ctx, fx);                  // 0x1405f1c00
    String s40, s20;
    Operation* opi = c.current;                           // 0x1405ef2b0
    if (!opi) return;
    const String& s60 = tempStep;                         // 0x1401c6f20
    if (oprFlagOff(*opi)) removeFx(s60, fx);              // 0x1400509d0, QFile::remove
    GuiModel* m = engine::opModel(p, *opi);               // 0x14005f770
    if (!m) return;
    DB_STEP_DATA* step = m->step.get();                   // 0x140c7acd0
    if (!step) return;
    if (!engine::glScenePresent()) return;                // 0x1402c8170(APP)
    GuiModel* sc = workModel(c, st, p);                   // 0x1401c7370
    if (!sc) return;
    sc->viewerMirrorPlanes.clear();                       // 0x140c82c70: viewer items of the work model
    DB_STEP_DATA* sstep = sc->step.get();
    std::int32_t rc = 2;
    auto tmp = std::make_unique<DB_STEP_DATA>(3);         // 0x140821bf0 = 1401c8950
    Operation* prev = engine::prevOp(p, *opi);            // 0x140062960
    if (prev) {
        const String path = simCtrlPath(*prev);           // 0x140059290
        if (existsFx(path, fx)) {
            const char reset = 1;
            const std::int32_t mode = 0;
            const std::string lp = engine::engineKeyPath(path);
            rc = def::cmd::read_keyword_file(tmp.get(), lp.c_str(), &reset, &mode);   // 0x1408969a0
        }
    }
    if (prev) {
        Cls_1401e7b10* d = m->simCtl();
        Cls_1401e7b10* s = sc->simCtl();
        Cls_1401e7b10* t = &tmp->sim_ctrl;                // tmp+0x750
        if (d) {
            if (rc == 0) d->geotyp = t->geotyp;           // +0x100
            if (oprFlagOff(*opi) && t) d->pdie1 = t->pdie1;   // +0x168
        }
        if (s && rc == 0) s->geotyp = t->geotyp;
    }
    if (m && step) {
        if (existsFx(s60, fx)) {
            const char reset = 1;
            const std::int32_t mode = 0;
            const std::string ap = qt::toAscii(s60);      // QString::operator const char*
            engine::engineKeyPath(s60);                    // refuses a path the reader cannot open
            def::cmd::read_keyword_file(sstep, ap.c_str(), &reset, &mode);   // 0x1408969a0
        }
        const int n = m->objectCount();                   // 0x140c7c1a0
        for (int i = 0; i < n; ++i) {
            LLOBJ_DATA* o = m->object(i + 1);             // 0x140c7ae80
            if (!o || !kindNot7(o)) continue;
            *at<std::uint8_t>(o, 0x2c68) = 0;
            *at<double>(o, 0x2bf8) = 0.0;
            *at<double>(o, 0x2bf0) = 360.0;               // 0x1421249c8
            const int no = *at<std::int32_t>(o, 8);
            s40 = meshSettingsFile(c, no, 2);
            s20 = qt::cleanPath(s40);
            if (existsFx(s20, fx)) {
                const std::string lp = engine::engineKeyPath(s20);
                def::cmd::key_import_mesh_settings(step, lp.c_str(), no);   // 0x1408986a0 = 1401b7c00
            }
            s20 = qt::cleanPath(geoConvtFile(c, no));
            if (existsFx(s20, fx)) {
                const std::string lp = engine::engineKeyPath(s20);
                def::cmd::key_import_geo_convt23(step, lp.c_str(), no);     // 0x140898880 = 1401b7de0
            }
            *at<std::int32_t>(o, 0x2be8) = def::model::geo_kw::simctl_geo_kind(m->simCtl());   // 0x1408da610
            if (!existsFx(s20, fx) && *at<std::int32_t>(o, 0x2be8) == 1 && *at<std::int32_t>(o, 0x2d94) == 0)
                setRevolveType(step, o);
            LLOBJ_DATA* o2 = sc->objectWrap(no, false, true);   // 0x140c7c950(sc, no, 0, 1)
            if (o2) {
                def::model::llobj_copy::copy_u3aa0(o2, o);       // 0x140835b50 = 140161ac0
                def::model::llobj_copy::copy_u3b50(o2, o);       // 0x140835bf0 = 140161b60
                def::model::llobj_copy::copy_u3998(o2, o);       // 0x140835a50 = 1401619c0
                def::model::cls_1401889a0_assign(at<std::byte>(o2, 0x2be8), at<std::byte>(o, 0x2be8));   // 0x14085ceb0
                const char shapes = 0;
                const char byDim = 1;
                def::model::llobj_copy::meshgen_copy(at<std::byte>(o2, 0x2d10), at<std::byte>(o, 0x2d10), &shapes,
                                                     &byDim);    // 0x14085dd40
                *at<std::int32_t>(o2, 0x39c0) = 3;               // 0x1401cc360
                s40 = meshSettingsFile(c, no, 3);
                s20 = qt::cleanPath(s40);
                if (existsFx(s20, fx)) {
                    const std::string lp = engine::engineKeyPath(s20);
                    def::cmd::key_import_mesh_settings(sstep, lp.c_str(), no);
                }
                *at<std::int32_t>(o, 0x2d94) = *at<std::int32_t>(o2, 0x2d94);
            }
            int idx = no - 1;
            const int ns = objSlots(*opi);                // 0x14005fa80
            for (int j = 0; j < ns; ++j) {
                if (no == engine::resolvedCode(p, *opi, j)) {   // 0x14005fd20
                    idx = j;
                    break;
                }
            }
            if (*at<std::int32_t>(o, 0xc) == 0 && !engine::objHasMesh(o)) {   // 0x140834c80
                *at<std::uint8_t>(o, 0x2c20) = 0;
                if (o2) *at<std::uint8_t>(o2, 0x2c20) = 0;
                continue;
            }
            if (*at<std::int32_t>(o, 0xc) == 5) {
                const int code = engine::resolvedCode(p, *opi, idx);
                Item* rec = objItem(*opi, code);           // 0x140061360: SfMoObjItem+0x40 record
                if (rec && rec->recF08 == 0 && !rec->hasMesh) {   // 0x14004d660 (+8), 0x14004d800 (+0x19)
                    *at<std::uint8_t>(o, 0x2c20) = 0;
                    if (o2) *at<std::uint8_t>(o2, 0x2c20) = 0;
                }
            }
        }
        if (sstep) sstep->u1b4c = 1;                      // 0x140c7b1f0(sc, 1, 0): 0x140823c80; no viewer
    }
}

// fopen(path, "w"/"wt") of an engine writer: fails when the folder does not exist (nor is made by
// this save, FileEffects::mkdirs), as the original's does for a task folder made only at Save
bool folderExistsFx(const String& file, const FileEffects& fx)
{
    const String dir = qt::cleanPath(qt::absolutePath(file));
    for (const String& d : fx.mkdirs)
        if (qt::cleanPath(d) == dir) return true;
    std::error_code ec;
    return std::filesystem::is_directory(std::filesystem::path(dir.u16()), ec);
}

// A writer given a scratch file in place of `path`; its bytes are recorded for `path` (a removal of
// `path` earlier in this run is undone). Returns the writer's status, 1 when the open would fail.
int captureFx(const String& path, FileEffects& fx, const std::function<int(const char*)>& write)
{
    if (!folderExistsFx(path, fx)) return 1;
    const std::filesystem::path tmp = std::filesystem::temp_directory_path() /
                                      ("m2m_l5_" + std::to_string(GetCurrentProcessId()) + "_" +
                                       std::to_string(GetCurrentThreadId()) + ".KEY");
    std::error_code ec;
    std::filesystem::remove(tmp, ec);
    const int rc = write(tmp.string().c_str());
    if (std::filesystem::exists(tmp, ec)) {
        std::ifstream in(tmp, std::ios::binary);
        std::string bytes((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
        in.close();
        std::filesystem::remove(tmp, ec);
        std::erase(fx.deletes, path);
        fx.writes.push_back({path, std::move(bytes)});
    }
    return rc;
}

// @src 0x140c7d100  write the model as a KEY file (0x140895350 = 1401c5810, flags 0); true when 0
bool writeModelKey(GuiModel& w, const String& path, FileEffects& fx)
{
    if (!w.step) return false;
    const std::uint32_t flags = 0;
    const int rc = captureFx(path, fx, [&](const char* f) { return def::cmd::write_key_file(w.step.get(), f, &flags); });
    return rc == 0;
}

// @src 0x140c7d020  read a KEY file into the model: the bytes this load wrote to `path` (FileEffects)
// are read from a scratch copy, as the original reads the file it just wrote; false when the file
// is not there (the original's fopen fails)
bool readModelKeyFx(GuiModel& m, const String& path, const FileEffects& fx)
{
    if (!existsFx(path, fx)) return false;
    const FileEffects::Write* last = nullptr;
    for (const FileEffects::Write& w : fx.writes)
        if (w.path == path) last = &w;
    if (!last) return m.readKey(path);
    std::error_code ec;
    const std::filesystem::path tmp =
        std::filesystem::temp_directory_path(ec) / ("m2m_l5_" + std::to_string(reinterpret_cast<std::uintptr_t>(&m)) + ".KEY");
    {
        std::ofstream out(tmp, std::ios::binary);
        out << last->bytes;
    }
    const bool ok = m.readKey(String(tmp.u16string()));
    std::filesystem::remove(tmp, ec);
    return ok;
}

// ---- SfConv23Config, the "Configuration" item's editor ------------------------------------------

// QLineEdit::setText then on_<edit>_textChanged: the slot reads the text back with toDouble
double editRoundTrip(String& text, double v)
{
    text = qt::number(v, 'g', 6);                         // QString::number(d, 'g', 6)
    return qt::toDouble(text);
}

// @src 0x1401e1c10  editor init for object objNo (ctor 0x1401e1a80 runs setupUi 0x1401e4120 first:
// "Z is up" checked, "Y is up" not, empty line edits). The display's second model gets the object
// (below); 0x140d011d0 and 0x140cfe9c0 at the end are the display's redraw and fit (viewer state).
void configEditorInit(OpClass& c, Convert2Dto3DState& st, Project& p)
{
    Conv23ConfigEditor& ed = st.editor;
    ed = Conv23ConfigEditor{};
    ed.shown = true;
    Operation* opi = c.current;                           // 0x140130af0
    GuiModel* m = opi ? engine::opModel(p, *opi) : nullptr;
    LLOBJ_DATA* o = m ? m->object(ed.objNo) : nullptr;    // 0x1401e2e90
    if (!o || !kindNot7(o)) return;                       // setEnabled(false)
    ed.hasObject = true;
    // the display's second model (0x1402c81b0): its object of the same number, made from the
    // operation's object when missing; the model's u1b4c = the object number (0x140c7b1f0, no redraw)
    GuiModel* s2 = engine::displaySecond(engine::projectDisplay(p));
    if (!s2) unsupported("Convert2Dto3D editor: no second model in the display (0x140c7ae80 dereferences it)");
    LLOBJ_DATA* o2 = s2->object(ed.objNo);                // 0x140c7ae80
    if (!o2) {
        o2 = s2->objectWrap(ed.objNo, false, true);       // 0x140c7c950(s2, no, 0, 1)
        def::model::llobj_copy::copy_u3aa0(o2, o);        // 0x140835b50 = 140161ac0
        def::model::llobj_copy::copy_u3b50(o2, o);        // 0x140835bf0 = 140161b60
        def::model::llobj_copy::copy_u3998(o2, o);        // 0x140835a50 = 1401619c0
        def::model::cls_1401889a0_assign(at<std::byte>(o2, 0x2be8), at<std::byte>(o, 0x2be8));   // 0x14085ceb0
        const char shapes = 0;
        const char byDim = 1;
        def::model::llobj_copy::meshgen_copy(at<std::byte>(o2, 0x2d10), at<std::byte>(o, 0x2d10), &shapes,
                                             &byDim);     // 0x14085dd40
        *at<std::int32_t>(o2, 0x39c0) = 3;                // 0x1401cc360
        engine::viewerSync(*s2, o2, true);                // 0x140c80280(s2, o2, 1)
    }
    if (s2->step) s2->step->u1b4c = ed.objNo;             // 0x140c7b1f0(s2, no, 0): 0x140823c80
    DB_STEP_DATA* step = m->step.get();                   // 0x140c7acd0
    if (step) {
        const std::int32_t gk = def::model::geo_kw::simctl_geo_kind(&step->sim_ctrl);   // 0x1408da610
        if (gk != *at<std::int32_t>(o, 0x2be8)) *at<std::int32_t>(o, 0x2be8) = gk;
        const std::int32_t mode = *at<std::int32_t>(o, 0x2be8);
        if (mode != 1 && mode != 2) *at<std::int32_t>(o, 0x2be8) = def::model::geo_kw::simctl_geo_kind(m->simCtl());
        if (*at<std::int32_t>(o, 0x2be8) == 1) {
            *at<double>(o, 0x2bf0) = editRoundTrip(ed.revAngle, *at<double>(o, 0x2bf0));   // ui+0xc0, 0x1401e3320
            *at<double>(o, 0x2bf8) = editRoundTrip(ed.revStart, *at<double>(o, 0x2bf8));   // ui+0xb0, 0x1401e33b0
            ed.revTol = qt::number(*at<double>(o, 0x2c00), 'g', 6);   // ui+0x70, slot 0x1401e3440 empty
            // qrbSolid / qrbHollow by +0x2c08: their slots 0x1401e3300 / 0x1401e3310 are empty
        } else if (*at<std::int32_t>(o, 0x2be8) == 2) {
            *at<double>(o, 0x2c10) = editRoundTrip(ed.extLen, *at<double>(o, 0x2c10));     // ui+0xe8, 0x1401e3470
            *at<double>(o, 0x2c18) = editRoundTrip(ed.extStart, *at<double>(o, 0x2c18));   // ui+0xf8, 0x1401e3500
        }
        if (*at<std::int32_t>(o, 0x2be8) == 1 || *at<std::int32_t>(o, 0x2be8) == 2) {
            ed.yup = *at<std::int32_t>(o, 0x2c38) == 2;   // setChecked(ui+0x20, ...)
            if (ed.yup) {
                // toggled: on_Yup_toggled(true) / on_Zup_toggled(false), 0x1401e3280 / 0x1401e3200
                std::int32_t axis = 2;
                def::cmd::cnvt3d::wpaxis_set_axis(at<def::model::linked::Cls_140258580List>(o, 0x20a0), &axis);
                *at<std::int32_t>(o, 0x2c38) = axis;
            }
        } else {
            unsupported("Convert2Dto3D editor: GEO23 mode not 1 or 2 after the init (the literal texts of "
                        "0x1401e1c10 are not ported)");
        }
        // the object's geometry check (0x1408374f0, 0x140834d50) only sets widget states (an
        // information box for a die without geometry is UI)
    }
    ed.geo23.assign(at<std::uint8_t>(o, 0x2be8), at<std::uint8_t>(o, 0x2be8) + 0x128);   // 0x14085ceb0(+0x50)
}

// @src 0x14085d170  GEO23 blocks differ (mode, axis +0x50, mode's values, +0x39/+0x3c, +0x38/+0x40/+4)
bool geo23Differ(const std::uint8_t* a, const std::uint8_t* b)
{
    auto i32 = [](const std::uint8_t* q, int off) { return *reinterpret_cast<const std::int32_t*>(q + off); };
    auto f64 = [](const std::uint8_t* q, int off) { return *reinterpret_cast<const double*>(q + off); };
    if (i32(a, 0) != i32(b, 0)) return true;
    if (i32(a, 0x50) != i32(b, 0x50)) return true;
    if (i32(a, 0) == 1) {
        if (f64(a, 0x8) != f64(b, 0x8) || f64(a, 0x10) != f64(b, 0x10) || f64(a, 0x18) != f64(b, 0x18)) return true;
    } else if (i32(a, 0) == 2) {
        if (f64(a, 0x28) != f64(b, 0x28) || f64(a, 0x30) != f64(b, 0x30)) return true;
    }
    if (a[0x39] != b[0x39]) return true;
    if (a[0x39] != 0 && i32(a, 0x3c) != i32(b, 0x3c)) return true;
    if (a[0x38] != b[0x38]) return true;
    if (a[0x38] != 0 && (i32(a, 0x40) != i32(b, 0x40) || i32(a, 4) != i32(b, 4))) return true;
    return false;
}

// @src 0x1401c7560  the work model's objects (all, or number no) lose their surfaces and mesh; the
// scene's second model likewise (viewer); TEMP_STEP.KEY removed, rewritten from the work model unless
// removeOnly
void clearModelObjects(GuiModel& w, int no)
{
    for (int i = 0; i < w.objectCount(); ++i) {           // 0x140c7c1a0
        LLOBJ_DATA* o = w.objectAt(i + 1);                // 0x140c7aed0
        if (o && (no < 1 || *at<std::int32_t>(o, 8) == no)) {
            def::cmd::objpos::fun_1401ffed0(o);           // 0x140873d30 = 1401ffed0
            engine::objDeleteMesh(o);                     // 0x140873be0
        }
    }
}

void clearWorkObjects(Convert2Dto3DState& st, Project& p, int no, bool removeOnly, const String& tempStep,
                      FileEffects& fx)
{
    if (st.work) clearModelObjects(*st.work, no);
    if (GuiModel* s2 = engine::displaySecond(engine::projectDisplay(p)))   // 0x1402c81b0(APP)
        clearModelObjects(*s2, no);
    removeFx(tempStep, fx);                               // 0x1401c6f20, QFile::remove
    if (!removeOnly && st.work) writeModelKey(*st.work, tempStep, fx);   // 0x140c7d100
}

// @src 0x1401e2ef0  editor apply: axis from "Y is up" (2) / "Z is up" (3), GEO23 mode and the
// line-edit values into every object with kind != 7 and a GEO23 mode
void configEditorApply(OpClass& c, Convert2Dto3DState& st, Project& p)
{
    const Conv23ConfigEditor& ed = st.editor;
    Operation* opi = c.current;                           // 0x140130af0
    GuiModel* m = opi ? engine::opModel(p, *opi) : nullptr;
    if (!m) return;
    std::int32_t axis = ed.yup ? 2 : 3;
    const int n = m->objectCount();
    for (int i = 0; i < n; ++i) {
        LLOBJ_DATA* o = m->object(i + 1);
        if (!o || !kindNot7(o) || !def::cmd::cnvt3d::geo23_active(at<std::int32_t>(o, 0x2be8))) continue;
        def::cmd::cnvt3d::wpaxis_set_axis(at<def::model::linked::Cls_140258580List>(o, 0x20a0), &axis);   // 0x1408ea190
        *at<std::int32_t>(o, 0x2c38) = axis;
        if (m->simCtl()) *at<std::int32_t>(o, 0x2be8) = def::model::geo_kw::simctl_geo_kind(m->simCtl());
        if (*at<std::int32_t>(o, 0x2be8) == 1) {
            *at<double>(o, 0x2bf0) = qt::toDouble(ed.revAngle);   // ui+0xc0
            *at<double>(o, 0x2bf8) = qt::toDouble(ed.revStart);   // ui+0xb0
        } else {
            *at<double>(o, 0x2c10) = qt::toDouble(ed.extLen);     // ui+0xe8
            *at<double>(o, 0x2c18) = qt::toDouble(ed.extStart);   // ui+0xf8
        }
    }
}

// @src 0x1401e2d70  SfConv23Config vt+0x1d0 (signalUpdateDataInWidgetArea): apply, the work model
// cleared when the object's GEO23 changed since the init, then the operation's props vt+0x58
void c23UpdateWidget(OpClass& c, Convert2Dto3DState& st, const String& tempStep, Project& p, Operation&,
                     FileEffects& fx)
{
    configEditorApply(c, st, p);
    Operation* opi = c.current;                           // 0x140130af0
    if (!opi) return;
    GuiModel* m = engine::opModel(p, *opi);
    LLOBJ_DATA* o = (st.editor.hasObject && m) ? m->object(st.editor.objNo) : nullptr;   // +0x48
    if (o && geo23Differ(st.editor.geo23.data(), at<std::uint8_t>(o, 0x2be8)))
        clearWorkObjects(st, p, -1, true, tempStep, fx);     // 0x140054e90 class, 0x1401c7560(cls, -1, 1)
    // props (0x140051230) as SfOprConvert2Dto3DProperties (0x1435d68a8): vt+0x58
    c.propsLoad(p, *opi);
}

// @src 0x1401c3a60  vt+0x58
void c23Load(OpClass& c, Convert2Dto3DState& st, const String& tempStep, Project& p, Operation& op, Ctx& ctx,
             FileEffects& fx)
{
    st.geometryTypeShown = false;
    st.editor.shown = false;
    st.loading = 1;                                       // +0x130
    engine::baseLoad(p, op, ctx, fx);                     // 0x1405ef430
    st.loading = 0;
    Operation* opi = c.current;
    if (!opi) return;
    GuiModel* m = engine::opModel(p, *opi);
    if (!m) return;
    // 0x1402c8170(APP): the GL scene (present, see c23PostLoad); 0x1402c81b0: its second model,
    // made from the main model the views step showed (0x140054820)
    if (!engine::glScenePresent()) return;                // 0x1402c8170(APP)
    engine::Display& disp = engine::projectDisplay(p);
    GuiModel* s2 = engine::displaySecond(disp);           // 0x1402c81b0
    if (st.work) {
        writeModelKey(*st.work, tempStep, fx);            // 0x1401c6f20, 0x140c7d100
        if (!s2) unsupported("Convert2Dto3D load: no second model in the display (0x140c7b720 dereferences it)");
        s2->reset();                                      // 0x140c7b720
        s2->db = m->db;                                   // 0x140c7aa80(s2, 0x140c7ac30(m))
        readModelKeyFx(*s2, tempStep, fx);                // 0x140c7d020
    }
    engine::displayModels(disp).keepSecond = true;        // 0x140cf9cd0(scene+0x10, 1)
    // 0x1401c5000: a chain head selects Geometry Type; other operations select Configuration.
    // Factory 0x1401cca90 delegates Geometry Type to 0x1405ff0b0 -> 0x1410580a0.
    if (isChainHead(op)) {
        c.propsToEngine(p, op);                           // 0x1405f36d0 -> 0x1400543a0
        if (engine::guiModelDim(*m) != 2)
            unsupported("Convert2Dto3D Geometry Type editor: non-2D model");
        st.geometryTypeShown = true;
        st.geometryType = 1;                            // setupUi checks Axisymmetric
        if (auto* sc = m->simCtl(); sc && sc->geotyp >= 1 && sc->geotyp <= 4)
            st.geometryType = sc->geotyp;               // 0x141058370; unknown keeps default
        return;
    }
    // For a non-head operation the GUI shows "Configuration" (oracle B 97
    // opened.png); SfOprConvert2Dto3DFactory vt+0x8 0x1401cca90 makes its editor SfConv23Config.
    // The tree item handler OB vt+0x158 0x1405f36d0 with no editor shown (property-stack index 0)
    // runs props vt+0x60 (0x1400543a0) before the factory: the up axis read by the load's props
    // vt+0x58 goes back into WPAXIS and +0x2c38 over the GeoConvt23Data import (oracle B 117 / 118:
    // CRDSYS 2 in the file, "Z is up" after Open). The editor's vt+0x1d0 runs only when the widget
    // area leaves, deletes or replaces it (0x140dae3c0 / 0x140dae450 / scroll-area show 0x140cda320),
    // never at Save (vt+0xd0: signalUpdateDataInWidgetArea -> vt+0x1c0 0x1401e2a10, empty); without
    // it 97 and 117 save axis 0, so the first editor is replaced before Save. That replacement (a
    // second show of the item) is inferred from oracle B 97 / 114 / 117 / 118, not traced through
    // the Qt signals (tree rebuild 0x140d3d730, SfNotifier signalRefreshTree 0x1402ceae0 ->
    // SfTreeView::slotRefresh 0x140d3aa60).
    c.propsToEngine(p, op);                               // 0x1405f36d0 -> 0x1400543a0
    configEditorInit(c, st, p);
    c23UpdateWidget(c, st, tempStep, p, op, fx);
    configEditorInit(c, st, p);
}

// 0x140dae340 -> Geometry Type vt+0x1c0 0x141058600 -> 0x141058670.
// The extra property setter after values 1/2 requires an Express operation;
// SfOprConvert2Dto3D does not satisfy that original dynamic_cast.
bool c23UpdateData(Convert2Dto3DState& st, Project& p, Operation& op)
{
    if (!st.geometryTypeShown) return st.editor.shown;    // Configuration's apply is empty
    if (auto* m = engine::opModel(p, op); m && engine::guiModelDim(*m) == 2) {
        if (auto* sc = m->simCtl()) sc->geotyp = st.geometryType;
    }
    return true;
}

// @src 0x1401ce3e0  properties vt+0x58: data block from the first object with a GEO23 mode
bool c23PropsLoad(Project& p, Operation& op, data::ConvertData& d)
{
    GuiModel* m = engine::opModel(p, op);                 // 0x1405f8d90 (props+0x20 = op), 0x14005f770
    if (!m) return false;
    d.conType = def::model::geo_kw::simctl_geo_kind(m->simCtl());   // 0x1408da610(0x140c7b2d0(m))
    const int n = m->objectCount();
    for (int i = 0; i < n; ++i) {
        LLOBJ_DATA* o = m->object(i + 1);
        if (o && kindNot7(o) && def::cmd::cnvt3d::geo23_active(at<std::int32_t>(o, 0x2be8))) {   // 0x14085d330
            d.upAxis = def::cmd::cnvt3d::wpaxis_axis(
                at<def::model::linked::Cls_140258580List>(o, 0x20a0));   // 0x1408ea140 = 14025a360
            d.revType = *at<std::int32_t>(o, 0x2c08);
            d.revAngle = *at<double>(o, 0x2bf0);
            d.revStartAngle = *at<double>(o, 0x2bf8);
            d.revTol = *at<double>(o, 0x2c00);
            d.extLen = *at<double>(o, 0x2c10);
            return true;
        }
    }
    return false;
}

void c23PropsLoadSlot(Project& p, Operation& op, data::ConvertData& d)
{
    c23PropsLoad(p, op, d);
}

// ---- SfOprCopyMirror ----------------------------------------------------------------------------

// @src 0x140e24b80  vt+0x58
void cmLoad(OpClass&, Project& p, Operation& op, Ctx& ctx, FileEffects& fx)
{
    engine::baseLoad(p, op, ctx, fx);                     // 0x1405ef430
    if (!engine::glScenePresent()) return;                // 0x1402c8170(APP)
    engine::Display& disp = engine::projectDisplay(p);
    engine::DisplayModels& dm = engine::displayModels(disp);
    GuiModel* s2 = engine::displaySecond(disp);           // 0x1402c81b0
    if (s2 && engine::guiModelDim(*s2) == 2) {            // 0x140c7c0e0
        dm.keepSecond = false;                            // 0x140cf9cd0(scene+0x10, 0)
        engine::displayDropSecond(disp);                  // 0x140cf9bd0
        s2 = nullptr;
    }
    if (!s2) {
        engine::displaySetSecond(disp, std::make_unique<GuiModel>(nullptr, 3));   // new 0x278, 0x140c7a6b0(mem, 3); 0x140cf9930
        s2 = dm.second.get();
    }
    dm.keepSecond = true;                                 // 0x140cf9cd0(scene+0x10, 1)
    engine::viewerClear(*s2);                             // 0x140c82c70
}

// @src 0x140e22900  vt+0xb8
void cmPrologue(OpClass& c, Project& p, Operation& op)
{
    baseLoadPrologue(c, p, op);                           // 0x1405f1bb0
    engine::setTrans2(p, op, true);                       // 0x1405ef6f0
    engine::setTrans1(p, op, true);                       // 0x1405ef750
}

// QChar::isSpace (Qt 4.8, Unicode 5.0 tables): 9..13, space, 0x85, 0xa0 and the Zs/Zl/Zp separators
bool isSpace(char16_t ch)
{
    if (ch >= 9 && ch <= 13) return true;
    switch (ch) {
    case 0x20: case 0x85: case 0xa0: case 0x1680: case 0x180e: case 0x2028: case 0x2029: case 0x202f:
    case 0x205f: case 0x3000:
        return true;
    default:
        return ch >= 0x2000 && ch <= 0x200a;
    }
}

// QString::section(QRegExp("\\s+"), k, k) with SectionDefault: the text between whitespace runs;
// a null string past the last section
String sectionWs(const String& line, int k)
{
    const std::u16string& u = line.u16();
    int idx = 0;
    std::size_t start = 0;
    std::size_t i = 0;
    while (true) {
        while (i < u.size() && !isSpace(u[i])) ++i;
        if (idx == k) return String(u.substr(start, i - start));
        if (i >= u.size()) return String();
        while (i < u.size() && isSpace(u[i])) ++i;
        start = i;
        ++idx;
    }
}

// QString::section(QString(","), k, k) with SectionDefault
String sectionSep(const String& line, const String& sep, int k)
{
    const std::vector<String> parts = qt::split(line, sep, qt::SplitBehavior::KeepEmptyParts);
    if (k < static_cast<int>(parts.size())) return parts[static_cast<std::size_t>(k)];
    return String();
}

// QTextStream over a QFile opened ReadOnly | Text: Text mode drops every '\r' (QIODevice::read),
// the codec is the locale's (system ANSI); readLine splits at '\n'
bool readTextLines(const Project& p, const String& path, std::vector<String>& lines)
{
    std::string bytes;
    if (!datafiles::readFileBytes(p, path, bytes)) return false;
    if ((bytes.size() >= 2 && ((bytes[0] == '\xff' && bytes[1] == '\xfe') || (bytes[0] == '\xfe' && bytes[1] == '\xff'))) ||
        (bytes.size() >= 3 && bytes.compare(0, 3, "\xef\xbb\xbf") == 0))
        unsupported(path.utf8() + " starts with a Unicode byte-order mark (QTextStream's codec detection is not "
                                  "reproduced)");
    std::string t;
    t.reserve(bytes.size());
    for (char ch : bytes)
        if (ch != '\r') t += ch;
    std::u16string u;
    if (!t.empty()) {
        const int n = MultiByteToWideChar(CP_ACP, 0, t.data(), static_cast<int>(t.size()), nullptr, 0);
        u.resize(static_cast<std::size_t>(n));
        MultiByteToWideChar(CP_ACP, 0, t.data(), static_cast<int>(t.size()), reinterpret_cast<wchar_t*>(u.data()), n);
    }
    std::size_t s = 0;
    while (s < u.size()) {
        std::size_t e = u.find(u'\n', s);
        if (e == std::u16string::npos) e = u.size();
        lines.push_back(String(u.substr(s, e - s)));
        s = e + 1;
    }
    return true;
}

// @src 0x140e22950  read mirrormerge.ini into the class: KFREAD n (then the next line is kept),
// TOLERANCE x, MIRROR_MERGE <object> <planes> followed by one line per plane (whitespace sections
// 1 and 2 of the MIRROR_MERGE line, 0..9 of a plane line)
bool readMirrorMerge(CopyMirrorState& st, const Project& p, const String& file)
{
    st.entries.clear();                                   // 0x140e2a410(self+0x158)
    st.tolerance = 0.0;                                   // +0x148
    const String sep = L(",");                            // QString(QChar(','))
    std::vector<String> lines;
    if (!readTextLines(p, file, lines)) return false;     // QFile::open(ReadOnly | Text)
    MirrorEntry cur;                                      // 0x140e29e70
    int nPlanes = 0;                                      // [rsp+0x394]
    int kfread = 0;                                       // [rsp+0x398]
    for (const String& line : lines) {
        // the trimmed() copy (QString::trimmed) is made and dropped: the line stays untrimmed
        if (qt::startsWith(line, L("!"))) continue;
        String key, val;
        if (!qt::contains(line, sep)) {
            key = sectionWs(line, 0);
            val = sectionWs(line, 1);
        } else {
            key = sectionSep(line, sep, 0);
            val = sectionSep(line, sep, 1);
        }
        if (key == L("KFREAD")) {                         // 0x142abf714
            kfread = qt::toInt(val);
        } else if (key == L("TOLERANCE")) {               // 0x142abf720
            st.tolerance = qt::toDouble(val);
        } else if (key == L("MIRROR_MERGE")) {            // 0x142abf730
            String a[3];
            for (int k = 0; k < 3; ++k) a[k] = sectionWs(line, k);
            cur.objNo = qt::toInt(a[1]);
            nPlanes = qt::toInt(a[2]);
        } else {
            if (0 < kfread) {
                st.kfread = line;                         // +0x150
                kfread = 0;
            }
            if (0 < nPlanes) {
                String a[10];
                for (int k = 0; k < 10; ++k) a[k] = sectionWs(line, k);
                MirrorPlane r;
                r.a = qt::toInt(a[0]);
                r.b = qt::toInt(a[1]);
                r.c = qt::toInt(a[2]);
                for (int k = 0; k < 6; ++k) r.v[k] = static_cast<float>(qt::toDouble(a[3 + k]));
                r.flag = qt::toInt(a[9]);
                cur.planes.push_back(r);                  // 0x140e2a030
                nPlanes = nPlanes - 1;
                if (nPlanes == 0) {
                    st.entries.push_back(cur);            // 0x140e2a460
                    cur.planes.clear();                   // 0x140e29fe0
                }
            }
        }
    }
    return true;
}

// 0x140c82d50(m, &no): viewer item no, made when missing (null for no < 1); only its plane count
int* viewerItemPlanes(GuiModel& m, int no)
{
    if (no < 1) return nullptr;
    if (m.viewerMirrorPlanes.size() < static_cast<std::size_t>(no))
        m.viewerMirrorPlanes.resize(static_cast<std::size_t>(no), 0);   // 0x1411f07d0: count 0 (0x141249140)
    return &m.viewerMirrorPlanes[static_cast<std::size_t>(no - 1)];
}

// @src 0x140e23490  mirror planes of mirrormerge.ini onto the viewer items of the current model
void loadMirrorPlanes(OpClass& c, CopyMirrorState& st, Project& p)
{
    Operation* opi = c.current;
    if (!opi) return;
    GuiModel* m = engine::opModel(p, *opi);
    if (!m) return;
    // cleanDirPath(OPI+0x130 + sep + "mirrormerge.ini") is relative: QFile resolves it against the
    // current directory, the project folder after File > Open
    const String rel = qt::cleanPath(taskDirRel(*opi) + qt::kSep + L("mirrormerge.ini"));   // 0x142abf740
    String latin;                                         // toLatin1 -> QString(const char*)
    for (char16_t ch : rel.u16()) latin += ch > 0xff ? u'?' : ch;
    readMirrorMerge(st, p, qt::cleanPath(projectDir(p) + qt::kSep + latin));
    for (const MirrorEntry& e : st.entries) {
        int* planes = viewerItemPlanes(*m, e.objNo);
        if (!planes) return;
        // per plane 0x140e23a00 builds the plane on the item (viewer geometry 0x1411f1a10,
        // 0x141249730 / 0x141249f80), then 0x14004e810 / 0x141247ed0 set its record: viewer code,
        // not ported; the count records that planes were asked for
        *planes += static_cast<int>(e.planes.size());
    }
}

// @src 0x140e24b50  vt+0xc0
void cmPostLoad(OpClass& c, CopyMirrorState& st, Project& p, Operation& op, Ctx& ctx, FileEffects& fx)
{
    basePostLoadBody(c, p, op, ctx, fx);                  // 0x1405f1c00
    loadMirrorPlanes(c, st, p);                           // 0x140e23490
}

// @src 0x140e2d080  properties vt+0x58: the same body as 0x1401ce3e0
void cmPropsLoad(Project& p, Operation& op, data::ConvertData& d)
{
    c23PropsLoad(p, op, d);
}

// @src 0x140e25b20  mirror the provided object when the viewer item has planes
bool mirrorObject(OpClass& c, Project& p, int no, LLOBJ_DATA* o)
{
    Operation* cur = c.current;
    if (!cur) return false;
    GuiModel* m = engine::opModel(p, *cur);
    if (!m) unsupported("CopyMirror object without an engine model (0x140e25b20)");
    int* planes = viewerItemPlanes(*m, no);               // 0x140c82d50
    if (!kindNot7(o)) return false;
    if (!planes) unsupported("CopyMirror object number < 1 (0x140c82d50 gives no viewer item)");
    if (0 < *planes && engine::objHasGeometry(o))         // 0x141249420(item+0x348), 0x140834d50
        unsupported("CopyMirror mirror construction (0x140e25c00 on the viewer item's planes, packet V)");
    return true;
}

// @src 0x140e25980  vt+0x2a0: object `no` of the current CopyMirror operation, as geometry, copied
// into dst
bool cmProvideObject(OpClass& c, CopyMirrorState&, Project& p, int no, LLOBJ_DATA* dst)
{
    Operation* cur = c.current;                           // 0x1405ef2b0
    if (!cur) return false;
    GuiModel* m = engine::opModel(p, *cur);               // 0x14005f770
    if (!m) unsupported("CopyMirror operation without an engine model (0x140e25980)");
    LLOBJ_DATA* o = engine::loadSlotBare(p, *cur, *m, no - 1);   // 0x14005af80
    bool ok = false;
    if (o) {
        if (engine::objHasGeometry(o)) {                  // 0x140834d50
            if (engine::objHasMesh(o)) engine::objDeleteMesh(o);   // 0x140834c80, 0x140873be0
            ok = true;
        } else if (engine::objHasMesh(o)) {
            char compact = 1;
            engine::objGeometryFromMesh(o, &compact);     // 0x140870590
            engine::objDeleteMesh(o);
            ok = true;
        }
    }
    if (!ok) return false;
    mirrorObject(c, p, no, o);                            // 0x140e25b20
    DB_STEP_DATA* owner = *at<DB_STEP_DATA*>(o, 0x3a98);  // 0x1400c35b0
    const bool p4 = true, p5 = true, p6 = true, p7 = true, p8 = false, p9 = true;
    def::model::llobj_copy_from(dst, o, owner->u15f8, &p4, &p5, &p6, &p7, &p8, &p9);   // 0x140835430 = 1401613a0
    return true;
}

// ---- save current operation (MDT L5S) ------------------------------------------------------------

// 0x140dbbb10(o + 0x2d50): number of mesh windows (+0x14 of the window list)
std::int32_t windowCount(const LLOBJ_DATA* o)
{
    return *at<std::int32_t>(o, 0x2d50 + 0x14);
}

// 0x1401c5fa0 / 0x14095cd90: the work object's mesh windows from the 2D object's (mesh generator
// classes 0x140953560 .. 0x14098fc30, DEF_PRE 3D window construction): not ported
[[noreturn]] void refuseWindowRebuild(int no)
{
    unsupported("Convert2Dto3D save: object " + std::to_string(no) +
                " needs its 3D mesh windows built from the 2D ones (0x14095cd90 / 0x1401c5fa0)");
}

// @src 0x1401c5750  M23_INPUT.KEY: GEO23 blocks of the objects to convert, their 3D mesh settings,
// the TEMP_STEP.KEY reference and the CNVT3D list; CONV_STATUS.DAT is removed after it
void writeM23Input(OpClass& c, Convert2Dto3DState& st, const String& m23, const String& tempStep,
                   const String& convStatus, Project& p, FileEffects& fx)
{
    Operation* opi = c.current;                           // 0x1405ef2b0
    if (!opi) return;
    GuiModel* m = engine::opModel(p, *opi);               // 0x14005f770
    if (!m) return;
    DB_STEP_DATA* step = m->step.get();                   // 0x140c7acd0
    if (!step) return;
    // 0x1402c8170(APP): the main window's GL scene, present (see c23PostLoad)
    GuiModel* sc = workModel(c, st, p);                   // 0x1401c7370
    DB_STEP_DATA* sstep = sc ? sc->step.get() : nullptr;
    const std::int32_t n = step->u1378;
    if (n <= 0) return;
    captureFx(m23, fx, [&](const char* file) {
        std::FILE* f = std::fopen(file, "w");             // 0x14146fcd8, "w" 0x1421ffb14
        if (!f) return 1;
        std::string list;                                 // [rsp+0x170], strcat 0x14146fe20
        bool any = false;
        std::int32_t count = 0;
        char buf[0x400];
        auto put = [&](const char* s) { std::fwrite(s, 1, std::strlen(s), f); };   // 0x141471ad8
        for (std::int32_t i = 0; i < n; ++i) {
            LLOBJ_DATA* o = m->object(i + 1);             // 0x140c7ae80
            if (!o) unsupported("Convert2Dto3D save: object " + std::to_string(i + 1) +
                                " missing (the original dereferences it, 0x1401c5750)");
            if (!kindNot7(o)) continue;                   // 0x140834fa0
            if (*at<std::uint8_t>(o, 0x3610) != 0) continue;   // 0x1408350f0
            if (!def::cmd::cnvt3d::geo23_active(at<std::int32_t>(o, 0x2be8))) continue;   // 0x14085d330
            any = true;
            count = count + 1;
            const std::int32_t no = *at<std::int32_t>(o, 8);
            std::snprintf(buf, sizeof buf, "\t%d", no);  // 0x1421ffc74
            list += buf;
            def::key::write_geo23_block(*step, f, o, static_cast<std::uint32_t>(no));   // 0x14097fa30 = 1402a9c80
            if (*at<std::uint8_t>(o, 0x2c20) == 0) {
                std::snprintf(buf, sizeof buf, "MSHSEC\t%d\t0\n", no);   // 0x1422032a0
                put(buf);
            } else if (*at<std::int32_t>(o, 0x2d94) == 1) {
                *at<std::int32_t>(o, 0x2d8c) = (*at<std::int32_t>(o, 0x2c28) - 1) * *at<std::int32_t>(o, 0x2d80);
                def::key::g2mg::write_mesh_block(*step, f, *o, static_cast<std::uint32_t>(no));   // 0x14097ef30 = 1402a9180
            } else if (sc && sstep) {
                LLOBJ_DATA* o2 = sc->object(no);          // 0x140c7ae80
                if (!o2) refuseWindowRebuild(no);         // 1408bde10, 140830270, 0x1401c5fa0, 0x14096a7d0
                if (0 < windowCount(o) && windowCount(o2) == 0) refuseWindowRebuild(no);
                put("DIM\t3\n");                          // 0x14220328c
                def::key::g2mg::write_mesh_block(*sstep, f, *o2, static_cast<std::uint32_t>(*at<std::int32_t>(o2, 8)));
                put("DIM\t2\n");                          // 0x142203294
            }
        }
        if (!any) return 0;                               // the open file is never closed (empty)
        // vt+0xf0 (0x1405f2390 -> 0x1400a4f60(P, 1)): the project DB path; relative to its folder
        const String rel = app::relToDb(projectDbPath(p), tempStep);   // 0x1401c6a70, 0x140819590
        if (!rel.isEmpty()) {
            const std::string r = engine::local8(rel);
            std::snprintf(buf, sizeof buf, "ACTOUT\t10\n%s\n", r.c_str());   // 0x1422032b0
            put(buf);
        }
        if (st.b60 != 0) unsupported("Convert2Dto3D save: OB+0x60 set (its CNVT3D line, 0x1422032c0)");
        if (0 < count) {
            std::snprintf(buf, sizeof buf, "CNVT3D\t%d\n%s", count, list.c_str());   // 0x1422032d0
            put(buf);
        }
        std::fclose(f);
        return 0;
    });
    // 0x1401c6c50 CONV_STATUS.DAT, removed by 0x1407fbc10 (= 1401b85d0: fopen "r" then remove)
    removeFx(convStatus, fx);
}

// @src 0x1401c3be0  vt+0xe8: the task files of a Convert2Dto3D operation (its own 0x140057370)
void c23SaveFiles(OpClass& c, Convert2Dto3DState& st, const String& m23, const String& tempStep,
                  const String& convStatus, Project& p, Operation&, FileEffects& fx)
{
    Operation* opi = c.current;                           // 0x1405ef2b0
    if (!opi) return;
    GuiModel* m = engine::opModel(p, *opi);               // 0x14005f770
    if (!m) return;
    engine::makeFolders(p, *opi, fx);                     // 0x140051190
    engine::saveSimCtrl(p, *opi, fx);                     // 0x1400597d0
    const int ns = objSlots(*opi);                        // 0x14005fa80
    for (int s = 0; s < ns; ++s)
        if (objId(*opi, s) > 0) engine::saveObject(p, *opi, s, fx);   // 0x140060690, 0x14005c760
    DB_STEP_DATA* step = m->step.get();
    if (!step) return;
    // 0x1402c8170(APP): the GL scene (result unused here)
    GuiModel* sc = workModel(c, st, p);                   // 0x1401c7370
    DB_STEP_DATA* sstep = sc ? sc->step.get() : nullptr;
    if (m && step) {
        const int n = m->objectCount();                   // 0x140c7c1a0
        for (int i = 0; i < n; ++i) {
            LLOBJ_DATA* o = m->object(i + 1);
            if (!o || !kindNot7(o)) continue;
            const int no = *at<std::int32_t>(o, 8);
            if (sc && sstep) {
                if (LLOBJ_DATA* o2 = sc->object(no)) {    // 0x140c7ae80
                    if (*at<std::int32_t>(o, 0x2d94) == 0 && 0 < windowCount(o) && windowCount(o2) == 0)
                        refuseWindowRebuild(no);          // 0x140953560, 0x14095cd90, 0x140953610
                    const String path = qt::cleanPath(meshSettingsFile(c, no, 3));
                    captureFx(path, fx, [&](const char* f) {
                        return def::key::write_meshsettings_key_file(*sstep, f, no);   // 0x140896360 = 1401c6820
                    });
                }
            }
            const String path2 = qt::cleanPath(meshSettingsFile(c, no, 2));
            if (*at<std::int32_t>(o, 0x2d94) == 1)
                *at<std::int32_t>(o, 0x2d8c) = (*at<std::int32_t>(o, 0x2c28) - 1) * *at<std::int32_t>(o, 0x2d80);
            captureFx(path2, fx, [&](const char* f) { return def::key::write_meshsettings_key_file(*step, f, no); });
            const String path3 = qt::cleanPath(geoConvtFile(c, no));
            captureFx(path3, fx, [&](const char* f) {
                return def::key::write_geoconvt_key_file(*step, f, no);   // 0x140896460 = 1401c6920
            });
        }
        if (sc && sstep) sstep->u1b4c = 1;                // 0x140c7b1f0(sc, 1, 0)
    }
    writeM23Input(c, st, m23, tempStep, convStatus, p, fx);   // 0x1401c68c0, 0x1401c5750
    engine::saveMaterials(p, *opi, fx);                   // 0x140058cc0
    if (opi->restartGiven)                                // 0x140064740
        writeModelKey(*m, restartKeyPath(*opi, true), fx);   // 0x140064a50(opi, 1), 0x140c7d100
    if (startsFromKey(*opi))                              // 0x14005f230
        if (Cls_1401e7b10* s = m->simCtl()) opi->startStepNo = s->nstart;   // 0x14005a350(opi, +0x160)
}

// @src 0x1401ce590  properties vt+0x60: the data block into every object (kind != 7)
bool convertPropsToEngine(Project& p, Operation& op, data::ConvertData& d)
{
    GuiModel* m = engine::opModel(p, op);                 // 0x1405f8d90, 0x14005f770
    if (!m) return false;
    std::int32_t axis = d.upAxis;
    const int n = m->objectCount();
    for (int i = 0; i < n; ++i) {
        LLOBJ_DATA* o = m->object(i + 1);
        if (!o || !kindNot7(o)) continue;
        def::cmd::cnvt3d::wpaxis_set_axis(at<def::model::linked::Cls_140258580List>(o, 0x20a0), &axis);   // 0x1408ea190 = 14025a3b0
        *at<std::int32_t>(o, 0x2c38) = axis;
        *at<std::int32_t>(o, 0x2be8) = d.conType;
        if (*at<std::int32_t>(o, 0x2be8) == 1) {
            *at<double>(o, 0x2bf0) = d.revAngle;
            *at<double>(o, 0x2bf8) = d.revStartAngle;
        } else {
            *at<double>(o, 0x2c10) = d.extLen;
            *at<double>(o, 0x2c18) = d.extStart;
        }
    }
    return true;
}

// @src 0x140e2d230  properties vt+0x60: the body of 0x1401ce590
bool copyMirrorPropsToEngine(Project& p, Operation& op, data::ConvertData& d)
{
    return convertPropsToEngine(p, op, d);
}

// @src 0x140e24cf0  vt+0xe8: Step.KEY of the model in the task folder (relative to the current
// directory, the project folder); no other task file
void cmSaveFiles(OpClass& c, const String& stepKey, Project& p, Operation&, FileEffects& fx)
{
    Operation* opi = c.current;
    if (!opi) return;
    GuiModel* m = engine::opModel(p, *opi);               // 0x14005f7a0
    DB_STEP_DATA* step = m ? m->step.get() : nullptr;
    if (!step) return;
    const String rel = qt::cleanPath(taskDirRel(*opi) + qt::kSep + stepKey);   // 0x140e24ac0, cleanDirPath
    const String path = qt::cleanPath(projectDir(p) + qt::kSep + rel);
    const std::uint32_t flags = 0;
    captureFx(path, fx, [&](const char* f) { return def::cmd::write_key_file(step, f, &flags); });   // 0x140895350
}

// @src 0x140e24b30 -> 0x140e23d80  vt+0x80: mirrormerge.ini from the viewer items' planes
void cmSaveAfter(OpClass& c, CopyMirrorState& st, const String& stepKey, Project& p, Operation&, FileEffects& fx)
{
    Operation* opi = c.current;
    if (!opi) return;
    GuiModel* m = engine::opModel(p, *opi);
    if (!m) return;
    // relative paths resolve against the current directory, the project folder
    const String ini = qt::cleanPath(taskDirRel(*opi) + qt::kSep + L("mirrormerge.ini"));   // 0x142abf750
    const String path = qt::cleanPath(projectDir(p) + qt::kSep + ini);
    // 0x140e23d80 opens QFile(WriteOnly) before the object/viewer scan. This models only
    // the missing-directory failure, including directories already recorded by this save.
    if (!folderExistsFx(path, fx)) return;
    const int n = m->objectCount();                       // 0x140c7c1a0
    for (int i = 0; i < n; ++i) {
        int* planes = viewerItemPlanes(*m, i + 1);        // 0x140c82d50 (made when missing)
        if (planes && 0 < *planes)                        // 0x141249420
            unsupported("CopyMirror save: MIRROR_MERGE lines of the viewer items' planes (0x140e23d80, packet V)");
    }
    captureFx(path, fx, [&](const char* file) {
        // QFile::open(WriteOnly) without Text, QTextStream with the locale codec (ASCII here): "\n"
        std::FILE* f = std::fopen(file, "wb");
        if (!f) return 1;
        const String step = qt::cleanPath(taskDirRel(*opi) + qt::kSep + stepKey);   // 0x140e24ac0
        std::string s = def::key::keyword_label(3);      // 0x140921f30(3)
        s += "\t";
        s += qt::formatInt(1);
        s += "\n";
        s += step.isEmpty() ? std::string("(null)") : qt::toAscii(step);   // 0x142abf760
        s += "\n";
        const auto it = st.tolerances.find(1);           // 0x140e29d30(self, 1): +0x138, else 0.001
        const double tol = it == st.tolerances.end() ? 0.001 : it->second;
        s += "TOLERANCE\t" + qt::formatDouble(tol) + "\n";   // 0x142abf768
        std::fwrite(s.data(), 1, s.size(), f);
        std::fclose(f);
        return 0;
    });
}

}  // namespace

// @src 0x1401c3570  vt+0xc8 (tail of the sync from the model 0x1400538f0, MDT DB packet): base
// 0x1405f1e10 (empty); outside the class's own load (+0x130), the work objects lose surfaces and
// mesh (0x1401c7560(-1, 1)), then the work model is filled again from the operation's objects as
// the post-load does, written to TEMP_STEP.KEY and read into the display's second model
void c23SyncTail(OpClass& c, Convert2Dto3DState& st, const String& tempStep, Project& p, FileEffects& fx)
{
    if (st.loading) return;                               // +0x130
    clearWorkObjects(st, p, -1, true, tempStep, fx);      // 0x1401c7560(self, -1, 1)
    Operation* opi = c.current;                           // 0x1405ef2b0
    if (!opi) return;
    GuiModel* m = engine::opModel(p, *opi);               // 0x14005f770
    if (!m) return;
    DB_STEP_DATA* step = m->step.get();                   // 0x140c7acd0
    if (!step) return;
    if (!engine::glScenePresent()) return;                // 0x1402c8170(APP)
    GuiModel* w = workModel(c, st, p);                    // 0x1401c7370
    engine::viewerClear(*w);                              // 0x140c82c70
    const int n = m->objectCount();                       // 0x140c7c1a0
    for (int i = 0; i < n; ++i) {
        LLOBJ_DATA* o = m->object(i + 1);                 // 0x140c7ae80
        if (!o || !kindNot7(o)) continue;                 // 0x140834fa0
        *at<std::uint8_t>(o, 0x2c68) = 0;
        *at<double>(o, 0x2bf8) = 0.0;
        *at<double>(o, 0x2bf0) = 360.0;                   // 0x1421249c8
        *at<std::int32_t>(o, 0x2be8) = def::model::geo_kw::simctl_geo_kind(m->simCtl());   // 0x1408da610
        if (*at<std::int32_t>(o, 0x2be8) == 1 && *at<std::int32_t>(o, 0x2d94) == 0)
            setRevolveType(step, o);                      // 0x1401c71f0
        LLOBJ_DATA* o2 = w->objectWrap(*at<std::int32_t>(o, 8), false, true);   // 0x140c7c950(w, no, 0, 1)
        if (o2) {
            def::model::llobj_copy::copy_u3aa0(o2, o);    // 0x140835b50
            def::model::llobj_copy::copy_u3b50(o2, o);    // 0x140835bf0
            def::model::llobj_copy::copy_u3998(o2, o);    // 0x140835a50
            def::model::cls_1401889a0_assign(at<std::byte>(o2, 0x2be8), at<std::byte>(o, 0x2be8));   // 0x14085ceb0
            const char shapes = 0;
            const char byDim = 1;
            def::model::llobj_copy::meshgen_copy(at<std::byte>(o2, 0x2d10), at<std::byte>(o, 0x2d10), &shapes,
                                                 &byDim); // 0x14085dd40
            *at<std::int32_t>(o2, 0x39c0) = 3;            // 0x1401cc360
            *at<std::int32_t>(o, 0x2d94) = *at<std::int32_t>(o2, 0x2d94);
        }
        if (*at<std::int32_t>(o, 0xc) == 0 && !engine::objHasMesh(o)) {   // 0x140834c80
            *at<std::uint8_t>(o, 0x2c20) = 0;
            if (o2) *at<std::uint8_t>(o2, 0x2c20) = 0;
        }
    }
    if (w->step) w->step->u1b4c = 1;                      // 0x140c7b1f0(w, 1, 0): 0x140823c80
    if (GuiModel* s2 = engine::displaySecond(engine::projectDisplay(p))) {   // 0x1402c81b0
        writeModelKey(*w, tempStep, fx);                  // 0x1401c6f20, 0x140c7d100
        s2->reset();                                      // 0x140c7b720
        s2->db = m->db;                                   // 0x140c7aa80(s2, 0x140c7ac30(m))
        readModelKeyFx(*s2, tempStep, fx);                // 0x140c7d020
    }
}

void installL5Slots()
{
    g_l5Slots.c23SaveFiles = &c23SaveFiles;
    g_l5Slots.c23PropsToEngine = &convertPropsToEngine;
    g_l5Slots.c23UpdateData = &c23UpdateData;
    g_l5Slots.c23SyncTail = &c23SyncTail;
    g_l5Slots.cmSaveFiles = &cmSaveFiles;
    g_l5Slots.cmSaveAfter = &cmSaveAfter;
    g_l5Slots.cmPropsToEngine = &copyMirrorPropsToEngine;
    g_l5Slots.booleanPrologue = &booleanPrologue;
    g_l5Slots.booleanPostLoad = &booleanPostLoad;
    g_l5Slots.booleanEditorFill = &booleanEditorFill;
    g_l5Slots.booleanEditorApply = &booleanEditorApply;
    g_l5Slots.booleanSideFiles = &booleanSideFiles;
    g_l5Slots.c23Load = &c23Load;
    g_l5Slots.c23Prologue = &c23Prologue;
    g_l5Slots.c23PostLoad = &c23PostLoad;
    g_l5Slots.c23PropsLoad = &c23PropsLoadSlot;
    g_l5Slots.cmLoad = &cmLoad;
    g_l5Slots.cmPrologue = &cmPrologue;
    g_l5Slots.cmPostLoad = &cmPostLoad;
    g_l5Slots.cmPropsLoad = &cmPropsLoad;
    g_l5Slots.cmProvideObject = &cmProvideObject;
}

}  // namespace m2m::mst
