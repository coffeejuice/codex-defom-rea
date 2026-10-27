// m2m/model.hpp: the project model the MST writers read (seam).
//
// Structs follow the field tables of s1-01 9 and s1-02 9; each field keeps the original's offset in
// a comment so a spec line "OPI+0x150" maps to one member. Ownership:
//   structs ............ G0 wrote them; a packet that needs a new field adds it in its own
//                        "added by" block at the end of the struct (G1 merges)
//   P01 functions ...... file read, DOM walk, item lists               (src/moproj/read.cpp)
//   P02 functions ...... operations, objects, keywords, accessors      (src/model/operation*.cpp,
//                                                                       src/model/accessors.cpp)
//   P03 functions ...... project record, sub-projects, flow chart     (src/model/project.cpp,
//                                                                       src/model/subproject.cpp)
//   P17 functions ...... units and DB paths                             (src/model/units.cpp,
//                                                                       src/model/paths.cpp)
// The original's process globals are per-call state here (the Project or mst::State).
#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "m2m/common.hpp"
#include "m2m/qt4.hpp"
#include "m2m/xml.hpp"
#include "m2m/engine/mo_model_slots.hpp"
#include "m2m/engine/operation_info_guard.hpp"
#include "m2m/engine/operation_info_model_cache.hpp"
#include "m2m/engine/report_document_slot.hpp"

namespace m2m {
using qt::String;

struct Project;
struct SubProject;
struct Operation;
struct FlowItem;
struct Props;           // m2m/props.hpp
class KeyTokenizer;     // below
namespace mst { class OpClass; }   // m2m/mst/writer.hpp (the SfOpr* singleton, "OB")
namespace doe { class Model; struct Study; class StudyIo; class FlowReceiverWitness; class FlowReceiverWitnessRegistry; } // DOE manager and adapter I/O
namespace data { struct CogProcess; }   // m2m/data/cogging.hpp (MDT)
namespace engine { struct GuiModel; struct Display; struct DbHandle; }   // m2m/engine.hpp (MDT L0; Display: packet moproj; DbHandle: MDT DB)

// ---- items of the four managers (s1-01 4, 9.2) ---------------------------------------------------
enum class ItemKind { Object, Material, Equipment, Lubricant };

struct Item {                      // SfMoItem (QStandardItem)
    ItemKind kind{};
    int id = 0;                    // +0x18
    String name;                   // item text (0x14006bb40); read and set through itemName / setItemName
    String keyPath;                // +0x28, built by vt+0x178 (not cleaned: "<dir>/Objects\Object00001.KEY")
    // SfMoObjItem +0x40 -> object data
    int objectType = 7;            // d+0x04
    bool hasGeo = false;           // d+0x18
    bool hasMesh = false;          // d+0x19
    std::vector<int> matIds;       // d+0x20
    // added by MDT L0: the rest of the record d (+0x40, SlotItem layout) that the loader's
    // 0x1400601b0 writes
    int recF08 = 7;                // d+0x08
    String recName;                // d+0x10
    bool recB1c0 = false;          // d+0x1c0
    // SfMoMatItem
    int matType = 0;               // +0x40: 0 Single, 1 Mixture, 2 other
    std::vector<int> phases;       // +0x48
};

struct ItemManager {               // SfMoObjManager / Mat / Eqp / Lub (QStandardItemModel)
    ItemKind kind{};
    std::vector<std::unique_ptr<Item>> rows;   // document order
    Item* find(int id) const;      // vt+0x1c8: first row whose id matches, or null
};

// ---- per-object data of an operation (s1-01 9.4, s1-02 6.4) --------------------------------------
struct SvValue {
    std::int32_t set = 0;          // +0 flag (the readers store 1)
    double v = 0.0;                // +8
};

struct StateVars {                 // OBJ+0x30 (0x188 bytes); offsets below are relative to it
    SvValue ndtmp;                 // +0x00 / +0x08
    SvValue strain;                // +0x10 / +0x18
    SvValue stress;                // +0x20 / +0x28
    SvValue damage;                // +0x30 / +0x38
    std::int32_t urzSet = 0;       // +0x40
    double urz[3] = {};            // +0x48 +0x50 +0x58
    std::int32_t drzSet = 0;       // +0x60
    double drz[3] = {};            // +0x68 +0x70 +0x78
    SvValue grain;                 // +0x80 / +0x88
    SvValue sizesh;                // +0xa0 / +0xa8
    SvValue densty;                // +0xb0 / +0xb8
    SvValue avgstr;                // +0xc0 / +0xc8
    SvValue lmtstr;                // +0xd0 / +0xd8
    std::uint8_t nrecvlFlag = 0;   // +0xe0 (3D)
    std::int32_t nrecvlCount = 0;  // +0xf0; SimCtrl.PPV assigns local+root point count here (MDT L0)
    std::uint8_t erecvlFlag = 0;   // +0xf4
    double erecvlValue = 0.0;      // +0xf8
    std::int32_t erecvlCount = 0;  // +0x100
    std::int32_t objNumber = 0;    // +0x104 (slot + 1)
    // SVSCAL (+0x108 table, +0x118 layout = 7, +0x124 count, +0x130 x values, +0x180 flag);
    // reader 0x1400aba20/0x1400abab0 (P02), emitter 0x14004f050 (P09)
    std::int32_t svLayout = 0;     // +0x118
    std::int32_t svCount = 0;      // +0x124
    std::vector<double> svX;       // +0x130
    std::vector<double> svTable;   // +0x108 (+0x48 data), lookup 0x14089c490
    std::uint8_t svscal = 0;       // +0x180
    // ---- added by P09: header of the +0x108 table, read by the lookup 0x14089c2a0; filled by
    // P02's SVSCAL reader together with svTable (+0x48 data; empty = null)
    std::int32_t svTabKind = 0;    // +0x108+0x04 (0: no table; 1: index = i * dim[3] + l)
    std::int32_t svTabDim[4] = {0, 0, 0, 1};   // +0x108+0x1c, +0x20, +0x24, +0x40 (0x140899010: +0x40 = 1)
    // ---- added by P08 (table ctor 0x140898dc0 -> 0x140899010(t, 0), reader 0x1400abab0) --------
    // svCount (+0x124) and svTabDim[0] are the same int (+0x108+0x1c), svLayout (+0x118) is
    // +0x108+0x10 and svX (+0x130) is the dim-0 axis +0x108+0x28; the P08 functions keep them equal.
    std::int32_t svTabId = 0;      // +0x108+0x00 (0x14089bc90 tests it when kind == 0)

