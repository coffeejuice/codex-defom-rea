// defmodel/geo_kw/curve_model.hpp: model operations behind the curve keywords DIEGEO (curve and
// type-3 forms), TPLOGY and CSPMTX (packet kw-g2geo, family "curve").
//
//   * geometry 0 of LLOBJ_DATA+0x3998 (a GEO2D_AND_MESH2D) holds the curves as edges, the
//     topology (vertices, loops, faces) and the 4x4 placement matrix at +0x38;
//   * the writers drop degenerate curves first (140304e40);
//   * the TPLOGY writer builds the topology from the curves when none was read (140281bf0:
//     vertices at curve ends, loops by walking edges around vertices, faces from the loops).
//
// The topology classes (_DEF_VERTEX/_DEF_EDGE/_DEF_LOOP/_DEF_FACE) belong to other packets, so
// their methods in the binary's ranges that only this family uses are free functions here, named
// after their role and tagged with their address.
#pragma once

#include <cstdint>

namespace def::model {
class LLOBJ_DATA;
class GEO2D_AND_MESH2D;
class OBJ_GEO2D;
class DEF_EDGE;
class DEF_LOOP;
class DEF_FACE;
class DEF_VERTEX;
class IEdge;
class ILoop;
class IVertex;
namespace linked {
class Matrix4;
}
}  // namespace def::model

