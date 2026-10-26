// defmodel/step/material_select.hpp: the material the TEXT_PRE material menu (1400157c0) edits
// (packet p10-01). DB_STEP_DATA+0x1b48 holds the number of the current material.
#pragma once

#include <cstdint>

namespace def::model {

class DB_STEP_DATA;
class Material;

// 14019cfb0: material `number`; if there is none, add it (number set, +0x1320 = list count) and
// remap the per-material state (1401d0fc0(step, 1) between the two index rebuilds 14029ca10)
Material* step_material_get_or_add(DB_STEP_DATA& s, std::int32_t number);
// 1401ca9a0: current material number = number (+0x1b48)
void step_set_current_material(DB_STEP_DATA& s, std::int32_t number);
// 1401ca9c0: current material number (+0x1b48)
std::int32_t step_current_material(const DB_STEP_DATA& s);

} // namespace def::model
