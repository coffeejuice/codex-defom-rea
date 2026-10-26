// ctest-labels: p67-1 objpos
// Census packet C11 (root 1403dedd0): the OBJ_MESH2D topology arrays' MSVC container code
// (defmodel/obj_mesh2d/rc_array.hpp) and the sides / regions pass (1403deee0, 1403df640,
// 1403df8d0 in defmodel/obj_mesh2d/topology.cpp). Hand-made inputs; expected values derived from
// the decompiles (deform-rea-work/out/c1x/dec.c): growth size + 1 + size / 2 (1403e2ae0 /
// 1403e34c0), _Insert_n capacity size + max(n, size) and its two in-place branches (1403e64a0 /
// 1403e6cd0), resize leaving the erased tail constructed (1403e2920 / 1403e2c80), detach on the
// mutating accessors only (1403e28e0 / 1403e2aa0 vs 1403e2a80 / 1403e3bc0 / 1403e3d50).
#include <cstdint>
#include <cstring>

#include <defmodel/layout/obj_mesh2d.hpp>
#include <defmodel/obj_mesh2d/mesh_access.hpp>
#include <defmodel/obj_mesh2d/rc_array.hpp>
#include <defmodel/obj_mesh2d/topology.hpp>
#include <testkit/check.hpp>

using namespace def::model;
using namespace def::model::mesh2d;

namespace {

template <class T>
std::size_t cap(RcArray<T>* b)
{
    return static_cast<std::size_t>(b->u18 - b->data);
}

Face18 face(std::int32_t v) { return Face18{v, v, {v, v, v, v}}; }

// a Face18 block holding 0..n-1 (mat = i) with capacity c
RcArray<Face18>* faces_block(std::int32_t n, std::size_t c)
{
    auto* b = new RcArray<Face18>();
    b->reallocate(c);
    for (std::int32_t i = 0; i < n; ++i) *b->u10++ = face(i);
    return b;
}

std::int32_t mat_at(RcArray<Face18>* b, std::size_t i) { return b->data[i].mat; }

OBJ_MESH2D* two_quads(std::int32_t g0, std::int32_t g1)
{
    auto* m = new OBJ_MESH2D();
    const std::int32_t nn = 6, ne = 2, four = 4;
    fun_1403de680(m, &nn);
    fun_1403de6e0(m, &ne, &four);
    const std::int32_t conn[8] = {1, 2, 5, 4, 2, 3, 6, 5};
    std::memcpy(fun_140220460(m), conn, sizeof conn);
    fun_140220480(m)[0] = g0;
    fun_140220480(m)[1] = g1;
    return m;
}

} // namespace

// 1403e2ae0 / 1403e5b00 / 1403e6aa0: push_back grows to size + 1 + size / 2 exactly
TEST_CASE(c11_sides_push_back_growth)
{
    RcArray<Elem18_1403e2450>* blk = new_rc_array<Elem18_1403e2450>();
    RcHandle<Elem18_1403e2450> sides(blk);
    const std::size_t expect[12] = {1, 2, 4, 4, 7, 7, 7, 11, 11, 11, 11, 17};
    for (std::int32_t i = 0; i < 12; ++i) {
        Elem18_1403e2450 s;
        s.n0 = i;
        s.n1 = i + 1;
        s.curve = -1;
        s.kind = i % 3;
        s.elems.insert(s.elems.mutable_end(), 1, 100 + i);
        sides.push_back(s);
        CHECK_EQ(cap(sides.block()), expect[i]);
        CHECK_EQ(sides.size(), i + 1);
    }
    for (std::int32_t i = 0; i < 12; ++i) {
        CHECK_EQ(sides.at(i).n0, i);
        CHECK_EQ(sides.at(i).kind, i % 3);
        CHECK_EQ(sides.at(i).elems[0], 100 + i);
    }
    release_rc_array(sides.block());
}

