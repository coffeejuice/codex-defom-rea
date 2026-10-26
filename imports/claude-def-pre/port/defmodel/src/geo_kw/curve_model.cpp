// kw-g2geo family "curve": model operations (defmodel/geo_kw/curve_model.hpp).
// Decompiles: deform-rea-work/out/g2geo/fam_curve.c (FP order checked against the disassembly:
// every expression here is one operation or a two-term sum/difference in the decompile's order).
#include <defmodel/geo_kw/curve_model.hpp>

#include <defrt/stdlib/libm.hpp>

#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <iterator>
#include <vector>

#include <defmodel/geo/pending.hpp>
#include <defmodel/geo_kw/geo_kw.hpp>
#include <defmodel/layout/def_edge.hpp>
#include <defmodel/layout/def_face.hpp>
#include <defmodel/layout/def_loop.hpp>
#include <defmodel/layout/def_vertex.hpp>
#include <defmodel/layout/geo2d_and_mesh2d.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/layout/obj_geo2d.hpp>
#include <defmodel/linked/matrix4.hpp>
#include <defmodel/mesh2d/geo2d_mesh_ops.hpp>
#include <defmodel/topo/alloc.hpp>
#include <defrt/rdata.hpp>

namespace def::model::geo_kw {

namespace {

// rdata 14051f1b8  1e-6, point tolerance of the topology build
constexpr double kPointTol = def::rt::rdata::d_14051f1b8;
// rdata 14051f260  1e-12, degenerate segment tolerance
constexpr double kSegmentTol = def::rt::rdata::d_14051f260;
// rdata 14051bed8  0.0
constexpr double kZero = def::rt::rdata::d_14051bed8;
// @data 140567490  3*pi (9.42477796076938), start value of the smallest-angle search
constexpr double kAngleStart = 9.42477796076938;
// @data 140567488  2*pi
constexpr double kTwoPi = 6.283185307179586;

inline std::size_t idx(std::int32_t i) { return static_cast<std::size_t>(static_cast<std::int64_t>(i)); }

std::int32_t edge_count(GEO2D_AND_MESH2D* g) { return static_cast<std::int32_t>(g->edges.size()); }        // 140159190
std::int32_t loop_count(GEO2D_AND_MESH2D* g) { return static_cast<std::int32_t>(g->loops.size()); }        // 140256780
std::int32_t vertex_count(GEO2D_AND_MESH2D* g) { return static_cast<std::int32_t>(g->vertices.size()); }   // 1402567a0

DEF_VERTEX* as_vertex(IVertex* v) { return v == nullptr ? nullptr : dynamic_cast<DEF_VERTEX*>(v); }
DEF_EDGE* as_edge(IEdge* e) { return e == nullptr ? nullptr : dynamic_cast<DEF_EDGE*>(e); }
DEF_LOOP* as_loop(ILoop* l) { return l == nullptr ? nullptr : dynamic_cast<DEF_LOOP*>(l); }
OBJ_GEO2D* as_geo2d(OBJ_GEO_BASE* g) { return g == nullptr ? nullptr : dynamic_cast<OBJ_GEO2D*>(g); }

IVertex* end_vertex(DEF_EDGE* e, std::int32_t k) { return e->vertex(&k); }  // 14041c590

void set_flag_b8(GEO2D_AND_MESH2D* g, bool v)
{
    const char c = v ? 1 : 0;
    geo2d_set_b8(g, &c);  // 1402847d0
}

}  // namespace

// ---- object level ---------------------------------------------------------------------------------

// @src 1401983e0
std::int32_t geometry_count(LLOBJ_DATA& obj)
{
    if (obj.u3998 == nullptr) return 0;
    return geo_list_count(geo_list(obj));  // 140301c60
}

// @src 140304e00
void remove_curve(LLOBJ_DATA& obj, const std::int32_t* i) { remove_edge(geometry(obj, 0), *i); }

// @src 140304e40
std::int32_t remove_degenerate_curves(LLOBJ_DATA& obj, bool remove)
{
    std::int32_t good = 0;
    if (curve_count(obj) < 1) return 1;  // 140198310
    std::vector<char> keep(idx(curve_count(obj)), 1);  // malloc(count) + memset 1
    std::int32_t i = 0;
    while (i < curve_count(obj)) {
        OBJ_GEO2D* c = curve(obj, i);  // 1401982c0
        if (c->n_points == 0) {
            keep[idx(i)] = 0;
        } else if (c->curve_type == 1 && c->n_points < 2) {
            keep[idx(i)] = 0;
        } else if (c->curve_type == 2 && c->n_points == 1 &&
                   points_near(c->points + 1, c->points + 3, kSegmentTol)) {
            keep[idx(i)] = 0;
        } else {
            good = good + 1;
        }
        i = i + 1;
    }
    if (good == curve_count(obj)) return 1;
    if (!remove) return 0;
    i = 0;
    while (i < curve_count(obj)) {
        if (keep[idx(i)] == 0) {
            for (std::int32_t k = i; k < curve_count(obj) - 1; k = k + 1) keep[idx(k)] = keep[idx(k + 1)];
            remove_curve(obj, &i);
            i = i - 1;
        }
        i = i + 1;
    }
    return 0;
}

// ---- GEO2D_AND_MESH2D ---------------------------------------------------------------------------------

// @src 14026a3e0  -> 140220670(m, g + 0x38)
void get_matrix(GEO2D_AND_MESH2D* g, linked::Matrix4* m)
{
    m->assign(*reinterpret_cast<linked::Matrix4*>(g->matrix));
}

// @src 14039aa60  -> 140220670(g + 0x38, m)
void set_matrix(GEO2D_AND_MESH2D* g, linked::Matrix4* m)
{
    reinterpret_cast<linked::Matrix4*>(g->matrix)->assign(*m);
}

// @src 14026a4a0
double* matrix_element(linked::Matrix4* m, const std::int32_t* i, const std::int32_t* j)
{
    return reinterpret_cast<double*>(reinterpret_cast<std::byte*>(m->m) + static_cast<std::int64_t>(*i) * 0x20 +
                                     static_cast<std::int64_t>(*j) * 8);
}

// @src 140280140
std::int32_t remove_edge(GEO2D_AND_MESH2D* g, std::int32_t i)
{
    const std::int32_t n = edge_count(g);
    if (n < 1) return 0;
    if (i < 0) return -1;
    if (n <= i) return -2;
    DEF_EDGE* e = as_edge(g->edges.mutable_at(idx(i)));  // 140284ff0, 140285820
    if (e == nullptr) return -3;
    edge_delete_curve(e);
    topo::free_block(e);  // free(): the edge's destructor does not run
    auto& items = g->edges.mutable_items();  // 140284f90: erase the node
    items.erase(std::next(items.begin(), static_cast<std::ptrdiff_t>(i)));
    geo2d_renumber_edges(g);  // 140284220
    set_flag_b8(g, true);
    return 1;
}

// @src 140284690
// @src 140284c00  edge: clear the loop list (+0x10)
void reset_topology(GEO2D_AND_MESH2D* g)
{
    g->faces.clear();  // 140286880
    g->loops.clear();  // 140286660
    const std::int32_t n = edge_count(g);
    for (std::int32_t i = 0; i < n; i = i + 1) {
        DEF_EDGE* e = g->edge_at(i);  // 1402800a0
        if (e != nullptr) e->loops.clear();
    }
    g->vertices.clear();  // 140286990
}

// @src 140284730
bool topology_dirty(GEO2D_AND_MESH2D* g)
{
    const std::int32_t n = edge_count(g);
    for (std::int32_t i = 0; i < n; i = i + 1) {
        DEF_EDGE* e = g->edge_at(i);
        if (e->curve_flag28()) return true;  // 14041ccd0
    }
    return g->ub8 != 0;
}

// @src 140281bf0
void build_topology(GEO2D_AND_MESH2D* g, const bool* faces_for_many_loops)
{
    if (topology_dirty(g)) {
        build_vertices(g);  // 140281c50
        build_loops_and_faces(g, faces_for_many_loops);
        set_flag_b8(g, false);
    }
}

// @src 140281c50
// @src 140281c70
void build_vertices(GEO2D_AND_MESH2D* g)
{
    g->faces.clear();     // 140286880
    g->loops.clear();     // 140286660
    g->vertices.clear();  // 140286990
    const std::int32_t n = edge_count(g);
    double p0[2] = {0, 0};
    double p1[2] = {0, 0};
    for (std::int32_t i = 0; i < n; i = i + 1) {
        DEF_EDGE* e = g->edge_at(i);
        OBJ_GEO2D* c = as_geo2d(edge_curve(e));  // 140284990
        const bool closed = curve_is_closed(c);
        edge_endpoints(e, p0, p1);
        if (!closed) {
            DEF_VERTEX* v = find_vertex(g, p0);
            if (v == nullptr) {
                v = geo2d_add_vertex(g);  // 140283a30
                v->set_xy(p0);            // 14041cf80
            }
            e->vertices[0] = v;  // 140256730
            v->add_edge(e);      // 14041d0b0
            v = find_vertex(g, p1);
            if (v == nullptr) {
                v = geo2d_add_vertex(g);
                v->set_xy(p1);
            }
            e->vertices[1] = v;  // 140256750
            v->add_edge(e);
        } else {
            DEF_VERTEX* v = find_vertex(g, p0);
            if (v == nullptr) v = geo2d_add_vertex(g);  // coordinates not set: stay 0, 0
            e->vertices[0] = v;
            e->vertices[1] = v;
            v->add_edge(e);
        }
    }
}

// @src 140281ec0
void build_loops_and_faces(GEO2D_AND_MESH2D* g, const bool* faces_for_many_loops)
{
    build_loops(g);
    remove_duplicate_loops(g);
    remove_enclosing_loop(g);
    if (0 < loop_count(g)) {
        if (loop_count(g) == 1) {
            build_single_face(g);
        } else if (1 < loop_count(g) && *faces_for_many_loops) {
            build_faces(g, true);
        }
    }
}

// @src 140282350
DEF_EDGE* next_unvisited_edge(GEO2D_AND_MESH2D* g)
{
    const std::int32_t n = edge_count(g);
    DEF_EDGE* e = nullptr;
    for (std::int32_t i = 0; i < n; i = i + 1) {
        e = g->edge_at(i);
        if (edge_get_34(e) == 0) return e;  // 140284a10
        if (edge_get_35(e) == 0) return e;  // 140284a20
    }
    return nullptr;
}

namespace {

// a stack entry of 1402823f0 (0x18 bytes: edge, vertex, direction byte); the stack is a local
// COW list (140285c80 ctor, 140285d70 push_back, 140285cc0 pop_back, 140285dc0 empty)
struct WalkEntry {
    DEF_EDGE* edge;
    DEF_VERTEX* vertex;
    bool forward;
};

// part of @src 1402823f0: the next edge at V found by `pick` (null when it was walked already in
// the direction it would be taken), with the far vertex and direction set
DEF_EDGE* take_next(IEdge* (*pick)(DEF_VERTEX*, IEdge*), DEF_VERTEX* V, DEF_EDGE* from, bool same_start,
                    DEF_VERTEX** far, bool* fwd)
{
    // same_start: the next edge is walked forward (+0x34) when it starts at V (cases A and D of
    // the decompile); otherwise backward (+0x35) when it starts at V (cases B and C)
    DEF_EDGE* next = as_edge(pick(V, from));
    if (next != nullptr) {
        if (end_vertex(next, 0) == V) {
            if ((same_start ? edge_get_34(next) : edge_get_35(next)) != 0) next = nullptr;
        } else if (end_vertex(next, 1) == V &&
                   (same_start ? edge_get_35(next) : edge_get_34(next)) != 0) {
            next = nullptr;
        }
    }
    if (next != nullptr) {
        if (end_vertex(next, 0) == V) {
            *far = as_vertex(end_vertex(next, 1));
            if (same_start) {
                *fwd = true;
                edge_set_34(next, 1);
            } else {
                *fwd = false;
                edge_set_35(next, 1);
            }
        } else if (end_vertex(next, 1) == V) {
            *far = as_vertex(end_vertex(next, 0));
            if (same_start) {
                *fwd = false;
                edge_set_35(next, 1);
            } else {
                *fwd = true;
                edge_set_34(next, 1);
            }
        }
    }
    return next;
}

IEdge* pick_cw(DEF_VERTEX* v, IEdge* e) { return vertex_next_edge_cw(v, e); }    // 14041d3e0
IEdge* pick_ccw(DEF_VERTEX* v, IEdge* e) { return vertex_next_edge_ccw(v, e); }  // 14041d200

}  // namespace

// @src 1402823f0
// @src 140284b10  edge: clear the loop list (+0x10)
// @src 140285c80  stack ctor (140286090, 140287a40, 140287ea0)
// @src 140285ca0  stack dtor (140286100, 1402865c0, 140286620, 140287af0)
// @src 140285d70  push_back (140286230, 140286120, 140287b70, 140287e60)
// @src 140285cc0  pop_back (140286200, 140286160, 1402861a0, 140287c60)
// @src 140285dc0  empty
void build_loops(GEO2D_AND_MESH2D* g)
{
    g->faces.clear();  // 140286880
    g->loops.clear();  // 140286660
    const std::int32_t n = edge_count(g);
    for (std::int32_t i = 0; i < n; i = i + 1) edge_clear_34_35(g->edge_at(i));  // 140284af0
    DEF_EDGE* cur;
    bool fwd = false;  // local_d8
    while ((cur = next_unvisited_edge(g)) != nullptr) {
        if (edge_get_34(cur) == 0 && edge_get_35(cur) == 0) cur->loops.clear();
        if (edge_get_34(cur) == 0) {
            edge_set_34(cur, 1);
            fwd = true;
        } else if (edge_get_35(cur) == 0) {
            edge_set_35(cur, 1);
            fwd = false;
        }
        DEF_LOOP* L = geo2d_add_loop(g);  // 140283980
        L->add_edge(cur);                 // 1404121e0
        cur->add_loop(L);                 // 14041c6c0
        if (edge_is_closed(cur)) continue;
        std::vector<WalkEntry> stack;
        WalkEntry rec{cur, as_vertex(end_vertex(cur, 0)), fwd};  // local_e8 / local_e0 / local_d8
        stack.push_back(rec);
        rec.vertex = as_vertex(end_vertex(cur, 1));
        stack.push_back(rec);
        while (!stack.empty()) {
            rec = stack.back();
            stack.pop_back();
            DEF_EDGE* e = rec.edge;
            DEF_VERTEX* V = rec.vertex;
            if (!rec.forward) {
                if (end_vertex(e, 0) == V) {
                    cur = take_next(pick_cw, V, e, true, &rec.vertex, &rec.forward);
                } else if (end_vertex(e, 1) == V) {
                    cur = take_next(pick_ccw, V, e, false, &rec.vertex, &rec.forward);
                }
            } else {
                if (end_vertex(e, 0) == V) {
                    cur = take_next(pick_ccw, V, e, false, &rec.vertex, &rec.forward);
                } else if (end_vertex(e, 1) == V) {
                    cur = take_next(pick_cw, V, e, true, &rec.vertex, &rec.forward);
                }
            }
            if (cur != nullptr) {
                L->add_edge(cur);
                cur->add_loop(L);
                rec.edge = cur;
                stack.push_back(rec);
            }
        }
    }
    for (std::int32_t i = 0; i < n; i = i + 1) edge_clear_34_35(g->edge_at(i));
}

// @src 140282f80
void remove_duplicate_loops(GEO2D_AND_MESH2D* g)
{
    loop_count(g);  // 140256780, result unused
    std::int32_t j;
    while (-1 < (j = find_duplicate_loop(g))) {
        remove_loop(g, j);
        geo2d_renumber_loops(g);  // 1402842b0
    }
}

// @src 140282ff0
std::int32_t find_duplicate_loop(GEO2D_AND_MESH2D* g)
{
    const std::int32_t n = loop_count(g);
    for (std::int32_t i = 0; i < n; i = i + 1) {
        DEF_LOOP* a = geo2d_loop_at(g, &i);  // 140284030
        if (a == nullptr) continue;
        for (std::int32_t j = 0; j < n; j = j + 1) {
            if (j == i) continue;
            DEF_LOOP* b = geo2d_loop_at(g, &j);
            if (b != nullptr && loop_same_edges(a, b)) return j;
        }
    }
    return -1;
}

// @src 1402830f0
// @src 140220810  push_back of the local int vector (std::vector here; 1402217c0 / 140222010 grow it)
void remove_enclosing_loop(GEO2D_AND_MESH2D* g)
{
    const std::int32_t n = loop_count(g);
    std::int32_t found = -1;
    std::vector<std::int32_t> cand;  // 140159ad0
    for (std::int32_t i = 0; i < n; i = i + 1) {
        DEF_LOOP* l = geo2d_loop_at(g, &i);
        if (l != nullptr && !loop_vertices_simple(l)) cand.push_back(i);
    }
    const std::int32_t nc = static_cast<std::int32_t>(cand.size());
    for (std::int32_t k = 0; k < nc; k = k + 1) {
        std::int32_t c = cand[idx(k)];
        DEF_LOOP* lc = geo2d_loop_at(g, &c);
        if (lc == nullptr) continue;
        bool all_inside = true;
        for (std::int32_t j = 0; j < n; j = j + 1) {
            if (j == c) continue;
            DEF_LOOP* lj = geo2d_loop_at(g, &j);
            if (lj == nullptr) continue;
            if (!lj->inside_bbox_of(lc)) all_inside = false;  // slot 4
            if (!all_inside) break;
        }
        if (all_inside) {
            found = c;
            break;
        }
    }
    if (0 <= found) {
        remove_loop(g, found);
        geo2d_renumber_loops(g);
    }
}

// @src 1402832f0
void build_single_face(GEO2D_AND_MESH2D* g)
{
    g->faces.clear();  // 140286880
    if (loop_count(g) < 2 && 0 < loop_count(g)) {
        DEF_FACE* f = geo2d_add_face(g);  // 1402838d0
        std::int32_t zero = 0;
        DEF_LOOP* l = geo2d_loop_at(g, &zero);
        f->add_loop(l);   // 1402f7e80
        l->add_face(f);   // 140412440
    }
}

// @src 140283390
// @src 140284b30  curve +0x0c
void build_faces(GEO2D_AND_MESH2D* g, bool set_attr)
{
    std::int32_t i = 0;
    const std::int32_t nf = count_ccw_curves(g);
    std::int32_t start = 0;
    if (nf < 1) return;
    g->faces.clear();
    for (i = 0; i < nf; i = i + 1) geo2d_add_face(g);
    i = 0;
    std::int32_t k;
    while (i < nf && 0 < (k = collect_face_loops(g, &i, &start))) {
        start = start + k;
        DEF_FACE* f = geo2d_face_at(g, &i);  // 140283f80
        std::int32_t attr = 0;
        for (std::int32_t j = 0; j < k; j = j + 1) {
            // 140159a70: element j of the int list at +0xc0
            std::int32_t li = g->uc0.mutable_at(idx(j));
            DEF_LOOP* l = geo2d_loop_at(g, &li);
            f->add_loop(l);
            l->add_face(f);
            std::int32_t zero = 0;
            IEdge* ie = l->edge_at(&zero);  // 140412150
            if (ie != nullptr) {
                DEF_EDGE* e = as_edge(ie);
                if (e != nullptr) {
                    OBJ_GEO_BASE* c = edge_curve(e);
                    if (c != nullptr) attr = c->u0c;
                }
            }
        }
        if (set_attr) {
            if (attr < 1) {
                std::int32_t v = i + 1;
                f->set_u40(&v);  // 1402f7f90
            } else {
                f->set_u40(&attr);
            }
        }
        face_outer_loop_first(f);
        i = i + 1;
    }
}

// @src 1402835f0
std::int32_t count_ccw_curves(GEO2D_AND_MESH2D* g)
{
    std::int32_t n = 0;
    const std::int32_t nl = loop_count(g);
    for (std::int32_t i = 0; i < nl; i = i + 1) {
        OBJ_GEO2D* c = g->edge_curve(i);  // 14027ff20: the curve of EDGE i
        if (c->is_ccw()) n = n + 1;       // 1402789f0
    }
    return n;
}

// @src 140283680
// @src 1401599e0  push_back on the int list at +0xc0 (140159f90, 14015a6b0, 14015e330, 14015aa90,
//                 14015aef0: COW detach and node insert of p1-05's CowList)
std::int32_t collect_face_loops(GEO2D_AND_MESH2D* g, const std::int32_t*, const std::int32_t* start)
{
    std::int32_t next = *start;
    std::int32_t k = 0;
    const std::int32_t n = loop_count(g);
    g->uc0.clear();  // 14015a620
    for (std::int32_t i = *start; i < n; i = i + 1) {
        OBJ_GEO2D* c = g->edge_curve(i);  // 14027ff20
        const std::int32_t v = i;  // the list holds 4-byte ints (14015aef0 stores 4 bytes)
        if (next == *start) {
            g->uc0.push_back(v);
            next = next + 1;
            k = k + 1;
        } else if (!c->is_ccw()) {
            g->uc0.push_back(v);
            k = k + 1;
        }
    }
    return k;
}

// @src 140283840
DEF_VERTEX* find_vertex(GEO2D_AND_MESH2D* g, const double* p)
{
    const std::int32_t n = vertex_count(g);
    for (std::int32_t i = 0; i < n; i = i + 1) {
        DEF_VERTEX* v = geo2d_vertex_at(g, &i);  // 1402840e0
        if (vertex_near(v, p, kPointTol)) return v;
    }
    return nullptr;
}

// @src 140283cd0
// @src 140284d80  erase a node of the loop list (140285df0, 1402856d0, 140286dc0)
std::int32_t remove_loop(GEO2D_AND_MESH2D* g, std::int32_t i)
{
    const std::int32_t n = loop_count(g);
    if (n < 1) return 0;
    if (i < 0) return -1;
    if (n <= i) return -2;
    DEF_LOOP* l = as_loop(g->loops.mutable_at(idx(i)));  // 140284de0, 140285730
    const std::int32_t ne = l->edge_count();           // slot 3
    for (std::int32_t k = 0; k < ne; k = k + 1) {
        DEF_EDGE* e = as_edge(l->edge_at(&k));
        if (e != nullptr) edge_remove_loop(e, l);
    }
    delete l;  // slot 5, scalar deleting dtor (1404136d0)
    auto& items = g->loops.mutable_items();
    items.erase(std::next(items.begin(), static_cast<std::ptrdiff_t>(i)));
    geo2d_renumber_loops(g);
    return 1;
}

// ---- curves -------------------------------------------------------------------------------------------

// @src 140278530
void curve_endpoints(OBJ_GEO2D* c, double* p0, double* p1)
{
    OBJ_GEO2D tmp;      // 140275cd0
    p0[0] = 0;
    p0[1] = 0;
    p1[0] = 0;
    p1[1] = 0;
    tmp.copy_from(c);   // 140276230: assign when c != 0
    if (0 < tmp.n_points) {
        if (tmp.curve_type == 2) tmp.set_curve_type(1);
        const std::int32_t dof = tmp.dof();
        p0[0] = tmp.points[0];
        p0[1] = tmp.points[1];
        p1[0] = tmp.points[(tmp.n_points - 1) * dof];
        p1[1] = tmp.points[(tmp.n_points - 1) * dof + 1];
    }
}   // 140275e00

// @src 140278670  (FUN_140484558 = fabs)
bool curve_is_closed(OBJ_GEO2D* c)
{
    double p0[2] = {0.0, 0.0};
    double p1[2] = {0.0, 0.0};
    curve_endpoints(c, p0, p1);
    return std::fabs(p0[0] - p1[0]) < kPointTol && std::fabs(p0[1] - p1[1]) < kPointTol;
}

// @src 140308d60
bool points_near(const double* a, const double* b, double tol)
{
    return std::fabs(a[0] - b[0]) < tol && std::fabs(a[1] - b[1]) < tol;
}

// @src 1403090e0
double dot2(const double* a, const double* b) { return a[0] * b[0] + a[1] * b[1]; }

// @src 140309230  (FUN_140484644 = sqrt)
double length2(const double* v)
{
    const double s = v[0] * v[0] + v[1] * v[1];
    if (s == kZero) return 0.0;
    return std::sqrt(s);
}

// @src 1403091c0
double normalize2(double* v)
{
    const double len = length2(v);
    if (len != kZero) {
        v[0] = v[0] / len;
        v[1] = v[1] / len;
    }
    return len;
}

// @src 14023b4c0
void cross3(const double* a, const double* b, double* out)
{
    out[0] = a[1] * b[2] - a[2] * b[1];
    out[1] = a[2] * b[0] - a[0] * b[2];
    out[2] = a[0] * b[1] - a[1] * b[0];
}

// ---- edges, vertices, loops, faces ---------------------------------------------------------------------

// @src 14041c4c0
void edge_delete_curve(DEF_EDGE* e)
{
    if (e->geo != nullptr) delete e->geo;  // (**vtable)(geo, 1)
    e->geo = nullptr;
}

// @src 14041c530
bool edge_is_closed(DEF_EDGE* e) { return e->vertices[0] == e->vertices[1]; }

// @src 14041c760
// @src 1402f83a0  erase a node of a loop list (140285df0, 1402856d0, 140286dc0)
void edge_remove_loop_at(DEF_EDGE* e, const std::int32_t* i)
{
    auto& items = e->loops.mutable_items();  // 140284de0
    items.erase(std::next(items.begin(), static_cast<std::ptrdiff_t>(*i)));
}

// @src 14041c7d0
void edge_remove_loop(DEF_EDGE* e, DEF_LOOP* l)
{
    const std::int32_t n = static_cast<std::int32_t>(e->loops.size());  // 140159c20
    for (std::int32_t i = 0; i < n; i = i + 1) {
        if (e->loop_at(&i) == static_cast<ILoop*>(l)) {  // 14041c630
            edge_remove_loop_at(e, &i);
            return;
        }
    }
}

// @src 14041c8c0
void edge_endpoints(DEF_EDGE* e, double* p0, double* p1)
{
    OBJ_GEO2D* c = as_geo2d(edge_curve(e));
    curve_endpoints(c, p0, p1);
    if (curve_is_closed(c)) {
        p0[0] = p1[0];
        p0[1] = p1[1];
    }
}

// @src 14041c9d0
bool edge_tangent_at(DEF_EDGE* e, IVertex* v, double* t)
{
    if (edge_is_closed(e)) return false;
    OBJ_GEO2D* c = as_geo2d(edge_curve(e));
    if (c == nullptr) return false;
    OBJ_GEO2D tmp;
    tmp.copy_from(c);
    double p0[2];
    double p1[2];
    curve_endpoints(&tmp, p0, p1);
    DEF_VERTEX* dv = as_vertex(v);
    if (!vertex_near(dv, p1, kPointTol)) {
        if (!vertex_near(dv, p0, kPointTol)) return false;
    } else {
        tmp.reverse();  // 1402567c0
    }
    tmp.set_curve_type(1);
    tmp.dof();
    const double x0 = tmp.points[0];
    const double y0 = tmp.points[1];
    const double y1 = tmp.points[4];
    t[0] = tmp.points[3] - x0;
    t[1] = y1 - y0;
    normalize2(t);
    return true;
}

// @src 14041cfc0
void vertex_set_xy(DEF_VERTEX* v, const double* x, const double* y)
{
    v->xy[0] = *x;
    v->xy[1] = *y;
}

// @src 14041d1c0
bool vertex_is_simple(DEF_VERTEX* v) { return v->edge_count() < 3; }

namespace {

// part of @src 14041d200 / 14041d3e0: the smallest mapped angle
IEdge* smallest_angle_edge(DEF_VERTEX* v, IEdge* e, bool ccw)
{
    const std::int32_t n = v->edge_count();  // slot 2
    if (n < 2) return nullptr;
    double t0[2] = {0.0, 0.0};  // uninitialised in the original when 14041c9d0 fails
    double ti[2] = {0.0, 0.0};
    edge_tangent_at(as_edge(e), v, t0);
    double best = kAngleStart;
    IEdge* res = nullptr;
    for (std::int32_t i = 0; i < n; i = i + 1) {
        IEdge* ei = v->edge_at(&i);  // 14041d020
        if (ei == e) continue;
        edge_tangent_at(as_edge(ei), v, ti);
        double a = signed_angle(v, t0, ti);
        if (ccw) {
            if (a < 0.0) a = kTwoPi + a;
        } else {
            if (0.0 <= a) {
                a = kTwoPi - a;
            } else {
                a = -a;  // xor with the sign mask 14051bec0
            }
        }
        if (a < best) {
            best = a;
            res = ei;
        }
    }
    return res;
}

}  // namespace

// @src 14041d200
IEdge* vertex_next_edge_ccw(DEF_VERTEX* v, IEdge* e) { return smallest_angle_edge(v, e, true); }

// @src 14041d3e0
IEdge* vertex_next_edge_cw(DEF_VERTEX* v, IEdge* e) { return smallest_angle_edge(v, e, false); }

// @src 14041d630
bool vertex_near(DEF_VERTEX* v, const double* p, double tol)
{
    return std::fabs(v->xy[0] - p[0]) < tol && std::fabs(v->xy[1] - p[1]) < tol;
}

// @src 14041d6b0
// FUN_140486fd4 is the static CRT acos (def::rt::libm::crt_acos, bit for bit).
double signed_angle(DEF_VERTEX*, double* a, double* b)
{
    normalize2(a);
    normalize2(b);
    const double d = dot2(a, b);
    double r = def::rt::libm::crt_acos(d);  // 140486fd4
    const double a3[3] = {a[0], a[1], 0.0};
    const double b3[3] = {b[0], b[1], 0.0};
    double c[3];
    cross3(a3, b3, c);
    if (c[2] < 0.0) r = -r;
    return r;
}

// @src 140413250
bool loop_same_edges(DEF_LOOP* a, DEF_LOOP* b)
{
    const std::int32_t n = a->edge_count();
    const std::int32_t m = b->edge_count();
    if (n != m) return false;
    for (std::int32_t i = 0; i < n; i = i + 1) {
        if (!loop_has_edge(b, a->edge_at(&i))) return false;
    }
    return true;
}

// @src 140413320
bool loop_has_edge(DEF_LOOP* l, IEdge* e)
{
    const std::int32_t n = l->edge_count();
    for (std::int32_t i = 0; i < n; i = i + 1) {
        if (l->edge_at(&i) == e) return true;
    }
    return false;
}

// @src 140413580
bool loop_vertices_simple(DEF_LOOP* l)
{
    const std::int32_t n = static_cast<std::int32_t>(l->edges.size());  // 140159c40
    for (std::int32_t i = 0; i < n; i = i + 1) {
        DEF_EDGE* e = as_edge(l->edge_at(&i));
        if (e == nullptr) continue;
        if (!vertex_is_simple(as_vertex(end_vertex(e, 0)))) return false;
        if (!vertex_is_simple(as_vertex(end_vertex(e, 1)))) return false;
    }
    return true;
}

// @src 1402f7eb0
// @src 1402f8340  insert before a node of a loop list (140285df0, 1402856d0, 140286cd0)
void face_insert_loop(DEF_FACE* f, const std::int32_t* i, ILoop* l)
{
    auto& items = f->loops.mutable_items();  // 140284de0
    items.insert(std::next(items.begin(), static_cast<std::ptrdiff_t>(*i)), l);
}

// @src 1402f7f20
void face_remove_loop_at(DEF_FACE* f, const std::int32_t* i)
{
    auto& items = f->loops.mutable_items();
    items.erase(std::next(items.begin(), static_cast<std::ptrdiff_t>(*i)));
}

// @src 1402f7fd0
std::int32_t face_outer_loop_first(DEF_FACE* f)
{
    bool ok = true;
    const std::int32_t n = static_cast<std::int32_t>(f->loops.size());  // 140158f10
    if (n < 2) return 1;
    std::int32_t i = 0;
    ILoop* li = nullptr;
    for (i = 0; i < n; i = i + 1) {
        li = f->loop_at(&i);  // slot 12
        ok = true;
        for (std::int32_t j = 0; j < n; j = j + 1) {
            if (j == i) continue;
            ILoop* lj = f->loop_at(&j);
            if (li->inside_bbox_of(lj)) {
                ok = false;
                break;
            }
        }
        if (ok) break;
    }
    if (i == 0) return 1;
    if (!ok) return 0;
    face_remove_loop_at(f, &i);
    std::int32_t zero = 0;
    face_insert_loop(f, &zero, li);
    return 1;
}

}  // namespace def::model::geo_kw
