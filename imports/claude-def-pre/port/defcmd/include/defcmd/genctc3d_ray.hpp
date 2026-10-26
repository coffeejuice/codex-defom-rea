// defcmd/genctc3d_ray.hpp: ray queries of the shared GENCTC 3D contact engine (the ray along a
// direction against the polygons of one cube, and its ray-triangle / ray-quad kernels).
// Written by packet p67-3 (OBJPOS drop, initial drop §5.6); packet p67-7 (3D contact generation,
// vertex re-search 14006de2d, triangle projection 1400702c8) reuses these.
//
// Specs: docs/re/commands/OBJPOS_drop_solver.md §5.6 (ray query), GENCTC_3d_search.md §10.2
// (ray-triangle), OBJPOS_drop_ray.md (ray-quad children 140063ab3, 14007ff21, 1400653e5 and the
// fall-backs 140065ef8, 14007f8c4, 140066698, 140066902, 14007a47d).
// API: name fun_<address>, the binary's argument list in order, every argument by pointer
// (INTEGER = std::int32_t, REAL*8 = double); arrays column-major, X(:,k) = X + 3(k-1).
#pragma once

#include <cstdint>

namespace def::cmd::ctc3d {

using i32 = std::int32_t;

// @src 14001f54f  ray from p along d, length *len, against the valid polygons (EI2 == 1) of cube
// (obj, inst) that face the ray (ER1·d̂ < 0): nearest hit -> q, uv = (s, t), *face = polygon,
// *frac = hit distance / ray length, *len *= *frac. *face = 0 when nothing is hit. p10 is passed
// by the callers but never read.
void fun_14001f54f(i32* obj, i32* inst, double* p, double* d, double* q, double* len,
                   double* frac, double* uv, i32* face, i32* p10 = nullptr);

// @src 1400647a0  ray-triangle (GENCTC_3d_search §10.2): plane hit of the ray (p, dir) on the
// triangle x(3,3) with normal n; mode 1: only dir·n <= 0 and |dist| <= |*d|, mode 2: only
// |dist| <= |*d|; in-triangle test on the dominant-axis projection. *bad == 1 (or a parallel
// ray) -> edge fall-back 140065ef8.
void fun_1400647a0(double* p, double* dir, double* n, double* x, double* d, double* s, double* t,
                   double* q, i32* hit, i32* mode, i32* bad);
// @src 140064f58  ray-quad: planar quad (|z4| < 1e-6 in the frame of nodes 1-3) -> plane hit
// 140063ab3 + point-in-quad 14007ff21; non-planar, *bad == 1 or parallel -> 1400653e5
void fun_140064f58(double* p, double* dir, double* n, double* x, double* d, double* s, double* t,
                   double* q, i32* hit, i32* mode, i32* bad);
// @src 140063ab3  ray-plane intersection: plane (n, x1); *ok = 0 when |dir·n| <= 1e-11, else
// q = p - dir*tt, *d = -tt
void fun_140063ab3(double* p, double* dir, double* n, double* x1, double* q, double* d, i32* ok);
// @src 14007ff21  point p in quad x(3,4): natural coordinates (s, t), *in = 0 inside, 1 outside
// (or off the plane); mode 1..6 zeroes normal components first
void fun_14007ff21(double* x, double* p, double* s, double* t, i32* in, i32* mode);
// @src 1400653e5  non-planar quad: four triangles (edge k, centroid), nearest hit, final
// point-in-quad 14007ff21 (n is not read)
void fun_1400653e5(double* p, double* dir, double* n, double* x, double* d, double* s, double* t,
                   double* q, i32* hit, i32* mode);
// @src 140065ef8  ray in the plane of the triangle x(3,3) (parallel case): 2D intersection with
// the three edges, nearest along the ray; flag 1 builds the frame from an edge x dir, else from n
void fun_140065ef8(double* p, double* dir, double* n, double* x, double* dist, double* s,
                   double* t, double* q, i32* hit, i32* flag);
// @src 14007f8c4  point p in triangle x(3,3): barycentric (s, t), *in = 0 inside, 1 outside
void fun_14007f8c4(double* x, double* p, double* s, double* t, i32* in, i32* mode);
// @src 140066698  2D line-line intersection (x, y of a(:,1)-a(:,2) and b(:,1)-b(:,2), column
// stride *n): par = (r along a, q along b), out = a(:,1) + r*(...), *ok
void fun_140066698(double* a, double* b, double* out, double* par, i32* n, i32* ok);
// @src 140066902  snap a, b to [0,1] within 1e-8, *ok = both in [0,1]
void fun_140066902(double* a, double* b, i32* ok);
// @src 14007a47d  *dist = |b - a| (sqrt only when the square is not 0)
void fun_14007a47d(double* a, double* b, double* dist);

// ---- module statics that a call can read before it writes them (docs/port-conventions.md §5) ----

// 14001f54f: attribute buffers (a Get on a missing field leaves them unchanged) and the outputs
// of the ray kernels (140065ef8 with flag 1 can return without writing them).
// @data 140649460  EI2 of the current polygon; @data 140649464  its count
inline i32 g_ray_ei2 = 0;
inline i32 g_ray_ei2_cnt = 0;
// @data 140649470  polygon node coordinates Xp(3,4); @data 1406494d0  node numbers;
// @data 1406494f0  node count (14004f363)
inline double g_ray_xp[12]{};
inline i32 g_ray_conn[8]{};
inline i32 g_ray_cnt = 0;
// @data 140649500  ER1 (unit normal); @data 140649520  its count
inline double g_ray_er1[3]{};
inline i32 g_ray_er1_cnt = 0;
// @data 140649550  s; @data 140649558  t; @data 140649560  Q(3); @data 140649580  hit
inline double g_ray_s = 0.0;
inline double g_ray_t = 0.0;
inline double g_ray_q[3]{};
inline i32 g_ray_hit = 0;

// 140064f58: @data 1406a1a38  in-flag of 14007ff21
inline i32 g_quad_in = 0;
// 1400653e5: @data 1406a1b30  in-flag of 14007f8c4 / 14007ff21 (read at the end even when no
// call in this invocation wrote it); @data 1406a1b10  distance of 140065ef8; @data 1406a1ae0  Q'
inline i32 g_nq_in = 0;
inline double g_nq_dist = 0.0;
inline double g_nq_q[3]{};
// 140065ef8: @data 1406a1e40  (ray, edge) parameters of the three edges (140066902 reads and
// clamps them also when 140066698 did not write them)
inline double g_ef8_par[6]{};
// 14007f8c4: @data 1406d2e70  local triangle V'(3,4) (node 4 = copy of node 1 before 1404d41f8)
inline double g_f8c4_v[12]{};

// Resets every static above (tests).
inline void reset_ray_statics()
{
    g_ray_ei2 = g_ray_ei2_cnt = g_ray_cnt = g_ray_er1_cnt = g_ray_hit = 0;
    for (double& v : g_ray_xp) v = 0.0;
    for (i32& v : g_ray_conn) v = 0;
    for (double& v : g_ray_er1) v = 0.0;
    g_ray_s = g_ray_t = 0.0;
    for (double& v : g_ray_q) v = 0.0;
    g_quad_in = g_nq_in = 0;
    g_nq_dist = 0.0;
    for (double& v : g_nq_q) v = 0.0;
    for (double& v : g_ef8_par) v = 0.0;
    for (double& v : g_f8c4_v) v = 0.0;
}

} // namespace def::cmd::ctc3d
