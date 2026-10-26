// defrt/ftn/geoutl_poly.hpp: part of the geometry kernel library, see geoutl.hpp for the API
// contract and docs/re/status/geoutl.md for the routine list.
//
// Point in polygon, point-segment (2D and 3D), and the bilinear (4-node quad) inverse map:
// given the corner coordinates x(3,4) of a quad and a point p, find (s,t) in [-1,1]^2 with
// N(s,t)·x = p, analytically (1404cd2e1) or by bisection (1404ce05b), plus the Newton
// projection of a point onto a curved patch (1404d5a1d with its line search 1404d6647).
#pragma once

#include <cstddef>
#include <cstdint>

namespace def::rt::geoutl {

// ---- SAVE state (one struct per routine, members at the binary offsets, named by the low hex
// digits of their address: d_ double, i_ INTEGER; pad_* are never touched). Each struct spans
// exactly the routine's SAVE area, so tests compare it byte for byte with the original's.

// @data 14072fdd8  SAVE of 1404cc289 (point in polygon), 0x14072fdd8..0x14072ff18
struct PolySave {
    std::int32_t i_dd8;         // edges to test (n, or n-1 when open)
    std::int32_t i_ddc;         // nearest vertex (loop index)
    std::int32_t i_de0;         // vertex number (through idx)
    std::int32_t pad_de4;
    double d_de8;               // distance to the nearest vertex
    std::int32_t i_df0;         // loop counter
    std::int32_t pad_df4;
    double d_df8;               // distance to the current vertex
    double d_e00[2];            // copy of p (in/out point of 1404d064d)
    double pad_e10[2];
    std::int32_t i_e20, i_e24;  // edge ends (loop indices)
    std::int32_t i_e28, i_e2c;  // edge ends (vertex numbers)
    double d_e30;               // edge length; later |dist - nearest vertex distance|
    double d_e38, d_e40, d_e48; // 1404d064d: length, signed distance, parameter
    double d_e50, d_e58;        // signed distance and parameter of the accepted edge
    std::int32_t i_e60, i_e64;  // previous / next vertex (loop indices)
    std::int32_t i_e68, i_e6c, i_e70; // previous / nearest / next vertex numbers
    std::int32_t pad_e74;
    double d_e78, d_e80, d_e88; // 1404d064d on edge previous-nearest
    double d_e90, d_e98, d_ea0; // 1404d064d on edge nearest-next
    double pad_ea8;
    double d_eb0[3];            // next - nearest (z = 0)
    double pad_ec8;
    double d_ed0[3];            // previous - nearest (z = 0)
    double pad_ee8;
    double d_ef0[3];            // cross product (1404cce5e)
    double pad_f08;
    double d_f10;               // its length
};
static_assert(sizeof(PolySave) == 0x140);
static_assert(offsetof(PolySave, i_e20) == 0x48 && offsetof(PolySave, d_e78) == 0xa0);
static_assert(offsetof(PolySave, d_eb0) == 0xd8 && offsetof(PolySave, d_f10) == 0x138);
inline PolySave g_poly{};

// @data 14072ff70  SAVE of 1404ccf58 (bilinear inverse-map driver), 0x14072ff70..0x14072fff8
struct BilinSave {
    double d_f70[4];            // x row: st, s, t coefficients, constant - p(1)
    double d_f90[4];            // y row
    std::int32_t i_fb0;         // 1 when both st coefficients exceed 1e-5 in magnitude
    std::int32_t pad_fb4;
    double pad_fb8;
    double d_fc0[3];            // x(s,t) of the first root (1404d6974)
    double pad_fd8;
    double d_fe0, d_fe8;        // p - x(s,t)
    double d_ff0;               // squared residual
};
static_assert(sizeof(BilinSave) == 0x88 && offsetof(BilinSave, d_fc0) == 0x50);
inline BilinSave g_bilin{};

// @data 14072fff8  SAVE of 1404cd2e1 (bilinear analytic solve), 0x14072fff8..0x1407300a8
struct BilinSolveSave {
    std::int32_t i_ff8;         // case: 0 rows subtracted, 1 row 1 linear, 2 row 2 linear
    std::int32_t pad_ffc;
    double d_000[3];            // the linear row E (s, t, constant)
    double pad_018;
    double d_020[4];            // roots (s1, t1, s2, t2)
    std::int32_t i_040;         // number of roots
    std::int32_t pad_044;
    double d_048, d_050, d_058; // quadratic a, b, c
    double d_060, d_068;        // discriminant, its square root
    double d_070, d_078;        // s1, s2
    double d_080, d_088;        // t1, t2
    double d_090;               // denominator of the linear cases
    std::int32_t i_098, i_09c, i_0a0; // accepted roots, root loop, component loop
    std::int32_t pad_0a4;
};
static_assert(sizeof(BilinSolveSave) == 0xb0 && offsetof(BilinSolveSave, d_020) == 0x28);
static_assert(offsetof(BilinSolveSave, d_090) == 0x98);
inline BilinSolveSave g_bilin_solve{};

// @data 1407300a8  SAVE of 1404ce05b (bilinear bisection), 0x1407300a8..0x140730208
struct BisectSave {
    double d_0a8;               // 1e-6
    std::int32_t i_0b0;         // halvings
    std::int32_t pad_0b4;
    double d_0b8, d_0c0;        // sums of the corner x, y
    std::int32_t i_0c8;         // loop counter
    std::int32_t pad_0cc;
    double d_0d0, d_0d8;        // corner 1 - centroid
    double d_0e0, d_0e8;        // edge (y, x): first corner2 - corner1, then the mid line
    std::int32_t i_0f0;         // orientation +1 / -1
    std::int32_t i_0f4;         // loop counter
    double pad_0f8;
    double d_100[12];           // working quad w(3,4)
    double d_160, d_168;        // s interval (lo, hi) of the bisection
    double d_170;               // midpoint
    double d_178;               // accumulator
    std::int32_t i_180;         // loop counter
    std::int32_t pad_184;
    double pad_188;
    double d_190[6];            // mid line end points m(3,2)
    double d_1c0, d_1c8;        // base point of the mid line
    double d_1d0;               // its angle (atan2)
    double d_1d8, d_1e0;        // sin, cos
    double d_1e8, d_1f0;        // signed distance of p from the line, its magnitude
    double d_1f8, d_200;        // line length, projection of p
};
static_assert(sizeof(BisectSave) == 0x160 && offsetof(BisectSave, d_100) == 0x58);
static_assert(offsetof(BisectSave, d_190) == 0xe8 && offsetof(BisectSave, d_200) == 0x158);
inline BisectSave g_bisect{};

// @data 140730490  SAVE of 1404d064d (point-segment 2D), 0x140730490..0x1407304d0
struct PointSegSave {
    double d_490, d_498;        // unit direction
    double d_4a0, d_4a8;        // p - a
    double d_4b0;               // signed normal distance
    double d_4b8;               // projection length
    std::int32_t i_4c0;         // 0 interior, 1 nearest a, 2 nearest b
    std::int32_t pad_4c4;
    double d_4c8;               // |p - b|
};
static_assert(sizeof(PointSegSave) == 0x40);
inline PointSegSave g_pointseg{};

// @data 140731280  SAVE of 1404d5a1d (Newton projection), 0x140731280..0x140731308
struct NewtonSave {
    std::int32_t i_280;         // iteration count
    std::int32_t pad_284;
    double d_288;               // step factor (1404d6647)
    double d_290, d_298;        // Newton step (ds, dt)
    double d_2a0, d_2a8, d_2b0, d_2b8; // Jacobian entries
    double d_2c0;               // determinant
    double pad_2c8;
    double d_2d0, d_2d8, d_2e0, d_2e8; // inverse
    double d_2f0, d_2f8;        // minus the residuals
    double d_300;               // |step|
};
static_assert(sizeof(NewtonSave) == 0x88 && offsetof(NewtonSave, d_2d0) == 0x50);
inline NewtonSave g_newton{};

// @data 140731398  SAVE of 1404d6647 (line search), 0x140731398..0x140731428
struct LineSearchSave {
    double d_398, d_3a0;        // (s, t) at step 0
    double pad_3a8;
    double d_3b0[3];            // x(s, t)
    double pad_3c8;
    double d_3d0[3];            // unit vector to p (1404d68dd)
    double pad_3e8;
    double d_3f0;               // distance at step 0
    double d_3f8, d_400;        // (s, t) at half step
    double d_408;               // distance there
    double d_410, d_418;        // (s, t) at full step
    double d_420;               // distance there
};
static_assert(sizeof(LineSearchSave) == 0x90 && offsetof(LineSearchSave, d_3f0) == 0x58);
inline LineSearchSave g_linesearch{};

// @data 140731430  SAVE of 1404d6974 (bilinear shape functions), 0x140731430..0x140731458
struct ShapeSave {
    double d_430[4];            // N1..N4
    std::int32_t i_450, i_454;  // loop counters (component, node)
};
static_assert(sizeof(ShapeSave) == 0x28);
inline ShapeSave g_shape{};

// @data 140731460  SAVE of 1404d6b13 (PNTLIN), 0x140731460..0x140731498
struct PntlinSave {
    double d_460[3];            // b - a
    double pad_478;
    double d_480[3];            // p - a, at the end proj - p
};
static_assert(sizeof(PntlinSave) == 0x38);
inline PntlinSave g_pntlin{};

// Resets all SAVE state above (tests).
void reset_poly_globals();

// @src 1404cc289  point in polygon: nearest vertex/edge of the polygon xy (2 x n, leading dim
// *ld, vertices optionally through idx when *useidx != 0; closed when *closed == 1) to p;
// q = nearest point, *dist = |distance| (in/out: an edge is taken when its |d| <= *dist),
// *iedge = edge (or nearest vertex), *inside = side: -1 left of the edges (inside a
// counter-clockwise polygon), +1 right, 0 on the boundary (*dist == 0)
void fun_1404cc289(const double* xy, const double* p, double* q, double* dist,
                   std::int32_t* iedge, const std::int32_t* n, std::int32_t* inside,
                   const std::int32_t* closed, const std::int32_t* idx,
                   const std::int32_t* useidx, const std::int32_t* ld);

// @src 1404d064d  point-segment (2D): p against segment a-b; *len = |b-a|, *dist = signed
// distance (normal distance inside, signed end-point distance outside), *t = projection / len;
// when *mode != 1 p is replaced by the nearest point
void fun_1404d064d(double* p, const double* a, const double* b, double* len, double* dist,
                   double* t, const std::int32_t* mode);

// @src 1404ccf58  bilinear inverse map driver: (s,t) of p (2) in the quad x(3,4) (x, y used);
// st(2,2) the roots, *nroot their count (in: 1 lets a root with residual^2 < 1e-6 stand),
// *ierr = 0 found / 1 not; otherwise falls back to 1404ce05b with *nroot = 1
void fun_1404ccf58(const double* x, const double* p, double* st, std::int32_t* nroot,
                   std::int32_t* ierr);

// @src 1404cd2e1  bilinear analytic solve of a1 st + a2 s + a3 t + a4 = 0, b1 st + b2 s + b3 t
// + b4 = 0 (normalises the rows in place); roots in r(2,2) inside [-1.001,1.001] clamped
// to [-1,1], *nroot = count, *ierr = 0 / 1
void fun_1404cd2e1(double* a1, double* a2, double* a3, double* a4, double* b1, double* b2,
                   double* b3, double* b4, double* r, std::int32_t* nroot, std::int32_t* ierr);

// @src 1404ce05b  bilinear bisection fallback: (s,t) of p (2) in the quad x(3,4) by bisecting
// the s-interval against the mid line; st(2) out, *ierr = 1 after 50 halvings
void fun_1404ce05b(const double* x, const double* p, double* st, std::int32_t* ierr);

// @src 1404d6974  bilinear interpolation: out(3) = sum_k N_k(s,t) x(:,k), x(3,4)
void fun_1404d6974(const double* x, double* out, const double* s, const double* t);

// @src 1404d6647  line search of 1404d5a1d: halves *step (start 1) until the point at
// (s,t) + step*(ds,dt) is nearer to p than the half step and the start, or step < 0.001
void fun_1404d6647(const double* x, const double* p, const double* s, const double* t,
                   const double* ds, const double* dt, double* step);

// @src 1404d6b13  PNTLIN: projection of p (3) on the line a-b clamped to the segment;
// proj(3), *t in [0,1], *dist = |proj - p|; zero-length segment: WRITE ' ERROR IN PNTLIN '
// and a(1..3) to unit 6, call 1404edb2f, then continue
void fun_1404d6b13(const double* a, const double* b, const double* p, double* proj, double* t,
                   double* dist);

// @src 1404d5a1d  Newton projection on a quadratic patch: c1(6), c2(6) the coefficients of the
// two residual equations in (s,t); (s,t) in/out; x(3,4), p the quad and point for the line
// search; *ierr = 1 when not converged (31 iterations or step < 0.001)
void fun_1404d5a1d(const double* c1, const double* c2, double* s, double* t, const double* x,
                   const double* p, std::int32_t* ierr);

} // namespace def::rt::geoutl
