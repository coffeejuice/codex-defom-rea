// defcmd/src/ifd/d2/d2.hpp: internal declarations of the 2D interference kernel of OBJPOS modes
// 2/12/102/112 (packet p67-2, part d2). Spec: docs/re/commands/OBJPOS_interference.md section 7,
// OBJPOS_interference_kernels.md section 4. Fortran routines take every argument by pointer in
// the binary's order (1-based arrays in the comments).
//
// Fortran SAVE locals that the original keeps in static memory but that are always written
// before they are read within one call are C++ locals here; only the state that crosses calls
// (the COMMON-like context of 1403460c2 / 1403460e7 / 14033ffb8 / 14034c209) is global.
#pragma once

#include <defcmd/ctc2d_msg.hpp>

#include <cstdint>

#include <defmodel/globals_p1_10.hpp>
#include <defrt/ftn/common_140b84720.hpp>

namespace def::model {
class DB_STEP_DATA;
class LLOBJ_DATA;
class OBJ_GEO2D;
} // namespace def::model

namespace def::cmd::ifd::d2 {

using model::DB_STEP_DATA;
using model::LLOBJ_DATA;
using model::OBJ_GEO2D;

// ---- shared 2D gap context (globals of the original) --------------------------------------------
// @data 1405f6000  1 when gaps below -1e-15 are skipped (flag == 0: modes 102/112), set by 14033ffb8
inline std::int32_t g_skip_negative = 0;
// 140b74428 (owner: def::model::dat_140b74428, p1-10)  GEOERR 1 as stored by 1403460c2
inline double& g_geoerr1_in = def::model::dat_140b74428;
// 140b74430 (owner: def::model::dat_140b74430, p1-10)  GEOERR 2 as stored by 1403460c2
inline double& g_geoerr2_in = def::model::dat_140b74430;
// 140b84720..30: the COMMON the 2D curve kernels also read (owner: def::rt::geoutl::g_common_140b84720)
// 140b84720  GEOERR 1 (1403460e7)
inline double& g_geoerr1 = def::rt::geoutl::g_common_140b84720.u00;
// 140b84728  GEOERR 2 (1403460e7)
inline double& g_geoerr2 = def::rt::geoutl::g_common_140b84720.u08;
// 140b84730  extent of the target (14034c209, overridden by 1403460e7)
inline double& g_ext = def::rt::geoutl::g_common_140b84720.u10;
// @data 140b84748  gap tolerance override (0.0 in the image; no writer found)
inline double g_tol_override = 0.0;
// @data 140b74990  bounding box stored by 14034c209: xmin, ymin, xmax, ymax (140b74990..9a8)
inline double g_box[4] = {0.0, 0.0, 0.0, 0.0};
// 140b847a4 (Fortran unit of the 2D warnings, never assigned: 0): owned by defcmd/ctc2d_msg.hpp
inline std::int32_t& g_msg_unit = def::cmd::ftn::dat_140b847a4;

// Reset the globals above to their image values (tier-1 tests).
void reset_state();

// ---- context setters ----------------------------------------------------------------------------
// @src 1403460c2  store GEOERR 1/2
void fun_1403460c2(double* e1, double* e2);
// @src 1403460e7  ext = *e; GEOERR copies
void fun_1403460e7(double* e);
// @src 14033ffb8  g_skip_negative = (*flag == 0)
void fun_14033ffb8(std::int32_t* flag);
// @src 14034c209  store the box, ext = max(ymax - ymin, xmax - xmin)
void fun_14034c209(double* xmin, double* ymin, double* xmax, double* ymax);

// ---- gap kernels --------------------------------------------------------------------------------
// @src 14030bba0  swept segment seg(4) with end-point velocities vel(4) against the point q(2)
// along dir(2): gap written on a hit, flag 1 hit / -1 no solution / 0
void fun_14030bba0(double* seg, double* vel, double* q, double* dir, double* gap, std::int32_t* flag);
// @src 1403407d7  arc (start s, centre c, sweep *phi) against q along dir
void fun_1403407d7(double* s, double* c, double* phi, double* q, double* dir, double* gap,
                   double* tola, std::int32_t* flag);
// @src 14032bd2a  is x on the arc (s, c, *phi)
void fun_14032bd2a(double* s, double* c, double* phi, double* x, std::int32_t* in);
// @src 14032beef  signed angle from a to b (Fortran ATAN2), 0 for short vectors
void fun_14032beef(double* a, double* b, double* ang);
// @src 140341100  arc contact refinement driver
void fun_140341100(double* s, double* tmp, double* c, double* phi, double* vel, double* q,
                   double* dir, double* tola, double* gap, std::int32_t* flag);
// @src 14034151e  arc contact refinement in angle space (p2 is not read)
void fun_14034151e(double* p1, double* p2, double* cc, double* r, double* tola, double* phi,
                   double* gap, double* len, std::int32_t* status);

// ---- samplers, area, root -----------------------------------------------------------------------
// @src 14033ffe0  mesh boundary sampler (the caller's 10th argument is not read)
void fun_14033ffe0(std::int32_t* objnum, double* pts, std::int32_t* np, double* yref, double* x0,
                   double* x1, std::int32_t* n, double* dir, double* g);
// @src 140340273  die curve sampler
void fun_140340273(std::int32_t* objnum, double* recs, std::int32_t* nrec, double* yref, double* x0,
                   double* x1, std::int32_t* n, double* dir, double* g, std::int32_t* err);
// @src 14033fe8e  overlap area A(s)
double fun_14033fe8e(std::int32_t* n, double* dx, double* g, double* s);
// @src 14033fdb8  shoelace sum * 0.5
double fun_14033fdb8(std::int32_t* n, double* p);
// @src 14033f8d4  overlap-area root (Illinois false position)
void fun_14033f8d4(double* step, double* atol, double* g, std::int32_t* n, double* dx, double* s,
                   std::int32_t* err);
// @src 14033f2bf  rotate a boundary loop, extremes, area, reorder from the extreme node
void fun_14033f2bf(std::int32_t* side, double* xy, double* out, std::int32_t* range,
                   std::int32_t* nodes, std::int32_t* count, double* area, double* xmm, double* ymm,
                   double* theta);

// ---- object level (C++) -------------------------------------------------------------------------
// @src 1402204a0  x ** n (140221390, binary powering)
double fun_1402204a0(double x, std::int32_t n);
// @src 14017ed40  bounding box of a 2D boundary loop record (Cls_14017e740) over coords xy
bool fun_14017ed40(void* rec, double* min, double* max, double* xy);
// @src 1401ffa90  outer boundary loop (1-based) of a meshed object; false without mesh
bool fun_1401ffa90(LLOBJ_DATA* obj, std::int32_t* idx);
// @src 1401ff7f0  outer die curve (1-based); false without geometry
bool fun_1401ff7f0(LLOBJ_DATA* obj, std::int32_t* idx);
// node coordinates of the node set at obj+0x8a0 (forwards to ifd::node_coords, 1401808d0)
double* fun_1401808d0(void* nodeset);
// @src 14027a720  circle (centre c, end points a, b) of a rounded corner p0-p1-p2 (radius p1[2])
bool fun_14027a720(double* p0, double* p1, double* p2, double* a, double* b, double* c);
// @src 14027ab80  bounding box of an arc (start x1/y1, centre, sweep in degrees)
void fun_14027ab80(double x1, double y1, double cx, double cy, double sweep, double* xmin,
                   double* xmax, double* ymin, double* ymax);
// @src 140279960  bounding box of an OBJ_GEO2D (arcs included)
bool fun_140279960(OBJ_GEO2D* c, double* min, double* max);
// @src 14027e990  drop zero-length segment rows of a type-2 curve; true when any dropped
bool fun_14027e990(OBJ_GEO2D* c);
// @src 140306a70  OR of 14027e990 over the object's curves
bool fun_140306a70(LLOBJ_DATA* obj);
// @src 14019a620  dst->assign(src)
void fun_14019a620(OBJ_GEO2D* dst, OBJ_GEO2D* src);
// @src 1401a28c0  rotated boundary of one object
void fun_1401a28c0(DB_STEP_DATA* m, LLOBJ_DATA* obj, OBJ_GEO2D* curve, double* xy, double* buf,
                   std::int32_t side, std::int32_t* kind, double* theta, std::int32_t* count,
                   double* xmm, double* ymm, double* area);
// @src 1401a1370  2D kernel: 0 on success with out(2) = displacement of t, 1 on failure
std::int32_t fun_1401a1370(DB_STEP_DATA* m, LLOBJ_DATA* s, LLOBJ_DATA* t, double* d, double tol,
                           std::int32_t n, double* out, std::int32_t flag);

} // namespace def::cmd::ifd::d2
