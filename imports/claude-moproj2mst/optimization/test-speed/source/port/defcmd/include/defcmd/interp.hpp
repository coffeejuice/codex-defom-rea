// defcmd/interp.hpp: the data-interpolation half of remeshing (packet p8-08): the _MeshGenBase
// drivers of DEF_INT / DEF_REM3_GUI (DEFINT, REMESH) and DEF_FRZ / DEF_FRZ3 (DEFFRZ), their
// exchange files and the read-back of the result KEY, plus the model copy 1401d7390 used by the
// read-back into a temporary model (also called by CNVT3D, p9-01).
// Specs: docs/re/commands/DEFINT.md, DEFFRZ.md. Decompile deform-rea-work/out/p8mg/p8-08/all.c.
#pragma once

#include <cstdint>

#include <defcmd/meshgen/meshgen_base.hpp>

namespace def::model {
class DB_STEP_DATA;
class Cls_1401e7b10;
class MaterialList;
class PhasePairList;
class Material;
class PhasePair;
class Cls_14017ef20;
class Cls_1403753f0;
}

namespace def::cmd {

// ---- _MeshGenBase interpolation members -----------------------------------------------------
// @src 140155c70  mg+0x3f40 = mode (1 DEF_INT, 2 force interpolation)
void interp_set_mode(MeshGenBase& mg, std::int32_t mode);
// @src 1401575a0  mg+0x4778 = n, mg+0x4780 = objs, DB name (140157610), mg+0x4b90 = step
void interp_set_sources(MeshGenBase& mg, std::int32_t n, model::LLOBJ_DATA** objs,
                        const char* db, std::int32_t step);
// @src 140157610  strncpy(mg+0x4790, db, 0x400), mg+0x4b8f = 0, strcpy(mg+0x4360, mg+0x4790)
void interp_set_db_name(MeshGenBase& mg, const char* db);
// @src 140157670  mg+0x4788 = value
void interp_set_value(MeshGenBase& mg, double value);
// @src 1401576a0  _objectInterpolation: mg+0x4bd0 = flag; NSTART saved/restored around the
// mode 0/1 (140157780) or mode 2 (1401577e0) routine
bool object_interpolation(MeshGenBase& mg, bool flag);

// 2D DEF_INT (DEFINT.md section 4): writes DEF_INT.ACT, DEF_REM.INI, <prob>.RMS, runs DEF_INT,
// reads back <db stem>_REM.KEY
bool interp_2d(MeshGenBase& mg, const char* db);                  // 140155c90
bool write_def_rem_ini(MeshGenBase& mg);                         // 1401562f0
bool write_mesh_2d(MeshGenBase& mg, const char* name);           // 1401563c0
bool read_back_2d(MeshGenBase& mg, const char* name);            // 140156640
// 3D DEF_REM3_GUI (fails on this PC like the original: 32-bit mpich.dll missing)
bool interp_3d(MeshGenBase& mg, const char* unused);             // 140156950
bool write_def_rem3_ini(MeshGenBase& mg);                        // 140156ba0
bool write_mesh_3d(MeshGenBase& mg, const char* name);           // 140156cd0
bool read_back_3d(MeshGenBase& mg, const char* name);            // 140157040
// force interpolation
void clear_node_forces(MeshGenBase& mg);                         // 1401573c0
bool frz_2d(MeshGenBase& mg);                                    // 140157890
bool frz_3d(MeshGenBase& mg);                                    // 140157be0

// @src 140180a40  node set: allocate (key 3) when missing, sync, return the slot +0x20 array
double* node_set_array3(model::Cls_14017ef20* set);

// ---- model copy (1401d7390 family) ------------------------------------------------------------
// @src 1401d7390  copy simulation control, materials, phase pairs of src into dst, then
// 1401d6450 and 1401d0fc0(dst, 0)
void model_copy_data(model::DB_STEP_DATA* dst, const model::DB_STEP_DATA* src);
// @src 1401d7320  copy the 0x4f ints of m+0x15f8 into out (when out != null)
void model_copy_layout_table(const model::DB_STEP_DATA* m, std::int32_t* out);
// @src 1401f3660  simulation control copy (1401e8040 .. 1401eaf80)
void sim_ctrl_copy(model::Cls_1401e7b10* dst, const model::Cls_1401e7b10* src);
// Parts of 1401f3660 that CNVT3D (p9-01, 1401f36f0) calls on their own; bodies in
// src/p8_08/model_copy.cpp (names, run ids, TRANS/STNOUT/ELMNOD; NSTART..NSTEP; environment)
void sim_ctrl_copy_names(model::Cls_1401e7b10* dst, const model::Cls_1401e7b10* src);   // 1401e8040
void sim_ctrl_copy_steps(model::Cls_1401e7b10* dst, const model::Cls_1401e7b10* src);   // 1401e84f0
void sim_ctrl_copy_environment(model::Cls_1401e7b10* dst, model::Cls_1401e7b10* src);   // 1401ea900
// @src 14029b630  material list copy
void material_list_copy(model::MaterialList* dst, model::MaterialList* src);
// @src 140339d30  phase-pair list copy
void phase_pair_list_copy(model::PhasePairList* dst, model::PhasePairList* src);
// The single-record copies inside those lists, also called by the KEY import (packet MDT); bodies
// in src/p8_08/model_copy.cpp
void material_record_copy(model::Material* dst, const model::Material* src);        // 14028d370
void phase_pair_record_copy(model::PhasePair* dst, const model::PhasePair* src);    // 1403357a0
// The furnace model copy inside 1401ea900, also called by the GUI's furnace properties (packet
// MDT L2, GUI twin 0x1409f0230); body in src/p8_08/model_copy.cpp
void furnace_model_copy(model::Cls_1403753f0* dst, model::Cls_1403753f0* src);       // 140375550

} // namespace def::cmd
