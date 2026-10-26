// m2m/mst/writer.hpp: operation classes (the SfOpr* singletons, "OB") and their MST writers (seam).
//
// One OpClass instance per class key and conversion (Project::registry). Virtual slots keep the
// original vtable offsets in their comments. Ownership:
//   OpClass base slot bodies ......... P10 (src/ops/base.cpp): 0x280 base writer, 0x288, 0x200,
//                                      0x290, 0x190
//   vt+0x298 sim lists ............... P14 (src/model/simcount.cpp, include/m2m/mst/simcount.hpp):
//                                      base 0x1405f5880 and every class's body; owners override
//                                      simList() with a one-line call into P14's function
//   makeProps / displayName base ..... P02 (factory side, s1-01 5.2, 6.3)
//   make<Class>() factories .......... the packet of the writer (table below); a factory returns an
//                                      OpClass subclass that overrides the slots of its class
// Until a packet merges, its factories return a stub class whose writeMst throws NotPorted
// (src/stubs/<packet>.cpp), so a project that uses it converts to M2M_E_UNSUPPORTED.
#pragma once

#include <memory>
#include <vector>

#include "m2m/common.hpp"
#include "m2m/model.hpp"
#include "m2m/props.hpp"
#include "m2m/mst/state.hpp"
#include "m2m/mst/stream.hpp"

namespace m2m::engine {
struct FileEffects;
struct GuiModel;
struct LoadPatternForm;
}

namespace def::model {
class LLOBJ_DATA;
}

namespace m2m::doe { struct OutputClassRefs; }

namespace m2m::mst {
class OpClass;

// MDT L0: SfOprBase bodies of the loader slots, set by src/engine/loader.cpp before it calls any
// slot (standalone test builds that do not link the engine never reach them).
struct LoaderSlots {
    void (*loadOperation)(OpClass&, Project&, Operation&, Ctx&, engine::FileEffects&) = nullptr;
    void (*loadPrologue)(OpClass&, Project&, Operation&) = nullptr;
    void (*loadClassData)(OpClass&, Project&, Operation&) = nullptr;
    void (*postLoad)(OpClass&, Project&, Operation&, Ctx&, engine::FileEffects&) = nullptr;
    void (*propsLoad)(OpClass&, Project&, Operation&) = nullptr;
    bool (*restartCapable)(const OpClass&) = nullptr;
    // added by MDT L6: vt+0x1b8 (Diestress, DiestressBM2D/3D and ShapeOptim override it)
    void (*loadData)(OpClass&, Project&, Operation&, Ctx&, engine::FileEffects&) = nullptr;
    // added by MDT L7: the vt+0x1b0 base body and the engine calls of the class slots that live
    // with their classes (src/ops), so those files need no link to the engine
    std::shared_ptr<engine::GuiModel> (*createModel)(OpClass&, Project&, Operation&) = nullptr;   // 0x1405f6410
    engine::GuiModel* (*opModel)(Project&, Operation&) = nullptr;     // 0x14005f770 / 0x14005f820
    // 0x140c7a5b0(mem, P+8, dim) then 0x140822210(E, 0x1400a2180(P), 1): a new wrapper, unit set
    std::shared_ptr<engine::GuiModel> (*newModel)(Project&, int dim) = nullptr;
    void (*resetModel)(engine::GuiModel&) = nullptr;                  // 0x140c7b720
    void (*setDimension)(engine::GuiModel&, int dim) = nullptr;       // 0x140c7acd0(W)->vt[0](&dim)
    void (*showModel)(Project&, engine::GuiModel*) = nullptr;         // 0x140cf9360
    void (*readDoeDocument)(Project&, Operation&) = nullptr;         // 0x140e2ec40 document phase
    void (*finishDoeOutputClassData)(Project&, Operation&, const doe::OutputClassRefs&) = nullptr;
    Operation* (*prevOp)(Project&, const Operation&) = nullptr;      // 0x140062960
    // added by MDT L4S: save-current-operation slot vt+0xd8 (base 0x1405f22e0, src/engine/save_current.cpp)
    void (*saveNames)(OpClass&, Project&, Operation&, engine::FileEffects&) = nullptr;
};
inline LoaderSlots g_loaderSlots{};
inline void loaderSlotsMissing() { throw Error(M2M_E_UNSUPPORTED, "operation loader not linked"); }
// Packet S: OB vt+0x230, the equipment sync (engine::equipmentSync, src/engine/postload_views.cpp),
// set when that file is linked; the classes' vt+0x228 bodies call it through this pointer so the
// standalone tests that build src/ops without the engine still link.
inline void (*g_equipmentSync)(Project&, Operation&, bool multiBlow, engine::FileEffects&) = nullptr;

// What every writer and emitter gets in place of the original's QTextStream: the stream plus the
// per-call globals and context.
struct W {
    Stream& ts;
    State& st;
    Ctx& ctx;
    engine::FileEffects* sideEffects = nullptr; // files generated during MST emission (Save step 5)
};

struct SimEntry {                  // record of 0x1400b8360 (0x1400683f0): +0x8 position, +0xc sim
    int pos = 0;
    int sim = 0;
    // added by packet S (0x140054540 reads it): +0x10 entry name, set by 0x1400682a0; `named` is
    // false where the class's vt+0x298 body that builds the name is not ported
    String name;
    bool named = false;
};

class OpClass {
public:
    explicit OpClass(String key) : key_(std::move(key)) {}
    virtual ~OpClass() = default;
    OpClass(const OpClass&) = delete;
    OpClass& operator=(const OpClass&) = delete;

