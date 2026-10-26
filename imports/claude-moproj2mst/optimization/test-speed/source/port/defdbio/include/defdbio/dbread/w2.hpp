// defdbio/dbread/w2.hpp: DBREAD (FUN_1401cc420) helper W2 of packet p-dbread.
//
// Subtrees: ReadSimCtrl 1401cf4a0 (sim-control block, function tables, UNNAME/UENAME) and
// ReadMatlInfo 1401cff20 (LLMPR records), plus the per-object node / element array readers
// (14016dfb0, 140170ed0), ReadSingleAArrayFromDB (1401d4400, 1401cecc0) and the old-format
// (version < 2301) step reader of the DB core (1403dc957 -> 1403bef07).
// Word maps: docs/re/dbmap/simctrl.md, material.md, objarrays.md (branch claude/project-thread-
// bz5k5e); the decompile and listing are the reference.
//
// Arguments keep the binary's order and by-pointer style. Arrays (IISIM, LLSIM, LLMPR records, the
// MAR / A arrays) are the decoded step arrays DBREAD holds at DB_STEP_DATA+0x1b88 / +0x1b78.
#pragma once

#include <cstdint>

#include <defmodel/layout/cls_14028b850.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/llobj_data.hpp>

namespace def::model {
class Cls_14028a4c0;
class Cls_1403753f0;
class Cls_1401dbfe0;
class DYNARY;
class Material;
} // namespace def::model

