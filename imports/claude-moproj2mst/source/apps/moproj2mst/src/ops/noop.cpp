// SfOprGroup and SfOprDoeOutput: MST writers that write nothing (owner P13). Spec: s1-04 3.
//
// Both writers are `mov [rsp+8..0x20], rcx..r9; ret` (no .pdata): no output, *sim unchanged.
// Group members are not written by the group; the main walk (0x1400b6630, P03) decides what is.
#include <memory>
#include <vector>

#include "m2m/model.hpp"
#include "m2m/doe_output_data.hpp"

#include "m2m/mst/writer.hpp"

#include "l7.hpp"
#include "p13.hpp"
#if __has_include("m2m/mst/simcount.hpp")
#include "m2m/mst/simcount.hpp"
#define P13_HAVE_SIMCOUNT 1
#endif

namespace m2m::mst {
namespace {

class GroupClass : public OpClass {
public:
    // @src 0x141352020  init (registry key)
    GroupClass() : OpClass(String(u"SfOprGroup")) {}

    // @src 0x141352540
    void writeMst(W&, Project&, const String&, int*, Operation*, int, int) override {}
    // vt+0x298 is the base 0x1405f5880 (P14); vt+0x190 is the base 0x1405f5bf0; vt+0x98 (props)
    // is P02's.
    String displayName() const override;

    // ---- MDT L7: load slots (vt+0xe0, +0x1b0, +0x1b8, +0x1f8 are the base ones) -----------------
    // @src 0x1413524f0  vt+0x58: SfOprBase load 0x1405ef430
    void loadOperation(Project& p, Operation& op, Ctx& ctx, engine::FileEffects& fx) override
    {
        OpClass::loadOperation(p, op, ctx, fx);
    }
    // @src 0x141352490  vt+0xb8: SfOprBase 0x1405f1bb0
    void loadPrologue(Project& p, Operation& op) override { OpClass::loadPrologue(p, op); }
    // @src 0x1413524b0  vt+0x78: empty
    void loadClassData(Project&, Operation&) override {}
    // @src 0x1413524d0  vt+0xc0: SfOprBase 0x1405f1c00
    void postLoad(Project& p, Operation& op, Ctx& ctx, engine::FileEffects& fx) override
    {
        OpClass::postLoad(p, op, ctx, fx);
    }
    // @src 0x141352e70  SfOprGroupProperties vt+0x58: returns false, does nothing
    void propsLoad(Project&, Operation&) override {}
};

// @src 0x141352560  vt+0x88: tr("Cycle") (the string is "Cycle" in the binary)
String GroupClass::displayName() const
{
    return String(u"Cycle");
}

class DoeOutputClass : public OpClass {
public:
    // @src 0x140e2df30  init (registry key)
    DoeOutputClass() : OpClass(String(u"SfOprDoeOutput")) {}

    // @src 0x140e32970
    void writeMst(W&, Project&, const String&, int*, Operation*, int, int) override {}
#ifdef P13_HAVE_SIMCOUNT
    // vt+0x298 0x140e32990 (P14): empty, the operation adds no simulation entry
    void simList(int pos, int* counter, Operation* op, std::vector<SimEntry>& out) override
    {
        simListNone(*this, pos, counter, op, out);
    }
#endif
    String displayName() const override;
    // vt+0x98 (props) is P02's.
    int dim() const override;

