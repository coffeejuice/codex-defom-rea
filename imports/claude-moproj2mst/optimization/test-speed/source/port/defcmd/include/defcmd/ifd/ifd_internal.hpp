// defcmd/ifd/ifd_internal.hpp: functions the parts of the interference port call across files
// (packet p67-2). Spec: docs/re/commands/OBJPOS_interference.md (section numbers below).
#pragma once

#include <cstdint>

namespace def::model {
class DB_STEP_DATA;
class LLOBJ_DATA;
} // namespace def::model

namespace def::cmd::ifd {

using model::DB_STEP_DATA;
using model::LLOBJ_DATA;

// ---- kernel switch and drivers (src/ifd/drv/) ---------------------------------------------------

// @src 1401a0b00  2D (1401a0c70) / 3D (1401a3aa0) kernel, moves nothing (section 4.2)
bool fun_1401a0b00(DB_STEP_DATA* m, LLOBJ_DATA* a, LLOBJ_DATA* b, double* dir, double* tol,
                   double* dist, double* disp, char flag);

// @src 1401a0a10  2D / 3D kernel, then move the mover by the result (14019eb60); also called by
// the edit menu 6 positioning form (p10-02)
bool fun_1401a0a10(DB_STEP_DATA* m, LLOBJ_DATA* a, LLOBJ_DATA* b, double* dir, double* tol,
                   double* dist, double* disp, char flag, std::int32_t* one);

// ---- path branch (src/ifd/path/, section 8) -----------------------------------------------------

// @src 14019ec30  true iff the mover and every coupled object found (n-th in the list) and not
// empty move on a non-zero arbitrary path (14021e470)
bool fun_14019ec30(DB_STEP_DATA* m, LLOBJ_DATA* a, std::int32_t* n, std::int32_t* list);

// @src 14019fb50  path branch: search the path time of contact, set the model time (argument
// list checked against the listing: tol is the 5th argument, n / list the 9th / 10th; dir, dist,
// disp and flag are not used)
bool fun_14019fb50(DB_STEP_DATA* m, LLOBJ_DATA* a, LLOBJ_DATA* b, double* dir, double* tol,
                   double* dist, double* disp, char flag, std::int32_t* n, std::int32_t* list);

// @src 14019ed10  put every path-moving object at the current time
void fun_14019ed10(DB_STEP_DATA* m);

// @src 1401f3ae0  current time of the simulation-control object (model+0x750)
double sim_time(void* simctl);
// @src 1401f3b30  set it
void set_sim_time(void* simctl, double* t);

// ---- p67-1's helpers (interim declarations, see src/ifd/p671_stubs.cpp) -------------------------
namespace ext {
// @src 14021d0b0  move an object by v (p67-1: objpos translate)
void translate(LLOBJ_DATA* obj, double* v, std::int32_t* a, char* b);
// 1401952e0 (ported in defdbio/objcheck/objcheck.hpp, press_stroke_vector)  out = (b - a) * unit(mv+0x50) when both lengths > 1e-6, else 0 (p67-1)
void move_along_movctl(void* mv, double a, double b, double* out);
// @src 1401611f0  usable object test with the two messages (p67-1)
bool usable(LLOBJ_DATA* obj);
// 140160cc0 (ported in defdbio/gendb/counts_frame.hpp, object_has_geometry)  geometry present (3D/2D by the owning model) (p67-1)
bool has_geometry(LLOBJ_DATA* obj);
// 140203980 (ported in defdbio/gendb3d/obj3d.hpp, g3d::object_has_geometry)  die geometry present (3D 140203a20 / 2D 1402039d0) (p67-1)
bool has_die_geometry(LLOBJ_DATA* obj);
} // namespace ext

} // namespace def::cmd::ifd

namespace def::cmd::ifd::ext {
// Test hooks standing in for p67-1's functions until its code is on the branch (p671_stubs.cpp).
extern void (*g_translate_hook)(LLOBJ_DATA*, double*, std::int32_t*, char*);
extern void (*g_move_along_movctl_hook)(void*, double, double, double*);
extern bool (*g_usable_hook)(LLOBJ_DATA*);
extern bool (*g_has_geometry_hook)(LLOBJ_DATA*);
extern bool (*g_has_die_geometry_hook)(LLOBJ_DATA*);
} // namespace def::cmd::ifd::ext

namespace def::cmd::ifd {
// tests: reset the SAVE storage of the 3D ray kernels 1403539a1 / 1403541fc (a fresh process)
void reset_ray_state();
} // namespace def::cmd::ifd
