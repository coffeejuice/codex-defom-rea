// defcmd/src/dxf/ext.cpp: default bodies of the p8-04 seam (see ext.hpp).
#include "ext.hpp"

#include <bit>
#include <cmath>
#include <new>
#include <type_traits>

#include <defcmd/dxf.hpp>
#include <defrt/stdlib/libm.hpp>

namespace def::cmd::dxf::ext {

#define DEF_P804_ROUTE(owner_macro, ns, addr, ...)                                 \
    do {                                                                           \
        if (g_##addr != nullptr) return g_##addr(__VA_ARGS__);                     \
        DEF_P804_OWNER_##owner_macro(ns, addr, __VA_ARGS__)                        \
        DEF_NOT_PORTED(0x##addr);                                                  \
    } while (0)

#ifdef DEF_P804_HAVE_P801
#define DEF_P804_OWNER_P801(ns, addr, ...) return static_cast<decltype(g_##addr(__VA_ARGS__))>(call_owner(&ns::fun_##addr, __VA_ARGS__));
#else
#define DEF_P804_OWNER_P801(ns, addr, ...)
#endif
#ifdef DEF_P804_HAVE_P803
#define DEF_P804_OWNER_P803(ns, addr, ...) return static_cast<decltype(g_##addr(__VA_ARGS__))>(call_owner(&ns::fun_##addr, __VA_ARGS__));
#else
#define DEF_P804_OWNER_P803(ns, addr, ...)
#endif
#ifdef DEF_P804_HAVE_P809
#define DEF_P804_OWNER_P809(ns, addr, ...) return static_cast<decltype(g_##addr(__VA_ARGS__))>(call_owner(&ns::fun_##addr, __VA_ARGS__));
#else
#define DEF_P804_OWNER_P809(ns, addr, ...)
#endif

// The Uic2CObjectIgesFile on 1404093a0's stack: p8-03's class when present (its C++ ctor/dtor
// run the 1404099d0 / 140409a90 bodies), else zeroed storage: what 1404099d0 leaves in the three
// fields the DXF path uses (+0x18, +0x1c, +0x20 = 0; FUN_140409a50).
void f1404099d0(IgesHelper* h)
{
    if (g_1404099d0 != nullptr) return g_1404099d0(h);
#ifdef DEF_P804_HAVE_P803
    static_assert(sizeof(def::cmd::iges::Uic2CObjectIgesFile) <= sizeof h->raw);
    new (h->raw) def::cmd::iges::Uic2CObjectIgesFile(nullptr);
#else
    for (unsigned char& b : h->raw) b = 0;
#endif
}
void f140409a90(IgesHelper* h)
{
    if (g_140409a90 != nullptr) return g_140409a90(h);
#ifdef DEF_P804_HAVE_P803
    std::launder(reinterpret_cast<def::cmd::iges::Uic2CObjectIgesFile*>(h->raw))->~Uic2CObjectIgesFile();
#endif
}

// 140409330 (p8-01): when p8-01 is absent the Uic2CObjectDxfFile ctor's own stores stand.
void f140409330(def::cmd::Uic2CObjectDxfFile* d)
{
    if (g_140409330 != nullptr) return g_140409330(d);
#ifdef DEF_P804_HAVE_P801
    def::cmd::gfread::fun_140409330(d);
#else
    (void)d;
#endif
}

// 1404098d0 / 140409930 move a double (XMM1 / XMM0); p8-01's header declares the field as void*,
// so the 8 bytes are passed through std::bit_cast (identical contents).
template <class F>
void store_3a48(F f, LLOBJ_DATA* obj, double v)
{
    if constexpr (std::is_invocable_v<F, LLOBJ_DATA*, double>) {
        f(obj, v);
    } else {
        f(obj, std::bit_cast<void*>(v));
    }
}
template <class F>
double load_3a58(F f, LLOBJ_DATA* obj)
{
    if constexpr (std::is_pointer_v<std::invoke_result_t<F, LLOBJ_DATA*>>) {
        return std::bit_cast<double>(f(obj));
    } else {
        return static_cast<double>(f(obj));
    }
}
void f1404098d0(LLOBJ_DATA* obj, double v)
{
    if (g_1404098d0 != nullptr) return g_1404098d0(obj, v);
#ifdef DEF_P804_HAVE_P801
    return store_3a48(&def::cmd::gfread::fun_1404098d0, obj, v);
#else
    DEF_NOT_PORTED(0x1404098d0);
#endif
}
double f140409930(LLOBJ_DATA* obj)
{
    if (g_140409930 != nullptr) return g_140409930(obj);
#ifdef DEF_P804_HAVE_P801
    return load_3a58(&def::cmd::gfread::fun_140409930, obj);
#else
    DEF_NOT_PORTED(0x140409930);
#endif
}

std::int64_t f140197bc0(LLOBJ_DATA* obj) { DEF_P804_ROUTE(P801, def::cmd::gfread, 140197bc0, obj); }
void f14019a650(LLOBJ_DATA* obj, std::int32_t v) { DEF_P804_ROUTE(P801, def::cmd::gfread, 14019a650, obj, v); }
std::int32_t f1402566b0(IgesHelper* h) { DEF_P804_ROUTE(P801, def::cmd::gfread, 1402566b0, h); }
std::int32_t f1404098f0(LLOBJ_DATA* obj) { DEF_P804_ROUTE(P801, def::cmd::gfread, 1404098f0, obj); }
void f14040cde0(OBJ_GEO2D* curve, double* data) { DEF_P804_ROUTE(P801, def::cmd::gfread, 14040cde0, curve, data); }
void f14040ce40(IgesHelper* h) { DEF_P804_ROUTE(P801, def::cmd::gfread, 14040ce40, h); }
void f14040ce60(IgesHelper* h, std::int32_t v) { DEF_P804_ROUTE(P801, def::cmd::gfread, 14040ce60, h, v); }

int f140409af0(def::cmd::iges::Uic2CObjectIgesFile* iges, const char* name, const char* prefix, LLOBJ_DATA* obj,
               double* p5, std::int32_t p6, char p7, std::int32_t p8, std::int32_t mode)
{
    if (g_140409af0 != nullptr) return g_140409af0(iges, name, prefix, obj, p5, p6, p7, p8, mode);
#ifdef DEF_P804_HAVE_P803
    return def::cmd::iges::fun_140409af0(iges, name, prefix, obj, p5, p6, p7, p8, mode);
#else
    DEF_NOT_PORTED(0x140409af0);
#endif
}
int f14040c220(char* buf, void* ctx, std::int32_t kind, std::int32_t mode)
{
    DEF_P804_ROUTE(P803, def::cmd::iges, 14040c220, buf, ctx, kind, mode);
}
void f140463374(double* ctrl, double* knots, double* w, std::int32_t* nc, std::int32_t* degree,
                double* out, std::int32_t* nout, std::int32_t* idx, double* t0, double* tn)
{
    DEF_P804_ROUTE(P803, def::cmd::iges, 140463374, ctrl, knots, w, nc, degree, out, nout, idx, t0, tn);
}

void f140198270(LLOBJ_DATA* obj) { DEF_P804_ROUTE(P809, def::cmd::geolib, 140198270, obj); }

// MSVC CRT atan 1404865ac / tan 140487b70 / atan2 140485d6c: the defrt libm ports (bit-exact
// against the mapped binary, libmdiff_crt2.cpp; tan: scripts_p101/libmdiff_tan.cpp)
double crt_atan(double x) { return g_1404865ac != nullptr ? g_1404865ac(x) : def::rt::libm::crt_atan(x); }
double crt_tan(double x) { return g_140487b70 != nullptr ? g_140487b70(x) : def::rt::libm::crt_tan(x); }
double crt_atan2(double y, double x) { return g_140485d6c != nullptr ? g_140485d6c(y, x) : def::rt::libm::crt_atan2(y, x); }

} // namespace def::cmd::dxf::ext
