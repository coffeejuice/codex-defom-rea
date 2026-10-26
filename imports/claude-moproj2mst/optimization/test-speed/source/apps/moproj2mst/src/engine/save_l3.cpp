// MDT L3S: save slots of the Express classes (SfOprExpressBase: SfOprHeatTransferExpress(2D);
// SfOprFormingExpressCommon: SfOprFormingExpress(2D)) on File > Save step 2, and their contact
// defaults for the system-lubricant rebuild (docs/re/moproj2mst/mdt/L3/spec_save.md; decompiles
// gui_mo/s1/mdt/L3/s0.c .. s2.c). save_current.cpp calls them by class key where the original calls
// the class's vtable slot. Forming(2D) keeps every base save slot (vt+0x80 empty).
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <vector>

#include <defcmd/cnvt3d.hpp>
#include <defdbio/objcheck/objcheck.hpp>
#include <defmodel/layout/cls_1401e7b10.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/llobj_data.hpp>

#include "../ops/express.hpp"
#include "engine_internal.hpp"
#include "m2m/data/express.hpp"
#include "m2m/data/heattransfer.hpp"
#include "m2m/engine.hpp"
#include "m2m/model.hpp"
#include "m2m/mst/file_device.hpp"
#include "m2m/props.hpp"
#include "m2m/qt4path.hpp"

