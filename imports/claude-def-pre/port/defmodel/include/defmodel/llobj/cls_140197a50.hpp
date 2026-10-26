// Cls_140197a50 (no RTTI): LLOBJ_DATA+0x3920, a count and three malloc'd arrays.
//
//   ctor     140197a50 (-> 140197b00; Phase 2, not ported here)
//   dtor     140197a80 (-> 140197b40)
#pragma once

#include <cstddef>
#include <cstdint>

#include <defmodel/layout/layout_check.hpp>

namespace def::model {

class Cls_140197a50 {
public:
    Cls_140197a50();          // 140197a50, Phase 2 (DEF_NOT_PORTED)
    ~Cls_140197a50();
    Cls_140197a50(const Cls_140197a50&) = delete;
    Cls_140197a50& operator=(const Cls_140197a50&) = delete;

    void free_arrays();       // 140197b40

    std::int32_t count;       // 0x00
    std::byte pad_04[4];
    void* u08;                // 0x08
    void* u10;                // 0x10
    void* u18;                // 0x18
};

static_assert(sizeof(Cls_140197a50) == 0x20);
DEF_LAYOUT_OFFSET(Cls_140197a50, u08, 0x08);
DEF_LAYOUT_OFFSET(Cls_140197a50, u18, 0x18);

} // namespace def::model
