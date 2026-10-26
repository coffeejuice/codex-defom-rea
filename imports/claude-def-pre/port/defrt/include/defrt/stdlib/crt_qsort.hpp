// defrt/stdlib/crt_qsort.hpp: the MSVC CRT qsort linked into DEF_PRE_64 (140484bd0, shortsort
// 140484b10). Not stable; use it wherever the comparator can tie two elements whose order is
// visible (the boundary face sort 14043c760, BRDEXT's suffix sort 14013cc10).
#pragma once

#include <cstddef>

namespace def::rt {

// qsort(base, num, width, comp) with the original's permutation (median of three, cutoff 8,
// byte-wise element swaps).
void crt_qsort(void* base, std::size_t num, std::size_t width, int (*comp)(const void*, const void*));

} // namespace def::rt
