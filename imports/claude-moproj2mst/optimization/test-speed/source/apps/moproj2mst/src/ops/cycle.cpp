// SfOprCycle (key SfOprCycle): MST writer (owner P13). Spec: s1-04 2.
//
// The cycle writes no block of its own: for k = 1..count it runs the writer of every member of the
// cycle rectangle, in flow order, with the cycle context on (OPRNAM = cycle name, SIMNAM prefix
// "CYCL<k>-", CURSIM middle value = the cycle's flow number; the emitters read mst::State).
// The count is CycleData.count from Cycle.CWZ (P06's reader 0x14134ec40).
//
// Rule (plan 2.1, s1-04 O-3): the original does not set the member class's "current item" before
// calling its writer, so it keeps whatever the GUI last set. Here it is the member being written.
#include <memory>
#include <vector>

#include "m2m/model.hpp"
#include "m2m/mst/writer.hpp"
#include "m2m/props.hpp"
#include "m2m/data/common.hpp"
#if __has_include("m2m/mst/simcount.hpp")
#include "m2m/mst/simcount.hpp"
#define P13_HAVE_SIMCOUNT 1
#endif

#if __has_include("m2m/data/cycle.hpp")
#include "m2m/data/cycle.hpp"
#else
// Until P06 merges include/m2m/data/cycle.hpp (same member name, agreed with P06).
namespace m2m::data {
struct CycleData {                 // SfOprCycleProperties+0x38, ctor 0x14134ef10
    int count = 5;                 // +0x00 WZD_CYCLE_COUNT
};
}  // namespace m2m::data
#endif

namespace m2m::mst {
namespace {

// @src 0x14134eeb0  CycleData.count (props+0x38)
int cycleCountOf(const CycleProps& p)
{
    return p.data ? p.data->count : 5;
}

class CycleClass : public OpClass {
public:
    // @src 0x14134d2e0  init (registry key)
    CycleClass() : OpClass(String(u"SfOprCycle")) {}

    void writeMst(W& w, Project& prj, const String& db, int* sim, Operation* item, int dbreadArg,
                  int onlySim) override;
    std::shared_ptr<Props> makeProps(int guiUnit) const override;
    String displayName() const override;
#ifdef P13_HAVE_SIMCOUNT
    // vt+0x298 0x14134e280 (P14)
    void simList(int pos, int* counter, Operation* op, std::vector<SimEntry>& out) override
    {
        simListCycle(*this, pos, counter, op, out);
    }
#endif
    int dim() const override;

