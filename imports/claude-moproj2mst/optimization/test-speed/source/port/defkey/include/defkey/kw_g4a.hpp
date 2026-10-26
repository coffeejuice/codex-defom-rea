// defkey/kw_g4a.hpp: KEY reader and writer of the group 4 material keywords of packet kw-g4a
// (flow stress, fracture and the dislocation-density group): FRAE2H, FPERV, FRCMOD, FSTRES,
// CREEP, ANISO, BURGRS, ALPHA, NDISFM, RECVRY. Specs: docs/re/keywords/<KW>.md and
// _common_g457.md (branch claude/project-thread-ujpfry).
//
// Read side. The group 4 parser FUN_140384a50 (ported by kw-g4b) reads the material number with
// fscanf("%i"), finds or appends the material, then switches on `id - 0x5c`. read_<kw>() is the
// body of one switch case including the parser's common tail, so the parser returns its result
// unchanged: 0 ok, 2 "Reading Error" (no case of this packet returns 3 on read). The stream is
// positioned right after the material number, on the keyword line.
//
// Write side. write_<kw>() is the registry handler2 (KEY write handler) with the original's
// signature: the material is looked up by ids[0] in DB_STEP_DATA::materials (FUN_14029c2c0, no
// null check, like the original) and the keyword block is printed. Return 0 ok, 3 "Data Error"
// (the KEY writer FUN_1403cdca0 aborts on the first non-zero return).
//
// accessor_<kw>() is the registry handler3 (data accessor of the edit menus).
#pragma once

#include <cstdint>
#include <cstdio>
#include <vector>

namespace def::model {
class DB_STEP_DATA;
class Material;
} // namespace def::model

namespace def::key::g4a {

using def::model::DB_STEP_DATA;
using def::model::Material;
using Ids = std::vector<std::int32_t>;

// ---- read: cases of FUN_140384a50 --------------------------------------------------------------
int read_frae2h(std::FILE* fp, Material& mat);  // case 0x01, id  93
int read_fperv(std::FILE* fp, Material& mat);   // case 0x0c, id 104
int read_frcmod(std::FILE* fp, Material& mat);  // case 0x0d, id 105
int read_fstres(std::FILE* fp, Material& mat);  // case 0x02, id  94
int read_creep(std::FILE* fp, Material& mat);   // case 0x0e, id 106
int read_aniso(std::FILE* fp, Material& mat);   // case 0x1f, id 123
int read_burgrs(std::FILE* fp, Material& mat);  // case 0x12, id 110
int read_alpha(std::FILE* fp, Material& mat);   // case 0x13, id 111
int read_ndisfm(std::FILE* fp, Material& mat);  // case 0x14, id 112
int read_recvry(std::FILE* fp, Material& mat);  // case 0x15, id 113

// ---- write: registry handler2 --------------------------------------------------------------------
int write_frae2h(std::FILE* fp, DB_STEP_DATA* md, Ids ids);  // FUN_1403cf3f0
int write_fperv(std::FILE* fp, DB_STEP_DATA* md, Ids ids);   // FUN_1403cf830
int write_frcmod(std::FILE* fp, DB_STEP_DATA* md, Ids ids);  // FUN_1403d3820
int write_fstres(std::FILE* fp, DB_STEP_DATA* md, Ids ids);  // FUN_1403d0c90
int write_creep(std::FILE* fp, DB_STEP_DATA* md, Ids ids);   // FUN_1403d22a0
int write_aniso(std::FILE* fp, DB_STEP_DATA* md, Ids ids);   // FUN_1403d0a10
int write_burgrs(std::FILE* fp, DB_STEP_DATA* md, Ids ids);  // FUN_1403d2c20
int write_alpha(std::FILE* fp, DB_STEP_DATA* md, Ids ids);   // FUN_1403d2fb0
int write_ndisfm(std::FILE* fp, DB_STEP_DATA* md, Ids ids);  // FUN_1403d3280
int write_recvry(std::FILE* fp, DB_STEP_DATA* md, Ids ids);  // FUN_1403d3550

// The same writers after the material lookup (number = the number printed, ids[0] in the
// handler). The handlers above are thin wrappers over these; the tests call them directly.
int write_frae2h_mat(std::FILE* fp, std::int32_t number, Material& mat);
int write_fperv_mat(std::FILE* fp, std::int32_t number, Material& mat);
int write_frcmod_mat(std::FILE* fp, std::int32_t number, Material& mat);
int write_fstres_mat(std::FILE* fp, std::int32_t number, Material& mat);
int write_creep_mat(std::FILE* fp, std::int32_t number, Material& mat);
int write_aniso_mat(std::FILE* fp, std::int32_t number, Material& mat);
int write_burgrs_mat(std::FILE* fp, std::int32_t number, Material& mat);
int write_alpha_mat(std::FILE* fp, std::int32_t number, Material& mat);
int write_ndisfm_mat(std::FILE* fp, std::int32_t number, Material& mat);
int write_recvry_mat(std::FILE* fp, std::int32_t number, Material& mat);

// FUN_14028e140 (Material method used by the FRCMOD writer): stored FRCMOD model -> KEY type *t and
// the five header numbers v (a, b', c', 0, e); false when the model cannot be written.
bool frcmod_key_values(const Material& mat, std::int32_t* t, double v[5]);

// ---- accessor: registry handler3 (FRAE2H and FRCMOD have the stub 1402f7610, returns -1) -------
int accessor_fperv(DB_STEP_DATA* md, const char* label, Ids ids, void** out);   // FUN_1403cfbf0
int accessor_fstres(DB_STEP_DATA* md, const char* label, Ids ids, void** out);  // FUN_1403d1c50
int accessor_creep(DB_STEP_DATA* md, const char* label, Ids ids, void** out);   // FUN_1403d2a00
int accessor_aniso(DB_STEP_DATA* md, const char* label, Ids ids, void** out);   // FUN_1403d0c00
int accessor_burgrs(DB_STEP_DATA* md, const char* label, Ids ids, void** out);  // FUN_1403d2f20
int accessor_alpha(DB_STEP_DATA* md, const char* label, Ids ids, void** out);   // FUN_1403d31f0
int accessor_ndisfm(DB_STEP_DATA* md, const char* label, Ids ids, void** out);  // FUN_1403d34c0
int accessor_recvry(DB_STEP_DATA* md, const char* label, Ids ids, void** out);  // FUN_1403d3790

} // namespace def::key::g4a