namespace m2m::engine {

using def::model::Cls_1401e7b10;
using qt::L;

namespace {

constexpr double kEps = 1e-06;             // 0x142157420

// @src 0x1413fd0a0  the current operation's props are heat-transfer Express props (vt+0x100:
// 0x1413e5ef0 true; FormingExpress 0x1400707e0 false)
bool heatExpressProps(const Operation& op)
{
    return dynamic_cast<const HeatTransferExpressProps*>(op.props.get()) != nullptr;
}

// 140835030 (= DEF_PRE 140160fa0): object kind is neither 0 nor 3
bool kindNot0or3(const LLOBJ_DATA* o)
{
    const int k = *at<std::int32_t>(o, 0xc);
    return !(k == 0 || k == 3);
}

// heat transfer type 0, or a class name with "SfOprHeatExpress" (Qt::CaseInsensitive)
bool heatExpressMode(Operation& op, const char* lit)
{
    bool r = expressHeatTransType(op) == 0;                // 0x1413fd0f0
    if (qt::contains(op.classKey, L(lit), qt::Case::Insensitive)) r = true;   // 0x140050d00
    return r;
}

}  // namespace

// @src 0x1413fd4c0
void expressContactDefaults(Project&, Operation& op, double* frc, double* ihtc, bool flag)
{
    Props* pr = op.props.get();                             // 0x1405ef320(self, 0)
    if (!pr) return;
    *frc = propsFriction(*pr);                              // 0x1405fc890
    *ihtc = propsIhtc(*pr);                                 // 0x1405fc8e0
    if (!(std::fabs(*ihtc) < kEps)) return;                 // 0x141470a78 fabs
    const int unit = op.project ? op.project->unit : 0;     // 0x140054ff0 (0x1400a2180)
    const String sub = op.subType;                          // 0x140050da0: OPI+0x128
    if (!heatExpressProps(op)) {
        if (unit == 2) *ihtc = !flag ? 0.004 : 0.002;       // 0x142498288 / 0x142265008
        else *ihtc = !flag ? 11.0 : 5.0;                    // 0x142498280 / 0x1421d3c60
    }
    if (sub == L("Rest on die")) {                          // 0x142e83178
        if (unit == 2) *ihtc = !flag ? 0.0003 : 0.0002;     // 0x142498298 / 0x1424982a0
        else *ihtc = !flag ? 1.0 : 0.5;                     // 0x1421249e0 / 0x142101640
    } else if (sub == L("Dwell on die")) {                  // 0x142e83188
        if (unit == 2) *ihtc = !flag ? 0.0006 : 0.0003;     // 0x142498290 / 0x142498298
        else *ihtc = !flag ? 2.0 : 1.0;                     // 0x142101520 / 0x1421249e0
    } else {
        if (unit == 2) *ihtc = !flag ? 0.0003 : 0.0002;
        else *ihtc = !flag ? 1.0 : 0.5;
    }
}

// @src 0x14141ef60
std::vector<ContactPair> feDefaultPairs(Project& p, Operation& op)
{
    std::vector<ContactPair> out;
    GuiModel* m = opModel(p, op);                          // 0x14005f770
    if (!m || !m->step) return out;
    DB_STEP_DATA* s = m->step.get();
    s->inter_obj.clear();                                  // 0x1408e6cc0 = 1401fade0
    const int n = m->objectCount();                        // 0x140830180
    for (int i = 1; i <= n; ++i) {
        LLOBJ_DATA* o = m->objectAt(i);                    // 0x140c7aed0
        if (!o || *at<std::int32_t>(o, 0xc) == 7) continue;   // 0x140834fa0
        if (*at<std::int32_t>(s, 0x1c) == 3 && i == 1) continue;   // 0x14035b400: 3D
        ContactPair c;                                     // 0x1400657a0
        c.master = i;
        c.slave = 1;
        c.lub = 0;
        c.frc = propsFriction(*op.props);                  // 0x1405fc890 (props of current)
        c.ihtc = propsIhtc(*op.props);                     // 0x1405fc8e0
        out.push_back(c);                                  // 0x1400ad3b0
    }
    return out;
}

// @src 0x1405fcad0  HeatingTime (B+0x18) in the props' unit system, category 0 (time)
double propsHeatingTime(const Props& pr)
{
    bool ok = false;
    const String t = pr.common ? pr.common->heatingTime : String();
    return evalUnitString(t, 0, propsUnit(pr), &ok);       // 0x1402ede00(0x143d76368, 0, 0x1405f8dc0)
}

// SfOprExpressBase vt+0x68  @src 0x1413f8f60: in heat-transfer type 0 every object that is not
// kind 0 or 3 gets the temperature window +0x1ef8/+0x1f00 (object +0x1ea0 block) centred on ENVTMP
// (SC+0x3d0), then the base vt+0x68 0x1405ef510 (called by save_current.cpp)
void expressSaveOperation(Project& p, Operation& op)
{
    if (!heatExpressMode(op, "SfOprHeatExpress")) return;   // 0x142e82b78
    GuiModel* m = opModel(p, op);
    if (!m) return;
    Cls_1401e7b10* sc = m->simCtl();                        // 0x140c7b2d0
    if (!sc) return;
    const int n = m->objectCount();                         // 0x140c7c1a0
    for (int i = 0; i < n; ++i) {
        LLOBJ_DATA* o = m->objectAt(i + 1);                 // 0x140c7aed0
        if (!o || !kindNot0or3(o)) continue;
        const double d = std::fabs(*at<double>(o, 0x1f00) - *at<double>(o, 0x1ef8)) / 2.0;   // 0x142101520
        *at<double>(o, 0x1f00) = *at<double>(sc, 0x3d0) + d;  // block+0x60
        *at<double>(o, 0x1ef8) = *at<double>(sc, 0x3d0) - d;  // block+0x58
    }
}

// SfOprHeatTransferExpress vt+0xd8  @src 0x1413e6460 (3D) / 0x1413e5ba0 (2D): TMAX = (TNOW 1 when
// TNOW 3 is 0, else TNOW 2) + HeatingTime (0x1408cb0c0); the base 0x140054540 is not called
void hteSaveTmax(Project& p, Operation& op)
{
    if (!op.props) return;
    const double t = propsHeatingTime(*op.props);          // 0x1405fcad0
    GuiModel* m = opModel(p, op);                          // 0x14005f7a0 (the original dereferences it)
    if (!m || !m->step) return;
    Cls_1401e7b10& sc = m->step->sim_ctrl;
    if (sc.tnow3 == 0) sc.tmax = sc.tnow1 + t;             // S+0xda0, S+0x8c8 = S+0xd90 + t
    else sc.tmax = sc.tnow2 + t;                           // S+0xd98
}

// SfOprExpressBase vt+0xe8  @src 0x1413f91f0: base 0x1405f2350 (0x140057370), then in heat-transfer
// type 0 the contact block's "generate" byte is cleared and INICTC set for the objects
bool expressSyncOperation(Project& p, Operation& op, FileEffects& fx)
{
    const bool r = syncOperationFiles(p, op, fx);          // 0x1405f2350 -> 0x140057370
    if (!heatExpressMode(op, "SfOprHeatExpress")) return r;   // 0x142e82b90
    GuiModel* m = opModel(p, op);
    if (!m) return r;
    ContactBlock& icb = op.icb;                            // 0x140065790: OPI+0x70
    icb.gen = 0;                                           // 0x1400ab7e0: +0x65
    for (int i = 0; i < m->objectCount(); ++i) {           // 0x140c7c1a0 each round
        LLOBJ_DATA* o = m->objectAt(i);                    // 0x140c7aed0(m, i): position 0 is the first
        if (*at<std::int32_t>(o, 0xc) != 7)                // 0x140834fa0
            icb.flag[static_cast<std::size_t>(*at<std::int32_t>(o, 8) - 1)] = 1;   // 0x140065270
    }
    return r;
}

namespace {

// @src 0x1413f8380  approximate die stroke (data+0x38), times 1.2 unless "use exact" (data+0x40)
double expressDieStroke(const data::ExpressData& d)
{
    if (!d.dieStrokeUseExact) return d.approxDieStroke * 1.2;   // 0x1413f8ee0, 0x14004ef70, 0x1422d3a88
    return d.approxDieStroke;
}

// @src 0x1416f0cb0  DTTYPE mode `mode`; mode 0: DSMAX = v, DTMAX 1 = 0; else DTMAX 1 = v, DSMAX = 0
bool setStepBy(Cls_1401e7b10* sc, int mode, double v)
{
    if (!sc) return false;
    *at<std::int32_t>(sc, 0x240) = mode;                    // 0x1408da6f0
    if (mode == 0) {
        sc->dsmax = v;                                      // 0x1405fd4b0
        sc->dtmax1 = 0.0;                                   // +0x1d0
    } else {
        sc->dtmax1 = v;
        sc->dsmax = 0.0;
    }
    return true;
}

// @src 0x1413f7110  DSMAX per step from the die stroke and NSTEP; DTTYPE mode set to 0 first;
// -1 without a sim control
double expressDsmaxFromStroke(const data::ExpressData& d, Cls_1401e7b10* sc)
{
    double r = -1.0;                                        // 0x1421d3c90
    if (!sc) return r;
    if (*at<std::int32_t>(sc, 0x240) != 0) *at<std::int32_t>(sc, 0x240) = 0;   // 0x1408da6d0 / 0x1408da6f0
    r = 0.0;
    if (0 < sc->nstep) r = expressDieStroke(d) / static_cast<double>(sc->nstep);   // SC+0x16c
    return r;
}

// @src 0x1413f6b60  wizard step definition "System" (data+0xc0 == 0): DSMAX from the die stroke
// (0.001 when the stroke gives none); the messages (tr texts, messenger signal 0x1402cf3c0) are UI
bool expressStepFromWizard(const data::ExpressData& d, Cls_1401e7b10* sc)
{
    if (!sc) return false;
    if (d.stepDefinition != 0) return true;                // 0x1413f6b20
    const int mode = *at<std::int32_t>(sc, 0x240);          // 0x1408da6d0
    const double dsmax = sc->dsmax;
    const double v = expressDsmaxFromStroke(d, sc);         // 0x1413f7110
    if (v <= 0.0) {                                         // 0x1421ad1d0
        setStepBy(sc, 0, 0.001);                            // 0x1421b5050
        (void)expressDieStroke(d);                          // picks one of two messages
        return false;
    }
    if (mode != 0 || 1e-12 < std::fabs(dsmax - v))         // 0x142248190
        setStepBy(sc, 0, v);
    return true;
}

// @src 0x141424e60  TRGVOL of a 3D workpiece updated from its volume (0x140891530 = 14021d630); the
// "target volume" status-bar message (0x1402d0bd0) is UI
bool feUpdateTargetVolume(LLOBJ_DATA* o)
{
    if (!o) return false;
    if (*at<std::int32_t>(o, 0x39c0) == 2) return false;   // 0x140065800
    return def::cmd::cnvt3d::update_target_volume(o) != 0;
}

}  // namespace

// SfOprFormingExpressCommon vt+0xd8  @src 0x14141cf90
void feSaveSimNames(Project& p, Operation& op)
{
    GuiModel* m = opModel(p, op);                           // 0x14005f770
    if (!m || !m->simCtl()) return;                         // 0x140c7b2d0
    auto* pe = dynamic_cast<FormingExpressProps*>(op.props.get());   // __RTDynamicCast(.., 0x1439e12d0)
    if (!pe) return;                                        // the main window (0x1402c8090) exists
    if (ObjEntry* e = objEntryN(op, 1)) {                   // 0x1400612d0
        auto* b = reinterpret_cast<std::uint8_t*>(&e->sv.urzSet);   // 0x14004df00 (+0x30), 0x1412e2450:
        *b = 1;                                             // byte +0x40 of the state variables
    }
    storeSimNames(p, op);                                   // 0x1405f22e0 (ProcType 5) -> 0x140054540
    Cls_1401e7b10* sc = m->simCtl();
    sc->pdie1 = 2;                                          // SC+0x168
    const std::byte* mv = nullptr;                          // 0x14082f4b0: PDIE movement
    if (LLOBJ_DATA* o = m->object(sc->pdie1)) mv = at<std::byte>(o, 0x2160);
    if (pe->data) expressStepFromWizard(*pe->data, sc);    // 0x140025010, 0x1413f6b60
    if (mv && *at<std::int32_t>(mv, 0x68) == 4)
        throw Error(M2M_E_UNSUPPORTED, "save current operation: FormingExpress mechanical press stroke "
                                       "(0x140355ab0 / 0x140869480) is not ported yet");
    if (m->step && *at<std::int32_t>(m->step.get(), 0x1c) == 2)   // 0x140c7c0e0
        throw Error(M2M_E_UNSUPPORTED, "save current operation: FormingExpress 2D (0x141424be0) is not ported yet");
    // 0x140c9c490(main window): the first object of the main window's current model (vt+0x268; the
    // operation being saved, inferred) whose kind is not 7, 0 or 3
    LLOBJ_DATA* w = nullptr;
    for (int i = 0; i < m->objectCount(); ++i) {
        LLOBJ_DATA* o = m->objectAt(i + 1);
        if (o && *at<std::int32_t>(o, 0xc) != 7 && kindNot0or3(o)) {
            w = o;
            break;
        }
    }
    if (w && *at<std::int32_t>(w, 0x39c0) == 3 && *at<std::int32_t>(w, 0xc) != 7 &&   // 0x1400c3530
        *at<std::int32_t>(w, 0xc) != 5 && objHasMesh(w)) {                             // 0x1408350b0, 0x140834c80
        const double v = def::dbio::objcheck::obj_volume(w, 1);   // 0x1408375d0 = 140163540
        const double d = std::fabs(v - *at<double>(w, 0x2090));   // TRGVOL
        if (v * 0.006 < d) feUpdateTargetVolume(w);         // 0x1427aaab8, 0x141424e60
    }
}

// SfOprHeatTransferExpressProperties vt+0x70  @src 0x1413ed710  step data (+0x60) -> sim control
bool hteStepDataToSimCtl(const HeatTransferExpressProps& pr, Cls_1401e7b10* sc)
{
    if (!sc) return false;
    const data::HeatTransferStepData& d = *pr.step;
    sc->stpdef = d.stepDef + 1;                            // +0x1c8
    sc->dtmax1 = d.dtMax;                                  // +0x1d0
    sc->dtpmax1 = d.dTempMax;                              // +0x270
    sc->dtpmax2_5[0] = d.dTempLmt0;                        // +0x2a0
    sc->dtpmax2_5[1] = d.dTempLmt1;                        // +0x2a8
    sc->stpinc = d.stpInc;                                 // +0x164
    return true;
}

}  // namespace m2m::engine

