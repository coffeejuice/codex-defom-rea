// OBJ_MESH2D node / element accessors (see defmodel/obj_mesh2d/mesh_access.hpp).
#include <defmodel/obj_mesh2d/mesh_access.hpp>

#include <defmodel/bc/node_arrays.hpp>
#include <defmodel/layout/obj_mesh2d.hpp>
#include <defmodel/node_access.hpp>

namespace def::model {

// @src 1403de680
// @src 140182860  (inlined) node set +0x12c = n
void fun_1403de680(OBJ_MESH2D* mesh, const std::int32_t* n)
{
    *reinterpret_cast<std::int32_t*>(mesh->s008 + 300) = *n;
    bc::node_set_alloc(mesh->s008, *n, mesh->flags, 0);  // 14017ffe0
}

// @src 1403de6e0
void fun_1403de6e0(OBJ_MESH2D* mesh, const std::int32_t* ne, const std::int32_t* nper)
{
    bc::element_set_alloc(mesh->s148, *ne, mesh->flags, *nper, -2);  // 1401836c0
}

// @src 140220440
double* fun_140220440(OBJ_MESH2D* mesh) { return fun_1401808d0(reinterpret_cast<Cls_14017ef20*>(mesh->s008)); }

// @src 140220460
// 140183cb0 (inlined; its sync 1401d5820 returns at once because 1401d2bb0 is always 1): set+8
std::int32_t* fun_140220460(OBJ_MESH2D* mesh) { return *reinterpret_cast<std::int32_t**>(mesh->s148 + 8); }

// @src 140220480
// 140183d00 on +0x148 (its sync 1401d5820 returns at once, as for 140220460): set+0x18, the
// material group of each element
std::int32_t* fun_140220480(OBJ_MESH2D* mesh) { return *reinterpret_cast<std::int32_t**>(mesh->s148 + 0x18); }

} // namespace def::model
