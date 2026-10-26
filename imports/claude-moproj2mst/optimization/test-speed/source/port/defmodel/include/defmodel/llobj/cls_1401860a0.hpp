// Cls_1401860a0 (no RTTI): LLOBJ_DATA+0x1ea0, three DYNARY tables and two p1-06 objects.
// Sub-objects are kept as byte slots and destroyed explicitly, in the binary's order.
//
//   ctor     1401860a0 (Phase 2, not ported here)
//   dtor     140186130 (reset 140186510; dtors of 0x238, 0x218, 0x200, 0x140, 0xc0)
#pragma once

#include <cstddef>
#include <cstdint>

#include <defmodel/layout/layout_check.hpp>

namespace def::model {

class Cls_1401860a0 {
public:
    Cls_1401860a0();       // 1401860a0, Phase 2 (DEF_NOT_PORTED)
    ~Cls_1401860a0();
    Cls_1401860a0(const Cls_1401860a0&) = delete;
    Cls_1401860a0& operator=(const Cls_1401860a0&) = delete;

    void reset();          // 140186510

    std::byte pad_000[0xc0];
    std::byte s0c0[0x78];  // 0x0c0 DYNARY
    std::byte pad_138[8];
    std::byte s140[0x78];  // 0x140 DYNARY
    std::byte pad_1b8[0x48];
    std::byte s200[0x18];  // 0x200 p1-06 object (reset 140259c70, dtor 1402598a0)
    std::byte s218[0x20];  // 0x218 p1-06 object (reset 14025ab20, dtor 14025ae70)
    std::byte s238[0x78];  // 0x238 DYNARY (reset also clears its int at +0x40, i.e. 0x278)
    std::byte pad_2b0[0x10];
};

static_assert(sizeof(Cls_1401860a0) == 0x2c0);
DEF_LAYOUT_OFFSET(Cls_1401860a0, s0c0, 0x0c0);
DEF_LAYOUT_OFFSET(Cls_1401860a0, s140, 0x140);
DEF_LAYOUT_OFFSET(Cls_1401860a0, s200, 0x200);
DEF_LAYOUT_OFFSET(Cls_1401860a0, s218, 0x218);
DEF_LAYOUT_OFFSET(Cls_1401860a0, s238, 0x238);

} // namespace def::model
