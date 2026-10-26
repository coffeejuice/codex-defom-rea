// defkey/kw_g4.hpp: KEY reader and writer for keyword group 4 (material), packet kw-g4b.
//
// This packet owns the group parser FUN_140384a50 (material lookup / creation and the switch on
// id - 92) and the per-material writer FUN_1403cdca0, plus the read cases and write / accessor
// handlers of 32 keywords:
//   MTNAME YOUNG POISON EXPAND THRCND HEATCP EMSVTY LTHEAT TMPSOL TMPLIQ DIFCOE GRNDAT GRNMDL
//   HDNPHA ELRST MSTMTR HDNTIM JOMINY UTSDAT HDNRUL PMEAB PMITT MATDEN MASDEN COARSE SIZEMD
//   TXTURE DIFBND SINTER GBENGY GBMOBI NUCSIZ
// The other ten group 4 keywords (FSTRES CREEP FRCMOD FPERV FRAE2H ANISO BURGRS ALPHA NDISFM
// RECVRY) are kw-g4a's; the parser and the writer call their functions.
// Specs: docs/re/keywords/<KW>.md, _common_g457.md, group4_5_7-index.md.
#pragma once

#include <cstdio>
#include <vector>

namespace def::model {
class DB_STEP_DATA;
class Material;
} // namespace def::model

