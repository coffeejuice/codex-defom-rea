// OBJ_MESH2D boundary topology (1403dedd0 family) and its conversion to GEO2D_AND_MESH2D
// (1403e0f20). See defmodel/obj_mesh2d/topology.hpp. Decompile / listing:
// deform-rea-work/out/w2/p67-1-2d/d1.c, d2.c, d3.c, lst.txt. Integer code only; the one floating
// point step (1403e0f20) copies coordinates.
#include <defmodel/obj_mesh2d/topology.hpp>

#include <defmodel/geo_kw/curve_model.hpp>
#include <defmodel/layout/def_edge.hpp>
#include <defmodel/layout/def_face.hpp>
#include <defmodel/layout/def_loop.hpp>
#include <defmodel/layout/def_vertex.hpp>
#include <defmodel/layout/geo2d_and_mesh2d.hpp>
#include <defmodel/layout/obj_geo2d.hpp>
#include <defmodel/layout/obj_mesh2d.hpp>
#include <defmodel/linked/accessors.hpp>
#include <defmodel/mesh2d/geo2d_mesh_ops.hpp>
#include <defmodel/obj_mesh2d/mesh_access.hpp>

#include <cstdint>

namespace def::model {

using mesh2d::Elem10_1403e2610;
using mesh2d::Elem10_1403e2750;
using mesh2d::Elem18_1403e23f0;
using mesh2d::Elem18_1403e2450;
using mesh2d::Elem20_1403e25a0;
using mesh2d::Elem28_1403e2370;
using mesh2d::Face18;
using mesh2d::IntList;
using mesh2d::IntVector;
using mesh2d::LoopItem;
using mesh2d::RcHandle;

namespace {

// ---- p1-05 container calls, as the topology code makes them --------------------------------------

// 140220810: push_back on an int vector (detach, grow 1402217c0 / 140222010, store)
void push(IntVector& v, std::int32_t x) { v.push_back(x); }

// 1400030b0: size of an int vector, truncated to int by the callers
std::int32_t size_of(const IntVector& v) { return static_cast<std::int32_t>(v.size()); }

// 140005100: element i of an int vector, detaching
std::int32_t& at(IntVector& v, std::int32_t i)
{
    return v.mutable_at(static_cast<std::size_t>(static_cast<std::int64_t>(i)));
}

// The four "contains" members (one per element type, same code): size 1400030b0, then
// 140005100 per entry; true at the first equal value.
// 1403e2470 (vertex curve list, C08) is the same code: topology_curves.cpp
// @src 1403e24e0  curve (+0x15a8) region list contains *v
// (1403e2630 / 1403e26a0, the loop and region members, are tagged in topology_loops.cpp)
bool contains(IntVector& list, const std::int32_t* v)
{
    const std::int32_t n = size_of(list);
    for (std::int32_t i = 0; i < n; ++i) {
        if (at(list, i) == *v) return true;
    }
    return false;
}

// 140220700: push_back on an int list (detach 140159f90, end() 140159ef0, insert 14015aa90)
void list_push_back(IntList& l, std::int32_t x) { l.push_back(x); }

// @src 1403e35d0  pop_back of an int list (the "queues" of 1403deee0 / 1403df970 are stacks):
//                 the value of the last node (1403e3b20: detach, head->prev + 0x10), then, when
//                 the list is not empty (1401599b0), erase the last node (1403e3a80,
//                 1403e3ac0 -> 14015ab80)
// @src 1403e3b20  back: detach, head->prev + 0x10
// @src 1403e3a80  iterator(head->prev), detaching
// @src 1403e3ac0  erase(it): detach, 14015ab80
// @src 14015ab80  unlink and free the node, size--
std::int32_t list_pop_back(IntList& l)
{
    // On an empty list the binary reads the value slot of the list's sentinel node (never
    // written, so indeterminate); std::list::back() is undefined there, so the port returns 0.
    // Both callers pop only after !list_empty(queue), so the empty case is never reached.
    auto& items = l.mutable_items();
    const std::int32_t v = items.empty() ? 0 : items.back();
    if (!l.empty()) items.pop_back();
    return v;
}

// The local int lists ("queues") of 1403deee0 / 1403df970:
// @src 1403e3590  ctor: 140159860 (IntList())
// @src 1403e35b0  dtor: 140159930 (~IntList())
// @src 1401599b0  int list empty (body + 0x10 == 0)
bool list_empty(const IntList& l) { return l.empty(); }

// 140159950: int list size; 140159a70: element i, detaching (p1-05 / surface_sets)
std::int32_t list_size(const IntList& l) { return static_cast<std::int32_t>(l.size()); }
std::int32_t& list_at(IntList& l, std::int32_t i)
{
    return l.mutable_at(static_cast<std::size_t>(static_cast<std::int64_t>(i)));
}

// 1403e31d0: loop list element i, detaching
LoopItem& loop_at(mesh2d::TopoLoopList& l, std::int32_t i)
{
    return l.mutable_at(static_cast<std::size_t>(static_cast<std::int64_t>(i)));
}

} // namespace

// @src 1403dedd0  clear six arrays, regions, curves and loops
void fun_1403dedd0(OBJ_MESH2D* m)
{
    RcHandle<Elem28_1403e2370>(m->h1588).clear();  // 1403e29c0
    RcHandle<Elem18_1403e2450>(m->h1590).clear();  // 1403e2b80
    RcHandle<Face18>(m->h1598).clear();            // 1403e2d20
    RcHandle<Elem20_1403e25a0>(m->h15a8).clear();  // 1403e3070
    RcHandle<Elem10_1403e2750>(m->h15b0).clear();  // 1403e33a0
    RcHandle<Elem18_1403e23f0>(m->h15b8).clear();  // 1403e3560
    fun_1403deee0(m);
    fun_1403df940(m);
}

// @src 1403dee60  clear six arrays (not +0x15a0)
void fun_1403dee60(OBJ_MESH2D* m)
{
    RcHandle<Elem28_1403e2370>(m->h1588).clear();  // 1403e29c0
    RcHandle<Elem18_1403e2450>(m->h1590).clear();  // 1403e2b80
    RcHandle<Face18>(m->h1598).clear();            // 1403e2d20
    RcHandle<Elem20_1403e25a0>(m->h15a8).clear();  // 1403e3070
    RcHandle<Elem10_1403e2750>(m->h15b0).clear();  // 1403e33a0
    RcHandle<Elem18_1403e23f0>(m->h15b8).clear();  // 1403e3560
}

// @src 1403deee0  nodes, element records, sides, then the regions by flood fill
void fun_1403deee0(OBJ_MESH2D* m)
{
    RcHandle<Elem28_1403e2370> nodes(m->h1588);
    RcHandle<Elem18_1403e2450> sides(m->h1590);
    RcHandle<Face18> faces(m->h1598);
    RcHandle<Elem18_1403e23f0> regions(m->h15b8);

    Elem28_1403e2370 node_init;  // local_c0 (1403e2320)
    node_init.vertex = -1;
    node_init.u20 = -1;
    node_init.u24 = -1;
    nodes.resize(static_cast<std::size_t>(static_cast<std::int64_t>(mesh2d_count_08(m))), node_init);  // 140284aa0, 1403e2920
    const Face18 face_init = {-1, -1, {-1, -1, -1, -1}};  // local_80..local_6c
    faces.resize(static_cast<std::size_t>(static_cast<std::int64_t>(mesh2d_count_148(m))), face_init);  // 140284ac0, 1403e2c80

    std::int32_t e = 0;                      // [0x50]
    std::int32_t k = 0;                      // [0x4c]
    std::int32_t nd[4] = {-1, -1, -1, -1};  // [0x28]
    std::int32_t side = -1;                  // [0x20]
    const std::int32_t* conn = fun_140220460(m);
    const std::int32_t ne = mesh2d_count_148(m);
    for (e = 0; e < ne; ++e) {
        for (k = 0; k < 4; ++k) nd[k] = conn[k + e * 4] - 1;
        for (k = 0; k < 4; ++k) {
            side = fun_1403df640(m, &nd[k], &nd[(k + 1) % 4], &e);
            faces.at(e).edge[k] = side;                   // 1403e2c40
            const std::int32_t* mat = fun_140220480(m);   // 140183d00 on +0x148
            faces.at(e).mat = mat[e];
        }
    }

    std::int32_t seed = -1;                  // [0x48]
    std::int32_t region = -1;                // [0x90]
    std::int32_t mat = -1;                   // [0xb0]
    IntList queue;  // used as a stack                           // [0xc0] 1403e3590 -> 140159860
    Face18* f = nullptr;                     // [0x88]
    Elem18_1403e2450* s = nullptr;           // [0x40]
    Elem18_1403e23f0* re = nullptr;          // [0x38]
    std::int32_t j = -1;                     // [0x80]
    std::int32_t other = -1;                 // [0xc8]
    for (;;) {
        seed = fun_1403df8d0(m);
        if (seed < 0) break;
        region = region + 1;
        f = &faces.at(seed);
        f->region = region;
        mat = f->mat;
        Elem18_1403e23f0 tmp;                // [0xd0] 1403e23b0
        tmp.mat = mat;
        regions.push_back(tmp);              // 1403e34c0
        re = &regions.at(region);            // 1403e3480
        list_push_back(queue, seed);
        while (!list_empty(queue)) {
            seed = list_pop_back(queue);
            f = &faces.at(seed);
            push(re->elems, seed);
            for (j = 0; j < 4; ++j) {
                side = f->edge[j];
                s = &sides.at(side);         // 1403e2aa0
                const std::int32_t cnt = size_of(s->elems);
                if (cnt == 1) s->kind = 1;
                for (e = 0; e < cnt; ++e) {
                    other = at(s->elems, e);
                    if (other == seed) continue;
                    Face18* of = &faces.at(other);
                    if (of->region < 0) {
                        if (of->mat == mat) {
                            s->kind = 0;
                            of->region = region;
                            list_push_back(queue, other);
                        } else {
                            s->kind = 2;
                        }
                    } else if (of->region == region) {
                        s->kind = 0;
                    } else {
                        s->kind = 2;
                    }
                }
            }
        }
    }  // tmp: 1403e23f0 per iteration; queue 1403e35b0; node_init 1403e2370
}

// @src 1403df640  side of nodes *a, *b for element *elem
std::int32_t fun_1403df640(OBJ_MESH2D* m, const std::int32_t* a, const std::int32_t* b, const std::int32_t* elem)
{
    RcHandle<Elem28_1403e2370> nodes(m->h1588);
    RcHandle<Elem18_1403e2450> sides(m->h1590);
    const std::int32_t na = size_of(nodes.at(*a).edges);  // 1403e28e0, 1400030b0
    const std::int32_t nb = size_of(nodes.at(*b).edges);
    std::int32_t found = -1;
    std::int32_t ea = -1;
    std::int32_t eb = -1;
    for (std::int32_t i = 0; i < na; ++i) {
        ea = at(nodes.at(*a).edges, i);
        for (std::int32_t k = 0; k < nb; ++k) {
            eb = at(nodes.at(*b).edges, k);
            if (ea == eb) {
                found = ea;
                push(sides.at(ea).elems, *elem);  // 1403e2aa0 + 8
                return found;
            }
        }
    }
    Elem18_1403e2450 tmp;  // local_50 (1403e2420)
    tmp.kind = -1;
    push(tmp.elems, *elem);
    tmp.n0 = *a;
    tmp.n1 = *b;
    tmp.curve = -1;
    sides.push_back(tmp);                          // 1403e2ae0
    found = sides.size();                          // 1403e2a80
    found = found - 1;
    push(nodes.at(*a).edges, found);
    push(nodes.at(*b).edges, found);
    return found;                                  // tmp: 1403e2450
}

// @src 1403df8d0  first element with region < 0
std::int32_t fun_1403df8d0(OBJ_MESH2D* m)
{
    RcHandle<Face18> faces(m->h1598);
    const std::int32_t n = mesh2d_count_148(m);  // 140284ac0
    for (std::int32_t i = 0; i < n; ++i) {
        if (faces.at(i).region < 0) return i;
    }
    return -1;
}

// 1403df940, 1403df970, 1403dfef0, 1403dff90, 1403e0cd0 (curves): topology_curves.cpp (census packet C08)
// 1403e0070 (loops) and 1403e0bd0: topology_loops.cpp (census packet C10)

// @src 1403e0f20  topology of g from the arrays
void fun_1403e0f20(OBJ_MESH2D* m, GEO2D_AND_MESH2D* g)
{
    RcHandle<Elem10_1403e2610> vertices(m->h15a0);
    RcHandle<Elem20_1403e25a0> curves(m->h15a8);
    RcHandle<Elem10_1403e2750> loops(m->h15b0);
    RcHandle<Elem18_1403e23f0> regions(m->h15b8);

    g->clear_topology();                              // 14027fa30
    const std::int32_t nreg = regions.size();         // 1403e3460
    for (std::int32_t i = 0; i < nreg; ++i) geo2d_add_face(g);    // 1402838d0
    const std::int32_t nloop = loops.size();          // 1403e32a0
    for (std::int32_t i = 0; i < nloop; ++i) geo2d_add_loop(g);   // 140283980
    const std::int32_t ncurve = curves.size();        // 1403e2f70
    for (std::int32_t i = 0; i < ncurve; ++i) g->add_edge_curve(); // 14027fce0
    const std::int32_t nvert = vertices.size();       // 1403e2de0
    for (std::int32_t i = 0; i < nvert; ++i) geo2d_add_vertex(g); // 140283a30

    const double* xy = fun_140220440(m);
    for (std::int32_t i = 0; i < nvert; ++i) {
        Elem10_1403e2610* ve = &vertices.at(i);       // 1403e2e00
        const std::int32_t n = size_of(ve->curves);
        DEF_VERTEX* v = geo2d_vertex_at(g, &i);       // 1402840e0
        for (std::int32_t k = 0; k < n; ++k) {
            DEF_EDGE* e = g->edge_at(at(ve->curves, k));  // 1402800a0
            v->add_edge(e);                               // 14041d0b0
        }
        v->set_xy(xy + static_cast<std::int64_t>(ve->node * 2));  // 14041cf80
    }

    for (std::int32_t c = 0; c < ncurve; ++c) {
        Elem20_1403e25a0* ce = &curves.at(c);         // 1403e2f90
        const std::int32_t np = list_size(ce->nodes);
        DEF_EDGE* e = g->edge_at(c);
        std::int32_t id = c + 1;
        e->set_id(&id);                               // 14041c5f0
        const std::int32_t nl = size_of(ce->loops);
        for (std::int32_t k = 0; k < nl; ++k) {
            std::int32_t li = at(ce->loops, k);
            DEF_LOOP* l = geo2d_loop_at(g, &li);      // 140284030
            e->add_loop(l);                           // 14041c6c0
        }
        std::int32_t vi = ce->v0;
        linked::set_u64_18(e, reinterpret_cast<std::uint64_t>(geo2d_vertex_at(g, &vi)));  // 140256730
        vi = ce->v1;
        linked::set_u64_20(e, reinterpret_cast<std::uint64_t>(geo2d_vertex_at(g, &vi)));  // 140256750
        OBJ_GEO2D* cv = new OBJ_GEO2D();              // operator new(0x30), 140275cd0
        e->set_geo(cv);                               // 14041c4a0 (the curve 14027fce0 made is left)
        cv->n_points = np;
        cv->curve_type = 1;
        cv->clear_data();                             // slot 3: allocate_points
        for (std::int32_t p = 0; p < np; ++p) {
            const std::int32_t nd = list_at(ce->nodes, p);
            cv->points[p * 3] = xy[nd * 2];
            cv->points[p * 3 + 1] = xy[nd * 2 + 1];
            cv->points[p * 3 + 2] = 0.0;
        }
        cv->fun_1402765e0();
    }

    for (std::int32_t li = 0; li < nloop; ++li) {
        Elem10_1403e2750* le = &loops.at(li);         // 1403e32c0
        DEF_LOOP* l = geo2d_loop_at(g, &li);
        const std::int32_t n = static_cast<std::int32_t>(le->items.size());  // 1403e3130
        for (std::int32_t k = 0; k < n; ++k) {
            DEF_EDGE* e = g->edge_at(loop_at(le->items, k).curve);
            l->add_edge(e);                           // 1404121e0
        }
        const std::int32_t nr = size_of(le->regions);
        for (std::int32_t k = 0; k < nr; ++k) {
            std::int32_t r = at(le->regions, k);
            DEF_FACE* f = geo2d_face_at(g, &r);       // 140283f80
            l->add_face(f);                           // 140412440
        }
        std::int32_t id = li + 1;
        loop_set_number(l, &id);                      // 140284b60
        l->order_edges();                             // 140412da0
    }

    for (std::int32_t r = 0; r < nreg; ++r) {
        Elem18_1403e23f0* re = &regions.at(r);        // 1403e3480
        DEF_FACE* f = geo2d_face_at(g, &r);
        const std::int32_t nl = size_of(re->loops);
        for (std::int32_t k = 0; k < nl; ++k) {
            std::int32_t li = at(re->loops, k);
            f->add_loop(geo2d_loop_at(g, &li));       // 1402f7e80
        }
        std::int32_t id = r + 1;
        face_set_number(f, &id);                      // 140284b40
        f->set_u40(&re->mat);                         // 1402f7f90
        geo_kw::face_outer_loop_first(f);             // 1402f7fd0
    }
}

// @src 140220430  +0x28
OBJ_MESH2D* fun_140220430(GEO2D_AND_MESH2D* g) { return g->mesh; }

// @src 1402815a0  mesh present with nodes (140284aa0), elements (140284ac0), coordinates
//                 (140220440) and connectivity (140220460)
bool fun_1402815a0(GEO2D_AND_MESH2D* g)
{
    if (g->mesh == nullptr) return false;
    if (mesh2d_count_08(g->mesh) < 1) return false;
    if (mesh2d_count_148(g->mesh) < 1) return false;
    if (fun_140220440(g->mesh) == nullptr) return false;
    if (fun_140220460(g->mesh) == nullptr) return false;
    return true;
}

} // namespace def::model
