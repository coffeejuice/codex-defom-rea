// defdbio/step/db_header_cache.hpp: the step-header cache of the DB info object Cls_1401dbfe0
// (db+0xae8, one 0xe0-byte record per directory entry) and the directory queries that TCYCLE
// (command case 0x12, packet p67-5) uses. Spec: docs/re/commands/TCYCLE.md sections 3, 4, 6.
//
// The cache is filled from the step headers read through datarm23 on Fortran unit 3 (p1-12
// dbcore). Layout of a cache record (DbInfoEntryE0, defmodel/step/db_step.hpp), as FUN_1401dfb80
// writes it from the step header H (doubles, 0-based):
//   +0x00 int    (int)H[0]  number of objects (NUMOBJ)
//   +0x04 int    (int)H[1]  primary die (PDIE): the stroke below is taken from object PDIE-1
//   +0x08 double H[2]       TNOW
//   +0x10 double H[3]       TNOW (TEND slot of the header echo)
//   +0x18 int    (int)H[8]  only for 3D entries
//   +0x1c int    (int)H[hdr_index(0x1406e8524)]
//   +0x20 ptr    per-object records (0xcd0 bytes each: 400 doubles, name char[0x50] at +0xc80)
//   +0x28 double stroke: |(x, y[, z])| of the PDIE object's displacement items
//   +0x30..+0x3c (int)H[12], H[9], H[10], H[11]
//   +0x4c char[0x41] operation name, +0x8d char[0x51] simulation name
// Side effect of every header read: the global min/max table db+0xaf0 (doubles) is set from the
// first object of the entry and widened by the others, for the first two pairs of the
// dimension's min/max pair table (FUN_1401dfb80; the table thus describes the last entry read).
#pragma once

#include <cstdint>

#include <defmodel/step/db_step.hpp>

namespace def::dbio::step {

using def::model::Cls_1401dbfe0;

// ---- per-object record of a cache entry (array at entry+0x20) ----------------------------------
struct DbObjHeader {
    // @src 1401d9150  ctor body: 400 doubles = 0, name[0] = 0 (1401d9120 is the ctor thunk)
    void init();
    double v[400];   // +0x000 the object's header block (min(stride, 400) doubles)
    char name[0x50]; // +0xc80 object name (64 chars, trimmed)
};
static_assert(sizeof(DbObjHeader) == 0xcd0);

// Field accessors of a cache record (the layout class has only padding there).
std::int32_t& entry_i32(def::model::DbInfoEntryE0& e, std::size_t off);
double& entry_f64(def::model::DbInfoEntryE0& e, std::size_t off);
char* entry_chars(def::model::DbInfoEntryE0& e, std::size_t off);
DbObjHeader*& entry_objects(def::model::DbInfoEntryE0& e);
def::model::DbInfoEntryE0* cache_entries(Cls_1401dbfe0& db);

// @src 1401d9210  cache record init (the element ctor 1401d91a0 calls it)
void init_cache_entry(def::model::DbInfoEntryE0& e);

// 1401c7f80 (ported in defmodel/hdrstats/object_stats.hpp, p407::load_hdr_slots)  fill the step-header layout table (globals_p67_5.hpp) for *dim == 2 (2D) or
// any other value (3D)
void set_header_layout(const std::int32_t* dim);

// ---- directory queries ----------------------------------------------------------------------
// @src 1401dde10  SimNo of directory entry idx; 0 without a directory or for idx out of range
std::int32_t entry_sim_no(const Cls_1401dbfe0& db, std::int32_t idx);
// @src 1401ddb40  dimension of entry idx (+0x1c); 3 without a directory or out of range
std::int32_t entry_dim(const Cls_1401dbfe0& db, std::int32_t idx);
// @src 1401ddba0  version of entry idx (+0x20); 10000 without a directory, when empty, or idx >= n
std::int32_t entry_version(const Cls_1401dbfe0& db, std::int32_t idx);
// @src 1401e5de0  DB part (file) that holds entry idx; 0 for a single-file DB
std::int32_t entry_part(const Cls_1401dbfe0& db, std::int32_t idx);
// @src 1401e5eb0  first entry index of part `part`; 0 for a single-file DB or a bad part
std::int32_t part_first_entry(const Cls_1401dbfe0& db, std::int32_t part);
// @src 1401e5d60  file name of part `part` into buf (the DB name db+0x650 for a single file)
void part_file_name(const Cls_1401dbfe0& db, char* buf, std::int32_t part);
// @src 1401e1de0  word offset of the first object block in the step header and words per object
void object_block_layout(Cls_1401dbfe0& db, const std::int32_t* idx, std::int32_t* first,
                         std::int32_t* stride);

// ---- datarm23 front ends on a Fortran unit ----------------------------------------------------
// @src 1403adc74  OPEN the DB `name` (Fortran field, 200 characters used) on *unit (1..49, else
// 3) read-only, STATUS='OLD'; when the name differs from the last one opened this way (200-char
// field DAT_140b6ca20), run op 0 and remember the name. *err: 0, 99 (the open failed; op 0 still
// runs and may overwrite it). *print == 1 (echo) is not ported: no caller passes it.
void open_db_unit(const char* name, const std::int32_t* name_len, std::int32_t* unit,
                  std::int32_t* err, const std::int32_t* print);
// @src 1403daf3e  op 2: read the step header of directory slot *slot (1-based) into h (*nh words;
// *nh < 1 = the entry's size, written back). *err from the DB operation (0x66 read error).
void read_step_header(const std::int32_t* unit, std::int32_t* slot, std::int32_t* nh, double* h,
                      std::int32_t* err, const std::int32_t* print);
// @src 1403ae06f  CLOSE(*unit)
void close_db_unit(const std::int32_t* unit);
// The 200-character name field of FUN_1403adc74 (DAT_140b6ca20, inside p1-12's DbCommon).
char* last_open_name();

// ---- the cache ---------------------------------------------------------------------------------
// @src 1401dfb80  read the step header of entry *idx into h and its cache record. *part is the
// part whose file is open on unit 3 (-1 none): another part is opened first (closing the old
// one when close_old). Returns early, record untouched, on an open or read error.
void read_entry_header(Cls_1401dbfe0& db, std::int32_t* idx, double* h, std::int32_t* part,
                       bool close_old);
// @src 1401dd250  load the header cache of every entry once (db+0x1900 = 1 afterwards):
// allocate db+0xae8 if absent, read all headers, CLOSE(3), re-read the DB header. `flag` is
// passed on to FUN_1401dfb80, which does not use it.
void load_header_cache(Cls_1401dbfe0& db, bool flag);
// @src 1401dd0d0  re-read the DB header of db+0x650 (all outputs discarded): -1 file missing,
// -2 header error, 0 ok
std::int32_t reread_db_header(Cls_1401dbfe0& db);
// @src 1401df8a0  first and last entries with arrays of operation `sim`: from the first one dim,
// nobj, pdie, step, TNOW (+0x08), stroke; from the last one step, TNOW (+0x10), stroke. Outputs
// of a part not found stay as they were. false when nothing matched or there is no directory
// or no cache.
bool sim_first_last(const Cls_1401dbfe0& db, std::int32_t sim, std::int32_t* dim, std::int32_t* nobj,
                    std::int32_t* pdie, std::int32_t* step_first, std::int32_t* step_last,
                    double* t_first, double* t_last, double* s_first, double* s_last);

} // namespace def::dbio::step
