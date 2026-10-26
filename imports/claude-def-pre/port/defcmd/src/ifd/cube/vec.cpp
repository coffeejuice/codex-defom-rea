// vec.cpp: the small 3-vector helpers the cube code calls (C++ value class in the binary, every
// result returned through a hidden pointer and copied with rep movsb), the polygon normal and the
// box overlap test. Operation order from the listing.
#include <defcmd/ifd/cube_lib.hpp>

#include <defcmd/vec3.hpp>

#include <cmath>

namespace def::cmd::cube {

// @src 140239700  Vec3(x, y, z)
Vec3 fun_140239700(double x, double y, double z) { return Vec3{x, y, z}; }

// @src 140239fb0  a - b
Vec3 fun_140239fb0(const double* a, const double* b)
{
    return Vec3{a[0] - b[0], a[1] - b[1], a[2] - b[2]};
}

// @src 14023a4b0  a x b = (a1 b2 - a2 b1, a2 b0 - a0 b2, a0 b1 - a1 b0)
Vec3 fun_14023a4b0(const Vec3& a, const Vec3& b)
{
    return Vec3{a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x};
}

// @src 14023a1b0  a + b
Vec3 fun_14023a1b0(const Vec3& a, const Vec3& b) { return Vec3{a.x + b.x, a.y + b.y, a.z + b.z}; }

// 1402399b0 / 140239a30 / 140239b40 are p67-1's (defcmd/vec3.hpp, def::cmd::v3::len2/len/unit);
// these forward to them with this file's Vec3. (The cube worker read 140239b40's listing as
// "comisd fabs, eps; jbe -> zero", i.e. a NaN length gives the zero vector; reported to p67-1.)
double fun_1402399b0(const Vec3& a) { return v3::len2(v3::V3{a.x, a.y, a.z}); }
double fun_140239a30(const Vec3& a) { return v3::len(v3::V3{a.x, a.y, a.z}); }
Vec3 fun_140239b40(const Vec3& a)
{
    const v3::V3 u = v3::unit(v3::V3{a.x, a.y, a.z});
    Vec3 r = a;
    r.x = u.x;
    r.y = u.y;
    r.z = u.z;
    return r;
}

// @src 140240d80  unit normal of polygon v(3, np): sum over k = 2..np-1 of
// (v_{k-1} - v_0) x (v_k - v_0), normalised (140239b40)
Vec3 fun_140240d80(const double* v, std::int32_t np)
{
    Vec3 sum = fun_140239700(0.0, 0.0, 0.0);
    Vec3 e1 = fun_140239fb0(v + 3, v);
    for (std::int32_t k = 2; k <= np - 1; ++k) {
        const Vec3 e2 = fun_140239fb0(v + k * 3, v);
        const Vec3 c = fun_14023a4b0(e1, e2);
        sum = fun_14023a1b0(sum, c);
        e1 = fun_140239700(e2.x, e2.y, e2.z);
    }
    return fun_140239b40(sum);
}

// @src 14023aa90  1 iff fabs(a_i - b_i) < eps for i = x, y, z (strict, NaN fails)
std::int32_t fun_14023aa90(const Vec3& a, const Vec3& b, double eps)
{
    if (std::fabs(a.x - b.x) < eps && std::fabs(a.y - b.y) < eps && std::fabs(a.z - b.z) < eps) {
        return 1;
    }
    return 0;
}

// @src 140240b40  box overlap: a = (min x, y, z, max x, y, z), b the same. Per axis: a.min in
// [b.min, b.max], or a.max in it, or b.min in [a.min, a.max], or b.max in it (inclusive,
// comisd + jc/jnc: a NaN coordinate fails each test); all three axes required
std::int32_t fun_140240b40(const double* a, const double* b)
{
    for (int i = 0; i < 3; ++i) {
        const double amin = a[i];
        const double amax = a[i + 3];
        const double bmin = b[i];
        const double bmax = b[i + 3];
        const bool ok = (amin >= bmin && bmax >= amin) || (amax >= bmin && bmax >= amax) ||
                        (bmin >= amin && amax >= bmin) || (bmax >= amin && amax >= bmax);
        if (!ok) {
            return 0;
        }
    }
    return 1;
}

} // namespace def::cmd::cube