    // @src 0x1400594d0 assigns PPV's total point count to each slot subrecord
    // +0xf0 (0x14004df00); this is the same storage KEY emits as NRECVL count.
    std::int32_t ppvCount() const noexcept { return nrecvlCount; }
    void setPpvCount(std::int32_t count) noexcept { nrecvlCount = count; }
};

struct ObjEntry {                  // OBJ (0x1c8, ctor 0x14004d160, defaults 0x14004d450)
    int number = 0;                // +0x00 slot + 1
    int objectType = 7;            // +0x04 (7 = undefined, 5 = from DB)
    int f08 = 7;                   // +0x08
    String name;                   // +0x10
    bool hasGeo = false;           // +0x18
    bool hasMesh = false;          // +0x19
    std::vector<int> matIds;       // +0x20
    int equipmentId = 0;           // +0x28 (default set by 0x14004de70, not decompiled)
    StateVars sv;                  // +0x30
    bool skipDefine = false;       // +0x1b8
    bool remesh = false;           // +0x1b9
    bool meshSetting = false;      // +0x1ba
    bool mesh2dAbsolute = false;   // +0x1bb
    bool mesh3dFinerInternal = false;  // +0x1bc
    bool f1bd = false;             // +0x1bd
    bool defineBCC = false;        // +0x1be
    bool assignMovement = false;   // +0x1bf
    bool defineRotationMovement = false;  // +0x1c0
    bool thermalStop = false;      // +0x1c1
};

struct ObjPos {                    // OBJPOS record (0xd8, ctor 0x1400658e0), s1-01 7.2, s1-02 6.3
    int mode = 1;                  // +0x00
    int object = 0;                // +0x04
    int i08 = 0;                   // +0x08
    std::array<double, 24> dbl{};  // +0x10 .. +0xc8
    std::vector<int> list;         // +0xd0
    // d(0x30) is the double at byte offset 0x30, as the specs name them.
    double& d(unsigned off) { return dbl[(off - 0x10) / 8]; }
    double d(unsigned off) const { return dbl[(off - 0x10) / 8]; }
};

struct ContactBlock {              // OPI+0x70 (ICB, s1-04 0.4); defaults from 0x1400651f0
    std::array<std::uint8_t, 100> flag{1};   // +0x00 INICTC per object (n-1); [0] = 1
    std::uint8_t enable = 1;       // +0x64 (OPI+0xd4)
    std::uint8_t gen = 1;          // +0x65 (OPI+0xd5)
    double genVal = 0.0;           // +0x68 (OPI+0xd8)
};

struct RecoveryEntry {             // BC of s1-02 6.4: +0 type, +0x18 KEY value; PPV fields occupy the intervening native slots
    int type = 0;
    double value = 0.0;
    // PPV uses the same native list records as KEY recovery state. These named
    // semantic fields preserve PPV's other comparison keys without treating
    // this C++ type as a byte-for-byte native BC overlay.
    int ppvSecondInt = 0;           // native BC +0x04 when populated from PPV
    double ppvFirstDouble = 0.0;    // native BC +0x08 when populated from PPV
    double ppvSecondDouble = 0.0;  // native BC +0x10 when populated from PPV
};

// ---- operation (SfOperationInfo "OPI", s1-01 9.3; "SimItem"/"OpItem" in s1-02..05) -------------
struct Operation {
private:
    // The native OPI and its outer item are folded into this adapter. Keep one
    // stable OPI identity; fallback expiry occurs after all represented members
    // and is NOT the earlier native QObject boundary or accepted Close replay.
    engine::OperationInfoIdentity operationInfo_{*this};
    // Canonical observation for native OPI+0x160. It starts Unknown and does
    // not construct, reset or own the native item model.
    engine::OperationInfoModelCache operationInfoModelCache_{*this};
public:
    Operation() = default;
    ~Operation() = default;
    Operation(const Operation&) = delete;
    Operation& operator=(const Operation&) = delete;
    Operation(Operation&&) = delete;
    Operation& operator=(Operation&&) = delete;
    engine::OperationInfoIdentity& operationInfoIdentity() noexcept { return operationInfo_; }
    const engine::OperationInfoIdentity& operationInfoIdentity() const noexcept { return operationInfo_; }
    const engine::OperationInfoModelCache& operationInfoModelCache() const noexcept {
        return operationInfoModelCache_;
    }
    bool hasEngineModel() const noexcept { return static_cast<bool>(engineModel); }
    bool sameEngineModelOwner(const std::weak_ptr<engine::GuiModel>& expected) const noexcept {
        return !engineModel.owner_before(expected) && !expected.owner_before(engineModel);
    }
    // Future qualified OPI driver must complete payload/member cleanup first.
    // No normal destruction or public Close path commits this automatically.
    bool commitOperationInfoQObjectBoundary(const std::string& qualification) {
        return operationInfo_.commitQObjectBoundary(qualification);
    }

    std::shared_ptr<Props> props;  // +0x10
    std::uint8_t nstartGiven = 0;  // +0x18
    int startStepNo = 0;           // +0x1c
    std::shared_ptr<std::vector<RecoveryEntry>> recoveryB =       // +0x20 (list B, s1-02 6.4)
        std::make_shared<std::vector<RecoveryEntry>>();
    std::shared_ptr<std::vector<bool>> recoveryFlags;             // +0x28 (F, may be null)
    std::vector<int> objIds;       // +0x40 object id per slot (-1 none, -2 from DB)
    int lubricantId = -1;          // +0x48
    int equipmentId = -1;          // +0x4c
    std::vector<std::unique_ptr<ObjEntry>> objs;       // +0x50 (grown on demand)
    std::vector<std::unique_ptr<ObjPos>> objpos;       // +0x60
    ContactBlock icb;              // +0x70 .. +0xd8
    std::uint8_t lubSystem = 0;    // +0xe0
    double tloc = 0.0;             // +0xe8
    Project* project = nullptr;    // +0x100
    int id = 0;                    // +0x108
    int oprFlag = 0;               // +0x10c (the reader sets 1 unless <OprFlag>)
    int f110 = 0;                  // +0x110
    String name;                   // +0x118 (unique)
    String classKey;               // +0x120 (registry key, *!SIMTYP)
    String subType;                // +0x128 (<SubType> or legacy Type)
    String taskDir;                // +0x130 "Operations/TaskNNNNN"
    std::uint8_t userKeyGiven = 0; // +0x140
    String userKey;                // +0x148
    std::uint8_t restartGiven = 0; // +0x150
    String restartKey = String(u"RestartStep.KEY");   // +0x158
    // SfMoOprItem (owner of the OPI)
    String diagName;               // item+0x50 "<Type>DiagItem"
    mst::OpClass* opClass = nullptr;   // cached 0x140054e90 lookup (registry[classKey])
    // ---- added by P07: raw OPI fields only the ctor/copy (0x140050550, 0x1400500b0) touch ----
    std::uint64_t f30 = 0;         // +0x30 (0x14005fa60 sets 0)
    std::uint64_t ff0 = 0;         // +0xf0
    std::uint64_t ff8 = 0;         // +0xf8
    std::uint64_t f138 = 0;        // +0x138
    // ---- added by MDT L0: the engine model (QPointer OPI+0x38, created by class vt+0x1b0) -------
    std::shared_ptr<engine::GuiModel> engineModel;
    // +0x38 engine model lifetime and +0x160 item-model ownership are not
    // modelled; the private cache above records only Unknown identity.
};

// ---- flow chart of a sub-project (s1-01 8, s1-02 4.2) --------------------------------------------
// One row of a diagram item's expansion list (item vt+0x50): SfDiagItemExpansion (ctor 0x140187590)
struct ExpansionRow {
    int type = 0;                  // +0x10: 0 Text, 1 Line, 2 Space
    String label;                  // Text: indent (+0x14, 2) spaces + text, as vt+0x10(.., 1) returns it
    int lineStyle = 1;             // Line: vt+0x90 (Qt::PenStyle: 0 NoLine, 1 Solid, 2 Dash, ...)
    int r = 0, g = 0, b = 0, a = 255;   // Line: vt+0xa8 colour
    std::vector<String> accepts;   // +0x48 accepted mime types (vt+0x38 / vt+0x40)
};

enum class ObjectRowPortDirection { Input, Output };

// Internal row-port state from 0x140082760 / 0x140160f50. Identity is the
// owning FlowItem instance, its zero-based object slot and direction;
// Operation_Id is not unique. ExpansionRow copies do not own or copy ports.
struct ObjectRowPort {
    ObjectRowPort(FlowItem& item, std::size_t slot, ObjectRowPortDirection dir)
        : owner(&item), rowSlot(slot), direction(dir) {}
    ObjectRowPort(const ObjectRowPort&) = delete;
    ObjectRowPort& operator=(const ObjectRowPort&) = delete;
    ObjectRowPort(ObjectRowPort&&) = delete;
    ObjectRowPort& operator=(ObjectRowPort&&) = delete;

