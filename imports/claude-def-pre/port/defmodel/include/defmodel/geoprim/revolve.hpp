// defmodel/geoprim/revolve.hpp: the GEOPRIM revolve (packet p9-05). Spec:
// docs/re/commands/GEOPRIM_REVOLVE.md (from the decompiles/listings in deform-rea-work/out/p905).
//
//   fun_1402ce190  revolve dispatch: axis kind, then the geometry path (fun_1402d7cd0, when
//                  prim->mode_geo != 0) and the mesh path (fun_1402ce290, mode_mesh != 0)
//   fun_1402d7cd0  surface of revolution of the 2D profile (prim->profile)
//   fun_1402ce310  surface of revolution of the 2D mesh boundary (mode_mesh == 2)
//
// The +0x23a4 == 1 branch of fun_1402ce290 (1402d2920 / 1402d4420, the shape-rolling cross
// sections) is not reachable in batch (docs/phase5_8_9_10-packets.md, "Not needed"); it stops in
// DEF_NOT_PORTED.
//
// Calls into code owned elsewhere go through P905Ext (one pointer per callee, DEF_NOT_PORTED
// until set; def::cmd::geoprim binds the owners, tests set listing implementations), and through
// p9-04's P904Ext (dot, polygon area, polygon normal).
#pragma once

#include <cstdint>

#include <defmodel/geoprim/def_geoprim.hpp>

namespace def::model {
class LLOBJ_DATA;
class DB_STEP_DATA;
class OBJ_GEO2D;
} // namespace def::model

