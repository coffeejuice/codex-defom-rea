// MDT DB: loading an operation from a project database step (docs/re/moproj2mst/mdt/DB/spec.md,
// packets DB-1 .. DB-4 of packets.md): the DB handle of the data root (section 1), the step
// availability check (section 2), the step read into the engine model (sections 4, 5) and the GUI
// sync from the model (section 6). One function per binary function, in the original's call order.
// The engine (DB info, directory, header cache, ReadRecordFromDB) is the DEF_PRE port in
// port/defdbio, called, never copied. UI-only calls are dropped; each drop is named.
//
// The DB file is only read: load_db_info (fopen "r"), the datarm23 unit opens of the header cache
// and the step read (ACTION='READ', STATUS='OLD').
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <filesystem>
#include <fstream>
#include <functional>
#include <iterator>

#include <defdbio/db_query.hpp>
#include <defdbio/dbread/dbread.hpp>
#include <defdbio/objcheck/objcheck.hpp>
#include <defdbio/step/db_header_cache.hpp>
#include <defdbio/step/db_info_read.hpp>
#include <defkey/item_writers.hpp>
#include <defmodel/layout/cls_1401dbfe0.hpp>
#include <defmodel/layout/cls_1401e7b10.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/layout/material.hpp>
#include <defmodel/mesh2d/material_ops.hpp>
#include <defmodel/step/db_directory_receipt.hpp>

#include "engine_internal.hpp"
#include "gui_model_access.hpp"
#include "m2m/engine.hpp"
#include "m2m/doe_slice.hpp"
#include "m2m/longpath.hpp"
#include "m2m/mst/writer.hpp"
#include "m2m/qt4path.hpp"

