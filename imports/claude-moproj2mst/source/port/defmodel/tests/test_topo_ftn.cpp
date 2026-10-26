// ctest-labels: p1-11 topo
// Tier-1 tests of the Fortran geometry routines 14041f590, 1404cf5c0, 1404d71d7, 1404ded0c.
// Expected values are computed by hand from the operation order in the assembly; inputs are
// chosen so that every intermediate is exact, and the signed-zero cases check the order of the
// accumulation (0.0 + -0.0 = +0.0) and the sign flips.
#include <defmodel/topo/ftn_geom.hpp>
#include <testkit/check.hpp>

#include <bit>

#include <cstdint>

using namespace def::model::topo;

TEST_CASE(transform_forward_and_back)
{
    reset_globals_p1_11();
    // R(1,2) = R(2,3) = R(3,1) = 1, column-major r[(j-1) + 3(k-1)]
    double r[9] = {0, 0, 1, 1, 0, 0, 0, 1, 0};
    const double a[3] = {1, 2, 3};
    const double c[3] = {1, 1, 1};
    double b[3] = {9, 9, 9};
    std::int32_t n = 1;
    std::int32_t mode = 1;
    // b(j) = sum_k (a(k) - c(k)) R(j,k): d = (0, 1, 2) -> (d2, d3, d1) = (1, 2, 0)
    transform_points(a, b, c, r, &n, &mode);
    CHECK_BITS(b[0], 1.0);
    CHECK_BITS(b[1], 2.0);
    CHECK_BITS(b[2], 0.0);
    CHECK_EQ(g_xform.i, 2);
    CHECK_EQ(g_xform.j, 4);
    CHECK_EQ(g_xform.k, 4);

    // mode 2: b(j) = sum_k a(k) R(k,j) + c(j) = (a3, a1, a2) + 1 = (4, 2, 3)
    mode = 2;
    transform_points(a, b, c, r, &n, &mode);
    CHECK_BITS(b[0], 4.0);
    CHECK_BITS(b[1], 2.0);
    CHECK_BITS(b[2], 3.0);
}

TEST_CASE(transform_sum_starts_from_plus_zero)
{
    // every term is (0) * (-1) = -0.0; the running sum starts at +0.0, so b = +0.0, not -0.0
    double r[9];
    for (double& x : r) x = -1.0;
    const double a[6] = {1, 1, 1, 2, 2, 2};
    const double c[3] = {1, 1, 1};
    double b[6];
    std::int32_t n = 1;
    std::int32_t mode = 0;
    transform_points(a, b, c, r, &n, &mode);
    for (int j = 0; j < 3; ++j) CHECK_BITS(b[j], 0.0);
    n = 2; // second point: (1,1,1) * -1 summed: -1 + -1 + -1 = -3
    transform_points(a, b, c, r, &n, &mode);
    for (int j = 3; j < 6; ++j) CHECK_BITS(b[j], -3.0);
}

TEST_CASE(direction_matrix_cases)
{
    reset_globals_p1_11();
    double m[9];
    for (double& x : m) x = 7.0;
    const double zero[3] = {0.0, 0.0, 0.0};
    direction_matrix(zero, m); // zero vector: flag 1, m untouched
    CHECK_EQ(g_dir_matrix.zero_vector, 1);
    CHECK_BITS(m[0], 7.0);

    const double up[3] = {0.0, 5.0, 0.0}; // rxz = 0 < 1e-9: c = 0, s = 1
    direction_matrix(up, m);
    CHECK_EQ(g_dir_matrix.zero_vector, 0);
    const double want_up[9] = {1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, -1.0, 0.0};
    for (int i = 0; i < 9; ++i) CHECK_BITS(m[i], want_up[i]);

    const double v[3] = {3.0, 0.0, 4.0}; // rxz = r = 5: c = 1, s = 0, ux = 3/5, uz = 4/5
    direction_matrix(v, m);
    const double ux = 3.0 / 5.0;
    const double uz = 4.0 / 5.0;
    const double want[9] = {uz, -0.0, ux, 0.0, 1.0, 0.0, -ux, -0.0, uz};
    for (int i = 0; i < 9; ++i) CHECK_BITS(m[i], want[i]);
    CHECK_BITS(g_dir_matrix.rxz, 5.0);
    CHECK_BITS(g_dir_matrix.ux, ux);
}

TEST_CASE(rotate_2d_by_zero)
{
    // The binary's sincos (1404b3b30, def::rt::libm) gives sin 0 = 0 and cos 0 = 1 - 2^-53
    // (0x3fefffffffffffff, the value the original routine returns): p1 = 0*x + c*y,
    // p0 = c*x - 0*y, each product rounded once.
    double p[2] = {2.0, -3.0};
    const double a = 0.0;
    rotate_2d(p, &a);
    const double c = std::bit_cast<double>(0x3fefffffffffffffull);
    CHECK_BITS(p[0], std::bit_cast<double>(0x3fffffffffffffffull)); // 2c = 1.9999999999999998
    CHECK_BITS(p[1], std::bit_cast<double>(0xc007ffffffffffffull)); // -3c = -2.9999999999999996
    CHECK_BITS(g_rot2d.cos_a, c);
    CHECK_BITS(g_rot2d.x, 2.0);
}

TEST_CASE(reverse_points_two_columns)
{
    double a[6] = {1, 2, 3, 4, 5, 6};
    std::int32_t nc = 2;
    std::int32_t np = 3;
    reverse_point_rows(&nc, &np, a);
    const double want[6] = {5, 6, 3, 4, 1, 2};
    for (int i = 0; i < 6; ++i) CHECK_BITS(a[i], want[i]);
    CHECK_EQ(g_rev_points.i, 2); // one swap (3 / 2 = 1), then i = 2
    CHECK_EQ(g_rev_points.j, 3);

    double b[8] = {1, 2, 3, 4, 5, 6, 7, 8};
    np = 4;
    reverse_point_rows(&nc, &np, b);
    const double want_b[8] = {7, 8, 5, 6, 3, 4, 1, 2};
    for (int i = 0; i < 8; ++i) CHECK_BITS(b[i], want_b[i]);
}

TEST_CASE(reverse_points_six_column_records)
{
    // record 1: line (type 1) from (1,2) to (3,4), angle 45 -> angle 0 (14045a140), ends swapped
    // record 2: arc (type 2) start (5,6), centre (7,8), angle 0 deg -> 0 rad; start rotated by 0
    //           about the centre stays (5,6) (dy*1 + (dx*0 + cy) = -2 + 8), angle -> -0.0
    // then the two records swap places.
    double a[12] = {1, 1, 2, 3, 4, 45, 2, 5, 6, 7, 8, 0};
    std::int32_t nc = 6;
    std::int32_t np = 2;
    reverse_point_rows(&nc, &np, a);
    const double want[12] = {2, 5, 6, 7, 8, -0.0, 1, 3, 4, 1, 2, 0.0};
    for (int i = 0; i < 12; ++i) CHECK_BITS(a[i], want[i]);
    CHECK_EQ(g_rev_points.type, 2);
}

int main() { return testkit::run_all(); }
