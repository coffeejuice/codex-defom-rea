// m2m/mst/loader_l1.hpp: own loader slots of SfOprCogging (packet MDT L1 part A,
// docs/re/moproj2mst/mdt/L1/spec_L1A.md). The class in src/ops/cogging.cpp overrides the OpClass
// slots and calls these bodies (src/engine/cogging_load.cpp), which installLoaderSlots() sets;
// standalone test builds that do not link the engine never reach them (as with g_loaderSlots).
#pragma once

#include "m2m/mst/writer.hpp"

namespace m2m::mst {

struct LoaderSlotsL1 {
    void (*cogLoadPrologue)(OpClass&, Project&, Operation&) = nullptr;                        // 0x140750650
    void (*cogPostLoad)(OpClass&, Project&, Operation&, Ctx&, engine::FileEffects&) = nullptr;  // 0x1407507b0
    void (*cogPropsLoad)(OpClass&, Project&, Operation&) = nullptr;                           // 0x14075ee20
    void (*cogSaveNames)(OpClass&, Project&, Operation&, engine::FileEffects&) = nullptr;
    bool (*cogProcessUpdate)(Project&, Operation&) = nullptr;   // SfCoggingProcess vt+0x1c0 0x140789410 (L4)                           // 0x14075c260 (L1B)
};
inline LoaderSlotsL1 g_loaderSlotsL1{};

}  // namespace m2m::mst
