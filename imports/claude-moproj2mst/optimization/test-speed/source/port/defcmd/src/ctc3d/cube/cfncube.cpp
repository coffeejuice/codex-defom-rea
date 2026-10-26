// cfnCUBE entry points GENCTC 3D calls: rebuild, polygon connectivity, point coordinates,
// attribute access (NI/NR/EI/ER), unique polygon list, nearest point, initial radius, cell sizes
// (packet p67-7, group "cube"; GENCTC_3d_search.md sec. 3.1, 4.3-4.5). The table is p67-2's
// (cube::g_cubes / g_cube_used, the binary's DAT_1406fe4f0 / DAT_1406fe4fc).
#include <defcmd/ctc3d/cube.hpp>

#include <cmath>
#include <cstdarg>
#include <cstdio>
#include <cstring>

#include <defrt/stdlib/legacy_printf.hpp>

namespace def::cmd::ctc3d {

namespace {

using namespace def::cmd::cube;
namespace crt = def::rt::crt;

// part of @src 14039d3a0  the warning print with integer arguments (p67-2's fun_14039d3a0 is typed
// for the allocation messages): 14039d570 = vsprintf to 4096 bytes, strcat "\n" (1406111c4),
// fflush(stdout), fputs to stdout, fflush(NULL)
void warn(const char* fmt, ...)
{
    char buf[4096];
    std::va_list ap;
    va_start(ap, fmt);
    crt::vsnprintf(buf, sizeof buf, fmt, ap);
    va_end(ap);
    const std::size_t len = std::strlen(buf);
    buf[len] = '\n';
    buf[len + 1] = '\0';
    std::fflush(stdout);
    std::fputs(buf, stdout);
    std::fflush(nullptr);
}

// first record of (obj, inst), -1 when none (the loop every entry point repeats)
inline i32 find_cube(i32 obj, i32 inst)
{
    for (i32 i = 0; i < g_cube_used; ++i) {
        if (g_cubes[i].id == obj && g_cubes[i].sub == inst) {
            return i;
        }
    }
    return -1;
}

// cell index of a coordinate clamped into the bounds (searches, eps 1e-8 = 14051bed0)
inline i32 search_index(const Locator* l, double x, i32 k)
{
    const double lo = l->box[k * 2];
    const double hi = l->box[k * 2 + 1];
    if (1e-08 < std::fabs(x - hi)) {
        return static_cast<i32>(((x - lo) / (hi - lo)) * static_cast<double>(l->n[k]));
    }
    return static_cast<i32>((((x - lo) * 0.999) / (hi - lo)) * static_cast<double>(l->n[k])); // 140522f68
}

inline double clamp_to(const Locator* l, double x, i32 k)
{
    if (l->box[k * 2] <= x) {
        return x <= l->box[k * 2 + 1] ? x : l->box[k * 2 + 1];
    }
    return l->box[k * 2];
}

inline CellList* cell_at(CellList* cells, const Locator* l, i32 ix, i32 iy, i32 iz)
{
    return cells + static_cast<std::int64_t>(ix + iy * l->n[0] + iz * l->n[0] * l->n[1]);
}

// set-up shared by 14022d770 and 14022fcd0: clamped point, its cell, R0 = sqrt((r0^2 + r1^2) + r2^2)
// with r_k the larger distance to the two bounds, minimum cell size, level count
struct SearchStart {
    double pc[3];
    i32 c[3];
    double r2;
    double r0;
    double hmin;
    i32 levels;
};

SearchStart search_start(const Locator* l, const double* p)
{
    SearchStart s{};
    for (i32 k = 0; k < 3; ++k) {
        s.pc[k] = clamp_to(l, p[k], k);
        s.c[k] = search_index(l, s.pc[k], k);
    }
    double rk[3];
    for (i32 k = 0; k < 3; ++k) {
        const double d0 = std::fabs(s.pc[k] - l->box[k * 2]);
        const double d1 = std::fabs(s.pc[k] - l->box[k * 2 + 1]);
        rk[k] = d0 <= d1 ? d1 : d0;
    }
    s.r2 = (rk[0] * rk[0] + rk[1] * rk[1]) + rk[2] * rk[2];
    s.r0 = std::sqrt(s.r2);
    i32 nmax = 0;
    s.hmin = 1e+29; // 14052c598
    for (i32 k = 0; k < 3; ++k) {
        s.hmin = (s.hmin < l->h[k] || s.hmin == l->h[k]) ? s.hmin : l->h[k];
        nmax = l->n[k] < nmax ? nmax : l->n[k];
    }
    const i32 cl = static_cast<i32>(std::ceil(s.r0 / s.hmin)); // 140486e8c
    s.levels = nmax < cl ? nmax : cl;
    return s;
}

inline bool on_shell(const i32* c, i32 lev, i32 ix, i32 iy, i32 iz)
{
    return ix == c[0] + lev || ix == c[0] - lev || iy == c[1] + lev || iy == c[1] - lev || iz == c[2] + lev ||
           iz == c[2] - lev;
}

} // namespace

// @src 1402260a0  140226150 with create = 0 (keeps the attribute lists)
void fun_1402260a0(i32* obj, i32* inst, i32* type, i32* np, double* x, i32* nf, i32* nv, i32* faces, i32* base, i32* verbose)
{
    i32 create = 0;
    def::cmd::cube::fun_140226150(obj, inst, type, np, x, nf, nv, faces, base, &create, verbose);
}

// @src 140227e00  cfnCUBE_Get_Elem_Conn: the polygon's node numbers minus base plus 1, count = nv
void fun_140227e00(i32* obj, i32* inst, i32* poly, i32* conn, i32* count)
{
    const i32 i = find_cube(*obj, *inst);
    if (i == -1) {
        warn("cfnCUBE_Get_Elem_Conn: the bucket of object(%d) and entity data set (%d) does not exist", *obj, *inst); // str 1405dfb80
        *count = 0;
        return;
    }
    Cube* c = &g_cubes[i];
    *count = fun_1404056e0(&c->polys);
    if (*count < 1) {
        return;
    }
    const i32* src = fun_140405720(&c->polys, *poly);
    if (src != nullptr) {
        std::memcpy(conn, src, static_cast<std::size_t>(static_cast<std::int64_t>(*count)) << 2);
        for (i32 k = 0; k < *count; ++k) {
            conn[k] = (conn[k] - c->loc.offset) + 1;
        }
    }
}

// @src 140227fc0  cfnCUBE_Get_Node_Coord
void fun_140227fc0(i32* obj, i32* inst, i32* point, double* xyz)
{
    const i32 i = find_cube(*obj, *inst);
    if (i == -1) {
        warn("cfnCUBE_Get_Node_Coord: the bucket of object(%d) and entity data set (%d) does not exist", *obj, *inst); // str 1405dfbf0
        return;
    }
    const double* src = fun_140406300(&g_cubes[i].points, *point);
    if (src != nullptr) {
        std::memcpy(xyz, src, 0x18);
    }
}

// @src 1402280d0  cfnCUBE_Init_Object_EI
void fun_1402280d0(i32* obj, i32* inst, i32* entity, i32* field, void* buf, i32* count)
{
    const i32 i = find_cube(*obj, *inst);
    if (i == -1) {
        warn("cfnCUBE_Init_Object_EI: the bucket of object(%d) and entity data set (%d) does not exist", *obj, *inst); // str 1405dfcc0
        return;
    }
    if (fun_140404560(&g_cubes[i].a190, *entity, *field, buf, *count) == 0) {
        warn("cfnCUBE_Init_Object_EI: Invalid Polygon/Element no (%d) \nObjectNo = %d, EntityNo = %d, DataSetNo = %d", *entity, *obj,
             *inst, *field); // str 1405dfc50
    }
}

// @src 140228260  cfnCUBE_Get_Object_EI
void fun_140228260(i32* obj, i32* inst, i32* entity, i32* field, void* buf, i32* count)
{
    const i32 i = find_cube(*obj, *inst);
    if (i == -1) {
        warn("cfnCUBE_Get_Object_EI: the bucket of object(%d) and entity data set (%d) does not exist", *obj, *inst); // str 1405dfd90
        *count = 0;
        return;
    }
    if (fun_1404045e0(&g_cubes[i].a190, *entity, *field, buf, count) == 0) {
        warn("cfnCUBE_Get_Object_EI: Invalid Polygon/Element no (%d) \nObjectNo = %d, EntityNo = %d, DataSetNo = %d", *entity, *obj,
             *inst, *field); // str 1405dfd20
    }
}

// @src 1402283b0  cfnCUBE_Init_Object_ER
void fun_1402283b0(i32* obj, i32* inst, i32* entity, i32* field, void* buf, i32* count)
{
    const i32 i = find_cube(*obj, *inst);
    if (i == -1) {
        warn("cfnCUBE_Init_Object_ER: the bucket of object(%d) and entity data set (%d) does not exist", *obj, *inst); // str 1405dfe80
        return;
    }
    if (fun_140404660(&g_cubes[i].a190, *entity, *field, buf, *count) == 0) {
        warn("cfnCUBE_Init_Object_ER: Invalid Polygon/Element no (%d) \nObjectNo = %d, EntityNo = %d, DataSetNo = %d", *entity, *obj,
             *inst, *field); // str 1405dfe10
    }
}

// @src 140228540  cfnCUBE_Get_Object_ER
void fun_140228540(i32* obj, i32* inst, i32* entity, i32* field, void* buf, i32* count)
{
    const i32 i = find_cube(*obj, *inst);
    if (i == -1) {
        warn("cfnCUBE_Get_Object_ER: the bucket of object(%d) and entity data set (%d) does not exist", *obj, *inst); // str 1405dff50
        *count = 0;
        return;
    }
    if (fun_1404046f0(&g_cubes[i].a190, *entity, *field, buf, count) == 0) {
        warn("cfnCUBE_Get_Object_ER: Invalid Polygon/Element no (%d) \nObjectNo = %d, EntityNo = %d, DataSetNo = %d", *entity, *obj,
             *inst, *field); // str 1405dfee0
    }
}

// @src 140228690  cfnCUBE_Init_Object_NI (a missing cube also sets *count = 0)
void fun_140228690(i32* obj, i32* inst, i32* entity, i32* field, void* buf, i32* count)
{
    const i32 i = find_cube(*obj, *inst);
    if (i == -1) {
        warn("cfnCUBE_Init_Object_NI: the bucket of object(%d) and entity data set (%d) does not exist", *obj, *inst); // str 1405e0020
        *count = 0;
        return;
    }
    if (fun_140404560(&g_cubes[i].a180, *entity, *field, buf, *count) == 0) {
        warn("cfnCUBE_Init_Object_NI: Invalid Node no (%d) \nObjectNo = %d, EntityNo = %d, DataSetNo = %d", *entity, *obj, *inst,
             *field); // str 1405dffc0
    }
}

// @src 140228830  cfnCUBE_Get_Object_NI
void fun_140228830(i32* obj, i32* inst, i32* entity, i32* field, void* buf, i32* count)
{
    const i32 i = find_cube(*obj, *inst);
    if (i == -1) {
        warn("cfnCUBE_Get_Object_NI: the bucket of object(%d) and entity data set (%d) does not exist", *obj, *inst); // str 1405e00e0
        *count = 0;
        return;
    }
    if (fun_1404045e0(&g_cubes[i].a180, *entity, *field, buf, count) == 0) {
        warn("cfnCUBE_Get_Object_NI: Invalid Node no (%d) \nObjectNo = %d, EntityNo = %d, DataSetNo = %d", *entity, *obj, *inst,
             *field); // str 1405e0080
    }
}

// @src 140228980  cfnCUBE_Init_Object_NR
void fun_140228980(i32* obj, i32* inst, i32* entity, i32* field, void* buf, i32* count)
{
    const i32 i = find_cube(*obj, *inst);
    if (i == -1) {
        warn("cfnCUBE_Init_Object_NR: the bucket of object(%d) and entity data set (%d) does not exist", *obj, *inst); // str 1405e01c0
        return;
    }
    if (fun_140404660(&g_cubes[i].a180, *entity, *field, buf, *count) == 0) {
        warn("cfnCUBE_Init_Object_NR: Invalid Node no (%d) \nObjectNo = %d, EntityNo = %d, DataSetNo = %d", *entity, *obj, *inst,
             *field); // str 1405e0160
    }
}

// @src 140228b10  cfnCUBE_Get_Object_NR (its missing-cube text says Get_Object_NI: str 1405e0280)
void fun_140228b10(i32* obj, i32* inst, i32* entity, i32* field, void* buf, i32* count)
{
    const i32 i = find_cube(*obj, *inst);
    if (i == -1) {
        warn("cfnCUBE_Get_Object_NI: the bucket of object(%d) and entity data set (%d) does not exist", *obj, *inst);
        *count = 0;
        return;
    }
    if (fun_1404046f0(&g_cubes[i].a180, *entity, *field, buf, count) == 0) {
        warn("cfnCUBE_Get_Object_NR: Invalid Node no (%d) \nObjectNo = %d, EntityNo = %d, DataSetNo = %d", *entity, *obj, *inst,
             *field); // str 1405e0220
    }
}

// @src 14022a5c0  cfnCUBE_GetPoly3_Obj: result list of the last box query -> unique ascending ids
// (bit map DAT_1406fe4e8 of DAT_1406fe4e4 bytes for DAT_1406fe4e0 polygons, kept between calls in
// the static image); the list is rewritten in place and copied to list
void fun_14022a5c0(i32* obj, i32* inst, void* list, std::uint32_t* n, i32* verbose)
{
    const i32 v = *verbose;
    const i32 i = find_cube(*obj, *inst);
    if (i == -1) {
        fun_14039d450("cfnCUBE_GetPoly3_Obj : Illegal combination of object number (%d) and entity set number (%d) \n", *obj,
                      *inst); // str 1405e0820
    }
    Cube* c = &g_cubes[i];
    const i32 ty = fun_1403ff870(&c->loc);
    if (ty != 1 && ty != 3 && ty != 5 && ty != 6) {
        fun_14039d450("defCUBE_GetPoly3_Obj : Locator type not for polygon seraching\n"); // str 1405e0880
    }
    if (v != 0) {
        std::fprintf(stdout, "...constructing polygon list from bucketdata list\n"); // str 1405e08c0
    }
    i32 cnt = fun_1404056c0(&c->result);
    i32* raw = nullptr;
    if (0 < cnt) {
        raw = fun_140405720(&c->result, 1);
        if (2 < v) {
            std::fprintf(stdout, "... NoPoly %d\npoly list\n\n", cnt); // str 1405e08f8
            for (i32 k = 0; k < cnt; ++k) {
                std::fprintf(stdout, "%d-", raw[k]); // str 1405e04fc
            }
            std::fprintf(stdout, "\n"); // str 1405e07d4
        }
        if (v != 0) {
            std::fprintf(stdout, "...finding unique polygons\n"); // str 1405e0918
        }
        const i32 npoly = fun_1404056c0(&c->polys);
        i32& bits = m<i32>(0x1406fe4e0);
        i32& nbytes = m<i32>(0x1406fe4e4);
        std::uint8_t*& map = m<std::uint8_t*>(0x1406fe4e8);
        const i32 need = (npoly + 7) / 8; // (x + (x >> 31 & 7)) >> 3
        if (map == nullptr) {
            nbytes = need;
            map = static_cast<std::uint8_t*>(fun_140256a60(nbytes));
            bits = npoly;
        } else if (bits < npoly) {
            nbytes = need;
            map = static_cast<std::uint8_t*>(fun_140256ac0(map, nbytes));
            bits = npoly;
        }
        const i32 nclear = need < nbytes ? need : nbytes;
        for (i32 k = 0; k < nclear; ++k) {
            map[k] = 0;
        }
        for (i32 k = 0; k < cnt; ++k) {
            const i32 b = raw[k] - 1;
            map[b / 8] = static_cast<std::uint8_t>(map[b / 8] | (0x80u >> ((b % 8) & 0x1f)));
        }
        i32 u = 0;
        for (i32 k = 0; k < npoly; ++k) {
            if ((static_cast<std::uint32_t>(map[k / 8]) & (0x80u >> ((k % 8) & 0x1f))) != 0) {
                raw[u] = k + 1;
                u = u + 1;
            }
        }
        cnt = u;
        if (0 < u) {
            std::memcpy(list, raw, static_cast<std::size_t>(static_cast<std::int64_t>(u)) << 2);
        }
    }
    *n = static_cast<std::uint32_t>(cnt);
    if (v != 0) {
        std::fprintf(stdout, "...leaving function, found %d elements\n", cnt); // str 1405e0938
    }
    if (2 < v) {
        std::fprintf(stdout, "... element list\n\n"); // str 1405e0960
        for (i32 k = 0; k < cnt; ++k) {
            std::fprintf(stdout, "%d-", raw[k]); // str 1405e0934
        }
        std::fprintf(stdout, "\n"); // str 1405e0974
    }
}

// @src 14022d770  cfnCUBE_SearchPoint_Obj (GENCTC_3d_search.md sec. 4.3): fewer than 16 points:
// brute force over all points; else shell search around the cell of the clamped point (distances
// from the CLAMPED point), stop when sqrt(best) < the distance to the next shell; the result
// distance is from the unclamped point
void fun_14022d770(i32* obj, i32* inst, double* p, double* dist, std::uint32_t* point, i32* verbose)
{
    const i32 e = *inst;
    const i32 v = *verbose;
    const i32 idx = find_cube(*obj, e);
    if (idx == -1) {
        fun_14039d450("cfnCUBE_SearchPoint_Obj : Illegal combination of object number (%d) and entity set number (%d) \n", *obj,
                      e); // str 1405e1290
    }
    Cube* r = &g_cubes[idx];
    const i32 ty = fun_1403ff870(&r->loc);
    if (ty != 0 && ty != 3 && ty != 6 && ty != 4) {
        fun_14039d450("defCUBE_SearchPoint_Obj : Locator type not for point searching\n"); // str 1405e1300
    }
    if (v != 0) {
        std::fprintf(stdout, "...cfnCUBE_SearchPoint_Obj\n"); // str 1405e1340
        std::fprintf(stdout, "...   cube  = %d\n", idx);      // str 1405e1278
    }
    Locator* l = &r->loc;
    if (fun_1404062e0(l->pts) < 16) {
        if (v != 0) {
            std::fprintf(stdout, "...brute force checking for %d points\n", fun_1404062e0(l->pts)); // str 1405e1360
        }
        i32 best = 1;
        const i32 np = fun_1404062e0(l->pts);
        double bestd = fun_140406540(p, fun_140406300(l->pts, 1));
        for (i32 i = 0; i < np; ++i) {
            const double d = fun_140406540(p, fun_140406300(l->pts, i + 1));
            if (d < bestd) {
                best = i + 1;
                bestd = d;
            }
        }
        const i32 checked = np;
        if (best == 0) {
            fun_14039d450("defCUBE_SearchPoint_Obj : Could not find a min distance point\n"); // str 1405e1388
        }
        *point = static_cast<std::uint32_t>(best);
        const double s = std::sqrt(fun_140406540(p, fun_140406300(l->pts, best)));
        *dist = s;
        if (v != 0) {
            crt::fprintf(stdout, "...min distance     %g at point %d\n", s, best);                   // str 1405e13c8
            std::fprintf(stdout, "...points checked   %d of %d\n", checked, fun_1404062e0(l->pts)); // str 1405e13f0
        }
        return;
    }
    const SearchStart s = search_start(l, p);
    double best2 = s.r2;
    if (1 < v) {
        crt::fprintf(stdout, "...   max distance for search  %g\n", s.r0);   // str 1405e1410
        crt::fprintf(stdout, "...   min distance of division %g\n", s.hmin); // str 1405e1438
        std::fprintf(stdout, "...   Insertion level          %d\n", s.levels); // str 1405e1460
    }
    i32 checked = 0;
    i32 best = 0;
    for (i32 lev = 0; lev < s.levels; ++lev) {
        if (1 < v) {
            std::fprintf(stdout, "...   searching level %d\n", lev); // str 1405e1488
        }
        i32 lo[3], hi[3], rlo[3], rhi[3];
        for (i32 k = 0; k < 3; ++k) {
            rlo[k] = s.c[k] - lev;
            rhi[k] = s.c[k] + lev;
            lo[k] = rlo[k] < 1 ? 0 : rlo[k];
            hi[k] = rhi[k] < l->n[k] - 1 ? rhi[k] : l->n[k] - 1;
        }
        for (i32 ix = lo[0]; ix <= hi[0]; ++ix) {
            for (i32 iy = lo[1]; iy <= hi[1]; ++iy) {
                for (i32 iz = lo[2]; iz <= hi[2]; ++iz) {
                    if (!on_shell(s.c, lev, ix, iy, iz)) {
                        continue;
                    }
                    CellList* cell = cell_at(l->cells_a8, l, ix, iy, iz);
                    if (0 < fun_1404056c0(cell)) {
                        const i32 cnt = fun_1404056c0(cell);
                        for (i32 j = 0; j < cnt; ++j) {
                            const i32 id = *fun_140405720(cell, j + 1);
                            const double d = fun_140406540(s.pc, fun_140406300(l->pts, id));
                            if (d < best2) {
                                best = id;
                                best2 = d;
                            }
                        }
                        checked = checked + cnt;
                    }
                }
            }
        }
        const double rr = std::sqrt(best2);
        if (1 < v) {
            crt::fprintf(stdout, "...   current min distance %g, point %d\n", rr, best); // str 1405e14a8
        }
        double g[3];
        for (i32 k = 0; k < 3; ++k) {
            const i32 h2 = rhi[k] == rlo[k] ? rlo[k] + 1 : rhi[k]; // the upper face uses c + L
            const double flo = l->box[k * 2] + static_cast<double>(rlo[k]) * l->h[k];
            const double fhi = l->box[k * 2] + static_cast<double>(h2) * l->h[k];
            const double a = std::fabs(s.pc[k] - flo);
            const double b = std::fabs(s.pc[k] - fhi);
            g[k] = b <= a ? b : a;
        }
        const double t = g[1] <= g[0] ? g[1] : g[0];
        const double gmin = g[2] <= t ? g[2] : t;
        if (1 < v) {
            crt::fprintf(stdout, "...   distance to nearest next level %g\n", gmin); // str 1405e14d8
        }
        if (rr < gmin) {
            break;
        }
    }
    if (best == 0) {
        fun_14039d450("defCUBE_SearchPoint_Obj : Could not find a min distance point\n"); // str 1405e1508
    }
    *point = static_cast<std::uint32_t>(best);
    const double d = std::sqrt(fun_140406540(p, fun_140406300(l->pts, best)));
    *dist = d;
    if (v != 0) {
        crt::fprintf(stdout, "...min distance     %g at point %d\n", d, best);                   // str 1405e1548
        std::fprintf(stdout, "...points checked   %d of %d\n", checked, fun_1404062e0(l->pts)); // str 1405e1570
    }
}

// @src 14022fcd0  cfnCUBE_SearchPoint_CellPolygon_Obj (GENCTC_3d_search.md sec. 4.4): *radius = R0,
// then the first cell (shell order) holding a polygon gives sqrt(max over its 8 corners of the
// squared distance from the UNCLAMPED p) (140230bb8 passes param_3, not the clamped point)
void fun_14022fcd0(i32* obj, i32* inst, double* p, double* radius, i32* verbose)
{
    const i32 e = *inst;
    const i32 v = *verbose;
    const i32 idx = find_cube(*obj, e);
    if (idx == -1) {
        fun_14039d450("cfnCUBE_SearchPoint_CellPolygon_Obj : Illegal combination of object number (%d) and entity set number (%d) \n",
                      *obj, e); // str 1405e17f0
    }
    Cube* r = &g_cubes[idx];
    const i32 ty = fun_1403ff870(&r->loc);
    if (ty != 3 && ty != 1 && ty != 6 && ty != 5 && ty != 4 && ty != 2) {
        fun_14039d450("defCUBE_SearchPoint_CellPolygon_Obj : Locator type not for point searching\n"); // str 1405e1860
    }
    if (v != 0) {
        std::fprintf(stdout, "...cfnCUBE_SearchPoint_CellPolygon_Obj\n"); // str 1405e18b0
        std::fprintf(stdout, "...   cube  = %d\n", idx);                  // str 1405e18d8
    }
    Locator* l = &r->loc;
    const double hx = l->h[0], hy = l->h[1], hz = l->h[2];
    const SearchStart s = search_start(l, p);
    double q[3] = {s.pc[0], s.pc[1], s.pc[2]}; // local_188
    *radius = s.r0;
    if (1 < v) {
        crt::fprintf(stdout, "...   max distance for search  %g\n", s.r0);   // str 1405e18f0
        crt::fprintf(stdout, "...   min distance of division %g\n", s.hmin); // str 1405e1918
        std::fprintf(stdout, "...   Insertion level          %d\n", s.levels); // str 1405e1940
    }
    bool found = false;
    for (i32 lev = 0; lev < s.levels && !found; ++lev) {
        if (1 < v) {
            std::fprintf(stdout, "...   searching level %d\n", lev); // str 1405e1968
        }
        i32 lo[3], hi[3];
        for (i32 k = 0; k < 3; ++k) {
            const i32 a = s.c[k] - lev;
            const i32 b = s.c[k] + lev;
            lo[k] = a < 1 ? 0 : a;
            hi[k] = b < l->n[k] - 1 ? b : l->n[k] - 1;
        }
        for (i32 ix = lo[0]; ix <= hi[0] && !found; ++ix) {
            for (i32 iy = lo[1]; iy <= hi[1] && !found; ++iy) {
                for (i32 iz = lo[2]; iz <= hi[2] && !found; ++iz) {
                    if (!on_shell(s.c, lev, ix, iy, iz) || fun_1404056c0(cell_at(l->cells_poly, l, ix, iy, iz)) < 1) {
                        continue;
                    }
                    double mx = -1e+29; // 14052c5a0
                    const double x0 = l->box[0] + static_cast<double>(ix) * hx;
                    const double y0 = l->box[2] + static_cast<double>(iy) * hy;
                    const double z0 = l->box[4] + static_cast<double>(iz) * hz;
                    for (i32 k = 0; k < 8; ++k) {
                        switch (k) {
                        case 0: q[0] = x0; q[1] = y0; q[2] = z0; break;
                        case 1: q[0] = x0 + hx; q[1] = y0; q[2] = z0; break;
                        case 2: q[0] = x0 + hx; q[1] = y0 + hy; q[2] = z0; break;
                        case 3: q[0] = x0; q[1] = y0 + hy; q[2] = z0; break;
                        case 4: q[0] = x0; q[1] = y0; q[2] = z0 + hz; break;
                        case 5: q[0] = x0 + hx; q[1] = y0; q[2] = z0 + hz; break;
                        case 6: q[0] = x0 + hx; q[1] = y0 + hy; q[2] = z0 + hz; break;
                        default: q[0] = x0; q[1] = y0 + hy; q[2] = z0 + hz; break;
                        }
                        const double d = fun_140406540(p, q);
                        mx = d < mx ? mx : d;
                    }
                    *radius = std::sqrt(mx);
                    found = true;
                }
            }
        }
    }
    if (!found) {
        fun_14039d450("defCUBE_SearchPoint_CellPolygon_Obj : Could not find a bucket cell that contains any polygon.\n"); // str 1405e1990
    }
    if (v != 0) {
        crt::fprintf(stdout, "...distance   %g between point(%g, %g, %g) and point (%g, %g, %g)\n", *radius, p[0], p[1], p[2], q[0],
                     q[1], q[2]); // str 1405e19f0
    }
}

// @src 140232090  cfnCUBE_GetDimension_Obj: the cell sizes of the LAST record of (obj, inst)
void fun_140232090(i32* obj, i32* inst, double* h, i32* verbose)
{
    const i32 v = *verbose;
    const i32 o = *obj;
    const i32 e = *inst;
    i32 idx = -1;
    for (i32 i = 0; i < g_cube_used; ++i) {
        if (g_cubes[i].id == o && g_cubes[i].sub == e) {
            idx = i;
        }
    }
    if (idx == -1) {
        h[0] = 0.0;
        h[1] = 0.0;
        h[2] = 0.0;
        fun_14039d450("cfnCUBE_SearchPoly : Illegal combination of object number (%d) and entity set number (%d)\n", o, e); // str 1405e1df0
    }
    if (v != 0) {
        std::fprintf(stdout, "...cfnCUBE_GetDimension_Obj\n"); // str 1405e1e50
    }
    const Locator* l = &g_cubes[idx].loc;
    h[0] = l->h[0];
    h[1] = l->h[1];
    h[2] = l->h[2];
    if (v != 0) {
        std::fprintf(stdout, "...leaving function\n"); // str 1405e1e70
    }
}

} // namespace def::cmd::ctc3d
