// defkey/kw_g2bc_tables.hpp: registry columns 2 and 3 of the kw-g2bc local family (BCCFNC,
// LOCTMP, LOCATM, LOCDEF). Bodies in src/kw_g2bc/local.cpp.
//
// The KEY writers reach the column-2 handlers through def::key::g2bc::write_keyword(id, ...)
// (kw_g2bc.hpp); they are also declared here for direct calls. Every handler takes the reference
// (object number, 1-based entry index) like the original.
//
// Column 3 (data accessors, GUI only; no KEY writer calls them): *out = the address of the
// entry's table object (by name for the LOC* keywords). Return 0, or 3 when the object or entry
// is unknown (or, LOC*, when the reference has fewer than 2 slots or the name is null). An
// unknown name, or (BCCFNC) an index outside 1..count, leaves *out unchanged and returns 0.
#pragma once

#include <cstdint>
#include <cstdio>

#include <defkey/kw_g2bc.hpp>

namespace def::key::g2bc {

int write_bccfnc(std::FILE* f, def::model::DB_STEP_DATA& m, const KeyRef& ref);  // 1402b4050
int write_loctmp(std::FILE* f, def::model::DB_STEP_DATA& m, const KeyRef& ref);  // 1402ae590
int write_locatm(std::FILE* f, def::model::DB_STEP_DATA& m, const KeyRef& ref);  // 1402aeb40
int write_locdef(std::FILE* f, def::model::DB_STEP_DATA& m, const KeyRef& ref);  // 1402aef30

// BCCFNC: the table at list position ref[1] - 1 (FUN_140136860, whose n < 2 gives the head, so
// ref[1] 1 and 2 both give the first table: the original's off-by-one). Name ignored.
int table_bccfnc(def::model::DB_STEP_DATA* m, const char* name, const KeyRef& ref, void** out);  // 1402b4180
// LOCTMP: "Env Tempreature" [sic], "Conv. Coef.", "Emissivity", "Heat Flux", "Heat Trans. Coef."
int table_loctmp(def::model::DB_STEP_DATA* m, const char* name, const KeyRef& ref, void** out);  // 1402ae950
// LOCATM: "Env. Atom", "React. Coef.", "Atom Flux"
int table_locatm(def::model::DB_STEP_DATA* m, const char* name, const KeyRef& ref, void** out);  // 1402aeda0
// LOCDEF: "Pressure", "Friction"
int table_locdef(def::model::DB_STEP_DATA* m, const char* name, const KeyRef& ref, void** out);  // 1402af1f0

} // namespace def::key::g2bc
