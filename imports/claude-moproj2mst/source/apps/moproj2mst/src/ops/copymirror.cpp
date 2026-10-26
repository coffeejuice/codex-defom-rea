// SfOprCopyMirror: MST writer (owner P12). Spec: s1-04 5. No corpus project (spec tests only).
//
// The native writer calls properties vt+0x60 0x140e2d230 before its block writer. mirrormerge.ini
// is only named here, never written.
#include <memory>

#include "m2m/data/common.hpp"
#include "m2m/data/convert.hpp"
#include "m2m/model.hpp"
#include "m2m/mst/emit.hpp"
#include "m2m/mst/loader_l5.hpp"
#include "m2m/mst/writer.hpp"
#include "m2m/props.hpp"
#include "m2m/qt4path.hpp"

#include "p12_common.hpp"

namespace m2m::mst {
namespace {

struct CopyMirrorProps : Props {   // SfOprCopyMirrorProperties (TD 0x1439c1d88)
    std::shared_ptr<data::ConvertData> data;   // +0x38 (0x140e2c100: ctor 0x140e2d400)
    // qt_metacall 0x140e2dbf0 -> qt_static_metacall 0x140e2d500 set/get slots on the data block, then the base (L2)
    void applyValue(PropNode& node, const std::vector<PropNode*>& ancestors, const qt::Variant& v) override
    {
        if (!data || !slots::convertSet(*data, node.name.u16(), v)) Props::applyValue(node, ancestors, v);
    }
    bool getValue(const PropNode& node, const std::vector<PropNode*>&, qt::Variant& out) override
    {
        if (data && slots::convertGet(*data, node.name.u16(), out)) return true;
        return base::getValue(*this, node, out);
    }
};

String nameStepKey();   // 0x140e24ac0 (below)

class CopyMirrorClass : public OpClass {
public:
    // @src 0x140e22470  class init: key (0x1405fd780) and dimension flags (0x1405fd8a0); the rest
    // of the init is GUI only
    CopyMirrorClass() : OpClass(String(u"SfOprCopyMirror")) { dimFlags = 0x2100009a; }

    void writeMst(W& w, Project& prj, const String& db, int* sim, Operation* op, int dbreadArg,
                  int onlySim) override;
    std::shared_ptr<Props> makeProps(int /*guiUnit*/) const override;
    String displayName() const override;

    // MDT L5 loader slots (m2m/mst/loader_l5.hpp); vt+0x1b8 and vt+0x1f8 (false) stay on the
    // SfOprBase bodies.
    // vt+0x58  @src 0x140e24b80
    void loadOperation(Project& p, Operation& op, Ctx& ctx, engine::FileEffects& fx) override
    {
        l5Slot(g_l5Slots.cmLoad)(*this, p, op, ctx, fx);
    }
    // vt+0xb8  @src 0x140e22900
    void loadPrologue(Project& p, Operation& op) override { l5Slot(g_l5Slots.cmPrologue)(*this, p, op); }
    // vt+0x78  @src 0x140e22940  empty
    void loadClassData(Project&, Operation&) override {}
    // vt+0xc0  @src 0x140e24b50
    void postLoad(Project& p, Operation& op, Ctx& ctx, engine::FileEffects& fx) override
    {
        l5Slot(g_l5Slots.cmPostLoad)(*this, l5_, p, op, ctx, fx);
    }
    // props vt+0x58 through 0x140054350: @src 0x140e2d080
    void propsLoad(Project& p, Operation& op) override
    {
        if (!op.props) return;
        auto* cp = dynamic_cast<CopyMirrorProps*>(op.props.get());
        if (!cp || !cp->data)
            throw Error(M2M_E_UNSUPPORTED, "operation load: SfOprCopyMirror properties of another class");
        l5Slot(g_l5Slots.cmPropsLoad)(p, op, *cp->data);
    }
    // vt+0x2a0  @src 0x140e25980
    bool provideObject(Project& p, int no, def::model::LLOBJ_DATA* dst) override
    {
        return l5Slot(g_l5Slots.cmProvideObject)(*this, l5_, p, no, dst);
    }