    FlowItem* const owner;
    const std::size_t rowSlot;
    const ObjectRowPortDirection direction;
    String type = String(u"Object2d");
    int capacity = 1;
    // Native connections are ordered and reciprocal. Construction leaves this
    // empty; callback dispatch and row removal have separate ownership.
    std::vector<ObjectRowPort*> peers;
};

struct ObjectRowPorts {
    std::optional<ObjectRowPort> input;    // native row-private +0x30
    std::optional<ObjectRowPort> output;   // native row-private +0x38
};

struct FlowItem {                  // diagram item (model+0x20 list)
    FlowItem() = default;
    ~FlowItem() noexcept;
    FlowItem(const FlowItem&) = delete;
    FlowItem& operator=(const FlowItem&) = delete;
    FlowItem(FlowItem&&) = delete;
    FlowItem& operator=(FlowItem&&) = delete;

    int opId = 0;                  // vt+0x28 Operation_Id
    String className;              // after the legacy mapping
    String itemName;
    double rect[4] = {};           // RectF as QRectF (x, y, width, height); vt+0x90 / vt+0x98
    int row = 0, col = 0;          // CellPos (vt+0xc8)
    bool group = false;            // vt+0x58 flag 0x1000 (Cycle/Group)
    int beginRow = 0, beginCol = 0, endRow = 0, endCol = 0;   // GroupInfo (vt+0xd8 / +0xe8)
    std::vector<std::pair<String, int>> itemData;             // ItemData attributes (CycleCount)
    std::vector<ExpansionRow> expansion;   // vt+0x50, built when the item is created (0x140073900)
    FlowItem* next = nullptr;      // conn+0x130 successor
    FlowItem* prev = nullptr;      // conn+0x138 predecessor
    // Map nodes preserve port addresses as more expansion slots are installed.
    // Keys are zero-based object slots (expansion index 7 + slot), not
    // operation IDs or copied row values. The selected construction is Forming2D.
    std::map<std::size_t, ObjectRowPorts> objectRowPorts;
    // Diagnostic lifetime identity: a weak observer must reject an item
    // destroyed and reallocated at the same address. Not serialized or ABI.
    std::shared_ptr<int> diagnosticLifetime = std::make_shared<int>(0);
};

inline FlowItem::~FlowItem() noexcept
{
    // Inline because the P11 namespace-renamed source double instantiates
    // FlowItem without linking the original model's out-of-line lifetime unit.
    const auto detach = [](ObjectRowPort& port) noexcept {
        while (!port.peers.empty()) {
            ObjectRowPort* peer = port.peers.back();
            port.peers.pop_back();
            if (!peer) continue;
            auto& links = peer->peers;
            for (auto it = links.begin(); it != links.end();) {
                if (*it == &port) it = links.erase(it);
                else ++it;
            }
        }
    };
    for (auto& [slot, ports] : objectRowPorts) {
        (void)slot;
        if (ports.input) detach(*ports.input);
        if (ports.output) detach(*ports.output);
    }
}

// Internal Forming2D construction helper only: expansion[7 + rowSlot] must be Text.
// Native 0x140082760 ensures input before output and retains existing ports.
ObjectRowPorts& ensureObjectRowPorts(FlowItem& item, std::size_t rowSlot);

// Internal Object2d connector operations. Native +0x88 clears each endpoint's
// prior reciprocal peer before +0x78 links the selected output/input pair.
// Notification delivery belongs to the owning graph/model call site.
void clearObjectRowPort(ObjectRowPort& port) noexcept;
void linkObjectRowPorts(ObjectRowPort& fromOutput, ObjectRowPort& toInput);

struct FlowChart {                 // SfMoDiagEditorModel
    std::vector<std::unique_ptr<FlowItem>> items;
    int activeItem = -1;           // index set by Active (model vt+0xf8)
    std::map<int, int> numberCache;   // +0x40, built by 0x140085800
    bool numberCacheBuilt = false;
};

struct SubProject {                // SUB (0x50, ctor 0x1400b5470)
    SubProject() = default;
    ~SubProject() noexcept;
    FlowChart flow;                // +0x00
    bool loaded = false;           // +0x08
    Project* project = nullptr;    // +0x10
    int type = 0;                  // +0x18 (0 main, 2 DOE, 3 optimisation, 4 die stress)
    int id = 0;                    // +0x1c
    doe::Model* doe = nullptr;     // +0x20 alias of ownedDoeStudy for recovered Type2
    std::shared_ptr<doe::Study> ownedDoeStudy; // manager ownership; existing writers use the alias
    String dbName;                 // +0x38 Subproject_DbName DbName

