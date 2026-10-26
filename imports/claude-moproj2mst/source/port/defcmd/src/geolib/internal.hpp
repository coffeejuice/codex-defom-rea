// geolib internals (packet p8-09): raw member access by the binary's offsets.
#pragma once

#include <cstddef>
#include <cstdint>

namespace def::cmd::geolib::detail {

template <class T, class O>
T& at(O* o, std::size_t off)
{
    return *reinterpret_cast<T*>(reinterpret_cast<std::byte*>(o) + off);
}

} // namespace def::cmd::geolib::detail