namespace def::dbio::dbread {

// ---- simulation control (w2_simctrl.cpp) ------------------------------------------------------

// @src 1401cf4a0  ReadSimCtrl: function / window table pointers (+0x2220..+0x2238), model counts
// from IISIM, the sim-control block (1401eb870), then UNNAME / UENAME (1401cf930 / 1401cfc30)
void read_sim_ctrl(model::DB_STEP_DATA* m);

// @src 1401eb870  sim-control block from IISIM / LLSIM / IIUVL (3D and 2D branches)
void sim_ctrl_read_from_db(model::Cls_1401e7b10* s, const std::int32_t* iisim, const double* llsim,
                           const char* iiuvl, const std::int32_t* iifnc, const double* llfnc);

// @src 1401cf930  UNNAME: node names from the tail of LLND1 (3D) or A slot 57 (2D)
void read_node_names(model::DB_STEP_DATA* m);
// @src 1401cfc30  UENAME: element names from the tail of LLET1 (3D) or A slot 56 (2D)
void read_element_names(model::DB_STEP_DATA* m);
// @src 1401cb9e0  calloc the node-name array: *n pointers, 0x40 bytes each (nothing when *n == 0)
void alloc_node_names(model::DB_STEP_DATA* m, const std::int32_t* n);
// @src 1401cb730  same for the element names (+0x33b0)
void alloc_element_names(model::DB_STEP_DATA* m, const std::int32_t* n);

// @src 1403759b0  furnace record (ENVTMP 10): FRNNAM and the zone functions
void furnace_read_from_db(model::Cls_1403753f0* f, model::DB_STEP_DATA* m, const std::int32_t* iifnc,
                          const double* llfnc, std::int32_t fid);

// @src 140130dc0  self-describing function `fid` (tag 10.0) into f
void dynary_read_selfdesc(model::DYNARY* f, const std::int32_t* iifnc, const double* llfnc, std::int32_t fid);
// @src 140131e30  data block of f by ndim (1: 140131ea0, 2: 140131fa0); nothing for a null block
void dynary_read_data(model::DYNARY* f, const double* data);
// @src 140131fa0  2-D/3-D data: the axes back to back, then the value block
void dynary_read_axes_values(model::DYNARY* f, const double* data);

// ---- materials (w2_material.cpp) --------------------------------------------------------------

// @src 1401cff20  ReadMatlInfo: one material per LLMPR record, MSTMTR slots -> numbers, 1401d6250
void read_materials(model::DB_STEP_DATA* m);

// @src 14028f300  LLMAT_DATA::ReadInfoFromDBArray: one LLMPR record
void material_read_from_db(model::Material* mat, const double* rec, const std::int32_t* iifnc,
                           const double* llfnc, std::int32_t version);

// @src 14028a740  GRNDAT mesoscale grain record
void grain_record_read(model::Cls_14028a4c0* r, const double* in);
// @src 14028a950  grain record sub-table: code, n1 / constant, n2, data
void grain_subtable_read(model::Cls_14028a4c0* r, const double* code, const double* n1, const double* n2,
                         const double* data, model::DYNARY* f);
// @src 14028b3b0  GRNDAT record list: `count` records whose function numbers are in function `fid`
void grain_record_list_read(model::Cls_14028b640::RecordList* l, const std::int32_t* iifnc,
                            const double* llfnc, std::int32_t count, std::int32_t fid);
// @src 14028c420  FRCMOD code 11: nine reals into the FRCMOD block (+0x1b0..+0x1f0)
void frcmod_code11_read(void* frc, const double* in);

// ---- object arrays, ReadSingleAArrayFromDB (w2_objarrays.cpp) ---------------------------------

// @src 1401d4400  ReadSingleAArrayFromDB(region, elem): reads A region *region of the step and
// hands it to every object; returns 1 when it did (*region valid and not read yet), else 0
std::uint64_t read_single_a_array(model::DB_STEP_DATA* m, const std::int32_t* region, const char* elem);
// @src 1401cecc0  read A region *region from the DB file (body unreachable: 1401d2b70 returns 1)
void read_single_a_array_from_file(model::DB_STEP_DATA* m, const std::int32_t* region);

// @src 14016dfb0  LLOBJ_DATA node arrays from the A array (region *region, -1 = all)
void obj_read_node_arrays(model::LLOBJ_DATA* obj, std::int32_t* mar, double* a, std::int32_t* mar_tab,
                          std::int32_t* a_tab, std::int32_t* widths, std::int32_t numnp,
                          const std::int32_t* region, std::int32_t limit);
// @src 140170ed0  LLOBJ_DATA element arrays from the A array (region *region, -1 = all)
void obj_read_element_arrays(model::LLOBJ_DATA* obj, std::int32_t* mar, double* a, std::int32_t* mar_tab,
                             std::int32_t* a_tab, std::int32_t* widths, std::int32_t* iiobj_rec,
                             const std::int32_t* region);
// @src 140161060  byte at LLOBJ_DATA+0x3610
std::uint8_t obj_byte_3610(const model::LLOBJ_DATA* obj);

// @src 1401cc160  step version setter (+0x1b64)
void set_step_version(model::DB_STEP_DATA* m, std::int32_t v);
// @src 1401cc1a0  directory sizes of step entry m+0x740 (hsize, MAR size, A size, version);
// updates the step version when it differs; 0 when db is null
std::uint64_t step_dir_sizes(model::DB_STEP_DATA* m, model::Cls_1401dbfe0* db, std::int32_t step,
                             std::int32_t* hsize, std::int32_t* mar_size, std::int32_t* a_size,
                             std::int32_t* version);

// DB file set of Cls_1401dbfe0: +8 file count, +0xc (first, last) step ranges, +0x330 names,
// +0x650 the single file name.
// @src 1401e5d60  file name of entry i (the single name when count < 2 or i out of range)
void db_file_name(model::Cls_1401dbfe0* db, char* out, std::int32_t i);
// @src 1401e5de0  entry whose step range holds `step` (0 when none or count < 2)
std::int32_t db_file_index(model::Cls_1401dbfe0* db, std::int32_t step);
// @src 1401e5eb0  first step of entry i (0 when count < 2 or i out of range)
std::int32_t db_file_first(model::Cls_1401dbfe0* db, std::int32_t i);
// @src 1401e5f00  file name and first step of the entry holding `step`
void db_file_for_step(model::Cls_1401dbfe0* db, char* out, std::int32_t* first, std::int32_t step);

// ---- old-format step reader of the DB core (w2_dbops.cpp) -------------------------------------

// @src 1403dc957  open `file` (CHARACTER*200) on unit 3, read A region *region of step slot *slot
// (version < 2301: 1403bef07 op 3, else 1403b5a04 op |*region| + 30), close
void read_a_region_from_file(const char* file, const std::int32_t* nchar, std::int32_t* slot, std::int32_t* nh,
                             double* h, std::int32_t* nmar, std::int32_t* mar, std::int32_t* na, double* a,
                             const std::int32_t* region, const std::int32_t* version, std::int32_t* ierr,
                             const std::int32_t* iprint);
// @src 1403bef07  DB operations of the old (single-family, version < 2301) format: 0 open,
// 1 directory, 2 step header, 3 MAR and A, 4 write a step
void db_operation_old(const std::int32_t* unit, std::int32_t* op, std::int32_t* slot, std::int32_t* step,
                      std::int32_t* nh, double* h, std::int32_t* nmar, std::int32_t* mar, std::int32_t* na,
                      double* a, std::int32_t* ierr);
// @src 1403c0e60  old-format directory record of entry *slot: mode 1 load, 2 write;
// *rec = record number, *pos = position of the entry in it
void chunk_io_old(const std::int32_t* mode, const std::int32_t* unit, const std::int32_t* slot,
                  std::int32_t* rec, std::int32_t* pos, std::int32_t* ierr);
// @src 140444458  copy *n characters of src to dst one at a time (140469eb0 when the platform
// swap mode is 2, else 140469eb6); the loop index lives in the global DAT_14070dd00
void copy_chars(const char* src, char* dst, const std::int32_t* n);
// @src 140469eb0  dst[0] = src[0] (the length argument is not used)
void copy_char(char* dst, const char* src, std::int64_t n);

} // namespace def::dbio::dbread
