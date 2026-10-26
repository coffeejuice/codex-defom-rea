// Cls_140289b80 (no RTTI, not polymorphic): creep block of a Material (Material+0xac8), data of
// keyword CREEP (docs/re/keywords/CREEP.md).
//
//   ctor     140289b80 (9 DYNARYs, model = 0, types, routine = 0, cnt = -1)
//   dtor     14029cee0
//   size     0x448: the last DYNARY starts at 0x3d0 (param_1[0xf4]); Material's next member is at
//            0xf10 = 0xac8 + 0x448.
#pragma once

#include <cstddef>
#include <cstdint>

#include <defmodel/layout/dynary.hpp>
#include <defmodel/layout/layout_check.hpp>

namespace def::model {

class Cls_140289b80 {
public:
    // @src 140289b80  ctor
    Cls_140289b80();
    // @src 14029cee0  dtor
    ~Cls_140289b80();

    std::int32_t model;          // 0x000 =0; CREEP model M (mat+0xac8)
    std::byte pad_004[4];        // 0x004
    DYNARY d6;                   // 0x008 type 0x43; "Creep - TableData" (mat+0xad0)
    DYNARY d5;                   // 0x080 type 0x44; "Creep - TableDataStr" (mat+0xb48)
    DYNARY d1;                   // 0x0f8 type 0x45; "Creep - Prezyna" (mat+0xbc0)
    DYNARY d2;                   // 0x170 type 0x46; "Creep - Power Law" (mat+0xc38)
    DYNARY d3;                   // 0x1e8 type 0x47; "Creep - GE" (mat+0xcb0)
    DYNARY d4;                   // 0x260 type 0x48; "Creep - WG" (mat+0xd28)
    std::int32_t user_routine;   // 0x2d8 =0; CREEP -t (mat+0xda0)
    std::int32_t cnt;            // 0x2dc =-1; CREEP t7/t8 count (mat+0xda4)
    DYNARY a;                    // 0x2e0 type 0xa9 (mat+0xda8)
    DYNARY b;                    // 0x358 type 0xaa (mat+0xe20)
    DYNARY c;                    // 0x3d0 type 0xab (mat+0xe98)
};

static_assert(sizeof(Cls_140289b80) == 0x448);
DEF_LAYOUT_OFFSET(Cls_140289b80, d6, 0x008);
DEF_LAYOUT_OFFSET(Cls_140289b80, d5, 0x080);
DEF_LAYOUT_OFFSET(Cls_140289b80, d1, 0x0f8);
DEF_LAYOUT_OFFSET(Cls_140289b80, d4, 0x260);
DEF_LAYOUT_OFFSET(Cls_140289b80, user_routine, 0x2d8);
DEF_LAYOUT_OFFSET(Cls_140289b80, cnt, 0x2dc);
DEF_LAYOUT_OFFSET(Cls_140289b80, a, 0x2e0);
DEF_LAYOUT_OFFSET(Cls_140289b80, c, 0x3d0);

} // namespace def::model