// 1403e34c0 / 1403e6060 / 1403e7c50: the same policy for the regions
TEST_CASE(c11_regions_push_back_growth)
{
    RcArray<Elem18_1403e23f0>* blk = new_rc_array<Elem18_1403e23f0>();
    RcHandle<Elem18_1403e23f0> regions(blk);
    const std::size_t expect[5] = {1, 2, 4, 4, 7};
    for (std::int32_t i = 0; i < 5; ++i) {
        Elem18_1403e23f0 r;
        r.mat = 7 * i;
        regions.push_back(r);
        CHECK_EQ(cap(regions.block()), expect[i]);
    }
    CHECK_EQ(regions.at(4).mat, 28);
    CHECK_EQ(regions.at(0).mat, 0);
    release_rc_array(regions.block());
}

// 1403e2920 / 1403e54d0 / 1403e64a0 / 1403e3c40: node resize
TEST_CASE(c11_nodes_resize)
{
    RcArray<Elem28_1403e2370>* blk = new_rc_array<Elem28_1403e2370>();
    RcHandle<Elem28_1403e2370> nodes(blk);
    Elem28_1403e2370 v;
    v.vertex = -1;
    v.u20 = -1;
    v.u24 = -1;
    nodes.resize(3, v);  // grow: 0 + max(3, 0)
    CHECK_EQ(nodes.size(), 3);
    CHECK_EQ(cap(nodes.block()), 3u);
    for (int i = 0; i < 3; ++i) nodes.at(i).vertex = 10 + i;
    nodes.resize(5, v);  // grow: 3 + max(2, 3)
    CHECK_EQ(nodes.size(), 5);
    CHECK_EQ(cap(nodes.block()), 6u);
    CHECK_EQ(nodes.at(2).vertex, 12);
    CHECK_EQ(nodes.at(3).vertex, -1);
    CHECK_EQ(nodes.at(4).u24, -1);
    nodes.at(4).vertex = 44;
    nodes.resize(2, v);  // erase the tail: last moves back, the elements stay
    CHECK_EQ(nodes.size(), 2);
    CHECK_EQ(cap(nodes.block()), 6u);
    CHECK_EQ(nodes.block()->data[4].vertex, 44);
    nodes.resize(2, v);  // no change
    CHECK_EQ(nodes.size(), 2);
    v.vertex = -5;
    nodes.resize(4, v);  // in place, nothing behind the insert point: fill at last
    CHECK_EQ(nodes.size(), 4);
    CHECK_EQ(cap(nodes.block()), 6u);
    CHECK_EQ(nodes.at(1).vertex, 11);
    CHECK_EQ(nodes.at(2).vertex, -5);
    CHECK_EQ(nodes.at(3).vertex, -5);
    CHECK_EQ(nodes.block()->data[4].vertex, 44);
    release_rc_array(nodes.block());
}

// 1403e2c80 / 1403e5660 / 1403e6cd0 / 1403e3dd0: element (plain 0x18) resize
TEST_CASE(c11_faces_resize)
{
    RcArray<Face18>* blk = new_rc_array<Face18>();
    RcHandle<Face18> faces(blk);
    faces.resize(2, face(-1));
    CHECK_EQ(faces.size(), 2);
    CHECK_EQ(cap(faces.block()), 2u);
    CHECK_EQ(faces.at(1).edge[3], -1);
    faces.resize(3, face(9));  // 2 + max(1, 2)
    CHECK_EQ(cap(faces.block()), 4u);
    CHECK_EQ(faces.at(2).region, 9);
    CHECK_EQ(faces.at(0).region, -1);
    faces.resize(0, face(0));
    CHECK_EQ(faces.size(), 0);
    CHECK_EQ(cap(faces.block()), 4u);
    release_rc_array(faces.block());
}

