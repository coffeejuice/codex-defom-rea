// m2m/mst/loader_l3.hpp: own loader slots of SfOprForming(2D), the FormingExpress family
// (SfOprFormingExpressCommon: SfOprFormingExpress, SfOprFormingExpress2D) and the HeatTransferExpress
// family (SfOprExpressBase: SfOprHeatTransferExpress, SfOprHeatTransferExpress2D) (packet MDT L3,
// docs/re/moproj2mst/mdt/L3/spec.md). The classes in src/ops/family.cpp and src/ops/express.cpp
// override the OpClass slots and call these bodies (src/engine/loader_l3.cpp), which
// installLoaderSlots() sets; standalone test builds that do not link the engine never reach them
// (as with g_loaderSlots).
#pragma once

#include "m2m/mst/writer.hpp"

namespace m2m::mst {

struct LoaderSlotsL3 {
    // SfOprForming / SfOprForming2D (same bodies apart from their string literals)
    void (*formingLoadPrologue)(OpClass&, Project&, Operation&) = nullptr;   // 0x140c060f0 / 0x140c04910
    void (*formingPostLoad)(OpClass&, Project&, Operation&, Ctx&, engine::FileEffects&) = nullptr;   // 0x140c06130 / 0x140c04950
    // SfOprFormingProperties vt+0x58
    void (*formingPropsLoad)(OpClass&, Project&, Operation&) = nullptr;      // 0x140c09da0
    // SfOprFormingExpressCommon (SfOprFormingExpress / 2D use these slots unchanged)
    void (*feLoadPrologue)(OpClass&, Project&, Operation&) = nullptr;        // 0x14141bc90
    void (*fePostLoad)(OpClass&, Project&, Operation&, Ctx&, engine::FileEffects&) = nullptr;   // 0x14141bda0
    // SfOprFormingExpressProperties vt+0x58
    void (*fePropsLoad)(OpClass&, Project&, Operation&) = nullptr;           // 0x14142a6a0
    // SfOprHeatTransferExpress / 2D (same bodies)
    void (*hteLoadPrologue)(OpClass&, Project&, Operation&) = nullptr;       // 0x1413e6360 / 0x1413e5aa0
    void (*htePostLoad)(OpClass&, Project&, Operation&, Ctx&, engine::FileEffects&) = nullptr;  // 0x1413e63a0 / 0x1413e5ae0
    // SfOprHeatTransferExpressProperties vt+0x58
    void (*htePropsLoad)(OpClass&, Project&, Operation&) = nullptr;          // 0x1413ed890
    // vt+0x228 object-tree add of SfOprHeatTransferExpress / 2D (MDT L3S)
    void (*hteObjectTreeAdd)(OpClass&, Project&, Operation&) = nullptr;      // 0x1413e6520 / 0x1413e5c60
    // Forming / Forming2D at Save: the "Simulation controls" editor (SfPreSimControl) fill 0x1417ec030
    // and apply 0x1417ec870
    // 300 constructor Open fill 0x140eb26f0 -> 0x140eb5630 -> 0x140ec19f0,
    // recovered CG toggled callback 0x140ed01b0 only; other widget behavior is incomplete.
    void (*formingSimControl300Open)(Project&, Operation&) = nullptr;
    bool (*formingSimControlApply)(OpClass&, Project&, Operation&, engine::FileEffects&) = nullptr;
};
inline LoaderSlotsL3 g_loaderSlotsL3{};

template <class F> F l3Slot(F f)
{
    if (!f) loaderSlotsMissing();
    return f;
}

}  // namespace m2m::mst
