// defdbio/gendb/p4_06_frame.hpp: the DB_STEP_DATA-level GENDB steps of packet p4-06, called by
// FUN_1401bcbb0 (p4-01) in this order: sim control 1401c4950, materials 1401c49b0, objects
// 1401c4d90 (with the UNNAME / UENAME blocks 1401c4a30 / 1401c4be0), inter-objects 1401c4ea0,
// inter-materials 1401c4fa0. Region pointers are those 1401bcbb0 passes (start of each region in
// the GENDB buffers m+0x50 / m+0x58); widths come from the A address table m+0x1dc0.
#pragma once

#include <cstdint>

#include <defmodel/layout/db_step_data.hpp>

namespace def::dbio::gendb {

// @src 1401c4950  sim control: IISIM, LLSIM, LLCPU (unused), LLFNC, IIFNC, IIUVL
void write_step_sim_ctrl(model::DB_STEP_DATA* m, std::int32_t* iisim, double* llsim, double* llcpu, double* llfnc,
                         std::int32_t* iifnc, std::int32_t* iiuvl);

// @src 1401c49b0  materials: LLMPR, LLFNC, IIFNC (width of LLMPR from the A table)
void write_step_materials(model::DB_STEP_DATA* m, double* llmpr, double* llfnc, std::int32_t* iifnc);

// @src 1401c4a30  UNNAME block: one 64-character blank-padded name per user node variable, in A
//                 at LLND1 start - 1 + NUMNP * count (3D) or at A slot 57's start (2D)
void write_step_node_var_names(model::DB_STEP_DATA* m);

// @src 1401c4be0  UENAME block: the same for user element variables (LLET1 / NUMEL; 2D slot 56)
void write_step_elem_var_names(model::DB_STEP_DATA* m);

// @src 1401c4d90  objects: 2D 14017cb60 or 3D 14017b370 (p4-05), then the two name blocks.
//                 MAR, A, MAR table, A table, LLFNC, IIFNC, LLWIN, LLWRZ
void write_step_objects(model::DB_STEP_DATA* m, std::int32_t* mar, double* a, std::int32_t* mt, std::int32_t* at,
                        double* llfnc, std::int32_t* iifnc, double* llwin, double* llwrz);

// @src 1401c4ea0  inter-objects: LLBRT, LLFNC, IIFNC, LLWIN, LLWRZ, LLBND
void write_step_inter_objects(model::DB_STEP_DATA* m, double* llbrt, double* llfnc, std::int32_t* iifnc,
                              double* llwin, double* llwrz, double* llbnd);

// @src 1401c4fa0  inter-materials: LLTFM, LLFNC, IIFNC (width of LLTFM from the A table)
void write_step_inter_materials(model::DB_STEP_DATA* m, double* lltfm, double* llfnc, std::int32_t* iifnc);

} // namespace def::dbio::gendb
