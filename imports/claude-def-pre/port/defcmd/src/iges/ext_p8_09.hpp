// p8-03 seam to packet p8-09 (defcmd/geolib.hpp, namespace def::cmd::geolib): curve bookkeeping
// of geometry 0. Order per call: test hook, else the owner's function when its header is present,
// else DEF_NOT_PORTED.
#pragma once

#include <cstdint>

#include <defrt/port.hpp>

#if __has_include(<defcmd/geolib.hpp>)
#include <defcmd/geolib.hpp>
#define DEF_P803_HAVE_P809 1
#endif

namespace def::model {
class LLOBJ_DATA;
class GEO2D_AND_MESH2D;
} // namespace def::model

namespace def::cmd::iges::ext {

struct P809Hooks {
    void (*f140198270)(def::model::LLOBJ_DATA*) = nullptr;
    void (*f140198230)(def::model::LLOBJ_DATA*, std::int32_t) = nullptr;
    void (*f14027fe20)(def::model::GEO2D_AND_MESH2D*, const std::int32_t*) = nullptr;
};
inline P809Hooks g_p809{};

// 140198270 make sure geometry 0 exists, then add one curve
inline void f140198270(def::model::LLOBJ_DATA* obj)
{
    if (g_p809.f140198270) return g_p809.f140198270(obj);
#ifdef DEF_P803_HAVE_P809
    def::cmd::geolib::fun_140198270(obj);
#else
    DEF_NOT_PORTED(0x140198270);
#endif
}
// 140198230 set the curve count of geometry 0
inline void f140198230(def::model::LLOBJ_DATA* obj, std::int32_t n)
{
    if (g_p809.f140198230) return g_p809.f140198230(obj, n);
#ifdef DEF_P803_HAVE_P809
    def::cmd::geolib::fun_140198230(obj, n);
#else
    DEF_NOT_PORTED(0x140198230);
#endif
}
// 14027fe20 set the edge count of a geometry
inline void f14027fe20(def::model::GEO2D_AND_MESH2D* g, const std::int32_t* n)
{
    if (g_p809.f14027fe20) return g_p809.f14027fe20(g, n);
#ifdef DEF_P803_HAVE_P809
    def::cmd::geolib::fun_14027fe20(g, n);
#else
    DEF_NOT_PORTED(0x14027fe20);
#endif
}

} // namespace def::cmd::iges::ext
