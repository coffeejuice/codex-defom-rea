// m2m/engine.hpp: the GUI's engine model of an operation and the operation loader that fills it
// (packet MDT L0, docs/re/moproj2mst/mdt/L0/spec_A.md, spec_B.md, spec_C.md). The engine is the
// DEF_PRE port (port/): the GUI embeds byte-identical twins of those functions.
//
// Remaining branches guard with M2M_E_UNSUPPORTED: populated geometry-list viewer
// contributions, an object inherited from the project database (0x14005cc00), a 2D object inherited
// into a 3D operation (runs FIXGEO.EXE / DEF_GEOPRIM3.EXE), CopyMirror objects, and the class
// slots of classes whose packet (L2..L7) has not landed.
#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

#include "m2m/model.hpp"

namespace def::model {
class Cls_1401dbfe0;
class DB_STEP_DATA;
class LLOBJ_DATA;
class Cls_1401e7b10;
class Material;
}

namespace m2m::moproj {
class DiagramViews;
}

namespace m2m::doe { struct StepDataState; class SlicePlaneStore; class StudyXmlJournal; }

namespace m2m::engine {

namespace simcontrol300 { struct StepCountContext; struct StepCountPage; }

using def::model::DB_STEP_DATA;
using def::model::LLOBJ_DATA;

// Files the load writes or deletes in the project (the converter applies them only when asked).
struct FileEffects {
    // Observation-only alias for XML effects already executed by the actual
    // StudyIo. It is never replayed by applyEffects or projected as SideBytes.
    std::shared_ptr<const doe::StudyXmlJournal> xmlJournal;
    struct Write {
        String path;
        std::string bytes;
    };
    std::vector<Write> writes;
    std::vector<String> deletes;
    std::vector<String> mkdirs;    // QDir::mkpath targets (packet S, 0x140051190), in call order
    bool nativeDirectories = false; // real Save: attempt at the call site, not in applyEffects
    struct DirectoryAttempt { String path; bool succeeded; };
    std::vector<DirectoryAttempt> directoryAttempts; // failed helper still returns true to caller
    bool nativeSimCtrl = false;
    bool nativeTaskKeys = false; // task KEY writers, including loaded-model Restart
    bool nativeRemovals = false; // loader removals occur before subsequent state/row changes
    struct RemovalAttempt { String path; bool succeeded; };
    std::vector<RemovalAttempt> removalAttempts; // original callers ignore false
    std::function<void(const String&)> beforeNativeRemoval; // adapter output-alias check
    unsigned nativePathCodepage = 1252; // explicit locale profile, separate from ASCII writers
    std::function<void(const String&)> beforeNativeKeyWrite; // adapter output-alias check
    struct Rename {
        String from, to;
    };
    std::vector<Rename> renames;   // QFile::rename (DOE study files, MDT L6T), in call order
};

struct Viewer;                             // packet V (src/engine/viewer.cpp)
struct Display;                            // the GL display widget (src/engine/viewer_camera.cpp)
struct GuiModel;
struct GuiModelLifetime;
class GeometrySyncBarrier;
struct GeometrySyncAccess;
struct GuiModelAccess;
struct AuxiliaryDrawList;
class PendingPickingOwner;
class PostGraphManagerState;

// Observation only: the weak owner identity and storage generation never grant
// native access. Only GuiModel can issue a bound token; default tokens are inert.
class StepIdentity {
public:
    StepIdentity() = default;
    bool isCurrent() const noexcept;
    bool operator==(const StepIdentity& other) const noexcept;
    bool operator!=(const StepIdentity& other) const noexcept { return !(*this == other); }
private:
    friend struct GuiModel;
    StepIdentity(const std::shared_ptr<GuiModelLifetime>& owner, std::uint64_t generation)
        : owner_(owner), generation_(generation) {}
    std::weak_ptr<GuiModelLifetime> owner_;
    std::uint64_t generation_ = 0;
};

// Nonowning QObject-style model guard. It remains valid across display backlink
// changes and expires with the model, without extending the model's ownership.
class GuiModelGuard {
public:
    GuiModelGuard() = default;
    explicit GuiModelGuard(GuiModel* model) { reset(model); }
    void reset(GuiModel* model = nullptr);
    GuiModel* get() const noexcept;
private:
    std::weak_ptr<GuiModelLifetime> lifetime_;
};

// ---- MDT DB: partial SfModelBase/SfMoModel projection (Project+8) ----------------------------
// cda460 constructs SfModelBase;043fe0 derives SfMoModel on the SAME object.
// This is distinct from the outer SfModelManager collection/current selection.
// Its DB part (0x19c0-byte base): path DBH+0x28 (set flag
// +0x20), loaded flag +0x38 and the DB reader DBR = DBH+0x40, the GUI build of DEF_PRE's DB info
// class Cls_1401dbfe0 (the engine fields are the same; the GUI's extras past +0x1900, spec Q2, are
// kept beside it). Code: src/engine/db_load.cpp.
struct DbHandle {
    enum class Kind { Base, Mo };
    explicit DbHandle(Kind kind = Kind::Base);
    ~DbHandle();
    DbHandle(const DbHandle&) = delete;
    DbHandle& operator=(const DbHandle&) = delete;
    // Adopt the same Project slot storage on this Mo identity. Base children
    // and replacement/foreign owners refuse; this is not native Close replay.
    void adoptMoModelSlots(std::shared_ptr<MoModelSlots>);
    const std::shared_ptr<MoModelSlots>& moModelSlotsOwner() const noexcept { return moModelSlots_; }

