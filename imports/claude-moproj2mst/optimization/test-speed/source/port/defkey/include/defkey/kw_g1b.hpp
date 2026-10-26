// defkey/kw_g1b.hpp: KEY reader and writer for keyword group 1 (simulation control), packet
// kw-g1b: the "Iteration Controls", "Processing Conditions" and "User Defined Variables"
// sections, registry ids 61..91 (CVGERR .. FRQNCY, plus INIGES which sits in that id range).
//
// The group 1 shells belong to kw-g1a (port/defkey/include/defkey/kw_g1a.hpp):
//   * the parser switch FUN_14039aa90 calls read_keyword() for its cases 0x27..0x45;
//   * the KEY writer FUN_1403d4990 calls write_sections() where its "Iteration Controls" banner
//     starts.
// Specs: docs/re/keywords/<KW>.md and _common.md (group 1/3 spec thread).
#pragma once

#include <cstdio>
#include <vector>

namespace def::model {
class DB_STEP_DATA;
}

namespace def::key::g1b {

// True for the ids read_keyword() handles (61..91).
bool handles(int id);

// part of @src 14039aa90  cases 0x27..0x45 of the group 1 parser. Returns what the parser returns
// for that case: 0, 2 ("Reading Error") or 3 ("Data Error"); 2 for an id outside 61..91 (the
// switch default). `m` is the object FUN_1401d81b0 hands to the parser.
int read_keyword(def::model::DB_STEP_DATA& m, std::FILE* f, int id);

// part of @src 1403d4990  the group 1 KEY writer from the "Iteration Controls" banner (included)
// to its end: Iteration Controls, Processing Conditions, User Defined Variables. Returns 0, or
// the first non-zero code of a per-keyword writer (which stops the rest, like the original).
int write_sections(std::FILE* f, def::model::DB_STEP_DATA& m);

// ---- registry handler columns (keyword_registry_table.hpp keeps their addresses) -------------
// Column 2 (KEY write handler): (FILE*, model, int vector) -> status; the vector is taken by value
// and unused, as in the original (each one destroys its copy).
int write_cvgerr(std::FILE* f, def::model::DB_STEP_DATA& m, std::vector<int> v);  // FUN_1403d8260
int write_geoerr(std::FILE* f, def::model::DB_STEP_DATA& m, std::vector<int> v);  // FUN_1403d82f0
int write_peninf(std::FILE* f, def::model::DB_STEP_DATA& m, std::vector<int> v);  // FUN_1403d83a0
int write_unte2h(std::FILE* f, def::model::DB_STEP_DATA& m, std::vector<int> v);  // FUN_1403d8420
int write_tintgf(std::FILE* f, def::model::DB_STEP_DATA& m, std::vector<int> v);  // FUN_1403d7250
int write_blzman(std::FILE* f, def::model::DB_STEP_DATA& m, std::vector<int> v);  // FUN_1403d72d0
int write_envtmp(std::FILE* f, def::model::DB_STEP_DATA& m, std::vector<int> v);  // FUN_1403d6c90
int write_cnvcof(std::FILE* f, def::model::DB_STEP_DATA& m, std::vector<int> v);  // FUN_1403d6f30
int write_envatm(std::FILE* f, def::model::DB_STEP_DATA& m, std::vector<int> v);  // FUN_1403d70c0
int write_acvcof(std::FILE* f, def::model::DB_STEP_DATA& m, std::vector<int> v);  // FUN_1403d7350
int write_envmpr(std::FILE* f, def::model::DB_STEP_DATA& m, std::vector<int> v);  // FUN_1403d75a0
int write_envmpt(std::FILE* f, def::model::DB_STEP_DATA& m, std::vector<int> v);  // FUN_1403d7620
int write_ehrate(std::FILE* f, def::model::DB_STEP_DATA& m, std::vector<int> v);  // FUN_1403d76a0

// Column 3 (data accessor): stores the address of the keyword's table in *out, unless `m` is
// the null-object sentinel (the original tests `m != -0x750`, i.e. a null simulation-control
// pointer turned back into a model pointer). Always returns 0. The second argument is unused.
int table_envtmp(def::model::DB_STEP_DATA* m, void* unused, std::vector<int> v, void** out);  // FUN_1403d6ec0
int table_cnvcof(def::model::DB_STEP_DATA* m, void* unused, std::vector<int> v, void** out);  // FUN_1403d7050
int table_envatm(def::model::DB_STEP_DATA* m, void* unused, std::vector<int> v, void** out);  // FUN_1403d71e0
int table_acvcof(def::model::DB_STEP_DATA* m, void* unused, std::vector<int> v, void** out);  // FUN_1403d7530

}  // namespace def::key::g1b
