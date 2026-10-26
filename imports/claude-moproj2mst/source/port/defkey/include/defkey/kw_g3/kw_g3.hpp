// defkey/kw_g3/kw_g3.hpp: KEY reader and writer of registry group 3 (inter-object data: CNTACT,
// FRCFAC, IHTCOF, SEPRES, INTRST, FRCROT, SEPDEN, FRCWIN, CNTMTH, WMODEL, RGDCNT), packet kw-g3.
//
// Specs: docs/re/keywords/<KW>.md, _common.md section 7 (pair list), CNTACT.md (mirroring).
// Status: docs/re/status/kw-g3.tsv.
//
// Model side (also kw-g3, in defmodel): the window class LLWINDEF_DATA and the window-list
// methods (defmodel/window/), the pair-list add/find/nth, the mirror copy and the DYNARY copy
// (defmodel/window/inter_object_ops.hpp). The record, list and window-list ctors/dtors/clear are
// p1-06's (defmodel/src/linked/inter_object.cpp). Shared KEY I/O: key-common's defkey API.
#pragma once

#include <cstdint>
#include <cstdio>
#include <vector>

#include <defmodel/window/inter_object_ops.hpp>
#include <defmodel/window/llwindef_data.hpp>
#include <defmodel/window/window_list.hpp>
#include <defmodel/layout/cls_1401f46e0.hpp>
#include <defmodel/layout/cls_1401fa020.hpp>

namespace def::key::g3 {

using def::model::Cls_140269250;
using def::model::Cls_1401f46e0;
using def::model::Cls_1401fa020;
using def::model::Cls_1401fb020;
using def::model::DB_STEP_DATA;
using def::model::DYNARY;
using def::model::LLWINDEF_DATA;

// Keyword ids (registry group 3).
inline constexpr int kCNTACT = 0x10f;
inline constexpr int kFRCFAC = 0x110;
inline constexpr int kIHTCOF = 0x111;
inline constexpr int kSEPRES = 0x112;
inline constexpr int kINTRST = 0x113;
inline constexpr int kFRCROT = 0x114;
inline constexpr int kSEPDEN = 0x115;
inline constexpr int kFRCWIN = 0x116;
inline constexpr int kCNTMTH = 0x117;
inline constexpr int kWMODEL = 0x118;
inline constexpr int kRGDCNT = 0x119;

// ---- window geometry KEY I/O ------------------------------------------------------------------
int window_read_points_3d(std::FILE* f, LLWINDEF_DATA* w, bool indexed);            // 14041b7d0
int window_read_points_2d(std::FILE* f, LLWINDEF_DATA* w, std::int32_t n, bool indexed); // 14041b6a0
int window_write_points(std::FILE* f, LLWINDEF_DATA* w, std::int32_t mode);         // 1402f4890

// ---- KEY reader -------------------------------------------------------------------------------
// FUN_140388fb0: one group 3 keyword; `m` is what FUN_1401d81b0 returned. Returns 0, 2 or 3.
int read_keyword(DB_STEP_DATA* m, std::FILE* f, int id);

// ---- KEY writer -------------------------------------------------------------------------------
int write_inter_objects(DB_STEP_DATA* m, std::FILE* f);                   // 1403c9ad0
int write_pair(DB_STEP_DATA* m, std::FILE* f, Cls_1401f46e0* p);          // 1403c8560
int write_frcfac(DB_STEP_DATA* m, std::FILE* f, Cls_1401f46e0* p, std::int32_t master,
                 std::int32_t slave);                                     // 1403c9be0
int write_frcrot(std::FILE* f, Cls_1401f46e0* p);                         // 1403caa40
int write_ihtcof(DB_STEP_DATA* m, std::FILE* f, Cls_1401f46e0* p, std::int32_t master,
                 std::int32_t slave);                                     // 1403ca830
int write_sepres(std::FILE* f, Cls_1401f46e0* p);                         // 1403cad60
int write_sepden(std::FILE* f, DB_STEP_DATA* m, Cls_1401f46e0* p);        // 1403caf20
int write_intrst(std::FILE* f, Cls_1401f46e0* p);                         // 1403cb0a0
int write_cntmth(std::FILE* f, DB_STEP_DATA* m, Cls_1401f46e0* p);        // 1403cbce0
int write_wmodel(std::FILE* f, Cls_1401f46e0* p);                         // 1403cb840
int write_rgdcnt(std::FILE* f, Cls_1401f46e0* p);                         // 1403cbb10

// ---- registry handler columns 2 and 3 ---------------------------------------------------------
// The original passes std::vector<int> by value {pairIndex, master, slave}; the callee destroys it.
int handler2_frcrot(std::FILE* f, DB_STEP_DATA* m, std::vector<int> v);   // 1403cab90
int handler2_ihtcof(std::FILE* f, DB_STEP_DATA* m, std::vector<int> v);   // 1403cb3e0
int handler2_sepres(std::FILE* f, DB_STEP_DATA* m, std::vector<int> v);   // 1403cae30
int handler2_sepden(std::FILE* f, DB_STEP_DATA* m, std::vector<int> v);   // 1403cafb0
int handler2_intrst(std::FILE* f, DB_STEP_DATA* m, std::vector<int> v);   // 1403cb1f0
int handler2_frcfac(std::FILE* f, DB_STEP_DATA* m, std::vector<int> v);   // 1403cb5f0
int handler2_wmodel(std::FILE* f, DB_STEP_DATA* m, std::vector<int> v);   // 1403cba00
int handler2_rgdcnt(std::FILE* f, DB_STEP_DATA* m, std::vector<int> v);   // 1403cbbd0
int handler2_cntmth(std::FILE* f, DB_STEP_DATA* m, std::vector<int> v);   // 1403cbdf0
int handler3_frcrot(DB_STEP_DATA* m, const char* label, std::vector<int> v, DYNARY** out);  // 1403cac80
int handler3_ihtcof(DB_STEP_DATA* m, const char* label, std::vector<int> v, DYNARY** out);  // 1403cb520
int handler3_intrst(DB_STEP_DATA* m, const char* label, std::vector<int> v, DYNARY** out);  // 1403cb300
int handler3_frcfac(DB_STEP_DATA* m, const char* label, std::vector<int> v, DYNARY** out);  // 1403cb740

} // namespace def::key::g3
