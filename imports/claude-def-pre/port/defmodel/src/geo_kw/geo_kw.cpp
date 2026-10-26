// kw-g2geo: object-level geometry access (defmodel/geo_kw/geo_kw.hpp).
// Decompiles: deform-rea-work/out/g2geo/glist.c, pred.c, objdef.c, todo_all.c.
#include <defmodel/geo_kw/geo_kw.hpp>

#include <cstddef>
#include <cstring>
#include <new>

#include <defmodel/geo/pending.hpp>
#include <defmodel/layout/geo2d_and_mesh2d.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/layout/obj_geo2d.hpp>
#include <defmodel/layout/obj_geo3d.hpp>
#include <defmodel/llobj/cls_140182880.hpp>
#include <defmodel/llobj/cls_14017e740.hpp>
#include <defmodel/llobj/cls_14017ef20.hpp>

namespace def::model::geo_kw {

namespace {

template <class T>
T& at(LLOBJ_DATA& o, std::size_t off)
{
    return *reinterpret_cast<T*>(reinterpret_cast<std::byte*>(&o) + off);
}

}  // namespace

// ---- geometry list ------------------------------------------------------------------------------

// @src 140301950  list ctor: body (140301d60: new 0x18, 140302290), then one default entry
// @src 140301d60  (handle ctor, inlined: CowList())
// @src 140302290  (body ctor, inlined: refcount 1, sentinel node holding a default GEO2D_AND_MESH2D, size 0)
GeoList* new_geo_list()
{
    auto* l = new GeoList();
    geo_list_append_default(l);
    return l;
}

// @src 14019a670  (p, 1): dtor 140301990 (clear 140302180, release 140301dd0), then free
// @src 140301990  dtor
void delete_geo_list(GeoList* l)
{
    if (l == nullptr) return;
    geo_list_clear(l);
    delete l;
}

// @src 140301d10  -> 140302180
// @src 140302180  refs == 1: free the nodes (140302860); else release (140001010) and a new body
void geo_list_clear(GeoList* l) { l->clear(); }

// @src 140301ad0  default entry (14027f6e0) copied in at the end (140301ef0: detach, end,
//                 140302410 insert with the node copy ctor 140302680), temp destroyed (14027f870)
GEO2D_AND_MESH2D* geo_list_append_default(GeoList* l)
{
    GEO2D_AND_MESH2D tmp;
    l->push_back(tmp);
    return &l->mutable_items().back();
}

// @src 140301c60  count (body + 0x10, 140301e70)
std::int32_t geo_list_count(GeoList* l) { return static_cast<std::int32_t>(l->size()); }

// @src 140301b40  entry i: count != 0 and 0 <= i <= count; 140301f50 detaches (140302090) and
//                 walks i nodes from head->next (1403025c0); 140302020 returns node + 0x10
GEO2D_AND_MESH2D* geo_list_at(GeoList* l, std::int32_t i)
{
    std::int32_t n = geo_list_count(l);
    if (n == 0 || i < 0 || n < i) return nullptr;
    if (i == n) return nullptr;  // the original returns the sentinel's payload; never reached by i == 0
    return &l->mutable_at(static_cast<std::size_t>(i));
}

GeoList* geo_list(LLOBJ_DATA& obj) { return static_cast<GeoList*>(obj.u3998); }

// @src 140197e40  create the list when obj+0x3998 is null (new 8, ctor 140301950)
void ensure_geo_list(LLOBJ_DATA& obj)
{
    if (obj.u3998 == nullptr) obj.u3998 = new_geo_list();
}

// @src 140197f40  geometry i
GEO2D_AND_MESH2D* geometry(LLOBJ_DATA& obj, std::int32_t i)
{
    if (obj.u3998 == nullptr) return nullptr;
    return geo_list_at(geo_list(obj), i);
}

// @src 1401982c0  curve i of geometry 0
// @src 14027ff20  (geometry, i): count < 1 or i out of range -> 0; edge i (140284ff0, detaching),
//                 dynamic_cast IEdge -> _DEF_EDGE, its curve (140284990, 140284960)
OBJ_GEO2D* curve(LLOBJ_DATA& obj, std::int32_t i)
{
    GEO2D_AND_MESH2D* g = geometry(obj, 0);
    if (g == nullptr) return nullptr;
    std::int32_t n = static_cast<std::int32_t>(g->edges.size());  // 140159190
    if (n < 1 || i < 0 || n <= i) return nullptr;
    return g->edge_curve(i);
}

// @src 140198310  edge count of geometry 0 (140159190)
// @src 1401983a0  the same code
std::int32_t curve_count(LLOBJ_DATA& obj)
{
    GEO2D_AND_MESH2D* g = geometry(obj, 0);
    if (g == nullptr) return 0;
    return static_cast<std::int32_t>(g->edges.size());
}

// ---- predicates ---------------------------------------------------------------------------------

// @src 140198050  140301b40(obj+0x3998, i) then 140159160 (edge count > 0)
bool has_curve(LLOBJ_DATA& obj, std::int32_t i)
{
    GEO2D_AND_MESH2D* g = geo_list_at(geo_list(obj), i);  // the list pointer is not checked
    if (g == nullptr) return false;
    return 0 < static_cast<std::int32_t>(g->edges.size());
}

// @src 140160d90
bool has_surface_flag(LLOBJ_DATA& obj) { return obj.u3990 != 0; }

// @src 140160d20  0 < 1401983a0(obj)
bool geometry_has_edges(LLOBJ_DATA& obj) { return 0 < curve_count(obj); }

// part of @src 14029e250  1401982c0(obj, 0) != 0 and its +0x10 (curve type) != 0
bool curve_type0_nonzero(LLOBJ_DATA& obj)
{
    OBJ_GEO2D* c = curve(obj, 0);
    if (c == nullptr) return false;
    return c->curve_type != 0;
}

// @src 1402f9360
bool surface_has_mesh(OBJ_GEO3D* s) { return 0 < s->node_count() && 0 < s->elem_count(); }

// @src 140160e50
bool has_fldgeo(LLOBJ_DATA& obj) { return surface_has_mesh(reinterpret_cast<OBJ_GEO3D*>(obj.s3328)); }

// @src 140160eb0
bool has_flwnet(LLOBJ_DATA& obj) { return surface_has_mesh(reinterpret_cast<OBJ_GEO3D*>(obj.s3458)); }

// ---- surfaces -------------------------------------------------------------------------------------

// @src 140198620
OBJ_GEO3D* surface(LLOBJ_DATA& obj, std::int32_t i) { return static_cast<OBJ_GEO3D*>(obj.u3c00[i]); }

// @src 140198600
void set_surface_count(LLOBJ_DATA& obj, std::int32_t v) { at<std::int32_t>(obj, 0x3994) = v; }

// @src 1401985a0
std::int32_t surface_count(LLOBJ_DATA& obj) { return at<std::int32_t>(obj, 0x3994); }

// @src 1401985e0
void set_surface_flag(LLOBJ_DATA& obj, bool v) { obj.u3990 = v ? 1 : 0; }

// @src 140158ce0
std::int32_t object_dim(LLOBJ_DATA& obj) { return at<std::int32_t>(obj, 0x39c0); }

}  // namespace def::model::geo_kw
