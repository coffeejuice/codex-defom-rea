// SfOprConvert2Dto3D: MST writer (owner P12). Spec: s1-04 4. No corpus project (spec tests only).
//
// Not ported, by plan 2.1, because no MST byte depends on them:
//   - props vt+0x60 0x1401ce590 (applies the properties to the operation model, O-5);
//   - the DB_STEP_DATA / Cls_1401e7b10 / Cls_14018b220 temporaries (0x140821bf0, 0x1408d3bc0,
//     0x14085f3f0, 0x1408d3cd0, 0x1408e0200, props vt+0x70) that are filled and destroyed unused;
//   - the deletions 0x1407fbc10 of ./DEF_CNV23.DAT (0x1401c6e00), ./DEF_REM.INI, ./REMESH.SCR;
//   - the status signal 0x1402cf3c0 "2signalSetProcessStatusFilename()" with CONV_STATUS.DAT
//     (0x1401c6c50, 0x140818f10).
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

struct Convert2Dto3DProps : Props {   // SfOprConvert2Dto3DProperties (TD 0x1435d68a8)
    std::shared_ptr<data::ConvertData> data;   // +0x38 (0x1401cd460: ctor 0x140e2d400)
    // qt_metacall 0x1401e8df0 -> qt_static_metacall 0x1401e8700 set/get slots on the data block, then the base (L2)
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

class Convert2Dto3DClass : public OpClass {
public:
    // @src 0x1401c24d0  class init: key (0x1405fd780) and dimension flags (0x1405fd8a0); the rest
    // of the init is GUI only
    Convert2Dto3DClass() : OpClass(String(u"SfOprConvert2Dto3D")) { dimFlags = 0x1100009d; }

    void writeMst(W& w, Project& prj, const String& db, int* sim, Operation* op, int dbreadArg,
                  int onlySim) override;
    std::shared_ptr<Props> makeProps(int /*guiUnit*/) const override;

    // MDT L5 loader slots (m2m/mst/loader_l5.hpp); vt+0x1b8 and vt+0x1f8 (true) stay on the
    // SfOprBase bodies.
    // vt+0x58  @src 0x1401c3a60
    void loadOperation(Project& p, Operation& op, Ctx& ctx, engine::FileEffects& fx) override
    {
        l5Slot(g_l5Slots.c23Load)(*this, l5_, taskFile("SfOprConvert2Dto3D_TEMP_STEP.KEY"), p, op, ctx, fx);
    }
    // vt+0xb8  @src 0x1401c2970
    void loadPrologue(Project& p, Operation& op) override { l5Slot(g_l5Slots.c23Prologue)(*this, p, op); }
    // vt+0x78  @src 0x1401c29b0  empty
    void loadClassData(Project&, Operation&) override {}
    // vt+0xc0  @src 0x1401c29d0
    void postLoad(Project& p, Operation& op, Ctx& ctx, engine::FileEffects& fx) override
    {
        l5Slot(g_l5Slots.c23PostLoad)(*this, l5_, taskFile("SfOprConvert2Dto3D_TEMP_STEP.KEY"), p, op, ctx, fx);
    }
    // props vt+0x58 through 0x140054350: @src 0x1401ce3e0
    void propsLoad(Project& p, Operation& op) override
    {
        if (!op.props) return;
        auto* cp = dynamic_cast<Convert2Dto3DProps*>(op.props.get());
        if (!cp || !cp->data)
            throw Error(M2M_E_UNSUPPORTED, "operation load: SfOprConvert2Dto3D properties of another class");
        l5Slot(g_l5Slots.c23PropsLoad)(p, op, *cp->data);
    }