// 1403e6cd0 in place, n < elements after pos: copy the last n up, copy_backward (1403e8210), fill
// (1403e8280); n >= after: fill n - after at last, move [pos, last) up, fill [pos, old last)
TEST_CASE(c11_insert_n_in_place_branches)
{
    RcArray<Face18>* b = faces_block(4, 10);  // 0 1 2 3
    b->insert_n(b->data + 1, 2, face(-7));
    REQUIRE(b->size() == 6u);
    const std::int32_t e1[6] = {0, -7, -7, 1, 2, 3};
    for (int i = 0; i < 6; ++i) CHECK_EQ(mat_at(b, i), e1[i]);
    CHECK_EQ(cap(b), 10u);
    b->insert_n(b->data + 4, 3, face(-8));  // after = 2 <= 3
    REQUIRE(b->size() == 9u);
    const std::int32_t e2[9] = {0, -7, -7, 1, -8, -8, -8, 2, 3};
    for (int i = 0; i < 9; ++i) CHECK_EQ(mat_at(b, i), e2[i]);
    b->insert_n(b->data + 9, 1, face(5));  // at last, fills the last free slot
    CHECK_EQ(b->size(), 10u);
    CHECK_EQ(cap(b), 10u);
    CHECK_EQ(mat_at(b, 9), 5);
    // full: grow to 10 + max(2, 10) = 20, the new values between the two halves
    b->insert_n(b->data + 1, 2, face(-9));
    CHECK_EQ(b->size(), 12u);
    CHECK_EQ(cap(b), 20u);
    const std::int32_t e3[12] = {0, -9, -9, -7, -7, 1, -8, -8, -8, 2, 3, 5};
    for (int i = 0; i < 12; ++i) CHECK_EQ(mat_at(b, i), e3[i]);
    // grow by more than size: 12 + max(30, 12)
    b->insert_n(b->data + 12, 30, face(1));
    CHECK_EQ(b->size(), 42u);
    CHECK_EQ(cap(b), 42u);
    delete b;
}

// The same template for the 0x28 nodes (1403e64a0 / 1403e80f0 / 1403e8150)
TEST_CASE(c11_insert_n_nodes_in_place)
{
    auto* b = new RcArray<Elem28_1403e2370>();
    b->reallocate(8);
    for (int i = 0; i < 5; ++i) {
        b->u10->vertex = i;
        b->u10->edges.insert(b->u10->edges.mutable_end(), 1, 50 + i);
        ++b->u10;
    }
    Elem28_1403e2370 v;
    v.vertex = -1;
    b->insert_n(b->data + 2, 1, v);  // 0 1 -1 2 3 4
    const std::int32_t e[6] = {0, 1, -1, 2, 3, 4};
    for (int i = 0; i < 6; ++i) CHECK_EQ(b->data[i].vertex, e[i]);
    CHECK_EQ(b->data[3].edges[0], 52);  // the vectors are moved along by assignment
    CHECK_EQ(b->data[5].edges[0], 54);
    CHECK_EQ(b->data[2].edges.size(), 0u);
    delete b;
}

// detach: the mutating accessors copy a shared block (refs > 1), size does not
TEST_CASE(c11_detach_on_shared_block)
{
    RcArray<Elem18_1403e2450>* a = new_rc_array<Elem18_1403e2450>();
    RcHandle<Elem18_1403e2450> h(a);
    Elem18_1403e2450 s;
    s.n0 = 3;
    h.push_back(s);
    h.push_back(s);
    h.push_back(s);  // capacity 4, size 3
    RcArray<Elem18_1403e2450>* b = a;  // second handle on the same block
    ++a->refs;
    RcHandle<Elem18_1403e2450> h2(b);
    CHECK_EQ(h2.size(), 3);
    CHECK(h2.block() == h.block());
    h2.at(1).n0 = 8;  // 1403e2aa0: detach first
    CHECK(h2.block() != h.block());
    CHECK_EQ(h.block()->refs, 1);
    CHECK_EQ(h2.block()->refs, 1);
    CHECK_EQ(h.at(1).n0, 3);
    CHECK_EQ(h2.at(1).n0, 8);
    CHECK_EQ(cap(h2.block()), 3u);  // the clone has exactly size elements
    h2.push_back(s);                // full: 3 + 1 + 1
    CHECK_EQ(cap(h2.block()), 5u);
    CHECK_EQ(h.size(), 3);
    release_rc_array(h.block());
    release_rc_array(h2.block());
}

