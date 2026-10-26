// OBJ_GEO2D_DIGITIZED (RTTI .?AVOBJ_GEO2D_DIGITIZED@@, base OBJ_GEO_BASE): the extension object
// of a _OBJ_GEO2D (class name text "C2dGeometryExtension"): a buffer of digitized 2D points and
// three digitizing parameters. 140277830 creates it at geo2d+0x20 and sets its owner (14027f430).
// Most curve operations print "to be implemented ." in the binary.
//
//   vtable   0x1405681d8 (12 slots, the OBJ_GEO_BASE slots)
//   ctor     14041dcc0 (OBJ_GEO_BASE ctor 14027f200, vptr, defaults, vector handle, reset 14041deb0)
//   copy     14041dd80 (as the ctor, then copy_points 14041e090)
//   dtor     14041de60 (free points, vector handle, OBJ_GEO_BASE dtor); scalar deleting dtor
//            14041f0b0 (slot 0; missing from the ledger)
//   size     0x50: 140277830 does operator new(0x50) before 14041dcc0
//   open     roles of u30/u34 and of the vector u48; meaning of the three defaults
//            (0.001, 20.0, 300.0 from DAT_1406397c8..d8).
#pragma once

#include <cstddef>
#include <cstdint>

#include <defmodel/layout/layout_check.hpp>
#include <defmodel/topo/alloc.hpp>
#include <defmodel/topo/geo_fallback.hpp>
#include <defmodel/topo/ptr_list.hpp>

namespace def::model {

// @data 1406397c8  OBJ_GEO2D_DIGITIZED defaults (u18, u20, u28)
inline double g_digitized_defaults[3] = {0.001, 20.0, 300.0};

class OBJ_GEO2D_DIGITIZED : public OBJ_GEO_BASE {
public:
    // @src 14041dcc0  ctor
    OBJ_GEO2D_DIGITIZED();
    // @src 14041dd80  copy ctor (copy_points(other))
    OBJ_GEO2D_DIGITIZED(OBJ_GEO2D_DIGITIZED* other);
    // @src 14041de60  dtor; slot 0 is the scalar deleting dtor 14041f0b0
    ~OBJ_GEO2D_DIGITIZED() override;

    // @src 14041f090  slot 1: "C2dGeometryExtension"
    const char* class_name() override;
    // @src 14041deb0  slot 2: defaults, counters 0, points pointer 0 (not freed), owner 0
    void reset() override;
    // @src 14041e050  slot 3: (re)allocate a zeroed point buffer of n_points
    void clear_data() override;
    // @src 14041e130  slot 4: copy_points(other)
    void copy_from(OBJ_GEO_BASE* other) override;
    // @src 14041ef70  slot 5: "to be implemented ."
    void reverse() override;
    // @src 14041efd0  slot 6: "to be implemented ."
    void scale(const double* f) override;
    // @src 14041f010  slot 7: "to be implemented ."
    void translate(const double* d) override;
    // @src 14041f050  slot 8: "to be implemented ."
    void rotate(const double* center, const void* unused, double angle) override;
    // @src 14041efa0  slot 9: "to be implemented .", 0.0
    double char_length() override;
    // @src 14041ef20  slot 10: hook_ef60, then hook_ef50
    void notify() override;
    // @src 14041f0a0  slot 11: 1
    std::int32_t is_extension() override;

    // @src 14041df30  free_points (wrapper)
    void free_points_wrap();
    // @src 14041df50  free the point buffer
    void free_points();
    // @src 14041dfa0  allocate n_points * 16 zeroed bytes (after freeing); returns the buffer
    double* alloc_points();
    // @src 14041e090  copy parameters and points from other
    void copy_points(OBJ_GEO2D_DIGITIZED* other);
    // @src 14041e160  copy the point buffer from other (only when other has points)
    void copy_point_buffer(OBJ_GEO2D_DIGITIZED* other);
    // @src 14041ef50  empty hook (also called from _OBJ_GEO2D 1402779c0)
    void hook_ef50();
    // @src 14041ef60  empty hook (also called from _OBJ_GEO2D 1402779f0)
    void hook_ef60();

    // operator new 1404797d8 (malloc) / free in the scalar deleting dtor: owners free these
    // objects with free() (GEO2D_AND_MESH2D clear_topology)
    static void* operator new(std::size_t n) { return topo::malloc_or_throw(n); }
    static void operator delete(void* p) { topo::free_block(p); }

    // vptr, u08, u0c at 0x00..0x0f (OBJ_GEO_BASE)
    OBJ_GEO2D* owner;                  // 0x10 owning _OBJ_GEO2D (14027f430); reset sets 0
    double u18;                        // 0x18 default 0.001
    double u20;                        // 0x20 default 20.0
    double u28;                        // 0x28 default 300.0
    std::int32_t u30;                  // 0x30 =0
    std::int32_t u34;                  // 0x34 =0
    std::int32_t n_points;             // 0x38 =0
    double* points;                    // 0x40 =0; n_points x (x, y)
    topo::CowPtrVector<void> u48;      // 0x48 (1402f8400)
};

static_assert(sizeof(OBJ_GEO2D_DIGITIZED) == 0x50);
DEF_LAYOUT_OFFSET(OBJ_GEO2D_DIGITIZED, owner, 0x10);
DEF_LAYOUT_OFFSET(OBJ_GEO2D_DIGITIZED, u18, 0x18);
DEF_LAYOUT_OFFSET(OBJ_GEO2D_DIGITIZED, u30, 0x30);
DEF_LAYOUT_OFFSET(OBJ_GEO2D_DIGITIZED, n_points, 0x38);
DEF_LAYOUT_OFFSET(OBJ_GEO2D_DIGITIZED, points, 0x40);
DEF_LAYOUT_OFFSET(OBJ_GEO2D_DIGITIZED, u48, 0x48);

} // namespace def::model
