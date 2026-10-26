// SfOprMultiBlow(2D/Common) MST writer (P14): +0x280 0x140602810 -> 0x1406028b0 and its block
// writers. Spec s1-04 7; decompiles s1-04/d1c.c, d3c.c; literal bytes checked in the exe copy.
//
// Not ported (no effect on the MST, s1-04 8): the DEF_PRE temporaries DB_STEP_DATA(3),
// Cls_1401e7b10 and Cls_14018b220 of the driver and the props vt+0x70 fill; only the movement
// direction the writer reads (mv+0x50..+0x60, overwritten from MBD by 0x14060def0) is kept.
// The operation model 0x14005f770 (seam hasEngineModel, P15): 0x14005f820 creates it through the
// class's vt+0x1b0 when the operation has none yet, so on Save it is not null.
#include <cmath>
#include <memory>

#include "m2m/data/multiblow.hpp"
#include "m2m/engine_multiblow.hpp"
#include "m2m/mst/emit.hpp"
#include "m2m/mst/simcount.hpp"
#include "m2m/mst/writer.hpp"
#include "m2m/data/common.hpp"
#include "m2m/props_base.hpp"
#include "m2m/qt4path.hpp"

#include <defrt/stdlib/libm.hpp>

namespace m2m {
// src/ops/multiblow_process.cpp: SfMultiBlowProcess fill 0x140636f10 then apply 0x1406370a0
bool multiBlowProcessUpdate(MultiBlowProps& pr, const Operation& op);
}

namespace m2m::mst {

namespace {

using data::MultiBlowData;

constexpr const char* T = "\t";   // every separator literal in 0x1424acbd4..0x1424b0af0 is "\t"
constexpr double kGap = 0.0001;   // 0x1421ce788

// @src 0x141470fec  static-CRT atan2 (byte twin of DEF_PRE 0x140485d6c)
double crtAtan2(double y, double x)
{
    return def::rt::libm::crt_atan2(y, x);
}

// QString::arg(double, 0, 'g', -1, ' ') of the rotation texts (Qt 4.8); tr() has no translator.
String argG(const String& fmt, double v)
{
    return qt::arg(fmt, v, 0, 'g', -1, u' ');
}

// @src 0x140609b10
bool rotValid(const data::BlowRotation& r)
{
    if (r.angle == 0.0) return false;
    if (r.a[0] == 0.0 && r.a[1] == 0.0 && r.a[2] == 0.0) return false;
    return true;
}

// @src 0x140609b80
String rotError(const data::BlowRotation& r)
{
    String s;
    if (r.angle == 0.0) s += qt::L("Rotation angle is zero");                     // 0x1424b9700
    if (r.a[0] == 0.0 && r.a[1] == 0.0 && r.a[2] == 0.0)
        s += qt::L(" ") + qt::L("Rotation axis is not defined");                  // 0x1424b5e64, 0x1424b9718
    return s;
}

// @src 0x140609690
String rotAxisText(const data::BlowRotation& r)
{
    const double x = r.a[0], y = r.a[1], z = r.a[2];
    if (x == 0.0 && y == 0.0 && z == 0.0) return qt::L("undefined axis");          // 0x1424b96b0
    if (y == 0.0 && z == 0.0) return qt::L(x <= 0.0 ? "-X axis" : "+X axis");      // 0x1424b96c8 / c0
    if (x == 0.0 && z == 0.0) return qt::L(y <= 0.0 ? "-Y axis" : "+Y axis");      // 0x1424b96d8 / d0
    if (x == 0.0 && y == 0.0) return qt::L(z <= 0.0 ? "-Z axis" : "+Z axis");      // 0x1424b96e8 / e0
    return argG(argG(argG(qt::L("axis (%1,%2,%3)"), x), y), z);                    // 0x1424b96f0
}

// @src 0x1406091d0
String rotText(const data::BlowRotation& r)
{
    if (r.angle == 0.0) return qt::L("undefined");                                // 0x1424b9668
    String s;
    s += argG(qt::L("Angle %1"), r.angle);                                         // 0x1424b9678
    s += qt::L(",");                                                               // 0x1424b5e0c
    s += rotAxisText(r);
    s += qt::L(",");                                                               // 0x1424b5e4c
    if (r.selfCenter == 0)
        s += argG(argG(argG(qt::L("center (%1,%2,%3)"), r.c[0]), r.c[1]), r.c[2]); // 0x1424b9698
    else
        s += qt::L("Self Center");                                                 // 0x1424b9688
    return s;
}

class MultiBlowClass final : public OpClass {
public:
    MultiBlowClass(String key, bool is2D) : OpClass(std::move(key)), is2D_(is2D)
    {
        // @src 0x140601da0 -> 0x140601e40
        wp_ = 1;
        topDie_ = 2;
        botDie_ = 3;
        scale_ = 1.0;
        away_[0] = away_[1] = away_[2] = 0.0;
        // class init vt+0x180: 0x1405fd8a0(info, flags); the rest of the init is GUI only
        dimFlags = is2D ? 0x29 : 0x2a;   // @src 0x140600a50 (SfOprMultiBlow2D) / 0x140601480 (SfOprMultiBlow)
    }

