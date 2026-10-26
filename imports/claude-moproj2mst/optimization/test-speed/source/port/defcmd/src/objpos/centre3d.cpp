// defcmd/src/objpos/centre3d.cpp: own centre (OBJPOS mode 6) and the 3D path: the temporary
// surface built from the mesh and the call of the mass properties (p67-3, drop/mode8).
// Spec: OBJPOS.md 7.4, OBJPOS_geo_kernels.md 2. Decompile: deform-rea-work/out/p67/objpos.c,
// objpos-geo/geo.c.
#include <defcmd/objpos.hpp>
#include <defcmd/drop/mode8.hpp>
#include <defcmd/objpos_centre.hpp>

#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <vector>

#include <defmodel/geo_kw/geo_kw.hpp>
#include <defmodel/geo_kw/surface_model.hpp>
#include <defmodel/layout/obj_geo3d.hpp>
#include <defmodel/llobj_ctor/llobj_copy.hpp>
#include <defrt/port.hpp>
#include <defrt/stdlib/legacy_printf.hpp>

#include "internal.hpp"

namespace def::cmd::objpos {

using model::LLOBJ_DATA;
using model::OBJ_GEO3D;
namespace gk = model::geo_kw;
using detail::at;

// @src 1401ff360  own centre: 3D 1401fe7d0 (axis into a discarded local), 2D 1401ff4f0
bool own_centre(LLOBJ_DATA* o, double* c)
{
    bool r;
    if (!detail::owner_is_2d(o)) {
        double axis[3] = {0.0, 0.0, 0.0};
        r = centre_3d(o, c, axis);
    } else {
        r = centre_2d(o, c);
    }
    return r;
}

// 1401fe7d0: ported once by p67-3 (drop::axis_point, defcmd/drop/mode8.hpp: CENTER and AXIS,
// the whole 1403e8510 closure); OBJPOS uses only the centre.
bool centre_3d(LLOBJ_DATA* o, double* c, double* axis)
{
    double ax[3] = {0.0, 0.0, 0.0};
    return drop::axis_point(o, c, axis != nullptr ? axis : ax);
}

// The 3D body of 1401fc730; 1401fcd50 has the same code inlined for its 3D branch.
void surface_from_mesh_3d(LLOBJ_DATA* o, char* compact)
{
    set_surface_slots(o, at<std::int32_t>(o, 0xbd8));
    const std::int32_t nnode = at<std::int32_t>(o, 0x880);
    const std::int32_t nface = at<std::int32_t>(o, 0xbf0);  // boundary record 0: face count
    if (0 < nnode && 0 < nface) {
        size_surface(o, nnode, nface, 0);
        double* dst = gk::geo3d_coords(gk::surface(*o, 0));
        std::int32_t* dstf = gk::geo3d_elems(gk::surface(*o, 0));
        std::memcpy(dst, at<double*>(o, 0x8a8),
                    static_cast<std::size_t>(static_cast<std::int64_t>(nnode * 3) << 3));
        const std::int32_t n = gk::surface(*o, 0)->elem_count();
        std::memcpy(dstf, at<std::int32_t*>(o, 0xbf8),
                    static_cast<std::size_t>(static_cast<std::int64_t>(n << 2) << 2));
        gk::set_surface_flag(*o, true);  // 1401985e0
        if (*compact != 0) surface_compact_nodes(gk::surface(*o, 0));
    }
}

// @src 1401fc730  geometry from the mesh: 3D temporary surface, 2D 1401fcd50 / 1401fd240
void fun_1401fc730(LLOBJ_DATA* o, char* compact)
{
    if (!detail::owner_is_2d(o)) {
        surface_from_mesh_3d(o, compact);
    } else if (fun_1401fcc50(o)) {
        fun_1401fcd50(o, compact);
    } else {
        fun_1401fd240(o, compact);
    }
}

// @src 1401ffed0  free the temporary surface(s)
void fun_1401ffed0(LLOBJ_DATA* o)
{
    for (std::int32_t i = 0; i < gk::surface_count(*o); ++i) {
        gk::surface(*o, i)->clear();  // 1402f9340
        gk::surface(*o, i)->reset();  // virtual slot 2 (+0x10)
    }
    gk::set_surface_count(*o, 0);
    gk::set_surface_flag(*o, false);
}

// @src 140163340  surface slots (3D): clear slots n..count-1 when n > 0, count = min(n, 100);
//                 2D: edge count of geometry 0 (140198230)
void set_surface_slots(LLOBJ_DATA* o, std::int32_t n)
{
    if (detail::owner_is_3d(o)) {
        if (0 < n) {
            for (std::int32_t i = n; i < gk::surface_count(*o); ++i) {
                auto* s = static_cast<OBJ_GEO3D*>(o->u3c00[i]);
                s->clear();  // 1402f9340
                s->reset();  // virtual slot 2
            }
        }
        at<std::int32_t>(o, 0x3994) = n > 100 ? 100 : n;
    } else if (detail::owner_is_2d(o)) {
        fun_140198230(o, n);
    }
}

// @src 1402009f0  size surface bdry, or the error message
void size_surface(LLOBJ_DATA* o, std::int32_t nnode, std::int32_t nface, std::int32_t bdry)
{
    if (bdry < gk::surface_count(*o)) {
        gk::geo3d_size(gk::surface(*o, bdry), &nnode, &nface);  // 1402fcd90
    } else {
        rt::crt::printf("ERROR: iBdryNo = %d, larger than NoGeoBdry = %d\n", bdry, gk::surface_count(*o));
    }
}

// @src 1402fc1b0  keep only the nodes that faces use, renumbered in node order
void surface_compact_nodes(OBJ_GEO3D* s)
{
    const std::int32_t n = s->node_count();
    const std::int32_t ne = s->elem_count();
    auto* map = static_cast<std::int32_t*>(model::llobj_copy::calloc_int(n));  // 140256dc0
    std::int32_t used = 0;   // local_20
    std::int32_t full = 0;   // local_40: faces whose 4 nodes are in 1..n
    for (std::int32_t e = 0; e < ne; ++e) {
        std::int32_t ok = 0;
        for (std::int32_t k = 0; k < 4; ++k) {
            const std::int32_t v = s->elems.mutable_at(static_cast<std::size_t>(k + e * 4));  // 140005100
            if (0 < v && v <= n) {
                ++ok;
                map[v - 1] = 1;
            }
        }
        if (ok == 4) ++full;
    }
    for (std::int32_t i = 0; i < n; ++i) {
        if (map[i] == 1) {
            ++used;
            map[i] = used;
        }
    }
    if (used == n) {
        if (map != nullptr) std::free(map);
        return;
    }
    OBJ_GEO3D copy;      // 1402f9020 on local_188
    copy.assign(s);      // 140199c20 -> 1402f9b60
    if (used == 0 || full == 0) {
        s->clear_nodes();  // 1402f9670
        s->clear_elems();  // 1402f9710
        return;            // copy destroyed (1402f91f0); map is not freed in this branch (leak kept)
    }
    s->resize_nodes(&used);  // 1402f9600
    s->resize_elems(&full);  // 1402f96a0
    const double* cp = gk::geo3d_coords(&copy);
    for (std::int32_t i = 0; i < n; ++i) {
        const std::int32_t v = map[i];
        if (0 < v && v <= n) {
            std::memcpy(&s->coords.mutable_at(static_cast<std::size_t>((v - 1) * 3)),  // 140005e70
                        cp + static_cast<std::int64_t>(i * 3), 0x18);
        }
    }
    const std::int32_t* ce = gk::geo3d_elems(&copy);
    for (std::int32_t e = 0; e < ne; ++e) {
        for (std::int32_t k = 0; k < 4; ++k) {
            const std::int32_t v = ce[k + e * 4];
            if (0 < v && v <= n) {
                const auto idx = static_cast<std::size_t>(k + e * 4);
                // the binary writes past the resized array when full < ne (faces with nodes out
                // of range); the port skips those writes
                if (idx < s->elems.size()) s->elems.mutable_at(idx) = map[v - 1];
            }
        }
    }
    if (map != nullptr) std::free(map);
}

} // namespace def::cmd::objpos
