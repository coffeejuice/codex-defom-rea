// ctest-labels: p67-6 contact tier1
// Tier 1 for the arc contact refinement 140341100 / 14034151e (defcmd/arc_refine.hpp).
//
// The callees 1404d93ae (sort) and 14032beef (vector angle) have no port yet; the library has
// DEF_NOT_PORTED stubs for them in their own object (arc_refine_ext.cpp). This test defines both
// itself, so that object is not linked:
//   * 1404d93ae: the bubble sort as read from its listing (columns of a(nrow, n) by row key);
//   * 14032beef: the spec formula (OBJPOS_interference_kernels.md 4.4) with the Fortran-runtime
//     atan2 (def::rt::libm::ps_atan2); not checked against its listing.
// The early paths of 14034151e use neither and are checked bit for bit; the angle-space paths
// are checked against closed forms derived from the code (tolerance, the fakes are not exact).
#include <defcmd/arc_refine.hpp>
#include <defrt/stdlib/libm.hpp>
#include <testkit/check.hpp>

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <utility>

namespace def::cmd::ftn {

void fun_1404d93ae(double* a, const std::int32_t* key, const std::int32_t* n,
                   const std::int32_t* dir, const std::int32_t* nrow)
{
    const std::int64_t nr = std::max<std::int32_t>(*nrow, 0);
    for (std::int32_t i = 1; i <= *n - 1; ++i) {
        for (std::int32_t m = *n; m >= i + 1; --m) {
            double* c0 = a + (m - 2) * nr;
            double* c1 = a + (m - 1) * nr;
            const double x0 = c0[*key - 1];
            const double x1 = c1[*key - 1];
            const bool swap = (*dir == 1) ? (x0 > x1) : (x1 > x0);
            if (swap) {
                for (std::int64_t r = 0; r < nr; ++r) {
                    std::swap(c0[r], c1[r]);
                }
            }
        }
    }
}

void fun_14032beef(const double* a, const double* b, double* out)
{
    const double la = std::sqrt(a[1] * a[1] + a[0] * a[0]);
    const double lb = std::sqrt(b[1] * b[1] + b[0] * b[0]);
    if (la > 1e-10 && lb > 1e-10) {
        *out = def::rt::libm::ps_atan2(((a[0] * b[1] - a[1] * b[0]) / la) / lb,
                                       ((b[1] * a[1] + b[0] * a[0]) / la) / lb);
    } else {
        *out = 0.0;
    }
}

} // namespace def::cmd::ftn

using namespace def::cmd::ftn;

namespace {

bool near(double a, double b, double tol) { return std::fabs(a - b) <= tol; }

struct Run {
    double e[2] = {0.0, 0.0};
    double g = 0.0;
    std::int32_t res = 99;
};

// the arc kernel's call: (S, tmp, C, &phi, zeros2, Q, dir, &tolA, &g, &f2), Q = 0, dir = (0, 1)
Run refine(double sx, double sy, double cx, double cy, double phi, double r, double g0 = 1e10)
{
    reset_arc_refine_save();
    Run out;
    out.g = g0;
    const double s[2] = {sx, sy};
    const double c[2] = {cx, cy};
    const double zeros[2] = {0.0, 0.0};
    const double q[2] = {0.0, 0.0};
    const double dir[2] = {0.0, 1.0};
    fun_140341100(s, out.e, c, &phi, zeros, q, dir, &r, &out.g, &out.res);
    return out;
}

constexpr double kHalfPi = 1.5707963267948966;
constexpr double kRefPi = 3.141592653589793;

} // namespace

// Early paths of 14034151e (centre on the ray line, cy == 0): no table, no callee.
TEST_CASE(centre_on_ray_probe_covers_arc)
{
    reset_arc_refine_save();
    const double p1[2] = {1.0, 0.0}, p2[2] = {2.0, 0.0}, cf[2] = {5.0, 0.0};
    const double rad = 0.5, r = 1.0, phi = 1.0, len = 2.0;
    double g = -7.0;
    std::int32_t st = 99;
    fun_14034151e(p1, p2, cf, &rad, &r, &phi, &g, &len, &st);  // R <= r: g = cx / len
    CHECK_EQ(st, 1);
    CHECK_BITS(g, 2.5);
    CHECK_BITS(g_save_14034151e.gnum, 5.0);
    CHECK_EQ(g_save_14034151e.mode, 1);
}