namespace def::key::g4 {

// @src 140384a50  group 4 parser. `m` is what FUN_1401b6520 hands to the group parsers
// (FUN_1401d81b0(model)); the stream is right after the keyword name. Reads the material number
// with fscanf("%i"), creates the material if it is new, then the keyword's data. Returns 0, 2
// ("Reading Error") or 3 ("Data Error").
int read_keyword(def::model::DB_STEP_DATA& m, std::FILE* f, int id);

// @src 1403cdca0  writes one material block (banner, MTNAME, then every keyword in the fixed
// order of _common_g457.md section 6). Writes nothing for a null material or one whose skip
// byte (+0x2210) is 1. `name_override` replaces the name when non-empty (the KEY writer passes
// null). Returns 0 or the first non-zero code of a keyword writer, which stops the block.
int write_material(def::model::DB_STEP_DATA& m, std::FILE* f, def::model::Material* mat,
                   const char* name_override);

// ---- registry column 2: KEY write handlers, (FILE*, model, {material number}) -> status ------
int write_young(std::FILE* f, def::model::DB_STEP_DATA& m, std::vector<int> v);   // 1403cf050
int write_poison(std::FILE* f, def::model::DB_STEP_DATA& m, std::vector<int> v);  // 1403cf180
int write_expand(std::FILE* f, def::model::DB_STEP_DATA& m, std::vector<int> v);  // 1403cf2b0
int write_thrcnd(std::FILE* f, def::model::DB_STEP_DATA& m, std::vector<int> v);  // 1403cf490
int write_heatcp(std::FILE* f, def::model::DB_STEP_DATA& m, std::vector<int> v);  // 1403cf5c0
int write_emsvty(std::FILE* f, def::model::DB_STEP_DATA& m, std::vector<int> v);  // 1403cf700
int write_difcoe(std::FILE* f, def::model::DB_STEP_DATA& m, std::vector<int> v);  // 1403cfca0
int write_hdnpha(std::FILE* f, def::model::DB_STEP_DATA& m, std::vector<int> v);  // 1403cffe0
int write_elrst(std::FILE* f, def::model::DB_STEP_DATA& m, std::vector<int> v);   // 1403d0280
int write_hdntim(std::FILE* f, def::model::DB_STEP_DATA& m, std::vector<int> v);  // 1403d03b0
int write_jominy(std::FILE* f, def::model::DB_STEP_DATA& m, std::vector<int> v);  // 1403d0510
int write_utsdat(std::FILE* f, def::model::DB_STEP_DATA& m, std::vector<int> v);  // 1403d0680
int write_pmeab(std::FILE* f, def::model::DB_STEP_DATA& m, std::vector<int> v);   // 1403d07b0
int write_pmitt(std::FILE* f, def::model::DB_STEP_DATA& m, std::vector<int> v);   // 1403d08e0
int write_masden(std::FILE* f, def::model::DB_STEP_DATA& m, std::vector<int> v);  // 1403d1eb0
int write_coarse(std::FILE* f, def::model::DB_STEP_DATA& m, std::vector<int> v);  // 1403d1fe0
int write_difbnd(std::FILE* f, def::model::DB_STEP_DATA& m, std::vector<int> v);  // 1403d2170
int write_grndat(std::FILE* f, def::model::DB_STEP_DATA& m, std::vector<int> v);  // 1403d3a90
int write_gbengy(std::FILE* f, def::model::DB_STEP_DATA& m, std::vector<int> v);  // 1403d4400
int write_gbmobi(std::FILE* f, def::model::DB_STEP_DATA& m, std::vector<int> v);  // 1403d44a0
int write_nucsiz(std::FILE* f, def::model::DB_STEP_DATA& m, std::vector<int> v);  // 1403d4550
int write_sinter(std::FILE* f, def::model::DB_STEP_DATA& m, std::vector<int> v);  // 1403d45f0
// MTNAME, MSTMTR, HDNRUL, MATDEN, SIZEMD and TXTURE are written inline by write_material;
// LTHEAT, TMPSOL, TMPLIQ and GRNMDL are never written (registry stub 1402f75d0).

// ---- registry column 3: data accessors. *out = the keyword's table in the material named by
// v[0]; *out is untouched when that material does not exist. Always 0; the label is unused. ----
int table_young(def::model::DB_STEP_DATA* m, const char* label, std::vector<int> v, void** out);  // 1403cf0f0
int table_poison(def::model::DB_STEP_DATA* m, const char* label, std::vector<int> v, void** out); // 1403cf220
int table_expand(def::model::DB_STEP_DATA* m, const char* label, std::vector<int> v, void** out); // 1403cf360
int table_thrcnd(def::model::DB_STEP_DATA* m, const char* label, std::vector<int> v, void** out); // 1403cf530
int table_heatcp(def::model::DB_STEP_DATA* m, const char* label, std::vector<int> v, void** out); // 1403cf670
int table_emsvty(def::model::DB_STEP_DATA* m, const char* label, std::vector<int> v, void** out); // 1403cf7a0
// DIFCOE: the table of the current mode (1, 2, 3); modes 0 and 4 leave *out untouched.
int table_difcoe(def::model::DB_STEP_DATA* m, const char* label, std::vector<int> v, void** out); // 1403cff00
int table_hdnpha(def::model::DB_STEP_DATA* m, const char* label, std::vector<int> v, void** out); // 1403d01f0
int table_elrst(def::model::DB_STEP_DATA* m, const char* label, std::vector<int> v, void** out);  // 1403d0320
int table_hdntim(def::model::DB_STEP_DATA* m, const char* label, std::vector<int> v, void** out); // 1403d0480
int table_jominy(def::model::DB_STEP_DATA* m, const char* label, std::vector<int> v, void** out); // 1403d05f0
int table_utsdat(def::model::DB_STEP_DATA* m, const char* label, std::vector<int> v, void** out); // 1403d0720
int table_pmeab(def::model::DB_STEP_DATA* m, const char* label, std::vector<int> v, void** out);  // 1403d0850
int table_pmitt(def::model::DB_STEP_DATA* m, const char* label, std::vector<int> v, void** out);  // 1403d0980
int table_masden(def::model::DB_STEP_DATA* m, const char* label, std::vector<int> v, void** out); // 1403d1f50
int table_coarse(def::model::DB_STEP_DATA* m, const char* label, std::vector<int> v, void** out); // 1403d20e0
int table_difbnd(def::model::DB_STEP_DATA* m, const char* label, std::vector<int> v, void** out); // 1403d2210

} // namespace def::key::g4