    const Kind kind;
    // Native +10 counter/+18 QList of nonowning plane pointers. Allocation
    // retention belongs to the separately bound fresh-process heap projection.
    std::unique_ptr<doe::SlicePlaneStore> slicePlanes;
    std::int32_t modelId = 0;             // +0x30; assigned by outer manager after creation
    Project* moProject = nullptr;         // derived Mo+0x19c0; committed after Project construction
    std::shared_ptr<PostGraphManagerState> postGraphManager; // lazy +0x1970, ctor null
    bool pathSet = false;                  // +0x20
    String path;                           // +0x28
    bool loaded = false;                   // +0x38
    std::unique_ptr<def::model::Cls_1401dbfe0> reader;   // +0x40 (0x140cdab70), never null
    // GUI extras of DBR: +0x1908 header cache build started (0x140d396d0), +0x1910 QDateTime of the
    // file's last modification at the last (re)open (0x140d395c0; invalid until then); the owner
    // +0x1918 is this handle
    bool cacheStarted = false;
    bool stampValid = false;
    long long stamp = 0;                   // QFileInfo::lastModified in ms (file time / 10000)
private:
    std::shared_ptr<MoModelSlots> moModelSlots_; // same Mo-owned slots, not a second model
};

// SfStepData (0x278 bytes, ctor 0x140c7a5b0): the GUI wrapper of one DEF_PRE DB_STEP_DATA.
struct GuiModel {
private:
    friend class GuiModelGuard;
    friend struct GeometrySyncAccess;
    friend struct GuiModelAccess;
    // Declared first so guards expire after the represented model members.
    // Unrecovered reentrant destructor callbacks remain outside this contract.
    std::shared_ptr<GuiModelLifetime> lifetime_;
    // Adapter continuation, not native fields. A lowered query version cannot
    // bypass this model-owned barrier by replacing the public Viewer pointer.
    std::shared_ptr<GeometrySyncBarrier> geometrySyncBarrier_;
    void requireGeometrySyncAvailable() const;
    // Retained interrupted c90f10 constructor allocation, NOT published +248.
    // Adapter storage lifetime is not a native constructor-unwind assertion.
    std::unique_ptr<PendingPickingOwner> pickingConstruction_;
public:
    // @src 0x140c7a5b0 (0x140c7a870: +0xe0 = new DB_STEP_DATA(dim), 0x140c7aa80: +0xd8 = db)
    GuiModel(DbHandle* db, int dim);
    ~GuiModel();
    GuiModel(const GuiModel&) = delete;
    GuiModel& operator=(const GuiModel&) = delete;

    // Covers constructor/reset/destruction only during the legacy migration.
    // Direct public step replacement and other destructive loaders still bypass
    // invalidation. Currentness cannot authorize native access or publication.
    StepIdentity stepIdentity() const noexcept;
    bool isCurrentStepIdentity(const StepIdentity& identity) const noexcept;