TEST_CASE(centre_on_ray_end_point_before_centre)
{
    reset_arc_refine_save();
    const double p1[2] = {3.0, 0.0}, p2[2] = {9.0, 1.0}, cf[2] = {5.0, 0.0};
    const double rad = 2.0, r = 1e-3, phi = 1.0, len = 2.0;
    double g = -7.0;
    std::int32_t st = 99;
    fun_14034151e(p1, p2, cf, &rad, &r, &phi, &g, &len, &st);  // g = (r + x0) / len
    CHECK_EQ(st, 1);
    CHECK_BITS(g, (1e-3 + 3.0) / 2.0);
}

TEST_CASE(centre_on_ray_end_point_beyond_centre)
{
    reset_arc_refine_save();
    const double p1[2] = {7.0, 0.0}, p2[2] = {9.0, 1.0}, cf[2] = {5.0, 0.0};
    const double rad = 2.0, r = 1e-3, phi = 1.0, len = 2.0;
    double g = -7.0;
    std::int32_t st = 99;
    fun_14034151e(p1, p2, cf, &rad, &r, &phi, &g, &len, &st);  // g = (x0 - r) / len
    CHECK_EQ(st, 1);
    CHECK_BITS(g, (7.0 - 1e-3) / 2.0);
}

// R == r (mode 2) and |cy / -(r + R)| = 2.5 > 1: no candidate angle, status 0, g untouched.
TEST_CASE(no_solution_returns_early)
{
    reset_arc_refine_save();
    const double p1[2] = {0.0, 1.0}, p2[2] = {1.0, 1.0}, cf[2] = {0.0, 5.0};
    const double rad = 1.0, r = 1.0, phi = 1.0, len = 1.0;
    double g = 123.0;
    std::int32_t st = 99;
    fun_14034151e(p1, p2, cf, &rad, &r, &phi, &g, &len, &st);
    CHECK_EQ(st, 0);
    CHECK_BITS(g, 123.0);
    CHECK_EQ(g_save_14034151e.mode, 2);
    CHECK_BITS(g_save_14034151e.a[24], 5.0 / -2.0);
}

// Driver, early path: C = (0, 10), S = (0, 8) on the ray below the centre, R = 2 > r, no swap
// (S par 8 < E par ~10): status 1 -> f2 = 0, g = (r + 8) / |dir|.
TEST_CASE(driver_end_point_on_ray)
{
    const Run run = refine(0.0, 8.0, 0.0, 10.0, kHalfPi, 1e-3);
    CHECK_EQ(run.res, 0);
    CHECK_BITS(run.g, 1e-3 + 8.0);
    CHECK_EQ(g_save_140341100.flag, 1);
    CHECK_BITS(g_save_140341100.rad, 2.0);
    CHECK(near(run.e[0], 2.0, 1e-12));  // E = rot((0, -2), pi/2) + C = (2, 10)
    CHECK(near(run.e[1], 10.0, 1e-12));
}

// Ray hitting an arc at Q: C = (0.6, 0.8), R = 1, Q = 0 on the circle, dir = (0, 1), r = 1e-3.
// Frame (par, perp) = (y, -x): cx = 0.8, cy = -0.6. Candidates asin(0.6/0.999) = a,
// asin(0.6/1.001) = b; marked intervals [b, a] and [pi - a, pi - b] (+-2 pi); th0 = angle of
// (-0.8, 0.6) lies in [pi - a, pi - b]. Arc S = (0.6, -0.2) (frame angle pi) sweeping -pi/2:
// the other end (pi/2) is outside that interval, the sweep is negative, so the contact angle is
// pi - a and g = cx - sqrt((R - r)^2 - cy^2) > 0: status 1 -> f2 = 0.
TEST_CASE(ray_hits_arc_interior)
{
    const Run run = refine(0.6, -0.2, 0.6, 0.8, -kHalfPi, 1e-3);
    CHECK_EQ(run.res, 0);
    CHECK_EQ(g_save_140341100.flag, 1);
    CHECK_EQ(g_save_140341100.status, 1);
    const double expect = 0.8 - std::sqrt(0.999 * 0.999 - 0.36);
    CHECK(near(run.g, expect, 1e-12));
    CHECK(near(g_save_14034151e.chosen, kRefPi - std::asin(0.6 / 0.999), 1e-12));
    CHECK_EQ(g_save_14034151e.n, 12);  // no adjacent marked intervals, nothing merged
}