    const String& key() const { return key_; }   // +0x28 class name (registry key)

    Operation* current = nullptr;  // +0x20 QPointer "current item" (0x1405ef2b0 / 0x1405ef280)
    int dimFlags = 0;              // *(op+0x38)+0x80 dimension flags (s1-02 7, O6)

    // vt+0x280  @src 0x1405f5340 (base)  writeMst(ts, prj, db, sim, item, dbreadArg, onlySim)
    virtual void writeMst(W& w, Project& prj, const String& db, int* sim, Operation* op,
                          int dbreadArg, int onlySim);
    // vt+0x288  @src 0x1405f56c0  setup lines (MDSOBJ ... TLOC)
    virtual void writeSetup(W& w, Project& prj, const String& db, int n, Operation* op);
    // vt+0x200  @src 0x1413e5ef0 (true) / 0x1405f79b0 (false): writes DIM/NSTART
    virtual bool writesDimNstart() const;
    // vt+0x290  @src 0x1405f5860  empty for every class
    virtual void writeExtra(W& w, Project& prj, const String& db, int n, Operation* op);
    // vt+0x190  @src 0x1405f5bf0  dimension from dimFlags: (f&3)==3 -> 4, f&1 -> 2, f&2 -> 3, else 0
    virtual int dim() const;
    // vt+0x298  @src 0x1405f5880  simulation entries of one operation at flow position pos (P14)
    virtual void simList(int pos, int* counter, Operation* op, std::vector<SimEntry>& out);
    // vt+0x98   properties object with the class's default tree (P02 base; classes override).
    // guiUnit = the GUI start-up unit (mainWindow vt+0x260, global 0x143d79724, s1-01 Q2) that
    // unit-dependent data ctors read (FE data 0x14142fac0, MultiBlow data); P02 supplies it.
    virtual std::shared_ptr<Props> makeProps(int guiUnit) const;
    // vt+0x88   display name used for unnamed operations (P02 base; classes override)
    virtual String displayName() const;

    // @src 0x1405ef320  props of op (or of current when op is null)
    Props* propsOf(Operation* op) const;

