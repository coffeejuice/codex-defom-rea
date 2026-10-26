// Cls_14028a4c0 (no RTTI, not polymorphic): GRNDAT type-2 record (docs/re/keywords/GRNDAT.md).
// One "main" record is embedded in Cls_14028b640 at +8; list records hang off its five lists.
//
//   ctor     14028a4c0 (3 DYNARYs, next = 0, then 14028a580 init)
//   init     14028a580 (DYNARY types 0x5e/0x5f/0x60, n = 12 via 14029ccd0, values = 0,
//            0x68 = 0, 0x70 = 0, byte 0x1e0 = 0)
//   dtor     14028a530
//   size     0x1f0: ctor writes next at 0x1e8; Cls_14028b640's lists start at 0x1f8 = 8 + 0x1f0.
#pragma once

#include <cstddef>
#include <cstdint>

#include <defmodel/layout/dynary.hpp>
#include <defmodel/layout/layout_check.hpp>

namespace def::model {

class Cls_14028a4c0 {
public:
    // @src 14028a4c0  ctor
    Cls_14028a4c0();
    // @src 14028a530  dtor
    ~Cls_14028a4c0();

    std::int32_t n;              // 0x000 number of values (setter 14029ccd0, getter 14029cd20)
    std::byte pad_004[4];        // 0x004
    double values[12];           // 0x008 zeroed for i < n (14028a580)
    double d;                    // 0x068 =0
    std::int32_t i;              // 0x070 =0
    std::byte pad_074[4];        // 0x074
    DYNARY sub1;                 // 0x078 type 0x5e
    DYNARY sub2;                 // 0x0f0 type 0x5f
    DYNARY sub3;                 // 0x168 type 0x60
    std::uint8_t is_main;        // 0x1e0 1 = main record (14029cda0 from 14028b640)
    std::byte pad_1e1[7];        // 0x1e1
    Cls_14028a4c0* next;         // 0x1e8 =0
};

static_assert(sizeof(Cls_14028a4c0) == 0x1f0);
DEF_LAYOUT_OFFSET(Cls_14028a4c0, values, 0x008);
DEF_LAYOUT_OFFSET(Cls_14028a4c0, d, 0x068);
DEF_LAYOUT_OFFSET(Cls_14028a4c0, i, 0x070);
DEF_LAYOUT_OFFSET(Cls_14028a4c0, sub1, 0x078);
DEF_LAYOUT_OFFSET(Cls_14028a4c0, sub3, 0x168);
DEF_LAYOUT_OFFSET(Cls_14028a4c0, is_main, 0x1e0);
DEF_LAYOUT_OFFSET(Cls_14028a4c0, next, 0x1e8);

} // namespace def::model
