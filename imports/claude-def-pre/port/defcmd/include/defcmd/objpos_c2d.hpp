// defcmd/objpos_c2d.hpp: element types of the 2D centring branch 1401fd240 (packets C08-C13)
// that census packet C07 ported ahead of it. Packet p67-1 rows.
//
// The container helpers of that subtree are instances of the copy-on-write templates in
// defmodel/sdctl/cow.hpp (tagged there): 1403e51b0 size, 1403e5cb0 / 1403e6320 list body and
// sentinel ctor, 1403e61f0 / 1403e6180 / 1403e81a0 / 1403e82e0 / 1403e8350 / 1403e83c0 / 1403e8430
// copy loops (cow_detail::copy). The list insert 1403e5de0, its node value ctor 1403e62f0 and the
// iterator ctor / copy 1403e5280 / 1403e52a0 are std::list internals (not-needed, as p1-05's
// 14019c9c0 / 14019cc30 / 14019b5f0 / 14019c6d0).
//
// Note for C08-C13: these vectors hold elements with copy-on-write handles (operator= 1403e38b0,
// 1403e3920, 1403e3960, 1403e39f0, 1403e3a30, 1403e6260) and their bodies clone through new[] with
// element ctors (1403e6930, 1403e70b0, 1403e7360, 1403e7830, 1403e7c50), which
// model::CowVector (trivially copyable elements only) does not cover.
#pragma once

#include <cstddef>
#include <cstdint>

#include <defmodel/sdctl/cow.hpp>

namespace def::cmd::objpos {

// 0x28-byte element of the vector that 1403e2920 resizes and 1403e3c40 erases from (1401fd240
// subtree, via 1403dedd0 > 1403deee0). Role not recovered yet; fields by offset. The three
// handles are int32 copy-on-write vectors: their assignment 1402207c0 adds a reference to the
// source (140159910) and releases its own body with 14015a3d0 (int32 elements).
struct C2dRec28 {
    model::CowVector<std::int32_t> u00;  // 0x00
    std::int32_t u08 = 0;                // 0x08
    model::CowVector<std::int32_t> u10;  // 0x10
    model::CowVector<std::int32_t> u18;  // 0x18
    std::int32_t u20 = 0;                // 0x20
    std::int32_t u24 = 0;                // 0x24

    // 1403e6260 (owner: obj_mesh2d/rc_array.hpp, C09) operator=: u00 (1402207c0), u08, u10 (1402207c0), u18 (1402207c0), u20, u24
    C2dRec28& operator=(const C2dRec28& o)
    {
        u00 = o.u00;
        u08 = o.u08;
        u10 = o.u10;
        u18 = o.u18;
        u20 = o.u20;
        u24 = o.u24;
        return *this;
    }
};

static_assert(sizeof(C2dRec28) == 0x28);
static_assert(offsetof(C2dRec28, u08) == 0x08);
static_assert(offsetof(C2dRec28, u10) == 0x10);
static_assert(offsetof(C2dRec28, u18) == 0x18);
static_assert(offsetof(C2dRec28, u20) == 0x20);
static_assert(offsetof(C2dRec28, u24) == 0x24);

} // namespace def::cmd::objpos
