// MDT L0: the operation loader of DEF_GUI_MO_64 (docs/re/moproj2mst/mdt/L0/spec_C.md activation,
// OB vt+0x58 and the mode-0 decision; spec_A.md the KEY load, materials and equipment; spec_B.md
// the object slots). One function per binary function, in the original's call order. UI-only
// calls (messenger signals, status bar, tree models, viewer) are dropped; each drop is named.
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <algorithm>
#include <cstdio>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <map>
#include "m2m/engine/file_effects.hpp"

#include <defcmd/key_file.hpp>
#include <defcmd/key_import.hpp>
#include <defcmd/geolib.hpp>
#include <defkey/item_writers.hpp>
#include <defmodel/gui_twins.hpp>
#include <defmodel/layout/cls_1401e7b10.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/layout/material.hpp>
#include <defmodel/llobj_ctor/llobj_copy.hpp>
#include <defmodel/llobj_ctor/llobj_ctor.hpp>
#include <defmodel/llobj/object_list.hpp>
#include <defmodel/geo_kw/geo_kw.hpp>
#include <defmodel/llobj/accessors.hpp>
#include <defmodel/mesh2d/geo2d_mesh_ops.hpp>
#include <defmodel/mesh2d/material_ops.hpp>
#include <defdbio/db_query.hpp>
#include <defdbio/dbread/dbread.hpp>
#include <defdbio/step/db_info_read.hpp>
#include <defmodel/layout/cls_1401dbfe0.hpp>
#include <defmodel/step/db_step.hpp>
#include <defdbio/gendb/checks_p4_04.hpp>
#include <defdbio/objcheck/objcheck.hpp>

#include "engine_internal.hpp"
#include "m2m/engine.hpp"
#include "m2m/engine/simctrl_ppv_open_sequence.hpp"
#include "m2m/objects.hpp"
#include "m2m/p17.hpp"
#include "m2m/mst/writer.hpp"
#include "m2m/qt4path.hpp"

namespace m2m::mst {
void installLoaderSlots();
}

