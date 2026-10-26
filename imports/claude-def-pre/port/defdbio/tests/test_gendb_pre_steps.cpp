// Tier 1 for the GENDB command pre-steps (1401d7f40) and the functions under it: mixture-material
// cleanup (1401d6250), surface normal lists (1401d63c0 / 1402059a0), the {id, point, direction}
// list helpers (1402580f0, 140258260, 140257510, 140257590) and the 3-vector kernels (14023b850,
// 14023e050, 14023c770). Expected values are worked out by hand from the recovered code.
// ctest-labels: p4-gate gendb
#include <defdbio/gendb/pre_steps.hpp>

#include <climits>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <initializer_list>

#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/layout/material.hpp>
#include <defmodel/layout/obj_geo3d.hpp>
#include <defmodel/linked/axis_point_list.hpp>
#include <defmodel/llobj_ctor/llobj_ctor.hpp>
#include <defmodel/material/material_ctor.hpp>
#include <defmodel/step/db_step.hpp>
#include <testkit/check.hpp>

using namespace def::dbio::gendb;
using def::model::DB_STEP_DATA;
using def::model::field;
using def::model::LLOBJ_DATA;
using def::model::Material;
using def::model::OBJ_GEO3D;
using def::model::linked::Cls_140257340;
using def::model::linked::Cls_140257340List;

namespace {

DB_STEP_DATA* new_model() { return new DB_STEP_DATA(3); } // real ctor (p1-03); leaked in tests

Cls_140257340List& surf_list(OBJ_GEO3D* s) { return *reinterpret_cast<Cls_140257340List*>(s->u28); }
Cls_140257340List& sym_list(LLOBJ_DATA* o)
{
    return *reinterpret_cast<Cls_140257340List*>(reinterpret_cast<unsigned char*>(o) + 0x2e80);
}

// Nodes (1-based):
//   1 (0,0,0)  2 (1,0,0)  3 (1,1,0)  4 (0,1,0)  5 (0,0,1)  6 (0,0,0.5)  7 (1,0,0.5)  8 (1,1,0.5)
// Elements (4th node 0 = triangle):
//   e0 1 2 3 0   n = (p3-p2) x (p1-p2) = (0,1,0) x (-1,0,0) = (0,-0,1), |n| = 1
//   e1 1 3 4 0   (-1,0,0) x (-1,-1,0) = (0,0,1); same direction and plane as e0
//   e2 1 2 3 4   quadrilateral: skipped, flag stays 0
//   e3 1 4 5 0   (0,-1,1) x (0,-1,0) = (1,0,0)
//   e4 6 7 8 0   (0,0,1) at z = 0.5: plane distance from an entry at z = 0 is 0.5 >
//                0.001 * char_length (sqrt 3)
//   e5 1 2 2 0   zero normal, length 0 < 1e-6: skipped, flag stays 0
OBJ_GEO3D* make_surface(LLOBJ_DATA* o)
{
    auto* s = new OBJ_GEO3D();
    const std::int32_t nn = 8, ne = 6;
    s->resize_nodes(&nn);
    s->resize_elems(&ne);
    s->alloc_elem_flags();
    const double xyz[8][3] = {{0, 0, 0}, {1, 0, 0}, {1, 1, 0}, {0, 1, 0},
                              {0, 0, 1}, {0, 0, 0.5}, {1, 0, 0.5}, {1, 1, 0.5}};
    double* c = s->coords.mutable_begin();
    for (int i = 0; i < 8; ++i)
        for (int k = 0; k < 3; ++k) c[i * 3 + k] = xyz[i][k];
    const std::int32_t el[6][4] = {{1, 2, 3, 0}, {1, 3, 4, 0}, {1, 2, 3, 4},
                                   {1, 4, 5, 0}, {6, 7, 8, 0}, {1, 2, 2, 0}};
    std::int32_t* e = s->elems.mutable_begin();
    for (int i = 0; i < 6; ++i)
        for (int k = 0; k < 4; ++k) e[i * 4 + k] = el[i][k];
    o->u3c00[0] = s;
    o->u3990 = 1;
    return s;
}

LLOBJ_DATA* add_object(DB_STEP_DATA* m, std::int32_t type)
{
    LLOBJ_DATA* o = def::model::llobj_ctor::list_add(&m->objects);
    field<std::int32_t>(o, 0xc) = type;
    m->u1378 = m->u1378 + 1;
    return o;
}

Material* add_material(DB_STEP_DATA* m, std::int32_t number, std::int32_t flag,
                       std::initializer_list<std::int32_t> comps)
{
    Material* mat = def::model::material_list_add(&m->materials);
    mat->number = number;
    mat->mstmtr_flag = flag;
    mat->mstmtr_count = static_cast<std::int32_t>(comps.size());
    mat->mstmtr_list = nullptr;
    if (comps.size() != 0) {
        mat->mstmtr_list = static_cast<std::int32_t*>(std::calloc(comps.size(), 4));
        int i = 0;
        for (std::int32_t v : comps) mat->mstmtr_list[i++] = v;
    }
    return mat;
}

} // namespace

