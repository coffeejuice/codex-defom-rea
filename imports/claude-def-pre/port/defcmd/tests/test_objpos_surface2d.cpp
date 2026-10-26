// ctest-labels: p67-1 objpos
// The 2D branch of 1401fc730 (defcmd/objpos_centre.hpp: 1401fcc50, 1401fcd50, 1401fd240) and the
// OBJ_MESH2D boundary topology under 1401fd240 (defmodel/obj_mesh2d/topology.hpp).
//
// Seed: tests/commands/CNVT3D/data/sweep_offaxis.KEY (object 2: six nodes, two quads, the mesh
// of the cnvt3d_sweep_* oracle cases), read through the KEY loop into a per-process copy.
// Expected values were derived by hand from the listings of 1403deee0 / 1403df970 / 1403e0070 /
// 1403e0f20 (deform-rea-work/out/w2/p67-1-2d/lst.txt); the end-to-end check against the original
// is the CNVT3D sweep oracle (test_cnvt3d_sweep_e2e on a branch with the CNVT3D packets).
//
// The *_spike cases (ported from census packet C07's test_objpos_c07.cpp) read the corpus seed
// 2D/SI/Forging/SPIKE_SI.KEY (skipped when the corpus is missing): RZ 1: node 1 = (0, 0), node 2 =
// (1.9743107, 0), node 3 = (3.9439832, 0), node 333 = (19.526181, 57.15); ELMCON 1: 289 quads,
// none repeats a node.
#define _CRT_SECURE_NO_WARNINGS
#include "../../defkey/tests/key_test_util.hpp"

#include <cstddef>
#include <cstdio>
#include <cstring>
#include <filesystem>
#include <string>

#include <defcmd/objpos.hpp>
#include <defcmd/objpos_centre.hpp>
#include <defkey/group1.hpp>
#include <defkey/key_reader.hpp>
#include <defkey/kw_g2obj.hpp>
#include <defkey/kw_g3/kw_g3.hpp>
#include <defkey/kw_g4.hpp>
#include <defkey/kw_g57.hpp>
#include <defmodel/geo_kw/geo_kw.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/def_edge.hpp>
#include <defmodel/layout/def_face.hpp>
#include <defmodel/layout/def_loop.hpp>
#include <defmodel/layout/def_vertex.hpp>
#include <defmodel/layout/geo2d_and_mesh2d.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/layout/obj_geo2d.hpp>
#include <defmodel/layout/obj_mesh2d.hpp>
#include <defmodel/llobj/object_list.hpp>
#include <defmodel/mesh2d/geo2d_mesh_ops.hpp>
#include <defmodel/obj_mesh2d/mesh_access.hpp>
#include <defmodel/obj_mesh2d/topology.hpp>
#include <testkit/check.hpp>

namespace fs = std::filesystem;
using namespace def::model;
namespace op = def::cmd::objpos;
namespace gk = def::model::geo_kw;

