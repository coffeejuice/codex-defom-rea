// SfOprHeatTreat / SfOprHeatTreat2D (owner P16). Spec: s1-05 4 (writer). The open-time schedule
// reader (props vt+0xc0 0x1411c0940 -> 0x1411b4770 -> 0x1411b4870 -> 0x1411b4ab0) and the props
// refresh 0x1411bfd60 are MDT L6's (src/ops/heattreat_htwz.cpp, m2m/ht_schedule.hpp).
//
// Not ported, by plan 2.1 and because no MST byte depends on them:
//   0x1411b71c0  HT_Proc<k>.MOKEY side files written during Save (and the checks 0x1411b9080,
//                0x1411bae90 it runs); the MST names those files, the converter warns when one
//                is missing.
//   engine scratch of the body (LLOBJ 0x140821bf0, SimCtrl 0x1408d3bc0, Movement 0x14085f3f0,
//                props vt+0x70 0x1411c05a0) and the unused QFileInfo(db).baseName().
// The simulation lists (0x1411b2790 / 0x1411b0170) are P14's.
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <limits>
#include <memory>
#include <string>

#include "m2m/data/heattreat.hpp"
#include "m2m/datafiles.hpp"
#include "m2m/ht_schedule.hpp"
#include "m2m/model.hpp"
#include "m2m/mst/emit.hpp"
#include "m2m/mst/loader_l6.hpp"
#include "m2m/mst/simcount.hpp"
#include "m2m/mst/writer.hpp"
#include "m2m/props.hpp"
#include "m2m/qt4path.hpp"
#include "p16_internal.hpp"

namespace m2m {
namespace {

// @src 0x1411b38e0  <taskDir>\HT_Proc<k+1>.MOKEY
String htProcPath(const data::HTSchedule& s, int k)
{
    String dir;
    if (s.owner && s.owner->owner) dir = taskDirAbs(*s.owner->owner);   // 0x1405f8d90 -> 0x140050df0
    String name = String(u"HT_Proc") + qt::number(k + 1) + String(u".MOKEY");   // @0x142ca5000/4ff8
    return qt::convertSeparators(dir + qt::kSep + name);
}

}  // namespace

String ht::procPath(const data::HTSchedule& s, int k)
{
    return htProcPath(s, k);
}

// @src 0x1411c0940  SfOprHeatTreatProperties vt+0xc0: 0x1411b4770(schedule), then vt+0x58
// (0x1411bfd60); bodies in src/ops/heattreat_htwz.cpp (MDT L6)
void HeatTreatProps::postLoad(Project& p, Ctx&)
{
    if (!data) return;
    ht::loadSchedule(p, data->schedule);
    ht::propsRefresh(p, *this);
}

}  // namespace m2m

