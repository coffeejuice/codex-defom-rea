// defrt/ftn/geoutl_tri.hpp: part of the geometry kernel library, see geoutl.hpp for the API
// contract and docs/re/status/geoutl.md for the routine list.
//
// Triangle kernels of FTN_geoutl.f: Heron area, barycentric coordinates, tetrahedron volume,
// 2D polygon orientation, and the point-to-triangle projection drivers (1404d3094, 1404e2914,
// 1404e305a). All arrays column-major; a triangle X(3,3) holds vertex j in column j.
#pragma once

#include <defrt/ftn/common_140b84720.hpp>
#include <cstddef>
#include <cstdint>

namespace def::rt::geoutl {

// ---- SAVE state ---------------------------------------------------------------------------

// @data 140755450  SAVE of 1404e2914
struct ProjDrvState {
    double dist;          // 0x00 (140755450) distance from 1404d3094
    double zeta;          // 0x08 (140755458) third barycentric coordinate from 1404e305a
    std::int32_t flag;    // 0x10 (140755460) flag of 1404e305a
    std::int32_t pad14;
};
inline ProjDrvState g_proj_drv{};

// @data 140755468  SAVE of 1404e29fa (Heron)
struct HeronState {
    double sum;           // 0x00 (140755468) perimeter
    std::int32_t i;       // 0x08 (140755470) edge index
    std::int32_t j;       // 0x0c (140755474) next vertex
    double sumsq;         // 0x10 (140755478) squared edge length
    std::int32_t k;       // 0x18 (140755480) component index
    std::int32_t pad1c;
    double pad20;         // 0x20 (140755488) not referenced
    double len[3];        // 0x28 (140755490, 498, 4a0) edge lengths |X(:,i) - X(:,i+1)|
};
inline HeronState g_heron{};

// @data 1407554b0  SAVE of 1404e2c31
struct Area3State {
    std::int32_t i;       // 0x00 (1407554b0)
    std::int32_t pad04;
    double pad08;         // 0x08 (1407554b8) not referenced
    double x[9];          // 0x10 (1407554c0) X(3,3) = (p1, p2, p3)
};
inline Area3State g_area3{};

// @data 140755510  SAVE of 1404e2d44
struct BaryState {
    std::int32_t i;       // 0x00 (140755510)
    std::int32_t j;       // 0x04 (140755514)
    double a0;            // 0x08 (140755518) area of the triangle (x, y only)
    double a1, a2, a3;    // 0x10 (140755520, 528, 530) areas of (p,x2,x3), (p,x3,x1), (p,x2,x1)
    double sum;           // 0x28 (140755538) (l1 + l2) + l3
};
inline BaryState g_bary{};

// @data 140644700  .data work triangles of 1404e2d44 (DATA-initialised to 0). Only rows 1 and 2
// are ever written, so row 3 (z) keeps its initial 0: the areas are planar (x, y).
struct BaryWork {
    double t0[9];         // 0x000 (140644700) copy of X
    double pad48;
    double t1[9];         // 0x050 (140644750) (p, x2, x3)
    double pad98;
    double t2[9];         // 0x0a0 (1406447a0) (p, x3, x1)
    double pad_e8;
    double t3[9];         // 0x0f0 (1406447f0) (p, x2, x1)
    double pad138;
    double third;         // 0x140 (140644840) 1/3 (0x3fd5555555555555), read by 1404e305a
};
inline BaryWork g_bary_work{{}, 0.0, {}, 0.0, {}, 0.0, {}, 0.0, 0x1.5555555555555p-2};

// @data 140755540  SAVE of 1404e305a
struct ProjTriState {
    double n[3];          // 0x000 (140755540) unit normal (1404d3d43)
    double pad18;
    double r[9];          // 0x020 (140755560) frame of n (1404cf5c0)
    double pad68;
    double c[3];          // 0x070 (1407555b0) centroid
    double pad88;
    double p[3];          // 0x090 (1407555d0) point in the frame
    double pad_a8;
    double x[9];          // 0x0b0 (1407555f0) triangle in the frame
    double pad_f8;
    double tol;           // 0x100 (140755640) b84728, else b84730 * 0.01f, else 0.02f
    double dmin;          // 0x108 (140755648) 1404cc289 distance (initial 1e8)
    double nearest[3];      // 0x110 (140755650) nearest point, later the point at (l1, l2)
    double pad128;
    std::int32_t edge;    // 0x130 (140755670)
    std::int32_t inout;   // 0x134 (140755674)
    std::int32_t a9;      // 0x138 (140755678) 9th argument of 1404cc289
    std::int32_t pad13c;
};
inline ProjTriState g_proj_tri{};

// @data 1407556b0  SAVE of 1404e34eb
struct BaryPtState {
    double w[3];          // 0x00 (1407556b0, 6b8, 6c0) a, b, (1 - a) - b
    double pad18;
    std::int32_t i;       // 0x20 (1407556d0)
    std::int32_t k;       // 0x24 (1407556d4)
};
inline BaryPtState g_bary_pt{};

// @data 14072fbb0  SAVE of 1404ca2cb
struct TetVolState {
    double area;          // 0x00 (14072fbb0) area of face (x1, x2, x3)
    double pad08;
    double n[3];          // 0x10 (14072fbc0) unit normal of that face
    double pad28;
    double h;             // 0x30 (14072fbe0) n . (x4 - x1)
};
inline TetVolState g_tet_vol{};

// @data 140730a88  SAVE of 1404d3094
struct FaceProjState {
    std::int32_t one;     // 0x000 (140730a88) set to 1 (5th argument of 1404d0939)
    std::int32_t three;   // 0x004 (140730a8c) set to 3
    double n[3];          // 0x008 (140730a90) unit normal
    double pad20;
    double r[9];          // 0x028 (140730ab0) frame of n
    double pad70;
    std::int32_t i;       // 0x078 (140730b00)
    std::int32_t pad7c;
    double pad80;         // 0x080 (140730b08)
    double c[3];          // 0x088 (140730b10) centroid
    double pad_a0;
    std::int32_t j;       // 0x0a8 (140730b30)
    std::int32_t pad_ac;
    double pad_b0;
    double x[9];          // 0x0b8 (140730b40) triangle in the frame
    double pad100[3];
    double p[3];          // 0x118 (140730ba0) point in the frame; p[2] = signed height
    double pad130;
    std::int32_t nneg;    // 0x138 (140730bc0) count from 1404d37d7
    std::int32_t pad13c;
    double dmin;          // 0x140 (140730bc8)
    double nearest[3];      // 0x148 (140730bd0) nearest point of 1404cc289
    double pad160;
    std::int32_t edge;    // 0x168 (140730bf0)
    std::int32_t inout;   // 0x16c (140730bf4) > 0: foot of the normal outside the triangle
    std::int32_t a9;      // 0x170 (140730bf8) 9th argument of 1404cc289
    std::int32_t pad174;
    double q[3];          // 0x178 (140730c00) nearest point on the edge (1404d6b13)
    double pad190;
    double t;             // 0x198 (140730c20) edge parameter (1404d6b13)
    std::int32_t k;       // 0x1a0 (140730c28)
    std::int32_t pad1a4;
    double t_best;        // 0x1a8 (140730c30)
};
inline FaceProjState g_face_proj{};

// @data 140730c38  SAVE of 1404d37d7
struct OrientState {
    std::int32_t last;    // 0x00 (140730c38) last vertex with a zero edge or a right turn
    std::int32_t nloop;   // 0x04 (140730c3c) 4, or 1 when n == 3
    std::int32_t i;       // 0x08 (140730c40)
    std::int32_t iprev;   // 0x0c (140730c44)
    std::int32_t inext;   // 0x10 (140730c48)
    std::int32_t pad14;
    double dx1, dy1;      // 0x18 (140730c50, c58) x_i - x_prev
    double dx2, dy2;      // 0x28 (140730c60, c68) x_next - x_i
    double cross;         // 0x38 (140730c70) dx1*dy2 - dx2*dy1, then / l1 / l2
    double l1, l2;        // 0x40 (140730c78, c80)
    std::int32_t jsrc;    // 0x50 (140730c88) source column while reversing
    std::int32_t k;       // 0x54 (140730c8c)
    double t[12];         // 0x58 (140730c90) T(3,4) reversed copy (stride 3: ldim <= 3)
};
inline OrientState g_orient{};

// COMMON 140b84720 (1404e305a reads 140b84728 and 140b84730: projection tolerance and
// characteristic length, OBJPOS_drop_solver.md 5.2): one object, def::rt::geoutl::
// g_common_140b84720 in defrt/ftn/common_140b84720.hpp (included above).

static_assert(offsetof(HeronState, len) == 0x28);
static_assert(offsetof(Area3State, x) == 0x10);
static_assert(offsetof(BaryState, sum) == 0x28);
static_assert(offsetof(BaryWork, t1) == 0x50 && offsetof(BaryWork, t2) == 0xa0 &&
              offsetof(BaryWork, t3) == 0xf0 && offsetof(BaryWork, third) == 0x140);
static_assert(offsetof(ProjTriState, c) == 0x70 && offsetof(ProjTriState, x) == 0xb0 &&
              offsetof(ProjTriState, tol) == 0x100 && offsetof(ProjTriState, edge) == 0x130);
static_assert(offsetof(BaryPtState, i) == 0x20);
static_assert(offsetof(TetVolState, h) == 0x30);
static_assert(offsetof(FaceProjState, i) == 0x78 && offsetof(FaceProjState, c) == 0x88 &&
              offsetof(FaceProjState, j) == 0xa8 && offsetof(FaceProjState, x) == 0xb8 &&
              offsetof(FaceProjState, p) == 0x118 && offsetof(FaceProjState, nneg) == 0x138 &&
              offsetof(FaceProjState, edge) == 0x168 && offsetof(FaceProjState, q) == 0x178 &&
              offsetof(FaceProjState, t_best) == 0x1a8);
static_assert(offsetof(OrientState, t) == 0x58);

// Resets the SAVE state above to its load-time values (tests).
void reset_globals_geoutl_tri();

// ---- routines -------------------------------------------------------------------------------

// 1404e29fa: area of the triangle X(3,3) by Heron's formula (0 when the product is negative).
void fun_1404e29fa(const double* x, double* area);

// 1404e2d44: planar (x, y) barycentric coordinates l1, l2, l3 of p(3) in X(3,3) (ratios of Heron
// areas, all >= 0); iflag = 1 when the triangle area < 1e-16 or l1 + l2 + l3 >= 1.000001f (or
// NaN), i.e. p is outside, else 0.
void fun_1404e2d44(const double* x, const double* p, double* l1, double* l2, double* l3,
                   std::int32_t* iflag);

// 1404e34eb: out(i) = X(i,1)*a + X(i,2)*b + X(i,3)*((1 - a) - b), i = 1..n, X(n,3).
void fun_1404e34eb(const double* x, double* out, const double* a, const double* b,
                   const std::int32_t* n);

// 1404e2c31: area of the triangle (p1, p2, p3) (Heron, via 1404e29fa).
void fun_1404e2c31(const double* p1, const double* p2, const double* p3, double* area);

// 1404ca2cb: signed volume of the tetrahedron X(3,4): (n . (x4 - x1)) * area(x1,x2,x3) / 3.
void fun_1404ca2cb(const double* x, double* vol);

// 1404d37d7: 2D orientation of the polygon X(ldim, n) (rows 1, 2): nneg = number of right turns
// (cross < 0), tested at vertex 1 only when n == 3, at vertices 1..4 otherwise; mode = 1 reverses
// the vertex order when a right turn or zero edge was found, mode = -1 when none was, 0 never.
void fun_1404d37d7(double* x, std::int32_t* nneg, const std::int32_t* ldim, const std::int32_t* n,
                   const std::int32_t* mode);

// 1404d3094: project p(3) onto the triangle X(3,3): q = foot of the normal when it falls inside
// (iflag 0), else the nearest point of the nearest edge (iflag -i on edge i, or the vertex index
// when the edge parameter is >= 0.9999f / <= 1e-4f); dist = distance, negative below the face;
// iflag = 10000 for a degenerate triangle (q, dist untouched).
void fun_1404d3094(const double* x, const double* p, double* q, double* dist, std::int32_t* iflag);

// 1404e2914: 1404d3094, then the barycentric coordinates (xi, eta) of q by 1404e305a; dist = 1e8
// when the triangle is degenerate.
void fun_1404e2914(const double* x, const double* p, double* xi, double* eta, double* q,
                   double* dist, std::int32_t* iflag);

// 1404e305a: barycentric coordinates (l1, l2, l3) of p in the triangle X(3,3), both mapped into
// the triangle's frame; iflag = 1 when p lies farther than the tolerance outside (l1..l3 not
// set) or 1404e2d44 says outside; else the point at (l1, l2) is left in g_proj_tri.nearest.
void fun_1404e305a(const double* x, const double* p, double* l1, double* l2, double* l3,
                   std::int32_t* iflag);

} // namespace def::rt::geoutl
