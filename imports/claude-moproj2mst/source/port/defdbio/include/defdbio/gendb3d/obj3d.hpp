// defdbio/gendb3d/obj3d.hpp: packet p4-05, GENDB object arrays for 3D (FUN_14017b370 and the
// writers it calls). docs/phase4-packets.md, mapping: docs/re/dbmap/objarrays.md and object.md
// (branch claude/project-thread-bz5k5e).
//
// Argument conventions (as in the binary, Fortran style: scalars the callee may change are passed
// by pointer):
//   mar / a          the GENDB MAR (int32) and A (double) buffers, 0-based C pointers
//   mar_tab / a_tab  the address tables: mar_tab[0..69] region starts (1-based), [70..139] widths;
//                    a_tab[0..139] starts, [140..279] widths. Indexed through def::model::g_mar_index /
//                    g_a_index (DAT_1406e81fc / DAT_1406e82d0), filled by 1401c7d40.
//   iiobj / llobj    the object's row in IIOBJ (MAR) / LLOBJ (A)
//   iifnc / llfnc    the function tables (IIFNC int pairs, LLFNC doubles)
//   llwin / llwrz    window rows (LLWIN, row width win_w) and window coordinates (LLWRZ)
//   m                DB_STEP_DATA (its DB_WRITE_VAR base holds the GENDB running counters)
#pragma once

#include <cstdint>

namespace def::model {
class DB_STEP_DATA;
class LLOBJ_DATA;
class Cls_14017a6c0;
class DYNARY;
} // namespace def::model

