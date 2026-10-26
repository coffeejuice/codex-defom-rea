// defdbio/gendb/counts_frame.hpp: the GENDB counts frame (packet p4-02b).
//
// FUN_1401bc230 fills the DB_WRITE_VAR counters of a model before GENDB sizes the MAR/A regions
// (docs/re/dbmap/stepdriver.md section 5.4): number of objects, nodes, elements, boundary data,
// materials (MSTMTR), inter-material relations (PHASTF), material groups, the name-array widths
// and the property-function counts (NFN = DB_WRITE_VAR+0x88, LLFNC length = +0x8c) of the
// simulation control, the materials, the phase pairs, the objects (p4-02a) and the inter-object
// records.
//
// Every function keeps the binary's argument passing (pointers to the counters, Fortran style).
// "n" is the function counter (NFN) and "len" the LLFNC length the counted functions add to.
// Calls into functions of other packets that are not on this branch go through gendb_ext()
// (counts_ext.hpp).
#pragma once

#include <cstdint>

#include <defmodel/layout/cls_1401e7b10.hpp>
#include <defmodel/layout/cls_1401f46e0.hpp>
#include <defmodel/layout/cls_1401fa020.hpp>
#include <defmodel/layout/cls_1403753f0.hpp>
#include <defmodel/llobj/cls_14017e740.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/dynary.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/layout/material.hpp>
#include <defmodel/layout/material_list.hpp>
#include <defmodel/layout/obj_geo2d.hpp>
#include <defmodel/layout/phase_pair.hpp>
#include <defmodel/layout/phase_pair_list.hpp>

