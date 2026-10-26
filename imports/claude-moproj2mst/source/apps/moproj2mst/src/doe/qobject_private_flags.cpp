#include "m2m/doe_qobject_private_flags.hpp"

namespace m2m::doe {

std::uint32_t isolatedQObjectPrivateFlags(std::uint32_t previousWord) noexcept
{
    return (previousWord & 0xfffffcf0u) | 0x60u;
}

} // namespace m2m::doe
