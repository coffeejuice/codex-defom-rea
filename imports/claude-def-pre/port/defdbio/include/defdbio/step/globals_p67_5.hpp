// defdbio/step/globals_p67_5.hpp: globals that the step-header cache code (packet p67-5, TCYCLE)
// needs and no other packet has declared (docs/port-conventions.md section 5).
#pragma once

#include <cstdint>

#include <defmodel/hdrstats/globals_p4_07.hpp>

namespace def::dbio::step {

// 1406e8524 (owner: def::model::p407::g_hdr_slots, p4-07)  step-header layout table: 122 ints
// at 0x1406e8524..0x1406e870b, filled by FUN_1401c7f80 (p407::load_hdr_slots); readers index it
// by address: hdr_index(0x1406e8528) is DAT_1406e8528.
inline constexpr std::int32_t kHdrIndexWords = 122;
inline std::int32_t (&dat_1406e8524)[kHdrIndexWords] = model::p407::g_hdr_slots;

// 1406e8710 (owner: def::model::p407::g_hdr_label_table): which min/max pair table belongs to
// the current dimension (DAT_1405d4ac0 for 2D, DAT_1405d54c0 for 3D); header_minmax_table()
// in db_header_cache.cpp returns its rows.

// Word of the layout table at `addr` (0x1406e8524 <= addr <= 0x1406e8708, 4-aligned).
inline std::int32_t& hdr_index(std::uint64_t addr)
{
    return dat_1406e8524[(addr - 0x1406e8524ull) / 4];
}

// FUN_1403daf3e (step header read) statics, Fortran locals placed in .data. Only the step number
// is an input of FUN_1403b5a04 (used when the slot is < 1, never the case for the callers here);
// the MAR / A outputs are not touched by op 2.
// @data 140707f84  step number argument of op 2 (never written)
inline std::int32_t dat_140707f84 = 0;
// @data 140707f90  MAR argument of op 2 (unused by op 2)
inline std::int32_t dat_140707f90 = 0;
// @data 140707fc0  A argument of op 2 (unused by op 2)
inline double dat_140707fc0 = 0.0;

// FUN_1403adc74 (open the DB on a unit) passes three statics to the op-0 call; op 0 does not
// write them in the port (dbcore::op_open ignores those arguments).
// @data 140706e20  h argument of op 0
inline double dat_140706e20 = 0.0;
// @data 140706e30  mar argument of op 0
inline std::int32_t dat_140706e30 = 0;
// @data 140706e60  a argument of op 0
inline double dat_140706e60 = 0.0;

// Zero all of the above (a fresh process).
void reset_globals_p67_5();

} // namespace def::dbio::step
