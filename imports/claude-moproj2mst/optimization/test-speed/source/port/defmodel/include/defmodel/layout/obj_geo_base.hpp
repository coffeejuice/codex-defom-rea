// OBJ_GEO_BASE (RTTI .?AVOBJ_GEO_BASE@@, base C2d3dGeometry): interface of one geometry object
// (2D outline _OBJ_GEO2D, 3D surface mesh _OBJ_GEO3D, digitized outline OBJ_GEO2D_DIGITIZED).
// Slots 2..11 are the operations; the subclasses' implementations give their meaning. Slot names
// are shared with p1-11 (OBJ_GEO2D_DIGITIZED overrides them).
//
//   vtable   0x140537aa8 (12 slots; 4..11 _purecall)
//   ctor     14027f200 (C2d3dGeometry ctor, vptr, 0x0c = 0)
//   dtor     14027f2c0 (vptr, C2d3dGeometry dtor); scalar deleting dtor 14027f320 (slot 0)
//   size     0x10
//   open     0x08: no ctor or method in Phase 1 writes it.
#pragma once

#include <cstddef>
#include <cstdint>

#include <defmodel/layout/c2d3d_geometry.hpp>
#include <defmodel/layout/layout_check.hpp>

namespace def::model {

class OBJ_GEO_BASE : public C2d3dGeometry {
public:
    // @src 14027f200  ctor
    OBJ_GEO_BASE();

    // @src 14027f320  slot 0: scalar deleting dtor (dtor body 14027f2c0)
    ~OBJ_GEO_BASE() override;
    // @src 14027f310  slot 1: "OBJ_GEO_BASE"
    const char* class_name() override;
    // @src 14027f2f0  slot 2: no-op (2D: zero the fields, 3D: clear)
    virtual void reset();
    // @src 14027f300  slot 3: no-op (2D: allocate the point array; 3D inherits the no-op)
    virtual void clear_data();
    // slot 4: copy the geometry of another object of the same class
    virtual void copy_from(OBJ_GEO_BASE* other) = 0;
    // slot 5: reverse the orientation
    virtual void reverse() = 0;
    // slot 6: multiply coordinates by factor[0]
    virtual void scale(const double* factor) = 0;
    // slot 7: add offset to the coordinates
    virtual void translate(const double* offset) = 0;
    // slot 8: rotate by angle (degrees) about center (3D: about axis through center)
    // (2D ignores axis)
    virtual void rotate(const double* center, const void* axis, double angle) = 0;
    // slot 9: characteristic length: the bounding-box diagonal
    virtual double char_length() = 0;
    // slot 10: 2D: forwards to the digitized outline's slot 10; 3D: no-op
    virtual void notify() = 0;
    // slot 11: 0 = _OBJ_GEO2D, 100 = _OBJ_GEO3D
    virtual std::int32_t is_extension() = 0;

    // vptr at 0x000
    std::int32_t u08;   // 0x008 not written by any Phase 1 code
    std::int32_t u0c;   // 0x00c ctor = 0
};

static_assert(sizeof(OBJ_GEO_BASE) == 0x10);
DEF_LAYOUT_OFFSET(OBJ_GEO_BASE, u0c, 0x0c);

} // namespace def::model
