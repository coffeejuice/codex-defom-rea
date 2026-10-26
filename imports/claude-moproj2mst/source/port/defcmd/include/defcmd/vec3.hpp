// defcmd/vec3.hpp: the small 3D and 2D vector helpers of the object-positioning code (OBJPOS and
// the object transforms it calls). Spec: docs/re/commands/OBJPOS.md section 10 (helper table).
//
// The C++ helpers are compiled without optimisation, so the decompiled expression order is the
// executed order: a 3-term sum is ((t1 + t2) + t3). The returned-by-value helpers of the binary
// (hidden return pointer) return a struct here; callers copy exactly as the binary does.
//
// fabs is FUN_140484558 and sqrt FUN_140484644 (the CRT's sqrtsd); both are exact operations, so
// std::fabs / std::sqrt give the same bits.
#pragma once

#include <cmath>

namespace def::cmd::v3 {

struct V3 {
    double x, y, z;
};
struct V2 {
    double x, y;
};

// @src 14023ace0  1 when |a[i] - b[i]| < eps for i = 0..2 (stops at the first failure)
inline bool near(const double* a, const double* b, double eps)
{
    return std::fabs(a[0] - b[0]) < eps && std::fabs(a[1] - b[1]) < eps && std::fabs(a[2] - b[2]) < eps;
}
// @src 140308d60  2D form of near
inline bool near2(const double* a, const double* b, double eps)
{
    return std::fabs(a[0] - b[0]) < eps && std::fabs(a[1] - b[1]) < eps;
}
// @src 14023b050  b = a
inline void copy(const double* a, double* b)
{
    b[0] = a[0];
    b[1] = a[1];
    b[2] = a[2];
}
// @src 14023b110  c = a + b
inline void add(const double* a, const double* b, double* c)
{
    c[0] = a[0] + b[0];
    c[1] = a[1] + b[1];
    c[2] = a[2] + b[2];
}
// @src 14023b1f0  c = a - b (also def::rt::ftn::vec3_sub)
inline void sub(const double* a, const double* b, double* c)
{
    c[0] = a[0] - b[0];
    c[1] = a[1] - b[1];
    c[2] = a[2] - b[2];
}
// @src 14023b2d0  c = a * s
inline void scale(const double* a, double s, double* c)
{
    c[0] = a[0] * s;
    c[1] = a[1] * s;
    c[2] = a[2] * s;
}
// @src 14023b3a0  a . b
inline double dot(const double* a, const double* b) { return a[0] * b[0] + a[1] * b[1] + a[2] * b[2]; }
// 14023b850  squared distance: ported in defdbio/gendb/pre_steps.hpp (vec3_dist2)
double dist2(const double* a, const double* b);
// @src 14023b8f0  distance: sqrt of the same sum (the binary returns the sqrt in xmm0)
inline double dist(const double* a, const double* b)
{
    return std::sqrt((a[0] - b[0]) * (a[0] - b[0]) + (a[1] - b[1]) * (a[1] - b[1]) +
                     (a[2] - b[2]) * (a[2] - b[2]));
}
// 14023b720 normalise in place: n = sqrt((x*x + y*y) + z*z); unchanged when n == 0; returns n
// (def::rt::ftn::vec3_normalize owns the address)
double normalize(double* v);
// @src 14023bd90  projection of p on the line a-b: t = ((p-a).d) / (d.d), q = a + d*t, d2 = |p-q|^2
// (returns 0; t, d2 and q are outputs)
int project(const double* p, const double* a, const double* b, double* t, double* d2, double* q);

// Value-returning helpers of the Vector3 class (hidden return pointer in the binary)
// 140239800  copy-construct: ported in defdbio/objcheck/objcheck.hpp (vec3_copy)
V3 make(const double* a);
// @src 1402399b0  squared length
inline double len2(const V3& a) { return a.x * a.x + a.y * a.y + a.z * a.z; }
// @src 140239a30  length = sqrt(len2)
inline double len(const V3& a) { return std::sqrt(len2(a)); }
// 140239b40  unit vector, zero when !(fabs(len) > 1e-12) (DAT_14051f260): ported in
// defdbio/gendb/obj_counts.hpp (vec3_normalized)
V3 unit(const V3& a);
// 140239ea0  s * a: ported in defdbio/objcheck/objcheck.hpp (vec3_scale)
V3 mul(const V3& a, double s);

// 2D (Vector2 class)
// @src 140308730  copy-construct
inline V2 make2(const double* a) { return V2{a[0], a[1]}; }
// @src 1403087a0  squared length
inline double len2(const V2& a) { return a.x * a.x + a.y * a.y; }
// @src 1403087e0  length
inline double len(const V2& a) { return std::sqrt(len2(a)); }
// @src 140308810  unit vector (1e-12 threshold as 140239b40)
V2 unit(const V2& a);
// @src 140308940  s * a
inline V2 mul(const V2& a, double s) { return V2{s * a.x, s * a.y}; }
// @src 1403089b0  a - b
inline V2 minus(const V2& a, const V2& b) { return V2{a.x - b.x, a.y - b.y}; }
// @src 140308a20  a + b
inline V2 plus(const V2& a, const V2& b) { return V2{a.x + b.x, a.y + b.y}; }
// @src 140308a90  a . b
inline double dot(const V2& a, const V2& b) { return a.x * b.x + a.y * b.y; }
// @src 140308fb0  c = a - b (2 components, by pointer)
inline void sub2(const double* a, const double* b, double* c)
{
    c[0] = a[0] - b[0];
    c[1] = a[1] - b[1];
}
// @src 1403090e0  a . b (2 components, by pointer)
inline double dot2(const double* a, const double* b) { return a[0] * b[0] + a[1] * b[1]; }
// @src 1403091c0  normalise in place (2D): unchanged when the length is 0; returns the length
double normalize2(double* v);

} // namespace def::cmd::v3