    // vt+0x280  @src 0x140602810
    void writeMst(W& w, Project& prj, const String& db, int* sim, Operation* op, int dbreadArg,
                  int onlySim) override
    {
        auto* mp = dynamic_cast<MultiBlowProps*>(propsOf(op));   // 0x140608ea0, TD 0x1437751e8
        if (mp) {
            // Not in the original: it silently keeps the defaults when the saved root name is not the
            // process's template name (plan 8). The port writes the same bytes and says so.
            const String saved = data::savedMultiBlowRoot(prj, op->id);
            if (!saved.isNull() && !mp->tree.empty() && !(saved == mp->tree[0].name))
                w.ctx.warn("MultiBlow operation \"" + op->name.utf8() + "\": its saved properties are under \"" +
                           saved.utf8() + "\" but a fresh DEF_GUI_MO names the root \"" + mp->tree[0].name.utf8() +
                           "\" (the first MultiBlow operation it creates), so it ignores them and writes the "
                           "defaults; the MST written here does the same");
            drive(w, prj, db, sim, op, *mp, dbreadArg, onlySim);
        }
    }

    // vt+0x298  @src 0x140606c10
    void simList(int pos, int* counter, Operation* op, std::vector<SimEntry>& out) override
    {
        simListMultiBlow(*this, pos, counter, op, out);
    }

    // vt+0x98: SfOprMultiBlowProperties (0x14060b500) with the class's default tree
    std::shared_ptr<Props> makeProps(int guiUnit) const override
    {
        auto p = std::make_shared<MultiBlowProps>();
        data::initMultiBlowProps(*p, is2D_ ? 2 : 3, guiUnit);
        p->tree = data::multiBlowPropTree(is2D_ ? 2 : 3, *p->data);   // 0x140612150
        data::mbSetNumHeats(*p->data, 1);                       // 0x140613f40 -> 0x1406140c0(this, 1)
        // 0x140613640: Interobject children from the base adders
        p->common = std::make_shared<data::CommonBlock>();
        for (PropNode& g : p->tree[0].children) {
            if (g.name.u16() != u"Interobject") continue;
            base::addFriction(*p, g, guiUnit, 0.0, 0.0);                          // 0x1405fad10
            base::addHeatTransCoef(*p, g, guiUnit, qt::L("0.0 Btu/sec/in^2/F"),   // 0x1405fa7d0, 0x1424ba0b0
                                   qt::L("0.0 N/sec/mm/C"));                      // 0x1424ba0a0
        }
        // 0x140612150 continued: the base groups
        base::addStepCounts(*p, false, false, 100, 10, qt::L("0.0 sec"));      // 0x1405f9eb0, 0x1424ba048
        base::addEnvTemp(*p, guiUnit, qt::L("68.00 F"), qt::L("20.00 C"));     // 0x1405fa560
        base::addConvection(*p, guiUnit, qt::L("0.0 Btu/sec/in^2/F"),          // 0x1405faaa0
                            qt::L("0.0 N/sec/mm/C"));
        base::addStepBy(*p, guiUnit, 0, qt::L("0.0 in"), qt::L("0.0 mm"),       // 0x1405fb020
                        qt::L("0.0 sec"));
        return p;
    }

    // vt+0x88  @src 0x140608df0
    String displayName() const override { return qt::L("Multi Blow"); }   // 0x1424b0cd0

