#pragma once
#include <vector>

namespace m2m::engine {
struct GuiModel;

// ISfAuxDraw* values in the model's lazy +250 QList wrapper. The wrapper owns
// list storage only. Entries can outlive their pointees and are never dereferenced
// here; manager deletion must not silently expire or prune their identities.
struct AuxiliaryDrawList {
    std::vector<const void*> entries;
};

AuxiliaryDrawList& modelAuxiliaryDrawings(GuiModel&); // c91070
void registerAuxiliaryDrawing(GuiModel&, const void*); // c91070 -> d64780
void clearAuxiliaryDrawings(GuiModel&) noexcept; // c91120; not part of reset()
} // namespace m2m::engine