namespace def::model::geo_kw {

// ---- object level --------------------------------------------------------------------------------
// @src 1401983e0  number of entries of the geometry list (0 when obj+0x3998 is null)
std::int32_t geometry_count(LLOBJ_DATA& obj);
// @src 140304e40  degenerate curves of geometry 0 (no points; type 1 with < 2 points; type 2 with
//                 one segment whose ends coincide within 1e-12). 1 when there are none (or no
//                 curves); else 0, after deleting them when `remove`.
std::int32_t remove_degenerate_curves(LLOBJ_DATA& obj, bool remove);
// @src 140304e00  delete edge *i of geometry 0 (140280140)
void remove_curve(LLOBJ_DATA& obj, const std::int32_t* i);

// ---- GEO2D_AND_MESH2D ------------------------------------------------------------------------------
// @src 14026a3e0  m = the matrix at +0x38
void get_matrix(GEO2D_AND_MESH2D* g, linked::Matrix4* m);
// @src 14039aa60  the matrix at +0x38 = m
void set_matrix(GEO2D_AND_MESH2D* g, linked::Matrix4* m);
// @src 14026a4a0  address of element (i, j) of a 4x4 matrix (row stride 0x20)
double* matrix_element(linked::Matrix4* m, const std::int32_t* i, const std::int32_t* j);
// @src 140280140  delete edge i (its curve through the curve's deleting dtor, the edge with free),
//                 renumber the edges, +0xb8 = 1. 0 no edges, -1 i < 0, -2 i >= count, -3 cast
//                 failed, 1 done.
std::int32_t remove_edge(GEO2D_AND_MESH2D* g, std::int32_t i);
// @src 140284690  clear faces and loops, empty every edge's loop list, clear vertices (the
//                 entities themselves are not freed)
void reset_topology(GEO2D_AND_MESH2D* g);
// @src 140284730  some edge's curve flag (14041ccd0) is set, else +0xb8
bool topology_dirty(GEO2D_AND_MESH2D* g);
// @src 140281bf0  when topology_dirty: build vertices (140281c50), loops and faces (140281ec0),
//                 then +0xb8 = 0
void build_topology(GEO2D_AND_MESH2D* g, const bool* faces_for_many_loops);
// @src 140281c50  -> 140281c70
// @src 140281c70  clear faces/loops/vertices; one vertex per distinct curve end (1e-6)
void build_vertices(GEO2D_AND_MESH2D* g);
// @src 140281ec0  loops (1402823f0), drop duplicate loops (140282f80), drop an enclosing loop
//                 (1402830f0), faces (1402832f0 for one loop, 140283390 when *flag)
void build_loops_and_faces(GEO2D_AND_MESH2D* g, const bool* faces_for_many_loops);
// @src 1402823f0  walk the edges into loops
void build_loops(GEO2D_AND_MESH2D* g);
// @src 140282350  first edge not visited in both directions (+0x34 / +0x35), or null
DEF_EDGE* next_unvisited_edge(GEO2D_AND_MESH2D* g);
// @src 140282f80  delete loops that use the same edges as an earlier one
void remove_duplicate_loops(GEO2D_AND_MESH2D* g);
// @src 140282ff0  index j of a loop with the same edges as some loop i != j, or -1
std::int32_t find_duplicate_loop(GEO2D_AND_MESH2D* g);
// @src 1402830f0  delete the first loop (with a vertex of >= 3 edges) that every other loop is
//                 inside of
void remove_enclosing_loop(GEO2D_AND_MESH2D* g);
// @src 1402832f0  one face holding loop 0 (when there is exactly one loop)
void build_single_face(GEO2D_AND_MESH2D* g);
// @src 140283390  faces: one per counter-clockwise curve, with the clockwise ones that follow it
void build_faces(GEO2D_AND_MESH2D* g, bool set_attr);
// @src 1402835f0  number of i < loop count whose curve of edge i (14027ff20) is counter-clockwise
std::int32_t count_ccw_curves(GEO2D_AND_MESH2D* g);
// @src 140283680  collect into +0xc0 loop *start and the clockwise loops after it; returns the
//                 number collected
std::int32_t collect_face_loops(GEO2D_AND_MESH2D* g, const std::int32_t* face, const std::int32_t* start);
// @src 140283840  the first vertex within 1e-6 of p, or null
DEF_VERTEX* find_vertex(GEO2D_AND_MESH2D* g, const double* p);
// @src 140283cd0  delete loop i (unlinked from its edges first), renumber the loops.
//                 0 no loops, -1 i < 0, -2 i >= count, 1 done
std::int32_t remove_loop(GEO2D_AND_MESH2D* g, std::int32_t i);

// ---- curves ----------------------------------------------------------------------------------------
// @src 140278530  first and last point of the curve (type 2 converted to points on a copy);
//                 0, 0 when it has no points
void curve_endpoints(OBJ_GEO2D* c, double* p0, double* p1);
// @src 140278670  first and last point within 1e-6
bool curve_is_closed(OBJ_GEO2D* c);
// @src 140308d60  |a - b| < tol in x and in y
bool points_near(const double* a, const double* b, double tol);
// @src 1403090e0  2D dot product
double dot2(const double* a, const double* b);
// @src 1403091c0  normalise a 2-vector in place (unless its length is 0); returns the length
double normalize2(double* v);
// @src 140309230  length of a 2-vector (0 when the squared length is exactly 0)
double length2(const double* v);
// @src 14023b4c0  out = a x b (3-vectors)
void cross3(const double* a, const double* b, double* out);

// ---- edges, vertices, loops, faces ------------------------------------------------------------------
// @src 14041c4c0  delete the edge's curve (deleting dtor), geo = 0
void edge_delete_curve(DEF_EDGE* e);
// @src 14041c530  start vertex == end vertex
bool edge_is_closed(DEF_EDGE* e);
// @src 14041c760  remove loop *i from the edge's loop list
void edge_remove_loop_at(DEF_EDGE* e, const std::int32_t* i);
// @src 14041c7d0  remove loop l from the edge's loop list (first match)
void edge_remove_loop(DEF_EDGE* e, DEF_LOOP* l);
// @src 14041c8c0  end points of the edge's curve (p0 = p1 when it is closed)
void edge_endpoints(DEF_EDGE* e, double* p0, double* p1);
// @src 14041c9d0  unit tangent of the edge's curve leaving vertex v; false (t untouched) when
//                 the edge is closed, has no curve, or v is at neither end
bool edge_tangent_at(DEF_EDGE* e, IVertex* v, double* t);
// @src 14041cfc0  x = *x, y = *y
void vertex_set_xy(DEF_VERTEX* v, const double* x, const double* y);
// @src 14041d1c0  fewer than 3 edges meet at the vertex
bool vertex_is_simple(DEF_VERTEX* v);
// @src 14041d200  the other edge at v with the smallest angle from e's tangent, angles in
//                 [0, 2pi) (negative ones + 2pi); null when fewer than 2 edges
IEdge* vertex_next_edge_ccw(DEF_VERTEX* v, IEdge* e);
// @src 14041d3e0  the same with angle a mapped to 2pi - a (a >= 0) or -a
IEdge* vertex_next_edge_cw(DEF_VERTEX* v, IEdge* e);
// @src 14041d630  |v - p| < tol in x and in y
bool vertex_near(DEF_VERTEX* v, const double* p, double tol);
// @src 14041d6b0  signed angle from a to b (both normalised in place); acos of the dot product,
//                 negated when the cross product's z < 0
double signed_angle(DEF_VERTEX* v, double* a, double* b);
// @src 140413250  both loops have the same number of edges and every edge of a is in b
bool loop_same_edges(DEF_LOOP* a, DEF_LOOP* b);
// @src 140413320  e is one of the loop's edges
bool loop_has_edge(DEF_LOOP* l, IEdge* e);
// @src 140413580  every end vertex of every edge of the loop has fewer than 3 edges
bool loop_vertices_simple(DEF_LOOP* l);
// @src 1402f7eb0  insert loop l before position *i of the face's loop list
void face_insert_loop(DEF_FACE* f, const std::int32_t* i, ILoop* l);
// @src 1402f7f20  remove position *i of the face's loop list
void face_remove_loop_at(DEF_FACE* f, const std::int32_t* i);
// @src 1402f7fd0  move the first loop that is inside no other loop to the front. 1 done (or
//                 fewer than 2 loops, or already first), 0 none found
std::int32_t face_outer_loop_first(DEF_FACE* f);

}  // namespace def::model::geo_kw
