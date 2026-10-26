// Cls_14032f100 (no RTTI, not polymorphic): member of DB_STEP_DATA at 0x33b8: an int and an
// owning handle to a heap std::list.
//
//   ctor     14032f100 (14032f920 at +8, then int at 0 = 0)
//   dtor     14032f140 (-> 14032f990 on +8)
//   size     0x10: the ctor writes 0x00 (int) and 0x08 (8-byte handle); DB_STEP_DATA's init
//            1401c8c00 writes its own int at 0x33c8 = 0x33b8 + 0x10.
//   open     meaning of the int (a count?) and of the list (see Cls_14032f920).
#pragma once

#include <cstddef>
#include <cstdint>

#include <defmodel/layout/cls_14032f920.hpp>
#include <defmodel/layout/layout_check.hpp>

namespace def::model {

class Cls_14032f100 {
public:
    // @src 14032f100  ctor
    Cls_14032f100();
    // @src 14032f140  dtor
    ~Cls_14032f100();

    std::int32_t u0;      // 0x0 ctor =0
    std::byte pad_4[4];   // 0x4
    Cls_14032f920 u8;     // 0x8 heap list handle
};

static_assert(sizeof(Cls_14032f100) == 0x10);
DEF_LAYOUT_OFFSET(Cls_14032f100, u0, 0x0);
DEF_LAYOUT_OFFSET(Cls_14032f100, u8, 0x8);

} // namespace def::model
