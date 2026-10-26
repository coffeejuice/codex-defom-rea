// MDT L2: own loader slots of SfOprHeatTransferCommon (SfOprHeatTransfer, SfOprHeatTransfer2D),
// SfOprHTFurnace and SfOprPre(2D), and the slots of their properties classes that the load calls
// (docs/re/moproj2mst/mdt/L2/spec.md; decompiles gui_mo/s1/mdt/L2/d0..d5.c, ht_post.asm). The
// classes are in src/ops/family.cpp; installLoaderSlotsL2() hands them these bodies. One function
// per binary function, in the original's call order; UI-only calls are dropped and named.
#include <cstring>
#include <memory>
#include <string>

#include <defcmd/interp.hpp>
#include <defcmd/key_file.hpp>
#include <defcmd/p67_5.hpp>
#include <defmodel/layout/cls_1401e7b10.hpp>
#include <defmodel/layout/cls_1403753f0.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/members/ext_deps.hpp>

#include "engine_internal.hpp"
#include "m2m/data/common.hpp"
#include "m2m/data/heattransfer.hpp"
#include "m2m/engine.hpp"
#include "m2m/mst/loader_l2.hpp"
#include "m2m/mst/writer.hpp"
#include "m2m/p17.hpp"
#include "m2m/props.hpp"
#include "m2m/props_base.hpp"
#include "m2m/qt4path.hpp"
#include "props_sync.hpp"

namespace m2m::mst {
void baseLoadPrologue(OpClass& c, Project& p, Operation& op);   // 0x1405f1bb0 (class_slots.cpp)
}

