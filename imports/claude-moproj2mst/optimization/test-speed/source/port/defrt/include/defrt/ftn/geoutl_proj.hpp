// defrt/ftn/geoutl_proj.hpp: part of the geometry kernel library, see geoutl.hpp for the API
// contract and docs/re/status/geoutl.md for the routine list.
//
// Quadrilateral projection library (FTN_geoutl): project a point P on the bilinear 4-node
// surface X(3,4) (planar quads in a local frame, curved ones by Newton on the patch, with
// triangle and edge fall-backs), the (s,t) location codes of quads and triangles, point boxes
// and box overlap, and a column sort. Bodies in src/ftn/geoutl_proj.cpp (1404d41f8 in
// src/ftn/geoutl_d41f8.cpp, owned by p67-3). Summary: docs/re/commands/GENCTC_3d_search.md,
// section "Quad projection library (ported)".
#pragma once

#include <cstddef>
#include <cstdint>

namespace def::rt::geoutl {

// ---- SAVE state -------------------------------------------------------------------------------
// Every member sits at the binary's address (the comment; static_asserts in the .cpp check the
// offsets from the block start).

// @data 1407306e0  SAVE of fun_1404d1b1c, fun_1404d1d05, fun_1404d203b and fun_1404d275e
struct QuadProjState {
    // fun_1404d1b1c (box of listed points)
    std::int32_t b1b_c;        // 1407306e0 component index
    std::int32_t b1b_i;        // 1407306e4 list index
    std::int32_t b1b_node;     // 1407306e8 idx(i)
    // fun_1404d1d05 (box of all points)
    std::int32_t b1d_c;        // 1407306ec component index
    std::int32_t b1d_i;        // 1407306f0 point index
    // fun_1404d203b
    std::int32_t nonplanar;    // 1407306f4 6th argument of fun_1404d4a0f
    std::int32_t newton_fail;  // 1407306f8 ierr of fun_1404d5a1d
    std::int32_t edge_code;    // 1407306fc code of fun_1404d275e
    std::int32_t i;            // 140730700 loop index
    std::int32_t pad704[3];
    double c1[6];              // 140730710 Newton coefficients, equation 1
    double c2[6];              // 140730740 Newton coefficients, equation 2
    double a, b, c, d;         // 140730770..788 per-axis coefficients (last axis)
    double b_copy, d_copy;     // 140730790, 798 copies of b and d
    std::int32_t skip;         // 1407307a0 corner left out (code / 10000)
    std::int32_t ntri;         // 1407307a4 corners copied
    std::int32_t j;            // 1407307a8 corner index
    std::int32_t pad7ac;
    double tri[9];             // 1407307b0 triangle (3,3) of the other three corners
    double pad7f8;
    double tri_s, tri_t;       // 140730800, 808 s, t of fun_1404e2914
    double st[2];              // 140730810 (s, t) of fun_1404ddeba
    double pad820[2];
    std::int32_t nroot;        // 140730830 root count of fun_1404ddeba
    std::int32_t ierr;         // 140730834 ierr of fun_1404ddeba (not read)
    double pad838;
    double u[3];               // 140730840 unit vector of fun_1404d68dd
    double pad858;
    std::int32_t loc;          // 140730860 code of fun_1404d3e13 (not read)
    // fun_1404d275e
    std::int32_t k;            // 140730864 loop index / edge
    double pad868;
    double cen[3];             // 140730870 centroid 0.25 * sum of the corners
    double pad888;
    std::int32_t j2;           // 140730890 inner loop index
    std::int32_t k_copy;       // 140730894 edge start corner
    std::int32_t k2;           // 140730898 edge end corner
    std::int32_t pad89c;
    double t3[9];              // 1407308a0 triangle (corner k, corner k2, centroid)
    double pad8e8;
    double q3[3];              // 1407308f0 foot point of fun_1404d3094 / fun_1404d6974
    double pad908;
    double d3;                 // 140730910 distance of fun_1404d3094
    std::int32_t code3;        // 140730918 code of fun_1404d3094
    std::int32_t pad91c;
    double n[3];               // 140730920 direction Q - P (or triangle normal)
    double pad938;
    double len;                // 140730940 |Q - P|
    std::int32_t one;          // 140730948 flag 1 of fun_1404d0939
    std::int32_t pad94c;
    double rot[9];             // 140730950 frame of n (fun_1404cf5c0)
    double pad998;
    double xl[12];             // 1407309a0 corners in the frame at P
    double roots[8];           // 140730a00 roots of fun_1404ccf58
    std::int32_t nr;           // 140730a40 root count
    std::int32_t ir;           // 140730a44 root index
    double rs, rt;             // 140730a48, a50 current root
    double pada58;
    double u2[3];              // 140730a60 unit vector of fun_1404d68dd
    double pada78;
    double dr;                 // 140730a80 distance of the root's point
};
inline QuadProjState g_quad_proj{};

// @data 140730d30  SAVE of fun_1404d3e13 (quad location code)
struct QuadLocState {
    double hi;                 // 140730d30 1 - e
    double lo;                 // 140730d38 e + (-1)
    std::int32_t count;        // 140730d40 edges whose band holds (s, t)
    std::int32_t k;            // 140730d44 edge / corner
    std::int32_t k2;           // 140730d48 next corner
    std::int32_t pad1c;
    double s_lo, s_hi;         // 140730d50, d58 band in s
    double t_lo, t_hi;         // 140730d60, d68 band in t
    double dmin;               // 140730d70 nearest corner distance
    double d2;                 // 140730d78 squared distance
    double d;                  // 140730d80 distance
};
inline QuadLocState g_quad_loc{};

// @data 140730e28  SAVE of fun_1404d4a0f (planar quad projection)
struct QuadPlaneState {
    std::int32_t three;        // 140730e28 3 (last argument of fun_1404cc289)
    std::int32_t one;          // 140730e2c 1 (flag of fun_1404d0939)
    std::int32_t same;         // 140730e30 X(:,1) == X(:,4) (triangle)
    std::int32_t nv;           // 140730e34 3 or 4 (not read)
    double pad010;
    double n[3];               // 140730e40 normal
    double pad038;
    double q0[3];              // 140730e60 origin N(s,t)X, then nearest point / Q
    double pad058;
    double rot[9];             // 140730e80 frame
    double pad0a8;
    double xl[12];             // 140730ed0 corners in the frame
    std::int32_t k;            // 140730f30 corner index of the planarity test
    std::int32_t pad114;
    double dz;                 // 140730f38 |z_k - z_1|
    double pl[3];              // 140730f40 P in the frame
    double pad138;
    double dmin;               // 140730f60 distance bound / result of fun_1404cc289
    std::int32_t iedge;        // 140730f68
    std::int32_t outside;      // 140730f6c > 0: P outside the polygon
    std::int32_t idx[4];       // 140730f70 (not used: useidx = 0)
    double st[2];              // 140730f80 (s, t) of fun_1404ccf58
    std::int32_t nroot;        // 140730f90
    std::int32_t ierr;         // 140730f94
    double pad178;
    double u[3];               // 140730fa0 unit vector of fun_1404d68dd
};
inline QuadPlaneState g_quad_plane{};

// @data 1407544a0  SAVE of fun_1404ddeba (planar inverse map of a point in the quad frame)
struct QuadInvState {
    double n[3];               // 1407544a0 quad normal at (0, 0)
    double pad018;
    double rot[9];             // 1407544c0 frame
    double pad068;
    double q0[3];              // 140754510 origin N(0,0)X
    double pad088;
    std::int32_t i;            // 140754530 row index
    std::int32_t pad094[3];
    double pl[3];              // 140754540 P in the frame
    double pad0b8;
    std::int32_t j;            // 140754560 column index
    std::int32_t kc;           // 140754564 corner index
    double pad0c8;
    double xl[12];             // 140754570 corners in the frame
    double tol;                // 1407545d0 tolerance
    double dmin;               // 1407545d8 distance bound / result of fun_1404cc289
    double q[3];               // 1407545e0 nearest point, then N(s,t) xl
    double pad158;
    std::int32_t iedge;        // 140754600
    std::int32_t outside;      // 140754604
    std::int32_t idx[4];       // 140754608 (not used: useidx = 0)
};
inline QuadInvState g_quad_inv{};

// @data 140755700  SAVE of fun_1404e37c4 (triangle location code)
struct TriLocState {
    double l[3];               // 140755700 L1 = s, L2 = t, L3 = (1 - s) - t
    double pad18;
    std::int32_t i;            // 140755720
    std::int32_t pad24;
    double dl;                 // 140755728 |L_i - 1|
};
inline TriLocState g_tri_loc{};

// @data 140731818  SAVE of fun_1404d93ae (column sort)
struct ColSortState {
    std::int32_t i;            // 140731818 pass
    std::int32_t m;            // 14073181c column compared with column m - 1
    std::int32_t j;            // 140731820 inner loop index
    std::int32_t r;            // 140731824 row index of the swap
    double pad10;
    double tmp[64];            // 140731830 swapped column, tmp(r) (the binary does not
                               // bound it; the callers pass *ld <= 64)
};
inline ColSortState g_col_sort{};

// fun_1404ddeba also reads the COMMON 140b84720 (u08 = 140b84728 tolerance, u10 = 140b84730
// characteristic length): def::rt::geoutl::g_common_140b84720 in geoutl_tri.hpp.

// Resets the SAVE state above (tests).
void reset_proj_globals();

// ---- routines ---------------------------------------------------------------------------------

// 1404d203b: project P(3) on the quad X(3,4): (*s, *t) and Q(3) = N(s,t)X, *dist = |P - Q|
// (unsigned). *flag is set by the triangle-split path (fun_1404e2914) only, else 0.
void fun_1404d203b(const double* x, const double* p, double* s, double* t, double* q,
                   double* dist, std::int32_t* flag);

// 1404d275e: edge fall-back of fun_1404d203b: nearest point of P on the four triangles
// (corner k, corner k+1, centroid); *code = code of the best triangle, or k*10000 when that
// triangle is degenerate; then (s, t) of the best point by the planar inverse map in a frame
// along Q - P (several roots: the nearest wins). Q(3), *dist, *s, *t out.
void fun_1404d275e(const double* x, const double* p, double* q, double* dist, double* s,
                   double* t, std::int32_t* code);

// 1404d41f8 (PNTEDG): nearest edge/corner fall-back: (*s, *t), Q(3) and *dmin of the nearest
// point of P on the boundary of X(3,4) (triangle when X(:,1) == X(:,4)). Body owned by p67-3
// (src/ftn/geoutl_d41f8.cpp).
// (x and p are not written; the signature is the one of the body.)
void fun_1404d41f8(double* x, double* p, double* xi, double* eta, double* q, double* dmin);

// 1404d4a0f: planar projection: frame of the quad normal at (0,0) (triangle normal when
// X(:,1) == X(:,4)); *nonplanar = 1 when some |z_k - z_1| > 1e-6 in that frame. Planar: inside
// -> inverse bilinear map, *dist = |Q - P|; outside -> fun_1404d41f8.
void fun_1404d4a0f(const double* x, const double* p, double* s, double* t, double* dist,
                   std::int32_t* nonplanar);

// 1404ddeba: (s,t) = st(2) of the point P in the quad X(3,4), all in the frame of the quad
// normal at (0,0): *ierr = 1 when P is outside the quad by more than the tolerance
// (COMMON 140b84728, else 0.01f * 140b84730, else 0.02f) or off the surface by more than
// 5 tolerances; *nroot passed on to fun_1404ccf58.
void fun_1404ddeba(const double* x, const double* p, double* st, std::int32_t* nroot,
                   std::int32_t* ierr);

// 1404d3e13: location code of (s,t) in the quad: 0 inside (-0.999, 0.999)^2, k = 1..4 on edge k
// (band 0.001 wide), -k near corner k; see the .cpp for the exact rule.
void fun_1404d3e13(const double* s, const double* t, std::int32_t* code);

// 1404e37c4: location code of the triangle coordinates (s, t, 1-s-t): -i near vertex i
// (|L_i - 1| < 0.001), else edge code i+1 (4 -> 1) for the first |L_i| < 0.001, else 0.
void fun_1404e37c4(const double* s, const double* t, std::int32_t* code);

// 1404d1b1c: box(6) = (min xyz, max xyz) of the points X(:, idx(1..*n)) widened by *margin.
void fun_1404d1b1c(const double* x, double* box, const double* margin, const std::int32_t* n,
                   const std::int32_t* idx);

// 1404d1ed7: *ov = 1 when the boxes a(6), b(6) overlap (closed intervals; NaN overlaps), else 0.
void fun_1404d1ed7(const double* a, const double* b, std::int32_t* ov);

// 1404d1d05: box(6) of the points X(:, 1..*n) widened by *margin.
void fun_1404d1d05(const double* x, double* box, const double* margin, const std::int32_t* n);

// 1404d93ae: bubble sort of the *n columns of a(*ld, *n) by row *key: ascending when
// *order == 1, else descending (columns swapped whole).
void fun_1404d93ae(double* a, const std::int32_t* key, const std::int32_t* n,
                   const std::int32_t* order, const std::int32_t* ld);

} // namespace def::rt::geoutl
