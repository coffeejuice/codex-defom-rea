// defcmd/src/dxf/ext.hpp: calls from the DXF reader (p8-04) into code other packets own.
//
// One wrapper per callee (binary address in the name). Each wrapper runs, in this order:
//   1. its test hook g_<addr> when a test installed one;
//   2. the owner's fun_<addr> when the owner's API header is present (program build default):
//        p8-01  defcmd/gfread.hpp  (def::cmd::gfread)
//        p8-03  defcmd/iges.hpp    (def::cmd::iges)
//        p8-09  defcmd/geolib.hpp  (def::cmd::geolib)
//      call_owner() converts the arguments to the owner's declared types (pointer -> pointer /
//      reference by reinterpret_cast, numbers by static_cast); the two double-valued object
//      fields that p8-01 declares as void* are converted bit for bit in ext.cpp;
//   3. DEF_NOT_PORTED(addr).
// Ninja does not notice when an owner header appears: touch ext.cpp after a merge.
#pragma once

#include <cstdint>
#include <type_traits>

#include <defrt/port.hpp>

#if __has_include(<defcmd/gfread.hpp>)
#include <defcmd/gfread.hpp>
#define DEF_P804_HAVE_P801 1
#endif
#if __has_include(<defcmd/iges.hpp>)
#include <defcmd/iges.hpp>
#define DEF_P804_HAVE_P803 1
#endif
#if __has_include(<defcmd/geolib.hpp>)
#include <defcmd/geolib.hpp>
#define DEF_P804_HAVE_P809 1
#endif

namespace def::model {
class LLOBJ_DATA;
class OBJ_GEO2D;
} // namespace def::model
namespace def::cmd {
class Uic2CObjectDxfFile;
} // namespace def::cmd
namespace def::cmd::iges {
class Uic2CObjectIgesFile;
} // namespace def::cmd::iges

