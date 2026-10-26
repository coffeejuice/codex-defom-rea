// geolib: _DEF_AMGGEO readers, stores and the 2D topology section (packet p8-09).
// Spec: docs/re/commands/GFREAD_STL_GEO_NAS.md sections 3 and 5 (5.4 topology).
// Decompile / listing: deform-rea-work/out/p809/all.c, all_l.txt.
#define _CRT_SECURE_NO_WARNINGS
#include <defcmd/geolib.hpp>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>

#include <defkey/line_io.hpp>
#include <defmodel/geo/pending.hpp>
#include <defmodel/geo_kw/curve_model.hpp>
#include <defmodel/geo_kw/geo_kw.hpp>
#include <defmodel/geo_kw/surface_model.hpp>
#include <defmodel/layout/def_edge.hpp>
#include <defmodel/layout/def_face.hpp>
#include <defmodel/layout/def_loop.hpp>
#include <defmodel/layout/def_vertex.hpp>
#include <defmodel/layout/geo2d_and_mesh2d.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/layout/obj_geo2d.hpp>
#include <defmodel/llobj_ctor/llobj_copy.hpp>
#include <defmodel/mesh2d/geo2d_mesh_ops.hpp>
#include <defrt/rdata.hpp>
#include <defrt/stdlib/crt_emulation.hpp>

#include "ext_p67.hpp"
#include "internal.hpp"

