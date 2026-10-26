// Cls_14017e740 (no RTTI): 0x30-byte element of the array of 100 at LLOBJ_DATA+0xbe0 (built by
// the eh vector constructor iterator with ctor 14017e740 and dtor 14017e720).
#pragma once

#include <cstddef>
#include <cstdint>

#include <defmodel/layout/layout_check.hpp>

namespace def::model {

class Cls_14017e740 {
public:
    Cls_14017e740();                 // 14017e740
    ~Cls_14017e740();                // 14017e720
    Cls_14017e740(const Cls_14017e740&) = delete;
    Cls_14017e740& operator=(const Cls_14017e740&) = delete;

    void set_owner(void* owner);     // 14017e770: u028 = owner, then init
    void init();                     // 14017e7b0
    void free_arrays();              // 14017e800

    std::int32_t u000;     // 0x00
    std::byte pad_004[4];  // 0x04
    void* u008;            // 0x08 freed by 14017e800
    std::int32_t u010;     // 0x10
    std::byte pad_014[4];  // 0x14
    void* u018;            // 0x18 freed by 14017e800
    void* u020;            // 0x20 freed by 14017e800
    void* u028;            // 0x28 owner (DB_STEP_DATA*, LLOBJ_DATA::owner in 14015fbf0)
};

static_assert(sizeof(Cls_14017e740) == 0x30);
DEF_LAYOUT_OFFSET(Cls_14017e740, u008, 0x08);
DEF_LAYOUT_OFFSET(Cls_14017e740, u010, 0x10);
DEF_LAYOUT_OFFSET(Cls_14017e740, u018, 0x18);
DEF_LAYOUT_OFFSET(Cls_14017e740, u020, 0x20);
DEF_LAYOUT_OFFSET(Cls_14017e740, u028, 0x28);

} // namespace def::model