    // ---- added by MDT L0: the operation loader (docs/re/moproj2mst/mdt/L0, src/engine) ---------
    // Class packets L1..L7 override the slots of their classes; until then a base body that the
    // class does not use refuses with M2M_E_UNSUPPORTED.
    // vt+0x58  @src 0x1405ef430 (base; own bodies: Convert2Dto3D, CopyMirror, HeatTreat*, Group,
    // Cycle, Pre*, DoeOutput, ShapeOptim, Report)
    virtual void loadOperation(Project& p, Operation& op, Ctx& ctx, engine::FileEffects& fx)
    {
        if (!g_loaderSlots.loadOperation) loaderSlotsMissing();
        g_loaderSlots.loadOperation(*this, p, op, ctx, fx);
    }
    // vt+0xb8  @src 0x1405f1bb0  load prologue (sim control TNOW/TRANS)
    virtual void loadPrologue(Project& p, Operation& op)
    {
        if (!g_loaderSlots.loadPrologue) loaderSlotsMissing();
        g_loaderSlots.loadPrologue(*this, p, op);
    }
    // vt+0x78  pure in SfOprBase (0x141470184); every class has its own
    virtual void loadClassData(Project& p, Operation& op)
    {
        if (!g_loaderSlots.loadClassData) loaderSlotsMissing();
        g_loaderSlots.loadClassData(*this, p, op);
    }
    // vt+0xc0  @src 0x1405f1c00  post-load
    virtual void postLoad(Project& p, Operation& op, Ctx& ctx, engine::FileEffects& fx)
    {
        if (!g_loaderSlots.postLoad) loaderSlotsMissing();
        g_loaderSlots.postLoad(*this, p, op, ctx, fx);
    }
    // props vt+0x58 through 0x140054350; @src 0x1405fb600 (base: nothing)
    virtual void propsLoad(Project& p, Operation& op)
    {
        if (!g_loaderSlots.propsLoad) loaderSlotsMissing();
        g_loaderSlots.propsLoad(*this, p, op);
    }
    // vt+0x1f8  class supports restart: @src 0x1413e5ef0 (true) / 0x1405f79a0 (false)
    virtual bool restartCapable() const
    {
        if (!g_loaderSlots.restartCapable) loaderSlotsMissing();
        return g_loaderSlots.restartCapable(*this);
    }
    // ---- added by MDT L6 ----------------------------------------------------------------------
    // vt+0x1b8  @src 0x1405f6520  KEY / DB load by GUI mode (own bodies: Diestress, DiestressBM2D/3D,
    // ShapeOptim)
    virtual void loadData(Project& p, Operation& op, Ctx& ctx, engine::FileEffects& fx)
    {
        if (!g_loaderSlots.loadData) loaderSlotsMissing();
        g_loaderSlots.loadData(*this, p, op, ctx, fx);
    }
    // vt+0xd8 of save current operation (0x140052ec0): base 0x1405f22e0, names into the engine
    // (added by MDT L4S)
    virtual void saveNames(Project& p, Operation& op, engine::FileEffects& fx)
    {
        if (!g_loaderSlots.saveNames) loaderSlotsMissing();
        g_loaderSlots.saveNames(*this, p, op, fx);
    }

