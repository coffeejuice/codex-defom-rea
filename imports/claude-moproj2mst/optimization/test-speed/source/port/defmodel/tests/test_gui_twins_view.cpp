// ctest-labels: mdt gui_twins mdt-viewer
#define _CRT_SECURE_NO_WARNINGS
// Tier-1 tests of the object tests the DEF_GUI_MO_64 3D viewer calls as twins
// (defmodel/gui_twins_view.hpp): 140160060, 1402fabc0, 140160fe0. Expected values are worked out by
// hand from the decompile (spec docs/re/moproj2mst/mdt/V/spec_V.md section 6): the limit 1e9 is
// DAT_140522f50 (bytes 00 00 00 00 65 cd cd 41), the prefix "*Corrupt* " has 10 characters, so the
// old name keeps at most 0x3f - 10 = 53 of them.
#include <defmodel/gui_twins_view.hpp>

#include <cstdint>
#include <cstring>
#include <string>

#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/layout/obj_geo3d.hpp>
#include <defmodel/llobj/cls_14017ef20.hpp>
#include <defmodel/llobj_ctor/llobj_ctor.hpp>
#include <defmodel/node_access.hpp>
#include <testkit/check.hpp>

using namespace def::model;

namespace {

template <class T>
T& at(void* p, std::size_t off)
{
    return *reinterpret_cast<T*>(static_cast<unsigned char*>(p) + off);
}

// object 1 of m with n nodes whose coordinates are `xyz` (3 per node), allocated by the model
LLOBJ_DATA* object_with_nodes(DB_STEP_DATA& m, std::int32_t n, const double* xyz)
{
    LLOBJ_DATA* o = llobj_ctor::get_or_create(&m, 1);
    m.u15f8[0] = 3;
    at<std::int32_t>(o, 0x880) = n;
    auto* set = reinterpret_cast<Cls_14017ef20*>(o->s8a0);
    set->n = n;
    double* x = fun_1401808d0(set);
    CHECK(n == 0 || x != nullptr);
    if (n > 0 && x != nullptr) {
        std::memcpy(x, xyz, sizeof(double) * 3 * static_cast<std::size_t>(n));
    }
    return o;
}

}  // namespace

TEST_CASE(plausible_mesh_keeps_the_name)
{
    DB_STEP_DATA m(3);
    double xyz[12 * 3] = {};
    xyz[0] = 1e9;          // limits are inclusive
    xyz[4] = -1e9;
    xyz[10 * 3] = 5e9;     // node 11: beyond the first 10, not looked at
    LLOBJ_DATA* o = object_with_nodes(m, 12, xyz);
    std::strcpy(o->name, "Workpiece");
    CHECK(llobj_mesh_coords_plausible(o));
    CHECK(std::strcmp(o->name, "Workpiece") == 0);
}

TEST_CASE(implausible_mesh_renames)
{
    DB_STEP_DATA m(3);
    double xyz[2 * 3] = {0.0, 0.0, 0.0, 0.0, -1.0000001e9, 0.0};
    LLOBJ_DATA* o = object_with_nodes(m, 2, xyz);
    std::strcpy(o->name, "Top Die");
    CHECK(!llobj_mesh_coords_plausible(o));
    CHECK_EQ(std::string(o->name), std::string("*Corrupt* Top Die"));
    // every call adds the prefix again (one per viewer bind)
    CHECK(!llobj_mesh_coords_plausible(o));
    CHECK_EQ(std::string(o->name), std::string("*Corrupt* *Corrupt* Top Die"));
}

TEST_CASE(implausible_mesh_cuts_long_names_to_53)
{
    DB_STEP_DATA m(3);
    double xyz[3] = {2e9, 0.0, 0.0};
    LLOBJ_DATA* o = object_with_nodes(m, 1, xyz);
    const std::string n52(52, 'a');
    std::strcpy(o->name, n52.c_str());
    CHECK(!llobj_mesh_coords_plausible(o));
    CHECK_EQ(std::string(o->name), "*Corrupt* " + n52);  // 52 < 53: whole name, zero padded
    const std::string n60(60, 'b');
    std::strcpy(o->name, n60.c_str());
    CHECK(!llobj_mesh_coords_plausible(o));
    CHECK_EQ(std::string(o->name), "*Corrupt* " + std::string(53, 'b'));
}

TEST_CASE(no_nodes_is_plausible)
{
    DB_STEP_DATA m(3);
    double xyz[3] = {9e9, 9e9, 9e9};
    LLOBJ_DATA* o = object_with_nodes(m, 0, xyz);
    std::strcpy(o->name, "X");
    CHECK(llobj_mesh_coords_plausible(o));
    CHECK(std::strcmp(o->name, "X") == 0);
}

TEST_CASE(face_node_count)
{
    DB_STEP_DATA m(3);
    LLOBJ_DATA* o = llobj_ctor::get_or_create(&m, 1);
    OBJ_GEO3D* s = reinterpret_cast<OBJ_GEO3D*>(o->s3328);  // any constructed OBJ_GEO3D
    CHECK_EQ(geo3d_face_node_count(s), 0);                 // no elements
    std::int32_t n = 1;
    std::int32_t* e = s->resize_elems(&n);
    e[0] = 1; e[1] = 2; e[2] = 3; e[3] = -1;               // abs(e[3]) == e[0]
    CHECK_EQ(geo3d_face_node_count(s), 3);
    e[3] = 4;
    CHECK_EQ(geo3d_face_node_count(s), 4);
    n = 2;
    e = s->resize_elems(&n);
    e[0] = 1; e[1] = 2; e[2] = 3; e[3] = 1;
    e[4] = 2; e[5] = 3; e[6] = 4; e[7] = -2;               // second element also a triangle
    CHECK_EQ(geo3d_face_node_count(s), 3);
    e[7] = 5;                                              // second element a quad
    CHECK_EQ(geo3d_face_node_count(s), 4);
    e[3] = 6; e[7] = -2;                                   // only the first two are looked at
    CHECK_EQ(geo3d_face_node_count(s), 4);
}

TEST_CASE(rigid_is_kind_0)
{
    DB_STEP_DATA m(3);
    LLOBJ_DATA* o = llobj_ctor::get_or_create(&m, 1);
    at<std::int32_t>(o, 0xc) = 0;
    CHECK(llobj_is_rigid(o));
    at<std::int32_t>(o, 0xc) = 5;
    CHECK(!llobj_is_rigid(o));
}

int main()
{
    return testkit::run_all();
}