namespace m2m::engine {

using def::model::Cls_1401dbfe0;
using def::model::Material;
using qt::L;

namespace {

[[noreturn]] void unsupported(const std::string& what)
{
    throw Error(M2M_E_UNSUPPORTED, "database step load: " + what);
}

// QString::fromLocal8Bit (system ANSI code page)
String fromLocal8(const std::string& s)
{
    if (s.empty()) return L("");
    const int n = MultiByteToWideChar(CP_ACP, 0, s.data(), static_cast<int>(s.size()), nullptr, 0);
    std::u16string u(static_cast<std::size_t>(n > 0 ? n : 0), u'\0');
    if (n > 0)
        MultiByteToWideChar(CP_ACP, 0, s.data(), static_cast<int>(s.size()), reinterpret_cast<wchar_t*>(u.data()), n);
    return String(u);
}

// QFileInfo(path).lastModified() in ms; false when the file cannot be read
bool lastModifiedMs(const String& path, long long* ms)
{
    std::error_code ec;
    const auto t = std::filesystem::last_write_time(std::filesystem::path(win32Path(path.u16())), ec);
    if (ec) return false;
    *ms = static_cast<long long>(t.time_since_epoch().count() / 10000);   // 100 ns file time -> ms
    return true;
}

// A KEY the engine writes to a path: written to a scratch file and recorded as an effect
void captureWrite(const String& target, FileEffects& fx, const std::function<void(const char*)>& write)
{
    const std::filesystem::path tmp = std::filesystem::temp_directory_path() /
                                      ("m2m_db_" + std::to_string(GetCurrentProcessId()) + "_" +
                                       std::to_string(GetCurrentThreadId()) + ".KEY");
    std::error_code ec;
    std::filesystem::remove(tmp, ec);
    write(tmp.string().c_str());
    std::ifstream in(tmp, std::ios::binary);
    std::string bytes((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    in.close();
    fx.writes.push_back({target, std::move(bytes)});
    std::filesystem::remove(tmp, ec);
}

}  // namespace

// ==== DB-1: the DB handle ===========================================================================

// @src 0x140cda460 (DB part) SfModelBase ctor: path null, not loaded, the DB reader (DEF_PRE ctor
// 1401dbfe0 of the engine part)
DbHandle::DbHandle(Kind modelKind) : kind(modelKind),
    slicePlanes(std::make_unique<doe::SlicePlaneStore>()), reader(std::make_unique<Cls_1401dbfe0>())
{
    (void)def::model::directory_receipts::bindOwner(this, reader.get());
}

void DbHandle::adoptMoModelSlots(std::shared_ptr<MoModelSlots> slots)
{
    if (kind != Kind::Mo)
        throw Error(M2M_E_UNSUPPORTED, "Mo model slot storage requires an actual Mo model");
    if (!slots) throw Error(M2M_E_UNSUPPORTED, "Mo model slot storage owner is missing");
    if (moModelSlots_ && moModelSlots_ != slots)
        throw Error(M2M_E_UNSUPPORTED, "Mo model already owns a different capsule");
    if (moModelSlots_ == slots) return;
    slots->bindModelIdentity(*this);
    moModelSlots_ = std::move(slots);
}

DbHandle::~DbHandle()
{
    def::model::directory_receipts::retireOwner(this, reader.get());
    // Adapter fallback lease release only. Native staged slot effects and
    // Project deletion are separate; a live Project still owns its capsule.
    moModelSlots_.reset();
    // cda520 -> cda6f0 deletes+1970 before the DB reader+40 destructor.
    // Other native manager fields/Qt callbacks remain separately qualified.
    postGraphManager.reset();
    // The later native QList+18 destructor frees only its backing storage.
    // slicePlanes holds nonowning refs; process-heap retention survives this
    // direct model teardown. Full normal-Close cleanup remains separate.
}

// @src 0x1400a5890 P+8, SfMoModel (043fe0). Lazy adapter allocation here does
// not reproduce outer ModelManager registration/selection, recovered separately.
DbHandle* projectDb(Project& p)
{
    if (!p.dataDb) p.dataDb = std::make_shared<DbHandle>(DbHandle::Kind::Mo);
    if (p.dataDb->kind == DbHandle::Kind::Mo) p.dataDb->adoptMoModelSlots(p.modelSlotsOwner());
    // Existing Base fixtures retain their legacy adapter profile. They do not
    // establish native Mo/Project ownership or authorize accepted Close.
    return p.dataDb.get();
}

// @src 0x140cdcdc0  setPath(name, keepItems)
void dbSetPath(DbHandle& h, const String& name, bool keepItems)
{
    h.pathSet = true;                                       // +0x20
    if (!(h.path == name)) {
        h.path = name;
        // !keepItems: 0x140cdabc0 deletes the objects of the list DBH+0x1960 (vt+0x18) and clears it;
        // nothing on the converter's path adds to that list, so there is nothing to delete
        (void)keepItems;
        h.loaded = false;                                   // +0x38
        h.stampValid = false;                               // 0x140d39950: DBR+0x1910 = QDateTime()
    }
    if (!h.path.isEmpty() && !app::endsWithSuffix(h.path, L(".DB"))            // 0x140818b50
        && !qt::endsWith(h.path, L("FOR003"), qt::Case::Sensitive))
        h.path = app::changeSuffix(h.path, L(".DB"));      // 0x140818d50
    h.path = qt::cleanPath(h.path);
    // <dir>/.ObjectColor.DAT through 0x140cdeab0: the viewer's object colour table (UI)
}

namespace {

// @src 0x140d395c0  DBR::reopen(path, headerOnly)
bool dbReopen(DbHandle& h, const String& path, bool headerOnly)
{
    const std::string lp = local8(path);
    const int rc = def::dbio::load_db_info(*h.reader, lp.c_str(), headerOnly);   // 0x14080ca10 = 1401dc3f0
    if (rc != 0) return false;
    long long ms = 0;
    h.stampValid = lastModifiedMs(path, &ms);               // DBR+0x1910 = QFileInfo(path).lastModified()
    h.stamp = ms;
    h.cacheStarted = false;                                 // DBR+0x1908 = 0
    return true;
}

// @src 0x140d396d0  header cache, once per (re)open
void dbBuildCache(DbHandle& h, bool async)
{
    if (h.cacheStarted) return;
    h.cacheStarted = true;                                  // DBR+0x1908 = 1
    if (async)
        unsupported("asynchronous header cache build (0x140d399d0 QThread); no caller on this path passes it");
    def::dbio::step::load_header_cache(*h.reader, false);  // 0x14080d870 = 1401dd250
}

}  // namespace

// @src 0x140cdc480  open(buildCache, async)
bool dbOpen(DbHandle& h, Ctx& ctx, bool buildCache, bool async)
{
    if (h.loaded) return true;
    if (h.path.isEmpty()) return false;
    if (!dbReopen(h, h.path, false)) return false;          // 0x140d395c0
    bool yes = true;                                        // QMessageBox::Yes
    if (def::dbio::query::has_previous_part(*h.reader)) {  // 0x140814e80 = 1401e4c10
        // QMessageBox::question "2G Split DBs" (Yes | Default, No | Escape): the default button
        ctx.warn("2G Split DBs: " + h.path.utf8() + " may have been split into multiple files; answered with the "
                 "default button Yes (load all of them as an integrated DB)");
        yes = true;
    }
    if (yes) def::dbio::query::setup_part_chain(*h.reader); // 0x1408150f0 = 1401e4e80
    // @src 0x140cdc480 vt+0x78  omitted: licence (the "License error" message box is never shown)
    if (buildCache) dbBuildCache(h, async);                 // 0x140d396d0
    h.loaded = true;                                        // +0x38
    return true;
}

// @src 0x14008a9b0 (data root part)  h = 0x1400a5890(P): 0x140cdcdc0(h, 0x1400a4ee0(P), false), then
// 0x140cdc400(h) = 0x140cdc480(h, &true, &false)
void openProjectDb(Project& p, Ctx& ctx)
{
    if (DbHandle* h = projectDb(p)) {
        dbSetPath(*h, primaryDbPath(p), false);
        // A project whose database opens shows the main window with the Operation Editor's diagram
        // view 175 px high instead of 123 (and the 3D view 52 px lower). Taken from Oracle B: every
        // fresh save with its primary DB present (124, 127..133, the DOE runs) has the 175 px view,
        // every one without (125 no DB, 126 renamed DB, the corpus re-saves) 123; which widget the
        // open DB shows is not traced. moproj::viewCentre, viewer_camera.cpp.
        if (dbOpen(*h, ctx, true, false)) p.diagramViewHeight = 175;
    }
}

// @src 0x140cdbcd0  header cache on demand: only for a loaded handle
bool dbHeaderCache(DbHandle& h, bool async)
{
    if (h.loaded) dbBuildCache(h, async);                   // 0x140d396d0
    return h.loaded;
}

// @src 0x1400b9cd0
DbHandle* subProjectDb(Project& p, SubProject& sp)
{
    Project* parent = sp.project;                           // 0x1400b5c40
    if (!parent) return nullptr;
    (void)p;
    DbHandle* h = projectDb(*parent);                       // 0x1400a5890
    if (sp.type == 4) {
        if (!sp.ownDb) {
            const String name = subProjectDbPath(*parent, sp, true);   // 0x1400b9e80 (-> 0x1400bbee0)
            sp.ownDb = std::make_shared<DbHandle>();        // new(0x19c0) 0x140cda460(nullptr)
            if (const auto heap = h->slicePlanes->heapOwner())
                sp.ownDb->slicePlanes->bindHeap(heap); // same process, no new native owner
            dbSetPath(*sp.ownDb, name, false);              // 0x140cdcdc0
        }
        h = sp.ownDb.get();
    }
    return h;
}

// @src 0x140c7ac50
Cls_1401dbfe0* modelDbReader(const GuiModel& m)
{
    requireModelGeometrySyncAvailable(m);
    return m.db ? m.db->reader.get() : nullptr;             // 0x140cdab70
}

// @src 0x140d397a0  refreshIfChanged(DBR, arg)
bool dbRefreshIfChanged(DbHandle& h)
{
    bool ok = false;
    const String path = h.pathSet ? h.path : String();      // 0x140cdd190(owner, &path, true)
    long long t = 0;
    if (!path.isEmpty() && qt::fileExists(path) && lastModifiedMs(path, &t)) {
        if (!h.stampValid || t != h.stamp) {
            if (dbReopen(h, path, false)) {                 // 0x140d395c0
                h.cacheStarted = false;
                dbBuildCache(h, false);                     // 0x140d396d0, f = false
                // 0x140cdd800(owner, arg): step browser refresh (UI)
                ok = true;
            }
        } else {
            def::dbio::step::reread_db_header(*h.reader);  // 0x14080d6f0 = 1401dd0d0
            ok = true;
        }
    }
    // the toString() of both stamps is computed and discarded
    return ok;
}

// ==== DB-2: step availability ========================================================================

// @src 0x14080f690
std::vector<DbOperation> dbOperationList(const Cls_1401dbfe0& r)
{
    std::vector<DbOperation> out;
    int last = -1;
    const auto* dir = static_cast<const std::byte*>(r.uad8);
    const auto* cache = static_cast<const std::byte*>(r.uae8);
    for (int i = 0; i < r.uac8; ++i) {
        if (*reinterpret_cast<const std::int32_t*>(dir + static_cast<std::size_t>(i) * 0x28 + 0x24) == 0) continue;
        if (!cache)
            unsupported("DB operation list without a header cache (0x14080f690 reads DBR+0xae8 unchecked)");
        const std::byte* c = cache + static_cast<std::size_t>(i) * 0xe0;
        const int opNo = *reinterpret_cast<const std::int32_t*>(c + 0x38);
        if (last < opNo) {
            last = opNo;
            DbOperation d;
            d.name = reinterpret_cast<const char*>(c + 0x4c);   // 0x140140460 (char* name)
            d.opNo = opNo;
            out.push_back(std::move(d));                    // 0x1401405b0
        }
    }
    return out;                                             // 0x140917800
}

namespace {

// @src 0x1400b8b80  prefixMatchesDb(sp, idx, x, &lastMatch, &unused): the chain's operations
// 1, 2, ... carry the names and operation numbers of the DB's list; true when the match reaches
// operation idx - 1. Documented deviation (spec Q4): the binary indexes the list past its end
// (linked list with a sentinel head, 0x1400bc740 / 0x1400ad700); the port stops there. Oracle B
// 125_B_DBSTEP_NODB (no DB: empty list) shows the GUI neither crashes nor asks, i.e. the result is
// false there, which this gives.
bool prefixMatchesDb(Project& p, SubProject& sp, int idx, int /*x*/, int* lastMatch, int* /*unused*/)
{
    DbHandle* dbh = subProjectDb(p, sp);                    // 0x1400b9cd0
    if (!dbh) return false;
    const std::vector<DbOperation> dbOps = dbOperationList(*dbh->reader);   // 0x14080f690(0x140cdab70(dbh))
    const std::vector<Operation*> ops = flowOperations(sp); // 0x1400b94f0
    int k = 1;
    for (Operation* o : ops) {
        if (!o) unsupported("a gap in the flow chain's operation list (0x1400b94f0) is used unchecked by 0x1400b8b80");
        const String nm = o->name;                          // 0x140050ac0; 0x140050b10 returns 0, unused
        if (static_cast<std::size_t>(k - 1) >= dbOps.size()) break;   // deviation, see above
        const DbOperation& d = dbOps[static_cast<std::size_t>(k - 1)];
        if (!(fromLocal8(d.name) == nm && d.opNo == k)) break;
        *lastMatch = k;
        ++k;
    }
    return *lastMatch + 1 >= idx;
}

// the shared tail of 0x14005f520 and 0x14005f270 after the prefix test
bool enoughSteps(Project& p, Operation& op, DbHandle& dbh, int idx)
{
    const int n = def::dbio::query::op_step_count(*dbh.reader, idx - 1);   // 0x14080e840 = 1401de240
    Operation* prev = prevOp(p, op);                        // 0x140062960
    if (!prev) return true;
    mst::OpClass* pob = opBase(*prevOp(p, op));             // 0x140054e90, no null check in the binary
    if (!pob) unsupported("previous operation without a class (0x140054e90 result used unchecked by 0x14005f520)");
    if (pob->oneStepPrev() && n >= 1) return true;          // OB vt+0x1f0 (0x1405f68e0)
    return n > 1;
}

}  // namespace

// @src 0x14005f520
bool dbHasStepsFor(Project& p, Operation& op, int* lastMatch, int* unused)
{
    SubProject* sp = activeSubProject(p);                   // 0x140055050
    if (!sp) return false;
    DbHandle* dbh = subProjectDb(p, *sp);                   // 0x1400b9cd0
    if (!dbh) return false;
    bool r = false;
    const int idx = flowNumber(op);                         // 0x140050b20
    const int x = mst::nextSim(p, *sp, idx);                // 0x140050b70 -> 0x1400b8820; unused by the callee
    if (prefixMatchesDb(p, *sp, idx, x, lastMatch, unused) && dbh->reader)   // 0x140cdab70
        r = enoughSteps(p, op, *dbh, idx);
    return r;
}

// @src 0x14005f270  qDebug "SfOperationInfo in INTERACTIVE Mode ..." / "... BATCH Mode ..." dropped
bool interactiveSetup(Project& p, Operation& op)
{
    SubProject* sp = activeSubProject(p);
    if (!sp) return false;
    DbHandle* dbh = subProjectDb(p, *sp);
    if (!dbh) return false;
    const int idx = flowNumber(op);
    if (idx < 2) return true;
    bool r = false;
    const int x = mst::nextSim(p, *sp, idx);                // 0x140050b70
    int lastMatch = 0, unused = 0;
    if (prefixMatchesDb(p, *sp, idx, x, &lastMatch, &unused) && dbh->reader)
        r = enoughSteps(p, op, *dbh, idx);
    return r;
}

// ==== DB-3: the step read ============================================================================

// @src 0x140c7b870
bool readDbStep(GuiModel& m, const int* step, bool resetViewer)
{
    return GuiModelAccess::withStep(m, [&](DB_STEP_DATA* nativeStep) -> bool {
        if (!m.db) return false;
        Cls_1401dbfe0* r = modelDbReader(m);                    // 0x140c7ac50
        if (!r) return false;
        const int idx = def::dbio::query::step_index(*r, *step);   // 0x14080e560 = 1401ddf60
        if (idx < 0) return false;
        if (!nativeStep) return false;                          // 0x140c7acd0
        return readDbEntry(m, &idx, resetViewer);               // 0x140c7b900
    });
}

// @src 0x140c7b900
bool readDbEntry(GuiModel& m, const int* idx, bool resetViewer)
{
    return GuiModelAccess::withStep(m, [&](DB_STEP_DATA* nativeStep) -> bool {
        if (!m.db || !nativeStep) return false;
        if (resetViewer) {
            viewerClear(m);                                     // 0x140c82c70
            m.viewerMirrorPlanes.clear();                       // its items (MDT L5 keeps only these)
        }
        // 0x140c8ba80: delete M+0x30 (viewer, packet V), M+0x38 = 0
        const bool f = true;                                    // APP->procType (0x1402cb7a0) != 3: 5 after a .moproj open
        nativeStep->u1b58 = f ? 1 : 0;                           // 0x140896940
        Cls_1401dbfe0* r = modelDbReader(m);
        // 0x1408256c0 = 1401cc420 (the GUI's 4th argument `progress` is false: progress bar only)
        const int rc = def::dbio::dbread::fun_1401cc420(nativeStep, r, *idx);
        if (rc != 0) return false;
        // procType == 1 (DEF_PRE mode) NSTART flip: never for MO (5)
        bool async = false;
        dbHeaderCache(*m.db, async);                            // 0x140cdbcd0
        furnaceTableToBlock(m);                                 // 0x140c7d3a0
        primitiveTable(m);                                      // 0x140c7ebe0
        (void)def::dbio::query::entry_step(*r, *idx);           // 0x14080e500 = 1401ddf00: step number, UI use only
        viewerRebuild(m, resetViewer, false);                   // 0x140c7ff80
        // UI: step browser 0x140d0cc00, 0x140c911c0 / 0x140c905b0 / 0x140d15b40 / 0x1402ca890 (viewer
        // windows), 0x140c90150 (M+0x230, spec Q5), signals 0x140d77610, "signalActiveStepNoChanged()",
        // "signalAssignFlownetData()", 0x140c8c930, 0x140cf97f0, 0x140cff370, 0x140c80a20 / 0x140c80b00
        m.state100 = 1;                                         // 0x140c833c0(m, &1)
        return true;
    });
}

// @src 0x1400572e0
GuiModel* loadDbStep(Project& p, Operation& op, int step, bool sync, Ctx& ctx, FileEffects& fx)
{
    GuiModel* m = opModel(p, op);                           // 0x14005f770
    if (!m) return nullptr;
    if (step != 0) {
        m->reset();                                         // 0x140c7b720
        // spec Q8: the binary drops the result; a failed read leaves the reset model (no message)
        (void)readDbStep(*m, &step, true);                  // 0x140c7b870
        syncFromModel(p, op, sync, false, ctx, fx);         // 0x1400538f0(opi, sync, 0)
    }
    return m;
}

// @src 0x140057230
GuiModel* loadRestart(Project& p, Operation& op, Ctx& ctx, FileEffects& fx)
{
    GuiModel* m = opModel(p, op);                           // 0x14005f770
    if (!m) return nullptr;
    m->reset();                                             // 0x140c7b720
    const String f = restartKeyPath(op, true);              // 0x140064a50
    (void)m->readKey(f);                                    // 0x140c7d020 (with 0x140c7d3a0 / 0x140c7ebe0 on success)
    viewerRebuild(*m, true, true);                          // 0x140c7ff80(m, 1, 1)
    syncFromModel(p, op, true, true, ctx, fx);              // 0x1400538f0(opi, 1, 1)
    return m;
}

// ==== DB-4: the GUI sync from the model ==============================================================

namespace {

// SfMoItemManager vt+0x168 (0x14006c230): the smallest id >= 1 the manager does not hold
int firstFreeItemId(const ItemManager& mgr)
{
    int id = 1;
    while (mgr.find(id)) ++id;
    return id;
}

// SfMoItemManager vt+0x188 (0x14006c570): vt+0x170 new item of the manager's kind (base ctor
// 0x14006b6a0 / 0x14006b7c0: text "New item"), vt+0x178 its file, 0x14006ba30 (+0x28), appendRow
Item* appendItem(Project& p, ItemManager& mgr, int id)
{
    auto it = std::make_unique<Item>();
    if (mgr.kind == ItemKind::Object) initObjItem(*it, id);   // 0x1400688f0
    it->kind = mgr.kind;
    it->id = id;
    it->name = L("New item");                               // 0x14006bb40
    it->keyPath = itemKeyPath(p, mgr.kind, id);             // vt+0x178, not cleaned
    Item* raw = it.get();
    mgr.rows.push_back(std::move(it));                      // invisibleRootItem()->appendRow
    return raw;
}

// @src 0x140060c40  material item `id` <- engine material `id`: name, type and phase list
void syncMaterialItem(Project& p, Operation& op, int id)
{
    ItemManager& mm = p.materials;                          // 0x140055110
    Item* it = mm.find(id);                                 // 0x140042ad0
    withOpStep(p, op, [&](DB_STEP_DATA* s) {                // 0x14005f7a0
        Material* mat = s ? def::model::material_list_find(&s->materials, id) : nullptr;   // 0x140830430
        if (!it || !mat) return;
        it->name = String::latin1(mat->name);                   // vt+0x40(QString(mat+0xc))
        if (mat->mstmtr_flag == 0) {                            // 0x140041fa0
            it->matType = 0;
            it->phases.clear();                                 // 0x1400424b0
        } else if (mat->mstmtr_flag == 1) {                     // 0x140042010
            it->matType = 1;
            it->phases.clear();
            for (int i = 0; i < mat->mstmtr_count; ++i) it->phases.push_back(mat->mstmtr_list[i]);   // 0x140025100
        } else {
            it->matType = 2;                                    // 0x140041ff0
        }
    });
}

// Expansion rows of the diagram items. Every row of the object area comes from item vt+0x58
// (0x140160e70: a Text row with indent 2, as the open builds them, 0x140073900); row vt+0x8 sets
// its text, vt+0x10(.., 1) returns indent spaces + text (ExpansionRow::label).
constexpr int kExpansionTemplate = 7;                       // DAT_14358133c (.data, 7 template rows)

void setRowText(ExpansionRow& r, const String& text)
{
    if (r.type != 0)
        unsupported("diagram expansion row of type " + std::to_string(r.type) + " given a text (row vt+0x8)");
    r.label = L("  ") + text;
}

// @src 0x14007bf20  clear(model, id, row, text): `text` is not used. Diagram model+0x18 (the editor
// scene) is taken as present: the GUI's main window has the diagram editor (inferred).
void diagramRowClear(FlowChart& flow, int id, int row)
{
    for (auto& up : flow.items) {
        FlowItem* it = up.get();
        if (!it || it->opId != id) continue;                // vt+0x28
        const int a = row + kExpansionTemplate;
        std::vector<ExpansionRow>& rows = it->expansion;    // vt+0x50 (a copy; rows removed by pointer)
        // UI: MSG signal 0x14213a5e0
        const int n = static_cast<int>(rows.size());
        if (a < n - 1) {
            setRowText(rows[static_cast<std::size_t>(a)], L("Undefined"));   // 0x14213a608
        } else if (a < n) {
            rows.erase(rows.begin() + a);                   // vt+0x78
            for (int i = a - 1; i >= kExpansionTemplate; --i) {
                const ExpansionRow& r = rows[static_cast<std::size_t>(i)];
                if (r.type != 0)
                    unsupported("diagram expansion row of type " + std::to_string(r.type) + " compared by text");
                if (!(r.label == L("  Undefined"))) break; // 0x14213a618, latin1 compare 0x1400889b0
                rows.erase(rows.begin() + i);               // vt+0x78
            }
        }
        // UI: model+0x54 == 0 -> MSG signal 0x14213a638
    }
}

// @src 0x14007b630  setText(model, id, row, text): append the row when it is the next one
// (vt+0x58 with the object mime data, then vt+0x28 / vt+0x8 / accepted mime types eqp and mat:
// drag-and-drop properties, not saved), then set its text. model+0x18 as in 0x14007bf20.
void diagramRowText(FlowChart& flow, int id, int row, const String& text)
{
    for (auto& up : flow.items) {
        FlowItem* it = up.get();
        if (!it || it->opId != id) continue;
        std::vector<ExpansionRow>& rows = it->expansion;
        if (row >= 0 && row == static_cast<int>(rows.size()) - kExpansionTemplate) {
            ExpansionRow r;                                 // item vt+0x58 = 0x140160e70
            r.type = 0;
            setRowText(r, text);
            rows.push_back(r);
            // 0x140082760 (scene update), vt+0x28 (mime), vt+0x38 (accepted types): UI
        }
        if (row >= 0 && row < static_cast<int>(rows.size()) - kExpansionTemplate) {
            setRowText(rows[static_cast<std::size_t>(row + kExpansionTemplate)], text);   // vt+0x8
            // 0x140082760 and MSG signal 0x14213a5b0: UI
            return;
        }
    }
}

// @src 0x140087270  diagram item of operation `id` <- model m
void updateDiagramItem(Project& p, FlowChart& flow, int id, GuiModel& m)
{
    for (auto& up : flow.items) {
        FlowItem* it = up.get();
        if (!it || it->opId != id) continue;                // vt+0x28
        // model+0x54 = 1 around the update (signal suppression, UI)
        // item vt+0x10: the operation's name (main window 0x1402c8090 and its project 0x14001d7d0 exist;
        // the item text from S+0x7a1 is only for a missing main window / operation)
        Operation* o = operationById(p, id);                // 0x140047dc0
        if (!o) unsupported("diagram item without its operation (0x140087270 then names it from S+0x7a1)");
        if (!(it->itemName == o->name))
            unsupported("diagram item " + it->itemName.utf8() + " renamed to its operation's name " + o->name.utf8() +
                        " (item vt+0x10, 0x140087270): that setter was not read");
        const int count = static_cast<int>(it->expansion.size());   // vt+0x50 copy, its count fixed
        for (int r = kExpansionTemplate; r < count; ++r)
            diagramRowClear(flow, id, r - kExpansionTemplate);       // 0x14007bf20(.., "")
        Operation* lop = operationById(p, id);              // 0x14001d7d0 -> 0x140047dc0
        const int n = m.objectCount();                      // 0x140c7c1a0
        for (int i = 0; i < n; ++i) {
            LLOBJ_DATA* obj = m.object(i + 1);              // 0x140c7ae80 (by number)
            if (!obj) unsupported("object number " + std::to_string(i + 1) + " missing (0x140834fa0 on null)");
            if (*at<std::int32_t>(obj, 0xc) == 7) {         // 0x140834fa0 false: undefined
                diagramRowText(flow, id, i, L("undefined"));   // tr("undefined") 0x14213b040, no translator
            } else {
                String t = String::latin1(at<char>(obj, 0x828));
                if (lop && slotCode(*lop, i) == -2) t = L("Read From DB");   // 0x14213b030
                diagramRowText(flow, id, i, t);             // 0x14007b630
            }
        }
        for (int k = n; k - 1 >= 0; --k) {                  // trailing undefined objects
            LLOBJ_DATA* obj = m.object(k);
            if (!obj) unsupported("object number " + std::to_string(k) + " missing (0x140834fa0 on null)");
            if (*at<std::int32_t>(obj, 0xc) != 7) break;
            diagramRowClear(flow, id, k - 1);               // 0x14007bf20(.., "")
        }
        // model+0x54 = 0; MSG signal 0x14213b050 (UI)
    }
}

}  // namespace

// @src 0x1400538f0
void syncFromModel(Project& p, Operation& op, bool updateDiagram, bool writeEqp, Ctx& ctx, FileEffects& fx)
{
    GuiModel* m = opModel(p, op);                           // 0x14005f770
    if (!m) return;
    ItemManager& mm = p.materials;                          // 0x140055110 (P+8 -> 0x140044dc0)
    withOpStep(p, op, [&](DB_STEP_DATA* s) {                // 0x14005f7a0
        const int nMat = s->n_materials;                        // S+0x1320
        for (int i = 0; i < nMat; ++i) {
            Material* mat = def::model::material_list_nth(&s->materials, i + 1);   // 0x140830460
            if (!mat) continue;
            int id = mat->number;
            if (!mm.find(id)) appendItem(p, mm, id);            // vt+0x1c8 / vt+0x188
            syncMaterialItem(p, op, id);                        // 0x140060c40
            const String f = managerFile(mm, id);               // vt+0x208 (0x14006d380)
            if (mat->mstmtr_flag != 2) {
                if (fx.nativeTaskKeys) saveMaterialKey(p, *s, id, f, fx);
                else captureWrite(f, fx, [&](const char* tmp) { // 0x1408959e0 = 1401c5ea0
                    def::key::write_material_key_file(*s, tmp, id, false, nullptr, nullptr);
                });
            }
        }
        // 0x140055280: material manager vt+0x220 (0x14006d700) dataChanged (UI)
        // max(0x1408bc4a0(S+0x1328), else 100) ints allocated and freed unused
        const int n = m->objectCount();                         // 0x140c7c1a0
        // 0x140060ee0: slot codes resized to n (dropped from the end / -1 appended)
        while (static_cast<int>(op.objIds.size()) > n) op.objIds.pop_back();
        while (static_cast<int>(op.objIds.size()) < n) op.objIds.push_back(-1);
        for (int k = 0; k < n; ++k) {
            LLOBJ_DATA* o = m->objectAt(k + 1);                 // 0x140c7aed0
            if (!o) continue;
            int code = slotCode(op, k);                         // 0x14005fb30
            if (code == -1 && *at<std::int32_t>(o, 0xc) != 7) {
                if (*at<std::int32_t>(o, 0xc) == 5) {
                    code = -2;
                } else {
                    code = newObjectItem(p);                    // object manager vt+0x180 (0x14006c520)
                    if (Item* it = p.objects.find(code)) setItemName(*it, String::latin1(at<char>(o, 0x828)));   // vt+0x40
                }
                addObjectAt(p, op, k, code, false, false, fx);  // 0x140057c60(opi, k, code, 0, 0, updateDiagram)
            }
            const int no = *at<std::int32_t>(o, 8);
            ObjEntry* r = objEntryN(op, no);                    // 0x1400612d0
            if (!r) unsupported("slot record of object number " + std::to_string(no) + " missing (0x1400612d0 result used unchecked)");
            r->name = String::latin1(at<char>(o, 0x828));       // 0x14004d700
            syncSlotMaterials(p, op, no);                       // 0x140064d40
            if (!def::dbio::objcheck::obj_has_movement(o)) {    // 0x14084d4a0 = 140179280
                r->assignMovement = false;                      // +0x1bf (0x14004e0e0)
                r->equipmentId = 0;                             // 0x14004deb0
            } else {
                r->assignMovement = true;
                int eq = r->equipmentId;                        // 0x14004de90
                if (eq < 1) {
                    eq = firstFreeItemId(p.equipment);          // 0x1400a5970 -> 0x140044e60, vt+0x180 = 0x14006c520
                    appendItem(p, p.equipment, eq);
                    r->equipmentId = eq;
                }
                if (writeEqp) saveEquipment(p, *s, o, managerFile(p.equipment, eq), fx);   // vt+0x208, 0x140358190(.., 1)
            }
        }
        refreshObjManager(p, fx);                               // 0x140069620
        if (mst::OpClass* ob = opBase(op)) {                    // 0x140054e90
            // overrides: MultiBlow 0x140602230, the Express family 0x1413fbd10 (thread L4), Convert2Dto3D
            // 0x1401c3570 (thread L5)
            ob->syncFromModelTail(p, op, ctx, fx);              // OB vt+0xc8 (base 0x1405f1e10: empty)
        }
        if (updateDiagram) {
            if (SubProject* sp = activeSubProject(p))           // 0x140055150: the operation's diagram model
                updateDiagramItem(p, sp->flow, op.id, *m);      // 0x140087270
            // UI: MSG "2signalUpdateUIInWidgetArea()"
        }
    });
}

}  // namespace m2m::engine
