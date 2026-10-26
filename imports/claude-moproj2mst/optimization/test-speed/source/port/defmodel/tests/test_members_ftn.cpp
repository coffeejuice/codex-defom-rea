// ctest-labels: p1-10 members
// Tier 1 for the Fortran routines 140325691 and 140345f5b. Their callees belong to other packets
// (members/ext_deps.hpp); this test defines simple fakes for exactly those callees, so the
// library's stub object is not linked. Expected values are hand calculations with the fakes.
#include <defmodel/globals_p1_10.hpp>
#include <defmodel/members/ext_deps.hpp>
#include <defmodel/members/ftn_geom.hpp>
#include <testkit/check.hpp>

using namespace def::model;

namespace {
double g_angle = -1.0;
std::int32_t g_modes[2] = {0, 0};
int g_calls = 0;
} // namespace

namespace def::model::ext {

void fun_1404b3b30(double a, double* s, double* c)
{
    g_angle = a;
    *s = 0.6;
    *c = 0.8;
}
void fun_1404cf5c0(const double*, double* rot)
{
    for (int i = 0; i < 9; ++i)
        rot[i] = (i % 4 == 0) ? 1.0 : 0.0;
}
// mode 1: out = pts - origin; mode 2: out = pts + origin (identity rotation)
void fun_1404ded0c(double* pts, double* out, const double* origin, const double*,
                   const std::int32_t* n, const std::int32_t* mode)
{
    CHECK_EQ(*n, 1);
    g_modes[g_calls++ & 1] = *mode;
    for (int k = 0; k < 3; ++k)
        out[k] = (*mode < 2) ? pts[k] - origin[k] : pts[k] + origin[k];
}
// column min/max like 140437b3b
void fun_140437aa9(const double* a, const std::int32_t* ld, const std::int32_t* n,
                   const std::int32_t* comp, double* mn, double* mx, const std::int32_t* init)
{
    if (*init == 1) {
        *mn = 1e30;
        *mx = -1e30;
    }
    for (int i = 0; i < *n; ++i) {
        double v = a[(*comp - 1) + i * *ld];
        if (v < *mn)
            *mn = v;
        if (v > *mx)
            *mx = v;
    }
}

} // namespace def::model::ext

TEST_CASE(rotate_point_about_axis)
{
    reset_globals_p1_10();
    double p[3] = {3.0, 4.0, 5.0};
    const double origin[3] = {1.0, 1.0, 1.0};
    const double dir[3] = {0.0, 0.0, 1.0};
    const double deg = 90.0;
    fun_140325691(p, origin, dir, &deg);
    CHECK_BITS(g_angle, (90.0 / 45.0) * 0.7853981633974483);
    CHECK_EQ(g_modes[0], 1);
    CHECK_EQ(g_modes[1], 2);
    // w = p - origin = (2, 3, 4); x = w0*c - s*w1, y = s*w0 + w1*c; then + origin
    const double x = 2.0 * 0.8 - 0.6 * 3.0;
    const double y = 0.6 * 2.0 + 3.0 * 0.8;
    CHECK_BITS(p[0], x + 1.0);
    CHECK_BITS(p[1], y + 1.0);
    CHECK_BITS(p[2], 5.0);
    CHECK_BITS(g_work_1407017f0[0], p[0]);
    CHECK_BITS(dat_1407017e0, 0.6);
    CHECK_BITS(dat_1407017e8, 0.8);
}

TEST_CASE(largest_xy_extent)
{
    reset_globals_p1_10();
    dat_140b74428 = 11.0;
    dat_140b74430 = 12.0;
    const double xyz[9] = {0, 0, 0, 5, 1, 0, 2, 7, 0};
    const std::int32_t n = 3;
    fun_140345f5b(xyz, &n);
    CHECK_EQ(dat_140702710, 0.0);
    CHECK_EQ(dat_140702718, 5.0);
    CHECK_EQ(dat_140702720, 0.0);
    CHECK_EQ(dat_140702728, 7.0);
    CHECK_EQ(dat_140b84730, 7.0);
    CHECK_EQ(dat_140b84720, 11.0);
    CHECK_EQ(dat_140b84728, 12.0);
    const double wide[6] = {0, 0, 0, 9, 1, 0};
    const std::int32_t two = 2;
    fun_140345f5b(wide, &two);
    CHECK_EQ(dat_140b84730, 9.0); // x range wins
}

int main() { return testkit::run_all(); }
