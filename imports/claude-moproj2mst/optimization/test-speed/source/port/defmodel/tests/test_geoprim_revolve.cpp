// ctest-labels: p9-05 geoprim
// Tier-1 tests of the GEOPRIM revolve (packet p9-05): axis class 1402d9bb0, dispatch 1402ce190,
// profile projection 1402da250, strips 1402db310/1402dbb20, point compaction 1402dc160,
// non-uniform section steps 1402cac30, and the surface generator 1402d7cd0 end to end on a square
// profile. Expected ids are worked out by hand from the decompile (GEOPRIM_REVOLVE.md); expected
// coordinates are cos/sin of the layer angles (checked to 1e-12, the rotation itself belongs to
// p1-10's 140325691). Hooks owned by other packets get listing implementations here.
#include <defmodel/geo/pending.hpp>
#include <defmodel/geo_kw/geo_kw.hpp>
#include <defmodel/geoprim/def_geoprim.hpp>
#include <defmodel/geoprim/extrude.hpp>
#include <defmodel/geoprim/revolve.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <map>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/layout/obj_geo2d.hpp>
#include <defmodel/layout/obj_geo3d.hpp>
#include <defmodel/linked/record_e8_list.hpp>
#include <defrt/clock.hpp>
#include <testkit/check.hpp>

#include <algorithm>
#include <cstdio>
#include <array>
#include <cmath>
#include <cstring>
#include <fstream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

using namespace def::model;
using namespace def::model::geoprim;

namespace {

void* list_ctor(void* l) { return l; }
void list_fn(void*) {}
void** h_ctor(void** h) { *h = nullptr; return h; }
void h_fn(void**) {}
void* tree_ctor(void* t) { return t; }

void install_pending()
{
    pending::fun_140257bf0 = &list_ctor;
    pending::fun_140257c60 = &list_fn;
    pending::fun_1402581b0 = &list_fn;
    pending::fun_1402fedd0 = &h_ctor;
    pending::fun_1402fef20 = &h_ctor;
    pending::fun_1402fee40 = &h_fn;
    pending::fun_1402fef90 = &h_fn;
    pending::fun_1402ff680 = &h_fn;
    pending::fun_1402ff790 = &h_fn;
    pending::fun_14042a7e0 = &tree_ctor;
    pending::fun_14042a870 = &list_fn;
}

// listing implementations of the owners' kernels
void cross_l(double* out, const double* a, const double* b)   // 14023a4b0
{
    const double r[3] = {a[1] * b[2] - a[2] * b[1], a[2] * b[0] - a[0] * b[2], a[0] * b[1] - a[1] * b[0]};
    std::memcpy(out, r, sizeof r);
}
double len_l(const double* v) { return std::sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]); }   // 140239a30
double dot_l(const double* a, const double* b) { return a[0] * b[0] + a[1] * b[1] + a[2] * b[2]; }
void normal_l(double* out, const double* p, std::int32_t n)   // 140240d80
{
    double acc[3] = {0, 0, 0};
    double prev[3] = {p[3] - p[0], p[4] - p[1], p[5] - p[2]};
    for (std::int32_t k = 2; k <= n - 1; ++k) {
        double cur[3] = {p[k * 3] - p[0], p[k * 3 + 1] - p[1], p[k * 3 + 2] - p[2]};
        double c[3];
        cross_l(c, prev, cur);
        for (int i = 0; i < 3; ++i) acc[i] = acc[i] + c[i];
        std::memcpy(prev, cur, sizeof prev);
    }
    const double len = len_l(acc);
    for (int i = 0; i < 3; ++i) out[i] = len <= 1e-12 ? 0.0 : acc[i] / len;
}
double area_l(std::int32_t n, const double* p)   // 14023e670: 0.5 |sum p_i x p_i+1|
{
    double acc[3] = {0, 0, 0};
    for (std::int32_t i = 0; i < n; ++i) {
        const double* q = (i == n - 1) ? p : p + 3 * (i + 1);
        double c[3];
        cross_l(c, p + 3 * i, q);
        for (int k = 0; k < 3; ++k) acc[k] = acc[k] + c[k];
    }
    return 0.5 * len_l(acc);
}
void surface_size(LLOBJ_DATA* o, std::int32_t nn, std::int32_t nf, std::int32_t bdry)
{
    OBJ_GEO3D* g = geo_kw::surface(*o, bdry);
    g->resize_nodes(&nn);
    g->resize_elems(&nf);
}
int g_fixgeo_calls = 0;
void par_init(void* p) { std::memset(p, 0, 0x30); }
void par_free(void*) {}
void fixgeo_rec(OBJ_GEO2D*, void*, std::int32_t* k, std::int32_t mode, char)
{
    ++g_fixgeo_calls;
    CHECK_EQ(mode, 1);
    *k = 0;
}

