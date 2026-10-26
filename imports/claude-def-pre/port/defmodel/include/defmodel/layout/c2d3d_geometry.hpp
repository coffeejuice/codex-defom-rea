// C2d3dGeometry (RTTI .?AVC2d3dGeometry@@): abstract root of the geometry objects (OBJ_GEO_BASE
// and below). Only a vptr.
//
//   vtable   0x140537b10 (2 slots: scalar deleting dtor 14027f260, _purecall)
//   ctor     14027f240 (vptr only)
//   dtor     14027f2a0 (vptr only); scalar deleting dtor 14027f260 frees with free()
//   size     0x8
// Allocation: operator new (malloc) / free, through geo::MallocObject.
#pragma once

#include <cstddef>
#include <cstdint>

#include <defmodel/geo/geo_alloc.hpp>
#include <defmodel/layout/layout_check.hpp>

namespace def::model {

class C2d3dGeometry : public geo::MallocObject {
public:
    // @src 14027f240  ctor
    C2d3dGeometry();

    // @src 14027f260  slot 0: scalar deleting dtor (dtor body 14027f2a0)
    virtual ~C2d3dGeometry();
    // slot 1: pure (_purecall); OBJ_GEO_BASE and below return their class name
    virtual const char* class_name() = 0;

    // vptr at 0x000
};

static_assert(sizeof(C2d3dGeometry) == 0x8);

} // namespace def::model