namespace def::dbio::gendb {

using model::Cls_14017e740;
using model::Cls_1401e7b10;
using model::Cls_1401f46e0;
using model::Cls_1401fa020;
using model::Cls_1403753f0;
using model::DB_STEP_DATA;
using model::DYNARY;
using model::LLOBJ_DATA;
using model::Material;
using model::MaterialList;
using model::OBJ_GEO2D;
using model::PhasePair;
using model::PhasePairList;

// ---- frame (DB_STEP_DATA methods) ----------------------------------------------------------
void counts_frame(DB_STEP_DATA* m, std::int32_t* err);                       // 1401bc230
void module_licence_flags(DB_STEP_DATA* m);                                  // 1401bd300
void object_counts_2d(DB_STEP_DATA* m, std::int32_t* nobj, std::int32_t* numnp,
                      std::int32_t* numel, std::int32_t* nbcrv, std::int32_t* nbseg,
                      std::int32_t* ntot, std::int32_t* nout, std::int32_t* ndof,
                      std::int32_t* ndof2, std::int32_t* err);               // 1401bdaf0
void object_counts_3d(DB_STEP_DATA* m, std::int32_t* nobj, std::int32_t* numnp,
                      std::int32_t* numel, std::int32_t* nconn, std::int32_t* nmesh,
                      std::int32_t* nbnd, std::int32_t* nbfac, std::int32_t* ngelm,
                      std::int32_t* ngnod, std::int32_t* ntot, std::int32_t* ndnp,
                      std::int32_t* ndel, std::int32_t* nrnp, std::int32_t* nrfac,
                      std::int32_t* nconn8, std::int32_t* err);              // 1401bd3d0
void material_count(DB_STEP_DATA* m, std::int32_t* nmat, std::int32_t* err);  // 1401be510
void relation_count(DB_STEP_DATA* m, std::int32_t* nrel, std::int32_t* err);  // 1401be660
void material_group_count(DB_STEP_DATA* m, std::int32_t* ngrp, std::int32_t* err); // 1401be700
void element_name_width(DB_STEP_DATA* m, std::int32_t* w, std::int32_t* err); // 1401be960
void node_name_width(DB_STEP_DATA* m, std::int32_t* w, std::int32_t* err);    // 1401be9d0
void sim_ctrl_function_counts(DB_STEP_DATA* m, std::int32_t* n, std::int32_t* len,
                              std::int32_t* err);                            // 1401bea40
void step_material_function_counts(DB_STEP_DATA* m, std::int32_t* n, std::int32_t* len,
                                   std::int32_t* err);                       // 1401bea90
void phase_pair_function_counts(DB_STEP_DATA* m, std::int32_t* n, std::int32_t* len,
                                std::int32_t* err);                          // 1401beae0
void inter_object_function_counts(DB_STEP_DATA* m, std::int32_t* n, std::int32_t* len,
                                  std::int32_t* nwin, std::int32_t* lwin,
                                  std::int32_t* err);                        // 1401beb90
std::int32_t max_object_number(DB_STEP_DATA* m);                            // 1401caf40
void set_u134(DB_STEP_DATA* m, const std::int32_t* v);                       // 14019a5c0
void set_u138(DB_STEP_DATA* m, const std::int32_t* v);                       // 14019a600
std::int32_t object_count_field(DB_STEP_DATA* m);                           // 1401f4610 (+0x7c)
void set_u0e4(DB_STEP_DATA* m, const std::int32_t* v);                       // 1401fb1e0

// Diagnostic hooks of the original: empty functions taking the message code, the keyword and
// two values (14001cd60 TRANS, 14001cd70 MSTMTR, 14001cd80 PHASTF, 14001cd90 the others).
void diag_14001cd60(const std::int32_t* code, const char* kw, const void* a, const void* b);
void diag_14001cd70(const std::int32_t* code, const char* kw, const void* a, const void* b);
void diag_14001cd80(const std::int32_t* code, const char* kw, const void* a, const void* b);
void diag_14001cd90(const std::int32_t* code, const char* kw, const void* a, const void* b);

// ---- objects (LLOBJ_DATA methods) ------------------------------------------------------------
bool object_mark_groups(LLOBJ_DATA* o, std::uint32_t flags, std::int32_t* bad);  // 14015ff30
bool object_group_list(LLOBJ_DATA* o, bool* single, std::int32_t* matno);         // 140160260
bool object_skip_mesh(LLOBJ_DATA* o);                                             // 140160c20
bool object_has_geometry(LLOBJ_DATA* o);                                          // 140160cc0
std::int32_t object_geo3d_nodes(LLOBJ_DATA* o);                                   // 140167d70
std::int32_t object_geo3d_elems(LLOBJ_DATA* o);                                   // 140167e00
bool object_mesh_range(LLOBJ_DATA* o, double* min, double* max);                  // 1401ff5b0
bool object_geo2d_range(LLOBJ_DATA* o, double* min, double* max);                 // 1401ff600
bool object_geo2d_usable(LLOBJ_DATA* o);                                          // 1402039d0
void boundary_node_list(Cls_14017e740* b, const std::int32_t* nnode);             // 14017e990
bool node_coord_range(void* nodes, double* min, double* max);                     // 140181ae0
std::int32_t geo2d_entity_count(OBJ_GEO2D* g);                                    // 140276c70
void dynary_set_int_list(DYNARY* t, std::int32_t n, const std::int32_t* list);    // 1401320a0

// ---- DYNARY lengths ---------------------------------------------------------------------------
std::int32_t dynary_length_3d_comp(const DYNARY* t);                     // 140130090
std::int32_t dynary_length_plus10(const DYNARY* t);                      // 140130380
void dynary_count_plus10(const DYNARY* t, std::int32_t* n, std::int32_t* len); // 1401304e0

// ---- materials (MaterialList / Material methods) --------------------------------------------
void material_list_clear_flags(MaterialList* l);                         // 14029b5c0
void material_list_function_counts(MaterialList* l, std::int32_t* n, std::int32_t* len,
                                   std::int32_t* err);                  // 14029b8e0
std::int32_t material_list_max_number(MaterialList* l);                  // 14029c410
std::int32_t material_list_mark_mixtures(MaterialList* l);               // 14029c490
std::int32_t material_list_renumber(MaterialList* l);                    // 14029c5c0
void material_function_counts(Material* mat, std::int32_t* n, std::int32_t* len,
                              std::int32_t* err);                        // 14028e4b0
void flow_stress_counts(void* fs, std::int32_t* n, std::int32_t* len);    // 1402894b0 (Material+0x50)
void diffusion_counts(std::int32_t* d, std::int32_t* n, std::int32_t* len);  // 140289a40 (+0x950)
void creep_counts(std::int32_t* c, std::int32_t* n, std::int32_t* len);      // 140289d80 (+0xac8)
void hardness_counts(std::int32_t* h, std::int32_t* n, std::int32_t* len);   // 14028a080 (+0x1e40)
std::int32_t grain_record_length(void* g);                               // 14028a650
void grain_record_counts(void* g, std::int32_t* n, std::int32_t* len);    // 14028a6f0
void grain_list_counts(void* l, std::int32_t* n, std::int32_t* len);      // 14028b2f0
std::int32_t grain_table_length(void* g);                                // 14028b990
void grain_counts(void* g, std::int32_t* n, std::int32_t* len);           // 14028bb60 (+0x1078)
void elastic_counts(void* e, std::int32_t* n, std::int32_t* len);         // 14028bd80 (+0x18f0)
void material_misc_counts(std::int32_t* p, std::int32_t* n, std::int32_t* len); // 14028c240 (+0x1ad0)

// ---- phase pairs (PhasePairList / PhasePair methods) ----------------------------------------
void phase_pair_counts(PhasePair* p, std::int32_t* n, std::int32_t* len);   // 140335de0
std::int32_t phase_pair_kinetics_length(PhasePair* p);                     // 140336300
bool phase_pair_list_remove(PhasePairList* l, PhasePair* p);               // 140339ec0
void phase_pair_list_prune(PhasePairList* l);                              // 14033a1d0
std::int32_t phase_pair_list_active(PhasePairList* l);                     // 14033a300
void phase_pair_list_function_counts(PhasePairList* l, std::int32_t* n, std::int32_t* len,
                                     std::int32_t* err);                  // 14033a370

// ---- simulation control ---------------------------------------------------------------------
void sim_ctrl_counts(Cls_1401e7b10* s, std::int32_t* n, std::int32_t* len);  // 1401ee650
std::int32_t sim_ctrl_smode(Cls_1401e7b10* s);                             // 1401f3f50
void furnace_counts(Cls_1403753f0* f, std::int32_t* n, std::int32_t* len);   // 140375dc0

// ---- inter-object records -------------------------------------------------------------------
void inter_object_list_counts(Cls_1401fa020* l, std::int32_t* n, std::int32_t* len,
                              std::int32_t* nwin, std::int32_t* lwin);    // 1401fa170
void inter_object_counts(Cls_1401f46e0* r, std::int32_t* n, std::int32_t* len,
                         std::int32_t* nwin, std::int32_t* lwin, std::int32_t* any); // 1401f49c0
std::int32_t inter_object_window_count(Cls_1401f46e0* r);                   // 1401f98d0

} // namespace def::dbio::gendb
