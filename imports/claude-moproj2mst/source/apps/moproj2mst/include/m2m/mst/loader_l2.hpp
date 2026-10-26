// m2m/mst/loader_l2.hpp: own loader slots of the HeatTransfer family, SfOprHTFurnace and
// SfOprPre(2D) (packet MDT L2, docs/re/moproj2mst/mdt/L2/spec.md). The classes in
// src/ops/family.cpp override the OpClass slots and call these bodies (src/engine/loader_l2.cpp),
// which installLoaderSlots() sets; standalone test builds that do not link the engine never reach
// them (as with g_loaderSlots).
#pragma once

#include "m2m/mst/writer.hpp"

namespace m2m::mst {

struct LoaderSlotsL2 {
    // SfOprHeatTransferCommon (SfOprHeatTransfer / 2D forward to these)
    void (*htLoadPrologue)(OpClass&, Project&, Operation&) = nullptr;                     // 0x141354ca0
    void (*htPostLoad)(OpClass&, Project&, Operation&, Ctx&, engine::FileEffects&) = nullptr;  // 0x141354ce0
    // SfOprHeatTransferProperties vt+0x58
    void (*htPropsLoad)(OpClass&, Project&, Operation&) = nullptr;                        // 0x141357520
    // SfOprHTFurnace
    void (*furnaceLoadPrologue)(OpClass&, Project&, Operation&) = nullptr;                // 0x140c657b0
    void (*furnacePostLoad)(OpClass&, Project&, Operation&, Ctx&, engine::FileEffects&) = nullptr;  // 0x140c65830
    // SfOprHTFurnaceProperties vt+0x58
    void (*furnacePropsLoad)(OpClass&, Project&, Operation&) = nullptr;                   // 0x140c69480
    // SfOprPreProperties vt+0x58
    void (*prePropsLoad)(OpClass&, Project&, Operation&) = nullptr;                       // 0x14134c380
    // SfOprHTFurnace vt+0x228 (object-tree add; the thermal BC reset of its objects)
    void (*furnaceObjectTreeAdd)(OpClass&, Project&, Operation&) = nullptr;               // 0x140c66230
};
inline LoaderSlotsL2 g_loaderSlotsL2{};

}  // namespace m2m::mst
