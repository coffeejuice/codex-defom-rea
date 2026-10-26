// defdbio/objcheck/objcheck.hpp: packet p4-03, the GENDB input checks for objects, materials and
// inter-material data, and the model checks that run before GENDB (1401b9d50).
//
// What the checks do: print INFO / WARNING / ERROR lines to stdout (the CRT's stdout,
// __iob_func()+0x30), set the caller's error / warning flags, and in a few places correct the
// model (NBCD codes of nodes that point at a missing object, removed edge boundary conditions,
// unused symmetry planes, the mechanical-press stroke vector, the rotational symmetry angle, the
// IIPHAS-like flag at DB_STEP_DATA+0x898). The per-keyword error records 14001cd60..14001cd90 are
// empty functions in the binary; they are still called through the thin interface.
//
// Pointers are the binary's objects (DB_STEP_DATA, LLOBJ_DATA, Material, PhasePair and their
// lists), passed untyped where the original passes a sub-object address.
#pragma once

#include <cstdint>

namespace def::model {
class DB_STEP_DATA;
class LLOBJ_DATA;
class Material;
} // namespace def::model

namespace def::dbio::objcheck {

using def::model::DB_STEP_DATA;
using def::model::LLOBJ_DATA;

// ---- the check chain (called by p4-04's 1401bebf0) ------------------------------------------------
// 14001cdc0: empty function called at the start of every check.
void fun_14001cdc0();
// 1401bfdf0: "INFO   : Checking material properties", then 14029b980 on the material list.
void check_material_data(DB_STEP_DATA* step, void* unused, std::int32_t* err, std::int32_t* warn);
// 1401bfe60: "INFO   : Checking inter-material data", then 14033a450 on the phase-pair list.
void check_inter_material_data(DB_STEP_DATA* step, void* unused, std::int32_t* err, std::int32_t* warn);
// 1401bfed0: "INFO   : Checking object data", then 14017adb0 on the object list.
void check_object_data(DB_STEP_DATA* step, void* unused, std::int32_t* err, std::int32_t* warn);

// 14017adb0: every object (type != 7) through 140167e90, then the model-wide object checks.
void check_object_list(void* list, std::int32_t* err, std::int32_t* warn);
// 140167e90: one object. n_rigid counts rigid objects (deformation on); the five flags are set to
// '!' when this object is the moving / principal die / principal workpiece / MDSOBJ object.
void check_object(LLOBJ_DATA* obj, std::int32_t* n_rigid, char* movctl, char* pdie1, char* pdie2,
                  char* mdsobj1, char* mdsobj2, std::int32_t* err, std::int32_t* warn);

// materials and inter-material relations
void check_material_list(void* list, std::int32_t* err, std::int32_t* warn);   // 14029b980
void check_material(void* mat, std::int32_t* err, std::int32_t* warn);         // 14028ed70
void material_undefined(void* mat, const char* key, char defined, std::int32_t* err);              // 14028f130
void material_zero_warning(void* mat, const char* key, const void* table, std::int32_t code, std::int32_t* warn); // 14028f1c0
void material_zero_error(void* mat, const char* key, const void* table, std::int32_t code, std::int32_t* err);    // 14028f260
void check_phase_pair_list(void* list, std::int32_t* err, std::int32_t* warn); // 14033a450
void check_phase_pair(void* pair, std::int32_t* n_on, std::int32_t* err);      // 140336130
void phase_pair_undefined(void* pair, const char* key, std::uint32_t mask, char defined, std::int32_t* err); // 140336250

// ---- model checks before GENDB (1401b9d50, called by menu 7 and 1401bb550) --------------------------
bool gendb_model_checks(DB_STEP_DATA* step);   // 1401b9d50
bool check_aniso(DB_STEP_DATA* step);          // 1401b9de0
bool check_wpaxis(DB_STEP_DATA* step);         // 1401b9fe0
bool check_transformation(DB_STEP_DATA* step); // 1401ba460
bool check_bearing_surfaces(DB_STEP_DATA* step); // 1401ba5a0

// ---- LLOBJ_DATA helpers of the checks ---------------------------------------------------------------
bool obj_has_heat_exchange_bc(LLOBJ_DATA* obj);  // 1401790a0
bool obj_has_heat_exchange(LLOBJ_DATA* obj);     // 140179150
bool obj_has_diffusion_bc(LLOBJ_DATA* obj);      // 140179190
bool obj_has_diffusion(LLOBJ_DATA* obj);         // 140179240
bool obj_has_movement(LLOBJ_DATA* obj);          // 140179280
bool obj_has_translation(LLOBJ_DATA* obj);       // 1401792d0
bool obj_has_rotation(LLOBJ_DATA* obj);          // 1401797e0
bool obj_has_rot_move1(LLOBJ_DATA* obj);         // 140179840
bool obj_has_rot_move2(LLOBJ_DATA* obj);         // 140179900
bool obj_has_rot_axis1(LLOBJ_DATA* obj);         // 1401799c0
bool obj_has_rot_axis2(LLOBJ_DATA* obj);         // 140179a90
bool obj_has_rotsym_bc(LLOBJ_DATA* obj);         // 140179b70
bool obj_mark_function(LLOBJ_DATA* obj, char* used, const std::int32_t* fn, char* any, const std::int32_t* n_fn); // 14016c4e0
bool obj_clean_edge_bcs(LLOBJ_DATA* obj, std::int32_t* n_ecdefn, std::int32_t* n_ectmfn, std::int32_t* n_ecatfn,
                        std::int32_t* n_ecrhfn);   // 14016c550
void obj_remove_unused_symmetry_planes(LLOBJ_DATA* obj);  // 14016c780
void obj_undefined(LLOBJ_DATA* obj, const char* key, std::uint32_t mask, char defined, std::int32_t* err); // 14016ca40
bool obj_has_free_surface_code(LLOBJ_DATA* obj, const std::int32_t* code);  // 140201320
bool obj_touches_object(LLOBJ_DATA* obj, std::int32_t other);               // 1402014c0
bool obj_has_rotsym_code(LLOBJ_DATA* obj);       // 140203fa0
bool obj_has_velocity_code(LLOBJ_DATA* obj);     // 140204040
bool obj_node_temperatures(LLOBJ_DATA* obj, const char* count, std::int32_t* n_bad); // 14020aa50
void obj_temperature_range(LLOBJ_DATA* obj, double* tmin, double* tmax);   // 14020ab60
std::int32_t obj_negative_radius_nodes(LLOBJ_DATA* obj);                    // 140304cb0
std::int32_t obj_edge_index(LLOBJ_DATA* obj, const std::int32_t* edge);     // 1403054e0
std::int32_t boundary_edge_index(const std::int32_t* edge, const std::int32_t* n_bnd, const void* bnd); // 140305520
double obj_volume(LLOBJ_DATA* obj, std::int32_t mode);                      // 140163540
void obj_area_volume_2d(LLOBJ_DATA* obj, double* area, double* volume);    // 140306940
bool obj_contains_material(LLOBJ_DATA* obj, std::int32_t material);         // 1401fbfc0
std::int32_t step_inter_object_relation(DB_STEP_DATA* step, std::int32_t a, std::int32_t b); // 1401a9820
void* step_inter_object_at(DB_STEP_DATA* step, std::int32_t n);             // 14019d5e0
std::uint32_t step_u13c(const DB_STEP_DATA* step);                          // 140199cc0
std::int32_t sim_u114(const void* sim);                                     // 1401f3c90
bool material_has_aniso(const void* mat);                                   // 14028d120

// sub-objects of LLOBJ_DATA
double* node_block_coords(void* blk);            // 1401808d0 (Cls_14017ef20)
double* elem_block_density(void* blk);           // 140184050 (Cls_140182880)
double* elem_block_volume_fractions(void* blk);  // 1401842f0 (Cls_140182880)
bool rotsym_angle_set(void* blk);                // 140186bc0 (Cls_1401860a0)
void rotsym_angle_reset(void* blk);              // 140186c10
bool rotsym_axis_set(void* blk);                 // 140186c50
bool window_unset(void* blk);                    // 14018b0f0 (Cls_140189440)
bool movement_direction_set(void* blk);          // 14018cd10 (Cls_14018b220)
double press_stroke_projection(void* blk, const double* stroke, const double* dir); // 140194f90
double press_curve_range(void* blk);             // 1401951d0
void press_stroke_vector(void* blk, double stroke0, double stroke, double* out);    // 1401952e0

// symmetry planes (Cls_140257340 / Cls_140257340List)
void plane_direction(const void* node, double* out);   // 1402575d0
bool plane_direction_zero(const void* node);           // 140257670
bool plane_list_remove(void* list, void* node);        // 140257f70
std::int32_t plane_list_max_id(void* list);            // 1402582b0

// DYNARY tests used by the material checks (not in the ledger; needed here)
bool dynary_all_zero(const void* t);       // 14012fd80
bool dynary_values_zero(const void* t);    // 14012fe90

// small 3-vector helpers
double* vec3_copy(double* out, const double* v);                  // 140239800
double* vec3_scale(double* out, const double* v, double s);       // 140239ea0
bool vec3_near(const double* a, const double* b, double tol);     // 14023aa90

// ---- Fortran kernels: volumes and areas ---------------------------------------------------------------
// 14030355f: volume enclosed by a surface of triangles / quads (nodes 1-based, `stride` ints per
// facet, quad when stride == 4 and node 1 != node 4).
void surface_volume(const double* coords, const std::int32_t* numnp, const std::int32_t* facets,
                    const std::int32_t* stride, const std::int32_t* nfacets, double* volume);
// 1404335e0: area and axisymmetric volume of 4-node 2D elements.
void quad_area_volume(const double* coords, const std::int32_t* elems, const std::int32_t* numnp,
                      const std::int32_t* numel, double* volume, double* area, std::int32_t* ierr);
// 1404337cd: one quadrilateral: area and 2*pi*r_centroid*area.
void quad_area_volume_1(const double* p, double* volume, double* area);
// 1404cce5e: c = a x b, |c| in *len; c normalised when |c| > 1e-16, else *len = 0.
void cross_normalize(const double* a, const double* b, double* c, double* len);
// 1404d3d43: unit normal of the triangle p[0..8] (cross of p2-p1 and p3-p1).
void triangle_normal(const double* p, double* n);
// 1404e29fa: triangle area by Heron's formula.
void triangle_area(const double* p, double* area);

} // namespace def::dbio::objcheck
