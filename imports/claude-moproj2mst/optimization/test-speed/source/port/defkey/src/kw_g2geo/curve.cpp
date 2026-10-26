// kw-g2geo family "curve": DIEGEO (curve, 3D curve and type-3 forms; the 3D surface form is
// routed to surface::read_surface), TPLOGY and CSPMTX, readers and writers.
// Specs: docs/re/keywords/DIEGEO.md, TPLOGY.md, CSPMTX.md. Decompiles: deform-rea-work/out/g2geo/
// parser.c (cases 0x0e, 0x7d, 0x7e), fam_curve.c. Model code: defmodel/geo_kw/curve_model.hpp.
#define _CRT_SECURE_NO_WARNINGS
#include "internal.hpp"

#include <cstddef>
#include <cstring>
#include <vector>

#include <defkey/crt_scan.hpp>
#include <defmodel/geo/pending.hpp>
#include <defmodel/geo_kw/curve_model.hpp>
#include <defmodel/geo_kw/geo_kw.hpp>
#include <defmodel/layout/def_edge.hpp>
#include <defmodel/layout/def_face.hpp>
#include <defmodel/layout/def_loop.hpp>
#include <defmodel/layout/def_vertex.hpp>
#include <defmodel/layout/geo2d_and_mesh2d.hpp>
#include <defmodel/layout/obj_geo2d.hpp>
#include <defmodel/linked/matrix4.hpp>
#include <defmodel/mesh2d/geo2d_mesh_ops.hpp>
#include <defrt/stdlib/legacy_printf.hpp>
#include <defrt/rdata.hpp>

