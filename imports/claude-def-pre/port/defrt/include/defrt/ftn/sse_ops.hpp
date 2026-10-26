// defrt/ftn/sse_ops.hpp: scalar double/float operations with the NaN result of the original SSE2
// instruction for a given operand order (dest first). SSE rule: if the first (dest) operand is
// NaN the result is that NaN, quieted; otherwise (second operand NaN, or an invalid operation)
// the hardware result, which does not depend on the operand order. MSVC freely commutes a + b
// and a * b, and it commutes _mm_add_sd / _mm_mul_sd intrinsics too (seen in the disassembly), so
// the order cannot be forced through the instruction; the first-operand NaN is selected instead.
// Write add(dest, src) / mul(dest, src) in the listing's order. sqrt is SQRTSD.
#pragma once

#include <emmintrin.h>

#include <bit>
#include <cstdint>

namespace def::rt::sse {

inline double first_nan(double a, double r) {
    return a != a ? std::bit_cast<double>(std::bit_cast<std::uint64_t>(a) | 0x0008000000000000ull) : r;
}
inline float first_nan(float a, float r) {
    return a != a ? std::bit_cast<float>(std::bit_cast<std::uint32_t>(a) | 0x00400000u) : r;
}

inline double add(double a, double b) { return first_nan(a, a + b); }
inline double sub(double a, double b) { return first_nan(a, a - b); }
inline double mul(double a, double b) { return first_nan(a, a * b); }
inline double div(double a, double b) { return first_nan(a, a / b); }
inline float add(float a, float b) { return first_nan(a, a + b); }
inline float sub(float a, float b) { return first_nan(a, a - b); }
inline float mul(float a, float b) { return first_nan(a, a * b); }
inline float div(float a, float b) { return first_nan(a, a / b); }

inline double sqrt(double x) {
    const __m128d v = _mm_set_sd(x);
    return _mm_cvtsd_f64(_mm_sqrt_sd(v, v));
}

} // namespace def::rt::sse
