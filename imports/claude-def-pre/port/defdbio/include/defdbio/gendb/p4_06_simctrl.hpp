// defdbio/gendb/p4_06_simctrl.hpp: GENDB writer of the simulation control block (packet p4-06,
// group simctrl): S = DB_STEP_DATA+0x750 (Cls_1401e7b10) into IISIM / LLSIM / IIUVL, with its
// functions allocated in IIFNC / LLFNC (FNCLOC), the furnace record, the self-describing
// FUNCTION_DATA writer (also used by the material and inter-object writers of p4-06) and the
// DB_WRITE_VAR count getters the writer reads.
//
// Word layout: docs/re/dbmap/simctrl.md (scratch copy dbmap/simctrl.md); the decompile and the
// listing are authoritative.
#pragma once

#include <cstdint>

#include <defmodel/layout/cls_1401e7b10.hpp>
#include <defmodel/layout/cls_1403753f0.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/dynary.hpp>
#include <defmodel/step/db_step.hpp>

namespace def::dbio::gendb {

// ---- DB_WRITE_VAR count getters (DB_STEP_DATA base at 0) --------------------------------------

// @src 1401f45b0  +0x68 (IISIM 3D [3] / 2D [5])
inline std::int32_t dbw_u068(const model::DB_STEP_DATA* m) { return model::field<std::int32_t>(m, 0x68); }
// @src 1401f45c0  +0xbc number of inter-material relations
inline std::int32_t dbw_u0bc(const model::DB_STEP_DATA* m) { return model::field<std::int32_t>(m, 0xbc); }
// @src 1401f45e0  +0xc0 number of windows
inline std::int32_t dbw_u0c0(const model::DB_STEP_DATA* m) { return model::field<std::int32_t>(m, 0xc0); }
// @src 1401f4600  +0x74
inline std::int32_t dbw_u074(const model::DB_STEP_DATA* m) { return model::field<std::int32_t>(m, 0x74); }
// @src 1401f4620  +0x84
inline std::int32_t dbw_u084(const model::DB_STEP_DATA* m) { return model::field<std::int32_t>(m, 0x84); }
// @src 1401f4640  +0x88 NFN (number of functions in IIFNC)
inline std::int32_t dbw_u088(const model::DB_STEP_DATA* m) { return model::field<std::int32_t>(m, 0x88); }
// @src 1401f4660  +0x8c LLFNC length
inline std::int32_t dbw_u08c(const model::DB_STEP_DATA* m) { return model::field<std::int32_t>(m, 0x8c); }
// @src 1401f4680  +0x90
inline std::int32_t dbw_u090(const model::DB_STEP_DATA* m) { return model::field<std::int32_t>(m, 0x90); }
// @src 1401f46a0  +0x94
inline std::int32_t dbw_u094(const model::DB_STEP_DATA* m) { return model::field<std::int32_t>(m, 0x94); }
// @src 1401f46c0  +0x9c
inline std::int32_t dbw_u09c(const model::DB_STEP_DATA* m) { return model::field<std::int32_t>(m, 0x9c); }

// ---- writers ----------------------------------------------------------------------------------

// 140131600: self-describing FUNCTION_DATA record: word 0 = 10.0, word 1 = ndim, shape words
// 2..6 by ndim, data from word 10 (ndim 1: 1401317c0, ndim 2: 140131360). Null out: nothing.
void write_func_selfdesc(const model::DYNARY* f, double* out); // 140131600

// 14021e170: sqrt(v0^2 + v1^2 (+ v2^2 in 3D)) of the doubles at +0x170/+0x178/+0x180 of an
// OMV_DATA (dimension from its model pointer at +0xa80).
double omv_magnitude(const void* omv); // 14021e170

// 1401f3bb0: omv_magnitude of the movement block (+0x2160) of the object with id PDIE 1, or 0.
double sim_pdie_magnitude(model::Cls_1401e7b10* s); // 1401f3bb0

// 140375bb0: furnace record (20 words: 20.0, name 64 chars, first/last zone fid in [9], [10])
// at a new function id stored in *fid, then one self-describing function per zone curve
// (IIFNC attribute = zone number).
void write_furnace_to_db(model::Cls_1403753f0* f, model::DB_STEP_DATA* m, std::int32_t* iifnc, double* llfnc,
                         double* fid); // 140375bb0

// 1401effc0: simulation control into IISIM (int words), LLSIM (real words), IIUVL (10 x 80
// characters of USRDEF), allocating its functions in IIFNC / LLFNC. Stores the clamped DTSUB
// back into S and (3D) copies counts into the model.
void write_sim_ctrl_to_db(model::Cls_1401e7b10* s, std::int32_t* iisim, double* llsim, std::int32_t* iiuvl,
                          std::int32_t* iifnc, double* llfnc); // 1401effc0

} // namespace def::dbio::gendb
