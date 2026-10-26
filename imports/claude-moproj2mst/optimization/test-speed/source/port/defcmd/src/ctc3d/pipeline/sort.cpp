// Packet p67-7, group "pipeline": the column sort of integer tables A(nrow, ncol) used by
// FUN_1400501d7 (edge list), and the plane box FUN_140080a83.
//
// FUN_14008b65b sorts the columns of A lexicographically by rows 1, 2, ..., nrow: a quicksort of
// all columns on row 1, then each run of columns equal in rows 1..k is quicksorted on row k+1.
// The quicksort is the binary's (FUN_14008bb26 / 14008bd37 descending, 14008bf48 / 14008c159
// ascending, each pair calling each other because the Fortran is not RECURSIVE): pivot = the
// middle column moved to the front, Lomuto partition, recursion into both halves. Each routine
// keeps its partition indices in SAVE variables, so a nested call of the same routine (two levels
// down) overwrites them and the caller then recurses on the overwritten index: this decides the
// order of equal keys (and can leave runs unsorted) and is reproduced by keeping those variables
// in the image (DAT_1406dc278.. DAT_1406dc294).
#include <defcmd/ctc3d/pipeline.hpp>
#include <defcmd/ctc3d/search.hpp>

#include <defrt/ftn/geoutl_pending.hpp>
#include <defrt/ftn/sse_ops.hpp>

