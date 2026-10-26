// p8-02 seams to Phase 6-7 owners and to functions outside the Phase 8 packets. Each entry is a
// hookable pointer ext::fun_<addr> whose default is the owner's function when the owner's header
// is visible (__has_include), else a DEF_NOT_PORTED stub. Owners:
//   cube group (p67-2, src/ifd/cube/cube_internal.hpp, def::cmd::cube; the layout p8-09 also
//     wires to): defCELL list 1404050b0 140405150 140405540 1404056c0 140405720 1404057a0,
//     realloc ints 140256e20, fatal error 14039d450, allocation message 14039d3a0
//   p67-1 (defcmd/objpos_centre.hpp, def::cmd::objpos): 140163340 1402009f0 1401fc730
//   p67-3 (defmodel/node_access.hpp, def::model): 1401808d0
//   p67-2 (src/ifd/drv/ifd3d.hpp, def::cmd::ifd::elem_conn) or p8-07 (defdbio/gendb3d/obj3d.hpp,
//     def::dbio::g3d::element_connectivity): 140183cb0 element connectivity (p8-07's calls the
//     region sync 1401d5820, still DEF_NOT_PORTED there; p67-2 found the sync a no-op)
//   p67-5 (defcmd/p67_5_gensub.hpp, def::cmd::gensub): 1401b83d0 line reader; 1401b85d0 delete:
//     GENDB (defdbio/gendb/gendb.hpp, def::dbio::gendb::remove_if_exists)
//   p8-07 (defcmd/launch.hpp, def::cmd): 140224130 launcher wrapper (dir kind 3)
//   p4-07 / p8-07 (no public header): 140158d00 object dimension == 2, wired to the defmodel
//     getter 140158ce0 (kw-g2geo) it calls
//   P4 gate (claude/p4-gate, defdbio/gendb/pre_steps.hpp, def::dbio::gendb): 1402059a0 surface
//     normal lists (3D GEO plane ids)
//   unowned: 1401b06e0 (&surface->elems, wired to the OBJ_GEO3D member)
#pragma once

#include <cstdio>

#include <defmodel/geo_kw/geo_kw.hpp>
#include <defmodel/layout/obj_geo3d.hpp>

#include "ext_common.hpp"
#include "stl_types.hpp"

#if __has_include(<defcmd/ifd/cube_lib.hpp>)  // p67-2's cube library (public since p67-7 43a0695)
#include <defcmd/ifd/cube_lib.hpp>
#define DEF_P802_HAS_CUBE 1
#endif
#if __has_include(<defcmd/objpos_centre.hpp>)
#include <defcmd/objpos_centre.hpp>
#define DEF_P802_HAS_P671 1
#endif
#if __has_include(<defmodel/node_access.hpp>)
#include <defmodel/node_access.hpp>
#define DEF_P802_HAS_P673 1
#endif
#if __has_include("../ifd/drv/ifd3d.hpp")
#include "../ifd/drv/ifd3d.hpp"
#define DEF_P802_HAS_OBJ3D 1
#define DEF_P802_CONN_P672 1
#elif __has_include(<defdbio/gendb3d/obj3d.hpp>)
#include <defdbio/gendb3d/obj3d.hpp>
#define DEF_P802_HAS_OBJ3D 1
#endif
#if __has_include(<defdbio/gendb/pre_steps.hpp>)
#include <defdbio/gendb/pre_steps.hpp>
#define DEF_P802_HAS_P4GATE 1
#endif
#if __has_include(<defcmd/p67_5_gensub.hpp>) && __has_include(<defdbio/gendb/gendb.hpp>)
#include <defcmd/p67_5_gensub.hpp>
#include <defdbio/gendb/gendb.hpp>
#define DEF_P802_HAS_P675 1
#endif
#if __has_include(<defcmd/launch.hpp>)
#include <defcmd/launch.hpp>
#define DEF_P802_HAS_LAUNCH 1
#endif

