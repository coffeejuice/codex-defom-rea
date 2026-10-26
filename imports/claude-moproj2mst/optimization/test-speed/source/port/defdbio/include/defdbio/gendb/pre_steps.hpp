// defdbio/gendb/pre_steps.hpp: the model pre-steps of the GENDB keyword command (1401d7f40) and
// the functions under it that had no port yet: the mixture-material cleanup (1401d6250), the
// per-object surface normal lists (1401d63c0 -> 1402059a0) and the small helpers those call
// (the {id, point, direction} list of defmodel/linked/axis_point_list.hpp, 3-vector kernels and
// three generic getters).
//
// Sources: src/gendb/pre_steps/pre_steps.cpp, src/gendb/pre_steps/surface_normals.cpp.
#pragma once

#include <cstdint>

namespace def::model {
class DB_STEP_DATA;
class LLOBJ_DATA;
namespace linked {
class Cls_140257340;
class Cls_140257340List;
} // namespace linked
} // namespace def::model

namespace def::dbio::gendb {

// ---- driver ------------------------------------------------------------------------------------

// 1401d7f40: sim_ctrl DTTYPE mode (1401ee510), mixture-material cleanup (1401d6250), surface
// normals of every object (1401d63c0 with flag 0); then, when the byte +0x2f88 is set, NSTART
// (+0x8b0) is made negative (-(n + 1) when n >= 0) and copied to +0x744.
void gendb_command_pre_steps(model::DB_STEP_DATA* m);

// 1401d6250: for every mixture material (+0x1cc8 == 1) of the first +0x1320 materials, each
// component material that exists and is not of kind 2 loses its component list (freed when
// its count > 0) and becomes kind 2.
void mixture_material_cleanup(model::DB_STEP_DATA* m);

// 1401d63c0: object_surface_normals(obj, flag) for each of the first +0x1378 objects whose type
// (+0xc) is not 7.
void objects_surface_normals(model::DB_STEP_DATA* m, char flag);

// 1402059a0: for an object with a surface (+0x3990): rebuild the normal list of surface 0 (+0x28)
// (a copy of the object's list at +0x2e80 first when flag != 0) and give each triangle
// (4th node < 1) with a normal of length >= 1e-6 the id + 600 of a list entry whose direction is
// within 0.01 (squared distance) and whose plane passes within 0.001 * char_length of the
// triangle's first node, or of a new entry (first free id, that node, the unit normal).
void object_surface_normals(model::LLOBJ_DATA* obj, char flag);

// ---- {id, point, direction} list helpers --------------------------------------------------------

// 1402580f0: first node whose id == id, or null
model::linked::Cls_140257340* point_list_find(model::linked::Cls_140257340List* list, std::int32_t id);
// 140258260: the smallest id >= 1 that no node has (counting up from 1)
std::int32_t point_list_free_id(model::linked::Cls_140257340List* list);
// 140257510: point = *p (3 doubles) unless p is null
void point_node_set_point(model::linked::Cls_140257340* node, const double* p);
// 140257590: direction = *d (3 doubles) unless d is null
void point_node_set_direction(model::linked::Cls_140257340* node, const double* d);

// ---- 3-vector kernels -----------------------------------------------------------------------------

// 14023b850: squared distance |a - b|^2
double vec3_dist2(const double* a, const double* b);
// 14023e050: out = (c - b) x (a - b), divided by its length unless the length is 0
void triangle_normal(const double* a, const double* b, const double* c, double* out);
// 14023c770: |n . (p - q)|
double plane_distance(const double* p, const double* n, const double* q);

// ---- generic getters ------------------------------------------------------------------------------

// 140158e60: self + 0x28
void* addr_at_28(void* self);
// 140158e80: *(void**)(self + 0x20)
void* ptr_at_20(const void* self);

} // namespace def::dbio::gendb