namespace def::cmd::ctc3d {

namespace {

inline i32& I(std::uint64_t a) { return m<i32>(a); }
inline i32* P(std::uint64_t a) { return mp<i32>(a); }
inline double& D(std::uint64_t a) { return m<double>(a); }

inline std::int64_t lead(const i32* nrow) { return *nrow >= 0 ? static_cast<std::int64_t>(*nrow) : 0; }

// A(row, col), 1-based, leading dimension ld
inline i32& A(i32* a, std::int64_t ld, i32 row, i32 col)
{
    return a[static_cast<std::int64_t>(row) + (static_cast<std::int64_t>(col) - 1) * ld - 1];
}

// part of @src 14008bb26 / 14008bd37 / 14008bf48 / 14008c159: one quicksort level; lo_hi_addr is
// the routine's SAVE pair (last index, scan index), desc selects the comparison, next is the
// partner routine for the recursion
using SortFn = void (*)(i32*, const i32*, const i32*, const i32*, const i32*);

void qsort_level(i32* a, const i32* nrow, const i32* key, const i32* lo, const i32* hi,
                 std::uint64_t last_addr, bool desc, SortFn next)
{
    const std::int64_t ld = lead(nrow);
    if (!(*lo < *hi)) {
        return;
    }
    i32 local = (*lo + *hi) / 2; // SAR/AND/ADD/SAR: toward zero
    fun_14008ba00(a, nrow, lo, &local);
    const std::uint64_t scan_addr = last_addr + 4;
    I(last_addr) = *lo;
    I(scan_addr) = I(last_addr) + 1;
    if (I(scan_addr) <= *hi) {
        // 14008bbef: the pivot value is read once into a register
        const i32 pivot = A(a, ld, *key, *lo);
        do {
            const i32 v = A(a, ld, *key, I(scan_addr));
            const bool move = desc ? (v > pivot) : (v < pivot);
            if (move) {
                I(last_addr) = I(last_addr) + 1;
                local = I(scan_addr);
                fun_14008ba00(a, nrow, mp<i32>(last_addr), &local);
            }
            I(scan_addr) = I(scan_addr) + 1;
        } while (I(scan_addr) <= *hi);
    }
    fun_14008ba00(a, nrow, lo, mp<i32>(last_addr));
    local = I(last_addr) - 1;
    next(a, nrow, key, lo, &local);
    // the SAVE index is read again here, after the nested calls (see the file comment)
    local = I(last_addr) + 1;
    next(a, nrow, key, &local, hi);
}

} // namespace

// @src 14008ba00  swap columns *i and *j of A(nrow, *)
void fun_14008ba00(i32* a, const i32* nrow, const i32* i, const i32* j)
{
    const i32 n = *nrow;
    const std::int64_t ld = n >= 0 ? static_cast<std::int64_t>(n) : 0;
    I(0x1406dc270) = 1;
    if (n < 1) {
        return;
    }
    const std::int64_t ci = (static_cast<std::int64_t>(*i) - 1) * ld;
    const std::int64_t cj = (static_cast<std::int64_t>(*j) - 1) * ld;
    do {
        const std::int64_t r = I(0x1406dc270);
        I(0x1406dc274) = a[ci + r - 1];
        a[ci + r - 1] = a[cj + r - 1];
        a[cj + r - 1] = I(0x1406dc274);
        I(0x1406dc270) = static_cast<i32>(r) + 1;
    } while (I(0x1406dc270) <= n);
}

// @src 14008bb26  quicksort of columns *lo..*hi of A on row *key, descending (calls 14008bd37)
void fun_14008bb26(i32* a, const i32* nrow, const i32* key, const i32* lo, const i32* hi)
{
    qsort_level(a, nrow, key, lo, hi, 0x1406dc278, true, &fun_14008bd37);
}

// @src 14008bd37  copy of 14008bb26 with its own SAVE indices (calls 14008bb26)
void fun_14008bd37(i32* a, const i32* nrow, const i32* key, const i32* lo, const i32* hi)
{
    qsort_level(a, nrow, key, lo, hi, 0x1406dc280, true, &fun_14008bb26);
}

// @src 14008bf48  quicksort of columns *lo..*hi of A on row *key, ascending (calls 14008c159)
void fun_14008bf48(i32* a, const i32* nrow, const i32* key, const i32* lo, const i32* hi)
{
    qsort_level(a, nrow, key, lo, hi, 0x1406dc288, false, &fun_14008c159);
}

// @src 14008c159  copy of 14008bf48 with its own SAVE indices (calls 14008bf48)
void fun_14008c159(i32* a, const i32* nrow, const i32* key, const i32* lo, const i32* hi)
{
    qsort_level(a, nrow, key, lo, hi, 0x1406dc290, false, &fun_14008bf48);
}

// @src 14008b65b  sort the *ncol columns of A(*nrow, *) lexicographically on rows 1..nrow,
// ascending when *dir == 1, else descending
void fun_14008b65b(i32* a, const i32* nrow, const i32* ncol, const i32* dir)
{
    const std::int64_t ld = lead(nrow);
    I(0x1406dc248) = 1;
    I(0x1406dc24c) = 1;
    I(0x1406dc250) = *ncol;
    if (*dir == 1) {
        fun_14008bf48(a, nrow, P(0x1406dc248), P(0x1406dc24c), P(0x1406dc250));
    } else {
        fun_14008bb26(a, nrow, P(0x1406dc248), P(0x1406dc24c), P(0x1406dc250));
    }
    const i32 r14 = *ncol - 1; // 14008b70f: read once
    I(0x1406dc254) = 1;        // start of the current run
    I(0x1406dc258) = 1;        // rows 1..258 define the runs
    for (;;) {
        // 14008b744: find the end 25c of the run starting at 254
        I(0x1406dc25c) = I(0x1406dc254);
        i32 ebx; // column after the run
        if (I(0x1406dc254) > r14) {
            ebx = I(0x1406dc25c) + 1;
        } else {
            const i32 r11 = I(0x1406dc258) - 1;
            const bool dl = r11 >= 1;
            const i32 row = I(0x1406dc258);
            ebx = I(0x1406dc25c) + 1;
            bool broke = false;
            for (;;) {
                // 14008b77e
                const i32 c0 = I(0x1406dc25c);
                if (A(a, ld, row, c0) != A(a, ld, row, ebx)) {
                    broke = true;
                    break;
                }
                if (I(0x1406dc258) != 1) {
                    I(0x1406dc260) = r11;
                    I(0x1406dc264) = 1;
                    if (dl) {
                        do {
                            const i32 r = I(0x1406dc260);
                            if (A(a, ld, r, c0) != A(a, ld, r, ebx)) {
                                broke = true;
                                break;
                            }
                            I(0x1406dc260) = r - 1;
                            I(0x1406dc264) = I(0x1406dc264) + 1;
                        } while (I(0x1406dc264) <= r11);
                        if (broke) {
                            break;
                        }
                    }
                }
                I(0x1406dc25c) = I(0x1406dc25c) + 1;
                ++ebx;
                if (I(0x1406dc25c) > r14) {
                    break;
                }
            }
            if (!broke) {
                // 14008b85f
                ebx = I(0x1406dc25c) + 1;
            }
        }
        // 14008b86b: sort the run 254..25c on the next row
        I(0x1406dc268) = I(0x1406dc25c) - I(0x1406dc254) + 1;
        if (I(0x1406dc268) > 1) {
            I(0x1406dc26c) = 1;
            i32 key = I(0x1406dc258) + 1;
            i32* sub = a + (static_cast<std::int64_t>(I(0x1406dc254)) - 1) * ld;
            if (*dir == 1) {
                fun_14008bf48(sub, nrow, &key, P(0x1406dc26c), P(0x1406dc268));
            } else {
                fun_14008bb26(sub, nrow, &key, P(0x1406dc26c), P(0x1406dc268));
            }
        }
        // 14008b8fd
        const i32 edx = *ncol;
        const i32 edi = *nrow - 1;
        const bool at_end = edx <= ebx;
        I(0x1406dc254) = ebx;
        if (!at_end) {
            continue;
        }
        if (I(0x1406dc258) >= edi) {
            return;
        }
        I(0x1406dc258) = I(0x1406dc258) + 1;
        I(0x1406dc254) = 1;
        if (edi > I(0x1406dc258)) {
            continue;
        }
        if (edx > 1) {
            continue;
        }
        return;
    }
}

// @src 140080a83  box(6) around the square of half-diagonal *r centred at c(3) in the plane with
// unit normal n(3) (two in-plane axes from cross products), widened by 1e-3f
void fun_140080a83(const double* c, const double* n, const double* r, double* box)
{
    namespace sse = def::rt::sse;
    double* const T = mp<double>(0x1406d31b0); // T(1..3)
    double* const U = mp<double>(0x1406d31d0); // U(1..3)
    const double n1 = n[0];
    const double n2 = n[1];
    const double n3 = n[2];
    I(0x1406d31a4) = 1;
    if (n1 > n2) {
        I(0x1406d31a4) = 2;
    }
    // 140080ae7: smallest component (index imin, value vmin)
    std::int64_t imin = I(0x1406d31a4);
    double vmin = n[imin - 1];
    if (vmin > n3) {
        imin = 3;
        vmin = n3;
    }
    I(0x1406d31a8) = 1;
    if (n2 > n1) {
        I(0x1406d31a8) = 2;
    }
    if (n3 > vmin) {
        I(0x1406d31a8) = 3;
    }
    const std::int64_t ia = I(0x1406d31a8);
    T[0] = n1;
    const double xa = n[ia - 1];
    T[1] = n2;
    T[2] = n3;
    T[imin - 1] = xa;
    T[ia - 1] = vmin;
    // 140080b96: ordered equality
    if (T[imin - 1] == vmin) {
        T[imin - 1] = sse::mul(T[imin - 1], 2.0); // DAT_140bad4d0
    }
    double* const nn = const_cast<double*>(n);
    fun_140066ff1(nn, T, U, mp<double>(0x1406d31f0));
    fun_140066ff1(nn, U, T, mp<double>(0x1406d31f0));
    // 140080c16: the four corners X(3, 4) at DAT_1406d3200: c + rT, c + rU, c - rT, c - rU
    I(0x1406d31f8) = 1;
    do {
        const std::int64_t i = I(0x1406d31f8) - 1;
        const double rr = *r;
        const double ci = c[i];
        const double rt = sse::mul(rr, T[i]);
        D(0x1406d3200 + 8 * i) = sse::add(ci, rt);
        const double ru = sse::mul(rr, U[i]);
        D(0x1406d3230 + 8 * i) = sse::sub(ci, rt);
        D(0x1406d3248 + 8 * i) = sse::sub(ci, ru);
        I(0x1406d31f8) = static_cast<i32>(i) + 2;
        D(0x1406d3218 + 8 * i) = sse::add(ci, ru);
    } while (I(0x1406d31f8) <= 3);
    i32 four = 4;
    D(0x1406d3260) = 0.0010000000474974513; // DAT_140bad3d8 = (double)1e-3f
    def::rt::geoutl::fun_1404d1b1c(mp<double>(0x1406d3200), box, mp<double>(0x1406d3260), &four,
                                   P(0x1405c8860));
}

} // namespace def::cmd::ctc3d
