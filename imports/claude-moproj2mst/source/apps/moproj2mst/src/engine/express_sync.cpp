// MDT L4 (DB-5): the Express family's vt+0x2c0 and vt+0x2b8, called by its vt+0xc8 0x1413fbd10
// (src/ops/express.cpp) at the end of the DB / restart sync 0x1400538f0. Decompiles
// gui_mo/s1/mdt/L4/db/ex.c, ex3.c. The data is SfOprExpressProperties+0x38 (data::ExpressData):
// +0x04 and +0x08 are the die-mesh setting this code reads.
#include <cstdint>

#include <defmodel/layout/llobj_data.hpp>

#include "engine_internal.hpp"
#include "m2m/data/express.hpp"
#include "m2m/engine.hpp"
#include "m2m/mst/loader_l4.hpp"
#include "m2m/props.hpp"
#include "ops/express.hpp"

namespace m2m::engine {

namespace {

// 0x1405ef320(OB, 0) dynamic_cast to SfOprExpressProperties (TD 0x143579bb8), then +0x38
data::ExpressData* expressData(mst::OpClass& c)
{
    auto* pr = dynamic_cast<ExpressProps*>(c.propsOf(nullptr));
    return pr ? pr->data.get() : nullptr;                // 0x140025010
}

// @src 0x1413fc840  vt+0x2c0
bool expressDieMeshes(mst::OpClass& c, Project& p, bool keep)
{
    data::ExpressData* d = expressData(c);
    if (!d) return false;
    const int a = d->formingType;                        // 0x14142ff20: +0x04
    const int b = d->dieTemperatureType;                 // 0x140cbc690: +0x08
    Operation* cur = c.current;                          // 0x1405ef2b0
    if (!cur) return false;
    GuiModel* m = opModel(p, *cur);                      // 0x14005f770
    if (!m) return false;
    bool found = false, deleted = false;
    const int n = m->objectCount();                      // 0x140c7c1a0
    for (int i = 0; i < n; ++i) {
        LLOBJ_DATA* o = m->objectAt(i + 1);              // 0x140c7aed0
        // a rigid die (+0xc == 0) with nodes (+0x87c) while the setting does not keep die meshes
        if ((a == 0 || b != 0) && o && *at<std::int32_t>(o, 0xc) == 0 && *at<std::int32_t>(o, 0x87c) != 0) {
            found = true;
            if (!keep) {
                objDeleteMesh(o);                        // 0x140873be0
                deleted = true;
            }
        }
    }
    if (deleted) {
        viewerRebuild(*m, true, true);                   // 0x140c7ff80(m, 1, 1)
        // 0x1402ca890(APP): UI
    }
    return found;
}

// @src 0x1413fc6b0  vt+0x2b8
void expressTrans(mst::OpClass& c, Project& p)
{
    Operation* cur = c.current;                          // 0x1405ef2b0
    if (!cur) return;
    data::ExpressData* d = expressData(c);
    if (!d) return;
    // 0x140050d00: the operation's class name; contains "SfOprFormingExpress" (0x142e830e8,
    // case-sensitive)
    if (qt::contains(cur->classKey, qt::L("SfOprFormingExpress"))) setTrans2(p, *cur, true);   // 0x1405ef6f0
    setTrans1(p, *cur, d->formingType != 0);             // 0x1405ef750: 0 -> 0, 1 / 2 / other -> 1
}

}  // namespace

void installLoaderSlotsL4()
{
    mst::g_loaderSlotsL4.expressDieMeshes = &expressDieMeshes;
    mst::g_loaderSlotsL4.expressTrans = &expressTrans;
    mst::g_loaderSlotsL4.expressSyncTail = &mst::expressSyncFromModelTail;
}

}  // namespace m2m::engine
