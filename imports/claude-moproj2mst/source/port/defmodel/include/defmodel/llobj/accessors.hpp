// defmodel/llobj/accessors.hpp: small p1-04 accessors whose caller classes belong to other
// packets (the objects are passed as raw pointers; offsets as in the binary).
#pragma once

#include <cstddef>
#include <cstdint>

namespace def::model {
class DB_STEP_DATA;
}

namespace def::model::llobj {

// Size of the COW list handle (defmodel/sdctl/cow.hpp) at self+8 / self+0x18 (140159c20,
// 140159c60, 140159c40). The callers use the low 32 bits.
std::size_t list8_size_a(const void* self);   // 140158f10
std::size_t list8_size_b(const void* self);   // 140158f30
std::size_t list18_size(const void* self);    // 140159190

// 1 when self's list at +8 is not empty and the first item's (140283f80(self, 0), p1-08) list at
// +8 is not empty, else 0.
std::int32_t first_item_has_items(void* self);  // 140158ec0

// DB_STEP_DATA+0x1b5c = *value (p1-03's class; the ctor passes 0).
void set_step_u1b5c(DB_STEP_DATA* step, const std::uint8_t* value);  // 140158fa0

} // namespace def::model::llobj
