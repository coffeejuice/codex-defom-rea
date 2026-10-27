#include "color_pack2d.hpp"

#include <emmintrin.h>

namespace m2m::engine {
namespace {

// MULSS rounds as binary32; CVTTSS2SI64 truncates toward zero. Narrow only
// after the signed conversion so no out-of-range float-to-byte cast occurs.
std::uint32_t channel(float input) {
    const __m128 product = _mm_mul_ss(_mm_set_ss(input), _mm_set_ss(255.0f));
    const std::int64_t integer = _mm_cvttss_si64(product);
    return static_cast<std::uint8_t>(integer);
}

} // namespace

std::uint32_t packColor2D(const std::array<float, 3>& rgb, float scalar) {
    const std::uint32_t s = channel(scalar);
    const std::uint32_t b = channel(rgb[2]);
    const std::uint32_t g = channel(rgb[1]);
    const std::uint32_t r = channel(rgb[0]);
    return r | (g << 8) | (b << 16) | (s << 24);
}

} // namespace m2m::engine