    // Operation loader slots (MDT L4, src/engine/multiblow_load.cpp)
    void loadPrologue(Project& p, Operation& op) override { engine::multiBlowLoadPrologue(*this, p, op); }
    void loadClassData(Project& p, Operation& op) override { engine::multiBlowLoadClassData(*this, p, op); }
    void postLoad(Project& p, Operation& op, Ctx&, engine::FileEffects& fx) override
    {
        engine::multiBlowPostLoad(*this, p, op, fx, &MultiBlowClass::v120);
    }
    void propsLoad(Project& p, Operation& op) override { engine::multiBlowPropsLoad(*this, p, op); }
    // Save current operation slot vt+0xd8 (MDT L4S); vt+0x80 0x140602770 is empty
    void saveNames(Project& p, Operation& op, engine::FileEffects& fx) override
    {
        engine::multiBlowSaveNames(*this, p, op, fx, &MultiBlowClass::v120);
    }
    // vt+0xa8  @src 0x1406019f0 (SfOprMultiBlow) / 0x140600fc0 (SfOprMultiBlow2D): vt+0x228, then
    // the tree widget columns, model and header (0x140d3d730 .. 0x140d3d8c0, UI) (MDT L4S)
    bool objectTreeFill(Project& p, Operation& op, engine::FileEffects& fx) override
    {
        objectTreeAdd(p, op, fx);
        processShown_ = &op;   // the tree's "Process" item (SfMultiBlowProcess): the editor the GUI shows
        return false;
    }
    // vt+0x228  @src 0x140601960 (SfOprMultiBlow) / 0x140600f30 (SfOprMultiBlow2D): tree states
    // 0x1405eea60, 0x1405eeab0, 0x1405eebb0(1, 1, 1) (UI); with the tree OB+0x18 (always in the GUI)
    // 0x1400c4670(tree, 2) (UI) and vt+0x230 0x140607d10, the equipment sync (packet S) (MDT L4S)
    void objectTreeAdd(Project& p, Operation& op, engine::FileEffects& fx) override
    {
        if (!g_equipmentSync) loaderSlotsMissing();
        g_equipmentSync(p, op, true, fx);
    }

    // vt+0xd0 with the Process editor shown (index != 0): signalUpdateDataInWidgetArea -> its
    // vt+0x1c0 0x1406370a0 (MDT L4); the caller then runs 0x140054350
    bool updateDataInWidgetArea(Project&, Operation& op, engine::FileEffects&) override
    {
        if (processShown_ != &op) return false;
        auto* mp = dynamic_cast<MultiBlowProps*>(op.props.get());
        if (!mp) throw Error(M2M_E_UNSUPPORTED, "save: SfMultiBlowProcess without MultiBlow properties");
        return multiBlowProcessUpdate(*mp, op);
    }

private:
    const Operation* processShown_ = nullptr;
    static int v118() { return 1; }   // @src 0x140601f30
    static int v120() { return 2; }   // @src 0x140601f50

public:
    // vt+0xc8  @src 0x140602230 (MDT L4, DB-5)
    void syncFromModelTail(Project& p, Operation&, Ctx& ctx, engine::FileEffects&) override
    {
        engine::multiBlowSyncFromModelTail(*this, p, ctx, &MultiBlowClass::v120);
    }

    void drive(W& w, Project& prj, const String& db, int* sim, Operation* op, MultiBlowProps& mp,
               int dbreadArg, int onlySim);
    bool blowBlock(W& w, Project& prj, const String& db, int* sim, Operation* op, int dbreadArg,
                   int onlySim, const MultiBlowData& d, int i, int j, bool* first, const String& eqp,
                   int nextSimNo);
    bool dwellBlock(W& w, const String& db, int* sim, Operation* op, int dbreadArg, int onlySim,
                    const MultiBlowData& d, int i, int j, double ang);
    bool heatBlocks(W& w, Project& prj, const String& db, int* sim, Operation* op, int dbreadArg,
                    int onlySim, const MultiBlowData& d, int i, int j, bool* first);
    void heatBlock(W& w, Project& prj, const String& db, int simv, Operation* op, int dbreadArg,
                   double total, double dt, double env, const String& name, int kind, bool* first,
                   const MultiBlowData& d, bool otp, double o1, double o2);
    void cont(W& w, Project& prj, const String& db, int simv, Operation* op, int kind, bool* first);