// ---- kernels ------------------------------------------------------------------------------------

TEST_CASE(dist2_sums_squares)
{
    const double a[3] = {1, 2, 3}, b[3] = {4, 6, 3};
    CHECK_BITS(vec3_dist2(a, b), 25.0); // 9 + 16 + 0
}

TEST_CASE(triangle_normal_unit_and_zero)
{
    // a (1,2,3), b 0, c (0,1,1): v = c - b = (0,1,1), u = a - b = (1,2,3)
    // n = (1*3 - 1*2, 1*1 - 0*3, 0*2 - 1*1) = (1, 1, -1), |n| = sqrt 3
    const double a[3] = {1, 2, 3}, b[3] = {0, 0, 0}, c[3] = {0, 1, 1};
    double n[3];
    triangle_normal(a, b, c, n);
    const double r = std::sqrt(3.0);
    CHECK_BITS(n[0], 1.0 / r);
    CHECK_BITS(n[1], 1.0 / r);
    CHECK_BITS(n[2], -1.0 / r);
    // all three points equal: n = 0, length 0, no division (no NaN)
    double z[3] = {7, 7, 7};
    triangle_normal(a, a, a, z);
    CHECK_BITS(z[0], 0.0);
    CHECK_BITS(z[1], 0.0);
    CHECK_BITS(z[2], 0.0);
}

TEST_CASE(plane_distance_is_absolute)
{
    const double p[3] = {1, 1, 5}, q[3] = {0, 0, 2};
    const double up[3] = {0, 0, 1}, down[3] = {0, 0, -1};
    CHECK_BITS(plane_distance(p, up, q), 3.0);
    CHECK_BITS(plane_distance(p, down, q), 3.0);
}

TEST_CASE(point_list_find_and_free_id)
{
    Cls_140257340List l;
    for (std::int32_t id : {1, 2, 4}) l.add(id);
    CHECK(point_list_find(&l, 4) == l.nth(3));
    CHECK(point_list_find(&l, 5) == nullptr);
    CHECK_EQ(point_list_free_id(&l), 3);
    Cls_140257340List empty;
    CHECK_EQ(point_list_free_id(&empty), 1);
    Cls_140257340* n = l.nth(1);
    const double p[3] = {1.5, 2.5, 3.5}, d[3] = {0, 1, 0};
    point_node_set_point(n, p);
    point_node_set_direction(n, d);
    point_node_set_point(n, nullptr); // null: unchanged
    CHECK_BITS(n->point[1], 2.5);
    CHECK_BITS(n->direction[1], 1.0);
}

// ---- 1402059a0 ------------------------------------------------------------------------------------

TEST_CASE(surface_normals_without_copy)
{
    DB_STEP_DATA* m = new_model();
    LLOBJ_DATA* o = add_object(m, 0);
    OBJ_GEO3D* s = make_surface(o);
    surf_list(s).add(42); // cleared first
    sym_list(o).add(9);   // flag 0: not copied
    object_surface_normals(o, 0);
    Cls_140257340List& l = surf_list(s);
    REQUIRE(l.size() == 3);
    // e0 -> new id 1 at node 1, e1 matches it, e3 -> id 2, e4 -> id 3 at node 6
    const std::int32_t want[6] = {601, 601, 0, 602, 603, 0};
    for (int i = 0; i < 6; ++i) CHECK_EQ(s->elem_flags[i], want[i]);
    CHECK_EQ(l.nth(1)->id, 1);
    CHECK_BITS(l.nth(1)->direction[2], 1.0);
    CHECK_BITS(l.nth(1)->direction[1], -0.0); // 0 * -1 - 0 * 0
    CHECK_BITS(l.nth(2)->direction[0], 1.0);
    CHECK_EQ(l.nth(3)->id, 3);
    CHECK_BITS(l.nth(3)->point[2], 0.5);
}

TEST_CASE(surface_normals_with_copy)
{
    DB_STEP_DATA* m = new_model();
    LLOBJ_DATA* o = add_object(m, 0);
    OBJ_GEO3D* s = make_surface(o);
    // object list: id 1 (0,0,1) through (5,5,0) (the z = 0 plane), id 3 (1,0,0) through (0,7,7)
    Cls_140257340* a = sym_list(o).add(1);
    a->point[0] = 5, a->point[1] = 5, a->direction[2] = 1;
    Cls_140257340* b = sym_list(o).add(3);
    b->point[1] = 7, b->point[2] = 7, b->direction[0] = 1;
    object_surface_normals(o, 1);
    Cls_140257340List& l = surf_list(s);
    REQUIRE(l.size() == 3);
    // e0, e1 match id 1; e3 matches id 3; e4 needs a new entry: first free id 2
    const std::int32_t want[6] = {601, 601, 0, 603, 602, 0};
    for (int i = 0; i < 6; ++i) CHECK_EQ(s->elem_flags[i], want[i]);
    CHECK_EQ(l.nth(3)->id, 2);
    CHECK_BITS(l.nth(3)->point[2], 0.5);
    CHECK(sym_list(o).size() == 2); // source untouched
}

