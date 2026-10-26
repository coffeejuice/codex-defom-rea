// FormingExpress(2D) MST writer (packet P15). Spec: s1-05 5, checked line by line against the
// decompiles (s1-05 d1s/d4s/d5s/d7s.c, s1/p15/m1..m6.c). Corrections to the spec text:
//   * the SIMNAM builders get the STEP number s (the 5th argument), not the blow b;
//   * 0x14005f770 builds the model on demand (vt+0x1b0 = 0x1405f6410) whenever the operation has a
//     project, so the writer stops early only for an operation without a project;
//   * SimCtrl+0x1ec is 0: 0x140899010(sc+0x1e8, 0x80) sets word 1 to 0 for keyword 0x80.
#include "ops/express.hpp"

#include <cmath>

#include "m2m/data/express.hpp"
#include "m2m/mst/emit.hpp"
#include "m2m/mst/loader_l4.hpp"
#include "m2m/mst/simcount.hpp"
#include "m2m/props_base.hpp"
#include "m2m/qt4path.hpp"

namespace m2m {

namespace {
const data::FormingExpressData& D(const FormingExpressProps& p) { return *p.fe; }

template <class T>
T at2(const std::vector<std::vector<T>>& l, int b, int s)
{
    if (b < 1 || static_cast<int>(l.size()) < b) return T{};
    const auto& r = l[static_cast<size_t>(b - 1)];
    if (s < 1 || static_cast<int>(r.size()) < s) return T{};
    return r[static_cast<size_t>(s - 1)];
}

const data::CommonBlock& B(const Props& p)
{
    static const data::CommonBlock empty;
    return p.common ? *p.common : empty;
}

// 0x1402ede00(0x143d76368, cat, unit, text, &out, &err); the callers here ignore the result flag
double unitValue(const Props& p, const String& text, int cat, bool* ok)
{
    bool dummy = false;
    return evalUnitString(text, cat, propsUnit(p), ok ? ok : &dummy);
}
}  // namespace

// @src 0x14141ce30
FormingExpressProps* feProps(const mst::OpClass& self, Operation* op)
{
    return dynamic_cast<FormingExpressProps*>(self.propsOf(op));
}

// @src 0x14142bb30
bool feIsExpressType(const FormingExpressProps& p) { return D(p).type == 3 || D(p).type == 4; }
// @src 0x14142bf50
bool feExpressFlag(const FormingExpressProps& p) { return D(p).express != 0; }
// @src 0x14142c080
int feBlowCount(const FormingExpressProps& p) { return D(p).blows; }
// @src 0x14142c110
int feStepCount(const FormingExpressProps& p, int b)
{
    const auto& l = D(p).steps;
    if (b < 1 || static_cast<int>(l.size()) < b) return 0;
    return l[static_cast<size_t>(b - 1)];
}
// @src 0x14142c210
double feStroke(const FormingExpressProps& p, int b, int s) { return at2(D(p).stroke, b, s); }
// @src 0x14142c3e0
double feL158(const FormingExpressProps& p, int b, int s) { return at2(D(p).l158, b, s); }
// @src 0x14142c490
double feGapAt(const FormingExpressProps& p, int b, int s) { return at2(D(p).gap, b, s); }
// @src 0x14142c5f0
bool feDropAt(const FormingExpressProps& p, int b, int s) { return at2(D(p).drop, b, s) != 0; }
// @src 0x14142c750
bool feFlipAt(const FormingExpressProps& p, int b, int s) { return at2(D(p).flip, b, s) != 0; }
// @src 0x14142be90
double feDwIhtc(const FormingExpressProps& p) { return unitValue(p, D(p).dwIhtc, 0x20, nullptr); }
// @src 0x14142be70 -> 0x14142bdb0
double feDwStep(const FormingExpressProps& p) { return unitValue(p, D(p).dwStep, 0, nullptr); }

// @src 0x1405f8dc0 -> 0x140054ff0
int propsUnit(const Props& p)
{
    if (!p.owner || !p.owner->project) return 0;
    return p.owner->project->unit;
}
// @src 0x1405fc890
double propsFriction(const Props& p) { return B(p).friction; }
// @src 0x1405fc8e0
double propsIhtc(const Props& p) { return unitValue(p, B(p).ihtc, 0x20, nullptr); }

// @src 0x1408d47a0 (fields read here) with 0x140899010(+0x1e8, 0x80) and 0x1408da660
void initSimCtrlVals(SimCtrlVals& sc)
{
    sc = SimCtrlVals{};
    sc.i1c8 = 1;
    sc.dsmax = 0.0;                // 0x1405fd4b0(sc, 0)
    sc.dtmax = 0.0;
    sc.d1d8 = 0.0;
    sc.kw1ec = 0;                  // case 0x80: word 1 = 0
    if (sc.kw1ec == 0) sc.dtMode = sc.dsmax <= 0.0 ? 1 : 0;   // 0x1408da660
}

// @src 0x1405fb980
bool propsSimCtrl80(const Props& p, SimCtrlVals& sc)
{
    sc.i16c = B(p).numSteps;
    sc.i164 = B(p).stepIncr;
    bool ok = false;
    double v = unitValue(p, B(p).dtmax, 0, &ok);
    if (ok) sc.dtmax = v;
    return true;
}
// @src 0x1405fbd10
bool propsSimCtrl90(const Props& p, SimCtrlVals& sc)
{
    bool ok = false;
    double v = unitValue(p, B(p).envTemp, 9, &ok);
    if (ok) sc.d3d0 = v;
    return ok;
}
// @src 0x1405fc060
bool propsSimCtrlA0(const Props& p, SimCtrlVals& sc)
{
    bool ok = false;
    double v = unitValue(p, B(p).convection, 0x20, &ok);
    if (ok) sc.d4c8 = v;
    return ok;
}
// @src 0x1405fc5d0
bool propsSimCtrlB0(const Props& p, SimCtrlVals& sc)
{
    sc.dtMode = B(p).dtMode;                         // 0x1408da6f0
    bool ok = false;
    if (B(p).dtMode == 0) {
        double v = unitValue(p, B(p).dsmax, 1, &ok);
        if (ok) {
            sc.dsmax = v;                            // 0x1405fd4b0
            sc.dtmax = 0.0;
        }
    } else {
        double v = unitValue(p, B(p).dtmax, 0, &ok);
        if (ok) {
            sc.dtmax = v;
            sc.dsmax = 0.0;                          // 0x1405fd4b0(sc, 0)
        }
    }
    return true;
}
// @src 0x1405fb6a0  vt+0x80, +0x90, +0xa0, +0xb0 (the SfOprBaseProperties bodies; no class that
// reaches this writer overrides them)
void propsToSimCtrl(const Props& p, SimCtrlVals& sc)
{
    propsSimCtrl80(p, sc);
    propsSimCtrl90(p, sc);
    propsSimCtrlA0(p, sc);
    propsSimCtrlB0(p, sc);
}

// @src 0x14005f770 -> 0x14005f820: opBase->vt[0xe0](op) is 0x140100e20 (returns 0) for every
// writer class except Cycle; the cached model (OPI+0x38) is then created by vt+0x1b0 =
// 0x1405f6410, which returns null only when the operation (made current) has no project. The
// Diestress, DiestressBM, DoeOutput and ShapeOptim classes have their own vt+0x1b0 and Cycle its
// own vt+0xe0; none of them calls this.
bool hasEngineModel(const Operation& op)
{
    if (!opBase(op)) return false;                  // 0x140054e90
    return op.project != nullptr;                   // 0x1405f6410: 0x140055030(current)
}

// @src 0x14005f770 (fresh model) and the model getters 0x140c7c0e0, 0x140c7c0a0, 0x140830270;
// the widths are DB_STEP_DATA's array size table after 0x1401d09c0 (port/defmodel
// step_setup_array_sizes: every entry 1 except the dimension-dependent ones, 68/69 included).
EngineFacts engineFacts(const Operation& op)
{
    EngineFacts f;
    if (!hasEngineModel(op)) return f;
    f.exists = true;
    f.dim = opBase(op)->dim();                      // 0x1405f6410: vt+0x190 -> 0x140821bf0(dim)
    f.is2D = f.dim == 2;
    f.is3D = f.dim == 3;
    f.objectCount = 0;
    f.ecctmpWidth = 1;
    f.ectmfnWidth = 1;
    return f;
}

namespace mst {

// @src 0x14142cb20  (the name copy +0x18 and the type fields go to the engine Movement)
void feToMovement(const FormingExpressProps& p, MovementVals& mv)
{
    switch (D(p).type) {
    case 0: mv.mode = 1; break;
    case 1: mv.mode = 0; break;
    case 2: mv.mode = 4; break;
    case 3: mv.mode = 2; break;
    case 4: mv.mode = 3; break;
    default: break;
    }
    mv.dir[0] = D(p).dir[0];
    mv.dir[1] = D(p).dir[1];
    mv.dir[2] = D(p).dir[2];
}

// @src 0x1408609b0
double movementAngle(const MovementVals& mv)
{
    return std::atan2(mv.dir[1], mv.dir[0]) * 57.29577951308232;   // 0x1421ec2d8
}

// @src 0x14141bbe0 (via 0x14141bb40 / 0x141412f90)
FormingExpressClass::FormingExpressClass(bool is2D)
    : OpClass(qt::L(is2D ? "SfOprFormingExpress2D" : "SfOprFormingExpress")), is2D_(is2D)
{
}

// @src 0x14141cf10 -> 0x141426320 -> 0x141401ee0 + 0x1414263f0
std::shared_ptr<Props> FormingExpressClass::makeProps(int guiUnit) const
{
    auto p = std::make_shared<FormingExpressProps>();
    data::initFormingExpressData(*p->fe, guiUnit);  // 0x14142fac0
    // 0x1414269a0: movement direction by class name (the property tree itself: express_tree.cpp)
    if (is2D_) {
        p->fe->dir[0] = 0.0;
        p->fe->dir[1] = -1.0;                       // 0x1421d3c90
        p->fe->dir[2] = 0.0;
    } else {
        p->fe->dir[0] = 0.0;
        p->fe->dir[1] = 0.0;
        p->fe->dir[2] = -1.0;
    }
    data::feSetNumHeats(*p->fe, 1);                 // 0x14142d490 -> 0x14142d610(this, 1)
    p->tree = data::formingExpressPropTree(*p->fe, is2D_);   // 0x141429cc0
    // 0x141429cc0 continued: 0x14142cf80 (Interobject children), then the base groups
    data::initCommonBlock(*p->common, guiUnit);    // base ctor 0x1405f8ca0 -> 0x1405fd250 (ran first)
    for (PropNode& g : p->tree[0].children) {
        if (g.name.u16() != u"Interobject") continue;
        base::addFriction(*p, g, guiUnit, 0.0, 0.0);                       // 0x1405fad10
        base::addHeatTransCoef(*p, g, guiUnit, qt::L("0.0 Btu/sec/in^2/F"),   // 0x1405fa7d0
                               qt::L("0.0 N/sec/mm/C"));                   // 0x142ed7090 / 0x142ed7080
    }
    base::addStepCounts(*p, false, false, 100, 10, qt::L("0.0 sec"));    // 0x1405f9eb0
    base::addEnvTemp(*p, guiUnit, qt::L("68.00 F"), qt::L("20.00 C"));   // 0x1405fa560
    base::addConvection(*p, guiUnit, qt::L("0.0 Btu/sec/in^2/F"),        // 0x1405faaa0
                        qt::L("0.0 N/sec/mm/C"));
    base::addStepBy(*p, guiUnit, 0, qt::L("0.0 in"), qt::L("0.0 mm"),     // 0x1405fb020
                    qt::L("0.0 sec"));
    return p;
}

// @src 0x1414136b0 (SfOprFormingExpress) / 0x141412c90 (SfOprFormingExpress2D): identical bodies
void FormingExpressClass::writeMst(W& w, Project& prj, const String& db, int* sim, Operation* op,
                                   int dbreadArg, int onlySim)
{
    bool express = false;
    FormingExpressProps* p = feProps(*this, op);
    if (p && feIsExpressType(*p)) express = feExpressFlag(*p);
    if (!express)
        OpClass::writeMst(w, prj, db, sim, op, dbreadArg, onlySim);   // 0x1405f5340
    else
        writeExpress(w, prj, db, sim, op, dbreadArg, onlySim);
}

// @src 0x14141f2f0
void FormingExpressClass::writeExpress(W& w, Project& prj, const String& db, int* sim,
                                       Operation* op, int dbreadArg, int onlySim)
{
    FormingExpressProps* p = feProps(*this, op);
    if (!p || !op) return;
    const bool d2 = is2D(*op);                      // 0x14005f150
    double angle = 0.0;
    const int wp = 1, top = 2, bot = 3;
    const int pos[6] = {1, 1, 0, 0, 0, 0};          // TRANS arguments, never changed
    int nextSimNo = 0;
    if (SubProject* cur = activeSubProject(prj))    // 0x1400a7390
        nextSimNo = nextSim(prj, *cur, flowNumber(*op) + 1);   // 0x1400b8820(cur, 0x140050b20 + 1)
    // LLOBJ(3) 0x140821bf0, SIM_CTRL 0x1408d3bc0, Movement 0x14085f3f0: engine scratch objects
    if (!hasEngineModel(*op)) return;               // 0x14005f770
    SimCtrlVals sc;
    initSimCtrlVals(sc);                            // 0x1408d3cd0(&sc, &llobj)
    if (sc.unit != op->project->unit) sc.unit = op->project->unit;   // 0x1408e0200(&sc, unit, false)
    propsToSimCtrl(*p, sc);                         // props vt+0x70 = 0x1405fb6a0
    MovementVals mv;                                // 0x14085f540(&mv, &llobj)
    feToMovement(*p, mv);                           // 0x14142cb20
    moveDist = op->project->unit == 1 ? 100.0 : 5.0;   // 0x1421de200 / 0x1421d3c60 (unit via 0x140054ff0)
    back[0] = -moveDist * mv.dir[0];
    back[1] = -moveDist * mv.dir[1];
    if (!d2) back[2] = -moveDist * mv.dir[2];
    dwWritten = false;
    String equip;
    if (ObjEntry* o = objEntry(*op, top - 1)) {     // 0x140061250
        String e;
        int eid = o->equipmentId;                   // 0x14004de90
        if (eid > 0) e = equipmentKeyPath(*op->project, eid);   // 0x1400a5970 -> vt+0x208
        if (!e.isEmpty()) equip = app::relToDb(db, e);   // QDir(0x140819590(db)).relativeFilePath
    }
    if (d2) angle = movementAngle(mv);              // 0x1408609b0
    bool first = true;
    const int nBlow = feBlowCount(*p);              // 0x14142c080
    for (int b = 1; b <= nBlow; ++b) {
        const int nStep = feStepCount(*p, b);       // 0x14142c110
        for (int s = 1; s <= nStep; ++s) {
            if (!blockBL(w, prj, db, sim, op, dbreadArg, onlySim, *p, b, s, pos, &first, sc, mv,
                         equip, wp, top, bot, nextSimNo))
                break;
            if (!blockDW(w, prj, db, sim, op, dbreadArg, onlySim, *p, b, s, pos, &first, mv, angle,
                         wp, top, bot))
                break;
            if (!blockTO(w, prj, db, sim, op, dbreadArg, onlySim, *p, b, s, pos, &first))
                break;
        }
    }
}

// @src 0x14141fc50
bool FormingExpressClass::blockBL(W& w, Project& prj, const String& db, int* sim, Operation* op,
                                  int dbreadArg, int onlySim, FormingExpressProps& p, int b, int s,
                                  const int* pos, bool* first, const SimCtrlVals& sc,
                                  const MovementVals& mv, const String& equip, int wp, int top,
                                  int bot, int nextSimNo)
{
    Stream& S = w.ts;
    const bool d2 = is2D(*op);
    *sim = *sim + 1;
    if (onlySim != 0 && *sim != onlySim) return true;
    if (!op->restartGiven || !*first) {             // 0x140064740
        emitSimHeader(w, *sim, op);                 // 0x140131c10
        if (!writesDimNstart() || !*first)          // vt+0x200
            emitDbread(w, db, *sim, dbreadArg);     // 0x1401343e0
        else
            emitDimNstart(w, db, *sim, op, dbreadArg);   // 0x140134250
        writeObjects(w, prj, db, *sim, op, 0x200010, first);   // 0x141423130
        bool drop = false, flp = false;
        double g = 0.0;
        if (s > 1) {
            drop = feDropAt(p, b, s - 1);
            flp = feFlipAt(p, b, s - 1);
            g = feGapAt(p, b, s - 1);
        }
        if (!d2) {
            if (s != 1) {
                S << "* Positioning" << endl;
                if (!flp) {
                    if (!drop) {
                        if (g <= 0.0) {
                            S << "*    Put Top die on Workpiece" << endl;
                            S << "OBJPOS " << "\t" << topNo << "\t" << 2 << "\t" << 0.0001 << "\t"
                              << mv.dir[0] << "\t" << mv.dir[1] << "\t" << mv.dir[2] << "\t" << wpNo
                              << endl;
                        } else {
                            S << "*    Move Top die back" << endl;
                            S << "OBJPOS " << "\t" << topNo << "\t" << 1 << "\t" << -back[0] << "\t"
                              << -back[1] << "\t" << -back[2] << endl;
                        }
                    } else {
                        S << "*    Move Top die back" << endl;
                        S << "OBJPOS " << "\t" << topNo << "\t" << 1 << "\t" << -back[0] << "\t"
                          << -back[1] << "\t" << -back[2] << endl;
                        S << "*    Move Bottom die back" << endl;
                        S << "OBJPOS " << "\t" << botNo << "\t" << 1 << "\t" << back[0] << "\t"
                          << back[1] << "\t" << back[2] << endl;
                    }
                }
            }
        } else {
            if (flp) {
                S << "* Flip" << endl;
                S << "*    Flip the workpiece with respect to a line perpendicular to the movement "
                     "direction"
                  << endl;
                S << "OBJPOS " << "\t" << wp << "\t" << 4 << "\t" << 0 << "\t" << 0 << "\t"
                  << mv.dir[1] << "\t" << mv.dir[0] << endl;
            }
            if (s != 1) {
                S << "* Positioning" << endl;
                if (!flp) {
                    if (!drop) {
                        if (g <= 0.0) {
                            S << "*    Put Top die on Workpiece" << endl;
                            S << "OBJPOS " << "\t" << topNo << "\t" << 2 << "\t" << 0.0001 << "\t"
                              << wpNo << "\t" << mv.dir[0] << "\t" << mv.dir[1] << endl;
                        } else {
                            S << "*    Move Top die back" << endl;
                            S << "OBJPOS " << "\t" << topNo << "\t" << 1 << "\t" << -back[0] << "\t"
                              << -back[1] << endl;
                        }
                    } else {
                        S << "*    Move Top die back" << endl;
                        S << "OBJPOS " << "\t" << topNo << "\t" << 1 << "\t" << -back[0] << "\t"
                          << -back[1] << endl;
                        S << "*    Move Bottom die back" << endl;
                        S << "OBJPOS " << "\t" << botNo << "\t" << 1 << "\t" << back[0] << "\t"
                          << back[1] << endl;
                    }
                } else {
                    S << "*    Put the flipped Workpiece on the Bottom die" << endl;
                    S << "OBJPOS " << "\t" << wpNo << "\t" << 2 << "\t" << 0.0001 << "\t" << botNo
                      << "\t" << mv.dir[0] << "\t" << mv.dir[1] << endl;
                    S << "*    Put Top die on Workpiece" << endl;
                    S << "OBJPOS " << "\t" << topNo << "\t" << 2 << "\t" << 0.0001 << "\t" << wpNo
                      << "\t" << mv.dir[0] << "\t" << mv.dir[1] << endl;
                }
            }
        }
        S << "* Generate the contacts" << endl;
        S << "INICTC " << "\t" << wp << endl;
        S << "GENCTC " << "\t" << 0.0001 << endl;
        const double fr = propsFriction(p);         // 0x1405fc890
        S << "FRCFAC " << "\t" << wp << "\t" << top << "\t" << 1 << "\t" << 0 << "\t" << fr << endl;
        const double h = propsIhtc(p);              // 0x1405fc8e0
        S << "IHTCOF " << "\t" << wp << "\t" << top << "\t" << 0 << "\t" << h << endl;
        S << "IHTCOF " << "\t" << wp << "\t" << bot << "\t" << 0 << "\t" << h << endl;
        double st = feStroke(p, b, s);              // 0x14142c210
        (void)feL158(p, b, s);                      // 0x14142c3e0
        if (!equip.isEmpty()) {
            S << "IMPEQP " << "\t" << top << endl;
            S << equip << endl;
            st = st / 100.0;                        // 0x1421de200
            S << "DOESCL " << "\t" << st << endl;
            S << "MOVCTL " << "\t" << top << endl;
        }
        if (sc.kw1ec == 0) {
            if (sc.dtMode == 0)                     // 0x1408da6d0
                S << keywordName(0x30) << "\t" << sc.dsmax << endl;
            if (sc.dtMode == 1)
                S << keywordName(0x32) << "\t" << sc.dtmax << "\t" << sc.d1d8 << endl;
            else if (0.0 < sc.d1d8)
                S << keywordName(0x32) << "\t" << 0.0 << "\t" << sc.d1d8 << endl;
            S << keywordName(0x33) << "\t" << 0 << "\t" << 0 << endl;
        }
        S << "NSTEP " << "\t" << 10000 << endl;
        S << "TRANS " << "\t" << pos[0] << "\t" << pos[1] << "\t" << pos[2] << "\t" << pos[3] << "\t"
          << pos[4] << "\t" << pos[5] << endl;
        S << "* Define next simulation number - if minimum die distance criteria met, jump to next "
             "operation"
          << endl;
        S << "NEXTSIMNO" << "\t" << 3 << endl;
        const int n = *sim;
        S << n + 1 << "\t" << n + 1 << "\t" << nextSimNo << endl;
        emitKfreadUser(w, op);                      // 0x140134680
        emitOprnam(w, op->name);                    // 0x140134730(0x140050ac0)
        emitSimnam(w, simNameBL(op, b, s, false));  // 0x1401347e0(0x141423f20)
        const int cb = 0;                           // 0x140050b10
        emitCursim(w, *sim, flowNumber(*op), cb);   // 0x140134900
        emitGendb(w, db, *sim, 2, false);           // 0x140134a10
        if (onlySim != 0) return false;
    } else {
        emitSimHeader(w, *sim, op);
        emitKfreadRestart(w, db, op);               // 0x140134510
        emitKfreadUser(w, op);
        emitOprnam(w, op->name);
        emitSimnam(w, simNameBL(op, b, s, false));
        const int cb = 0;
        emitCursim(w, *sim, flowNumber(*op), cb);
        emitGendb(w, db, *sim, 0, false);
        *first = false;
        if (onlySim != 0) return false;
    }
    return true;
}

// @src 0x1414216e0
bool FormingExpressClass::blockDW(W& w, Project&, const String& db, int* sim, Operation* op,
                                  int dbreadArg, int onlySim, FormingExpressProps& p, int b, int s,
                                  const int* pos, bool*, const MovementVals&, double angle, int wp,
                                  int top, int bot)
{
    Stream& S = w.ts;
    const bool d2 = is2D(*op);
    dwWritten = false;
    const double g = feGapAt(p, b, s);              // 0x14142c490
    if (!(0.0 < g)) return true;
    *sim = *sim + 1;
    if (onlySim != 0 && *sim != onlySim) return true;
    emitSimHeader(w, *sim, op);
    emitDbread(w, db, *sim, dbreadArg);
    dwWritten = true;
    S << "* Positioning" << endl;
    S << "*    Move Top die away" << endl;
    if (!d2)
        S << "OBJPOS " << "\t" << top << "\t" << 1 << "\t" << back[0] << "\t" << back[1] << "\t"
          << back[2] << endl;
    else
        S << "OBJPOS " << "\t" << top << "\t" << 1 << "\t" << back[0] << "\t" << back[1] << endl;
    S << "* Generate the contacts" << endl;
    S << "INICTC " << "\t" << wp << endl;
    S << "GENCTC " << "\t" << 0.0001 << endl;
    const double h = feDwIhtc(p);                   // 0x14142be90
    S << "IHTCOF " << "\t" << wp << "\t" << top << "\t" << 0 << "\t" << h << endl;
    S << "IHTCOF " << "\t" << wp << "\t" << bot << "\t" << 0 << "\t" << h << endl;
    if (!d2) {
        S << "MOVCTL " << "\t" << top << "\t" << 1 << "\t" << 0 << "\t" << 0 << "\t" << 0 << "\t"
          << -1 << "\t" << 0 << endl;
        S << "MOVCTL " << "\t" << bot << "\t" << 1 << "\t" << 0 << "\t" << 0 << "\t" << 0 << "\t"
          << 1 << "\t" << 0 << endl;
    } else {
        S << "MOVCTL " << "\t" << top << "\t" << 1 << "\t" << 0 << "\t" << angle << "\t" << 0 << endl;
        S << "MOVCTL " << "\t" << bot << "\t" << 1 << "\t" << 0 << "\t" << -angle << "\t" << 0
          << endl;
    }
    S << "DSMAX " << "\t" << 0 << endl;
    const double dt = feDwStep(p);                  // 0x14142be70
    S << "DTMAX " << "\t" << dt << endl;
    int nstep = 1;
    if (0.0 < dt) {
        const double r = g / dt + 0.5;              // 0x142101640
        nstep = static_cast<int>(1.0 <= r ? r : 1.0);
    }
    S << "NSTEP " << "\t" << nstep << endl;
    S << "TRANS " << "\t" << pos[0] << "\t" << 0 << "\t" << pos[2] << "\t" << pos[3] << "\t"
      << pos[4] << "\t" << pos[5] << endl;
    emitKfreadUser(w, op);
    emitOprnam(w, op->name);
    emitSimnam(w, simNameDW(op, b, s, false));      // 0x141424130
    const int cb = 0;
    emitCursim(w, *sim, flowNumber(*op), cb);
    emitGendb(w, db, *sim, 2, false);
    return onlySim == 0;
}

// @src 0x141422320
bool FormingExpressClass::blockTO(W& w, Project& prj, const String& db, int* sim, Operation* op,
                                  int dbreadArg, int onlySim, FormingExpressProps& p, int b, int s,
                                  const int* pos, bool* first)
{
    Stream& S = w.ts;
    const bool d2 = is2D(*op);
    if (!feDropAt(p, b, s)) return true;            // 0x14142c5f0
    const double dist = D(p).toTime;                // 0x14142bd00
    if (!(0.0 < dist)) return true;
    *sim = *sim + 1;
    if (onlySim != 0 && *sim != onlySim) return true;
    const double dt = D(p).toStep;                  // 0x14142bd50
    (void)D(p).roomTemp;                                 // 0x14142bce0
    emitSimHeader(w, *sim, op);
    emitDbread(w, db, *sim, dbreadArg);
    writeObjects(w, prj, db, *sim, op, 0x100020, first);
    const double T = D(p).toTemp;                   // 0x14142bc90
    S << "* Initialize node temperature" << endl;
    S << "NDTMP " << "\t" << wpNo << "\t" << 0 << "\t" << T << endl;
    if (D(p).initStrain) {                          // 0x14142bc40
        S << "* Initialize strain" << endl;
        S << "STRAIN " << "\t" << wpNo << "\t" << 0 << "\t" << 0.0 << endl;
    }
    S << "* Positioning" << endl;
    if (!d2) {
        if (!dwWritten) {
            S << "*    Move Top die away" << endl;
            S << "OBJPOS " << "\t" << topNo << "\t" << 1 << "\t" << back[0] << "\t" << back[1] << "\t"
              << back[2] << endl;
        }
        S << "*    Move Bottom die away" << endl;
        S << "OBJPOS " << "\t" << botNo << "\t" << 1 << "\t" << -back[0] << "\t" << -back[1] << "\t"
          << -back[2] << endl;
    } else {
        if (!dwWritten) {
            S << "*    Move Top die away" << endl;
            S << "OBJPOS " << "\t" << topNo << "\t" << 1 << "\t" << back[0] << "\t" << back[1]
              << endl;
        }
        S << "*    Move Bottom die away" << endl;
        S << "OBJPOS " << "\t" << botNo << "\t" << 1 << "\t" << -back[0] << "\t" << -back[1] << endl;
    }
    S << "* Generate the contacts" << endl;
    S << "INICTC " << "\t" << wpNo << endl;
    S << "GENCTC " << "\t" << 0.0001 << endl;
    S << "DSMAX " << "\t" << 0 << endl;
    S << "DTMAX " << "\t" << dt << endl;
    int nstep = 1;
    if (0.0 < dt) {
        const double r = dist / dt + 0.5;
        nstep = static_cast<int>(1.0 <= r ? r : 1.0);
    }
    S << "NSTEP " << "\t" << nstep << endl;
    S << "TRANS " << "\t" << pos[0] << "\t" << 0 << "\t" << pos[2] << "\t" << pos[3] << "\t"
      << pos[4] << "\t" << pos[5] << endl;
    emitKfreadUser(w, op);
    emitOprnam(w, op->name);
    emitSimnam(w, simNameTO(op, b, s, false));      // 0x141423d10
    const int cb = 0;
    emitCursim(w, *sim, flowNumber(*op), cb);
    emitGendb(w, db, *sim, 2, false);
    return onlySim == 0;
}

// @src 0x141423130
void FormingExpressClass::writeObjects(W& w, Project& prj, const String& db, int n, Operation* op,
                                       int mode, bool* first)
{
    Stream& S = w.ts;
    bool doObj = false, doPos = false, notFirst = true;
    if (first && *first) {
        doObj = true;
        doPos = true;
        notFirst = false;
        *first = false;
    }
    if (mode == 0x100010 || mode == 0x100000 || mode == 0x100020) {
        S << "PDIE " << "\t" << 1 << endl;
    } else if (mode == 0x200010) {
        if (doObj) {
            emitSimCtrl(w, prj, db, n, op);             // 0x140134bf0
            emitMaterials(w, prj, db, n, op);           // 0x140134db0
            emitObjects(w, prj, db, n, op, notFirst);   // 0x140135220
            emitMeshSettings(w, prj, db, n, op);        // 0x1401360e0
            emitObjProperties(w, prj, db, n, op);       // 0x1401365f0
        }
        S << "PDIE " << "\t" << 2 << endl;
        if (doPos) {
            S << "* Positioning" << endl;
            emitObjpos(w, prj, db, n, op);              // 0x140135f10
        }
        emitContact(w, prj, db, n, op);                 // 0x140135da0
    }
}

namespace {
// QString(tag).arg(s, 0, 10, QChar(' ')), then op name + "-" + it unless shortName
String simName(const char* tag, Operation* op, int s, bool shortName)
{
    const String name = op->name;                       // 0x140050ac0
    const String r = qt::arg(qt::L(tag), s, 0, 10, u' ');
    if (!shortName) return name + qt::L("-") + r;
    return r;
}
}  // namespace

// @src 0x141423f20
String FormingExpressClass::simNameBL(Operation* op, int, int s, bool shortName) const
{
    return simName("BL%1", op, s, shortName);
}
// @src 0x141424130
String FormingExpressClass::simNameDW(Operation* op, int, int s, bool shortName) const
{
    return simName("DW%1", op, s, shortName);
}
// @src 0x141423d10
String FormingExpressClass::simNameTO(Operation* op, int, int s, bool shortName) const
{
    return simName("TO%1", op, s, shortName);
}

// @src 0x141423390
void FormingExpressClass::simList(int pos, int* counter, Operation* op, std::vector<SimEntry>& out)
{
    simListFormingExpress(*this, pos, counter, op, out);
}

// ---- data seams of P14's +0x298 body (simcount.hpp); the props are cast as 0x14141ce30 does ----
namespace simdata {
namespace {
const FormingExpressProps* fe(const ExpressProps& p) { return dynamic_cast<const FormingExpressProps*>(&p); }
}  // namespace
// @src 0x14142bf50
bool feExpress(const ExpressProps& p) { return fe(p) && feExpressFlag(*fe(p)); }
// @src 0x14142c080
int feBlows(const ExpressProps& p) { return fe(p) ? feBlowCount(*fe(p)) : 0; }
// @src 0x14142c110
int feSteps(const ExpressProps& p, int b) { return fe(p) ? feStepCount(*fe(p), b) : 0; }
// @src 0x14142c490
double feGap(const ExpressProps& p, int b, int s) { return fe(p) ? feGapAt(*fe(p), b, s) : 0.0; }
// @src 0x14142c5f0
bool feDrop(const ExpressProps& p, int b, int s) { return fe(p) && feDropAt(*fe(p), b, s); }
// @src 0x14142bd00
double feToTime(const ExpressProps& p) { return fe(p) ? fe(p)->fe->toTime : 0.0; }
}  // namespace simdata

// ---- MDT L4 (DB-5): OB vt+0xc8 of the Express family -------------------------------------------

namespace {

// props vt+0x100: SfOprFormingExpressProperties 0x1400707e0 false, SfOprHeatTransferExpressProperties
// 0x1413e5ef0 true; SfOprExpressProperties' own slot is _purecall (0x141470184)
bool expressPropsV100(const ExpressProps& p)
{
    if (dynamic_cast<const FormingExpressProps*>(&p)) return false;
    if (dynamic_cast<const HeatTransferExpressProps*>(&p)) return true;
    throw Error(M2M_E_UNSUPPORTED, "Express vt+0xc8: props vt+0x100 of SfOprExpressProperties is _purecall "
                                   "(0x141470184)");
}

// @src 0x1413fd0f0  4 when the props' vt+0x100 is false (0x1413fd0a0 on 0x1413fd030), else the heat
// transfer type (+0x18, 0x140010320) of the current item's props data (0x1413fa180), -1 without
int expressHeatTransType(OpClass& c)
{
    auto* pr = c.current ? dynamic_cast<ExpressProps*>(c.current->props.get()) : nullptr;   // 0x1413fd030
    if (!pr || !expressPropsV100(*pr)) return 4;
    return pr->data ? pr->data->heatTransType : -1;
}

}  // namespace

// @src 0x1413fbd10  SfOprExpressBase / SfOprFormingExpress* / SfOprHeatTransferExpress* vt+0xc8
void expressSyncFromModelTail(OpClass& c, Project& p, Ctx& ctx)
{
    auto* pr = dynamic_cast<ExpressProps*>(c.propsOf(nullptr));   // 0x1405ef320(OB, 0), TD 0x143579bb8
    Operation* cur = c.current;                                   // 0x1405ef2b0
    data::ExpressData* d = pr ? pr->data.get() : nullptr;         // 0x140025010
    if (!pr || !cur || !d) return;
    if (cur->oprFlag == 0 && d->stepDefinition == 0) {            // 0x1400509d0, 0x1413f6b20
        d->stepDefinition = 1;                                    // 0x1413f6ae0(d, 0)
        // MSG signalShowMessage(false, type, text): the GUI's log
        ctx.warn("Express (" + cur->name.utf8() + "): Solution step mode has been switched to User defined.");
    }
    bool ask = false;
    if (expressHeatTransType(c) != 0) {                           // 0x1413fd0f0
        // 0x140050d00: the operation's class name; "SfOprHeatExpress" (0x142e82fc0) case-insensitive
        if (!qt::contains(cur->classKey, qt::L("SfOprHeatExpress"), qt::Case::Insensitive))
            ask = l4Slot(g_loaderSlotsL4.expressDieMeshes)(c, p, true);   // vt+0x2c0(OB, 1)
    }
    if (ask) {
        // QMessageBox::question(main window, "Keep Mesh of Dies?", "Currently meshed dies are
        // imported.\n...", Yes (0x4000), No (0x10000)): the obsolete (StandardButton, StandardButton)
        // overload. Neither button carries QMessageBox::Default (0x100), so Qt 4.8 (showNewMessageBox
        // -> showOldMessageBox -> addOldButtons -> findButton) sets no default button; Return goes to
        // the focused auto-default button, Yes, the first of the dialog (oracle B 130: Return gave
        // Yes). Documented deviation (docs/re/moproj2mst/mdt/L4/spec.md section 10): the converter
        // answers the default without a user; g_keepMeshOfDiesNo answers No (tests, 130n).
        if (!g_keepMeshOfDiesNo) {                                // 0x4000 (Yes)
            ctx.warn("Express (" + cur->name.utf8() + "): \"Keep Mesh of Dies?\" answered Yes (the dialog's "
                     "default button; documented deviation, no user to ask)");
            if (d->formingType == 0) d->formingType = 2;          // 0x14142ff20, 0x1413fd9d0(d, 2)
            if (d->dieTemperatureType != 0) d->dieTemperatureType = 0;   // 0x140cbc690, 0x141371aa0(d, 0)
        } else {
            l4Slot(g_loaderSlotsL4.expressDieMeshes)(c, p, false);    // vt+0x2c0(OB, 0): die meshes deleted
        }
    }
    l4Slot(g_loaderSlotsL4.expressTrans)(c, p);                   // vt+0x2b8
}

// Factory (the registry keys SfOprFormingExpress / SfOprFormingExpress2D)
std::unique_ptr<OpClass> makeFormingExpress(bool is2D)
{
    return std::make_unique<FormingExpressClass>(is2D);
}

}  // namespace mst
}  // namespace m2m
