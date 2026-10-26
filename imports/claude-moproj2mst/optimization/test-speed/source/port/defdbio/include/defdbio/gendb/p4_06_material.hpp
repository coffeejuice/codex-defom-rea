// defdbio/gendb/p4_06_material.hpp: GENDB writers of the materials (LLMPR records) and the
// inter-material / phase-transformation pairs (LLTFM records), packet p4-06.
//
// All record words are doubles. Functions are allocated through FNCLOC (ext::f1401bb3d0) in the
// binary's call order, so function numbers follow the order of the code below. Material
// functions get the material's DB slot as IIFNC owner word, inter-material functions get 0.
//
// Known behaviour of the original that is reproduced on purpose (not fixed):
//   * GRNDAT mode 1: the 9th table's code chooses 2 from the 8th table's variable (+0x13e0)
//     instead of its own (+0x1458) when its own variable is not 3.
//   * MASDEN as a 2-D table writes its n1 and n2 both into word 1, overwriting MATDEN.
//   * FSTRES 13/15 and CREEP 7/8 copy n of their first table into the int before the table
//     (Material +0x478 / +0xda4): the writer modifies the material.
#pragma once

#include <cstdint>

#include <defmodel/layout/cls_14028a4c0.hpp>
#include <defmodel/layout/cls_14028b850.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/dynary.hpp>
#include <defmodel/layout/material.hpp>
#include <defmodel/layout/material_list.hpp>
#include <defmodel/layout/phase_pair.hpp>
#include <defmodel/layout/phase_pair_list.hpp>

namespace def::dbio::gendb {

// 14029bdf0: writes one LLMPR record per used material, for material numbers 1..max in
// increasing order, at llmpr + (slot - 1) * *width (slot = Material+4). Builds the number -> slot
// table (14029ba10) passed to the record writer for the MSTMTR list.
void write_material_list(const model::MaterialList* list, double* llmpr, std::int32_t* iifnc, double* llfnc,
                         const std::int32_t* width);

// 140294ae0: one LLMPR record. *slot is the IIFNC owner word of the material's functions;
// slot_table[number - 1] is the dense slot of material `number` (MSTMTR list).
void write_material_record(model::Material* mat, double* rec, std::int32_t* iifnc, double* llfnc,
                           const std::int32_t* slot, const std::int32_t* slot_table);

// 14029b0d0: the common property pattern P(fn, const, type, n1, n2): word indices into rec.
// nvar 0: rec[fn] = 0, rec[const] = value; nvar 1: rec[type] = 0 / 1 / 2 for var 3 / 9 / 0xc;
// nvar 2: rec[type] = 3, rec[n1], rec[n2]; any nvar != 0 then allocates the function (number in
// rec[fn], owner *attr) and writes it with the plain layout (140131360).
void write_material_property(const model::Material* mat, double* rec, std::int32_t* iifnc, double* llfnc,
                             const model::DYNARY* f, const std::int32_t* w_fn, const std::int32_t* w_const,
                             const std::int32_t* w_type, const std::int32_t* w_n1, const std::int32_t* w_n2,
                             const std::int32_t* attr);

// 14028e030: FRCMOD words out[0] = code, out[1..5] = the five reals of 14028e140; all six 0.0
// and false for an unknown friction type.
bool material_frcmod_words(const model::Material* mat, double* out);

// 14028c340: the nine reals of FRCMOD code 11; frc = the FRCMOD block (Material+0x1ad0).
void material_frcmod_code11_reals(const void* frc, double* out);

// 14028aa70: one GRNDAT mesoscale grain record into LLFNC (length 14028a650).
void write_grain_record(const model::Cls_14028a4c0* rec, double* out);

// 14028acb0: one sub-table of a grain record: *code (0 const, 1 var 3, 2 var 0x17, 3 (3, 0x17)),
// *n1 (or the constant), *n2, and the table's plain layout at out. rec is not read.
void write_grain_subtable(const model::Cls_14028a4c0* rec, double* code, double* n1, double* n2, double* out,
                          const model::DYNARY* f);

// 14028b4a0: one GRNDAT mode-2 record list: *count_word = count; if count > 0, a list function
// (number in *fn_word) holding one function number per record, each a grain record function.
void write_grain_record_list(const model::Cls_14028b640::RecordList* list, model::DB_STEP_DATA* m,
                             const std::int32_t* attr, std::int32_t* iifnc, double* llfnc, double* count_word,
                             double* fn_word);

// 14033a5a0: one LLTFM record per ordered pair (from, to) of used materials that has a pair
// object, at lltfm + *width * ((slot_to - 1) * NMAT - 1 + slot_from). *counter of 140337f50
// starts at 0 for each call.
void write_phase_pair_list(const model::PhasePairList* list, double* lltfm, std::int32_t* iifnc, double* llfnc,
                           const std::int32_t* width);

// 140337f50: one LLTFM record (nothing past word 0 when the pair is not active). *counter counts
// pairs with PHASTF type 1 or 2 (word 30).
void write_phase_pair_record(const model::PhasePair* pp, double* rec, std::int32_t* iifnc, double* llfnc,
                             std::int32_t* counter);

} // namespace def::dbio::gendb