namespace def::cmd::geolib {

namespace gk = def::model::geo_kw;
using def::model::DEF_EDGE;
using def::model::DEF_FACE;
using def::model::DEF_LOOP;
using def::model::DEF_VERTEX;
using def::model::GEO2D_AND_MESH2D;
using def::model::OBJ_GEO2D;

namespace {
// rdata 140522fd0  pi/180
constexpr double kDegToRad = def::rt::rdata::d_140522fd0;

// The original reads or writes through a null entity pointer here: end like the crash.
[[noreturn]] void null_deref()
{
    def::rt::abnormal_exit(def::rt::kStatusAccessViolation);
}

std::int32_t* calloc_ints(std::int32_t n)
{
    return static_cast<std::int32_t*>(def::model::llobj_copy::calloc_int(n));  // 140256dc0
}
double* calloc_doubles(std::int32_t n)
{
    return static_cast<double*>(def::model::llobj_copy::calloc_double(n));  // 140257150
}

// Pre-check shared by 1402488a0 / 140248390: line 2 (or line 1 when there is no line 2) cut
// with strtok(" ,\t\n\r").
bool precheck(const char* file, std::int32_t want, bool first_is_one)
{
    char sep[8];
    std::memcpy(sep, " ,\t\n\r", 6);  // str 140532868 (6 bytes copied to the stack)
    std::int32_t n = 0;
    bool ok = false;
    std::FILE* f = std::fopen(file, "rt");  // 14047b934
    if (f == nullptr) return false;
    char buf[2000];
    buf[0] = '\0';  // the original's stack buffer is uninitialised when the file is empty
    std::fgets(buf, 2000, f);
    std::fgets(buf, 2000, f);
    char* t = std::strtok(buf, sep);
    if (first_is_one) {
        if (t != nullptr && std::atol(t) == 1) {  // 14047b6c0
            while (t != nullptr) {
                n = n + 1;
                t = std::strtok(nullptr, sep);
            }
            if (n == want) ok = true;
        }
    } else {
        while (t != nullptr) {
            n = n + 1;
            t = std::strtok(nullptr, sep);
        }
        if (n == want) ok = true;
    }
    std::fclose(f);
    return ok;
}
} // namespace

// @src 140246d00
DEF_AMGGEO* fun_140246d00(DEF_AMGGEO* g)
{
    return new (g) DEF_AMGGEO();
}

// @src 140246d40
void fun_140246d40(DEF_AMGGEO* g)
{
    g->~DEF_AMGGEO();
}

// @src 1402566d0
void* fun_1402566d0(DEF_AMGGEO* g, std::uint32_t flags)
{
    g->~DEF_AMGGEO();
    if ((flags & 1u) != 0) ::operator delete(static_cast<void*>(g));
    return g;
}

// @src 140246f70
bool fun_140246f70(DEF_AMGGEO* g, const char* file, const char* close, std::int32_t mode, LLOBJ_DATA* obj)
{
    if (mode == 2) return fun_1402478f0(g, file, close, 2, obj);
    if (mode == 4) return fun_1402478f0(g, file, close, 4, obj);
    return fun_1402484c0(g, file);
}

// @src 14024bac0
bool fun_14024bac0(DEF_AMGGEO* g, LLOBJ_DATA* obj, std::int32_t mode)
{
    if (mode == 2) return fun_14024bb30(g, obj, 2);
    if (mode == 4) return fun_14024bb30(g, obj, 4);
    return fun_14024be40(g, obj);
}

// @src 1402488a0
bool fun_1402488a0(const char* file)
{
    return precheck(file, 4, true);
}

// @src 140248390
bool fun_140248390(const char* file)
{
    return precheck(file, 2, false);
}

// @src 1402484c0
bool fun_1402484c0(DEF_AMGGEO* g, const char* file)
{
    if (!fun_1402488a0(file)) return false;
    std::FILE* f = std::fopen(file, "rt");
    if (f == nullptr) return false;
    bool ok = true;
    std::fscanf(f, "%d\n", &g->npoint);
    if (g->npoint < 1 || 1.0E7 <= static_cast<double>(g->npoint)) {  // DAT_14051f1a8
        std::fclose(f);
        return false;
    }
    if (g->points != nullptr) std::free(g->points);
    g->points = nullptr;
    g->points = calloc_doubles(g->npoint * 3);
    std::int32_t i = 0;
    while (i < g->npoint) {
        double x = 0, y = 0, z = 0;
        const int r = std::fscanf(f, "%*d %lf %lf %lf\n", &x, &y, &z);
        if (r < 3) {
            std::fclose(f);
            return false;
        }
        if (r != -1) {
            g->points[i * 3] = x;
            g->points[i * 3 + 1] = y;
            g->points[i * 3 + 2] = z;
            i = i + 1;
        }
    }
    g->npoint = i;
    std::fscanf(f, "%d\n", &g->nfacet);
    if (g->nfacet < 1 || 1.0E7 <= static_cast<double>(g->nfacet)) {
        ok = false;
    } else {
        if (g->facets != nullptr) std::free(g->facets);
        g->facets = nullptr;
        g->facets = calloc_ints(g->nfacet << 2);
        std::int32_t j = 0;
        while (j < g->nfacet) {
            std::int32_t a = 0, b = 0, c = 0, d = 0;
            const int r = std::fscanf(f, "%*d %d %d %d %d\n", &a, &b, &c, &d);
            if (r < 4) {
                std::fclose(f);
                return false;
            }
            if (r != -1) {
                g->facets[j << 2] = a;
                g->facets[j * 4 + 1] = b;
                g->facets[j * 4 + 2] = c;
                g->facets[j * 4 + 3] = d;
                j = j + 1;
            }
        }
        g->nfacet = j;
    }
    std::fclose(f);
    return ok;
}

// @src 1402478f0
bool fun_1402478f0(DEF_AMGGEO* g, const char* file, const char* close, std::int32_t /*mode*/, LLOBJ_DATA* obj)
{
    TokenReader rdr;
    fun_140411920(&rdr);
    const std::int32_t topo_mark = 99;
    bool topo = false;
    if (!fun_140248390(file)) {
        fun_140411940(&rdr);
        return false;
    }
    std::FILE* f = std::fopen(file, "rt");
    if (f == nullptr) {
        fun_140411940(&rdr);
        return false;
    }
    g->reset_curves();  // 140246e40
    g->curve_start = calloc_ints(100);
    g->curve_code = calloc_ints(100);
    std::int32_t grow = 1;
    std::int32_t prev = -1;
    // local_118: uninitialised in the original when the first token read ends the loop (then no
    // curve is kept and the store fails, so the value only reaches geo+8)
    std::int32_t cur = 0;
    std::int32_t type = 0, npts = 0, code = 0, idx = 0;
    char buf[208];
    for (;;) {
        if (fun_140411ad0(&rdr, f, tok_int, &type, tok_new_line) < 1) break;
        if (type == topo_mark) {
            topo = true;
            break;
        }
        cur = type;
        fun_140411ad0(&rdr, f, tok_int, &npts, 0);
        fun_140411ad0(&rdr, f, tok_int, &code, 0);
        bool skip;
        if (prev < 0 || prev == cur) {
            skip = false;
            if (cur == 3) {
                g->points = fun_1402571b0(g->points, (g->npoint + npts) * 6);
            } else if (*close == '\0' || code < 0) {
                g->points = fun_1402571b0(g->points, (g->npoint + npts) * 3);
            } else {
                g->points = fun_1402571b0(g->points, (g->npoint + 1 + npts) * 3);
            }
        } else {
            skip = true;
        }
        double* const pts = g->points;  // re-read per call in the original; unchanged in the loop
        (void)pts;
        if (cur == 1) {
            for (std::int32_t i = 0; i < npts; i = i + 1) {
                if (!skip) {
                    const std::int32_t k = (g->npoint + i) * 3;
                    fun_140411ad0(&rdr, f, tok_int, &idx, tok_new_line);
                    fun_140411ad0(&rdr, f, tok_double, g->points + k, 0);
                    fun_140411ad0(&rdr, f, tok_double, g->points + (k + 1), 0);
                    g->points[k + 2] = 0.0;
                } else {
                    fun_140411ad0(&rdr, f, tok_line, buf, tok_new_line);
                }
            }
        } else if (cur == 2) {
            for (std::int32_t i = 0; i < npts; i = i + 1) {
                if (!skip) {
                    const std::int32_t k = (g->npoint + i) * 3;
                    fun_140411ad0(&rdr, f, tok_int, &idx, tok_new_line);
                    fun_140411ad0(&rdr, f, tok_double, g->points + k, 0);
                    fun_140411ad0(&rdr, f, tok_double, g->points + (k + 1), 0);
                    fun_140411ad0(&rdr, f, tok_double, g->points + (k + 2), 0);
                } else {
                    fun_140411ad0(&rdr, f, tok_line, buf, tok_new_line);
                }
            }
        } else if (cur == 3) {
            for (std::int32_t i = 0; i < npts; i = i + 1) {
                if (!skip) {
                    const std::int32_t k = (g->npoint + i) * 6;
                    double a = 0;
                    fun_140411ad0(&rdr, f, tok_int, &idx, tok_new_line);
                    fun_140411ad0(&rdr, f, tok_double, g->points + k, 0);
                    fun_140411ad0(&rdr, f, tok_double, g->points + (k + 1), 0);
                    fun_140411ad0(&rdr, f, tok_double, g->points + (k + 2), 0);
                    fun_140411ad0(&rdr, f, tok_double, g->points + (k + 3), 0);
                    fun_140411ad0(&rdr, f, tok_double, g->points + (k + 4), 0);
                    fun_140411ad0(&rdr, f, tok_double, &a, 0);
                    g->points[k + 5] = a * kDegToRad;
                } else {
                    fun_140411ad0(&rdr, f, tok_line, buf, tok_new_line);
                }
            }
        } else if (cur == 4) {
            for (std::int32_t i = 0; i < npts; i = i + 1) {
                if (!skip) {
                    const std::int32_t k = (g->npoint + i) * 3;
                    fun_140411ad0(&rdr, f, tok_int, &idx, tok_new_line);
                    fun_140411ad0(&rdr, f, tok_double, g->points + k, 0);
                    fun_140411ad0(&rdr, f, tok_double, g->points + (k + 1), 0);
                    fun_140411ad0(&rdr, f, tok_double, g->points + (k + 2), 0);
                    g->points[k + 2] = 0.0;
                } else {
                    fun_140411ad0(&rdr, f, tok_line, buf, tok_new_line);
                }
            }
        }
        if (skip) cur = prev;
        if (-1 < code && !skip) {
            if (*close != '\0' && cur != 3) {
                std::memcpy(g->points + (g->npoint + npts) * 3, g->points + g->npoint * 3, 0x18);
                npts = npts + 1;
            }
            g->curve_start[g->curve_count] = g->npoint + 1;
            g->curve_code[g->curve_count] = code;
            g->curve_count = g->curve_count + 1;
            if (99 < g->curve_count) {
                grow = grow + 1;
                g->curve_start = ext::realloc_int(g->curve_start, g->curve_count * grow);  // 140256e20
                g->curve_code = ext::realloc_int(g->curve_code, g->curve_count * grow);
            }
            g->npoint = g->npoint + npts;
            prev = cur;
        }
    }
    cur = cur - 1;
    if (cur == 0 || cur == 3) {
        g->curve_type = 1;
    } else {
        g->curve_type = cur;
    }
    const bool ok = fun_14024bac0(g, obj, 4);
    if (topo) fun_140249700(g, f, obj);
    std::fclose(f);
    fun_140411940(&rdr);
    return ok;
}

// @src 14024ae10
void* fun_14024ae10(DEF_AMGGEO*, LLOBJ_DATA* obj)
{
    if (obj == nullptr) return nullptr;
    const std::int32_t n = gk::geometry_count(*obj);  // 1401983e0
    if (n == 0) return nullptr;
    if (n < 1) return nullptr;
    return gk::geometry(*obj, 0);  // 140197f40
}

// @src 14024bb30
bool fun_14024bb30(DEF_AMGGEO* g, LLOBJ_DATA* obj, std::int32_t /*mode*/)
{
    if (g->points == nullptr || g->curve_start == nullptr) return false;
    if (obj == nullptr) return false;
    GEO2D_AND_MESH2D* geom = gk::geometry(*obj, 0);
    if (geom == nullptr) {
        fun_140198270(obj);
        geom = gk::geometry(*obj, 0);
        if (geom == nullptr) return false;
    }
    geom->clear_topology();  // 14027fa30
    fun_140198230(obj, g->curve_count);
    for (std::int32_t i = 0; i < g->curve_count; i = i + 1) {
        OBJ_GEO2D* c = gk::curve(*obj, i);  // 1401982c0
        c->curve_type = g->curve_type;
        if (i == g->curve_count - 1) {
            c->n_points = (g->npoint - g->curve_start[i]) + 1;
        } else {
            c->n_points = g->curve_start[i + 1] - g->curve_start[i];
        }
        c->clear_data();  // vtable +0x18: allocate_points
        const std::int32_t dof = c->dof();  // 1402760e0
        std::memcpy(c->points, g->points + static_cast<std::int64_t>((g->curve_start[i] - 1) * dof),
                    static_cast<std::size_t>(static_cast<std::int64_t>(c->n_points * dof) << 3));
        if (g->curve_code != nullptr) fun_1402567e0(c, g->curve_code[i]);
    }
    return true;
}

// @src 1402567e0
void fun_1402567e0(void* curve, std::int32_t code)
{
    detail::at<std::int32_t>(curve, 0xc) = code;
}

// @src 14024be40
bool fun_14024be40(DEF_AMGGEO* g, LLOBJ_DATA* obj)
{
    if (g->points == nullptr || g->facets == nullptr) return false;
    ext::set_surface_slots(obj, 1);                      // 140163340
    ext::size_surface(obj, g->npoint, g->nfacet, 0);     // 1402009f0
    def::model::OBJ_GEO3D* s = gk::surface(*obj, 0);     // 140198620
    if (s == nullptr) return false;
    const std::int32_t nn = g->npoint;
    std::memcpy(gk::geo3d_coords(s), g->points, static_cast<std::size_t>(static_cast<std::int64_t>(nn * 3) << 3));  // 140006410
    const std::int32_t ne = g->nfacet;
    std::memcpy(gk::geo3d_elems(s), g->facets, static_cast<std::size_t>(static_cast<std::int64_t>(ne << 2) << 2));  // 140005510
    gk::set_surface_flag(*obj, true);  // 1401985e0
    return true;
}

// @src 140249700  topology section after the 99 line (spec docs/re/commands/GEO_TOPOLOGY.md):
// the TPLOGY records with one leading line; every read error returns; no range checks. Where the
// original dereferences a null entity (an id outside the created ones) the port ends the process
// with the access-violation status (defrt/stdlib/crt_emulation.hpp), at the same point.
void fun_140249700(DEF_AMGGEO* g, std::FILE* f, LLOBJ_DATA* obj)
{
    namespace key = def::key;
    if (f == nullptr) return;
    auto* geom = static_cast<GEO2D_AND_MESH2D*>(fun_14024ae10(g, obj));
    if (geom == nullptr) return;
    gk::reset_topology(geom);  // 140284690
    const char off = 0;
    def::model::geo2d_set_b8(geom, &off);  // 1402847d0
    char line[208];
    std::int32_t nv = 0, ne = 0, nl = 0, nf = 0;
    if (key::read_string_line(f, line, 200) != 0) return;  // 140413c60
    if (key::read_i4(f, &nv, &ne, &nl, &nf) != 0) return;  // 140414100
    for (std::int32_t i = 0; i < nv; i = i + 1) def::model::geo2d_add_vertex(geom);  // 140283a30
    for (std::int32_t i = 0; i < nl; i = i + 1) def::model::geo2d_add_loop(geom);    // 140283980
    for (std::int32_t i = 0; i < nf; i = i + 1) def::model::geo2d_add_face(geom);    // 1402838d0
    std::int32_t a = 0, b = 0, c = 0, d = 0, link = 0;
    double x = 0, y = 0;
    // vertices
    if (key::read_string_line(f, line, 200) != 0) return;
    for (std::int32_t i = 0; i < nv; i = i + 1) {
        if (key::read_ii_dd(f, &a, &b, &x, &y) != 0) return;  // 140415ff0
        std::int32_t vi = a - 1;
        DEF_VERTEX* v = def::model::geo2d_vertex_at(geom, &vi);  // 1402840e0
        if (v == nullptr) null_deref();  // 14041cfdd: MOVSD [RCX + 0x8]
        gk::vertex_set_xy(v, &x, &y);  // 14041cfc0
        for (std::int32_t k = 0; k < b; k = k + 1) {
            if (key::read_i(f, &link, false) != 0) return;  // 140413de0
            for (std::int32_t e = 0; e < ne; e = e + 1) {
                DEF_EDGE* ed = geom->edge_at(e);  // 1402800a0
                if (ed == nullptr) null_deref();  // 14041c61a: MOV EAX, [RAX + 0x30]
                if (link == ed->get_id()) v->add_edge(ed);  // 14041c610, 14041d0b0
            }
        }
    }
    // edges
    if (key::read_string_line(f, line, 200) != 0) return;
    for (std::int32_t e = 0; e < ne; e = e + 1) {
        if (key::read_i4(f, &a, &b, &c, &d) != 0) return;
        DEF_EDGE* ed = geom->edge_at(a - 1);  // not checked: used only when a link matches
        for (std::int32_t i = 0; i < nv; i = i + 1) {
            DEF_VERTEX* v = def::model::geo2d_vertex_at(geom, &i);
            if (c == v->id) {  // 140256770
                if (ed == nullptr) null_deref();  // 140256744: MOV [RCX + 0x18]
                ed->vertices[0] = v;  // 140256730
            }
            if (d == v->id) {
                if (ed == nullptr) null_deref();  // 140256764: MOV [RCX + 0x20]
                ed->vertices[1] = v;  // 140256750
            }
        }
        for (std::int32_t k = 0; k < b; k = k + 1) {
            if (key::read_i(f, &link, false) != 0) return;
            for (std::int32_t l = 0; l < nl; l = l + 1) {
                DEF_LOOP* L = def::model::geo2d_loop_at(geom, &l);  // 140284030
                if (link == L->id) {  // 140256720
                    if (ed == nullptr) null_deref();  // 14041c6c0 -> 140284d30 on null + 0x10
                    ed->add_loop(L);  // 14041c6c0
                }
            }
        }
    }
    // loops
    if (key::read_string_line(f, line, 200) != 0) return;
    for (std::int32_t l = 0; l < nl; l = l + 1) {
        if (key::read_iii(f, &a, &b, &c, nullptr) != 0) return;  // 140413fb0
        std::int32_t li = a - 1;
        DEF_LOOP* L = def::model::geo2d_loop_at(geom, &li);  // not checked: used only on a match
        for (std::int32_t k = 0; k < b; k = k + 1) {
            if (key::read_i(f, &link, false) != 0) return;
            for (std::int32_t e = 0; e < ne; e = e + 1) {
                DEF_EDGE* ed = geom->edge_at(e);
                if (ed == nullptr) null_deref();  // 14041c61a
                if (link == ed->get_id()) {
                    if (L == nullptr) null_deref();  // 1404121e0 -> 140284f40 on null + 0x8
                    L->add_edge(ed);  // 1404121e0
                }
            }
        }
        for (std::int32_t k = 0; k < c; k = k + 1) {
            if (key::read_i(f, &link, false) != 0) return;
            for (std::int32_t i = 0; i < nf; i = i + 1) {
                DEF_FACE* F = def::model::geo2d_face_at(geom, &i);  // 140283f80
                if (link == F->id) {  // 140256710
                    if (L == nullptr) null_deref();  // 140412440 -> 140285190 on null + 0x10
                    L->add_face(F);  // 140412440
                }
            }
        }
    }
    // faces
    if (key::read_string_line(f, line, 200) != 0) return;
    for (std::int32_t i = 0; i < nf; i = i + 1) {
        if (key::read_iii(f, &a, &b, &c, nullptr) != 0) return;
        std::int32_t fi = a - 1;
        DEF_FACE* F = def::model::geo2d_face_at(geom, &fi);
        if (F == nullptr) null_deref();  // 1402f7fa6: MOV [RCX + 0x40]
        F->set_u40(&c);  // 1402f7f90
        for (std::int32_t k = 0; k < b; k = k + 1) {
            if (key::read_i(f, &link, false) != 0) return;
            for (std::int32_t l = 0; l < nl; l = l + 1) {
                DEF_LOOP* L = def::model::geo2d_loop_at(geom, &l);
                if (link == L->id) F->add_loop(L);  // 1402f7e80
            }
        }
    }
}

} // namespace def::cmd::geolib
