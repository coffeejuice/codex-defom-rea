// _OBJ_GEO2D (p1-07): fields, point-array handling, transforms, bounding box, duplicate
// removal, append, reversal, conversion and orientation. Expected values are worked out by hand
// from the decompiled code (comments give the arithmetic); other packets' routines are fakes.
// ctest-labels: p1-07 geo
#include <cmath>
#include <cstring>
#include <string>
#include <vector>

#include <defmodel/layout/obj_geo2d.hpp>
#include <testkit/check.hpp>

#include "geo_fakes.hpp"

using namespace def::model;

namespace {

OBJ_GEO2D* make(std::int32_t type, std::vector<double> rows)
{
    auto* g = new OBJ_GEO2D();
    g->curve_type = type;
    g->n_points = static_cast<std::int32_t>(rows.size()) / g->dof();
    g->allocate_points();
    std::memcpy(g->points, rows.data(), rows.size() * sizeof(double));
    return g;
}

std::vector<double> pts(const OBJ_GEO2D* g)
{
    return std::vector<double>(g->points, g->points + g->n_points * g->dof());
}

} // namespace

TEST_CASE(ctor_and_constants)
{
    geofake::reset();
    OBJ_GEO2D g;
    CHECK_EQ(g.curve_type, 0);
    CHECK_EQ(g.n_points, 0);
    CHECK(g.points == nullptr);
    CHECK(g.ext == nullptr);
    CHECK_EQ(int(g.flag28), 0);
    CHECK_EQ(g.u0c, 0);
    CHECK_EQ(std::string(g.class_name()), std::string("_OBJ_GEO2D"));
    CHECK_EQ(g.is_extension(), 0);
    CHECK_EQ(geo2d_dof(0), 0);
    CHECK_EQ(geo2d_dof(1), 3);
    CHECK_EQ(geo2d_dof(2), 6);
    CHECK_EQ(geo2d_dof(7), 0);
    CHECK(geo2d_alloc_zeroed(0) == nullptr);
    double* z = geo2d_alloc_zeroed(3);
    CHECK_BITS(z[0] + z[1] + z[2], 0.0);
    geo2d_free(z);
}

TEST_CASE(allocate_points_is_zeroed)
{
    geofake::reset();
    OBJ_GEO2D g;
    g.curve_type = 2;
    g.n_points = 2;
    g.clear_data(); // slot 3: 12 zeros
    REQUIRE(g.points != nullptr);
    for (int i = 0; i < 12; ++i) CHECK_BITS(g.points[i], 0.0);
}

TEST_CASE(scale_type1_scales_all_columns_type2_only_coordinates)
{
    geofake::reset();
    OBJ_GEO2D* a = make(1, {1, 2, 0.5, 3, 4, 1});
    double f = 2.0;
    a->scale(&f);
    CHECK(pts(a) == (std::vector<double>{2, 4, 1, 6, 8, 2}));
    OBJ_GEO2D* b = make(2, {1, 1, 2, 3, 4, 0.7});
    b->scale(&f);
    CHECK(pts(b) == (std::vector<double>{1, 2, 4, 6, 8, 0.7}));
    delete a;
    delete b;
}

TEST_CASE(translate)
{
    geofake::reset();
    OBJ_GEO2D* a = make(1, {1, 2, 0.5});
    double d[2] = {10, 20};
    a->translate(d);
    CHECK(pts(a) == (std::vector<double>{11, 22, 0.5}));
    OBJ_GEO2D* b = make(2, {1, 1, 2, 3, 4, 0.7});
    b->translate(d); // columns 1..4 take d[(j+1)%2] = d0, d1, d0, d1
    CHECK(pts(b) == (std::vector<double>{1, 11, 22, 13, 24, 0.7}));
    delete a;
    delete b;
}

TEST_CASE(bounding_box_and_char_length)
{
    geofake::reset();
    OBJ_GEO2D* a = make(1, {0, 0, 0.1, 3, 4, 0.2});
    double mn[2], mx[2], r;
    CHECK(a->bounding_box(mn, mx, &r));
    CHECK_BITS(mn[0], 0.0);
    CHECK_BITS(mn[1], 0.0);
    CHECK_BITS(mx[0], 3.0);
    CHECK_BITS(mx[1], 4.0);
    CHECK_BITS(r, 0.2);
    CHECK_BITS(a->char_length(), 5.0); // sqrt(9 + 16)
    a->points[2] = 10.0;               // radius 10: 2 * 10 > 5
    CHECK_BITS(a->char_length(), 20.0);

    // segments: arc radius column only for type 2 rows; box over both ends
    OBJ_GEO2D* b = make(2, {1, -1, 0, 2, 5, 9, 2, 0, 1, 1, 1, 0.5});
    CHECK(b->bounding_box(mn, mx, &r));
    CHECK_BITS(mn[0], -1.0);
    CHECK_BITS(mn[1], 0.0);
    CHECK_BITS(mx[0], 2.0);
    CHECK_BITS(mx[1], 5.0);
    CHECK_BITS(r, 0.5);

    OBJ_GEO2D empty;
    CHECK(!empty.bounding_box(mn, mx, &r));
    delete a;
    delete b;
}