// Same arc given from the other end (S = (-0.4, 0.8), sweep +pi/2): the end points are swapped
// (flag 2, sweep negated) and the result is bit-identical (R, cx, cy and the table are the same).
TEST_CASE(swapped_end_points_same_result)
{
    const Run a = refine(0.6, -0.2, 0.6, 0.8, -kHalfPi, 1e-3);
    const Run b = refine(-0.4, 0.8, 0.6, 0.8, kHalfPi, 1e-3);
    CHECK_EQ(g_save_140341100.flag, 2);
    CHECK_BITS(g_save_140341100.phi, -kHalfPi);
    CHECK_EQ(b.res, 0);
    CHECK_BITS(a.g, b.g);
}

// Arc ending just past Q: sweep -0.6439 from frame angle pi ends at pi - 0.6439, inside the
// contact interval (pi - a, pi - b) = (2.49734, 2.49884): contact at the end point, status -1
// -> f2 = 2 (S first) or 1 (swapped); g = cx + |cy / sin v| cos v with v = pi - 0.6439.
TEST_CASE(contact_at_arc_end)
{
    const double phi = -0.6439;
    const Run run = refine(0.6, -0.2, 0.6, 0.8, phi, 1e-3);
    CHECK_EQ(g_save_140341100.status, -1);
    CHECK_EQ(run.res, 2);
    const double v = kRefPi + phi;
    const double expect = 0.8 + std::fabs(-0.6 / std::sin(v)) * std::cos(v);
    CHECK(expect > 0.0);
    CHECK(near(run.g, expect, 1e-12));

    // the same arc from its other end
    const Run back = refine(run.e[0], run.e[1], 0.6, 0.8, -phi, 1e-3);
    CHECK_EQ(g_save_140341100.flag, 2);
    CHECK_EQ(back.res, 1);
    CHECK(near(back.g, expect, 1e-9));
}

// Ray tangent to the circle at Q: C = (1, 0), R = 1, dir = (0, 1); cy = -1, cx = 0. Only
// asin(1/(R + r)) = b exists (|1/(r - R)| > 1 + 1e-4); th0 = pi/2 lies in [b, pi - b] (marked).
// Arc S = (1, -1) (frame angle pi), sweep -2: other end outside, contact angle b,
// g = sqrt((R + r)^2 - 1) > 0, status 1 -> f2 = 0.
TEST_CASE(ray_tangent_to_arc)
{
    const Run run = refine(1.0, -1.0, 1.0, 0.0, -2.0, 1e-3);
    CHECK_EQ(g_save_140341100.status, 1);
    CHECK_EQ(run.res, 0);
    CHECK(near(g_save_14034151e.chosen, std::asin(1.0 / 1.001), 1e-12));
    CHECK(near(run.g, std::sqrt(1.001 * 1.001 - 1.0), 1e-9));
    CHECK_EQ(g_save_14034151e.n, 6);
}

// Q off the arc circle's contact band: the interval holding th0 is not marked -> status 0,
// f2 = -1, g untouched. C = (0, 10), R = 2 (S = (0, 8)), Q = 0 far below; sweep pi/2 from S.
// cy = 0 here would take the early path, so shift the centre: C = (1, 10), S = (1, 8).
TEST_CASE(no_contact_status_zero)
{
    const Run run = refine(1.0, 8.0, 1.0, 10.0, kHalfPi, 1e-3, 55.0);
    CHECK_EQ(g_save_140341100.status, 0);
    CHECK_EQ(run.res, -1);
    CHECK_BITS(run.g, 55.0);
}

int main() { return testkit::run_all(); }
