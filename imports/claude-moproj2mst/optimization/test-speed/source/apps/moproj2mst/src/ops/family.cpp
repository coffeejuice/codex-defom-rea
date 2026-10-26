// Class table of the base-writer family (packet P10). Spec: s1-03 2.1 (vtables from s1/vtables.tsv).
// Every class here uses the OpClass base slots: writer 0x1405f5340 (+0x280), 0x1405f56c0 (+0x288),
// 0x1405f5860 (+0x290), 0x1405f5bf0 (+0x190), 0x1405f5880 (+0x298, P14). They differ only in +0x200:
//
//   +0x200 = 0x1413e5ef0 (true):  SfOprHeatTransfer(2D), SfOprHeatTransferCommon,
//                                 SfOprHeatTransferExpress(2D), SfOprExpressBase,
//                                 SfOprFormingExpressCommon, SfOprForming(2D)
//   +0x200 = 0x1405f79b0 (false): SfOprBase, SfOprHTFurnace, SfOprPre, SfOprPre2D, SfOprDiestressBM
//
// Only the registry keys of s1-01 6.1 get a factory; HeatTransferCommon, ExpressBase,
// FormingExpressCommon and DiestressBM are base classes of other keys (FormingExpress and
// DiestressBM2D/3D override +0x280 in P15/P16).
#include "m2m/engine.hpp"
#include "m2m/mst/loader_l2.hpp"
#include "m2m/mst/loader_l6.hpp"
#include "m2m/mst/loader_l3.hpp"
#include "m2m/mst/loader_l4.hpp"
#include "m2m/mst/writer.hpp"

