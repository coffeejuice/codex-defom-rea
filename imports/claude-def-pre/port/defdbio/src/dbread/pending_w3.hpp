// dbread/pending_w3.hpp: functions the W3 part of packet p-dbread calls that belong to other
// helpers (W1, W2) or to no list yet. Declared with the binary's argument lists, NOT defined here;
// the lead forwards each to its port at integration. Argument roles are from the call sites in
// FUN_1401cca80, FUN_1401cd1f0, FUN_1401cf220 and FUN_1401dfb80.
#pragma once

#include <cstdint>

namespace def::model {
class DB_STEP_DATA;
class Cls_1401dbfe0;
} // namespace def::model

namespace def::dbio::dbread {

// ---- W2 -----------------------------------------------------------------------------------------
// 1401cc1a0: entry m->u740 of db's directory: *hsize = +8, *mar_size = +0xc, *a_size = +0x10,
// *version = +0x20; m->step_version re-synced (1401cc160) when it differs. false when db == 0.
// (idx is passed but the body indexes with m->u740.)
// 1401e5f00: data file name of directory entry idx (multi-file DB) into name (strcpy), and the
// index of the first entry of that file into *first (0 for a single-file DB).
// 1401e5de0: number of the data file that holds directory entry idx (0 for a single-file DB).
// 1401e5eb0: first directory entry of data file `file` (0 for a single-file DB).
// 1401e5d60: name of data file `file` into name (strcpy of db+0x650 or db+0x330[file]).
// 1403dc957: Fortran, read one A region (op 30 + |*sel|, or 1403bef07 op 3 for version < 2301)
// of the step at record *rec of file `name` into a; same argument list as 1403dc13f.
// 1403bef07: Fortran, the DB operations of 3D files before version 2301 (same argument list as
// FUN_1403b5a04 / dbcore::db_operation).

// ---- W1 -----------------------------------------------------------------------------------------
// 140324020: platform text conversion of `words` 8-byte words of s (FUN_140323e50 byte swap of
// 2*words 4-byte halves, then FUN_140323ec0 unless `no_second`) when 2*100 + platform is one of
// 1..3, 100, 200, 300 (FUN_140324000 returns 2).
void fun_140324020(char* s, std::int32_t platform, std::int32_t words, std::int32_t no_second);

// ---- not in any list (message helper, ledger "not-needed") ---------------------------------------
// 14039d3a0: message printf through 14039d570(0, fmt, va): fmt "%s Error when re-allocating %s
// memory of size %d\n" with "ERROR    ", "INT" / "DOUBLE" and the size.
void fun_14039d3a0(const char* fmt, const char* a, const char* b, std::int32_t n);

} // namespace def::dbio::dbread
