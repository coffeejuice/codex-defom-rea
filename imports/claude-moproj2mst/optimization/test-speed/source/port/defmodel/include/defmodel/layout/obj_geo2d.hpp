// OBJ_GEO2D = RTTI .?AV_OBJ_GEO2D@@ (_OBJ_GEO2D, base OBJ_GEO_BASE): a 2D outline as a flat
// array of doubles, n_points rows of dof() values. Names shared with p1-11
// (defmodel/topo/geo_fallback.hpp) and p1-05 (SdctlPtrList<_OBJ_GEO2D> tests for this header).
//
//   curve_type 1  dof 3: point rows   x, y, r (r is scaled with x, y; the bounding box reports
//                        its maximum)
//   curve_type 2  dof 6: segment rows type (1 line, 2 arc), x1, y1, x2, y2, a (arc angle in
//                        radians; reverse() converts it to degrees)
//
//   vtable   0x140537a38 (12 slots)
//   ctor     140275cd0 (OBJ_GEO_BASE ctor, vptr, fields 0, then the slot-2 body 140275e50)
//   dtor     140275e00 (clear_all 140275e90, OBJ_GEO_BASE dtor); slot 0 is the vector deleting
//            dtor 1401b0650 (element size 0x30), compiler-generated in C++
//   size     0x30 (140276e30 and 14027fce0 do operator new(0x30))
#pragma once

#include <cstddef>
#include <cstdint>

#include <defmodel/layout/layout_check.hpp>
#include <defmodel/layout/obj_geo_base.hpp>

namespace def::model {

class OBJ_GEO2D_DIGITIZED;

class OBJ_GEO2D : public OBJ_GEO_BASE {
public:
    // @src 140275cd0  ctor
    OBJ_GEO2D();
    OBJ_GEO2D(const OBJ_GEO2D&) = delete;
    OBJ_GEO2D& operator=(const OBJ_GEO2D&) = delete;

    // @src 1401b0650  slot 0: vector deleting dtor (dtor body 140275e00)
    ~OBJ_GEO2D() override;
    // @src 14027f380  slot 1: "_OBJ_GEO2D"
    const char* class_name() override;
    // @src 140275e50  slot 2: curve_type, n_points = 0, points, ext = 0 (nothing freed)
    void reset() override;
    // @src 14027f360  slot 3: -> allocate_points 140275f30
    void clear_data() override;
    // @src 140276230  slot 4: if other != 0, assign(other)
    void copy_from(OBJ_GEO_BASE* other) override;
    // @src 1402567c0  slot 5: -> reverse_points 1402788a0
    void reverse() override;
    // @src 140277a20  slot 6
    void scale(const double* f) override;
    // @src 140277b00  slot 7
    void translate(const double* d) override;
    // @src 140277c50  slot 8 (axis unused)
    void rotate(const double* center, const void* axis, double angle) override;
    // @src 140278f70  slot 9: bounding-box diagonal, at least twice the largest radius
    double char_length() override;
    // @src 140277970  slot 10: ext's slot 10
    void notify() override;
    // @src 14027f390  slot 11: 0
    std::int32_t is_extension() override;

    // @src 140275e90  curve_type, n_points = 0, free points, delete ext
    void clear_all();
    // @src 140275f30  free points, new zeroed n_points * dof() doubles; returns points
    double* allocate_points();
    // @src 140276030  free points, points = 0
    void free_points();
    // @src 1402760e0  values per row for curve_type
    std::int32_t dof() const;
    // @src 140276140  copy curve_type, n_points, points, ext
    void assign(OBJ_GEO2D* other);
    // @src 1402763d0  convert to curve type t (Fortran outline routines); 1 when converted
    std::int64_t set_curve_type(std::int32_t t);
    // @src 1402765e0  curve type 1: drop each point equal (1e-6) to the next; true if any dropped
    bool fun_1402765e0();
    // @src 140276e30  append other's rows (converted to this curve type)
    void append(OBJ_GEO2D* other);
    // @src 140277830  replace ext by a new OBJ_GEO2D_DIGITIZED that points back at this
    void make_ext();
    // @src 1402778f0  delete ext
    void delete_ext();
    // @src 1402788a0  reverse the row order (Fortran 14041f590)
    void reverse_points();
    // @src 1402789f0  signed area of the outline (arcs as 4 chords) >= 0
    bool is_ccw();
    // @src 140279040  bounding box (min x,y / max x,y) and the largest radius; false if empty
    bool bounding_box(double* min, double* max, double* radius);
    // @src 140278ed0  true when this outline's bounding box lies inside other's
    bool contains_bbox(OBJ_GEO2D* other);
    // @src 14027f410  0x28 = v
    void set_flag28(bool v);
    // 14041ce20 (p1-11)
    bool get_flag28() const;

    // ext as its base class (OBJ_GEO2D_DIGITIZED derives from OBJ_GEO_BASE at offset 0)
    OBJ_GEO_BASE* ext_base() const { return reinterpret_cast<OBJ_GEO_BASE*>(ext); }

    // vptr, OBJ_GEO_BASE 0x08..0x0f
    std::int32_t curve_type;     // 0x010 0, 1 (points) or 2 (segments)
    std::int32_t n_points;       // 0x014 rows
    double* points;              // 0x018 n_points * dof() doubles (operator new / free)
    OBJ_GEO2D_DIGITIZED* ext;    // 0x020 owned (new 0x50, p1-11)
    std::uint8_t flag28;         // 0x028 set when the points change
    std::byte pad_029[7];        // 0x029
};

static_assert(sizeof(OBJ_GEO2D) == 0x30);
DEF_LAYOUT_OFFSET(OBJ_GEO2D, curve_type, 0x10);
DEF_LAYOUT_OFFSET(OBJ_GEO2D, n_points, 0x14);
DEF_LAYOUT_OFFSET(OBJ_GEO2D, points, 0x18);
DEF_LAYOUT_OFFSET(OBJ_GEO2D, ext, 0x20);
DEF_LAYOUT_OFFSET(OBJ_GEO2D, flag28, 0x28);

// @src 140276090  values per row: curve type 1 -> 3, 2 -> 6, else 0
std::int32_t geo2d_dof(std::int32_t curve_type);
// @src 140275fa0  new zeroed n doubles (0 when n < 1)
double* geo2d_alloc_zeroed(std::int32_t n);
// @src 140276060  free(p) when p != 0
void geo2d_free(void* p);
// @src 14027f3e0  -> 14041e090(dst, src) (p1-11: copy an OBJ_GEO2D_DIGITIZED)
void geo2d_copy_ext(OBJ_GEO2D_DIGITIZED* dst, OBJ_GEO2D_DIGITIZED* src);
// @src 14027f430  ext + 0x10 = owner (OBJ_GEO2D_DIGITIZED's first own member)
void geo2d_set_ext_owner(OBJ_GEO2D_DIGITIZED* ext, OBJ_GEO2D* owner);
// @src 14027f460  p = (p, 1) * m (m row-major 4x4 doubles, rows 0x20 apart); x, y, z kept
void geo_transform_point(double* p, const double* m);

} // namespace def::model
