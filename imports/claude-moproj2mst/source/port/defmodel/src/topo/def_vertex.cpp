// _DEF_VERTEX methods (layout: defmodel/layout/def_vertex.hpp).
#include <defmodel/layout/def_vertex.hpp>

#include <defmodel/port.hpp>

namespace def::model {

// @src 14041ce30  ctor: IVertex ctor 14041d810, vptr, edges 140284e30, u20 = 0, xy = 0
DEF_VERTEX::DEF_VERTEX() : xy{0.0, 0.0}, u20(0) {}

// @src 14041cea0  dtor: edges (folded handle dtor)
// @src 14041d7d0  scalar deleting dtor (slot 3)
DEF_VERTEX::~DEF_VERTEX() = default;

// @src 14041cee0  slot 0: clear out (14041d8d0), then push every edge (14041d830) by index
void DEF_VERTEX::collect_edges(topo::CowPtrVector<IEdge>* out) {
    IEdge* e = nullptr;
    out->clear();
    const std::int32_t n = edge_count(); // slot 2
    for (std::int32_t i = 0; i < n; ++i) {
        e = edge_at(&i);
        out->push_back(&e);
    }
}

// @src 14041cf70  slot 1
void DEF_VERTEX::vslot1() {}

// @src 14041d7b0  slot 2: edges size (140159c40)
std::int32_t DEF_VERTEX::edge_count() { return topo::list_size(edges); }

// @src 14041cf80  xy[0] = in[0], xy[1] = in[1] (copied as 8-byte words)
void DEF_VERTEX::set_xy(const double* in) {
    xy[0] = in[0];
    xy[1] = in[1];
}

// @src 14041d000  this + 8
double* DEF_VERTEX::get_xy() { return xy; }

// @src 14041d020  edge i through slot 2; 0 when out of range
IEdge* DEF_VERTEX::edge_at(const std::int32_t* i) {
    const std::int32_t n = edge_count();
    if (n < 1) return nullptr;
    if (*i < 0) return nullptr;
    if (*i < n) return topo::list_at(edges, *i);
    return nullptr;
}

// @src 14041d0b0  edges push_back (140284f40)
void DEF_VERTEX::add_edge(IEdge* e) { topo::list_push_back(edges, e); }

} // namespace def::model
