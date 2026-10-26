// defmodel/linked/matrix4.hpp: the 4x4 homogeneous transform (0x80 bytes, 16 doubles, row-major:
// element (i, j) at byte i * 0x20 + j * 8) used by the geometry classes to rotate and move
// objects. Points are row vectors (translation in row 3), so A.multiply_by(B) applies A first.
//
//   ctor 140220540 (identity 140220570), dtor 140220560 (empty), assign 140220670,
//   multiply_by 140221a90, translation 140221c20, rotation about an axis through the origin
//   140221cd0, about an axis through a point 1402214c0 (returns by value), copy_doubles 140222620.
//   Callers: 1402fb550 (_OBJ_GEO3D rotation, angle given in degrees and converted), 1402002b0,
//   1402f2110, 140281ac0, 1402baae0, 140432130.., 14027f460 (transform a point).
#pragma once

#include <cstddef>
#include <cstdint>

namespace def::model::linked {

class Matrix4 {
public:
    // @src 140220540  ctor: identity
    Matrix4();
    // @src 140220560  dtor (empty)
    ~Matrix4();
    Matrix4(const Matrix4& other) = default; // 1402214c0 copies the 0x80 bytes (rep movsb)

    // @src 140220570  set to identity
    void set_identity();
    // @src 140220670  element-wise copy; returns this
    Matrix4& assign(const Matrix4& other);
    // @src 140221a90  this = this * other (sum over k from 0.0, k ascending); returns this
    Matrix4& multiply_by(const Matrix4& other);
    // @src 140221c20  this = translation by (*x, *y, *z)
    void set_translation(const double* x, const double* y, const double* z);
    // @src 140221cd0  this = rotation by *angle (radians) about axis; axis is normalised in
    //                 place, and nothing but the identity is set when |axis| < 1e-7
    void set_rotation(double* axis, const double* angle);

    double m[16];
};

static_assert(sizeof(Matrix4) == 0x80);

// @src 1402214c0  rotation by *angle (radians) about the axis through point: T(-p) R T(p)
Matrix4 rotation_about_axis(const double* point, const double* axis, const double* angle);

// @src 140222620  memcpy of n doubles
void copy_doubles(double* dst, const double* src, std::int32_t n);

} // namespace def::model::linked
