// defcmd/arc_refine.hpp: the 2D arc contact refinement of the OBJPOS interference arc kernel
// (FUN_1403407d7, docs/re/commands/OBJPOS_interference_kernels.md 4.4 / 4.5), packet p67-6.
//
// Fortran calling style: every argument by pointer, INTEGER = std::int32_t, REAL*8 = double,
// 2-vectors as double[2]. Both routines keep their locals in SAVE storage at fixed addresses;
// none is carried from one call to the next, but 14034151e reads one element past its angle
// table (the next SAVE variable) and passes 140341100's SAVE variables by address, so both
// blocks are kept as globals with their recovered layout. reset_arc_refine_save() zeroes them.
//
// Frame used below: Q (the query point) is the origin, u = dir/|dir| the ray direction; a point
// v is written (par, perp) = (v.u, u x v) = (vy*uy + vx*ux, vy*ux - vx*uy). The probe is a
// circle of radius *r (the caller's tolA) moving along u; the arc has centre C and radius R.
#pragma once

#include <cstddef>
#include <cstdint>

namespace def::cmd::ftn {

// @data 140702490  SAVE locals of 140341100
struct Save140341100 {
    double d[2];          // 140702490 dir - vel
    double len;           // 1407024a0 |dir - vel|
    double rad;           // 1407024a8 arc radius |S - C|
    double u[2];          // 1407024b0 unit ray direction
    double cf[2];         // 1407024c0 centre in the ray frame (par, perp)
    double p1[2];         // 1407024d0 first end point in the frame (smaller par)
    double p2[2];         // 1407024e0 second end point in the frame
    std::int32_t flag;    // 1407024f0 1 = S first, 2 = end points swapped
    std::int32_t pad4f4;
    double phi;           // 1407024f8 sweep seen from p1 (negated when swapped)
    double swp[2];        // 140702500 copy of S in the frame, written only when swapped
    std::int32_t status;  // 140702510 status of 14034151e
};
inline Save140341100 g_save_140341100{};

// @data 140702518  SAVE locals of 14034151e
struct Save14034151e {
    double r;             // 140702518 probe radius (copy of *r)
    std::int32_t mode;    // 140702520 1, or 2 when R == r (skip the R - r branch)
    std::int32_t pad524;
    double gnum;          // 140702528 gap before division by |dir|
    std::int32_t i;       // 140702530 loop index
    std::int32_t k;       // 140702534 loop index
    double pad538;        // 140702538
    // 140702540 A(2,12): A(1,j) candidate angle, A(2,j) = 1.0 when the interval j..j+1 is
    // within r of the arc circle. a[24] is 140702600 (asin argument x) and a[25] is
    // 140702608 (|1 - |x||): the code reads A(1,13) = x when the table is full.
    double a[26];
    std::int32_t n3;      // 140702610 table length (3 n, then n after merging)
    std::int32_t pad614;
    double mid;           // 140702618 interval mid angle
    double v;             // 140702620 cy / sin(angle) + R
    double one;           // 140702628 1.0 (written, never read)
    std::int32_t n;       // 140702630 table length after merging
    std::int32_t pad634;
    double pad638;        // 140702638
    double vec[2];        // 140702640 vector argument of 14032beef (later cos of the result)
    double ex[2];         // 140702650 (1, 0)
    double th0;           // 140702660 angle of Q seen from C, in [0, 2 pi)
    double th1;           // 140702668 angle of p1 seen from C, in [0, 2 pi)
    double dd;            // 140702670 |A(1,i) - th0|
    std::int32_t idx;     // 140702678 chosen table column
    std::int32_t pad67c;
    double val;           // 140702680 th1 + sweep (angle of the other end point)
    double chosen;        // 140702688 contact angle
    double h;             // 140702690 |cy / sin(chosen)|
};
inline Save14034151e g_save_14034151e{};

void reset_arc_refine_save();

// @src 140341100  arc contact refinement driver, called by the arc kernel 1403407d7 as
// (S, tmp, C, &phi, zeros2, Q, dir, &tolA, &g, &f2) when the probe at Q touches the arc.
// s: arc start point, e: out, the arc end point rot(S - C, *phi) + C; c: centre; phi: sweep
// (radians, signed); vel: velocity subtracted from dir (zeros here); q: query point; dir: ray
// direction (any length); r: probe radius / tolerance; g: out, gap along dir in units of |dir|
// (written by 14034151e, see there); res: out, -1 no contact (status 0), 0 status > 0 (contact
// at a table angle), status < 0 (contact at the arc's other end): 2 when S was first, 1 when the
// end points were swapped.
void fun_140341100(const double* s, double* e, const double* c, const double* phi,
                   const double* vel, const double* q, const double* dir, const double* r,
                   double* g, std::int32_t* res);

// @src 14034151e  ray/arc contact in angle space. p1: first end point (par, perp) in the ray
// frame, p2: second end point (not read), cf: centre in the frame, rad: arc radius R, r: probe
// radius, phi: sweep from p1, g: out, gap / *len, len: |dir|, st: out status: 1 contact at a
// table angle or an early special case, -1 contact at the other end point, 0 none (also when the
// gap is negative; *g is still written then), and on paths that return early with 0, *g is not
// written. The candidate angles are the asin solutions of cy / (r - R) and cy / -(r + R), sorted
// with +-2 pi copies (1404d93ae), intervals within r of the circle marked and merged; the
// interval holding th0 (Q seen from C) decides.
void fun_14034151e(const double* p1, const double* p2, const double* cf, const double* rad,
                   const double* r, const double* phi, double* g, const double* len,
                   std::int32_t* st);

// ---- callees not yet declared in def::rt::geoutl (local stand-ins, see arc_refine_ext.cpp) ----

// 1404d71d7: rotate v (2) by *ang about the origin, in place. Forwards to the existing p1-11
// port def::model::topo::rotate_2d until def::rt::geoutl::fun_1404d71d7 is declared.
void fun_1404d71d7(double* v, const double* ang);

// 1404d93ae: bubble sort of the *n columns of a(*nrow, *n) by row *key, ascending when
// *dir == 1, else descending. Forwards to def::rt::geoutl::fun_1404d93ae.
void fun_1404d93ae(double* a, const std::int32_t* key, const std::int32_t* n,
                   const std::int32_t* dir, const std::int32_t* nrow);

// 14032beef: *out = angle from vector a to vector b (OBJPOS interference packet, spec 4.4).
// STUB (DEF_NOT_PORTED) until the owning packet ports it.
void fun_14032beef(const double* a, const double* b, double* out);

} // namespace def::cmd::ftn

static_assert(offsetof(def::cmd::ftn::Save140341100, status) == 0x140702510 - 0x140702490);
static_assert(offsetof(def::cmd::ftn::Save140341100, phi) == 0x1407024f8 - 0x140702490);
static_assert(offsetof(def::cmd::ftn::Save14034151e, a) == 0x140702540 - 0x140702518);
static_assert(offsetof(def::cmd::ftn::Save14034151e, n3) == 0x140702610 - 0x140702518);
static_assert(offsetof(def::cmd::ftn::Save14034151e, n) == 0x140702630 - 0x140702518);
static_assert(offsetof(def::cmd::ftn::Save14034151e, h) == 0x140702690 - 0x140702518);
