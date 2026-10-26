// defcmd/geoprim/builder.hpp: GEOPRIM builder and extrude primitive (packet p9-04).
//
//   1402ba500 builder: 3D model/object (own or given), 1402ba850 profile -> geometry curve 0,
//             kind 1 -> extrude 1402baa60, kind 2 -> revolve 1402ce190 (p9-05), KEY file
//   1402baa60 extrude: geometry path 1402bb460 (+0x23a0), mesh path 1402bbb10 (+0x23a4):
//             == 2 -> mesh-boundary path 1402bbb60, == 1 -> shape roll 1402bd210 (not-needed in
//             batch, docs/re/status/p9-shape-roll.tsv: stubbed with an assertion)
//   1402c1130 orientation to the extrude direction, 1402bc580 axis file, 1402dc560 KEY file
//
// Spec: docs/re/commands/CNVT3D_SWEEP.md sections 10.4-10.6. The helpers shared with the
// revolve are in defmodel/geoprim/extrude.hpp. This part lives in defcmd because it calls the
// object transforms (p67-1, defcmd/objpos.hpp) and the KEY writers (defkey).
#pragma once

#include <cstdint>
#include <cstdio>

namespace def::model {
class DB_STEP_DATA;
class LLOBJ_DATA;
class Cls_14017ef20;
namespace geoprim {
class DEF_GEOPRIM;
}
} // namespace def::model

namespace def::cmd::geoprim {

using def::model::DB_STEP_DATA;
using def::model::LLOBJ_DATA;
using def::model::geoprim::DEF_GEOPRIM;

// Calls into code owned elsewhere. Defaults: the owner's function when its header is in the
// build (__has_include), else a DEF_NOT_PORTED stub. Tests replace them.
struct BuilderExt {
    // FUN_14041f830(1, &n, pts, T): cap triangulation (p9-05, defmodel/geoprim/triangulate.hpp)
    std::int32_t (*cap_triangulate)(std::int32_t ncontours, const std::int32_t* cntr,
                                    const double* pts, std::int32_t* tris);
    // FUN_1402ce190(prim): revolve primitive (p9-05)
    bool (*revolve)(DEF_GEOPRIM* prim);
    // FUN_14021d2f0(o, origin, dir, angle_deg) / FUN_14021d0b0(o, d, &a, &b) (p67-1 objpos.hpp)
    void (*rotate)(LLOBJ_DATA* o, const double* origin, const double* dir, double angle_deg);
    void (*translate)(LLOBJ_DATA* o, const double* d, const char* flag_a, const char* flag_b);
    // FUN_140202700(o, nnode, nelem, elem_type, dims, dim) / FUN_140198270(o) (p8-09 geolib.hpp)
    void (*mesh_alloc)(LLOBJ_DATA* o, std::int32_t nnode, std::int32_t nelem, std::int32_t type,
                       const std::int32_t* dims, std::int32_t dim);
    void (*add_curve)(LLOBJ_DATA* o);
    // FUN_1401808d0(set): node coordinates of a node set (P4; defmodel/node_access.hpp)
    double* (*node_coords)(def::model::Cls_14017ef20* set);
    // FUN_1401b85d0(name): remove the file when it exists (p4-01)
    std::int32_t (*remove_if_exists)(const char* name);
    // FUN_1402a4a80(m, f, obj, code, n, k): state-variable KEY writer (kw-g2sv)
    int (*write_state_var)(DB_STEP_DATA& m, std::FILE* f, LLOBJ_DATA& obj, int code,
                           std::uint32_t n, int k);
};
BuilderExt& builder_ext();

// Point def::model::geoprim::p904_ext() at the owners (called by fun_1402ba500).
void bind_ext();

// @src 1402ba500  builder; o3 / model3 may be null (then owned and deleted at the end);
// *write_key -> DEF_GEOPRIM.KEY (1402dc560) after a success; ctx -> +0x23d8
bool fun_1402ba500(DEF_GEOPRIM* prim, LLOBJ_DATA* o3, const char* write_key, DB_STEP_DATA* model3,
                   void* ctx = nullptr);
// @src 1402ba850  profile -> curve 0 of the object geometry (closed: point 1 repeated); false
bool fun_1402ba850(DEF_GEOPRIM* prim);
// @src 1402baa60  extrude primitive
bool fun_1402baa60(DEF_GEOPRIM* prim);
// @src 1402bb460  geometry path: triangulated closed surface
bool fun_1402bb460(DEF_GEOPRIM* prim);
// @src 1402bbb10  mesh path dispatch on +0x23a4
bool fun_1402bbb10(DEF_GEOPRIM* prim);
// @src 1402bbb60  mesh-boundary path: quad shell of the extruded 2D mesh
bool fun_1402bbb60(DEF_GEOPRIM* prim);
// @src 1402bc580  axis file prim+0x1b30
void fun_1402bc580(DEF_GEOPRIM* prim);
// @src 1402c1130  orientation to the extrude direction (second argument unused)
void fun_1402c1130(DEF_GEOPRIM* prim, const char* unused = nullptr);
// @src 1402dc560  DEF_GEOPRIM.KEY (prim+0x1730)
void fun_1402dc560(DEF_GEOPRIM* prim);

} // namespace def::cmd::geoprim