TEST_CASE(surface_normals_need_surface_flag)
{
    DB_STEP_DATA* m = new_model();
    LLOBJ_DATA* o = add_object(m, 0);
    OBJ_GEO3D* s = make_surface(o);
    o->u3990 = 0;
    surf_list(s).add(5);
    object_surface_normals(o, 0);
    CHECK(surf_list(s).size() == 1); // not even cleared
    CHECK_EQ(s->elem_flags[0], 0);
}

// ---- 1401d6250 ------------------------------------------------------------------------------------

TEST_CASE(mixture_cleanup)
{
    DB_STEP_DATA* m = new_model();
    Material* m1 = add_material(m, 1, 1, {2, 3, 7}); // 7 does not exist
    Material* m2 = add_material(m, 2, 0, {9});
    Material* m3 = add_material(m, 3, 2, {4});
    Material* m4 = add_material(m, 4, 0, {8});
    Material* m5 = add_material(m, 5, 1, {4}); // beyond n_materials: not looked at
    m->n_materials = 4;
    mixture_material_cleanup(m);
    CHECK_EQ(m1->mstmtr_flag, 1);
    CHECK_EQ(m1->mstmtr_count, 3);
    CHECK_EQ(m2->mstmtr_flag, 2);
    CHECK_EQ(m2->mstmtr_count, 0);
    CHECK(m2->mstmtr_list == nullptr);
    CHECK_EQ(m3->mstmtr_flag, 2); // already kind 2: list kept
    CHECK_EQ(m3->mstmtr_count, 1);
    CHECK(m3->mstmtr_list != nullptr);
    CHECK_EQ(m4->mstmtr_flag, 0);
    CHECK_EQ(m4->mstmtr_count, 1);
    CHECK_EQ(m5->mstmtr_flag, 1);
}

TEST_CASE(mixture_cleanup_self_reference)
{
    // a mixture listing itself: it becomes kind 2 with no list; the loop re-reads the count (0)
    DB_STEP_DATA* m = new_model();
    Material* m1 = add_material(m, 1, 1, {1, 2});
    Material* m2 = add_material(m, 2, 0, {});
    m->n_materials = 2;
    mixture_material_cleanup(m);
    CHECK_EQ(m1->mstmtr_flag, 2);
    CHECK_EQ(m1->mstmtr_count, 0);
    CHECK(m1->mstmtr_list == nullptr);
    CHECK_EQ(m2->mstmtr_flag, 0); // never reached
}

// ---- 1401d63c0 / 1401d7f40 ------------------------------------------------------------------------

TEST_CASE(pre_steps_skip_type7_and_nstart)
{
    DB_STEP_DATA* m = new_model();
    LLOBJ_DATA* o7 = add_object(m, 7);
    OBJ_GEO3D* s7 = make_surface(o7);
    LLOBJ_DATA* o1 = add_object(m, 1);
    OBJ_GEO3D* s1 = make_surface(o1);
    m->sim_ctrl.dttype.ndim = 0;
    m->sim_ctrl.dsmax = -1.0;
    m->sim_ctrl.dttype.u58 = 5;
    m->u2f88 = 1;
    m->sim_ctrl.nstart = 5;
    gendb_command_pre_steps(m);
    CHECK_EQ(s7->elem_flags[0], 0);
    CHECK(surf_list(s7).size() == 0);
    CHECK_EQ(s1->elem_flags[0], 601);
    CHECK_EQ(m->sim_ctrl.dttype.u58, 1); // 1401ee510: DSMAX <= 0
    CHECK_EQ(m->sim_ctrl.nstart, -6);    // -(5 + 1)
    CHECK_EQ(m->u744, -6);

    m->sim_ctrl.nstart = -3; // negative: kept
    gendb_command_pre_steps(m);
    CHECK_EQ(m->sim_ctrl.nstart, -3);
    CHECK_EQ(m->u744, -3);

    m->sim_ctrl.nstart = INT_MAX; // 32-bit wrap: INT_MAX + 1 = INT_MIN, -INT_MIN = INT_MIN
    gendb_command_pre_steps(m);
    CHECK_EQ(m->sim_ctrl.nstart, INT_MIN);

    m->u2f88 = 0;
    m->sim_ctrl.nstart = 4;
    m->u744 = 11;
    gendb_command_pre_steps(m);
    CHECK_EQ(m->sim_ctrl.nstart, 4);
    CHECK_EQ(m->u744, 11);
}

int main() { return testkit::run_all(); }
