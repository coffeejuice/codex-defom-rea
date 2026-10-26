// p8-02 seam to p8-09 (geometry support library, origin/claude/p8-09, defcmd/geolib.hpp,
// namespace def::cmd::geolib; header 049f49f, bodies df8a426). The wrappers convert to the
// owner's types (geolib::CellList / FltPointList / SurfCheck / TokenReader / DEF_AMGGEO, the same
// layouts as stl_types.hpp) with owner_call. Without the header every entry is DEF_NOT_PORTED.
#pragma once

#include <cstdio>

#include "ext_common.hpp"
#include "stl_types.hpp"

#if __has_include(<defcmd/geolib.hpp>)
#include <defcmd/geolib.hpp>
#define DEF_P802_HAS_GEOLIB 1
#endif

namespace def::model {
class LLOBJ_DATA;
}

namespace def::cmd::stlnas::ext {

using def::model::LLOBJ_DATA;
using DVec = def::model::CowVector<double>;

#if defined(DEF_P802_HAS_GEOLIB)
namespace wire809 {
namespace g = def::cmd::geolib;
inline void w140202700(LLOBJ_DATA* o, i32 nn, i32 ne, i32 k, i32* comp, i32 d) { owner_call(&g::fun_140202700, o, nn, ne, k, comp, d); }
inline void w140246d00(void* geo) { owner_call(&g::fun_140246d00, geo); }
inline void w140246d40(void* geo) { owner_call(&g::fun_140246d40, geo); }
inline bool w140246f70(void* geo, const char* f, char* c, i32 m, LLOBJ_DATA* o) { return pcast<bool>(owner_call(&g::fun_140246f70, geo, f, c, m, o)); }
inline bool w14024bac0(void* geo, LLOBJ_DATA* o, i32 m) { return pcast<bool>(owner_call(&g::fun_14024bac0, geo, o, m)); }
inline void w140256800(DVec* v, const double* x) { owner_call(&g::fun_140256800, v, x); }
inline float* w1402570e0(float* p, i32 n) { return pcast<float*>(owner_call(&g::fun_1402570e0, p, n)); }
inline void w14040e620(SurfCheck* s, PointList* p, CellList* c, float a) { owner_call(&g::fun_14040e620, s, p, c, a); }
inline void w14040e810(SurfCheck* s) { owner_call(&g::fun_14040e810, s); }
inline void w14040fde0(SurfCheck* s) { owner_call(&g::fun_14040fde0, s); }
inline void w140410fd0(PointList* p, i32 cap, i32 grow) { owner_call(&g::fun_140410fd0, p, cap, grow); }
inline void w140411080(PointList* p) { owner_call(&g::fun_140411080, p); }
inline i32 w1404115d0(PointList* p, const float* x) { return pcast<i32>(owner_call(&g::fun_1404115d0, p, x)); }
inline i32 w140411620(PointList* p) { return pcast<i32>(owner_call(&g::fun_140411620, p)); }
inline void w140411920(TokenReader* r) { owner_call(&g::fun_140411920, r); }
inline void w140411940(TokenReader* r) { owner_call(&g::fun_140411940, r); }
inline i32 w140411ad0(TokenReader* r, std::FILE* f, i32 t, void* out, i32 pos) { return pcast<i32>(owner_call(&g::fun_140411ad0, r, f, t, out, pos)); }
}  // namespace wire809
#define DEF_P802_809(addr, R, ...) P802_EXT(addr, &wire809::w##addr, R, __VA_ARGS__)
#else
#define DEF_P802_809(addr, R, ...) P802_EXT(addr, (&not_ported<0x##addr, R, __VA_ARGS__>), R, __VA_ARGS__)
#endif

// 140202700  mesh allocation of an object (nn nodes, ne elements, element kind, component table)
DEF_P802_809(140202700, void, LLOBJ_DATA*, i32, i32, i32, i32*, i32);
// 140246d00 / 140246d40  _DEF_AMGGEO ctor / dtor (object of 0x40 bytes)
DEF_P802_809(140246d00, void, void*);
DEF_P802_809(140246d40, void, void*);
// 140246f70  GEO read dispatch (mode 2/4: 2D reader, else 3D reader)
DEF_P802_809(140246f70, bool, void*, const char*, char*, i32, LLOBJ_DATA*);
// 14024bac0  GEO store dispatch
DEF_P802_809(14024bac0, bool, void*, LLOBJ_DATA*, i32);
// 140256800  push_back of a double vector
DEF_P802_809(140256800, void, DVec*, const double*);
// 1402570e0  realloc(p, n floats), NULL for n == 0
DEF_P802_809(1402570e0, float*, float*, i32);
// 14040e620 / 14040e810  surface checker ctor (points, cells, small area) / dtor
DEF_P802_809(14040e620, void, SurfCheck*, PointList*, CellList*, float);
DEF_P802_809(14040e810, void, SurfCheck*);
// 14040fde0  surface checker: node -> cells links
DEF_P802_809(14040fde0, void, SurfCheck*);
// 140410fd0 / 140411080  point list init (cap, grow) / free
DEF_P802_809(140410fd0, void, PointList*, i32, i32);
DEF_P802_809(140411080, void, PointList*);
// 1404115d0  append a point (float[3]); returns its 1-based id
DEF_P802_809(1404115d0, i32, PointList*, const float*);
// 140411620  point count
DEF_P802_809(140411620, i32, PointList*);
// 140411920 / 140411940  token reader ctor / dtor; 140411ad0 token reader (type, out, pos)
DEF_P802_809(140411920, void, TokenReader*);
DEF_P802_809(140411940, void, TokenReader*);
DEF_P802_809(140411ad0, i32, TokenReader*, std::FILE*, i32, void*, i32);

#undef DEF_P802_809

inline void reset_ext_p8_09()
{
    fun_140202700 = default_140202700;
    fun_140246d00 = default_140246d00;
    fun_140246d40 = default_140246d40;
    fun_140246f70 = default_140246f70;
    fun_14024bac0 = default_14024bac0;
    fun_140256800 = default_140256800;
    fun_1402570e0 = default_1402570e0;
    fun_14040e620 = default_14040e620;
    fun_14040e810 = default_14040e810;
    fun_14040fde0 = default_14040fde0;
    fun_140410fd0 = default_140410fd0;
    fun_140411080 = default_140411080;
    fun_1404115d0 = default_1404115d0;
    fun_140411620 = default_140411620;
    fun_140411920 = default_140411920;
    fun_140411940 = default_140411940;
    fun_140411ad0 = default_140411ad0;
}

}  // namespace def::cmd::stlnas::ext
