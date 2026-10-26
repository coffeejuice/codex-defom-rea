// defdbio/db_query.hpp: step queries on the DB info object Cls_1401dbfe0 (directory db+0xad8,
// header cache db+0xae8) and the multi-file DB chain (`<name><N>.DB` parts in front of the DB).
//
// DEF_GUI_MO_64's operation loader uses byte-identical twins of these to decide whether a DB step
// exists for an operation (docs/re/moproj2mst/mdt/L0/spec_B.md section 4.2, spec_C.md 3.4-3.5).
// None of them has a row in the DEF_PRE batch ledger: inside DEF_PRE_64 the chain functions
// 1401e4c10 / 1401e4e80 have no callers at all (only their .pdata entries).
//
// Directory record (DbDirRecord, 0x28 bytes): +0x04 step, +0x14 SimNo, +0x1c dim, +0x24 stored.
// Cache record (0xe0 bytes, defdbio/step/db_header_cache.hpp): +0x00 NOBJ, +0x04 PDIE, +0x08
// TNOW (first), +0x10 TNOW (end), +0x28 stroke, +0x38 (int)H[10] = the operation number.
#pragma once

#include <cstdint>

#include <defmodel/layout/cls_1401dbfe0.hpp>

namespace def::dbio::query {

using def::model::Cls_1401dbfe0;

// ---- small accessors ------------------------------------------------------------------------
// @src 1401e3ee0  header cache built (db+0x1900)
bool header_cache_built(const Cls_1401dbfe0& db);
// @src 1401ddf00  step number of directory entry idx; 0 without a directory or out of range
std::int32_t entry_step(const Cls_1401dbfe0& db, std::int32_t idx);
// @src 1401ddf60  first directory index whose step is `step`; -1 when none or no directory
std::int32_t step_index(const Cls_1401dbfe0& db, std::int32_t step);

// ---- operation queries (header cache, built on demand) ----------------------------------------
// @src 1401df660  first and last stored entries whose cache record has +0x38 == op: from the first
// one dim, NOBJ, PDIE, step, TNOW (+0x08), stroke; from the last one step, TNOW (+0x10), stroke.
// Outputs not reached stay as they were. false when nothing matched, or without directory/cache.
bool op_first_last(const Cls_1401dbfe0& db, std::int32_t op, std::int32_t* dim, std::int32_t* nobj,
                   std::int32_t* pdie, std::int32_t* step_first, std::int32_t* step_last,
                   double* t_first, double* t_last, double* s_first, double* s_last);
// @src 1401de240  number of steps of operation `op`: 0 not found, 1 when first == last, else
// |last| - (first < 1 ? -1 - first : first) + 1 (the binary's asymmetric form)
std::int32_t op_step_count(Cls_1401dbfe0& db, std::int32_t op);
// @src 1401de110  last stored step of operation `op` (0 when not found)
std::int32_t op_last_step(Cls_1401dbfe0& db, std::int32_t op);
// @src 1401ddfe0  first stored step of operation `op` (0 when not found)
std::int32_t op_first_step(Cls_1401dbfe0& db, std::int32_t op);

// ---- simulation queries (directory SimNo; the cache must already exist) -----------------------
// @src 1401de420  first stored step of simulation `sim` (0 when not found or no cache)
std::int32_t sim_first_step(const Cls_1401dbfe0& db, std::int32_t sim);
// @src 1401de530  last stored step of simulation `sim` (0 when not found or no cache)
std::int32_t sim_last_step(const Cls_1401dbfe0& db, std::int32_t sim);

// ---- multi-file DB chain ----------------------------------------------------------------------
// @src 140323ce0  cut a file name at its last '.' (no change without one)
void strip_extension(char* name);
// @src 1401e4d80  load the DB `name` into a new DB info object (leaked, as in the original) and
// test whether its last directory entry has step *step or *step - 1; false when it has none
bool chain_part_matches(const char* name, const std::int32_t* step);
// @src 1401e4c10  whether a previous part exists: |first step| >= 2 and `<base><|s|-1>.DB` (else
// `<base><-|s|>.DB`) exists and its last step joins (1401e4d80); base = db+0x650 without extension
bool has_previous_part(const Cls_1401dbfe0& db);
// @src 1401e51d0  prepend the previous part to the directory and the part tables (db+0x8 count,
// db+0xc first/last index pairs, db+0x330 names). 0 done, 5 no previous part (|first step| < 2,
// no file, or no entries), 2 header error or unknown family, 3 directory read error.
std::int32_t prepend_previous_part(Cls_1401dbfe0& db);
// @src 1401e4e80  set up the part chain: one part for the DB itself, then prepend parts while
// 1401e51d0 returns 0 (at most 99 tries), hand the part table to the DB module (1403b18c3) and
// drop the header cache. Returns the last 1401e51d0 result, with 5 mapped to 0.
std::int32_t setup_part_chain(Cls_1401dbfe0& db);
// @src 1403b18c3  DB module part table: *n parts, names (200 chars each) and 4 words per part
// (first entry + 1, last entry + 1, first step, last step); also sums the entry counts
void set_db_parts(const char* names, const std::int32_t* n, const std::int32_t* table);

// The DB module's part table (DbCommon padding, see db_chain.cpp): part count (DAT_140b49100),
// part k name (1-based, 200 chars at DAT_140b4903c + 200 k), its 4 words (DAT_140b4df14 + 16 k)
// and the summed entry count (DAT_140b4e568).
std::int32_t db_part_count();
const char* db_part_name(std::int32_t k);
const std::int32_t* db_part_words(std::int32_t k);
std::int32_t db_part_entries();

} // namespace def::dbio::query
