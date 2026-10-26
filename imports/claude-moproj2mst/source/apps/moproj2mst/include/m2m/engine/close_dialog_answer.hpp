#pragma once

#include "m2m/engine.hpp"

#include <cstdint>
#include <functional>
#include <optional>
#include <string>
#include <vector>

namespace m2m::engine::close_dialog {

enum class ModalBoundary { CloseQuestion, LastOperationWarning };

// An explicit event schedule is required for each modal interval. A complete
// schedule may contain zero timer IDs, but only with a source-backed receipt.
struct ModalSchedule {
    ModalBoundary boundary;
    std::string sourceOccurrence;
    std::string qualification;
    std::vector<std::uint64_t> dueTimerIds;
};

enum class StyleFocusProfile {
    // Retained static contract: shipped Windows-family style, unchanged
    // initial focus; no runtime/custom-style claim.
    ShippedWindowsStyleInitialFocus,
    Unqualified
};

struct NestedSaveResult {
    DialogAnswer answer = DialogAnswer::NoButton;
    bool lastOperationWarning = false;
    std::optional<ModalSchedule> warningSchedule;
};

struct CloseInput {
    int processType = 0;
    bool hasProjectModel = false;
    StyleFocusProfile styleFocus = StyleFocusProfile::Unqualified;
    std::optional<DialogAnswer> selectedAnswer;
    ModalSchedule closeQuestionSchedule{
        ModalBoundary::CloseQuestion, {}, {}, {}};
};

enum class Outcome { CloseAllowed, CloseCancelled };

using DispatchModal = std::function<void(const ModalSchedule&)>;
using NestedSave = std::function<NestedSaveResult(const DispatchModal&)>;

// Isolated decision helper; it is intentionally not wired to public Close.
// It dispatches only explicitly qualified modal schedules, then applies the
// statically recovered Qt integer mapping. Nested Save owns its own effects.
Outcome resolve(const CloseInput&, const DispatchModal&, const NestedSave&);

} // namespace m2m::engine::close_dialog
