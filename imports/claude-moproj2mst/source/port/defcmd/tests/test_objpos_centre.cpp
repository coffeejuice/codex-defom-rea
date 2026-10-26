// ctest-labels: p67-1 objpos
// Own centre (OBJPOS mode 6), the mass-property kernels, the boxes / WPAXIS helpers of modes 13
// and 14, the surface union and the object predicates (defcmd/objpos_centre.hpp).
//
// Seeds (copied from the corpus, C:/Users/alext/Projects/DEFORM_corpus, to a per-process temp
// file; the test passes with a note when the corpus is absent) are read through the KEY loop with
// the group 1-5/7 parsers. Expected values: the original's stdout of the cached oracle runs
// OBJPOS/6-basic_3d, gk-6-tri_3d, 6-basic_2d, 13-sub0_3d, 13-sub1_3d, 14-sub1_3d (deform-rea-work
// out/p671/oracle/OBJPOS), and independent long double computations.
#define _CRT_SECURE_NO_WARNINGS
#include "../../defkey/tests/key_test_util.hpp"

#include <cmath>
#include <cstdio>
#include <cstring>
#include <filesystem>
#include <string>
#include <vector>

#include <defcmd/objpos.hpp>
#include <defcmd/objpos_centre.hpp>
#include <defkey/group1.hpp>
#include <defkey/key_reader.hpp>
#include <defkey/kw_g2obj.hpp>
#include <defkey/kw_g3/kw_g3.hpp>
#include <defkey/kw_g4.hpp>
#include <defkey/kw_g57.hpp>
#include <defmodel/geo_kw/geo_kw.hpp>
#include <defmodel/geo_kw/surface_model.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/layout/obj_geo3d.hpp>
#include <defmodel/llobj/object_list.hpp>
#include <defrt/stdlib/legacy_printf.hpp>
#include <testkit/check.hpp>

namespace fs = std::filesystem;
using def::model::DB_STEP_DATA;
using def::model::LLOBJ_DATA;
using def::model::OBJ_GEO3D;
namespace op = def::cmd::objpos;
namespace gk = def::model::geo_kw;

