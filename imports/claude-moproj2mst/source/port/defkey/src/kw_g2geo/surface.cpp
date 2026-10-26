// kw-g2geo family "surface": the DIEGEO 3D surface form (after its first line), POLYTP, DIEPNT,
// DIEEDG, FLDGEO, FLWNET; the geometry dispatcher and surface writers of FUN_14029e250 steps 20
// and 22. Parser cases are those of FUN_14038b8a0 (case = id - 0x90): POLYTP 0x9b, DIEPNT 0x9c,
// DIEEDG 0x9d, FLDGEO 0x91, FLWNET 0x92. Decompiles: deform-rea-work/out/g2geo/parser.c,
// fam_surface.c. Specs: docs/re/keywords/{DIEGEO,POLYTP,DIEPNT,DIEEDG,FLDGEO,FLWNET}.md.
//
// No floating-point arithmetic happens here: doubles are only read into the model and printed.
#define _CRT_SECURE_NO_WARNINGS
#include "internal.hpp"

#include <defkey/crt_scan.hpp>
#include <defkey/table_io.hpp>
#include <defmodel/geo/range_helpers.hpp>
#include <defmodel/geo_kw/geo_kw.hpp>
#include <defmodel/geo_kw/surface_model.hpp>
#include <defmodel/layout/geo2d_and_mesh2d.hpp>
#include <defmodel/layout/obj_geo3d.hpp>
#include <defrt/stdlib/legacy_printf.hpp>

