// SfOprReport and SfOprShapeOptim (owner P16). Spec: s1-05 1.
//
// Both writers are `ret` after spilling their arguments (bytes checked in the exe copy): they write
// nothing and do not touch the simulation counter. Their simulation lists are empty too, so they
// consume no simulation numbers. The other slots are the base ones (+0x200 = 0x1405f79b0, false).
#include <cstdint>
#include <memory>
#include <vector>

#include "m2m/data/heattransfer.hpp"
#include "m2m/mst/simcount.hpp"
#include "m2m/mst/writer.hpp"
#include "m2m/props.hpp"

#include "l7.hpp"
#include "p13.hpp"
#include "shapeoptim.hpp"

namespace m2m::mst {
namespace {

class ReportClass : public OpClass {
public:
    using OpClass::OpClass;
    // @src 0x1412b05a0  vt+0x280: empty
    void writeMst(W&, Project&, const String&, int*, Operation*, int, int) override {}
    // vt+0x298 0x1412b05c0 (P14): empty
    void simList(int pos, int* counter, Operation* op, std::vector<SimEntry>& out) override
    {
        simListNone(*this, pos, counter, op, out);
    }

    // ---- MDT L7: load slots (vt+0xe0, +0x1b0, +0x1b8, +0x1f8 are the base ones) -----------------
    void loadOperation(Project& p, Operation& op, Ctx& ctx, engine::FileEffects& fx) override;
    // @src 0x1412af720  vt+0xb8: SfOprBase 0x1405f1bb0
    void loadPrologue(Project& p, Operation& op) override { OpClass::loadPrologue(p, op); }
    void loadClassData(Project& p, Operation& op) override;
    // @src 0x1412af740  vt+0xc0: empty
    void postLoad(Project&, Operation&, Ctx&, engine::FileEffects&) override {}
    void propsLoad(Project& p, Operation& op) override;
    // @src 0x1412b0600  vt+0x190: dimension of the last flow operation of the active sub-project
    // (0x1400b82e0, 0x14005f0f0), 3 without one
    int dim() const override
    {
        SubProject* sp = subProject();                   // 0x1412b1010
        if (!sp) return 3;
        Operation* o = p13::lastOperation(*sp);
        return o ? opDim(*o) : 3;
    }

private:
    // @src 0x1412b0fc0  project of the current item, or null
    Project* project() const { return current ? current->project : nullptr; }
    // @src 0x1412b1010  its active sub-project, or null
    SubProject* subProject() const { return project() ? activeSubProject(*project()) : nullptr; }
};


// ---- SfOprReport -----------------------------------------------------------------------------------

// @src 0x1412add20  vt+0x58. After vt+0x78 the original shows the coupon geometry files in the
// viewer (0x1412ae3d0), fills the report UI (0x1412b0660, mainWindow vt+0x7e8) and signals; none of
// that is reached here because vt+0x78 stops first.
void ReportClass::loadOperation(Project& p, Operation&, Ctx&, engine::FileEffects&)
{
    SubProject* sp = subProject();                       // 0x1412b1010
    if (!sp) return;
    Project* prj = project();                            // 0x1412b0fc0
    if (!prj) return;
    // 0x1400a58b0(prj): the operation table, always there
    loadClassData(p, *current);                          // vt+0x78
    l7::refuse("SfOprReport 0x1412add20: the steps after vt+0x78 are not reproduced");
}

// @src 0x1412ae070  vt+0x78: the report document of the sub-project (0x1412b0f70 -> 0x1400b5ad0:
// SP+0x30, created on first use by 0x140643100 with the main window as parent) reads
// "<project folder>/<project base name>.DS" (0x140644fd0, 0x140645dc0, 0x140644440(1, 1, 0))
void ReportClass::loadClassData(Project&, Operation&)
{
    if (!project()) return;                              // 0x1412b0fc0
    // 0x14008a4b0 (.moproj path), QFileInfo baseName / absolutePath: strings only
    if (!subProject()) return;                           // 0x1412b0f70 -> 0x1412b1010
    l7::refuse("SfOprReport 0x1412ae070 reads the report document (SP+0x30, 0x1400b5ad0 -> "
               "0x140643100, <project>.DS), a GUI object that is not reproduced");
}

// @src 0x1412b1f10  SfOprReportProperties vt+0x58: 0x1405fb720(props, 0), 0x1405fbae0(props, 0),
// 0x1405fc180(props, 0) read the engine model's sim control into the common block
void ReportClass::propsLoad(Project&, Operation&)
{
    l7::refuse("SfOprReportProperties 0x1412b1f10: the common-block readers 0x1405fb720, 0x1405fbae0 "
               "and 0x1405fc180 are not ported");
}

}  // namespace

// ---- SfOprShapeOptim -------------------------------------------------------------------------------

// @src 0x14013f5a0
void ShapeOptimClass::init()
{
    b60_.f6c = 1;                  // +0x6c
    fc4_ = 1;                      // +0xc4
    fc0_ = 0x3fe;                  // +0xc0
    fc8_ = 0;                      // +0xc8
    fcc_ = 200;                    // +0xcc
    fe0_ = 0.001;                  // +0xe0 = .rdata 0x1421b5050
    fe8_ = 0;                      // +0xe8
    fe9_ = 0;                      // +0xe9
    fd0_ = 1;                      // +0xd0
    b60_.init();                   // 0x1405e0d50(+0x60)
    listF0_.clear();               // 0x1400aced0(+0xf0)
}

// @src 0x140140140  operation number fd8_ of the sub-project's flow (0x1400b94f0, 1-based) when
// there is one, else the predecessor of the current item
Operation* ShapeOptimClass::stepOperation() const
{
    if (!current) l7::refuse("SfOprShapeOptim 0x140140140: no current item (0x140062960 dereferences it)");
    Operation* r = l7::prevOperation(*current);          // 0x140062960
    SubProject* sp = project() ? activeSubProject(*project()) : nullptr;   // 0x140055050
    if (!sp) l7::refuse("SfOprShapeOptim 0x140140140: no sub-project (0x1400b94f0 dereferences it)");
    std::vector<Operation*> ops = flowOperations(*sp);
    if (fd8_ > 0 && fd8_ <= static_cast<int>(ops.size())) r = ops[static_cast<std::size_t>(fd8_ - 1)];
    return r;
}

// @src 0x14013d740  vt+0x78: the optimisation study of the sub-project (SP+0x28, type 3 only) into
// the OB fields, then MaxSims / MinObjFuncVal of the "StopCritGroup" node into the props data
// (0x1401424e0: data+8 = +0xcc, 0x140142580: data+0x10 = +0xe0), then +0xb0's vt+0x58
void ShapeOptimClass::loadClassData(Project&, Operation&)
{
    Project* prj = project();                            // 0x14013f660
    if (!prj) return;
    SubProject* sp = activeSubProject(*prj);             // 0x1400a7390
    if (!sp) return;
    if (sp->type == 3)                                   // 0x1400b5ab0: created by the SP ctor 0x1400b5470
        l7::refuse("SfOprShapeOptim 0x14013d740: the optimisation study (SP+0x28, ctor 0x140917ff0) is "
                   "not modelled");
    // 0x1405ef2b0 -> 0x140051230, dynamic_cast to SfOprShapeOptimProperties (TD 0x1435b4c20)
    auto* pr = current ? dynamic_cast<ShapeOptimProps*>(current->props.get()) : nullptr;
    if (pr) {
        if (!pr->data) l7::refuse("SfOprShapeOptim 0x14013d740: props without data (+0x38)");
        // top-level properties (0x140353770 of props+0x10), then the sub-properties (0x140370f60) of
        // each named "StopCritGroup" (0x1421b4a30); the qDebug of each name is a log line
        for (const PropNode& top : pr->tree) {
            if (top.name.u16() != u"StopCritGroup") continue;
            for (const PropNode& n : top.children) {
                if (n.name.u16() == u"MaxSims")                  // 0x1421b4a40
                    pr->data->maxSims = fcc_;                    // 0x1401424e0(QVariant(int +0xcc))
                else if (n.name.u16() == u"MinObjFuncVal")       // 0x1421b4a48
                    pr->data->minObjFuncVal = fe0_;              // 0x140142580(QVariant(double +0xe0))
            }
        }
    }
    if (auto b0 = propsB0_.lock()) propsFromClass(*b0);  // +0xb0 vt+0x58 = 0x140142ca0
}

// @src 0x140142ca0  SfOprShapeOptimProperties vt+0x58: data+8 = OB+0xcc, data+0x10 = OB+0xe0 when
// the props know their class (+0x40)
void ShapeOptimClass::propsFromClass(ShapeOptimProps& pr)
{
    if (!pr.cls) return;
    if (!pr.data) l7::refuse("SfOprShapeOptimProperties 0x140142ca0: props without data (+0x38)");
    const auto& c = static_cast<const ShapeOptimClass&>(*pr.cls);
    pr.data->maxSims = c.fcc_;
    pr.data->minObjFuncVal = c.fe0_;
}

// @src 0x140142d00  SfOprShapeOptimProperties vt+0x60: OB+0xcc = data+8, OB+0xe0 = data+0x10 when
// the props know their class (+0x40)
void ShapeOptimClass::propsToClass(ShapeOptimProps& pr)
{
    if (!pr.cls) return;
    if (!pr.data) l7::refuse("SfOprShapeOptimProperties 0x140142d00: props without data (+0x38)");
    auto& c = static_cast<ShapeOptimClass&>(*pr.cls);
    c.fcc_ = pr.data->maxSims;
    c.fe0_ = pr.data->minObjFuncVal;
}

// props vt+0x58 (0x140054350) on the operation's props
void ShapeOptimClass::propsLoad(Project&, Operation& op)
{
    auto* pr = dynamic_cast<ShapeOptimProps*>(op.props.get());
    if (!pr) l7::refuse("SfOprShapeOptim: props vt+0x58 of an operation without SfOprShapeOptimProperties");
    propsFromClass(*pr);
}

// props vt+0x60 (0x1400543a0) on the operation's props
bool ShapeOptimClass::propsToEngine(Project&, Operation& op)
{
    auto* pr = dynamic_cast<ShapeOptimProps*>(op.props.get());
    if (!pr) l7::refuse("SfOprShapeOptim: props vt+0x60 of an operation without SfOprShapeOptimProperties");
    propsToClass(*pr);
    return true;
}

// @src 0x14013be10  vt+0x98: new SfOprShapeOptimProperties(vt+0x188, this) (0x140142620 keeps this
// at +0x40), remembered in the QPointer +0xb0 (0x140cbc920) and returned
std::shared_ptr<Props> ShapeOptimClass::makeProps(int guiUnit) const
{
    auto p = std::dynamic_pointer_cast<ShapeOptimProps>(OpClass::makeProps(guiUnit));
    if (!p) l7::refuse("SfOprShapeOptim 0x14013be10: the default props are not SfOprShapeOptimProperties");
    p->cls = const_cast<ShapeOptimClass*>(this);
    propsB0_ = p;
    return p;
}

// @src 0x14013fc20  vt+0x1b0: as SfOprBase 0x1405f6410 without the TNOW 3 store. vt+0x190 of the
// class is 0x14013fbf0 (opDim of 0x140140140).
std::shared_ptr<engine::GuiModel> ShapeOptimClass::createModel(Project& p, Operation&)
{
    Operation* opi = current;                            // 0x1405ef2b0
    if (!opi) return nullptr;
    if (!opi->project) return nullptr;                   // 0x140055030
    if (!g_loaderSlots.newModel) loaderSlotsMissing();
    return g_loaderSlots.newModel(p, dim());             // 0x140c7a5b0(P+8, vt+0x190), 0x140822210
}

std::unique_ptr<OpClass> makeReport()
{
    return std::make_unique<ReportClass>(qt::L("SfOprReport"));
}

std::unique_ptr<OpClass> makeShapeOptim()
{
    return std::make_unique<ShapeOptimClass>(qt::L("SfOprShapeOptim"));
}

}  // namespace m2m::mst
