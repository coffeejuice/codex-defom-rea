// Tier-1 tests of the 4x4 transform (140220540 .. 140222620).
// ctest-labels: p1-06 linked
//
// Hand-derived expectations: the identity layout of 140220570; translation in row 3 (140221c20
// multiplies the identity by a matrix whose m[12..14] hold the offsets); the rotation formula of
// 140221cd0 (m[1] = (1-c)xy + zs, m[4] = (1-c)xy - zs); cos of the double nearest pi/2 is
// 6.123233995736766e-17 (correctly rounded), sin is 1.0.
#include <defmodel/linked/matrix4.hpp>
#include <testkit/check.hpp>

#include <cmath>

using def::model::linked::Matrix4;

TEST_CASE(identity)
{
    Matrix4 m;
    for (int i = 0; i < 16; ++i) {
        CHECK_BITS(m.m[i], (i % 5 == 0) ? 1.0 : 0.0);
    }
}

TEST_CASE(translation_row_three)
{
    Matrix4 m;
    const double x = 1.5, y = -2.0, z = 0.25;
    m.set_translation(&x, &y, &z);
    CHECK_BITS(m.m[12], 1.5);
    CHECK_BITS(m.m[13], -2.0);
    CHECK_BITS(m.m[14], 0.25);
    CHECK_BITS(m.m[15], 1.0);
    CHECK_BITS(m.m[3], 0.0);
}

TEST_CASE(multiply_composes_translations)
{
    Matrix4 a;
    Matrix4 b;
    const double one = 1.0, two = 2.0, zero = 0.0;
    a.set_translation(&one, &zero, &zero);
    b.set_translation(&zero, &two, &zero);
    Matrix4& r = a.multiply_by(b);
    CHECK(&r == &a);
    CHECK_BITS(a.m[12], 1.0);
    CHECK_BITS(a.m[13], 2.0);
    CHECK_BITS(a.m[0], 1.0);
}

TEST_CASE(multiply_sum_starts_at_positive_zero)
{
    // m[4] of identity * diag(-1, 1, 1, 1) is 0*(-1) + 1*0 + 0*0 + 0*0 summed from 0.0:
    // the first product is -0.0 and 0.0 + -0.0 is +0.0
    Matrix4 a;
    Matrix4 b;
    b.m[0] = -1.0;
    a.multiply_by(b);
    CHECK_BITS(a.m[0], -1.0);
    CHECK_BITS(a.m[4], 0.0);
}

TEST_CASE(rotation_quarter_turn_about_z)
{
    Matrix4 m;
    double axis[3] = {0.0, 0.0, 2.0};
    const double a = 1.5707963267948966; // nearest double to pi/2
    m.set_rotation(axis, &a);
    CHECK_BITS(axis[2], 1.0); // normalised in place
    CHECK_BITS(m.m[0], 6.123233995736766e-17);
    CHECK_BITS(m.m[1], 1.0);
    CHECK_BITS(m.m[4], -1.0);
    CHECK_BITS(m.m[10], 1.0);
    CHECK_BITS(m.m[15], 1.0);
}

TEST_CASE(rotation_short_axis_is_identity)
{
    Matrix4 m;
    double axis[3] = {1e-8, 0.0, 0.0};
    const double a = 1.0;
    m.set_rotation(axis, &a);
    CHECK_BITS(axis[0], 1e-8); // not normalised
    CHECK_BITS(m.m[0], 1.0);
    CHECK_BITS(m.m[5], 1.0);
}

TEST_CASE(rotation_about_point)
{
    // quarter turn about the z axis through (1, 0, 0): row 3 = (-p) R + p = (1 - c, -1 + 0, 0)
    const double p[3] = {1.0, 0.0, 0.0};
    const double ax[3] = {0.0, 0.0, 1.0};
    const double a = 1.5707963267948966;
    Matrix4 m = def::model::linked::rotation_about_axis(p, ax, &a);
    // m[12] = ((0 + -c) + -0) + 0) + 1 with c = 6.12e-17: below 1 the spacing is 2^-53 = 1.11e-16,
    // and c is more than half of it, so 1 - c rounds down to 1 - 2^-53
    CHECK_BITS(m.m[12], 1.0 - 0x1p-53);
    CHECK_BITS(m.m[13], -1.0);
    CHECK(std::fabs(m.m[14]) == 0.0);
    CHECK_BITS(ax[2], 1.0); // the caller's axis is copied, not normalised
}

TEST_CASE(copy_doubles_count)
{
    double src[4] = {1.0, 2.0, 3.0, 4.0};
    double dst[4] = {0.0, 0.0, 0.0, 9.0};
    def::model::linked::copy_doubles(dst, src, 3);
    CHECK_BITS(dst[2], 3.0);
    CHECK_BITS(dst[3], 9.0);
}

int main() { return testkit::run_all(); }