namespace m2m::mst {
namespace {

// Classes whose +0x200 is 0x1413e5ef0.
class DimNstartClass : public OpClass {
public:
    using OpClass::OpClass;
    // @src 0x1413e5ef0  vt+0x200 (and +0x1f8): true
    bool writesDimNstart() const override { return true; }
};

// Classes that keep 0x1405f79b0 at +0x200.
class PlainClass : public OpClass {
public:
    using OpClass::OpClass;
};

// ---- MDT L2: own loader slots (bodies in src/engine/loader_l2.cpp, loader_l2.hpp) ---------------
template <class F> F l2(F f)
{
    if (!f) loaderSlotsMissing();
    return f;
}

// SfOprHeatTransferCommon (no registry key; base of SfOprHeatTransfer and SfOprHeatTransfer2D).
// vt+0x58, +0x1b8 and +0x1f8 stay the base slots (0x1405ef430, 0x1405f6520, 0x1413e5ef0).
class HeatTransferCommonClass : public DimNstartClass {
public:
    using DimNstartClass::DimNstartClass;
    // vt+0xb8  @src 0x141354ca0
    void loadPrologue(Project& p, Operation& op) override { l2(g_loaderSlotsL2.htLoadPrologue)(*this, p, op); }
    // vt+0x78  @src 0x141355340: empty
    void loadClassData(Project&, Operation&) override {}
    // vt+0xc0  @src 0x141354ce0
    void postLoad(Project& p, Operation& op, Ctx& ctx, engine::FileEffects& fx) override
    {
        l2(g_loaderSlotsL2.htPostLoad)(*this, p, op, ctx, fx);
    }
    // SfOprHeatTransferProperties vt+0x58  @src 0x141357520
    void propsLoad(Project& p, Operation& op) override { l2(g_loaderSlotsL2.htPropsLoad)(*this, p, op); }
};

// SfOprHeatTransfer
class HeatTransfer3DClass : public HeatTransferCommonClass {
public:
    HeatTransfer3DClass() : HeatTransferCommonClass(qt::L("SfOprHeatTransfer")) {}
    // vt+0xb8  @src 0x1413543a0  SfOprHeatTransferCommon::loadPrologue
    void loadPrologue(Project& p, Operation& op) override { HeatTransferCommonClass::loadPrologue(p, op); }
    // vt+0x78  @src 0x1413543e0: empty
    void loadClassData(Project&, Operation&) override {}
    // vt+0xc0  @src 0x1413543c0  SfOprHeatTransferCommon::postLoad
    void postLoad(Project& p, Operation& op, Ctx& ctx, engine::FileEffects& fx) override
    {
        HeatTransferCommonClass::postLoad(p, op, ctx, fx);
    }
};

// SfOprHeatTransfer2D
class HeatTransfer2DClass : public HeatTransferCommonClass {
public:
    HeatTransfer2DClass() : HeatTransferCommonClass(qt::L("SfOprHeatTransfer2D")) {}
    // vt+0xb8  @src 0x1413535b0  SfOprHeatTransferCommon::loadPrologue
    void loadPrologue(Project& p, Operation& op) override { HeatTransferCommonClass::loadPrologue(p, op); }
    // vt+0x78  @src 0x1413535f0: empty
    void loadClassData(Project&, Operation&) override {}
    // vt+0xc0  @src 0x1413535d0  SfOprHeatTransferCommon::postLoad
    void postLoad(Project& p, Operation& op, Ctx& ctx, engine::FileEffects& fx) override
    {
        HeatTransferCommonClass::postLoad(p, op, ctx, fx);
    }
};

// SfOprHTFurnace: vt+0x58, +0x1b8, +0x1f8 (false) stay the base slots
class HTFurnaceClass : public PlainClass {
public:
    HTFurnaceClass() : PlainClass(qt::L("SfOprHTFurnace")) {}
    // vt+0xb8  @src 0x140c657b0
    void loadPrologue(Project& p, Operation& op) override { l2(g_loaderSlotsL2.furnaceLoadPrologue)(*this, p, op); }
    // vt+0x78  @src 0x140c659d0: empty
    void loadClassData(Project&, Operation&) override {}
    // vt+0xc0  @src 0x140c65830
    void postLoad(Project& p, Operation& op, Ctx& ctx, engine::FileEffects& fx) override
    {
        l2(g_loaderSlotsL2.furnacePostLoad)(*this, p, op, ctx, fx);
    }
    // SfOprHTFurnaceProperties vt+0x58  @src 0x140c69480
    void propsLoad(Project& p, Operation& op) override { l2(g_loaderSlotsL2.furnacePropsLoad)(*this, p, op); }
    // vt+0xa8  @src 0x140c659f0: resource registration and vt+0x228, then the tree and panel (UI)
    bool objectTreeFill(Project& p, Operation& op, engine::FileEffects& fx) override
    {
        objectTreeAdd(p, op, fx);
        // the tree's first item, "Load Pattern" (SfPreLoadPattern), is the editor the GUI shows:
        // its ctor 0x14106e8b0 fills the form from the model (src/engine/htf_loadpattern.cpp)
        l6Slot(g_l6Slots.loadPatternInit)(p, op, loadPattern_);
        loadPatternOp_ = &op;
        return false;
    }
    // vt+0x228  @src 0x140c66230 (model part: 0x140c668c0)
    void objectTreeAdd(Project& p, Operation& op, engine::FileEffects&) override
    {
        l2(g_loaderSlotsL2.furnaceObjectTreeAdd)(*this, p, op);
    }
    // vt+0xd0 with the Load Pattern editor shown: signalUpdateDataInWidgetArea -> its vt+0x1c0
    // 0x141070870 (the form into the model only when it was modified: no conversion modifies it);
    // the caller then runs 0x140054350 (props vt+0x58)
    bool updateDataInWidgetArea(Project& p, Operation& op, engine::FileEffects&) override
    {
        if (loadPatternOp_ != &op) return false;
        l6Slot(g_l6Slots.loadPatternUpdate)(p, op, loadPattern_);
        return true;
    }
    engine::LoadPatternForm* loadPatternEditor(const Operation& op) override
    {
        return loadPatternOp_ == &op ? &loadPattern_ : nullptr;
    }

private:
    engine::LoadPatternForm loadPattern_;
    const Operation* loadPatternOp_ = nullptr;   // the operation whose editor is shown
};

// SfOprPre (key SfOprPre3D) / SfOprPre2D: their own vt+0x58 is empty, so opening the operation
// loads nothing; vt+0xb8 and vt+0xc0 stay the base slots (0x1405f1bb0, 0x1405f1c00), unused.
class PreClass : public PlainClass {
public:
    explicit PreClass(bool is2D) : PlainClass(qt::L(is2D ? "SfOprPre2D" : "SfOprPre3D")) {}
    // vt+0x58  @src 0x141348e80 (Pre) / 0x141347980 (Pre2D): empty
    void loadOperation(Project&, Operation&, Ctx&, engine::FileEffects&) override {}
    // vt+0x78  @src 0x141348ea0 (Pre) / 0x1413479a0 (Pre2D): empty
    void loadClassData(Project&, Operation&) override {}
    // SfOprPreProperties vt+0x58  @src 0x14134c380 (not reached from the empty load above)
    void propsLoad(Project& p, Operation& op) override { l2(g_loaderSlotsL2.prePropsLoad)(*this, p, op); }
};

}  // namespace

namespace {

// ---- MDT L3: own loader slots (bodies in src/engine/loader_l3.cpp, loader_l3.hpp) ---------------
// vt+0x58, +0x1b8 and +0x1f8 stay the base slots (0x1405ef430, 0x1405f6520, 0x1413e5ef0).

// SfOprForming / SfOprForming2D
class FormingClass : public DimNstartClass {
public:
    explicit FormingClass(bool is2D) : DimNstartClass(qt::L(is2D ? "SfOprForming2D" : "SfOprForming")) {}
    // vt+0xb8  @src 0x140c060f0 (Forming) / 0x140c04910 (Forming2D)
    void loadPrologue(Project& p, Operation& op) override { l3Slot(g_loaderSlotsL3.formingLoadPrologue)(*this, p, op); }
    // vt+0x78  @src 0x140c06760 (Forming) / 0x140c04f80 (Forming2D): empty
    void loadClassData(Project&, Operation&) override {}
    // vt+0xc0  @src 0x140c06130 (Forming) / 0x140c04950 (Forming2D)
    void postLoad(Project& p, Operation& op, Ctx& ctx, engine::FileEffects& fx) override
    {
        l3Slot(g_loaderSlotsL3.formingPostLoad)(*this, p, op, ctx, fx);
    }
    // SfOprFormingProperties vt+0x58  @src 0x140c09da0
    void propsLoad(Project& p, Operation& op) override { l3Slot(g_loaderSlotsL3.formingPropsLoad)(*this, p, op); }
    // vt+0xa8  @src 0x140c06780 (Forming) / 0x140c05000 (Forming2D): vt+0x228, then the property
    // panel model (UI). Packet S; L3 may take these bodies over.
    bool objectTreeFill(Project& p, Operation& op, engine::FileEffects& fx) override
    {
        objectTreeAdd(p, op, fx);
        // Factory 0x1405ff0b0: mode!=0 constructs SfPreSimControl300; mode0 constructs
        // SfPreSimControl100. Retain the inherited mode0 approximation for now.
        simControl300Shown_ = p.originalSettings.moInputMode != 0;
        if (simControl300Shown_) l3Slot(g_loaderSlotsL3.formingSimControl300Open)(p, op);
        simControlShown_ = &op;
        return false;
    }
    // vt+0xd0 with that editor shown (index != 0): its vt+0x1c0 0x1417ec870; the caller then runs
    // 0x140054350
    bool updateDataInWidgetArea(Project& p, Operation& op, engine::FileEffects& fx) override
    {
        if (simControlShown_ != &op) return false;
        // 300 vt+0x1c0 applies all pages, unlike the inherited small-editor helper.
        // Do not silently run that different editor for newly exposed settings state.
        if (simControl300Shown_) not_ported(0x140eb5180, "SfPreSimControl300 Save apply");
        return l3Slot(g_loaderSlotsL3.formingSimControlApply)(*this, p, op, fx);
    }
    // vt+0x228  @src 0x140c07130 (Forming) / 0x140c04fa0 (Forming2D): tree states 0x1405eea60,
    // 0x1405eeab0, 0x1405eebb0 (UI), then vt+0x230 0x140c068c0 / 0x140c05200 (packet S)
    void objectTreeAdd(Project& p, Operation& op, engine::FileEffects& fx) override
    {
        if (!g_equipmentSync) loaderSlotsMissing();
        g_equipmentSync(p, op, false, fx);
    }

private:
    bool simControl300Shown_ = false;
    const Operation* simControlShown_ = nullptr;
};

// SfOprHeatTransferExpress / SfOprHeatTransferExpress2D (SfOprExpressBase subclasses)
class HeatTransferExpressClass : public DimNstartClass {
public:
    explicit HeatTransferExpressClass(bool is2D)
        : DimNstartClass(qt::L(is2D ? "SfOprHeatTransferExpress2D" : "SfOprHeatTransferExpress")) {}
    // vt+0xb8  @src 0x1413e6360 (3D) / 0x1413e5aa0 (2D)
    void loadPrologue(Project& p, Operation& op) override { l3Slot(g_loaderSlotsL3.hteLoadPrologue)(*this, p, op); }
    // vt+0x78  @src 0x1413e64c0 (3D) / 0x1413e5c00 (2D): empty
    void loadClassData(Project&, Operation&) override {}
    // vt+0xc0  @src 0x1413e63a0 (3D) / 0x1413e5ae0 (2D)
    void postLoad(Project& p, Operation& op, Ctx& ctx, engine::FileEffects& fx) override
    {
        l3Slot(g_loaderSlotsL3.htePostLoad)(*this, p, op, ctx, fx);
    }
    // SfOprHeatTransferExpressProperties vt+0x58  @src 0x1413ed890
    void propsLoad(Project& p, Operation& op) override { l3Slot(g_loaderSlotsL3.htePropsLoad)(*this, p, op); }
    // vt+0xa8  @src 0x1413e64e0 (3D) / 0x1413e5c20 (2D) -> 0x1413f93c0: vt+0x228, then the object
    // tree's item model (UI)
    bool objectTreeFill(Project& p, Operation& op, engine::FileEffects& fx) override
    {
        objectTreeAdd(p, op, fx);
        return false;
    }
    // vt+0x228  @src 0x1413e6520 (3D) / 0x1413e5c60 (2D)
    void objectTreeAdd(Project& p, Operation& op, engine::FileEffects&) override
    {
        l3Slot(g_loaderSlotsL3.hteObjectTreeAdd)(*this, p, op);
    }
    // vt+0xc8  @src 0x1413fbd10 (MDT L4, DB-5; body in src/ops/express.cpp through g_loaderSlotsL4)
    void syncFromModelTail(Project& p, Operation&, Ctx& ctx, engine::FileEffects&) override
    {
        l4Slot(g_loaderSlotsL4.expressSyncTail)(*this, p, ctx);
    }
};

}  // namespace

std::unique_ptr<OpClass> makeForming(bool is2D)
{
    return std::make_unique<FormingClass>(is2D);
}

std::unique_ptr<OpClass> makeHeatTransfer(bool is2D)
{
    if (is2D) return std::make_unique<HeatTransfer2DClass>();
    return std::make_unique<HeatTransfer3DClass>();
}

std::unique_ptr<OpClass> makeHeatTransferExpress(bool is2D)
{
    return std::make_unique<HeatTransferExpressClass>(is2D);
}

std::unique_ptr<OpClass> makeHTFurnace()
{
    return std::make_unique<HTFurnaceClass>();
}

std::unique_ptr<OpClass> makePre(bool is2D)
{
    return std::make_unique<PreClass>(is2D);
}

}  // namespace m2m::mst
