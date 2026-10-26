// <prob>.GEO writer of the mesh generators (packet p8-07): 3D surface form inline in 140143920,
// 2D through the _DEF_AMGGEO writers 140248bf0 / 140248fb0 / 14024ae90 / 14024b600 / 1402489e0.
// Spec: docs/re/commands/DEFAMG.md 4.1 step 6 and 4.2 step 2.
#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>

#include <defcmd/meshgen/p8_seams.hpp>
#include <defmodel/geo_kw/curve_model.hpp>
#include <defmodel/geo_kw/geo_kw.hpp>
#include <defmodel/geo_kw/mesh_model.hpp>
#include <defmodel/geo_kw/surface_model.hpp>
#include <defmodel/hdrstats/object_stats.hpp>
#include <defmodel/layout/def_amggeo.hpp>
#include <defmodel/layout/def_face.hpp>
#include <defmodel/layout/def_loop.hpp>
#include <defmodel/layout/geo2d_and_mesh2d.hpp>
#include <defmodel/layout/obj_geo2d.hpp>
#include <defmodel/layout/obj_geo3d.hpp>
#include <defmodel/llobj/accessors.hpp>
#include <defmodel/llobj/dynary_table.hpp>
#include <defmodel/mesh2d/geo2d_mesh_ops.hpp>
#include <defmodel/topo/loop_geo2d.hpp>
#include <defrt/stdlib/legacy_printf.hpp>
#include <defrt/stdlib/refcount.hpp>

#include "mg_internal.hpp"
#include "mg_util.hpp"