void install()
{
    install_pending();
    P904Ext& e = p904_ext();
    e.dot = dot_l;
    e.poly_normal = normal_l;
    e.polygon_area = area_l;
    e.surface_size = surface_size;
    P905Ext& f = p905_ext();
    f.vec3_cross = cross_l;
    f.vec3_length = len_l;
    f.fixgeo_par_init = par_init;
    f.fixgeo_par_free = par_free;
    f.fixgeo = fixgeo_rec;
    g_fixgeo_calls = 0;
}

// zero-filled object with a real OBJ_GEO3D as surface 0 (as p9-04's builder test)
struct Obj {
    void* raw;
    OBJ_GEO3D* geo;
    Obj()
    {
        raw = ::operator new(sizeof(LLOBJ_DATA));
        std::memset(raw, 0, sizeof(LLOBJ_DATA));
        geo = new OBJ_GEO3D();
        o()->u3c00[0] = geo;
        geo_kw::set_surface_count(*o(), 1);
    }
    LLOBJ_DATA* o() { return static_cast<LLOBJ_DATA*>(raw); }
    linked::Cls_140258580* wpaxis()
    {
        return reinterpret_cast<linked::Cls_140258580List*>(static_cast<char*>(raw) + 0x20a0)->head;
    }
};

template <class T>
T at(const void* p, std::size_t off)
{
    T v;
    std::memcpy(&v, static_cast<const char*>(p) + off, sizeof v);
    return v;
}

std::unique_ptr<DEF_GEOPRIM> revolve_prim(double ax, double ay, double az)
{
    std::unique_ptr<DEF_GEOPRIM> p(new DEF_GEOPRIM());
    p->revolve = new RevolveRec();
    p->revolve->axis_dir[0] = ax;
    p->revolve->axis_dir[1] = ay;
    p->revolve->axis_dir[2] = az;
    p->kind = 2;
    return p;
}

} // namespace

TEST_CASE(axis_class)
{
    install();
    double dir[3], pt[3];
    std::int32_t t = -1;
    {
        auto p = revolve_prim(-2.0, 0.0, 0.0);
        p->revolve->axis_pt[0] = 1.0;
        p->revolve->axis_pt[1] = 2.0;
        p->revolve->axis_pt[2] = 3.0;
        fun_1402d9bb0(p.get(), dir, &t, pt);
        CHECK_EQ(t, 0);
        CHECK_BITS(dir[0], -1.0);
        CHECK_BITS(dir[1], 0.0);
        CHECK_BITS(pt[0], 1.0);   // kind 0: (P.x, P.y, 0)
        CHECK_BITS(pt[1], 2.0);
        CHECK_BITS(pt[2], 0.0);
    }
    {
        auto p = revolve_prim(0.0, 5.0, 0.0);
        p->revolve->axis_pt[0] = 1.0;
        p->revolve->axis_pt[1] = 2.0;
        p->revolve->axis_pt[2] = 3.0;
        p->revolve->axis_kind = 2;
        fun_1402d9bb0(p.get(), dir, &t, pt);
        CHECK_EQ(t, 1);
        CHECK_BITS(dir[1], 1.0);
        CHECK_BITS(pt[0], 0.0);   // t 1, kind 2: (0, P.y, P.z)
        CHECK_BITS(pt[1], 2.0);
        CHECK_BITS(pt[2], 3.0);
    }
    {
        auto p = revolve_prim(0.0, 0.0, -4.0);   // z axis: normalised, not snapped
        fun_1402d9bb0(p.get(), dir, &t, pt);
        CHECK_EQ(t, 2);
        CHECK_BITS(dir[2], -1.0);
    }
    {
        auto p = revolve_prim(3.0, 4.0, 0.0);
        fun_1402d9bb0(p.get(), dir, &t, pt);
        CHECK_EQ(t, 3);
        CHECK_BITS(dir[0], 0.6);
        CHECK_BITS(dir[1], 0.8);
    }
    {   // no record: nothing written
        std::unique_ptr<DEF_GEOPRIM> p(new DEF_GEOPRIM());
        t = 77;
        fun_1402d9bb0(p.get(), dir, &t, pt);
        CHECK_EQ(t, 77);
    }
}

