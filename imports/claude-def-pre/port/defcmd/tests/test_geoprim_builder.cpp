// GEOPRIM extrude primitive (packet p9-04): geometry path 1402bb460 and mesh-boundary path
// 1402bbb60 through 1402baa60, orientation 1402c1130. Expected faces, points and WPAXIS values
// are worked out by hand from the listing (CNVT3D_SWEEP.md 10.5). The callees other packets own
// (p9-05 cap triangulation, p67-1 rotate/translate/size_surface, p8-09 mesh allocation, the node
// coordinate accessor) are replaced by recorders; the cap triangles given here stand in for
// p9-05's output.
// ctest-labels: p9-04 geoprim
#include <defcmd/geoprim/builder.hpp>
#include <defmodel/geo/pending.hpp>
#include <defmodel/geo_kw/geo_kw.hpp>
#include <defmodel/geoprim/def_geoprim.hpp>
#include <defmodel/geoprim/extrude.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/layout/obj_geo3d.hpp>
#include <defmodel/linked/record_e8_list.hpp>
#include <testkit/check.hpp>

#include <cmath>
#include <cstring>
#include <memory>
#include <vector>

using namespace def::model;
using namespace def::model::geoprim;
namespace cg = def::cmd::geoprim;

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

double dot_listing(const double* a, const double* b) { return a[0] * b[0] + a[1] * b[1] + a[2] * b[2]; }
void cross(const double* p, const double* q, double* r)
{
    r[0] = p[1] * q[2] - p[2] * q[1];
    r[1] = p[2] * q[0] - p[0] * q[2];
    r[2] = p[0] * q[1] - p[1] * q[0];
}
void normal_listing(double* out, const double* p, std::int32_t n)
{
    double acc[3] = {0, 0, 0};
    double prev[3] = {p[3] - p[0], p[4] - p[1], p[5] - p[2]};
    for (std::int32_t k = 2; k <= n - 1; ++k) {
        double cur[3] = {p[k * 3] - p[0], p[k * 3 + 1] - p[1], p[k * 3 + 2] - p[2]};
        double c[3];
        cross(prev, cur, c);
        for (int i = 0; i < 3; ++i) acc[i] = acc[i] + c[i];
        std::memcpy(prev, cur, sizeof prev);
    }
    const double len = std::sqrt(acc[0] * acc[0] + acc[1] * acc[1] + acc[2] * acc[2]);
    for (int i = 0; i < 3; ++i) out[i] = len <= 1e-12 ? 0.0 : acc[i] / len;
}

void surface_size(LLOBJ_DATA* o, std::int32_t nn, std::int32_t nf, std::int32_t bdry)
{
    OBJ_GEO3D* g = geo_kw::surface(*o, bdry);
    g->resize_nodes(&nn);
    g->resize_elems(&nf);
}

int g_rotations = 0;
void rotate_rec(LLOBJ_DATA*, const double*, const double*, double) { ++g_rotations; }
void translate_rec(LLOBJ_DATA*, const double*, const char*, const char*) { ++g_rotations; }
std::int32_t cap_two(std::int32_t nc, const std::int32_t* n, const double*, std::int32_t* t)
{
    // stand-in for 14041f830 on the unit square
    if (nc != 1 || *n != 4) return 0;
    const std::int32_t v[6] = {1, 2, 3, 1, 3, 4};
    std::memcpy(t, v, sizeof v);
    return 2;
}

std::vector<double> g_nodes;
std::int32_t g_alloc_nn = -1, g_alloc_ne = -1, g_alloc_type = -1;
void mesh_alloc(LLOBJ_DATA*, std::int32_t nn, std::int32_t ne, std::int32_t t, const std::int32_t*,
                std::int32_t)
{
    g_alloc_nn = nn;
    g_alloc_ne = ne;
    g_alloc_type = t;
    g_nodes.assign(static_cast<std::size_t>(nn) * 3, -1.0);
}
double* node_coords(Cls_14017ef20*) { return g_nodes.data(); }

std::vector<double> g_xy;
std::vector<std::int32_t> g_conn;
double* mesh_coords(OBJ_MESH2D*) { return g_xy.data(); }
std::int32_t* mesh_conn(OBJ_MESH2D*) { return g_conn.data(); }

// zero-filled object with only the members the extrude touches; surface 0 is a real OBJ_GEO3D
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
T at(void* p, std::size_t off)
{
    T v;
    std::memcpy(&v, static_cast<char*>(p) + off, sizeof v);
    return v;
}

