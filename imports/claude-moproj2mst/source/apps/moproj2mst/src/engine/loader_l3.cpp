// MDT L3: own loader slots of SfOprForming(2D), SfOprFormingExpressCommon (SfOprFormingExpress,
// SfOprFormingExpress2D) and SfOprExpressBase (SfOprHeatTransferExpress, SfOprHeatTransferExpress2D),
// and the props vt+0x58 bodies of their properties classes (docs/re/moproj2mst/mdt/L3/spec.md;
// decompiles gui_mo/s1/mdt/L3/d0..d5.c, facf0.asm). The classes are in src/ops/family.cpp and
// src/ops/express.hpp; installLoaderSlotsL3() hands them these bodies. One function per binary
// function, in the original's call order; UI-only calls are dropped and named. Paths that need code
// this packet does not reproduce (adding, merging or deleting objects, the wizard defaults singleton
// 0x141401bd0, creating an equipment item) refuse with M2M_E_UNSUPPORTED at the point the original
// would start changing state.
#include <cmath>
#include <memory>
#include <string>

#include <defcmd/key_file.hpp>
#include <defdbio/objcheck/objcheck.hpp>
#include <def_pre/menu56.hpp>
#include <defmodel/layout/cls_1401e7b10.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/llobj_data.hpp>

#include "engine_internal.hpp"
#include "m2m/data/express.hpp"
#include "m2m/data/heattransfer.hpp"
#include "m2m/engine.hpp"
#include "m2m/doe_io.hpp"
#include "m2m/engine/model_manager.hpp"
#include "m2m/engine/conversion_session.hpp"
#include "m2m/engine/simcontrol300.hpp"
#include "m2m/mst/loader_l3.hpp"
#include "m2m/mst/writer.hpp"
#include "m2m/props.hpp"
#include "m2m/props_forming.hpp"
#include "m2m/qt4path.hpp"
#include "ops/express.hpp"
#include "props_sync.hpp"

namespace m2m::mst {
void baseLoadPrologue(OpClass& c, Project& p, Operation& op);   // 0x1405f1bb0 (class_slots.cpp)
}

