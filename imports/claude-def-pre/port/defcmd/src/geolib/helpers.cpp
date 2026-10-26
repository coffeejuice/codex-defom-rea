// geolib: shared model helpers of packet p8-09 (string append, DB_STEP_DATA deleting dtor,
// geometry/mesh set-up, dot product, double realloc).
// Decompile / listing: deform-rea-work/out/p809/all.c, dep.c, all_l.txt.
#include <defcmd/geolib.hpp>

#include <cstdlib>
#include <cstring>
#include <iterator>
#include <list>
#include <new>
#include <vector>

#include <defdbio/dbread/w1.hpp>
#include <defdbio/dbread/w3.hpp>
#include <defmodel/bc/edge_face.hpp>
#include <defmodel/bc/node_arrays.hpp>
#include <defmodel/geo/pending.hpp>
#include <defmodel/geo_kw/curve_model.hpp>
#include <defmodel/geo_kw/geo_kw.hpp>
#include <defmodel/geo_kw/mesh_model.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/geo2d_and_mesh2d.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/llobj/cls_14017e740.hpp>
#include <defmodel/llobj_ctor/obj_defaults.hpp>
#include <defmodel/mesh2d/geo2d_mesh_ops.hpp>
#include <defrt/port.hpp>
#include <defrt/stdlib/refcount.hpp>

#include "ext_p67.hpp"
#include "internal.hpp"

namespace def::cmd::geolib {

namespace gk = def::model::geo_kw;
using detail::at;

// @src 140158fc0  DB_STEP_DATA scalar deleting dtor
void* fun_140158fc0(def::model::DB_STEP_DATA* step, std::uint32_t flags)
{
    step->~DB_STEP_DATA();  // 1401cb2c0
    if ((flags & 1u) != 0) ::operator delete(static_cast<void*>(step));
    return step;
}

// 140280290: DBREAD's port owns it (defdbio/src/dbread/w1_geo.cpp geo2d_remove_edges)
bool fun_140280290(def::model::GEO2D_AND_MESH2D* g, const std::int32_t* idx, const std::int32_t* n)
{
    return def::dbio::dbread::geo2d_remove_edges(g, const_cast<std::int32_t*>(idx), n);  // idx only read
}

// 14027fe20: DBREAD's port owns it (defdbio/src/dbread/w1_geo.cpp geo2d_set_edge_count)
void fun_14027fe20(def::model::GEO2D_AND_MESH2D* g, const std::int32_t* n) { def::dbio::dbread::geo2d_set_edge_count(g, n); }

// 140198230: DBREAD's port owns it (defdbio/src/dbread/w1_objects.cpp set_curve_count)
void fun_140198230(LLOBJ_DATA* obj, std::int32_t n) { def::dbio::dbread::set_curve_count(obj, n); }

// @src 140198270
void fun_140198270(LLOBJ_DATA* obj)
{
    if (gk::geometry(*obj, 0) == nullptr) {
        gk::geo_list_append_default(at<gk::GeoList*>(obj, 0x3998));  // 140301ad0, no null test
    }
    gk::geometry(*obj, 0)->add_edge_curve();  // 14027fce0
}

// @src 1401ffd80
void fun_1401ffd80(LLOBJ_DATA* obj)
{
    for (std::int32_t i = 0; i < at<std::int32_t>(obj, 0xbd8); i = i + 1) {
        auto* e = reinterpret_cast<def::model::Cls_14017e740*>(reinterpret_cast<std::byte*>(obj) + 0xbe0 +
                                                               static_cast<std::int64_t>(i) * 0x30);
        e->free_arrays();  // 14017e800
        e->init();         // 14017e7b0
    }
    void* owner = at<void*>(obj, 0x3a98);
    if (owner != nullptr && def::rt::dimension_is_2(owner)) {  // 1400015e0
        def::model::bc::free_edge_face_arrays(*obj);  // 1401632d0
        at<std::int32_t>(obj, 0x88c) = 0;
    }
    def::model::bc::free_node_arrays(*obj);  // 140163270
    at<std::int32_t>(obj, 0x880) = 0;
    def::model::bc::free_element_arrays(*obj);  // 1401632a0
    at<std::int32_t>(obj, 0x884) = 0;
    at<std::int32_t>(obj, 0x87c) = 0;
}

// @src 1402029e0
void fun_1402029e0(LLOBJ_DATA* obj, std::int32_t group)
{
    at<std::int32_t>(obj, 0x3980) = group;
}

// @src 1402027e0
void fun_1402027e0(LLOBJ_DATA* obj, std::int32_t group, bool only_old, bool apply)
{
    std::int32_t old = at<std::int32_t>(obj, 0x3980);
    fun_1402029e0(obj, group);
    // obj + 0x9e0 != 0 is always true (the 0x9e0 set is embedded)
    if (apply && at<std::int32_t>(obj, 0x87c) != 0 && at<std::int32_t>(obj, 0x884) != 0 &&
        at<std::int32_t*>(obj, 0x9f8) != nullptr) {
        const std::int32_t n = at<std::int32_t>(obj, 0x884);
        std::int32_t* g = at<std::int32_t*>(obj, 0x9f8);
        if (!only_old) {
            for (std::int32_t i = 0; i < n; i = i + 1) g[i] = group;
        } else {
            std::int32_t count = 0;
            std::int32_t most = 0;
            if (old == 0) {
                gk::material_groups_counted(*obj, &count, &most);  // 1401fb440
                old = most;
            }
            for (std::int32_t i = 0; i < n; i = i + 1) {
                if (g[i] == old) g[i] = group;
            }
        }
    }
}

// @src 140202700
void fun_140202700(LLOBJ_DATA* obj, std::int32_t nnode, std::int32_t nelem, std::int32_t elem_type,
                   const std::int32_t* dims, std::int32_t dim)
{
    void* owner = at<void*>(obj, 0x3a98);
    if (dim == 0 && owner != nullptr) {
        const std::int32_t d = *reinterpret_cast<const std::int32_t*>(static_cast<const char*>(owner) + 0x1c);  // 140158e90
        def::model::llobj_ctor::set_u39c0(reinterpret_cast<std::byte*>(obj), d);  // 140199af0
    } else {
        def::model::llobj_ctor::set_u39c0(reinterpret_cast<std::byte*>(obj), dim);
    }
    std::int32_t count = 0;
    std::int32_t group = 0;
    gk::material_groups(*obj, &count, &group);  // 1401fb270
    at<std::int32_t>(obj, 0x87c) = 1;
    gk::set_element_type(*obj, elem_type);                 // 140165500
    def::model::bc::set_node_count(*obj, nnode, dims);     // 1401652b0
    def::model::bc::set_element_count(*obj, nelem, dims);  // 1401653b0
    if (group != 0) fun_1402027e0(obj, group, false, true);
}

// @src 14023a380  (a0*b0 + a1*b1) + a2*b2
double fun_14023a380(const double* a, const double* b)
{
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

// 1402571b0: DBREAD's port owns it (defdbio/src/dbread/w3_arrays.cpp realloc_doubles)
double* fun_1402571b0(double* p, std::int32_t n) { return def::dbio::dbread::realloc_doubles(p, n); }

} // namespace def::cmd::geolib
