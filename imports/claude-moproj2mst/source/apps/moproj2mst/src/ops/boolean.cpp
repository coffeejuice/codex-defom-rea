// SfOprBoolean (keys SfOprBoolean2D / SfOprBoolean3D): MST writer (owner P12). Spec: s1-04 1.
//
// The Object editor keeps its filled selections for Save; its engine-backed
// hooks and the side-file writer 0x1411e1820 are in engine/boolean_editor.cpp.
// The BWZ reader (0x1411e1dd0, open path) is P06's.
#include <memory>

#include "m2m/model.hpp"
#include "m2m/mst/emit.hpp"
#include "m2m/mst/loader_l5.hpp"
#include "m2m/mst/writer.hpp"
#include "m2m/props.hpp"
#include "m2m/data/common.hpp"
#include "m2m/qt4path.hpp"

#if __has_include("m2m/data/boolean.hpp")
#include "m2m/data/boolean.hpp"
#else
// Until P06 merges include/m2m/data/boolean.hpp (same member names, agreed with P06).
namespace m2m::data {
struct BooleanData {               // 0x18 bytes, ctor 0x1411e23f0
    int workpiece = 1;             // +0x04 WZD_MACHDIST_WORKPIECE_OBJECT_NO
    int cutter = 0;                // +0x08 WZD_BOOLEAN_CUTTER_OBJECT_NO
    std::uint8_t remesh = 0;       // +0x0c WZD_BOOLEAN_REMESH
};
}  // namespace m2m::data
#endif

#include "p12_common.hpp"

namespace m2m::mst {
namespace {

class BooleanClass : public OpClass {
public:
    // @src 0x1411de8d0  ctor: OB+0x60 = 1 (2D) / 0 (3D), then the class init 0x1411de9b0 (via
    // 0x1411de980): key (0x1405fd780) and dimension flags (0x1405fd8a0: info+0x80) by OB+0x60.
    // Icons, colours and texts of the init are GUI only.
    explicit BooleanClass(bool is2D)
        : OpClass(String(is2D ? u"SfOprBoolean2D" : u"SfOprBoolean3D")), is2D_(is2D)
    {
        dimFlags = is2D_ ? 0x1000099 : 0x100009a;
    }

    void writeMst(W& w, Project& prj, const String& db, int* sim, Operation* op, int dbreadArg,
                  int onlySim) override;
    std::shared_ptr<Props> makeProps(int guiUnit) const override;
    String displayName() const override;

    // MDT L5 loader slots (m2m/mst/loader_l5.hpp); vt+0x58, vt+0x1b8, props vt+0x58 and vt+0x1f8
    // stay on the SfOprBase bodies.
    // vt+0xb8  @src 0x1411def50
    void loadPrologue(Project& p, Operation& op) override { l5Slot(g_l5Slots.booleanPrologue)(*this, p, op); }
    // vt+0x78  @src 0x1411df000  empty
    void loadClassData(Project&, Operation&) override {}
    // vt+0xc0  @src 0x1411df020
    void postLoad(Project& p, Operation& op, Ctx&, engine::FileEffects& fx) override
    {
        l5Slot(g_l5Slots.booleanPostLoad)(*this, p, op, fx);
    }
    // The Object tree's first editor is factory 0x1411e0510 -> 0x1411e2480.
    // Its constructor fills the combo selections; no apply is inferred here.
    bool objectTreeFill(Project& p, Operation& op, engine::FileEffects&) override
    {
        return l5Slot(g_l5Slots.booleanEditorFill)(editor_, p, op);
    }
    bool updateDataInWidgetArea(Project& p, Operation& op, engine::FileEffects& fx) override
    {
        return l5Slot(g_l5Slots.booleanEditorApply)(editor_, p, op, fx);
    }

private:
    void writeBlock(W& w, Project& prj, const String& db, int* sim, Operation* item,
                    BooleanProps* bp, int dbreadArg, int onlySim);

