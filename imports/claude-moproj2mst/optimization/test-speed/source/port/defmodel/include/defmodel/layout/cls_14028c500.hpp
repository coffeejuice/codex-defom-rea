// Cls_14028c500 (no RTTI, not polymorphic): texture triple at Material+0x2154, data of keyword
// TXTURE (docs/re/keywords/TXTURE.md).
//
//   ctor     14028c500 (a = 0, b = 1, c = 1)
//   dtor     14028c540 (empty)
//   size     0xc: three ints; Material's next member (DIFBND DYNARY) is at 0x2160 = 0x2154 + 0xc.
#pragma once

#include <cstddef>
#include <cstdint>

#include <defmodel/layout/layout_check.hpp>

namespace def::model {

class Cls_14028c500 {
public:
    // @src 14028c500  ctor
    Cls_14028c500();
    // @src 14028c540  dtor (empty)
    ~Cls_14028c500();

    std::int32_t a;              // 0x0 =0 (mat+0x2154)
    std::int32_t b;              // 0x4 =1 (mat+0x2158)
    std::int32_t c;              // 0x8 =1 (mat+0x215c)
};

static_assert(sizeof(Cls_14028c500) == 0xc);
DEF_LAYOUT_OFFSET(Cls_14028c500, b, 0x4);
DEF_LAYOUT_OFFSET(Cls_14028c500, c, 0x8);

} // namespace def::model
