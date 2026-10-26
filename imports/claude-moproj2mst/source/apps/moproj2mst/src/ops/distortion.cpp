// SfOprDistortion (keys SfOprDistortion2D / SfOprDistortion3D): MST writer (owner P13).
// Spec: s1-04 6; reader of the pass data: distortion_load.cpp.
//
// No corpus project has a Distortion operation: the writer is ported from the code and checked by
// the tier-1 test against the spec, not yet by an oracle.
// Not ported: the DB_STEP_DATA / Cls_1401e7b10 / Cls_14018b220 temporaries and dp->vfunc_0x70
// (0x1405fb6a0) at the start of the body; they are filled from properties and destroyed, and no
// output depends on them (s1-04 8). Also unused there: QString("Step.KEY") and the DB base name.
#include <memory>
#include <string>

#include "m2m/data/distortion.hpp"
#include "m2m/model.hpp"
#include "m2m/mst/emit.hpp"
#include "m2m/mst/writer.hpp"
#include "m2m/props.hpp"
#include "m2m/data/common.hpp"
#include "m2m/props_base.hpp"
#include "m2m/qt4path.hpp"

#if __has_include("m2m/mst/simcount.hpp")
#include "m2m/mst/simcount.hpp"
#define P13_HAVE_SIMCOUNT 1
#endif

#include "p13.hpp"
#include "m2m/mst/loader_l6.hpp"

namespace m2m::mst {
namespace {

class DistortionClass : public OpClass {
public:
    // @src 0x141361f70  ctor: OB+0x60 = 1 (2D) / 0 (3D); init 0x141362050 sets the key
    explicit DistortionClass(bool is2D)
        : OpClass(String(is2D ? u"SfOprDistortion2D" : u"SfOprDistortion3D")), is2D_(is2D)
    {
    }

