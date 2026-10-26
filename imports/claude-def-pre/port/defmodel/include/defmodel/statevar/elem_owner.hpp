// Point count that StateVarElemSTS/TEPS/DAMAG take as their third dimension.
//
// The owner is the element state-variable container (LLOBJ_DATA + 0x9e0, Cls_140182880), owned
// by p1-04. Until its class has these members, this is a local copy of p1-04's 140183c20 (with
// p1-05's 14019a450 inlined) that reads the owner's fields at their recovered offsets:
//
//   owner + 0xf0  pointer to an object with an int per index at + 0x888 + 4 * k
//   owner + 0xf8  int; == 3 is 14019a450's test
//   owner + 0xec  int
#pragma once

#include <cstdint>

namespace def::model::statevar {

// copy of 140183c20(owner, k): 1 unless the per-index type is 3; then 4, or 8/1 by owner + 0xec
// when owner + 0xf8 == 3. owner must not be null (the original dereferences it unchecked).
std::int32_t owner_point_count(const void* owner, std::int32_t k);

} // namespace def::model::statevar
