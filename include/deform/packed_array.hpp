#pragma once

#include <cstdint>
#include <span>
#include <stdexcept>
#include <vector>

namespace deform {

class packed_array_error : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

// Decode the datarm23.f packed integer/real representation. The final two
// elements are the logical size and the 1-based position of the last run link.
// The input is already in host byte order. These functions do not infer whether
// a database version uses packing; the caller must select that from its header.
//
// Valid payload values are copied bit-for-bit, including real NaNs and -0.
// Unsafe/contradictory metadata raises packed_array_error. This checked behavior
// is an API safety boundary, not a claim about the original's malformed inputs.
[[nodiscard]] std::vector<std::int32_t>
unpack_integers(std::span<const std::int32_t> packed);

[[nodiscard]] std::vector<double>
unpack_reals(std::span<const double> packed);

} // namespace deform
