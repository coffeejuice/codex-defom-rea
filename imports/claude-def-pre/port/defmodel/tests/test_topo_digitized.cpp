// ctest-labels: p1-11 topo
// Tier-1 tests of OBJ_GEO2D_DIGITIZED (14041dcc0..14041f0b0). Defaults are the doubles at
// DAT_1406397c8..d8 (0.001, 20.0, 300.0) read from DEF_PRE_64.exe; the rest follows the decompile.
#include <defmodel/layout/obj_geo2d_digitized.hpp>
#include <testkit/check.hpp>

#include <cstdlib>
#include <cstring>
#include <string>

using namespace def::model;

TEST_CASE(ctor_defaults_and_constant_slots)
{
    OBJ_GEO2D_DIGITIZED d; // 14041dcc0 + reset 14041deb0
    CHECK_BITS(d.u18, 0.001);
    CHECK_BITS(d.u20, 20.0);
    CHECK_BITS(d.u28, 300.0);
    CHECK_EQ(d.u30, 0);
    CHECK_EQ(d.u34, 0);
    CHECK_EQ(d.n_points, 0);
    CHECK(d.points == nullptr);
    CHECK(d.owner == nullptr);
    CHECK_EQ(std::string(d.class_name()), std::string("C2dGeometryExtension")); // 14041f090
    CHECK_EQ(d.is_extension(), 1);                                          // 14041f0a0
    OBJ_GEO_BASE* b = &d;
    CHECK_EQ(std::string(b->class_name()), std::string("C2dGeometryExtension")); // virtual slot 1
}

TEST_CASE(alloc_points_zeroed)
{
    OBJ_GEO2D_DIGITIZED d;
    CHECK(d.alloc_points() == nullptr); // n_points 0: nothing allocated
    d.n_points = 3;
    d.clear_data(); // 14041e050 -> 14041dfa0: 3 * 16 zero bytes
    REQUIRE(d.points != nullptr);
    for (int i = 0; i < 6; ++i) CHECK_BITS(d.points[i], 0.0);
}

TEST_CASE(copy_from_copies_parameters_and_points)
{
    OBJ_GEO2D_DIGITIZED src;
    src.u18 = 1.0;
    src.u20 = 2.0;
    src.u28 = 3.0;
    src.u30 = 4;
    src.u34 = 5;
    src.n_points = 2;
    src.alloc_points();
    const double pts[4] = {1.25, -2.5, 3.75, 4.0};
    std::memcpy(src.points, pts, sizeof pts);

    OBJ_GEO2D_DIGITIZED dst;
    OBJ_GEO_BASE* b = &dst;
    b->copy_from(&src); // slot 4: 14041e130 -> 14041e090 -> 14041e160
    CHECK_BITS(dst.u18, 1.0);
    CHECK_BITS(dst.u20, 2.0);
    CHECK_BITS(dst.u28, 3.0);
    CHECK_EQ(dst.u30, 4);
    CHECK_EQ(dst.u34, 5);
    CHECK_EQ(dst.n_points, 2);
    REQUIRE(dst.points != nullptr);
    CHECK(dst.points != src.points);
    for (int i = 0; i < 4; ++i) CHECK_BITS(dst.points[i], pts[i]);

    OBJ_GEO2D_DIGITIZED copy(&src); // 14041dd80
    CHECK_EQ(copy.n_points, 2);
    CHECK_BITS(copy.points[3], 4.0);
}

TEST_CASE(copy_from_empty_keeps_count_drops_buffer)
{
    // 14041e160: other.n_points <= 0 leaves n_points as it was; the buffer was freed by 14041e090
    OBJ_GEO2D_DIGITIZED dst;
    dst.n_points = 3;
    dst.alloc_points();
    OBJ_GEO2D_DIGITIZED empty;
    dst.copy_from(&empty);
    CHECK_EQ(dst.n_points, 3);
    CHECK(dst.points == nullptr);
}

TEST_CASE(reset_zeroes_pointer_without_freeing)
{
    OBJ_GEO2D_DIGITIZED d;
    d.n_points = 1;
    double* p = d.alloc_points();
    d.u18 = 9.0;
    d.reset(); // 14041deb0: points = 0, not freed
    CHECK(d.points == nullptr);
    CHECK_EQ(d.n_points, 0);
    CHECK_BITS(d.u18, 0.001);
    std::free(p);
}

TEST_CASE(unimplemented_slots_return_defaults)
{
    OBJ_GEO2D_DIGITIZED d;
    CHECK_BITS(d.char_length(), 0.0); // 14041efa0 prints "to be implemented .", returns 0.0
    d.notify();                       // 14041ef20: two empty hooks
}

int main() { return testkit::run_all(); }
