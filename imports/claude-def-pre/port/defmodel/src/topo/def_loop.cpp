// _DEF_LOOP methods (layout: defmodel/layout/def_loop.hpp).
#include <defmodel/layout/def_loop.hpp>

#include <defmodel/layout/def_edge.hpp>
#include <defmodel/port.hpp>
#include <defmodel/topo/geo_fallback.hpp>
#include <defmodel/topo/loop_geo2d.hpp>

namespace def::model {

using topo::list_at;
using topo::list_size;

// @src 140412010  ctor: ILoop ctor 140413710, vptr, edges 140284e30, faces 140285080, u1c = 0
DEF_LOOP::DEF_LOOP() : u1c(0) {}

// @src 140412080  dtor: faces, then edges (folded handle dtors)
// @src 1404136d0  scalar deleting dtor (slot 5)
DEF_LOOP::~DEF_LOOP() = default;

// @src 1404120d0  slot 0
void DEF_LOOP::vslot0(void*) {}

// @src 1404120f0  slot 1
void DEF_LOOP::vslot1() {}

// @src 140412100  slot 2
void DEF_LOOP::vslot2(void*) {}

// @src 140412120  slot 3: edges size (140159c40)
std::int32_t DEF_LOOP::edge_count() { return list_size(edges); }

// @src 140412150  edge i through slot 3; 0 when the list is empty or i is out of range
IEdge* DEF_LOOP::edge_at(const std::int32_t* i) {
    const std::int32_t n = edge_count();
    if (n < 1) return nullptr;
    if (*i < 0) return nullptr;
    if (*i < n) return list_at(edges, *i);
    return nullptr;
}

// @src 1404121e0  edges push_back (140284f40)
void DEF_LOOP::add_edge(IEdge* e) { topo::list_push_back(edges, e); }

// @src 1404123b0  face i through face_count; 0 when out of range
IFace* DEF_LOOP::face_at(const std::int32_t* i) {
    const std::int32_t n = face_count();
    if (n < 1) return nullptr;
    if (*i < 0) return nullptr;
    if (*i < n) return list_at(faces, *i);
    return nullptr;
}

// @src 140412440  faces push_back (140285190)
void DEF_LOOP::add_face(IFace* f) { topo::list_push_back(faces, f); }

// @src 1404125e0  faces size (140159c60)
std::int32_t DEF_LOOP::face_count() { return list_size(faces); }

// @src 1404133a0  first edge of list, not visited, with vertex v at either end
IEdge* DEF_LOOP::find_unvisited_edge(IVertex* v, EdgeList* list) {
    const std::int32_t n = list_size(*list);
    for (std::int32_t i = 0; i < n; ++i) {
        IEdge* p = list_at(*list, i);
        DEF_EDGE* e = p == nullptr ? nullptr : dynamic_cast<DEF_EDGE*>(p);
        if (!e->visited()) {
            std::int32_t k = 0;
            if (e->vertex(&k) == v) return p;
            k = 1;
            if (e->vertex(&k) == v) return p;
        }
    }
    return nullptr;
}

// @src 140412da0  reorder edges into a chain: start with edge 0, extend from its second vertex,
// then prepend from its first vertex; true when both ends meet. Visited flags end cleared.
bool DEF_LOOP::order_edges() {
    bool closed = false;
    EdgeList work; // 140284e30
    const std::int32_t n = edge_count();
    if (n < 1) return false;

    auto as_edge = [](IEdge* p) { return p == nullptr ? nullptr : dynamic_cast<DEF_EDGE*>(p); };
    auto vtx = [](DEF_EDGE* e, std::int32_t k) { return e->vertex(&k); };

    for (std::int32_t i = 0; i < n; ++i) {
        DEF_EDGE* e = as_edge(edge_at(&i));
        e->set_visited(false);
        IEdge* p = e; // pushes the _DEF_EDGE pointer (local_78 = local_88)
        topo::list_push_back(work, p);
    }
    topo::list_clear(edges); // 140286770

    IEdge* p0 = list_at(work, 0);
    DEF_EDGE* e = as_edge(p0);
    e->set_visited(true);
    IVertex* tail_start = vtx(e, 0); // local_b0
    IVertex* tail_end = vtx(e, 1);   // local_b8
    { IEdge* q = e; topo::list_push_back(edges, q); }

    for (;;) {
        e = as_edge(find_unvisited_edge(tail_end, &work));
        if (e == nullptr) break;
        e->set_visited(true);
        if (vtx(e, 0) == tail_end) {
            tail_end = vtx(e, 1);
        } else if (vtx(e, 1) == tail_end) {
            tail_end = vtx(e, 0);
        }
        IEdge* q = e;
        topo::list_push_back(edges, q);
    }
    for (;;) {
        e = as_edge(find_unvisited_edge(tail_start, &work));
        if (e == nullptr) break;
        e->set_visited(true);
        if (vtx(e, 0) == tail_start) {
            tail_start = vtx(e, 1);
        } else if (vtx(e, 1) == tail_start) {
            tail_start = vtx(e, 0);
        }
        IEdge* q = e;
        topo::list_push_front(edges, q); // 140413790
    }
    if (tail_start == tail_end) closed = true;

    for (std::int32_t i = 0; i < n; ++i) {
        as_edge(list_at(work, i))->set_visited(false);
    }
    return closed;
}

// part of @src 1404126d0  (the body is topo::loop_to_geo2d)
void DEF_LOOP::to_geo2d(OBJ_GEO2D* out) { topo::loop_to_geo2d<OBJ_GEO2D>(this, out); }

// part of @src 140412610  (the body is topo::loop_inside_bbox)
bool DEF_LOOP::inside_bbox_of(ILoop* other) { return topo::loop_inside_bbox<OBJ_GEO2D>(this, other); }

} // namespace def::model
