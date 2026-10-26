// defmodel/hdrstats/globals_p4_07.hpp: globals of packet p4-07 (step header, object statistics).
#pragma once

#include <cstddef>
#include <cstdint>

namespace def::model::p407 {

// @data 1406e8524  step-header slot table: 122 ints (0x1406e8524 .. 0x1406e8708), the position of
// each item inside the step header or inside one object's block, filled per dimension by
// 1401c7f80 (2D and 3D sets). Read by GENDB's header writer 1401e2180 and by DBREAD 1401cd020.
inline std::int32_t g_hdr_slots[122] = {};

// The slot word at the original address `addr` (0x1406e8524 .. 0x1406e8708), e.g.
// hdr_slot(0x1406e852c) is DAT_1406e852c.
constexpr std::size_t hdr_slot_index(std::uint64_t addr) { return (addr - 0x1406e8524) / 4; }
inline std::int32_t& hdr_slot(std::uint64_t addr) { return g_hdr_slots[hdr_slot_index(addr)]; }

// Labels of the step-header items (type, slot count, 32-character name per row: "Load", "Stroke",
// "Velocity", ...), at 0x1405d4ac0 (2D) and 0x1405d54c0 (3D). Only display code reads them; the
// rows are not recovered, the port keeps which table is selected.
enum class HdrLabelTable : std::int32_t { none = 0, table_1405d4ac0 = 2, table_1405d54c0 = 3 };
// @data 1406e8710  pointer to the label table for the current dimension (1401c7f80)
inline HdrLabelTable g_hdr_label_table = HdrLabelTable::none;

// @data 14070b084  index (1..3) of the largest root, kept between calls of 1403f1ec3 (Fortran SAVE)
inline std::int32_t g_root_imax = 0;
// @data 14070b088  index (1..3) of the smallest root, kept between calls of 1403f1ec3
inline std::int32_t g_root_imin = 0;

// Resets the globals above (tests).
void reset_globals_p4_07();

} // namespace def::model::p407