namespace def::cmd {

using namespace mg;
namespace gk = model::geo_kw;
using model::DEF_AMGGEO;
using model::OBJ_GEO2D;
using model::OBJ_GEO3D;

namespace {

// 1401b0cc0: case-folded suffix compare, s strictly longer than p. Ported in def_pre/src/menu7.cpp
// (ends_with), which defcmd cannot call (dependencies point down); same body, to be moved into
// defrt at the integration.
bool ends_with_ci(const std::string& s, const char* p)
{
    const std::int32_t n = static_cast<std::int32_t>(std::strlen(p));
    if (static_cast<std::int32_t>(s.size()) - n < 1) return false;
    std::string a = s.substr(s.size() - static_cast<std::size_t>(n));
    std::string b(p);
    for (std::int32_t i = 0; i < n; i = i + 1) {
        char& x = a[static_cast<std::size_t>(i)];
        char& y = b[static_cast<std::size_t>(i)];
        if ('@' < x && x < '[') x = static_cast<char>(x + ' ');
        if ('@' < y && y < '[') y = static_cast<char>(y + ' ');
    }
    return std::strncmp(a.c_str(), b.c_str(), static_cast<std::size_t>(n)) == 0;
}

}  // namespace

// @src 14027f140  type-1 curve whose third value (z) is <= 1e-6 at every point
bool curve_points_planar(OBJ_GEO2D* c)
{
    if (c->n_points < 1 || c->points == nullptr) return false;
    if (c->curve_type != 1) return false;
    const std::int32_t stride = c->dof();  // 1402760e0
    for (std::int32_t i = 0; i < c->n_points; i = i + 1) {
        if (1e-6 < c->points[i * stride + 2]) return false;  // DAT_14051f1b8
    }
    return true;
}

// @src 1402489e0  surface (nodes, 4-node facets) in the GEO format of DEF_AMG3
static bool write_surface_geo(DEF_AMGGEO*, const char* name, const std::uint32_t* nnode,
                              const std::uint32_t* nfacet, const double* xyz, const std::int32_t* f4)
{
    std::FILE* f = crt_fopen(name, "wt");
    if (f == nullptr) return false;
    std::fprintf(f, "%d\n", *nnode);
    for (std::int32_t i = 0; i < static_cast<std::int32_t>(*nnode); i = i + 1)
        rt::crt::fprintf(f, "%10d %15.7f %15.7f %15.7f\n", i + 1, xyz[i * 3], xyz[i * 3 + 1], xyz[i * 3 + 2]);
    std::fprintf(f, "%d\n", *nfacet);
    for (std::int32_t i = 0; i < static_cast<std::int32_t>(*nfacet); i = i + 1)
        std::fprintf(f, "%10d %10d %10d %10d %10d\n", i + 1, f4[i * 4], f4[i * 4 + 1], f4[i * 4 + 2], f4[i * 4 + 3]);
    std::fclose(f);
    return true;
}

// @src 14024b600  3D form of 140248bf0: *mode 0 surface 0 of the object, 1 the object's own
// surface polygons (meshed, not obj_kind 2)
static bool write_geo_3d(DEF_AMGGEO* g, const char* name, LLOBJ_DATA* obj, const std::int32_t* mode)
{
    std::uint32_t nn = 0, ne = 0;
    const double* xyz = nullptr;
    const std::int32_t* f4 = nullptr;
    if (*mode == 0) {
        if (!gk::object_has_surface_mesh(*obj)) return false;  // 140203a20
        OBJ_GEO3D* s = gk::surface(*obj, 0);
        nn = static_cast<std::uint32_t>(s->node_count());
        ne = static_cast<std::uint32_t>(gk::surface(*obj, 0)->elem_count());
        xyz = gk::geo3d_coords(gk::surface(*obj, 0));
        f4 = gk::geo3d_elems(gk::surface(*obj, 0));
    } else {
        if (*mode != 1) return false;
        if (model::p407::obj_kind_is_2(obj)) return false;  // 140158d00
        if (has_mesh(obj) == 0) return false;
        nn = static_cast<std::uint32_t>(node_count(obj));
        xyz = node_coords(obj);
        ne = at<std::uint32_t>(obj, 0xbf0);
        f4 = at<std::int32_t*>(obj, 0xbf8);
    }
    return write_surface_geo(g, name, &nn, &ne, xyz, f4);
}

// @src 14024ae90  2D regions of geometry 0: per region and loop the curve type, point count and
// material (outer loop), then the points
static bool write_geo_regions(DEF_AMGGEO*, const char* name, LLOBJ_DATA* obj)
{
    OBJ_GEO2D tmp;  // 140275cd0 (local_a0), dtor 140275e00 on every exit
    if (obj == nullptr) return false;
    model::GEO2D_AND_MESH2D* g0 = gk::geometry(*obj, 0);
    if (g0 == nullptr) return false;
    // 14024af2a: operator new(200) + copy ctor 14027f7a0; the copy is never deleted
    model::GEO2D_AND_MESH2D* g = new model::GEO2D_AND_MESH2D(*g0);
    const std::int32_t nf = static_cast<std::int32_t>(model::llobj::list8_size_b(g));  // 140158f30
    if (nf < 1) return false;
    std::FILE* f = crt_fopen(name, "wt");
    if (f == nullptr) return false;
    bool ok = true;
    bool planar = true;
    for (std::int32_t i = 0; i < nf; i = i + 1) {
        model::DEF_FACE* face = model::geo2d_face_at(g, &i);  // 140283f80
        if (face == nullptr) continue;
        const std::int32_t nl = static_cast<std::int32_t>(model::llobj::list8_size_a(face));  // 140158f10
        for (std::int32_t j = 0; j < nl; j = j + 1) {
            model::ILoop* il = face->loop_at(&j);  // vtable +0x60
            model::DEF_LOOP* l = il == nullptr ? nullptr : dynamic_cast<model::DEF_LOOP*>(il);
            if (l != nullptr) {
                model::topo::loop_to_geo2d(l, &tmp);  // 1404126d0
                if (tmp.curve_type == 1 && !curve_points_planar(&tmp)) planar = false;
            }
        }
    }
    for (std::int32_t i = 0; i < nf; i = i + 1) {
        model::DEF_FACE* face = model::geo2d_face_at(g, &i);
        if (face == nullptr) continue;
        std::int32_t mat = face->u40_value();  // 1402f7fb0
        if (mat < 1) {
            std::int32_t cnt = 0;
            gk::material_groups(*obj, &cnt, &mat);  // 1401fb270
        }
        if (mat < 1) mat = 99999;
        const std::int32_t nl = static_cast<std::int32_t>(model::llobj::list8_size_a(face));
        for (std::int32_t j = 0; j < nl; j = j + 1) {
            model::ILoop* il = face->loop_at(&j);
            model::DEF_LOOP* l = il == nullptr ? nullptr : dynamic_cast<model::DEF_LOOP*>(il);
            if (l == nullptr) continue;
            model::topo::loop_to_geo2d(l, &tmp);
            if (0 < j && tmp.is_ccw()) {  // inner loop counter-clockwise: reverse it (1402567c0)
                tmp.reverse();
                mat = 0;
            }
            if (planar && tmp.curve_type != 2) {
                std::fprintf(f, "%d\n", tmp.curve_type);
            } else {
                std::fprintf(f, "%d\n", tmp.curve_type + 1);
            }
            if (j == 0) {
                std::fprintf(f, "%d %d\n", tmp.n_points, mat);
            } else {
                std::fprintf(f, "%d %d\n", tmp.n_points, 0);
            }
            const double* p = tmp.points;
            if (p == nullptr) {
                ok = false;
                break;
            }
            for (std::int32_t k = 0; k < tmp.n_points; k = k + 1) {
                if (tmp.curve_type == 1) {
                    rt::crt::fprintf(f, "%10d %20.10E %20.10E %20.10E\n", k + 1, p[k * 3], p[k * 3 + 1], p[k * 3 + 2]);
                } else if (tmp.curve_type == 2) {
                    rt::crt::fprintf(f, "%10d %10d %20.10E %20.10E %20.10E %20.10E %7.2lf\n", k + 1,
                                     static_cast<std::int32_t>(p[k * 6]), p[k * 6 + 1], p[k * 6 + 2], p[k * 6 + 3],
                                     p[k * 6 + 4], p[k * 6 + 5] * 57.29577951308232);  // DAT_140522fd8
                }
            }
        }
    }
    std::fclose(f);
    return ok;
}

// @src 140248fb0  2D form: with regions (flag) 14024ae90, else every curve of the object
// (dim 4: the edges of geometry 0) as one loop each
static bool write_geo_curves(DEF_AMGGEO* g, const char* name, LLOBJ_DATA* obj, bool regions, std::int32_t dim)
{
    if (regions) return write_geo_regions(g, name, obj);
    OBJ_GEO2D tmp;  // 140275cd0 (unused by the reachable code; destroyed on exit)
    if (obj == nullptr) return false;
    model::GEO2D_AND_MESH2D* g0 = nullptr;
    std::int32_t n = 0;
    if (!model::p407::obj_kind_is_2(obj) && dim == 4) {  // 140158d00
        g0 = gk::geometry(*obj, 0);
        if (g0 == nullptr) return false;
        n = static_cast<std::int32_t>(model::llobj::list18_size(g0));  // 140159190
        if (n < 1) return false;
    } else {
        n = gk::curve_count(*obj);
        if (n < 1) return false;
    }
    std::FILE* f = crt_fopen(name, "wt");
    if (f == nullptr) return false;
    bool ok = true;
    for (std::int32_t i = 0; i < n; i = i + 1) {
        OBJ_GEO2D* c = nullptr;
        if (!model::p407::obj_kind_is_2(obj) && dim == 4) {
            c = g0->edge_curve(i);  // 14027ff20
            if (c == nullptr) continue;
        } else {
            c = gk::curve(*obj, i);  // 1401982c0
        }
        std::fprintf(f, "%d\n", c->curve_type + 1);
        std::uint32_t mat = static_cast<std::uint32_t>(i + 1);
        if (!c->is_ccw()) mat = 0;
        std::fprintf(f, "%d %d\n", c->n_points, mat);
        const double* p = c->points;
        if (p == nullptr) {
            ok = false;
            break;
        }
        for (std::int32_t k = 0; k < c->n_points; k = k + 1) {
            if (c->curve_type == 1) {
                rt::crt::fprintf(f, "%10d %20.10E %20.10E %20.10E\n", k + 1, p[k * 3], p[k * 3 + 1], p[k * 3 + 2]);
            } else if (c->curve_type == 2) {
                rt::crt::fprintf(f, "%10d %10d %20.10E %20.10E %20.10E %20.10E %7.2lf\n", k + 1,
                                 static_cast<std::int32_t>(p[k * 6]), p[k * 6 + 1], p[k * 6 + 2], p[k * 6 + 3],
                                 p[k * 6 + 4], p[k * 6 + 5] * 57.29577951308232);
            }
        }
        // 1402494ae: a block guarded by the region flag (false here) that would write the curve
        // again when 140200560 finds it inside another; unreachable, not ported (140200560 has no
        // other caller).
    }
    std::fclose(f);
    return ok;
}

// @src 140248bf0  GEO file of the object: dims 2 / 4 the curve forms (regions when *mode > 0),
// otherwise the 3D surface form
static bool write_amggeo(DEF_AMGGEO* g, const char* name, LLOBJ_DATA* obj, const std::int32_t* mode, std::int32_t dim)
{
    if (dim == 2) return write_geo_curves(g, name, obj, 0 < *mode, 2);
    if (dim == 4) return write_geo_curves(g, name, obj, 0 < *mode, 4);
    return write_geo_3d(g, name, obj, mode);
}

// @src 140143920  <geo>.GEO (<prob> without a trailing ".DB"). 3D: surface 0, quads split in two
// triangles; ids/ids_out/ntri (all three or none): per written triangle the id of its polygon.
bool write_geo_file(MeshGenBase& mg, const std::int32_t* ids, std::int32_t* ids_out, std::uint32_t* ntri)
{
    std::string prob(mg.u3f60);  // 1400066a0
    const bool with_ids = ids != nullptr && ids_out != nullptr && ntri != nullptr;
    if (ends_with_ci(prob, ".DB")) {  // 1401b0cc0, str 14051d94c
        const std::int32_t len = static_cast<std::int32_t>(prob.size());  // 140002800
        prob = fun_1401b1520(prob, len - 3);                               // p8-08
    }
    char name[0x400];
    std::sprintf(name, "%s.GEO", prob.c_str());
    if (rt::dimension_is_2(mg.u3f48)) {
        DEF_AMGGEO g;  // 140246d00, dtor 140246d40
        const std::int32_t mode = 1;
        const std::int32_t dim = model::llobj::int_at_1c(mg.u3f48);  // 140158e90
        return write_amggeo(&g, name, mg.u008, &mode, dim);
    }
    const char* node_fmt = "%10d %15.7f %15.7f %15.7f \n";  // str 14051dbf0
    const char* poly_fmt = "%10d %10d %10d %10d %10d\n";     // str 14051dc10
    const std::int32_t zero = 0;
    OBJ_GEO3D* s = gk::surface(*mg.u008, zero);  // 140198620
    if (s == nullptr) return false;
    const std::uint32_t nn = static_cast<std::uint32_t>(s->node_count());  // 1400034d0
    const double* xyz = gk::geo3d_coords(s);                                // 140006410
    if (nn == 0) return false;
    if (static_cast<std::int32_t>(nn) < 1) return false;
    std::FILE* f = crt_fopen(name, "wt");
    if (f == nullptr) return false;
    std::fprintf(f, "%10d\n", nn);
    for (std::int32_t i = 0; i < static_cast<std::int32_t>(nn); i = i + 1)
        rt::crt::fprintf(f, node_fmt, i + 1, xyz[i * 3], xyz[i * 3 + 1], xyz[i * 3 + 2]);
    const std::int32_t np = s->elem_count();  // 140003500
    if (np < 1) {
        std::fclose(f);
        return false;
    }
    const std::int32_t* p = gk::geo3d_elems(s);  // 140005510
    std::uint32_t k = 0;
    for (std::int32_t i = 0; i < np; i = i + 1) {
        if (p[i * 4] == std::abs(p[i * 4 + 3])) {  // 140484ab8
            k = k + 1;
        } else {
            k = k + 2;
        }
    }
    if (with_ids) *ntri = k;
    std::fprintf(f, "%10d\n", k);
    k = 0;
    for (std::int32_t i = 0; i < np; i = i + 1) {
        const std::int32_t* q = p + i * 4;
        if (q[0] == std::abs(q[3])) {
            if (with_ids) ids_out[static_cast<std::int32_t>(k)] = ids[i];
            k = k + 1;
            std::fprintf(f, poly_fmt, k, q[0], q[1], q[2], std::abs(q[3]));
        } else {
            if (with_ids) ids_out[static_cast<std::int32_t>(k)] = ids[i];
            k = k + 1;
            std::fprintf(f, poly_fmt, k, q[0], q[1], q[2], q[0]);
            if (with_ids) ids_out[static_cast<std::int32_t>(k)] = ids[i];
            k = k + 1;
            std::fprintf(f, poly_fmt, k, q[0], q[2], std::abs(q[3]), q[0]);
        }
    }
    std::fclose(f);
    return true;
}

} // namespace def::cmd