namespace def::cmd::dxf::ext {

using def::model::LLOBJ_DATA;
using def::model::OBJ_GEO2D;

template <class T, class U>
T conv(U u)
{
    if constexpr (std::is_lvalue_reference_v<T>) {
        return *reinterpret_cast<std::remove_reference_t<T>*>(u);
    } else if constexpr (std::is_pointer_v<T> && std::is_pointer_v<U>) {
        return reinterpret_cast<T>(u);
    } else {
        return static_cast<T>(u);
    }
}
template <class R, class... P, class... A>
R call_owner(R (*f)(P...), A... a)
{
    static_assert(sizeof...(P) == sizeof...(A), "owner signature has a different argument count");
    return f(conv<P>(a)...);
}

// The Uic2CObjectIgesFile that 1404093a0 builds on its stack (class: p8-03, defcmd/iges.hpp;
// ctor/dtor bodies 1404099d0 / 140409a90). Raw storage of the class size (0x28; vptr, base
// 0x08..0x17, +0x18 int, +0x1c int boundary count, +0x20 double); f1404099d0 constructs a real
// iges::Uic2CObjectIgesFile in it when p8-03's header is present, else zeroes it (the three
// fields the DXF path uses are 0 after the ctor).
struct IgesHelper {
    alignas(8) unsigned char raw[0x28];
    // the object in the storage
    def::cmd::iges::Uic2CObjectIgesFile* obj() { return reinterpret_cast<def::cmd::iges::Uic2CObjectIgesFile*>(raw); }
};

// ---- p8-01 -------------------------------------------------------------------------------------
// 1404099d0 Uic2CObjectIgesFile ctor / 140409a90 its dtor (p8-01's bodies, run by p8-03's C++
// ctor/dtor; see IgesHelper)
inline void (*g_1404099d0)(IgesHelper*) = nullptr;
inline void (*g_140409a90)(IgesHelper*) = nullptr;
void f1404099d0(IgesHelper* h);
void f140409a90(IgesHelper* h);
// 140197bc0 undo snapshot of the object's curves
inline std::int64_t (*g_140197bc0)(LLOBJ_DATA*) = nullptr;
std::int64_t f140197bc0(LLOBJ_DATA* obj);
// 14019a650 obj+0xbc8 = v
inline void (*g_14019a650)(LLOBJ_DATA*, std::int32_t) = nullptr;
void f14019a650(LLOBJ_DATA* obj, std::int32_t v);
// 1402566b0 iges+0x1c
inline std::int32_t (*g_1402566b0)(IgesHelper*) = nullptr;
std::int32_t f1402566b0(IgesHelper* h);
// 140409330 Uic2CObjectDxfFile +0x18 = +0x1c = 0, +0x20 = 0 (run by the Uic2CObjectDxfFile ctor)
inline void (*g_140409330)(def::cmd::Uic2CObjectDxfFile*) = nullptr;
void f140409330(def::cmd::Uic2CObjectDxfFile* d);
// 1404098d0 obj+0x3a48 = v (MOVSD from XMM1: a double; p8-01 declares void*, see ext.cpp)
inline void (*g_1404098d0)(LLOBJ_DATA*, double) = nullptr;
void f1404098d0(LLOBJ_DATA* obj, double v);
// 1404098f0 -> 140409910: int obj+0x3a50 (user tolerance flag)
inline std::int32_t (*g_1404098f0)(LLOBJ_DATA*) = nullptr;
std::int32_t f1404098f0(LLOBJ_DATA* obj);
// 140409930 double obj+0x3a58 (user tolerance; MOVSD to XMM0; p8-01 declares void*)
inline double (*g_140409930)(LLOBJ_DATA*) = nullptr;
double f140409930(LLOBJ_DATA* obj);
// 14040cde0 curve+0x18 = data (the curve's point array)
inline void (*g_14040cde0)(OBJ_GEO2D*, double*) = nullptr;
void f14040cde0(OBJ_GEO2D* curve, double* data);
// 14040ce40 -> 14040ce20(iges, 1): iges+0x18 = 1
inline void (*g_14040ce40)(IgesHelper*) = nullptr;
void f14040ce40(IgesHelper* h);
// 14040ce60 iges+0x1c = v
inline void (*g_14040ce60)(IgesHelper*, std::int32_t) = nullptr;
void f14040ce60(IgesHelper* h, std::int32_t v);

// ---- p8-03 -------------------------------------------------------------------------------------
// 140409af0 IGES reader (DXF modes other than 300), p8-03's signature: (Uic2CObjectIgesFile*, const
// char* name, const char* prefix, LLOBJ_DATA*, double* p5, int32 p6, char p7, int32 p8, int32 mode);
// the DXF call passes (iges, 0, 0, obj, &tol, 2, (char)0, 1, mode), listing 140409700..140409748
inline int (*g_140409af0)(def::cmd::iges::Uic2CObjectIgesFile*, const char*, const char*, LLOBJ_DATA*, double*,
                          std::int32_t, char, std::int32_t, std::int32_t) = nullptr;
int f140409af0(def::cmd::iges::Uic2CObjectIgesFile* iges, const char* name, const char* prefix, LLOBJ_DATA* obj,
               double* p5, std::int32_t p6, char p7, std::int32_t p8, std::int32_t mode);
// 14040c220 Fortran conversion driver: (name buffer, &DAT_14070bc20, 2, mode); mode 700 = DXF
// (duplicate removal 14040c740, then the chaining stage 140457312). Returns 0, 200, -9, -10, -11.
inline int (*g_14040c220)(char*, void*, std::int32_t, std::int32_t) = nullptr;
int f14040c220(char* buf, void* ctx, std::int32_t kind, std::int32_t mode);
// 140463374 B-spline sampling (Fortran): (ctrl xyz, knots, weights, &nc, &degree, out xyz,
// &nout, &idx, &t0, &tN)
inline void (*g_140463374)(double*, double*, double*, std::int32_t*, std::int32_t*, double*,
                           std::int32_t*, std::int32_t*, double*, double*) = nullptr;
void f140463374(double* ctrl, double* knots, double* w, std::int32_t* nc, std::int32_t* degree,
                double* out, std::int32_t* nout, std::int32_t* idx, double* t0, double* tn);

// ---- p8-09 -------------------------------------------------------------------------------------
// 140198270 add a curve to geometry 0 (creating the geometry list entry when missing)
inline void (*g_140198270)(LLOBJ_DATA*) = nullptr;
void f140198270(LLOBJ_DATA* obj);

// ---- MSVC CRT (VS2008 x64) atan / tan / atan2 --------------------------------------------------
// 1404865ac, 140487b70, 140485d6c: def::rt::libm::crt_atan / crt_tan / crt_atan2 (defrt); the hooks
// let tests substitute values.
inline double (*g_1404865ac)(double) = nullptr;
inline double (*g_140487b70)(double) = nullptr;
inline double (*g_140485d6c)(double, double) = nullptr;
double crt_atan(double x);
double crt_tan(double x);
double crt_atan2(double y, double x);

} // namespace def::cmd::dxf::ext
