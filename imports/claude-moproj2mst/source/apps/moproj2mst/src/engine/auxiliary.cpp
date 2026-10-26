#include "m2m/engine_auxiliary.hpp"
#include "m2m/engine.hpp"

namespace m2m::engine {
AuxiliaryDrawList& modelAuxiliaryDrawings(GuiModel& model)
{
    if (!model.auxiliaryDrawings)
        model.auxiliaryDrawings = std::make_unique<AuxiliaryDrawList>();
    return *model.auxiliaryDrawings;
}

void registerAuxiliaryDrawing(GuiModel& model, const void* identity)
{
    auto& entries = modelAuxiliaryDrawings(model).entries;
    for (auto it = entries.rbegin(); it != entries.rend(); ++it)
        if (*it == identity) return;
    entries.push_back(identity); // preserve existing duplicates, order, and raw identities
}

void clearAuxiliaryDrawings(GuiModel& model) noexcept
{
    model.auxiliaryDrawings.reset();
}
} // namespace m2m::engine
