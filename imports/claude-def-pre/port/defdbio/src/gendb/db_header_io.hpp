// gendb/db_header_io.hpp: the Fortran-side helpers the GENDB frame calls (packet p4-01): opening
// and closing the DB unit, the directory words and record 2 of the step to write, the dynamic-array
// tables, and the Fortran string helpers they use.
#pragma once

#include <cstdint>

namespace def::dbio::gendb {

// 1403ae0b6: OPEN the DB on `*unit` (mode 4: direct, unformatted, RECL 512) with STATUS 'OLD'
// (*mode == 1) or 'NEW'; *ierr = 99 on failure. *unit outside 1..49 becomes 3.
void open_db(const char* name, const std::int32_t* len, std::int32_t* unit, const std::int32_t* mode,
             std::int32_t* ierr, const std::int32_t* verbose);
// 1403ae06f: CLOSE(*unit)
void close_db(const std::int32_t* unit);
// 1403b3f73: directory words of the step to write (mesh, step, interval, sim number, sim mode)
// and the step's family / version.
void set_step_words(const std::int32_t* mesh, const std::int32_t* step, const std::int32_t* interval,
                    const std::int32_t* sim_no, const std::int32_t* sim_mode,
                    const std::int32_t* family, const std::int32_t* version);
// 1403b400b: record 2: title (left-justified, blank-padded 80 chars), SMI1..SMI4 (8 chars each
// from smi[0..31]) and the six header ints.
void set_record2(char* title, const char* smi, const std::int32_t* istart, const std::int32_t* ismtyp,
                 const std::int32_t* nrsets, const std::int32_t* nxtrec, const std::int32_t* w33,
                 const std::int32_t* w34);
// 1403bc2c4: clear the dynamic-array tables, store NI / NR and their sizes, then RMALLOC them
// (p1-12's alloc_dynamic 1403bc85b). 1403bc640 (RFREE of the previous arrays) runs first.
void set_dynamic_tables(const std::int32_t* ni, const std::int32_t* nr, const std::int32_t* int_sizes,
                        const std::int32_t* real_sizes);
void free_dynamic_arrays(); // 1403bc640
// 1403bcda9 / 1403bcaa6: get (*mode 1) or put a real / int dynamic array k of the tables.
void dynamic_real(const std::int32_t* mode, const std::int32_t* k, double* data, std::int32_t* n,
                  std::int32_t* w2, std::int32_t* w3, std::int32_t* w4, std::int32_t* w5, void* unused1,
                  void* unused2, std::int32_t* ierr);
void dynamic_int(const std::int32_t* mode, const std::int32_t* k, std::int32_t* data, std::int32_t* n,
                 std::int32_t* w2, std::int32_t* w3, std::int32_t* w4, std::int32_t* w5, void* unused1,
                 void* unused2, std::int32_t* ierr);

// 14044a4b1: ADJUSTL of s(1:n) with NULs counting as blanks; returns the trimmed length.
std::int32_t adjust_left(char* s, std::int32_t n);
// 1404b33e4: Fortran CHARACTER assignment dst(1:dlen) = src(1:slen) (blank padded)
void char_assign(char* dst, const char* src, std::int32_t dlen, std::int32_t slen);

// 140235800: month (1..12), day and years since 1900 of the local time (140235890 _time64,
// 140235870 _localtime64).
void local_date(std::int32_t* month, std::int32_t* day, std::int32_t* year);

} // namespace def::dbio::gendb