    // ---- added by P01 ----
    // Recovery list A of the per-sub-project data root (sub+0x48) that 0x1400b9cd0 creates on
    // first use for a die-stress sub-project (type 4); see Project::recoveryA.
    std::shared_ptr<std::vector<RecoveryEntry>> ownRootRecoveryA;
    // ---- added by MDT DB: the DB part of that own data root (QPointer sub+0x48, 0x1400b9cd0) ----
    std::shared_ptr<engine::DbHandle> ownDb;
    // CP82: actual SUB+0x30 guard projection. Unknown until explicitly observed;
    // no document allocation/deletion, default-null inference or destructor hook.
    engine::ReportDocumentSlot reportDocumentSlot;
private:
    friend std::unique_ptr<SubProject> makeSubProject(Project&, int, int);
    // Adapter lifetime witness, not a native SUB field or QObject completion.
    std::weak_ptr<doe::FlowReceiverWitnessRegistry> flowReceiverRegistry_;
    std::shared_ptr<doe::FlowReceiverWitness> flowReceiverWitness_;
};

// ---- project (P, s1-01 9.1) ----------------------------------------------------------------------
struct Project {
public:
    // Complete feature-context types live in a model-private header. Project
    // retains the same owning pointer and physical layout.
    enum class FeatureContextStage : int;
    enum class FeatureContextStop : int;
    class FeatureContextAnchor;
    struct FeatureContextAttemptReport;
private:
    friend std::unique_ptr<SubProject> makeSubProject(Project&, int, int);
    friend void initProject(Project&, const String&, const String&);
    // Registry lease outlives slot storage; marker expires after all ordinary
    // Project members. These anchors are adapter owners, not native fields.
    std::shared_ptr<engine::OperationClassRegistry> registryAnchor_;
    std::shared_ptr<engine::MoModelSlots> slotsAnchor_;
    std::shared_ptr<const engine::ProjectStorageLifetime> slotBorrow_;
    std::shared_ptr<FeatureContextAnchor> featureContext_; // same Project epoch; no native effects in destruction
    std::weak_ptr<doe::FlowReceiverWitnessRegistry> flowReceiverRegistry_;
    bool flowReceiverRegistryBound_ = false;
    bool flowReceiverConstructionStarted_ = false;
    bool preparedFeatureContextModel(bool beforeBacklink) const noexcept;
    void beginFeatureContextConstructorAttempt() noexcept;
public:
    Project();
    Project(std::shared_ptr<engine::MoModelSlots>,
            std::shared_ptr<engine::OperationClassRegistry>);
    ~Project();
    Project(const Project&) = delete;
    Project& operator=(const Project&) = delete;
    Project(Project&&) = delete;
    Project& operator=(Project&&) = delete;
    const std::shared_ptr<engine::MoModelSlots>& modelSlotsOwner() const noexcept;
    const std::shared_ptr<engine::OperationClassRegistry>& registryOwner() const noexcept;
    std::shared_ptr<const FeatureContextAnchor> featureContextAnchor() const noexcept;
    FeatureContextAttemptReport featureContextAttemptReport() const noexcept;
    // Bind before initProject creates SUB0; never create a notifier/registry here.
    void bindFlowReceiverWitnessRegistry(const std::shared_ptr<doe::FlowReceiverWitnessRegistry>&);
    std::shared_ptr<doe::FlowReceiverWitnessRegistry> flowReceiverWitnessRegistry() const;

    // Per-conversion snapshot, not an original Project member/layout field.
    OriginalSettings originalSettings;
    int tokMode = 0;               // +0x00 KEY tokenizer mode
    int f10 = 5;                   // +0x10
    String path;                   // +0x18 .moproj path (cleaned)
    int unit = 0;                  // +0x20 1 SI, 2 English (ctor: start-up global, s1-01 Q2)
    int geotyp = 0;                // +0x24
    String title;                  // +0x28
    String primaryDb;              // +0x30
    std::shared_ptr<xml::Document> dom;   // +0x48
    int activeSub = 0;             // +0x50
    // the Operation Editor's diagram view height in the fresh-GUI profile (moproj::viewCentre): 123
    // after open; 175 when the project's database opens (openProjectDb, db_load.cpp)
    int diagramViewHeight = 123;
    bool openedOk = false;         // +0x55
    std::vector<std::unique_ptr<SubProject>> subs;   // +0x58, [0] = main

    // data root M (+0x08): the managers
    // Borrowed aliases to the SAME Mo-owned manager representations. Like all
    // reference members, these do not propagate const from Project; code must
    // use the capsule const getters when a const manager view is required.
    engine::MoModelSlots::OperationRows& operations; // M+0x19c8
    ItemManager& objects;        // M+0x19d0
    ItemManager& materials;      // M+0x19d8
    ItemManager& equipment;      // M+0x19e0
    ItemManager& lubricants;     // M+0x19e8

    // Registry of operation classes (mainWindow+0x2b8, 0x14006dba0): one instance per class and
    // conversion, keyed by class key. Filled by P02 (make_registry).
    engine::OperationClassRegistry::Entries& registry; // borrowed session registry

    // Files the original writes at open (ObjPropertiesNNNNN.KEY, SimCtrl.KEY, InterObject.KEY,
    // MoItem Data) are not written; their bytes are kept here, keyed by the cleaned absolute path
    // (s1-01 Q1). Readers consult it before the disk.
    std::map<String, std::string> overlay;

