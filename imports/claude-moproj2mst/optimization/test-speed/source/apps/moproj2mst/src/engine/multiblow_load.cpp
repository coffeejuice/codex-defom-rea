// MDT L4: load slots of the MultiBlow family (SfOprMultiBlow, SfOprMultiBlow2D, SfOprMultiBlowCommon
// share them: vtables.tsv). docs/re/moproj2mst/mdt/L4/spec.md; decompiles gui_mo/s1/mdt/L4/d0.c,
// d1.c, assembly slots.asm, post.asm. vt+0x58 (0x1405ef430), vt+0x1b8 (0x1405f6520) and vt+0x1f8
// (0x1405f79a0, false) are the SfOprBase bodies (src/engine/class_slots.cpp).
#include <defmodel/layout/cls_1401e7b10.hpp>
#include <defmodel/layout/db_step_data.hpp>

#include "engine_internal.hpp"
#include "m2m/engine.hpp"
#include "m2m/engine_multiblow.hpp"
#include "props_sync.hpp"
#include "m2m/mst/writer.hpp"
#include "m2m/setters.hpp"

namespace m2m::engine {

using def::model::Cls_1401e7b10;

namespace {

[[noreturn]] void unsupported(const std::string& what)
{
    throw Error(M2M_E_UNSUPPORTED, "operation load: " + what);
}

// 0x140060640: slot code names an object (own file id or inherited)
bool codeHasObject(int code)
{
    return code > 0 || code == -2;
}

}  // namespace

// @src 0x140057770  add object no to the operation: the model object (0x140c7c950(m, no, 1, 1)), a new
// object item (0x1400550d0 vt+0x180 = 0x14006c520), a "2signalObjDataAdded()" ("Undefined", k, op id)
// per slot k from the slot count to no - 2 (0x1402cf3c0, to the diagram), the slot code
// (0x140057c60(op, no - 1, id, 0, 1, 1)) and the manager refresh (0x140069620(0x1400550d0(op)))
LLOBJ_DATA* addObjectNo(Project& p, Operation& op, int no, FileEffects& fx)
{
    GuiModel* m = opModel(p, op);          // 0x14005f770
    if (!m) return nullptr;
    LLOBJ_DATA* o = m->objectWrap(no, true, true);
    const int id = newObjectItem(p);
    for (int k = objSlots(op); k < no - 1; ++k) diagramObjDataAdded(p, op.id, k, qt::L("Undefined"));   // MSG, 0x14007b630
    addObjectAt(p, op, no - 1, id, false, true, fx);
    refreshObjManager(p, fx);
    return o;
}

void multiBlowLoadPrologue(mst::OpClass& c, Project& p, Operation& op)
{
    c.m2m::mst::OpClass::loadPrologue(p, op);   // 0x1405f1bb0 (SfOprBase body)
    setTrans2(p, op, true);                // 0x1405ef6f0(self, 1) again (MDT L2)
    setTrans1(p, op, true);                // 0x1405ef750(self, 1) again (MDT L2)
    GuiModel* m = opModel(p, op);          // 0x1405ef2b0 -> 0x14005f770
    if (!m) return;
    Cls_1401e7b10* sc = m->simCtl();       // 0x140c7b2d0
    if (!sc) return;
    const std::int32_t mode = 0;
    sc->dttype.u58 = mode;                 // 0x1408da6f0(sc, &0): SC+0x240 (DTTYPE mode)
    sc->nstep = 10000;                     // SC+0x16c
}

void multiBlowLoadClassData(mst::OpClass&, Project&, Operation&)
{
}

void multiBlowPostLoad(mst::OpClass&, Project& p, Operation& op, FileEffects& fx, int (*vt120)())
{
    // opi = 0x1405ef2b0(self) is `op` (the class's current operation)
    GuiModel* m = opModel(p, op);          // 0x14005f770
    if (!m) return;
    Operation* prev = prevOp(p, op);       // 0x140062960
    if (m->objectCount() < 3) {            // 0x140c7c1a0
        for (int s = 0; s < 3; ++s) {
            if (codeHasObject(objId(op, s)) && slotObject(p, op, s)) continue;   // 0x14005fb30, 0x140057690 (MDT L2)
            const int no = s + 1;
            LLOBJ_DATA* o = addObjectNo(p, op, no, fx);   // 0x140057770
            if (!o) continue;
            bool inherited = false;
            if (prev) {
                const int pc = objId(*prev, s);        // 0x14005fb30
                if (no == 1 && codeHasObject(pc))
                    inherited = true;
                else if (no > 1 && codeHasObject(pc))
                    inherited = true;
            }
            if (inherited) *at<std::int32_t>(o, 0xc) = 5;
            refreshSlotItems(p, op, no, fx);       // 0x140060100 (MDT L7)
        }
    }
    Cls_1401e7b10* sc = m->simCtl();       // 0x140c7b2d0, no null test in the binary
    if (!sc) unsupported("MultiBlow post-load: the operation model has no sim control (the binary writes "
                         "through a null pointer)");
    sc->pdie1 = vt120();                   // SC+0x168 = vt+0x120
    if (ObjEntry* e = objEntryN(op, vt120()))   // 0x1400612d0(op, vt+0x120)
        e->assignMovement = true;          // 0x14004e0e0(e, 1): +0x1bf
    if (op.lubricantId != 0)               // 0x1400657f0: OPI+0x48
        set::lubricantId(op, 0);           // 0x1407b5170
}

void multiBlowPropsLoad(mst::OpClass&, Project& p, Operation& op)
{
    // 0x140054350: the main window lookup's result is unused; props = OPI+0x10 (0x140051230)
    Props* pr = op.props.get();
    if (!pr) return;
    // props vt+0x58 = 0x140612ba0 (returns true; the caller ignores it)
    propsStepsFromSimCtl(p, *pr, nullptr);   // 0x1405fb720(pr, 0), MDT L2
    propsEnvTempFromSimCtl(p, *pr, nullptr);   // 0x1405fbae0(pr, 0), MDT L2
    propsStepByFromSimCtl(p, *pr, nullptr);   // 0x1405fc180(pr, 0), MDT L2
    propsConvectionFromSimCtl(p, *pr, nullptr);   // 0x1405fbe30(pr, 0), MDT L2
}

void multiBlowSaveNames(mst::OpClass& c, Project& p, Operation& op, FileEffects& fx, int (*vt120)())
{
    c.m2m::mst::OpClass::saveNames(p, op, fx);   // 0x1405f22e0 (SfOprBase body)
    GuiModel* m = opModel(p, op);          // 0x1405ef2b0 -> 0x14005f770
    if (!m) return;
    if (!m->simCtl()) return;              // 0x140c7b2d0
    m->simCtl()->pdie1 = vt120();          // second 0x140c7b2d0, SC+0x168 = vt+0x120
}

// @src 0x140602230  (the base 0x1405f1e10 is empty) -> 0x140602260
bool multiBlowSyncFromModelTail(mst::OpClass& c, Project& p, Ctx& ctx, int (*vt120)())
{
    Operation* cur = c.current;                                  // 0x1405ef2b0
    if (!cur) return false;
    // 0x140608ea0: 0x1405ef320(OB, op) dynamic_cast to SfOprMultiBlowProperties (TD 0x1437751e8)
    auto* pr = dynamic_cast<MultiBlowProps*>(c.propsOf(cur));
    if (!pr) return false;
    GuiModel* m = opModel(p, *cur);                              // 0x14005f770
    if (!m || !m->simCtl()) return false;                        // 0x140c7b2d0
    bool error = false;
    std::string msg;
    LLOBJ_DATA* o = m->object(vt120());                          // vt+0x120 (0x140601f50: 2), 0x140c7ae80
    if (!o) {
        msg = "Currently Multi-Blow requires primary die defined as object number 2.";   // 0x1424b01f0
        error = true;
    } else if (*at<std::int32_t>(o, 0x21c8) == 2 || *at<std::int32_t>(o, 0x21c8) == 3) {
        multiBlowEquipmentFromMovement(*pr, at<std::uint8_t>(o, 0x2160));   // 0x14060b6b0
    } else {
        msg = "Currently Multi-Blow can support only hammer and screw type.";   // 0x1424b01a8
        error = true;
    }
    // MSG signalShowMessage(false, type 3 when error, text): the GUI's log
    if (!msg.empty()) ctx.warn("MultiBlow (" + cur->name.utf8() + "): " + msg);
    return !error;
}

}  // namespace m2m::engine
