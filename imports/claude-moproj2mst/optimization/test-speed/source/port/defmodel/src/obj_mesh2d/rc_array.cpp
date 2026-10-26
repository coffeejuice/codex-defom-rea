// OBJ_MESH2D's topology arrays (see obj_mesh2d/rc_array.hpp). The element types, their
// constructors, assignments and destructors and the handle operations are header templates /
// inline members; this file keeps the translation unit of p1-13's original layout.
#include <defmodel/obj_mesh2d/rc_array.hpp>

namespace def::model::mesh2d {

static_assert(sizeof(RcArray<Face18>) == 0x20);
static_assert(sizeof(TopoLoopList) == 8);
static_assert(sizeof(IntList) == 8);

} // namespace def::model::mesh2d
