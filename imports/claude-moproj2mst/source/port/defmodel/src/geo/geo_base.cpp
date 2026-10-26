// C2d3dGeometry and OBJ_GEO_BASE (p1-07). Layout: defmodel/layout/c2d3d_geometry.hpp,
// defmodel/layout/obj_geo_base.hpp.
#include <defmodel/layout/c2d3d_geometry.hpp>
#include <defmodel/layout/obj_geo_base.hpp>

namespace def::model {

// @src 14027f240  ctor: *this = C2d3dGeometry::vftable
C2d3dGeometry::C2d3dGeometry() = default;

// @src 14027f2a0  dtor: *this = C2d3dGeometry::vftable
// @src 14027f260  slot 0: scalar deleting dtor (free through MallocObject)
C2d3dGeometry::~C2d3dGeometry() = default;

// @src 14027f200  ctor: C2d3dGeometry ctor, vptr, 0x0c = 0 (0x08 untouched)
OBJ_GEO_BASE::OBJ_GEO_BASE() { u0c = 0; }

// @src 14027f2c0  dtor: vptr, C2d3dGeometry dtor
// @src 14027f320  slot 0: scalar deleting dtor
OBJ_GEO_BASE::~OBJ_GEO_BASE() = default;

// @src 14027f310  slot 1  // str 140537b20
const char* OBJ_GEO_BASE::class_name() { return "OBJ_GEO_BASE"; }

// @src 14027f2f0  slot 2: empty
void OBJ_GEO_BASE::reset() {}

// @src 14027f300  slot 3: empty (also _OBJ_GEO3D slot 3)
void OBJ_GEO_BASE::clear_data() {}

} // namespace def::model
