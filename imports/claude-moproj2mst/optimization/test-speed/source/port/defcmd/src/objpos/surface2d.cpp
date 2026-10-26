// defcmd/src/objpos/surface2d.cpp: the 2D branch of 1401fc730 (geometry 0 rebuilt from the mesh),
// packet p67-1. No spec document covers it; the header comments (objpos_centre.hpp,
// defmodel/obj_mesh2d/topology.hpp) describe it. Decompile / listing:
// deform-rea-work/out/w2/p67-1-2d/d0.c, d3.c, lst.txt.
//
// 1401fc730 (2D) picks 1401fcd50 when an element repeats a node (1401fcc50), else 1401fd240:
//   * 1401fcd50 turns each boundary record of the object (obj+0xbd8 records at obj+0xbe0, 0x30
//     bytes: node count, node list at +8) into the point curve of edge b of geometry 0;
//   * 1401fd240 copies the mesh into a new OBJ_MESH2D, builds its boundary topology and makes it
//     the topology of geometry 0 (one face per material region, one loop per boundary loop, one
//     edge per boundary curve between vertices where curves meet).
#include <defcmd/contact_cmds.hpp>
#include <defcmd/objpos.hpp>
#include <defcmd/objpos_2d_tree.hpp>
#include <defcmd/objpos_centre.hpp>

#include <cstdint>
#include <cstring>

#include <defmodel/geo_kw/geo_kw.hpp>
#include <defmodel/layout/geo2d_and_mesh2d.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/layout/obj_geo2d.hpp>
#include <defmodel/layout/obj_mesh2d.hpp>
#include <defmodel/llobj/cls_140182880.hpp>
#include <defmodel/mesh2d/geo2d_mesh_ops.hpp>
#include <defmodel/obj_mesh2d/mesh_access.hpp>
#include <defmodel/obj_mesh2d/topology.hpp>
#include <defrt/port.hpp>
#if __has_include(<defdbio/dbread/w1.hpp>)
#include <defdbio/dbread/w1.hpp>
#endif
#include <defdbio/gendb3d/obj3d.hpp>

#include "../contact/contact_ext.hpp"
#include "internal.hpp"