namespace {

template <class T, class C>
T& at(C* base, std::size_t off)
{
    return *reinterpret_cast<T*>(reinterpret_cast<unsigned char*>(base) + off);
}

const char* kCorpus = "C:/Users/alext/Projects/DEFORM_corpus/key/";

struct Hooks : def::key::KeyReaderHooks {
    DB_STEP_DATA* m;
    explicit Hooks(DB_STEP_DATA* model) : m(model) {}
    bool has_sub_model() override { return false; }
    void set_model_dimension(int dim) override
    {
        std::int32_t d = dim;
        m->set_dimension(&d);
    }
    void set_sub_model_dimension(int) override {}
    int parse_keyword(int group, std::FILE* f, int id) override
    {
        switch (group) {
        case 1: return def::key::read_group1(m, f, id);
        case 2: return def::key::g2obj::read_object_keyword(*m, f, id);
        case 3: return def::key::g3::read_keyword(m, f, id);
        case 4: return def::key::g4::read_keyword(*m, f, id);
        case 5: return def::key::g57::read_phase_keyword(m, f, id);
        case 7: return def::key::g57::read_dim_keyword(m, f, id);
        default: break;
        }
        char buf[0x400];
        (void)std::fgets(buf, sizeof buf, f);
        return 0;
    }
};

// a model read from a copy of corpus file `rel`; null when the corpus is absent
DB_STEP_DATA* load(const char* rel)
{
    const fs::path src = fs::path(kCorpus) / rel;
    if (!fs::exists(src)) {
        std::printf("corpus file %s missing: skipped\n", src.string().c_str());
        return nullptr;
    }
    const fs::path tmp = fs::temp_directory_path() / keytest::tmp_name("objpos_centre_seed.key");
    fs::copy_file(src, tmp, fs::copy_options::overwrite_existing);
    auto* m = new DB_STEP_DATA(3);  // leaked on purpose (the dtor is not under test)
    Hooks h(m);
    std::FILE* f = std::fopen(tmp.string().c_str(), "r");
    std::FILE* sink = std::fopen("NUL", "w");
    def::key::read_key_stream(f, h, sink);
    std::fclose(f);
    std::fclose(sink);
    std::error_code ec;
    fs::remove(tmp, ec);
    return m;
}

LLOBJ_DATA* obj(DB_STEP_DATA* m, int n) { return def::model::object_list_find(&m->objects, n); }

std::string g3(const double* c)
{
    char b[128];
    def::rt::crt::snprintf(b, sizeof b, "%g, %g, %g", c[0], c[1], c[2]);
    return b;
}
std::string g2(const double* c)
{
    char b[128];
    def::rt::crt::snprintf(b, sizeof b, "%g, %g", c[0], c[1]);
    return b;
}

// Volume centroid of a closed polyhedral surface (faces of 4 node numbers, a triangle as "a b c a",
// quads split into two triangles), in long double: sum of signed tetrahedra from the origin.
void reference_centroid(const double* p, const std::int32_t* f, int nf, long double* c)
{
    long double v = 0, s[3] = {0, 0, 0};
    auto pt = [&](int n, int j) { return static_cast<long double>(p[(n - 1) * 3 + j]); };
    auto tri = [&](int a, int b, int d) {
        const long double det = pt(a, 0) * (pt(b, 1) * pt(d, 2) - pt(b, 2) * pt(d, 1)) -
                                pt(a, 1) * (pt(b, 0) * pt(d, 2) - pt(b, 2) * pt(d, 0)) +
                                pt(a, 2) * (pt(b, 0) * pt(d, 1) - pt(b, 1) * pt(d, 0));
        v += det / 6;
        for (int j = 0; j < 3; ++j) s[j] += det / 24 * (pt(a, j) + pt(b, j) + pt(d, j));
    };
    for (int i = 0; i < nf; ++i) {
        const std::int32_t* q = f + i * 4;
        const int d = q[3] < 0 ? -q[3] : q[3];
        tri(q[0], q[1], q[2]);
        if (d != q[0]) tri(q[0], q[2], d);
    }
    for (int j = 0; j < 3; ++j) c[j] = s[j] / v;
}

// the temporary surface of a mesh object (as own_centre builds it), as plain arrays
void mesh_surface(LLOBJ_DATA* o, std::vector<double>& p, std::vector<std::int32_t>& f)
{
    char one = 1;
    op::fun_1401fc730(o, &one);
    OBJ_GEO3D* s = gk::surface(*o, 0);
    p.assign(s->coords.begin(), s->coords.end());
    f.assign(s->elems.begin(), s->elems.end());
    op::fun_1401ffed0(o);
}

} // namespace

// ---- kernels on synthetic surfaces ----

// The mass-property kernels themselves are tested by p67-3 (drop mode 8).

TEST_CASE(surface_union_appends)
{
    OBJ_GEO3D u, s;
    std::int32_t np = 3, nf = 1;
    gk::geo3d_size(&u, &np, &nf);
    gk::geo3d_size(&s, &np, &nf);
    for (int i = 0; i < 9; ++i) {
        u.coords.mutable_at(i) = i;
        s.coords.mutable_at(i) = 100 + i;
    }
    const std::int32_t fu[4] = {1, 2, 3, 1}, fs_[4] = {1, 3, 2, -1};
    for (int k = 0; k < 4; ++k) {
        u.elems.mutable_at(k) = fu[k];
        s.elems.mutable_at(k) = fs_[k];
    }
    s.elem_flags[0] = 7;
    op::fun_1402fa880(&u, &s);
    CHECK_EQ(u.node_count(), 6);
    CHECK_EQ(u.elem_count(), 2);
    CHECK_EQ(u.coords.begin()[9], 100.0);
    CHECK_EQ(u.coords.begin()[17], 108.0);
    const std::int32_t* e = u.elems.begin();
    CHECK_EQ(e[0], 1);
    CHECK_EQ(e[4], 4);
    CHECK_EQ(e[5], 6);
    CHECK_EQ(e[6], 5);
    CHECK_EQ(e[7], -4);  // v < 1: v - np0
    CHECK_EQ(u.elem_flags[0], 0);
    CHECK_EQ(u.elem_flags[1], 7);
    CHECK_EQ(static_cast<int>(u.elem_ids.size()), 2);
}