namespace m2m::engine {

// @src 0x141402030 -> 0x1413f73a0 -> 0x1413f74a0  SfOprExpressProperties vt+0xb8 (Save step 4)
bool expressPropsSave(Project& p, Operation& op, FileEffects& fx, Ctx& ctx)
{
    (void)p;
    auto* ep = dynamic_cast<ExpressProps*>(op.props.get());
    if (!ep) return false;
    if (!ep->data) return true;                                     // props+0x38 null: nothing
    const data::ExpressData& d = *ep->data;
    // 0x1413f73a0: data+0xd0 (the owner props) -> its OPI (0x1405f8d90) -> task folder
    // (0x140050df0); -1 and nothing written without one
    if (!ep->owner) return true;
    const String dir = taskDirAbs(*ep->owner);
    if (dir.isNull()) return true;
    // 0x1413f7230 builds "Forming.FWZ" too; that copy is unused
    const String path = qt::convertSeparators(dir + qt::kSep + qt::L("Forming.FWZ"));
    // QFile(path).open(WriteOnly): no text translation, '\n' as written; QTextStream with the
    // locale codec, endl = '\n'
    std::unique_ptr<mst::FileDevice> file;
    if (ctx.nativePropertyFiles) {
        file = std::make_unique<mst::FileDevice>(path);
        // This function's bool identifies the handled property class. The
        // original writer's false is ignored by its vt+b8 caller/Save loop.
        if (!file->opened()) return true;
    }
    mst::Stream stream(ctx.codepage, file.get());
    auto line = [&stream](const auto& value) { stream << value; stream.endl(); };
    auto num = [&line](int v) { line(v); line(""); };
    auto dbl = [&line](double v) { line(v); line(""); };
    if (d.geometryType2DSet) {                                      // +0xbc
        line("WZD_FORM_2D_GEOMETRY_TYPE");
        line("  * GEOTYP_AXISYMMETRIC        = 1 ");
        line("  * GEOTYP_PLANE_STRAIN        = 2 ");
        line("  * GEOTYP_ROTSYMMETRIC        = 3 ");
        line("  * GEOTYP_PLANE_STRESS        = 4 ");
        num(d.geometryType2D);                                      // +0xb8
    }
    line("WZD_FORM_FORMING_TYPE");
    line("  * COLD FORMING        = 0 ");
    line("  * WARM FORMING        = 1 ");
    line("  * HOT FORMING         = 2 ");
    num(d.formingType);                                             // +0x04
    line("WZD_FORM_HEATTRANS_TYPE");
    line("  * UNDEFINED       = -1 ");
    line("  * HEATING         = 0 ");
    line("  * TRANSFER        = 1 ");
    line("  * REST ON DIE     = 2 ");
    line("  * DWELL ON DIE    = 3 ");
    num(d.heatTransType);                                           // +0x18
    line("WZD_FORM_DIE_TEMPERATURE_TYPE");
    line("  * TEMPERATURE IN DIES         = 0 ");
    line("  * CONSTANT DIE TEMPERATURE    = 1 ");
    line("  * IGNORE DIE TEMPERATRUE      = 2 ");
    num(d.dieTemperatureType);                                      // +0x08
    line("WZD_FORM_SHAPE_COMPLEXITY");
    line("  * SIMPLE      = 0 ");
    line("  * MODERATE    = 50 ");
    line("  * COMPLEX     = 100 ");
    num(d.shapeComplexity);                                         // +0x0c
    line("WZD_FORM_SIMULATION_ACCURACY");
    line("  * FAST        = 0 ");
    line("  * MODERATE    = 50 ");
    line("  * ACCURATE    = 100 ");
    num(d.simulationAccuracy);                                      // +0x10
    line("WZD_FORM_WORKPIECE_SHAPE");
    line("  * WHOLE_PART      = 0 ");
    line("  * SYMMETRY        = 1 ");
    num(d.workpieceShape);                                          // +0x14
    line("WZD_FORM_REV_ANGLE");
    dbl(d.revAngle);                                                // +0x20
    line("WZD_FORM_BUILT_IN_FLOWNET");
    dbl(d.builtInFlownet);                                          // +0x28
    line("WZD_FORM_CONTACT_TOLRANCE");
    dbl(d.contactTolerance);                                        // +0x30
    line("WZD_FORM_APPROXIMATE_DIE_STROKE");
    dbl(d.approxDieStroke);                                         // +0x38
    line("WZD_FORM_DIE_STROKE_USE_EXACT");
    num(d.dieStrokeUseExact);                                       // +0x40 byte as int
    line("WZD_FORM_WORKPIECE_GEO_CHANGED");
    num(d.workpieceGeoChanged);                                     // +0x41
    line("WZD_FORM_IMPORT_MAT_CAT_WORKPIECE");
    line(d.importMatCatWorkpiece);                                  // +0xa8
    line("");
    line("WZD_FORM_IMPORT_MAT_CAT_DIE");
    line(d.importMatCatDie);                                        // +0xb0
    line("");
    line("WZD_FORM_STEP_DEFINITION");
    line("  * SYSTEM      = 0 ");
    line("  * USER        = 1 ");
    num(d.stepDefinition);                                          // +0xc0
    line("WZD_MESH_TYPE_DEFINITION");
    line("  * ObjectNo, Mesh definition type");
    line(static_cast<int>(d.meshType.size()));                      // QMap::keys().size()
    for (const auto& [obj, type] : d.meshType) {                    // keys ascending, value(key)
        line(obj);
        line(type);
    }
    line("");
    line("DONE");
    line("");
    if (file) file->close();                                       // no status query in 1413f74a0
    else fx.writes.push_back({path, stream.bytes()});
    return true;
}

}  // namespace m2m::engine
