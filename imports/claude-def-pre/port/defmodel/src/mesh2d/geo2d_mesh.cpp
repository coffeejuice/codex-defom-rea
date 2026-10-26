// GEO2D_AND_MESH2D topology helpers, topology field accessors and OBJ_MESH2D getters (p1-08).
// See geo2d_mesh_ops.hpp for why fields are read by offset and other packets are called
// through mesh2d::Ext (defaults wired to the owners at the Phase 1 gate; tests install fakes).
#include <defmodel/mesh2d/geo2d_mesh_ops.hpp>

#include <defmodel/layout/def_edge.hpp>
#include <defmodel/layout/def_face.hpp>
#include <defmodel/layout/def_loop.hpp>
#include <defmodel/layout/def_vertex.hpp>
#include <defmodel/layout/geo2d_and_mesh2d.hpp>
#include <defmodel/layout/obj_geo_base.hpp>
#include <defmodel/layout/obj_mesh2d.hpp>
#include <defmodel/port.hpp>

namespace def::model {

namespace mesh2d {
namespace {

// Default targets: the owners' code (wired at the Phase 1 gate).
DEF_FACE* x_new_face() { return new DEF_FACE(); }       // operator new(0x48) + 1402f7b20 (p1-07)
DEF_LOOP* x_new_loop() { return new DEF_LOOP(); }       // operator new(0x20) + 140412010 (p1-11)
DEF_VERTEX* x_new_vertex() { return new DEF_VERTEX(); } // operator new(0x28) + 14041ce30 (p1-11)
DEF_FACE* x_face_cast(IFace* p) { return dynamic_cast<DEF_FACE*>(p); }
DEF_LOOP* x_loop_cast(ILoop* p) { return dynamic_cast<DEF_LOOP*>(p); }
DEF_VERTEX* x_vertex_cast(IVertex* p) { return dynamic_cast<DEF_VERTEX*>(p); }
DEF_EDGE* x_edge_at(GEO2D_AND_MESH2D* g, std::int32_t i) { return g->edge_at(i); }             // 1402800a0
void x_edge_set_number(DEF_EDGE* e, const std::int32_t* v) { e->set_id(v); }                   // 14041c5f0
void x_edge_set_curve_flag(DEF_EDGE* e, std::uint8_t v) { e->set_curve_flag28(v != 0); }       // 14041cd40
std::int32_t x_geo_slot11(OBJ_GEO_BASE* o) { return o->is_extension(); }                      // slot 11

const Ext k_defaults = {x_new_face,   x_new_loop,       x_new_vertex,         x_face_cast,
                        x_loop_cast,  x_vertex_cast,    x_edge_at,            x_edge_set_number,
                        x_edge_set_curve_flag, x_geo_slot11};

Ext g_ext = k_defaults;

} // namespace

Ext& ext() { return g_ext; }
void reset_ext() { g_ext = k_defaults; }

} // namespace mesh2d

using mesh2d::ext;
using mesh2d::field;

FaceList& geo2d_faces(GEO2D_AND_MESH2D* g) { return g->faces; }
LoopList& geo2d_loops(GEO2D_AND_MESH2D* g) { return g->loops; }
EdgeList& geo2d_edges(GEO2D_AND_MESH2D* g) { return g->edges; }
VertexList& geo2d_vertices(GEO2D_AND_MESH2D* g) { return g->vertices; }

// Counts as the original reads them: 140158f30 / 140256780 / 140159190 / 1402567a0 return the
// list's 64-bit size, truncated to int by the caller.
static std::int32_t face_count(GEO2D_AND_MESH2D* g) { return static_cast<std::int32_t>(geo2d_faces(g).size()); }
static std::int32_t loop_count(GEO2D_AND_MESH2D* g) { return static_cast<std::int32_t>(geo2d_loops(g).size()); }
static std::int32_t edge_count(GEO2D_AND_MESH2D* g) { return static_cast<std::int32_t>(geo2d_edges(g).size()); }
static std::int32_t vertex_count(GEO2D_AND_MESH2D* g)
{
    return static_cast<std::int32_t>(geo2d_vertices(g).size());
}

// ---- create ------------------------------------------------------------------------------------

// @src 1402838d0  new face: operator new(0x48) + ctor, number = face count + 1, append
DEF_FACE* geo2d_add_face(GEO2D_AND_MESH2D* g)
{
    DEF_FACE* f = ext().new_face();
    std::int32_t n = face_count(g);
    n = n + 1;
    face_set_number(f, &n);
    IFace* p = reinterpret_cast<IFace*>(f); // IFace is the primary base (offset 0)
    geo2d_faces(g).push_back(p);
    return f;
}

// @src 140283980  new loop: operator new(0x20) + ctor, number = loop count + 1, append
DEF_LOOP* geo2d_add_loop(GEO2D_AND_MESH2D* g)
{
    DEF_LOOP* l = ext().new_loop();
    std::int32_t n = loop_count(g);
    n = n + 1;
    loop_set_number(l, &n);
    ILoop* p = reinterpret_cast<ILoop*>(l);
    geo2d_loops(g).push_back(p);
    return l;
}

// @src 140283a30  new vertex: operator new(0x28) + ctor, number = vertex count + 1, append
DEF_VERTEX* geo2d_add_vertex(GEO2D_AND_MESH2D* g)
{
    DEF_VERTEX* v = ext().new_vertex();
    std::int32_t n = vertex_count(g);
    n = n + 1;
    vertex_set_number(v, &n);
    IVertex* p = reinterpret_cast<IVertex*>(v);
    geo2d_vertices(g).push_back(p);
    return v;
}

// ---- lookup ------------------------------------------------------------------------------------

// @src 140283f80  face *i
DEF_FACE* geo2d_face_at(GEO2D_AND_MESH2D* g, const std::int32_t* i)
{
    const std::int32_t n = face_count(g);
    if (n < 1) return nullptr;
    if (*i < 0) return nullptr;
    if (*i >= n) return nullptr;
    IFace* p = geo2d_faces(g).mutable_at(static_cast<std::size_t>(static_cast<std::int64_t>(*i)));
    return ext().face_cast(p);
}

// @src 140284030  loop *i
DEF_LOOP* geo2d_loop_at(GEO2D_AND_MESH2D* g, const std::int32_t* i)
{
    const std::int32_t n = loop_count(g);
    if (n < 1) return nullptr;
    if (*i < 0) return nullptr;
    if (*i >= n) return nullptr;
    ILoop* p = geo2d_loops(g).mutable_at(static_cast<std::size_t>(static_cast<std::int64_t>(*i)));
    return ext().loop_cast(p);
}

// @src 1402840e0  vertex *i
DEF_VERTEX* geo2d_vertex_at(GEO2D_AND_MESH2D* g, const std::int32_t* i)
{
    const std::int32_t n = vertex_count(g);
    if (n < 1) return nullptr;
    if (*i < 0) return nullptr;
    if (*i >= n) return nullptr;
    IVertex* p = geo2d_vertices(g).mutable_at(static_cast<std::size_t>(static_cast<std::int64_t>(*i)));
    return ext().vertex_cast(p);
}

// ---- renumber ----------------------------------------------------------------------------------

// @src 140284190  vertices 1..n (no null check on the cast result)
void geo2d_renumber_vertices(GEO2D_AND_MESH2D* g)
{
    const std::int32_t n = vertex_count(g);
    std::int32_t number = 1;
    for (std::int32_t i = 0; i < n; i = i + 1) {
        DEF_VERTEX* v = geo2d_vertex_at(g, &i);
        vertex_set_number(v, &number);
        number = number + 1;
    }
}

// @src 140284220  edges 1..n through 1402800a0 / 14041c5f0
void geo2d_renumber_edges(GEO2D_AND_MESH2D* g)
{
    const std::int32_t n = edge_count(g);
    std::int32_t number = 1;
    for (std::int32_t i = 0; i < n; i = i + 1) {
        DEF_EDGE* e = ext().edge_at(g, i);
        ext().edge_set_number(e, &number);
        number = number + 1;
    }
}

// @src 1402842b0  loops 1..n
void geo2d_renumber_loops(GEO2D_AND_MESH2D* g)
{
    const std::int32_t n = loop_count(g);
    std::int32_t number = 1;
    for (std::int32_t i = 0; i < n; i = i + 1) {
        DEF_LOOP* l = geo2d_loop_at(g, &i);
        loop_set_number(l, &number);
        number = number + 1;
    }
}

// @src 140284340  faces 1..n
void geo2d_renumber_faces(GEO2D_AND_MESH2D* g)
{
    const std::int32_t n = face_count(g);
    std::int32_t number = 1;
    for (std::int32_t i = 0; i < n; i = i + 1) {
        DEF_FACE* f = geo2d_face_at(g, &i);
        face_set_number(f, &number);
        number = number + 1;
    }
}

// ---- flags -------------------------------------------------------------------------------------

// @src 1402847d0  +0xb8 = *v; on 0 every existing edge gets 14041cd40(edge, 0)
void geo2d_set_b8(GEO2D_AND_MESH2D* g, const char* v)
{
    field<char>(g, 0xb8) = *v; // GEO2D_AND_MESH2D::ub8, raw byte copy
    if (*v == '\0') {
        const std::int32_t n = edge_count(g);
        for (std::int32_t i = 0; i < n; i = i + 1) {
            DEF_EDGE* e = ext().edge_at(g, i);
            if (e != nullptr) ext().edge_set_curve_flag(e, 0);
        }
    }
}

// @src 1402849a0  address of the block at +0x38
void* geo2d_block_38(GEO2D_AND_MESH2D* g) { return g->matrix; }

// @src 1402849b0  set byte +0x30
void geo2d_set_30(GEO2D_AND_MESH2D* g, std::uint8_t v) { field<std::uint8_t>(g, 0x30) = v; } // u30, raw byte

// @src 1402849d0  get byte +0x30
std::uint8_t geo2d_get_30(const GEO2D_AND_MESH2D* g) { return field<std::uint8_t>(g, 0x30); } // u30, raw byte

// @src 140284960  o unless virtual slot 11 (+0x58) returns nonzero
OBJ_GEO_BASE* geo_if_slot11_zero(OBJ_GEO_BASE* o)
{
    if (ext().geo_slot11(o) != 0) o = nullptr;
    return o;
}

// ---- topology accessors ------------------------------------------------------------------------

// @src 140284990  _DEF_EDGE +0x08 (its curve object)
OBJ_GEO_BASE* edge_curve(const DEF_EDGE* e) { return field<OBJ_GEO_BASE*>(e, 0x08); }
// @src 140284a10  _DEF_EDGE byte +0x34
std::uint8_t edge_get_34(const DEF_EDGE* e) { return field<std::uint8_t>(e, 0x34); }
// @src 140284a20  _DEF_EDGE byte +0x35
std::uint8_t edge_get_35(const DEF_EDGE* e) { return field<std::uint8_t>(e, 0x35); }
// @src 140284a30  _DEF_EDGE byte +0x34
void edge_set_34(DEF_EDGE* e, std::uint8_t v) { field<std::uint8_t>(e, 0x34) = v; }
// @src 140284a50  _DEF_EDGE byte +0x35
void edge_set_35(DEF_EDGE* e, std::uint8_t v) { field<std::uint8_t>(e, 0x35) = v; }
// @src 140284af0  _DEF_EDGE bytes +0x34, +0x35 = 0
void edge_clear_34_35(DEF_EDGE* e)
{
    field<std::uint8_t>(e, 0x34) = 0;
    field<std::uint8_t>(e, 0x35) = 0;
}
// @src 1402849e0  _DEF_LOOP byte +0x1c
void loop_set_1c(DEF_LOOP* l, std::uint8_t v) { field<std::uint8_t>(l, 0x1c) = v; }
// @src 140284a00  _DEF_LOOP byte +0x1c
std::uint8_t loop_get_1c(const DEF_LOOP* l) { return field<std::uint8_t>(l, 0x1c); }
// @src 140284b60  _DEF_LOOP number +0x18
void loop_set_number(DEF_LOOP* l, const std::int32_t* n) { field<std::int32_t>(l, 0x18) = *n; }
// @src 140284a70  _DEF_VERTEX byte +0x20
void vertex_set_20(DEF_VERTEX* v, std::uint8_t b) { field<std::uint8_t>(v, 0x20) = b; }
// @src 140284a90  _DEF_VERTEX byte +0x20
std::uint8_t vertex_get_20(const DEF_VERTEX* v) { return field<std::uint8_t>(v, 0x20); }
// @src 140284b80  _DEF_VERTEX number +0x24
void vertex_set_number(DEF_VERTEX* v, const std::int32_t* n) { field<std::int32_t>(v, 0x24) = *n; }
// @src 140284b40  _DEF_FACE number +0x44
void face_set_number(DEF_FACE* f, const std::int32_t* n) { field<std::int32_t>(f, 0x44) = *n; }

// ---- OBJ_MESH2D --------------------------------------------------------------------------------

// @src 140284aa0  140182840(mesh + 8): int at +0x12c of the member at +8
std::int32_t mesh2d_count_08(const OBJ_MESH2D* m) { return field<std::int32_t>(m->s008, 300); }

// @src 140284ac0  1401854f0(mesh + 0x148): int at +0xe8 of the member at +0x148
std::int32_t mesh2d_count_148(const OBJ_MESH2D* m) { return field<std::int32_t>(m->s148, 0xe8); }

} // namespace def::model
