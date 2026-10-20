#include "deform/packed_array.hpp"

#include <algorithm>
#include <bit>
#include <cmath>
#include <cstring>
#include <limits>
#include <string>
#include <type_traits>

namespace deform {
namespace {

// Reconstructed from REA evidence:
// integers 0x14044aeeb, ev_03cd34b921df3d39a1f4bceb7276df27d7ead5f9cfe508a2fa8b3d0975a86967
// reals    0x14044b3f5, ev_d797ec8acbdf7d482af4b6fcb9e555eb49b147d22e85125eba5c18814857c383
//
// A run is [value, count, previous_link]. Links are 1-based positions of the
// previous_link slot, not the value slot. The original expands backward in
// place; the reversed traversal here keeps the same ordering.

template <typename T>
using bits_type = std::conditional_t<sizeof(T) == 4, std::uint32_t, std::uint64_t>;

template <typename T>
std::int32_t metadata(bits_type<T> bits, const char* field) {
    const T value = std::bit_cast<T>(bits);
    if constexpr (std::is_same_v<T, double>) {
        // The original casts to int32, truncating fractional metadata toward
        // zero. Avoid C++ undefined behavior for NaN, infinity or overflow.
        if (!std::isfinite(value) ||
            std::trunc(value) < std::numeric_limits<std::int32_t>::min() ||
            std::trunc(value) > std::numeric_limits<std::int32_t>::max()) {
            throw packed_array_error(std::string("Invalid packed array ") + field);
        }
    }
    return static_cast<std::int32_t>(value);
}

template <typename T>
std::vector<T> unpack(std::span<const T> packed) {
    if (packed.size() < 2 ||
        packed.size() > static_cast<std::size_t>(std::numeric_limits<std::int32_t>::max())) {
        throw packed_array_error("Packed array must contain two metadata elements");
    }
    using Bits = bits_type<T>;
    auto at = [&](std::size_t index) { return std::bit_cast<Bits>(packed[index]); };
    const auto logical = metadata<T>(at(packed.size() - 2), "logical size");
    auto link = metadata<T>(at(packed.size() - 1), "last link");
    if (logical < 0 || link < 0) {
        throw packed_array_error("Negative packed array size or link");
    }
    const auto size = static_cast<std::size_t>(logical);
    const auto payload_size = packed.size() - 2;
    if (link == 0) {
        if (size > payload_size) {
            throw packed_array_error("Unpacked array size exceeds available payload");
        }
        // Copying through integer representations also preserves signaling NaN
        // bits; no floating-point arithmetic is applied to payload values.
        std::vector<T> result(size);
        for (std::size_t i = 0; i < size; ++i) {
            result[i] = std::bit_cast<T>(at(i));
        }
        return result;
    }
    if (static_cast<std::size_t>(link) > payload_size || link < 3) {
        throw packed_array_error("Packed array link lies outside its payload");
    }

    // Validate the entire chain before allocating according to a file-supplied
    // size. This also avoids accepting dangling links as ordinary literals.
    std::size_t source = payload_size;
    std::size_t remaining = size;
    auto current_link = link;
    while (source != 0) {
        if (source == static_cast<std::size_t>(current_link)) {
            if (source < 3) {
                throw packed_array_error("Truncated packed array run");
            }
            const auto count = metadata<T>(at(source - 2), "run count");
            const auto previous = metadata<T>(at(source - 1), "previous link");
            if (count < 0 || static_cast<std::size_t>(count) > remaining) {
                throw packed_array_error("Packed array run exceeds logical size");
            }
            if (previous < 0 ||
                (previous != 0 &&
                 (previous < 3 || static_cast<std::size_t>(previous) > source - 3))) {
                throw packed_array_error("Invalid packed array link chain");
            }
            remaining -= static_cast<std::size_t>(count);
            source -= 3;
            current_link = previous;
        } else {
            if (remaining == 0) {
                throw packed_array_error("Packed array literals exceed logical size");
            }
            --remaining;
            --source;
        }
    }
    if (remaining != 0 || current_link != 0) {
        throw packed_array_error("Packed array does not fill its logical size");
    }

    std::vector<T> result(size);
    source = payload_size;
    std::size_t destination = size;
    while (source != 0) {
        if (source == static_cast<std::size_t>(link)) {
            const auto count = static_cast<std::size_t>(
                metadata<T>(at(source - 2), "run count"));
            link = metadata<T>(at(source - 1), "previous link");
            destination -= count;
            const Bits value = at(source - 3);
            for (std::size_t i = destination; i < destination + count; ++i) {
                std::memcpy(&result[i], &value, sizeof(T));
            }
            source -= 3;
        } else {
            const Bits value = at(--source);
            std::memcpy(&result[--destination], &value, sizeof(T));
        }
    }
    return result;
}

} // namespace

std::vector<std::int32_t> unpack_integers(std::span<const std::int32_t> packed) {
    return unpack<std::int32_t>(packed);
}

std::vector<double> unpack_reals(std::span<const double> packed) {
    return unpack<double>(packed);
}

} // namespace deform