void install()
{
    install_pending();
    P904Ext& e = p904_ext();
    e.dot = dot_listing;
    e.poly_normal = normal_listing;
    e.surface_size = surface_size;
    cg::BuilderExt& b = cg::builder_ext();
    b.rotate = rotate_rec;
    b.translate = translate_rec;
    b.cap_triangulate = cap_two;
    b.mesh_alloc = mesh_alloc;
    b.node_coords = node_coords;
    ext().mesh_coords = mesh_coords;
    ext().mesh_conn = mesh_conn;
    g_rotations = 0;
}

} // namespace

TEST_CASE(geometry_path_square_profile)
{
    install();
    Obj obj;
    std::unique_ptr<DEF_GEOPRIM> prim(new DEF_GEOPRIM());
    const std::int32_t n = 4;
    const double xy[8] = {0, 0, 1, 0, 1, 1, 0, 1};
    prim->profile.assign(&n, xy);
    prim->extrude = new ExtrudeRec();   // dir (0, 0, 1)
    prim->extrude->length = 10.0;
    prim->u2360 = 10.0;
    prim->kind = 1;
    prim->mode_geo = 1;
    prim->obj = obj.o();
    CHECK(cg::fun_1402baa60(prim.get()));
    CHECK_EQ(g_rotations, 0);   // d . ez = 1 > 0.9
    CHECK_EQ(int(prim->profile.u10), 1);
    OBJ_GEO3D* g = obj.geo;
    CHECK_EQ(g->node_count(), 8);
    CHECK_EQ(g->elem_count(), 12);
    const double want_x[8][3] = {{0, 0, 10}, {1, 0, 10}, {1, 1, 10}, {0, 1, 10},
                                 {0, 0, 0},  {1, 0, 0},  {1, 1, 0},  {0, 1, 0}};
    for (int i = 0; i < 8; ++i)
        for (int k = 0; k < 3; ++k) CHECK_BITS(g->coords[static_cast<std::size_t>(i * 3 + k)], want_x[i][k]);
    const std::int32_t want_f[12][3] = {{1, 2, 3}, {1, 3, 4}, {7, 6, 5}, {8, 7, 5},
                                        {1, 5, 6}, {1, 6, 2}, {2, 6, 7}, {2, 7, 3},
                                        {3, 7, 8}, {3, 8, 4}, {4, 8, 5}, {4, 5, 1}};
    for (int i = 0; i < 12; ++i) {
        for (int k = 0; k < 3; ++k) CHECK_EQ(g->elems[static_cast<std::size_t>(i * 4 + k)], want_f[i][k]);
        CHECK_EQ(g->elems[static_cast<std::size_t>(i * 4 + 3)], want_f[i][0]);
    }
    CHECK(geo_kw::has_surface_flag(*obj.o()));
    linked::Cls_140258580* w = obj.wpaxis();
    CHECK(w != nullptr);
    CHECK_EQ(at<std::int32_t>(w, 4), 1);
    CHECK_BITS(at<double>(w, 0x18), 10.0);
    CHECK_BITS(at<double>(w, 0x30), 0.0);
}

TEST_CASE(geometry_path_open_profile_and_zero_length)
{
    install();
    Obj obj;
    std::unique_ptr<DEF_GEOPRIM> prim(new DEF_GEOPRIM());
    // closing edge (0,0)-(1,2) crosses (-1,1)-(2,1): no caps, and no closing strip
    const std::int32_t n = 4;
    const double xy[8] = {0, 0, -1, 1, 2, 1, 1, 2};
    prim->profile.assign(&n, xy);
    prim->extrude = new ExtrudeRec();
    prim->u2360 = 2.0;
    prim->mode_geo = 1;
    prim->obj = obj.o();
    CHECK(cg::fun_1402baa60(prim.get()));
    CHECK_EQ(obj.geo->elem_count(), 6);
    prim->u2360 = 1e-13;
    CHECK(!cg::fun_1402baa60(prim.get()));
}

TEST_CASE(orientation_along_x)
{
    install();
    Obj obj;
    std::unique_ptr<DEF_GEOPRIM> prim(new DEF_GEOPRIM());
    prim->extrude = new ExtrudeRec();
    prim->obj = obj.o();
    prim->extrude->dir[0] = 1.0;
    prim->extrude->dir[2] = 0.0;
    cg::fun_1402c1130(prim.get());
    CHECK_EQ(g_rotations, 2);
    g_rotations = 0;
    prim->extrude->dir[0] = -1.0;
    cg::fun_1402c1130(prim.get());
    CHECK_EQ(g_rotations, 3);   // two rotations and the translation
    g_rotations = 0;
    prim->extrude->dir[0] = 0.0;
    prim->extrude->dir[1] = 1.0;
    cg::fun_1402c1130(prim.get());
    CHECK_EQ(g_rotations, 2);   // rotation and translation
}

