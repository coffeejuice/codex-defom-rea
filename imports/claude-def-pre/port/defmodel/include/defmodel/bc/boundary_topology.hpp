// defmodel/bc/boundary_topology.hpp: the object's boundary topology as ELMCON stores it, and the
// slot functions over it: boundary edge (2D) / face (3D) slot count, lookup by nodes, nodes of a
// slot. Plain free functions over LLOBJ_DATA (used by kw-g2bc's EC* keywords and kw-g2sv).
// Spec: docs/re/keywords/ECCDEF.md ("How an edge/face is located").
//
// Object fields used (offsets as in the binary; LLOBJ_DATA keeps them opaque):
//   +0x87c mesh present, +0x880 node count, +0x884 element count, +0x88c boundary segment total;
//   +0xbd8 number of boundary loops (2D) / surfaces (3D), at most 100;
//   +0xbe0 100 x Cls_14017e740 (0x30 bytes): +0 node count, +8 node list (2D: closed, last =
//          first), +0x10 face count, +0x18 face nodes (4 per face, a triangle is "a b c a"),
//          +0x20 face elements, +0x28 model.
//
// The boundary itself is built by ELMCON (FUN_1401fc290 with the Fortran finders FUN_14032d65e
// (2D) / FUN_140354ed0 (3D)); that construction belongs to the mesh packet and is not here.
#pragma once

#include <cstdint>

namespace def::model {

class LLOBJ_DATA;
class Cls_14017e740;

namespace bc {

// 140256dc0 / 140257150: calloc(n, 4) / calloc(n, 8); null when n == 0; the message 14039d3a0
// "%s Error when allocating %s memory of size %d" when calloc fails.
std::int32_t* calloc_ints(std::int32_t n);
double* calloc_doubles(std::int32_t n);
// 14039d3a0: message printf (fmt, two strings, one int) through 14039d570 to stdout; no exit.
void alloc_message(const char* fmt, const char* a, const char* b, std::int32_t n);

// 14017ebc0: segments of one boundary record: 2D node count - 1, 3D face count, else -1
// (dimension of the record's model +0x28).
std::int32_t boundary_segment_count(const Cls_14017e740& b);
// 14017e8a0: (re)allocate the node list (node count ints) and, when the face count > 0, the face
// node (4 per face) and face element arrays. Zero filled.
void alloc_boundary_arrays(Cls_14017e740& b);
// 140163300: obj+0xbd8 = min(n, 100).
void set_boundary_count(LLOBJ_DATA& obj, std::int32_t n);
// 140200a70: face count of boundary `bdry` (< obj+0xbd8), then alloc_boundary_arrays; else an
// error message on stdout.
void set_boundary_face_count(LLOBJ_DATA& obj, std::int32_t nfaces, std::int32_t bdry);

// 140165d30: obj+0x88c = sum of boundary_segment_count over the obj+0xbd8 records.
void recount_boundary_segments(LLOBJ_DATA& obj);
// 140165170: number of edge/face slots NS (0 without mesh, elements or boundary).
std::int32_t edge_face_slot_count(LLOBJ_DATA& obj);

// 140200c90: slot of the 2D edge (n1, n2), either order; -1 when not found or no mesh.
std::int32_t edge_slot(const LLOBJ_DATA& obj, std::int32_t n1, std::int32_t n2);
// 140200ff0: index of the 3D face of surface 0 whose nodes are all in {n1..n4} (quad mode) or
// whose first 3 are in {n1, n2, n3} (triangle mode, chosen from face 0); -1 otherwise.
std::int32_t face_slot(const LLOBJ_DATA& obj, std::int32_t n1, std::int32_t n2, std::int32_t n3,
                       std::int32_t n4);
// 140200e20: the two nodes of 2D slot s (0, 0 when out of range or no mesh).
void edge_slot_nodes(const LLOBJ_DATA& obj, std::int32_t s, std::int32_t* p, std::int32_t* q);
// 140200ec0: the same on the raw boundary table (count at *nloops, records at `loops`).
void loop_slot_nodes(std::int32_t s, const std::int32_t* nloops, const Cls_14017e740* loops,
                     std::int32_t* p, std::int32_t* q);
// 140201210: the 4 stored nodes of 3D face i of surface 0 (all 0 without mesh).
void face_nodes(const LLOBJ_DATA& obj, std::int32_t i, std::int32_t* a, std::int32_t* b,
                std::int32_t* c, std::int32_t* d);

} // namespace bc
} // namespace def::model