TEST_CASE(dispatch_sets_axis_kind)
{
    install();
    auto p = revolve_prim(0.0, 0.0, 1.0);
    CHECK(!fun_1402ce190(p.get()));   // no path selected
    CHECK_EQ(p->revolve->axis_kind, 2);
    CHECK_EQ(int(p->u24db), 1);
    auto q = revolve_prim(0.0, 1.0, 0.0);
    fun_1402ce190(q.get());
    CHECK_EQ(q->revolve->axis_kind, 1);
    CHECK_EQ(int(q->u24db), 0);
}

TEST_CASE(profile_projection_onto_axis)
{
    install();
    auto p = revolve_prim(0.0, 0.0, 1.0);
    p->revolve->axis_kind = 2;
    // profile in (r, z); axis kind 2: d = (dir.y, dir.z, dir.x) = (0, 1, 0), |d.z| < 1e-6 ->
    // a2 = (d.x, d.y) = (0, 1), o = (pt.y, pt.z) = (0, 0). s = v.x * 1 - 0 * v.y = r.
    // r: 10, 10, -1, 20 -> pos 3, neg 1: the negative point is projected (r = 0, z kept).
    const std::int32_t n = 4;
    const double xy[8] = {10, 0, 10, 5, -1, 7, 20, 9};
    p->profile.assign(&n, xy);
    const double dir[3] = {0, 0, 1}, pt[3] = {0, 0, 0};
    fun_1402da250(p.get(), dir, pt);
    CHECK_EQ(g_fixgeo_calls, 1);
    CHECK_EQ(p->profile.n, 4);
    CHECK_BITS(p->profile.pts[2 * 3], 0.0);
    CHECK_BITS(p->profile.pts[2 * 3 + 1], 7.0);
    CHECK_BITS(p->profile.pts[2 * 1], 10.0);
    // all on one side: no FIXGEO
    g_fixgeo_calls = 0;
    const double xy2[8] = {10, 0, 10, 5, 1, 7, 20, 9};
    p->profile.assign(&n, xy2);
    fun_1402da250(p.get(), dir, pt);
    CHECK_EQ(g_fixgeo_calls, 0);
}

TEST_CASE(strips_triangles_open_profile)
{
    install();
    std::unique_ptr<DEF_GEOPRIM> p(new DEF_GEOPRIM());
    const std::int32_t n = 3;
    const double xy[6] = {1, 0, 2, 0, 3, 0};
    p->profile.assign(&n, xy);   // u10 = 0: the closing segment is skipped
    std::int32_t nsec = 2, nf = 0;
    char full = 0;
    char axis[3] = {0, 0, 0};
    std::vector<std::int32_t> f(64, 0);
    fun_1402db310(p.get(), &nsec, &full, axis, f.data(), &nf);
    CHECK_EQ(nf, 8);
    const std::int32_t want[8][3] = {{1, 4, 5}, {1, 5, 2}, {2, 5, 6}, {2, 6, 3},
                                     {4, 7, 8}, {4, 8, 5}, {5, 8, 9}, {5, 9, 6}};
    for (int i = 0; i < 8; ++i) {
        for (int k = 0; k < 3; ++k) CHECK_EQ(f[i * 4 + k], want[i][k]);
        CHECK_EQ(f[i * 4 + 3], want[i][0]);
    }
    // point 1 on the axis: its triangle of each pair collapses
    axis[0] = 1;
    nf = 0;
    fun_1402db310(p.get(), &nsec, &full, axis, f.data(), &nf);
    CHECK_EQ(nf, 6);
    CHECK_EQ(f[0], 1);   // c1 set: only (i+1, base+i+2+n, base+2+i) = (1, 5, 2)
    CHECK_EQ(f[1], 5);
    CHECK_EQ(f[2], 2);
}

