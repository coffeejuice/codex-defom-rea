// geolib seam to Phase 6-7 code (packet p8-09). Owners:
//   p67-1 (claude/p67-1, defcmd/objpos_centre.hpp, def::cmd::objpos):
//     140163340 set_surface_slots, 1402009f0 size_surface
//   p67-2 (claude/p67-2, src/ifd/cube/cube_internal.hpp, def::cmd::cube; same CellList layout):
//     1404050b0 140405150 140405540 1404056c0 1404056e0 140405720 1404057a0 (defCELL),
//     140256e20 (realloc ints), 14039d3a0 (allocation message)
// Program build: the owner's function when its header is present; otherwise DEF_NOT_PORTED.
// Tests may install a hook (hooks win over the owner).
#pragma once

#include <cstdint>

#include <defcmd/geolib.hpp>
#include <defrt/port.hpp>

#if __has_include(<defcmd/objpos_centre.hpp>)
#include <defcmd/objpos_centre.hpp>
#define DEF_GEOLIB_HAVE_P67_1 1
#endif
#if __has_include(<defcmd/ifd/cube_lib.hpp>)  // p67-2's cube library (public since p67-7 43a0695)
#include <defcmd/ifd/cube_lib.hpp>
#define DEF_GEOLIB_HAVE_P67_2 1
#endif

namespace def::cmd::geolib::ext {

struct Hooks {
    void (*set_surface_slots)(LLOBJ_DATA*, std::int32_t) = nullptr;                       // 140163340
    void (*size_surface)(LLOBJ_DATA*, std::int32_t, std::int32_t, std::int32_t) = nullptr; // 1402009f0
    void (*cell_init)(CellList*, std::int32_t, std::int32_t, std::int32_t) = nullptr;     // 1404050b0
    void (*cell_free)(CellList*) = nullptr;                                               // 140405150
    std::int32_t (*cell_append)(CellList*, std::int32_t, const std::int32_t*) = nullptr;   // 140405540
    std::int32_t (*cell_count)(const CellList*) = nullptr;                                // 1404056c0
    std::int32_t (*cell_width)(const CellList*) = nullptr;                                // 1404056e0
    std::int32_t* (*cell_get)(CellList*, std::int32_t) = nullptr;                         // 140405720
    CellList* (*cell_array)(std::int32_t) = nullptr;                                      // 1404057a0
    std::int32_t* (*realloc_int)(std::int32_t*, std::int32_t) = nullptr;                  // 140256e20
    void (*alloc_message)(const char*, const char*, const char*, std::int32_t) = nullptr;  // 14039d3a0
};
inline Hooks g_hooks;

#if DEF_GEOLIB_HAVE_P67_2
inline def::cmd::cube::CellList* cube_cells(CellList* c) { return reinterpret_cast<def::cmd::cube::CellList*>(c); }
inline const def::cmd::cube::CellList* cube_cells(const CellList* c)
{
    return reinterpret_cast<const def::cmd::cube::CellList*>(c);
}
#endif

inline void set_surface_slots(LLOBJ_DATA* o, std::int32_t n)
{
    if (g_hooks.set_surface_slots) return g_hooks.set_surface_slots(o, n);
#if DEF_GEOLIB_HAVE_P67_1
    def::cmd::objpos::set_surface_slots(o, n);
#else
    DEF_NOT_PORTED(0x140163340);
#endif
}

inline void size_surface(LLOBJ_DATA* o, std::int32_t nn, std::int32_t ne, std::int32_t bdry)
{
    if (g_hooks.size_surface) return g_hooks.size_surface(o, nn, ne, bdry);
#if DEF_GEOLIB_HAVE_P67_1
    def::cmd::objpos::size_surface(o, nn, ne, bdry);
#else
    DEF_NOT_PORTED(0x1402009f0);
#endif
}

inline void cell_init(CellList* c, std::int32_t cap, std::int32_t inc, std::int32_t width)
{
    if (g_hooks.cell_init) return g_hooks.cell_init(c, cap, inc, width);
#if DEF_GEOLIB_HAVE_P67_2
    def::cmd::cube::fun_1404050b0(cube_cells(c), cap, inc, width);
#else
    DEF_NOT_PORTED(0x1404050b0);
#endif
}

inline void cell_free(CellList* c)
{
    if (g_hooks.cell_free) return g_hooks.cell_free(c);
#if DEF_GEOLIB_HAVE_P67_2
    def::cmd::cube::fun_140405150(cube_cells(c));
#else
    DEF_NOT_PORTED(0x140405150);
#endif
}

inline std::int32_t cell_append(CellList* c, std::int32_t n, const std::int32_t* rec)
{
    if (g_hooks.cell_append) return g_hooks.cell_append(c, n, rec);
#if DEF_GEOLIB_HAVE_P67_2
    return def::cmd::cube::fun_140405540(cube_cells(c), n, rec);
#else
    DEF_NOT_PORTED(0x140405540);
#endif
}

inline std::int32_t cell_count(const CellList* c)
{
    if (g_hooks.cell_count) return g_hooks.cell_count(c);
#if DEF_GEOLIB_HAVE_P67_2
    return def::cmd::cube::fun_1404056c0(cube_cells(c));
#else
    DEF_NOT_PORTED(0x1404056c0);
#endif
}

inline std::int32_t cell_width(const CellList* c)
{
    if (g_hooks.cell_width) return g_hooks.cell_width(c);
#if DEF_GEOLIB_HAVE_P67_2
    return def::cmd::cube::fun_1404056e0(cube_cells(c));
#else
    DEF_NOT_PORTED(0x1404056e0);
#endif
}

inline std::int32_t* cell_get(CellList* c, std::int32_t k)
{
    if (g_hooks.cell_get) return g_hooks.cell_get(c, k);
#if DEF_GEOLIB_HAVE_P67_2
    return def::cmd::cube::fun_140405720(cube_cells(c), k);
#else
    DEF_NOT_PORTED(0x140405720);
#endif
}

inline CellList* cell_array(std::int32_t n)
{
    if (g_hooks.cell_array) return g_hooks.cell_array(n);
#if DEF_GEOLIB_HAVE_P67_2
    return reinterpret_cast<CellList*>(def::cmd::cube::fun_1404057a0(n));
#else
    DEF_NOT_PORTED(0x1404057a0);
#endif
}

inline std::int32_t* realloc_int(std::int32_t* p, std::int32_t n)
{
    if (g_hooks.realloc_int) return g_hooks.realloc_int(p, n);
#if DEF_GEOLIB_HAVE_P67_2
    return def::cmd::cube::fun_140256e20(p, n);
#else
    DEF_NOT_PORTED(0x140256e20);
#endif
}

inline void alloc_message(const char* fmt, const char* a, const char* b, std::int32_t n)
{
    if (g_hooks.alloc_message) return g_hooks.alloc_message(fmt, a, b, n);
#if DEF_GEOLIB_HAVE_P67_2
    def::cmd::cube::fun_14039d3a0(fmt, a, b, n);
#else
    DEF_NOT_PORTED(0x14039d3a0);
#endif
}

} // namespace def::cmd::geolib::ext
