// Free helpers in the p1-07 address range (defmodel/geo/range_helpers.hpp).
#include <defmodel/geo/range_helpers.hpp>

#include <cstddef>

namespace def::model::geo {

// @src 1402b7400  this + 0x148
void* fun_1402b7400(void* self) { return static_cast<std::byte*>(self) + 0x148; }

// @src 1402b7830  node ctor: zero the value at +0x10
void* fun_1402b7830(void* node)
{
    fun_1402b7860(reinterpret_cast<std::uint64_t*>(static_cast<std::byte*>(node) + 0x10));
    return node;
}

// @src 1402b7860  zero five fields
std::uint64_t* fun_1402b7860(std::uint64_t* p)
{
    p[0] = 0;
    p[1] = 0;
    p[2] = 0;
    p[3] = 0;
    p[4] = 0;
    return p;
}

} // namespace def::model::geo