// ---- seeds ----

TEST_CASE(centre_3d_quad_rule_upset_brick)
{
    keytest::legacy_printf();
    DB_STEP_DATA* m = load("3D/SI/Forging/UPSET_Brick.KEY");
    if (m == nullptr) return;
    LLOBJ_DATA* o = obj(m, 2);
    CHECK(o != nullptr);
    double c[3] = {0, 0, 0};
    op::own_centre(o, c);
    // 6-basic_3d: "rotated by 45 w.r.t. origin(6, 7.5, 7.5)"
    CHECK_EQ(g3(c), std::string("6, 7.5, 7.5"));
    // independent: the polyhedral centroid of the same surface (geometry 0 or the mesh surface)
    OBJ_GEO3D* s = gk::has_surface_flag(*o) ? gk::surface(*o, 0) : nullptr;
    std::vector<double> p;
    std::vector<std::int32_t> f;
    if (s != nullptr) {
        p.assign(s->coords.begin(), s->coords.end());
        f.assign(s->elems.begin(), s->elems.end());
    } else {
        mesh_surface(o, p, f);
    }
    long double r[3];
    reference_centroid(p.data(), f.data(), static_cast<int>(f.size() / 4), r);
    for (int j = 0; j < 3; ++j) CHECK(std::fabs(static_cast<long double>(c[j]) - r[j]) <= 1e-12L * (1 + std::fabs(r[j])));
}

TEST_CASE(centre_3d_triangle_rule_tet_mesh)
{
    keytest::legacy_printf();
    DB_STEP_DATA* m = load("3D/EN/Forging/Spike_ForgingBlow2.KEY");
    if (m == nullptr) return;
    LLOBJ_DATA* o = obj(m, 1);
    CHECK(o != nullptr);
    CHECK(!gk::has_surface_flag(*o));
    std::vector<double> p;
    std::vector<std::int32_t> f;
    mesh_surface(o, p, f);
    bool all_tri = true;
    for (std::size_t i = 0; i < f.size(); i += 4) all_tri = all_tri && f[i] == f[i + 3];
    CHECK(all_tri);
    double c[3] = {0, 0, 0};
    op::own_centre(o, c);
    // gk-6-tri_3d: "origin(0.500461, 0.502164, -0.21001)"
    CHECK_EQ(g3(c), std::string("0.500461, 0.502164, -0.21001"));
    long double r[3];
    reference_centroid(p.data(), f.data(), static_cast<int>(f.size() / 4), r);
    for (int j = 0; j < 3; ++j) CHECK(std::fabs(static_cast<long double>(c[j]) - r[j]) <= 1e-12L);
    // the temporary surface is gone again
    CHECK(!gk::has_surface_flag(*o));
    CHECK_EQ(gk::surface_count(*o), 0);
}

TEST_CASE(centre_2d_box_spike)
{
    keytest::legacy_printf();
    DB_STEP_DATA* m = load("2D/SI/Forging/SPIKE_SI.KEY");
    if (m == nullptr) return;
    LLOBJ_DATA* o = obj(m, 2);
    CHECK(o != nullptr);
    double c[2] = {0, 0};
    CHECK(op::own_centre(o, c));
    // 6-basic_2d: "rotated by 10 w.r.t. (31.75, 68.9057)"
    CHECK_EQ(g2(c), std::string("31.75, 68.9057"));
    double mn[2], mx[2];
    CHECK(op::geometry_bbox_2d(o, mn, mx));
    CHECK_EQ(c[0], (mn[0] + mx[0]) / 2.0);
    CHECK_EQ(c[1], (mn[1] + mx[1]) / 2.0);
    CHECK(op::has_geometry(o));
    CHECK(op::usable(o));
}