namespace m2m::engine {

using def::model::Cls_1401e7b10;
using qt::L;

namespace {

[[noreturn]] void unsupported(const std::string& what)
{
    throw Error(M2M_E_UNSUPPORTED, "operation load: " + what);
}

// 0x140060640 (P01's slotHoldsObject)
bool slotHasObject(int code)
{
    return slotHoldsObject(code);
}

}  // namespace

// @src 0x14005f7e0  the operation's sim control, or null
Cls_1401e7b10* opSimCtl(Project& p, Operation& op)
{
    GuiModel* m = opModel(p, op);
    return m ? m->simCtl() : nullptr;
}

// @src 0x1405ef6f0  SfOprBase: SC+0x108 (TRANS 2) = v
void setTrans2(Project& p, Operation& op, bool v)
{
    if (Cls_1401e7b10* sc = opSimCtl(p, op)) sc->trans[1] = v ? 1 : 0;
}

// @src 0x1405ef750  SfOprBase: SC+0x104 (TRANS 1) = v
void setTrans1(Project& p, Operation& op, bool v)
{
    if (Cls_1401e7b10* sc = opSimCtl(p, op)) sc->trans[0] = v ? 1 : 0;
}

// @src 0x14005fab0  number of slots of op whose code holds an object
int objectSlotCount(const Operation& op)
{
    int n = 0;
    const int ns = objSlots(op);                            // 0x14005fa80
    for (int i = 0; i < ns; ++i)
        if (slotHasObject(objId(op, i))) ++n;               // 0x14005fb30
    return n;
}

// @src 0x140057690  engine object of slot s when the slot's code holds one
LLOBJ_DATA* slotObject(Project& p, Operation& op, int s)
{
    GuiModel* m = opModel(p, op);
    if (!m) return nullptr;
    if (!slotHasObject(objId(op, s))) return nullptr;
    return m->object(s + 1);                                // 0x140c7ae80
}

namespace {

// ---- SfOprHeatTransferProperties -----------------------------------------------------------------

HeatTransferProps* htProps(Props* pr)
{
    return dynamic_cast<HeatTransferProps*>(pr);            // __RTDynamicCast(.., 0x143af4bc8)
}

// @src 0x141357460  vt+0x68: step data (+0x58) from a sim control
bool htStepDataFromSimCtl(HeatTransferProps& pr, Cls_1401e7b10* sc)
{
    if (!sc) return false;
    data::HeatTransferStepData& d = *pr.step;
    d.stepDef = sc->stpdef - 1;                             // SC+0x1c8
    d.dtMax = sc->dtmax1;                                   // SC+0x1d0
    d.dTempMax = sc->dtpmax1;                               // SC+0x270
    d.dTempLmt0 = sc->dtpmax2_5[0];                         // SC+0x2a0
    d.dTempLmt1 = sc->dtpmax2_5[1];                         // SC+0x2a8
    d.stpInc = sc->stpinc;                                  // SC+0x164
    return true;
}

// @src 0x141358150  vt+0x120: heating time, its label, TMAX and ENVTMP by the operation's sub type
void htSubTypeDefaults(Project& p, HeatTransferProps& pr)
{
    Operation* owner = pr.owner;                            // 0x1405f8d90
    if (!owner) return;
    [&p, &pr, owner](Cls_1401e7b10* sc) -> void {
        if (!sc) return;
        const String name = owner->subType;                     // 0x140050da0 (OPI+0x128)
        String label = L("Heating time");                       // 0x142e0a050
        const int unit = sc->unit;                              // 0x1408e01e0
        double t = 3600.0;                                      // 0x1424bb068
        double env = unit == 1 ? 20.0 : 68.0;                   // 0x1421ce520 / 0x1422ad710
        if (qt::contains(name, L("Transfer through air"), qt::Case::Insensitive)) {   // 0x142e0a060
            t = 15.0;                                           // 0x142142628
            label = L("Transfer time");                         // 0x142e0a078
        } else if (qt::contains(name, L("Rest on die"), qt::Case::Insensitive)) {     // 0x142e0a088
            t = 4.0;                                            // 0x1421d3c70
            label = L("Resting time");                          // 0x142e0a098
        } else if (qt::contains(name, L("Dwell on die"), qt::Case::Insensitive)) {    // 0x142e0a0a8
            t = 4.0;
            label = L("Dwell time");                            // 0x142e0a0b8
        } else {
            env = unit == 1 ? 1200.0 : 2250.0;                  // 0x142e0a6b0 / 0x142e0a6a8
        }
        propsSetHeatingTime(p, pr, t);                          // 0x1405fcb90
        // 0x1405fd090: B+0x48 = label, the display label of HeatingTime (UI, see data/common.hpp)
        (void)label;
        sc->tmax = t;                                           // SC+0x178
        sc->envtmp.ndim = 0;                                    // SC+0x3cc
        sc->envtmp.value = env;                                 // SC+0x3d0
    }(opSimCtl(p, *owner));
}

// @src 0x1413556c0  sub type index of the class's current operation: "Heat in furnace" 0,
// "Rest on die" 2, "Dwell on die" 3, "Transfer through air" 1, else -1 (case-insensitive contains,
// in this order)
int subTypeIndex(const String& name)
{
    if (qt::contains(name, L("Heat in furnace"), qt::Case::Insensitive)) return 0;
    if (qt::contains(name, L("Rest on die"), qt::Case::Insensitive)) return 2;
    if (qt::contains(name, L("Dwell on die"), qt::Case::Insensitive)) return 3;
    if (qt::contains(name, L("Transfer through air"), qt::Case::Insensitive)) return 1;
    return -1;
}

// @src 0x1413558b0  sub type text of an index (0x140050d50 assigns it when it differs)
void setSubType(Operation& op, int v)
{
    String s;
    if (v == 0) s = L("Heat in furnace");                   // 0x142e02650
    else if (v == 2) s = L("Rest on die");                  // 0x142e02660
    else if (v == 3) s = L("Dwell on die");                 // 0x142e02670
    else if (v == 1) s = L("Transfer through air");         // 0x142e02680
    if (!s.isEmpty() && !(op.subType == s)) op.subType = s;
}

// ---- SfOprHTFurnaceProperties --------------------------------------------------------------------

HTFurnaceProps* furnaceProps(Props* pr)
{
    return dynamic_cast<HTFurnaceProps*>(pr);               // __RTDynamicCast(.., 0x14393d500)
}

// @src 0x140c69350  vt+0x68: data (+0xd0) from a sim control
bool furnaceDataFromSimCtl(HTFurnaceProps& pr, Cls_1401e7b10* sc)
{
    if (!sc) return false;
    data::HTFurnaceData& d = *pr.data;
    d.viewFactor = sc->vfactr_mode == 1;                    // SC+0x9d8
    d.stepDef = (sc->stpdef == 1 || sc->stpdef == 2) ? 0 : 1;   // SC+0x1c8
    d.dtMax = sc->dtmax1;                                   // SC+0x1d0
    d.dTempMax = sc->dtpmax1;                               // SC+0x270
    d.dTempLmt0 = sc->dtpmax2_5[0];                         // SC+0x2a0
    d.dTempLmt1 = sc->dtpmax2_5[1];                         // SC+0x2a8
    d.stpInc = sc->stpinc;                                  // SC+0x164
    return true;
}

// @src 0x140c69580  ENVTMP constant (S+0xb20 = SC+0x3d0) = value (9, 0, 0, 1) of furnace curve 8;
// then MSG signalEqpDataAdded(furnace name, -1, operation id) to the diagram (packet moproj)
void furnaceEnvTemp(Project& p, HTFurnaceProps& pr)
{
    withPropsStep(p, pr, [&](DB_STEP_DATA* s) {  // 0x1405f8df0
        if (!s) return;
        std::int32_t k = 8;
        def::model::DYNARY* c = s->sim_ctrl.furnace.curve(&k);  // 0x1409f0660 = 140375980
        if (c) s->sim_ctrl.envtmp.value = def::model::ext::fun_140130720(c, 9, 0, 0, 1);   // 0x14089c490
        // MSG signalEqpDataAdded(QString(S+0xb90), -1, id of 0x1405f8d90) -> the diagram's object row
        if (pr.owner) diagramEqpDataAdded(p, pr.owner->id, -1, String::latin1(s->sim_ctrl.furnace.name));   // 0x14007d730
    });
}

// @src 0x140c67a60  furnace model (+0x38) = the engine's (0x1409f0230 = 140375550), FURNAM (+0xd0
// +0) = its name; MSG signal 0x1402cea00 (UI)
void furnaceSetModel(HTFurnaceProps& pr, def::model::Cls_1403753f0* f)
{
    def::cmd::furnace_model_copy(pr.furnace.get(), f);
    pr.data->name = String::latin1(f->name);                // QString(const char*)
}

}  // namespace

// ==== SfOprHeatTransferCommon ======================================================================

// @src 0x141354ca0  vt+0xb8: base prologue, TRANS 2 = 0, TRANS 1 = 1
void htLoadPrologue(mst::OpClass& c, Project& p, Operation& op)
{
    mst::baseLoadPrologue(c, p, op);                        // 0x1405f1bb0
    setTrans2(p, op, false);                                // 0x1405ef6f0(self, 0)
    setTrans1(p, op, true);                                 // 0x1405ef750(self, 1)
}

// @src 0x141354ce0  vt+0xc0: sub type from the predecessor, missing objects, STNOUT/ELMNOD
void htPostLoad(mst::OpClass& c, Project& p, Operation& op, Ctx&, FileEffects&)
{
    // 0x1405ef3a0: the class's object-tree list (UI object, never null)
    GuiModel* m = opModel(p, op);                           // 0x14005f770
    if (!m) return;
    HeatTransferProps* pr = htProps(c.propsOf(nullptr));    // 0x1405ef320(self, 0)
    if (!pr) return;
    int keyRes = 2;
    auto scratch = std::make_unique<DB_STEP_DATA>(3);       // 0x140821bf0(stack, 3)
    Operation* prev = prevOp(p, op);                        // 0x140062960
    if (prev) {
        const String path = simCtrlPath(*prev);             // 0x140059290
        if (qt::fileExists(path)) {
            const std::int32_t zero = 0;
            const char reset = 1;
            const std::string lp = local8(path);
            keyRes = def::cmd::read_keyword_file(scratch.get(), lp.c_str(), &reset, &zero);   // 0x1408969a0
        }
    }
    // Acquire after the predecessor KEY read, before subtype/object/default effects.
    // simCtl keeps its geometry gate and branch-dependent nullable-control policy.
    [&p, &op, m, pr, prev, keyRes, &scratch](Cls_1401e7b10* sc) {
        int v = subTypeIndex(op.subType);                       // 0x1413556c0
        if (op.oprFlag == 0 || v == -1) {                       // 0x1400509d0
            v = 1;
            if (isChainHead(op)) {                              // 0x140050bc0
                v = 1;
            } else if (prev) {
                const int r = subTypeIndex(prev->subType);      // 0x141355500(self, 0x140050da0(prev))
                if (r == -1) {
                    if (keyRes == 0 && scratch->sim_ctrl.trans[1] != 0) v = 3;   // 0x1408dfd80: SC+0x108
                } else if (r == 1) {
                    v = 2;
                }
            }
            setSubType(op, v);                                  // 0x1413558b0
        }
        v = subTypeIndex(op.subType);
        int nObj = m->objectCount();                            // 0x140c7c1a0
        if (nObj < 0) nObj = 0;
        int n = 1;
        if (prev) {
            n = objectSlotCount(*prev);                         // 0x14005fab0
            if (n < 1) n = 1;
        }
        if (op.oprFlag == 0 && n < 2) {
            n = 3;
            if (v == 1 || v == 0) n = 1;
        }
        if (op.oprFlag != 0 && slotHasObject(objId(op, 0)) && nObj >= 1) return;
        for (int i = 0; i < n; ++i) {
            if (slotHasObject(objId(op, i)) && slotObject(p, op, i)) continue;
            // 0x140057ba0 adds an object item to the project and the slot (SfMoObjManager vt+0x180,
            // 0x140057c60), then 0x140060100 and props vt+0x128 (0x141358890, object temperature)
            unsupported("the heat-transfer post-load adds object " + std::to_string(i + 1) + " to the project (0x140057ba0)");
        }
        if (op.oprFlag == 0) {
            htSubTypeDefaults(p, *pr);                          // props vt+0x120
            if (prev && keyRes == 0) {
                for (int i = 0; i < 7; ++i) sc->stnout[i] = scratch->sim_ctrl.stnout[i];     // SC+0x11c
                for (int i = 0; i < 10; ++i) sc->elmnod[i] = scratch->sim_ctrl.elmnod[i];   // SC+0x138
            }
        }
    }(opSimCtl(p, op));                                    // 0x14005f7e0
    // ~scratch: 0x140824560
}

// @src 0x141357520  SfOprHeatTransferProperties vt+0x58
void htPropsLoad(mst::OpClass& c, Project& p, Operation& op)
{
    Props* base = op.props.get();                           // 0x140051230 (OPI+0x10)
    if (!base) return;
    HeatTransferProps* pr = htProps(base);
    if (!pr) unsupported(c.key().utf8() + " properties are not SfOprHeatTransferProperties");
    propsStepsFromSimCtl(p, *pr, nullptr);                  // 0x1405fb720
    propsEnvTempFromSimCtl(p, *pr, nullptr);                // 0x1405fbae0
    propsConvectionFromSimCtl(p, *pr, nullptr);             // 0x1405fbe30
    if (pr->owner) htStepDataFromSimCtl(*pr, opSimCtl(p, *pr->owner));   // vt+0x68
}

// ==== SfOprHTFurnace ===============================================================================

// @src 0x140c657b0  vt+0xb8: base prologue, TRANS 2 = 0, TRANS 1 = 1, TNOW 3 = 1
void furnaceLoadPrologue(mst::OpClass& c, Project& p, Operation& op)
{
    mst::baseLoadPrologue(c, p, op);                        // 0x1405f1bb0
    setTrans2(p, op, false);
    setTrans1(p, op, true);
    if (GuiModel* m = opModel(p, op)) m->setFurnaceLoadTnow3();
}

// @src 0x140c65830  vt+0xc0: furnace on, furnace curves with their defaults when there are none
void furnacePostLoad(mst::OpClass& c, Project& p, Operation& op, Ctx&, FileEffects&)
{
    // 0x1405ef3a0: object-tree list (UI object, never null)
    GuiModel* m = opModel(p, op);                           // 0x14005f820
    if (!m) return;
    m->initializeFurnacePostLoadDefaults();
    (void)furnaceProps(c.propsOf(nullptr));                 // dynamic_cast, result unused
}

// @src 0x140c69480  SfOprHTFurnaceProperties vt+0x58
void furnacePropsLoad(mst::OpClass& c, Project& p, Operation& op)
{
    Props* base = op.props.get();
    if (!base) return;
    HTFurnaceProps* pr = furnaceProps(base);
    if (!pr) unsupported(c.key().utf8() + " properties are not SfOprHTFurnaceProperties");
    withPropsStep(p, *pr, [&](DB_STEP_DATA* s) {  // 0x1405f8df0
        if (!s) return;
        furnaceEnvTemp(p, *pr);                                 // 0x140c69580
        furnaceSetModel(*pr, &s->sim_ctrl.furnace);             // 0x140c67a60(self, S+0xb90)
        if (pr->owner) furnaceDataFromSimCtl(*pr, opSimCtl(p, *pr->owner));   // vt+0x68
        // MSG signal 0x1402cea00 (UI)
    });
}

// ==== SfOprPre(2D) ================================================================================

// @src 0x14134c380  SfOprPreProperties vt+0x58
void prePropsLoad(mst::OpClass&, Project& p, Operation& op)
{
    Props* pr = op.props.get();
    if (!pr) return;
    propsStepsFromSimCtl(p, *pr, nullptr);                  // 0x1405fb720
    propsEnvTempFromSimCtl(p, *pr, nullptr);                // 0x1405fbae0
    propsStepByFromSimCtl(p, *pr, nullptr);                 // 0x1405fc180
}

// @src 0x140c66230  vt+0x228: the base 0x1405eea60 (UI tree, vt+0x258 0x1405ef120: UI), then with
// the object tree (OB+0x18, a UI object that always exists): tree rows 0x1400c3ce0 / 0x1400c4770 /
// 0x1400c45b0 (UI), 0x140c668c0, 0x140c666f0 (UI) and vt+0x248 0x140c667a0 (UI)
void furnaceObjectTreeAdd(mst::OpClass&, Project& p, Operation& op)
{
    // @src 0x140c668c0  thermal BCs of every object cleared (0x140878b80 = DEF_PRE 140204d20)
    for (int i = 0;; ++i) {
        GuiModel* m = opModel(p, op);                       // 0x14005f770
        if (m->objectCount() <= i) break;                   // 0x140c7c1a0
        LLOBJ_DATA* o = slotObject(p, op, i);               // 0x140057690
        if (!o)
            unsupported("HTFurnace object-tree slot: slot " + std::to_string(i + 1) +
                        " has no engine object (0x140878b80 reads a null object)");
        def::cmd::genbcc::reset_thermal_bc(*o);
    }
}

// ==== save current operation (packet S's 0x140052ec0) =================================================

// @src 0x140c69920  furnace name (S+0xb90) = FURNAM (+0xd0 +0, toLocal8Bit, strcpy: no bound) and
// curve 8 value (9, 0, 0, 1) = ENVTMP constant (S+0xb20); MSG signalEqpDataAdded to the diagram
void furnaceToEngine(Project& p, HTFurnaceProps& pr)
{
    withPropsStep(p, pr, [&](DB_STEP_DATA* s) {  // 0x1405f8df0
        if (!s) return;
        const std::string name = local8(pr.data->name);
        if (name.size() >= sizeof s->sim_ctrl.furnace.name)
            unsupported("FURNAM of " + std::to_string(name.size()) + " bytes overruns the engine's 0x40-byte furnace name "
                        "(0x140c69920 strcpy)");
        std::strcpy(s->sim_ctrl.furnace.name, name.c_str());
        std::int32_t k = 8;
        if (def::model::DYNARY* c = s->sim_ctrl.furnace.curve(&k))            // 0x1409f0660
            def::model::ext::fun_140130800(c, s->sim_ctrl.envtmp.value, 9, 0, 0, 1);   // 0x14089c570
        if (pr.owner) diagramEqpDataAdded(p, pr.owner->id, -1, String::latin1(s->sim_ctrl.furnace.name));   // 0x14007d730
    });
}

// @src 0x140c691f0  vt+0x70: sim control from the data (+0xd0)
bool furnaceDataToSimCtl(HTFurnaceProps& pr, Cls_1401e7b10* sc)
{
    if (!sc) return false;
    const data::HTFurnaceData& d = *pr.data;
    sc->vfactr_mode = d.viewFactor ? 1 : 0;                 // SC+0x9d8
    sc->stpdef = d.stepDef == 0 ? 1 : 3;                    // SC+0x1c8
    sc->dtmax1 = d.stepDef == 0 ? d.dtMax : d.dTempLmt0;    // SC+0x1d0 (+0x18 or +0x28)
    sc->dtpmax1 = d.dTempMax;                               // SC+0x270
    sc->dtpmax2_5[0] = d.dTempLmt0;                         // SC+0x2a0
    sc->dtpmax2_5[1] = d.dTempLmt1;                         // SC+0x2a8
    sc->stpinc = d.stpInc;                                  // SC+0x164
    return true;
}

// @src 0x140c69520  SfOprHTFurnaceProperties vt+0x60 (no base helpers)
void furnacePropsToEngine(Project& p, Operation& op)
{
    HTFurnaceProps* pr = furnaceProps(op.props.get());
    if (!pr) unsupported(op.classKey.utf8() + " properties are not SfOprHTFurnaceProperties");
    furnaceToEngine(p, *pr);                                // 0x140c69920
    if (pr->owner) furnaceDataToSimCtl(*pr, opSimCtl(p, *pr->owner));   // vt+0x70
}

void installLoaderSlotsL2()
{
    mst::g_loaderSlotsL2.htLoadPrologue = &htLoadPrologue;
    mst::g_loaderSlotsL2.htPostLoad = &htPostLoad;
    mst::g_loaderSlotsL2.htPropsLoad = &htPropsLoad;
    mst::g_loaderSlotsL2.furnaceLoadPrologue = &furnaceLoadPrologue;
    mst::g_loaderSlotsL2.furnacePostLoad = &furnacePostLoad;
    mst::g_loaderSlotsL2.furnacePropsLoad = &furnacePropsLoad;
    mst::g_loaderSlotsL2.prePropsLoad = &prePropsLoad;
    mst::g_loaderSlotsL2.furnaceObjectTreeAdd = &furnaceObjectTreeAdd;
}

}  // namespace m2m::engine