TEST_CASE(contains_bbox_means_inside_other)
{
    geofake::reset();
    OBJ_GEO2D* small = make(1, {1, 1, 0, 2, 2, 0});
    OBJ_GEO2D* big = make(1, {0, 0, 0, 3, 3, 0});
    CHECK(small->contains_bbox(big));
    CHECK(!big->contains_bbox(small));
    CHECK(small->contains_bbox(small));
    delete small;
    delete big;
}

TEST_CASE(duplicate_points_are_dropped)
{
    geofake::reset();
    // rows 0 and 1 differ by 1e-7 in y (< 1e-6): row 0 is dropped; rows 1, 2 are kept
    OBJ_GEO2D* a = make(1, {0, 0, 7, 0, 1e-7, 8, 1, 1, 9});
    CHECK(a->fun_1402765e0());
    CHECK_EQ(a->n_points, 2);
    CHECK(pts(a) == (std::vector<double>{0, 1e-7, 8, 1, 1, 9}));
    CHECK_EQ(int(a->flag28), 1);
    CHECK(!a->fun_1402765e0()); // nothing left to drop
    OBJ_GEO2D* b = make(2, {1, 0, 0, 0, 0, 0});
    CHECK(!b->fun_1402765e0()); // type 2 is left alone
    delete a;
    delete b;
}

TEST_CASE(assign_copies_points_and_builds_ext)
{
    geofake::reset();
    geofake::install_ext();
    OBJ_GEO2D* a = make(1, {1, 2, 3, 4, 5, 6});
    OBJ_GEO2D b;
    b.copy_from(a);
    CHECK_EQ(b.curve_type, 1);
    CHECK(pts(&b) == pts(a));
    CHECK(b.points != a->points);
    REQUIRE(b.ext != nullptr);
    CHECK_EQ(geofake::calls.ext_ctor, 1);
    CHECK(reinterpret_cast<geofake::FakeExt*>(b.ext)->owner == &b); // 14027f430
    CHECK_EQ(geofake::calls.ext_copy, 0);                             // a has no ext
    b.notify();
    CHECK_EQ(geofake::calls.ext_notify, 1);
    b.copy_from(nullptr); // slot 4 ignores 0
    CHECK_EQ(b.n_points, 2);
    b.clear_all();
    CHECK_EQ(geofake::calls.ext_dtor, 1);
    CHECK(b.ext == nullptr);
    CHECK(b.points == nullptr);
    delete a;
}

TEST_CASE(append_same_type)
{
    geofake::reset();
    geofake::install_ext();
    OBJ_GEO2D* a = make(1, {1, 1, 0, 2, 2, 0});
    OBJ_GEO2D* b = make(1, {3, 3, 0});
    a->append(b);
    CHECK_EQ(a->n_points, 3);
    CHECK(pts(a) == (std::vector<double>{1, 1, 0, 2, 2, 0, 3, 3, 0}));
    CHECK_EQ(int(a->flag28), 1);
    CHECK_EQ(b->n_points, 1); // other untouched
    delete a;
    delete b;
}

namespace rev {
std::int32_t seen_dof = 0;
void fake_reverse(std::int32_t* dof, std::int32_t* n, double* buf)
{
    seen_dof = *dof;
    std::vector<double> t(buf, buf + *n * *dof);
    for (int r = 0; r < *n; ++r)
        for (int c = 0; c < *dof; ++c) buf[r * *dof + c] = t[(*n - 1 - r) * *dof + c];
}
} // namespace rev

TEST_CASE(reverse_converts_arc_angle_and_calls_fortran)
{
    geofake::reset();
    pending::fun_14041f590 = &rev::fake_reverse;
    OBJ_GEO2D* a = make(2, {1, 0, 0, 1, 0, 0, 2, 1, 0, 2, 0, 1.0});
    a->reverse();
    CHECK_EQ(rev::seen_dof, 6);
    // last column times 180/pi (DAT_140522fd8) before the reversal
    const double deg = 1.0 * 57.29577951308232;
    CHECK(pts(a) == (std::vector<double>{2, 1, 0, 2, 0, deg, 1, 0, 0, 1, 0, 0}));
    CHECK_EQ(int(a->flag28), 1);
    delete a;
}