TEST_CASE(boxes_and_wpaxis_upset_brick)
{
    keytest::legacy_printf();
    DB_STEP_DATA* m = load("3D/SI/Forging/UPSET_Brick.KEY");
    if (m == nullptr) return;
    LLOBJ_DATA* o1 = obj(m, 1);
    LLOBJ_DATA* o2 = obj(m, 2);
    double mn[3], mx[3];
    // 13-sub0_3d: object 1 centred on YZ by (0, -18, -35): (max + min) / 2.0 of its node box
    CHECK(op::node_bbox(reinterpret_cast<unsigned char*>(o1) + 0x8a0, mn, mx));
    CHECK_EQ((mx[1] + mn[1]) / 2.0, 18.0);
    CHECK_EQ((mx[2] + mn[2]) / 2.0, 35.0);
    CHECK(!op::node_bc_code_7_or_8(o1));  // the centring ran, so no code 7 / 8
    // 14-sub1_3d: reference object 2 has a z height of 15
    CHECK(op::node_bbox(reinterpret_cast<unsigned char*>(o2) + 0x8a0, mn, mx));
    CHECK_EQ(mx[2] - mn[2], 15.0);
    // the geometry box (1e29 init) equals the node box for these DIEGEO = mesh objects
    if (gk::has_surface_flag(*o2)) {
        double gmn[3], gmx[3];
        op::surface_bbox(gk::surface(*o2, 0), gmn, gmx);
        CHECK(gmn[0] < 1e29 && gmx[0] > -1e29);
        CHECK(gmx[2] >= gmn[2]);
    }
    OBJ_GEO3D empty;
    op::surface_bbox(&empty, mn, mx);
    CHECK_EQ(mn[0], 1e29);
    CHECK_EQ(mx[2], -1e29);
    // 13-sub1_3d: WPAXIS item created as "WPAXIS 2 1 3 1" with zero vectors
    unsigned char* list = reinterpret_cast<unsigned char*>(o2) + 0x20a0;
    CHECK(op::wpaxis_find(list, 1) == nullptr);
    unsigned char* it = op::wpaxis_find_or_create(list, 1);
    CHECK(it != nullptr);
    CHECK_EQ(at<std::int32_t>(it, 0), 1);
    CHECK_EQ(at<std::int32_t>(it, 4), 3);
    CHECK_EQ(at<std::int32_t>(it, 0x60), 1);
    double a[3] = {9, 9, 9}, b[3] = {9, 9, 9};
    op::wpaxis_vector_a(it, a);
    op::wpaxis_vector_b(it, b);
    for (int j = 0; j < 3; ++j) CHECK(a[j] == 0.0 && b[j] == 0.0);
    CHECK(op::wpaxis_find_or_create(list, 1) == it);
    CHECK_EQ(at<std::int32_t>(list, 0x10), 1);
    op::wpaxis_vector_a(it, nullptr);  // no write
}

TEST_CASE(predicates_and_messages)
{
    keytest::legacy_printf();
    DB_STEP_DATA* m = load("3D/SI/Forging/UPSET_Brick.KEY");
    if (m == nullptr) return;
    LLOBJ_DATA* o = obj(m, 2);
    CHECK(op::usable(o));
    CHECK(op::has_mesh(o));
    CHECK(op::has_mesh_or_geometry(o));
    CHECK_EQ(op::has_die_geometry(o), gk::object_has_surface_mesh(*o));
    const std::int32_t type = at<std::int32_t>(o, 0xc);
    at<std::int32_t>(o, 0xc) = 7;
    CHECK(!op::usable(o));  // prints "ERROR  : Object 2 Undefined"
    at<std::int32_t>(o, 0xc) = type;
    const std::int32_t mesh = at<std::int32_t>(o, 0x87c);
    const std::uint8_t flag = at<std::uint8_t>(o, 0x3990);
    at<std::int32_t>(o, 0x87c) = 0;
    at<std::uint8_t>(o, 0x3990) = 0;
    CHECK(!op::usable(o));  // prints "ERROR  : No Mesh or Die Defined for Object 2"
    double c[3] = {1, 2, 3};
    CHECK(!op::centre_3d(o, c, nullptr));  // neither mesh nor geometry: c untouched
    CHECK_EQ(c[0], 1.0);
    at<std::int32_t>(o, 0x87c) = mesh;
    at<std::uint8_t>(o, 0x3990) = flag;
}

int main() { return testkit::run_all(); }
