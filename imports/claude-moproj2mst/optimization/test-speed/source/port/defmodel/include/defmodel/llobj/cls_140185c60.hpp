// Cls_140185c60 (no RTTI): variable set without arrays, LLOBJ_DATA+0xb58. Base Cls_140329c70
// (p1-10, the COW list of StateVar pointers) at 0.
//
//   ctor     140185c60 (base ctor 140329c70, 140185ce0; Phase 2, not ported here)
//   dtor     140185ca0 (140185d10, which is empty; base dtor 140329cb0)
#pragma once

#include <cstddef>
#include <cstdint>

namespace def::model {

class Cls_140185c60 {
public:
    Cls_140185c60();      // 140185c60, Phase 2 (DEF_NOT_PORTED)
    ~Cls_140185c60();
    Cls_140185c60(const Cls_140185c60&) = delete;
    Cls_140185c60& operator=(const Cls_140185c60&) = delete;

    void fun_140185d10(); // empty in the binary

    void* base_list;      // 0x00 base Cls_140329c70 (p1-10)
    std::byte u008[0x10]; // 0x08 owner fields (140199a90 / 140199ab0, p1-05)
};

static_assert(sizeof(Cls_140185c60) == 0x18);

} // namespace def::model
