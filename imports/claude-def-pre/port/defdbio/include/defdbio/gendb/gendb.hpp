// defdbio/gendb/gendb.hpp: GENDB, writing the model as one step of a .DB file (Phase 4, packet
// p4-01: driver and frame). Plan: docs/phase4-packets.md. Spec: docs/re/db-model-mapping.md
// section 3 and dbmap/stepdriver.md (branch claude/project-thread-bz5k5e).
//
// Call chain: menu 7 (def_pre) or the KEY command GENDB -> gendb() 1401b9940 -> write_db()
// 1401bb550 -> write_step() 1401bcbb0, which writes the DB header, the region tables, the zeroed
// MAR / A buffers and the address tables, then hands the buffers to the class writers of the other
// P4 packets (sim control, materials, objects, inter-objects, material pairs), collects the
// dynamic arrays, builds the step header and writes everything with DB operation 4.
#pragma once

#include <cstdint>

namespace def::model {
class DB_STEP_DATA;
class Cls_1401dbfe0;
class SfLinkedList;
class LLOBJ_DATA;
} // namespace def::model

namespace def::dbio::gendb {

namespace model = def::model;
using def::model::DB_STEP_DATA;

// ---- driver ------------------------------------------------------------------------------------
// 1401b9940: MESHNO + 1 when the remesh flag (+0x2f89) is set (flag cleared), then write_db() when
// direct writing (+0x8) is on, else the no-op 1401b9a40. chdir(path) first; its result is unused.
std::int32_t gendb(DB_STEP_DATA& m, const char* path, std::int32_t cur_idx, std::int32_t check,
                   std::int32_t dbopt);
// 1401bb550: checks, old/new DB, NSTART sign mode, step placement, open, write_step(), close,
// temp-file swap. 0 ok, 1 failed (messages on stdout).
std::int32_t write_db(DB_STEP_DATA& m, const char* path, std::int32_t check, std::int32_t dbopt,
                      std::int32_t signmode);
// 1401bcbb0: the step write itself (unit 3 is open). *ierr != 0 on a DB error.
void write_step(DB_STEP_DATA& m, const char* path, std::int32_t* pos, std::int32_t* count,
                std::int32_t* prev, model::Cls_1401dbfe0* info, const char* keep_version,
                std::int32_t* ierr);

// 1401b9c00: warns when the DB on `path` was written by a higher version. true if it opened.
bool check_db_version(DB_STEP_DATA& m, const char* path);
// 1401ba780: fopen(name, "r") succeeds.
bool file_readable(const char* name);
// 1401b85d0: removes the file if it can be opened ("rt"). 1 when there was nothing to remove or
// the remove worked, 0 when remove failed.
std::int32_t remove_if_exists(const char* name);
// 1401b8bf0: _chdir; *err = _doserrno on failure. true on success.
bool change_dir(const char* path, unsigned long* err);
// 1401b9810: true if a step of version *cur may be added to a DB of version *db.
bool versions_compatible(const DB_STEP_DATA& m, const std::int32_t* db, const std::int32_t* cur);
// 1401e1f60: where the step goes in the directory of `info` (see stepdriver.md, placement).
// Returns -1 when MESHNO (*meshno) is lower than the last step's (*last_mesh), else 0.
std::int32_t place_step(const model::Cls_1401dbfe0* info, const std::int32_t* nstart,
                        std::int32_t* count, std::int32_t* pos, std::int32_t* prev,
                        std::int32_t* prev_step, const std::int32_t* meshno, std::int32_t* last_mesh);
// 1401ddc10: highest version word (+0x20) of the directory entries, 10000 without a directory.
std::int32_t db_max_version(const model::Cls_1401dbfe0* info);
// 1401dc3a0: name of a new DB and its version.
void db_info_set_new(model::Cls_1401dbfe0* info, const char* name, const std::int32_t* version);

// ---- frame -------------------------------------------------------------------------------------
// 1401bffb0: directory words of the step (1403b3f73) and record 2 of the file (1403b400b).
void write_db_header(DB_STEP_DATA& m, std::int32_t* step, std::int32_t* pos, std::int32_t* count,
                     std::int32_t* prev, model::Cls_1401dbfe0* info, const char* keep_version);
// 1401c03f0: region widths for the current counts, then the starts (1401c1980 3D, 1401c2d10 2D).
void region_sizes(DB_STEP_DATA& m);
void region_starts_3d(DB_STEP_DATA& m); // 1401c1980
void region_starts_2d(DB_STEP_DATA& m); // 1401c2d10
// 1401cafe0: width code of ELMNOD option slot k (0..2): 1, 4 (2D), 8 (3D with an 8-node object)
std::int32_t elmnod_width(DB_STEP_DATA& m, std::int32_t k);
// 1401c4560: MAR / A / header lengths and the zeroed buffers at +0x50 / +0x58 / +0x60.
void alloc_buffers(DB_STEP_DATA& m);
// 1401c4840: the two address tables into IIAD1 / IIAD2 of the MAR buffer.
void store_address_tables(DB_STEP_DATA& m);
// 1401c5260: the objects' dynamic arrays into the DB core's tables (1403bc2c4).
void collect_dynamic_arrays(DB_STEP_DATA& m);
// 1401c54c0: progress stage n (reported through the empty 14001cdb0).
void progress(DB_STEP_DATA& m, const std::int32_t* stage);
// 1401c5630: simulation mode written to the directory: 4 with microstructure, diffusion or grain.
std::int32_t sim_mode(const DB_STEP_DATA& m);
// 140160bf0: the object has a mesh (+0x87c != 0).
bool has_mesh(const model::LLOBJ_DATA* o);

// ---- small model accessors this packet uses --------------------------------------------------
bool direct_write(const DB_STEP_DATA& m);           // 1401c5690 (+0x8)
std::int32_t stored_version(const DB_STEP_DATA& m); // 1401cc180 (+0x1b64)
bool remesh_flag(const DB_STEP_DATA& m);            // 1401d7ed0 (+0x2f89)
void set_remesh_flag(DB_STEP_DATA& m, std::uint8_t v); // 1401d7eb0

} // namespace def::dbio::gendb
