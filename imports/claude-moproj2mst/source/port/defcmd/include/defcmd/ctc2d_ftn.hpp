// defcmd/ctc2d_ftn.hpp: the Fortran kernels of 2D GENCTC (registry id 6, 2D driver 1401a2e90).
// Spec: docs/re/commands/GENCTC.md section 4 (branch claude/project-thread-6cvah9).
//
// Fortran calling style: every argument by pointer, arrays 1-based in the comments and
// column-major (xy(2, n): node i at xy[2*(i-1)], xy[2*(i-1)+1]; codes bc3(3, n)). The routines
// keep their locals in SAVE storage (fixed addresses in .data/.bss); those that are read before
// being written in a later call (state carried from call to call) are the globals in
// ctc2d_save.hpp, ported with their addresses. Operation order comes from the listings.
#pragma once

#include <cstdint>

namespace def::cmd::ftn {

// @src 140350f12  rigid master: per node k = 1..*n of the loop list nodes(*n), find the nearest
// segment (140436a9e) of the die curve seg(6, *nseg); move the node and set its code to -*mobj
// (rule of GENCTC.md 4.3) when in contact; warnings 1001/1002/1004. *changed = 1 when a node
// was moved, *fail = 1 when a node was put back (1002/1004). p6/p7 are passed through unused.
void fun_140350f12(double* xy, const std::int32_t* n, const std::int32_t* nodes,
                   const std::int32_t* nseg, double* seg, const std::int32_t* p6,
                   const std::int32_t* p7, const std::int32_t* mobj, std::int32_t* bc3,
                   const double* tol, std::int32_t* changed, const std::int32_t* do_codes,
                   std::int32_t* fail);

// @src 140436a9e  nearest segment of seg(6, *nseg) to p: *d signed distance, *iseg the segment
// (0 when none), *st = 1 when *tol < *d (not in contact), else 2 and p moved onto the segment.
// *p8 is set to 0.
void fun_140436a9e(const std::int32_t* nseg, double* seg, double* p, const double* tol,
                   double* d, std::int32_t* iseg, std::int32_t* st, std::int32_t* p8);

// @src 1404345c0  point p to segment a-b: *len = |b - a| (0 when degenerate), *d signed distance
// (normal distance inside, +-distance to the nearer end outside), *s = position along a-b over
// the length; *mode != 1 moves p onto the segment (degenerate segment: only *mode == 2).
void fun_1404345c0(double* p, const double* a, const double* b, double* len, double* d,
                   double* s, const std::int32_t* mode);

// @src 14043574f  point p to the arc starting at a with centre c and sweep *ang (radians):
// *d signed distance, *ang_out = *ang; *mode != 1 moves p onto the arc.
void fun_14043574f(double* p, const double* a, const double* c, const double* ang, double* d,
                   double* ang_out, const std::int32_t* mode);

// @src 140435caa  *in = 1 when x lies within the sweep of the arc (start a, centre c, sweep *ang)
void fun_140435caa(const double* a, const double* c, const double* ang, const double* x,
                   std::int32_t* in);

// @src 140435625  *out = angle from vector a to vector b (atan2 of the normalised cross and dot
// products), 0 when either is shorter than 1e-10
void fun_140435625(const double* a, const double* b, double* out);

// @src 14043556b  rotate the vector v by *ang (radians), in place
void fun_14043556b(double* v, const double* ang);

// @src 14034fc56  meshed master: per node of the slave loop nodes(*n), nearest segment of the
// master's boundary loops; move the node and set its code to -*mobj when |d| <= *tol (warning
// 1001). xy slave coordinates, mxy master coordinates, loops(2, *nloops) = first/last+1 positions
// in mnodes, mnodes the master loop node list.
void fun_14034fc56(double* xy, const std::int32_t* n, const std::int32_t* nodes, double* mxy,
                   const std::int32_t* nloops, std::int32_t* loops, std::int32_t* mnodes,
                   const std::int32_t* mobj, std::int32_t* bc3, const double* tol,
                   std::int32_t* changed, const std::int32_t* do_codes);

// @src 14032e34e  element check: *ok = 1 when the area-like measure of quad *e (conn(4, *))
// on the coordinates xy is > 0, else 0; *p5 = 0
void fun_14032e34e(const std::int32_t* e, const std::int32_t* conn, const double* xy,
                   std::int32_t* ok, std::int32_t* p5);

} // namespace def::cmd::ftn