namespace def::key::g2geo::curve {

namespace gk = def::model::geo_kw;
using model::DEF_EDGE;
using model::DEF_FACE;
using model::DEF_LOOP;
using model::DEF_VERTEX;
using model::GEO2D_AND_MESH2D;
using model::OBJ_GEO2D;
using model::linked::Matrix4;

namespace {

// rdata 140522fd0  pi/180
constexpr double kDegToRad = def::rt::rdata::d_140522fd0;
// rdata 140522fd8  180/pi
constexpr double kRadToDeg = def::rt::rdata::d_140522fd8;

inline std::size_t idx(std::int32_t i) { return static_cast<std::size_t>(static_cast<std::int64_t>(i)); }

// @src 14039a200  curve point reader: n real points of curve type `form` (1: 3 values, 2: 6
// values per point), fscanf free format; a mark of -1 ends the current curve and does not count.
// 0, 2 (EOF or bad form) or 3 (no conversion).
int read_curve_points(DB_STEP_DATA&, std::FILE* f, LLOBJ_DATA& obj, std::int32_t form, std::int32_t n)
{
    std::int32_t dof;
    if (form == 2) {
        dof = 6;
    } else {
        if (form != 1) return 2;
        dof = 3;
    }
    // malloc(n * dof * 8) (thunk 1404797d8, which throws when a negative product makes the size
    // huge; the port then allocates nothing, and no point is read since n < 1)
    const std::int32_t cells =
        static_cast<std::int32_t>(static_cast<std::uint32_t>(n) * static_cast<std::uint32_t>(dof));
    std::vector<double> buf(cells > 0 ? idx(cells) : 0);
    std::int32_t cur = 0;  // local_38: curve being filled
    std::int32_t row = 0;  // local_44: rows of that curve, separator included
    std::int32_t i = 0;    // local_50
    while (i < n) {
        std::int32_t mark = 0;
        int r = def::key::crt::fscanf(f, "%i", &mark);
        if (r == -1) return 2;
        if (r < 1) return 3;
        for (std::int32_t k = 0; k < dof; k = k + 1) {
            r = def::key::crt::fscanf(f, "%Flf", &buf[idx(row * dof + k)]);
            if (r == -1) return 2;
            if (r < 1) return 3;
        }
        row = row + 1;
        if (mark == -1 || i == n - 1) {
            while (gk::curve(obj, cur) == nullptr) gk::geometry(obj, 0)->add_edge_curve();  // 14027fce0
            OBJ_GEO2D* c = gk::curve(obj, cur);
            if (mark == -1) {
                c->n_points = row - 1;
                i = i - 1;
            } else {
                c->n_points = row;
            }
            c->curve_type = form;
            c->clear_data();  // vtable +0x18: new zeroed points
            const std::int32_t nd = c->n_points * dof;
            if (0 < nd) std::memcpy(c->points, buf.data(), idx(nd) * 8);
            cur = cur + 1;
            row = 0;
        }
        i = i + 1;
    }
    if (form == 2) {
        // every curve of geometry 0, stride 6 whatever its type
        for (std::int32_t j = 0; j < gk::curve_count(obj); j = j + 1) {
            OBJ_GEO2D* c = gk::curve(obj, j);
            for (std::int32_t k = 0; k < c->n_points; k = k + 1) {
                // the original writes past a type-1 curve's array here; the port stays inside it
                if (c->n_points * c->dof() <= k * 6 + 5) break;
                c->points[k * 6 + 5] = c->points[k * 6 + 5] * kDegToRad;
            }
        }
    }
    return 0;
}

// matrix row i as the four outputs of 140415990 (columns 0..3, 14026a4a0)
struct MatrixRow {
    double* p[4];
    MatrixRow(Matrix4* m, std::int32_t i)
    {
        const std::int32_t j[4] = {0, 1, 2, 3};
        for (int k = 0; k < 4; ++k) p[k] = gk::matrix_element(m, &i, &j[k]);
    }
};

// @src 140399e60  DIEGEO 3D type-3 form: total rows, nhdr header rows (2 discarded lines when
// nhdr is 2 or 6, the 4 matrix rows when nhdr is 4 or 6), then total - nhdr type-1 points
int read_type3(DB_STEP_DATA& m, std::FILE* f, LLOBJ_DATA& obj, std::int32_t total, std::int32_t nhdr)
{
    std::int32_t id = 0;
    double scratch = 0;
    if (total < 1) return 2;
    if (nhdr < 1) return 2;
    if (!(nhdr < total)) return 2;
    if (nhdr == 2 || nhdr == 6) {
        if (read_i_d3(f, &id, &scratch, &scratch, &scratch) != 0) return 2;  // 140415880
        if (read_i_d3(f, &id, &scratch, &scratch, &scratch) != 0) return 2;
    }
    gk::ensure_geo_list(obj);  // 140197e40
    GEO2D_AND_MESH2D* g = gk::geometry(obj, 0);
    if (g == nullptr) return 2;
    g->clear_topology();  // 14027fa30
    if (g->add_edge_curve() == nullptr) return 2;
    if (nhdr == 6 || nhdr == 4) {
        Matrix4 mat;  // 140220540
        for (std::int32_t i = 0; i < 4; i = i + 1) {
            MatrixRow row(&mat, i);
            if (read_i_d4(f, &id, row.p) != 0) return 2;  // 140415990
        }
        gk::set_matrix(g, &mat);  // 14039aa60
    }
    return read_curve_points(m, f, obj, 1, total - nhdr);
}

// @src 140399120  TPLOGY reader: 0, 2 (read error) or 3 (no geometry)
int read_tplogy(DB_STEP_DATA&, std::FILE* f, LLOBJ_DATA& obj)
{
    char line[208];
    const std::int32_t ng = gk::geometry_count(obj);
    const std::int32_t gi = 1;
    if (ng == 0) return 3;
    if (gi < 1 || ng < gi) return 3;
    GEO2D_AND_MESH2D* g = gk::geometry(obj, gi - 1);
    if (g == nullptr) return 3;
    gk::reset_topology(g);  // 140284690
    const char off = 0;
    model::geo2d_set_b8(g, &off);  // 1402847d0
    std::int32_t nv = 0, ne = 0, nl = 0, nf = 0;
    if (read_i4(f, &nv, &ne, &nl, &nf) != 0) return 2;  // 140414100
    for (std::int32_t i = 0; i < nv; i = i + 1) model::geo2d_add_vertex(g);
    for (std::int32_t i = 0; i < nl; i = i + 1) model::geo2d_add_loop(g);
    for (std::int32_t i = 0; i < nf; i = i + 1) model::geo2d_add_face(g);
    std::int32_t a = 0, b = 0, c = 0, d = 0, link = 0;
    double x = 0, y = 0;
    // vertices
    if (read_string_line(f, line, 200) != 0) return 2;  // 140413c60
    for (std::int32_t i = 0; i < nv; i = i + 1) {
        if (read_ii_dd(f, &a, &b, &x, &y) != 0) return 2;  // 140415ff0
        std::int32_t vi = a - 1;
        DEF_VERTEX* v = model::geo2d_vertex_at(g, &vi);
        gk::vertex_set_xy(v, &x, &y);
        for (std::int32_t k = 0; k < b; k = k + 1) {
            if (read_i(f, &link) != 0) return 2;
            for (std::int32_t e = 0; e < ne; e = e + 1) {
                DEF_EDGE* ed = g->edge_at(e);  // 1402800a0
                if (ed != nullptr && link == ed->get_id()) v->add_edge(ed);  // 14041d0b0
            }
        }
    }
    // edges
    if (read_string_line(f, line, 200) != 0) return 2;
    for (std::int32_t e = 0; e < ne; e = e + 1) {
        if (read_i4(f, &a, &b, &c, &d) != 0) return 2;
        DEF_EDGE* ed = g->edge_at(a - 1);
        if (ed == nullptr) continue;  // its loop lines are not read
        for (std::int32_t i = 0; i < nv; i = i + 1) {
            DEF_VERTEX* v = model::geo2d_vertex_at(g, &i);
            if (c == v->id) ed->vertices[0] = v;  // 140256770, 140256730
            if (d == v->id) ed->vertices[1] = v;  // 140256750
        }
        for (std::int32_t k = 0; k < b; k = k + 1) {
            if (read_i(f, &link) != 0) return 2;
            for (std::int32_t l = 0; l < nl; l = l + 1) {
                DEF_LOOP* L = model::geo2d_loop_at(g, &l);
                if (link == L->id) ed->add_loop(L);  // 140256720, 14041c6c0
            }
        }
    }
    // loops
    if (read_string_line(f, line, 200) != 0) return 2;
    for (std::int32_t l = 0; l < nl; l = l + 1) {
        if (read_iii(f, &a, &b, &c) != 0) return 2;
        std::int32_t li = a - 1;
        DEF_LOOP* L = model::geo2d_loop_at(g, &li);
        for (std::int32_t k = 0; k < b; k = k + 1) {
            if (read_i(f, &link) != 0) return 2;
            for (std::int32_t e = 0; e < ne; e = e + 1) {
                DEF_EDGE* ed = g->edge_at(e);
                // the original calls 14041c610 without a null check
                if (ed != nullptr && link == ed->get_id()) L->add_edge(ed);  // 1404121e0
            }
        }
        for (std::int32_t k = 0; k < c; k = k + 1) {
            if (read_i(f, &link) != 0) return 2;
            for (std::int32_t i = 0; i < nf; i = i + 1) {
                DEF_FACE* F = model::geo2d_face_at(g, &i);
                if (link == F->id) L->add_face(F);  // 140256710, 140412440
            }
        }
    }
    // faces
    if (read_string_line(f, line, 200) != 0) return 2;
    for (std::int32_t i = 0; i < nf; i = i + 1) {
        if (read_iii(f, &a, &b, &c) != 0) return 2;
        std::int32_t fi = a - 1;
        DEF_FACE* F = model::geo2d_face_at(g, &fi);
        if (F == nullptr) return 2;
        F->set_u40(&c);  // 1402f7f90
        for (std::int32_t k = 0; k < b; k = k + 1) {
            if (read_i(f, &link) != 0) return 2;
            for (std::int32_t l = 0; l < nl; l = l + 1) {
                DEF_LOOP* L = model::geo2d_loop_at(g, &l);
                if (link == L->id) F->add_loop(L);  // 1402f7e80
            }
        }
    }
    return 0;
}

// @src 14039a5f0  CSPMTX reader: 0 or 2
int read_cspmtx(DB_STEP_DATA&, std::FILE* f, LLOBJ_DATA& obj)
{
    GEO2D_AND_MESH2D* g = gk::geometry(obj, 0);
    if (g == nullptr) return 2;
    std::int32_t tmp = 0;
    std::int32_t id = 0;
    if (read_i(f, &tmp) != 0) return 2;
    Matrix4 mat;
    for (std::int32_t i = 0; i < 4; i = i + 1) {
        MatrixRow row(&mat, i);
        if (read_i_d4(f, &id, row.p) != 0) return 2;
    }
    gk::set_matrix(g, &mat);
    return 0;
}

// part of @src 14038b8a0  case 0x0e (DIEGEO)
int read_diegeo(DB_STEP_DATA& m, std::FILE* f, LLOBJ_DATA& obj)
{
    if (!is_2d(m)) {
        std::int32_t a = 0, b = 0, c = 0;
        int nconv = 0;  // iStack_1064
        if (read_iii(f, &a, &b, &c, &nconv) != 0) return 2;
        if (a == 3) {
            if (read_type3(m, f, obj, b, c) != 0) return 2;
            return 0;
        }
        if (nconv == 2) {
            std::int32_t form;
            if (a == 2) {
                form = 2;
            } else {
                if (a != 1) return 2;
                form = 1;
            }
            if (gk::geometry_has_edges(obj) && gk::geometry(obj, 0) != nullptr) {
                gk::geometry(obj, 0)->clear_topology();  // 14027fa30
            }
            read_curve_points(m, f, obj, form, b);  // status ignored
            return 0;
        }
        // surface form: everything after line 0 (sizing 1402fcd90, node and facet lines, then
        // obj+0x3994 = 1 and obj+0x3990 = 1) is the surface family's
        return surface::read_surface(m, f, obj, a, b, c);
    }
    std::int32_t a = 0, b = 0;
    if (read_ii(f, &a, &b) != 0) return 2;  // 140413ee0
    std::int32_t form;
    if (a == 2) {
        form = 2;
    } else {
        if (a != 1) return 2;
        form = 1;
    }
    if (read_curve_points(m, f, obj, form, b) != 0) return 2;
    return 0;
}

}  // namespace

bool handles(int id) { return id == kDIEGEO || id == kTPLOGY || id == kCSPMTX; }

// part of @src 14038b8a0  cases 0x0e, 0x7d, 0x7e
int read_case(DB_STEP_DATA& m, std::FILE* f, LLOBJ_DATA& obj, int id)
{
    switch (id) {
    case kDIEGEO:
        return read_diegeo(m, f, obj);
    case kTPLOGY:
        if (read_tplogy(m, f, obj) != 0) return 2;
        return 0;
    case kCSPMTX:
        if (read_cspmtx(m, f, obj) != 0) return 2;
        return 0;
    default:
        return 2;
    }
}

// ---- writers ---------------------------------------------------------------------------------------

// @src 1402aaa70  DIEGEO curves
int write_curves(DB_STEP_DATA&, std::FILE* f, LLOBJ_DATA& obj, std::uint32_t n)
{
    if (gk::curve(obj, 0) == nullptr) return 0;
    gk::remove_degenerate_curves(obj, true);  // 140304e40
    if (gk::curve(obj, 0) == nullptr) return 0;
    const std::int32_t type = gk::curve(obj, 0)->curve_type;
    std::uint32_t k = 1;
    std::int32_t total = 0;
    for (std::int32_t i = 0; i < gk::curve_count(obj); i = i + 1) total = total + gk::curve(obj, i)->n_points;
    def::rt::crt::fprintf(f, "%-6s %7i %7i %7i\n", kw(0x9e), n, type, total);
    for (std::int32_t i = 0; i < gk::curve_count(obj); i = i + 1) {
        gk::curve(obj, i)->set_curve_type(type);  // 1402763d0
        if (type == 1) {
            if (0 < i) def::rt::crt::fprintf(f, " %7i %20.10E %20.10E %20.10E\n", -1, 0.0, 0.0, 0.0);
            for (std::int32_t j = 0; j < gk::curve(obj, i)->n_points; j = j + 1) {
                const double* p = gk::curve(obj, i)->points;
                def::rt::crt::fprintf(f, " %7i %20.10E %20.10E %20.10E\n", k, p[j * 3], p[j * 3 + 1], p[j * 3 + 2]);
                k = k + 1;
            }
        } else {
            if (0 < i) {
                def::rt::crt::fprintf(f, " %7i %7i %20.10E %20.10E %20.10E %20.10E %20.10E\n", -1, 0, 0.0, 0.0, 0.0, 0.0, 0.0);
            }
            for (std::int32_t j = 0; j < gk::curve(obj, i)->n_points; j = j + 1) {
                const double* p = gk::curve(obj, i)->points;
                const double ang = p[j * 6 + 5] * kRadToDeg;
                def::rt::crt::fprintf(f, " %7i %7i %20.10E %20.10E %20.10E %20.10E %20.10E\n", k,
                        static_cast<std::int32_t>(p[j * 6]), p[j * 6 + 1], p[j * 6 + 2], p[j * 6 + 3], p[j * 6 + 4],
                        ang);
                k = k + 1;
            }
        }
    }
    return 0;
}

// @src 1402a9f90  TPLOGY
int write_tplogy(DB_STEP_DATA&, std::FILE* f, LLOBJ_DATA& obj, std::uint32_t n)
{
    if (f == nullptr) return 3;
    const std::int32_t ng = gk::geometry_count(obj);
    if (ng == 0) return 3;
    if (ng < 1) return 3;
    GEO2D_AND_MESH2D* g = gk::geometry(obj, 0);
    if (g == nullptr) return 3;
    if (!model::pending::fun_140158ec0(g)) {  // face 0 has a loop
        if (1 < static_cast<std::int32_t>(g->edges.size())) return 3;
        const bool many = false;
        gk::build_topology(g, &many);  // 140281bf0
    }
    const std::int32_t nv = static_cast<std::int32_t>(g->vertices.size());  // 1402567a0
    const std::int32_t ne = static_cast<std::int32_t>(g->edges.size());     // 140159190
    const std::int32_t nl = static_cast<std::int32_t>(g->loops.size());     // 140256780
    const std::int32_t nf = static_cast<std::int32_t>(g->faces.size());     // 140158f30
    def::rt::crt::fprintf(f, "%-6s %7i %7i %7i %7i %7i\n", kw(0x10d), n, nv, ne, nl, nf);
    def::rt::crt::fprintf(f, "%s\n", "* Vertex Data: ");
    for (std::int32_t i = 0; i < nv; i = i + 1) {
        DEF_VERTEX* v = model::geo2d_vertex_at(g, &i);
        if (v == nullptr) continue;
        const double* xy = v->get_xy();          // 14041d000
        const std::int32_t k = v->edge_count();  // slot 2
        def::rt::crt::fprintf(f, " %7i %7i %20.10E %20.10E\n", v->id, k, xy[0], xy[1]);
        for (std::int32_t j = 0; j < k; j = j + 1) {
            model::IEdge* ie = v->edge_at(&j);  // 14041d020
            DEF_EDGE* e = ie == nullptr ? nullptr : dynamic_cast<DEF_EDGE*>(ie);
            if (e != nullptr) def::rt::crt::fprintf(f, " %7i\n", e->get_id());
        }
    }
    def::rt::crt::fprintf(f, "%s\n", "* Edge Data:");
    for (std::int32_t i = 0; i < ne; i = i + 1) {
        DEF_EDGE* e = g->edge_at(i);  // 1402800a0
        if (e == nullptr) continue;
        const std::int32_t id = e->get_id();
        const std::int32_t z = 0, o = 1;
        model::IVertex* iv0 = e->vertex(&z);
        DEF_VERTEX* v0 = iv0 == nullptr ? nullptr : dynamic_cast<DEF_VERTEX*>(iv0);
        if (v0 == nullptr) continue;
        model::IVertex* iv1 = e->vertex(&o);
        DEF_VERTEX* v1 = iv1 == nullptr ? nullptr : dynamic_cast<DEF_VERTEX*>(iv1);
        if (v1 == nullptr) continue;
        const std::int32_t k = e->loop_count();  // 14041c5c0
        def::rt::crt::fprintf(f, " %7i %7i %7i %7i\n", id, k, v0->id, v1->id);
        for (std::int32_t j = 0; j < k; j = j + 1) {
            model::ILoop* il = e->loop_at(&j);  // 14041c630
            DEF_LOOP* l = il == nullptr ? nullptr : dynamic_cast<DEF_LOOP*>(il);
            if (l != nullptr) def::rt::crt::fprintf(f, " %7i\n", l->id);
        }
    }
    def::rt::crt::fprintf(f, "%s\n", "* Loop Data:");
    for (std::int32_t i = 0; i < nl; i = i + 1) {
        DEF_LOOP* l = model::geo2d_loop_at(g, &i);  // 140284030
        if (l == nullptr) continue;
        const std::int32_t ke = l->edge_count();  // slot 3
        const std::int32_t kf = l->face_count();  // 1404125e0
        def::rt::crt::fprintf(f, " %7i %7i %7i\n", l->id, ke, kf);
        for (std::int32_t j = 0; j < ke; j = j + 1) {
            model::IEdge* ie = l->edge_at(&j);  // 140412150
            DEF_EDGE* e = ie == nullptr ? nullptr : dynamic_cast<DEF_EDGE*>(ie);
            if (e != nullptr) def::rt::crt::fprintf(f, " %7i\n", e->get_id());
        }
        for (std::int32_t j = 0; j < kf; j = j + 1) {
            model::IFace* ifc = l->face_at(&j);  // 1404123b0
            DEF_FACE* fc = ifc == nullptr ? nullptr : dynamic_cast<DEF_FACE*>(ifc);
            if (fc != nullptr) def::rt::crt::fprintf(f, " %7i\n", fc->id);
        }
    }
    def::rt::crt::fprintf(f, "%s\n", "* Face Data:");
    for (std::int32_t i = 0; i < nf; i = i + 1) {
        DEF_FACE* fc = model::geo2d_face_at(g, &i);  // 140283f80
        if (fc == nullptr) continue;
        const std::int32_t attr = fc->u40_value();                           // 1402f7fb0
        const std::int32_t k = static_cast<std::int32_t>(fc->loops.size());  // 140158f10
        def::rt::crt::fprintf(f, " %7i %7i %7i\n", fc->id, k, attr);
        for (std::int32_t j = 0; j < k; j = j + 1) {
            model::ILoop* il = fc->loop_at(&j);  // slot 12
            DEF_LOOP* l = il == nullptr ? nullptr : dynamic_cast<DEF_LOOP*>(il);
            if (l != nullptr) def::rt::crt::fprintf(f, " %7i\n", l->id);
        }
    }
    return 0;
}

// @src 1402aaf40  CSPMTX
int write_cspmtx(DB_STEP_DATA&, std::FILE* f, LLOBJ_DATA& obj, std::uint32_t n)
{
    if (gk::geometry_count(obj) == 0) return 3;
    GEO2D_AND_MESH2D* g = gk::geometry(obj, 0);
    if (g == nullptr) return 3;
    def::rt::crt::fprintf(f, "%-6s %7i %7i\n", kw(0x10e), n, 1);
    Matrix4 mat;
    std::uint32_t r = 1;
    gk::get_matrix(g, &mat);  // 14026a3e0
    for (std::int32_t i = 0; i < 4; i = i + 1) {
        MatrixRow row(&mat, i);
        def::rt::crt::fprintf(f, " %7i %20.10E %20.10E %20.10E %20.10E\n", r, *row.p[0], *row.p[1], *row.p[2], *row.p[3]);
        r = r + 1;
    }
    return 0;
}

// @src 1402ab160  curves, TPLOGY, CSPMTX; 3 at the first failure
int write_curve_group(DB_STEP_DATA& m, std::FILE* f, LLOBJ_DATA& obj, std::uint32_t n)
{
    if (write_curves(m, f, obj, n) != 0) return 3;
    if (write_tplogy(m, f, obj, n) != 0) return 3;
    if (write_cspmtx(m, f, obj, n) != 0) return 3;
    return 0;
}

}  // namespace def::key::g2geo::curve
