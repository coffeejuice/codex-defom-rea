// CP297 internal static-contract leaf. No public render or palette binding.
#pragma once

#include <array>
#include <cstdint>

namespace m2m::engine {

// CP296 packer 0x14126a7e0: RGB and scalar are binary32 values in [0,1].
// Assumes ordinary finite inputs and the normal masked SSE exception state.
// The result is the raw little-endian byte pattern [R,G,B,scalar], not a
// numerical floating-point conversion. Exceptional inputs/MXCSR modes are
// outside this portable leaf's qualified contract.
std::uint32_t packColor2D(const std::array<float, 3>& rgb, float scalar);

} // namespace m2m::engine