    // MDT L5S save slots; vt+0x68, vt+0xd0, vt+0xd8 stay on SfOprBase
    // vt+0xe8  @src 0x1401c3be0
    bool saveOperationFiles(Project& p, Operation& op, engine::FileEffects& fx) override
    {
        l5Slot(g_l5Slots.c23SaveFiles)(*this, l5_, taskFile("M23_INPUT.KEY"),   // 0x1401c68c0
                                       taskFile("SfOprConvert2Dto3D_TEMP_STEP.KEY"),   // 0x1401c6a70
                                       taskFile("CONV_STATUS.DAT"), p, op, fx);   // 0x1401c6c50
        return true;
    }
    // signalUpdateDataInWidgetArea -> the selected editor's vt+0x1c0;
    // the caller then runs props vt+0x58.
    bool updateDataInWidgetArea(Project& p, Operation& op, engine::FileEffects&) override
    {
        return l5Slot(g_l5Slots.c23UpdateData)(l5_, p, op);
    }
    // vt+0xc8  @src 0x1401c3570 (MDT L5S; the caller 0x1400538f0 is MDT's DB packet)
    void syncFromModelTail(Project& p, Operation&, Ctx&, engine::FileEffects& fx) override
    {
        l5Slot(g_l5Slots.c23SyncTail)(*this, l5_, taskFile("SfOprConvert2Dto3D_TEMP_STEP.KEY"), p, fx);
    }
    // vt+0x80  @src 0x1401c29c0  empty
    bool saveAfterFiles(Project&, Operation&, engine::FileEffects&) override { return true; }
    // props vt+0x60  @src 0x1401ce590
    bool propsToEngine(Project& p, Operation& op) override
    {
        auto* cp = dynamic_cast<Convert2Dto3DProps*>(op.props.get());
        if (!cp || !cp->data)
            throw Error(M2M_E_UNSUPPORTED, "save: SfOprConvert2Dto3D properties of another class");
        l5Slot(g_l5Slots.c23PropsToEngine)(p, op, *cp->data);
        return true;
    }

private:
    Convert2Dto3DState l5_;
    void writeBlock(W& w, Project& prj, const String& db, int* sim, Operation* item,
                    Convert2Dto3DProps* cp, int dbreadArg, int onlySim);
    String taskFile(const char* name) const;
};

// cleanPath(taskDirAbs(current) + sep + name); the directory part is "" without a current item
// (literals 0x1421ff727 / 0x1421ff74d). Body shared by 0x1401c68c0, 0x1401c6f20, 0x1401c6c50.
String Convert2Dto3DClass::taskFile(const char* name) const
{
    String s;
    String dir = String(u"");
    if (Operation* cur = current) dir = taskDirAbs(*cur);
    s = dir + qt::kSep + String::latin1(name);
    return qt::cleanPath(s);
}

// @src 0x1401c4440
void Convert2Dto3DClass::writeMst(W& w, Project& prj, const String& db, int* sim, Operation* item,
                                  int a7, int a8)
{
    Props* props = propsOf(item);
    auto* cp = dynamic_cast<Convert2Dto3DProps*>(props);
    if (cp) {
        // cp->vt+0x60 (0x1401ce590): model update, not ported (see the file comment)
        writeBlock(w, prj, db, sim, item, cp, a7, a8);
    }
}

// @src 0x1401c4520
void Convert2Dto3DClass::writeBlock(W& w, Project& prj, const String& db, int* sim,
                                    Operation* item, Convert2Dto3DProps*, int dbreadArg,
                                    int onlySim)
{
    using qt::cleanPath;
    bool first = startsFromKey(*item);       // 0x14005f230 (0x14005f150 is called too, unused)
    // DB_STEP_DATA tmp(3), sim-control and movement temporaries, deletions: not ported
    String p23;
    ++*sim;
    if (onlySim == 0 || *sim == onlySim) {
        emitSimHeader(w, *sim, item);
        if (!first) emitDbread(w, db, *sim, dbreadArg);
        else emitSimCtrl(w, prj, db, *sim, item);
        emitMaterials(w, prj, db, *sim, item);
        emitObjects(w, prj, db, *sim, item, false);
        // @src 0x1401c68c0
        String m23 = taskFile("M23_INPUT.KEY");
        m23 = app::relToDb(db, m23);
        w.ts << "KFREAD" << endl;
        w.ts << cleanPath(m23) << endl;
        w.ts << "KFREAD" << endl;
        // @src 0x1401c6a70 -> 0x1401c6f20
        p23 = taskFile("SfOprConvert2Dto3D_TEMP_STEP.KEY");
        p23 = app::relToDb(db, p23);
        w.ts << cleanPath(p23) << endl;
        emitOprnam(w, item->name);
        emitSimnam(w, item->name);
        emitCursim(w, *sim, flowNumber(*item), p12::cursimThird(*item));
        w.ts << "NSTEP" << "\t" << "0" << endl;
        emitGendb(w, db, *sim, 2, true);
    }
    // CONV_STATUS.DAT status signal: not ported
}

std::shared_ptr<Props> Convert2Dto3DClass::makeProps(int guiUnit) const
{
    auto p = std::make_shared<Convert2Dto3DProps>();
    data::initCommonBlock(*p->common, guiUnit);    // base ctor 0x1405f8ca0 -> 0x1405fd250 (ran first)
    p->data = std::make_shared<data::ConvertData>();   // 0x1401cd460 -> 0x140e2d400
    p->tree = defaultPropTree(key());
    return p;
}

}  // namespace

std::unique_ptr<OpClass> makeConvert2Dto3D()
{
    return std::make_unique<Convert2Dto3DClass>();
}

}  // namespace m2m::mst
