// defdbio/dbread/w3.hpp: the step header and address-table part of the DB step read path DBREAD
// (FUN_1401cc420), packet p-dbread, helper W3. The analysis is in docs/re/dbmap/stepdriver.md
// (branch bz5k5e) sections 1.4, 1.6, 2.1 and 3.1; the code is ported from the binary.
//
// Return codes follow the original: ReadStepHeader 0 ok, 3 Fortran status 99, 4 other read
// error; ReadArrays 0 ok, 999 no DB object, 5 allocation / read error, 3 status 99, 1 version not
// supported (message in DB_STEP_DATA+0x1401, flag +0x1400).
//
// The Fortran routines keep their by-reference argument lists. Their print branches (argument
// `verbose` == 1: list-directed WRITEs of 'OPENING DATABASE FILE = ' and friends) are reached by
// no caller in DEF_PRE_64 (every caller passes 0) and are DEF_NOT_PORTED, as in p1-13.
#pragma once

#include <cstddef>
#include <cstdint>

namespace def::model {
class DB_STEP_DATA;
class Cls_1401dbfe0;
} // namespace def::model

namespace def::dbio::dbread {

// ---- C++ side (DB_STEP_DATA / Cls_1401dbfe0 methods) ---------------------------------------------

// @src 1401cca80  ReadStepHeader: step header of directory entry m->u740 into m->u1b80 (calloc of
// HSize doubles), NOBJ, platform, header cache, CURSIM, SIMNAM, OPRNAM, TITLE, fold flag.
std::int32_t read_step_header(model::DB_STEP_DATA* m, model::Cls_1401dbfe0* db);
// @src 1401cd1f0  ReadArraysFromDB: MAR / A of entry m->u740 into m->u1b88 / m->u1b78, the
// dynamic-array descriptor (version > 10999), DBMADR, address tables m->u1b90 / m->u1dc0.
std::int32_t read_arrays(model::DB_STEP_DATA* m, model::Cls_1401dbfe0* db);
// @src 1401cf190  ReadSingleMARArrayFromDB (only on the dead partial-load branch)
void read_single_mar_array(model::DB_STEP_DATA* m, std::int32_t* dst, std::int32_t* sel);
// @src 1401cf220  one MAR region (start *sel) of the current entry into dst (dead branch)
bool read_mar_region(model::DB_STEP_DATA* m, std::int32_t* dst, std::int32_t sel);
// @src 1401d8f40  byte at +0x1b5d ("link the DB object": DBREAD then stores db in +0x3390)
bool step_links_db(const model::DB_STEP_DATA* m);

// @src 1401dd4d0  header cache: allocate db+0xae8 (one 0xe0 record per directory entry) if
// needed, read the header of entry *idx into it (1401dfb80), close unit 3, db+0xae0 = *idx.
// keep_raw: the raw header stays in db+0x1908 / db+0x1910, else it is freed.
void read_header_cache(model::Cls_1401dbfe0* db, std::int32_t* idx, bool keep_raw);
// @src 1401dfb80  read the step header of entry *idx into h and fill cache record *idx;
// *cur_file = data file open on unit 3 (-1 none), close_prev: close it first when switching.
void read_header_cache_entry(model::Cls_1401dbfe0* db, std::int32_t* idx, double* h,
                             std::int32_t* cur_file, bool close_prev);
// @src 1401ddb40  dimension of directory entry idx (3 when there is no directory / bad index)
std::int32_t dir_dim(const model::Cls_1401dbfe0* db, std::int32_t idx);
// @src 1401ddba0  version of directory entry idx (10000 when there is none)
std::int32_t dir_version(const model::Cls_1401dbfe0* db, std::int32_t idx);
// @src 1401e1de0  first per-object header word F and per-object block size S of entry *idx
void header_layout(const model::Cls_1401dbfe0* db, const std::int32_t* idx, std::int32_t* f,
                   std::int32_t* s);

// @src 1401d9210  ctor of a 0xe0 header-cache record (through 1401d91a0)
void hdr_cache_record_init(void* rec);
// @src 1401d9150  ctor of a 0xcd0 per-object block of a cache record (through 1401d9120)
void hdr_object_block_init(void* blk);
// @src 1401e6930  `vector constructor iterator' (CRT): ctor on n elements of `size` bytes
void vector_ctor_iterator(void* p, std::size_t size, std::int32_t n, void (*ctor)(void*));

// @src 140256e20  realloc of n ints; n == 0 gives NULL (the old block is kept, not freed)
std::int32_t* realloc_ints(void* p, std::int32_t n);
// @src 1402571b0  realloc of n doubles
double* realloc_doubles(void* p, std::int32_t n);

// ---- Fortran side ---------------------------------------------------------------------------------

// @src 1403adc74  OPEN `name` on *unit (1..49, else 3; STATUS 'OLD', read-only direct access);
// *ierr = 99 on failure. Then, when name(1:200) differs from the cached name, op 0 and cache it.
void ftn_open_db(char* name, std::int32_t* len, std::int32_t* unit, std::int32_t* ierr,
                 std::int32_t* verbose);
// @src 1403da0b6  open `name` on unit 3, op 0, cache the name, op 2 (step header of entry *rec
// into h, *nh words), close. *ierr 99 when the open failed.
void ftn_read_step_header(char* name, std::int32_t* len, std::int32_t* rec, std::int32_t* nh,
                          double* h, std::int32_t* ierr, std::int32_t* verbose);
// @src 1403daf3e  op 2 on the already open *unit
void ftn_read_step_header_unit(std::int32_t* unit, std::int32_t* rec, std::int32_t* nh, double* h,
                               std::int32_t* ierr, std::int32_t* verbose);
// @src 1403db9e8  open, op 0 if the name changed, op 3 (MAR and A of entry *rec), close
void ftn_read_mar_a(char* name, std::int32_t* len, std::int32_t* rec, std::int32_t* nh, double* h,
                    std::int32_t* nmar, std::int32_t* mar, std::int32_t* na, double* a,
                    std::int32_t* ierr, std::int32_t* verbose);
// @src 1403dc13f  as 1403db9e8 for the MAR region starting at |*sel| (op -(|*sel| + 30)); version
// < 2301: op 3 of the pre-2301 3D reader 1403bef07
void ftn_read_mar_region(char* name, std::int32_t* len, std::int32_t* rec, std::int32_t* nh,
                         double* h, std::int32_t* nmar, std::int32_t* mar, std::int32_t* na,
                         double* a, std::int32_t* sel, std::int32_t* version, std::int32_t* ierr,
                         std::int32_t* verbose);
// @src 1403dd16d  open, op 0 if the name changed, op 5 (dynamic arrays of entry *rec, allocated in
// the datarm23 tables), close; *ni / *max_int / *nr / *max_real = descriptor words 1, 2, 11, 12
void ftn_read_dynamic_counts(char* name, std::int32_t* len, std::int32_t* rec, std::int32_t* ni,
                             std::int32_t* max_int, std::int32_t* nr, std::int32_t* max_real,
                             std::int32_t* ierr, std::int32_t* verbose);
// @src 1403dd950  DBMADR: I1 = MAR(2) - MAR(1), I2 = MAR(3) - MAR(2); NADS1(1..I1) = MAR(1..I1),
// NADS2(1..I2) = MAR(I1+1..I1+I2). No bound check against the 140 / 280 words of the buffers.
void dbmadr(const std::int32_t* mar, std::int32_t* nads1, std::int32_t* nads2,
            const std::int32_t* verbose);
// @src 1404b4108  Fortran runtime: compare a(1:la) with b(1:lb), the shorter padded with blanks;
// -1, 0, 1
std::int32_t ftn_compare(const char* a, const char* b, std::int32_t la, std::int32_t lb);

// @data 140b6ca20  CHARACTER*200 inside the datarm23 COMMON (dbcore::g_db): name of the DB file
// whose header and index op 0 last read for the DBREAD Fortran routines (1403adc74, 1403da0b6,
// 1403db9e8, 1403dc13f, 1403dc957, 1403dd16d and the 1403aa150.. family); they skip op 0 while
// the name is unchanged.
char* db_name_cache();
inline constexpr std::int32_t kDbNameCacheLen = 200;

} // namespace def::dbio::dbread
