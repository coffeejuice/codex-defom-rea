// kw-g2geo boundary family: the sort of the 3D face finders (14043c760) and the statically
// linked MSVC CRT qsort it calls (140484bd0 with shortsort 140484b10; now defrt/stdlib/crt_qsort.cpp), from the
// disassembly because the comparator only compares keys and qsort is not stable: the order of
// faces with equal node sums decides which face of a pair is kept and the output order.
#include <defmodel/geo_kw/boundary.hpp>

#include <cstddef>
#include <cstdint>

#include <defrt/stdlib/crt_qsort.hpp>

namespace def::model::geo_kw {

namespace {

using Cmp = int (*)(const void*, const void*);

// @src 14043c7f0  comparator: 1 / 0 / -1 on the first int
int compare_key(const void* a, const void* b)
{
    const std::int32_t x = *static_cast<const std::int32_t*>(a);
    const std::int32_t y = *static_cast<const std::int32_t*>(b);
    if (y < x) return 1;
    if (x == y) return 0;
    return -1;
}

} // namespace

using def::rt::crt_qsort;

// @src 14043c760  qsort(pairs, *n, 8, 14043c7f0)
void sort_key_pairs(std::int32_t* pairs, const std::int32_t* n)
{
    // 14043c773: movsxd of *n, passed as size_t (a negative count would be huge; never happens)
    crt_qsort(pairs, static_cast<std::size_t>(static_cast<std::int64_t>(*n)), 8, compare_key);
}

} // namespace def::model::geo_kw
