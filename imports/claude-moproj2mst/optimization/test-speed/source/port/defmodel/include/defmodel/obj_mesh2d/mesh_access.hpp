// defmodel/obj_mesh2d/mesh_access.hpp: node / element accessors of OBJ_MESH2D (Cls_1403ddf20) that
// the 2D geometry primitives (p9-02) call; ported at the Phase 6-7 gate (callees of the P6-7
// node/element set code). Node set at mesh+8 (Cls_14017ef20), element set at mesh+0x148
// (Cls_140182880), dimension table at mesh+0x15c0.
#pragma once

#include <cstdint>

namespace def::model {

class OBJ_MESH2D;

// 1403de680: node count := *n (140182860), then allocate the node arrays (14017ffe0, key 0)
void fun_1403de680(OBJ_MESH2D* mesh, const std::int32_t* n);
// 1403de6e0: allocate *ne elements of *nper nodes (1401836c0, key -2)
void fun_1403de6e0(OBJ_MESH2D* mesh, const std::int32_t* ne, const std::int32_t* nper);
// 140220440: node coordinates (1401808d0 on mesh+8)
double* fun_140220440(OBJ_MESH2D* mesh);
// 140220460: element connectivity (140183cb0 on mesh+0x148)
std::int32_t* fun_140220460(OBJ_MESH2D* mesh);
// 140220480: material group per element (+0x148 set +0x18)
std::int32_t* fun_140220480(OBJ_MESH2D* mesh);

} // namespace def::model
