// defcmd/ifd/path.hpp: the "arbitrary path" branch of OBJPOS interference positioning (packet
// p67-2, part path). Spec: docs/re/commands/OBJPOS_interference.md sections 4.1, 8 and 9;
// OBJPOS_interference_kernels.md section 5 (object backup/restore).
//
// The entry points the router and the dispatcher call (14019ec30, 14019fb50, 14019ed10, sim
// time get/set) are declared in ifd_internal.hpp. This header names the helpers so tier-1 tests
// can reach them. Several are really methods of model classes owned by finished packets; the
// comment on each says whose:
//   movement Cls_14018b220 (LLOBJ_DATA+0x2160): 14021e470, 140194cb0, 140194d40, 14021ee40
//   LLOBJ_DATA:                                  1402007d0
//   DYNARY (path table, movement+0x448):         14012fd80, 1401350e0, 1401351c0, 140132a60,
//                                                140134de0, 140135bc0, 14012fd30
//   Fortran-style 3x3 helper:                    1402416d0
// Bodies in src/ifd/path/*.cpp.
#pragma once

#include <cstdint>

#include <defcmd/ifd/ifd_internal.hpp>

namespace def::model {
class DYNARY;
}

namespace def::cmd::ifd::path {

using model::DYNARY;

// ---- path table (DYNARY), src/ifd/path/path_table.cpp ------------------------------------------

// 14012fd80 (ported in defdbio/gendb/checks_p4_04.hpp, dynary_all_zero)  table is all zero: ndim 0 -> value == 0.0; else every abscissa and every value
// == 0.0 (NaN counts as non-zero)
bool table_all_zero(const DYNARY* t);
// @src 1401350e0  point k of a path table (types 0x79/0x7a, var[0] == 1 = time): 3 components when
// ncomp == 4, else 2; false (out untouched) otherwise
bool path_point(const DYNARY* t, const std::int32_t* k, double* out);
// @src 1401351c0  path position at time *t: linear interpolation between the points of the
// interval 140132a60 finds; false when the table is not a time path or no interval is found
bool path_at_time(const DYNARY* t, const double* time, double* out);
// 140132a60 (owner p67-4, defmodel/obj_scale.hpp scale::table_segment): interval of x on axis 0: index i (or -1) and fraction *f; above the last
// abscissa -> n - 2, below the first -> 0 (extrapolation)
std::int32_t path_interval(const DYNARY* t, double x, double* f);
// @src 140134de0  speed path (var[0] == 0x1a) -> time path: t_0 = 0, t_k = t_{k-1} + len_k / v_{k-1}
// (t_k = t_{k-1} when v_{k-1} <= 0); var[0] becomes 1
bool path_speed_to_time(DYNARY* t);
// @src 140135bc0  type 0x7b, ncomp 3: re-shape to type 0x7a (ncomp 4) and map each point
// (x, y, 1) through the 3x3 matrix m; the third input component goes to component 3
bool path_transform(DYNARY* t, const double* m);
// @src 14012fd30  dynary_init(t, type), n[0] = n, dynary_alloc(t)
void dynary_reshape(DYNARY* t, std::int32_t type, std::int32_t n);
// @src 1402416d0  out = M v (row-major 3x3), each row ((M0 v0 + M1 v1) + M2 v2)
void mat3_mul_vec(const double* v, double* out, const double* m);

// ---- movement / object, src/ifd/path/path_move.cpp ---------------------------------------------

// @src 14021e470  movement type (mv+0x68) is 6 and the path table (mv+0x448) is not all zero
bool movement_on_path(const void* mv);
// @src 140194cb0  current position of the movement: mv+0x630 for type 6, else mv+0x170 (STROKE)
double* movement_position(void* mv);
// @src 140194d40  the 3x3 path transform at mv+0x4c8..+0x508 into m (row-major); returns 1
bool movement_path_matrix(const void* mv, double* m);
// @src 14021ee40  out = the movement's path as a time path (copy of mv+0x448, speed -> time when
// mv+0x458 != 1, transformed when mv+0x4c0 == 2). `out` must be a constructed DYNARY.
void movement_path_curve(const void* mv, DYNARY* out);
// @src 1402007d0  put a path-moving object at time *t (move by P(t) - current position when
// |.| > 1e-12, then STROKE = current position); true iff it moved
bool place_on_path(LLOBJ_DATA* obj, const double* t);

// ---- branch, src/ifd/path/path_branch.cpp ------------------------------------------------------

// @src 14019ffc0  walk the path segments of x, run the kernel from the start of each segment
// along it; on the first contact within the segment: *tout = contact time, x placed there
bool fun_14019ffc0(DB_STEP_DATA* m, LLOBJ_DATA* x, LLOBJ_DATA* b, double* tol, double* d,
                   double* disp, double* tout);

// Test seam for the kernel switch 1401a0b00 (another part of p67-2): when set, 14019ffc0 calls
// it instead of fun_1401a0b00.
extern bool (*g_kernel_hook)(DB_STEP_DATA*, LLOBJ_DATA*, LLOBJ_DATA*, double*, double*, double*,
                             double*, char);
// Test seam for the object backup / restore of 14019fb50 (1401613a0 with the flags 1,1,1,1,0,1).
// When set it replaces the copy call: dst, src.
extern void (*g_copy_hook)(LLOBJ_DATA* dst, const LLOBJ_DATA* src);

} // namespace def::cmd::ifd::path
