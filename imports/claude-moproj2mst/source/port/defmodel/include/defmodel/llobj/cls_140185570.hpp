// Cls_140185570 (no RTTI): twelve malloc'd arrays of an object, LLOBJ_DATA+0xae8.
//
//   ctor     140185570 (u060 = 0, u068 = 3, init 140185630)
//   dtor     1401855b0 (-> free 1401856e0)
#pragma once

#include <cstddef>
#include <cstdint>

#include <defmodel/layout/layout_check.hpp>

namespace def::model {

class Cls_140185570 {
public:
    Cls_140185570();
    ~Cls_140185570();
    Cls_140185570(const Cls_140185570&) = delete;
    Cls_140185570& operator=(const Cls_140185570&) = delete;

    void init();          // 140185630
    void free_arrays();   // 1401856e0
    void set_u068(std::int32_t v);  // 14019a4b0

    void* arrays[12];     // 0x00..0x58 freed and zeroed by 1401856e0
    void* u060;           // 0x60 ctor = 0; owner (set by 1401855d0)
    std::int32_t u068;    // 0x68 ctor = 3
    std::byte pad_06c[4]; // 0x6c
};

static_assert(sizeof(Cls_140185570) == 0x70);
DEF_LAYOUT_OFFSET(Cls_140185570, u060, 0x60);
DEF_LAYOUT_OFFSET(Cls_140185570, u068, 0x68);

} // namespace def::model
