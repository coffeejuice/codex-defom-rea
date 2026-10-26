// GEO2D_AND_MESH2D helpers (1402838d0..140284b80). The geometry object and the topology
// objects are raw buffers of the recovered sizes with the lists placed at their offsets; the
// other packets' calls are fakes installed in mesh2d::Ext. Expected values follow the decompile.
// ctest-labels: p1-08 mesh2d
#include <defmodel/layout/geo2d_and_mesh2d.hpp>
#include <defmodel/layout/obj_mesh2d.hpp>
#include <defmodel/mesh2d/geo2d_mesh_ops.hpp>
#include <testkit/check.hpp>

#include <cstdlib>
#include <new>
#include <string>
#include <vector>

using namespace def::model;
using mesh2d::field;

namespace {

std::vector<void*> g_blocks;
void* raw(std::size_t n)
{
    void* p = std::calloc(1, n);
    g_blocks.push_back(p);
    return p;
}

struct Geo {
    alignas(8) std::byte b[0xc8] = {};
    Geo()
    {
        new (b + 0x08) FaceList;
        new (b + 0x10) LoopList;
        new (b + 0x18) EdgeList;
        new (b + 0x20) VertexList;
    }
    ~Geo()
    {
        geo2d_faces(g()).~FaceList();
        geo2d_loops(g()).~LoopList();
        geo2d_edges(g()).~EdgeList();
        geo2d_vertices(g()).~VertexList();
    }
    GEO2D_AND_MESH2D* g() { return reinterpret_cast<GEO2D_AND_MESH2D*>(b); }
};

std::vector<std::pair<DEF_EDGE*, std::uint8_t>> g_flag_calls;

void install()
{
    auto& e = mesh2d::ext();
    e.new_face = [] { return static_cast<DEF_FACE*>(raw(0x48)); };
    e.new_loop = [] { return static_cast<DEF_LOOP*>(raw(0x20)); };
    e.new_vertex = [] { return static_cast<DEF_VERTEX*>(raw(0x28)); };
    e.face_cast = [](IFace* p) { return reinterpret_cast<DEF_FACE*>(p); };
    e.loop_cast = [](ILoop* p) { return reinterpret_cast<DEF_LOOP*>(p); };
    e.vertex_cast = [](IVertex* p) { return reinterpret_cast<DEF_VERTEX*>(p); };
    e.edge_at = [](GEO2D_AND_MESH2D* g, std::int32_t i) -> DEF_EDGE* {
        // 1402800a0: bounds-checked at + cast
        std::int32_t n = static_cast<std::int32_t>(geo2d_edges(g).size());
        if (i < 0 || i >= n) return nullptr;
        return reinterpret_cast<DEF_EDGE*>(geo2d_edges(g).mutable_at(static_cast<std::size_t>(i)));
    };
    e.edge_set_number = [](DEF_EDGE* d, const std::int32_t* n) { field<std::int32_t>(d, 0x30) = *n; };
    e.edge_set_curve_flag = [](DEF_EDGE* d, std::uint8_t v) { g_flag_calls.push_back({d, v}); };
    e.geo_slot11 = [](OBJ_GEO_BASE* o) { return field<std::int32_t>(o, 0); };
}

} // namespace

TEST_CASE(add_and_number)
{
    install();
    Geo geo;
    DEF_FACE* f1 = geo2d_add_face(geo.g());
    DEF_FACE* f2 = geo2d_add_face(geo.g());
    CHECK_EQ(field<std::int32_t>(f1, 0x44), 1);
    CHECK_EQ(field<std::int32_t>(f2, 0x44), 2);
    DEF_LOOP* l1 = geo2d_add_loop(geo.g());
    CHECK_EQ(field<std::int32_t>(l1, 0x18), 1);
    DEF_VERTEX* v1 = geo2d_add_vertex(geo.g());
    DEF_VERTEX* v2 = geo2d_add_vertex(geo.g());
    DEF_VERTEX* v3 = geo2d_add_vertex(geo.g());
    CHECK_EQ(field<std::int32_t>(v3, 0x24), 3);

    std::int32_t i = 1;
    CHECK(geo2d_face_at(geo.g(), &i) == f2);
    CHECK(geo2d_vertex_at(geo.g(), &i) == v2);
    i = 0;
    CHECK(geo2d_loop_at(geo.g(), &i) == l1);
    i = -1;
    CHECK(geo2d_face_at(geo.g(), &i) == nullptr);
    i = 3;
    CHECK(geo2d_vertex_at(geo.g(), &i) == nullptr);
    i = 1;
    CHECK(geo2d_loop_at(geo.g(), &i) == nullptr);

    field<std::int32_t>(v1, 0x24) = 77;
    field<std::int32_t>(f1, 0x44) = 77;
    geo2d_renumber_vertices(geo.g());
    geo2d_renumber_faces(geo.g());
    geo2d_renumber_loops(geo.g());
    CHECK_EQ(field<std::int32_t>(v1, 0x24), 1);
    CHECK_EQ(field<std::int32_t>(v2, 0x24), 2);
    CHECK_EQ(field<std::int32_t>(f1, 0x44), 1);
    CHECK_EQ(field<std::int32_t>(l1, 0x18), 1);
}

