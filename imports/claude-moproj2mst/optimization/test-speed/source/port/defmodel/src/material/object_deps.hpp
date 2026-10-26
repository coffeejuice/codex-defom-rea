// material/object_deps.hpp: what FUN_1401d0fc0 / FUN_1401d0320 call in other packets, declared
// with the addresses they stand for until the owners' headers are merged here (tier-1 tests
// link interim definitions), plus this packet's small object-list helpers.
#pragma once

#include <cstdint>

#include <defmodel/layout/cls_14017a6c0.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/layout/material_list.hpp>
#include <defmodel/llobj/object_list.hpp>  // object_list_find 14017aaf0
#include <defmodel/step/db_step.hpp> // p1-03: step_has_array 1401d64f0, objects_set_owner 1401d8340

namespace def::model {

// ---- kw-g2obj (defmodel/llobj_ctor/llobj_copy.hpp, llobj_ctor.cpp) ----
// 1401613a0: copies an object into `dst` (flags as FUN_1401d0fc0 passes them).
void llobj_copy_from(LLOBJ_DATA* dst, const LLOBJ_DATA* src, const std::int32_t* sizes,
                     const bool* p4, const bool* p5, const bool* p6, const bool* p7,
                     const bool* p8, const bool* p9);
// 140199650: re-derives the object's state-variable arrays (+0x8a0, +0x9e0) for new sizes.
void llobj_remap_state(LLOBJ_DATA* o, const std::int32_t* sizes, const std::int32_t* old_sizes);

// ---- this packet (src/material/state_remap.cpp) ----
std::int32_t material_list_position(const MaterialList* list, std::int32_t number); // 14029c7f0
bool llobj_not_kind7(const LLOBJ_DATA* o);                                          // 140160f10

} // namespace def::model
