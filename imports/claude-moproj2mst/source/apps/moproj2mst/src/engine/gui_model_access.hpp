#pragma once

// Private migration seam for synchronous native calls. Legacy public fields
// remain outside this converted seam. A callback must not retain its borrowed
// step; C++ capture/alias escape is still a code-review boundary.
#include <functional>
#include <type_traits>
#include <utility>

#include "m2m/engine.hpp"

namespace m2m::engine {
namespace detail {
template<class F>
concept GuiModelStepMutationCallback = std::is_invocable_v<F, DB_STEP_DATA*> &&
    (std::is_void_v<std::invoke_result_t<F, DB_STEP_DATA*>> ||
     std::is_same_v<std::invoke_result_t<F, DB_STEP_DATA*>, bool>);
} // namespace detail

struct GuiModelAccess {
    template<class F> requires detail::GuiModelStepMutationCallback<F>
    static auto withStep(GuiModel& model, F&& callback) -> std::invoke_result_t<F, DB_STEP_DATA*>
    {
        model.requireGeometrySyncAvailable(); // before acquiring the actual borrow
        // Preserve the caller's nullable-step behavior. No owner/default is made.
        return std::invoke(std::forward<F>(callback), model.step.get());
    }
};
} // namespace m2m::engine