    // Explicit lazy-getter prefix. Native +248 publication is unavailable until
    // its constructor returns; this stops before the unresolved graphics call.
    PendingPickingOwner& beginPickingConstructionPrefix();
    const PendingPickingOwner* pendingPickingConstruction() const noexcept { return pickingConstruction_.get(); }

    std::unique_ptr<DB_STEP_DATA> step;    // LEGACY public +0xe0; bypasses identity on replacement
    DbHandle* db = nullptr;                // +0xd8: the DB handle (P+8 for vt+0x1b0 models); null = none
    std::int32_t tableF8[16] = {};         // +0xf8 .. +0x13c ints of 0x140c7d3a0
    double tableF8d[6] = {};               // +0x138 .. doubles of 0x140c7d3a0 (rows 0 and 1)
    double tableF8box[6] = {};             // block +0x70 / +0x88: node-box min of objects 1 and 2 (0x141071060)
    std::int32_t state100 = 0;             // +0x100 (0x140c833c0)
    std::shared_ptr<Viewer> viewer;        // +0x18 / +0x20 viewer items and settings (packet V)
    std::shared_ptr<doe::StepDataState> doeStepData; // lazy imported property/state tables
    Display* display = nullptr;            // +0x10 the display that shows the model (0x140c7ade0)
    std::unique_ptr<AuxiliaryDrawList> auxiliaryDrawings; // lazy +0x250; never owns graph pointees
    std::int32_t f25c = 0;                 // +0x25c set to 2 by 0x140d0d670 (0x140d12560)
    // +0x18 viewer item list (packet V), kept only as far as engine code reads it: per object number,
    // the mirror planes CopyMirror's post-load (0x140e23490) gives the item. The planes are built by
    // viewer code (0x140e23a00) that is not ported, so a count here means "planes requested"; the
    // only reader, CopyMirror vt+0x2a0 (0x140e25b20), refuses when it is not 0. Cleared with the
    // list by reset (0x140c82c70).
    std::vector<int> viewerMirrorPlanes;