namespace m2m::mst {
namespace {

class HeatTreatClass : public OpClass {
public:
    explicit HeatTreatClass(bool is2D) : OpClass(qt::L(is2D ? "SfOprHeatTreat2D" : "SfOprHeatTreat")) {}
    void writeMst(W& w, Project& prj, const String& db, int* sim, Operation* op, int dbreadArg,
                  int onlySim) override;
    // @src 0x1413e5ef0  vt+0x200: true (same body as P10's DimNstartClass)
    bool writesDimNstart() const override { return true; }
    std::shared_ptr<Props> makeProps(int guiUnit) const override;
    // vt+0x298 0x1411b2790 (3D) / 0x1411b0170 (2D) (P14)
    void simList(int pos, int* counter, Operation* op, std::vector<SimEntry>& out) override
    {
        simListHeatTreat(*this, pos, counter, op, out);
    }
    // ---- operation loader (MDT L6, bodies in src/engine/l6_slots.cpp) ----
    // vt+0x58  @src 0x1411b1240 (3D) / 0x1411aec20 (2D)
    void loadOperation(Project& p, Operation& op, Ctx& ctx, engine::FileEffects& fx) override
    {
        l6Slot(g_l6Slots.heatTreatLoad)(*this, p, op, ctx, fx);
    }
    // vt+0xb8  @src 0x1411b11d0 (3D) / 0x1411aebb0 (2D)
    void loadPrologue(Project& p, Operation& op) override { l6Slot(g_l6Slots.heatTreatPrologue)(*this, p, op); }
    // vt+0x78  @src 0x1411b1220 (3D) / 0x1411aec00 (2D): empty
    void loadClassData(Project&, Operation&) override {}
    // props vt+0x58  @src 0x1411bfd60
    void propsLoad(Project& p, Operation& op) override { l6Slot(g_l6Slots.heatTreatPropsLoad)(*this, p, op); }
    // vt+0xc0 (0x1405f1c00), vt+0x1b8 (0x1405f6520) and vt+0x1f8 (0x1413e5ef0, true) are base bodies
    // vt+0x80  @src 0x1411b1230 (3D) / 0x1411aec10 (2D): empty (MDT L6S)
    bool saveAfterFiles(Project&, Operation&, engine::FileEffects&) override { return true; }

private:
    void body(W& w, Project& prj, const String& db, int* sim, Operation* op, HeatTreatProps* p,
              int dbreadArg, int target);
};

// @src 0x1411b1460 (3D) / 0x1411aee40 (2D)
void HeatTreatClass::writeMst(W& w, Project& prj, const String& db, int* sim, Operation* op,
                              int dbreadArg, int target)
{
    auto* p = dynamic_cast<HeatTreatProps*>(propsOf(op));
    if (p) body(w, prj, db, sim, op, p, dbreadArg, target);
}

// @src 0x1411b1530 (3D) / 0x1411aef10 (2D): identical code and literals
void HeatTreatClass::body(W& w, Project& prj, const String& db, int* sim, Operation* op,
                          HeatTreatProps* p, int dbreadArg, int target)
{
    bool firstOp = isChainHead(*op) && target == 0;            // 0x140050bc0
    data::HTSchedule* sch = p->data ? &p->data->schedule : nullptr;   // 0x141681de0
    int n = sch ? data::htStepCount(*sch) : 0;                 // 0x1411b0a70
    bool needEmitters = true;
    if (n <= 0) return;
    // 0x1411b71c0(sch, false): writes HT_Proc<k>.MOKEY; not run (plan 2.1)
    for (int i = 0; i < n; ++i) {
        ++*sim;
        if (i >= sch->first - 1 && (target == 0 || *sim == target)) {   // 0x140514c40: +0x20
            emitSimHeader(w, *sim, op);
            if (!firstOp || i != 0) emitDbread(w, db, *sim, dbreadArg);
            w.ts << endl << "* Operation No " << *sim << endl;        // @0x142c8eba0
            if (i == 0 || needEmitters) {
                needEmitters = false;
                // The 4th argument is the loop index i, not *sim (disassembly 0x1411b189e).
                emitSimCtrl(w, prj, db, i, op);
                emitMaterials(w, prj, db, i, op);
                emitMeshSettings(w, prj, db, i, op);
                emitObjects(w, prj, db, i, op, false);
                emitContact(w, prj, db, i, op);
                emitObjpos(w, prj, db, i, op);
                emitContactFlags(w, prj, db, i, op);
                emitStateVars(w, prj, db, i, op);
            }
            w.ts << " * Keyword file which ECC data " << endl;           // @0x142c8ebb0
            w.ts << "KFREAD" << endl;                                    // @0x142c8cd04
            String keyAbs = htProcPath(*sch, i);
            bool pending = false;                                        // written by this Save (convert)
            for (const std::u16string& f : w.ctx.pendingWrites)
                if (qt::cleanPath(String(f)) == qt::cleanPath(keyAbs)) pending = true;
            if (!pending && !qt::fileExists(keyAbs))
                w.ctx.warn("HeatTreat: the MST refers to " + keyAbs.utf8() +
                           ", which the original writes during Save and which is missing");
            String keyPath = app::relToDb(db, keyAbs);                  // 0x140819590
            w.ts << qt::cleanPath(keyPath) << endl;                      // cleanDirPath
            const data::HTStep& st = *sch->steps[static_cast<size_t>(i)];   // 0x1411b0a90
            String uk = st.userKey;                                      // 0x1412c4c80: +0x118
            if (!uk.isEmpty() && !(uk == String(u"None"))) {             // @0x142c8d4d4
                w.ts << "KFREAD" << endl;                                // @0x142c8ebd0
                w.ts << qt::cleanPath(uk) << endl;
            }
            emitCursim(w, *sim, flowNumber(*op), 0);                     // 0x140050b10 always 0
            emitGendb(w, db, *sim, 2, false);
            if (target != 0) break;
        }
    }
}

// @src 0x1411bd570 / 0x1411bd6a0  SfOprHeatTreatProperties ctor: schedule 0x1411b3440 (first = 1,
// owner = the props)
std::shared_ptr<Props> HeatTreatClass::makeProps(int guiUnit) const
{
    auto p = std::make_shared<HeatTreatProps>();
    data::initCommonBlock(*p->common, guiUnit);    // base ctor 0x1405f8bb0 -> 0x1405fd250 (P15)
    p->tree = defaultPropTree(key());
    p->data = std::make_shared<data::HeatTreatData>();
    p->data->schedule.owner = p.get();
    return p;
}

}  // namespace

// @src 0x141681de0 -> 0x1411b0a70  schedule step count for P14's simulation list
bool simdata::heatTreatSteps(const HeatTreatProps& p, int* n)
{
    if (!p.data) return false;
    *n = data::htStepCount(p.data->schedule);
    return true;
}

std::unique_ptr<OpClass> makeHeatTreat(bool is2D)
{
    return std::make_unique<HeatTreatClass>(is2D);
}

}  // namespace m2m::mst