    // ---- added by P03 (ctor 0x140089880, activation 0x1400a7b90); no MST writer reads them ----
    std::uint8_t f38 = 1;          // +0x38
    std::uint8_t f39 = 1;          // +0x39 (0x1400a4150; 0 only for "NewProject")
    std::uint8_t f3a = 0;          // +0x3a (0x1400a41f0)
    int f3c = 5;                   // +0x3c
    std::uint8_t f40 = 0;          // +0x40
    bool loadingSub = false;       // +0x54 set while 0x14008f530 parses an activated sub-project
    // ---- added by P06: process globals of the Cogging.MDT reader that outlive one file ---------
    struct CogGlobals {
        int noDies = 2;            // 0x1437e3440 (.data initial value 2)
        int dieMov = 1;            // 0x1437e3444 (.data initial value 1)
        int unit = 1;              // 0x143da6de8 COG_FNC_SetUnits (1 SI, 2 English)
        int mergeL = 1;            // 0x1437e3a7c (.data 1; the BILLET reader stores MergeL here)
        double axialSub = 0.0;     // 0x143db1a00 (.bss; the BILLET reader stores AxialSub here)
        // added by MDT: the other globals the Cogging.MDT writer reads (data/cogging.hpp);
        // created by the first SfOprCogData ctor or MDT read (datafiles::cogProcess)
        std::shared_ptr<data::CogProcess> proc;
    } p06Cog;
    // ---- added by P01 ----
    // Data root M+0x19a8: QList<BC*>* recovery list A (0x14005a400; NRECVL/ERECVL of s1-02 6.4).
    // Nothing on the open path writes it and no corpus MST has NRECVL/ERECVL; 0x1400613b0 uses it
    // without a null check, so it is an existing, empty list. Its filler (data root ctor
    // 0x140cda460 or the DB engine) was not traced.
    std::shared_ptr<std::vector<RecoveryEntry>> recoveryA = std::make_shared<std::vector<RecoveryEntry>>();
    // ---- added by P14 ----
    // 0x143d7b7a8 / 0x143d7b7a4: when set (DOE sample loop), 0x140068210 stores doeSimPos as the
    // position of every simulation entry. Never set on Save.
    bool doeSimPosSet = false;
    int doeSimPos = 0;
    // 0x143d9ef10: the MultiBlow root template name, filled by the first MultiBlow props made
    // (0x140375640 first-wins). Per conversion; seeded from the .moproj (plan 8).
    bool multiBlowRootSet = false;
    String multiBlowRoot;
    // ---- added by packet moproj (SfDispView) ----
    // The main window's 3D display (SfGLDisplay, ctor 0x140cf8b00) with its camera
    // (src/engine/viewer_camera.cpp); made on first use.
    std::shared_ptr<engine::Display> display;
    // Native only for file-writing Save; recording explicitly simulates constructor effects.
    std::shared_ptr<doe::StudyIo> doeIo;
    // ---- added by MDT DB ----
    // The DB part of the data root (P+8, 0x1400a5890): path, loaded flag and the DB reader
    // (src/engine/db_load.cpp). The GUI creates the data root at start-up (0x140043fe0); here it is
    // made on first use (engine::projectDb), at the latest by File > Open (0x14008a9b0).
    std::shared_ptr<engine::DbHandle> dataDb;
};

// The project of the conversion running on this thread (set by convert.cpp for one conversion;
// thread-local, so parallel conversions on different threads do not share it). For class slots
// whose signature has no Project, e.g. OB vt+0x98 makeProps (added by P14).
Project* currentProject();
struct ProjectScope {
    explicit ProjectScope(Project& p);
    ~ProjectScope();
    ProjectScope(const ProjectScope&) = delete;
    ProjectScope& operator=(const ProjectScope&) = delete;
private:
    Project* prev_;
};

// =================================================================================================
// P01: open path (s1-01 1-4)
// =================================================================================================
// @src 0x14008a9b0  OpenProject(P, file): suffix dispatch, P+0x18, then 0x14008afa0. Returns the
// original's P+0x55 (the parse result), not its always-true return value.
// Returns accepted-suffix status; openedOk separately stores the XML read result.
bool openProject(Project& p, const String& file, Ctx& ctx);
// @src 0x14008afa0  ReadProjectFile: QFile read (text mode), setContent, then 0x14008b410
bool readProjectFile(Project& p, Ctx& ctx);
// @src 0x14008b410  WalkProject: Title, GeoTyp, Unit, Primary_DB_Name, Sub_Project, lists, operations
bool walkProject(Project& p, const xml::Document& doc, Ctx& ctx);
// @src 0x14008e290  item list: each MoItem child -> 0x14008e3d0
void parseItemList(Project& p, const xml::Node& list, ItemManager& mgr, Ctx& ctx);
// @src 0x14008e3d0  ParseMoItem (Data blobs go to the overlay)
void parseMoItem(Project& p, const xml::Node& item, ItemManager& mgr, Ctx& ctx);
// @src 0x140068e60 / 0x140042850 / 0x1400bef30 / 0x1400bd910  manager vt+0x178: KEY path of an item
String itemKeyPath(const Project& p, ItemKind kind, int id);
// @src 0x1400a2150  set unit (P+0x20); the original also sets engine globals (kept per call)
void setUnit(Project& p, int unit);
// @src 0x1400a4da0  SetPrimaryDb: P+0x30 = cleanPath(projectDir + sep + name) if that file exists
void setPrimaryDb(Project& p, const String& name);

// ---- added by P01: accessors moved from P02 (called by P09) --------------------------------------
// @src 0x14005a400  recovery list A: data root of the active sub-project (0x1400b9cd0) +0x19a8;
// null when the operation has no project or the project no active sub-project
std::vector<RecoveryEntry>* recoveryListA(Operation& op);
// @src 0x14005fc10  slot number: slot + 1, 0 when the slot is out of range (0x140060600)
int slotNumber(const Operation& op, int slot);
// @src 0x140060640  slot object id is valid: own (> 0) or "Read From DB" (-2) (P01)
bool slotHoldsObject(int id);
// @src 0x14005fb80  first slot whose object id is valid (0x140060640) and equals id, -1 if none
int slotOfObject(const Operation& op, int id);
// @src 0x140061310  object entry of the slot holding id (0x14005fb80 + 0x140061250), null if none
ObjEntry* objEntryById(Operation& op, int id);
// @src 0x14005ff30  object name for DOE text: "Read From DB" for id -2, else the entry's name
// (null string when there is no entry)
String objDisplayName(Operation& op, int id);

// =================================================================================================
// P02: operations (s1-01 5-7, 10)
// =================================================================================================
// @src 0x14006dba0  build the class registry (one OpClass per key, 30 keys; insert 0x14006f940)
void makeRegistry(Project& p);
// @src 0x140047c00  registry lookup by class key (null if absent)
mst::OpClass* findClass(const Project& p, const String& classKey);
// @src 0x14008cb50  Operation_List
bool parseOperationList(Project& p, const xml::Node& list, Ctx& ctx);
// @src 0x14008ccf0  ParseOperation(P, e, withProps, ordinal)
bool parseOperation(Project& p, const xml::Node& op, bool withProps, int ordinal, Ctx& ctx);
// @src 0x140046cc0  CreateOperation (factory, legacy type mapping, unique name)
Operation* createOperation(Project& p, const String& name, const String& type,
                           const String& diagName, int id);
// @src 0x140047630  unique operation name
String uniqueName(const Project& p, const String& name);
// @src 0x14004fcd0 + 0x140050550 + 0x140050820  OPI ctor, defaults, id/task folder (no mkpath)
void initOperation(Operation& op, Project& p, int id);
// @src 0x140093c30  Objects of an operation
void parseObjects(Project& p, const xml::Node& objects, Operation& op, Ctx& ctx);
// @src 0x140093ab0  ObjPropertiesKey children -> 0x1400937b0
void parseObjPropertiesKey(Project& p, const xml::Node& obj, Operation& op, int slot, Ctx& ctx);
// @src 0x1400937b0  WriteBlob: last Data child, qUncompress(fromHex), stored in the overlay
void storeBlob(Project& p, const xml::Node& e, const String& path, Ctx& ctx);
// @src 0x140095e40  NSTART
void parseNstart(Project& p, const xml::Node& e, Operation& op);
// @src 0x140096010  UserDefinedKey
void parseUserDefinedKey(Project& p, const xml::Node& e, Operation& op);
// @src 0x1400961d0  RestartKey
void parseRestartKey(Project& p, const xml::Node& e, Operation& op);
// @src 0x140094f70  Properties (tree reset, then 0x1400952c0 per Property)
void parseProperties(Project& p, const xml::Node& e, Operation& op, Ctx& ctx);
// OPI field accessors of the NSTART / UserDefinedKey / RestartKey parse (P08, P02 split; s1-01 5.5)
// @src 0x14005a2a0 / 0x14005a2c0  OPI+0x18 NSTART given
bool nstartGiven(const Operation& op);
void setNstartGiven(Operation& op, bool v);
// @src 0x14005a2e0 / 0x14005a350  OPI+0x1c StartStepNo
int startStepNo(const Operation& op);
void setStartStepNo(Operation& op, int v);
// @src 0x14005a370  OPI+0x18 = 0, OPI+0x1c = 0 (defaults 0x140050550)
void resetNstart(Operation& op);
// @src 0x140062e60 / 0x140062ed0  OPI+0x140 UserDefinedKey given, OPI+0x148 its file
void setUserKeyGiven(Operation& op, bool v);
void setUserKey(Operation& op, const String& s);
// @src 0x140064740 / 0x140064760  OPI+0x150 RestartKey given (setter: emit is the signal flag)
bool restartGiven(const Operation& op);
void setRestartGiven(Operation& op, bool v, bool emit);
// @src 0x140064c60  OPI+0x158 restart KEY file name
void setRestartKey(Operation& op, const String& s);
// @src 0x1400ab710  qUncompress of a blob (QByteArray -> QByteArray)
std::string uncompressBlob(const std::string& bytes, std::string* error);
// Setters ParseKeywordText (0x140096390, P09) calls; moved P09 -> P08 (src/model/objdata.cpp).
// Every flag is a byte store in the original.
// @src 0x1400abc90  OPI+0xe8 TLOC
void setTloc(Operation& op, double t);
// @src 0x1400ab6f0  ICB+0x70 (= OPI+0xe0) lubricant InterObjectMethod "System"
void setLubSystem(Operation& op, std::uint8_t v);
// @src 0x140065270  ICB[n - 1] INICTC flag of object n
void setIcbFlag(ContactBlock& c, int n, std::uint8_t v);
// @src 0x1400ab7c0  ICB+0x64 (any INICTC)
void setIcbEnable(ContactBlock& c, std::uint8_t v);
// @src 0x1400ab7e0  ICB+0x65 (GENCTC present)
void setIcbGen(ContactBlock& c, std::uint8_t v);
// @src 0x140061e30  append an OBJPOS record (a new one from 0x1400658e0 when p is null); returns it
ObjPos* newObjpos(Operation& op, std::unique_ptr<ObjPos> p = nullptr);
// @src 0x1400658e0  OBJPOS record defaults (mode 1, the rest 0; +0x90 and +0xa8 are not set)
void initObjpos(ObjPos& p);
// @src 0x1400abc40  clear the record's list (+0xd0)
void objposClearList(ObjPos& p);
// @src 0x1400abc60  append to the record's list
void objposAppend(ObjPos& p, int v);
// State variables of one object (OBJ+0x30); value setters take the double, flag setters the byte.
void svSetNdtmpFlag(StateVars& sv, std::uint8_t v);   // @src 0x1400ab800  +0x00
void svSetNdtmp(StateVars& sv, double v);             // @src 0x1400ab820  +0x08
void svSetStrainFlag(StateVars& sv, std::uint8_t v);  // @src 0x1400ab840  +0x10
void svSetStrain(StateVars& sv, double v);            // @src 0x1400ab860  +0x18
void svSetStressFlag(StateVars& sv, std::uint8_t v);  // @src 0x1400ab880  +0x20
void svSetStress(StateVars& sv, double v);            // @src 0x1400ab8a0  +0x28
void svSetDamageFlag(StateVars& sv, std::uint8_t v);  // @src 0x1400ab8c0  +0x30
void svSetUrzA(StateVars& sv, double v);              // @src 0x1400ab8e0  +0x48
void svSetUrzB(StateVars& sv, double v);              // @src 0x1400ab900  +0x50
void svSetDrzB(StateVars& sv, double v);              // @src 0x1400ab920  +0x70
void svSetGrainFlag(StateVars& sv, std::uint8_t v);   // @src 0x1400ab940  +0x80
void svSetDenstyFlag(StateVars& sv, std::uint8_t v);  // @src 0x1400ab960  +0xb0
void svSetDensty(StateVars& sv, double v);            // @src 0x1400ab980  +0xb8
void svSetAvgstrFlag(StateVars& sv, std::uint8_t v);  // @src 0x1400ab9a0  +0xc0
void svSetAvgstr(StateVars& sv, double v);            // @src 0x1400ab9c0  +0xc8
void svSetLmtstr(StateVars& sv, double v);            // @src 0x1400ab9e0  +0xd8
void svSetSvscalFlag(StateVars& sv, std::uint8_t v);  // @src 0x1400aba00  +0x180
// @src 0x1400aba20  SVSCAL a b: a == 1 -> layout (+0x118) = 7; count (+0x124) = b; then the table
// rows are read with `tok` (0x1400abab0). True when the reader returns 0 (always).
bool svscalSet(StateVars& sv, int a, int b, KeyTokenizer& tok);
// @src 0x1400abab0  read the +0x108 table: nothing unless kind (+4) != 0 or nvals (+0x40) >= 2;
// otherwise size it (0x14089b840) and, for kind 1, read dim0 rows of x + nvals values (type 10)
int svscalReadTable(StateVars& sv, KeyTokenizer& tok);
// @src 0x140095cc0  ActionKeywords: each Keyword text -> 0x140096390
void parseActionKeywords(Project& p, const xml::Node& e, Operation& op, Ctx& ctx);
// @src 0x140096390  ParseKeywordText (TLOC, OBJPOS, INICTC, GENCTC, per-object keywords)
void parseKeywordText(Project& p, const String& text, Operation& op, Ctx& ctx);
// @src 0x1400651f0  InterObject block init of the OPI (ICB defaults, OPI+0xe0 = 0) (P09)
void initInterObjectBlock(Operation& op);

// Accessors the writers use (s1-01 10, s1-02 6, s1-04 0.3). Trivial field reads are members above;
// these are the ones with logic.
// @src 0x140054e90  op class (registry[classKey]), null if absent
mst::OpClass* opBase(const Operation& op);
// @src 0x14005f0f0  dimension: opBase->v190(), 3 if no op class
int opDim(const Operation& op);
// @src 0x14005f150
bool is2D(const Operation& op);
// @src 0x14005f1c0
bool is3D(const Operation& op);
// @src 0x14005a300  NSTART value: nstartGiven ? (v < 0 ? -1 - v : v) : 0
int startStep(const Operation& op);
// @src 0x14005f230  isChainHead(op) || restartGiven
bool startsFromKey(const Operation& op);
// @src 0x14005fa80  object slot count (objIds size)
int objSlots(const Operation& op);
// @src 0x14005fb30  object id of a slot, -99 when out of range
int objId(const Operation& op, int slot);
// @src 0x140061250  object entry of a slot, growing objs with defaults; null when out of range
ObjEntry* objEntry(Operation& op, int slot);
// @src 0x1400612d0  object entry by 1-based number
ObjEntry* objEntryN(Operation& op, int n);
// @src 0x140060030  object item of the project for an id (SfMoObjItem), null if none
Item* objItem(const Operation& op, int id);
// @src 0x140068a80 / 0x14006b9f0  item vt+0x48 name: an object item's is its record's name (+0x40
// record +0x10, 0x14004d730), not the item text; the other items return their text
String itemName(const Item& it);
// @src 0x140068a40 / 0x14006b9c0  item vt+0x40 setName: the item text (0x14006bb40) and, for an
// object item, its record's name first (0x14004d700)
void setItemName(Item& it, const String& name);
// @src 0x140050df0  taskDirAbs: convertSeparators(cleanPath(projectDir + sep + taskDir)), null if no project
String taskDirAbs(const Operation& op);
// @src 0x140059290  cleanPath(taskDirAbs + sep + "SimCtrl.KEY")
String simCtrlPath(const Operation& op);
// @src 0x14005a690  cleanPath(taskDirAbs + sep + "InterObject.KEY")
String interObjectPath(const Operation& op);
// @src 0x14005cf70 / 0x14005da90 / 0x14005eb10  cleanPath(taskDirAbs + sep + "%1%2.KEY"(prefix, slot+1))
String meshSettingsPath(const Operation& op, int slot);
String bccDefPath(const Operation& op, int slot);
String objPropertiesPath(const Operation& op, int slot);
// @src 0x140064a50  restart KEY path (raw = true: cleanPath(taskDirAbs + sep + restartKey))
String restartKeyPath(const Operation& op, bool raw);
// @src 0x140050b20  1-based flow-chart number (groups share one) in the ACTIVE sub-project, 0 if none
int flowNumber(const Operation& op);
// @src 0x140050bc0  no predecessor in the active sub-project's chain
bool isChainHead(const Operation& op);

// KEY line tokenizer of the ActionKeywords and Cycle.CWZ readers (s1-01 7.1). The original keeps
// its buffer in globals; here it is one object per text.
class KeyTokenizer {
public:
    KeyTokenizer(const Project& p, const String& text);
    // @src 0x140906e40  tok(type, out, idx): idx == -5000 reads the next line and returns its token
    // count (-2 at the end, type 2 then gives "DONE"); idx > 0 = token idx of the line; else next.
    int tok(int type, void* out, int idx = 0);
    // The same read through another tokenizer object on the same stream and globals (the original's
    // fresh tokenizer of the SVSCAL array reader 0x1400abab0 has mode 0, not P+0x00).
    int tokWithMode(int mode, int type, void* out, int idx = 0);
private:
    struct Impl;
    std::shared_ptr<Impl> impl_;
};

// =================================================================================================
// P03: project record, sub-projects, flow chart (s1-01 1.1, 8, 9.1)
// =================================================================================================
// @src 0x140089880  project ctor defaults (P+0x10 = 5, sub-project 0, unit global, ...)
void initProject(Project& p, const String& dir, const String& base);
// @src 0x1400b5470  SubProject ctor (own flow chart)
std::unique_ptr<SubProject> makeSubProject(Project& p, int type, int id);
// @src 0x14008f200  partial parse of sub-projects >= 1 at open (DbName)
void parseSubProjectDbName(Project& p, const xml::Node& e, int idx);
// @src 0x1400a7250 -> 0x1400a7b90  activate sub-project idx (parses it on first activation)
void activateSubProject(Project& p, int idx, Ctx& ctx);
// @src 0x14008f530  ParseSubProject: DiagramItem, Subproject_DbName, connections
void parseSubProject(Project& p, const xml::Node& e, Ctx& ctx);
// @src 0x140091f00  ParseDiagramItem (with 0x140091db0, 0x140092ff0, 0x140093140)
FlowItem* parseDiagramItem(Project& p, FlowChart& flow, const xml::Node& e, int k,
                           std::map<int, int>& next);
// @src 0x1400a78e0  idx-th Sub_Project element of the stored DOM
const xml::Node* subProjectElement(const Project& p, int idx);
// @src 0x1400a75a0  Name attribute of that element ("" if absent)
String subProjectName(const Project& p, int idx);
// @src 0x1400b6630  MST order: operation ids of the flow chain, groups collapsed (s1-01 8.3)
std::vector<int> mstOrder(const SubProject& sub);
// @src 0x1400b7470  members of a cycle/group item in flow order (0x140084f00, 0x140085470, 0x140084ad0)
std::vector<int> groupMembers(const SubProject& sub, int groupId);
// @src 0x140085800  1-based chain number of an operation id (cached in flow), 0 if unknown
int flowChainNumber(FlowChart& flow, int opId);
// @src 0x140086720  predecessor id of an operation id, -1 when none or unknown
int flowPredecessor(const FlowChart& flow, int opId);
// @src 0x1400a7390  active sub-project (subs[activeSub]) or null
SubProject* activeSubProject(const Project& p);

// The SfMoDiagEditorModel slots the engine's messages (0x1402cf3c0) reach: they rewrite the object
// rows of the expansion lists of the active sub-project's items with Operation_Id opId.
// @src 0x14007b630  slotDiagEditorObjDataAdded(id, index, name): a new object row (index = row count
// - 7) or the row's text
void diagramObjDataAdded(Project& p, int opId, int index, const String& name);
// @src 0x14007c710  slotDiagEditorObjDataChanged(id, index, name): object row index's text
void diagramObjDataChanged(Project& p, int opId, int index, const String& name);
// @src 0x14007d730  slotDiagEditorEqpDataAdded(id, index, name): for index -1 the first row that
// accepts equipment reads "<object>: <name>"
void diagramEqpDataAdded(Project& p, int opId, int index, const String& name);
// @src 0x14007d1b0  slotDiagEditorLubDataAdded(id, name): likewise for the first lubricant row
void diagramLubDataAdded(Project& p, int opId, const String& name);
// @src 0x140087b10  every item of the active sub-project's model: name = its operation's name,
// object row s = the slot's object name ("Read From DB" for id -2, "Undefined" for type 7 / none)
void refreshDiagramItems(Project& p);
// @src 0x1400a4220  projectDir = cleanPath(QFileInfo(path).absolutePath())
String projectDir(const Project& p);
// @src 0x14008a500  completeBaseName(path) (*!PRJNAM)
String projectBaseName(const Project& p);
// @src 0x1400a4ee0  cleanPath(primaryDb)
String primaryDbPath(const Project& p);
// @src 0x1400a7670  cleanPath(0x1400a4ee0) (the walk's dbPath)
String primaryDbPathClean(const Project& p);
// @src 0x140047dc0  operation with an id (OprManager row), null if none
Operation* operationById(const Project& p, int id);
// ---- added by P03 (slice from P13) ----
// @src 0x1400b94f0  operations of the sub-project's flow chain (groups not collapsed), placed at
// their flowNumber (0x140050b20, active sub-project) - 1: a later one with the same number
// replaces the earlier; a number past the end appends. Report, DoeOutput and ShapeOptim are left
// out. Empty when the sub-project has no project.
std::vector<Operation*> flowOperations(const SubProject& sub);
// @src 0x140084ad0  the connector chain of the flow chart: the non-group items from the head
std::vector<FlowItem*> flowChain(const FlowChart& flow);

// =================================================================================================
// P15: engine model of an operation (s1-01 Q4; used by P14, P15, P16)
// =================================================================================================
// @src 0x14005f770 / 0x14005f820  whether the operation has its DEF_PRE engine model: true whenever
// the op has a class and a project (the model is created on demand; verified by P14/P15)
bool hasEngineModel(const Operation& op);

// What the writers read from that model. The model made on demand (vt+0x1b0 = 0x1405f6410) holds a
// default-constructed DB_STEP_DATA(dim) (0x140821bf0, twin of DEF_PRE 0x1401c8950; dim = class
// vt+0x190) and reads no file, so these are the ctor values (s1-01 Q4: a model the GUI loaded
// from KEY files before Save, e.g. for the Active operation, is not modelled).
struct EngineFacts {
    bool exists = false;           // hasEngineModel
    int dim = 0;                   // engine+0x1c (= opBase vt+0x190)
    bool is2D = false;             // 0x140c7c0e0: engine+0x1c == 2 (0x140065560)
    bool is3D = false;             // 0x140c7c0a0: engine+0x1c == 3 (0x14035b400)
    int objectCount = 0;           // engine+0x1380 object list: empty, so 0x140830270(n) is null
    int ecctmpWidth = 0;           // engine+0x1708 (array size table u15f8[68]): 1 after the ctor
    int ectmfnWidth = 0;           // engine+0x170c (u15f8[69]): 1 after the ctor
};
// @src 0x14005f770 + 0x140c7c0e0 / 0x140c7c0a0 / 0x140830270 / engine+0x1708 / +0x170c (P15)
EngineFacts engineFacts(const Operation& op);

// =================================================================================================
// P17: cross-class helpers (units, DB paths)
// =================================================================================================
// @src 0x1402ede00  unit-string evaluator (value with unit text -> number in the project unit system)
double evalUnitString(const String& text, int category, int unit, bool* ok);
// @src 0x1400b9e80  DB path of a sub-project; full = with the folder (Diestress/DiestressBM pass true)
String subProjectDbPath(const Project& p, const SubProject& sub, bool full = true);
// @src 0x1400a4f60  DB path helper of the project
String projectDbPath(const Project& p);
// @src 0x1400a5970 -> vt+0x208  equipment KEY path of an equipment id (cleaned), null if none
String equipmentKeyPath(const Project& prj, int id);

// =================================================================================================
// added by P07 (slice from P02: operation info, item and project accessors; src/model/opinfo.cpp)
// The SfMoOprItem of the original is folded into Operation (item+0x18 id, +0x40 P, +0x48 OPI,
// +0x50 diagName); the OprManager's rows are Project::operations (column 1 of each row).
// =================================================================================================
// @src 0x140045fe0  SfMoOprManager ctor: no rows, current id -1 (0x140047af0), two columns
void initOprManager(Project& p);
// @src 0x1400468e0  OprManager vt+0x170(id): new item for a new operation (src = null)
std::unique_ptr<Operation> newOprItem(Project& p, int id);
// @src 0x140046960  OprManager vt+0x170(id, src): new item, its OPI copied from src when given
std::unique_ptr<Operation> newOprItem(Project& p, int id, const Operation* src);
// @src 0x140045c30  SfMoOprItem ctor (item id, P, diagName null) -> 0x140045d10
void initOprItem(Operation& item, Project& p, int id, const Operation* src);
// @src 0x140045d10  SfMoOprItem init: OPI from src (0x14004fe20) or new (0x140045f10)
void initOprItemData(Operation& item, Project& p, int id, const Operation* src);
// @src 0x140045f10  new SfOperationInfo(P, id) (0x14004fcd0, P01's initOperation)
void createOprInfo(Operation& item, Project& p, int id);
// @src 0x14004fe20  SfOperationInfo(P, id, src) ctor: defaults, copy of src, P, id
void initOperationFrom(Operation& op, Project& p, int id, const Operation& src);
// @src 0x1400500b0  copy of the data fields of src (not the task folder, flags or recovery lists)
void copyOperation(Operation& op, const Operation& src);
// Paste-path list copies of 0x1400500b0, NotPorted (src/stubs/p07.cpp): OBJ list (new OBJ copy
// per entry, selected fields + state-variable copy) and OBJPOS list
void copyObjList(Operation& op, const Operation& src);
void copyObjposList(Operation& op, const Operation& src);
// @src 0x140045e80  item text (vt+0x48): the OPI name
String oprItemText(const Operation& item);
// @src 0x140047910  dynamic_cast<SfMoOprItem*>(item)
Operation* asOprItem(Operation* item);
// @src 0x14004bd60  OprManager item(row, 1) as SfMoOprItem, null when the row does not exist
Operation* oprItemAt(const Project& p, int row);
// @src 0x14006d040  OprManager vt+0x1e0: some row's item text equals name
bool oprNameExists(const Project& p, const String& name);
// @src 0x140050930  OPI+0x10c = flag
void setOprFlag(Operation& op, int flag);
// @src 0x1400509d0  OPI+0x10c == 0
bool oprFlagOff(const Operation& op);
// @src 0x140050a50  id (0x140050910) < 1
bool hasNoId(const Operation& op);
// @src 0x140050a90  OPI+0x118 = name
void setOprName(Operation& op, const String& name);
// @src 0x140050d50  OPI+0x128 = subType when different
void setSubType(Operation& op, const String& subType);
// @src 0x140051110  OPI+0x130 (task folder relative to the project)
String taskDirRel(const Operation& op);
// @src 0x140051160  OPI+0x130 = dir
void setTaskDirRel(Operation& op, const String& dir);
// @src 0x140051190  mkpath(taskDirAbs) and the project folders: not done (s1-01 11)
void makeTaskDir(Operation& op);
// @src 0x1400511f0  OPI+0x10 = props, props->owner = op (0x1405f8d60)
void setOprProps(Operation& op, std::shared_ptr<Props> props);
// @src 0x140051250  props null, lubricant/equipment -1, TLOC 0, +0xf0/+0xf8 0
void resetOprInfo(Operation& op);
// @src 0x1400550d0  object manager of the operation's project, null without a project
ItemManager* oprObjManager(Operation& op);
// @src 0x140055150  flow chart (model) of the active sub-project of the op's project, or null
FlowChart* oprActiveFlow(const Operation& op);
// @src 0x1400688f0  SfMoObjItem ctor -> 0x1400689a0
void initObjItem(Item& it, int id);
// @src 0x1400689a0  SfMoObjItem init: setData("SfMoObjItem"), object data (0x140068ad0)
void initObjItemData(Item& it);
// @src 0x1400a7760  id of sub-project i, -1 when there is none
int subProjectId(const Project& p, int i);
// @src 0x1400b5a70  flow chart (model) of a sub-project
FlowChart* subFlow(SubProject& sub);
// @src 0x1400b5f40  1-based chain number of opId in the sub-project's flow, 0 without a model
int subFlowNumber(SubProject& sub, int opId);
// @src 0x1405f8d60  PROPS+0x20 = QPointer(op)
void setPropsOwner(Props& props, Operation* op);
// @src 0x14081afb0  QDir().mkpath(dir) with a message box on failure: not done, returns true
bool makePathWithMessage(const String& dir, bool message);

// List helpers (coordinator: P07). QList d-pointers become std::vector; Qt's implicit sharing is
// not observable in the MST.
// @src 0x1400653e0  QList<T> default ctor (d = shared_null): an empty list
template <class T>
void initList(std::vector<T>& l)
{
    l.clear();
}
// @src 0x140066860  owning pointer list ctor (OPI+0x50 OBJ*, +0x60 OBJPOS*): empty QList, +0x8
// autoDelete = 1 (the vector of unique_ptr owns its entries)
template <class T>
void initOwningList(std::vector<std::unique_ptr<T>>& l)
{
    l.clear();
}
// @src 0x1400667b0  list (count via 0x140066790 -> d+0x10) is non-empty and its entry 0
// (0x1408f1320: at(0) cast to the element class) is a non-empty list. The original reads the
// count of entry 0 without a null check; a null entry 0 is an error here.
// L: size() and at(i) returning a pointer to a list with size().
template <class L>
bool firstEntryNonEmpty(const L& l)
{
    if (static_cast<int>(l.size()) < 1) return false;
    const auto* e = l.at(0);
    if (!e) throw Error(M2M_E_INPUT, "0x1400667b0: entry 0 is not a list (the original dereferences null)");
    return static_cast<int>(e->size()) >= 1;
}

// P01's halves of 0x14004fcd0 that 0x14004fe20 and 0x1400500b0 call on their own
// @src 0x140050550  OPI defaults (s1-01 5.3)
void operationDefaults(Operation& op);
// @src 0x140050820  OPI+0x108 = id; id >= 1: OPI+0x130 = "Operations/Task%05d" (no mkpath)
void setOperationId(Operation& op, int id);

}  // namespace m2m
