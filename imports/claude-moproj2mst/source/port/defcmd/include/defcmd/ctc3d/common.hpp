// defcmd/ctc3d/common.hpp: shared conventions of the GENCTC 3D contact engine (packet p67-7).
//
// Static storage. The engine is PathScale Fortran (SAVE locals and COMMON blocks in static
// storage, thousands of addresses, indexed with the binary's own arithmetic such as
// `obj*3 + inst*300`) plus an MSVC C library with file-scope statics. The port keeps all of it in
// one image of the binary's static sections, addressed by the original address:
//
//     m<i32>(0x14076b6a8 + 4 * (obj * 3 + inst * 300))     // DAT_14076b6a8[...]
//     mp<double>(0x14066cd48)                              // &DAT_14066cd48
//
// so a decompiled access maps one-to-one onto the port, aliasing between COMMON members is the
// original's, and values persist between nodes and between GENCTC calls exactly as in the
// binary. The image covers .data (initialised part and bss) and .rodata; reset_image() zeroes it
// and re-applies the initial values (mem_init.cpp, generated from the exe) for tests.
// Constants that are plain literals in the code (DAT_140bad1xx doubles and the like) should be
// written as literals with the address in a comment; tables indexed at run time go through m<>.
//
// Fortran routines take every argument by pointer (INTEGER = i32, REAL*8 = double, REAL*4 =
// float); arrays are column-major and 1-based in the comments. Elementary math comes from
// defrt/stdlib/libm.hpp (ps_* Fortran runtime, crt_* MSVC CRT) and the FTN_geoutl kernels from
// defrt/ftn/geoutl.hpp; Fortran I/O from defrt/ftn/fio.hpp.
#pragma once

#include <cstddef>
#include <cstdint>

namespace def::model {
class DB_STEP_DATA;
} // namespace def::model

namespace def::cmd::ctc3d {

using i32 = std::int32_t;

// ---- static image ----------------------------------------------------------------------------

inline constexpr std::uint64_t k_data_lo = 0x1405bd000ull; // .data start
inline constexpr std::uint64_t k_data_hi = 0x140b86000ull; // .data end (bss included)
inline constexpr std::uint64_t k_ro_lo = 0x140bad000ull;   // .rodata start (Fortran constants)
inline constexpr std::uint64_t k_ro_hi = 0x140bb3000ull;   // .rodata end

extern std::byte* const g_image_data; // k_data_lo .. k_data_hi
extern std::byte* const g_image_ro;   // k_ro_lo .. k_ro_hi

[[noreturn]] void image_bad_address(std::uint64_t a);

inline std::byte* image_addr(std::uint64_t a)
{
    if (a - k_data_lo < k_data_hi - k_data_lo) {
        return g_image_data + (a - k_data_lo);
    }
    if (a - k_ro_lo < k_ro_hi - k_ro_lo) {
        return g_image_ro + (a - k_ro_lo);
    }
    image_bad_address(a);
}

// Reference to the object of type T at binary address a.
template <class T>
inline T& m(std::uint64_t a)
{
    return *reinterpret_cast<T*>(image_addr(a));
}

// Pointer to the object of type T at binary address a.
template <class T>
inline T* mp(std::uint64_t a)
{
    return reinterpret_cast<T*>(image_addr(a));
}

// Zero the image and re-apply the binary's initial values (start-of-process state).
void reset_image();

// ---- COMMON members other packets name (p67-3 genctc3d_engine.hpp, p67-6 driver) -------------
// Views into the image, so that every access, by name or by address, sees the same storage.

// @data 140778684  index obj + 100*inst: inst 0 = cube instance of object obj; 14005bbf2 writes the curve count at obj + 100*inst
inline i32 (&dat_140778684)[301] = *reinterpret_cast<i32 (*)[301]>(image_addr(0x140778684));
// @data 14077867c  number of partners in dat_140771918
inline i32& dat_14077867c = *reinterpret_cast<i32*>(image_addr(0x14077867c));
// @data 140771918  partner list of the current slave (1-based; [0] unused; the drop zeroes 1..1000)
inline i32 (&dat_140771918)[1001] = *reinterpret_cast<i32 (*)[1001]>(image_addr(0x140771918));
// @data 14076b6a8  number of points per cube, index (obj*3 + inst*300)
inline i32 (&dat_14076b6a8)[903] = *reinterpret_cast<i32 (*)[903]>(image_addr(0x14076b6a8));
// @data 14076e588  number of polygons per cube, index (obj*3 + inst*300)
inline i32 (&dat_14076e588)[903] = *reinterpret_cast<i32 (*)[903]>(image_addr(0x14076e588));
// @data 14076aa28  index obj + 100*inst
inline i32 (&dat_14076aa28)[301] = *reinterpret_cast<i32 (*)[301]>(image_addr(0x14076aa28));
// @data 140771788  index obj + 100*inst
inline i32 (&dat_140771788)[301] = *reinterpret_cast<i32 (*)[301]>(image_addr(0x140771788));
// @data 14076c008  node count of the free body (drop), index obj*3
inline i32 (&dat_14076c008)[303] = *reinterpret_cast<i32 (*)[303]>(image_addr(0x14076c008));
// @data 14076eee8  face count of the free body (drop)
inline i32 (&dat_14076eee8)[303] = *reinterpret_cast<i32 (*)[303]>(image_addr(0x14076eee8));
// @data 14076ea38  face count of a rigid partner (drop)
inline i32 (&dat_14076ea38)[303] = *reinterpret_cast<i32 (*)[303]>(image_addr(0x14076ea38));
// @data 14076bb58  node count of a rigid partner (drop)
inline i32 (&dat_14076bb58)[303] = *reinterpret_cast<i32 (*)[303]>(image_addr(0x14076bb58));
// @data 14076abb8  number of symmetry planes per object (0 in DEF_PRE)
inline i32 (&dat_14076abb8)[101] = *reinterpret_cast<i32 (*)[101]>(image_addr(0x14076abb8));
// @data 14076ad48
inline i32 (&dat_14076ad48)[101] = *reinterpret_cast<i32 (*)[101]>(image_addr(0x14076ad48));
// @data 140676710  passed by 14005150d to 140049361 / 14004946f
inline i32& dat_140676710 = *reinterpret_cast<i32*>(image_addr(0x140676710));
// @data 140676718
inline i32& dat_140676718 = *reinterpret_cast<i32*>(image_addr(0x140676718));

// Message unit of the engine. Written only by DEF_PRE's start-up 14001ce7b and exit 14001d1bf
// (def_pre/src/m_cli.cpp open_neweq_msg, menu_shell.cpp close_neweq_msg, packet p5-01):
// unit = |p| with 0 -> 6; main passes p = -1, so 1 = NEWEQ.MSG. Every reader and writer of the
// address goes through these names (a 0 here would make the Fortran runtime create fort.0).
// @data 14077a7bc  Fortran unit of the 3D contact engine's messages (1 = NEWEQ.MSG)
inline i32& dat_14077a7bc = *reinterpret_cast<i32*>(image_addr(0x14077a7bc));
// @data 14077a7c0  flag next to the message unit: 14001ce7b stores 0; read by 14005bbf2
inline i32& dat_14077a7c0 = *reinterpret_cast<i32*>(image_addr(0x14077a7c0));

} // namespace def::cmd::ctc3d
