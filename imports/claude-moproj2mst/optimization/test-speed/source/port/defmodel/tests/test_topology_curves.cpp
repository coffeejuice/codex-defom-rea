// ctest-labels: p67-1 topo
// Tier 1: the curve half of the OBJ_MESH2D boundary topology (census C08, root 1403df940):
// 1403df970 (curves), 1403dfef0, 1403dff90, 1403e0cd0 (topology_curves.cpp). The meshes are set
// up through 1403de680 / 1403de6e0 and the sides / regions built by 1403deee0. Expected values
// were derived by hand from the decompile and listing of 1403df970 (the walk is a stack: the
// start side's n1 is popped first and the curve grows at the back until the walk returns to the
// curve's first node, after which it grows at the front through 1403e2780), with the side order
// of 1403df640 (per element k = 0..3: nodes (k, k+1 mod 4), shared sides once).
#include <defmodel/layout/obj_mesh2d.hpp>
#include <defmodel/obj_mesh2d/mesh_access.hpp>
#include <defmodel/obj_mesh2d/rc_array.hpp>
#include <defmodel/obj_mesh2d/topology.hpp>
#include <testkit/check.hpp>

#include <cstdint>
#include <cstring>
#include <vector>

using namespace def::model;
using mesh2d::Elem10_1403e2610;
using mesh2d::Elem18_1403e2450;
using mesh2d::Elem20_1403e25a0;
using mesh2d::RcHandle;

namespace {

// a free OBJ_MESH2D: nodes (x, y), quads (1-based nodes), material groups
OBJ_MESH2D* make_mesh(const std::vector<double>& xy, const std::vector<std::int32_t>& conn,
                      const std::vector<std::int32_t>& mat)
{
    auto* m = new OBJ_MESH2D();
    const std::int32_t nn = static_cast<std::int32_t>(xy.size() / 2);
    const std::int32_t ne = static_cast<std::int32_t>(mat.size());
    const std::int32_t four = 4;
    fun_1403de680(m, &nn);
    fun_1403de6e0(m, &ne, &four);
    std::memcpy(fun_140220440(m), xy.data(), xy.size() * sizeof(double));
    std::memcpy(fun_140220460(m), conn.data(), conn.size() * sizeof(std::int32_t));
    for (std::int32_t e = 0; e < ne; ++e) fun_140220480(m)[e] = mat[static_cast<std::size_t>(e)];
    return m;
}

// node i (0-based) at (10 + 10 * (i % 3), 10 * (i / 3)); quads 1 2 5 4 and 2 3 6 5
OBJ_MESH2D* two_quads(std::int32_t g0, std::int32_t g1)
{
    return make_mesh({10, 0, 20, 0, 30, 0, 10, 10, 20, 10, 30, 10}, {1, 2, 5, 4, 2, 3, 6, 5}, {g0, g1});
}

// 3 x 3 quads on a 4 x 4 node grid (node r * 4 + c at (c, r), element r * 3 + c), the centre
// element (4) in group 2, the others in group 1
OBJ_MESH2D* grid_with_centre()
{
    std::vector<double> xy;
    for (int r = 0; r < 4; ++r)
        for (int c = 0; c < 4; ++c) {
            xy.push_back(c);
            xy.push_back(r);
        }
    std::vector<std::int32_t> conn;
    std::vector<std::int32_t> mat;
    for (int r = 0; r < 3; ++r)
        for (int c = 0; c < 3; ++c) {
            const std::int32_t n = r * 4 + c + 1;
            conn.insert(conn.end(), {n, n + 1, n + 5, n + 4});
            mat.push_back(r == 1 && c == 1 ? 2 : 1);
        }
    return make_mesh(xy, conn, mat);
}

std::vector<std::int32_t> nodes_of(Elem20_1403e25a0& c)
{
    return std::vector<std::int32_t>(c.nodes.items().begin(), c.nodes.items().end());
}

std::vector<std::int32_t> ints(const mesh2d::IntVector& v)
{
    std::vector<std::int32_t> out;
    for (std::size_t i = 0; i < v.size(); ++i) out.push_back(v[i]);
    return out;
}

using V = std::vector<std::int32_t>;

} // namespace