TEST_CASE(strips_full_revolution_join)
{
    install();
    std::unique_ptr<DEF_GEOPRIM> p(new DEF_GEOPRIM());
    const std::int32_t n = 2;
    const double xy[4] = {1, 0, 2, 0};
    p->profile.assign(&n, xy);
    std::int32_t nsec = 3, nf = 0;
    char full = 1;
    char axis[2] = {0, 0};
    std::vector<std::int32_t> f(64, 0);
    fun_1402db310(p.get(), &nsec, &full, axis, f.data(), &nf);
    // layers 0..1 (nsec-1 = 2 layer pairs) then the join of layer 2 to layer 0
    CHECK_EQ(nf, 6);
    const std::int32_t want[6][3] = {{1, 3, 4}, {1, 4, 2}, {3, 5, 6}, {3, 6, 4}, {5, 1, 2}, {5, 2, 6}};
    for (int i = 0; i < 6; ++i)
        for (int k = 0; k < 3; ++k) CHECK_EQ(f[i * 4 + k], want[i][k]);
}

TEST_CASE(quad_strips)
{
    install();
    std::unique_ptr<DEF_GEOPRIM> p(new DEF_GEOPRIM());
    const std::int32_t n = 2;
    const double xy[4] = {1, 0, 2, 0};
    p->profile.assign(&n, xy);
    std::int32_t nsec = 2, nf = 0;
    char full = 1;
    char axis[2] = {0, 0};
    std::vector<std::int32_t> f(64, 0);
    fun_1402dbb20(p.get(), &nsec, &full, axis, f.data(), &nf);
    // s 0: i 0 (1, 3, 4, 2); i 1 last: (2, 4, 3, 1). s 1 (last layer, full): (3, 1, 2, 4), (4, 2, 1, 3)
    CHECK_EQ(nf, 4);
    const std::int32_t want[4][4] = {{1, 3, 4, 2}, {2, 4, 3, 1}, {3, 1, 2, 4}, {4, 2, 1, 3}};
    for (int i = 0; i < 4; ++i)
        for (int k = 0; k < 4; ++k) CHECK_EQ(f[i * 4 + k], want[i][k]);
}

TEST_CASE(nonuniform_ranges)
{
    std::unique_ptr<DEF_GEOPRIM> p(new DEF_GEOPRIM());
    double a2 = 10.0, a3 = 20.0, start = 0.0, angle = 90.0;
    char full = 0;
    double p7 = -1, p8 = -1, p9 = -1, p10 = -1, p11 = -1, p12 = -1, p13 = -1, p14 = -1;
    fun_1402cac30(p.get(), &a2, &a3, &start, &angle, &full, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14);
    // |(10 + 20) - 0 + 90| > 0.01, not full: first branch
    CHECK_BITS(p11, 0.0);
    CHECK_BITS(p12, 10.0);
    CHECK_BITS(p13, 30.0);
    CHECK_BITS(p14, 60.0);   // (90 - 10) - 20
    CHECK_BITS(p7, 10.0);
    CHECK_BITS(p8, 20.0);
    CHECK_BITS(p10, 0.0);
    CHECK_BITS(p9, -1.0);
}