    // ---- MDT L7: load slots (vt+0xe0, +0x1b8, +0x1f8 are the base ones) -------------------------
    void loadOperation(Project& p, Operation& op, Ctx& ctx, engine::FileEffects& fx) override;
    // @src 0x140e30ec0  vt+0xb8: SfOprBase 0x1405f1bb0
    void loadPrologue(Project& p, Operation& op) override { OpClass::loadPrologue(p, op); }
    void loadClassData(Project& p, Operation& op) override;
    // @src 0x140e30ee0  vt+0xc0: empty
    void postLoad(Project&, Operation&, Ctx&, engine::FileEffects&) override {}
    void propsLoad(Project& p, Operation& op) override;
    std::shared_ptr<engine::GuiModel> createModel(Project& p, Operation& op) override;

private:
    l7::TaskBlock60 b60_;          // +0x60 (ctor 0x1405e0ca0)
    std::int32_t fbc_ = 0;         // +0xbc DB step shown (0x140e31c30)
    doe::OutputClassState outputData_; //recovered e2ec40 class-private assignments
};

}  // namespace

namespace p13 {

// @src 0x1400b9b10  ids (0x140050910) of the operations of 0x1400b94f0, in that order
std::vector<int> subProjectOperationIds(const SubProject& sub)
{
    std::vector<int> ids;
    for (Operation* op : flowOperations(sub)) ids.push_back(op->id);
    return ids;
}

// @src 0x1400b5db0  last of those ids, -1 when there is none
int lastOperationId(const SubProject& sub)
{
    std::vector<int> ids = subProjectOperationIds(sub);
    return ids.empty() ? -1 : ids.back();
}

// @src 0x1400b82e0  that operation (sub+0x10 project, 0x1400a58b0 table, 0x140047dc0), or null
Operation* lastOperation(const SubProject& sub)
{
    int id = lastOperationId(sub);
    if (!sub.project) return nullptr;                     // 0x1400b5c40
    return operationById(*sub.project, id);
}

}  // namespace p13

namespace {

// @src 0x140e329d0  vt+0x190: dimension of the last operation of the current item's active
// sub-project (0x140e326f0 current->project, 0x140e32740 activeSubProject), 3 when there is none
int DoeOutputClass::dim() const
{
    Project* prj = current ? current->project : nullptr;  // 0x140e326f0
    SubProject* sub = prj ? activeSubProject(*prj) : nullptr;   // 0x140e32740 (0x1400a7390)
    if (!sub) return 3;
    Operation* op = p13::lastOperation(*sub);
    if (!op) return 3;
    return opDim(*op);                                    // 0x14005f0f0
}

// @src 0x140e2e320 vt+0x58. Show/reset are supported; session part/task/model
// processing remains guarded after the recovered document-read phase.
void DoeOutputClass::loadOperation(Project& p, Operation&, Ctx&, engine::FileEffects&)
{
    fbc_ = 0;                                            // OB+0xbc
    b60_.init();                                         // 0x1405e0d50(OB+0x60)
    // 0x140054820(current): messenger signals 1 and 2 (UI)
    Operation* cur = current;                            // 0x1405ef2b0
    if (!cur) l7::refuse("SfOprDoeOutput 0x140e2e320: no current item (the original dereferences it)");
    if (!g_loaderSlots.prevOp || !g_loaderSlots.opModel) loaderSlotsMissing();
    Operation* prev = g_loaderSlots.prevOp(p, *cur);     // 0x140062960
    engine::GuiModel* m = g_loaderSlots.opModel(p, *cur);   // 0x14005f770
    int dim = 3;
    if (!prev) {
        SubProject* sp = cur->project ? activeSubProject(*cur->project) : nullptr;   // 0x140055050
        if (!sp) l7::refuse("SfOprDoeOutput 0x140e2e320: no sub-project (0x1400b94f0 dereferences it)");
        std::vector<Operation*> ops = flowOperations(*sp);   // 0x1400b94f0
        // from the last one back: the first operation whose own project table holds its id
        for (int i = static_cast<int>(ops.size()) - 1; i >= 0; --i) {
            Operation* o = ops[static_cast<std::size_t>(i)];
            if (!o) continue;
            if (!l7::tableLookup(*o, o->id)) continue;   // 0x140055090(o)->vt[0x1c8](&id)
            dim = opDim(*o);                             // 0x14005f0f0
            break;
        }
    } else {
        dim = opDim(*prev);                              // 0x14005f0f0
    }
    if (!m) l7::refuse("SfOprDoeOutput 0x140e2e320: no engine model (0x140c7b720 dereferences it)");
    g_loaderSlots.resetModel(*m);                        // 0x140c7b720
    g_loaderSlots.setDimension(*m, dim);                 // 0x140c7acd0(m)->vt[0](&dim)
    if (!g_loaderSlots.showModel) loaderSlotsMissing();
    g_loaderSlots.showModel(p, m);                       // 0x140cf9360
    loadClassData(p, *cur);                              // vt+78, before global-task replacement
    l7::refuse("SfOprDoeOutput selected session/global task application remains unsupported");
}

// @src 0x140e2ec40  vt+0x78. Reached only from 0x140e2e320 after the viewer step. It reads the DOE
// output document of the sub-project (0x140e326a0 -> 0x1400b5ad0: SP+0x30, a GUI object created
// on first use by 0x140643100 with the main window as parent).
void DoeOutputClass::loadClassData(Project&, Operation&)
{
    Project* prj = current ? current->project : nullptr;     // 0x140e326f0
    if (!prj) return;
    SubProject* sp = activeSubProject(*prj);                 // 0x140e32740
    if (!sp) return;
    if (sp->type != 2 || !sp->doe || !sp->ownedDoeStudy || !prj->doeIo)
        l7::refuse("SfOprDoeOutput Type2 document needs owned study and configured I/O");
    if (!g_loaderSlots.readDoeDocument) loaderSlotsMissing();
    g_loaderSlots.readDoeDocument(*prj, *current);
    if (!g_loaderSlots.finishDoeOutputClassData) loaderSlotsMissing();
    const doe::OutputClassRefs refs{outputData_, b60_.f98, b60_.listA0, b60_.listA8};
    g_loaderSlots.finishDoeOutputClassData(*prj, *current, refs);
}

// @src 0x140e36290  SfOprDoeOutputProperties vt+0x58: when the props know their class (props+0x40)
// and the sub-project has a DOE study (SP+0x20), two study values go into the props data (+0x38);
// returns true. Neither props+0x40 nor that data is modelled, so a study refuses.
void DoeOutputClass::propsLoad(Project& p, Operation&)
{
    SubProject* sp = activeSubProject(p);                // 0x140e32740(props+0x40)
    if (sp && sp->doe)                                   // 0x1400b5a90
        l7::refuse("SfOprDoeOutputProperties 0x140e36290: DOE study values (0x14091e0b0, 0x14091e0f0) "
                   "into the props data (+0x38) are not modelled");
}

// @src 0x140e32a30  vt+0x1b0: as SfOprBase 0x1405f6410 without the TNOW 3 store
std::shared_ptr<engine::GuiModel> DoeOutputClass::createModel(Project& p, Operation&)
{
    Operation* opi = current;                            // 0x1405ef2b0
    if (!opi) return nullptr;
    if (!opi->project) return nullptr;                   // 0x140055030
    if (!g_loaderSlots.newModel) loaderSlotsMissing();
    return g_loaderSlots.newModel(p, dim());             // 0x140c7a5b0(P+8, vt+0x190), 0x140822210
}

// @src 0x140e31830  vt+0x88: tr("DOE Output")
String DoeOutputClass::displayName() const
{
    return String(u"DOE Output");
}

}  // namespace

std::unique_ptr<OpClass> makeGroup()
{
    return std::make_unique<GroupClass>();
}

std::unique_ptr<OpClass> makeDoeOutput()
{
    return std::make_unique<DoeOutputClass>();
}

}  // namespace m2m::mst
