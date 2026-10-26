// Cls_140189440 (no RTTI): LLOBJ_DATA+0x2d10, three Cls_140269250 (p1-06, 0x30 bytes) and two
// malloc'd arrays.
//
//   ctor     140189440 (Phase 2, not ported here)
//   dtor     140189520 (reset 140189ad0; 140269320 on 0x128, 0xf8, 0x40)
#pragma once

#include <cstddef>
#include <cstdint>

#include <defmodel/layout/layout_check.hpp>

namespace def::model {

class Cls_140189440 {
public:
    Cls_140189440();          // 140189440, Phase 2 (DEF_NOT_PORTED)
    ~Cls_140189440();
    Cls_140189440(const Cls_140189440&) = delete;
    Cls_140189440& operator=(const Cls_140189440&) = delete;

    void reset();             // 140189ad0

    std::byte pad_000[0x40];
    std::byte s040[0x30];     // 0x040 Cls_140269250 (p1-06)
    std::byte pad_070[0x68];
    std::int32_t u0d8;        // 0x0d8 reset = 0
    std::byte pad_0dc[4];
    void* u0e0;               // 0x0e0 freed by reset
    void* u0e8;               // 0x0e8 freed by reset
    std::byte pad_0f0[8];
    std::byte s0f8[0x30];     // 0x0f8 Cls_140269250
    std::byte s128[0x30];     // 0x128 Cls_140269250
};

static_assert(sizeof(Cls_140189440) == 0x158);
DEF_LAYOUT_OFFSET(Cls_140189440, s040, 0x040);
DEF_LAYOUT_OFFSET(Cls_140189440, u0d8, 0x0d8);
DEF_LAYOUT_OFFSET(Cls_140189440, u0e0, 0x0e0);
DEF_LAYOUT_OFFSET(Cls_140189440, u0e8, 0x0e8);
DEF_LAYOUT_OFFSET(Cls_140189440, s0f8, 0x0f8);
DEF_LAYOUT_OFFSET(Cls_140189440, s128, 0x128);

} // namespace def::model
