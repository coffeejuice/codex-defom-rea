// defrt/stdlib/crt_qsort.cpp: the statically linked MSVC CRT qsort of DEF_PRE_64 (140484bd0, shortsort
// 140484b10), reproduced from the disassembly because qsort is not stable: callers whose comparator
// ties elements depend on its exact permutation. Moved here from defmodel/src/geo_kw/boundary_qsort.cpp
// (kw-g2geo) by p8-05 so that every packet shares it.
#include <defrt/stdlib/crt_qsort.hpp>

#include <cstddef>
#include <cstdint>

namespace def::rt {

namespace {

using Cmp = int (*)(const void*, const void*);

// byte swap of two elements (140484ce0 loop); never called with a == b
void swap_bytes(char* a, char* b, std::size_t width)
{
    if (a != b) {
        while (width--) {
            char t = *a;
            *a++ = *b;
            *b++ = t;
        }
    }
}

// @src 140484b10  shortsort: selection sort; each pass swaps the first of the equal maxima
// (cmovg: max moves only to a strictly greater element) with the last element
void shortsort(char* lo, char* hi, std::size_t width, Cmp comp)
{
    while (hi > lo) {
        char* max = lo;
        for (char* p = lo + width; p <= hi; p += width) {
            if (comp(p, max) > 0) max = p;
        }
        swap_bytes(max, hi, width);
        hi -= width;
    }
}

// @src 140484bd0  qsort (MSVC CRT, median of three, CUTOFF 8, explicit stack of 2 x 0x3e
// entries at rsp+0x30 / rsp+0x220). The parameter check (null base with num != 0, zero width or
// null comparator -> errno 22 and the invalid parameter handler) cannot trigger here.
void crt_qsort_impl(void* base, std::size_t num, std::size_t width, Cmp comp)
{
    if (num < 2) return;
    char* lostk[62];
    char* histk[62];
    std::ptrdiff_t stkptr = 0;
    char* lo = static_cast<char*>(base);
    char* hi = static_cast<char*>(base) + width * (num - 1);

recurse:
    {
        const std::size_t size = static_cast<std::size_t>(hi - lo) / width + 1;
        if (size <= 8) {
            shortsort(lo, hi, width, comp);
        } else {
            char* mid = lo + (size / 2) * width;
            if (comp(lo, mid) > 0) swap_bytes(lo, mid, width);
            if (comp(lo, hi) > 0) swap_bytes(lo, hi, width);
            if (comp(mid, hi) > 0) swap_bytes(mid, hi, width);

            char* loguy = lo;
            char* higuy = hi;
            for (;;) {
                if (mid > loguy) {
                    do {
                        loguy += width;
                    } while (loguy < mid && comp(loguy, mid) <= 0);
                }
                if (mid <= loguy) {
                    do {
                        loguy += width;
                    } while (loguy <= hi && comp(loguy, mid) <= 0);
                }
                do {
                    higuy -= width;
                } while (higuy > mid && comp(higuy, mid) > 0);
                if (higuy < loguy) break;
                swap_bytes(loguy, higuy, width);
                if (mid == higuy) mid = loguy;
            }

            higuy += width;
            if (mid < higuy) {
                do {
                    higuy -= width;
                } while (higuy > mid && comp(higuy, mid) == 0);
            }
            if (mid >= higuy) {
                do {
                    higuy -= width;
                } while (higuy > lo && comp(higuy, mid) == 0);
            }

            // 140484e45: signed compare of the byte distances
            if (higuy - lo >= hi - loguy) {
                if (lo < higuy) {
                    lostk[stkptr] = lo;
                    histk[stkptr] = higuy;
                    ++stkptr;
                }
                if (loguy < hi) {
                    lo = loguy;
                    goto recurse;
                }
            } else {
                if (loguy < hi) {
                    lostk[stkptr] = loguy;
                    histk[stkptr] = hi;
                    ++stkptr;
                }
                if (lo < higuy) {
                    hi = higuy;
                    goto recurse;
                }
            }
        }
    }
    --stkptr;
    if (stkptr >= 0) {
        lo = lostk[stkptr];
        hi = histk[stkptr];
        goto recurse;
    }
}


} // namespace

void crt_qsort(void* base, std::size_t num, std::size_t width, int (*comp)(const void*, const void*))
{
    crt_qsort_impl(base, num, width, comp);
}

} // namespace def::rt
