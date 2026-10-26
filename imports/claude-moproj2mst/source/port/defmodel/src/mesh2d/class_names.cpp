// Class-name virtuals (vtable slot 1) of the 2D mesh classes, packet p1-08.
#include <defmodel/layout/geo2d_and_mesh2d.hpp>
#include <defmodel/layout/obj_mesh2d.hpp>

namespace def::model {

// @src 1402f0610  OBJ_MESH2D slot 1; the same folded code is _DEF_GEOPRIM's slot 1, so a
//                 _DEF_GEOPRIM also answers "OBJ_MESH2D" (str "OBJ_MESH2D")
const char* OBJ_MESH2D::class_name() const { return "OBJ_MESH2D"; }

// @src 1402848f0  GEO2D_AND_MESH2D slot 1 (str "GEO2D_AND_MESH2D")
const char* GEO2D_AND_MESH2D::class_name() { return "GEO2D_AND_MESH2D"; }

} // namespace def::model