namespace {

template <class T, class C>
T& at(C* base, std::size_t off)
{
    return *reinterpret_cast<T*>(reinterpret_cast<unsigned char*>(base) + off);
}

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

DB_STEP_DATA* load_sweep_seed()
{
    const fs::path src = testkit::repo_root() / "tests" / "commands" / "CNVT3D" / "data" / "sweep_offaxis.KEY";
    const fs::path tmp = fs::temp_directory_path() / keytest::tmp_name("surface2d_seed.key");
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

GEO2D_AND_MESH2D* geometry0(LLOBJ_DATA* o)
{
    gk::ensure_geo_list(*o);
    if (gk::geo_list_count(gk::geo_list(*o)) == 0) gk::geo_list_append_default(gk::geo_list(*o));
    return gk::geometry(*o, 0);
}

OBJ_GEO2D* curve_of(GEO2D_AND_MESH2D* g, int i) { return static_cast<OBJ_GEO2D*>(g->edge_at(i)->get_geo()); }

// a free OBJ_MESH2D with the six nodes / two quads of the seed and the given material groups
OBJ_MESH2D* two_quads(std::int32_t g0, std::int32_t g1)
{
    auto* m = new OBJ_MESH2D();
    const std::int32_t nn = 6, ne = 2, four = 4;
    fun_1403de680(m, &nn);
    fun_1403de6e0(m, &ne, &four);
    const double xy[12] = {10, 0, 20, 0, 30, 0, 10, 10, 20, 10, 30, 10};
    const std::int32_t conn[8] = {1, 2, 5, 4, 2, 3, 6, 5};
    std::memcpy(fun_140220440(m), xy, sizeof xy);
    std::memcpy(fun_140220460(m), conn, sizeof conn);
    fun_140220480(m)[0] = g0;
    fun_140220480(m)[1] = g1;
    return m;
}

const char* kCorpus = "C:/Users/alext/Projects/DEFORM_corpus/key/";

DB_STEP_DATA* load_corpus(const char* rel)
{
    const fs::path src = fs::path(kCorpus) / rel;
    if (!fs::exists(src)) {
        std::printf("corpus file %s missing: skipped\n", src.string().c_str());
        return nullptr;
    }
    const fs::path tmp = fs::temp_directory_path() / keytest::tmp_name("surface2d_spike.key");
    fs::copy_file(src, tmp, fs::copy_options::overwrite_existing);
    auto* m = new DB_STEP_DATA(2);  // leaked on purpose (the dtor is not under test)
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

// test double for 140198230 (geometry 0 gets exactly n curves): grows only, as the seeds need
int g_set_count_calls = 0;
void grow_curves(LLOBJ_DATA* o, std::int32_t n)
{
    ++g_set_count_calls;
    CHECK(gk::curve_count(*o) <= n);
    while (gk::curve_count(*o) < n) gk::geometry(*o, 0)->add_edge_curve();
}

struct BoundaryRec {          // 0x30-byte record at obj+0xbe0
    std::int32_t n;           // 0x00
    std::int32_t pad;
    std::int32_t* nodes;      // 0x08
    std::byte rest[0x20];
};
static_assert(sizeof(BoundaryRec) == 0x30);

BoundaryRec* records(LLOBJ_DATA* o) { return reinterpret_cast<BoundaryRec*>(reinterpret_cast<unsigned char*>(o) + 0xbe0); }

// records: no node array, n = 0, and nodes 1 2 3 400 (400 > 333 is clamped to 333)
struct ThreeRecords {
    LLOBJ_DATA* o;
    BoundaryRec saved[3];
    std::int32_t saved_count;
    std::int32_t loop[4] = {1, 2, 3, 400};
    std::int32_t other[1] = {5};
    explicit ThreeRecords(LLOBJ_DATA* obj) : o(obj)
    {
        BoundaryRec* rec = records(o);
        for (int i = 0; i < 3; ++i) saved[i] = rec[i];
        saved_count = at<std::int32_t>(o, 0xbd8);
        rec[0].n = 4;
        rec[0].nodes = nullptr;
        rec[1].n = 0;
        rec[1].nodes = other;
        rec[2].n = 4;
        rec[2].nodes = loop;
        at<std::int32_t>(o, 0xbd8) = 3;
    }
    ~ThreeRecords()
    {
        BoundaryRec* rec = records(o);
        for (int i = 0; i < 3; ++i) rec[i] = saved[i];
        at<std::int32_t>(o, 0xbd8) = saved_count;
    }
};

// curve 2 = nodes 1 2 3 333 as (x, y, 0); four points, the first node is not repeated
void check_spike_curve2(LLOBJ_DATA* o)
{
    OBJ_GEO2D* c = gk::curve(*o, 2);
    REQUIRE(c != nullptr);
    CHECK_EQ(c->curve_type, 1);
    CHECK_EQ(c->n_points, 4);
    REQUIRE(c->points != nullptr);
    const double want[12] = {0.0, 0.0, 0.0, 1.9743107, 0.0, 0.0, 3.9439832, 0.0, 0.0, 19.526181, 57.15, 0.0};
    for (int k = 0; k < 12; ++k) CHECK_EQ(c->points[k], want[k]);
}

void check_xy(OBJ_GEO2D* c, int p, double x, double y)
{
    CHECK_EQ(c->points[p * 3], x);
    CHECK_EQ(c->points[p * 3 + 1], y);
    CHECK_EQ(c->points[p * 3 + 2], 0.0);
}

} // namespace

TEST_CASE(fd240_one_region_one_closed_curve)
{
    DB_STEP_DATA* m = load_sweep_seed();
    LLOBJ_DATA* o = object_list_find(&m->objects, 2);
    REQUIRE(o != nullptr);
    REQUIRE(at<std::int32_t>(o, 0x880) == 6);
    REQUIRE(at<std::int32_t>(o, 0x884) == 2);
    CHECK(!op::fun_1401fcc50(o));
    GEO2D_AND_MESH2D* g = geometry0(o);
    REQUIRE(g != nullptr);
    char one = 1;
    op::fun_1401fc730(o, &one);  // 2D, no repeated node -> 1401fd240
    CHECK_EQ(g->faces.size(), 1u);
    CHECK_EQ(g->loops.size(), 1u);
    CHECK_EQ(g->edges.size(), 1u);
    CHECK_EQ(g->vertices.size(), 1u);
    // the boundary walked from side (1,2) by stack: 1 2 3 6 5 4 1
    OBJ_GEO2D* c = curve_of(g, 0);
    CHECK_EQ(c->curve_type, 1);
    REQUIRE(c->n_points == 7);
    check_xy(c, 0, 10, 0);
    check_xy(c, 1, 20, 0);
    check_xy(c, 2, 30, 0);
    check_xy(c, 3, 30, 10);
    check_xy(c, 4, 20, 10);
    check_xy(c, 5, 10, 10);
    check_xy(c, 6, 10, 0);
    DEF_EDGE* e = g->edge_at(0);
    CHECK_EQ(e->get_id(), 1);
    CHECK(e->vertices[0] != nullptr);
    CHECK(e->vertices[0] == e->vertices[1]);
    auto* v = static_cast<DEF_VERTEX*>(e->vertices[0]);
    CHECK_EQ(v->xy[0], 10.0);
    CHECK_EQ(v->xy[1], 0.0);
    CHECK_EQ(v->edge_count(), 1);
    CHECK_EQ(e->loop_count(), 1);
    std::int32_t zero = 0;
    DEF_FACE* f = geo2d_face_at(g, &zero);
    REQUIRE(f != nullptr);
    CHECK_EQ(f->id, 1);
    CHECK_EQ(f->u40_value(), at<std::int32_t*>(o, 0x9f8)[0]);  // the material group
    DEF_LOOP* l = geo2d_loop_at(g, &zero);
    REQUIRE(l != nullptr);
    CHECK_EQ(l->edge_count(), 1);
    CHECK_EQ(l->face_count(), 1);
    CHECK_EQ(g->ub8, 0);  // 1402847d0(g, 0)
}

TEST_CASE(topology_two_material_groups)
{
    OBJ_MESH2D* mesh = two_quads(1, 2);
    fun_1403dedd0(mesh);
    mesh2d::RcHandle<mesh2d::Elem18_1403e2450> sides(mesh->h1590);
    mesh2d::RcHandle<mesh2d::Face18> faces(mesh->h1598);
    REQUIRE(sides.size() == 7);
    CHECK_EQ(faces.at(0).region, 0);
    CHECK_EQ(faces.at(1).region, 1);
    CHECK_EQ(sides.at(1).kind, 2);  // side (2,5) between the two groups
    CHECK_EQ(sides.at(0).kind, 1);
    GEO2D_AND_MESH2D g;
    fun_1403e0f20(mesh, &g);
    CHECK_EQ(g.faces.size(), 2u);
    CHECK_EQ(g.loops.size(), 2u);
    CHECK_EQ(g.edges.size(), 3u);
    CHECK_EQ(g.vertices.size(), 2u);
    // curve 0: region 0's outer sides, grown at the front from node 1: 5 4 1 2
    OBJ_GEO2D* c0 = curve_of(&g, 0);
    REQUIRE(c0->n_points == 4);
    check_xy(c0, 0, 20, 10);
    check_xy(c0, 1, 10, 10);
    check_xy(c0, 2, 10, 0);
    check_xy(c0, 3, 20, 0);
    // curve 1: the shared side 2 5
    OBJ_GEO2D* c1 = curve_of(&g, 1);
    REQUIRE(c1->n_points == 2);
    check_xy(c1, 0, 20, 0);
    check_xy(c1, 1, 20, 10);
    // curve 2: region 1's outer sides 2 3 6 5
    OBJ_GEO2D* c2 = curve_of(&g, 2);
    REQUIRE(c2->n_points == 4);
    check_xy(c2, 0, 20, 0);
    check_xy(c2, 3, 20, 10);
    std::int32_t i0 = 0, i1 = 1;
    CHECK_EQ(geo2d_loop_at(&g, &i0)->edge_count(), 2);
    CHECK_EQ(geo2d_loop_at(&g, &i1)->edge_count(), 2);
    CHECK_EQ(geo2d_face_at(&g, &i0)->u40_value(), 1);
    CHECK_EQ(geo2d_face_at(&g, &i1)->u40_value(), 2);
    CHECK_EQ(g.edge_at(1)->loop_count(), 2);  // the shared curve is on both loops
    fun_1403dee60(mesh);
    CHECK(mesh->h1590->data == nullptr);
    delete mesh;
}

TEST_CASE(fcc50_repeated_node_and_fcd50_boundary_records)
{
    DB_STEP_DATA* m = load_sweep_seed();
    LLOBJ_DATA* o = object_list_find(&m->objects, 2);
    REQUIRE(o != nullptr);
    std::int32_t* conn = at<std::int32_t*>(o, 0x9e8);
    const std::int32_t keep = conn[7];
    conn[7] = conn[6];  // element 2 as a triangle 2 3 6 6
    CHECK(op::fun_1401fcc50(o));
    conn[7] = keep;
    CHECK(!op::fun_1401fcc50(o));

    // 1401fcd50 (2D): one boundary record 1 2 3 6 5 4 -> the points of edge 0 of geometry 0
    GEO2D_AND_MESH2D* g = geometry0(o);
    REQUIRE(g != nullptr);
    op::surface2d_ext().fun_140198230 = grow_curves;
    static std::int32_t list[6] = {1, 2, 3, 6, 5, 4};
    const std::int32_t saved_count = at<std::int32_t>(o, 0xbd8);
    unsigned char saved_rec[0x30];
    std::memcpy(saved_rec, reinterpret_cast<unsigned char*>(o) + 0xbe0, 0x30);
    at<std::int32_t>(o, 0xbd8) = 1;
    at<std::int32_t>(o, 0xbe0) = 6;
    at<std::int32_t*>(o, 0xbe8) = list;
    char one = 1;
    op::fun_1401fcd50(o, &one);
    REQUIRE(g->edges.size() >= 1u);
    OBJ_GEO2D* c = curve_of(g, 0);
    CHECK_EQ(c->curve_type, 1);
    REQUIRE(c->n_points == 6);
    check_xy(c, 0, 10, 0);
    check_xy(c, 2, 30, 0);
    check_xy(c, 3, 30, 10);
    check_xy(c, 5, 10, 10);
    std::memcpy(reinterpret_cast<unsigned char*>(o) + 0xbe0, saved_rec, 0x30);
    at<std::int32_t>(o, 0xbd8) = saved_count;
    op::reset_surface2d_ext();
}

TEST_CASE(fcc50_repeated_node_spike)
{
    keytest::legacy_printf();
    DB_STEP_DATA* m = load_corpus("2D/SI/Forging/SPIKE_SI.KEY");
    if (m == nullptr) return;
    LLOBJ_DATA* o = object_list_find(&m->objects, 1);
    REQUIRE(o != nullptr);
    CHECK_EQ(at<std::int32_t>(o, 0x884), 289);
    CHECK(at<std::int32_t>(o, 0x87c) != 0);
    CHECK(!op::fun_1401fcc50(o));  // ELMCON 1: all quads
    std::int32_t* el = at<std::int32_t*>(o, 0x9e8);
    const std::int32_t keep = el[288 * 4 + 3];
    el[288 * 4 + 3] = el[288 * 4 + 0];  // last element, nodes 1 and 4
    CHECK(op::fun_1401fcc50(o));
    el[288 * 4 + 3] = keep;
    CHECK_EQ(el[1], 115);  // ELMCON 1, element 1: 128 115 124 129
    el[1] = el[2];         // element 1, nodes 2 and 3
    CHECK(op::fun_1401fcc50(o));
    el[1] = 115;
    CHECK(!op::fun_1401fcc50(o));
    const std::int32_t flag = at<std::int32_t>(o, 0x87c);
    at<std::int32_t>(o, 0x87c) = 0;  // no mesh flag: false whatever the connectivity
    el[1] = el[2];
    CHECK(!op::fun_1401fcc50(o));
    el[1] = 115;
    at<std::int32_t>(o, 0x87c) = flag;
}

TEST_CASE(fcd50_boundary_records_spike)
{
    keytest::legacy_printf();
    DB_STEP_DATA* m = load_corpus("2D/SI/Forging/SPIKE_SI.KEY");
    if (m == nullptr) return;
    LLOBJ_DATA* o = object_list_find(&m->objects, 1);
    REQUIRE(o != nullptr);
    REQUIRE(gk::geometry(*o, 0) != nullptr);
    CHECK_EQ(at<std::int32_t>(o, 0x880), 333);
    const std::int32_t had = gk::curve_count(*o);
    ThreeRecords recs(o);

    g_set_count_calls = 0;
    op::surface2d_ext().fun_140198230 = grow_curves;
    char compact = 1;  // unused in 2D
    op::fun_1401fcd50(o, &compact);
    op::reset_surface2d_ext();
    CHECK_EQ(g_set_count_calls, 1);  // 140163340 -> 140198230 once, with +0xbd8
    CHECK(had <= 3);
    CHECK_EQ(gk::curve_count(*o), 3);
    check_spike_curve2(o);
    // records 0 and 1 are skipped: their curves keep what they had (new curves: no points)
    if (had == 0) {
        CHECK_EQ(gk::curve(*o, 0)->n_points, 0);
        CHECK_EQ(gk::curve(*o, 1)->n_points, 0);
    }
}

// 1401fc730 on a 2D mesh with a repeated node takes 1401fcd50 (the boundary-record curves), not
// 1401fd240 (which would rebuild geometry 0 from the mesh topology).
TEST_CASE(fc730_repeated_node_reaches_fcd50_spike)
{
    keytest::legacy_printf();
    DB_STEP_DATA* m = load_corpus("2D/SI/Forging/SPIKE_SI.KEY");
    if (m == nullptr) return;
    LLOBJ_DATA* o = object_list_find(&m->objects, 1);
    REQUIRE(o != nullptr);
    REQUIRE(gk::geometry(*o, 0) != nullptr);
    std::int32_t* el = at<std::int32_t*>(o, 0x9e8);
    const std::int32_t keep = el[1];
    el[1] = el[2];  // element 1 as a triangle
    REQUIRE(op::fun_1401fcc50(o));
    const std::size_t faces_before = gk::geometry(*o, 0)->faces.size();
    {
        ThreeRecords recs(o);
        g_set_count_calls = 0;
        op::surface2d_ext().fun_140198230 = grow_curves;
        char compact = 1;
        op::fun_1401fc730(o, &compact);
        op::reset_surface2d_ext();
        CHECK_EQ(g_set_count_calls, 1);
        CHECK_EQ(gk::curve_count(*o), 3);
        CHECK_EQ(gk::geometry(*o, 0)->faces.size(), faces_before);  // 1401fd240 not run
        check_spike_curve2(o);
    }
    el[1] = keep;
}

int main() { return testkit::run_all(); }
