// defmodel/node_access.hpp: accessors of the node state-variable set Cls_14017ef20
// (LLOBJ_DATA+0x8a0) that allocate a missing array before returning it (packet p67-3).
// Spec of the "layout sync" 1401d48d0: docs/re/model/node_access_1401d48d0.md.
//
// The set's owner u130 (set+0x130) is the model DB_STEP_DATA; its component table
// DB_STEP_DATA::u15f8 (0x4f ints, indexed by state-variable id) gives the per-node width of
// every array. An accessor:
//   1. dims = fun_14017f020(set)                 (owner + 0x15f8, or null without owner)
//   2. fun_14017ffe0(set, set->n, dims, key)     (= bc::node_set_alloc: allocate if missing)
//   3. owner != 0: fun_1401d48d0(owner, &key)    (no effect: 1401d2b70 always returns 1)
//   4. return the data slot of `key`.
#pragma once

#include <cstdint>

#include <defmodel/bc/node_arrays.hpp>
#include <defmodel/llobj/cls_14017ef20.hpp>

namespace def::model {

class DB_STEP_DATA;

// 14017f020: the owner's component table (owner + 0x15f8 = DB_STEP_DATA::u15f8), null when the
// set has no owner (set+0x130 == 0).
std::int32_t* fun_14017f020(Cls_14017ef20* set);

// 14017ffe0: allocate the set's arrays for n nodes (coordinates set+8 when missing and key is
// 0, -1 or -2; the six int BC arrays and n = set+0x12c for key -1 / -2; then the state variables
// whose id is key, all for -1, through 140329e60). Ported as bc::node_set_alloc (@src there).
inline void fun_14017ffe0(Cls_14017ef20* set, std::int32_t n, const std::int32_t* dims, std::int32_t key)
{
    bc::node_set_alloc(set, n, dims, key);
}

// 1401808d0: node coordinates (set+8 = u008, n x dims[0] doubles), allocated when missing.
double* fun_1401808d0(Cls_14017ef20* set);

// 1401809c0: data of state variable id 2 (slot set+0x18 = u010[1], n x dims[2] doubles) after
// resizing that variable to the set's node count (140329e60 with key 2). URZ in GENBCC.md.
double* fun_1401809c0(Cls_14017ef20* set);

// 1401d48d0: rebuild the model's object arrays for the state variable or name code *which
// (-1: all) from the database (1401d4400 / 1401cf930 / 1401cfc30), only when 1401d2b70
// reports them out of date. 1401d2b70 always returns 1, so the call changes nothing.
void fun_1401d48d0(DB_STEP_DATA* model, const std::int32_t* which);

// ---- unreachable callees of 1401d48d0 (src/node_access/pending.cpp, DEF_NOT_PORTED) ----------
// 1401d4400(model, &id, &flag): read one state-variable array from the DB into the objects
// ("ReadSingleAArrayFromDB"); 1211 bytes, not ported.
void fun_1401d4400(DB_STEP_DATA* model, const std::int32_t* id, const char* flag);
// 1401cf930 / 1401cfc30: rebuild the node / element name arrays; not ported (name_arrays.cpp
// reaches them through the same dead 1401d2b70 branch).
void fun_1401cf930(DB_STEP_DATA* model);
void fun_1401cfc30(DB_STEP_DATA* model);

}  // namespace def::model
