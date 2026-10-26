// kw-g1a: small methods of the simulation-control block (Cls_1401e7b10, S = M+0x750) and of the
// step model that the group 1 reader and writer call. They are one-line accessors on classes other
// packets own; kw-g1a ports them here as free functions so that its readers and writers are
// complete. If the owning packet adds them as methods, these forward to (or are replaced by) those.
#pragma once

#include <cstdint>

#include <defmodel/layout/db_step_data.hpp>

namespace def::key::g1a::ops {

using model::Cls_1401e7b10;
using model::DB_STEP_DATA;
using model::LLOBJ_DATA;

// 1400015e0  M+0x1c == 2
bool is_2d(const DB_STEP_DATA& m);
// 1400015f0  M+0x1c == 3
bool is_3d(const DB_STEP_DATA& m);

// (int) of a double as cvttsd2si computes it: NaN and out-of-range values give INT32_MIN.
std::int32_t trunc_i32(double v);

// 1401f3e90  TRANS[0] = v (S+0x104)
void set_trans0(Cls_1401e7b10& s, std::int32_t v);
// 1401f3eb0  TRANS[1] = v (S+0x108)
void set_trans1(Cls_1401e7b10& s, std::int32_t v);
// 1401f3c10  TRANS[0]
std::int32_t trans0(const Cls_1401e7b10& s);
// 1401f3c30  TRANS[1]
std::int32_t trans1(const Cls_1401e7b10& s);
// 1401f3ff0  SMODE setter (also sets TRANS[0..1] for 1, 2, 3)
void set_smode(Cls_1401e7b10& s, std::int32_t mode);
// 1401f3f80  SMODE recomputed from TRANS[0..1]
void recompute_smode(Cls_1401e7b10& s);

// 1401ee580  time-step flag S+0x240 (the DTTYPE DYNARY's +0x58)
std::int32_t time_flag(const Cls_1401e7b10& s);
// 1401ee5a0  time-step flag setter
void set_time_flag(Cls_1401e7b10& s, const std::int32_t* v);
// 1401ee510  flag = (DSMAX <= 0) when no DTTYPE table is active
void recompute_time_flag(Cls_1401e7b10& s);
// 1401f4390  DSMAX setter (S+0x1e0)
void set_dsmax(Cls_1401e7b10& s, double v);
// 14039d2b0  ODMAX setter (S+0xbc8)
void set_odmax(Cls_1401e7b10& s, double v);
// 1403d8490  ODMAX getter
double get_odmax(const Cls_1401e7b10& s);

// 1401f4100  UNIT: reset the environment tables and the unit-dependent defaults
void reset_unit_defaults(Cls_1401e7b10& s);

// 1401d6ea0  object with id `id` in the object list M+0x1380 (via 14017aaf0), or null
LLOBJ_DATA* find_object(DB_STEP_DATA& m, std::int32_t id);
// 14017aaf0  list walk: first node whose int at +8 equals id (next link at +0x3940)
LLOBJ_DATA* find_in_list(model::Cls_14017a6c0& list, std::int32_t id);

// The double at LLOBJ_DATA+0x22e0 (Z stroke, docs/re/keywords/DSMAX_NSTEP.md), read through the
// opaque sub-object slot s2160 until the object layout names it.
double object_stroke(const LLOBJ_DATA& obj);

} // namespace def::key::g1a::ops