TEST_CASE(one_region_gives_one_closed_curve)
{
    OBJ_MESH2D* m = two_quads(1, 1);
    fun_1403deee0(m);
    REQUIRE(fun_1403dfef0(m) == 0);  // side (0, 1) of element 0
    fun_1403df970(m);
    RcHandle<Elem20_1403e25a0> curves(m->h15a8);
    RcHandle<Elem10_1403e2610> vertices(m->h15a0);
    RcHandle<Elem18_1403e2450> sides(m->h1590);
    REQUIRE(curves.size() == 1);
    // from (0, 1) along the back: 1 -> 2 -> 5 -> 4 -> 3 -> 0, then 0 is popped twice
    CHECK(nodes_of(curves.at(0)) == V({0, 1, 2, 5, 4, 3, 0}));
    CHECK_EQ(curves.at(0).v0, 0);
    CHECK_EQ(curves.at(0).v1, 0);
    CHECK(ints(curves.at(0).regions) == V({0}));
    CHECK_EQ(curves.at(0).loops.size(), 0u);
    REQUIRE(vertices.size() == 1);
    CHECK_EQ(vertices.at(0).node, 0);
    CHECK(ints(vertices.at(0).curves) == V({0}));  // added once (1403e2470)
    // every boundary side is on curve 0, the inner side (1, 4) on none
    for (std::int32_t s = 0; s < sides.size(); ++s) CHECK_EQ(sides.at(s).curve, s == 1 ? -1 : 0);
    CHECK_EQ(fun_1403dfef0(m), -1);
    delete m;
}

TEST_CASE(two_groups_give_three_open_curves)
{
    OBJ_MESH2D* m = two_quads(1, 2);
    fun_1403deee0(m);
    RcHandle<Elem18_1403e2450> sides(m->h1590);
    REQUIRE(sides.size() == 7);
    // 1403e0cd0: side 0 (0,1) and side 3 (3,0) both bound region 0 only; side 1 is between the
    // regions (kind 2); side 4 (1,2) bounds region 1
    std::int32_t s0 = 0, s1 = 1, s3 = 3, s4 = 4, s5 = 5;
    CHECK(fun_1403e0cd0(m, &s0, &s3));
    CHECK(!fun_1403e0cd0(m, &s0, &s1));
    CHECK(!fun_1403e0cd0(m, &s0, &s4));
    CHECK(fun_1403e0cd0(m, &s4, &s5));
    CHECK(fun_1403e0cd0(m, &s1, &s1));

    fun_1403df970(m);
    RcHandle<Elem20_1403e25a0> curves(m->h15a8);
    RcHandle<Elem10_1403e2610> vertices(m->h15a0);
    REQUIRE(curves.size() == 3);
    // curve 0 from side 0: nothing alike at node 1, then from node 0 at the front: 3, 4
    CHECK(nodes_of(curves.at(0)) == V({4, 3, 0, 1}));
    CHECK_EQ(curves.at(0).v0, 0);  // node 4 got the first vertex
    CHECK_EQ(curves.at(0).v1, 1);  // node 1
    CHECK(ints(curves.at(0).regions) == V({0}));
    // curve 1: the shared side (1, 4), regions in the order of its elements
    CHECK(nodes_of(curves.at(1)) == V({1, 4}));
    CHECK_EQ(curves.at(1).v0, 1);
    CHECK_EQ(curves.at(1).v1, 0);
    CHECK(ints(curves.at(1).regions) == V({0, 1}));
    // curve 2 from side 4 (1, 2) along the back: 5, 4
    CHECK(nodes_of(curves.at(2)) == V({1, 2, 5, 4}));
    CHECK_EQ(curves.at(2).v0, 1);
    CHECK_EQ(curves.at(2).v1, 0);
    CHECK(ints(curves.at(2).regions) == V({1}));
    REQUIRE(vertices.size() == 2);
    CHECK_EQ(vertices.at(0).node, 4);
    CHECK_EQ(vertices.at(1).node, 1);
    CHECK(ints(vertices.at(0).curves) == V({0, 1, 2}));
    CHECK(ints(vertices.at(1).curves) == V({0, 1, 2}));
    const std::int32_t want_curve[7] = {0, 1, 0, 0, 2, 2, 2};
    for (std::int32_t s = 0; s < 7; ++s) CHECK_EQ(sides.at(s).curve, want_curve[s]);
    // the node records point at their vertices (1403dff90), the others stay -1
    RcHandle<mesh2d::Elem28_1403e2370> nodes(m->h1588);
    const std::int32_t want_vertex[6] = {-1, 1, -1, -1, 0, -1};
    for (std::int32_t n = 0; n < 6; ++n) CHECK_EQ(nodes.at(n).vertex, want_vertex[n]);
    // 1403dff90 on a node that has a vertex returns it and adds none
    std::int32_t n4 = 4, n2 = 2;
    CHECK_EQ(fun_1403dff90(m, &n4), 0);
    CHECK_EQ(vertices.size(), 2);
    CHECK_EQ(fun_1403dff90(m, &n2), 2);  // a new one for node 2
    CHECK_EQ(vertices.size(), 3);
    CHECK_EQ(vertices.at(2).node, 2);
    delete m;
}

