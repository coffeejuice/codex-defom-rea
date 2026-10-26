// MDT L0: SfOprBase bodies of the operation-loader slots (spec_C section 2). A class whose own
// slot is not ported yet (packets L1..L7) refuses instead of running the base body it does not use.
#include <string>

#include <defmodel/layout/cls_1401e7b10.hpp>
#include <defmodel/layout/db_step_data.hpp>

#include "engine_internal.hpp"
#include "m2m/engine.hpp"
#include "m2m/doe_session.hpp"
#include "m2m/doe_output_data.hpp"
#include "m2m/mst/writer.hpp"

namespace m2m::engine {
void baseLoad(Project& p, Operation& op, Ctx& ctx, FileEffects& fx);
void baseLoadData(Project& p, Operation& op, Ctx& ctx, FileEffects& fx);
std::shared_ptr<GuiModel> newModel(Project& p, int dim);
Operation* previousOperation(Project& p, const Operation& op);
}

namespace m2m::mst {
void installL6Slots();   // src/engine/l6_slots.cpp
}

namespace m2m::mst {

namespace {

bool keyIn(const String& key, std::initializer_list<const char16_t*> keys)
{
    for (const char16_t* k : keys)
        if (key.u16() == k) return true;
    return false;
}

[[noreturn]] void notPorted(const String& key, const char* slot)
{
    throw Error(M2M_E_UNSUPPORTED, "operation load: " + key.utf8() + " " + slot +
                                       " is not ported yet (docs/re/moproj2mst/mdt/loader-packets.md)");
}

engine::GuiModel* model(Project& p, Operation& op)
{
    return engine::opModel(p, op);
}

}  // namespace

void baseLoadOperation(OpClass& c, Project& p, Operation& op, Ctx& ctx, engine::FileEffects& fx)
{
    const String& key = c.key();
    // classes with their own vt+0x58 (spec_C section 2)
    // (L7: Group, Cycle, DoeOutput, ShapeOptim and Report override loadOperation in src/ops)
    if (keyIn(key, {u"SfOprConvert2Dto3D", u"SfOprCopyMirror", u"SfOprHeatTreat", u"SfOprHeatTreat2D",
                      u"SfOprPre3D", u"SfOprPre2D"}))
        notPorted(key, "vt+0x58 (own load)");
    engine::baseLoad(p, op, ctx, fx);
}

void baseLoadPrologue(OpClass&, Project& p, Operation& op)
{
    engine::GuiModel* m = model(p, op);
    def::model::Cls_1401e7b10* sc = m ? m->simCtl() : nullptr;
    if (!sc) return;
    sc->tnow3 = 1;                         // 0x1405ef620(self, true): SC+0x650
    sc->tnow2 = 0.0;                       // 0x1405ef690: SC+0x648
    sc->trans[1] = 1;                      // 0x1405ef6f0(self, 1): SC+0x108 (TRANS 2)
    sc->trans[0] = 1;                      // 0x1405ef750(self, 1): SC+0x104 (TRANS 1)
}

void baseLoadClassData(OpClass& c, Project&, Operation&)
{
    notPorted(c.key(), "vt+0x78");           // pure virtual in SfOprBase (0x141470184)
}

void basePostLoad(OpClass& c, Project& p, Operation& op, Ctx& ctx, engine::FileEffects& fx)
{
    const String& key = c.key();
    // Group 0x1413524d0 and Cycle 0x14134d790 (-> 0x1413524d0) only forward to this body
    if (!keyIn(key, {u"SfOprBase", u"SfOprHeatTreat", u"SfOprHeatTreat2D", u"SfOprPre3D", u"SfOprPre2D",
                       u"SfOprShapeOptim", u"SfOprGroup", u"SfOprCycle"}))
        notPorted(key, "vt+0xc0");
    basePostLoadBody(c, p, op, ctx, fx);
}

// @src 0x1405f1c00  (also called first by the class post-loads that extend it)
void basePostLoadBody(OpClass&, Project& p, Operation& op, Ctx&, engine::FileEffects& fx)
{
    engine::GuiModel* m = model(p, op);
    if (!m) return;
    const int nObj = m->objectCount();
    SubProject* sp = activeSubProject(p);
    const int predId = sp ? flowPredecessor(sp->flow, op.id) : -1;
    Operation* prev = predId == -1 ? nullptr : operationById(p, predId);
    int n = 1;
    if (prev) {
        n = 0;                             // 0x14005fab0: slots of prev with an own or inherited object
        for (int c : prev->objIds)
            if (c > 0 || c == -2) ++n;
    }
    if (nObj == 0 && n > 0) {
        for (int i = 0; i < n; ++i) {
            engine::addObject(p, op, fx);          // 0x140057ba0 (MDT L7)
            if (prev) {                            // 0x140062960 asked again: same answer
                def::model::LLOBJ_DATA* o = m->object(i + 1);   // 0x140057710
                if (!o) throw Error(M2M_E_UNSUPPORTED, "operation load: post-load object " + std::to_string(i + 1) +
                                                           " missing (the original dereferences it)");
                *engine::at<std::int32_t>(o, 0xc) = 5;
            }
            engine::refreshSlotItems(p, op, i + 1, fx);   // 0x140060100
        }
    }
    if (op.oprFlag == 0 && prev) {
        engine::GuiModel* pm = model(p, *prev);
        def::model::Cls_1401e7b10* d = m->simCtl();
        def::model::Cls_1401e7b10* s = pm ? pm->simCtl() : nullptr;
        if (d && s) {
            for (int i = 0; i < 7; ++i) d->stnout[i] = s->stnout[i];
            for (int i = 0; i < 10; ++i) d->elmnod[i] = s->elmnod[i];
        }
    }
}

void basePropsLoad(OpClass& c, Project&, Operation&)
{
    const String& key = c.key();
    // props vt+0x58: base 0x1405fb600 (nothing) for Base, Boolean, Cycle, DiestressBM, Diestress,
    // Express properties; the others have their own (spec_C section 2.4)
    if (!keyIn(key, {u"SfOprBase", u"SfOprBoolean", u"SfOprBoolean2D", u"SfOprBoolean3D", u"SfOprCycle",
                       u"SfOprDiestressBM", u"SfOprDiestressBM2D", u"SfOprDiestressBM3D", u"SfOprDiestress",
                       u"SfOprExpressBase"}))
        notPorted(key, "properties vt+0x58");
}

bool baseRestartCapable(const OpClass& c)
{
    const String& key = c.key();
    // vt+0x1f8 = 0x1413e5ef0 (true) for these classes, else 0x1405f79a0 (false)
    return keyIn(key, {u"SfOprConvert2Dto3D", u"SfOprExpressBase", u"SfOprForming", u"SfOprForming2D",
                         u"SfOprFormingExpress", u"SfOprFormingExpress2D", u"SfOprFormingExpressCommon",
                         u"SfOprHeatTransfer", u"SfOprHeatTransfer2D", u"SfOprHeatTransferCommon",
                         u"SfOprHeatTransferExpress", u"SfOprHeatTransferExpress2D", u"SfOprHeatTreat",
                         u"SfOprHeatTreat2D"});
}

// @src 0x1405f6410  SfOprBase vt+0x1b0: model of the current item, TNOW 3 = 1
std::shared_ptr<engine::GuiModel> baseCreateModel(OpClass& c, Project& p, Operation&)
{
    Operation* opi = c.current;                    // 0x1405ef2b0
    if (!opi) return nullptr;
    if (!opi->project) return nullptr;             // OPI+0x100
    std::shared_ptr<engine::GuiModel> m = engine::newModel(p, c.dim());   // vt+0x190
    m->simCtl()->tnow3 = 1;                        // 0x140c7b2d0(m)->+0x650
    return m;
}

void installLoaderSlots()
{
    g_loaderSlots.loadOperation = &baseLoadOperation;
    g_loaderSlots.loadPrologue = &baseLoadPrologue;
    g_loaderSlots.loadClassData = &baseLoadClassData;
    g_loaderSlots.postLoad = &basePostLoad;
    g_loaderSlots.propsLoad = &basePropsLoad;
    g_loaderSlots.restartCapable = &baseRestartCapable;
    engine::installLoaderSlotsL1();        // MDT L1 cogging slots (cogging_load.cpp)
    engine::installLoaderSlotsL2();        // MDT L2 class slots (loader_l2.cpp)
    engine::installLoaderSlotsL3();        // MDT L3 class slots (loader_l3.cpp)
    g_loaderSlots.loadData = [](OpClass&, Project& p, Operation& op, Ctx& ctx, engine::FileEffects& fx) {
        engine::baseLoadData(p, op, ctx, fx);
    };
    engine::installLoaderSlotsL1();        // MDT L1 cogging slots (cogging_load.cpp)
    engine::installLoaderSlotsL2();        // MDT L2 class slots (loader_l2.cpp)
    engine::installLoaderSlotsL3();        // MDT L3 class slots (loader_l3.cpp)
    g_loaderSlots.createModel = &baseCreateModel;
    g_loaderSlots.opModel = &engine::opModel;
    g_loaderSlots.newModel = &engine::newModel;
    g_loaderSlots.resetModel = [](engine::GuiModel& m) { m.reset(); };
    g_loaderSlots.showModel = &engine::displayShow;
    g_loaderSlots.readDoeDocument = &doe::prepareCurrentOutputSession;
    g_loaderSlots.finishDoeOutputClassData = &doe::finishCurrentOutputClassData;
    g_loaderSlots.setDimension = [](engine::GuiModel& m, int dim) {
        const std::int32_t d = dim;
        m.step->set_dimension(&d);                 // 0x140c7acd0(m)->vt[0] = DB_STEP_DATA slot 0 (1401c8ba0)
    };
    g_loaderSlots.prevOp = &engine::previousOperation;
    installL6Slots();
    installL5Slots();                        // Boolean, Convert2Dto3D, CopyMirror (l5_slots.cpp)
    g_loaderSlots.saveNames = &engine::baseSaveNames;        // MDT L4S
    engine::installLoaderSlotsL4();        // MDT L4 (DB-5): Express vt+0x2c0 / vt+0x2b8 (express_sync.cpp)
}

}  // namespace m2m::mst
