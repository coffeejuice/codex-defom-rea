// defmodel/obj_mesh2d/topology.hpp: the boundary topology of a 2D quad mesh (OBJ_MESH2D code
// 0x1403dedd0..0x1403e1860) and its conversion to GEO2D_AND_MESH2D faces / loops / edges /
// vertices. Ported for p67-1 (2D branch of 1401fc730, 1401fd240); spec: the comments below and
// docs/re/commands/OBJPOS.md "2D temporary geometry".
//
// 1403dedd0 builds, in the arrays of rc_array.hpp:
//   1. sides (1403deee0 / 1403df640): the four sides of each element (nodes k, k+1 mod 4; shared
//      sides once, with the elements that use them);
//   2. regions (1403deee0): a flood fill over sides from the first element without a region,
//      joining neighbours of the same material group; a side is kind 1 (one element), 0 (both
//      elements in one region) or 2 (two regions);
//   3. curves (1403df970): chains of sides of kind != 0 that are "alike" (1403e0cd0: same kind
//      and the same regions on both), walked node to node from the first unassigned side; the
//      end nodes become topology vertices (1403dff90);
//   4. loops (1403e0070): for each (curve, region of the curve) not yet used, a loop that
//      follows curves of that region through shared vertices; regions whose every curve lies on
//      the loop also get the loop.
// 1403e0f20 then rebuilds the topology of a GEO2D_AND_MESH2D from these arrays (one face per
// region, one loop per loop, one edge with a point curve per curve, one vertex per vertex), and
// 1403dee60 frees the arrays again (the vertex array +0x15a0 is cleared by neither).
#pragma once

#include <cstdint>

#include <defmodel/obj_mesh2d/rc_array.hpp>

namespace def::model {

class OBJ_MESH2D;
class GEO2D_AND_MESH2D;

namespace mesh2d {
// the local array of 1403e0070: per curve, one "used" flag per entry of its region list
using CurveFlags = RcArray<CowVector<std::uint8_t>>;
} // namespace mesh2d

// 1403dedd0: clear the arrays (+0x1588, +0x1590, +0x1598, +0x15a8, +0x15b0, +0x15b8), then
// regions (1403deee0), curves and loops (1403df940)
void fun_1403dedd0(OBJ_MESH2D* m);
// 1403dee60: clear the same six arrays
void fun_1403dee60(OBJ_MESH2D* m);
// 1403deee0: nodes, element records, sides and regions
void fun_1403deee0(OBJ_MESH2D* m);
// 1403df640: index of side (*a, *b) (nodes 0-based) with element *elem added, created when the
// two nodes share no side yet
std::int32_t fun_1403df640(OBJ_MESH2D* m, const std::int32_t* a, const std::int32_t* b, const std::int32_t* elem);
// 1403df8d0: first element without a region, -1 when none
std::int32_t fun_1403df8d0(OBJ_MESH2D* m);
// 1403df940: 1403df970, then 1403e0070
void fun_1403df940(OBJ_MESH2D* m);
// 1403df970: boundary curves
void fun_1403df970(OBJ_MESH2D* m);
// 1403dfef0: first side of kind != 0 without a curve, -1 when none
std::int32_t fun_1403dfef0(OBJ_MESH2D* m);
// 1403dff90: topology vertex of node *node, created on first use
std::int32_t fun_1403dff90(OBJ_MESH2D* m, const std::int32_t* node);
// 1403e0070: loops
void fun_1403e0070(OBJ_MESH2D* m);
// 1403e0bd0: first (curve, entry) whose flag is 0; both -1 when every flag is set
void fun_1403e0bd0(OBJ_MESH2D* m, mesh2d::CurveFlags*& flags, std::int32_t* curve, std::int32_t* slot);
// 1403e0cd0: sides *a and *b are of one kind and touch the same regions
bool fun_1403e0cd0(OBJ_MESH2D* m, const std::int32_t* a, const std::int32_t* b);
// 1403e0f20: rebuild g's topology from the arrays
void fun_1403e0f20(OBJ_MESH2D* m, GEO2D_AND_MESH2D* g);

// 140220430: the mesh of a 2D geometry (+0x28)
OBJ_MESH2D* fun_140220430(GEO2D_AND_MESH2D* g);
// 1402815a0: g has a mesh with nodes, elements, coordinates and connectivity
bool fun_1402815a0(GEO2D_AND_MESH2D* g);

} // namespace def::model