namespace def::dbio::g3d {

using def::model::DB_STEP_DATA;
using def::model::DYNARY;
using def::model::LLOBJ_DATA;

// ---- driver --------------------------------------------------------------------------------
// @src 14017b370  3D object arrays: every object of the list into MAR/A, then PDIE, BCCDEF
//                 contact conversion and MTLGRP renumbering
void write_objects_3d(model::Cls_14017a6c0* objs, std::int32_t* mar, double* a,
                      const std::int32_t* mar_tab, const std::int32_t* a_tab, std::int32_t* iifnc,
                      double* llfnc, double* llwin, double* llwrz);
// @src 14017b230  read CHK_VOL.INI (target volume, object, compensation flag)
void read_chk_vol_ini(void* unused, double* volume, std::uint32_t* object, std::uint32_t* flag);

// ---- GENDB counters of DB_WRITE_VAR (getters / setters) --------------------------------------
std::int32_t numnp(const DB_STEP_DATA* m);                 // 140199da0  +0x6c
std::int32_t numel(const DB_STEP_DATA* m);                 // 140199db0  +0x70
std::int32_t n_boundaries(const DB_STEP_DATA* m);          // 14019a350  +0x80
std::int32_t n_objects_meshed(const DB_STEP_DATA* m);      // 14019a200  +0x98
std::int32_t fold_flag(const DB_STEP_DATA* m);             // 14019a2f0  +0xcc
std::int32_t fold_node_offset(const DB_STEP_DATA* m);      // 140199d60  +0xd0
std::int32_t fold_face_offset(const DB_STEP_DATA* m);      // 140199d80  +0xd4
std::int32_t record_counter(const DB_STEP_DATA* m);        // 14019a220  +0x110
std::int32_t node_offset(const DB_STEP_DATA* m);           // 140199ce0  +0x114
std::int32_t elem_offset(const DB_STEP_DATA* m);           // 140199d20  +0x118
std::int32_t bnd_node_offset(const DB_STEP_DATA* m);       // 14019a0c0  +0x11c
std::int32_t geo_point_offset(const DB_STEP_DATA* m);      // 14019a100  +0x124
std::int32_t geo_facet_offset(const DB_STEP_DATA* m);      // 14019a140  +0x128
std::int32_t geo_facet_id_offset(const DB_STEP_DATA* m);   // 14019a180  +0x12c
std::int32_t face_offset(const DB_STEP_DATA* m);           // 14019a1c0  +0x130
std::int32_t window_row(const DB_STEP_DATA* m);            // 14019a5a0  +0x134
std::int32_t window_pos(const DB_STEP_DATA* m);            // 14019a5e0  +0x138
void set_fold_node_offset(DB_STEP_DATA* m, const std::int32_t* v);    // 14019a310
void set_fold_face_offset(DB_STEP_DATA* m, const std::int32_t* v);    // 14019a330
void set_record_counter(DB_STEP_DATA* m, const std::int32_t* v);      // 14019a2d0
void set_node_offset(DB_STEP_DATA* m, const std::int32_t* v);         // 140199d00
void set_elem_offset(DB_STEP_DATA* m, const std::int32_t* v);         // 140199d40
void set_bnd_node_offset(DB_STEP_DATA* m, const std::int32_t* v);     // 14019a0e0
void set_geo_point_offset(DB_STEP_DATA* m, const std::int32_t* v);    // 14019a120
void set_geo_facet_offset(DB_STEP_DATA* m, const std::int32_t* v);    // 14019a160
void set_geo_facet_id_offset(DB_STEP_DATA* m, const std::int32_t* v); // 14019a1a0
void set_face_offset(DB_STEP_DATA* m, const std::int32_t* v);         // 14019a1e0

// ---- function table (IIFNC / LLFNC) ------------------------------------------------------
// @src 1401bb3d0  next global function number: m+0x10c += 1, *number = it, iifnc pair
//                 (id, LLFNC start), *pos = LLFNC start (1-based); warnings past the allocation
void alloc_function(DB_STEP_DATA* m, const std::int32_t* len, std::int32_t* iifnc,
                    const std::int32_t* id, double* number, std::int32_t* pos);
// @src 140131210  FUNCTION_DATA::WriteFuncToDBArray: type 0x7c values (map != 0: map[(int)v - 1])
void write_func_to_db_array(const DYNARY* f, double* out, const std::int32_t* map);
// @src 140131360  DYNARY values (constant / 1-D (x, y...) rows / axes then values)
void write_dynary_to_db_array(const DYNARY* f, double* out);
// @src 1401317c0  1-D table rows (x, y[ncomp]) of a DYNARY-shaped table
void write_table_1d_to_db_array(const DYNARY* f, double* out);
// @src 14029ba10  material number -> position table; returns the number of used materials
std::int32_t material_positions(void* materials, std::int32_t* table, const std::int32_t* n);

// ---- per-object writers (called by the driver) ----------------------------------------------
// @src 14016f8e0  WriteNodeInfoToDB (A node fields); node_off = running node offset
void write_node_info(LLOBJ_DATA* obj, std::int32_t* mar, double* a, const std::int32_t* mar_tab,
                     const std::int32_t* a_tab, const std::int32_t* node_off);
// @src 1401707e0  WriteNodeIntegerInfoToDB (BCCDEF, BCCDFN, BCCTMP, BCCTFN, BCCRHT, BCCCRB)
void write_node_int_info(LLOBJ_DATA* obj, std::int32_t* mar, const std::int32_t* mar_tab,
                         const std::int32_t* node_off, std::int32_t* fn_used, std::int32_t* sym_used,
                         const std::int32_t* fn_cap, const std::int32_t* sym_cap,
                         std::uint32_t* fn_kinds, std::int32_t* sym_any);
// @src 140172c90  WriteElemInfoToDB (A element fields)
void write_elem_info(LLOBJ_DATA* obj, std::int32_t* mar, double* a, const std::int32_t* mar_tab,
                     const std::int32_t* a_tab, const std::int32_t* elem_off,
                     const std::int32_t* node_off);
// @src 140174cd0  WriteElemIntegerInfoToDB (IINOD connectivity, IIMTR)
void write_elem_int_info(LLOBJ_DATA* obj, std::int32_t* mar, const std::int32_t* mar_tab,
                         const std::int32_t* conn_off, const std::int32_t* elem_off,
                         const std::int32_t* node_off);
// @src 1401774f0  WriteEdgeFaceInfoToDB (element-face BCs); face_off / n_faces as in the driver
void write_edge_face_info(LLOBJ_DATA* obj, std::int32_t* mar, double* a,
                          const std::int32_t* mar_tab, const std::int32_t* a_tab,
                          const std::int32_t* face_off, const std::int32_t* n_faces,
                          std::int32_t* fn_used, std::uint32_t* fn_kinds);
// @src 140178460  WriteBdryCndFuncToDB (local BC functions -> global IIFNC/LLFNC numbers)
void write_bc_functions(LLOBJ_DATA* obj, std::int32_t* mar, const std::int32_t* mar_tab,
                        const std::int32_t* node_off, const std::int32_t* face_off,
                        const std::int32_t* n_faces, std::int32_t* iifnc, double* llfnc,
                        std::int32_t* fn_used, std::int32_t* sym_used, std::uint32_t* fn_kinds,
                        std::int32_t* sym_any, std::uint32_t* face_kinds);
// @src 140175db0  WriteBdryInfoToDB (one LLBND record)
void write_boundary_info(LLOBJ_DATA* obj, const std::int32_t* rec_type, double* rec,
                         double* llgry, std::int32_t* iidod, std::int32_t* iifnc, double* llfnc,
                         std::int32_t* iibnd, const std::int32_t* rec_no);
// @src 14016cf10  WriteGeneralInfoToDB (IIOBJ / LLOBJ words); 10 arguments as in the binary
void write_general_info(LLOBJ_DATA* obj, std::int32_t* iiobj, double* llobj,
                        std::int32_t conn_first, std::int32_t rec_last, std::int32_t rec_n,
                        std::int32_t* iifnc, double* llfnc, std::int32_t* mat_pos,
                        const char* meshed);
// @src 14016d200  second general-info part (LLOBJ words 48/49: the object's curve u3598)
void write_general_info2(LLOBJ_DATA* obj, std::int32_t* iiobj, double* llobj,
                         std::int32_t conn_first, std::int32_t rec_last, std::int32_t rec_n,
                         std::int32_t* iifnc, double* llfnc, std::int32_t* mat_pos,
                         const char* meshed);
// @src 14016da70  fold meshes FLDGEO/FLWNET into LLFLD/IIFLD
void write_fold_info(LLOBJ_DATA* obj, std::int32_t* iiobj, double* llfld, std::int32_t* iifld,
                     std::int32_t* n_nodes, std::int32_t* n_faces);
// @src 14017e190  BCCDEF contact codes: -object -> -LLBND record (and PDIE/attach words)
void convert_contact_codes(model::Cls_14017a6c0* objs, double* llcrz, std::int32_t* iibcd,
                           double* llbnd, const std::int32_t* rec_w);
// @src 140166e60  2D: nearest boundary-loop node to (x, y); true when the object has loops
bool nearest_loop_node(LLOBJ_DATA* obj, double x, double y, std::int32_t* node,
                       std::int32_t* loop, double* px, double* py);
// @src 140165230  BC function capacity: max(NBF, nodes) + 1000
std::int32_t bc_function_capacity(LLOBJ_DATA* obj);
// 140183cb0  element set connectivity (region sync through the owner first); node_elem.cpp
std::int32_t* element_connectivity(void* elems);
// 140183d00  element set material group per element (region sync 2 first); node_elem.cpp, the one
//            owner (every other caller forwards here)
std::int32_t* element_material_groups(void* elems);
// @src 140203980  object geometry usable (2D 1402039d0 / 3D 140203a20)
bool object_has_geometry(LLOBJ_DATA* obj);
// 140258f10 / 140258f90  WPAXIS record (+0x200 list) vectors: if (dst) copy 3 doubles from r + 0x08
// / r + 0x20 (bclists.cpp)
void record_e8_get_08(const void* r, void* dst);
void record_e8_get_20(const void* r, void* dst);

// ---- volume fractions (VOLFC / VOLFS remap) -------------------------------------------------
// @src 140160640  normalise the object's VOLFC rows (width *w)
void normalize_volume_fractions(LLOBJ_DATA* obj, const std::int32_t* w);
// @src 1401607f0  normalise rows of a buffer of width *w
void normalize_volume_fraction_rows(LLOBJ_DATA* obj, double* rows, const std::int32_t* w);
// @src 140160980  remap phase columns (width *w) to the material count
std::uint64_t remap_volume_fractions(LLOBJ_DATA* obj, const double* src, double* dst,
                                     const std::int32_t* w);

// ---- sub-object writers (other files of this packet) --------------------------------------
// @src 1401912a0  LLBND movement / die words of the object (OMV_DATA, sub-object at obj+0x2160,
//                 Cls_14018b220); rec_type is not read; rec_no (write_boundary_info's last
//                 argument) is the IIFNC id of every function it allocates
void write_bnd_movement(void* mv, const std::int32_t* rec_type, double* rec, std::int32_t* iifnc,
                        double* llfnc, DB_STEP_DATA* m, const std::int32_t* rec_no);
// @src 140187a60  sub-object at obj+0x1ea0 to LLOBJ / functions
void write_obj_1ea0(void* s, std::int32_t* iiobj, double* llobj, std::int32_t* iifnc,
                    double* llfnc, DB_STEP_DATA* m, const std::int32_t* obj_no);
// @src 14018a760  sub-object at obj+0x2d10 (heat-exchange windows) to LLOBJ / LLWIN
void write_obj_2d10(void* s, std::int32_t* iiobj, double* llobj, std::int32_t* iifnc,
                    double* llfnc, double* llwin, std::int32_t win_w, double* llwrz,
                    const char* flag, DB_STEP_DATA* m, const std::int32_t* obj_no);
// @src 140196840  sub-object at obj+0x2e68 (BC lists) to LLOBJ / LLWIN
void write_obj_2e68(void* s, std::int32_t* iiobj, double* llobj, std::int32_t* iifnc,
                    double* llfnc, double* llwin, std::int32_t win_w, double* llwrz,
                    const char* f1, const char* f2, const char* f3, DB_STEP_DATA* m,
                    const std::int32_t* obj_no);

} // namespace def::dbio::g3d
