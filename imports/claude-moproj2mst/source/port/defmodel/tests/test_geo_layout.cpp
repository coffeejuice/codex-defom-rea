// Geometry layout headers: sizes and member offsets recovered from the ctors (p1-07).
// ctest-labels: p1-07 geo
#include <defmodel/layout/c2d3d_geometry.hpp>
#include <defmodel/layout/c3d_cross_section.hpp>
#include <defmodel/layout/def_face.hpp>
#include <defmodel/layout/geo2d_and_mesh2d.hpp>
#include <defmodel/layout/iface.hpp>
#include <defmodel/layout/obj_geo2d.hpp>
#include <defmodel/layout/obj_geo3d.hpp>
#include <defmodel/layout/obj_geo_base.hpp>
#include <defmodel/geo/range_helpers.hpp>
#include <testkit/check.hpp>

using namespace def::model;

// The headers carry the static_asserts; this repeats the sizes the allocation sites use.
TEST_CASE(sizes_match_allocations)
{
    CHECK_EQ(sizeof(C2d3dGeometry), std::size_t{0x8});
    CHECK_EQ(sizeof(OBJ_GEO_BASE), std::size_t{0x10});
    CHECK_EQ(sizeof(OBJ_GEO2D), std::size_t{0x30});   // 140276e30: new(0x30)
    CHECK_EQ(sizeof(OBJ_GEO3D), std::size_t{0x130});  // LLOBJ_DATA 0x3328 / 0x3458
    CHECK_EQ(sizeof(DEF_FACE), std::size_t{0x48});    // 1402838d0: new(0x48)
    CHECK_EQ(sizeof(GEO2D_AND_MESH2D), std::size_t{0xc8});
    CHECK_EQ(sizeof(C3dCrossSection), std::size_t{0x8});
}

// The three helpers placed in the geometry range for other packets' classes.
TEST_CASE(range_helpers)
{
    std::byte obj[0x150]{};
    CHECK(def::model::geo::fun_1402b7400(obj) == obj + 0x148);
    std::uint64_t node[7] = {11, 12, 1, 2, 3, 4, 5};
    CHECK(def::model::geo::fun_1402b7830(node) == node);
    CHECK_EQ(node[0], 11u); // links untouched
    CHECK_EQ(node[1], 12u);
    for (int i = 2; i < 7; ++i) CHECK_EQ(node[i], 0u);
}

int main() { return testkit::run_all(); }