TEST_CASE(surface_square_profile_quarter_turn)
{
    install();
    def::rt::pin_clock(1700000000);
    Obj obj;
    auto p = revolve_prim(0.0, 0.0, 1.0);
    const std::int32_t n = 4;
    const double xy[8] = {10, 0, 20, 0, 20, 10, 10, 10};   // (r, z), counter-clockwise
    p->profile.assign(&n, xy);
    p->angle = 90.0;
    p->sections = 2;
    p->mode_geo = 1;
    p->obj = obj.o();
    CHECK(fun_1402ce190(p.get()));
    def::rt::pin_clock(-1);
    OBJ_GEO3D* g = obj.geo;
    CHECK_EQ(g->node_count(), 12);
    CHECK_EQ(g->elem_count(), 20);   // 16 strip triangles, 2 + 2 cap triangles
    // layer k at 45 k degrees from +x (the -90 degree turn of axis kind 2 maps (0, r, z) to x = r)
    for (int k = 0; k < 3; ++k)
        for (int i = 0; i < 4; ++i) {
            const double r = xy[2 * i], z = xy[2 * i + 1];
            const double a = 3.141592653589793 / 4.0 * k;
            const double* c = &g->coords[static_cast<std::size_t>((k * 4 + i) * 3)];
            CHECK(std::fabs(c[0] - r * std::cos(a)) < 1e-12);
            CHECK(std::fabs(c[1] - r * std::sin(a)) < 1e-12);
            CHECK_BITS(c[2], z);
        }
    // strips of layer pair 0 (closed profile): (1 5 6) (1 6 2) (2 6 7) (2 7 3) (3 7 8) (3 8 4) (4 8 5) (4 5 1)
    const std::int32_t want[8][3] = {{1, 5, 6}, {1, 6, 2}, {2, 6, 7}, {2, 7, 3},
                                     {3, 7, 8}, {3, 8, 4}, {4, 8, 5}, {4, 5, 1}};
    // faces 0..1 are the near cap, 2..17 the strips, 18..19 the far cap
    for (int i = 0; i < 8; ++i)
        for (int k = 0; k < 3; ++k) CHECK_EQ(g->elems[static_cast<std::size_t>((2 + i) * 4 + k)], want[i][k]);
    // the far cap is the near cap reversed with ids + 8
    for (int c = 0; c < 2; ++c) {
        const std::int32_t* nc = &g->elems[static_cast<std::size_t>(c * 4)];
        const std::int32_t* fc = &g->elems[static_cast<std::size_t>((18 + c) * 4)];
        CHECK_EQ(fc[0], nc[3] + 8);
        CHECK_EQ(fc[1], nc[2] + 8);
        CHECK_EQ(fc[2], nc[1] + 8);
        CHECK_EQ(fc[3], nc[0] + 8);
    }
    // the near cap covers the square: diagonal 2-4 or 1-3, all ids in 1..4
    std::vector<std::int32_t> ids;
    for (int c = 0; c < 2; ++c)
        for (int k = 0; k < 3; ++k) ids.push_back(g->elems[static_cast<std::size_t>(c * 4 + k)]);
    for (std::int32_t v : ids) CHECK(v >= 1 && v <= 4);
    linked::Cls_140258580* w = obj.wpaxis();
    CHECK(w != nullptr);
    CHECK_EQ(at<std::int32_t>(w, 4), 0);
    CHECK_BITS(at<double>(w, 0x30), 1.0);   // axis dir z (record copy, raw)
}

TEST_CASE(compaction_drops_unused_points)
{
    install();
    Obj obj;
    std::unique_ptr<DEF_GEOPRIM> p(new DEF_GEOPRIM());
    p->obj = obj.o();
    const std::int32_t np = 4, nf = 1;
    const double xyz[12] = {0, 0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 3};
    const std::int32_t faces[4] = {2, 4, 4, 2};   // only the first 3 ids mark points
    fun_1402dc160(p.get(), &np, xyz, &nf, faces);
    CHECK_EQ(obj.geo->node_count(), 2);
    CHECK_BITS(obj.geo->coords[0], 1.0);
    CHECK_BITS(obj.geo->coords[3], 3.0);
    CHECK_EQ(obj.geo->elems[0], 1);
    CHECK_EQ(obj.geo->elems[1], 2);
    CHECK_EQ(obj.geo->elems[3], 1);
}