namespace conv {
std::vector<std::uint64_t> order;
void f345f5b(double*, std::int32_t*) { order.push_back(0x140345f5b); }
void f32b34b(std::int32_t* nin, double*, std::int32_t* nout, double* out)
{
    order.push_back(0x14032b34b);
    CHECK_EQ(*nin, 2); // after the duplicate removal
    *nout = 1;
    for (int i = 0; i < 6; ++i) out[i] = 10 + i;
}
} // namespace conv

TEST_CASE(set_curve_type_points_to_segments)
{
    geofake::reset();
    pending::fun_140345f5b = &conv::f345f5b;
    pending::fun_14032b34b = &conv::f32b34b;
    OBJ_GEO2D* a = make(1, {0, 0, 0, 0, 0, 0, 5, 5, 0}); // rows 0 and 1 equal
    CHECK_EQ(a->set_curve_type(1), 0);                    // same type
    CHECK_EQ(a->set_curve_type(2), 1);
    CHECK(conv::order == (std::vector<std::uint64_t>{0x140345f5b, 0x14032b34b}));
    CHECK_EQ(a->curve_type, 2);
    CHECK_EQ(a->n_points, 1);
    CHECK(pts(a) == (std::vector<double>{10, 11, 12, 13, 14, 15}));
    OBJ_GEO2D empty;
    empty.curve_type = 1;
    CHECK_EQ(empty.set_curve_type(2), 0); // no rows: only the type changes
    CHECK_EQ(empty.curve_type, 2);
    delete a;
}

namespace ccw {
std::vector<double> got;
double area_sign = 1.0;
double area(std::int32_t n, double* xy)
{
    got.assign(xy, xy + 2 * n);
    return area_sign;
}
} // namespace ccw

TEST_CASE(is_ccw_builds_polyline_from_lines)
{
    geofake::reset();
    pending::fun_140309550 = &ccw::area;
    OBJ_GEO2D* a = make(2, {1, 0, 0, 1, 0, 0, 1, 1, 0, 1, 1, 0});
    ccw::area_sign = 1.0;
    CHECK(a->is_ccw());
    CHECK(ccw::got == (std::vector<double>{0, 0, 1, 0, 1, 0, 1, 1}));
    ccw::area_sign = -0.5;
    CHECK(!a->is_ccw());
    ccw::area_sign = 0.0; // >= 0 counts as counter-clockwise
    CHECK(a->is_ccw());
    OBJ_GEO2D* b = make(1, {4, 5, 0, 6, 7, 0});
    ccw::area_sign = 1.0;
    CHECK(b->is_ccw());
    CHECK(ccw::got == (std::vector<double>{4, 5, 6, 7}));
    delete a;
    delete b;
}

namespace rot {
std::vector<std::ptrdiff_t> offs;
const double* base = nullptr;
void f(double* p, double* angle, const double*)
{
    CHECK_BITS(*angle, 30.0);
    offs.push_back(p - base);
}
} // namespace rot

TEST_CASE(rotate_passes_each_point)
{
    geofake::reset();
    pending::fun_14032ab90 = &rot::f;
    OBJ_GEO2D* b = make(2, {1, 0, 0, 1, 0, 0, 1, 1, 0, 2, 0, 0});
    rot::base = b->points;
    double c[2] = {0, 0};
    b->rotate(c, nullptr, 30.0);
    CHECK(rot::offs == (std::vector<std::ptrdiff_t>{1, 3, 7, 9}));
    delete b;
}

TEST_CASE(transform_point_row_vector_times_matrix)
{
    // m = translation (5, 6, 7) in row 3; p' = (p, 1) * m
    double m[16] = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 5, 6, 7, 1};
    double p[3] = {1, 2, 3};
    geo_transform_point(p, m);
    CHECK_BITS(p[0], 6.0);
    CHECK_BITS(p[1], 8.0);
    CHECK_BITS(p[2], 10.0);
    // 90 degrees about z: x' = sum_k v_k m[k][0] = -y
    double r[16] = {0, 1, 0, 0, -1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};
    double q[3] = {1, 2, 3};
    geo_transform_point(q, r);
    CHECK_BITS(q[0], -2.0);
    CHECK_BITS(q[1], 1.0);
    CHECK_BITS(q[2], 3.0);
}

TEST_CASE(delete_runs_clear_all)
{
    geofake::reset();
    geofake::install_ext();
    OBJ_GEO2D* a = make(1, {1, 2, 3});
    a->make_ext();
    OBJ_GEO_BASE* base = a;
    delete base; // virtual dtor through OBJ_GEO_BASE
    CHECK_EQ(geofake::calls.ext_dtor, 1);
}

int main() { return testkit::run_all(); }