    // MDT L5S save slots; vt+0x68, vt+0xd0, vt+0xd8 stay on SfOprBase
    // vt+0xe8  @src 0x140e24cf0
    bool saveOperationFiles(Project& p, Operation& op, engine::FileEffects& fx) override
    {
        l5Slot(g_l5Slots.cmSaveFiles)(*this, nameStepKey(), p, op, fx);
        return true;
    }
    // vt+0x80  @src 0x140e24b30 -> 0x140e23d80
    bool saveAfterFiles(Project& p, Operation& op, engine::FileEffects& fx) override
    {
        l5Slot(g_l5Slots.cmSaveAfter)(*this, l5_, nameStepKey(), p, op, fx);
        return true;
    }
    // props vt+0x60  @src 0x140e2d230
    bool propsToEngine(Project& p, Operation& op) override
    {
        auto* cp = dynamic_cast<CopyMirrorProps*>(op.props.get());
        if (!cp || !cp->data) throw Error(M2M_E_UNSUPPORTED, "save: SfOprCopyMirror properties of another class");
        l5Slot(g_l5Slots.cmPropsToEngine)(p, op, *cp->data);
        return true;
    }

private:
    CopyMirrorState l5_;
    void writeBlock(W& w, Project& prj, const String& db, int* sim, Operation* item,
                    CopyMirrorProps* cp, int dbreadArg, int onlySim);
};

// @src 0x140e24ac0
String nameStepKey()
{
    return String(u"Step.KEY");
}

// @src 0x140e24e80
void CopyMirrorClass::writeMst(W& w, Project& prj, const String& db, int* sim, Operation* item,
                               int a7, int a8)
{
    auto* cp = dynamic_cast<CopyMirrorProps*>(propsOf(item));
    if (cp) {
        // 0x140e24e80 calls the matching properties object's vt+0x60 before entering 0x140e24f60.
        // Keep this before writeBlock's simulation increment/onlySim filter.
        // The standalone P12 writer test has no engine slot table; the full
        // converter installs the slot table before any modeled writer call.
        if (g_l5Slots.cmPropsToEngine) (void)propsToEngine(prj, *item);
        writeBlock(w, prj, db, sim, item, cp, a7, a8);
    }
}

// @src 0x140e24f60
void CopyMirrorClass::writeBlock(W& w, Project& prj, const String& db, int* sim, Operation* item,
                                 CopyMirrorProps*, int dbreadArg, int onlySim)
{
    using qt::cleanPath;
    String ini = String::latin1("mirrormerge.ini");
    // 0x14005f150, 0x140050bc0 and convertSeparators(QFileInfo(db).baseName()) are computed, unused
    String nm;
    String outKey;
    ++*sim;
    if (onlySim == 0 || *sim == onlySim) {
        emitSimHeader(w, *sim, item);
        emitDbread(w, db, *sim, dbreadArg);
        emitSimCtrl(w, prj, db, 1, item);
        emitMaterials(w, prj, db, 1, item);
        emitMeshSettings(w, prj, db, 1, item);
        emitObjects(w, prj, db, 1, item, false);
        emitKfreadUser(w, item);
        nm = item->name;                                     // 0x140050ac0
        emitOprnam(w, nm);
        emitSimnam(w, nm);
        emitCursim(w, *sim, flowNumber(*item), p12::cursimThird(*item));
        p12::icbSetGen(item->icb, 0);
        emitContactFlags(w, prj, db, *sim, item);
        String stepKey = cleanPath(item->taskDir + qt::kSep + nameStepKey());
        w.ts << " * Keyword file which contains objects " << endl
             << keywordName(10) << endl
             << stepKey << endl
             << endl;
        w.ts << "EXECMD" << "\t" << "3" << endl;
        String out = cleanPath(item->taskDir + qt::kSep + String::latin1("DEF_MIRRORMERGE.OUT"));
        String inip = cleanPath(item->taskDir + qt::kSep + ini);
        w.ts << "DEF_MIRRORMERGE.EXE" << "   " << inip << "  >  " << out << endl;
        w.ts << "KFREAD" << endl;
        outKey = item->taskDir + qt::kSep + String::latin1("DEF_MIRRORMERGE_OUT.KEY");
        w.ts << cleanPath(outKey) << endl;
        nm = displayName();                                  // vt+0x88, result unused
        emitGendb(w, db, *sim, 2, true);
        w.ts << "ARMCTL\t0\t0\t" << *sim + 1 << endl;
    }
}

std::shared_ptr<Props> CopyMirrorClass::makeProps(int guiUnit) const
{
    auto p = std::make_shared<CopyMirrorProps>();
    data::initCommonBlock(*p->common, guiUnit);    // base ctor 0x1405f8ca0 -> 0x1405fd250 (ran first)
    p->data = std::make_shared<data::ConvertData>();   // 0x140e2c100 -> 0x140e2d400
    p->tree = defaultPropTree(key());
    return p;
}

// @src 0x140e27350  vt+0x88
String CopyMirrorClass::displayName() const
{
    return String(u"Copy/Mirroring");
}

}  // namespace

std::unique_ptr<OpClass> makeCopyMirror()
{
    return std::make_unique<CopyMirrorClass>();
}

}  // namespace m2m::mst
