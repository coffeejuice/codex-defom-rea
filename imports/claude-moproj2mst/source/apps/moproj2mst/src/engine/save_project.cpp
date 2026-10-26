// File > Save 0x1400980d0 up to the .moproj (owner: L3, coordinator 2026-10-22): the open check
// (P+0x55), the per sub-project check 0x1400b8fb0, step 2 (0x140052ec0 on the current operation)
// and step 3 (0x1400983c0(P, 0)); saveCurrent stops before step 3. Steps 4 (0x1400982e0,
// properties vt+0xb8) and 5 (0x1400a20d0, the MST) are in src/core/convert.cpp. Spec: docs/re/moproj2mst/mdt/spec.md section 1.
#include "m2m/engine.hpp"
#include "m2m/doe_io.hpp"
#include "m2m/model.hpp"
#include "m2m/moproj_write.hpp"

namespace m2m::engine {

namespace {

// The item info's second flag word: item ISfDiagItem vt+8 (0x140160d20) is the class's
// SfOprDiagItemInfo, whose ISfDiagItemInfo vt+0x60 (0x1405fed60) reads data+0x84 (zeroed by
// 0x1405fe440, set by 0x1405fd8d0). Only three class registrations set it, all to 3:
// @src 0x14013ab80 (SfOprShapeOptim), 0x140e2df30 (SfOprDoeOutput), 0x1412ad930 (SfOprReport).
bool mustBeLast(const String& itemClass)
{
    const std::u16string& c = itemClass.u16();
    return c == u"SfOprShapeOptim" || c == u"SfOprDoeOutput" || c == u"SfOprReport";
}

// @src 0x1400b8fb0  per sub-project check. An item of the connector chain (0x140084ad0) whose flag
// (info vt+0x60 & 2) is set and that is not the last one makes Save ask "Operation \"%1\" must be
// the last operation. Do you wish to correct this?" The shipped enum overload
// supplies only Yes, with absent No as requested default; fresh default is Yes.
// Modal event delivery before that answer remains unrecovered and guarded.
void lastOperationCheck(const Project& p, const SubProject& sub, SaveCallState& call)
{
    const std::vector<FlowItem*> chain = flowChain(sub.flow);
    for (std::size_t k = 0; k + 1 < chain.size(); ++k) {
        if (!mustBeLast(chain[k]->className)) continue;
        const Operation* op = operationById(p, chain[k]->opId);
        call.lastOperationModalPending = true;
        call.pendingDefault = DialogAnswer::Yes;
        throw Error(M2M_E_UNSUPPORTED, "save: operation \"" + (op ? op->name.utf8() : std::string()) +
                                           "\" must be the last operation (0x1400b8fb0 warning modal delivery not recovered)");
    }
}

}  // namespace

bool saveCurrent(Project& p, Ctx& ctx, SaveEffects& out)
{
    out.current = nullptr;
    out.call = SaveCallState{};
    out.call.opened = p.openedOk;
    if (!p.openedOk) {
        out.call.answer = DialogAnswer::No;
        return false;                                             // 0x1400a4090 (P+0x55)
    }
    for (const auto& sub : p.subs)                                  // 0x1400a7360 / 0x1400a74f0
        if (sub) {
            lastOperationCheck(p, *sub, out.call);
            out.call.answer = DialogAnswer::No;                    // 0b8fb0 normal return
            ++out.call.checkedSubprojects;
        }
    out.call.validationComplete = true;
    // @src 0x1400a58b0 -> 0x140047a40 / 0x140047dc0  the current item of the diagram model: the
    // one the open made current (0x140047f80, openActiveOperation)
    if (SubProject* sp = activeSubProject(p)) {
        const FlowChart& flow = sp->flow;
        if (flow.activeItem >= 0 && static_cast<std::size_t>(flow.activeItem) < flow.items.size())
            if (const FlowItem* it = flow.items[static_cast<std::size_t>(flow.activeItem)].get())
                out.current = operationById(p, it->opId);
    }
    if (out.current) saveCurrentOperation(p, *out.current, ctx, out.step2);   // 0x140052ec0
    return true;
}

bool saveProject(Project& p, Ctx& ctx, moproj::DiagramViews& views, const FileEffects& open, SaveEffects& out)
{
    if (!saveCurrent(p, ctx, out)) return false;
    // The caller owns SaveEffects across the writer's refresh guard. Keep a
    // read-only view of immediate XML I/O rather than scheduling a second write.
    out.project.xmlJournal = p.doeIo ? p.doeIo->xmlJournalOwner() : nullptr;
    out.call.projectWriteAttempted = true;
    out.call.projectWriteSucceeded = moproj::saveProjectFile(
        p, false, views, {&open, &out.step2}, out.project, ctx.session); // 0x1400983c0(P, 0)
    out.call.projectWriteReturned = true;
    return out.call.projectWriteSucceeded;
}

}  // namespace m2m::engine
