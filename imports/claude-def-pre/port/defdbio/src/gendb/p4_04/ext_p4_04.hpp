// Calls from packet p4-04 into functions other packets own and have not landed yet.
//
// One definition file per owner (ext_<owner>.cpp). Bodies are stubs ("stub for <addr>", not
// @src) written from the decompile so that the checks can be tested; when the owner's function
// lands, the merge replaces the body with a call to it and nothing else in p4-04 changes.
// Owners from docs/phase4-packets.md and the ledger phase column (P2/P3 = not a P4 packet).
#pragma once

#include <cstdint>

#include <defmodel/layout/cls_1401e7b10.hpp>
#include <defmodel/layout/cls_1401f46e0.hpp>
#include <defmodel/layout/cls_1401fa020.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/layout/material_list.hpp>

namespace def::dbio::gendb::ext {

using def::model::Cls_1401e7b10;
using def::model::Cls_1401f46e0;
using def::model::Cls_1401fa020;
using def::model::DB_STEP_DATA;
using def::model::LLOBJ_DATA;
using def::model::MaterialList;

// ---- p4-03 (object checks) -- ext_p4_03.cpp
bool fun_1401792d0(LLOBJ_DATA* obj);   // movement control defined (switch on +0x21c8)
bool fun_1401797e0(LLOBJ_DATA* obj);   // rotation control defined (140179840, 3D: 140179900)
bool fun_140179280(LLOBJ_DATA* obj);   // 1401792d0 || 1401797e0
void fun_1401bfdf0(DB_STEP_DATA& s, const std::int32_t* index, std::int32_t* err, std::int32_t* warn); // check 2
void fun_1401bfe60(DB_STEP_DATA& s, const std::int32_t* index, std::int32_t* err, std::int32_t* warn); // check 3
void fun_1401bfed0(DB_STEP_DATA& s, const std::int32_t* index, std::int32_t* err, std::int32_t* warn); // check 4
void fun_14001cdc0(const std::int32_t* index, std::int32_t* err); // summary hook (empty function)
// Test hook for checks 2..4 (1401bfdf0 / 1401bfe60 / 1401bfed0): when set, called with the check
// number instead of p4-03's check (tier-1 models of this packet are too small for those).
inline void (*g_checks_234_hook)(std::int32_t check, DB_STEP_DATA& s, std::int32_t* err, std::int32_t* warn) = nullptr;

// ---- p4-02b (counts frame) -- ext_p4_02b.cpp
void fun_14001cd60(const std::int32_t* code, const char* keyword, const std::uint32_t* value,
                   const std::int32_t* aux); // message hook (empty function)
std::int32_t fun_1401f4610(const DB_STEP_DATA* s);  // int at +0x7c (GENDB object count)
std::int32_t fun_1401f98d0(const Cls_1401f46e0* rec); // window count (rec+0x188, 14019a590)
std::int32_t fun_14029c410(MaterialList* l);        // largest material number

// ---- p4-02a (object counts) -- ext_p4_02a.cpp
std::int32_t fun_1401f3cb0(const Cls_1401e7b10* sim); // int at +0x118 (HTMTHD)

// ---- Phase 2 functions (no P4 owner) -- ext_p2.cpp
std::int32_t fun_1401ee580(const Cls_1401e7b10* sim); // int at +0x240 (DTTYPE mode)
std::int32_t fun_1401f3c10(const Cls_1401e7b10* sim); // int at +0x104 (TRANS 1, heat transfer)
std::int32_t fun_1401f3c30(const Cls_1401e7b10* sim); // int at +0x108 (TRANS 2, deformation)
std::int32_t fun_1400015d0(const Cls_1401fa020* l);   // int at +0x10 (list count)
Cls_1401f46e0* fun_1401fac90(Cls_1401fa020* l, std::int32_t n); // node n (1-based) or null
Cls_1401f46e0* fun_1401facf0(Cls_1401fa020* l, std::int32_t a, std::int32_t b); // record of objects (a, b)
void* fun_140269900(void* window_list, std::int32_t n); // window node n (1-based, link +0x840)
LLOBJ_DATA* fun_1401d6ea0(DB_STEP_DATA& s, std::int32_t number); // object whose +8 == number (14017aaf0)

// ---- Phase 3 functions (no P4 owner) -- ext_p3.cpp
bool fun_140160f10(const LLOBJ_DATA* obj); // type (+0xc) != 7

} // namespace def::dbio::gendb::ext