TEST_CASE(hole_gives_outer_and_inner_closed_curves)
{
    OBJ_MESH2D* m = grid_with_centre();
    fun_1403deee0(m);
    fun_1403df970(m);
    RcHandle<Elem20_1403e25a0> curves(m->h15a8);
    RcHandle<Elem10_1403e2610> vertices(m->h15a0);
    REQUIRE(curves.size() == 2);
    CHECK(nodes_of(curves.at(0)) == V({0, 1, 2, 3, 7, 11, 15, 14, 13, 12, 8, 4, 0}));
    CHECK_EQ(curves.at(0).v0, 0);
    CHECK_EQ(curves.at(0).v1, 0);
    CHECK(ints(curves.at(0).regions) == V({0}));
    // the inner ring starts at the first kind-2 side, the top of element 1 (6, 5), elements
    // {1, 4}; it is walked 5 -> 9 -> 10 -> 6 on the back
    CHECK(nodes_of(curves.at(1)) == V({6, 5, 9, 10, 6}));
    CHECK_EQ(curves.at(1).v0, 1);
    CHECK_EQ(curves.at(1).v1, 1);
    CHECK(ints(curves.at(1).regions) == V({0, 1}));
    REQUIRE(vertices.size() == 2);
    CHECK_EQ(vertices.at(0).node, 0);
    CHECK_EQ(vertices.at(1).node, 6);
    CHECK(ints(vertices.at(0).curves) == V({0}));
    CHECK(ints(vertices.at(1).curves) == V({1}));
    CHECK_EQ(fun_1403dfef0(m), -1);
    delete m;
}

TEST_CASE(df970_clears_the_curves_but_not_the_vertices)
{
    OBJ_MESH2D* m = two_quads(1, 1);
    fun_1403deee0(m);
    fun_1403df970(m);
    // a second run: every boundary side already has a curve, so no curve is made; the vertex
    // array (+0x15a0) is left as it was
    fun_1403df970(m);
    RcHandle<Elem20_1403e25a0> curves(m->h15a8);
    RcHandle<Elem10_1403e2610> vertices(m->h15a0);
    CHECK_EQ(curves.size(), 0);
    CHECK_EQ(vertices.size(), 1);
    delete m;
}

int main() { return testkit::run_all(); }
