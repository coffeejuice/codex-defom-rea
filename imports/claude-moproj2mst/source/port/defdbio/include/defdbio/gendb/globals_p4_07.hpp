// defdbio/gendb/globals_p4_07.hpp: DB-module globals that packet p4-07 needs and p1-12 did not
// declare. NADS1 (0x140b48ec0, 140 words) lies inside p1-12's DbCommon and is reached through
// g_db.words(); see nads1() in step_header.cpp.
#pragma once

#include <cstdint>

namespace def::dbio::gendb {

// @data 140b6ce80  NADS2: start of each A array region, 1-based (280 words; DBMADR fills it on
// read, 1403b3bfd / 1403b3c9b on write; 1403b9ad6 reads words 1..127)
inline std::int32_t g_nads2[0x118] = {};

// @data 140707310  fixed address table of 3D steps with version < 3000 (1403b3c2e, 1403b3c9b):
// 40 words 0x140707310 .. 0x1407073ac, written as overlapping word copies and stores
inline std::int32_t g_old_tables[40] = {};

} // namespace def::dbio::gendb
