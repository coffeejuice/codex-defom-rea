// defdbio/dbread/w1.hpp: DBREAD (FUN_1401cc420) object read path, helper W1 of packet p-dbread.
// Entry: read_obj_info (FUN_1401d2230, "ReadObjInfo"): one LLOBJ_DATA per IIOBJ/LLOBJ row, then the
// object's sub-blocks (Cls_1401860a0 at +0x1ea0, Cls_1401956d0 at +0x2e68, Cls_140189440 at
// +0x2d10), its boundary records (LLBND), 2D curves / 3D surfaces and the movement block
// (OMV_DATA, Cls_14018b220 at +0x2160).
//
// Argument conventions (as in the binary):
//   iiobj / llobj   the object's row in the MAR region IIOBJ / the A region LLOBJ
//   iifnc / llfnc   function table: iifnc[2k-2] = LLFNC start (1-based) of function k,
//                   iifnc[2k-1] = its id; llfnc the LLFNC doubles
//   llwin / win_w   window rows (LLWIN) and the row width; llwrz the window coordinates (LLWRZ)
//   mar_tab/a_tab   DB_STEP_DATA::u1b90 / u1dc0 address tables
// Pointers are non-const because 1402589a0 writes into the LLFNC record it reads (type 5).
#pragma once

#include <cstdint>

namespace def::model {
class DB_STEP_DATA;
class LLOBJ_DATA;
class LLWINDEF_DATA;
class DYNARY;
class OBJ_GEO2D;
class GEO2D_AND_MESH2D;
namespace geo_kw {
struct DiePointSet;
struct DieEdgeSet;
} // namespace geo_kw
} // namespace def::model

