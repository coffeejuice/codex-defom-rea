// p8-01 seam to packet p8-09 (geometry library, defcmd/geolib.hpp): the _DEF_AMGGEO stack
// objects of the 2D GEO path in FUN_140244010 (0x40 bytes each; ctor FUN_140246d00, dtor
// FUN_140246d40), the reader FUN_140246f70 and the store FUN_14024bac0. Order: test hook, else
// the owner's code when geolib.hpp is present, else DEF_NOT_PORTED. Signatures as published by
// p8-09 (origin/claude/p8-09 @dfaf4ab, defcmd/geolib.hpp), matching the calls in 140244010:
// 140246f70(RCX geo, RDX file, R8 char* flag, R9D mode 3|4, [rsp+20] obj or 0) -> AL;
// 14024bac0(RCX geo, RDX obj, R8D dim) -> AL; the ctor fun_140246d00 / dtor fun_140246d40 run on
// caller storage (model::DEF_AMGGEO, 0x40 bytes).
#pragma once

#include <cstdint>
#include <new>

#include <defcmd/gfread.hpp>
#include <defrt/port.hpp>

#if __has_include(<defcmd/geolib.hpp>)
#include <defcmd/geolib.hpp>
#define DEF_P801_HAVE_P8_09 1
#endif

namespace def::cmd::gfread::ext {

struct GeoHooks {
    void (*ctor)(void* geo) = nullptr;  // 140246d00
    void (*dtor)(void* geo) = nullptr;  // 140246d40
    bool (*read)(void* geo, const char* file, char* flag, std::int32_t mode, model::LLOBJ_DATA* obj) = nullptr;
    bool (*store)(void* geo, model::LLOBJ_DATA* obj, std::int32_t dim) = nullptr;
};
inline GeoHooks g_geo_hooks;

class GeoOnStack {
public:
    GeoOnStack()
    {
        if (g_geo_hooks.ctor != nullptr) {
            g_geo_hooks.ctor(buf_);
            return;
        }
#ifdef DEF_P801_HAVE_P8_09
        def::cmd::geolib::fun_140246d00(reinterpret_cast<def::cmd::geolib::DEF_AMGGEO*>(buf_));
#else
        DEF_NOT_PORTED(0x140246d00);
#endif
    }
    ~GeoOnStack()
    {
        if (g_geo_hooks.dtor != nullptr) {
            g_geo_hooks.dtor(buf_);
            return;
        }
#ifdef DEF_P801_HAVE_P8_09
        def::cmd::geolib::fun_140246d40(reinterpret_cast<def::cmd::geolib::DEF_AMGGEO*>(buf_));
#endif
    }
    GeoOnStack(const GeoOnStack&) = delete;
    GeoOnStack& operator=(const GeoOnStack&) = delete;
    void* get() { return buf_; }

private:
    alignas(16) unsigned char buf_[0x100];
};

inline bool geo_read(void* geo, const char* file, char* flag, std::int32_t mode, model::LLOBJ_DATA* obj)
{
    if (g_geo_hooks.read != nullptr) return g_geo_hooks.read(geo, file, flag, mode, obj);
#ifdef DEF_P801_HAVE_P8_09
    return def::cmd::geolib::fun_140246f70(reinterpret_cast<def::cmd::geolib::DEF_AMGGEO*>(geo), file, flag, mode,
                                           obj);
#else
    DEF_NOT_PORTED(0x140246f70);
#endif
}

inline bool geo_store(void* geo, model::LLOBJ_DATA* obj, std::int32_t dim)
{
    if (g_geo_hooks.store != nullptr) return g_geo_hooks.store(geo, obj, dim);
#ifdef DEF_P801_HAVE_P8_09
    return def::cmd::geolib::fun_14024bac0(reinterpret_cast<def::cmd::geolib::DEF_AMGGEO*>(geo), obj, dim);
#else
    DEF_NOT_PORTED(0x14024bac0);
#endif
}

} // namespace def::cmd::gfread::ext
