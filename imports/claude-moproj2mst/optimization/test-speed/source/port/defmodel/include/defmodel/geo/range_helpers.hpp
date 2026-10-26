// defmodel/geo/range_helpers.hpp: three small functions that the linker placed in the geometry
// range (p1-07) but that serve other packets' classes. Ported as free functions so the owners
// can call them.
#pragma once

#include <cstdint>

namespace def::model::geo {

// @src 1402b7400  this + 0x148 (called by 1403de460, the OBJ_MESH2D copy of p1-13, on the
//                 source object)
void* fun_1402b7400(void* self);

// @src 1402b7830  list node ctor for a 0x38-byte node (next, prev, 0x28-byte value): the value
//                 is zeroed (1402b7860), the links are left to the caller. Used by the list
//                 body ctor 1402ff930 (p1-09 instantiation).
void* fun_1402b7830(void* node);

// @src 1402b7860  zero five 8-byte fields; returns p
std::uint64_t* fun_1402b7860(std::uint64_t* p);

} // namespace def::model::geo
