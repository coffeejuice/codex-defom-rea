// MDT L6: operation-loader slots of SfOprHeatTreat(2D), SfOprDistortion, SfOprDiestress and
// SfOprDiestressBM(2D/3D) (docs/re/moproj2mst/mdt/L6/spec.md). One function per binary function, in
// the original's call order; UI-only calls (signals, diagram, property-tree items, viewer) are
// dropped and named. Paths that need the project database, or data no ported reader keeps, refuse
// with M2M_E_UNSUPPORTED.
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <cmath>
#include <cstring>
#include <functional>
#include <string>
#include <type_traits>
#include <utility>

#include <defcmd/interp.hpp>
#include <defkey/item_writers.hpp>
#include <defmodel/hdrstats/object_stats.hpp>
#include <defmodel/layout/cls_1401e7b10.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/window/inter_object_ops.hpp>

#include "engine_internal.hpp"
#include "gui_model_access.hpp"
#include "m2m/data/diestress.hpp"
#include "m2m/data/diestressbm.hpp"
#include "m2m/data/distortion.hpp"
#include "m2m/data/heattreat.hpp"
#include "m2m/data/l6_blocks.hpp"
#include "m2m/datafiles.hpp"
#include "m2m/engine.hpp"
#include "m2m/ht_schedule.hpp"
#include "m2m/mst/loader_l6.hpp"
#include "m2m/mst/writer.hpp"
#include "m2m/p17.hpp"
#include "m2m/props.hpp"
#include "m2m/props_base.hpp"
#include "m2m/qt4path.hpp"

namespace m2m::engine {
void baseLoad(Project& p, Operation& op, Ctx& ctx, FileEffects& fx);
void baseLoadData(Project& p, Operation& op, Ctx& ctx, FileEffects& fx);
std::shared_ptr<GuiModel> newModel(Project& p, int dim);
}  // namespace m2m::engine

namespace m2m::engine {
void installL6SaveSlots();
}

namespace m2m::mst {
void baseLoadPrologue(OpClass& c, Project& p, Operation& op);   // src/engine/class_slots.cpp
}

