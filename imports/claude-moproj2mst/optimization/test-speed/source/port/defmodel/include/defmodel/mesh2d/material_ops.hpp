// Material / MaterialList operations ported by packet p1-08 (address range 0x14028a320-0x14029e210).
// The classes are the skeleton's layout headers (layout/material.hpp, material_list.hpp and the
// sub-object headers); their destructors and the MaterialList ctor/dtor are defined in
// src/mesh2d/material.cpp. The rest are free functions because the layout headers are not
// edited outside the skeleton thread.
#pragma once

#include <cstdint>

#include <defmodel/layout/cls_14028b850.hpp>
#include <defmodel/layout/cls_14028c500.hpp>
#include <defmodel/layout/material.hpp>
#include <defmodel/layout/material_list.hpp>

namespace def::model {

// ---- MaterialList (DB_STEP_DATA+0x1328) -----------------------------------------------------

// 14029b500: head = tail = 0, count = 0, numbers = index_of = 0 (no free).
void material_list_reset(MaterialList* list);

// 14029c310: deletes every node (Material dtor + free), then head = tail = 0, count = 0.
void material_list_clear(MaterialList* list);

// 14029c2c0: first node whose number == number, or nullptr.
Material* material_list_find(const MaterialList* list, std::int32_t number);

// 14029c260: 1-based node i; i <= 1 gives the head, i > count gives nullptr.
Material* material_list_nth(const MaterialList* list, std::int32_t i);

// 14029cd50: count at +0x10 (the same folded function reads the GRNDAT record lists).
std::int32_t material_list_count(const MaterialList* list);

// 14029bcd0: GRNDAT type-2 record count of material *number; for a mixture material
// (MSTMTR) the sum over its component materials. *n_out gets 1, or the component count.
// Returns 0 (n_out untouched) when the material does not exist.
std::int32_t material_list_grain_record_count(const MaterialList* list, const std::int32_t* number,
                                              std::int32_t* n_out);

// ---- Material ------------------------------------------------------------------------------

// 14028d0a0: mstmtr_flag == 1.
bool material_is_mixture(const Material* m);

// 14029b2c0: GRNDAT type 2 ? record count of the type-2 part : 0.
std::int32_t material_grain_record_count(const Material* m);

// 14029b300: txture.a == 0 ? 0 : texture_count_num_odfs(&txture).
std::int32_t material_num_odfs(const Material* m);

// 14028c5a0: TEXTURE_DATA::countNumODFs (Cls_14028c500 is TEXTURE_DATA). Prints
// "Error in TEXTURE_DATA::countNumODFs()\n" to stdout for an unknown a.
std::int32_t texture_count_num_odfs(const Cls_14028c500* t);

// ---- GRNDAT type-2 part (Material+0x14d0) ----------------------------------------------------

// 14028b780: 1 (the main record) + the counts of the five record lists.
std::int32_t grndat_record_count(const Cls_14028b640* g);

// 14029cd50 on a record list.
std::int32_t grndat_record_list_count(const Cls_14028b640::RecordList* l);

// 14028af40 -> 14028b240: deletes every record (14029cd60: dtor + free), then resets.
void grndat_record_list_clear(Cls_14028b640::RecordList* l);

} // namespace def::model
