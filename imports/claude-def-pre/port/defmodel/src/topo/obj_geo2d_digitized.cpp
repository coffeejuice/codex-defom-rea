// OBJ_GEO2D_DIGITIZED methods (layout: defmodel/layout/obj_geo2d_digitized.hpp).
#include <defmodel/layout/obj_geo2d_digitized.hpp>

#include <defmodel/port.hpp>

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace def::model {

namespace {

// FUN_14047903c() + 0x30 is the CRT's second FILE (stdout);
// str 1405680b8 (also 1405680d0, 1405680e8, 140568100, 140568118: same text)
void print_to_be_implemented() { std::fprintf(stdout, "to be implemented .\n"); }

} // namespace

// @src 14041dcc0  ctor: OBJ_GEO_BASE ctor 14027f200, vptr, defaults, zeroes, u48 (1402f8400),
// then reset() called directly
OBJ_GEO2D_DIGITIZED::OBJ_GEO2D_DIGITIZED()
    : owner(nullptr), u18(g_digitized_defaults[0]), u20(g_digitized_defaults[1]),
      u28(g_digitized_defaults[2]), u30(0), u34(0), n_points(0), points(nullptr) {
    OBJ_GEO2D_DIGITIZED::reset(); // 14041deb0
}

// @src 14041dd80  copy ctor: as the ctor, then copy_points(other) (14041e090)
OBJ_GEO2D_DIGITIZED::OBJ_GEO2D_DIGITIZED(OBJ_GEO2D_DIGITIZED* other)
    : owner(nullptr), u18(g_digitized_defaults[0]), u20(g_digitized_defaults[1]),
      u28(g_digitized_defaults[2]), u30(0), u34(0), n_points(0), points(nullptr) {
    OBJ_GEO2D_DIGITIZED::reset();
    copy_points(other);
}

// @src 14041de60  dtor: free_points (14041df30), u48 (folded handle dtor), OBJ_GEO_BASE dtor
// @src 14041f0b0  scalar deleting dtor (slot 0; not in the ledger)
OBJ_GEO2D_DIGITIZED::~OBJ_GEO2D_DIGITIZED() { free_points_wrap(); }

// @src 14041f090  slot 1; str 140568238
const char* OBJ_GEO2D_DIGITIZED::class_name() { return "C2dGeometryExtension"; }

// @src 14041deb0  slot 2: the points pointer is zeroed, not freed
void OBJ_GEO2D_DIGITIZED::reset() {
    u18 = g_digitized_defaults[0];
    u20 = g_digitized_defaults[1];
    u28 = g_digitized_defaults[2];
    u30 = 0;
    u34 = 0;
    n_points = 0;
    points = nullptr;
    owner = nullptr;
}

// @src 14041e050  slot 3: alloc_points()
void OBJ_GEO2D_DIGITIZED::clear_data() { alloc_points(); }

// @src 14041e130  slot 4: copy_points(other) (no type check: other is taken as a digitized curve)
void OBJ_GEO2D_DIGITIZED::copy_from(OBJ_GEO_BASE* other) {
    copy_points(static_cast<OBJ_GEO2D_DIGITIZED*>(other));
}

// @src 14041ef70  slot 5
void OBJ_GEO2D_DIGITIZED::reverse() { print_to_be_implemented(); }

// @src 14041efd0  slot 6
void OBJ_GEO2D_DIGITIZED::scale(const double*) { print_to_be_implemented(); }

// @src 14041f010  slot 7
void OBJ_GEO2D_DIGITIZED::translate(const double*) { print_to_be_implemented(); }

// @src 14041f050  slot 8
void OBJ_GEO2D_DIGITIZED::rotate(const double*, const void*, double) { print_to_be_implemented(); }

// @src 14041efa0  slot 9: prints, returns 0.0 (xorpd xmm0)
double OBJ_GEO2D_DIGITIZED::char_length() {
    print_to_be_implemented();
    return 0.0;
}

// @src 14041ef20  slot 10
void OBJ_GEO2D_DIGITIZED::notify() {
    hook_ef60();
    hook_ef50();
}

// @src 14041f0a0  slot 11
std::int32_t OBJ_GEO2D_DIGITIZED::is_extension() { return 1; }

// @src 14041df30  free_points()
void OBJ_GEO2D_DIGITIZED::free_points_wrap() { free_points(); }

// @src 14041df50  free(points) when set; points = 0 (n_points unchanged)
void OBJ_GEO2D_DIGITIZED::free_points() {
    if (points != nullptr) std::free(points);
    points = nullptr;
}

// @src 14041dfa0  free, then for n_points > 0 a zeroed buffer of n_points * 2 doubles; returns
// the buffer (0 on allocation failure, or the zeroed pointer when n_points <= 0)
double* OBJ_GEO2D_DIGITIZED::alloc_points() {
    free_points();
    if (0 < n_points) {
        const std::int32_t n = n_points;
        // operator new(8 * (n << 1)) with the overflow clamp of the binary (1404797d8)
        const std::int64_t count = static_cast<std::int64_t>(static_cast<std::int32_t>(
            static_cast<std::uint32_t>(n) << 1));
        const std::uint64_t ucount = static_cast<std::uint64_t>(count);
        if (ucount > SIZE_MAX / 8) std::abort();
        points = static_cast<double*>(std::malloc(static_cast<std::size_t>(ucount * 8)));
        if (points == nullptr) return nullptr;
        std::memset(points, 0, static_cast<std::size_t>(static_cast<std::int64_t>(
                                   static_cast<std::int32_t>(static_cast<std::uint32_t>(n) << 4))));
    }
    return points;
}

// @src 14041e090  free points, copy the three parameters and u30/u34, then the point buffer
void OBJ_GEO2D_DIGITIZED::copy_points(OBJ_GEO2D_DIGITIZED* other) {
    free_points_wrap();
    u18 = other->u18;
    u20 = other->u20;
    u28 = other->u28;
    u30 = other->u30;
    u34 = other->u34;
    copy_point_buffer(other);
}

// @src 14041e160  when other has points: take its count, allocate, copy n_points * 16 bytes;
// otherwise n_points keeps its old value (the buffer is already gone)
void OBJ_GEO2D_DIGITIZED::copy_point_buffer(OBJ_GEO2D_DIGITIZED* other) {
    free_points();
    if (0 < other->n_points) {
        n_points = other->n_points;
        alloc_points();
        if (points != nullptr) {
            std::memcpy(points, other->points,
                        static_cast<std::size_t>(static_cast<std::int64_t>(
                            static_cast<std::int32_t>(static_cast<std::uint32_t>(n_points) << 4))));
        }
    }
}

// @src 14041ef50  empty
void OBJ_GEO2D_DIGITIZED::hook_ef50() {}

// @src 14041ef60  empty
void OBJ_GEO2D_DIGITIZED::hook_ef60() {}

} // namespace def::model
