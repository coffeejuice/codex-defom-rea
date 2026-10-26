// p8-03 seam to packet p8-01 (defcmd/gfread.hpp, namespace def::cmd::gfread): the object-field
// accessors, the undo copy and the member bodies of the Uic2CObject family. Order per call: test
// hook, else the owner's function when its header is present, else DEF_NOT_PORTED.
// Signatures as published on origin/claude/p8-01 @771f91e (1404098d0 / 140409930 take doubles).
#pragma once

#include <cstdint>

#include <defrt/port.hpp>

#if __has_include(<defcmd/gfread.hpp>)
#include <defcmd/gfread.hpp>
#define DEF_P803_HAVE_P801 1
#endif

namespace def::model {
class LLOBJ_DATA;
class OBJ_GEO2D;
} // namespace def::model

namespace def::cmd {
class Uic2CObject;
}
namespace def::cmd::iges {
class Uic2CObjectIgesFile;
}

namespace def::cmd::iges::ext {

using def::model::LLOBJ_DATA;

struct P801Hooks {
    std::int64_t (*f140197bc0)(LLOBJ_DATA*) = nullptr;
    void (*f14019a650)(LLOBJ_DATA*, std::int32_t) = nullptr;
    void (*f1404098d0)(LLOBJ_DATA*, double) = nullptr;
    std::int32_t (*f1404098f0)(const LLOBJ_DATA*) = nullptr;
    double (*f140409930)(const LLOBJ_DATA*) = nullptr;
    void (*f14040cde0)(def::model::OBJ_GEO2D*, double*) = nullptr;
    void (*f14040ce20)(Uic2CObjectIgesFile*, std::int32_t) = nullptr;
    void (*f14040ce40)(Uic2CObjectIgesFile*) = nullptr;
    void (*f14040ce60)(Uic2CObjectIgesFile*, std::int32_t) = nullptr;
};
inline P801Hooks g_p801{};

// 140197bc0 undo copy of the 2D curves into obj+0x3a60
inline std::int64_t f140197bc0(LLOBJ_DATA* obj)
{
    if (g_p801.f140197bc0) return g_p801.f140197bc0(obj);
#ifdef DEF_P803_HAVE_P801
    return def::cmd::gfread::fun_140197bc0(obj);
#else
    DEF_NOT_PORTED(0x140197bc0);
#endif
}
// 14019a650 obj+0xbc8 = n
inline void f14019a650(LLOBJ_DATA* obj, std::int32_t n)
{
    if (g_p801.f14019a650) return g_p801.f14019a650(obj, n);
#ifdef DEF_P803_HAVE_P801
    def::cmd::gfread::fun_14019a650(obj, n);
#else
    DEF_NOT_PORTED(0x14019a650);
#endif
}
// 1404098d0 obj+0x3a48 = tolerance
inline void f1404098d0(LLOBJ_DATA* obj, double v)
{
    if (g_p801.f1404098d0) return g_p801.f1404098d0(obj, v);
#ifdef DEF_P803_HAVE_P801
    def::cmd::gfread::fun_1404098d0(obj, v);
#else
    DEF_NOT_PORTED(0x1404098d0);
#endif
}
// 1404098f0 obj+0x3a50 (user tolerance flag)
inline std::int32_t f1404098f0(const LLOBJ_DATA* obj)
{
    if (g_p801.f1404098f0) return g_p801.f1404098f0(obj);
#ifdef DEF_P803_HAVE_P801
    return def::cmd::gfread::fun_1404098f0(obj);
#else
    DEF_NOT_PORTED(0x1404098f0);
#endif
}
// 140409930 obj+0x3a58 (user tolerance)
inline double f140409930(const LLOBJ_DATA* obj)
{
    if (g_p801.f140409930) return g_p801.f140409930(obj);
#ifdef DEF_P803_HAVE_P801
    return def::cmd::gfread::fun_140409930(obj);
#else
    DEF_NOT_PORTED(0x140409930);
#endif
}
// 14040cde0 curve+0x18 = points
inline void f14040cde0(def::model::OBJ_GEO2D* curve, double* pts)
{
    if (g_p801.f14040cde0) return g_p801.f14040cde0(curve, pts);
#ifdef DEF_P803_HAVE_P801
    def::cmd::gfread::fun_14040cde0(curve, pts);
#else
    DEF_NOT_PORTED(0x14040cde0);
#endif
}
// 14040ce20 iges+0x18 = v
inline void f14040ce20(Uic2CObjectIgesFile* p, std::int32_t v)
{
    if (g_p801.f14040ce20) return g_p801.f14040ce20(p, v);
#ifdef DEF_P803_HAVE_P801
    def::cmd::gfread::fun_14040ce20(p, v);
#else
    DEF_NOT_PORTED(0x14040ce20);
#endif
}
// 14040ce40 iges+0x18 = 1
inline void f14040ce40(Uic2CObjectIgesFile* p)
{
    if (g_p801.f14040ce40) return g_p801.f14040ce40(p);
#ifdef DEF_P803_HAVE_P801
    def::cmd::gfread::fun_14040ce40(p);
#else
    DEF_NOT_PORTED(0x14040ce40);
#endif
}
// 14040ce60 iges+0x1c = n (boundary count)
inline void f14040ce60(Uic2CObjectIgesFile* p, std::int32_t n)
{
    if (g_p801.f14040ce60) return g_p801.f14040ce60(p, n);
#ifdef DEF_P803_HAVE_P801
    def::cmd::gfread::fun_14040ce60(p, n);
#else
    DEF_NOT_PORTED(0x14040ce60);
#endif
}
} // namespace def::cmd::iges::ext