    // ---- MDT L7: load slots. SfOprCycle's vt+0x58 / +0xb8 / +0xc0 call SfOprGroup's bodies
    // (0x1413524f0 / 0x141352490 / 0x1413524d0), which call SfOprBase's. vt+0x1b0, +0x1b8, +0x1f8
    // and the props vt+0x58 (0x1405fb600) are the base ones.
    // @src 0x14134d7b0  vt+0x58 -> 0x1413524f0 -> 0x1405ef430
    void loadOperation(Project& p, Operation& op, Ctx& ctx, engine::FileEffects& fx) override
    {
        OpClass::loadOperation(p, op, ctx, fx);
    }
    // @src 0x14134d750  vt+0xb8 -> 0x141352490 -> 0x1405f1bb0
    void loadPrologue(Project& p, Operation& op) override { OpClass::loadPrologue(p, op); }
    // @src 0x14134d770  vt+0x78: empty
    void loadClassData(Project&, Operation&) override {}
    // @src 0x14134d790  vt+0xc0 -> 0x1413524d0 -> 0x1405f1c00
    void postLoad(Project& p, Operation& op, Ctx& ctx, engine::FileEffects& fx) override
    {
        OpClass::postLoad(p, op, ctx, fx);
    }
    engine::GuiModel* classModel(Project& p, Operation& op) override;

private:
    Operation* firstMemberWithModel(Operation* cur) const;
};

// @src 0x14134daf0  vt+0xe0 model(opi): the engine model of the cycle's first member
// (0x14005f820, created when missing), null when there is no member. The project is the main
// window's (0x1402c8090 / 0x14001d510), the one being converted.
engine::GuiModel* CycleClass::classModel(Project& p, Operation& op)
{
    SubProject* sub = op.project ? activeSubProject(*op.project) : nullptr;   // 0x140055050
    if (!sub) return nullptr;
    std::vector<int> ids = groupMembers(*sub, op.id);     // 0x1400b7470(sub, 0x140050910(op))
    if (ids.empty()) return nullptr;                      // 0x140004ae0
    Operation* it = operationById(p, ids[0]);             // 0x1400a58b0(p), 0x140047dc0
    if (!it) return nullptr;
    if (!g_loaderSlots.opModel) loaderSlotsMissing();
    return g_loaderSlots.opModel(p, *it);                 // 0x14005f820
}

// @src 0x14134daf0  vt+0xe0 model(cur): the engine model of the first member of the cycle
// (0x14005f820). The port has no engine model object (P15's hasEngineModel answers whether one
// exists), so this returns that member, or null.
Operation* CycleClass::firstMemberWithModel(Operation* cur) const
{
    Project* prj = cur->project;                          // 0x1402c8090 / 0x14001d510: the project
    if (!prj) return nullptr;
    SubProject* sub = activeSubProject(*prj);            // 0x140055050
    if (!sub) return nullptr;
    std::vector<int> ids = groupMembers(*sub, cur->id);   // 0x1400b7470
    if (ids.empty()) return nullptr;
    Operation* it = operationById(*prj, ids[0]);          // 0x140047dc0
    if (!it || !hasEngineModel(*it)) return nullptr;      // 0x14005f820
    return it;
}

// @src 0x14134dc50  vt+0x190: dimension of the first member's engine model, else the base answer
int CycleClass::dim() const
{
    if (current) {
        if (Operation* m = firstMemberWithModel(current)) {
            // @src 0x140c7fee0  3, or the LLOBJ (0x140c7acd0) dimension +0x1c (0x140100980) when the
            // model has one. A fresh model (0x1405f6410) always has it, set from the member
            // class's vt+0x190 (0x140821bf0): P15's engineFacts answers that.
            return engineFacts(*m).dim;
        }
    }
    return OpClass::dim();
}

// @src 0x14134dcc0
void CycleClass::writeMst(W& w, Project& prj, const String& db, int* sim, Operation* item, int a7,
                          int a8)
{
    if (!item) return;
    Props* props = item->props.get();                    // 0x140051230
    if (!props) return;
    // dynamic_cast<SfOprCycleProperties*> (TD 0x143ae2588), not null-checked by the original.
    auto* cp = dynamic_cast<CycleProps*>(props);
    if (!cp) throw Error(M2M_E_INPUT, "SfOprCycle: operation " + item->name.utf8() + " has no cycle properties");
    // 0x1400a58b0(prj): the operation table; always present here.
    SubProject* sub = item->project ? activeSubProject(*item->project) : nullptr;   // 0x140055050
    if (!sub) return;
    std::vector<int> ids = groupMembers(*sub, item->id);  // 0x1400b7470(sub, 0x140050910(item))
    int n = cycleCountOf(*cp);                            // 0x14134eeb0
    w.st.setCycleMode(flowNumber(*item), item->name);     // 0x140136af0(0x140050b20, 0x140050ac0)
    for (int k = 0; k < n; ++k) {
        w.st.setCycleIdx(k + 1);                          // 0x140136b40
        const std::vector<int> members = ids;             // Q_FOREACH works on a copy
        for (int id : members) {
            Operation* it = operationById(prj, id);       // 0x140047dc0
            if (!it) continue;
            OpClass* h = opBase(*it);                     // 0x140054e90
            if (!h) continue;
            Operation* saved = h->current;                // rule above (not in the original)
            h->current = it;
            h->writeMst(w, prj, db, sim, it, a7, a8);     // member's own vt+0x280
            h->current = saved;
        }
    }
    w.st.clearCycleMode();                                // 0x140136b30
}

// @src 0x14134e890  SfOprCycleProperties ctor; 0x14134e960 creates the CycleData (0x14134ef10)
std::shared_ptr<Props> CycleClass::makeProps(int guiUnit) const
{
    auto p = std::make_shared<CycleProps>();
    data::initCommonBlock(*p->common, guiUnit);    // base ctor 0x1405f8ca0 -> 0x1405fd250 (ran first)
    p->tree = defaultPropTree(key());
    p->data = std::make_shared<data::CycleData>();
    return p;
}

// @src 0x14134e0b0  vt+0x88: tr("Cycle") (no translator installed)
String CycleClass::displayName() const
{
    return String(u"Cycle");
}

}  // namespace

#ifdef P13_HAVE_SIMCOUNT
namespace simdata {
// @src 0x14134eeb0  (P14 seam; G1: drop its stub line from src/stubs/p14.cpp at merge)
int cycleCount(const CycleProps& p)
{
    return cycleCountOf(p);
}
}  // namespace simdata
#endif

std::unique_ptr<OpClass> makeCycle()
{
    return std::make_unique<CycleClass>();
}

}  // namespace m2m::mst