    bool is2D_;
    int wp_, topDie_, botDie_;     // +0x60 +0x64 +0x68
    double scale_;                 // +0x70
    double away_[3];               // +0x78 +0x80 +0x88
    std::uint8_t f90_ = 0;         // +0x90
    double mv_[3] = {};            // mv+0x50 +0x58 +0x60 of the driver's movement block
};

int projectUnit(const Operation* op)   // @src 0x140054ff0
{
    return op && op->project ? op->project->unit : 0;
}

// @src 0x14060c3a0 / 0x14060c4e0: unit value of a QString field, unit of the props' operation
// (0x1405f8dc0). The original ignores the evaluator's result flag.
double unitValue(const String& text, int category, const Operation* op)
{
    bool ok = false;
    return evalUnitString(text, category, projectUnit(op), &ok);
}

int stepCount(double total, double dt)
{
    int n = 1;
    if (0.0 < dt) {
        const double x = total / dt + 0.5;   // 0x142101640
        n = static_cast<int>(1.0 <= x ? x : 1.0);
    }
    return n;
}

}  // namespace

// Block names: QString("BL%1-xx").arg(j, 0, 10, ' '); bare = 1 from every caller (else
// opName + "-" + r). The pass index is passed but unused.
String blockName(const char* fmt, const Operation* op, int i, int j, bool bare)
{
    (void)i;
    const String r = qt::arg(qt::L(fmt), j, 0, 10, u' ');
    if (!bare) return op->name + qt::L("-") + r;
    return r;
}
// @src 0x1406078f0 "BL%1-BL", 0x140607b00 "BL%1-DW", 0x1406072c0 "BL%1-TI", 0x1406074d0 "BL%1-HT",
//      0x1406076e0 "BL%1-TO"
String nameBL(const Operation* op, int i, int j, bool bare) { return blockName("BL%1-BL", op, i, j, bare); }
String nameDW(const Operation* op, int i, int j, bool bare) { return blockName("BL%1-DW", op, i, j, bare); }
String nameTI(const Operation* op, int i, int j, bool bare) { return blockName("BL%1-TI", op, i, j, bare); }
String nameHT(const Operation* op, int i, int j, bool bare) { return blockName("BL%1-HT", op, i, j, bare); }
String nameTO(const Operation* op, int i, int j, bool bare) { return blockName("BL%1-TO", op, i, j, bare); }

namespace {

constexpr int kBlow = 0x200010, kTransferIn = 0x100010, kHeat = 0x100000, kTransferOut = 0x100020;

// @src 0x1406067e0
void MultiBlowClass::cont(W& w, Project& prj, const String& db, int simv, Operation* op, int kind,
                          bool* first)
{
    bool rd = true;
    if (first && *first) {
        rd = false;
        *first = false;
    }
    Stream& s = w.ts;
    if (kind == kTransferIn || kind == kHeat || kind == kTransferOut) {
        s << "PDIE " << T << v118() << endl;                    // 0x1424b0adc
    } else if (kind == kBlow) {
        emitSimCtrl(w, prj, db, simv, op);                      // 0x140134bf0
        emitMaterials(w, prj, db, simv, op);                    // 0x140134db0
        emitObjects(w, prj, db, simv, op, rd);                  // 0x140135220
        emitMeshSettings(w, prj, db, simv, op);                 // 0x1401360e0
        emitObjProperties(w, prj, db, simv, op);                // 0x1401365f0
        s << "PDIE " << T << v120() << endl;                    // 0x1424b0ae8
        emitObjpos(w, prj, db, simv, op);                       // 0x140135f10
        emitContact(w, prj, db, simv, op);                      // 0x140135da0
    }
}

// @src 0x1406028b0
void MultiBlowClass::drive(W& w, Project& prj, const String& db, int* sim, Operation* op,
                           MultiBlowProps& mp, int dbreadArg, int onlySim)
{
    if (!hasEngineModel(*op)) return;                           // 0x14005f770
    if (!mp.data) return;
    const MultiBlowData& d = *mp.data;
    // 0x14060def0: movement direction of the blow from MBD+0x68..+0x78
    mv_[0] = d.movDir[0];
    mv_[1] = d.movDir[1];
    mv_[2] = d.movDir[2];
    double ang = 0.0;
    int nextSimNo = 0;
    if (SubProject* sub = activeSubProject(prj))                // 0x1400a7390
        nextSimNo = nextSim(prj, *sub, flowNumber(*op) + 1);    // 0x1400b8820, 0x140050b20
    scale_ = projectUnit(op) == 1 ? 100.0 : 5.0;                // 0x1421de200 / 0x1421d3c60
    const bool two = is2D(*op);                                 // 0x14005f150
    away_[0] = -scale_ * mv_[0];
    away_[1] = -scale_ * mv_[1];
    if (!two) away_[2] = -scale_ * mv_[2];
    String eqp;
    if (ObjEntry* o = objEntry(*op, topDie_ - 1)) {             // 0x140061250
        String p;
        const int id = o->equipmentId;                          // 0x14004de90
        if (0 < id) p = equipmentKeyPath(*op->project, id);     // 0x1400a5970 -> vt+0x208 (P17)
        if (!p.isEmpty()) eqp = app::relToDb(db, p);            // 0x140819590 + relativeFilePath
    }
    if (two) ang = crtAtan2(mv_[1], mv_[0]) * 57.29577951308232;   // 0x1408609b0, 0x1421ec2d8
    f90_ = 0;
    bool first = true;
    const int np = data::mbHeats(d);                            // 0x14060c6f0
    for (int i = 1; i <= np; ++i) {
        const int nb = data::mbBlows(d, i);                     // 0x14060c710
        for (int j = 1; j <= nb; ++j) {
            if (!blowBlock(w, prj, db, sim, op, dbreadArg, onlySim, d, i, j, &first, eqp, nextSimNo))
                break;
            if (!dwellBlock(w, db, sim, op, dbreadArg, onlySim, d, i, j, ang)) break;
            if (!heatBlocks(w, prj, db, sim, op, dbreadArg, onlySim, d, i, j, &first)) break;
        }
    }
}

// @src 0x1406030c0
bool MultiBlowClass::blowBlock(W& w, Project& prj, const String& db, int* sim, Operation* op,
                               int dbreadArg, int onlySim, const MultiBlowData& d, int i, int j,
                               bool* first, const String& eqp, int nextSimNo)
{
    const bool two = is2D(*op);
    const bool heating = d.heating != 0;                        // 0x14060bef0
    const bool ndtmp = d.initTemp != 0;                         // 0x14060bce0
    const bool strain = d.initStrain != 0;                      // 0x14060bd30
    const bool rotOn = d.workpieceRotation != 0;                // 0x14060bc90
    *sim = *sim + 1;
    if (!(onlySim == 0 || *sim == onlySim)) return true;
    Stream& s = w.ts;
    emitSimHeader(w, *sim, op);                                 // 0x140131c10
    emitDbread(w, db, *sim, dbreadArg);                         // 0x1401343e0
    cont(w, prj, db, *sim, op, kBlow, first);
    bool rot = false;
    if (1 < j) {
        if (heating && data::mbReheat(d, i, j - 1) && ndtmp) {  // 0x14060ce60
            s << "NDTMP " << T << wp_ << T << 0 << T << d.heatingTemperature << endl;   // 0x1424b0278
            if (strain) s << "STRAIN " << T << wp_ << T << 0 << T << 0.0 << endl;      // 0x1424b0280
        }
        (void)data::mbHeatOk(d, i, j - 1);                      // 0x14060bdd0, result unused
        if (rotOn) rot = data::mbFlip(d, i, j - 1);             // 0x14060cfc0
    }
    if (!two) {
        if (rot) {
            s << "* Rotate" << endl;                            // 0x1424b03f0
            const data::BlowRotation r = data::mbRotation(d, i, j - 1);   // 0x14060d150
            if (!rotValid(r)) {
                s << "* Error : " << rotError(r) << endl;       // 0x1424b0470
            } else {
                s << "*    Rotate the workpiece: " << rotText(r) << endl;   // 0x1424b0400
                if (r.selfCenter == 0)
                    s << "OBJPOS " << T << wp_ << T << 3 << T << r.c[0] << T << r.c[1] << T << r.c[2] << T
                      << r.a[0] << T << r.a[1] << T << r.a[2] << T << r.angle << endl;    // 0x1424b0440
                else
                    s << "OBJPOS " << T << wp_ << T << 6 << T << r.a[0] << T << r.a[1] << T << r.a[2] << T
                      << r.angle << endl;                                                 // 0x1424b0420
            }
        }
        if (j != 1) {
            s << "* Positioning" << endl;                       // 0x1424b0480
            if (!rot) s << "*    Put the Workpiece on the Bottom die" << endl;            // 0x1424b0538
            else s << "*    Put the flipped Workpiece on the Bottom die" << endl;         // 0x1424b0490
            s << "OBJPOS " << T << wp_ << T << 2 << T << kGap << T << mv_[0] << T << mv_[1] << T << mv_[2] << T
              << botDie_ << endl;
            s << "*    Put Top die on Workpiece" << endl;
            s << "OBJPOS " << T << topDie_ << T << 2 << T << kGap << T << mv_[0] << T << mv_[1] << T << mv_[2]
              << T << wp_ << endl;
        }
    } else {
        if (rot) {
            s << "* Flip" << endl;                              // 0x1424b0288
            s << "*    Flip the workpiece with respect to a line perpendicular to the movement direction"
              << endl;                                          // 0x1424b0290
            s << "OBJPOS " << T << wp_ << T << 4 << T << 0 << T << 0 << T << mv_[1] << T << mv_[0] << endl;
        }
        if (j != 1) {
            s << "* Positioning" << endl;                       // 0x1424b02f0
            if (!rot) s << "*    Put the Workpiece on the Bottom die" << endl;            // 0x1424b0368
            else s << "*    Put the flipped Workpiece on the Bottom die" << endl;         // 0x1424b0300
            s << "OBJPOS " << T << wp_ << T << 2 << T << kGap << T << botDie_ << T << mv_[0] << T << mv_[1]
              << endl;
            s << "*    Put Top die on Workpiece" << endl;
            s << "OBJPOS " << T << topDie_ << T << 2 << T << kGap << T << wp_ << T << mv_[0] << T << mv_[1]
              << endl;
        }
    }
    s << "* Generate the contacts" << endl;                     // 0x1424b05d8
    s << "INICTC " << T << wp_ << endl;
    s << "GENCTC " << T << kGap << endl;
    if (!eqp.isEmpty()) {
        s << "IMPEQP " << T << topDie_ << endl;                 // 0x1424b0610
        s << eqp << endl;
        const double d1 = data::mbEnergy(d, i, j);              // 0x14060c810
        const double d2 = data::mbEfficiency(d, i, j);          // 0x14060cb20
        if (d.relativeEnergy == 0) {                            // 0x14060bb10
            s << "DOESCL " << T << d1 << endl;                  // 0x1424b0640
            s << "MOVCTL " << T << topDie_ << T << "1" << endl; // 0x1424b0650, 0x1424b0660
        } else {
            s << "DOESCL " << T << d1 / 100.0 << endl;          // 0x1424b0620
            s << "MOVCTL " << T << topDie_ << endl;             // 0x1424b0630
        }
        if (d.efficiencyFunction == 0) {                        // 0x14060bd80
            s << "DOESCL " << T << d2 << endl;                  // 0x1424b0668
            s << "MOVCTL " << T << topDie_ << T << "2" << endl; // 0x1424b0678, 0x1424b0688
        }
    }
    s << "* TRANS will not be redefined in Blow operation." << endl;   // 0x1424b0690
    s << "* Definition in SimCtrl.KEY will be used." << endl;          // 0x1424b06c8
    s << "* Define next simulation number - if minimum die distance criteria met, jump to next operation"
      << endl;                                                         // 0x1424b0700
    s << "NEXTSIMNO" << T << 3 << endl;                                // 0x1424b0760
    const int n = *sim;
    s << n + 1 << T << n + 1 << T << nextSimNo << endl;
    emitKfreadUser(w, op);                                      // 0x140134680
    emitOprnam(w, op->name);                                    // 0x140134730
    emitSimnam(w, nameBL(op, i, j, true));                      // 0x1401347e0
    emitCursim(w, *sim, flowNumber(*op), 0);                    // 0x140134900, 0x140050b10 = 0
    emitGendb(w, db, *sim, 2, false);                           // 0x140134a10
    return onlySim == 0;
}

// @src 0x140604910
bool MultiBlowClass::dwellBlock(W& w, const String& db, int* sim, Operation* op, int dbreadArg,
                                int onlySim, const MultiBlowData& d, int i, int j, double ang)
{
    const bool two = is2D(*op);
    f90_ = 0;
    const bool on = data::mbDwellOn(d, i, j);                   // 0x14060c280
    const double dwell = data::mbDwellTime(d, i, j);            // 0x14060ccc0
    const bool heatOk = data::mbHeatOk(d, i, j);                // 0x14060bdd0
    const bool otprng = d.adaptiveReheat != 0;                  // 0x14060c140
    if (!on) return true;
    *sim = *sim + 1;
    if (!(onlySim == 0 || *sim == onlySim)) return true;
    Stream& s = w.ts;
    emitSimHeader(w, *sim, op);
    emitDbread(w, db, *sim, dbreadArg);
    f90_ = 1;
    if (!two) {
        s << "* Positioning" << endl;                           // 0x1424b07b8
        s << "*    Move Top die away" << endl;                  // 0x1424b07c8
        s << "OBJPOS " << T << topDie_ << T << 1 << T << away_[0] << T << away_[1] << T << away_[2] << endl;
    } else {
        s << "* Positioning" << endl;                           // 0x1424b0778
        s << "*    Move Top die away" << endl;                  // 0x1424b0788
        s << "OBJPOS " << T << topDie_ << T << 1 << T << away_[0] << T << away_[1] << endl;
    }
    s << "* Generate the contacts" << endl;                     // 0x1424b0800
    s << "INICTC " << T << wp_ << endl;
    s << "GENCTC " << T << kGap << endl;
    const double h = unitValue(d.dwellHeatCoeff, 0x20, op);     // 0x14060c4e0
    s << "IHTCOF " << T << wp_ << T << topDie_ << T << 0 << T << h << endl;   // 0x1424b0838
    s << "IHTCOF " << T << wp_ << T << botDie_ << T << 0 << T << h << endl;   // 0x1424b0850
    if (!two) {
        s << "MOVCTL " << T << topDie_ << T << 1 << T << 0 << T << 0 << T << 0 << T << -1 << T << 0 << endl;
        s << "MOVCTL " << T << botDie_ << T << 1 << T << 0 << T << 0 << T << 0 << T << 1 << T << 0 << endl;
    } else {
        s << "MOVCTL " << T << topDie_ << T << 1 << T << 0 << T << ang << T << 0 << endl;     // 0x1424b0868
        s << "MOVCTL " << T << botDie_ << T << 1 << T << 0 << T << -ang << T << 0 << endl;    // 0x1424b0888
    }
    if (heatOk && otprng) {
        if (d.triggerHeatTempUpper == 0.0) (void)d.heatingTemperature;   // read and dropped
        const double b = d.triggerHeatTempUpper;                // 0x14060c1e0
        const double a = d.triggerHeatTemp;                     // 0x14060c190
        s << "OTPRNG" << T << wp_ << T << 1 << T << a << T << b << T << 0 << endl;   // 0x1424b08f4
        s << "NEXTSIMNO" << T << 3 << endl;                     // 0x1424b0910
        const int n = *sim;
        s << n + 4 << T << n + 1 << T << 0 << endl;
    }
    s << "DSMAX " << T << 0 << endl;                            // 0x1424b0928
    const double dt = unitValue(d.dwellTimeStep, 0, op);        // 0x14060c3a0
    s << "DTMAX " << T << dt << endl;                           // 0x1424b0934
    s << "NSTEP " << T << stepCount(dwell, dt) << endl;         // 0x1424b0940
    s << "* Turn off Deformation in Dwell" << endl;             // 0x1424b0950
    s << "TRANS " << T << 1 << T << 0 << T << -1 << T << -1 << T << -1 << T << -1 << endl;   // 0x1424b0970
    emitKfreadUser(w, op);
    emitOprnam(w, op->name);
    emitSimnam(w, nameDW(op, i, j, true));
    emitCursim(w, *sim, flowNumber(*op), 0);
    emitGendb(w, db, *sim, 2, false);
    return onlySim == 0;
}

// @src 0x140605830
bool MultiBlowClass::heatBlocks(W& w, Project& prj, const String& db, int* sim, Operation* op,
                                int dbreadArg, int onlySim, const MultiBlowData& d, int i, int j,
                                bool* first)
{
    (void)is2D(*op);                                            // 0x14005f150, result unused
    const bool ok = data::mbHeatOk(d, i, j);
    const bool otprng = d.adaptiveReheat != 0;
    const double tTI = d.transferInTime;                        // 0x14060c050
    const double tHT = d.heatingTime;                           // 0x14060bfb0
    const double tTO = d.transferOutTime;                       // 0x14060c0a0
    if (!ok) return true;
    if (0.0 < tTI) {
        *sim = *sim + 1;
        if (onlySim == 0 || *sim == onlySim) {
            heatBlock(w, prj, db, *sim, op, dbreadArg, tTI, d.transferTimeStep, d.roomTemperature,
                      nameTI(op, i, j, true), kTransferIn, first, d, false, 0.0, 0.0);
            if (onlySim != 0) return false;
        }
    }
    if (0.0 < tHT) {
        *sim = *sim + 1;
        if (onlySim == 0 || *sim == onlySim) {
            const double o2 = d.heatStopTemp * 1.1;             // 0x1424797e0
            heatBlock(w, prj, db, *sim, op, dbreadArg, tHT, d.heatingTimeStep, d.heatingTemperature,
                      nameHT(op, i, j, true), kHeat, first, d, otprng, d.heatStopTemp, o2);
            if (onlySim != 0) return false;
        }
    }
    if (0.0 < tTO) {
        *sim = *sim + 1;
        if (onlySim == 0 || *sim == onlySim) {
            heatBlock(w, prj, db, *sim, op, dbreadArg, tTO, d.transferTimeStep, d.roomTemperature,
                      nameTO(op, i, j, true), kTransferOut, first, d, false, 0.0, 0.0);
            if (onlySim != 0) return false;
        }
    }
    return true;
}

// @src 0x140605ee0
void MultiBlowClass::heatBlock(W& w, Project& prj, const String& db, int simv, Operation* op,
                               int dbreadArg, double total, double dt, double env, const String& name,
                               int kind, bool* first, const MultiBlowData& d, bool otp, double o1,
                               double o2)
{
    Stream& s = w.ts;
    emitSimHeader(w, simv, op);
    emitDbread(w, db, simv, dbreadArg);
    cont(w, prj, db, simv, op, kind, first);
    if (!otp) {
        s << "OTPRNG" << T << wp_ << T << 0 << T << 0.0 << T << 0.0 << T << 1 << endl;   // 0x1424b09c8
    } else {
        s << "OTPRNG" << T << wp_ << T << 2 << T << o1 << T << o2 << T << 1 << endl;     // 0x1424b0990
        s << "NEXTSIMNO" << T << 3 << endl;                     // 0x1424b09b0
        s << simv + 1 << T << simv + 1 << T << 0 << endl;
    }
    s << "OBJTYP" << T << topDie_ << T << 0 << endl;            // 0x1424b09e4
    s << "OBJTYP" << T << botDie_ << T << 0 << endl;            // 0x1424b09f4
    s << "DSMAX " << T << 0 << endl;                            // 0x1424b0a04
    s << "DTMAX " << T << dt << endl;                           // 0x1424b0a10
    s << "NSTEP " << T << stepCount(total, dt) << endl;         // 0x1424b0a1c
    s << "ENVTMP" << T << 0 << T << env << endl;                // 0x1424b0a28
    if (kind == kHeat && d.initStrain != 0) {                   // 0x14060bd30
        s << "* Initialize strain" << endl;                     // 0x1424b0a38
        s << "STRAIN " << T << wp_ << T << 0 << T << 0.0 << endl;   // 0x1424b0a50
    }
    s << "* Generate the contacts" << endl;                     // 0x1424b0a60
    s << "INICTC " << T << wp_ << endl;
    s << "GENCTC " << T << kGap << endl;
    s << "* Turn off Deformation in Heating" << endl;           // 0x1424b0a98
    s << "TRANS " << T << 1 << T << 0 << T << -1 << T << -1 << T << -1 << T << -1 << endl;   // 0x1424b0abc
    emitKfreadUser(w, op);
    emitOprnam(w, op->name);
    emitSimnam(w, name);
    emitCursim(w, simv, flowNumber(*op), 0);
    emitGendb(w, db, simv, 2, false);
}

}  // namespace

// P14 factory: SfOprMultiBlow2D (init 0x140600a50) / SfOprMultiBlow3D (class SfOprMultiBlow,
// init 0x140601480); both use the writer 0x140602810.
std::unique_ptr<OpClass> makeMultiBlow(bool is2D)
{
    return std::make_unique<MultiBlowClass>(qt::L(is2D ? "SfOprMultiBlow2D" : "SfOprMultiBlow3D"), is2D);
}

}  // namespace m2m::mst
