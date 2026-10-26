// m2m/mst/loader_l4.hpp: engine bodies of class slots that MDT L4 ported for classes whose files
// are also built without the engine (standalone tests of src/ops/express.cpp): the Express family's
// vt+0x2c0 and vt+0x2b8, which its vt+0xc8 0x1413fbd10 calls (MDT DB-5, docs/re/moproj2mst/mdt/DB
// spec section 7; bodies in src/engine/express_sync.cpp, set by installLoaderSlots()).
#pragma once

#include "m2m/mst/writer.hpp"

namespace m2m::mst {

struct LoaderSlotsL4 {
    // SfOprExpressBase / SfOprFormingExpress* / SfOprHeatTransferExpress* vt+0x2c0 0x1413fc840:
    // whether the model has meshed dies the props' setting does not keep; their meshes are
    // deleted unless `keep`
    bool (*expressDieMeshes)(OpClass&, Project&, bool keep) = nullptr;
    // the same classes' vt+0x2b8 0x1413fc6b0: TRANS 2 for FormingExpress, TRANS 1 by the setting
    void (*expressTrans)(OpClass&, Project&) = nullptr;
    // their vt+0xc8 0x1413fbd10 (mst::expressSyncFromModelTail, src/ops/express.cpp), for the classes
    // in src/ops/family.cpp that are built without express.cpp
    void (*expressSyncTail)(OpClass&, Project&, Ctx&) = nullptr;
};
inline LoaderSlotsL4 g_loaderSlotsL4{};
// The answer to "Keep Mesh of Dies?" (0x1413fbd10): false = Yes, the dialog's default (what the
// converter answers); true = No, for tests that check that branch against oracle B 130n
inline bool g_keepMeshOfDiesNo = false;

template <class F> F l4Slot(F f)
{
    if (!f) loaderSlotsMissing();
    return f;
}

}  // namespace m2m::mst