    void writeMst(W& w, Project& prj, const String& db, int* sim, Operation* item, int dbreadArg,
                  int onlySim) override;
    std::shared_ptr<Props> makeProps(int guiUnit) const override;
    String displayName() const override;
    // vt+0x190 is the base 0x1405f5bf0.
    // ---- operation loader (MDT L6, bodies in src/engine/l6_slots.cpp); vt+0x58 is the base ----
    // vt+0xb8  @src 0x1413627d0
    void loadPrologue(Project& p, Operation& op) override { l6Slot(g_l6Slots.distortionPrologue)(*this, p, op); }
    // vt+0x78  @src 0x141362890: empty
    void loadClassData(Project&, Operation&) override {}
    // vt+0xc0  @src 0x1413628b0
    void postLoad(Project& p, Operation& op, Ctx& ctx, engine::FileEffects& fx) override
    {
        l6Slot(g_l6Slots.distortionPostLoad)(*this, p, op, ctx, fx);
    }
    // props vt+0x58  @src 0x1413665c0
    void propsLoad(Project& p, Operation& op) override { l6Slot(g_l6Slots.distortionPropsLoad)(*this, p, op); }
    // vt+0x80  @src 0x1413628a0: empty (MDT L6S)
    bool saveAfterFiles(Project&, Operation&, engine::FileEffects&) override { return true; }
#ifdef P13_HAVE_SIMCOUNT
    // vt+0x298 0x1413642d0 (P14)
    void simList(int pos, int* counter, Operation* op, std::vector<SimEntry>& out) override
    {
        simListDistortion(*this, pos, counter, op, out);
    }
#endif

private:
    bool is2D_;                    // +0x60
};

// "PS%02d" of QString::sprintf (width 2, zero fill; the sign counts in the width)
String passSuffix(int pass)
{
    std::string d = std::to_string(pass);
    if (d.size() < 2) d.insert(0, 2 - d.size(), '0');
    return String(u"PS") + String::latin1(d);
}

// cleanDirPath(taskRel + SEP + name)
String taskFile(const Operation& item, const String& name)
{
    return qt::cleanPath(item.taskDir + qt::kSep + name);
}

String decimal(int v)
{
    return String::latin1(std::to_string(v));
}

// @src 0x141362e90
void DistortionClass::writeMst(W& w, Project& prj, const String& db, int* sim, Operation* item,
                               int a7, int a8)
{
    auto* dp = dynamic_cast<DistortionProps*>(propsOf(item));   // TD 0x1439ee1b0 (no throw)
    if (dp) p13::writeDistortionPasses(*this, w, prj, db, sim, item, *dp, a7, a8);
}

// SfOprDistortionProperties ctor (not followed); DistortionPassData ctor 0x1413690c0 (token-reader base 0, FreeDistBcc 0x1411631e0, defaults 0x1413666a0, owner +0x5d8)
// The props builder (0x141366100 / 0x141365c90, tree ported by P07 behind defaultPropTree) then
// calls the base adders with these arguments (P07's reading of the builder).
std::shared_ptr<Props> DistortionClass::makeProps(int guiUnit) const
{
    auto p = std::make_shared<DistortionProps>();
    data::initCommonBlock(*p->common, guiUnit);    // base ctor 0x1405f8ca0 -> 0x1405fd250 (ran first)
    p->tree = defaultPropTree(key());
    p->data = std::make_shared<data::DistortionData>();
    base::addStepCounts(*p, false, false, 2, 1, qt::L("1.0 sec"));                    // 0x1405f9eb0
    base::addEnvTemp(*p, guiUnit, qt::L("68.00 F"), qt::L("20.00 C"));                 // 0x1405fa560
    base::addConvection(*p, guiUnit, qt::L("0.0 Btu/sec/in^2/F"), qt::L("0.0 N/sec/mm/C"));   // 0x1405faaa0
    base::addStepBy(*p, guiUnit, 0, qt::L("0.0 in"), qt::L("0.0 mm"), qt::L("0.0 sec"));      // 0x1405fb020
    return p;
}

// @src 0x141362b70  vt+0x88: tr("Machining Distortion")
String DistortionClass::displayName() const
{
    return String(u"Machining Distortion");
}

}  // namespace

namespace p13 {

// @src 0x141363fc0
String distortionSimnam(const OpClass& self, int stage, int pass, bool bare)
{
    Operation* cur = self.current;                       // 0x1405ef2b0, dereferenced unchecked
    if (!cur) throw Error(M2M_E_INPUT, "SfOprDistortion: SIMNAM without a current operation");
    String name = cur->name;                             // 0x140050ac0
    String suf;                                          // null QString
    if (stage == 0) suf = passSuffix(pass);              // "PS%02d"
    else if (stage == 1) suf = String(u"UNLOAD");
    else if (stage == 2) suf = String(u"LOAD");
    return bare ? suf : name + suf;
}

// @src 0x141362f60
void writeDistortionPasses(OpClass& self, W& w, Project& prj, const String& db, int* sim,
                           Operation* item, DistortionProps& dp, int dbreadArg, int onlySim)
{
    (void)is2D(*item);                                   // 0x14005f150, result unused
    const bool noPred = isChainHead(*item);              // 0x140050bc0
    String simnam;
    String outKey;
    const data::DistortionData& D = *dp.data;            // 0x1401cc6b0: *(dp+0x40)
    const int n = D.passCount;                           // 0x140cbc690
    if (n > 0)
        for (int i = 0; i <= n + 1; ++i) {
            if (D.firstPass > i) continue;               // 0x140cbc680
            ++*sim;
            if (!(onlySim == 0 || *sim == onlySim)) continue;
            emitSimHeader(w, *sim, item);
            w.ts << endl << "* Operation No " << *sim;
            if (i == 0) w.ts << " Original workpiece";
            else if (i == n + 1) w.ts << " Unload";
            else w.ts << " Pass No " << i;
            w.ts << endl;
            if (i == 0) {
                if (!noPred) emitDbread(w, db, *sim, dbreadArg);
                emitSimCtrl(w, prj, db, *sim, item);
                emitMaterials(w, prj, db, *sim, item);
                emitMeshSettings(w, prj, db, *sim, item);
                emitObjects(w, prj, db, *sim, item, false);
                emitContact(w, prj, db, *sim, item);
                emitObjpos(w, prj, db, *sim, item);
                emitContactFlags(w, prj, db, *sim, item);
                emitStateVars(w, prj, db, *sim, item);
            } else {
                emitDbread(w, db, *sim, dbreadArg);
                emitSimCtrl(w, prj, db, i, item);         // 4th argument = pass index
            }
            emitDoeGeometry(w, item);                     // 0x140132a40
            emitDoeVariables(w, item);                    // 0x1401322d0
            bool gflag = false;
            const int g4 = 2;
            int stage;
            if (i == n + 1) {
                stage = 1;
            } else if (i == 0) {
                stage = 2;
                w.ts << "NSTEP\t0" << endl;
                gflag = true;
            } else {
                stage = 0;
                // 0x1400600a0(item, no) = objItem(item, objId(item, no - 1)) (0x14005fc50)
                // The original indexes +0x3d8 without a bound; the reader keeps passCount <= 100.
                if (i - 1 >= static_cast<int>(D.passObjects.size()))
                    throw Error(M2M_E_INPUT, "SfOprDistortion: more than 100 passes");
                const int no = D.passObjects[static_cast<size_t>(i - 1)];   // 0x1413651f0
                Item* obj = objItem(*item, objId(*item, no - 1));
                if (obj) {
                    String k = qt::cleanPath(obj->keyPath);  // 0x14006ba60
                    if (!k.isEmpty()) {
                        k = app::relToDb(db, k);
                        w.ts << "KFREAD    \t" << 1 << endl;
                        w.ts << k << endl;                  // not cleanDirPath'ed
                    }
                }
            }
            simnam = distortionSimnam(self, stage, i, false);
            emitKfreadUser(w, item);                      // 0x140134680
            emitOprnam(w, item->name);                    // 0x140134730(0x140050ac0)
            emitSimnam(w, simnam);                        // 0x1401347e0
            emitCursim(w, *sim, flowNumber(*item), 0);    // 0x140134900
            w.ts << " * Keyword file which contains objects " << endl
                 << keywordName(10) << endl
                 << taskFile(*item, String(u"Step") + decimal(i) + String(u".KEY")) << endl   // 0x141366850 "Step%d.KEY"
                 << endl;
            w.ts << "EXECMD" << "\t" << "4" << endl;
            w.ts << "DEF_MACHDIST_CTRL23.EXE" << "   "
                 << taskFile(*item, String(u"DEF_MACHDIST_CTRL2.INI") + String(u"_") + qt::number(i))   // 0x141366c30
                 << "  >  " << taskFile(*item, String(u"DEF_MACHDIST_CTRL2.OUT")) << endl;
            w.ts << "KFREAD" << endl;
            outKey = item->taskDir + qt::kSep + String(u"DEF_MACHDIST_OUT") + decimal(i) + String(u".KEY");   // 0x1413668e0
            w.ts << qt::cleanPath(outKey) << endl;
            emitGendb(w, db, *sim, g4, gflag);
            if (onlySim) break;
        }
}

}  // namespace p13

#ifdef P13_HAVE_SIMCOUNT
namespace simdata {
// @src 0x1401cc6b0 -> 0x140cbc690  DistortionPassData pass count (+8)
int distortionPasses(const DistortionProps& p)
{
    return p.data ? p.data->passCount : 0;
}
}  // namespace simdata
#endif

std::unique_ptr<OpClass> makeDistortion(bool is2D)
{
    return std::make_unique<DistortionClass>(is2D);
}

}  // namespace m2m::mst