namespace m2m::engine {

using def::model::Cls_1401e7b10;
using qt::L;

namespace {

constexpr double kEps = 1e-06;             // 0x142157420

[[noreturn]] void unsupported(const std::string& what)
{
    throw Error(M2M_E_UNSUPPORTED, "operation load: " + what);
}

std::string opLabel(const Operation& op)
{
    return op.classKey.utf8() + " \"" + op.name.utf8() + "\"";
}

// dynamic_cast<SfOprExpressProperties*> (__RTDynamicCast(.., 0x143579b60, 0x143579bb8))
ExpressProps* expressProps(Props* pr)
{
    return dynamic_cast<ExpressProps*>(pr);
}

// SfOprExpressProperties vt+0x100: pure in SfOprExpressProperties; 0x1400707e0 (false) in
// SfOprFormingExpressProperties, 0x1413e5ef0 (true) in SfOprHeatTransferExpressProperties
bool expressIsHeat(const ExpressProps& pe)
{
    if (dynamic_cast<const FormingExpressProps*>(&pe)) return false;
    if (dynamic_cast<const HeatTransferExpressProps*>(&pe)) return true;
    unsupported("SfOprExpressProperties vt+0x100 of a props object that is neither FormingExpress nor "
                "HeatTransferExpress (pure virtual 0x141470184)");
}

// 0x140065560: model dimension (S+0x1c) == 2 (ledger: out-of-scope; P16 reads the same flag)
bool stepIs2D(const DB_STEP_DATA* s)
{
    return *at<std::int32_t>(s, 0x1c) == 2;
}

// 0x140c7c0e0: the model's step is 2D, false without a step (ledger owner P16, engineFacts)
bool modelIs2D(GuiModel& m)
{
    return m.step && stepIs2D(m.step.get());
}

// @src 0x14082f4b0  (= DEF_PRE 0x1401d60e0) movement record (object+0x2160) of the PDIE object
// (S+0x8b8), null when PDIE < 1 or the object is missing
std::byte* pdieMovement(GuiModel& m)
{
    const int pdie = m.step->sim_ctrl.pdie1;               // S+0x8b8
    if (pdie < 1) return nullptr;
    LLOBJ_DATA* o = m.object(pdie);                        // 0x140830270
    return o ? at<std::byte>(o, 0x2160) : nullptr;
}

// The props' owner model (0x1405f8df0 -> 0x14005f7a0 builds it on demand) as a GuiModel
GuiModel* propsModel(Project& p, Props& pr)
{
    if (!pr.owner) return nullptr;                         // 0x1405f8d90: props+0x20
    return opModel(p, *pr.owner);
}

// "Contains" with Qt::CaseSensitivity as passed (1 sensitive, 0 insensitive)
bool has(const String& s, const char* what, qt::Case cs)
{
    return qt::contains(s, L(what), cs);
}

// ==== SfOprForming(2D) =============================================================================

// vt+0xb8  @src 0x140c060f0 (Forming) / 0x140c04910 (Forming2D)
void formingLoadPrologue(mst::OpClass& c, Project& p, Operation& op)
{
    mst::baseLoadPrologue(c, p, op);                        // 0x1405f1bb0
    setTrans2(p, op, true);                                 // 0x1405ef6f0(self, 1)
    setTrans1(p, op, true);                                 // 0x1405ef750(self, 1)
}

// vt+0xc0  @src 0x140c06130 (Forming) / 0x140c04950 (Forming2D): when the operation has no
// equipment (OPI+0x4c < 1) and the PDIE object and its GUI slot exist, the GUI writes the PDIE
// object's movement as an equipment KEY ("<temp>/Equipment (<n+1>).KEY", 0x140358190), adds it to
// the equipment manager (0x1400bf510 -> SfMoEqpManager vt+0x160), copies the name into the movement,
// writes the KEY again at the item's path and sets the slot's and the operation's equipment id.
void formingPostLoad(mst::OpClass&, Project& p, Operation& op, Ctx&, FileEffects& fx)
{
    GuiModel* m = opModel(p, op);                          // 0x14005f770
    if (!m) return;
    Cls_1401e7b10* sc = m->simCtl();                       // 0x140c7b2d0
    if (!sc) return;
    const int pdie = sc->pdie1;                            // SC+0x168
    const int eq = op.equipmentId;                         // 0x140066760: OPI+0x4c
    LLOBJ_DATA* o = m->object(pdie);                       // 0x140057710
    ObjEntry* slot = objEntryN(op, pdie);                  // 0x1400612d0
    // 0x1400a5970: the equipment manager (P+0x19e0) always exists in the converter's model
    if (eq < 1 && o && slot) {
        // packet S: createEquipment (postload_views.cpp), then 0x14004deb0 and 0x14004ca30
        const int id = createEquipment(p, *m->step, o, fx);
        slot->equipmentId = id;
        op.equipmentId = id;
    }
}

// @src 0x140c07eb0  data block <- movement record mv; true when the movement type maps to an
// equipment type, then both groups of the tree are re-made
bool formingFromMovement(FormingProps& pr, const std::byte* mv)
{
    FormingData& d = pr.d;
    bool r = false;
    d.name = String::latin1(reinterpret_cast<const char*>(mv));   // QString(const char*) of mv+0
    const int mt = *at<std::int32_t>(mv, 0x68);
    if (mt == 1) {
        d.type = 0;
        r = true;
    } else if (mt == 0) {
        d.type = 1;
        r = true;
    } else if (mt == 4) {
        d.type = 2;
        d.mechPressType = 0;
        d.totalStroke = *at<double>(mv, 0x318);
        d.cyclesSec = *at<double>(mv, 0x320);
        d.connectingRodLength = *at<double>(mv, 0x328);
        r = true;
    } else if (mt == 2) {
        d.type = 3;
        d.hammerEnergy = *at<double>(mv, 0x1f0);
        d.hammerMass = *at<double>(mv, 0x1e8);
        d.hammerEfficiency = *at<double>(mv, 0x1f8);
        r = true;
    } else if (mt == 3) {
        d.type = 4;
        d.screwEnergy = *at<double>(mv, 0x280);
        d.screwEfficiency = *at<double>(mv, 0x288);
        d.momentOfInertia = *at<double>(mv, 0x290);
        d.leadScrewPitch = *at<double>(mv, 0x298);
        r = true;
    } else if (mt == 7 || mt == 8 || mt == 9 || mt == 10) {
        d.type = 5;
        r = true;
    } else if (mt == 5) {
        d.type = 6;
        r = true;
    } else if (mt == 6) {
        d.type = 7;
        r = true;
    }
    if (r) {
        formingRemakeEquipmentType(pr);                    // 0x140c0a040
        formingRemakeEquipmentSpec(pr);                    // 0x140c0a920
    }
    return r;
}

// SfOprFormingProperties vt+0x58  @src 0x140c09da0
void formingPropsLoad(mst::OpClass&, Project& p, Operation& op)
{
    Props* base = op.props.get();                          // 0x140054350: OPI+0x10
    if (!base) return;
    auto* pr = dynamic_cast<FormingProps*>(base);
    if (!pr) unsupported(opLabel(op) + ": its properties are not SfOprFormingProperties");
    propsStepsFromSimCtl(p, *pr, nullptr);                 // 0x1405fb720(self, 0)
    propsEnvTempFromSimCtl(p, *pr, nullptr);               // 0x1405fbae0(self, 0)
    propsStepByFromSimCtl(p, *pr, nullptr);                // 0x1405fc180(self, 0)
    propsConvectionFromSimCtl(p, *pr, nullptr);            // 0x1405fbe30(self, 0)
    if (!withPropsStep(p, *pr, [](DB_STEP_DATA* s) -> bool { return s != nullptr; })) return; // 0x1405f8df0
    GuiModel* m = propsModel(p, *pr);
    const std::byte* mv = pdieMovement(*m);                // 0x14082f4b0
    if (!mv) {
        pr->d.disabled = false;                            // D+0x10 = 0
        // 0x140370e90(D+0x20 node, 1), 0x140370e90(D+0x80 node, 1): QtProperty enabled (display)
    } else {
        pr->d.disabled = true;                             // D+0x10 = 1
        // 0x140370e90(.., 0) on both groups (display)
        formingFromMovement(*pr, mv);                      // 0x140c07eb0
    }
    // 0x1402cea00(MSG): messenger signal (UI)
}

// ==== SfOprExpressBase (SfOprHeatTransferExpress, SfOprHeatTransferExpress2D) ======================

// @src 0x1413fa950  first set-up (OprFlag 0) of an Express operation: copy the wizard choices of the
// previous Express operation (0x1413faaa0) unless the wizard data was loaded
// @src 0x1400b7fb0  the operation before `op` in the sub-project's chain list (0x1400b94f0, without
// Report/DoeOutput/ShapeOptim): the index of the first entry with op's id (0x140050910), minus one;
// null when op is not in the list or is its first entry
Operation* prevInChain(const SubProject& sub, const Operation& op)
{
    const std::vector<Operation*> ops = flowOperations(sub);
    int k = 0;
    for (; k < static_cast<int>(ops.size()); ++k) {
        if (!ops[static_cast<std::size_t>(k)])
            unsupported("a gap in the flow chain's operation list (0x1400b94f0) is read unchecked by 0x1400b7fb0");
        if (ops[static_cast<std::size_t>(k)]->id == op.id) break;
    }
    const int i = k - 1;
    if (i < 0 || i >= static_cast<int>(ops.size())) return nullptr;
    return ops[static_cast<std::size_t>(i)];
}

// @src 0x1413faaa0  the Express props of the nearest earlier operation of the chain that has them
ExpressProps* prevExpressProps(Project& p, Operation& op)
{
    SubProject* sub = activeSubProject(p);                 // 0x140055050
    if (!sub) return nullptr;
    for (Operation* o = &op;;) {
        if (!o) return nullptr;
        o = prevInChain(*sub, *o);                         // 0x1400b7fb0
        if (!o) continue;                                  // the loop re-tests and returns 0
        if (ExpressProps* pe = expressProps(o->props.get())) return pe;   // 0x140051230, dynamic_cast
    }
}

void expressCopyPrevWizard(Project& p, Operation& op)
{
    if (op.oprFlag != 0) return;                           // 0x1400509d0
    ExpressProps* pe = expressProps(op.props.get());       // 0x1405ef320(self, 0)
    if (!pe || !pe->data) return;                          // 0x140025010: props+0x38
    ExpressProps* prev = prevExpressProps(p, op);          // 0x1413faaa0
    if (!prev) return;
    if (pe->data->loaded) return;                          // 0x1416210c0: data+0xd8
    if (!prev->data) unsupported(opLabel(op) + ": the previous Express operation has no wizard data (props+0x38)");
    // @src 0x1413f71b0  data+0x04 .. +0x18 from the previous operation's data
    data::ExpressData& d = *pe->data;
    const data::ExpressData& s = *prev->data;
    d.formingType = s.formingType;
    d.dieTemperatureType = s.dieTemperatureType;
    d.shapeComplexity = s.shapeComplexity;
    d.simulationAccuracy = s.simulationAccuracy;
    d.workpieceShape = s.workpieceShape;
    d.heatTransType = s.heatTransType;
    // props vt+0x110(0), vt+0x118(model), vt+0x138(0): empty in SfOprExpressProperties
    // (0x141402fe0, 0x1401b20e0); FormingExpress 0x14142b170 / 0x14142b290 / 0x14142b930 and
    // HeatTransferExpress vt+0x138 0x1413edcd0 apply the wizard defaults (0x141401bd0)
    if (dynamic_cast<FormingExpressProps*>(pe) || dynamic_cast<HeatTransferExpressProps*>(pe))
        unsupported(opLabel(op) + " is set up for the first time (OprFlag 0) after another Express operation: "
                    "the wizard defaults applied from its data (props vt+0x110 / vt+0x118 / vt+0x138, "
                    "0x141401bd0) are not ported");
    d.loaded = 1;                                          // 0x141656400(data, 1)
}

// vt+0xb8 of SfOprExpressBase  @src 0x1413fab90
void expressBaseLoadPrologue(mst::OpClass& c, Project& p, Operation& op)
{
    mst::baseLoadPrologue(c, p, op);                        // 0x1405f1bb0
    setTrans2(p, op, false);                                // 0x1405ef6f0(self, 0)
    setTrans1(p, op, true);                                 // 0x1405ef750(self, 1)
    Props* pr = op.props.get();                             // 0x140051230
    if (!pr) return;
    ExpressProps* pe = expressProps(pr);
    if (pe && expressIsHeat(*pe) && op.oprFlag == 0) {
        // 0x141371aa0(data, 2): heatTransType = 2, then props vt+0x60 (0x1413ed900), vt+0x160
        // (0x141402120 -> vt+0x140), vt+0x168, vt+0x58: the wizard defaults of 0x141401bd0
        unsupported(opLabel(op) + " is set up for the first time (OprFlag 0): the Express wizard defaults "
                    "(props vt+0x160 -> vt+0x140 -> 0x141401bd0) are not ported");
    }
    if (op.oprFlag == 0) op.lubSystem = 1;                  // 0x140065790 (OPI+0x70), 0x1400ab6f0: +0x70
}

}  // namespace

// @src 0x1413fd0f0  heat transfer type of the operation's Express data: 4 when its props are not
// heat-transfer Express props (0x1413fd0a0 -> 0x1413fd030, vt+0x100), else data+0x18, -1 without data
int expressHeatTransType(Operation& op)
{
    int r = -1;
    ExpressProps* pe = expressProps(op.props.get());       // 0x1413fd030
    if (!(pe && expressIsHeat(*pe))) {
        r = 4;
    } else if (pe->data) {                                 // 0x1413fa180
        r = pe->data->heatTransType;                       // 0x140010320
    }
    return r;
}

namespace {

// vt+0xc0 of SfOprExpressBase  @src 0x1413facf0
void expressBasePostLoad(Project& p, Operation& op)
{
    expressCopyPrevWizard(p, op);                           // 0x1413fa950
    // 0x1405ef3a0: the object tree (UI) exists
    GuiModel* m = opModel(p, op);                          // 0x14005f770
    if (!m) return;
    ExpressProps* pe = expressProps(op.props.get());       // 0x1405ef320(self, 0)
    if (!pe) return;
    data::ExpressData* d = pe->data.get();                 // 0x140025010
    if (!d) return;
    int ft = d->formingType;                               // 0x14142ff20: +0x04
    int ht = d->heatTransType;                             // 0x140010320: +0x18
    (void)d->simulationAccuracy;                           // 0x1407d3070: +0x10 (only for added objects)
    auto keep = std::make_unique<DB_STEP_DATA>(3);         // 0x140821bf0(stack, 3)
    int rc = 2;
    Operation* prev = prevOp(p, op);                       // 0x140062960
    if (prev) {
        const String path = simCtrlPath(*prev);            // 0x140059290
        if (qt::fileExists(path)) {
            // As in the original, the previous operation's SimCtrl.KEY is read into a second,
            // block-scoped model (stack+0x34e0) that is destroyed right after; `keep` stays as built.
            auto tmp = std::make_unique<DB_STEP_DATA>(3);   // 0x140821bf0
            const char reset = 1;
            const std::int32_t zero = 0;
            const std::string lp = local8(path);
            rc = def::cmd::read_keyword_file(tmp.get(), lp.c_str(), &reset, &zero);   // 0x1408969a0
        }                                                   // 0x140824560 (dtor)
    }
    Cls_1401e7b10* scD = opSimCtl(p, op);                  // 0x14005f7e0
    if (expressIsHeat(*pe)) {                              // vt+0x100
        if (op.oprFlag == 0 || ht == -1) {
            ht = 1;
            const String sub = op.subType;                 // 0x140050da0: OPI+0x128
            if (has(sub, "SfOprAirConvect", qt::Case::Sensitive)) {          // 0x142e82e78
                ht = 1;
            } else if (has(sub, "SfOprHeatResting", qt::Case::Sensitive)) {  // 0x142e82e88
                ht = 2;
            } else if (has(sub, "SfOprHeatDwelling", qt::Case::Sensitive)) { // 0x142e82ea0
                ht = 3;
            } else if (has(sub, "SfOprHeatExpress", qt::Case::Sensitive)) {  // 0x142e82eb8
                ht = 0;
            } else if (!isChainHead(op)) {                 // 0x140050bc0
                if (prev) {
                    ExpressProps* ppe = expressProps(prev->props.get());   // 0x140051230, cast
                    if (!ppe) {
                        if (rc == 0 && keep->sim_ctrl.trans[1] != 0) ht = 3;   // 0x1408dfd80: SC+0x108
                    } else if (!expressIsHeat(*ppe)) {
                        ht = 3;
                    } else if (ppe->data && ppe->data->heatTransType == 1) {
                        ht = 2;
                    }
                }
            } else {
                ht = 1;
            }
            d->heatTransType = ht;                         // 0x1412e2430
        }
    }
    ht = expressHeatTransType(op);                         // 0x1413fd0f0
    if (expressIsHeat(*pe) && ft == 0) {
        ft = 2;
        d->formingType = ft;                               // 0x1413fd9d0
    }
    int n = m->objectCount();                              // 0x140c7c1a0
    if (n < 0) n = 0;
    int cnt = 1;
    if (prev) cnt = objectSlotCount(*prev);                // 0x14005fab0
    if (op.oprFlag == 0 && cnt < 2) {
        cnt = 3;
        if (ht == 1) cnt = 1;
    }
    const String cls = op.classKey;                        // 0x140050d00: OPI+0x120
    if (ht == 0) cnt = 1;
    else if (has(cls, "SfOprHeatExpress", qt::Case::Insensitive)) cnt = 1;   // 0x142e82ed0
    bool block = true;
    if (op.oprFlag != 0) {
        const bool s0 = slotHoldsObject(objId(op, 0));     // 0x14005fb30, 0x140060640
        block = !s0 || (n < 2 && ht != 0 && ht != 1);
    }
    if (block) {
        for (int i = 0; i < cnt; ++i) {
            if (slotHoldsObject(objId(op, i)) && slotObject(p, op, i)) continue;   // 0x140057690
            unsupported(opLabel(op) + ": the GUI adds object " + std::to_string(i + 1) +
                        " with Express defaults (0x140057ba0, 0x141401060 / 0x141401440 of 0x141401bd0)");
        }
        if (op.oprFlag == 0)
            unsupported(opLabel(op) + " is set up for the first time (OprFlag 0): the Express wizard "
                        "defaults (props vt+0x140 / vt+0x158 / vt+0x148 -> 0x141401bd0) are not ported");
        // (OprFlag 0 only: props vt+0x140, 0x110, 0x118, 0x158, 0x148 and the STNOUT / ELMNOD copy
        // from `keep` into scD when prev and rc == 0)
    }
    (void)scD;
    bool merge = ht == 0;
    if (has(cls, "SfOprHeatExpress", qt::Case::Insensitive)) merge = true;   // 0x142e82f30
    if (merge && def::pre::step_count_objects(m->step.get()) > 1)            // 0x140c7c1e0 = 1401cab30
        unsupported(opLabel(op) + ": the GUI merges the objects into object 1 and deletes the others "
                    "(0x140057770, 0x140835800, 0x1400581d0), not ported");
    // OprFlag 0 only (refused above): lubSystem = 1, data+0xc0 (step definition) = 0
    if (op.lubricantId != 0) op.lubricantId = 0;           // 0x1400657f0 / 0x1407b5170: OPI+0x48
}                                                           // 0x140824560 (keep)

// vt+0xb8  @src 0x1413e6360 (HeatTransferExpress) / 0x1413e5aa0 (HeatTransferExpress2D)
void hteLoadPrologue(mst::OpClass& c, Project& p, Operation& op)
{
    expressBaseLoadPrologue(c, p, op);                      // 0x1413fab90
    setTrans2(p, op, false);                                // 0x1405ef6f0(self, 0)
    setTrans1(p, op, true);                                 // 0x1405ef750(self, 1)
}

// vt+0xc0  @src 0x1413e63a0 (HeatTransferExpress) / 0x1413e5ae0 (HeatTransferExpress2D)
void htePostLoad(mst::OpClass&, Project& p, Operation& op, Ctx&, FileEffects&)
{
    expressCopyPrevWizard(p, op);                           // 0x1413fa950
    expressBasePostLoad(p, op);                             // 0x1413facf0
    setTrans1(p, op, true);                                 // 0x1405ef750(self, 1)
    GuiModel* m = opModel(p, op);
    if (!m) return;
    Cls_1401e7b10* sc = m->simCtl();
    if (!sc) return;
    (void)*at<std::int32_t>(sc, 0x240);                     // 0x1408da6d0, result unused
    sc->pdie1 = m->objectCount() == 1 ? 1 : 2;              // SC+0x168
}

// vt+0x228 of HeatTransferExpress / 2D (0x1413e6520 / 0x1413e5c60): in heat-transfer type 0 with
// more than one object slot the GUI deletes objects from the last while more than one is defined
// (0x1400581d0, refused); then 0x1413f9e00 (base 0x1405eea60, 0x1405eeab0, 0x1405eebb0 and the
// column flags 0x1400c3ce0 / 0x1400c4370..) and in 3D 0x1413fcf70, which change only the object
// tree OB+0x18 (UI)
void hteObjectTreeAdd(mst::OpClass&, Project& p, Operation& op)
{
    ExpressProps* pe = expressProps(op.props.get());       // 0x1413fa180
    if (!pe || !pe->data || pe->data->heatTransType != 0) return;   // 0x140010320
    if (objectSlotCount(op) <= 1) return;                   // 0x14005fab0
    GuiModel* m = opModel(p, op);                           // 0x14005f770
    if (!m) return;
    for (int i = m->objectCount(); 0 < i; --i)              // 0x140c7c1a0
        if (1 < def::pre::step_count_objects(m->step.get()))   // 0x140c7c1e0 = 1401cab30
            unsupported(opLabel(op) + ": opening it deletes object slot " + std::to_string(i) +
                        " (0x1400581d0), not ported");
}

// SfOprHeatTransferExpressProperties vt+0x68  @src 0x1413ed7d0  step data (+0x60) <- sim control
bool hteStepDataFromSimCtl(HeatTransferExpressProps& pr, Cls_1401e7b10* sc)
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

// SfOprHeatTransferExpressProperties vt+0x58  @src 0x1413ed890
void htePropsLoad(mst::OpClass&, Project& p, Operation& op)
{
    Props* base = op.props.get();
    if (!base) return;
    auto* pr = dynamic_cast<HeatTransferExpressProps*>(base);
    if (!pr) unsupported(opLabel(op) + ": its properties are not SfOprHeatTransferExpressProperties");
    propsStepsFromSimCtl(p, *pr, nullptr);                 // 0x1405fb720(self, 0)
    propsEnvTempFromSimCtl(p, *pr, nullptr);               // 0x1405fbae0(self, 0)
    propsConvectionFromSimCtl(p, *pr, nullptr);            // 0x1405fbe30(self, 0)
    if (pr->owner) hteStepDataFromSimCtl(*pr, opSimCtl(p, *pr->owner));   // vt+0x68(self, 0x14005f7e0(owner))
}

// ==== SfOprFormingExpressCommon (SfOprFormingExpress, SfOprFormingExpress2D) ========================

// @src 0x141424af0  S+0xa34, +0xa68, +0xa30 = 1 and S+0x1118 = 0 for a 2D model, else 2
void feDimFlags(Project& p, Operation& op)
{
    GuiModel* m = opModel(p, op);
    if (!m || !m->step) return;
    DB_STEP_DATA* s = m->step.get();
    if (!modelIs2D(*m)) {
        *at<std::int32_t>(s, 0xa34) = 2;
        *at<std::int32_t>(s, 0xa68) = 2;
        *at<std::int32_t>(s, 0xa30) = 2;
    } else {
        *at<std::int32_t>(s, 0xa34) = 1;
        *at<std::int32_t>(s, 0xa68) = 1;
        *at<std::int32_t>(s, 0xa30) = 1;
        *at<std::int32_t>(s, 0x1118) = 0;
    }
}

// vt+0xb8  @src 0x14141bc90
void feLoadPrologue(mst::OpClass& c, Project& p, Operation& op)
{
    mst::baseLoadPrologue(c, p, op);                        // 0x1405f1bb0
    setTrans2(p, op, true);                                 // 0x1405ef6f0(self, 1)
    setTrans1(p, op, false);                                // 0x1405ef750(self, 0)
    GuiModel* m = opModel(p, op);
    if (!m) return;
    if (Cls_1401e7b10* sc = m->simCtl()) {
        *at<std::int32_t>(sc, 0x240) = 0;                   // 0x1408da6f0(sc, &0): DTTYPE mode
        if (std::fabs(sc->dsmax) < kEps) sc->dsmax = 0.01;  // 0x141470a78 fabs; 0x1405fd4b0, 0x142203f60
        feDimFlags(p, op);                                  // 0x141424af0
    }
    if (op.oprFlag == 0) op.lubSystem = 1;                  // 0x140065790 (OPI+0x70), 0x1400ab6f0: +0x70
}

// @src 0x1413fc6b0  TRANS 2 = 1 for a FormingExpress class, TRANS 1 by the forming type
void feTransFromType(Project& p, Operation& op)
{
    ExpressProps* pe = expressProps(op.props.get());       // 0x1405ef320(self, 0)
    if (!pe || !pe->data) return;
    if (has(op.classKey, "SfOprFormingExpress", qt::Case::Sensitive))   // 0x142e830e8
        setTrans2(p, op, true);
    const int t = pe->data->formingType;                   // 0x14142ff20
    if (t == 0) setTrans1(p, op, false);
    else if (t == 1 || t == 2) setTrans1(p, op, true);
    else setTrans1(p, op, true);
}

// @src 0x14141f1f0  "assign movement" on the slots of object 2 and of every moving object
void feAssignMovementFlags(Project& p, Operation& op)
{
    GuiModel* m = opModel(p, op);
    if (!m) return;
    const int n = m->objectCount();                         // 0x140c7c1a0
    for (int i = 0; i < n; ++i) {
        LLOBJ_DATA* o = m->objectAt(i + 1);                 // 0x140c7aed0
        if (!o) continue;
        const int no = *at<std::int32_t>(o, 8);
        ObjEntry* slot = objEntryN(op, no);                 // 0x1400612d0
        if (slot && (no == 2 || def::dbio::objcheck::obj_has_movement(o) ||   // 0x14084d4a0 = 140179280
                     *at<std::int32_t>(o, 0x2360) != 0))
            slot->assignMovement = true;                    // 0x14004e0e0: +0x1bf
    }
}

// vt+0xc0  @src 0x14141bda0
void fePostLoad(mst::OpClass&, Project& p, Operation& op, Ctx&, FileEffects&)
{
    ExpressProps* pe = expressProps(op.props.get());       // 0x1405ef320(self, 0)
    if (!pe || !pe->data) return;
    expressCopyPrevWizard(p, op);                           // 0x1413fa950
    (void)pe->data->formingType;                            // 0x14142ff20 (kept for the defaults path)
    // 0x1405ef3a0: the object tree (UI) exists
    GuiModel* m = opModel(p, op);
    if (!m) return;
    if (Cls_1401e7b10* sc = m->simCtl()) {
        (void)*at<std::int32_t>(sc, 0x240);                 // 0x1408da6d0, result unused
        sc->pdie1 = 2;                                      // SC+0x168
    }
    feTransFromType(p, op);                                 // 0x1413fc6b0
    const int n = m->objectCount();                         // 0x140c7c1a0
    if (n < 2)
        unsupported(opLabel(op) + " has fewer than two objects: the GUI adds objects and equipment with "
                    "FormingExpress defaults (0x140057ba0, 0x14141ca90, props vt+0x140 -> 0x141401bd0)");
    feAssignMovementFlags(p, op);                           // 0x14141f1f0
    // 0x1400a5970: equipment manager present. The loop over the object tree (0x1400c3a50 count,
    // 0x1400c3cb0) creates equipment for moving slots without one; the tree (OB+0x18) is empty
    // during the load (cleared by 0x1400c3950 in 0x140051c30, filled by 0x1400547b0 after vt+0xc0),
    // so the loop does not run.
    if (op.lubricantId != 0) op.lubricantId = 0;            // 0x1400657f0 / 0x1407b5170
}

// @src 0x1414264d0  FormingExpress data block <- movement record mv
bool feFromMovement(FormingExpressProps& pr, const std::byte* mv)
{
    data::FormingExpressData& d = *pr.fe;                  // props+0x40
    bool r = false;
    d.name = String::latin1(reinterpret_cast<const char*>(mv));
    const int mt = *at<std::int32_t>(mv, 0x68);
    if (mt == 1) {
        d.type = 0;
        r = true;
    } else if (mt == 0) {
        d.type = 1;
        r = true;
    } else if (mt == 4) {
        d.type = 2;
        d.mechPressType = 0;
        d.totalStroke = *at<double>(mv, 0x318);
        d.cyclesSec = *at<double>(mv, 0x320);
        d.connectingRodLength = *at<double>(mv, 0x328);
        r = true;
    } else if (mt == 2) {
        d.type = 3;
        d.hammerEnergy = *at<double>(mv, 0x1f0);
        d.hammerMass = *at<double>(mv, 0x1e8);
        d.hammerEfficiency = *at<double>(mv, 0x1f8);
        r = true;
    } else if (mt == 3) {
        d.type = 4;
        d.screwEnergy = *at<double>(mv, 0x280);
        d.screwEfficiency = *at<double>(mv, 0x288);
        d.momentOfInertia = *at<double>(mv, 0x290);
        d.leadScrewPitch = *at<double>(mv, 0x298);
        r = true;
    }
    d.dir[0] = *at<double>(mv, 0x50);
    d.dir[1] = *at<double>(mv, 0x58);
    d.dir[2] = *at<double>(mv, 0x60);
    if (r && !pr.tree.empty()) {
        // 0x14142a860 (0x14142a830 clear, 0x14142a740) and 0x14142b000 (0x14142afc0 clear, 0x14142a950)
        for (PropNode& g : pr.tree[0].children) {
            if (g.name.u16() == u"EquipmentType") g.children = data::feEquipmentTypeNodes();
            else if (g.name.u16() == u"EquipmentSpec") g.children = data::feEquipmentSpecNodes(d);
        }
    }
    return r;
}

// SfOprFormingExpressProperties vt+0x58  @src 0x14142a6a0
void fePropsLoad(mst::OpClass&, Project& p, Operation& op)
{
    Props* base = op.props.get();
    if (!base) return;
    auto* pr = dynamic_cast<FormingExpressProps*>(base);
    if (!pr) unsupported(opLabel(op) + ": its properties are not SfOprFormingExpressProperties");
    propsStepsFromSimCtl(p, *pr, nullptr);                 // 0x1405fb720(self, 0)
    propsEnvTempFromSimCtl(p, *pr, nullptr);               // 0x1405fbae0(self, 0)
    propsStepByFromSimCtl(p, *pr, nullptr);                // 0x1405fc180(self, 0)
    propsConvectionFromSimCtl(p, *pr, nullptr);            // 0x1405fbe30(self, 0)
    // @src 0x14142c8b0
    if (!withPropsStep(p, *pr, [](DB_STEP_DATA* s) -> bool { return s != nullptr; })) return; // 0x1405f8df0
    const std::byte* mv = pdieMovement(*propsModel(p, *pr));   // 0x14082f4b0
    if (mv) {
        pr->fe->f10 = 1;                                   // D+0x10
        // 0x140370e90(.., 0) on D+0x20, D+0x80, D+0xa0 (QtProperty enabled, display)
        feFromMovement(*pr, mv);                           // 0x1414264d0
    } else {
        pr->fe->f10 = 0;
        // 0x140370e90(.., 1) on the same nodes (display)
    }
    // 0x1402cea00(MSG): messenger signal (UI)
}

// ==== SfPreSimControl: the "Simulation controls" editor Forming's tree shows after the open ========
// Its fill (vt+0x1b8 0x1417ec030) puts the sim control into the widgets when the item is shown; at
// Save, vt+0xd0 (0x1405f1e20) finds the widget index non-zero and the editor's vt+0x1c0
// (0x1417ec870) applies the widgets back. Nothing changes the model in between, so both run here in
// turn; a widget's value is what its setter leaves (QSpinBox clamps to the range of setupUi
// 0x1417ed690, a line edit keeps the "%.6g <unit>" text).

// @src 0x140ed01b0  SfPreSimControl300::on_m_rbtnDefSolCG_toggled(true).
// Reached on construction/Open only for the selected active Forming editor300:
// 0x140eb26f0 -> 0x140eb5630 -> 0x140eb80c0(page0) -> 0x140ec19f0.
// 0x140ec21b6 checks the CG radio only for SOLMTD==2. False toggles are display-only.
void formingSimControl300Open(Project& p, Operation& op)
{
    GuiModel* m = opModel(p, op);
    if (auto* manager = p.doeIo ? p.doeIo->modelManager() : nullptr) {
        auto* session = manager->ownerSession();
        if (!session || !m || session->modelManagerOwner().get() != manager)
            unsupported("Editor300 factory requires its actual mainwindow/model binding");
        session->beginEditor300Factory(p, op, *m);
        // Native private construction consumes the current palette before
        // QObject/thread, QWidget and application wrappers can return. Later
        // native UI allocation/setup and General/CG fill have not occurred.
        not_ported(0x6504abb0, "QtGui QWidgetPrivate palette inputs for SfPreSimControl300");
    }
    // Explicit legacy fixture/MST-only profile without a bound original
    // mainwindow retains its historical isolated CG callback projection.
    Cls_1401e7b10* sc = m ? m->simCtl() : nullptr;
    if (sc) simcontrol300::normalizeCgAtOpen(*sc);
}

int spinClamp(int v, int lo, int hi) { return v < lo ? lo : (v > hi ? hi : v); }

// "%.6g" text of the step-size line edits, read back by the units parser 0x1402ede00
double viaText(double v)
{
    char buf[64];
    std::snprintf(buf, sizeof buf, "%.6g", v);
    return std::strtod(buf, nullptr);
}

bool formingSimControlApply(mst::OpClass& c, Project& p, Operation& op, FileEffects& fx)
{
    GuiModel* m = opModel(p, op);                           // editor +0x28 (vt+0x198)
    Cls_1401e7b10* sc = m ? m->simCtl() : nullptr;          // 0x140130f90 -> 0x140c7b2d0
    if (!sc) return true;                                   // both bodies skip; 0x140054350 still runs
    // ---- fill 0x1417ec030 ----
    const int unit = *at<std::int32_t>(sc, 0xfc);           // 0x140130f20 -> 0x1408e01e0 (dims of the line edits)
    (void)unit;
    int nstartSpin = sc->nstart;                            // iStack_78 = S+0x160
    bool nstartBox = false;                                 // check box +0x10 (hidden unless shown below)
    const bool head = startsFromKey(op);                    // 0x14005f230; ProcType 5 after a .moproj open
    if (!head && c.writesDimNstart()) {                     // class vt+0x200
        nstartBox = op.nstartGiven != 0;                    // 0x14005a2a0
        if (nstartBox) nstartSpin = op.startStepNo;         // 0x14005a2e0
    }
    if (0 < nstartSpin) nstartSpin = -(sc->nstart + 1);
    nstartSpin = spinClamp(nstartSpin, -999999, 999999);    // spin +0x38
    const int nstepSpin = spinClamp(sc->nstep, 0, 999999);  // spin +0x60 <- S+0x16c
    const int stpincSpin = spinClamp(sc->stpinc, 1, 10000); // spin +0x70 <- S+0x164
    const std::int32_t mode = sc->dttype.u58;               // 0x1408da6d0: S+0x240, button group id
    if (mode != 0 && mode != 1)
        unsupported(opLabel(op) + ": Simulation controls editor with step definition mode " + std::to_string(mode) +
                    " (no button of that id; 0x1417ec030)");
    // PDIE combo +0x80 (0x1417eccf0, the combo is enabled): item 0 = 0, then every object whose type
    // (+0x0c) is not 7, the current one being the object numbered S+0x168
    int pdieItem = 0;
    for (int i = 1, n = m->objectCount(); i <= n; ++i) {    // 0x140c7c1a0, 0x140c7ae80
        LLOBJ_DATA* o = m->object(i);
        if (!o || *at<std::int32_t>(o, 0xc) == 7) continue; // 0x140834fa0
        if (*at<std::int32_t>(o, 8) == sc->pdie1) pdieItem = *at<std::int32_t>(o, 8);
    }
    // ---- apply 0x1417ec870 ----
    sc->nstart = nstartSpin;                                // S+0x160
    if (!head) {
        if (!c.writesDimNstart()) {
            op.nstartGiven = 0;                             // 0x14005a2c0(op, 0)
        } else {
            op.nstartGiven = nstartBox ? 1 : 0;
            if (nstartBox) op.startStepNo = nstartSpin;     // 0x14005a350
        }
    } else {
        op.nstartGiven = 1;
        op.startStepNo = nstartSpin;
    }
    sc->nstep = nstepSpin;                                  // 0x1416f0c10: S+0x16c
    sc->stpinc = stpincSpin;                                // 0x1416f0c60: S+0x164
    sc->pdie1 = pdieItem;                                   // 0x1416f0d90: S+0x168 <- item data
    // 0x1416f0d40 -> 0x1416f0cb0(S, checkedId, value parsed from the matching line edit)
    if (mode == 0) {
        sc->dsmax = viaText(sc->dsmax);                     // 0x1405fd4b0: S+0x1e0
        sc->dtmax1 = 0.0;                                   // S+0x1d0
    } else {
        sc->dtmax1 = viaText(sc->dtmax1);
        sc->dsmax = 0.0;
    }
    // 0x140131110: the editor's slot index (vt+0x1a0 0x140130d90: +0x40, 0 -> 1) > 0 -> 0x1401311a0:
    // 0x140060100(op, 1), then 0x140055220 (manager dataChanged, UI)
    refreshSlotItems(p, op, 1, fx);
    // 0x140131230: class vt+0x260 0x1405efe40 on the object tree model (tree item display, UI)
    return true;
}

}  // namespace

void installLoaderSlotsL3()
{
    mst::g_loaderSlotsL3.formingSimControl300Open = &formingSimControl300Open;
    mst::g_loaderSlotsL3.formingSimControlApply = &formingSimControlApply;
    mst::g_loaderSlotsL3.formingLoadPrologue = &formingLoadPrologue;
    mst::g_loaderSlotsL3.formingPostLoad = &formingPostLoad;
    mst::g_loaderSlotsL3.formingPropsLoad = &formingPropsLoad;
    mst::g_loaderSlotsL3.feLoadPrologue = &feLoadPrologue;
    mst::g_loaderSlotsL3.fePostLoad = &fePostLoad;
    mst::g_loaderSlotsL3.fePropsLoad = &fePropsLoad;
    mst::g_loaderSlotsL3.hteLoadPrologue = &hteLoadPrologue;
    mst::g_loaderSlotsL3.htePostLoad = &htePostLoad;
    mst::g_loaderSlotsL3.htePropsLoad = &htePropsLoad;
    mst::g_loaderSlotsL3.hteObjectTreeAdd = &hteObjectTreeAdd;
}

}  // namespace m2m::engine