TEST_CASE(oracle_die_quarter_turn_20_sections)
{
    // Oracle: DEF_PRE CNVT3D of RING_SI with GEO23 1 1 / 90 0 0, GEOSEC 1 20 (case
    // tests/commands/CNVT3D/cnvt3d_geometry_die_rev90_obj1.json); the die (object 1) has no mesh, so
    // its 3D surface is this revolve of its 5-point profile r = 0 .. 3175 at z = 635 (the 2D
    // geometry block of the same CONV3D.KEY). The DIEGEO block of the 3D object is the golden file:
    // 85 nodes (the axis point once, 1402dc160), 140 triangles (1402db310, open profile: no caps).
    install();
    Obj obj;
    auto p = revolve_prim(0.0, 0.0, 1.0);
    const std::int32_t n = 5;
    const double xy[10] = {0.0, 635.0, 793.75, 635.0, 1587.5, 635.0, 2381.25, 635.0, 3175.0, 635.0};
    p->profile.assign(&n, xy);
    p->angle = 90.0;
    p->sections = 20;
    p->mode_geo = 1;
    p->obj = obj.o();
    CHECK(fun_1402ce190(p.get()));
    std::ifstream in(testkit::repo_root() / "port/defmodel/tests/data/p905_die_rev90_diegeo.txt");
    std::string kw;
    int a, b, nn, nf;
    in >> kw >> a >> b >> nn >> nf;
    CHECK_EQ(nn, 85);
    CHECK_EQ(nf, 140);
    OBJ_GEO3D* g = obj.geo;
    CHECK_EQ(g->node_count(), nn);
    CHECK_EQ(g->elem_count(), nf);
    int bad = 0;
    for (int i = 0; i < nn; ++i) {
        int id;
        double x[3];
        in >> id >> x[0] >> x[1] >> x[2];
        for (int k = 0; k < 3; ++k) {
            const double v = g->coords[static_cast<std::size_t>(i * 3 + k)];
            // the KEY prints 11 significant digits (%.10E)
            const double tol = 6e-11 * std::fabs(x[k]) + 1e-300;
            if (!(std::fabs(v - x[k]) <= tol) && bad++ < 10)
                std::printf("node %d comp %d: port %.10E oracle %.10E\n", id, k, v, x[k]);
        }
    }
    for (int i = 0; i < nf; ++i) {
        int id, q[4];
        in >> id >> q[0] >> q[1] >> q[2] >> q[3];
        for (int k = 0; k < 4; ++k)
            if (g->elems[static_cast<std::size_t>(i * 4 + k)] != q[k] && bad++ < 10)
                std::printf("face %d entry %d: port %d oracle %d\n", id, k, g->elems[static_cast<std::size_t>(i * 4 + k)], q[k]);
    }
    CHECK_EQ(bad, 0);
}