    // @src 0x140c7b2d0  sim control (step+0x750), null without a step
    def::model::Cls_1401e7b10* simCtl() const;
    bool matchesCurrentSimControl(const def::model::Cls_1401e7b10* expected) const;
    // Synchronously project a completed editor page while the current control is borrowed.
    // The page retains its separately qualified legacy Operation* context.
    bool refillEditorStepCountIfControlMatches(
        const def::model::Cls_1401e7b10* expected,
        const simcontrol300::StepCountContext& context, bool priorNstartChecked,
        simcontrol300::StepCountPage& result) const;
    // Furnace load prologue; a missing step remains a no-op.
    void setFurnaceLoadTnow3();
    // Continuation of the already-admitted DB-step loader branch.
    void setLoadedStepNumberAfterAdmission(int stepNumber);
    // Furnace post-load defaults; requires the existing step just like the legacy unchecked caller.
    void initializeFurnacePostLoadDefaults();
    // Editor title event: reject non-ASCII/oversize input, then barrier, identity check and copy.
    // expected is the legacy raw pointer identity token; the command never dereferences it.
    bool copyEditorTitleIfControlMatches(const def::model::Cls_1401e7b10* expected,
        std::string_view ascii);
    // Applies one validated editor frequency-index event without returning the control/table alias.
    bool applyEditorFrequencyIndexIfControlMatches(
        const def::model::Cls_1401e7b10* expected, int row, int selection);
    // Applies one admitted editor frequency-text event; conversion stays between shape and storage reads.
    bool applyEditorFrequencyTextIfControlMatches(
        const def::model::Cls_1401e7b10* expected, int row, const String& deliveredText);
    // @src 0x140822210 (GUI twin of DEF_PRE 0x1401c8f70): set UNIT and reset defaults on change.
    void setModelUnit(std::int32_t unit, bool reset);
    // @src 0x140c7b370  viewer geometry type; 0 without a 2D step/control
    int simGeometryTypeIf2D() const;
    // @src 0x140c7c0e0 / 0x140c7c0a0  unguarded native step dimension; 0 without a step
    int dimension() const noexcept;
    // @src 0x140c7acd0 / 0x140c7ff30  guarded native virtual dimension setter.
    // Requires an existing step; the legacy null-step path is unchecked.
    // This staged command does not extend StepIdentity invalidation coverage.
    void setDimension(int dim);
    // @src 0x141424af0  FormingExpress per-step dimension flags; no geometry admission check.
    void setFormingExpressDimensionFlags();
    // @src 0x140c7c220 -> 0x1408301c0  tail object number; 0 without a step
    int lastObjectNumber() const;
    // @src 0x140c7b720  reset: viewer list, +0x30, DB_STEP_DATA clear (1401cb400), +0x230, +0x100
    void reset();
    // @src 0x140c7ae80  object by number (140830270 -> 14017aaf0)
    LLOBJ_DATA* object(int no) const;
    // @src 0x140c7aed0  object at position no (1408302a0 -> 14017aa80)
    LLOBJ_DATA* objectAt(int no) const;
    // @src 0x140c7c1a0  object count (step+0x1378)
    int objectCount() const;
    // @src 0x140c7c950  object by number, created when missing (defaults, primary die, viewer)
    LLOBJ_DATA* objectWrap(int no, bool reset, bool withViewer);
    // @src 0x140c7c8b0  a new object numbered after the list's tail (0x1408301c0 + 1): kind-7 reset
    // (0x140834fe0, unit 0), kind 1 without a deformable object, default name, viewer, PDIE
    LLOBJ_DATA* appendObject();
    // @src 0x140c7d020  read a KEY file into the whole model (1401b5f00, reset 1, mode 0); on
    // success the furnace table (0x140c7d3a0) and the primitive table (0x140c7ebe0)
    bool readKey(const String& path);
    // @src 0x140c7cb20  material by number (14029c2c0)
    def::model::Material* material(int no) const;
    // @src 0x140c7cf80  read a material KEY with the requested number (1401b6ea0), viewer sync
    def::model::Material* readMaterial(const String& path, const int* reqNo);
    // @src 0x140c7cd90  import an object KEY as object no (1401b7250), mesh kept
    bool loadObjectKey(const String& path, const int* no);
    // @src 0x140c7ce40  import an object KEY as object no keeping only its geometry
    bool loadObjectKeyGeometry(const String& path, const int* no, const bool* mark5);
    // @src 0x140c7b5a0 / 0x140c7b5e0  PDIE (sim control +0x168)
    int pdie() const;
    void setPdie(int v);
};

// Original viewer-list count, independent of DB object count; never allocates a viewer.
int viewerItemCount(const GuiModel& model);

// ---- packet V: the 3D viewer state the model keeps per object (docs/re/moproj2mst/mdt/V) ------
// @src 0x140c80280  viewer sync of one object (items resized to the object count, bind, display
// flags when `flag`, then the bounding-box refresh 0x140c8e640); no-op for a null object
void viewerSync(GuiModel& m, LLOBJ_DATA* obj, bool flag);
// @src 0x140c7ff80  rebuild the viewer items of every object (after a KEY read into the model)
void viewerRebuild(GuiModel& m, bool a, bool b);
// display+0x202 axis display (1 from the display ctor 0x140cf8e64)
bool displayAxisOn(const Display& d);
// @src 0x140c8e640  refresh of the model's view boxes / rects and its scene boxes
void viewerRefresh(GuiModel& m);
// @src 0x140c7fee0  the model's dimension (engine+0x1c: 2 or 3; 0 without an engine)
int guiModelDim(const GuiModel& m);

// ---- packet moproj: the main window's 3D display and camera (src/engine/viewer_camera.cpp) -----
// What 0x140d05a10 collects for SfDispView (0x140d06610 writes it in this order).
struct CameraState {
    double version = 1.2;          // DAT_1422d3a88
    double quat[4] = {};           // QQuaternion::toVector4D (x, y, z, scalar)
    double eye[3] = {};            // camera +0x1a0
    double view[3] = {};           // +0x1ac
    double up[3] = {};             // +0x1b8
    double zoom = 1.0;             // +0x1c8
    double centerX = 0.0, centerY = 0.0;   // +0x398 pan
    double lrtb[4] = {};           // +0x3b0 left, right, top, bottom
    double origin[3] = {};         // +0x3d0
};
// Actual session primary Display alias; legacy unbound fixtures create lazily.
// Camera geometry here retains the declared historical OracleB profile.
Display& projectDisplay(Project& p);
// @src 0x140cf9360  the display shows model m (its main model slot +0x1a0; m+0x10 = display)
void displayShow(Project& p, GuiModel* m);
// @src 0x140054820  SfNotifier signalRefreshScene(model) -> main window slotRefreshScene
// (0x140ccdd10(model, false): 0x140cf9360 show, display vt+0x1f8) and signalSceneAction("viewFit")
// -> 0x140ccc180 -> the fit 0x140cfe9c0 (both through the notifier guard +0x141, never set here)
void displayRefreshScene(Project& p, GuiModel* m);
// @src 0x140cfb5f0 (display +0x708 == 0)  view reset for the shown model's dimension (2D:
// 0x140d011d0, 3D: the iso view 0x140cff570) and the fit 0x140cfe9c0
void displayResetView(Project& p);
// @src 0x140d05a10  the camera state SfDispView writes
CameraState displayCamera(Project& p);
// @src 0x140c82c70  remove every viewer item (model reset)
void viewerClear(GuiModel& m);
// @src 0x140c82d50 + 0x1411f5040(view, out, false)  the cached box of object no's view, as the
// Qt floats it holds (3D: view+0xd8; all zeros when empty); false when there is no such view
bool viewerBox(GuiModel& m, int objNo, float mn[3], float mx[3]);
// @src 0x140c8e0f0  the model's scene box (2D: the model rect m+0x108 as a box; else m+0x148)
bool viewerSceneBox(GuiModel& m, bool& valid, float mn[3], float mx[3]);

// @src 0x14005f770 / 0x14005f820  the operation's model (class vt+0xe0, else OPI+0x38 created by
// class vt+0x1b0 = 0x1405f6410); null without a class or a project
GuiModel* opModel(Project& p, Operation& op);

// SfPreLoadPattern (the HTFurnace "Load Pattern" editor, form :/SfOprHTFurnaceForm/
// SfOprHTFurnLoadPattern.ui) as far as its values reach the model: src/engine/htf_loadpattern.cpp
struct LoadPatternForm {
    int pattern = 0;                 // rbPattnAlgn / rbPattnRund / rbPattnRand (Algn checked in the .ui)
    bool sym[3] = {};                // cbSymRow, cbSymCol, cbSymLay
    int count[3] = {1, 1, 1};        // sbRows, sbCols, sbLays (1 .. 1000)
    double dist[3] = {};             // dsbDisRow, dsbDisCol, dsbDisLay (0 .. 999999999, 2 decimals)
    int countFxt[3] = {1, 1, 1};     // sbRows_Fxt, sbCols_Fxt, sbLays_Fxt
    double distFxt[3] = {};          // dsbDisRow_Fxt, dsbDisCol_Fxt, dsbDisLay_Fxt
    bool modified = false;           // editor+0x35 (0x140378e70)
};
// @src 0x140c7d3a0  furnace table (sim control +0x440, curve 0xe) into the model's block +0xf8
void furnaceTableToBlock(GuiModel& m);
// @src 0x14106e8b0 (model part)  the editor's ctor: 0x140c7d3a0 when SC+0x3c0, then the form from
// the block (0x141070920)
void loadPatternFormInit(GuiModel& m, LoadPatternForm& f);
// @src 0x141070870  editor vt+0x1c0: with a modified form, 0x141071060 and (SC+0x3c0) 0x140c7d8d0
void loadPatternUpdate(GuiModel& m, const LoadPatternForm& f);

// @src 0x1400569d0  KEY (batch) load of an operation into its model: SimCtrl.KEY / InterObject.KEY,
// materials, object slots (own object files or inherited from predecessors), equipment
GuiModel* loadFromKey(Project& p, Operation& op, bool withObjects, FileEffects& fx);

// @src 0x14003a3c0 -> 0x140047f80(model, -1) -> 0x140051c30: after File > Open the GUI loads the
// active diagram item's operation into its engine model (OB vt+0x58). Returns that operation, or
// null when there is none or its class gate fails.
Operation* openActiveOperation(Project& p, Ctx& ctx, FileEffects& fx);

// @src 0x140052ec0  File > Save step 2, "save current operation" (packet S,
// docs/re/moproj2mst/s/spec.md): `op` is the GUI's current operation (the one
// openActiveOperation loaded). Copies its properties into the engine model and writes the task
// KEY files, equipment and material KEYs into `fx` (nothing is written to disk here). Classes
// whose own save slots are not ported refuse with M2M_E_UNSUPPORTED.
void saveCurrentOperation(Project& p, Operation& op, Ctx& ctx, FileEffects& fx);
// File > Save step 3 (after the .moproj), props vt+0xb8 of the MDT L6 classes: DieStress.Wzd,
// DieStressBmWizard.Wzd, MachDistortion.MDWZ + DEF_MACHDIST_CTRL2.INI_<i>, HeatTreatWizard.HTWZ
// (src/engine/save_l6.cpp); false for other props
bool l6PropsSave(Project& p, Operation& op, FileEffects& fx, Ctx& ctx);
// Save step 4, props vt+0xb8 of SfOprExpressProperties (FormingExpress, HeatTransferExpress):
// Forming.FWZ in the owner's task folder (src/engine/save_l3.cpp); false for other props
bool expressPropsSave(Project& p, Operation& op, FileEffects& fx, Ctx& ctx);

// @src 0x1400547b0(op, 0)  the views step 0x140051340 runs after the load (packet S, spec section
// 9): its only model effect is the equipment sync OB vt+0x230 (Forming, Forming2D: the primary
// die and every object without one get an equipment, created as "Equipment (n)"; MultiBlow: the
// primary die likewise, every other object's equipment is removed from the project and its KEY
// file deleted). Changes the project's equipment rows, the slot records' and the operation's
// equipment ids, and records the created and deleted files in `fx`.
void postLoadViews(Project& p, Operation& op, Ctx& ctx, FileEffects& fx);

// @src 0x140c068c0 (Forming), 0x140c05200 (Forming2D), 0x140607d10 (MultiBlow, multiBlow = true):
// OB vt+0x230, the equipment sync the classes' vt+0x228 bodies end with (packet S, spec section 9).
void equipmentSync(Project& p, Operation& op, bool multiBlow, FileEffects& fx);

// @src 0x140057370  the OPI sync: task folders, SimCtrl.KEY, InterObject.KEY, object, mesh
// settings, RzElmCon/BccDef, ObjProperties, equipment and material KEYs, restart KEY, NSTART.
// Returns the status of the last object write (true without objects); false without a model.
bool syncOperationFiles(Project& p, Operation& op, FileEffects& fx);

// What File > Save writes up to the .moproj: step 2 (the current operation's KEY files) and the
// .moproj itself (step 3).
enum class DialogAnswer : int { NoButton = 0, Yes = 0x4000, No = 0x10000, Cancel = 0x400000 };

// 1400980d0 returns an answer integer, independently of output success. These
// are reached state checkpoints, not a claim of completed event/modal delivery.
struct SaveCallState {
    DialogAnswer answer = DialogAnswer::Yes;
    bool opened = false;
    unsigned checkedSubprojects = 0;
    bool validationComplete = false;
    bool lastOperationModalPending = false;
    DialogAnswer pendingDefault = DialogAnswer::NoButton;
    bool projectWriteAttempted = false;
    bool projectWriteReturned = false;
    bool projectWriteSucceeded = true;
    bool mstWriteAttempted = false;
    bool mstWriteReturned = false;
    bool mstWriteSucceeded = true;
};

struct SaveEffects {
    SaveCallState call;
    Operation* current = nullptr;  // the operation step 2 saved (null: none)
    FileEffects step2;
    FileEffects project;
};

// @src 0x1400980d0  File > Save up to step 3 (src/engine/save_project.cpp): returns at once
// (false) when the open failed (P+0x55); the per sub-project check 0x1400b8fb0 refuses at the
// unrecovered warning modal delivery boundary; 0x140052ec0 on the current operation; 0x1400983c0(P, 0) with the files
// of `open` and step 2 visible to it. Returns the .moproj writer's result. `views`: guiViews(p),
// which refuses the view parts not ported yet. This bool is an adapter helper;
// out.call.answer retains the original independent integer answer.
bool saveProject(Project& p, Ctx& ctx, moproj::DiagramViews& views, const FileEffects& open, SaveEffects& out);
// The same up to step 2 (out.project stays empty): what convert runs before steps 4 and 5 while
// the .moproj views are not ported. False when the open failed (P+0x55).
bool saveCurrent(Project& p, Ctx& ctx, SaveEffects& out);

// ---- MDT DB (src/engine/db_load.cpp, docs/re/moproj2mst/mdt/DB/spec.md) ------------------------
// @src 0x1400a5890  the DB part of the project's data root P+8 (made on first use here)
DbHandle* projectDb(Project& p);
// @src 0x140cdcdc0  setPath(name, keepItems): ".DB" appended unless the name has it or ends with
// "FOR003", cleanPath; a new name makes the handle "not loaded"
void dbSetPath(DbHandle& h, const String& name, bool keepItems);
// @src 0x140cdc480  open(buildCache, async): load_db_info (1401dc3f0), the "2G Split DBs" question
// answered with its default button Yes (warning) -> setup_part_chain (1401e4e80), the header cache
// (1401dd250); false when the path is empty or the file is not a readable DEFORM DB. Read only.
bool dbOpen(DbHandle& h, Ctx& ctx, bool buildCache, bool async);
// File > Open (0x14008a9b0): the data root gets cleanPath(P+0x30) (the default <dir>/<base>.DB of
// the project construction; the walk's <Database Name> comes later and does not reach the handle)
// and is opened with 0x140cdc400 (buildCache true, async false)
void openProjectDb(Project& p, Ctx& ctx);
// @src 0x1400b9cd0  DB handle of a sub-project: P+8, or for a die-stress sub-project (type 4) its
// own handle with the sub-project DB path (0x1400b9e80)
DbHandle* subProjectDb(Project& p, SubProject& sp);
// @src 0x140c7ac50  the model's DB reader (null without a handle)
def::model::Cls_1401dbfe0* modelDbReader(const GuiModel& m);
// @src 0x140d397a0  operation open: re-read the DB when the file changed since the last open,
// else re-read its header (1401dd0d0); false when the file does not exist
bool dbRefreshIfChanged(DbHandle& h);
// One entry of 0x14080f690's list: the operation name of the first stored step of each increasing
// operation number (cache record +0x4c) and that number (+0x38)
struct DbOperation {
    std::string name;
    int opNo = 0;
};
// @src 0x14080f690  operations stored in the DB, in directory order
std::vector<DbOperation> dbOperationList(const def::model::Cls_1401dbfe0& r);
// @src 0x14005f520  dbHasStepsFor(op, &lastMatch, &unused): the chain's operations up to op match
// the DB's (0x1400b8b80) and the previous operation has enough stored steps (one for the classes
// of OB vt+0x1f0, else two)
bool dbHasStepsFor(Project& p, Operation& op, int* lastMatch, int* unused);
// @src 0x14005f270  Interactive (true) or Batch setup: the same test, true for the first two chain
// positions (flow number < 2); reached from DiestressBM2D/3D vt+0x1b8 and the run dialog 0x14012ba20
bool interactiveSetup(Project& p, Operation& op);
// @src 0x1400572e0  loadDbStep(op, step, sync): model reset, the DB step read into the model
// (0x140c7b870, result ignored as in the binary), then the GUI sync 0x1400538f0(op, sync, false)
GuiModel* loadDbStep(Project& p, Operation& op, int step, bool sync, Ctx& ctx, FileEffects& fx);
// @src 0x140c7b870  read DB step *step into the model (step -> directory index 1401ddf60)
bool readDbStep(GuiModel& m, const int* step, bool resetViewer);
// @src 0x140c7b900  read directory entry *idx into the model (1401cc420 and the GUI tables)
bool readDbEntry(GuiModel& m, const int* idx, bool resetViewer);
// @src 0x1400538f0  syncFromModel(op, updateDiagram, writeEqp): material items and Material KEY
// files, slot codes / records / equipment ids from the model's objects, object manager refresh,
// OB vt+0xc8, the diagram item rows
void syncFromModel(Project& p, Operation& op, bool updateDiagram, bool writeEqp, Ctx& ctx, FileEffects& fx);
// @src 0x140057230  restart load: model reset, the restart KEY read into the model, viewer, then
// 0x1400538f0(op, 1, 1)
GuiModel* loadRestart(Project& p, Operation& op, Ctx& ctx, FileEffects& fx);

// GUI start-up: the version text of the KEY banners (0x1407fb240 -> global 0x143dbcbd0), from
// <deformDir>/PACK/pack.dat; empty = the v11.0.2 installation ("11.0 (Service Pack 2)  ").
// Process-wide, like the original.
void setEngineVersion(const std::u16string& deformDir);

}  // namespace m2m::engine
