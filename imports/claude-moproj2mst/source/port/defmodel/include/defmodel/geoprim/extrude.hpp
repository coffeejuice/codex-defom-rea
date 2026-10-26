// defmodel/geoprim/extrude.hpp: GEOPRIM helpers of packet p9-04 that the extrude (defcmd,
// defcmd/geoprim/builder.hpp) and the revolve (p9-05) share: accessors of the 2D mesh and the
// int lists, the boundary-list builder, the shell face generator, the object store, the section
// test and the min-area triangulation. Names are fun_<address> so the callers' decompiles map
// one to one. Spec: docs/re/commands/CNVT3D_SWEEP.md sections 10.5 and 10.6.
//
// Calls into code owned elsewhere go through P904Ext (one pointer per callee). Each starts at a
// DEF_NOT_PORTED stub; def::cmd::geoprim::bind_ext() (defcmd, called by the builder 1402ba500)
// points them at the owners when their headers are in the build, tests point them at listing
// implementations.
#pragma once

#include <cstdint>

#include <defmodel/geoprim/def_geoprim.hpp>

namespace def::model {
class LLOBJ_DATA;
class OBJ_MESH2D;
} // namespace def::model

namespace def::model::geoprim {

struct P904Ext {
    // FUN_14023e670(n, pts): 0.5 * |sum of p_i x p_(i+1)| over n 3-double points (p4-07)
    double (*polygon_area)(std::int32_t n, const double* pts);
    // FUN_14023b850(a, b): squared distance of two 3-double points (p67-1 vec3.hpp)
    double (*dist2)(const double* a, const double* b);
    // FUN_14023a380(a, b): dot product, left to right (p8-09 geolib.hpp)
    double (*dot)(const double* a, const double* b);
    // FUN_140240d80(out, pts, n): unit normal sum_k cross(p_(k-1)-p_0, p_k-p_0), k = 2..n-1,
    // normalised by 140239b40 (zero below 1e-12) (Phase 4)
    void (*poly_normal)(double* out, const double* pts, std::int32_t n);
    // FUN_1402009f0(obj, nn, nf, bdry): size surface bdry of the object geometry (p67-1)
    void (*surface_size)(LLOBJ_DATA* obj, std::int32_t nn, std::int32_t nf, std::int32_t bdry);
    // FUN_140220480(mesh): second element array of the 2D mesh (140183d00 on mesh+0x148)
    std::int32_t* (*mesh_elem_u018)(OBJ_MESH2D* mesh);
};
P904Ext& p904_ext();

// ---- accessors -------------------------------------------------------------------------------

// @src 1402e9b50  2D node count (140284aa0)
std::int32_t fun_1402e9b50(DEF_GEOPRIM* prim);
// @src 1402e9b70  2D element count (140284ac0)
std::int32_t fun_1402e9b70(DEF_GEOPRIM* prim);
// @src 1402e9b90  2D node coordinates, 2 doubles per node (140220440)
double* fun_1402e9b90(DEF_GEOPRIM* prim);
// @src 1402e9bb0  2D connectivity, 4 ints per element (140220460)
std::int32_t* fun_1402e9bb0(DEF_GEOPRIM* prim);
// @src 1402e9bd0  140220480
std::int32_t* fun_1402e9bd0(DEF_GEOPRIM* prim);
// @src 1402f07e0  byte +0x24b9 (min-area triangulation instead of 14041f830)
bool fun_1402f07e0(const DEF_GEOPRIM* prim);
// @src 1402e91d0  first == last of an int array with n >= 1 (false when empty)
bool fun_1402e91d0(const IntArray* a);
// @src 1402e9470  when 0 < *k <= l->n: items[*k - 1].resize(*count)
void fun_1402e9470(IntArrayList* l, const std::int32_t* k, const std::int32_t* count);

// ---- extrude / revolve helpers ---------------------------------------------------------------

// @src 1402caac0  sections uniform: true when |u2388 - 1| <= 1e-6 (u2388 raised to 1 first),
// or for a revolve with v2490 empty and (|v2398[0]| <= 0.01, or the revolve record set and
// |(v2390[0] - angle) - start| <= 1e-6)
bool fun_1402caac0(DEF_GEOPRIM* prim);
// @src 1402cc080  boundary node chains of the 2D mesh (FL2_FNDBND) into prim->int_lists
void fun_1402cc080(DEF_GEOPRIM* prim);
// @src 1402bc720  the first 2D element's normal (nodes taken from xyz, layer 0) points to +z
bool fun_1402bc720(DEF_GEOPRIM* prim, const double* xyz);
// @src 1402bc980  shell quads of the extruded mesh (bottom, strips, top); returns the count.
// Decrements the boundary list count in place when the list is closed (1402e91d0).
std::int32_t fun_1402bc980(DEF_GEOPRIM* prim, const std::int32_t* nsec, const double* xyz,
                           std::int32_t* quads, const char* flip, const char* closed);
// @src 1402dbe70  clear the object's geometry surfaces, store np points (3 doubles) and nf
// quads (4 ints) as surface 0
void fun_1402dbe70(DEF_GEOPRIM* prim, const std::int32_t* np, const double* xyz,
                   const std::int32_t* nf, const std::int32_t* quads);
// @src 1402dc040  triangles (3 ints) -> quads "a b c a", then 1402dbe70
void fun_1402dc040(DEF_GEOPRIM* prim, const std::int32_t* np, const double* xyz,
                   const std::int32_t* ntri, const std::int32_t* tris);
// @src 1402e24f0  min-area triangulation of the 1-based points pts (16-byte stride, n points);
// tri_out gets 3 1-based ids per triangle; returns the count (-1 when the solver fails)
std::int32_t fun_1402e24f0(DEF_GEOPRIM* prim, std::int32_t n, const double* pts,
                           std::int32_t* tri_out);

} // namespace def::model::geoprim