namespace m2m::mst {
namespace {

using def::model::Cls_1401e7b10;
using def::model::DB_STEP_DATA;
using def::model::LLOBJ_DATA;
using engine::at;
using engine::FileEffects;
using engine::GuiModel;
using qt::L;

[[noreturn]] void unsupported(const std::string& what)
{
    throw Error(M2M_E_UNSUPPORTED, "operation load: " + what);
}

// 0x1405ef2e0 -> 0x140c7b2d0: sim control of the operation's model, null without one
// The void callback borrows the control inline; it must not retain the pointer.
template<class F> requires std::is_same_v<std::invoke_result_t<F, Cls_1401e7b10*>, void>
void withOpSimCtl(Project& p, Operation& op, F&& writer)
{
    GuiModel* m = engine::opModel(p, op);
    std::invoke(std::forward<F>(writer), m ? m->simCtl() : nullptr);
}

// QString::fromLocal8Bit (system ANSI code page)
String fromLocal8(const char* s)
{
    const int n = static_cast<int>(std::strlen(s));
    if (n == 0) return String(u"");
    const int w = MultiByteToWideChar(CP_ACP, 0, s, n, nullptr, 0);
    std::u16string out(static_cast<std::size_t>(w > 0 ? w : 0), u'\0');
    if (w > 0) MultiByteToWideChar(CP_ACP, 0, s, n, reinterpret_cast<wchar_t*>(out.data()), w);
    return String(std::move(out));
}

int kind(const LLOBJ_DATA* o) { return *at<std::int32_t>(o, 0xc); }
int number(const LLOBJ_DATA* o) { return *at<std::int32_t>(o, 8); }
int dimOf(const GuiModel& m) { return *at<std::int32_t>(m.step.get(), 0x1c); }
// 0x140c7c0e0 (0x140065560) / 0x140c7c0a0 (0x14035b400): model dimension 2 / 3
bool is2D(const GuiModel& m) { return m.step && dimOf(m) == 2; }
bool is3D(const GuiModel& m) { return m.step && dimOf(m) == 3; }

// @src 0x140057690  engine object of slot s when its code is own or inherited (0x140060640)
LLOBJ_DATA* slotObject(Project& p, Operation& op, int s)
{
    GuiModel* m = engine::opModel(p, op);
    if (!m) return nullptr;
    const int c = objId(op, s);                            // 0x14005fb30
    if (c > 0 || c == -2) return m->object(s + 1);         // 0x140060690 || 0x1400606c0
    return nullptr;
}

// QString("%1 %2").arg(v, 0, 'g', -1, ' ').arg(QString(unit name), 0, ' ') of the base props
// readers 0x1405fb720 / 0x1405fbae0 / 0x1405fc180 / 0x1405fbe30
String valueUnit(double v, int unitKind, const Cls_1401e7b10* sc)
{
    const int sys = *at<std::int32_t>(sc, 0xfc);            // 0x1408e01e0
    const String u = String::latin1(units::unitName(unitKind, sys));   // 0x140917de0
    return qt::arg(qt::arg(L("%1 %2"), v, 0, 'g', -1, u' '), u, 0, u' ');
}

// 0x1405f8df0(props) + 0x750: sim control of the props owner's model (0x14005f7a0).
// Both callbacks run synchronously; only the bool reader result leaves this scope.
template<class F> requires std::is_same_v<std::invoke_result_t<F, Cls_1401e7b10*>, bool>
bool withPropsSimCtl(Project& p, Props& pr, Cls_1401e7b10* sc, F&& reader)
{
    if (sc) return std::invoke(std::forward<F>(reader), sc);
    if (!pr.owner) return std::invoke(std::forward<F>(reader), nullptr); // 0x1405f8d90
    return engine::withOpStep(p, *pr.owner, [&](DB_STEP_DATA* step) -> bool {
        return std::invoke(std::forward<F>(reader), step ? &step->sim_ctrl : nullptr);
    });
}

// ---- base props readers (SfOprBaseProperties, called directly by Distortion props vt+0x58) ----

// @src 0x1405fb720  NumSteps, StepIncr, TimePerStep (category 0) from the sim control
bool propsReadSteps(Project& p, Props& pr, Cls_1401e7b10* sc)
{
    return withPropsSimCtl(p, pr, sc, [&](Cls_1401e7b10* control) -> bool {
        if (!control) return false;
        data::CommonBlock& b = *pr.common;
        b.numSteps = *at<std::int32_t>(control, 0x16c);
        b.stepIncr = *at<std::int32_t>(control, 0x164);
        b.dtmax = valueUnit(*at<double>(control, 0x1d0), 0, control);    // B+0x40
        return true;
    });
}

// @src 0x1405fbae0  EnvTemp (category 9) = SimCtrl+0x3d0
bool propsReadEnvTemp(Project& p, Props& pr, Cls_1401e7b10* sc)
{
    return withPropsSimCtl(p, pr, sc, [&](Cls_1401e7b10* control) -> bool {
        if (!control) return false;
        pr.common->envTemp = valueUnit(*at<double>(control, 0x3d0), 9, control);   // B+0x20
        return true;
    });
}

// @src 0x1405fc180  StepBy = SimCtrl+0x240 (0x1405fb450), then DistPerStep (category 1, +0x1e0)
// or TimePerStep (category 0, +0x1d0)
bool propsReadStepBy(Project& p, Props& pr, Cls_1401e7b10* sc)
{
    return withPropsSimCtl(p, pr, sc, [&](Cls_1401e7b10* control) -> bool {
        if (!control) return false;
        base::setStepBy(pr, *at<std::int32_t>(control, 0x240));     // 0x1408da6d0, 0x1405fb450(props, v, 0)
        data::CommonBlock& b = *pr.common;
        if (b.dtMode == 0) b.dsmax = valueUnit(*at<double>(control, 0x1e0), 1, control);   // B+0x38
        else b.dtmax = valueUnit(*at<double>(control, 0x1d0), 0, control);                 // B+0x40
        return true;
    });
}

// @src 0x1405fbe30  ConvectionCoef (category 0x20) = SimCtrl+0x4c8
bool propsReadConvection(Project& p, Props& pr, Cls_1401e7b10* sc)
{
    return withPropsSimCtl(p, pr, sc, [&](Cls_1401e7b10* control) -> bool {
        if (!control) return false;
        pr.common->convection = valueUnit(*at<double>(control, 0x4c8), 0x20, control);   // B+0x30
        return true;
    });
}

// ---- SfOprHeatTreat / SfOprHeatTreat2D ----------------------------------------------------------

// @src 0x1411b11d0 (3D) / 0x1411aebb0 (2D): identical code
void heatTreatPrologue(OpClass& c, Project& p, Operation& op)
{
    baseLoadPrologue(c, p, op);                            // 0x1405f1bb0
    withOpSimCtl(p, op, [](Cls_1401e7b10* sc) -> void {
        if (sc) sc->trans[1] = 0;
    });   // 0x1405ef6f0(self, 0): SC+0x108
    withOpSimCtl(p, op, [](Cls_1401e7b10* sc) -> void {
        if (sc) sc->trans[3] = 0;
    });   // 0x1405ef7b0 -> 0x1408e0040: SC+0x110
    withOpSimCtl(p, op, [](Cls_1401e7b10* sc) -> void {
        if (sc) sc->trans[2] = 0;
    });   // 0x1405ef810 -> 0x1408e0020: SC+0x10c
    withOpSimCtl(p, op, [](Cls_1401e7b10* sc) -> void {
        if (sc) sc->trans[0] = 1;
    });   // 0x1405ef750(self, 1): SC+0x104
}

// @src 0x1411b1240 (3D) / 0x1411aec20 (2D): identical code
void heatTreatLoad(OpClass& c, Project& p, Operation& op, Ctx& ctx, FileEffects& fx)
{
    engine::baseLoad(p, op, ctx, fx);                      // 0x1405ef430
    Operation* cur = c.current;                            // 0x1405ef2b0
    if (!cur) return;
    // __RTDynamicCast(opi+0x10, SfOprHeatTreatProperties, no throw)
    auto* hp = dynamic_cast<HeatTreatProps*>(cur->props.get());   // 0x140051230
    if (!hp) return;
    if (!hp->data) unsupported("HeatTreat properties without their schedule (props+0x68)");
    ht::loadSchedule(p, hp->data->schedule);               // 0x141681de0 -> 0x1411b4770
}

// @src 0x1411bfd60  HeatTreat props vt+0x58 (body in src/ops/heattreat_htwz.cpp)
void heatTreatPropsLoad(OpClass&, Project& p, Operation& op)
{
    auto* hp = dynamic_cast<HeatTreatProps*>(op.props.get());
    if (!hp) unsupported("HeatTreat props vt+0x58 on properties of another class");
    ht::propsRefresh(p, *hp);
}

// ---- SfOprDistortion ----------------------------------------------------------------------------

// @src 0x1413627d0
void distortionPrologue(OpClass& c, Project& p, Operation& op)
{
    baseLoadPrologue(c, p, op);                            // 0x1405f1bb0
    withOpSimCtl(p, op, [](Cls_1401e7b10* sc) -> void {
        if (sc) sc->trans[1] = 1;
    });   // 0x1405ef6f0(self, 1)
    withOpSimCtl(p, op, [](Cls_1401e7b10* sc) -> void {
        if (sc) sc->trans[0] = 0;
    });   // 0x1405ef750(self, 0)
    withOpSimCtl(p, op, [](Cls_1401e7b10* sc) -> void {   // 0x1405ef870(self, 2, 1, -1, 1.0)
        if (!sc) return;
        *at<std::int32_t>(sc, 0x16c) = 2;
        *at<std::int32_t>(sc, 0x164) = 1;
        *at<std::int32_t>(sc, 0x160) = -1;
        *at<double>(sc, 0x1d0) = 1.0;                      // @0x1421249e0
    });
    withOpSimCtl(p, op, [](Cls_1401e7b10* sc) -> void {
        if (sc) *at<std::int32_t>(sc, 0x2e0) = 1;
    });   // 0x1405ef900(self, 1)
    if (!c.current) return;                                // 0x1405ef2b0 -> 0x14005f770
    withOpSimCtl(p, *c.current, [](Cls_1401e7b10* sc) -> void {
        if (sc) *at<std::int32_t>(sc, 0x2e4) = 1;
    });
}

// @src 0x1413628b0  workpiece number of the distortion data: an inherited object when the
// recorded one is undefined
void distortionPostLoad(OpClass& c, Project& p, Operation& op, Ctx&, FileEffects& fx)
{
    Operation* opi = c.current;                            // 0x1405ef2b0
    if (!opi) return;
    GuiModel* m = engine::opModel(p, *opi);
    const int n = m ? m->objectCount() : 0;                // 0x140c7c1a0
    if (n == 0) {
        engine::addObject(p, *opi, fx);                    // 0x140057ba0 (MDT L7)
        GuiModel* m1 = engine::opModel(p, *opi);           // 0x140057710(opi, 1)
        LLOBJ_DATA* o = m1 ? m1->object(1) : nullptr;
        if (o && kind(o) != 5) {
            *at<std::int32_t>(o, 0xc) = 4;
            *at<std::int32_t>(o, 0x2154) = 1;
        }
        return;
    }
    auto* dp = dynamic_cast<DistortionProps*>(c.propsOf(opi));   // 0x1405ef320, TD 0x1439ee1b0
    if (!dp) return;
    if (!dp->data) unsupported("Distortion properties without their pass data (props+0x40)");
    data::DistortionData& d = *dp->data;                   // 0x1401cc6b0
    LLOBJ_DATA* o = m->object(d.workpiece);                // 0x1413651b0, 0x140057710
    if (!o || kind(o) != 7) return;
    for (int i = 0; i < n; ++i) {
        LLOBJ_DATA* q = slotObject(p, *opi, i);            // 0x140057690
        if (q && kind(q) == 5) d.workpiece = number(q);    // 0x1413651d0
    }
    (void)op;
}

// @src 0x1413665c0  Distortion props vt+0x58
void distortionPropsLoad(OpClass&, Project& p, Operation& op)
{
    auto* dp = dynamic_cast<DistortionProps*>(op.props.get());
    if (!dp || !dp->data || !dp->counts) unsupported("Distortion properties without their blocks (+0x38, +0x40)");
    propsReadSteps(p, *dp, nullptr);                       // 0x1405fb720(self, 0)
    propsReadEnvTemp(p, *dp, nullptr);                     // 0x1405fbae0
    propsReadStepBy(p, *dp, nullptr);                      // 0x1405fc180
    propsReadConvection(p, *dp, nullptr);                  // 0x1405fbe30
    dp->counts->fixtures = dp->data->fixtureCount;         // 0x14142ff20: +0x40 -> +4
    dp->counts->passes = dp->data->passCount;              // 0x140cbc690: +0x40 -> +8
}

// ---- SfOprDiestress and SfOprDiestressBM: shared pieces ---------------------------------------

}  // namespace

// @src 0x1400585e0  rename slot s: the project's object item and the slot's engine object, then
// 0x140060100; the class tree 0x1405f0b60 is UI, and signalObjDataChanged reaches the diagram's
// expansion rows (0x14007c710, saved in the .moproj: MDT L4, oracle B 131). Also used by the
// Cogging post-load (MDT L4, cogging_load.cpp; declared in engine_internal.hpp)
bool renameSlot(Project& p, Operation& op, int s, const String& name, FileEffects& fx)
{
    if (!engine::opModel(p, op)) return false;
    if (!(s > -1 && s < objSlots(op))) return false;       // 0x140060600
    Item* it = objItem(op, objId(op, s));                  // 0x14005fb30, 0x140060030
    if (!it) return false;
    setItemName(*it, name);                                // item vt+0x40 (record name and text)
    if (LLOBJ_DATA* o = slotObject(p, op, s)) {            // 0x140057690
        const std::string n = engine::local8(name);
        o->set_name(n.c_str());                            // LLOBJ vt+0x8
        engine::refreshSlotItems(p, op, number(o), fx);    // 0x140060100
    }
    // 0x1405f0b60(class, tree model, s + 1, 0x3f2): UI
    diagramObjDataChanged(p, op.id, s, name);              // 0x1402cf3c0 "2signalObjDataChanged()"
    return true;
}

namespace {

// @src 0x1402b60f0 (Diestress) / 0x140299740 (DiestressBM): identical code
void slotNamesPostLoad(OpClass& c, Project& p, FileEffects& fx)
{
    Operation* opi = c.current;
    if (!opi) return;
    GuiModel* m = engine::opModel(p, *opi);
    if (!m) return;
    const int n = m->objectCount();
    for (int i = 0; i < n; ++i) {
        LLOBJ_DATA* o = m->objectAt(i + 1);                // 0x140c7aed0, no null check
        if (!o) unsupported("object position " + std::to_string(i + 1) + " is empty (0x140c7aed0 result used unchecked)");
        if (kind(o) != 7) renameSlot(p, *opi, number(o) - 1, fromLocal8(at<char>(o, 0x828)), fx);
    }
    withOpSimCtl(p, *opi, [](Cls_1401e7b10* sc) -> void {
        if (sc) sc->trans[1] = 1;
    });   // 0x1405ef6f0(self, 1)
    withOpSimCtl(p, *opi, [](Cls_1401e7b10* sc) -> void {
        if (sc) sc->trans[0] = 0;
    });   // 0x1405ef750(self, 0)
}

// @src 0x1402b59a0 (Diestress) / 0x140299230 (DiestressBM): identical code
void dieStressPrologue(OpClass& c, Project& p, Operation& op)
{
    baseLoadPrologue(c, p, op);                            // 0x1405f1bb0
    withOpSimCtl(p, op, [](Cls_1401e7b10* sc) -> void {
        if (sc) sc->trans[1] = 1;
    });
    withOpSimCtl(p, op, [](Cls_1401e7b10* sc) -> void {
        if (sc) sc->trans[0] = 0;
    });
}

// @src 0x1402b6410 (Diestress) / 0x140299c50 (DiestressBM): vt+0xa8, the object rename loop of the
// post-load around UI tree items ("Step selection", "Objects", "Controls")
bool dieStressTreeFill(OpClass& c, Project& p, Operation& op, FileEffects& fx)
{
    c.objectTreeAdd(p, op, fx);                            // vt+0x228 (0x1402b63d0 / 0x140299c10: UI only)
    GuiModel* m = engine::opModel(p, op);                  // 0x14005f770(opi)
    if (!m) return false;
    // 0x1400c53d0: object rows of the tree (UI)
    const int n = m->objectCount();
    for (int i = 0; i < n; ++i) {
        LLOBJ_DATA* o = m->objectAt(i + 1);                // 0x140c7aed0, no null check
        if (!o) unsupported("object position " + std::to_string(i + 1) + " is empty (0x140c7aed0 result used unchecked)");
        if (kind(o) != 7) renameSlot(p, op, number(o) - 1, fromLocal8(at<char>(o, 0x828)), fx);
    }
    return false;                                          // the tree items that follow are UI
}

// ---- SfOprDiestress -----------------------------------------------------------------------------

// 0x14081fd60(local8(path of the project DB handle P+8), &err) as Diestress's model makers use it:
// the dimension test opens the DB with the Fortran runtime (STATUS 'OLD' @0x145288000), which fails
// without the file; then neither 2D header ("SFTC-DEFORM-2D-DB450", "SFTC-DEF-2D-DB100-00") is read
// and the dimension stays 3. With the file the header read is not ported.
int dieStressModelDim(Project& p)
{
    if (qt::fileExists(engine::projectDb(p)->path))      // DBH+0x28 (MDT DB handle)
        unsupported("Diestress engine model: the dimension is read from the project database header "
                    "(0x14081fd60 -> 0x140b3a320)");
    return 3;
}

// @src 0x1402b57f0  scratch model of the project DB (the caller deletes it)
std::shared_ptr<GuiModel> dieStressScratch(OpClass& c, Project& p)
{
    if (!c.current || !c.current->project) return nullptr;  // 0x1405ef2b0, 0x140055030
    const int dim = dieStressModelDim(p);
    return engine::newModel(p, dim);                       // 0x140c7a5b0(P+8, dim), 0x140822210 (MDT L7)
}

// @src 0x1402b5600  Diestress vt+0x1b0: like 0x1402b57f0 with the active sub-project's DB handle
// (mainWindow vt+0x238 = 0x14001d440 -> 0x1400b9cd0), null without one; no TNOW3 store
std::shared_ptr<GuiModel> diestressCreateModel(OpClass& c, Project& p, Operation&)
{
    if (!c.current || !c.current->project) return nullptr;
    const int dim = dieStressModelDim(p);
    if (!activeSubProject(p)) return nullptr;              // 0x1402b7660
    return engine::newModel(p, dim);                       // 0x140c7a5b0(DBH, dim), 0x140822210
}

// The DB reader of a scratch model without the project DB file has no directory (+0xad8 = 0,
// +0xac8 = 0): 0x140814130 = 0 entries, 0x14080e500(r, -1) = 0, 0x14080e560 = -1.
int lastDbStep(GuiModel&)
{
    return 0;
}

// @src 0x1402b5e50  start step: -(last DB step + 1), or last - 1 when negative
int dieStressStartStep(OpClass& c, Project& p)
{
    if (!c.current || !engine::opModel(p, *c.current)) return 0;
    auto s = dieStressScratch(c, p);
    int v = lastDbStep(*s);
    v = v < 0 ? v - 1 : -(v + 1);
    return v;                                              // scratch deleted (vt+0x18)
}

// @src 0x1402b5d50  inter-object pairs of kind (+0x170) 1 or 2
int dieStressContactPairs(OpClass& c, Project& p)
{
    if (!c.current) return 0;
    GuiModel* m = engine::opModel(p, *c.current);
    if (!m) return 0;
    int k = 0;
    const int n = *at<std::int32_t>(&m->step->inter_obj, 0x10);   // 0x1407d3070
    for (int i = 0; i < n; ++i) {
        auto* pr = def::model::pair_nth(&m->step->inter_obj, i + 1);   // 0x1408e6b70 = 1401fac90
        if (pr && (*at<std::int32_t>(pr, 0x170) == 1 || *at<std::int32_t>(pr, 0x170) == 2)) ++k;
    }
    return k;
}

// @src 0x1402b6870  model from DB step `step` of a scratch model
void dieStressFromDbStep(OpClass& c, Project& p, int step)
{
    Operation* opi = c.current;
    if (!opi) return;
    GuiModel* m = engine::opModel(p, *opi);
    if (!m) return;
    (void)opi->oprFlag;                                    // 0x1400509b0, unused
    auto s = dieStressScratch(c, p);
    // 0x140c7b870(scratch, &step, 1): the reader has no directory, so the step index is -1 and
    // nothing is loaded (lastDbStep)
    (void)step;
    m->reset();                                            // 0x140c7b720
    std::int32_t d;
    engine::GuiModelAccess::withStep(*s, [&](DB_STEP_DATA* scratch) {
        d = *at<std::int32_t>(scratch, 0x1c);              // 0x140c7fee0 = 0x140100980 (+0x1c)
    });
    engine::GuiModelAccess::withStep(*m, [&](DB_STEP_DATA* actual) {
        actual->set_dimension(&d);                        // 0x140c7ff30: DB_STEP_DATA vt slot 0
    });
    opi->objIds.clear();                                   // 0x140060eb0 -> 0x1400424b0(opi+0x40)
    if (s->objectCount() != 0)
        unsupported("Diestress objects copied from a DB step (0x140835430 loop of 0x1402b6870)");
    auto* dp = dynamic_cast<DiestressProps*>(opi->props.get());   // TD 0x1436155e0, unchecked
    if (!dp || !dp->data) unsupported("Diestress properties without their data (props+0x38)");
    dp->data->stepNumber = step;                           // 0x1402bbf30: +0x38 -> +0x80 (signal UI)
    engine::GuiModelAccess::withStep(*m, [&](DB_STEP_DATA* actual) {
        engine::GuiModelAccess::withStep(*s, [&](DB_STEP_DATA* scratch) {
            actual->n_materials = scratch->n_materials;   // +0x1320
            def::cmd::material_list_copy(&actual->materials, &scratch->materials);        // 0x1408bb6c0 = 14029b630
            def::cmd::phase_pair_list_copy(&actual->phase_pairs, &scratch->phase_pairs);  // 0x1409dc7f0 = 140339d30
        });
    });
    engine::GuiModelAccess::withStep(*m, [&](DB_STEP_DATA*) {
        Cls_1401e7b10* sc = m->simCtl();
        if (!sc) return;
        if (is2D(*m)) {
            engine::GuiModelAccess::withStep(*s, [&](DB_STEP_DATA*) {
                sc->geotyp = s->simCtl()->geotyp;          // +0x100
            });
        }
        engine::GuiModelAccess::withStep(*s, [&](DB_STEP_DATA*) {
            sc->pdie1 = s->simCtl()->pdie1;                // +0x168
        });
        s.reset();                                        // vt+0x18(scratch, 1)
        sc->nstart = dieStressStartStep(c, p);             // +0x160
        if (*at<std::int32_t>(sc, 0x16c) < 1) *at<std::int32_t>(sc, 0x16c) = 1;
        if (*at<std::int32_t>(sc, 0x16c) == 0 || *at<std::int32_t>(sc, 0x164) == 0 ||
            std::fabs(*at<double>(sc, 0x1d0)) <= 1e-6) {    // 0x141470a78, @0x142157420
            if (dieStressContactPairs(c, p) == 0) {
                *at<std::int32_t>(sc, 0x16c) = 1;
                *at<std::int32_t>(sc, 0x164) = 1;
            } else {
                *at<std::int32_t>(sc, 0x16c) = 10;
                *at<std::int32_t>(sc, 0x164) = 10;
            }
        }
        *at<std::int32_t>(sc, 0x2e4) = is3D(*m) ? 2 : 1;   // 0x140c7c0a0
        *at<double>(sc, 0x1d0) = 1.0;                      // @0x1421249e0
    });
    // 0x140c7ff80(m, 1, 1): viewer (packet V)
}

// @src 0x1402b7730  node temperatures of every object: 20.0 (SI) or 68.0
void dieStressNodeTemps(OpClass& c, Project& p)
{
    if (!c.current) return;
    GuiModel* m = engine::opModel(p, *c.current);
    if (!m) return;
    DB_STEP_DATA* e = m->step.get();
    double t = 68.0;                                       // @0x1422ad710
    if (e && *at<std::int32_t>(e, 0x84c) == 1) t = 20.0;   // @0x1421ce520 (SC+0xfc unit)
    const int n = m->objectCount();
    for (int i = 0; i < n; ++i) {
        LLOBJ_DATA* o = m->object(i + 1);                  // 0x140c7ae80, no null check
        if (!o) unsupported("object " + std::to_string(i + 1) + " is missing (0x140c7ae80 result used unchecked)");
        const int nn = *at<std::int32_t>(o, 0x880);
        const int w = e->u15f8[8];                         // data+0x1618
        for (int k = 0; k < nn * w; ++k)
            def::model::p407::node_var_08(at<std::byte>(o, 0x8a0))[k] = t;   // 0x140854e60 = 140180c40
    }
}

// @src 0x1402b59e0  Diestress vt+0x1b8
void diestressLoadData(OpClass& c, Project& p, Operation&, Ctx&, FileEffects&)
{
    Operation* opi = c.current;
    if (!opi) return;
    GuiModel* m = engine::opModel(p, *opi);
    if (!m) return;
    // vt+0xf0 = 0x1402b6240(self, &s, 1): the sub-project DB path (0x140055050 -> 0x1400b9e80)
    SubProject* sp = activeSubProject(p);
    const String db = sp ? subProjectDbPath(p, *sp, true) : String();
    if (qt::fileExists(db))
        unsupported("Diestress model from the last step of " + db.utf8() + " (0x140c7b790)");
    const String key = app::changeSuffix(db, L(".KEY"));   // 0x1402b6310 -> 0x140818d50
    if (qt::fileExists(key)) {
        m->readKey(key);                                   // 0x140c7d020
        // 0x140c7ff80(m, 1, 1): viewer (packet V)
    } else {
        auto s = dieStressScratch(c, p);                   // 0x1402b57f0
        const int last = lastDbStep(*s);                   // 0x140814130, 0x14080e500(r, n - 1)
        s.reset();                                         // vt+0x18
        dieStressFromDbStep(c, p, last);                   // 0x1402b6870
    }
    dieStressNodeTemps(c, p);                              // 0x1402b7730
}

// ---- SfOprDiestressBM ---------------------------------------------------------------------------

// @src 0x1402a15d0  wizard block defaults
void bmResetWizard(data::DiestressBMData& d)
{
    d.objTotalNum = 0;
    for (std::size_t i = 0; i < 15; ++i) {
        d.objNo[i] = -1;                                   // +0x14 + 4i
        d.refNo[i] = 1;                                    // +0x50 + 4i
        d.interpolate[i] = 0;                              // +0x04 + i
        d.tolerance[i] = 0.0;                              // +0x90 + 8i
    }
    d.newDbName = String(u"");                             // @0x14228617f ""
    d.useNewDb = 0;                                        // +0x110
    d.f111 = 1;                                            // +0x111
}

// @src 0x1402a2910  wizard block <- the operation's engine objects; default DB name
void bmWizardObjects(Project& p, data::DiestressBMData& d, Operation* opi)
{
    if (!opi) return;
    if (GuiModel* m = engine::opModel(p, *opi)) {
        d.objTotalNum = m->objectCount();
        for (int i = 0; i < d.objTotalNum; ++i) {
            if (i >= 15) unsupported("DiestressBM wizard block of more than 15 objects (0x1402a2910 writes past it)");
            LLOBJ_DATA* o = m->objectAt(i + 1);            // 0x140c7aed0, no null check
            if (!o) unsupported("object position " + std::to_string(i + 1) + " is empty (0x1402a2910)");
            d.objNo[static_cast<std::size_t>(i)] = number(o);
        }
    }
    if (d.newDbName.size() < 2) d.newDbName = L("DieStressDb.DB");   // @0x142289b30
}

// @src 0x140299a90  DiestressBM vt+0x78: DieStressBmWizard.Wzd, else defaults from the model
void diestressBMClassData(OpClass& c, Project& p, Operation&)
{
    auto* bp = dynamic_cast<DiestressBMProps*>(c.propsOf(nullptr));   // 0x1405ef320, TD 0x1435fb1a8
    if (!bp) return;
    if (!bp->data) unsupported("DiestressBM properties without their wizard block (props+0x38)");
    // 0x14029eef0: convertSeparators(taskDirAbs(owner) + sep + "DieStressBmWizard.Wzd") (qDebug)
    const String dir = bp->owner ? taskDirAbs(*bp->owner) : String();
    const String path = qt::convertSeparators(dir + qt::kSep + L("DieStressBmWizard.Wzd"));
    if (!datafiles::readDiestressBmWzdFile(p, path, *bp->data)) {   // 0x1402a1df0
        Operation* opi = c.current;                        // 0x1405ef2b0
        bmResetWizard(*bp->data);                          // 0x1402a15d0
        bmWizardObjects(p, *bp->data, opi);                // 0x1402a2910
    }
}

// @src 0x14029b570 (2D) / 0x14029d3b0 (3D): identical code but the qDebug texts
void diestressBMLoadData(OpClass& c, Project& p, Operation&, Ctx&, FileEffects&)
{
    Operation* opi = c.current;
    if (!opi) return;
    const int fn = flowNumber(*opi);                       // 0x140050b20
    if (!(1 < fn) || !engine::opModel(p, *opi)) return;
    // 0x14080e710(DB reader of 0x14029a500, fn - 1) and 0x14005f270 (engine::interactiveSetup, MDT
    // DB) follow, then the model+0x149 flag and the step loads 0x14029bee0 / 0x14029b7e0
    // (0x14029dd00 / 0x14029d620) or 0x1402a14e0 + 0x1405f6520, none of which is ported.
    unsupported("DiestressBM operation after the first of its chain: its step is chosen from the project "
                "database (0x14080e710, 0x14005f270)");
}

}  // namespace

void installL6Slots()
{
    L6Slots& t = g_l6Slots;
    t.heatTreatLoad = &heatTreatLoad;
    t.heatTreatPrologue = &heatTreatPrologue;
    t.heatTreatPropsLoad = &heatTreatPropsLoad;
    t.distortionPrologue = &distortionPrologue;
    t.distortionPostLoad = &distortionPostLoad;
    t.distortionPropsLoad = &distortionPropsLoad;
    t.diestressPrologue = &dieStressPrologue;
    t.diestressPostLoad = [](OpClass& c, Project& p, Operation&, Ctx&, FileEffects& fx) { slotNamesPostLoad(c, p, fx); };
    t.diestressLoadData = &diestressLoadData;
    t.diestressCreateModel = &diestressCreateModel;
    t.diestressBMPrologue = &dieStressPrologue;
    t.diestressBMClassData = &diestressBMClassData;
    t.diestressBMPostLoad = [](OpClass& c, Project& p, Operation&, Ctx&, FileEffects& fx) { slotNamesPostLoad(c, p, fx); };
    t.diestressBMLoadData = &diestressBMLoadData;
    t.dieStressTreeFill = &dieStressTreeFill;
    engine::installL6SaveSlots();
}

}  // namespace m2m::mst
