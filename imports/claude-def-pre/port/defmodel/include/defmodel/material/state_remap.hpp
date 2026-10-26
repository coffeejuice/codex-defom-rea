// defmodel/material/state_remap.hpp: FUN_1401d0fc0, called by the group 4 KEY reader when a new
// material is created (remap = true, between the two MaterialList index rebuilds) and after
// every GRNDAT keyword (remap = false). Packet kw-g4b.
//
// It saves the 0x4f-int state-variable size table at DB_STEP_DATA+0x15f8, re-derives it for the
// current material set (FUN_1401d0320), lets every object re-derive its state-variable arrays
// from the old and new tables, and with remap = true copies the per-material columns of the
// object arrays at +0xa58 / +0xa60 from their old material index to the new one, using the
// MaterialList number / index arrays built just before the new material was added.
#pragma once

#include <defmodel/layout/db_step_data.hpp>

namespace def::model {

void step_remap_material_state(DB_STEP_DATA& s, bool remap);

// MaterialList accessors that 1401d0fc0 reads the pre-insert arrays through.
std::int32_t* material_list_numbers(const MaterialList* l);   // 1401d8fa0: +0x18
std::int32_t* material_list_index_of(const MaterialList* l);  // 1401d8fb0: +0x20

// 1401d0320: fills the 0x4f-int state-variable size table (+0x15f8) from the dimension, the
// material count (+0x1320), the phase-pair count (+0x13f4) and simulation-control settings.
void step_material_state_sizes(DB_STEP_DATA& s);

} // namespace def::model