namespace def::cmd::objpos {

using model::GEO2D_AND_MESH2D;
using model::LLOBJ_DATA;
using model::OBJ_GEO2D;
using model::OBJ_MESH2D;
namespace gk = model::geo_kw;
using detail::at;

namespace {

// 14027fe20 is p-dbread's geo2d_set_edge_count and 140198230 its set_curve_count
// (defdbio/dbread/w1.hpp); local bodies until that merges
#if __has_include(<defdbio/dbread/w1.hpp>)
const Surface2dExt k_defaults = {&dbio::dbread::geo2d_set_edge_count, &dbio::dbread::set_curve_count};
#else
[[noreturn]] void x_fun_14027fe20(GEO2D_AND_MESH2D*, const std::int32_t*) { DEF_NOT_PORTED(0x14027fe20); }

void local_140198230(LLOBJ_DATA* o, std::int32_t n);
const Surface2dExt k_defaults = {x_fun_14027fe20, local_140198230};
#endif
Surface2dExt g_ext = k_defaults;

// 140183d00  element set material group per element: owner defdbio gendb3d/node_elem.cpp
std::int32_t* element_set_groups(void* set) { return dbio::g3d::element_material_groups(set); }

} // namespace

Surface2dExt& surface2d_ext() { return g_ext; }
void reset_surface2d_ext() { g_ext = k_defaults; }

// 140198230  edge count of geometry 0 (owner p-dbread W1): p-dbread's dbread::set_curve_count when present,
//                 else local_140198230 (through surface2d_ext so tests can count the calls)
void fun_140198230(LLOBJ_DATA* o, std::int32_t n) { g_ext.fun_140198230(o, n); }

#if !__has_include(<defdbio/dbread/w1.hpp>)
namespace {
// fallback body of 140198230
void local_140198230(LLOBJ_DATA* o, std::int32_t n)
{
    std::int32_t v = n;
    GEO2D_AND_MESH2D* g = gk::geometry(*o, 0);  // 140197f40
    g_ext.fun_14027fe20(g, &v);
}
} // namespace
#endif

// @src 1401fcc50  2D mesh with an element that repeats a node
bool fun_1401fcc50(LLOBJ_DATA* o)
{
    bool degenerate = false;  // local_10
    if (detail::owner_is_2d(o)) {
        if (at<std::int32_t>(o, 0x87c) == 0) return false;
        const std::int32_t* conn = at<std::int32_t*>(o, 0x9e8);
        for (std::int32_t e = 0; e < at<std::int32_t>(o, 0x884); ++e) {
            for (std::int32_t i = 0; i < 4; ++i) {
                for (std::int32_t k = i + 1; k < 4; ++k) {
                    if (conn[i + e * 4] == conn[k + e * 4]) degenerate = true;
                }
            }
        }
    }
    return degenerate;
}

// @src 1401fcd50  geometry from the boundary records (2D) / temporary surface (3D)
void fun_1401fcd50(LLOBJ_DATA* o, char* compact)
{
    if (!detail::owner_is_2d(o)) {
        surface_from_mesh_3d(o, compact);
        return;
    }
    std::int32_t* rec = nullptr;      // [0x40]
    OBJ_GEO2D* cv = nullptr;          // [0x58]
    std::int32_t b = 0;               // [0x2c]
    std::int32_t j = 0;               // [0x50]
    std::int32_t a = 0;               // [0x28]
    std::int32_t nx = 0;              // [0x48]
    std::int32_t k = 0;               // [0x4c]
    std::int32_t n = 0;               // [0x34]
    std::int32_t done = 0;            // [0x30]
    set_surface_slots(o, at<std::int32_t>(o, 0xbd8));  // 140163340
    if (0 < at<std::int32_t>(o, 0xbd8) && 0 < at<std::int32_t>(o, 0x880)) {
        const unsigned char* set = detail::ptr(o, 0x8a0);
        if (set == nullptr) return;
        const double* xy = at<double*>(o, 0x8a8);  // [0x20]
        if (xy == nullptr) return;
        done = 0;
        for (b = 0; b < at<std::int32_t>(o, 0xbd8); ++b) {
            rec = reinterpret_cast<std::int32_t*>(detail::ptr(o, 0xbe0 + static_cast<std::int64_t>(b) * 0x30));
            const std::int32_t* list = *reinterpret_cast<std::int32_t**>(rec + 2);
            if (list == nullptr) continue;
            n = *rec;
            if (n < 1) continue;
            cv = gk::curve(*o, b);  // 1401982c0(o, &b)
            // the binary loops here without fetching the curve again
            while (cv == nullptr) gk::geometry(*o, 0)->add_edge_curve();  // 140197f40, 14027fce0
            cv->curve_type = 1;
            cv->n_points = n;
            cv->clear_data();       // slot 3 (+0x18)
            k = 0;
            for (j = 0; j < *rec; ++j) {
                a = list[j] - 1;
                if (j == *rec - 1) {
                    nx = list[0] - 1;
                } else {
                    nx = list[j + 1] - 1;
                }
                if (at<std::int32_t>(o, 0x880) <= a) a = at<std::int32_t>(o, 0x880) - 1;
                if (at<std::int32_t>(o, 0x880) <= nx) nx = at<std::int32_t>(o, 0x880) - 1;
                a = a << 1;
                nx = nx << 1;  // not read
                cv->points[k] = xy[a];
                k = k + 1;
                cv->points[k] = xy[a + 1];
                k = k + 1;
                cv->points[k] = 0.0;
                k = k + 1;
            }
            done = done + 1;
        }
    }
}

// @src 1401fd240  geometry 0 from the mesh topology
void fun_1401fd240(LLOBJ_DATA* o, char* /*compact*/)
{
    GEO2D_AND_MESH2D* g = gk::geometry(*o, 0);  // 140197f40
    if (g == nullptr) return;
    if (!detail::owner_is_2d(o)) {
        if (model::fun_1402815a0(g)) {
            OBJ_MESH2D* m = model::fun_140220430(g);
            objpos::fun_1403dedd0(m);  // C11 root (objpos_2d_tree.hpp)
            model::fun_1403e0f20(m, g);
            model::fun_1403dee60(m);
        }
    } else if (at<std::int32_t>(o, 0x87c) != 0) {
        OBJ_MESH2D* m = new OBJ_MESH2D();  // operator new(0x1700), 1403ddf20
        model::fun_1403de680(m, &at<std::int32_t>(o, 0x880));
        std::int32_t four = 4;
        model::fun_1403de6e0(m, &at<std::int32_t>(o, 0x884), &four);
        double* dxy = model::fun_140220440(m);
        std::int32_t* dconn = model::fun_140220460(m);
        const std::int32_t* conn = element_set_conn(detail::ptr(o, 0x9e0));  // 140183cb0
        const double* xy = node_set_coords(detail::ptr(o, 0x8a0));            // 1401808d0
        std::memcpy(dxy, xy, static_cast<std::size_t>(static_cast<std::int64_t>(at<std::int32_t>(o, 0x880) << 1) << 3));
        std::memcpy(dconn, conn, static_cast<std::size_t>(static_cast<std::int64_t>(at<std::int32_t>(o, 0x884) << 2) << 2));
        std::int32_t* dgrp = model::fun_140220480(m);
        const std::int32_t ne = at<std::int32_t>(o, 0x884);
        const std::int32_t* grp = element_set_groups(detail::ptr(o, 0x9e0));  // 140183d00
        std::memcpy(dgrp, grp, static_cast<std::size_t>(static_cast<std::int64_t>(ne) << 2));
        objpos::fun_1403dedd0(m);  // C11 root (objpos_2d_tree.hpp)
        model::fun_1403e0f20(m, g);
        const char off = 0;
        model::geo2d_set_b8(g, &off);  // 1402847d0
        model::fun_1403dee60(m);
        delete m;                      // virtual slot 0, flag 1
    }
}

} // namespace def::cmd::objpos
