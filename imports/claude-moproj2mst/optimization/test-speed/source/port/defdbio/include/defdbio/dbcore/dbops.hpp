// defdbio/dbcore/dbops.hpp: DBOperations23, the operations of datarm23 on an open DB unit
// (packet p1-12). The file format is in docs/db-format-spec.md; deformdb::Database and
// deformdb::Writer read and write the same container at a higher level.
//
// All routines keep the original's Fortran interface: arguments by pointer, a Fortran unit
// number that the caller opened with fortran_open() mode 4/5 (dbcore/ftn_io.hpp), state in
// g_db (dbcore/globals_p1_12.hpp), error codes in *ierr:
//   0x65 read error (header, index, chunk)    0x66 step header read     0x67 MAR read
//   0x68 A read     0x6f directory write      0x70 header write         0x71 MAR write
//   0x72 A write    0x73 region table write   0x74 dynamic-array write  100 / 0x12d version
//   0xc9 step not found   0xca step stores no arrays   0xde / 0x1f9 / 0x2c3 bad family
//   0x12d unknown op of the step reader       1 signature or conversion failure
#pragma once

#include <cstdint>

namespace def::dbio::dbcore {

// @src 1403b5a04  dispatcher. op: 0 open (read record 2, signature, index records), 1 copy
// directory words 1..10 of `*nmar` entries (0: all) into `mar`, 2 read the step header, 3 read
// MAR and A, 4 write a step, 5 read the dynamic arrays; op < -29 is op 3 for the MAR region
// whose start address is |op + 30| only (A skipped), op >= 30 op 3 for A region op - 30 only
// (MAR skipped); any other op is op 0. Op 3 and op < -29 / >= 30 rewrite *op to 3.
// The licence check on entry (the "License Error(1)" branch, FUN_1400d3410) is omitted.
void db_operation(const std::int32_t* unit, std::int32_t* op, std::int32_t* slot,
                  std::int32_t* step, std::int32_t* nh, double* h, std::int32_t* nmar,
                  std::int32_t* mar, std::int32_t* na, double* a, std::int32_t* ierr);

// @src 1403b600f  op 0
void op_open(const std::int32_t* unit, std::int32_t* ierr);
// @src 1403b41d9  record 2, byte order, signature family, version number
void read_file_header(const std::int32_t* unit, std::int32_t* ierr);
// @src 1403b6425  op 1
void op_directory(const std::int32_t* unit, std::int32_t* count, std::int32_t* out,
                  std::int32_t* ierr);
// @src 1403b92bc  directory chunk of entry `*slot`: mode 1 load it (if not loaded), 2 write
// the chunk in memory; *cnum = chunk number, *pos = position of the entry in it
void chunk_io(const std::int32_t* mode, const std::int32_t* unit, const std::int32_t* slot,
              std::int32_t* cnum, std::int32_t* pos, std::int32_t* ierr);
// @src 1403b6679  ops 2 and 3
void op_read_step(const std::int32_t* unit, const std::int32_t* op, std::int32_t* slot,
                  const std::int32_t* step, std::int32_t* nh, double* h, std::int32_t* nmar,
                  std::int32_t* mar, std::int32_t* na, double* a, std::int32_t* ierr);
// @src 1403b7729  op 4
void op_write_step(const std::int32_t* unit, std::int32_t* slot, const std::int32_t* step,
                   std::int32_t* nh, double* h, std::int32_t* nmar, std::int32_t* mar,
                   std::int32_t* na, double* a, std::int32_t* ierr);
// @src 1403bcf76  op 5
void op_read_dynamic(const std::int32_t* unit, std::int32_t* slot, std::int32_t* ierr);

// @src 1403b44d2  before writing into a 2D/3D-family file: stamp every entry with the file's
// family and version, write the combined signature, make the file family 23 (version 10000)
void convert_to_combined(const std::int32_t* unit, std::int32_t* ierr);
// @src 1403b9bef  read the MAR region table of entry *pos (file version >= 6000)
void read_mar_table(const std::int32_t* unit, const std::int32_t* pos, std::int32_t* ierr);
// @src 1403badb1  read the A region table
void read_a_table(const std::int32_t* unit, const std::int32_t* pos, std::int32_t* ierr);
// @src 1403b9d4c  read the MAR regions (all, or the one starting at *sel, into mar(1...))
void read_mar_regions(const std::int32_t* unit, const std::int32_t* pos, const std::int32_t* sel,
                      std::int32_t* mar, const std::int32_t* compressed, std::int32_t* ierr);
// @src 1403baf0e  read the A regions
void read_a_regions(const std::int32_t* unit, const std::int32_t* pos, const std::int32_t* sel,
                    double* a, const std::int32_t* compressed, std::int32_t* ierr);
// @src 1403ba54c  write the MAR regions and their table
void write_mar_regions(const std::int32_t* unit, const std::int32_t* pos, const std::int32_t* nh,
                       const std::int32_t* nmar, const std::int32_t* mar,
                       const std::int32_t* compressed, std::int32_t* ierr);
// @src 1403bb996  write the A regions and their table
void write_a_regions(const std::int32_t* unit, const std::int32_t* pos, const std::int32_t* nh,
                     const std::int32_t* na, const double* a, const std::int32_t* compressed,
                     std::int32_t* ierr);

// ---- dynamic arrays (version >= 11000) --------------------------------------------------------
// @src 1403bc85b  RMALLOC the arrays the tables describe (g_dyn_words counts the words)
void alloc_dynamic();
// @src 1403bd3b5  read and decode the integer dynamic arrays into their blocks
void read_int_dynamic(const std::int32_t* unit, std::int32_t* ierr);
// @src 1403bda24  same for the real arrays
void read_real_dynamic(const std::int32_t* unit, std::int32_t* ierr);
// @src 1403be3d4  write descriptor, tables and arrays of the step at chunk position *pos
void write_dynamic(const std::int32_t* unit, const std::int32_t* pos, std::int32_t* ierr);
// @src 1403be6af  write the integer arrays
void write_int_dynamic(const std::int32_t* unit, std::int32_t* ierr);
// @src 1403be8ac  write the real arrays
void write_real_dynamic(const std::int32_t* unit, std::int32_t* ierr);
// @src 1403be093  read `*n` ints at record *rec (advanced) and swap; *ierr 1 on error
void read_ints_at(const std::int32_t* unit, void* buf, const std::int32_t* n, std::int32_t* rec,
                  std::int32_t* ierr);
// @src 1403beaa9  write `*n` ints at NXTREC (swapped on disk), advance NXTREC
void write_ints_next(const std::int32_t* unit, void* buf, const std::int32_t* n, std::int32_t* ierr);

// Address of word 1 of integer / real dynamic array k (1-based) after alloc_dynamic().
std::int32_t* dyn_int_data(std::int32_t k);
double* dyn_real_data(std::int32_t k);

// ---- signature and version --------------------------------------------------------------------
// @src 1403b1e45  record 1 is a 2D signature (DB450 or DB720): *yes = 1
void check_sig_2d(const std::int32_t* unit, std::int32_t* yes, std::int32_t* ierr);
// @src 1403b1ff5  3D signature (DB230)
void check_sig_3d(const std::int32_t* unit, std::int32_t* yes, std::int32_t* ierr);
// @src 1403b26e5  combined signatures (23-DB100, DEF-2D/3D-DB100-00)
void check_sig_23(const std::int32_t* unit, std::int32_t* yes, std::int32_t* ierr);
// @src 1403b32b5  version number of the file from SMI1 and the file family
void db_version(std::int32_t* version);

// @src 1403fbab0  open `file` on unit 3, read record 1, close: 2D or 2D-DB100-00 signature
void file_sig_2d(const char* file, std::int32_t* yes, std::int32_t* ierr);
// @src 1403fbda5  3D or 3D-DB100-00
void file_sig_3d(const char* file, std::int32_t* yes, std::int32_t* ierr);
// @src 1403fc05c  23-DB100, 2D-DB100-00 or 3D-DB100-00
void file_sig_23(const char* file, std::int32_t* yes, std::int32_t* ierr);
// @src 1402248c0  is `name` (a C string, may be null) a DEFORM DB: 1 yes, 0 no; *err set only
// on an open or read failure of a check
std::int32_t is_deform_db(const char* name, std::int32_t* err);
// @src 140224930  the 2D check of FUN_1402248c0
std::int32_t is_db_2d(const char* name, std::int32_t* err);
// @src 140224a20  the 3D check
std::int32_t is_db_3d(const char* name, std::int32_t* err);
// @src 140224b10  the combined check
std::int32_t is_db_23(const char* name, std::int32_t* err);

} // namespace def::dbio::dbcore
