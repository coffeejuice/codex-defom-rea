// Internal declarations of the 3D interference kernels (packet p67-2, src/ifd/drv).
#pragma once

#include <cstdint>

#include <defcmd/ifd/ifd_internal.hpp>

namespace def::cmd::ifd {

// ---- Fortran (ftn_ray.cpp) ----
void fun_1403541fc(double* p, double* p1, std::int32_t* f1, std::int32_t* nf1, double* z,
                   double* unused, double* dn, double* dcur);
void fun_1403539a1(double* p1, double* p2, double* tol, std::int32_t* f1, std::int32_t* f2,
                   std::int32_t* nf1, std::int32_t* nf2, std::int32_t* nn1, std::int32_t* nn2,
                   std::int32_t* work, double* d, std::int32_t* sign, std::int32_t* err,
                   std::int32_t* flag);
// tests: reset the SAVE storage of the two routines above
void reset_ray_state();

// ---- object geometry access (ifd3d.cpp) ----
// @src 1401808d0  node coordinates of the node set obj+0x8a0 (allocated on demand)
double* node_coords(void* node_set);
// @src 140183cb0  connectivity of the element set obj+0x9e0
std::int32_t* elem_conn(void* elem_set);

// ---- C++ drivers (ifd3d.cpp) ----
// @src 1401a3aa0  3D kernel with the backwards retry; moves nothing
bool fun_1401a3aa0(DB_STEP_DATA* m, LLOBJ_DATA* a, LLOBJ_DATA* b, double* dir, double* tol,
                   double* dist, double* disp, char flag);
// @src 1401a3ea0  load and rotate S (target) and T (mover), one-sided distance
void fun_1401a3ea0(DB_STEP_DATA* m, LLOBJ_DATA* s, LLOBJ_DATA* t, double* u, double tol,
                   double* out, std::int32_t* err, std::int32_t flag);
// @src 1401a5470  both rigid surfaces: two passes, the shorter |d|
void fun_1401a5470(DB_STEP_DATA* m, LLOBJ_DATA* s, LLOBJ_DATA* t, double* u, double tol,
                   double* out, std::int32_t* err, std::int32_t flag);

} // namespace def::cmd::ifd
