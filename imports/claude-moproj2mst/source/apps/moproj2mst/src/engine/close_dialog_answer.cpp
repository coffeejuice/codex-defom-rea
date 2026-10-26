#include "m2m/engine/close_dialog_answer.hpp"

#include <algorithm>
#include <stdexcept>
#include <vector>

namespace m2m::engine::close_dialog {
namespace {

constexpr const char* closeQuestionOccurrence = "0x140044110:current-operation-question";
constexpr const char* warningOccurrence = "0x1400b94b3:last-operation-warning";

void requireSchedule(const ModalSchedule& schedule, ModalBoundary expected,
                    const DispatchModal& dispatch)
{
    const char* occurrence = expected == ModalBoundary::CloseQuestion
        ? closeQuestionOccurrence : warningOccurrence;
    if (schedule.boundary != expected || schedule.sourceOccurrence != occurrence ||
        schedule.qualification.empty() || !dispatch)
        throw std::runtime_error("Close decision lacks a source-qualified modal event schedule");
}

} // namespace

Outcome resolve(const CloseInput& input, const DispatchModal& dispatch,
                const NestedSave& nestedSave)
{
    // Original 044110's immediate-allow guard precedes the dialog.
    if (!input.hasProjectModel || (input.processType != 1 && input.processType != 5))
        return Outcome::CloseAllowed;

    requireSchedule(input.closeQuestionSchedule, ModalBoundary::CloseQuestion, dispatch);
    dispatch(input.closeQuestionSchedule);

    // For this specific recovered style/focus profile, the normal three-button
    // question defaults to Yes. Other profiles must supply an actual answer.
    DialogAnswer answer = DialogAnswer::NoButton;
    if (input.selectedAnswer) {
        answer = *input.selectedAnswer;
    } else if (input.styleFocus == StyleFocusProfile::ShippedWindowsStyleInitialFocus) {
        answer = DialogAnswer::Yes;
    } else {
        throw std::runtime_error("Close question answer is not qualified for this style/focus profile");
    }

    if (answer == DialogAnswer::Yes) {
        if (!nestedSave) throw std::runtime_error("Close Yes branch has no nested Save result");
        std::vector<ModalSchedule> dispatchedWarnings;
        const DispatchModal guardedDispatch = [&](const ModalSchedule& schedule) {
            if (schedule.boundary == ModalBoundary::LastOperationWarning) {
                requireSchedule(schedule, ModalBoundary::LastOperationWarning, dispatch);
                dispatch(schedule);
                dispatchedWarnings.push_back(schedule);
                return;
            }
            throw std::runtime_error("nested Save supplied an unexpected modal boundary");
        };
        const NestedSaveResult saved = nestedSave(guardedDispatch);
        if (saved.lastOperationWarning) {
            if (!saved.warningSchedule)
                throw std::runtime_error("last-operation modal lacks an explicit event schedule");
            const bool delivered = std::any_of(dispatchedWarnings.begin(), dispatchedWarnings.end(),
                [&](const ModalSchedule& value) {
                    return value.sourceOccurrence == saved.warningSchedule->sourceOccurrence &&
                           value.qualification == saved.warningSchedule->qualification &&
                           value.dueTimerIds == saved.warningSchedule->dueTimerIds;
                });
            if (!delivered)
                throw std::runtime_error("nested Save returned before its warning schedule was dispatched");
        } else if (!dispatchedWarnings.empty()) {
            throw std::runtime_error("nested Save dispatched a warning not present in its result");
        }
        // Original 044110 remaps exactly QMessageBox::Yes to Cancel.
        if (saved.answer == DialogAnswer::Yes) answer = DialogAnswer::Cancel;
        else answer = saved.answer;
    }

    return answer == DialogAnswer::Cancel ? Outcome::CloseCancelled : Outcome::CloseAllowed;
}

} // namespace m2m::engine::close_dialog