    bool is2D_;                    // +0x60
    BooleanObjectEditor editor_;
};

// @src 0x1411e0aa0
String nameStepKey()
{
    return String(u"Step.KEY");
}

// @src 0x1411e0b10
String nameMachdistOutKey()
{
    return String(u"DEF_MACHDIST_OUT.KEY");
}

// @src 0x1411e0da0
String nameMachdistIni()
{
    String s;
    s = String(u"DEF_MACHDIST_CTRL2.INI");
    return s;
}

// @src 0x1411df600
void BooleanClass::writeMst(W& w, Project& prj, const String& db, int* sim, Operation* item,
                            int a7, int a8)
{
    Props* props = propsOf(item);
    auto* bp = dynamic_cast<BooleanProps*>(props);
    if (bp) writeBlock(w, prj, db, sim, item, bp, a7, a8);
}

// @src 0x1411df6d0
void BooleanClass::writeBlock(W& w, Project& prj, const String& db, int* sim, Operation* item,
                              BooleanProps* bp, int dbreadArg, int onlySim)
{
    using qt::cleanPath;
    String nm, tmp;
    ++*sim;
    if (onlySim == 0 || *sim == onlySim) {
        emitSimHeader(w, *sim, item);
        emitDbread(w, db, *sim, dbreadArg);
        emitMaterials(w, prj, db, 1, item);
        emitMeshSettings(w, prj, db, 1, item);
        emitObjects(w, prj, db, 1, item, false);
        emitObjpos(w, prj, db, *sim, item);
        emitKfreadUser(w, item);
        nm = item->name;                                     // 0x140050ac0
        emitOprnam(w, nm);
        emitSimnam(w, nm);
        emitCursim(w, *sim, flowNumber(*item), p12::cursimThird(*item));
        w.ts << "NSTEP" << "\t" << "0" << endl;
        const data::BooleanData& bd = *bp->data;             // 0x140025010: props+0x38
        int wp = bd.workpiece;                               // 0x14142ff20: +4
        p12::icbSetFlag(item->icb, wp, 1);
        p12::icbSetGen(item->icb, 0);
        emitContactFlags(w, prj, db, *sim, item);
        String stepKey = cleanPath(item->taskDir + qt::kSep + nameStepKey());
        w.ts << " * Keyword file which contains objects " << endl
             << keywordName(10) << endl
             << stepKey << endl
             << endl;
        // 0x1411e1820(BD): side files are written at this point of MST generation,
        // after KFWRIT/Step.KEY and before EXECMD; only a full Save supplies a sink.
        if (w.sideEffects)
            l5Slot(g_l5Slots.booleanSideFiles)(prj, *item, bd, w.ctx, *w.sideEffects);
        w.ts << "EXECMD" << "\t" << "4" << endl;
        String out = cleanPath(item->taskDir + qt::kSep + String::latin1("DEF_MACHDIST_CTRL2.OUT"));
        String ini = cleanPath(item->taskDir + qt::kSep + nameMachdistIni());
        w.ts << "DEF_MACHDIST_CTRL23.EXE" << "   " << ini << "  >  " << out << endl;
        w.ts << "KFREAD    \t" << 1 << endl;
        tmp = item->taskDir + qt::kSep + nameMachdistOutKey();
        w.ts << cleanPath(tmp) << endl;
        nm = displayName();                                  // vt+0x88, result unused
        emitGendb(w, db, *sim, 2, true);
    }
}

// @src 0x1411e0780  SfOprBooleanProperties ctor; 0x1411e0850 creates the BooleanData
// (0x1411e23f0: 1, 0, 0)
std::shared_ptr<Props> BooleanClass::makeProps(int guiUnit) const
{
    auto p = std::make_shared<BooleanProps>();
    data::initCommonBlock(*p->common, guiUnit);    // base ctor 0x1405f8ca0 -> 0x1405fd250 (P13, coordinator)
    p->tree = defaultPropTree(key());
    p->data = std::make_shared<data::BooleanData>();
    return p;
}

// @src 0x1411df2d0  vt+0x88
String BooleanClass::displayName() const
{
    return String(u"Boolean");
}

}  // namespace

std::unique_ptr<OpClass> makeBoolean(bool is2D)
{
    return std::make_unique<BooleanClass>(is2D);
}

}  // namespace m2m::mst
