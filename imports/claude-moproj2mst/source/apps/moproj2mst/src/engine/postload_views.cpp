// Packet S: the model effects of the views step the loader 0x140051340 runs after an operation
// load, 0x1400547b0(op, 0) (docs/re/moproj2mst/s/spec.md section 9). Of that chain only the
// equipment sync OB vt+0x230 changes the project: it gives the primary die an equipment (creating
// "Equipment (n)" when it has none) and, for MultiBlow, removes every other object's equipment
// from the project with its KEY file. The rest builds the object tree and property panel models
// (UI) and is dropped; each drop is named.
#include <process.h>

#include <cstdio>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iterator>

#include <defkey/item_writers.hpp>
#include <defmodel/layout/cls_1401e7b10.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/llobj_data.hpp>

#include "engine_internal.hpp"
#include "m2m/engine.hpp"
#include "m2m/engine/file_effects.hpp"
#include "m2m/doe_io.hpp"
#include "m2m/doe_session_notifications.hpp"
#include "m2m/mst/writer.hpp"
#include "m2m/qt4path.hpp"

namespace m2m::engine {

using qt::L;

namespace {

bool keyIn(const String& key, std::initializer_list<const char16_t*> keys)
{
    for (const char16_t* k : keys)
        if (key.u16() == k) return true;
    return false;
}

// SfMoEqpManager vt+0x178 = 0x1400bef30: cleanPath(projectDir + sep + "Equipments") + sep +
// "Equipment%05d.KEY" (the separator after the folder is QDir::separator(), not cleaned)
String equipmentFile(const Project& p, int id)
{
    char buf[64];
    std::snprintf(buf, sizeof buf, "Equipment%05d.KEY", id);
    return qt::cleanPath(projectDir(p) + qt::kSep + L("Equipments")) + qt::kSep + L(buf);   // 0x1400a4770
}

// SfMoItemManager vt+0x168 = 0x14006c230: the first id from 1 that vt+0x1c8 does not find
int firstFreeId(const ItemManager& m)
{
    int id = 1;
    while (m.find(id)) ++id;
    return id;
}

// SfMoEqpManager vt+0x1a0 = 0x1400bff90 -> 0x14006ca50: remove the row of `id` and its KEY file
// (vt+0x1a8 = 0x14006ccf0: QFile::remove(vt+0x178(id)) through 0x14081a6e0); nothing when no row
// has the id (vt+0x1f0 < 0). The wrapper always emits after that base call,
// including absent rows; its fixed-source receiver list is empty.
void removeEquipment(Project& p, int id, FileEffects& fx)
{
    auto& rows = p.equipment.rows;
    for (auto it = rows.begin(); it != rows.end(); ++it)
        if (*it && (*it)->id == id) {
            if (fx.nativeRemovals)
                removeProjectFileAtCall(p, equipmentFile(p, id), fx);
            else
                fx.deletes.push_back(equipmentFile(p, id));
            rows.erase(it);                                   // takeRow
            break;
        }
    if (fx.nativeRemovals) {
        //0bff90: notify after attempted deletion and row destruction. The
        // equipment wrapper passes id-1 even when no row was found. The
        // original no-argument key still carries the generic integer argument.
        if (!p.doeIo)
            throw Error(M2M_E_UNSUPPORTED, "Equipment removal needs actual shared notifier state");
        doe::observeEmptySessionNotification(*p.doeIo, "2signalMoEquipManagerChanged()",
                                            {}, {}, id - 1);
    }
}

}  // namespace

// SfMoEqpManager vt+0x160 = 0x14006c610 (through 0x1400bf510): the equipment `name` from the KEY
// `src`: the next free id, the file copied to vt+0x178(id) (vt+0x238 = 0x1400bf310: mkpath of its
// folder, remove, QFile::copy), and a row with the name, the source (+0x20) and that path (+0x28)
// appended. -1 when `src` does not exist or the copy fails.
int addEquipment(Project& p, const String& name, const std::string* srcBytes, FileEffects& fx)
{
    if (!srcBytes) return -1;                                 // vt+0x230 = the path, QFile::exists
    const int id = firstFreeId(p.equipment);                  // vt+0x168
    const String dst = equipmentFile(p, id);                  // vt+0x178
    fx.mkdirs.push_back(qt::absolutePath(dst));               // 0x140819590, 0x14081afb0
    fx.writes.push_back({dst, *srcBytes});                    // remove + QFile::copy
    auto it = std::make_unique<Item>();                       // vt+0x240 = 0x1400bfdf0 (vt+0x170)
    it->kind = ItemKind::Equipment;
    it->id = id;
    it->name = name;                                          // item vt+0x40 (name not empty)
    it->keyPath = dst;                                        // 0x14006ba30 (+0x28)
    p.equipment.rows.push_back(std::move(it));                // appendRow
    return id;
}

// 0x140358190(step, obj, file, overwrite = 1) into a scratch file: the bytes it writes
std::string equipmentBytes(def::model::DB_STEP_DATA& step, def::model::LLOBJ_DATA* obj)
{
    const std::filesystem::path tmp = std::filesystem::temp_directory_path() /
                                      ("m2m_s_eqp_" + std::to_string(_getpid()) + ".KEY");   // per process: suites run in parallel
    std::error_code ec;
    std::filesystem::remove(tmp, ec);
    def::key::write_equipment_key_file(step, tmp.string().c_str(), *at<std::int32_t>(obj, 8));   // 0x140896840
    std::ifstream in(tmp, std::ios::binary);
    std::string b((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    in.close();
    std::filesystem::remove(tmp, ec);
    return b;
}

// @src 0x140c068c0 (Forming), 0x140c05200 (Forming2D, the same code), 0x140607d10 (MultiBlow):
// OB vt+0x230. The object tree (OB+0x18) has one node per slot (0x1400c3980, 0x1400c3a80), node i
// holds slot i-1 and 0x1400c04e0 gives its engine object (0x140057690). The loop runs over the
// engine object count (0x140c7c1a0); a node past the tree is null.
void equipmentSync(Project& p, Operation& op, bool multiBlow, FileEffects& fx)
{
    GuiModel* m = opModel(p, op);                             // 0x14005f770
    if (!m || !m->step) return;
    def::model::Cls_1401e7b10* sc = m->simCtl();              // 0x140c7b2d0
    if (!sc) return;                                          // (equipment manager 0x1400a5970 exists)
    const int n = m->objectCount();
    const int nodes = objSlots(op);
    for (int i = 0; i < n; ++i) {
        if (i >= nodes) continue;                             // 0x1400c3cb0 out of range
        def::model::LLOBJ_DATA* obj = slotObject(p, op, i);
        if (!obj) continue;
        const int no = *at<std::int32_t>(obj, 8);
        const bool primary = sc->pdie1 == no;                 // SC+0x168 (PDIE)
        ObjEntry* rec = objEntryN(op, no);                    // 0x1400612d0
        if (multiBlow && !primary) {
            const int eq = rec ? rec->equipmentId : 0;        // 0x14004de90 (the original reads a null rec)
            if (eq > 0) {
                removeEquipment(p, eq, fx);                   // vt+0x1a0
                rec->equipmentId = -1;                        // 0x14004deb0
            }
            continue;
        }
        if (!rec) continue;
        int eq = rec->equipmentId;
        if (eq < 1) {
            eq = createEquipment(p, *m->step, obj, fx);
            rec->equipmentId = eq;                            // 0x14004deb0
        }
        if (primary && op.equipmentId != eq) op.equipmentId = eq;   // 0x140066760 / 0x14004ca30
    }
}

// The equipment creation shared by Forming vt+0xc0 (0x140c06130) and the vt+0x230 bodies: name
// "Equipment (" + QString("%1").arg(rows + 1) + ")" (vt+0x1b8 row count), the object's equipment
// KEY written to QDir::tempPath()/<name>.KEY (0x140358190, overwrite 1; the temp file is removed
// before and after, so it leaves no trace), added through 0x1400bf510, the movement name
// (obj+0x2160) set to the name (strncpy 0x50), and the KEY written again at the item's project
// path (vt+0x208(id)), now with that name. Returns the new id (-1 when the add fails).
int createEquipment(Project& p, def::model::DB_STEP_DATA& step, def::model::LLOBJ_DATA* obj, FileEffects& fx)
{
    const String name = L("Equipment (") + qt::number(static_cast<int>(p.equipment.rows.size()) + 1) + L(")");
    const std::string bytes = equipmentBytes(step, obj);
    const int id = addEquipment(p, name, &bytes, fx);         // 0x1400bf510
    std::strncpy(at<char>(obj, 0x2160), local8(name).c_str(), 0x50);   // 0x141470020
    const String file = managerFile(p.equipment, id);
    if (!file.isEmpty()) fx.writes.push_back({equipmentKeyTarget(file), equipmentBytes(step, obj)});   // 0x140358190
    return id;
}

namespace {
// make vt+0x230 reachable from the class bodies in src/ops (mst::g_equipmentSync)
const bool g_equipmentSyncSet = (mst::g_equipmentSync = &equipmentSync, true);
}  // namespace

// @src 0x1400547b0(op, 0)  called by SfOprBase vt+0x58 0x1405ef430 after props vt+0x58: 0x140054820
// (views, UI), 0x1400548c0, 0x140054df0 and 0x1400bc5b0 (UI). 0x1400548c0: ProcType 5 (a .moproj
// open) -> 0x140054950: OB vt+0xa0 (0x1405f1b60 true; DoeOutput 0x140e30e50 and Report 0x1412af6b0
// false: vt+0xa8 only), then, on this first open (the tree model OPI+0x160 does not exist yet),
// vt+0xa8 (objectTreeFill) and vt+0x278 (base 0x1405f15e0: vt+0x228 first, then the tree model,
// UI; Cogging 0x1407514b0, Diestress 0x1402b6e70 and DiestressBM 0x140299fa0 have their own, which
// also call vt+0x228 and are their packets' to check), spec section 9.
void postLoadViews(Project& p, Operation& op, Ctx&, FileEffects& fx)
{
    displayRefreshScene(p, opModel(p, op));                   // 0x140054820 (packet moproj)
    mst::OpClass* ob = opBase(op);                            // 0x140054e90
    if (!ob) return;
    const bool tree = !keyIn(op.classKey, {u"SfOprDoeOutput", u"SfOprReport"});   // vt+0xa0
    ob->objectTreeFill(p, op, fx);                            // vt+0xa8
    if (!tree) return;
    ob->objectTreeAdd(p, op, fx);                             // vt+0x278 -> vt+0x228
}

}  // namespace m2m::engine
