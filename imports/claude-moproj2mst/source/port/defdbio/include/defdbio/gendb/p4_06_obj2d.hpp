// defdbio/gendb/p4_06_obj2d.hpp: GENDB writer of the 2D object list (packet p4-06).
//
// FUN_14017cb60 writes every object of DB_STEP_DATA::objects (m+0x1380) into its IIOBJ / LLOBJ
// records and its LLBND boundary records, calling the p4-05 array writers (nodes, elements, faces,
// boundary-condition functions) and sub-object writers in the binary's order; FUN_140176830 fills
// one 2D LLBND record (boundary node/element rows, rigid geometry rows of LLGRY, then movement).
//
// Arguments are the binary's: MAR / A are the whole GENDB buffers (m+0x50 / m+0x58), mt / at the
// MAR and A address tables (m+0x1b90: 70 starts then 70 widths; m+0x1dc0: 140 + 140).
#pragma once

#include <cstdint>

#include <defmodel/layout/cls_14017a6c0.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/llobj_data.hpp>

namespace def::dbio::gendb {

// @src 14017cb60  write the 2D objects: list, MAR, A, MAR table, A table, IIFNC, LLFNC, LLWIN,
//                 LLWRZ (the region pointers are 1401c4d90's p6/p5/p8/p9 in that order)
void write_objects_2d(model::Cls_14017a6c0* list, std::int32_t* mar, double* a, std::int32_t* mt,
                      std::int32_t* at, std::int32_t* iifnc, double* llfnc, double* llwin, double* llwrz);

// @src 140176830  one 2D LLBND record of obj: *type (1 rigid geometry, 2 rigid with mesh,
//                 3 deformable), *principal (record is the principal one), rec = the record,
//                 iibdy / iibel rows, iind (unused), *last (0, or -1 for the last record of the
//                 object), llgry rows, IIFNC, LLFNC, *index (1-based boundary / curve index)
void write_boundary_record_2d(model::LLOBJ_DATA* obj, const std::int32_t* type, const std::int32_t* principal,
                              double* rec, std::int32_t* iibdy, std::int32_t* iibel, std::int32_t* iind,
                              const std::int32_t* last, double* llgry, std::int32_t* iifnc, double* llfnc,
                              const std::int32_t* index);

// @src 140167900  1-based element of obj that has the boundary edge (*n1, *n2) (n1 at corner k,
//                 n2 at corner (k+1) mod 4 of the element's row), 0 if none, -1 without mesh
std::int32_t find_edge_element(model::LLOBJ_DATA* obj, const std::int32_t* n1, const std::int32_t* n2);

// @src 1401671b0  first LLBND record of obj (from obj+0x3588 to obj+0x358c) whose word 1 is 1.0
//                 (the principal flag); the single record when there is one; 0 if none
std::int32_t find_principal_record(model::LLOBJ_DATA* obj, const double* llbnd, const std::int32_t* width);

// @src 14017ed40  min / max (x, y) over the distinct consecutive node numbers of one boundary
//                 (Cls_14017e740: count at +0, node list at +8) with coordinates xy (2 per node);
//                 false for an empty boundary
bool boundary_bounding_box(const void* bnd, double* min, double* max, const double* xy);

// @src 1401ff7f0  1-based index of the rigid curve whose bounding box holds the overall minimum
//                 and maximum in both coordinates (the last such curve), 0 if none; false
//                 without geometry
bool principal_curve(model::LLOBJ_DATA* obj, std::int32_t* index);

// @src 1401ffa90  the same over the boundaries of a meshed object; false without mesh
bool principal_boundary(model::LLOBJ_DATA* obj, std::int32_t* index);

// @src 1402204a0  x ** n (thunk of 140221390)
double pow_int(double x, std::int32_t n);
// @src 140221390  x ** n by binary exponentiation (1 / x**|n| for n < 0)
double pow_int_impl(double x, std::int32_t n);

// @src 140276d70  rows of a rigid curve as LLGRY rows (6 doubles each); returns the row count.
//                 Segment curves (type 2) are copied; point curves are turned into segments.
std::int32_t write_curve_rows(void* geo2d, double* out);

// @src 14019a240  DB_WRITE_VAR +0x120 (next free IIBDY/IIBEL row)
std::int32_t gendb_bnd_row_cursor(const model::DB_STEP_DATA* m);
// @src 14019a260  set DB_WRITE_VAR +0x120
void gendb_set_bnd_row_cursor(model::DB_STEP_DATA* m, const std::int32_t* v);

} // namespace def::dbio::gendb
