// MDT L0 internal: raw field access at the binary's offsets and the engine object tests the loader
// uses (owners in port/).
#pragma once

#include <cstddef>
#include <cstdint>
#include <functional>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#include <defcmd/geolib.hpp>
#include <defcmd/objpos_centre.hpp>
#include <defdbio/gendb/counts_frame.hpp>
#include <defdbio/gendb/gendb.hpp>
#include <defmodel/layout/cls_1401e7b10.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/llobj_data.hpp>

#include "m2m/engine.hpp"
#include "m2m/model.hpp"
#include "m2m/engine.hpp"
#include "m2m/props.hpp"
#include "m2m/qt4.hpp"

namespace m2m::data {
struct CogModel;
}

namespace m2m::mst {
void installLoaderSlots(); // existing internal initializer in class_slots.cpp
}

namespace m2m::engine {

// Internal pre-effect gate for support/loader callbacks on a parked model.
// Public raw fields and previously returned pointers cannot retire its cursor.
void requireModelGeometrySyncAvailable(const GuiModel& model);

template <class T> T* at(void* p, std::size_t off)
{
    return reinterpret_cast<T*>(static_cast<std::byte*>(p) + off);
}
template <class T> const T* at(const void* p, std::size_t off)
{
    return reinterpret_cast<const T*>(static_cast<const std::byte*>(p) + off);
}

// QString::toLocal8Bit (system ANSI code page) for the char paths the engine's fopen takes
std::string local8(const qt::String& s);
// local8 for a KEY file the engine's reader opens (defcmd 1401b5f00 / key_import: _fsopen on the
// char path, as the GUI's CRT does). A path of MAX_PATH (260) characters or more cannot be opened
// there, so the conversion refuses (M2M_E_UNSUPPORTED) instead of loading without the file.
std::string engineKeyPath(const qt::String& s);

// @src 0x140062960  predecessor operation in the active sub-project's chain, or null (loader.cpp)
Operation* prevOp(Project& p, const Operation& op);
// @src 0x14005f7a0  synchronously use the operation's nullable DB_STEP_DATA.
// This seam deliberately preserves opModel lookup and does not add a geometry guard.
// Callers must neither retain the borrowed pointer nor capture it in escaping work.
template<class F>
concept OpStepCallback = std::is_invocable_v<F, def::model::DB_STEP_DATA*> &&
    (std::is_void_v<std::invoke_result_t<F, def::model::DB_STEP_DATA*>> ||
     std::is_same_v<std::invoke_result_t<F, def::model::DB_STEP_DATA*>, bool>);

template<class F> requires OpStepCallback<F>
auto withOpStep(Project& p, Operation& op, F&& callback)
    -> std::invoke_result_t<F, def::model::DB_STEP_DATA*>
{
    GuiModel* model = opModel(p, op);
    return std::invoke(std::forward<F>(callback), model ? model->step.get() : nullptr);
}

// @src 0x14005fd20  code of an inherited slot s: the first predecessor's own code, else -99 (loader.cpp)
int resolvedCode(Project& p, const Operation& op, int s);
// SfMoObjManager / SfMoEqpManager vt+0x208 (0x14006d380): cleanPath of item id's file, null if none
String managerFile(const ItemManager& m, int id);
// MDT L1 cogging class slot bodies (cogging_load.cpp), installed by installLoaderSlots()
void installLoaderSlotsL1();
// @src 0x140054540  OPRNAM, CURSIM, SIMNAM of the operation's sim control (save_current.cpp)
void storeSimNames(Project& p, Operation& op);
// @src 0x140766740  cogging: engine object no's primitive description from the cogging model
void cogSyncObjectGeometry(Project& p, Operation& op, data::CogModel& M, int no);
// MDT L2 accessors (loader_l2.cpp)
// @src 0x14005f7e0  the operation's sim control, or null
def::model::Cls_1401e7b10* opSimCtl(Project& p, Operation& op);
// @src 0x1405ef6f0  SfOprBase: SC+0x108 (TRANS 2) = v
void setTrans2(Project& p, Operation& op, bool v);
// @src 0x1405ef750  SfOprBase: SC+0x104 (TRANS 1) = v
void setTrans1(Project& p, Operation& op, bool v);
// @src 0x14005fab0  number of slots of op whose code holds an object
int objectSlotCount(const Operation& op);
// @src 0x140057690  engine object of slot s when the slot's code holds one
def::model::LLOBJ_DATA* slotObject(Project& p, Operation& op, int s);
// MDT L2 class slot bodies (loader_l2.cpp), installed by installLoaderSlots()
void installLoaderSlotsL2();
// MDT L4 (DB-5): the Express family's vt+0x2c0 / vt+0x2b8 bodies (express_sync.cpp)
void installLoaderSlotsL4();
struct FileEffects;
// Forming stores the two equipment IDs between this value-only prefix and final KEY write.
struct EquipmentCreationPrefix { int id; String pathBeforeIdStores; };
EquipmentCreationPrefix beginEquipmentCreation(Project&, def::model::DB_STEP_DATA&,
                                               def::model::LLOBJ_DATA*, FileEffects&);
void appendFinalEquipmentKey(def::model::DB_STEP_DATA&, def::model::LLOBJ_DATA*,
                             const String& path, FileEffects&);
// packet S (postload_views.cpp): create "Equipment (n)" from an engine object's movement (the
// common part of 0x140c06130 and the vt+0x230 bodies); returns the new equipment id
int createEquipment(Project& p, def::model::DB_STEP_DATA& step, def::model::LLOBJ_DATA* obj, FileEffects& fx);
// SfMoEqpManager vt+0x160 0x14006c610 (through 0x1400bf510): equipment `name` from KEY bytes, its new
// id (-1 without bytes) (postload_views.cpp; also the Cogging post-load, MDT L4)
int addEquipment(Project& p, const String& name, const std::string* srcBytes, FileEffects& fx);
// 0x140358190(step, obj, <temp file>, 1): the equipment KEY bytes of obj (postload_views.cpp)
std::string equipmentBytes(def::model::DB_STEP_DATA& step, def::model::LLOBJ_DATA* obj);
// @src 0x140057770  add object no to op (multiblow_load.cpp)
def::model::LLOBJ_DATA* addObjectNo(Project& p, Operation& op, int no, FileEffects& fx);
// packet S (save_current.cpp): the path 0x140358190 writes to for a file name
qt::String equipmentKeyTarget(qt::String file);
// packet S (postload_views.cpp): OB vt+0x230, the equipment sync (also reachable from src/ops as
// mst::g_equipmentSync); declared in m2m/engine.hpp
void equipmentSync(Project& p, Operation& op, bool multiBlow, FileEffects& fx);
// @src 0x140c69520  SfOprHTFurnaceProperties vt+0x60 (loader_l2.cpp), for 0x1400543a0
void furnacePropsToEngine(Project& p, Operation& op);
// material manager vt+0x178: cleanPath(projectDir + "/Materials" + sep + "MaterialNNNNN.KEY")
qt::String materialDefaultFile(const m2m::Project& p, int id);
// MDT L3 class slot bodies (loader_l3.cpp), installed by installLoaderSlots()
void installLoaderSlotsL3();
// @src 0x1413fd0f0  Express heat transfer type of op: 4 unless its props are heat-transfer Express
// props, else wizard data+0x18 (-1 without data) (loader_l3.cpp)
int expressHeatTransType(Operation& op);

// ---- MDT L3S: contact defaults of the system-lubricant rebuild 0x14005a9b0 (save_current.cpp) ----
// One default contact pair (0x20 bytes, ctor 0x1400657a0: master 1, slave 1, values 0, +0x18 = 0,
// +0x19 = 1)
struct ContactPair {
    std::int32_t master = 1;       // +0x00
    std::int32_t slave = 1;        // +0x04
    double frc = 0.0;              // +0x08 friction (pair+0x20)
    double ihtc = 0.0;             // +0x10 heat transfer (pair+0x1c0)
    std::uint8_t lub = 0;          // +0x18 lubricant side flag (0x140875a20)
    std::uint8_t f19 = 1;          // +0x19 3D flag (pair+0x174)
};
// Express classes (save_l3.cpp)
// @src 0x1413fd4c0  SfOprExpressBase vt+0x1a0: friction and IHTC of a default pair
void expressContactDefaults(Project& p, Operation& op, double* frc, double* ihtc, bool flag);
// @src 0x14141ef60  SfOprFormingExpressCommon vt+0x198: default pairs (every object with object 1)
std::vector<ContactPair> feDefaultPairs(Project& p, Operation& op);
// @src 0x1405fcad0  HeatingTime of the props in their unit system (save_l3.cpp)
double propsHeatingTime(const Props& pr);
// Express save slots (save_l3.cpp): vt+0x68 part before the base 0x1413f8f60, HeatTransferExpress
// vt+0xd8 0x1413e6460, vt+0xe8 0x1413f91f0, HTExpress props vt+0x70 0x1413ed710
void expressSaveOperation(Project& p, Operation& op);
void hteSaveTmax(Project& p, Operation& op);
void feSaveSimNames(Project& p, Operation& op);   // FormingExpress vt+0xd8 0x14141cf90
// 0x140054540 (save_current.cpp, packet S):  OPRNAM, SIMNAM, CURSIM into the engine
void storeSimNames(Project& p, Operation& op);
bool expressSyncOperation(Project& p, Operation& op, FileEffects& fx);
bool hteStepDataToSimCtl(const HeatTransferExpressProps& pr, def::model::Cls_1401e7b10* sc);

// 140834d50 = 140160cc0  object has geometry
inline bool objHasGeometry(def::model::LLOBJ_DATA* o) { return def::dbio::gendb::object_has_geometry(o); }
// 140834c80 = 140160bf0  object has a mesh (+0x87c != 0)
inline bool objHasMesh(const def::model::LLOBJ_DATA* o) { return def::dbio::gendb::has_mesh(o); }
// 140873be0 = 1401ffd80  delete the object's mesh
inline void objDeleteMesh(def::model::LLOBJ_DATA* o) { def::cmd::geolib::fun_1401ffd80(o); }
// 140870590 = 1401fc730  geometry from the mesh
inline void objGeometryFromMesh(def::model::LLOBJ_DATA* o, char* compact) { def::cmd::objpos::fun_1401fc730(o, compact); }

// ---- MDT L4S (src/engine/save_current.cpp, packet S) ----
// @src 0x140054540  operation and simulation names into the engine (OPRNAM, CURSIM, SIMNAM)
void storeSimNames(Project& p, Operation& op);
// base vt+0xd8 0x1405f22e0 (mst::g_loaderSlots.saveNames)
void baseSaveNames(mst::OpClass& c, Project& p, Operation& op, FileEffects& fx);

// 0x1402c8170(APP): the main window's GL scene (+0x120, main window vt+0x778 0x140c9bbd0), made by
// 0x140ca5850 when the posted event 0x123 is handled (0x14001e910)
bool glScenePresent();
// ---- packet S helpers of 0x140057370 (save_current.cpp), used by class save slots ----
// @src 0x140051190  task folder and project folders (FileEffects::mkdirs)
void makeFolders(Project& p, Operation& op, FileEffects& fx);
// @src 0x1400597d0  SimCtrl.KEY
bool saveSimCtrl(Project& p, Operation& op, FileEffects& fx);
// @src 0x14005c760  object file of slot s
bool saveObject(Project& p, Operation& op, int s, FileEffects& fx);
// @src 0x140058cc0  Materials/MaterialNNNNN.KEY of the engine materials
void saveMaterials(Project& p, Operation& op, FileEffects& fx);
int saveMaterialKey(Project& p, def::model::DB_STEP_DATA& step, int number,
                    const String& path, FileEffects& fx);
// 0x140895350: whole-model KEY. Loaded models set +0x1744 before fopen;
// the deferred DB-step temporary-model branch remains explicitly qualified.
int saveWholeModelKey(Project& p, def::model::DB_STEP_DATA& step,
                      const String& path, std::uint32_t flags, FileEffects& fx);
// ---- MDT L7 (src/engine/loader.cpp), for the post-load bodies of every class ----
struct FileEffects;
struct GuiModel;
// @src 0x140057ba0  add one object to the operation (new object item, first free slot); the GUI's
// object-manager refresh 0x140069620 may delete the files of object items no operation uses
def::model::LLOBJ_DATA* addObject(Project& p, Operation& op, FileEffects& fx);
// @src 0x140057c60  put object id into slot s and create (fromFile false) or load its engine object
def::model::LLOBJ_DATA* addObjectAt(Project& p, Operation& op, int s, int id, bool fromFile, bool ui,
                                    FileEffects& fx);
// @src 0x140060100  slot record and object-item record of object no <- engine object
// ---- shared between the loader (loader.cpp, class_slots.cpp) and the class packets -------------
// @src 0x140062960  predecessor operation in the active sub-project's chain, or null
Operation* prevOp(Project& p, const Operation& op);
// @src 0x14005fb30  slot code (OPI+0x40), -99 out of range
int slotCode(const Operation& op, int s);
// @src 0x14005fd20  code of an inherited slot: the first predecessor with an own object
int resolvedCode(Project& p, const Operation& op, int s);
// @src 0x1405ef430  SfOprBase load (OB vt+0x58)
void baseLoad(Project& p, Operation& op, Ctx& ctx, FileEffects& fx);
// @src 0x140055360  mode-0 decision (restart, setup type), then the KEY load (loader.cpp; also
// reached from SfOprBase vt+0x1c0 0x1405f65d0, MDT L4)
void decideAndLoad(Project& p, Operation& op, bool withObjects, Ctx& ctx, FileEffects& fx);
// @src 0x14005af80  one slot of op into model m without its side files (bare): own object
// (0x14005b030) or inherited (0x14005b2f0)
def::model::LLOBJ_DATA* loadSlotBare(Project& p, Operation& op, GuiModel& m, int s);
int refreshSlotItems(Project& p, Operation& op, int no, FileEffects& fx);
// @src 0x140069620  object manager refresh (drops object items no operation uses, with their files)
void refreshObjManager(Project& p, FileEffects& fx);
// SfMoObjManager vt+0x180 (0x14006c520): new object item with the smallest unused id
int newObjectItem(Project& p);
// ---- shared with MDT DB (src/engine/db_load.cpp) ------------------------------------------------
// @src 0x140c7ebe0  primitive table of the sim control into the objects (gui_model.cpp)
void primitiveTable(GuiModel& m);
// @src 0x140064d40  slot material list of object no from the engine object (loader.cpp)
bool syncSlotMaterials(Project& p, Operation& op, int no);
// @src 0x140358190(step, obj, file, overwrite = 1)  equipment KEY of an engine object (save_current.cpp)
void saveEquipment(Project& p, def::model::DB_STEP_DATA& step, def::model::LLOBJ_DATA* obj, String file, FileEffects& fx);

}  // namespace m2m::engine

namespace m2m::mst {
class OpClass;
// @src 0x1405f1bb0  SfOprBase vt+0xb8
void baseLoadPrologue(OpClass& c, Project& p, Operation& op);
// @src 0x1405f1c00  SfOprBase vt+0xc0
void basePostLoadBody(OpClass& c, Project& p, Operation& op, Ctx& ctx, engine::FileEffects& fx);
// MDT L5 slot bodies into g_l5Slots (l5_slots.cpp)
void installL5Slots();
// @src 0x1400585e0  rename slot s (l6_slots.cpp; also the Cogging post-load, MDT L4)
bool renameSlot(Project& p, Operation& op, int s, const String& name, engine::FileEffects& fx);
}  // namespace m2m::mst
