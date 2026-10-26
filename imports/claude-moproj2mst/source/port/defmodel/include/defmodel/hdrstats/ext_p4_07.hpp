// defmodel/hdrstats/ext_p4_07.hpp: calls from the step-header packet p4-07 into functions owned by
// packets that are not on this branch's base (claude/p1-gate): the KEY state-variable port
// (kw-g2sv, branch claude/project-thread-3dav7e), the geometry KEY port (kw-g2geo), the other
// Phase 4 packets (p4-02a vector helpers, p4-02b, p4-03, p4-05) and Phase 2 getters.
//
// Each call goes through a function pointer named after the callee's address (the p1-07
// pending.hpp scheme). The default target aborts in DEF_NOT_PORTED; tier-1 tests install fakes
// and call reset_ext_p4_07(); when the owner lands, the merge points the pointer at the owner's
// function (one line per entry; docs/re/status/p4-07.tsv lists the owners).
// Signatures follow the decompile; `set` is the node state-variable set LLOBJ_DATA+0x8a0
// (Cls_14017ef20) or the element set LLOBJ_DATA+0x9e0 (Cls_140182880), passed untyped as the
// owners' statevar_kw API does.
#pragma once

#include <cstdint>

#include <defmodel/port.hpp>

namespace def::model {
class DB_STEP_DATA;
class LLOBJ_DATA;
} // namespace def::model