namespace {
// Oracle cnvt3d_geometry_wp_rev90_obj2: the entry's recorded start second (oracle tool
// "start_time", the clock-pinning scheme of p67-gate's cmd_oracle_runner) and the cap faces of
// DIEGEO 2 1 336 668 in its CONV3D.KEY (faces 1..14 and 655..668; the KEY writer prints the 4th
// id negated for these symmetry faces). The cap order depends on that second: the triangulator
// seeds its segment order with srand(time(NULL)) (14045c530).
constexpr std::int64_t kWpStart = 1792598126;
const std::int32_t kWpNear[14][4] = {{4, 5, 16, -4}, {3, 4, 16, -3}, {2, 3, 16, -2}, {1, 2, 16, -1}, {6, 16, 5, -6},
                                     {6, 15, 16, -6}, {7, 15, 6, -7}, {7, 14, 15, -7}, {8, 14, 7, -8}, {8, 13, 14, -8},
                                     {12, 13, 8, -12}, {11, 12, 8, -11}, {10, 11, 8, -10}, {9, 10, 8, -9}};
const std::int32_t kWpFar[14][4] = {{324, 336, 325, -324}, {323, 336, 324, -323}, {322, 336, 323, -322},
                                    {321, 336, 322, -321}, {326, 325, 336, -326}, {326, 336, 335, -326},
                                    {327, 326, 335, -327}, {327, 335, 334, -327}, {328, 327, 334, -328},
                                    {328, 334, 333, -328}, {332, 328, 333, -332}, {331, 328, 332, -331},
                                    {330, 328, 331, -330}, {329, 328, 330, -329}};

// all 668 faces of the port's surface for the workpiece outline, clock pinned to `seed`
std::vector<std::int32_t> wp_faces(std::int64_t seed)
{
    Obj obj;
    auto p = revolve_prim(0.0, 0.0, 1.0);
    std::vector<double> xy;
    const double r0 = 952.5, r1 = 1905.0, z1 = 635.0;
    for (int k = 0; k < 4; ++k) { xy.push_back(r0 + (r1 - r0) * k / 4.0); xy.push_back(0.0); }
    for (int k = 0; k < 4; ++k) { xy.push_back(r1); xy.push_back(z1 * k / 4.0); }
    for (int k = 0; k < 4; ++k) { xy.push_back(r1 - (r1 - r0) * k / 4.0); xy.push_back(z1); }
    for (int k = 0; k < 4; ++k) { xy.push_back(r0); xy.push_back(z1 - z1 * k / 4.0); }
    const std::int32_t n = 16;
    p->profile.assign(&n, xy.data());
    p->angle = 90.0;
    p->sections = 20;
    p->mode_geo = 1;
    p->obj = obj.o();
    def::rt::pin_clock(seed);
    fun_1402ce190(p.get());
    def::rt::pin_clock(-1);
    std::vector<std::int32_t> r;
    if (obj.geo->node_count() != 336 || obj.geo->elem_count() != 668) return r;
    r.assign(obj.geo->elems.begin(), obj.geo->elems.begin() + 668 * 4);
    return r;
}

bool caps_match(const std::vector<std::int32_t>& f)
{
    if (f.size() != 668 * 4) return false;
    for (int i = 0; i < 14; ++i)
        for (int k = 0; k < 4; ++k) {
            const std::int32_t near_v = k == 3 ? -kWpNear[i][3] : kWpNear[i][k];
            const std::int32_t far_v = k == 3 ? -kWpFar[i][3] : kWpFar[i][k];
            if (f[static_cast<std::size_t>(i * 4 + k)] != near_v) return false;
            if (f[static_cast<std::size_t>((654 + i) * 4 + k)] != far_v) return false;
        }
    return true;
}
} // namespace

TEST_CASE(oracle_caps_at_recorded_start_second)
{
    install();
    // the shared scheme: the recorded second, then -1, +1, -2, +2 (DEF_PRE reads time() a moment
    // after the run starts); byte-exact, no face-set fallback
    int matched = 99;
    for (int off : {0, -1, 1, -2, 2}) {
        const auto f = wp_faces(kWpStart + off);
        CHECK_EQ(static_cast<int>(f.size()), 668 * 4);
        if (caps_match(f)) {
            matched = off;
            break;
        }
    }
    std::printf("caps match at start_time %+d s\n", matched);
    CHECK(matched != 99);
}

namespace {
std::map<void*, std::vector<double>> g_nodes;   // node coordinates per object node set
void mesh_alloc_rec(LLOBJ_DATA* o, std::int32_t nn, std::int32_t ne, std::int32_t nper, std::int32_t*, std::int32_t)
{
    CHECK_EQ(ne, 0);
    CHECK_EQ(nper, 4);
    g_nodes[o->s8a0].assign(static_cast<std::size_t>(nn) * 3, -1.0);
    std::memcpy(reinterpret_cast<char*>(o) + 0x880, &nn, 4);   // node count (+0x880)
}
double* node_coords_rec(void* set) { return g_nodes[set].data(); }
int g_surface_updates = 0;
void surface_update_rec(void*) { ++g_surface_updates; }
void model_copy_rec(DB_STEP_DATA*, std::int32_t*) {}
std::int32_t dim3(LLOBJ_DATA*) { return 3; }
std::vector<double> g_xy2;
std::vector<std::int32_t> g_conn2;
double* mesh_coords2(OBJ_MESH2D*) { return g_xy2.data(); }
std::int32_t* mesh_conn2(OBJ_MESH2D*) { return g_conn2.data(); }
} // namespace