namespace def::key::g2geo::surface {

namespace gk = def::model::geo_kw;
using def::model::OBJ_GEO3D;
namespace rtc = def::rt::crt;

namespace {

int as_int(std::uint32_t n) { return static_cast<int>(n); }

// 0 -> 0, anything else -> 2: the parser's epilogue
int status(int st) { return st == 0 ? 0 : 2; }

// @src 14039a800  read a surface mesh (FLDGEO / FLWNET): size it (1402fcd90), *nnode node lines
//                 (140415880), *nelem element lines (140414200); `negate` turns a negative 4th
//                 node into its absolute value. 0, or 2 on the first failed line.
int read_surface_mesh(std::FILE* f, OBJ_GEO3D* s, std::int32_t* nnode, std::int32_t* nelem,
                      const char* negate)
{
    std::int32_t dummy = 0;
    gk::geo3d_size(s, nnode, nelem);
    double* xyz = gk::geo3d_coords(s);
    std::int32_t* el = gk::geo3d_elems(s);
    for (std::int32_t i = 0; i < *nnode; i = i + 1) {
        if (read_i_d3(f, &dummy, xyz + i * 3, xyz + (i * 3 + 1), xyz + (i * 3 + 2)) != 0) return 2;
    }
    for (std::int32_t i = 0; i < *nelem; i = i + 1) {
        if (read_i5(f, &dummy, el + (i << 2), el + (i * 4 + 1), el + (i * 4 + 2), el + (i * 4 + 3),
                    false) != 0)
            return 2;
        if (*negate != '\0' && el[i * 4 + 3] < 0) {
            el[i * 4 + 3] = static_cast<std::int32_t>(0u - static_cast<std::uint32_t>(el[i * 4 + 3]));
        }
    }
    return 0;
}

// case 0x91 / 0x92: FLDGEO (obj+0x3328) / FLWNET (obj+0x3458), any dimension
int read_fldgeo_flwnet(std::FILE* f, LLOBJ_DATA& obj, bool flwnet)
{
    std::int32_t a = 0, nnode = 0, nelem = 0;
    int nconv = 0;
    if (read_iii(f, &a, &nnode, &nelem, &nconv) != 0) return 2;
    char negate = 1;
    auto* s = reinterpret_cast<OBJ_GEO3D*>(&at<std::byte>(obj, flwnet ? 0x3458 : 0x3328));
    if (read_surface_mesh(f, s, &nnode, &nelem, &negate) != 0) return 2;
    if (flwnet && gk::surface_has_mesh(s)) gk::geo3d_fun_1402fbb30(s);  // 1402f9360, 1402fbb30
    return 0;
}

// case 0x9b: POLYTP, 3D only
int read_polytp(DB_STEP_DATA& m, std::FILE* f, LLOBJ_DATA& obj)
{
    int st = 0;
    if (!is_3d(m)) return 0;
    std::int32_t n = 0, deflt = 0;
    st = read_ii(f, &n, &deflt);
    if (st != 0) return 2;
    std::int32_t* ids = gk::surface(obj, 0)->resize_elem_ids();  // 1402f9a50
    if (ids == nullptr) return 2;
    const std::int32_t ne = gk::surface(obj, 0)->elem_count();
    for (std::int32_t i = 0; i < ne; i = i + 1) ids[i] = deflt;
    std::int32_t e = 0, v = 0;
    for (std::int32_t i = 0; i < n; i = i + 1) {
        st = read_ii(f, &e, &v);  // errors are not checked; the last status is returned
        if (0 < e && e <= ne) ids[e - 1] = v;
    }
    gk::geo3d_rebuild_patches(gk::surface(obj, 0));  // 1402fced0
    return status(st);
}

// case 0x9c: DIEPNT, 3D only
int read_diepnt(DB_STEP_DATA& m, std::FILE* f, LLOBJ_DATA& obj)
{
    int st = 0;
    if (!is_3d(m)) return 0;
    std::int32_t id = 0, type = 0, n = 0, extra = 0;
    st = read_i4(f, &id, &type, &n, &extra);
    if (st != 0) return 2;
    OBJ_GEO3D* s = gk::surface(obj, 0);
    if (s == nullptr) return 2;
    std::int32_t id2 = id, type2 = type, n2 = n, extra2 = extra;
    double x = 0, y = 0, z = 0, p4 = 0, p5 = 0;
    std::int32_t dummy = 0;
    gk::DiePointSet* set = gk::geo3d_point_set_for(s, &id2, &type2);  // 1402fd020
    set->set_extra(&extra2);                                         // 1402fe900
    model::CowList<gk::DiePoint>& pts = set->points;                 // 1402b74b0
    gk::points_clear(pts);                                           // 1402ff5f0
    for (std::int32_t i = 0; i < n2; i = i + 1) {
        double* const d[5] = {&x, &y, &z, &p4, &p5};
        st = read_i_d5(f, &dummy, d);  // 140415ac0
        if (st != 0) return 2;
        gk::DiePoint p;
        model::geo::fun_1402b7860(reinterpret_cast<std::uint64_t*>(p.v));
        p.set_xyz(x, y, z);  // 1402fe850
        p.set_p4(p4);        // 1402fe8a0
        p.set_p5(p5);        // 1402fe8c0
        gk::points_push_back(pts, p);  // 1402fed80
    }
    return status(st);
}

// case 0x9d: DIEEDG, 3D only
int read_dieedg(DB_STEP_DATA& m, std::FILE* f, LLOBJ_DATA& obj)
{
    int st = 0;
    if (!is_3d(m)) return 0;
    std::int32_t id = 0, type = 0, n = 0, extra = 0;
    st = read_i4(f, &id, &type, &n, &extra);
    if (st != 0) return 2;
    OBJ_GEO3D* s = gk::surface(obj, 0);
    if (s == nullptr) return 2;
    std::int32_t id2 = id, type2 = type, n2 = n, extra2 = extra;
    std::int32_t v = 0;  // not reset per value: a failed conversion appends the previous one
    gk::DieEdgeSet* set = gk::geo3d_edge_set_for(s, &id2, &type2);  // 1402fd490
    if (type2 == 1) set->set_extra(&extra2);                        // 1402fe950
    if (type2 != 2 && type2 != 1) return 2;
    model::CowList<std::int32_t>& nodes = set->nodes;  // 1402b7510
    nodes.clear();                                     // 14015a620
    for (std::int32_t i = 0; i < n2; i = i + 1) {
        if (def::key::crt::fscanf(f, "%i", &v) == -1) return 2;
        nodes.push_back(v);  // 140220700
    }
    return status(st);
}

// @src 1402f5090  per-element int property block (only id 299 = POLYTP writes): nothing for
//                 n == 0 or a null array; 2 for a null file. All values equal to v[0]: the header
//                 "name obj 0 v[0]". Otherwise the header "name obj count deflt" with the number
//                 of values != deflt, then " %7i" index and the value (1402f2fe0) for each.
int write_int_property(std::FILE* f, int id, std::uint32_t objno, const std::int32_t* v, int n,
                       int deflt)
{
    bool same = true;
    if (n == 0 || v == nullptr) return 0;
    if (f == nullptr) return 2;
    if (id != 299) return 0;
    for (int i = 0; i < n; i = i + 1) {
        if (v[i] != *v) {
            same = false;
            break;
        }
    }
    if (!same) {
        std::uint32_t count = 0;
        for (int i = 0; i < n; i = i + 1) {
            if (v[i] != deflt) count = count + 1;
        }
        rtc::fprintf(f, "%-6s %7i %7i %7i\n", kw(299), as_int(objno), as_int(count), deflt);
        for (int i = 0; i < n; i = i + 1) {
            if (v[i] != deflt) {
                rtc::fprintf(f, " %7i", i + 1);
                const int r = def::key::write_ints(f, 1, v + i, 10, false);
                if (r != 0) return r;
            }
        }
    } else {
        rtc::fprintf(f, "%-6s %7i %7i %7i\n", kw(299), as_int(objno), 0, *v);
    }
    return 0;
}

// @src 1402ab210  POLYTP: the element ids of surface 0 (140199fa0), default 0
int write_polytp(std::FILE* f, LLOBJ_DATA& obj, std::uint32_t n)
{
    const std::int32_t ne = gk::surface(obj, 0)->elem_count();
    std::int32_t* ids = gk::geo3d_elem_ids(gk::surface(obj, 0));
    write_int_property(f, 299, n, ids, ne, 0);
    return 0;
}

// @src 1402ab2b0  DIEPNT: the non-empty point sets, renumbered from 1
int write_diepnt(std::FILE* f, LLOBJ_DATA& obj, std::uint32_t n)
{
    OBJ_GEO3D* s = gk::surface(obj, 0);
    const std::int32_t nset = static_cast<std::int32_t>(gk::die_point_sets(s).size());  // 14019a820
    std::int32_t seq = 0;
    for (std::int32_t k = 0; k < nset; k = k + 1) {
        std::int32_t k1 = k + 1;
        gk::DiePointSet* set = gk::geo3d_point_set(s, &k1);  // 1402fd3e0
        const std::int32_t type = set->get_type();
        const std::int32_t cnt = set->point_count();
        const std::int32_t extra = set->extra_if_type1();
        if (0 < cnt) {
            seq = seq + 1;
            rtc::fprintf(f, "%-6s %7i %7i %7i %7i %7i\n", kw(300), as_int(n), seq, type, cnt, extra);
            model::CowList<gk::DiePoint>& pts = set->points;
            for (std::int32_t j = 0; j < cnt; j = j + 1) {
                const gk::DiePoint& p = gk::points_at(pts, j);
                rtc::fprintf(f, " %7i %20.10E %20.10E %20.10E %20.10E %20.10E\n", j + 1, p.x(), p.y(), p.z(),
                        p.p4(), p.p5());
            }
        }
    }
    return 0;
}

// @src 1402ab530  DIEEDG: the non-empty edge sets, renumbered from 1; a set of another type than
//                 1 or 2 is written as a DIEPNT header line (kw 300) without values
int write_dieedg(std::FILE* f, LLOBJ_DATA& obj, std::uint32_t n)
{
    OBJ_GEO3D* s = gk::surface(obj, 0);
    const std::int32_t nset = static_cast<std::int32_t>(gk::die_edge_sets(s).size());  // 14019a840
    std::int32_t seq = 0;
    for (std::int32_t k = 0; k < nset; k = k + 1) {
        std::int32_t k1 = k + 1;
        gk::DieEdgeSet* set = gk::geo3d_edge_set(s, &k1);  // 1402fd740
        const std::int32_t type = set->get_type();
        const std::int32_t cnt = set->node_count();
        const std::int32_t extra = set->extra_if_type1();
        if (0 < cnt) {
            if (type == 2 || type == 1) {
                if (type == 2) {
                    rtc::fprintf(f, "%-6s %7i %7i %7i %7i\n", kw(0x12d), as_int(n), seq + 1, 2, cnt);
                } else {
                    rtc::fprintf(f, "%-6s %7i %7i %7i %7i %7i\n", kw(0x12d), as_int(n), seq + 1, 1, cnt, extra);
                }
                model::CowList<std::int32_t>& nodes = set->nodes;  // 1402b7510
                for (std::int32_t j = 0; j < cnt; j = j + 1) {
                    if (j != 0 && j % 10 == 0) rtc::fprintf(f, "\n");
                    rtc::fprintf(f, " %7i", gk::ints_at(nodes, j));  // 140159a70
                }
                rtc::fprintf(f, "\n");
            } else {
                rtc::fprintf(f, "%-6s %7i %7i %7i %7i\n", kw(300), as_int(n), seq + 1, type, 0);
            }
            seq = seq + 1;
        }
    }
    return 0;
}

// @src 140159160  geometry has at least one edge (140159190 > 0)
bool geometry_has_edges(model::GEO2D_AND_MESH2D* g) { return 0 < static_cast<std::int32_t>(g->edges.size()); }

}  // namespace

bool handles(int id)
{
    return id == kPOLYTP || id == kDIEPNT || id == kDIEEDG || id == kFLDGEO || id == kFLWNET;
}

// part of @src 14038b8a0  cases 0x91, 0x92, 0x9b, 0x9c, 0x9d
int read_case(DB_STEP_DATA& m, std::FILE* f, LLOBJ_DATA& obj, int id)
{
    switch (id) {
    case kFLDGEO: return read_fldgeo_flwnet(f, obj, false);
    case kFLWNET: return read_fldgeo_flwnet(f, obj, true);
    case kPOLYTP: return read_polytp(m, f, obj);
    case kDIEPNT: return read_diepnt(m, f, obj);
    case kDIEEDG: return read_dieedg(m, f, obj);
    default: return 2;
    }
}

// part of @src 14038b8a0  case 0x0e (DIEGEO), 3D, the surface form: after the first line
// (a, nnode, nelem) the surface 0 is sized (1402fcd90), nnode node lines (140415880) and, when
// a == 1, nelem element lines (140414200) are read; then obj+0x3994 = 1 and byte obj+0x3990 = 1.
int read_surface(DB_STEP_DATA&, std::FILE* f, LLOBJ_DATA& obj, std::int32_t a, std::int32_t nnode,
                 std::int32_t nelem)
{
    std::int32_t dummy = 0;
    gk::geo3d_size(gk::surface(obj, 0), &nnode, &nelem);
    const std::int32_t nn = gk::surface(obj, 0)->node_count();
    double* xyz = gk::geo3d_coords(gk::surface(obj, 0));
    const std::int32_t ne = gk::surface(obj, 0)->elem_count();
    std::int32_t* el = gk::geo3d_elems(gk::surface(obj, 0));
    for (std::int32_t i = 0; i < nn; i = i + 1) {
        if (read_i_d3(f, &dummy, xyz + i * 3, xyz + (i * 3 + 1), xyz + (i * 3 + 2)) != 0) return 2;
    }
    if (a == 1) {
        for (std::int32_t i = 0; i < ne; i = i + 1) {
            if (read_i5(f, &dummy, el + (i << 2), el + (i * 4 + 1), el + (i * 4 + 2), el + (i * 4 + 3),
                        false) != 0)
                return 2;
        }
    }
    gk::set_surface_count(obj, 1);   // 140198600
    gk::set_surface_flag(obj, true); // 1401985e0
    return 0;
}

// @src 1402a26c0  3 for a null file or object; 2D models: object dim 3 -> surface writer, else
// the curve writer 1402aaa70; other models: the surface writer
int write_geometry_dispatch(DB_STEP_DATA& m, std::FILE* f, LLOBJ_DATA& obj, std::uint32_t n)
{
    if (f == nullptr) return 3;  // (the object test cannot fail: obj is a reference)
    const std::int32_t dim = gk::object_dim(obj);  // 140158ce0
    if (!is_3d(m)) {
        if (dim == 3) return write_surface(m, f, obj, n);
        return curve::write_curves(m, f, obj, n);
    }
    return write_surface(m, f, obj, n);  // the dimension is passed as a 5th argument, unused
}

// @src 1402a27a0  not 2D: the curve group (1402ab160) when geometry 0 has edges; then, when the
// object has a surface mesh (140203a20) and obj+0x3994 > 0: DIEGEO surface form, POLYTP, DIEPNT
// (140160db0), DIEEDG (140160e00). Sub-writer statuses are ignored; 3 for a null file or object.
int write_surface(DB_STEP_DATA& m, std::FILE* f, LLOBJ_DATA& obj, std::uint32_t n)
{
    if (f == nullptr) return 3;  // (the object test cannot fail: obj is a reference)
    if (!is_2d(m)) {
        bool has = false;
        if (gk::geometry(obj, 0) != nullptr) has = geometry_has_edges(gk::geometry(obj, 0));
        if (has) curve::write_curve_group(m, f, obj, n);
    }
    if (gk::object_has_surface_mesh(obj) && 0 < gk::surface_count(obj)) {
        const std::int32_t nn = gk::surface(obj, 0)->node_count();
        const std::int32_t ne = gk::surface(obj, 0)->elem_count();
        const double* xyz = gk::geo3d_coords(gk::surface(obj, 0));
        const std::int32_t* el = gk::geo3d_elems(gk::surface(obj, 0));
        rtc::fprintf(f, "%-6s %7i %7i %7i %7i\n", kw(0x9e), as_int(n), 1, nn, ne);
        for (std::int32_t i = 0; i < nn; i = i + 1) {
            rtc::fprintf(f, " %7i %20.10E %20.10E %20.10E\n", i + 1, xyz[i * 3], xyz[i * 3 + 1], xyz[i * 3 + 2]);
        }
        for (std::int32_t i = 0; i < ne; i = i + 1) {
            rtc::fprintf(f, " %7i %7i %7i %7i %7i\n", i + 1, el[i << 2], el[i * 4 + 1], el[i * 4 + 2],
                    el[i * 4 + 3]);
        }
        write_polytp(f, obj, n);
        if (gk::object_has_die_points(obj)) write_diepnt(f, obj, n);
        if (gk::object_has_die_edges(obj)) write_dieedg(f, obj, n);
    }
    return 0;
}

// @src 1402a2b70  FLDGEO / FLWNET: 3 for a null file or surface or one without mesh (1402f9360);
// header "name obj 1 nnode nelem", the nodes, the elements
int write_surface_mesh(DB_STEP_DATA&, std::FILE* f, const char* name, std::byte* geo3d, std::uint32_t n)
{
    if (f == nullptr || geo3d == nullptr) return 3;
    auto* s = reinterpret_cast<OBJ_GEO3D*>(geo3d);
    if (!gk::surface_has_mesh(s)) return 3;
    const std::int32_t nn = s->node_count();
    const std::int32_t ne = s->elem_count();
    const double* xyz = gk::geo3d_coords(s);
    const std::int32_t* el = gk::geo3d_elems(s);
    rtc::fprintf(f, "%-6s %7i %7i %7i %7i\n", name, as_int(n), 1, nn, ne);
    for (std::int32_t i = 0; i < nn; i = i + 1) {
        rtc::fprintf(f, " %7i %20.10E %20.10E %20.10E\n", i + 1, xyz[i * 3], xyz[i * 3 + 1], xyz[i * 3 + 2]);
    }
    for (std::int32_t i = 0; i < ne; i = i + 1) {
        rtc::fprintf(f, " %7i %7i %7i %7i %7i\n", i + 1, el[i << 2], el[i * 4 + 1], el[i * 4 + 2], el[i * 4 + 3]);
    }
    return 0;
}

}  // namespace def::key::g2geo::surface