namespace def::model::p407 {

// 3-vector returned by value (0x18 bytes) by the vector helpers 140239700 / 14023a1b0 / 14023a4b0.
struct Vec3 {
    double v[3];
};

template <std::uint64_t Address, class R, class... A>
R ext_not_ported(A...)
{
    DEF_NOT_PORTED(Address);
}

// Adapters to the owners already on this branch (claude/kw-merge): src/hdrstats/ext_wiring.cpp.
// The pointers of those callees start at these; reset_ext_p4_07() restores them.
namespace wired {
std::int32_t fun_140158ce0(const LLOBJ_DATA*);
bool fun_140160d90(const LLOBJ_DATA*);
bool fun_140160f10(const LLOBJ_DATA*);
void* fun_140198620(LLOBJ_DATA*, const std::int32_t*);
std::int32_t* fun_140005510(void*);
double* fun_140006410(void*);
void fun_14017ffe0(void*, std::int32_t, std::int32_t*, std::int32_t);
void fun_1401836c0(void*, std::int32_t, std::int32_t*, std::int32_t, std::int32_t);
std::int32_t fun_1401cbbc0(DB_STEP_DATA*);
std::int32_t fun_1401cbc80(DB_STEP_DATA*);
double* fun_140257150(std::int32_t);
std::int32_t* fun_14017f020(void*);
void fun_1401d48d0(DB_STEP_DATA*, const std::int32_t*);
std::int32_t* fun_140182980(void*);
} // namespace wired

namespace ext {

// ---- LLOBJ_DATA getters ----
// 140158ce0  int at +0x39c0 (kw-g2geo)
inline std::int32_t (*fun_140158ce0)(const LLOBJ_DATA*) = &wired::fun_140158ce0;
// 140160c20  object has no mesh to report (p4-02b)
inline bool (*fun_140160c20)(LLOBJ_DATA*) = &ext_not_ported<0x140160c20, bool, LLOBJ_DATA*>;
// 140160d90  byte at +0x3990
inline bool (*fun_140160d90)(const LLOBJ_DATA*) = &wired::fun_140160d90;
// 140160f10  object type (+0xc) != 7 (kw-g2mg / kw-g4b)
inline bool (*fun_140160f10)(const LLOBJ_DATA*) = &wired::fun_140160f10;
// 140163540  volume (p4-03)
inline double (*fun_140163540)(LLOBJ_DATA*, std::int32_t) = &ext_not_ported<0x140163540, double, LLOBJ_DATA*, std::int32_t>;
// 140198620  u3c00[*i] (kw-g2geo)
inline void* (*fun_140198620)(LLOBJ_DATA*, const std::int32_t*) = &wired::fun_140198620;

// ---- geometry object LLOBJ_DATA::u3c00[0], an _OBJ_GEO3D (kw-g2geo) ----
// 140005510  first element (4 node numbers each)
inline std::int32_t* (*fun_140005510)(void*) = &wired::fun_140005510;
// 140006410  first coordinate
inline double* (*fun_140006410)(void*) = &wired::fun_140006410;

// ---- node set LLOBJ_DATA+0x8a0 (kw-g2sv) ----
// 14017f020  dims: owner model +0x15f8, or null
inline std::int32_t* (*fun_14017f020)(void*) = &wired::fun_14017f020;
// 14017ffe0  (re)allocate for variable id
inline void (*fun_14017ffe0)(void*, std::int32_t, std::int32_t*, std::int32_t) = &wired::fun_14017ffe0;
// 1401808d0  coordinates (p4-03)
inline double* (*fun_1401808d0)(void*) = &ext_not_ported<0x1401808d0, double*, void*>;
// 1401812a0  nodal damage
inline double* (*fun_1401812a0)(void*) = &ext_not_ported<0x1401812a0, double*, void*>;
// 1401814b0  TIMERC
inline double* (*fun_1401814b0)(void*) = &ext_not_ported<0x1401814b0, double*, void*>;
// 140181530  NRECVL
inline double* (*fun_140181530)(void*) = &ext_not_ported<0x140181530, double*, void*>;
// 1401815b0  USRNOD
inline double* (*fun_1401815b0)(void*) = &ext_not_ported<0x1401815b0, double*, void*>;

// ---- element set LLOBJ_DATA+0x9e0 (kw-g2sv) ----
// 140182980  dims: owner model +0x15f8, or null
inline std::int32_t* (*fun_140182980)(void*) = &wired::fun_140182980;
// 1401836c0  (re)allocate for variable id
inline void (*fun_1401836c0)(void*, std::int32_t, std::int32_t*, std::int32_t, std::int32_t) = &wired::fun_1401836c0;
// 140184050  relative density (p4-03)
inline double* (*fun_140184050)(void*) = &ext_not_ported<0x140184050, double*, void*>;
// 1401840d0  element damage
inline double* (*fun_1401840d0)(void*) = &ext_not_ported<0x1401840d0, double*, void*>;
// 140184150  MATAXI (mark the DB variable when *mark)
inline double* (*fun_140184150)(void*, const bool*) = &ext_not_ported<0x140184150, double*, void*, const bool*>;
// 140184a30  USRELM
inline double* (*fun_140184a30)(void*) = &ext_not_ported<0x140184a30, double*, void*>;

// ---- DB_STEP_DATA ----
// 1401d48d0  mark DB variable *id as used
inline void (*fun_1401d48d0)(DB_STEP_DATA*, const std::int32_t*) = &wired::fun_1401d48d0;
// 1401cbbc0  number of user node variables (+0x3398)
inline std::int32_t (*fun_1401cbbc0)(DB_STEP_DATA*) = &wired::fun_1401cbbc0;
// 1401cbc80  number of user element variables (+0x33a8)
inline std::int32_t (*fun_1401cbc80)(DB_STEP_DATA*) = &wired::fun_1401cbc80;
// 1401cc180  step version (+0x1b64)
inline std::int32_t (*fun_1401cc180)(DB_STEP_DATA*) = &ext_not_ported<0x1401cc180, std::int32_t, DB_STEP_DATA*>;
// 14019a2f0  fold-info flag (+0xcc, p4-05)
inline std::int32_t (*fun_14019a2f0)(DB_STEP_DATA*) = &ext_not_ported<0x14019a2f0, std::int32_t, DB_STEP_DATA*>;

// ---- helpers ----
// 140324000  platform code of the text in the step header (returns 2)
inline std::int32_t (*fun_140324000)() = &ext_not_ported<0x140324000, std::int32_t>;
// 140257150  calloc(n, 8); 0 -> null
inline double* (*fun_140257150)(std::int32_t) = &wired::fun_140257150;

// ---- p4-02a: 3-vector helpers ----
// 140239700  (x, y, z)
inline Vec3* (*fun_140239700)(Vec3*, double, double, double) = &ext_not_ported<0x140239700, Vec3*, Vec3*, double, double, double>;
// 140239a30  length: sqrt((x*x + y*y) + z*z) (1402399b0)
inline double (*fun_140239a30)(const Vec3*) = &ext_not_ported<0x140239a30, double, const Vec3*>;
// 14023a1b0  a + b
inline Vec3* (*fun_14023a1b0)(Vec3*, const Vec3*, const Vec3*) = &ext_not_ported<0x14023a1b0, Vec3*, Vec3*, const Vec3*, const Vec3*>;
// 14023a4b0  a x b
inline Vec3* (*fun_14023a4b0)(Vec3*, const Vec3*, const Vec3*) = &ext_not_ported<0x14023a4b0, Vec3*, Vec3*, const Vec3*, const Vec3*>;

} // namespace ext

// Restores every default (tests call it between cases).
void reset_ext_p4_07();

} // namespace def::model::p407
