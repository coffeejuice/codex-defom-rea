// defmodel/geo_kw/geo_kw.hpp: object-level geometry and mesh access used by the kw-g2geo keyword
// readers and writers (packet kw-g2geo). Model code lives in defmodel/src/geo_kw/.
//
//   geometry list   LLOBJ_DATA+0x3998: pointer to a COW list (8-byte handle) of GEO2D_AND_MESH2D
//                   (ctor 140301950, dtor 140301990, clear 140301d10, append default 140301ad0,
//                   entry i 140301b40). Geometry 0 holds the object's curves (DIEGEO/TPLOGY/CSPMTX).
//   surfaces        LLOBJ_DATA+0x3c00: 100 OBJ_GEO3D* (new 0x130 + ctor 1402f9020 in 14015f0b0);
//                   surface 0 holds the DIEGEO surface form, POLYTP, DIEPNT, DIEEDG.
//   FLDGEO/FLWNET   the embedded OBJ_GEO3D at +0x3328 / +0x3458.
//
// Family headers next to this one: curve_model.hpp, surface_model.hpp, mesh_model.hpp,
// boundary.hpp (one owner each, see the packet's status file).
#pragma once

#include <cstdint>

#include <defmodel/sdctl/cow.hpp>

namespace def::model {
class LLOBJ_DATA;
class GEO2D_AND_MESH2D;
class OBJ_GEO2D;
class OBJ_GEO3D;
}  // namespace def::model

namespace def::model::geo_kw {

using GeoList = CowList<GEO2D_AND_MESH2D>;

// ---- geometry list (LLOBJ_DATA+0x3998) -------------------------------------------------------
// @src 140301950  ctor of the list object: empty list, then one default entry (140301ad0)
GeoList* new_geo_list();
// @src 14019a670  scalar deleting dtor (140301990: clear the body, release the handle) + free
void delete_geo_list(GeoList* l);
// @src 140301d10  clear: refs == 1 -> delete the nodes; else release and take a new empty body
void geo_list_clear(GeoList* l);
// @src 140301ad0  append a copy of a default GEO2D_AND_MESH2D; returns the new entry
GEO2D_AND_MESH2D* geo_list_append_default(GeoList* l);
// @src 140301b40  entry i (detaching); null when the list is empty or i < 0 or i > count. i == count
//                 is accepted by the original and yields the sentinel's payload; returned as null here.
GEO2D_AND_MESH2D* geo_list_at(GeoList* l, std::int32_t i);
// @src 140301c60  number of entries
std::int32_t geo_list_count(GeoList* l);

GeoList* geo_list(LLOBJ_DATA& obj);
// @src 140197e40  create the list when obj+0x3998 is null
void ensure_geo_list(LLOBJ_DATA& obj);
// @src 140197f40  geometry i: null when obj+0x3998 is null, else geo_list_at
GEO2D_AND_MESH2D* geometry(LLOBJ_DATA& obj, std::int32_t i);
// @src 1401982c0  curve i of geometry 0 (14027ff20: the _OBJ_GEO2D of edge i); null when absent
OBJ_GEO2D* curve(LLOBJ_DATA& obj, std::int32_t i);
// @src 140198310  edge count of geometry 0 (0 without geometry) (also 1401983a0, same code)
std::int32_t curve_count(LLOBJ_DATA& obj);

// ---- predicates the object writer and the readers use ----------------------------------------
// @src 140198050  geometry i has at least one edge (140301b40 then 140159160; the list pointer is
//                 not checked for null in the original)
bool has_curve(LLOBJ_DATA& obj, std::int32_t i);
// @src 140160d90  byte obj+0x3990 ("has surface")
bool has_surface_flag(LLOBJ_DATA& obj);
// @src 140160d20  curve_count(obj) > 0
bool geometry_has_edges(LLOBJ_DATA& obj);
// part of @src 14029e250  curve 0 exists and its type (+0x10) != 0
bool curve_type0_nonzero(LLOBJ_DATA& obj);
// @src 140160e50  1402f9360 on obj+0x3328: node_count > 0 && elem_count > 0
bool has_fldgeo(LLOBJ_DATA& obj);
// @src 140160eb0  the same on obj+0x3458
bool has_flwnet(LLOBJ_DATA& obj);
// @src 1402f9360  node_count > 0 && elem_count > 0
bool surface_has_mesh(OBJ_GEO3D* s);

// ---- surfaces (LLOBJ_DATA+0x3c00) ---------------------------------------------------------------
// @src 140198620  surface i (obj+0x3c00[i]), no range check
OBJ_GEO3D* surface(LLOBJ_DATA& obj, std::int32_t i);
// @src 140198600  obj+0x3994 = v
void set_surface_count(LLOBJ_DATA& obj, std::int32_t v);
// @src 1401985e0  byte obj+0x3990 = v
void set_surface_flag(LLOBJ_DATA& obj, bool v);
// @src 140158ce0  obj+0x39c0 (an object-level dimension; 3 selects the 3D layouts in 2D models)
std::int32_t object_dim(LLOBJ_DATA& obj);
// @src 1401985a0  obj+0x3994
std::int32_t surface_count(LLOBJ_DATA& obj);



}  // namespace def::model::geo_kw
