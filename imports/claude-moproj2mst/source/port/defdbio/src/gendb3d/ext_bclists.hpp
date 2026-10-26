// gendb3d/ext_bclists.hpp: external calls of the sub-object writers in bclists.cpp
// (write_obj_1ea0 / write_obj_2d10 / write_obj_2e68 and the LLWINDEF_DATA record writer), see
// ext.hpp for the rules. Each wrapper offers the call to the test hook first and otherwise calls
// the owner's port (named next to it; gate ports in defdbio/gendb/gate_ports.hpp).
//
// Reused from ext_obj3d.hpp (same address, one wrapper only): f140130290 (DYNARY LLFNC length),
// f140006410 / f140005510 (OBJ_GEO3D &coords[0] / &elems[0] for writing).
#pragma once

#include <cstdint>

#include "gendb3d/access.hpp"
#include "gendb3d/ext.hpp"
#include "gendb3d/ext_obj3d.hpp"

#include <defdbio/gendb/counts_frame.hpp>
#include <defdbio/gendb/gate_ports.hpp>
#include <defdbio/gendb/obj_counts.hpp>
#include <defmodel/geo_kw/misc_model.hpp>
#include <defmodel/mesh_gen/sf_list_ops.hpp>
#include <defmodel/window/window_list.hpp>

namespace def::dbio::g3d::ext {

// ---- P2 (keyword model) ------------------------------------------------------------------------
// 140130200 (P2): LLFNC length of a 1-D table: type 0x7c -> ncomp (+0x40), else n[0] (+0x1c) << 1
// -> gendb::dynary_words_1d (gate port)
inline std::int32_t f140130200(void* f)
{
    std::uint64_t r;
    if (hooked(0x140130200, {u(f)}, &r)) return static_cast<std::int32_t>(r);
    return gendb::dynary_words_1d(static_cast<const model::DYNARY*>(f));
}
// 140259b60 (P2): node n (1-based) of the list at p (head *p, link +0xe0); 0 past the end
// -> model::geo_kw::wpaxis_nth
inline void* f140259b60(void* list, std::int32_t n)
{
    std::uint64_t r;
    if (hooked(0x140259b60, {u(list), i(n)}, &r)) return as_p<void>(r);
    return model::geo_kw::wpaxis_nth(*static_cast<model::linked::Cls_140258580List*>(list), n);
}
// 14025a9e0 (P2): node n (1-based) of the SfLinkedList at p (head +8, link +0x10); 0 past the end
// -> model::sf_list_nth
inline void* f14025a9e0(void* list, std::int32_t n)
{
    std::uint64_t r;
    if (hooked(0x14025a9e0, {u(list), i(n)}, &r)) return as_p<void>(r);
    return model::sf_list_nth(static_cast<const model::SfLinkedList*>(list), n);
}
// 140269900 (P2): node n (1-based) of the Cls_140269250 list at p (head *p, link +0x840)
// -> model::winlist_nth
inline void* f140269900(void* list, std::int32_t n)
{
    std::uint64_t r;
    if (hooked(0x140269900, {u(list), i(n)}, &r)) return as_p<void>(r);
    return model::winlist_nth(static_cast<const model::Cls_140269250*>(list), n);
}
// 14019a530 (P2): item count of the list at s + 0x200 (14019a550) -> gendb::body_force_list_count
inline std::int32_t f14019a530(void* s)
{
    std::uint64_t r;
    if (hooked(0x14019a530, {u(s)}, &r)) return static_cast<std::int32_t>(r);
    return gendb::body_force_list_count(s);
}
// 14019a560 (P2): item count of the list at s + 0x218 (14019a580) -> gendb::list218_count
inline std::int32_t f14019a560(void* s)
{
    std::uint64_t r;
    if (hooked(0x14019a560, {u(s)}, &r)) return static_cast<std::int32_t>(r);
    return gendb::list218_count(s);
}
// 14025c830 (P2): LLWINDEF_DATA is 2D: *(w + 0x888) != 0 && 1400015e0(*(w + 0x888))
// -> gendb::window_owner_is_2d
inline bool f14025c830(void* w)
{
    std::uint64_t r;
    if (hooked(0x14025c830, {u(w)}, &r)) return (r & 0xff) != 0;
    return gendb::window_owner_is_2d(w);
}

// ---- p4-02a --------------------------------------------------------------------------------
// 140186d00 (p4-02a): items of the list at s + 0x200 whose type (+4) is 5 or 6
// -> gendb::count_body_force_records
inline std::int32_t f140186d00(void* s)
{
    std::uint64_t r;
    if (hooked(0x140186d00, {u(s)}, &r)) return static_cast<std::int32_t>(r);
    return gendb::count_body_force_records(s);
}
// 1401f3c50 (p4-02a): return *(int *)(p + 0x10c) -> gendb::simctrl_u10c
inline std::int32_t f1401f3c50(void* p)
{
    std::uint64_t r;
    if (hooked(0x1401f3c50, {u(p)}, &r)) return static_cast<std::int32_t>(r);
    return gendb::simctrl_u10c(p);
}
// 140189580 (p4-02a): item count of the window list at s + 0x40 (14019a590) -> gendb::windows_2d10_count
inline std::int32_t f140189580(void* s)
{
    std::uint64_t r;
    if (hooked(0x140189580, {u(s)}, &r)) return static_cast<std::int32_t>(r);
    return gendb::windows_2d10_count(s);
}
// 140195b30 .. 140195c50 (p4-02a): item counts of the Cls_1401956d0 lists (14019a590)
// -> gendb::c2e68_win*_count
#define DEF_G3D_C2E68(addr, fn)                                                            \
    inline std::int32_t f##addr(void* s)                                                   \
    {                                                                                      \
        std::uint64_t r;                                                                   \
        if (hooked(0x##addr, {u(s)}, &r)) return static_cast<std::int32_t>(r);             \
        return gendb::fn(s);                                                               \
    }
DEF_G3D_C2E68(140195b30, c2e68_win48_count)   // s + 0x48
DEF_G3D_C2E68(140195b60, c2e68_win78_count)   // s + 0x78
DEF_G3D_C2E68(140195b90, c2e68_wina8_count)   // s + 0xa8
DEF_G3D_C2E68(140195bc0, c2e68_wind8_count)   // s + 0xd8
DEF_G3D_C2E68(140195bf0, c2e68_win108_count)  // s + 0x108
DEF_G3D_C2E68(140195c20, c2e68_win138_count)  // s + 0x138
DEF_G3D_C2E68(140195c50, c2e68_win168_count)  // s + 0x168
#undef DEF_G3D_C2E68

// ---- p4-02b (DB_WRITE_VAR window cursors) -----------------------------------------------------
// 14019a5c0 (p4-02b): *(int *)(m + 0x134) = *v -> gendb::set_u134
inline void f14019a5c0(model::DB_STEP_DATA* m, const std::int32_t* v)
{
    std::uint64_t r;
    if (hooked(0x14019a5c0, {u(m), u(v)}, &r)) return;
    gendb::set_u134(m, v);
}
// 14019a600 (p4-02b): *(int *)(m + 0x138) = *v -> gendb::set_u138
inline void f14019a600(model::DB_STEP_DATA* m, const std::int32_t* v)
{
    std::uint64_t r;
    if (hooked(0x14019a600, {u(m), u(v)}, &r)) return;
    gendb::set_u138(m, v);
}

} // namespace def::dbio::g3d::ext
