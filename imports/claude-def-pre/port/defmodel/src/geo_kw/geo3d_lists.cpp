// kw-g2geo: access to the die point/edge set lists of an OBJ_GEO3D (u50 / u58). Their ctor,
// dtor and clear (1402fedd0 ...) are wired in defmodel/src/geo/pending_wiring.cpp.
#include <defmodel/geo_kw/surface_model.hpp>

#include <defmodel/layout/obj_geo3d.hpp>

namespace def::model::geo_kw {

// @src 1402b74c0  this + 0x50
DiePointSets& die_point_sets(void* geo3d)
{
    return *reinterpret_cast<DiePointSets*>(&static_cast<OBJ_GEO3D*>(geo3d)->u50);
}

// @src 1402b7520  this + 0x58
DieEdgeSets& die_edge_sets(void* geo3d)
{
    return *reinterpret_cast<DieEdgeSets*>(&static_cast<OBJ_GEO3D*>(geo3d)->u58);
}

}  // namespace def::model::geo_kw
