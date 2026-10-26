// Cls_1401889a0 (no RTTI): LLOBJ_DATA+0x2be8, 0x128 bytes; its dtor 1401889e0 is empty.
//
//   ctor     1401889a0 (Phase 2, not ported here)
#pragma once

#include <cstddef>
#include <cstdint>

namespace def::model {

class Cls_1401889a0 {
public:
    Cls_1401889a0();          // 1401889a0, Phase 2 (DEF_NOT_PORTED)
    ~Cls_1401889a0();         // 1401889e0 (empty)
    Cls_1401889a0(const Cls_1401889a0&) = delete;
    Cls_1401889a0& operator=(const Cls_1401889a0&) = delete;

    std::byte raw[0x128];
};

static_assert(sizeof(Cls_1401889a0) == 0x128);

} // namespace def::model
