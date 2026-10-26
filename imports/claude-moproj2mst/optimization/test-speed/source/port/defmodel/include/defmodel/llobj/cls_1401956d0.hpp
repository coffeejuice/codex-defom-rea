// Cls_1401956d0 (no RTTI): LLOBJ_DATA+0x2e68. A DYNARY list (Cls_1401364a0) at 0, two p1-06
// objects at 0x18/0x30 and seven Cls_140269250 (p1-06) from 0x48.
//
//   ctor     1401956d0 (Phase 2, not ported here)
//   dtor     1401958b0 (reset 140195a40; 140269320 on 0x168 .. 0x48; 140257c60 on 0x30, 0x18;
//            Cls_1401364a0 dtor 140136510 on 0, which C++ runs after the body, as the binary does)
#pragma once

#include <cstddef>
#include <cstdint>

#include <defmodel/layout/cls_1401364a0.hpp>
#include <defmodel/layout/layout_check.hpp>

namespace def::model {

class Cls_1401956d0 {
public:
    Cls_1401956d0();          // 1401956d0, Phase 2 (DEF_NOT_PORTED)
    ~Cls_1401956d0();
    Cls_1401956d0(const Cls_1401956d0&) = delete;
    Cls_1401956d0& operator=(const Cls_1401956d0&) = delete;

    void reset();             // 140195a40

    Cls_1401364a0 tables;     // 0x000 DYNARY list
    std::byte s018[0x18];     // 0x018 p1-06 object (reset 1402581b0, dtor 140257c60)
    std::byte s030[0x18];     // 0x030 p1-06 object (dtor 140257c60)
    std::byte s048[7][0x30];  // 0x048..0x168 seven Cls_140269250 (p1-06)
    std::byte pad_198[0x328]; // 0x198
};

static_assert(sizeof(Cls_1401956d0) == 0x4c0);
DEF_LAYOUT_OFFSET(Cls_1401956d0, s018, 0x018);
DEF_LAYOUT_OFFSET(Cls_1401956d0, s030, 0x030);
DEF_LAYOUT_OFFSET(Cls_1401956d0, s048, 0x048);

} // namespace def::model
