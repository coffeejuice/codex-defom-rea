// m2m/mst/loader_l6.hpp: operation-loader slot bodies of the HeatTreat, Distortion, Diestress and
// DiestressBM classes (packet MDT L6, docs/re/moproj2mst/mdt/L6/spec.md). The class overrides in
// src/ops/{heattreat,distortion,diestress}.cpp call through this table, which src/engine/l6_slots.cpp
// fills before the loader calls any slot, so the standalone tests of those files still link.
#pragma once

#include <memory>

#include "m2m/mst/writer.hpp"

namespace m2m::mst {

struct L6Slots {
    // SfOprHeatTreat / SfOprHeatTreat2D
    void (*heatTreatLoad)(OpClass&, Project&, Operation&, Ctx&, engine::FileEffects&) = nullptr;  // vt+0x58
    void (*heatTreatPrologue)(OpClass&, Project&, Operation&) = nullptr;                          // vt+0xb8
    void (*heatTreatPropsLoad)(OpClass&, Project&, Operation&) = nullptr;                         // props vt+0x58
    // SfOprDistortion
    void (*distortionPrologue)(OpClass&, Project&, Operation&) = nullptr;                         // vt+0xb8
    void (*distortionPostLoad)(OpClass&, Project&, Operation&, Ctx&, engine::FileEffects&) = nullptr;   // vt+0xc0
    void (*distortionPropsLoad)(OpClass&, Project&, Operation&) = nullptr;                        // props vt+0x58
    // SfOprDiestress
    void (*diestressPrologue)(OpClass&, Project&, Operation&) = nullptr;                          // vt+0xb8
    void (*diestressPostLoad)(OpClass&, Project&, Operation&, Ctx&, engine::FileEffects&) = nullptr;    // vt+0xc0
    void (*diestressLoadData)(OpClass&, Project&, Operation&, Ctx&, engine::FileEffects&) = nullptr;   // vt+0x1b8
    std::shared_ptr<engine::GuiModel> (*diestressCreateModel)(OpClass&, Project&, Operation&) = nullptr;  // vt+0x1b0
    // SfOprDiestressBM / BM2D / BM3D
    void (*diestressBMPrologue)(OpClass&, Project&, Operation&) = nullptr;                        // vt+0xb8
    void (*diestressBMClassData)(OpClass&, Project&, Operation&) = nullptr;                       // vt+0x78
    void (*diestressBMPostLoad)(OpClass&, Project&, Operation&, Ctx&, engine::FileEffects&) = nullptr;  // vt+0xc0
    void (*diestressBMLoadData)(OpClass&, Project&, Operation&, Ctx&, engine::FileEffects&) = nullptr;  // vt+0x1b8
    // vt+0xa8 object-tree fill (MDT L6S): Diestress 0x1402b6410, DiestressBM 0x140299c50
    bool (*dieStressTreeFill)(OpClass&, Project&, Operation&, engine::FileEffects&) = nullptr;
    // save-current-operation slots (MDT L6S, src/engine/save_l6.cpp)
    bool (*diestressSaveFiles)(OpClass&, Project&, Operation&, engine::FileEffects&) = nullptr;   // vt+0xe8 0x1402b5be0
    void (*diestressBMSaveNames)(OpClass&, Project&, Operation&, engine::FileEffects&) = nullptr; // vt+0xd8 0x140299870
    bool (*diestressBMSaveAfter)(OpClass&, Project&, Operation&, engine::FileEffects&) = nullptr; // vt+0x80 0x140299b60
    // SfOprHTFurnace's Load Pattern editor (MDT L6T, src/engine/htf_loadpattern.cpp): its ctor's
    // model part 0x14106e8b0 and its vt+0x1c0 0x141070870
    void (*loadPatternInit)(Project&, Operation&, engine::LoadPatternForm&) = nullptr;
    void (*loadPatternUpdate)(Project&, Operation&, const engine::LoadPatternForm&) = nullptr;
};
inline L6Slots g_l6Slots{};

// A slot of the table that is still null (the loader installs it first).
template <class F> F& l6Slot(F& f)
{
    if (!f) loaderSlotsMissing();
    return f;
}

}  // namespace m2m::mst