// 1403deee0 / 1403df640 / 1403df8d0 on two quads of one material group
TEST_CASE(c11_sides_and_one_region)
{
    OBJ_MESH2D* mesh = two_quads(3, 3);
    fun_1403deee0(mesh);
    RcHandle<Elem28_1403e2370> nodes(mesh->h1588);
    RcHandle<Elem18_1403e2450> sides(mesh->h1590);
    RcHandle<Face18> faces(mesh->h1598);
    RcHandle<Elem18_1403e23f0> regions(mesh->h15b8);
    REQUIRE(nodes.size() == 6);
    REQUIRE(faces.size() == 2);
    REQUIRE(sides.size() == 7);
    // element 0: nodes 0 1 4 3 -> sides 0..3; element 1: 1 2 5 4 -> 4 5 6, shared side 1
    const std::int32_t fe0[4] = {0, 1, 2, 3};
    const std::int32_t fe1[4] = {4, 5, 6, 1};
    for (int k = 0; k < 4; ++k) {
        CHECK_EQ(faces.at(0).edge[k], fe0[k]);
        CHECK_EQ(faces.at(1).edge[k], fe1[k]);
    }
    CHECK_EQ(faces.at(0).mat, 3);
    CHECK_EQ(sides.at(1).n0, 1);
    CHECK_EQ(sides.at(1).n1, 4);
    REQUIRE(sides.at(1).elems.size() == 2u);
    CHECK_EQ(sides.at(1).elems[0], 0);
    CHECK_EQ(sides.at(1).elems[1], 1);
    CHECK_EQ(sides.at(1).kind, 0);
    CHECK_EQ(sides.at(1).curve, -1);
    for (int i = 0; i < 7; ++i) {
        if (i != 1) CHECK_EQ(sides.at(i).kind, 1);
    }
    CHECK_EQ(sides.at(6).n0, 5);
    CHECK_EQ(sides.at(6).n1, 4);
    // node side lists in creation order
    REQUIRE(nodes.at(1).edges.size() == 3u);
    CHECK_EQ(nodes.at(1).edges[0], 0);
    CHECK_EQ(nodes.at(1).edges[1], 1);
    CHECK_EQ(nodes.at(1).edges[2], 4);
    REQUIRE(nodes.at(4).edges.size() == 3u);
    CHECK_EQ(nodes.at(4).edges[2], 6);
    CHECK_EQ(nodes.at(0).vertex, -1);
    CHECK_EQ(nodes.at(5).u20, -1);
    // one region holding both elements, in stack order
    REQUIRE(regions.size() == 1);
    CHECK_EQ(regions.at(0).mat, 3);
    REQUIRE(regions.at(0).elems.size() == 2u);
    CHECK_EQ(regions.at(0).elems[0], 0);
    CHECK_EQ(regions.at(0).elems[1], 1);
    CHECK_EQ(faces.at(1).region, 0);
    CHECK_EQ(fun_1403df8d0(mesh), -1);
    faces.at(1).region = -1;
    CHECK_EQ(fun_1403df8d0(mesh), 1);
    // 1403df640 on an existing side adds the element; on new nodes it appends a side
    const std::int32_t a = 4, b = 1, e = 9;
    CHECK_EQ(fun_1403df640(mesh, &a, &b, &e), 1);
    CHECK_EQ(sides.at(1).elems.size(), 3u);
    const std::int32_t c = 0, d = 2;
    CHECK_EQ(fun_1403df640(mesh, &c, &d, &e), 7);
    CHECK_EQ(sides.at(7).kind, -1);
    CHECK_EQ(nodes.at(2).edges[nodes.at(2).edges.size() - 1], 7);
    delete mesh;
}

// 1403dedd0 clears the arrays first: a second run gives the same topology
TEST_CASE(c11_root_rebuilds)
{
    OBJ_MESH2D* mesh = two_quads(1, 2);
    fun_1403dedd0(mesh);
    fun_1403dedd0(mesh);
    RcHandle<Elem18_1403e2450> sides(mesh->h1590);
    RcHandle<Elem18_1403e23f0> regions(mesh->h15b8);
    RcHandle<Face18> faces(mesh->h1598);
    CHECK_EQ(sides.size(), 7);
    CHECK_EQ(regions.size(), 2);
    CHECK_EQ(faces.size(), 2);
    CHECK_EQ(sides.at(1).kind, 2);
    CHECK_EQ(regions.at(1).mat, 2);
    delete mesh;
}

int main() { return testkit::run_all(); }