TEST_CASE(edges_renumber_and_b8_flag)
{
    install();
    Geo geo;
    void* e1 = raw(0x38);
    void* e2 = raw(0x38);
    geo2d_edges(geo.g()).push_back(static_cast<IEdge*>(e1));
    geo2d_edges(geo.g()).push_back(static_cast<IEdge*>(e2));
    geo2d_renumber_edges(geo.g());
    CHECK_EQ(field<std::int32_t>(e1, 0x30), 1);
    CHECK_EQ(field<std::int32_t>(e2, 0x30), 2);

    g_flag_calls.clear();
    char one = 1;
    geo2d_set_b8(geo.g(), &one);
    CHECK_EQ(field<char>(geo.b, 0xb8), 1);
    CHECK_EQ(g_flag_calls.size(), 0u);
    char zero = 0;
    geo2d_set_b8(geo.g(), &zero);
    CHECK_EQ(field<char>(geo.b, 0xb8), 0);
    REQUIRE(g_flag_calls.size() == 2u);
    CHECK(g_flag_calls[0].first == static_cast<DEF_EDGE*>(e1));
    CHECK_EQ(g_flag_calls[1].second, 0);
}

TEST_CASE(accessors)
{
    install();
    Geo geo;
    geo2d_set_30(geo.g(), 5);
    CHECK_EQ(geo2d_get_30(geo.g()), 5);
    CHECK(geo2d_block_38(geo.g()) == geo.b + 0x38);
    CHECK_EQ(std::string(geo.g()->GEO2D_AND_MESH2D::class_name()), std::string("GEO2D_AND_MESH2D")); // 1402848f0

    DEF_EDGE* e = static_cast<DEF_EDGE*>(raw(0x38));
    edge_set_34(e, 1);
    edge_set_35(e, 2);
    CHECK_EQ(edge_get_34(e), 1);
    CHECK_EQ(edge_get_35(e), 2);
    edge_clear_34_35(e);
    CHECK_EQ(edge_get_34(e) + edge_get_35(e), 0);
    OBJ_GEO_BASE* curve = static_cast<OBJ_GEO_BASE*>(raw(0x30));
    field<OBJ_GEO_BASE*>(e, 0x08) = curve;
    CHECK(edge_curve(e) == curve);
    CHECK(geo_if_slot11_zero(curve) == curve);
    field<std::int32_t>(curve, 0) = 3; // fake slot 11 returns nonzero
    CHECK(geo_if_slot11_zero(curve) == nullptr);

    DEF_LOOP* l = static_cast<DEF_LOOP*>(raw(0x20));
    loop_set_1c(l, 9);
    CHECK_EQ(loop_get_1c(l), 9);
    DEF_VERTEX* v = static_cast<DEF_VERTEX*>(raw(0x28));
    vertex_set_20(v, 4);
    CHECK_EQ(vertex_get_20(v), 4);

    OBJ_MESH2D* m = static_cast<OBJ_MESH2D*>(raw(0x1700));
    field<std::int32_t>(m, 0x134) = 11;
    field<std::int32_t>(m, 0x230) = 12;
    CHECK_EQ(mesh2d_count_08(m), 11);
    CHECK_EQ(mesh2d_count_148(m), 12);
    CHECK_EQ(std::string(m->OBJ_MESH2D::class_name()), std::string("OBJ_MESH2D")); // 1402f0610
}

int main()
{
    int r = testkit::run_all();
    mesh2d::reset_ext();
    for (void* p : g_blocks) std::free(p);
    return r;
}