namespace def::dbio::dbread {

using model::DB_STEP_DATA;
using model::DYNARY;
using model::LLOBJ_DATA;

// ---- objects ---------------------------------------------------------------------------------
// @src 1401d2230  ReadObjInfo: objects of the step from IIOBJ / LLOBJ
void read_obj_info(DB_STEP_DATA* m);
// @src 14016caf0  object header: kind, mesh flag, node / element / boundary ranges, element type,
//                 name, the +0x3598 table (type 0x7c)
void read_object_header(LLOBJ_DATA* obj, std::int32_t* iiobj, double* llobj, std::int32_t* iifnc,
                        double* llfnc);
// @src 140186f30  Cls_1401860a0 (obj+0x1ea0) from IIOBJ / LLOBJ
void read_obj_block_1ea0(void* blk, std::int32_t* iiobj, double* llobj, std::int32_t* iifnc, double* llfnc);
// @src 14018a180  Cls_140189440 (obj+0x2d10): values and its windows
void read_obj_block_2d10(void* blk, std::int32_t* iiobj, double* llobj, std::int32_t* iifnc, double* llfnc,
                         double* llwin, std::int32_t win_w, double* llwrz);
// @src 140195ff0  Cls_1401956d0 (obj+0x2e68): its window lists (version dependent)
void read_obj_block_2e68(void* blk, std::int32_t* iiobj, double* llobj, std::int32_t* iifnc, double* llfnc,
                         double* llwin, std::int32_t win_w, double* llwrz, std::int32_t version);
// @src 140174e40  boundary records (LLBND), 2D curves / 3D surfaces, movement block
void read_obj_boundaries(LLOBJ_DATA* obj, std::int32_t* mar, double* a, std::int32_t* mar_tab,
                         std::int32_t* a_tab, std::int32_t* iifnc, double* llfnc);
// @src 140163340  3D: surfaces k >= n are cleared and reset, surface count = min(n, 100);
//                 2D: 140198230
void set_surface_or_curve_count(LLOBJ_DATA* obj, std::int32_t n);
// @src 140198230  curve count of geometry 0 = n (14027fe20)
void set_curve_count(LLOBJ_DATA* obj, std::int32_t n);

// ---- movement (OMV_DATA, obj+0x2160) ---------------------------------------------------------
// @src 14018d3c0  OMV_DATA::ReadFromDB: rec is the object's first LLBND record
void read_movement(void* mv, double* rec, std::int32_t* iifnc, double* llfnc, std::int32_t platform);

// ---- windows ---------------------------------------------------------------------------------
// @src 14025de80  LLWINDEF_DATA::ReadFromDBArrays: one LLWIN row
void read_window(model::LLWINDEF_DATA* w, double* row, double* llwrz, std::int32_t* iifnc, double* llfnc);

// ---- function tables (DYNARY) ------------------------------------------------------------------
// @src 1401309b0  DYNARY from function fn of IIFNC/LLFNC; *with_shape != 0: 2D/3D tables start
//                 with their point counts
void read_func_from_db_array(DYNARY* f, std::int32_t* iifnc, double* llfnc, std::int32_t fn,
                             const char* with_shape);
// @src 140130920  LLFNC record of function fn (null when fn < 1 or the record is empty)
double* func_record(std::int32_t* iifnc, double* llfnc, std::int32_t fn);
// @src 140131ea0  1-D table rows (x, ncomp values) into f (shape already set)
void read_table_rows_1d(DYNARY* f, double* rows);
// @src 140134b00  type 0x79/0x7a/0x7b over time (var[0] == 1): axis[0] becomes the speed of each
//                 segment (path length / time), var[0] = 0x1a; true when converted
bool dynary_time_axis_to_speed(DYNARY* f);
// @src 140135dd0  type 0x7a, ncomp 4 -> type 0x7b: rotate (x, y, z) by mat, keep (r0, r1, w)
bool dynary_rotate_7a(DYNARY* f, double* mat);
// @src 140135fd0  the same about the point (cx, cy, cz)
bool dynary_rotate_7a_about(DYNARY* f, double* mat, double cx, double cy, double cz);
// @src 14012fd30  dynary_init(f, type), n[0] = n0, dynary_alloc
void dynary_init_n0(DYNARY* f, std::int32_t type, std::int32_t n0);
// @src 140194e30  inverse of the movement frame (140194d40, 140241860); returns 140241860's code
std::int32_t movement_frame_inverse(void* mv, double* inv);

// ---- 3x3 matrices --------------------------------------------------------------------------------
// @src 140241390  determinant (row major)
double mat3_det(double* a);
// @src 140241470  inverse; false when the determinant is 0
bool mat3_inverse(double* a, double* inv);
// @src 1402416d0  out = m * v
void mat3_mul_vec(double* v, double* out, double* m);
// @src 1402417d0  zero 9 values
void mat3_zero(double* a);
// @src 140241860  1 = 3x3 inverse; 2 = inverse of the first non-singular 2x2 minor; 0 = none
std::int32_t mat3_inverse_or_2d(double* a, double* inv);

// ---- Cls_140258580 (body-force record, list at Cls_1401860a0 + 0x200) ---------------------------
// @src 1402589a0  record from 12 LLFNC values (type at rec[6])
void body_force_from_values(void* r, double* v);
// @src 140258ed0  +0x08 = 3 values
void body_force_set_08(void* r, const double* v);
// @src 140258f50  +0x20 = 3 values
void body_force_set_20(void* r, const double* v);
// @src 140258fd0  +0x68 = 3 values, normalised
void body_force_set_68(void* r, const double* v);
// @src 140259060  +0x80 = 3 values, normalised
void body_force_set_80(void* r, const double* v);
// @src 1402590f0  +0x98 = 3 values
void body_force_set_98(void* r, const double* v);

// ---- 2D geometry (GEO2D_AND_MESH2D / OBJ_GEO2D) -----------------------------------------------
// @src 14027e700  curve type 2: drop rows whose segment type is not 1 or 2; flag28 = 1; true when
//                 any was dropped
bool curve_drop_bad_segments(model::OBJ_GEO2D* c);
// @src 14027fe20  edge count = *n: new edges appended, or edges n.. removed (140280290); b8 = 1
void geo2d_set_edge_count(model::GEO2D_AND_MESH2D* g, const std::int32_t* n);
// @src 140280290  remove the edges at the given 0-based positions (their curves deleted); renumber
bool geo2d_remove_edges(model::GEO2D_AND_MESH2D* g, std::int32_t* idx, const std::int32_t* n);

// ---- 3D surfaces (OBJ_GEO3D) -------------------------------------------------------------------
// @src 1402f8850  die point set from an LLFNC record
void die_point_set_from_values(model::geo_kw::DiePointSet* s, double* v);
// @src 1402f8de0  die edge set from an LLFNC record
void die_edge_set_from_values(model::geo_kw::DieEdgeSet* s, double* v);
// @src 1402f9af0  element ids = 0
void geo3d_zero_elem_ids(void* geo3d);

// ---- text fields -------------------------------------------------------------------------------
// @src 140324020  byte-swap a text field stored in doubles (n doubles) for platform codes 0..3 /
//                 100 / 200 / 300 (after + 100 * 140324000()); keep_order == 0 also swaps the
//                 two 4-byte halves of each double
void text_byteswap(void* s, std::int32_t platform, std::int32_t n, std::int32_t keep_order);
// @src 140323e50  reverse the bytes of n 4-byte words
void swap_words(void* s, std::int32_t n);
// @src 140323ec0  swap the two 4-byte halves of n 8-byte words
void swap_halves(void* s, std::int32_t n);
// @src 140323de0  reverse 4 bytes
void swap4(void* p);

// ---- the fun_<addr> names other helpers of the packet declared (pending_w2/w3/w4.hpp, lead) --------
// Thin forwards to the functions above (src/dbread/w1_wiring.cpp).
void fun_1401d2230(model::DB_STEP_DATA* m);
void fun_1401309b0(model::DYNARY* f, const std::int32_t* iifnc, const double* llfnc, std::int32_t fid,
                   const char* flag);
void fun_1401309b0(model::DYNARY* f, std::int32_t* iifnc, double* llfnc, std::int32_t fnum, char* flag);
void fun_140131ea0(model::DYNARY* f, const double* data);
void fun_140131ea0(model::DYNARY* f, double* data);
void fun_14025de80(model::LLWINDEF_DATA* w, double* rec, double* llwrz, std::int32_t* iifnc, double* llfnc);
void fun_140324020(void* s, std::int32_t platform, std::int32_t nwords, std::int32_t ints_only);
void fun_140324020(char* s, std::int32_t platform, std::int32_t n, std::int32_t flag);

} // namespace def::dbio::dbread
