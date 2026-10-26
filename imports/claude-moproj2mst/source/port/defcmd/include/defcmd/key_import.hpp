// defcmd/key_import.hpp: the KEY import family next to the KEY loader 1401b5f00
// (defcmd/key_file.hpp). Each entry reads a keyword file into a scratch DB_STEP_DATA(3) with
// read_keyword_file(reset 1, flags 0) and copies one part of it into the model. DEF_GUI_MO_64 calls
// byte-identical twins when it loads an operation (docs/re/moproj2mst/mdt/L0/spec_A.md, spec_B.md:
// GUI 140897720 = 1401b6c80, 140897fd0 = 1401b7530, 140897940 = 1401b6ea0, 140897cf0 = 1401b7250,
// 1408986a0 = 1401b7c00, 140898490 = 1401b79f0, 140898880 = 1401b7de0). Packet mdt-keyimport.
//
// The scratch model lives on the stack in the original; the port builds it in zeroed heap memory
// (port-conventions gotcha: constructors leave some fields unset) and runs the real dtor 1401cb2c0.
#pragma once

#include <cstdint>

namespace def::model {
class DB_STEP_DATA;
class DYNARY;
class LLOBJ_DATA;
class Material;
class PhasePair;
class Cls_1401fa020;
class Cls_14017a6c0;
} // namespace def::model