namespace def::cmd::stlnas::ext {

using def::model::LLOBJ_DATA;
using def::model::OBJ_GEO3D;
using ElemVec = def::model::CowVector<std::int32_t>;

namespace wire67 {
#if defined(DEF_P802_HAS_CUBE)
namespace c = def::cmd::cube;
inline c::CellList* cc(CellList* l) { return reinterpret_cast<c::CellList*>(l); }
inline void w1404050b0(CellList* l, i32 cap, i32 inc, i32 w) { c::fun_1404050b0(cc(l), cap, inc, w); }
inline void w140405150(CellList* l) { c::fun_140405150(cc(l)); }
inline i32 w140405540(CellList* l, i32 n, const i32* r) { return c::fun_140405540(cc(l), n, r); }
inline i32 w1404056c0(CellList* l) { return c::fun_1404056c0(cc(l)); }
inline i32* w140405720(CellList* l, i32 k) { return c::fun_140405720(cc(l), k); }
inline CellList* w1404057a0(i32 n) { return reinterpret_cast<CellList*>(c::fun_1404057a0(n)); }
inline i32* w140256e20(i32* p, i32 n) { return c::fun_140256e20(p, n); }
inline void w14039d450(const char* fmt, i32 a, i32 b) { c::fun_14039d450(fmt, a, b); }
inline void w14039d3a0(const char* fmt, const char* a, const char* b, i32 n) { c::fun_14039d3a0(fmt, a, b, n); }
#endif
#if defined(DEF_P802_HAS_P671)
inline void w140163340(LLOBJ_DATA* o, i32 n) { def::cmd::objpos::set_surface_slots(o, n); }
inline void w1402009f0(LLOBJ_DATA* o, i32 nn, i32 ne, i32 b) { def::cmd::objpos::size_surface(o, nn, ne, b); }
inline void w1401fc730(LLOBJ_DATA* o, char* c) { def::cmd::objpos::fun_1401fc730(o, c); }
#endif
#if defined(DEF_P802_HAS_P673)
inline double* w1401808d0(void* set) { return def::model::fun_1401808d0(static_cast<def::model::Cls_14017ef20*>(set)); }
#endif
#if defined(DEF_P802_CONN_P672)
inline i32* w140183cb0(void* set) { return def::cmd::ifd::elem_conn(set); }
#elif defined(DEF_P802_HAS_OBJ3D)
inline i32* w140183cb0(void* set) { return def::dbio::g3d::element_connectivity(set); }
#endif
#if defined(DEF_P802_HAS_P4GATE)
inline void w1402059a0(LLOBJ_DATA* o, i32 flag) { def::dbio::gendb::object_surface_normals(o, static_cast<char>(flag)); }
#endif
#if defined(DEF_P802_HAS_P675)
inline i32 w1401b83d0(char* b, i32 n, std::FILE* f) { return static_cast<i32>(def::cmd::gensub::read_line(b, n, f)); }
inline bool w1401b85d0(const char* name) { return def::dbio::gendb::remove_if_exists(name) != 0; }
#endif
#if defined(DEF_P802_HAS_LAUNCH)
inline void w140224130(i32 mode, const char* cmd) { def::cmd::launch_program_3d(mode, cmd); }
#endif
}  // namespace wire67

// The binary's statically linked VS2008 CRT fscanf / sscanf (FID_conflict_fscanf 140484924,
// sscanf 14047c354): p67-5 ported its _input_l (%d %s %f, '*', 'l') with the old CRT's float
// rounding and its return values (a directive matched before EOF counts, so "endsolid" makes
// fscanf("%*s %*s %f ...") return 0, not -1). Without p67-5 the host CRT is used, which differs
// in both points.
template <class... A>
int crt_fscanf(std::FILE* f, const char* fmt, A*... a)
{
#if defined(DEF_P802_HAS_P675)
    void* const args[] = {static_cast<void*>(a)..., nullptr};
    return def::cmd::gensub::crt_fscanf(f, fmt, args);
#else
    return std::fscanf(f, fmt, a...);
#endif
}
template <class... A>
int crt_sscanf(const char* s, const char* fmt, A*... a)
{
#if defined(DEF_P802_HAS_P675)
    void* const args[] = {static_cast<void*>(a)..., nullptr};
    return def::cmd::gensub::crt_sscanf(s, fmt, args);
#else
    return std::sscanf(s, fmt, a...);
#endif
}

namespace wire67 {
// 1401b06e0: the element vector of a surface (OBJ_GEO3D +0x18)
inline ElemVec* w1401b06e0(OBJ_GEO3D* s) { return &s->elems; }
// 140158d00: 140158ce0(obj) == 2
inline bool w140158d00(LLOBJ_DATA* o) { return def::model::geo_kw::object_dim(*o) == 2; }
}  // namespace wire67

#define P802_NP(addr, R, ...) P802_EXT(addr, (&not_ported<0x##addr, R, __VA_ARGS__>), R, __VA_ARGS__)
#define P802_W(addr, R, ...) P802_EXT(addr, &wire67::w##addr, R, __VA_ARGS__)

#if defined(DEF_P802_HAS_CUBE)
#define P802_CUBE P802_W
#else
#define P802_CUBE P802_NP
#endif
#if defined(DEF_P802_HAS_P671)
#define P802_P671 P802_W
#else
#define P802_P671 P802_NP
#endif
#if defined(DEF_P802_HAS_P673)
#define P802_P673 P802_W
#else
#define P802_P673 P802_NP
#endif
#if defined(DEF_P802_HAS_OBJ3D)
#define P802_OBJ3D P802_W
#else
#define P802_OBJ3D P802_NP
#endif
#if defined(DEF_P802_HAS_P4GATE)
#define P802_P4GATE P802_W
#else
#define P802_P4GATE P802_NP
#endif
#if defined(DEF_P802_HAS_P675)
#define P802_P675 P802_W
#else
#define P802_P675 P802_NP
#endif
#if defined(DEF_P802_HAS_LAUNCH)
#define P802_LAUNCH P802_W
#else
#define P802_LAUNCH P802_NP
#endif

// ---- defCELL record list (cube group) ----
// 1404050b0  init: count 0, cap, width, increment; data = cap*width ints when cap > 0
P802_CUBE(1404050b0, void, CellList*, i32, i32, i32);
// 140405150  free: count = cap = 0, data freed
P802_CUBE(140405150, void, CellList*);
// 140405540  append one record of n ints (fatal when n > width)
P802_CUBE(140405540, i32, CellList*, i32, const i32*);
// 1404056c0  record count
P802_CUBE(1404056c0, i32, CellList*);
// 140405720  record k (1-based; fatal out of range; NULL when empty)
P802_CUBE(140405720, i32*, CellList*, i32);
// 1404057a0  calloc n lists
P802_CUBE(1404057a0, CellList*, i32);
// 140256e20  realloc(p, n ints), NULL for n == 0
P802_CUBE(140256e20, i32*, i32*, i32);
// 14039d450  fatal error: message (fmt with two ints here), SFTC note, exit(1)
P802_CUBE(14039d450, void, const char*, i32, i32);
// 14039d3a0  allocation failure message (fmt, two strings, one int); no exit
P802_CUBE(14039d3a0, void, const char*, const char*, const char*, i32);

// ---- object surface / mesh ----
// 140163340  surface slot count := n (3D: slots n.. cleared)
P802_P671(140163340, void, LLOBJ_DATA*, i32);
// 1402009f0  size surface `bdry` to nn nodes, ne faces (or the iBdryNo message)
P802_P671(1402009f0, void, LLOBJ_DATA*, i32, i32, i32);
// 1401fc730  boundary faces of the mesh -> surface 0 (*compact: renumber the used nodes)
P802_P671(1401fc730, void, LLOBJ_DATA*, char*);
// 1401808d0  coordinate array of the node set obj+0x8a0
P802_P673(1401808d0, double*, void*);
// 140183cb0  connectivity array of the element set obj+0x9e0
P802_OBJ3D(140183cb0, i32*, void*);
// 1401b83d0  fgets-style line read (buf, n, f), returns strlen or 0 at EOF
P802_P675(1401b83d0, i32, char*, i32, std::FILE*);
// 1401b85d0  delete the file if it can be opened
P802_P675(1401b85d0, bool, const char*);
// 140224130  run an external program (mode, command line), dir kind 3
P802_LAUNCH(140224130, void, i32, const char*);
// 1401b06e0  &surface->elems (unowned 15-byte accessor)
P802_W(1401b06e0, ElemVec*, OBJ_GEO3D*);
// 140158d00  object dimension is 2
P802_W(140158d00, bool, LLOBJ_DATA*);
// 1402059a0  surface normal lists / 3D GEO plane ids after the store (P4 gate)
P802_P4GATE(1402059a0, void, LLOBJ_DATA*, i32);

#undef P802_NP
#undef P802_W
#undef P802_CUBE
#undef P802_P671
#undef P802_P673
#undef P802_OBJ3D
#undef P802_P675
#undef P802_LAUNCH
#undef P802_P4GATE

inline void reset_ext_p67()
{
    fun_1404050b0 = default_1404050b0;
    fun_140405150 = default_140405150;
    fun_140405540 = default_140405540;
    fun_1404056c0 = default_1404056c0;
    fun_140405720 = default_140405720;
    fun_1404057a0 = default_1404057a0;
    fun_140256e20 = default_140256e20;
    fun_14039d450 = default_14039d450;
    fun_14039d3a0 = default_14039d3a0;
    fun_140163340 = default_140163340;
    fun_1402009f0 = default_1402009f0;
    fun_1401fc730 = default_1401fc730;
    fun_1401808d0 = default_1401808d0;
    fun_140183cb0 = default_140183cb0;
    fun_1401b83d0 = default_1401b83d0;
    fun_1401b85d0 = default_1401b85d0;
    fun_140224130 = default_140224130;
    fun_1401b06e0 = default_1401b06e0;
    fun_140158d00 = default_140158d00;
    fun_1402059a0 = default_1402059a0;
}

}  // namespace def::cmd::stlnas::ext
