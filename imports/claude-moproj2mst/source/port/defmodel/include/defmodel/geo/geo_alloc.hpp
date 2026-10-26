// defmodel/geo/geo_alloc.hpp: the allocation primitives the geometry code calls.
//
//   FUN_1404797d8   operator new (malloc; the callers test the result for 0)
//   free            the CRT free (140478b30 / 140479908); buffers from operator new are released
//                   with it directly, so operator new here must be malloc
//   FUN_140478d30   memset
//
// Array sizes go through the compiler's overflow check: SUB168(ZEXT816(elem) * n, 8) != 0 gives
// a byte count of -1 (operator new then fails), see mul_size.
#pragma once

#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <new>

namespace def::model::geo {

// operator new: MSVC's malloc(0) returns a unique pointer; keep that on every platform.
inline void* op_new(std::size_t bytes) { return std::malloc(bytes != 0 ? bytes : 1); }

// elem * n as the unsigned 128-bit product the compiler emits, saturated to SIZE_MAX.
inline std::size_t mul_size(std::uint64_t n, std::size_t elem)
{
    if (n != 0 && elem > SIZE_MAX / n) {
        return SIZE_MAX;
    }
    return static_cast<std::size_t>(n * elem);
}

// Same, for a signed 32-bit count sign-extended to 64 bits first (`(longlong)count`), as in
// `ZEXT816(8) * (longlong)n`: a negative count becomes a huge unsigned value.
inline std::size_t mul_size_i32(std::int32_t n, std::size_t elem)
{
    return mul_size(static_cast<std::uint64_t>(static_cast<std::int64_t>(n)), elem);
}

// 32-bit int arithmetic as the binary does it (two's complement wraparound, no UB).
inline std::int32_t wrap_mul(std::int32_t a, std::int32_t b)
{
    return static_cast<std::int32_t>(static_cast<std::uint32_t>(a) * static_cast<std::uint32_t>(b));
}
inline std::int32_t wrap_add(std::int32_t a, std::int32_t b)
{
    return static_cast<std::int32_t>(static_cast<std::uint32_t>(a) + static_cast<std::uint32_t>(b));
}
inline std::int32_t wrap_shl(std::int32_t a, int n)
{
    return static_cast<std::int32_t>(static_cast<std::uint32_t>(a) << n);
}

// (int)d as cvttsd2si: truncation, 0x80000000 for NaN and out-of-range values.
inline std::int32_t ftoi_trunc(double d)
{
    if (!(d > -2147483649.0 && d < 2147483648.0)) {
        return INT32_MIN;
    }
    return static_cast<std::int32_t>(d);
}

// Class-level allocation for the polymorphic geometry objects: the original allocates them with
// operator new (malloc) and its scalar deleting destructors release them with free.
struct MallocObject {
    static void* operator new(std::size_t bytes)
    {
        void* p = op_new(bytes);
        if (p == nullptr) {
            throw std::bad_alloc();
        }
        return p;
    }
    static void* operator new(std::size_t, void* where) noexcept { return where; }
    static void operator delete(void* p) noexcept { std::free(p); }
    static void operator delete(void*, void*) noexcept {}
    static void* operator new[](std::size_t bytes) { return operator new(bytes); }
    static void operator delete[](void* p) noexcept { std::free(p); }
};

} // namespace def::model::geo
