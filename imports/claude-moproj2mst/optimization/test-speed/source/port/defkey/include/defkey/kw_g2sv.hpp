// defkey/kw_g2sv.hpp: keyword group 2 (objects), packet kw-g2sv: the state-variable keywords.
//
//   nodal       DRZ URZ FRZ PRZ NDTMP NDHEAT NDFLUX DATOM VOTAGE USRNOD NWEAR DRMESH GRAIN2
//               TIMERC NRECVL
//   element     STRAIN STNCMP DENSTY DAMAGE STRESS MATAXI VOLFC VOLFS DVOLF TICF GRAIN SIZESH
//               TXTODF YLDS ZEFI ZMFI USRELM CURRNT ERECVL
//   other       CUSVAR (custom variables, obj+0xb58), HDNEST
//
// Writing: the state-variable writer FUN_1402a4a80 for every code (also the BCC*, HDNOBJ, CRBFLX,
// RHTFLX, MTLGRP and EC* codes whose readers other packets own), its line writers and the
// registry write handlers that wrap it. RZ (code 0) and ELMCON (0x25) go to kw-g2geo's writers.
//
// API: kw-g2obj's "Packet API" (defkey/kw_g2obj.hpp). Specs: docs/re/keywords/<KW>.md,
// group2-statevar-writers.md, group2-helpers.md. Status: docs/re/status/kw-g2sv.tsv.
#pragma once

#include <cstdint>
#include <cstdio>
#include <vector>

namespace def::model {
class DB_STEP_DATA;
class LLOBJ_DATA;
}  // namespace def::model

namespace def::key::g2sv {

using KeyRef = std::vector<std::int32_t>;  // same as def::key::g2obj::KeyRef

// Parser ids whose case of FUN_14038b8a0 this packet ports.
bool handles(int id);
// The case body after the object lookup: 0, 2 ("Reading Error") or 3 ("Data Error").
int read_case(def::model::DB_STEP_DATA& m, std::FILE* f, def::model::LLOBJ_DATA& obj, int id);

// Registry column 2 (KEY write handlers) owned here.
bool handles_write(int id);
int write_keyword(int id, std::FILE* f, def::model::DB_STEP_DATA& m, const KeyRef& ref);

// FUN_1402a4a80(model, f, obj, code, N, k): one object state variable by its code. N < 1 means
// obj+8; k is the custom-variable id for code 0x4e. 0, or 3 for a code it does not handle.
int write_state_var(def::model::DB_STEP_DATA& m, std::FILE* f, def::model::LLOBJ_DATA& obj,
                    int code, std::uint32_t no, std::int32_t k);

// The custom state-variable list obj+0xb58.
int custom_count(def::model::LLOBJ_DATA& obj);             // 14032a140
void* custom_at(def::model::LLOBJ_DATA& obj, int i);       // 140185f80: StateVarCustom or null
std::int32_t custom_id(void* entry);                       // 14032ec90

// ---- line writers (KwdWriteUtl) -------------------------------------------------------------
// FUN_1402f5c10 writeNodeElemVarDouble: header by fmt (0..5), records of the entities with a
// component != d, `m` values each, 5 per line.
int write_node_elem_double(std::FILE* f, const char* name, std::uint32_t no, const double* v,
                           std::int32_t n, std::int32_t m, double d, std::int32_t fmt,
                           std::int32_t k);
// FUN_1402f5360: uniformity / defaults / record count of `n x m` values (fmt 2: per component).
void analyse_values(const double* v, const std::int32_t* n, const std::int32_t* m, const double* d,
                    const std::int32_t* fmt, bool* uniform, std::int32_t* count, double* defs);
// FUN_1402f4df0: the integer per-entity writer (BCC*, MTLGRP).
int write_node_elem_int(std::FILE* f, const char* name, std::uint32_t no, const std::int32_t* v,
                        std::int32_t n, std::int32_t m, std::int32_t d);
// FUN_1402f60c0 writeNodeElemVarDouble2: element array plus nodal companion.
int write_node_elem_double2(std::FILE* f, const char* name, std::uint32_t no, const double* v1,
                            std::int32_t n1, std::int32_t m1, double d1, const double* v2,
                            std::int32_t n2, std::int32_t m2, double d2, std::int32_t fmt);
// FUN_1402f58d0: per element `ip` integration points of `m` values (DAMAGE storage mode 3).
int write_node_elem_ip(std::FILE* f, const char* name, std::uint32_t no, const double* v,
                       std::int32_t n, std::int32_t m, double d, std::int32_t ip, std::int32_t fmt);
// FUN_1402f7490: "rows  cols" then `rows` lines of `cols` values "%15.7E  " (v[r * cols + c]).
// 1 (after the header line) when rows or cols < 1, else 0.
int write_matrix(std::FILE* f, const double* v, std::int32_t cols, std::int32_t rows);

// FUN_1402a7b60: VOLFC (id 0x31) / VOLFS (0x32) gathered to the object's phases and written with
// write_matrix; no < 1 takes the object number. 0, or 3 for another id.
int write_phase_matrix(def::model::DB_STEP_DATA& m, std::FILE* f, def::model::LLOBJ_DATA& obj,
                       int id, std::int32_t no);

} // namespace def::key::g2sv
