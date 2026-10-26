// defdbio/resultset/globals_p1_13.hpp: static storage of the two result-set reader routines
// (Fortran locals the compiler placed in .data, docs/port-conventions.md section 5).
#pragma once

#include <cstdint>

namespace def::dbio::resultset {

// FUN_1403d84b0 (header read)
// @data 140707dd0  unit number of the DB (3)
inline std::int32_t dat_140707dd0 = 0;
// @data 140707e60  title length handed to FUN_140443fc4 (0x50 in, last non-blank + 1 out)
inline std::int32_t dat_140707e60 = 0;
// @data 140707e64  DO variable of the title copy (11 after the loop)
inline std::int32_t dat_140707e64 = 0;

// FUN_1403d96fc (result-set information)
// @data 140707e68  unit number of the DB (3)
inline std::int32_t dat_140707e68 = 0;
// @data 140707ed0  NUMRST: entries whose word 4 (MAR size) is nonzero
inline std::int32_t g_numrst = 0;
// @data 140707ed4  NUMRMS: entries whose word 2 (step number) is negative
inline std::int32_t g_numrms = 0;
// @data 140707ed8  DO variable of the count loop (NRSETS + 1 after it)
inline std::int32_t dat_140707ed8 = 0;
// @data 140707edc  word offset of the last entry counted
inline std::int32_t dat_140707edc = 0;

// Zero all of the above (a fresh process).
void reset_globals();

} // namespace def::dbio::resultset
