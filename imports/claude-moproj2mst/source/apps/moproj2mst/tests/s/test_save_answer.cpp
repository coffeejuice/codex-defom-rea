// Original Save answer state is independent of adapter helper/file outcomes.
// ctest-labels: m2m-s
#include <testkit/check.hpp>
#include "m2m/engine.hpp"
#include "m2m/moproj_write.hpp"
#include "m2m/mst/writer.hpp"
using namespace m2m;
using Answer = engine::DialogAnswer;

TEST_CASE(unopened_save_returns_no_without_reaching_validation_or_writes)
{
    Project p; Ctx ctx; engine::SaveEffects out;
    CHECK(!engine::saveCurrent(p, ctx, out));
    CHECK(out.call.answer == Answer::No);
    CHECK(!out.call.opened && !out.call.validationComplete);
    CHECK(!out.call.projectWriteAttempted && !out.call.mstWriteAttempted);
}
TEST_CASE(opened_save_without_nonnull_subprojects_keeps_yes)
{
    Project p; p.openedOk = true; p.subs.push_back(nullptr);
    Ctx ctx; engine::SaveEffects out;
    CHECK(engine::saveCurrent(p, ctx, out));
    CHECK(out.call.answer == Answer::Yes);
    CHECK_EQ(out.call.checkedSubprojects, 0u);
    CHECK(out.call.validationComplete && out.current == nullptr);
}
TEST_CASE(checked_save_keeps_no_when_project_writer_returns_false)
{
    Project p; p.openedOk = true; p.subs.push_back(makeSubProject(p, 0, 0));
    Ctx ctx; engine::SaveEffects out; engine::FileEffects opened;
    auto views = moproj::guiViews(p);
    // Original project writer rejects empty path, independently of Save answer.
    CHECK(!engine::saveProject(p, ctx, *views, opened, out));
    CHECK(out.call.answer == Answer::No);
    CHECK_EQ(out.call.checkedSubprojects, 1u);
    CHECK(out.call.validationComplete && out.call.projectWriteAttempted);
    CHECK(!out.call.projectWriteSucceeded);
}
TEST_CASE(last_operation_warning_keeps_pending_default_separate_from_returned_answer)
{
    Project p; p.openedOk = true;
    p.subs.push_back(makeSubProject(p, 0, 0));
    p.subs.push_back(makeSubProject(p, 0, 1));
    auto& flow = p.subs[1]->flow;
    auto first = std::make_unique<FlowItem>(); first->className = qt::L("SfOprReport");
    auto next = std::make_unique<FlowItem>(); next->col = 1;
    first->next = next.get(); next->prev = first.get();
    flow.items.push_back(std::move(first)); flow.items.push_back(std::move(next));
    Ctx ctx; engine::SaveEffects out;
    bool guarded = false;
    try { (void)engine::saveCurrent(p, ctx, out); }
    catch (const Error& e) { guarded = e.code == M2M_E_UNSUPPORTED; }
    CHECK(guarded);
    CHECK_EQ(out.call.checkedSubprojects, 1u);
    CHECK(out.call.answer == Answer::No); // prior check; modal has not returned
    CHECK(out.call.lastOperationModalPending && out.call.pendingDefault == Answer::Yes);
    CHECK(!out.call.validationComplete && !out.call.projectWriteAttempted);
    CHECK(out.step2.writes.empty() && out.project.writes.empty());
}
int main() { return testkit::run_all(); }
