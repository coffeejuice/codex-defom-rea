// Calls from packet p1-03 into functions other packets own and have not landed yet.
//
// One definition file per owner (ext_<owner>.cpp), each body DEF_NOT_PORTED. When the owner's
// function lands, the merge replaces the body with a call to it (or the call site with the
// owner's method) and nothing else in p1-03 changes. Signatures follow the decompile.
#pragma once

#include <cstdint>

#include <defmodel/layout/dynary.hpp>
#include <defmodel/layout/cls_14017a6c0.hpp>
#include <defmodel/layout/cls_1401fa020.hpp>
#include <defmodel/layout/material_list.hpp>
#include <defmodel/layout/material.hpp>
#include <defmodel/layout/phase_pair_list.hpp>
#include <defmodel/layout/cls_1403753f0.hpp>

namespace def::model::ext {

// ---- p1-04 (objects list Cls_14017a6c0) -- ext_p1_04.cpp
void fun_14017a700(Cls_14017a6c0* l);                 // reset head/tail/count (no free)
void fun_14017ab70(Cls_14017a6c0* l);                 // delete every node, reset
void* fun_14017aa80(Cls_14017a6c0* l, std::int32_t n); // node n (1-based) or null

// ---- p1-06 (inter-object list Cls_1401fa020, curve clear, object helpers) -- ext_p1_06.cpp
void fun_1401fa080(Cls_1401fa020* l);                 // reset
void fun_1401fade0(Cls_1401fa020* l);                 // delete every node, reset
std::int32_t fun_1401fc040(void* obj);                // LLOBJ_DATA: material id of the object
void fun_14012f480(DYNARY* c);                 // curve: free arrays

// ---- p1-08 (material list MaterialList, material Cls_14028c6f0) -- ext_p1_08.cpp
void fun_14029b500(MaterialList* l);                 // reset
void fun_14029c310(MaterialList* l);                 // delete every node, reset
Material* fun_14029c260(MaterialList* l, std::int32_t n); // node n (1-based) or null
Material* fun_14029c2c0(MaterialList* l, std::int32_t id); // node whose int at 0 == id, or null
std::int32_t fun_14029b300(Material* matl);               // material: count (0 when +0x2154 == 0)
std::int32_t fun_14029bcd0(MaterialList* l, const std::int32_t* id, std::int32_t* out);

// ---- p1-10 (inter-material list PhasePairList, furnace model Cls_1403753f0) -- ext_p1_10.cpp
void fun_140339cd0(PhasePairList* l);                 // reset
void fun_14033a120(PhasePairList* l);                 // delete every node, reset
void fun_140375520(Cls_1403753f0* f);                 // clear the curve list
void fun_140375600(Cls_1403753f0* f, const std::int32_t* n); // build the curves
void fun_140375e90(Cls_1403753f0* f);

// ---- KEY I/O helpers thread (DYNARY init family) -- ext_curve.cpp
void fun_14012d2a0(DYNARY* c, std::int32_t type); // init for a type code
void fun_14012fad0(DYNARY* c);                    // allocate the value arrays

} // namespace def::model::ext