namespace def::cmd {

using model::DB_STEP_DATA;

// ---- entries ----------------------------------------------------------------------------------

// @src 1401b6c80  SimCtrl.KEY: read; on success model vt[0](&scratch dim), simulation control copy
// 1401f3660 (model NSTART kept unless the file gave NSTART, scratch+0x2f88), and with
// copy_names the UNNAME/UENAME arrays (1401d7430). Returns the KEY loader's status.
int key_import_sim_ctrl(DB_STEP_DATA* m, const char* name, bool copy_names);

// @src 1401b7530  InterObject.KEY: read; on success model vt[0](&dim) and the inter-object list
// copy 1401fa0c0. Returns true on success.
bool key_import_inter_object(DB_STEP_DATA* m, const char* name);

// @src 1401b6ea0  material KEY: the first mixture material (MSTMTR flag 1) among materials
// 2..n, else material 1, goes into model material `number` (< 1: first free number, 14029c3c0).
// convert_units (the byte == 1): unit conversion between the file's UNIT and the model's UNIT.
// Then 1401d0fc0(model, 0) and an empty name becomes the file's base name (cut to 0x40).
// Returns the model material, or null (read error, no material).
model::Material* key_import_material(DB_STEP_DATA* m, const char* name, std::int32_t number,
                                     bool convert_units);

// @src 1401b7250  object KEY: scratch gets the model's simulation control first; the first object
// that is not kind 7 becomes model object `number` (< 1: first free number) through 140161770;
// empty name -> file base name (cut to 0x50); scratch+0x16cc > 0 -> model+0x16cc, model+0x864 = 1.
// Returns the model object, or null.
model::LLOBJ_DATA* key_import_object(DB_STEP_DATA* m, const char* name, std::int32_t number);

// @src 1401b7c00  MeshSettings KEY: model vt[0](&dim), then the obj+0x2d10 block of the first
// non-kind-7 scratch object into model object `number` (140189b70 with 1, 1).
bool key_import_mesh_settings(DB_STEP_DATA* m, const char* name, std::int32_t number);

// @src 1401b79f0  ObjProperties KEY: obj+0x1eec, +0x1ef0, +0x1ef8, +0x1f00, +0x1f08 and the list
// at +0x20b8 (14025a730) of the first non-kind-7 scratch object into model object `number`
// (no dimension call).
bool key_import_obj_properties(DB_STEP_DATA* m, const char* name, std::int32_t number);

// @src 1401b7de0  GeoConvt23Data KEY: model vt[0](&dim), then the GEO23 block obj+0x2be8
// (140188c90) of the first non-kind-7 scratch object into model object `number`.
bool key_import_geo_convt23(DB_STEP_DATA* m, const char* name, std::int32_t number);

// ---- helpers (src/key_import/model_parts.cpp) ----------------------------------------------------

// @src 1401d7e90  byte at +0x2f88 ("NSTART given" by the last KEY read)
std::uint8_t step_nstart_given(const DB_STEP_DATA* m);
// @src 1401d7430  copy the UNNAME / UENAME arrays of src into m (resizes only when src has any)
void step_copy_name_arrays(DB_STEP_DATA* m, DB_STEP_DATA* src);
// @src 1401fa0c0  inter-object list copy: clear dst, then append a copy of every src pair
void inter_object_list_copy(model::Cls_1401fa020* dst, model::Cls_1401fa020* src);
// Callees owned by lower layers (copies forward, AGENTS.md section 7):
//   1401cabc0 def::model::first_real_object_number, 14017ac20 first_free_object_number,
//   140161770 llobj_copy_keep_number, 140188c90 cls_1401889a0_assign, 140339ba0
//   phase_pair_in_mixture (defmodel/gui_twins.hpp, mdt-model); 140323ce0
//   def::dbio::query::strip_extension (defdbio/db_query.hpp, mdt-dbquery).

// @src 140323d50  remove everything up to the last '\\' or '/'
void path_strip_directory(char* s);
// @src 14028d9a0  material copy (14028d370) keeping dst's number; returns true
bool material_copy_keep_number(model::Material* dst, const model::Material* src);
// @src 14019d6b0  mixture material import: copy src into dst, then (when dst is a mixture and the
// scratch model has more than one material) add every component material to m under a fresh
// number, renumber the MSTMTR list and the FSTRES / CREEP material columns, and copy the scratch
// phase pairs between the components (renumbered). -1 when dst is null, else 0.
std::int32_t material_import_mixture(DB_STEP_DATA* m, model::Material* dst,
                                     const model::Material* src, DB_STEP_DATA* scratch);

// ---- unit conversion (src/key_import/material_units.cpp) -----------------------------------------

// @src 14019df50  (key_unit, model_unit) = (2, 1) -> mode 2, (1, 2) -> mode 1, else nothing
void material_convert_units_mixture(DB_STEP_DATA* m, model::Material* mat, std::int32_t key_unit,
                                    std::int32_t model_unit);
// @src 14019dfc0  convert mat, its component materials in m and the phase pairs of m that link
// two of its components
void material_convert_mixture_mode(DB_STEP_DATA* m, model::Material* mat, std::int32_t mode);
// @src 14033adb0  (key_unit, model_unit) = (2, 1) -> mode 2, (1, 2) -> mode 1, else nothing
void material_convert_units(model::Material* mat, std::int32_t key_unit, std::int32_t model_unit);
// @src 14033ae10  convert every unit-bearing field of the material (mode 0: nothing)
void material_convert_mode(model::Material* mat, std::int32_t mode);
// @src 14033aa30  convert every unit-bearing field of the phase pair (mode 0: nothing)
void phase_pair_convert_mode(model::PhasePair* p, std::int32_t mode);
// @src 14012e9b0  DYNARY conversion for multi-component tables (ncomp > 1): axes by their
// variable's unit kind, then the values by the table kind (0x27, 0x28, 0x30, 0x47, 0x48)
void dynary_convert_units_multi(model::DYNARY* t, std::int32_t kind, std::int32_t mode);

// ---- test seam -----------------------------------------------------------------------------------
// The single-record copies 14028d370 (material) and 1403357a0 (phase pair) are p8-08's
// (material_record_copy / phase_pair_record_copy in defcmd/interp.hpp). Null = the owner's
// function; a test may point these at doubles.
struct KeyImportExt {
    void (*material_copy)(model::Material* dst, const model::Material* src) = nullptr;     // 14028d370
    void (*phase_pair_copy)(model::PhasePair* dst, const model::PhasePair* src) = nullptr; // 1403357a0
};
inline KeyImportExt g_key_import_ext{};

} // namespace def::cmd
