// 4x4 transform (defmodel/linked/matrix4.hpp). Operation order follows the assembly of
// 140221cd0 (checked) and the decompile of 140221a90; one C++ expression per original one.
#include <defmodel/linked/matrix4.hpp>
#include <defrt/stdlib/libm.hpp>

#include <cmath>
#include <cstring>
#include <defrt/rdata.hpp>

namespace def::model::linked {

namespace {

// rdata 140509c90  1.0
constexpr double kOne = def::rt::rdata::d_140509c90;
// @data 140530cf8  1e-7: minimum axis length for a rotation
constexpr double kAxisEps = 1e-7;

} // namespace

// @src 140220540
Matrix4::Matrix4() { set_identity(); }

// @src 140220560
Matrix4::~Matrix4() = default;

// @src 140220570
void Matrix4::set_identity()
{
    m[0] = kOne;
    m[1] = 0.0;
    m[2] = 0.0;
    m[3] = 0.0;
    m[4] = 0.0;
    m[5] = kOne;
    m[6] = 0.0;
    m[7] = 0.0;
    m[8] = 0.0;
    m[9] = 0.0;
    m[10] = kOne;
    m[11] = 0.0;
    m[12] = 0.0;
    m[13] = 0.0;
    m[14] = 0.0;
    m[15] = kOne;
}

// @src 140220670
Matrix4& Matrix4::assign(const Matrix4& other)
{
    for (std::int32_t i = 0; i < 4; i = i + 1) {
        for (std::int32_t j = 0; j < 4; j = j + 1) {
            m[i * 4 + j] = other.m[i * 4 + j];
        }
    }
    return *this;
}

// @src 140221a90
Matrix4& Matrix4::multiply_by(const Matrix4& other)
{
    Matrix4 t; // 140220540: identity, then every element overwritten
    for (std::int32_t i = 0; i < 4; i = i + 1) {
        for (std::int32_t j = 0; j < 4; j = j + 1) {
            double s = 0.0;
            for (std::int32_t k = 0; k < 4; k = k + 1) {
                s = s + m[i * 4 + k] * other.m[k * 4 + j];
            }
            t.m[i * 4 + j] = s;
        }
    }
    assign(t);
    return *this;
}

// @src 140221c20
void Matrix4::set_translation(const double* x, const double* y, const double* z)
{
    set_identity();
    Matrix4 t;
    t.m[12] = *x;
    t.m[13] = *y;
    t.m[14] = *z;
    multiply_by(t);
}

// @src 140221cd0
// sqrt is the CRT's (correctly rounded, as std::sqrt). cos/sin are the VS2010 x64 CRT's SSE2
// routines 140bb3490 / 140bb3000, ported bit for bit as def::rt::libm::crt_cos / crt_sin.
void Matrix4::set_rotation(double* axis, const double* angle)
{
    set_identity();
    const double len = std::sqrt(axis[0] * axis[0] + axis[1] * axis[1] + axis[2] * axis[2]);
    if (kAxisEps > len) { // 140221d4f comisd / jbe: a NaN length goes on
        return;
    }
    axis[0] = axis[0] / len;
    axis[1] = axis[1] / len;
    axis[2] = axis[2] / len;
    const double c = def::rt::libm::crt_cos(*angle); // 140bb3490 (MSVC CRT cos)
    const double s = def::rt::libm::crt_sin(*angle); // 140bb3000 (MSVC CRT sin)
    m[0] = c + (kOne - c) * axis[0] * axis[0];
    m[4] = (kOne - c) * axis[0] * axis[1] - axis[2] * s;
    m[8] = (kOne - c) * axis[0] * axis[2] + axis[1] * s;
    m[1] = (kOne - c) * axis[0] * axis[1] + axis[2] * s;
    m[5] = c + (kOne - c) * axis[1] * axis[1];
    m[9] = (kOne - c) * axis[1] * axis[2] - axis[0] * s;
    m[2] = (kOne - c) * axis[0] * axis[2] - axis[1] * s;
    m[6] = (kOne - c) * axis[1] * axis[2] + axis[0] * s;
    m[10] = c + (kOne - c) * axis[2] * axis[2];
}

// @src 1402214c0
Matrix4 rotation_about_axis(const double* point, const double* axis, const double* angle)
{
    double a[3];
    copy_doubles(a, axis, 3); // 140222620: the rotation normalises its own copy
    Matrix4 to_origin;
    Matrix4 rot;
    Matrix4 back;
    Matrix4 r;
    const double nx = -point[0]; // xor with the sign mask 14051bec0
    const double ny = -point[1];
    const double nz = -point[2];
    to_origin.set_translation(&nx, &ny, &nz);
    rot.set_rotation(a, angle);
    back.set_translation(&point[0], &point[1], &point[2]);
    to_origin.multiply_by(rot).multiply_by(back);
    r.assign(to_origin);
    return r;
}

// @src 140222620
void copy_doubles(double* dst, const double* src, std::int32_t n)
{
    std::memcpy(dst, src, static_cast<std::size_t>(static_cast<std::int64_t>(n) << 3));
}

} // namespace def::model::linked
