// defcmd/ctc3d/search.hpp: MDSPS2_REC FUN_14006798b and the geometric kernels, GENCTC_3d_search.md sec. 9, sec. 10.
// Packet p67-7, group "search". Declarations follow the binary's argument lists (genctc3d.hpp
// explains the conventions): every argument by pointer, arrays column-major, X(:,k) = X + 3(k-1).
//
// The ray family (1400647a0 ray-triangle, 140064f58 ray-quad, 140063ab3, 14007ff21, 1400653e5,
// 140065ef8, 14007f8c4, 140066698, 140066902, 14007a47d) is ported by packet p67-3 and declared in
// defcmd/genctc3d_ray.hpp, included here so that callers of this header see those declarations.
#pragma once

#include <defcmd/ctc3d/common.hpp>
#include <defcmd/genctc3d_ray.hpp>

namespace def::cmd::ctc3d {

// @src 14004f363  coordinates x(3,k) of the nodes k = 1..4 of polygon *poly of cube (obj, inst)
// whose |node number| > 0 (cube getters 140227e00 / 140227fc0); conn(4) = the node numbers after
// 140062fb2 (first and last made positive); *cnt = number of such nodes. x(:,k) of a node <= 0
// is left as it was.
void fun_14004f363(i32* obj, i32* inst, i32* poly, double* x, i32* conn, i32* cnt);
// @src 140062fb2  polygon node list convention: *flag = 1; conn(n) < 1 -> flag -1, conn(n) =
// -conn(n); conn(1) <= 0 -> flag 0, conn(1) = -conn(1)
void fun_140062fb2(i32* conn, const i32* n, i32* flag);
// @src 140066a50  unit normal n of the triangle x(3,3) ((x2 - x1) x (x3 - x1), retried with unit
// edges); *bad = 1 when still degenerate
void fun_140066a50(const double* x, double* n, i32* bad);
// @src 140066ff1  c = a x b, *q = |c|, c normalised; c = 0, *q = 0 when |c|^2 <= 1e-16f
void fun_140066ff1(const double* a, const double* b, double* c, double* q);
// @src 14006790a  *empty = 1, 0 when the box query 140228c60 of cube (obj, inst) finds a polygon
void fun_14006790a(i32* obj, i32* inst, double* box, i32* empty);
// @src 14006798b  MDSPS2_REC: projection of p(3) on the surface of cube (obj, inst)
// (GENCTC_3d_search.md sec. 9). erfield: ER field of the edge test (3); npoly: only printed;
// st(2) natural coordinates, *poly polygon (0 none), *d signed distance (in: overwritten),
// *status 1 found / 0 / -1 failed, q(3) foot, dirn(4) direction + cosine limit, *mode > 0 all
// polygons, 0 valid ones, < 0 -(object) in contact; *dbg == 1 debug messages on unit 1.
void fun_14006798b(i32* obj, i32* inst, i32* erfield, i32* npoly, double* p, double* st, i32* poly,
                   double* d, i32* status, double* q, double* dirn, i32* mode, i32* dbg);
// @src 14006de2d  vertex re-search (sec. 10.7): sign of *d from the polygon hit first by the ray
// from p to the centroid of the largest polygon around *node, among list(1..*n). MDSPS2_REC
// passes the search mode as an eighth argument, which the routine never reads.
void fun_14006de2d(i32* obj, i32* inst, i32* n, i32* list, i32* node, double* p, double* d);
// @src 14006e938  stable partition of list(1..*n) by EI2: == -*k first (only when *k > 0), then
// EI2 > 0, then EI2 < 1 (except -*k)
void fun_14006e938(i32* obj, i32* inst, i32* list, i32* n, i32* k);
// @src 14006fb9b  projection of p on the polygon x: *kind 0 quad (1404d203b, distance signed by
// (p - q).nrm, inside code 1404d3e13), 1 triangle (1400702c8 with |*din|, *trimode = 1 after)
void fun_14006fb9b(double* x, double* nrm, i32* kind, double* p, double* s, double* t, double* q,
                   double* din, double* d, i32* trimode, i32* flag, i32* inside);
// @src 1400702c8  projection of p on the triangle x(3,3) (sec. 10.1): q, *d (signed), (*s, *t),
// *code 0 inside / k edge / -k vertex; *mode 1 rejects results farther than |*d| (*found = 0)
void fun_1400702c8(const double* p, const double* x, double* q, double* d, double* s, double* t,
                   i32* mode, i32* found, i32* code);
// @src 140070bf5  nearest point f on the segment a-b to p: *r in [0, 1], *dist = |f - p|
void fun_140070bf5(const double* p, const double* a, const double* b, double* f, double* r,
                   double* dist);
// @src 1400839c6  sign of the volume of (polygon edge *edge, p) (sec. 10.6): *sgn 1, 0, -1
void fun_1400839c6(const double* p, const double* x, const i32* nv, const i32* edge, i32* sgn);

} // namespace def::cmd::ctc3d
