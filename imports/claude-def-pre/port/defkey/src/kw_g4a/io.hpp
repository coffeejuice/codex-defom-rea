// kw_g4a/io.hpp: the shared KEY I/O mechanics the kw-g4a keywords call (line helpers, DYNARY
// body reader/writer, generic "t a b c" header, original-CRT printf, material lookup).
//
// Those functions belong to the key-common packet (defkey line_io/table_io, defmodel
// dynary_init) and to p1-08 (material list); io.cpp forwards to them, so the keyword files do
// not depend on their names. find_material is still a local copy until p1-08 ports it.
#pragma once

#include <cstdint>
#include <cstdio>

namespace def::model {
class DB_STEP_DATA;
class DYNARY;
class Material;
} // namespace def::model

namespace def::key::g4a::io {

using def::model::DYNARY;

// fprintf; the original CRT conventions (three-digit exponents, half-up rounding) come from the
// process-wide legacy printf options (def::rt::init_stdio_compat). Text mode gives CR LF.
int kfprintf(std::FILE* fp, const char* fmt, ...);

// One-line helpers (fgets 0x400, ',' -> ' ', outputs zeroed, sscanf). Return 1 on EOF or when
// sscanf returns -1 (blank line), else 0. Formats as in the original ("%Flf" = "%lf").
int line_i(std::FILE* fp, std::int32_t* a, bool skip_blank);                                  // 140413de0
int line_ii(std::FILE* fp, std::int32_t* a, std::int32_t* b);                                 // 140413ee0
int line_iii(std::FILE* fp, std::int32_t* a, std::int32_t* b, std::int32_t* c);               // 140413fb0 (nOut = NULL)
int line_d(std::FILE* fp, double* a);                                                         // 140414990
int line_dd(std::FILE* fp, double* a, double* b);                                             // 140414a40
int line_d4(std::FILE* fp, double* a, double* b, double* c, double* d);                       // 140414e40
int line_d5(std::FILE* fp, double* v0, double* v1, double* v2, double* v3, double* v4);       // 140414f50
int line_d7(std::FILE* fp, double* v[7]);                                                     // 1404151c0
int line_d9(std::FILE* fp, double* v[9]);                                                     // 140415320
int line_idd(std::FILE* fp, std::int32_t* t, double* a, double* b);                           // 140415680
int line_id5(std::FILE* fp, std::int32_t* t, double* v[5]);                                   // 140415ac0

// DYNARY body reader FUN_14041a800 (third argument ignored by the original, not passed here).
int read_body(std::FILE* fp, DYNARY* d);
// Generic property header + body FUN_14041b270(fp, dyn, wantExtra, extra).
int read_generic(std::FILE* fp, DYNARY* d, int want_extra, double* extra);
// DYNARY body writer FUN_1402f3130(fp, dyn, mode).
int write_body(std::FILE* fp, const DYNARY* d, int mode);

// DYNARY template family (defmodel, key-common packet).
void dyn_reinit_keep(DYNARY* d, int type);   // 14012fc70
bool dyn_complete(const DYNARY* d);          // 14012f610

// MaterialList find FUN_14029c2c0 on DB_STEP_DATA::materials.
def::model::Material* find_material(def::model::DB_STEP_DATA* md, std::int32_t number);

} // namespace def::key::g4a::io
