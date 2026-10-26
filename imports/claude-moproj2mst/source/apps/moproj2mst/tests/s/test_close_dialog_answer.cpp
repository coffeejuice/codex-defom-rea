// Isolated source-qualified Close answer mapping; no public Close binding.
// ctest-labels: m2m-s
#include <testkit/check.hpp>
#include "m2m/engine/close_dialog_answer.hpp"

#include <stdexcept>
#include <vector>

using namespace m2m::engine;
using namespace m2m::engine::close_dialog;

namespace {
ModalSchedule closeSchedule()
{
    return {ModalBoundary::CloseQuestion,
            "0x140044110:current-operation-question", "static-profile-close-001", {}};
}
ModalSchedule warningSchedule()
{
    return {ModalBoundary::LastOperationWarning,
            "0x1400b94b3:last-operation-warning", "static-profile-warning-001", {}};
}
}

TEST_CASE(close_guard_bypass_does_not_require_dialog_schedule)
{
    CloseInput in; in.hasProjectModel = false; in.processType = 5;
    CHECK(resolve(in, {}, {}) == Outcome::CloseAllowed);
    in.hasProjectModel = true; in.processType = 3;
    CHECK(resolve(in, {}, {}) == Outcome::CloseAllowed);
}

TEST_CASE(shipped_initial_focus_yes_enters_nested_save_and_maps_yes_to_cancel)
{
    CloseInput in; in.hasProjectModel = true; in.processType = 5;
    in.styleFocus = StyleFocusProfile::ShippedWindowsStyleInitialFocus;
    in.closeQuestionSchedule = closeSchedule();
    std::vector<ModalBoundary> delivered;
    const auto dispatch = [&](const ModalSchedule& s) { delivered.push_back(s.boundary); };
    bool saveCalled = false;
    const NestedSave save = [&](const DispatchModal& modalDispatch) {
        saveCalled = true;
        const auto warning = warningSchedule();
        modalDispatch(warning); // modal events precede returning the warning answer
        return NestedSaveResult{DialogAnswer::Yes, true, warning};
    };
    CHECK(resolve(in, dispatch, save) == Outcome::CloseCancelled);
    CHECK(saveCalled);
    CHECK_EQ(delivered.size(), 2u);
    CHECK(delivered[0] == ModalBoundary::CloseQuestion);
    CHECK(delivered[1] == ModalBoundary::LastOperationWarning);
}

TEST_CASE(unqualified_style_needs_answer_and_missing_schedule_fails_closed)
{
    CloseInput in; in.hasProjectModel = true; in.processType = 5;
    in.closeQuestionSchedule = closeSchedule();
    const auto noSave = [](const DispatchModal&) { return NestedSaveResult{}; };
    bool threw = false;
    try { (void)resolve(in, [](const ModalSchedule&) {}, noSave); }
    catch (const std::runtime_error&) { threw = true; }
    CHECK(threw);

    in.styleFocus = StyleFocusProfile::ShippedWindowsStyleInitialFocus;
    in.closeQuestionSchedule.qualification.clear();
    threw = false;
    try { (void)resolve(in, [](const ModalSchedule&) {}, noSave); }
    catch (const std::runtime_error&) { threw = true; }
    CHECK(threw);
}

TEST_CASE(warning_result_requires_pre_answer_modal_schedule)
{
    CloseInput in; in.hasProjectModel = true; in.processType = 5;
    in.styleFocus = StyleFocusProfile::ShippedWindowsStyleInitialFocus;
    in.closeQuestionSchedule = closeSchedule();
    const auto dispatch = [](const ModalSchedule&) {};
    const NestedSave missing = [](const DispatchModal&) {
        return NestedSaveResult{DialogAnswer::Yes, true, std::nullopt};
    };
    bool threw = false;
    try { (void)resolve(in, dispatch, missing); }
    catch (const std::runtime_error&) { threw = true; }
    CHECK(threw);

    const NestedSave wrongBoundary = [](const DispatchModal& modalDispatch) {
        auto bad = warningSchedule(); bad.sourceOccurrence = "unqualified";
        modalDispatch(bad);
        return NestedSaveResult{DialogAnswer::Yes, true, bad};
    };
    threw = false;
    try { (void)resolve(in, dispatch, wrongBoundary); }
    catch (const std::runtime_error&) { threw = true; }
    CHECK(threw);
}

TEST_CASE(non_yes_nested_save_answer_is_not_remapped)
{
    CloseInput in; in.hasProjectModel = true; in.processType = 5;
    in.styleFocus = StyleFocusProfile::ShippedWindowsStyleInitialFocus;
    in.closeQuestionSchedule = closeSchedule();
    const auto dispatch = [](const ModalSchedule&) {};
    const NestedSave save = [](const DispatchModal&) {
        return NestedSaveResult{DialogAnswer::No, false, std::nullopt};
    };
    CHECK(resolve(in, dispatch, save) == Outcome::CloseAllowed);
}

int main() { return testkit::run_all(); }