TEST_CASE(mesh_boundary_path_two_quads_two_sections)
{
    install();
    Obj obj;
    std::unique_ptr<DEF_GEOPRIM> prim(new DEF_GEOPRIM());
    std::unique_ptr<DB_STEP_DATA, void (*)(DB_STEP_DATA*)> model(
        static_cast<DB_STEP_DATA*>(std::memset(::operator new(sizeof(DB_STEP_DATA)), 0, sizeof(DB_STEP_DATA))),
        [](DB_STEP_DATA* p) { ::operator delete(p); });
    g_xy = {0, 0, 1, 0, 2, 0, 0, 1, 1, 1, 2, 1};
    g_conn = {1, 2, 5, 4, 2, 3, 6, 5};
    std::int32_t np = 6, ne = 2;
    std::memcpy(reinterpret_cast<char*>(prim.get()) + 0x8 + 0x12c, &np, 4);
    std::memcpy(reinterpret_cast<char*>(prim.get()) + 0x148 + 0xe8, &ne, 4);
    std::vector<std::int32_t> blist = {1, 2, 3, 6, 5, 4, 1};
    std::int32_t one = 1, nbl = 7;
    prim->int_lists.resize(&one);
    prim->int_lists.assign(&one, &nbl, blist.data());
    prim->extrude = new ExtrudeRec();
    prim->extrude->length = 10.0;
    prim->sections = 2;
    prim->kind = 1;
    prim->mode_mesh = 2;
    prim->obj = obj.o();
    prim->model = model.get();
    CHECK(cg::fun_1402baa60(prim.get()));
    CHECK_EQ(g_alloc_nn, 18);
    CHECK_EQ(g_alloc_ne, 0);
    CHECK_EQ(g_alloc_type, 4);
    CHECK_EQ(prim->u23e0, 6);
    CHECK_EQ(prim->u23e4, 12);
    CHECK_EQ(prim->u23e8, 18);
    CHECK_EQ(prim->u23ec, 6);
    // layer 1: boundary nodes 1 2 3 6 5 4 at z = (10 / 2) * 1; layer 2: all nodes at z = 10
    const double want_z[18] = {0, 0, 0, 0, 0, 0, 5, 5, 5, 5, 5, 5, 10, 10, 10, 10, 10, 10};
    const double want_x[18] = {0, 1, 2, 0, 1, 2, 0, 1, 2, 2, 1, 0, 0, 1, 2, 0, 1, 2};
    for (int i = 0; i < 18; ++i) {
        CHECK_BITS(g_nodes[static_cast<std::size_t>(i * 3)], want_x[i]);
        CHECK_BITS(g_nodes[static_cast<std::size_t>(i * 3 + 2)], want_z[i]);
    }
    // the first element is counter-clockwise: flip, so a/c swapped except on the top
    const std::int32_t want[16][4] = {
        {5, 2, 1, 4}, {6, 3, 2, 5},
        {8, 7, 1, 2}, {9, 8, 2, 3}, {10, 9, 3, 6}, {11, 10, 6, 5}, {12, 11, 5, 4}, {7, 12, 4, 1},
        {14, 13, 7, 8}, {15, 14, 8, 9}, {18, 15, 9, 10}, {17, 18, 10, 11}, {16, 17, 11, 12},
        {13, 16, 12, 7},
        {13, 14, 17, 16}, {14, 15, 18, 17}};
    const std::int32_t* faces = at<std::int32_t*>(obj.o(), 0xbf8);
    CHECK_EQ(at<std::int32_t>(obj.o(), 0xbd8), 1);
    CHECK_EQ(at<std::int32_t>(obj.o(), 0xbf0), 16);
    for (int i = 0; i < 16; ++i)
        for (int k = 0; k < 4; ++k) CHECK_EQ(faces[i * 4 + k], want[i][k]);
    linked::Cls_140258580* w = obj.wpaxis();
    CHECK_BITS(at<double>(w, 0x18), 0.0);
    CHECK_BITS(at<double>(w, 0x30), 10.0);
}

int main() { return testkit::run_all(); }
