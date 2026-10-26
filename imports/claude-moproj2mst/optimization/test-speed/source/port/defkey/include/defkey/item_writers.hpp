// defkey/item_writers.hpp: the single-item KEY writers (one material, one equipment) and the
// model UNIT setter that the DEF_GUI_MO_64 operation loader calls as byte-identical twins
// (docs/re/moproj2mst/mdt/L0/spec_A.md, spec_C.md). None of these addresses is in the DEF_PRE
// batch ledger; status rows in docs/re/status/mdt-model.tsv. Bodies in src/item_writers/.
// Decompile: deform-rea-work/out/mdt3/f.c.
#pragma once

#include <cstdint>
#include <cstdio>

namespace def::model {
class DB_STEP_DATA;
class LLOBJ_DATA;
class Material;
} // namespace def::model

namespace def::key {

// 1401c5ea0: write material `no` of the model as a material KEY file `name` (text mode, "wt"):
// optional comment block (`comment`: "*" then the text with "\n" as "\n*", then "\n\n"), the
// banner "*\n*  DEFORM MATERIAL KEYWORD FILE\n*\n", with `write_unit` the keyword 0x1e line with
// the int at model+0x84c, the material block (1403cdca0, `name_override` passed on); for a
// mixture (+0x1cc8 == 1) also the block of every listed material and every phase pair whose
// materials are both in the mixture (140339ba0, 1403cbf00). Returns 1 (no name, or the file
// does not open), 3 (no such material), or the first non-zero writer status, else 0.
int write_material_key_file(model::DB_STEP_DATA& m, const char* name, std::int32_t no,
                            bool write_unit, const char* comment, const char* name_override);
// Caller must resolve the material before opening, as GUI1408959e0 does.
int write_material_key_stream(model::DB_STEP_DATA& m, std::FILE* f, model::Material* mat,
                              bool write_unit, const char* comment, const char* name_override);

// 1401c6d00: write object `no` as an equipment KEY file `name` ("wt"): file banner (1401c5a20,
// dim_option 0), the movement block (1402a7c80), the keyword 0x97 block of the object whose
// number is at obj+0x2360 when that is > 0 (1402a83c0), then 1403d6690. Returns 3 when the file
// does not open, else the status of 1403d6690 (0). Like the original it does not check that the
// object exists.
int write_equipment_key_file(model::DB_STEP_DATA& m, const char* name, std::int32_t no);
int write_equipment_key_stream(model::DB_STEP_DATA& m, std::FILE* f, std::int32_t no);

// 1402a83c0: the keyword 0x97 lines of `obj` (movement type +0x21c8 must be 2, else 3; sub-mode
// +0x236c 0 or 1; mode 1 needs +0x2378 == 5, else 3, and appends the table +0x2368).
int write_kw97_block(model::DB_STEP_DATA& m, std::FILE* f, const model::LLOBJ_DATA* obj);

// 1403d6690: keyword 0x2d (movement types 3, 4, 7..10) and 0x2c (types 7..10) with the model's
// doubles +0x900.. and +0x8e8.. (two values in 2D, three in 3D). Returns 0.
int write_kw2d_2c(model::DB_STEP_DATA& m, std::FILE* f, const model::LLOBJ_DATA* obj);

// ---- task KEY files of "save current operation" (packet S, docs/re/moproj2mst/s/spec.md 2) -----
// Every one returns 1 for a null/empty name or a failed open ("wt"), else the block's status.

// 1401c5de0: SimCtrl.KEY; m+0x1744 = *flags (the GUI passes 0xf), banner, group 1 (1403d4990).
int write_simctrl_key_file(model::DB_STEP_DATA& m, const char* name, const std::int32_t* flags);
// Body after a successful open; caller owns the stream and ignores Close status.
// GUI twin14089597f..1408959bc. Enables the converter's native CRT file policy.
int write_simctrl_key_stream(model::DB_STEP_DATA& m, std::FILE* f, const std::int32_t* flags);
// 1401c64f0: InterObject.KEY; banner, inter-object blocks (1403c9ad0).
int write_interobject_key_file(model::DB_STEP_DATA& m, const char* name);
int write_interobject_key_stream(model::DB_STEP_DATA& m, std::FILE* f);
// 1401c63f0: object `no` as a KEY file, printed as `out_no` (0 = `no`); m+0x1744 = *flags (the GUI
// passes 0x3000); 14029e250 with its own banner.
int write_object_key_file(model::DB_STEP_DATA& m, const char* name, std::int32_t no,
                          const std::int32_t* flags, std::int32_t out_no);
int write_object_key_stream(model::DB_STEP_DATA& m, std::FILE* f, std::int32_t no,
                            const std::int32_t* flags, std::int32_t out_no);
// 1401c6820: MeshSettingsNNNNN.KEY; banner, 1402a9180; 3 without object `no`.
int write_meshsettings_key_file(model::DB_STEP_DATA& m, const char* name, std::int32_t no);
int write_meshsettings_key_stream(model::DB_STEP_DATA& m, std::FILE* f, std::int32_t no);
// 1402a9c80: CRDSYS, GEO23, GEOSEC, MSHSEC of an object (its GEO23 block); 0.
int write_geo23_block(model::DB_STEP_DATA& m, std::FILE* f, model::LLOBJ_DATA* obj, std::uint32_t no);
// 1401c6920: GeoConvt23DataNNNNN.KEY; banner, 1402a9c80 of object `no` (3 without it).
int write_geoconvt_key_file(model::DB_STEP_DATA& m, const char* name, std::int32_t no);
// 1401c6740: ObjPropertiesNNNNN.KEY; banner, 1402abe30.
int write_objproperties_key_file(model::DB_STEP_DATA& m, const char* name, std::int32_t no);
int write_objproperties_key_stream(model::DB_STEP_DATA& m, std::FILE* f, std::int32_t no);
// 1402abe30: OTPRNG line and OPSTOP list of an object; 0.
int write_objproperties_block(model::DB_STEP_DATA& m, std::FILE* f, model::LLOBJ_DATA* obj,
                              std::uint32_t no);
// 1401c6b00: RzElmConNNNNN.KEY; banner, RZ and ELMCON of a meshed object (kind != 7), else 3.
int write_rzelmcon_key_file(model::DB_STEP_DATA& m, const char* name, std::int32_t no);
// 1401c6c10: BccDefNNNNN.KEY; banner, 1402abe90 of a meshed object (kind != 7), else 3.
int write_bccdef_key_file(model::DB_STEP_DATA& m, const char* name, std::int32_t no);
// 1402abe90: EC* (1402abf20) and LOC* (1402ac560) blocks; `no` < 1 = obj+8; 0.
int write_bccdef_block(model::DB_STEP_DATA& m, std::FILE* f, model::LLOBJ_DATA* obj, std::int32_t no);

// 1401c8f70: the model's UNIT (simulation control +0xfc) through 1401f40b0: when it changes and
// `reset` is set, the unit-dependent defaults are reset (1401f4100).
void set_model_unit(model::DB_STEP_DATA& m, std::int32_t unit, bool reset);

} // namespace def::key