    // ---- added by MDT L5S: save-current-operation slots (packet S, src/engine/save_current.cpp) ----
    // Each returns false when the class keeps the SfOprBase body (or the slot list in S decides).
    // vt+0xe8  task files; base @src 0x1405f2350 (0x140057370, run by S). Convert2Dto3D 0x1401c3be0,
    // CopyMirror 0x140e24cf0
    virtual bool saveOperationFiles(Project&, Operation&, engine::FileEffects&) { return false; }
    // vt+0x80  after vt+0xe8 (SfOprBase: _purecall). Convert2Dto3D 0x1401c29c0 (empty), CopyMirror
    // 0x140e24b30
    virtual bool saveAfterFiles(Project&, Operation&, engine::FileEffects&) { return false; }
    // props vt+0x60 through 0x1400543a0: properties -> engine model. Convert2Dto3D 0x1401ce590,
    // CopyMirror 0x140e2d230
    virtual bool propsToEngine(Project&, Operation&) { return false; }
    // vt+0xd0 0x1405f1e20 when the property stack shows a class editor (widget index != 0):
    // signalUpdateDataInWidgetArea to that editor. false = index 0 (S then runs 0x1400543a0).
    // Convert2Dto3D: its Configuration editor SfConv23Config vt+0x1d0 0x1401e2d70 (MDT L5S)
    virtual bool updateDataInWidgetArea(Project&, Operation&, engine::FileEffects&) { return false; }
    // the HTFurnace Load Pattern editor shown for op (MDT L6T), null for other classes; tests edit it
    virtual engine::LoadPatternForm* loadPatternEditor(const Operation&) { return nullptr; }
    // ---- added by MDT L7 -------------------------------------------------------------------------
    // vt+0xe0  model(opi): a class's own engine model for opi, asked first by 0x14005f770.
    // @src 0x140100e20 (base: null); Cycle 0x14134daf0
    virtual engine::GuiModel* classModel(Project&, Operation&) { return nullptr; }
    // vt+0x1b0  new engine model for the current item (0x14005f770 stores it in OPI+0x38).
    // @src 0x1405f6410 (base); DoeOutput 0x140e32a30, ShapeOptim 0x14013fc20
    virtual std::shared_ptr<engine::GuiModel> createModel(Project& p, Operation& op)
    {
        if (!g_loaderSlots.createModel) loaderSlotsMissing();
        return g_loaderSlots.createModel(*this, p, op);
    }
    // vt+0x2a0  object `no` of the current operation provided to a later operation that inherits
    // it (0x14005b2f0 walks the predecessors): @src 0x1405f5b70 (base: false); CopyMirror
    // 0x140e25980 (MDT L5)
    virtual bool provideObject(Project&, int /*no*/, def::model::LLOBJ_DATA* /*dst*/) { return false; }
    // vt+0x228  object-tree add. The load reaches it through 0x1400547b0 -> 0x1400548c0 (ProcType 5
    // after a .moproj open) -> 0x140054950: vt+0xa8 and vt+0x278 (0x1405f15e0) each call it. Base
    // 0x1405eea60 and its vt+0x258 change only the UI tree (OB+0x18). Added by MDT L2: a class whose
    // override also changes the model overrides this (SfOprHTFurnace 0x140c66230); nearly every
    // class has its own vt+0x228 / vt+0xa8, which their L packets check. The FileEffects parameter
    // (packet S) carries the files the Forming / MultiBlow equipment sync vt+0x230 creates and deletes.
    virtual void objectTreeAdd(Project&, Operation&, engine::FileEffects&) {}
    // vt+0xa8  object-tree / property panel fill, called by 0x140054950 (packet S, postLoadViews)
    // before vt+0x278. Base @src 0x1405f1b70: empty. The classes' own bodies call vt+0x228
    // (objectTreeAdd) and build UI models; their L packets override this. Returns nothing useful.
    virtual bool objectTreeFill(Project&, Operation&, engine::FileEffects&) { return false; }
    // ---- added by MDT DB (src/engine/db_load.cpp) ------------------------------------------------
    // vt+0x1f0  @src 0x1405f68e0 (every class): class-info flags (vbase OB+0x30 vt+0x58, the word
    // 0x1405fd8a0 stores at info+0x80, i.e. dimFlags) & 0x01000000: set only for Convert2Dto3D,
    // CopyMirror and Boolean2D/3D (MDT DB spec 2.2); 0x14005f520 then accepts one stored step of
    // such a previous operation
    virtual bool oneStepPrev() const { return (dimFlags & 0x01000000) != 0; }
    // vt+0xc8  tail of 0x1400538f0 (sync from the model). Base @src 0x1405f1e10: empty. Overrides: MultiBlow 0x140602230, Express family 0x1413fbd10 (thread L4), Convert2Dto3D 0x1401c3570 (thread L5)
    virtual void syncFromModelTail(Project&, Operation&, Ctx&, engine::FileEffects&) {}

private:
    String key_;
};

// ---- factories, one per class key (30 keys, s1-01 6.1) -------------------------------------------
// P10: base-writer family (s1-02 5, s1-03 2)
std::unique_ptr<OpClass> makeForming(bool is2D);              // SfOprForming / SfOprForming2D
std::unique_ptr<OpClass> makeHeatTransfer(bool is2D);         // SfOprHeatTransfer(2D)
std::unique_ptr<OpClass> makeHeatTransferExpress(bool is2D);  // SfOprHeatTransferExpress(2D)
std::unique_ptr<OpClass> makeHTFurnace();                     // SfOprHTFurnace
std::unique_ptr<OpClass> makePre(bool is2D);                  // SfOprPre3D / SfOprPre2D
// P11: SfOprCogging (writer 0x1407520c0)
std::unique_ptr<OpClass> makeCogging();
// P12: SfOprBoolean2D/3D (0x1411df600), SfOprConvert2Dto3D (0x1401c4440), SfOprCopyMirror (0x140e24e80)
std::unique_ptr<OpClass> makeBoolean(bool is2D);
std::unique_ptr<OpClass> makeConvert2Dto3D();
std::unique_ptr<OpClass> makeCopyMirror();
// P13: SfOprCycle (0x14134dcc0), SfOprGroup (0x141352540), SfOprDoeOutput (0x140e32970),
//      SfOprDistortion2D/3D (0x141362e90)
std::unique_ptr<OpClass> makeCycle();
std::unique_ptr<OpClass> makeGroup();
std::unique_ptr<OpClass> makeDoeOutput();
std::unique_ptr<OpClass> makeDistortion(bool is2D);
// P14: SfOprMultiBlow2D / SfOprMultiBlow3D (0x140602810)
std::unique_ptr<OpClass> makeMultiBlow(bool is2D);
// P15: SfOprFormingExpress (0x1414136b0) / SfOprFormingExpress2D (0x141412c90)
std::unique_ptr<OpClass> makeFormingExpress(bool is2D);
// P16: SfOprDiestress (0x1402b5410), SfOprDiestressBM2D/3D (0x14029ad10 / 0x14029cb50),
//      SfOprHeatTreat / 2D (0x1411b1460 / 0x1411aee40), SfOprReport (0x1412b05a0),
//      SfOprShapeOptim (0x14013fb90)
std::unique_ptr<OpClass> makeDiestress();
std::unique_ptr<OpClass> makeDiestressBM(bool is2D);
std::unique_ptr<OpClass> makeHeatTreat(bool is2D);
std::unique_ptr<OpClass> makeReport();
std::unique_ptr<OpClass> makeShapeOptim();

// ---- simulation numbering (P14) --------------------------------------------------------------------
// @src 0x1400b8360  simulation entries of a sub-project (calls simList per flow item)
std::vector<SimEntry> simEntries(Project& p, SubProject& sub);
// @src 0x1400b8820  nextSim(sub, ord)
int nextSim(Project& p, SubProject& sub, int ord);

// Stub class for a packet that has not merged yet: writeMst throws NotPorted(writer address).
class StubClass : public OpClass {
public:
    StubClass(String key, std::uint64_t writer, const char* name)
        : OpClass(std::move(key)), writer_(writer), name_(name) {}
    void writeMst(W&, Project&, const String&, int*, Operation*, int, int) override
    {
        M2M_STUB(writer_, name_);
    }
    // Neutral answers so the open path works before the class's packet merges.
    int dim() const override { return 3; }
    std::shared_ptr<Props> makeProps(int) const override { return std::make_shared<Props>(); }
    String displayName() const override { return key(); }

private:
    std::uint64_t writer_;
    const char* name_;
};

}  // namespace m2m::mst
