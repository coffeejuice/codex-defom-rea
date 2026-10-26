// Byte swapping, run-length coding, RMALLOC and small helpers of the DB core. See dbcore/util.hpp.
#define _CRT_SECURE_NO_WARNINGS
#include <defdbio/dbcore/util.hpp>

#include <defdbio/dbcore/globals_p1_12.hpp>

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <map>

#include <emmintrin.h>

namespace def::dbio::dbcore {

// ---- byte order ------------------------------------------------------------------------------

// @src 140407e60  return DAT_1406277a0
std::int32_t swap_mode() { return g_swap_mode; }

// @src 140407e6d  constant 2
std::int32_t platform_swap_mode() { return 2; }

// @src 140407e75  DAT_1406277a0 = *mode
void set_swap_mode(const std::int32_t* mode) { g_swap_mode = *mode; }

// @src 1404445bf  bytes 0123 -> 3210
void swap4(void* p)
{
    auto* b = static_cast<unsigned char*>(p);
    const unsigned char t1 = b[1];
    const unsigned char t0 = b[0];
    b[0] = b[3];
    b[1] = b[2];
    b[2] = t1;
    b[3] = t0;
}

// @src 14044461c  loop over FUN_1404445bf
void swap4_words(void* p, const std::int32_t* n)
{
    auto* b = static_cast<unsigned char*>(p);
    for (std::int32_t i = 1; i <= *n; ++i) {
        swap4(b + static_cast<std::ptrdiff_t>(i - 1) * 4);
    }
}

// @src 140444955  bytes 01234567 -> 76543210
void swap8(void* p)
{
    auto* b = static_cast<unsigned char*>(p);
    for (int i = 0; i < 4; ++i) {
        const unsigned char t = b[i];
        b[i] = b[7 - i];
        b[7 - i] = t;
    }
}

// @src 1404449af  loop over FUN_140444955
void swap8_words(void* p, const std::int32_t* n)
{
    auto* b = static_cast<unsigned char*>(p);
    for (std::int32_t i = 1; i <= *n; ++i) {
        swap8(b + static_cast<std::ptrdiff_t>(i - 1) * 8);
    }
}

// ---- run-length coding -----------------------------------------------------------------------
//
// Both pairs share one body; the real versions keep count, link and length in REAL*8 words and
// convert them with truncation (CVTTSD2SI) and CVTSI2SD. Run values are compared with ==, so for
// reals a NaN never starts a run and -0.0 joins a run of +0.0 (stored as the run's first value).

namespace {

std::int32_t as_int(std::int32_t v) { return v; }
std::int32_t as_int(double v) { return _mm_cvttsd_si32(_mm_set_sd(v)); } // CVTTSD2SI
template <class T>
T from_int(std::int32_t v) { return static_cast<T>(v); }

// part of @src 14044ac31 / 14044b0fa
template <class T>
void rle_encode(std::int32_t n, T* a1, std::int32_t* stored)
{
    T* a = a1 - 1; // 1-based
    std::int32_t link = 0;
    std::int32_t next; // always *stored + 1: where the next literal or count goes
    if (n < 4) {
        next = n + 1;
        *stored = n;
    } else {
        T cur = a[1];
        next = 2;
        std::int32_t cnt = 1;
        std::int32_t open = 1;
        *stored = 1;
        // Ends a run of `cnt` equal values whose first one is already stored: 2 or 3 become
        // literals, 4 or more become (value, count, link).
        const auto flush = [&](T value, std::int32_t old_link) {
            if (cnt == 2) {
                const std::int32_t t = *stored;
                *stored = t + 1;
                next = next + 1;
                a[t + 1] = value;
            } else if (cnt == 3) {
                const std::int32_t t = *stored;
                const std::int32_t at = next;
                next = next + 2;
                *stored = t + 2;
                a[at] = value;
                a[t + 2] = value;
            } else if (3 < cnt) {
                const T c = from_int<T>(cnt);
                const T l = from_int<T>(old_link);
                const std::int32_t at = next;
                next = next + 2;
                link = *stored + 2;
                *stored = link;
                a[at] = c;
                a[link] = l;
            }
        };
        for (std::int32_t i = 2; i <= n; ++i) {
            const T value = cur;
            const std::int32_t old_link = link;
            if (a[i] == cur) {
                cnt = cnt + 1;
                if (n <= i) {
                    open = 0;
                    flush(value, old_link);
                }
            } else {
                flush(value, old_link);
                if (open == 1) {
                    const std::int32_t t = *stored;
                    a[t + 1] = a[i];
                    cur = a[i];
                    next = next + 1;
                    *stored = t + 1;
                    cnt = 1;
                }
            }
        }
    }
    const T l = from_int<T>(link);
    const std::int32_t t = *stored;
    a[next] = from_int<T>(n);
    *stored = t + 2;
    a[t + 2] = l;
}

// part of @src 14044aeeb / 14044b3f5
template <class T>
void rle_decode(std::int32_t n, T* a1, std::int32_t* out)
{
    T* a = a1 - 1;
    std::int32_t link = as_int(a[n]);
    *out = as_int(a[n - 1]);
    if (link == 0 || n <= 2) {
        return;
    }
    std::int32_t dst = *out;
    std::int32_t pos = n - 2;
    std::int32_t pc = n - 3; // count word of a triple whose link word is at pos
    std::int32_t pv = n - 4; // value word
    do {
        const T v = a[pos];
        if (pos == link) {
            link = as_int(v);
            const std::int32_t cnt = as_int(a[pc]);
            const T value = a[pv];
            for (std::int32_t k = 1; k <= cnt; ++k) {
                a[dst] = value;
                dst = dst - 1;
            }
            pos -= 3;
            pc -= 3;
            pv -= 3;
        } else {
            pc -= 1;
            pos -= 1;
            pv -= 1;
            a[dst] = v;
            dst = dst - 1;
        }
    } while (0 < pos && 0 < dst);
}

} // namespace

// @src 14044ac31  RLE encode, INTEGER
void rle_encode_int(const std::int32_t* n, std::int32_t* a, std::int32_t* stored) { rle_encode(*n, a, stored); }
// @src 14044b0fa  RLE encode, REAL*8
void rle_encode_real(const std::int32_t* n, double* a, std::int32_t* stored) { rle_encode(*n, a, stored); }
// @src 14044aeeb  RLE decode in place, INTEGER
void rle_decode_int(const std::int32_t* stored, std::int32_t* a, std::int32_t* n) { rle_decode(*stored, a, n); }
// @src 14044b3f5  RLE decode in place, REAL*8
void rle_decode_real(const std::int32_t* stored, double* a, std::int32_t* n) { rle_decode(*stored, a, n); }

// @src 14044ac11  MOD with 0 mapped to the modulus
std::int32_t mod1(const std::int32_t* i, const std::int32_t* m)
{
    const std::int32_t r = static_cast<std::int32_t>(static_cast<std::int64_t>(*i) % *m);
    return r != 0 ? r : *m;
}

// ---- RMALLOC / RFREE --------------------------------------------------------------------------

namespace {

struct Block {
    void* p;
    std::uint64_t bytes;
};
// Virtual address -> block, for blocks allocated against a Fortran base array. Addresses start
// above the bases so that indices are positive, and stay within 32-bit indices of them.
std::map<std::uint64_t, Block>& space()
{
    static std::map<std::uint64_t, Block> s;
    return s;
}
std::uint64_t g_next_vaddr = 0x150000000;

std::FILE* err_stream() { return stdout; } // FUN_14047903c() + 0x30: &_iob[1] (0x30-byte FILE), the CRT's stdout

// part of @src 14043c190
void* allocate(const std::int32_t* n, std::int32_t* total)
{
    const std::uint32_t words = static_cast<std::uint32_t>(*n);
    void* p = std::calloc(words, 4);
    if (p == nullptr) {
        const std::int32_t have = *total * kRmWordBytes;
        const std::int32_t want = *n * kRmWordBytes;
        // str 140636e40, 140636f00
        std::fprintf(err_stream(), "SFTC - Fatal error in RMALLOC. Ask %d, Total %d\n", want, have);
        std::fprintf(err_stream(), "The above error should not have happened. Contact SFTC.\n");
        std::exit(-1); // FUN_14047ab18(0xffffffff)
    }
    if (g_rm_trace != 0) {
        std::fprintf(err_stream(), "RMALLOC\n");                  // str 140636f98
        std::fprintf(err_stream(), "Old total %d\n", g_rm_total); // str 140636fa8
    }
    *total = *total + static_cast<std::int32_t>(words);
    g_rm_total = g_rm_total + static_cast<std::int32_t>(words);
    g_rm_allocs = g_rm_allocs + 1;
    std::int32_t peak = g_rm_total;
    if (g_rm_total < g_rm_peak) {
        peak = g_rm_peak;
    }
    g_rm_peak = peak;
    if (g_rm_trace != 0) {
        std::fprintf(err_stream(), "Size %d New total %d\n", *n, g_rm_total); // str 140636fb8
    }
    return p;
}

} // namespace

// @src 14043c190  RMALLOC (calloc, bookkeeping, index from the base array)
std::int64_t rmalloc(const std::int32_t* n, std::uint64_t base, std::int32_t* total)
{
    void* p = allocate(n, total);
    const std::uint64_t bytes = static_cast<std::uint64_t>(static_cast<std::uint32_t>(*n)) * 4;
    const std::uint64_t vaddr = g_next_vaddr;
    g_next_vaddr += (bytes + 31) & ~std::uint64_t{15};
    space()[vaddr] = Block{p, bytes};
    return static_cast<std::int64_t>((vaddr - base) >> 2) + 1;
}

void* rmalloc_scratch(const std::int32_t* n, std::int32_t* total) { return allocate(n, total); }

// @src 14043c520  RFREE
void rfree(void* p, const std::int32_t* n, std::int32_t* total)
{
    if (p == nullptr) {
        return;
    }
    for (auto it = space().begin(); it != space().end(); ++it) {
        if (it->second.p == p) {
            space().erase(it);
            break;
        }
    }
    std::free(p);
    if (g_rm_trace != 0) {
        std::fprintf(err_stream(), "RFREE\n");                    // str 140636ae4
        std::fprintf(err_stream(), "Old total %d\n", g_rm_total); // str 140637190
    }
    *total = *total - *n;
    g_rm_total = g_rm_total - *n;
    g_rm_frees = g_rm_frees + 1;
    if (g_rm_trace != 0) {
        std::fprintf(err_stream(), "New total %d\n", g_rm_total); // str 1406371a0
    }
}

void* rm_word(std::uint64_t base, std::int64_t idx)
{
    const std::uint64_t vaddr = base + static_cast<std::uint64_t>(idx - 1) * 4;
    auto it = space().upper_bound(vaddr);
    if (it == space().begin()) {
        return nullptr;
    }
    --it;
    if (vaddr >= it->first + it->second.bytes) {
        return nullptr;
    }
    return static_cast<unsigned char*>(it->second.p) + (vaddr - it->first);
}

} // namespace def::dbio::dbcore