TEST_CASE(mesh_boundary_sweep_two_quads)
{
    install();
    P905Ext& f = p905_ext();
    f.mesh_alloc = mesh_alloc_rec;
    f.node_coords = node_coords_rec;
    f.surface_update = surface_update_rec;
    f.model_copy_u15f8 = model_copy_rec;
    f.obj_dimension = dim3;
    ext().mesh_coords = mesh_coords2;
    ext().mesh_conn = mesh_conn2;
    g_nodes.clear();
    Obj obj;
    std::unique_ptr<DB_STEP_DATA> model(new DB_STEP_DATA(3));
    auto p = revolve_prim(0.0, 0.0, 1.0);
    // 2D mesh (r, z): nodes 1..3 at z 0, 4..6 at z 1, r = 10, 11, 12; two quads
    g_xy2 = {10, 0, 11, 0, 12, 0, 10, 1, 11, 1, 12, 1};
    g_conn2 = {1, 2, 5, 4, 2, 3, 6, 5};
    std::int32_t nn = 6, ne = 2;
    std::memcpy(reinterpret_cast<char*>(p.get()) + 0x8 + 0x12c, &nn, 4);
    std::memcpy(reinterpret_cast<char*>(p.get()) + 0x148 + 0xe8, &ne, 4);
    std::vector<std::int32_t> blist = {1, 2, 3, 6, 5, 4, 1};
    std::int32_t one = 1, nbl = 7;
    p->int_lists.resize(&one);
    p->int_lists.assign(&one, &nbl, blist.data());
    std::vector<std::int32_t> ctx(64, -1);
    p->ctx = ctx.data();
    p->angle = 90.0;
    p->sections = 2;
    p->mode_mesh = 2;
    p->obj = obj.o();
    p->model = model.get();
    CHECK(fun_1402ce190(p.get()));
    // layer 0: the 6 mesh nodes; layer 1 (45 deg): chain 1 2 3 6 5 4; last layer (90 deg): all 6
    CHECK_EQ(p->int_lists.items[0].n, 6);   // closed chain shortened by one
    CHECK_EQ(p->u23e0, 6);
    CHECK_EQ(p->u23e4, 12);   // (2 / 2) * 6 + 6
    CHECK_EQ(p->u23e8, 18);   // 2 * 6 + 6
    CHECK_EQ(p->u23ec, 6);
    CHECK_BITS(p->u2428, 90.0);
    const std::int32_t want_ctx[18] = {0, 1, 2, 3, 4, 5, 0, 1, 2, 5, 4, 3, 0, 1, 2, 3, 4, 5};
    for (int i = 0; i < 18; ++i) CHECK_EQ(ctx[static_cast<std::size_t>(i)], want_ctx[i]);
    CHECK_EQ(at<std::int32_t>(obj.o(), 0x880), 18);
    const std::vector<double>& c = g_nodes[obj.o()->s8a0];
    // axis kind 2 maps (r, z) to (0, r, z); layer angle a about +z: (-r sin a, r cos a, z)
    const int src[18] = {0, 1, 2, 3, 4, 5, 0, 1, 2, 5, 4, 3, 0, 1, 2, 3, 4, 5};
    for (int i = 0; i < 18; ++i) {
        const double a = (i < 6) ? 0.0 : (i < 12 ? 3.141592653589793 / 4 : 3.141592653589793 / 2);
        const double r = g_xy2[static_cast<std::size_t>(src[i] * 2)], z = g_xy2[static_cast<std::size_t>(src[i] * 2 + 1)];
        CHECK(std::fabs(c[static_cast<std::size_t>(i * 3)] + r * std::sin(a)) < 1e-12);
        CHECK(std::fabs(c[static_cast<std::size_t>(i * 3 + 1)] - r * std::cos(a)) < 1e-12);
        CHECK_BITS(c[static_cast<std::size_t>(i * 3 + 2)], z);
    }
    CHECK_EQ(at<std::int32_t>(obj.o(), 0xbd8), 1);
    CHECK_EQ(at<std::int32_t>(obj.o(), 0xbf0), 16);   // 6 * 2 strips + 2 * 2 caps, all quads
    CHECK_EQ(static_cast<int>(p->l2518.size()), 16);
    CHECK_EQ(int(p->u24fd), 1);
    CHECK_EQ(int(p->u24fc), 0);
    CHECK_EQ(g_surface_updates, 1);
    linked::Cls_140258580* w = obj.wpaxis();
    CHECK(w != nullptr);
    CHECK_BITS(at<double>(w, 0x30), 1.0);
}

int main() { return testkit::run_all(); }
