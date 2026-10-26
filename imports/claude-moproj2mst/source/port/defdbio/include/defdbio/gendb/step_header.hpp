// defdbio/gendb/step_header.hpp: GENDB's step header and the DB address tables of the step to
// write (packet p4-07). Per-object statistics: defmodel/hdrstats/object_stats.hpp.
//
// Layout of the header written by 1401e2180 (0-based doubles, docs/re/dbmap/stepdriver.md §2.3):
// 0 NOBJ, 1 PDIE, 2 / 3 TNOW, 4 CURSIM 1, platform code 2 at slot 0x1406e8524 (5 in 3D, 8 in 2D),
// 8 the fold flag (3D), 9 the object block length 400, 10 / 11 CURSIM 2 / 3, 12 the block start
// 100, 16..23 OPRNAM, 24..33 SIMNAM; then one block of 400 per object from 100 on.
#pragma once

#include <cstdint>

namespace def::model {
class Cls_14017a6c0;
class Cls_1401dbfe0;
class Cls_1401e7b10;
class DB_STEP_DATA;
} // namespace def::model

namespace def::dbio::gendb {

// @src 1401c5020  write the step header into m->u060, then load the Fortran address tables of
// op 4 from m->u1b90 / m->u1dc0. hsize: header length out; pos: passed through to the header
// writer (unused there); use_version: take the version from 1401c56a0 instead of the step's.
void write_step_header(def::model::DB_STEP_DATA* m, void* unused2, const std::int32_t* pos,
                       std::int32_t* hsize, def::model::Cls_1401dbfe0* db, const char* use_version);

// @src 1401c56a0  DB_WRITE_VAR version word * 10 (1100 -> 11000)
std::int32_t write_version(const def::model::DB_STEP_DATA* m);

// @src 1401e1f30  header block start F = 100 and object block length S = 400
void header_block_sizes(const std::int32_t* dim, std::int32_t* first, std::int32_t* stride);

// @src 1401e2180  the step header (see the layout above). db: the DB object whose header cache
// (+0xae0 / +0xae8) can carry the previous object blocks over; dim / version: the model's.
void fill_step_header(def::model::Cls_1401dbfe0* db, void* unused2, std::int32_t* hsize,
                      double* h, std::int32_t unused5, def::model::Cls_1401e7b10* sim,
                      const std::int32_t* nobj, const std::int32_t* fold,
                      def::model::Cls_14017a6c0* objects, const std::int32_t* dim,
                      const std::int32_t* version);

// ---- Fortran address tables of op 4 (DB module) ----
// @src 1403b3bcc  2D: NADS1 (0x140b48ec0) = mar[0..*n1-1]
void set_mar_table_2d(const std::int32_t* version, const std::int32_t* mar, const std::int32_t* n1);
// @src 1403b3bfd  2D: NADS2 (0x140b6ce80) = a[0..*n2-1]
void set_a_table_2d(const std::int32_t* version, const std::int32_t* a, const std::int32_t* n2);
// @src 1403b3c2e  3D: NADS1 = mar[..]; version < 3000 also sets 0x140707310 = 20, 0x140707314 = 50
void set_mar_table_3d(const std::int32_t* version, const std::int32_t* mar, const std::int32_t* n1);
// @src 1403b3c9b  3D: NADS2 = a[..]; version < 3000 fills the old fixed table 0x140707318 ..
void set_a_table_3d(const std::int32_t* version, const std::int32_t* a, const std::int32_t* n2);
// @src 1403b973a  MAR region table g_db.mar_regions from NADS1: 58 regions (length, start)
void build_mar_regions();
// @src 1403b9ad6  A region table g_db.a_regions from NADS2: 126 regions
void build_a_regions();

// Resets the globals of globals_p4_07.hpp (tests).
void reset_globals_p4_07();

} // namespace def::dbio::gendb
