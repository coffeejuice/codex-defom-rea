// defrt/ftn/geoutl_basic.hpp: part of the geometry kernel library, see geoutl.hpp for the API
// contract and docs/re/status/geoutl.md for the routine list. Vectors, normals, frames and
// point transforms; bodies in src/ftn/geoutl_basic.cpp.
#pragma once

#include <cstdint>

namespace def::rt::geoutl {

// ---- SAVE state ---------------------------------------------------------------------------------

// @data 14072ff68  SAVE of fun_1404cce5e: 1 / |c| of the last normalised cross product
inline double g_cross_inv{};

// @data 14072ff18  SAVE of fun_1404cccfb (quad normal)
struct QuadNormalState {
    std::int32_t i;         // 0x00 (14072ff18) loop index
    std::int32_t pad04;
    double dxi[3];          // 0x08 (14072ff20) dX/dxi
    double pad20;
    double deta[3];         // 0x28 (14072ff40) dX/deta
    double pad40;
    double len;             // 0x48 (14072ff60) |dxi x deta| (len argument of fun_1404cce5e)
};
inline QuadNormalState g_quad_normal{};

// @data 140730cf0  SAVE of fun_1404d3d43 (triangle normal): edge vectors
struct TriNormalState {
    double e1[3];           // 0x00 (140730cf0) x(:,2) - x(:,1)
    double pad18;
    double e2[3];           // 0x20 (140730d10) x(:,3) - x(:,1)
};
inline TriNormalState g_tri_normal{};
// @data 140757aa0  SAVE of fun_1404d3d43: |e1 x e2| (len argument of fun_1404cce5e)
inline double g_tri_normal_len{};

// @data 140730310  SAVE state of fun_1404cf5c0 (direction frame)
struct DirMatrixState {
    std::int32_t zero_vector; // 0x00 (140730310) 1 when the input is (0, 0, 0)
    std::int32_t pad04;
    double x, y, z;           // 0x08 (140730318, 320, 328)
    double rxz;               // 0x20 (140730330) sqrt(x*x + z*z)
    double r;                 // 0x28 (140730338) |v|
    double c;                 // 0x30 (140730340) rxz / r
    double s;                 // 0x38 (140730348) y / r
    double uz;                // 0x40 (140730350) z / rxz
    double ux;                // 0x48 (140730358) x / rxz
};
inline DirMatrixState g_dir_matrix{};

// @data 1407304d0  SAVE state of fun_1404d0939 (points into a frame)
struct RotFrameState {
    std::int32_t i;         // 0x00 (1407304d0) point index
    std::int32_t j;         // 0x04 (1407304d4) component index
    std::int32_t pad08[2];
    double d[3];            // 0x10 (1407304e0) a(:,i) - c
    double pad28;
    std::int32_t k;         // 0x30 (140730500) row index
    std::int32_t pad34[3];
    double w[3];            // 0x40 (140730510) R d
    double pad58;
    double sign;            // 0x60 (140730530) 1 if *flag > 0 else -1
};
inline RotFrameState g_rot_frame{};

// @data 140731518  SAVE state of fun_1404d71d7 (2D rotate)
struct Rot2dState {
    double sin_a;  // 0x00 (140731518)
    double cos_a;  // 0x08 (140731520)
    double x;      // 0x10 (140731528)
    double y;      // 0x18 (140731530)
};
inline Rot2dState g_rot2d{};

// @data 140755680  SAVE of fun_1404e33b7: |a - b| per component
struct SamePointState {
    double d[3];   // 140755680, 688, 690
};
inline SamePointState g_same_point{};

// @data 1407548a4  SAVE loop indices of fun_1404ded0c (transform points)
struct XformState {
    std::int32_t i; // 1407548a4
    std::int32_t j; // 1407548a8
    std::int32_t k; // 1407548ac
};
inline XformState g_xform{};

// Resets all the SAVE state above (tests).
void reset_globals_basic();

// ---- routines -----------------------------------------------------------------------------------

// 1404cce5e: c = a x b; *len = |c|; if *len > 1e-16 (a REAL*4 constant) or NaN, c is normalised
// (g_cross_inv = 1 / *len), else c is left unnormalised and *len = 0.
void fun_1404cce5e(const double* a, const double* b, double* c, double* len);

// 1404d68dd: u = q - p; *len = |u|^2; unless that is exactly 0: *len = |u| and u /= |u|
// (a zero vector leaves *len = 0 and u = q - p).
void fun_1404d68dd(const double* p, const double* q, double* u, double* len);

// 1404cf5c0: 3x3 rotation matrix m (column-major, 9 doubles) of the direction v(3); a zero v sets
// g_dir_matrix.zero_vector = 1 and leaves m untouched.
void fun_1404cf5c0(const double* v, double* m);

// 1404ded0c: mode < 2: b(:,i) = R * (a(:,i) - c); else b(:,i) = R^T * a(:,i) + c, for i = 1..n,
// with a, b (3, n), c (3), R (3, 3).
void fun_1404ded0c(const double* a, double* b, const double* c, const double* r,
                   const std::int32_t* n, const std::int32_t* mode);

// 1404d71d7: rotate the point p(2) by the angle *a about the origin.
void fun_1404d71d7(double* p, const double* a);

// 1404d0939: b(:,i) = R * (a(:,i) - c) for i = 1..*n (R (3,3) column-major, a, b (3, n)); then
// b(3,i) is negated unless *flag > 0. a and b may be the same array.
void fun_1404d0939(const double* a, const double* c, const double* r, double* b,
                   const std::int32_t* flag, const std::int32_t* n);

// 1404cccfb: unit normal n(3) of the bilinear quad x(3,4) at the local point (*xi, *eta):
// dX/dxi x dX/deta through fun_1404cce5e (a degenerate quad gives an unnormalised n).
void fun_1404cccfb(const double* x, const double* xi, const double* eta, double* n);

// 1404d3d43: unit normal n(3) of the triangle x(3,3): (x2 - x1) x (x3 - x1) via fun_1404cce5e.
void fun_1404d3d43(const double* x, double* n);

// 1404e33b7: *same = 1 when |a(k) - b(k)| <= 1e-5 for k = 1..3 (a NaN difference counts as
// equal), else 0.
void fun_1404e33b7(const double* a, const double* b, std::int32_t* same);

// 1404edb2f: bare RET (an emptied error routine; the callers pass an INTEGER code, -1).
void fun_1404edb2f(const std::int32_t* code);

} // namespace def::rt::geoutl
