// _DEF_FACE and GEO2D_AND_MESH2D (p1-07): face fields and loop list, the cross-section's
// reset, edge curves, edge accessors and the curve copy. Topology objects (p1-11), the matrix
// (p1-06) and p1-08's methods are fakes that record their calls.
// ctest-labels: p1-07 geo
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <vector>

#include <defmodel/layout/def_face.hpp>
#include <defmodel/layout/geo2d_and_mesh2d.hpp>
#include <defmodel/layout/obj_geo2d.hpp>
#include <testkit/check.hpp>

#include "geo_fakes.hpp"

using namespace def::model;

namespace {

std::vector<std::uint64_t> seq;
std::vector<bool> flags_847d0;

// a fake _DEF_EDGE in the 0x38 bytes the cross-section allocates: geometry at +8, id at +0x10
struct EdgeMem {
    void* vptr;
    OBJ_GEO_BASE* geo;
    std::int32_t id;
};
DEF_EDGE* edge_ctor(void* mem)
{
    std::memset(mem, 0, 0x38);
    return static_cast<DEF_EDGE*>(mem);
}
void edge_set_geo(DEF_EDGE* e, OBJ_GEO_BASE* g) { reinterpret_cast<EdgeMem*>(e)->geo = g; }
void edge_set_id(DEF_EDGE* e, std::int32_t* id) { reinterpret_cast<EdgeMem*>(e)->id = *id; }
std::int32_t edge_id(DEF_EDGE* e) { return reinterpret_cast<EdgeMem*>(e)->id; }
OBJ_GEO_BASE* edge_geo(DEF_EDGE* e) { return reinterpret_cast<EdgeMem*>(e)->geo; }
DEF_EDGE* cast_edge(IEdge* p) { return reinterpret_cast<DEF_EDGE*>(p); }
void f847d0(GEO2D_AND_MESH2D*, const bool* b) { flags_847d0.push_back(*b); }
void* mat_ctor(void* m) { seq.push_back(0x140220540); return m; }
void mat_identity(void* m)
{
    seq.push_back(0x140220570);
    double id[16] = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};
    std::memcpy(m, id, sizeof id);
}
void mat_dtor(void*) { seq.push_back(0x140220560); }
void* mat_copy(void* d, void* s)
{
    seq.push_back(0x140220670);
    std::memcpy(d, s, 0x80);
    return d;
}
bool no_faces(GEO2D_AND_MESH2D*) { return false; }

std::vector<std::int32_t> unlinked;
DEF_LOOP* cast_loop(ILoop* l) { return reinterpret_cast<std::uintptr_t>(l) % 2 == 0 ? reinterpret_cast<DEF_LOOP*>(l) : nullptr; }
void loop_remove_face(DEF_LOOP* l, DEF_FACE*) { unlinked.push_back(static_cast<std::int32_t>(reinterpret_cast<std::uintptr_t>(l))); }

void install()
{
    geofake::reset();
    geofake::install_ext();
    seq.clear();
    flags_847d0.clear();
    unlinked.clear();
    pending::fun_14041c300 = &edge_ctor;
    pending::fun_14041c4a0 = &edge_set_geo;
    pending::fun_14041c5f0 = &edge_set_id;
    pending::fun_14041c610 = &edge_id;
    pending::fun_140284990 = &edge_geo;
    pending::cast_def_edge = &cast_edge;
    pending::fun_1402847d0 = &f847d0;
    pending::fun_140220540 = &mat_ctor;
    pending::fun_140220570 = &mat_identity;
    pending::fun_140220560 = &mat_dtor;
    pending::fun_140220670 = &mat_copy;
    pending::fun_140158ec0 = &no_faces;
    pending::cast_def_loop = &cast_loop;
    pending::fun_140412550 = &loop_remove_face;
}

} // namespace

