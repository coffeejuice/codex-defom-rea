// _DEF_EDGE methods (layout: defmodel/layout/def_edge.hpp).
#include <defmodel/layout/def_edge.hpp>
#include <defmodel/mesh2d/geo2d_mesh_ops.hpp>

#include <defmodel/port.hpp>
#include <defmodel/topo/geo_fallback.hpp>

namespace def::model {

// @src 14041c300  ctor: IEdge ctor 14041ce00, vptr, loops 140284c20, u28 1402f8400, zeroes,
// then 140284af0 (u34 = u35 = 0)
DEF_EDGE::DEF_EDGE() : geo(nullptr), vertices{nullptr, nullptr}, id(0), u34(0), u35(0) {}

// @src 14041c3a0  dtor: the curve through its scalar deleting dtor (slot 0, flag 1), then u28
// and loops (folded handle dtors)
// @src 14041cdc0  scalar deleting dtor (slot 5)
DEF_EDGE::~DEF_EDGE() {
    OBJ_GEO_BASE* g = geo;
    if (g != nullptr) delete g;
}

// @src 14041c430  slot 0
void DEF_EDGE::vslot0(void*) {}

// @src 14041c450  slot 1
void DEF_EDGE::vslot1() {}

// @src 14041c460  slot 2: this + 0x28
topo::CowPtrVector<void>* DEF_EDGE::u28_vector() { return &u28; }

// @src 14041cdb0  slot 3
std::int32_t DEF_EDGE::vslot3() { return -1; }

// @src 14041c480  slot 4
void DEF_EDGE::vslot4(void*) {}

// @src 14041c4a0  geo = g (the old curve is not deleted)
void DEF_EDGE::set_geo(OBJ_GEO_BASE* g) { geo = g; }

// @src 14041c590  *(this + 0x18 + i * 8)
IVertex* DEF_EDGE::vertex(const std::int32_t* i) const { return vertices[*i]; }

// @src 14041c5c0  loops size (140159c20)
std::int32_t DEF_EDGE::loop_count() { return topo::list_size(loops); }

// @src 14041c5f0  id = *v
void DEF_EDGE::set_id(const std::int32_t* v) { id = *v; }

// @src 14041c610  id
std::int32_t DEF_EDGE::get_id() const { return id; }

// @src 14041c630  loop i through loop_count; 0 when out of range
ILoop* DEF_EDGE::loop_at(const std::int32_t* i) {
    const std::int32_t n = loop_count();
    if (n < 1) return nullptr;
    if (*i < 0) return nullptr;
    if (*i < n) return topo::list_at(loops, *i);
    return nullptr;
}

// @src 14041c6c0  loops push_back (140284d30)
void DEF_EDGE::add_loop(ILoop* l) { topo::list_push_back(loops, l); }

// @src 14041cc10  dynamic_cast of the curve to _OBJ_GEO2D, unused; false
bool DEF_EDGE::fun_14041cc10() {
    (void)dynamic_cast<OBJ_GEO2D*>(get_geo());
    return false;
}

// @src 14041cc70  dynamic_cast of the curve to _OBJ_GEO2D, unused
void DEF_EDGE::fun_14041cc70() { (void)dynamic_cast<OBJ_GEO2D*>(get_geo()); }

// @src 14041ccd0  true when the curve is not a _OBJ_GEO2D, else its flag at +0x28
bool DEF_EDGE::curve_flag28() {
    OBJ_GEO2D* g = dynamic_cast<OBJ_GEO2D*>(get_geo());
    if (g == nullptr) return true;
    return g->get_flag28(); // 14041ce20
}

// @src 14041cd40  set the curve's flag at +0x28 (14027f410) when it is a _OBJ_GEO2D
void DEF_EDGE::set_curve_flag28(bool v) {
    OBJ_GEO2D* g = dynamic_cast<OBJ_GEO2D*>(get_geo());
    if (g != nullptr) g->set_flag28(v);
}

// @src 14041ce20  _OBJ_GEO2D: the byte at +0x28 (method of p1-07's class placed in this range)
bool OBJ_GEO2D::get_flag28() const { return flag28 != 0; }

// Field accessors that p1-08 owns (geo2d_mesh_ops.hpp), wired at the Phase 1 gate.
OBJ_GEO_BASE* DEF_EDGE::get_geo() const { return edge_curve(this); }           // 140284990
bool DEF_EDGE::visited() const { return edge_get_34(this) != 0; }                 // 140284a10
void DEF_EDGE::set_visited(bool v) { edge_set_34(this, v ? 1 : 0); }              // 140284a30

} // namespace def::model