namespace m2m::engine {

using def::model::Cls_1401e7b10;
using def::model::Material;
using qt::L;

std::string local8(const qt::String& s)
{
    const std::u16string& u = s.u16();
    if (u.empty()) return {};
    int n = WideCharToMultiByte(CP_ACP, 0, reinterpret_cast<const wchar_t*>(u.data()), static_cast<int>(u.size()),
                                nullptr, 0, nullptr, nullptr);
    std::string out(static_cast<std::size_t>(n > 0 ? n : 0), '\0');
    if (n > 0)
        WideCharToMultiByte(CP_ACP, 0, reinterpret_cast<const wchar_t*>(u.data()), static_cast<int>(u.size()),
                            out.data(), n, nullptr, nullptr);
    return out;
}

std::string engineKeyPath(const qt::String& s)
{
    std::string local = local8(s);
    if (local.size() >= MAX_PATH)
        throw Error(M2M_E_UNSUPPORTED, "the KEY file " + s.utf8() + " has a path of " + std::to_string(local.size()) +
                                           " characters; the engine's KEY reader (1401b5f00, _fsopen as in the "
                                           "GUI) cannot open paths of MAX_PATH (260) characters or more");
    return local;
}

namespace {

// QString::compare(.., Qt::CaseInsensitive) == 0 for the ASCII class names compared here
bool equalsCI(const String& a, const char16_t* b)
{
    const std::u16string& u = a.u16();
    std::size_t i = 0;
    for (; i < u.size() && b[i]; ++i) {
        char16_t x = u[i], y = b[i];
        if (x >= u'A' && x <= u'Z') x = static_cast<char16_t>(x + 32);
        if (y >= u'A' && y <= u'Z') y = static_cast<char16_t>(y + 32);
        if (x != y) return false;
    }
    return i == u.size() && b[i] == 0;
}

[[noreturn]] void unsupported(const std::string& what)
{
    throw Error(M2M_E_UNSUPPORTED, "operation load: " + what);
}

mst::OpClass* classOf(const Operation& op)
{
    return opBase(op);                     // 0x140054e90 (registry by OPI+0x120)
}

}  // namespace

// @src 0x140062960  predecessor operation in the active sub-project's chain, or null
Operation* prevOp(Project& p, const Operation& op)
{
    SubProject* sp = activeSubProject(p);
    if (!sp) return nullptr;
    const int id = flowPredecessor(sp->flow, op.id);   // 0x140086720
    if (id == -1) return nullptr;
    return operationById(p, id);                        // 0x140047dc0
}

// @src 0x14005fb30  slot code (OPI+0x40), -99 out of range
int slotCode(const Operation& op, int s)
{
    if (s < 0 || static_cast<std::size_t>(s) >= op.objIds.size()) return -99;
    return op.objIds[static_cast<std::size_t>(s)];
}


// @src 0x14005fd20  code of an inherited slot: the first predecessor with an own object
int resolvedCode(Project& p, const Operation& op, int s)
{
    if (slotCode(op, s) == -2)
        for (Operation* q = prevOp(p, op); q; q = prevOp(p, *q)) {
            const int c = slotCode(*q, s);
            if (c > 0) return c;
            if (c != -2) return -99;
        }
    return -99;
}

// SfMoObjManager / SfMoEqpManager vt+0x208 (0x14006d380): cleanPath(item+0x28), null if none
String managerFile(const ItemManager& m, int id)
{
    const Item* it = m.find(id);
    if (!it) return String();
    return qt::cleanPath(it->keyPath);
}

namespace {

// "%1%2.KEY" side file of a slot in an operation folder (0x14005cce0 family)
String slotFile(const Operation& op, const char* stem, int s)
{
    char buf[64];
    std::snprintf(buf, sizeof buf, "%s%05d.KEY", stem, s + 1);
    return qt::cleanPath(taskDirAbs(op) + qt::kSep + L(buf));
}

std::string readAllBytes(const std::filesystem::path& f)
{
    std::ifstream in(f, std::ios::binary);
    return std::string(std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>());
}

}  // namespace

// @src 0x14005f770 / 0x14005f820  the operation's engine model: the class's own (vt+0xe0), else
// OPI+0x38, created by class vt+0x1b0 when missing
GuiModel* opModel(Project& p, Operation& op)
{
    mst::OpClass* c = classOf(op);
    if (!c) return nullptr;
    if (!mst::g_loaderSlots.createModel) mst::installLoaderSlots();   // engine entry points other than 0x140047f80
    if (GuiModel* m = c->classModel(p, op)) return m;      // vt+0xe0
    if (!op.engineModel) {
        c->current = &op;                                   // 0x1405ef280
        op.engineModel = c->createModel(p, op);                // vt+0x1b0
    }
    return op.engineModel.get();
}

// 0x140c7a5b0(new 0x278, P+8, dim) and 0x140822210(E, 0x1400a2180(P), 1) = 1401c8f70: the start of
// every vt+0x1b0 body (0x1405f6410, 0x140e32a30, 0x14013fc20)
std::shared_ptr<GuiModel> newModel(Project& p, int dim)
{
    auto m = std::make_shared<GuiModel>(projectDb(p), dim);   // P+8 (0x1400a5890)
    def::key::set_model_unit(*m->step, p.unit, true);
    return m;
}

// 0x140062960 for the class slots outside the engine (mst::g_loaderSlots.prevOp)
Operation* previousOperation(Project& p, const Operation& op)
{
    return prevOp(p, op);
}

namespace {

// @src 0x1400599c0(op, false)  PDIE (and GEOTYP in 2D) from the nearest predecessor's SimCtrl.KEY
bool inheritSimCtrl(Project& p, Operation& op)
{
    return withOpStep(p, op, [&](DB_STEP_DATA* e) -> bool {
        if (!e) return false;
        for (Operation* q = prevOp(p, op); q; q = prevOp(p, *q)) {
            const String path = simCtrlPath(*q);               // 0x140059290
            if (!qt::fileExists(path)) continue;
            auto tmp = std::make_unique<DB_STEP_DATA>(3);       // 0x140821bf0
            const char reset = 1;
            const std::int32_t zero = 0;
            const std::string lp = engineKeyPath(path);
            const int rc = def::cmd::read_keyword_file(tmp.get(), lp.c_str(), &reset, &zero);   // 1408969a0
            if (rc != 0) return rc != 0;
            if (*at<std::int32_t>(e, 0x1c) == 2) e->sim_ctrl.geotyp = tmp->sim_ctrl.geotyp;   // S+0x100
            e->sim_ctrl.pdie1 = tmp->sim_ctrl.pdie1;                                           // S+0x168
            return rc == 0;
        }
        return false;
    });
}

// @src 0x1400594d0  SimCtrl.KEY (and SimCtrl.PPV) of the operation
bool loadSimCtrl(Project& p, Operation& op)
{
    return withOpStep(p, op, [&](DB_STEP_DATA* e) -> bool {
        if (!e) return false;
        const String key = simCtrlPath(op);
        if (!qt::fileExists(key)) return inheritSimCtrl(p, op);
        int rc = 0;
        if (op.oprFlag != 0) {                                  // !0x1400509d0
            const std::string lp = engineKeyPath(key);
            rc = def::cmd::key_import_sim_ctrl(e, lp.c_str(), true);   // 0x140897720 = 1401b6c80
        }
        const String ppv = qt::cleanPath(taskDirAbs(op) + qt::kSep + L("SimCtrl.PPV"));   // 0x1400593b0
        if (qt::fileExists(ppv)) {
            const auto status = simctrl_ppv::readPpvOpenSequenceFromDisk(
                p, ppv, op, prevOp(p, op) != nullptr);
            if (status == simctrl_ppv::OpenSequenceStatus::UnsupportedInput ||
                status == simctrl_ppv::OpenSequenceStatus::OwnerUnavailable)
                unsupported("SimCtrl.PPV file profile or model owner remains unqualified");
            // Native caller ignores the PPV reader result, then returns the KEY result.
        }
        return rc == 0;
    });
}

// @src 0x14005a7b0  InterObject.KEY of the operation
bool loadInterObject(Project& p, Operation& op)
{
    return withOpStep(p, op, [&](DB_STEP_DATA* e) -> bool {
        if (!e) return false;
        const String f = interObjectPath(op);                  // 0x14005a690
        if (!qt::fileExists(f)) return false;
        bool r = true;
        if (op.oprFlag != 0) {
            const std::string lp = engineKeyPath(f);
            r = def::cmd::key_import_inter_object(e, lp.c_str());   // 0x140897fd0 = 1401b7530
        }
        return r;
    });
}

}  // namespace

// Default file of a material id (SfMoMatManager vt+0x178 = 0x140042850); also used by packet S
String materialDefaultFile(const Project& p, int id)
{
    char buf[64];
    std::snprintf(buf, sizeof buf, "%s%05d.KEY", "Material", id);
    return qt::cleanPath(projectDir(p) + L("/Materials") + qt::kSep + L(buf));
}

namespace {

// A KEY the engine writes to a path: written to a scratch file and recorded as an effect
template <class Fn> void captureWrite(const String& target, FileEffects& fx, Fn write)
{
    const std::filesystem::path tmp = std::filesystem::temp_directory_path() /
                                      ("m2m_key_" + std::to_string(GetCurrentProcessId()) + "_" +
                                       std::to_string(GetCurrentThreadId()) + ".KEY");
    write(tmp.string().c_str());
    fx.writes.push_back({target, readAllBytes(tmp)});
    std::error_code ec;
    std::filesystem::remove(tmp, ec);
}

// @src 0x1400569d0 (materials part, spec_A section 1)
void loadMaterials(Project& p, Operation& op, GuiModel& w, FileEffects& fx)
{
    for (std::size_t i = 0; i < p.materials.rows.size(); ++i) {   // vt+0x1b8 / vt+0x1c0
        Item* it = p.materials.rows[i].get();
        if (!it) continue;
        if (!(it->matType == 0 || it->matType == 1)) continue;    // 0x140041e30 || 0x140041e70
        const int matId = it->id;
        const String matFile = managerFile(p.materials, matId);  // vt+0x208
        int reqId = matId;
        if (w.material(matId)) reqId = -1;                       // 0x140c7cb20
        Material* mat = w.readMaterial(matFile, &reqId);         // 0x140c7cf80
        if (mat && mat->number != matId) {
            const int newId = mat->number;
            it->id = newId;                                       // item vt+0x50
            it->keyPath = materialDefaultFile(p, newId);          // vt+0x1b0 (path = vt+0x178)
            const String f = materialDefaultFile(p, newId);       // vt+0x178
            if (fx.nativeTaskKeys) withOpStep(p, op, [&](DB_STEP_DATA* e) {
                saveMaterialKey(p, *e, newId, f, fx);
            });
            else captureWrite(f, fx, [&](const char* tmp) {
                withOpStep(p, op, [&](DB_STEP_DATA* e) {
                    def::key::write_material_key_file(*e, tmp, newId, false, nullptr, nullptr);   // 0x1408959e0
                });
            });
        }
    }
}

// @src 0x14005d200  MeshSettingsNNNNN.KEY of a slot
bool loadMeshSettingsOwn(Project& p, Operation& op, int s)
{
    return withOpStep(p, op, [&](DB_STEP_DATA* e) -> bool {
        if (!e) return false;
        const String path = slotFile(op, "MeshSettings", s);
        if (!qt::fileExists(path)) return false;
        const std::string lp = engineKeyPath(path);
        return def::cmd::key_import_mesh_settings(e, lp.c_str(), s + 1);   // 0x1408986a0 = 1401b7c00
    });
}

// @src 0x14005d460  MeshSettings of the nearest predecessor that defines them
bool loadMeshSettingsInherited(Project& p, Operation& op, int s)
{
    return withOpStep(p, op, [&](DB_STEP_DATA* e) -> bool {
        if (!e) return false;
        for (Operation* q = prevOp(p, op); q; q = prevOp(p, *q)) {
            if (equalsCI(q->classKey, u"SfOprConvert2Dto3D")) return false;
            ObjEntry* li = objEntry(*q, s);
            String path = L("");
            if (li && li->meshSetting) path = slotFile(*q, "MeshSettings", s);
            if (qt::fileExists(path)) {
                const std::string lp = engineKeyPath(path);
                return def::cmd::key_import_mesh_settings(e, lp.c_str(), s + 1);
            }
        }
        return false;
    });
}

// @src 0x14005eda0  ObjPropertiesNNNNN.KEY of a slot
bool loadObjProperties(Project& p, Operation& op, int s)
{
    return withOpStep(p, op, [&](DB_STEP_DATA* e) -> bool {
        if (!e) return false;
        const String path = slotFile(op, "ObjProperties", s);
        if (!qt::fileExists(path)) return false;
        const std::string lp = engineKeyPath(path);
        return def::cmd::key_import_obj_properties(e, lp.c_str(), s + 1);   // 0x140898490 = 1401b79f0
    });
}

// @src 0x1400601b0  slot record <- engine object. The record is a slot's ObjEntry or the record of
// the project's object item (SfMoObjItem+0x40). A kind-5 object in a slot whose code is not -2 makes
// the slot "Read From DB" (-2); a slot coded -2 whose object is not kind 5 gets a new object item;
// both through 0x140057c60 with reload, view and signal (MDT L4S, on L7's addObjectAt).
struct SlotRecRef {
    int* type4;
    int* type8;
    String* name;
    bool* hasGeo;
    bool* hasMesh;
    bool* b1c0;
};

int syncSlotItem(Project& p, Operation& op, SlotRecRef r, LLOBJ_DATA* obj, FileEffects& fx)
{
    if (!obj) return 0;
    const int no = *at<std::int32_t>(obj, 8);
    *r.type4 = *at<std::int32_t>(obj, 0xc);
    if (slotCode(op, no - 1) > 0) *r.type8 = *at<std::int32_t>(obj, 0xc);   // 0x14005fc50
    *r.name = String::latin1(at<char>(obj, 0x828));
    *r.hasGeo = objHasGeometry(obj);
    *r.hasMesh = objHasMesh(obj);
    *r.b1c0 = *at<std::uint8_t>(obj, 0x2bd8) != 0;                           // 0x14004e120
    const int s = no - 1;
    if (slotCode(op, s) != -2) {                                             // 0x14005fb30, 0x1400606c0
        if (*at<std::int32_t>(obj, 0xc) == 5)
            addObjectAt(p, op, s, -2, true, true, fx);                       // 0x140057c60(op, s, -2, 1, 1, 1)
    } else if (*at<std::int32_t>(obj, 0xc) != 5) {
        const int id = newObjectItem(p);                                     // 0x1400550d0 vt+0x180
        addObjectAt(p, op, s, id, true, true, fx);                           // 0x140057c60(op, s, id, 1, 1, 1)
    }
    return 1;
}

// @src 0x14005b030  object from its own object file
LLOBJ_DATA* loadOwn(Project& p, Operation& op, GuiModel& m, int s, bool bare, FileEffects& fx)
{
    LLOBJ_DATA* obj = nullptr;
    const int code = slotCode(op, s);
    const int no = (s >= 0 && static_cast<std::size_t>(s) < op.objIds.size()) ? s + 1 : 0;   // 0x14005fc10
    if (code > 0) {
        const String path = managerFile(p.objects, code);
        if (!qt::fileExists(path)) return nullptr;
        if (!m.readKey(path)) return nullptr;              // 1408969a0 (reset 1, mode 0)
        obj = m.objectWrap(no, false, true);
        if (bare) return obj;
        if (ObjEntry* li = objEntry(op, s)) {
            if (li->meshSetting) loadMeshSettingsOwn(p, op, s);
            loadObjProperties(p, op, s);
        }
        if (Item* lib = objItem(op, code))                 // 0x140061360: the item's object record
            syncSlotItem(p, op, {&lib->objectType, &lib->recF08, &lib->recName, &lib->hasGeo, &lib->hasMesh, &lib->recB1c0},
                         obj, fx);
    }
    viewerSync(m, obj, true);                              // 0x140c80280 (no-op for null)
    return obj;
}

// @src 0x14005c890: read one object of a DB step into an existing slot.
// Step selection uses the model's reader, but this read opens the active
// sub-project's resolved DB path with a separate temporary reader/model.
bool copyInheritedObjectFromDbStep(Project& p, Operation& op, int no, int step, LLOBJ_DATA* dst)
{
    GuiModel* model = opModel(p, op);                       // 0x14005f770
    if (!model || !modelDbReader(*model)) return false;    // 0x140c7ac50
    SubProject* sub = activeSubProject(p);                 // 0x140055050
    if (!sub) return false;
    const String path = subProjectDbPath(*sub, true);      // 0x1400b9e80 (also stores generated DbName)
    if (!qt::fileExists(path)) return false;
    DB_STEP_DATA temporary(3);                            // 0x140821bf0
    def::model::Cls_1401dbfe0 reader;                     // 0x14080c660
    const std::string local = local8(path);               // QString::toLocal8Bit
    if (def::dbio::load_db_info(reader, local.c_str(), false) != 0) return false;
    if (def::dbio::query::has_previous_part(reader))      // 0x140814e80
        def::dbio::query::setup_part_chain(reader);       // 0x1408150f0, no question on this path
    if (def::dbio::dbread::fun_1401cc2a0(&temporary, &reader, step, 1, 0) != 0) return false;
    LLOBJ_DATA* src = def::model::object_list_find(&temporary.objects, no);  // 0x140830270
    if (!src) return false;
    const bool yes = true, noNumber = false;
    // 0x140835430: copy name, retain destination number; array sizes belong
    // to the loaded temporary, not the destination model.
    def::model::llobj_copy_from(dst, src, temporary.u15f8, &yes, &yes, &yes, &yes, &noNumber, &yes);
    def::model::bcc_groups_from_model(dst, temporary.u15f8);  // 0x14087e1a0
    return true;  // original preserves this byte across reader/model cleanup
}

// @src 0x14005cc00: refresh before querying, even when sim < 1. A failed
// refresh does not suppress the query of any cache the reader already holds.
bool inheritedObjectFromDb(Project& p, Operation& op, int no, int sim, bool last, LLOBJ_DATA* dst)
{
    GuiModel* model = opModel(p, op);
    if (!model || !model->db || !modelDbReader(*model)) return false;
    (void)dbRefreshIfChanged(*model->db);                   // 0x140d397a0
    if (sim < 1) return false;
    const auto* reader = modelDbReader(*model);
    const int step = last ? def::dbio::query::sim_last_step(*reader, sim)   // 0x14080eb30
                          : def::dbio::query::sim_first_step(*reader, sim); // 0x14080ea20
    return step != 0 && copyInheritedObjectFromDbStep(p, op, no, step, dst);
}

// @src 0x14005b2f0  object inherited from previous operations (or the project DB)
LLOBJ_DATA* loadInherited(Project& p, Operation& op, GuiModel& m, int s, bool bare)
{
    LLOBJ_DATA* dst = nullptr;
    const int no = s + 1;
    int code = resolvedCode(p, op, s);
    if (code > 0) {
        dst = def::model::llobj_ctor::get_or_create(m.step.get(), no);   // 1408bde10
        bool fromDb = false;
        const String cls = op.classKey;
        if (!isChainHead(op)) {
            SubProject* sub = activeSubProject(p);
            const int sim = sub ? mst::nextSim(p, *sub, flowNumber(op)) - 1 : -1; // 0x140050b70
            fromDb = inheritedObjectFromDb(p, op, no, sim, true, dst);
        }
        if (!fromDb) {
            const String path = managerFile(p.objects, code);
            if (!qt::fileExists(path)) return nullptr;
            bool handled = false;
            for (Operation* q = prevOp(p, op); q; q = prevOp(p, *q)) {
                mst::OpClass* c = classOf(*q);
                // class vt+0x2a0 with q as the class's current operation (0x1405ef2b0 / 0x1405ef280):
                // SfOprBase 0x1405f5b70 false, SfOprCopyMirror 0x140e25980 (MDT L5)
                if (c) {
                    const auto saved = c->current.snapshot();
                    c->current = q;
                    handled = c->provideObject(p, no, dst);
                    c->current.restore(saved); // preserve normal-return-only adapter restoration
                    if (handled) break;
                }
                const int pc = slotCode(*q, s);
                if (pc > 0) { code = pc; break; }
                if (pc != -2) break;
            }
            if (!handled) {
                const bool c23 = cls == L("SfOprConvert2Dto3D");
                auto sc = std::make_unique<GuiModel>(nullptr, c23 ? 2 : 3);   // 0x140c7a6b0 / 0x140c7a5b0
                if (c23) {
                    const int n1 = s + 1;
                    sc->loadObjectKey(path, &n1);                            // 0x140c7cd90
                } else if (equalsCI(cls, u"SfOprHeatTreat") ||
                           equalsCI(cls, u"SfOprHeatTreat2D")) {
                    (void)sc->readKey(path);                                 // 1408969a0, result ignored
                } else {
                    const bool mark5 = false;
                    const int n1 = s + 1;
                    sc->loadObjectKeyGeometry(path, &n1, &mark5);            // 0x140c7ce40
                }
                LLOBJ_DATA* src = sc->object(s + 1);
                if (!src) return nullptr;
                dst = def::model::llobj_ctor::get_or_create(m.step.get(), s + 1);
                const bool opIs3D = !classOf(op) || classOf(op)->dim() == 3;  // 0x14005f1c0
                if (*at<std::int32_t>(src, 0x39c0) == 2 && opIs3D)
                    unsupported("a 2D object inherited into a 3D operation (0x14005bd30 runs FIXGEO.EXE and "
                                "DEF_GEOPRIM3.EXE)");
                def::model::llobj_copy_keep_number(dst, src);                // 140835800 = 140161770
                std::strcpy(at<char>(dst, 0x828), at<char>(src, 0x828));
            }
        }
        if (!dst) return nullptr;
        const int oldType = *at<std::int32_t>(dst, 0xc);
        *at<std::int32_t>(dst, 0xc) = 5;
        if (bare) return dst;
        if (ObjEntry* li = objEntry(op, s)) {
            li->objectType = *at<std::int32_t>(dst, 0xc);
            li->f08 = oldType;
            li->name = String::latin1(at<char>(dst, 0x828));
            if (li->meshSetting) loadMeshSettingsOwn(p, op, s);
            else loadMeshSettingsInherited(p, op, s);
            if (li->defineBCC) unsupported("inherited object with a BCC definition (0x14005dd20)");
            loadObjProperties(p, op, s);
        }
    }
    if (!dst) return nullptr;
    *at<std::int32_t>(dst, 0xc) = 5;
    // HeatTransferExpress: movement reset of a moving inherited object (0x14085f6d0)
    if (op.classKey.u16().find(u"SfOprHeatTransferExpress") == 0 && def::dbio::objcheck::obj_has_movement(dst))
        unsupported("movement reset of an inherited object in a HeatTransferExpress operation (0x14085f6d0)");
    viewerSync(m, dst, true);                              // 0x140c80280
    return dst;
}

// @src 0x14005aec0  one slot
LLOBJ_DATA* loadSlot(Project& p, Operation& op, GuiModel& m, int s, FileEffects& fx)
{
    const int code = slotCode(op, s);
    if (code > 0) return loadOwn(p, op, m, s, false, fx);
    if (code == -2) return loadInherited(p, op, m, s, false);
    return nullptr;
}

// ---- MDT L7: adding an object to an operation (0x140057ba0) -----------------------------------------

// @src 0x140060530  first slot whose code is -1 or -99, else the slot count
int firstFreeSlot(const Operation& op)
{
    const int n = static_cast<int>(op.objIds.size());    // 0x14005fa80
    for (int s = 0; s < n; ++s) {
        const int c = slotCode(op, s);
        if (c == -1 || c == -99) return s;
    }
    return n;
}

// @src 0x140060d60  slots follow the model's objects: one per object, an undefined object (kind 7)
// frees its slot (-1), then trailing free slots (-1 / -99) are dropped
void trimSlots(Project& p, Operation& op)
{
    if (GuiModel* m = opModel(p, op)) {
        const int n = m->objectCount();
        // 0x140060ee0: resize to n (drop the last / append -1)
        if (static_cast<int>(op.objIds.size()) != n) {
            while (static_cast<int>(op.objIds.size()) > n) op.objIds.pop_back();   // 0x140066800
            while (static_cast<int>(op.objIds.size()) < n) op.objIds.push_back(-1);
        }
        for (int i = 0; i < n; ++i) {
            LLOBJ_DATA* o = m->objectAt(i + 1);
            if (o && *at<std::int32_t>(o, 0xc) == 7 && slotCode(op, i) != -1) setObjId(op, i, -1);
        }
    }
    int cnt = static_cast<int>(op.objIds.size());
    bool kept = false;
    do {
        if (cnt - 1 >= 0) {
            const int c = slotCode(op, cnt - 1);
            if (c == -1 || c == -99) op.objIds.pop_back();      // 0x140066800
            else kept = true;
        }
        cnt = static_cast<int>(op.objIds.size());
    } while (!kept && cnt != 0);
}

// SfMoObjManager vt+0x178 (0x140068e60) file of an object id, and whether the GUI would find it:
// the item Data the GUI writes at open is in the overlay here (s1-01 Q1)
bool objFileExists(const Project& p, const String& file)
{
    if (file.isEmpty()) return false;
    if (p.overlay.count(qt::cleanPath(file))) return true;
    return qt::fileExists(file);
}

// 81a6e0 and055360: QFile removal is attempted at the call site and its
// failure does not prevent the caller's following row/state transitions.
void removeLoaderFile(Project& p, const String& file, FileEffects& fx, bool existsAtCall = false)
{
    if (file.isEmpty()) return;
    if (!fx.nativeRemovals) {
        if (!objFileExists(p, file)) return;
        fx.deletes.push_back(file);
        p.overlay.erase(qt::cleanPath(file));
        return;
    }
    if (!existsAtCall && !qt::fileExists(file)) return;
    auto native = qt::convertSeparators(file).u16();
    const bool driveAbsolute = native.size() >= 3 &&
        ((native[0] >= u'A' && native[0] <= u'Z') || (native[0] >= u'a' && native[0] <= u'z')) &&
        native[1] == u':' && native[2] == u'\\';
    if (!driveAbsolute || native.size() >= MAX_PATH || native.find(u'\0') != std::u16string::npos)
        unsupported("native QFile removal requires an ordinary absolute path below MAX_PATH");
    const auto clean = qt::cleanPath(file);
    for (const auto& write : fx.writes)
        if (qt::cleanPath(write.path) == clean)
            unsupported("native QFile removal follows an unresolved deferred write to the same path");
    if (fx.beforeNativeRemoval) fx.beforeNativeRemoval(file);
    // Qt4 ordinary filesystem leaf670ff860. No CRT open probe, long-path
    // extension, exception on denial or delayed retry at applyEffects.
    const bool removed = DeleteFileW(reinterpret_cast<const wchar_t*>(native.c_str())) != 0;
    fx.removalAttempts.push_back({file, removed});
    if (removed) p.overlay.erase(clean);
}

}  // namespace

void removeProjectFileAtCall(Project& p, const String& file, FileEffects& fx)
{
    removeLoaderFile(p, file, fx);
}

// @src 0x140069620  SfMoObjManager refresh: 0x1400a5c10 recomputes which operations use each object
// item (vt+0x210 clears the sets, then every positive slot code of operations 1..max id adds its
// operation), 0x14006d490 removes every item no operation uses (vt+0x1a0: vt+0x1a8 deletes its
// file 0x14081a6e0, then takeRow)
void refreshObjManager(Project& p, FileEffects& fx)
{
    ItemManager& objs = p.objects;
    std::map<int, int> used;                              // item+0x38 QSet<int> sizes, by item id
    int maxOp = 0;                                        // SfMoOprManager vt+0x1d8 (0x14006cfa0)
    for (const auto& o : p.operations)
        if (o && o->id > maxOp) maxOp = o->id;
    for (int id = 1; id <= maxOp; ++id) {
        Operation* q = operationById(p, id);              // 0x140047dc0
        if (!q) continue;
        const int n = static_cast<int>(q->objIds.size());
        for (int s = 0; s < n; ++s) {
            const int c = slotCode(*q, s);
            if (c > 0 && objs.find(c)) ++used[c];         // 0x14006bc20: insert into the set
        }
    }
    int maxObj = 0;                                       // SfMoObjManager vt+0x1d8
    for (const auto& it : objs.rows)
        if (it && it->id > maxObj) maxObj = it->id;
    for (int id = 1; id <= maxObj; ++id) {
        if (!objs.find(id) || used.count(id)) continue;   // vt+0x1c8, 0x14006bba0
        auto row = std::find_if(objs.rows.begin(), objs.rows.end(),
                                [id](const std::unique_ptr<Item>& it) { return it && it->id == id; });   // vt+0x1f0
        if (row == objs.rows.end()) continue;
        const String file = itemKeyPath(p, ItemKind::Object, id);   // vt+0x1a8 -> vt+0x178
        removeLoaderFile(p, file, fx);                   // ignored result; row removal still follows
        objs.rows.erase(row);                             // takeRow, items deleted
    }
}

// SfMoObjManager vt+0x180 (0x14006c520): the smallest unused id from 1 (vt+0x168 0x14006c230), then
// vt+0x188 (0x14006c570) appends a new SfMoObjItem (vt+0x170 0x140068df0) with its file (vt+0x178)
int newObjectItem(Project& p)
{
    ItemManager& objs = p.objects;
    int id = 1;
    while (objs.find(id)) ++id;
    auto it = std::make_unique<Item>();
    initObjItem(*it, id);                                 // 0x1400688f0
    it->keyPath = itemKeyPath(p, ItemKind::Object, id);   // 0x14006ba30
    objs.rows.push_back(std::move(it));                   // invisibleRootItem()->appendRow
    return id;
}

// @src 0x140060100  slot record of object no and the record of its object item <- engine object
int refreshSlotItems(Project& p, Operation& op, int no, FileEffects& fx)
{
    ObjEntry* li = objEntryN(op, no);                     // 0x1400612d0
    GuiModel* m = opModel(p, op);                         // 0x140057710
    LLOBJ_DATA* o = m ? m->object(no) : nullptr;
    const int code = slotCode(op, no - 1);                // 0x14005fc50
    if (code > 0) {                                       // 0x140060690
        if (Item* lib = objItem(op, code))                // 0x140061360
            syncSlotItem(p, op, {&lib->objectType, &lib->recF08, &lib->recName, &lib->hasGeo, &lib->hasMesh, &lib->recB1c0},
                         o, fx);
    }
    if (!li) return 0;
    return syncSlotItem(p, op, {&li->objectType, &li->f08, &li->name, &li->hasGeo, &li->hasMesh,
                             &li->defineRotationMovement},
                        o, fx);
}

// @src 0x140057c60  put object id into slot s and create or load its engine object (fromFile: from
// the slot's KEY data, else a default object). UI parts dropped: messenger signals, 0x140054820,
// 0x140055220 (manager dataChanged), 0x140054c20 (class vt+0x270: the object tree for SfOprBase
// 0x1405f1570)
LLOBJ_DATA* addObjectAt(Project& p, Operation& op, int s, int id, bool fromFile, bool ui, FileEffects& fx)
{
    GuiModel* m = opModel(p, op);
    if (!m) return nullptr;
    (void)slotCode(op, s);                                // 0x14005fb30, result unused
    const int no = s + 1;
    setObjId(op, s, id);                                  // 0x14005fc80 (P12)
    LLOBJ_DATA* o = nullptr;
    if (!fromFile) {
        o = m->objectWrap(no, true, true);                // 0x140c7c950(m, no, 1, 1)
        if (id == -2) *at<std::int32_t>(o, 0xc) = 5;      // 0x1400606c0, no null check in the binary
        if (o) {
            *at<double>(o, 0x2bf0) = 360.0;               // .rdata 0x1421249c8
            *at<std::uint64_t>(o, 0x2bf8) = 0;
        }
    } else if (id > 0 || id == -2) {                      // 0x140060640
        o = loadSlot(p, op, *m, s, fx);                   // 0x14005aec0(op, m, s, 0)
    }
    refreshSlotItems(p, op, no, fx);                      // 0x140060100 (true -> 0x140055220, UI)
    if (ui) {
        mst::OpClass* c = classOf(op);
        if (c && (c->key() == L("SfOprPre") || c->key() == L("SfOprPre2D")))
            unsupported("object tree of " + c->key().utf8() + " (vt+0x270 0x141348ec0 / 0x1413479c0)");
    }
    refreshObjManager(p, fx);                             // 0x140069620(0x1400550d0(op))
    // ui: MSG signalObjDataAdded(id, s, 0x14005ff30 name) -> the diagram's object row s
    if (ui) diagramObjDataAdded(p, op.id, s, objDisplayName(op, id));   // 0x14007b630
    return o;
}

// @src 0x140057ba0  add one object to the operation: slots trimmed to the model, a new object item,
// its id in the first free slot (0x140057c60(op, slot, id, 0, 1, 1)), then the manager refresh
LLOBJ_DATA* addObject(Project& p, Operation& op, FileEffects& fx)
{
    if (!op.project) return nullptr;                      // 0x1400550d0: no object manager
    trimSlots(p, op);                                     // 0x140060d60
    const int id = newObjectItem(p);                      // manager vt+0x180
    const int s = firstFreeSlot(op);                      // 0x140060530
    LLOBJ_DATA* o = addObjectAt(p, op, s, id, false, true, fx);
    refreshObjManager(p, fx);                             // 0x140069620
    return o;
}

namespace {

// @src 0x1403568a0(target, file, obj, 0, noDialog = 1, &objNo)  equipment KEY onto an object
int loadEquipment(GuiModel& target, const String& file, LLOBJ_DATA* tgt, int objNo)
{
    auto scr = std::make_unique<GuiModel>(nullptr, 3);      // 0x140c7a5b0(mem, 0, 3)
    // 0x1402ca7a0: wait cursor (UI)
    if (!scr->readKey(file)) return 0;                      // 0x140c7d020
    const int n = scr->objectCount();
    for (int k = 0; k < n; ++k) {
        LLOBJ_DATA* o = scr->objectAt(k + 1);
        if (o && *at<std::int32_t>(o, 0xc) == 7 && *at<std::int32_t>(o, 0x2360) > 0) *at<std::int32_t>(o, 0xc) = 0;
    }
    LLOBJ_DATA* src = objNo == 0 ? nullptr : scr->object(objNo);
    if (objNo == 0) unsupported("equipment without an object number (0x140c7c3b0)");
    if (!src) return 0;
    def::model::llobj_copy::movement_copy(at<std::byte>(tgt, 0x2160), at<const std::byte>(src, 0x2160));   // 14018bd30
    const int mt = *at<std::int32_t>(tgt, 0x21c8);
    if (mt == 3 || mt == 4)
        std::memcpy(at<std::byte>(target.simCtl(), 0x1b0), at<const std::byte>(scr->simCtl(), 0x1b0), 0x18);
    if (mt == 7 || mt == 8 || mt == 9 || mt == 10) {
        std::memcpy(at<std::byte>(target.simCtl(), 0x198), at<const std::byte>(scr->simCtl(), 0x198), 0x18);
        std::memcpy(at<std::byte>(target.simCtl(), 0x1b0), at<const std::byte>(scr->simCtl(), 0x1b0), 0x18);
    }
    return 1;
}

}  // namespace

// @src 0x14005af80  one slot without its side files (CopyMirror's vt+0x2a0 calls it)
LLOBJ_DATA* loadSlotBare(Project& p, Operation& op, GuiModel& m, int s)
{
    LLOBJ_DATA* o = nullptr;
    const int code = slotCode(op, s);
    FileEffects none;                                       // bare: loadOwn returns before syncSlotItem
    if (code > 0) o = loadOwn(p, op, m, s, true, none);     // 0x140060690, 0x14005b030
    else if (code == -2) o = loadInherited(p, op, m, s, true);   // 0x1400606c0, 0x14005b2f0
    return o;
}

// @src 0x1400569d0  KEY (batch) load of the operation
GuiModel* loadFromKey(Project& p, Operation& op, bool withObjects, FileEffects& fx)
{
    GuiModel* w = opModel(p, op);
    if (!w) return nullptr;
    if (!activeSubProject(p)) return nullptr;               // 0x140055050, 0x1400b9cd0 (P+8 always set)
    (void)flowNumber(op);                                   // 0x140050b20, result unused
    Cls_1401e7b10* sc = w->simCtl();
    if (op.nstartGiven && sc) sc->nstart = op.startStepNo;  // OPI+0x18 / +0x1c
    loadSimCtrl(p, op);
    const int savedPdie = w->pdie();
    loadInterObject(p, op);
    loadMaterials(p, op, *w, fx);
    if (withObjects) {
        // 0x1400a3540: looks up every operation item of the sub-project (no state change)
        const int ns = static_cast<int>(op.objIds.size());  // 0x14005fa80
        for (int s = 0; s < ns; ++s) {
            const int code = slotCode(op, s);
            const int no = s + 1;
            if (code == -1) {
                *at<std::int32_t>(w->objectWrap(no, false, true), 0xc) = 7;
                continue;
            }
            if (code == -2) *at<std::int32_t>(w->objectWrap(no, false, true), 0xc) = 5;
            else if (code == -99) continue;
            LLOBJ_DATA* o = loadSlot(p, op, *w, s, fx);
            if (!o) continue;
            ObjEntry* li = objEntry(op, s);
            if (li && li->matIds.empty()) {
                // 0x140060be0(op, s, -1): the slot's object gets material group -1 (14004db80 on an
                // empty list; spec_A Q6)
                LLOBJ_DATA* so = (slotCode(op, s) > 0 || slotCode(op, s) == -2) ? w->object(s + 1) : nullptr;
                if (so) def::cmd::geolib::fun_1402027e0(so, -1, false, true);   // 0x140876640(o, g, 0, 1)
            }
            *at<std::uint8_t>(w->objectWrap(no, false, true), 0x2bd8) = li->defineRotationMovement ? 1 : 0;
        }
    }
    w->setPdie(savedPdie);
    const int ns2 = static_cast<int>(op.objIds.size());
    for (int j = 0; j < ns2; ++j) {
        const int no = j + 1;
        LLOBJ_DATA* obj = w->object(no);                    // 0x140057710
        ObjEntry* r = objEntry(op, j);
        if (r && obj && *at<std::int32_t>(obj, 8) == no && r->equipmentId > 0) {
            const String f = managerFile(p.equipment, r->equipmentId);
            loadEquipment(*w, f, obj, *at<std::int32_t>(obj, 8));
        }
    }
    return w;
}

// @src 0x140055360  mode-0 decision (restart, setup type), then the KEY, restart or DB-step load
void decideAndLoad(Project& p, Operation& op, bool withObjects, Ctx& ctx, FileEffects& fx)
{
    int step = 0;
    const int idx = flowNumber(op);                         // 0x140050b20
    SubProject* sp = activeSubProject(p);                   // 0x140055050
    if (!sp) return;
    DbHandle* dbh = subProjectDb(p, *sp);                   // 0x1400b9cd0
    if (!dbh) return;
    mst::OpClass* ob = classOf(op);
    if (!ob) return;
    if (isChainHead(op) || !ob->restartCapable()) {
        op.restartGiven = 0;                                // 0x140064760(opi, false, true): state only
        loadFromKey(p, op, withObjects, fx);
        return;
    }
    const String rf = restartKeyPath(op, true);             // 0x140064a50
    if (op.restartGiven && qt::fileExists(rf)) {
        // 0x140064760(opi, true, true): unchanged, no signal
        loadRestart(p, op, ctx, fx);                        // 0x140057230
        return;
    }
    if (qt::fileExists(rf)) {
        if (fx.nativeRemovals) removeLoaderFile(p, rf, fx, true);
        else fx.deletes.push_back(rf); // existing recording profile
    } // ignored QFile::remove before later decisions
    if (op.restartGiven) {
        // QMessageBox::critical "Missing Restart File" (OK only), then restart off
        ctx.warn("Missing Restart File: Restart mode is ON, but there is no restart file. Restart mode will be "
                 "deactivated. (" + op.name.utf8() + ")");
        op.restartGiven = 0;
    }
    // Setup-type branch (spec_C 3.1): an operation never set up (OprFlag 0, 0x1400509d0) whose sub-project
    // database holds steps for it (0x14005f520) makes the GUI ask "Setup Type (Interactive/Batch)".
    // Documented deviation (Aleksandr, 2026-10-22): the converter takes the dialog's default button
    // (setDefaultButton(Yes), as a fresh user pressing Enter; "Keep my choice" 0x143d77584 is 0 in a
    // fresh process), i.e. "YES - Interactive Setup": load from the DB step (0x14080e710 /
    // 0x14080e5e0, then 0x1400572e0 -> 0x1400538f0). Oracle B 124_B_DBSTEP: Enter on the dialog,
    // then Save, gives the state this branch leaves.
    bool interactive = false, useDb = false, lastOfPrev = false;
    if (op.oprFlag == 0) {
        int lastMatch = 0, unused = 0;
        if (dbHasStepsFor(p, op, &lastMatch, &unused)) {   // 0x14005f520
            ctx.warn("Setup Type (Interactive/Batch): [Op. " + std::to_string(idx) + "] " + op.name.utf8() +
                     ": stored step in database; answered with the default button \"YES - Interactive Setup\"");
            interactive = true;                             // box.exec() == Yes (default button)
            if (interactive) {
                useDb = true;
                def::model::Cls_1401dbfe0& r = *dbh->reader;  // 0x140cdab70
                if (!(op.classKey == L("SfOprConvert2Dto3D"))) {
                    if (lastMatch < idx) {
                        step = def::dbio::query::op_last_step(r, idx - 1);    // 0x14080e710 = 1401de110
                        lastOfPrev = true;
                    } else {
                        step = def::dbio::query::op_first_step(r, idx);       // 0x14080e5e0 = 1401ddfe0
                        lastOfPrev = false;
                    }
                } else {
                    step = def::dbio::query::op_last_step(r, idx - 1);
                    lastOfPrev = true;
                }
            }
        }
    }
    if (!useDb || step == 0) {
        loadFromKey(p, op, withObjects, fx);                // 0x1400569d0
        op.restartGiven = 0;                                // 0x140064760(opi, false, true), after the load
        return;
    }
    // ---- DB branch ----
    // 0x1408d3bc0 / 0x1408d3d70: a stack sim control that keeps the model's SC over the load only when
    // guiMode() == 1 (0x143d9da20 is 0 after a fresh start), so it is never filled or restored here
    GuiModel* m = loadDbStep(p, op, step, true, ctx, fx);  // 0x1400572e0
    if (!m || !m->simCtl())
        unsupported("DB-step load without an engine model (0x140c7b2d0(m) is used unchecked after 0x1400572e0)");
    Cls_1401e7b10* sc = m->simCtl();
    int ns = sc->nstart;                                    // SC+0x160 as read from the DB step
    if (ns < 1) {
        if (lastOfPrev) ns = ns - 1;
    } else {
        ns = -(ns + 1);
    }
    sc->nstart = ns;
    m->step->u744 = step;                                   // 0x140c7acd0(m)->+0x744
    sc->tnow3 = 1;                                          // SC+0x650
    sc->tnow2 = 0.0;                                        // SC+0x648
    op.nstartGiven = 1;                                     // 0x14005a2c0
    op.startStepNo = ns;                                    // 0x14005a350
    const String cn = op.classKey;                          // 0x140050d00
    const bool ht = qt::contains(cn, L("SfOprHeatTransferExpress")) || qt::contains(cn, L("SfOprHeatExpress")) ||
                    qt::contains(cn, L("SfOprAirConvect")) || qt::contains(cn, L("SfOprHeatResting")) ||
                    qt::contains(cn, L("SfOprHeatDwelling"));
    if (ht) {
        const double tmax = sc->tmax;                       // SC+0x178
        def::model::sim_ctrl_init_u170(*sc);                // 0x1408d5100 = 1401e8fb0
        sc->tmax = tmax;
        sc->trans[1] = 0;                                   // 0x1408e0000(SC, 0): SC+0x108
        sc->trans[0] = 1;                                   // 0x1408dffe0(SC, 1): SC+0x104
    }
    // UI: status bar and message log "Start step number has been set to %1" (0x1402d0bd0)
    op.restartGiven = 1;                                    // 0x140064760(opi, true, true)
}


// @src 0x1405f6520  SfOprBase vt+0x1b8: GUI mode 0x143d9da20 is 0 after a fresh start (set only by
// the Interactive Mode actions 0x140062f00 / 0x1400636d0), so the mode-0 decision runs
void baseLoadData(Project& p, Operation& op, Ctx& ctx, FileEffects& fx)
{
    if (!classOf(op) || !classOf(op)->current) return;    // 0x1405ef2b0
    decideAndLoad(p, op, true, ctx, fx);                   // 0x140055360(opi, true)
}

// @src 0x1405ef430  SfOprBase load (OB vt+0x58)
void baseLoad(Project& p, Operation& op, Ctx& ctx, FileEffects& fx)
{
    mst::OpClass* ob = classOf(op);
    GuiModel* m = opModel(p, op);
    if (!m) return;
    m->reset();
    ob->loadPrologue(p, op);                                // vt+0xb8
    ob->loadClassData(p, op);                               // vt+0x78
    ob->loadData(p, op, ctx, fx);                          // vt+0x1b8
    ob->postLoad(p, op, ctx, fx);                           // vt+0xc0
    ob->propsLoad(p, op);                                   // 0x140054350 (props vt+0x58)
    postLoadViews(p, op, ctx, fx);                          // 0x1400547b0(op, 0) (packet S)
    // 0x140054250: viewport name (UI)
}

namespace {

// @src 0x140064e60  slot materials from the mesh: distinct positive element materials
bool slotMaterialsFromMesh(Project& p, Operation& op, LLOBJ_DATA* o, ObjEntry* li)
{
    if (!objHasMesh(o)) return false;
    return withOpStep(p, op, [&](DB_STEP_DATA* e) -> bool {
        std::int32_t n = 0;
        std::int32_t cap = def::model::material_list_count(&e->materials);    // 0x1408bc4a0 = 14029c410
        if (cap < 1) cap = 100;
        std::vector<std::int32_t> buf(static_cast<std::size_t>(cap));
        const bool ok = def::dbio::gendb::object_material_ids(o, &n, buf.data(), &cap, e);   // 14086f6a0 = 1401fb840
        if (!ok) return false;
        li->matIds.clear();                                                     // 0x14004db20
        for (std::int32_t i = 0; i < n; ++i) {
            const int v = buf[static_cast<std::size_t>(i)];
            if (v > 0 && std::find(li->matIds.begin(), li->matIds.end(), v) == li->matIds.end())
                li->matIds.push_back(v);                                        // 0x14004dc20 (add unique)
        }
        return true;
    });
}

// @src 0x140065020  slot materials from the faces of geometry 0
bool slotMaterialsFromGeometry(LLOBJ_DATA* o, ObjEntry* li)
{
    if (!objHasGeometry(o)) return false;
    auto* gl = *at<def::model::geo_kw::GeoList*>(o, 0x3998);
    if (!gl) return false;
    auto* g = def::model::geo_kw::geo_list_at(gl, 0);                       // 14086c0e0(o, 0)
    if (!g) return false;
    if (!def::model::llobj::first_item_has_items(g)) return false;          // 0x1400667b0
    li->matIds.clear();
    const auto nf = static_cast<std::int32_t>(def::model::geo2d_faces(g).size());   // 0x140066790
    bool any = false;
    for (std::int32_t i = 0; i < nf; ++i) {
        auto* f = def::model::geo2d_face_at(g, &i);                         // 0x1408f1320 = 140283f80
        if (f && *at<std::int32_t>(f, 0x40) > 0) {                           // 0x1408ec3a0
            any = true;
            break;
        }
    }
    li->matIds.clear();
    if (!any) return false;
    for (std::int32_t i = 0; i < nf; ++i) {
        auto* f = def::model::geo2d_face_at(g, &i);
        if (!f) continue;
        const int v = *at<std::int32_t>(f, 0x40);
        if (v > 0 && std::find(li->matIds.begin(), li->matIds.end(), v) == li->matIds.end())
            li->matIds.push_back(v);
    }
    return true;
}

}  // namespace

// @src 0x140064d40  slot material list of object no (not for kind 7 or 5)
bool syncSlotMaterials(Project& p, Operation& op, int no)
{
    GuiModel* m = opModel(p, op);
    if (!m) return false;
    LLOBJ_DATA* o = m->object(no);
    if (!o) return false;
    const int kind = *at<std::int32_t>(o, 0xc);
    if (kind == 7 || kind == 5) return false;                               // 140834fa0, 1408350b0
    ObjEntry* li = objEntryN(op, no);                                       // 0x1400612d0
    if (!li) return false;
    bool r = slotMaterialsFromMesh(p, op, o, li);
    if (!r) r = slotMaterialsFromGeometry(o, li);
    if (!r) {
        li->matIds.clear();
        const int g = *at<std::int32_t>(o, 0x3980);                          // 0x140876860
        if (g > 0) {
            li->matIds.push_back(g);
            r = true;
        }
    }
    return r;
}

namespace {

// @src 0x140064cb0  object material lists of the slots <- engine objects
void syncObjectMaterials(Project& p, Operation& op)
{
    GuiModel* m = opModel(p, op);
    if (!m) return;
    const int n = m->objectCount();
    for (int i = 0; i < n; ++i)
        if (LLOBJ_DATA* o = m->objectAt(i + 1)) syncSlotMaterials(p, op, *at<std::int32_t>(o, 8));
}

}  // namespace

// Activation: 0x14003a3c0 -> 0x140047f80(model, -1) -> 0x140051c30
Operation* openActiveOperation(Project& p, Ctx& ctx, FileEffects& fx)
{
    mst::installLoaderSlots();
    SubProject* sp = activeSubProject(p);
    if (!sp) return nullptr;
    const FlowChart& flow = sp->flow;
    if (flow.activeItem < 0 || static_cast<std::size_t>(flow.activeItem) >= flow.items.size()) return nullptr;
    const FlowItem* it = flow.items[static_cast<std::size_t>(flow.activeItem)].get();
    if (!it) return nullptr;
    Operation* op = operationById(p, it->opId);
    if (!op || !classOf(*op)) return nullptr;
    // 0x1405ef9d0 class gate: current operation and project (its feature check is licence code,
    // @src 0x1400aab60  omitted: licence)
    classOf(*op)->current = op;
    if (!op->project) return nullptr;
    // 0x140051c30
    GuiModel* m = opModel(p, *op);
    if (!m) return nullptr;
    m->reset();
    classOf(*op)->current = op;                             // 0x1405ef280
    // QDir::setCurrent(project folder): the engine is given absolute paths here
    if (SubProject* s = activeSubProject(p))                // 0x140055050
        if (DbHandle* h = subProjectDb(p, *s))              // 0x1400b9cd0, reader 0x140cdab70 (never null)
            dbRefreshIfChanged(*h);                         // 0x140d397a0(DBR, 0)
    classOf(*op)->current = op;                             // 0x1405ef280 (again)
    classOf(*op)->loadOperation(p, *op, ctx, fx);           // OB vt+0x58
    if (op->oprFlag == 0) op->oprFlag = 1;                  // 0x1400509d0 / 0x140050930
    syncObjectMaterials(p, *op);
    // the main window's view reset for the shown model's dimension and its fit (vt+0x550 0x140c95890
    // and the paint-time dimension switch 0x140ccb670 -> 0x140d11ef0; which of them runs after the
    // load is not traced, the end state matches every fresh Oracle B camera); packet moproj
    displayResetView(p);
    return op;
}

}  // namespace m2m::engine