TEST_CASE(face_fields_and_slots)
{
    install();
    auto* f = new DEF_FACE();
    CHECK_EQ(f->u40, 0);
    CHECK(f->u10 == nullptr);
    CHECK(!f->flag18());
    CHECK(!f->flag19());
    f->set_flag18(true);
    f->set_flag19(true);
    CHECK(f->u18 && f->u19);
    std::int32_t v = 9;
    f->set_u40(&v);
    CHECK_EQ(f->u40_value(), 9);
    CHECK(reinterpret_cast<std::byte*>(f->coords()) == reinterpret_cast<std::byte*>(f) + 0x20);
    CHECK(reinterpret_cast<std::byte*>(f->u28_vector()) == reinterpret_cast<std::byte*>(f) + 0x28);
    CHECK(reinterpret_cast<std::byte*>(f->u30_vector()) == reinterpret_cast<std::byte*>(f) + 0x30);
    CHECK(reinterpret_cast<std::byte*>(f->u38_vector()) == reinterpret_cast<std::byte*>(f) + 0x38);
    std::int32_t i0 = 0;
    CHECK(f->loop_at(&i0) == nullptr); // empty
    f->add_loop(reinterpret_cast<ILoop*>(std::uintptr_t{2}));
    f->add_loop(reinterpret_cast<ILoop*>(std::uintptr_t{3}));
    f->add_loop(reinterpret_cast<ILoop*>(std::uintptr_t{4}));
    std::int32_t i1 = 1, im = -1, i3 = 3;
    CHECK(f->loop_at(&i1) == reinterpret_cast<ILoop*>(std::uintptr_t{3}));
    CHECK(f->loop_at(&im) == nullptr);
    CHECK(f->loop_at(&i3) == nullptr);
    delete f; // unlinks itself from the loops that cast to _DEF_LOOP (fake: even values)
    CHECK(unlinked == (std::vector<std::int32_t>{2, 4}));
}

TEST_CASE(cross_section_ctor_and_reset)
{
    install();
    {
        GEO2D_AND_MESH2D g;
        // matrix ctor, then reset: clear_topology -> 1402847d0(true), identity
        CHECK(seq == (std::vector<std::uint64_t>{0x140220540, 0x140220570}));
        CHECK(flags_847d0 == (std::vector<bool>{true}));
        CHECK(g.mesh == nullptr);
        CHECK_EQ(int(g.u30), 0);
        CHECK_EQ(int(g.u31), 0);
        CHECK_EQ(int(g.ub8), 1);
        CHECK_EQ(g.reset(), 0);
        seq.clear();
    }
    CHECK(seq == (std::vector<std::uint64_t>{0x140220560}));
}

TEST_CASE(edge_curves_and_copy)
{
    install();
    GEO2D_AND_MESH2D a;
    OBJ_GEO2D* c1 = a.add_edge_curve();
    OBJ_GEO2D* c2 = a.add_edge_curve();
    REQUIRE(c1 != nullptr && c2 != nullptr);
    CHECK_EQ(static_cast<int>(a.edges.size()), 2);
    CHECK_EQ(edge_id(a.edge_at(0)), 1); // id = count + 1
    CHECK_EQ(edge_id(a.edge_at(1)), 2);
    CHECK(a.edge_curve(1) == c2);
    CHECK(a.edge_curve(2) == nullptr);
    CHECK(a.edge_curve(-1) == nullptr);
    CHECK(a.edge_at(5) == nullptr);
    c2->curve_type = 1;
    c2->n_points = 1;
    c2->allocate_points();
    c2->points[0] = 4;
    c2->points[1] = 5;
    a.u30 = 7;
    a.ub8 = 0;
    reinterpret_cast<double*>(a.matrix)[12] = 3.0;

    GEO2D_AND_MESH2D b(a);
    CHECK_EQ(static_cast<int>(b.edges.size()), 2);
    OBJ_GEO2D* d2 = b.edge_curve(1);
    REQUIRE(d2 != nullptr);
    CHECK(d2 != c2);
    CHECK_EQ(d2->n_points, 1);
    CHECK_BITS(d2->points[1], 5.0);
    CHECK_EQ(int(b.u30), 7);
    CHECK_EQ(int(b.ub8), 0);
    CHECK_BITS(reinterpret_cast<double*>(b.matrix)[12], 3.0);
    // clear_topology frees the edge objects (not their curves) and notifies with true
    flags_847d0.clear();
    b.clear_topology();
    CHECK_EQ(static_cast<int>(b.edges.size()), 0);
    CHECK(flags_847d0 == (std::vector<bool>{true}));
}

int main() { return testkit::run_all(); }