namespace def::model::geoprim {

struct P905Ext {
    // FUN_14023a4b0(out, a, b): out = a x b (P4, p4-02a window_orient.cpp vec3_cross)
    void (*vec3_cross)(double* out, const double* a, const double* b);
    // FUN_140239a30(v): sqrt(x*x + y*y + z*z) (Vector3 length, p67-1 vec3.hpp len)
    double (*vec3_length)(const double* v);
    // FUN_140220390(par): FIXGEO parameter block (0x30 bytes) init (p9-01)
    void (*fixgeo_par_init)(void* par);
    // FUN_140220420(par): its dtor (empty in the binary) (p9-01)
    void (*fixgeo_par_free)(void* par);
    // FUN_14027ece0(geo, par, &k, 1, 0): FIXGEO on a 2D outline (p9-01, DXF_IGES_READERS §5.4)
    void (*fixgeo)(OBJ_GEO2D* geo, void* par, std::int32_t* k, std::int32_t mode, char flag);
    // FUN_140220520(obj): the object's dimension (140158ce0) (P4, p4-07 getters.cpp)
    std::int32_t (*obj_dimension)(LLOBJ_DATA* obj);
    // FUN_1401d7320(model, dst): copy model+0x15f8 (0x4f ints) to dst when dst != 0 (p8-08)
    void (*model_copy_u15f8)(DB_STEP_DATA* model, std::int32_t* dst);
    // FUN_140202700(obj, nn, ne, nper, layout, dim): allocate the object's mesh (p8-09)
    void (*mesh_alloc)(LLOBJ_DATA* obj, std::int32_t nn, std::int32_t ne, std::int32_t nper,
                       std::int32_t* layout, std::int32_t dim);
    // FUN_1401808d0(nodes): node coordinate array (3 doubles per node) of obj+0x8a0 (P4)
    double* (*node_coords)(void* nodes);
    // FUN_1402fc1b0(surface): recompute the 3D surface (p67-1 objpos/centre3d.cpp)
    void (*surface_update)(void* surface);
};
P905Ext& p905_ext();

// ---- small vector helpers (Vector2 class) ------------------------------------------------------

// @src 1403086e0  Vector2(x, y) (returns 0x18 bytes in the binary; the third is not used)
struct V2 {
    double x, y;
};
inline V2 fun_1403086e0(double x, double y) { return V2{x, y}; }
// @src 140308ad0  2D cross a.x * b.y - b.x * a.y
inline double fun_140308ad0(const V2& a, const V2& b) { return a.x * b.y - b.x * a.y; }

// ---- revolve -----------------------------------------------------------------------------------

// @src 1402ce190  revolve dispatch; returns the last path's result (false when none ran)
bool fun_1402ce190(DEF_GEOPRIM* prim);
// @src 1402ce290  mesh path: mode_mesh 1 -> 1402d2920/1402d4420 (not reachable), 2 -> 1402ce310
bool fun_1402ce290(DEF_GEOPRIM* prim);
// @src 1402ce310  revolve the 2D mesh boundary into the object's surface
bool fun_1402ce310(DEF_GEOPRIM* prim);
// @src 1402d7cd0  revolve the profile into the object's surface (and end caps)
bool fun_1402d7cd0(DEF_GEOPRIM* prim);

// @src 1402d9bb0  axis of the revolve record: dir (normalised, snapped to +-x / +-y), kind t
// (0 x, 1 y, 2 z, 3 other) and the axis point with the component along the profile plane
// dropped. Nothing is written without a revolve record.
void fun_1402d9bb0(DEF_GEOPRIM* prim, double* dir, std::int32_t* t, double* pt);
// @src 1402da250  profile points on the minority side of the axis are projected onto it; then
// FIXGEO (once) on the profile
void fun_1402da250(DEF_GEOPRIM* prim, const double* dir, const double* pt);
// @src 1402da9f0  reverse the profile points (and renumber the cap triangles, swapping their
// 2nd and 3rd ids) when the profile's orientation about the axis says so
void fun_1402da9f0(DEF_GEOPRIM* prim, const double* dir, const double* pt, const std::int32_t* np,
                   double* xyz, const std::int32_t* nface, std::int32_t* faces);
// @src 1402db0d0  on_axis[i] = 1 when |dir x (p_i - pt)| < 1e-6; returns the count
std::int32_t fun_1402db0d0(DEF_GEOPRIM* prim, const double* pt, const double* dir, std::int32_t n,
                           const double* xyz, char* on_axis);
// @src 1402db310  side faces (triangles as "a b c a") between the layers, skipping collapsed
// ones on the axis; appends to faces (4 ints each), *nface counts
void fun_1402db310(DEF_GEOPRIM* prim, const std::int32_t* nsec, const char* full, const char* on_axis,
                   std::int32_t* faces, std::int32_t* nface);
// @src 1402dbb20  side faces as quadrilaterals (mode_geo 2)
void fun_1402dbb20(DEF_GEOPRIM* prim, const std::int32_t* nsec, const char* full, const char* on_axis,
                   std::int32_t* faces, std::int32_t* nface);
// @src 1402dc160  drop the points no face uses (first 3 ids of each face), renumber, then store
// (p9-04's 1402dbe70)
void fun_1402dc160(DEF_GEOPRIM* prim, const std::int32_t* np, const double* xyz, const std::int32_t* nface,
                   const std::int32_t* faces);
// @src 1402cac30  non-uniform sections: the angle ranges of the refined zone
void fun_1402cac30(DEF_GEOPRIM* prim, double* a2, double* a3, const double* start, const double* angle,
                   const char* full, double* p7, double* p8, double* p9, double* p10, double* p11,
                   double* p12, double* p13, double* p14);
// @src 1402caf40  non-uniform sections: counts and step sizes of the four zones
void fun_1402caf40(DEF_GEOPRIM* prim, const std::int32_t* nsec, const double* p3, const double* p4,
                   const double* p5, const double* p6, const double* p7, const double* p8,
                   const double* p9, const double* p10, const double* p11, const double* p12,
                   const double* p13, const double* p14, std::int32_t* n15, double* s16,
                   std::int32_t* n17, double* s18, std::int32_t* n19, double* s20, std::int32_t* n21,
                   double* s22);
// @src 1402d6bf0  orientation of the first 2D element about the axis (xyz = the layer-0 points);
// with *invert the answer is negated; when the answer is set and *keep is 0 the 2D connectivity
// is reversed (entries 0 and 2 of every element swapped)
char fun_1402d6bf0(DEF_GEOPRIM* prim, const double* dir, const double* pt, const double* xyz,
                   const char* invert, const char* keep);

} // namespace def::model::geoprim
