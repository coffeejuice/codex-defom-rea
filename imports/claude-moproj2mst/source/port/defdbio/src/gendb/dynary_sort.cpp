// GENDB table sizes and in-place table sorting (packet p4-02a): the DYNARY helpers and the
// Fortran routines TPR_FNC_2DSORT / ACTUAL_2DSORT / TPR_FNC_3DSORT / ACTUAL_3DSORT and the
// column sort they share.
//
// The Fortran routines keep their loop counters and work-array descriptors in static storage
// (DAT_1406fe780..DAT_1406fe96c, SAVE'd locals: no other function references that range); the
// port keeps them local. Arrays are column major and 1-based in the comments.
#include <defdbio/gendb/obj_counts.hpp>

#include <cstdlib>
#include <cstring>
#include <vector>

#include <defmodel/dynary_init.hpp>
#include <defmodel/llobj/dynary_table.hpp>

namespace def::dbio::gendb {

namespace {

ObjCountExt& ext() { return obj_count_ext(); }

// Fortran ALLOCATE of n doubles (FUN_1404b62c4 with n * 8 bytes; n < 0 allocates nothing).
std::vector<double> ftn_alloc(std::int64_t n) { return std::vector<double>(n < 0 ? 0 : static_cast<std::size_t>(n)); }

void debug_line(const std::int32_t* unit, const char* text, std::int32_t len)
{
    ext().ftn_debug_write(*unit, text, len);
}

} // namespace

// @src 140130230  n[0] * (ncomp + 1), rounded up to an even number
std::int32_t dynary_words_1d_padded(const DYNARY* t)
{
    std::int32_t w = t->n[0] * (t->ncomp + 1);
    if (w % 2 == 1)  // (w & 1 ^ sign) - sign == 1
        w = w + 1;
    return w;
}

// @src 140134120  sort the table and remove repeated abscissae. ndim 1: rows (x, values) through
// 14023829e; ndim 2: TPR_FNC_2DSORT on (x1, x2, values); ndim 3 with ncomp < 3: TPR_FNC_3DSORT
// on (x1, x2, x3, first component), and for ncomp 2 again on (original axes, second component).
// The table is reallocated (14012fad0) with the new shape and the results copied back. For
// ndim 2 only the first n1 * n2 values take part in the sort; the remaining components are
// copied back from the unsorted positions, as in the binary.
void dynary_sort(DYNARY* t)
{
    if (t->ndim == 1) {
        std::int32_t n = t->n[0];
        std::int32_t lda = t->ncomp + 1;
        std::int32_t nkey = 1;
        const std::int32_t cnt = lda * n;
        auto* buf = static_cast<double*>(std::calloc(static_cast<std::size_t>(static_cast<std::int64_t>(cnt)), 8));
        for (std::int32_t i = 0; i < t->n[0]; ++i) {
            buf[i * lda] = t->axis[0][i];
            for (std::int32_t c = 0; c < t->ncomp; ++c)
                buf[i * lda + 1 + c] = model::llobj::dynary_get(*t, i, 0, 0, c);  // 140130720
        }
        sort_dedupe_columns(buf, &lda, &nkey, &n);
        t->n[0] = n;
        model::dynary_alloc(t);  // 14012fad0
        for (std::int32_t i = 0; i < t->n[0]; ++i) {
            t->axis[0][i] = buf[i * (t->ncomp + 1)];
            for (std::int32_t c = 0; c < t->ncomp; ++c)
                model::llobj::dynary_set(*t, buf[i * (t->ncomp + 1) + 1 + c], i, 0, 0, c);  // 140130800
        }
        std::free(buf);
    } else if (t->ndim == 2) {
        std::int32_t ndim = 2;
        std::int32_t n1 = t->n[0];
        std::int32_t n2 = t->n[1];
        std::int32_t nkey = 1;
        std::int32_t unit = 6;
        std::int32_t ierr = 0;
        std::int32_t debug = 1;
        auto* buf = static_cast<double*>(std::calloc(
            static_cast<std::size_t>(static_cast<std::int64_t>(n1 + n2 + n1 * n2 * t->ncomp)), 8));
        std::memcpy(buf, t->axis[0], static_cast<std::size_t>(static_cast<std::int64_t>(n1)) << 3);
        std::memcpy(buf + n1, t->axis[1], static_cast<std::size_t>(static_cast<std::int64_t>(n2)) << 3);
        std::memcpy(buf + (n1 + n2), t->values,
                    static_cast<std::size_t>(static_cast<std::int64_t>(model::dynary_value_count(t))) << 3);
        tpr_fnc_2dsort(buf, &ndim, &nkey, &n1, &n2, &unit, &ierr, &debug);
        t->n[0] = n1;
        t->n[1] = n2;
        model::dynary_alloc(t);
        std::memcpy(t->axis[0], buf, static_cast<std::size_t>(static_cast<std::int64_t>(n1)) << 3);
        std::memcpy(t->axis[1], buf + n1, static_cast<std::size_t>(static_cast<std::int64_t>(n2)) << 3);
        std::memcpy(t->values, buf + (n1 + n2),
                    static_cast<std::size_t>(static_cast<std::int64_t>(model::dynary_value_count(t))) << 3);
        std::free(buf);
    } else if (t->ndim == 3 && t->ncomp < 3) {
        std::int32_t ndim = 3;
        std::int32_t n1 = t->n[0];
        std::int32_t n2 = t->n[1];
        std::int32_t n3 = t->n[2];
        std::int32_t nkey = 1;
        std::int32_t unit = 6;
        std::int32_t ierr = 0;
        std::int32_t debug = 1;
        const std::int32_t nv = n1 * n2 * n3;
        auto* buf = static_cast<double*>(std::calloc(static_cast<std::size_t>(static_cast<std::int64_t>(n1 + n2 + n3 + nv)), 8));
        std::memcpy(buf, t->axis[0], static_cast<std::size_t>(static_cast<std::int64_t>(n1)) << 3);
        std::memcpy(buf + n1, t->axis[1], static_cast<std::size_t>(static_cast<std::int64_t>(n2)) << 3);
        std::memcpy(buf + (n1 + n2), t->axis[2], static_cast<std::size_t>(static_cast<std::int64_t>(n3)) << 3);
        std::memcpy(buf + (n1 + n2 + n3), t->values, static_cast<std::size_t>(static_cast<std::int64_t>(nv)) << 3);
        double* buf2 = nullptr;
        if (t->ncomp == 2) {
            buf2 = static_cast<double*>(std::calloc(static_cast<std::size_t>(static_cast<std::int64_t>(n1 + n2 + n3 + nv)), 8));
            std::memcpy(buf2, t->axis[0], static_cast<std::size_t>(static_cast<std::int64_t>(n1)) << 3);
            std::memcpy(buf2 + n1, t->axis[1], static_cast<std::size_t>(static_cast<std::int64_t>(n2)) << 3);
            std::memcpy(buf2 + (n1 + n2), t->axis[2], static_cast<std::size_t>(static_cast<std::int64_t>(n3)) << 3);
            std::memcpy(buf2 + (n1 + n2 + n3), t->values + nv, static_cast<std::size_t>(static_cast<std::int64_t>(nv)) << 3);
        }
        // second call's arguments: copies of the shape before the first call
        std::int32_t ndim_b = ndim;
        std::int32_t nkey_b = nkey;
        std::int32_t n1_b = n1;
        std::int32_t n2_b = n2;
        std::int32_t n3_b = n3;
        tpr_fnc_3dsort(buf, &ndim, &nkey, &n1, &n2, &n3, &unit, &ierr, &debug);
        t->n[0] = n1;
        t->n[1] = n2;
        t->n[2] = n3;
        model::dynary_alloc(t);
        const std::int32_t nn = n1 * n2 * n3;
        std::memcpy(t->axis[0], buf, static_cast<std::size_t>(static_cast<std::int64_t>(n1)) << 3);
        std::memcpy(t->axis[1], buf + n1, static_cast<std::size_t>(static_cast<std::int64_t>(n2)) << 3);
        std::memcpy(t->axis[2], buf + (n1 + n2), static_cast<std::size_t>(static_cast<std::int64_t>(n3)) << 3);
        std::memcpy(t->values, buf + (n1 + n2 + n3), static_cast<std::size_t>(static_cast<std::int64_t>(nn)) << 3);
        std::free(buf);
        if (t->ncomp == 2) {
            tpr_fnc_3dsort(buf2, &ndim_b, &nkey_b, &n1_b, &n2_b, &n3_b, &unit, &ierr, &debug);
            // offsets and size from the shape of the first sort
            std::memcpy(t->values + nn, buf2 + (n1 + n2 + n3), static_cast<std::size_t>(static_cast<std::int64_t>(nn)) << 3);
            std::free(buf2);
        }
    }
}

// @src 140135bc0  a velocity table (type 0x7b, 3 components) becomes type 0x7a (4 components,
// 14012fd30 with the same point count): per point the axis value is kept, (v1, v2, 1.0) is
// multiplied by m (1402416d0) into the first three components and v3 is copied to the fourth.
// Returns 1 when converted.
std::uint8_t dynary_rotate_velocity(DYNARY* t, const double* m)
{
    std::uint8_t r;
    if (t->type == 0x7b && t->ncomp == 3) {
        DYNARY copy;  // 14012d050
        ext().dynary_copy(&copy, t);
        ext().dynary_init_n0(t, 0x7a, copy.n[0]);
        for (std::int32_t i = 0; i < t->n[0]; ++i) {
            t->axis[0][i] = copy.axis[0][i];
            double* dst = t->values + i * t->ncomp;
            const double* src = copy.values + i * copy.ncomp;
            double v[3];
            v[0] = src[0];
            v[1] = src[1];
            v[2] = 1.0;  // DAT_140509c90 = 1.0
            double w[3];
            ext().mat3_mul_vec(v, w, m);
            dst[0] = w[0];
            dst[1] = w[1];
            dst[2] = w[2];
            dst[3] = src[2];
        }
        r = 1;
    } else {
        r = 0;
    }
    return r;
}

// @src 14023829e  sort the columns of a(lda, ncol) by row k, k = 1..nkey in turn (bubble sort,
// stable; a column moves down when its key is smaller or unordered against its predecessor's,
// COMISD + SETNC), then drop every column equal (==) to the previous one in rows 1..nkey
void sort_dedupe_columns(double* a, const std::int32_t* lda, const std::int32_t* nkey,
                         std::int32_t* ncol)
{
    const std::int32_t m = *lda;
    const std::int64_t ld = m < 0 ? 0 : m;
    const auto A = [&](std::int64_t i, std::int64_t j) -> double& { return a[(i - 1) + (j - 1) * ld]; };
    if (!(1 < *ncol && 0 < *nkey && *nkey <= m))
        return;
    const std::int32_t nm1 = *ncol - 1;
    std::int32_t k = 1;
    do {
        std::int32_t pass = 1;
        if (0 < nm1) {
            std::int32_t last = nm1;
            do {
                bool swapped = false;
                if (last < 1)
                    break;
                for (std::int32_t j = 1; j <= last; ++j) {
                    if (!(A(k, j + 1) >= A(k, j))) {
                        swapped = true;
                        for (std::int32_t l = 1; l <= m; ++l) {
                            const double tmp = A(l, j);
                            A(l, j) = A(l, j + 1);
                            A(l, j + 1) = tmp;
                        }
                    }
                }
                if (!swapped)
                    break;
                ++pass;
                --last;
            } while (pass <= nm1);
        }
        ++k;
    } while (k <= *nkey);

    std::int32_t js = 2;
    while (js <= *ncol) {
        std::int32_t j = js;
        for (;;) {
            std::int32_t kk = 1;
            if (!(0 < *nkey))
                goto dup;
            while (A(kk, j) == A(kk, j - 1)) {
                ++kk;
                if (*nkey < kk)
                    goto dup;
            }
            ++j;
            if (*ncol < j)
                return;
        }
    dup:
        if (*ncol < j + 1) {
            *ncol = *ncol - 1;
            return;
        }
        for (std::int32_t jj = j + 1; jj <= *ncol; ++jj)
            for (std::int32_t l = 1; l <= m; ++l)
                A(l, jj - 1) = A(l, jj);
        js = j;
        *ncol = *ncol - 1;
    }
}

// @src 140236f5c  TPR_FNC_2DSORT(a, ndim, nkey, n1, n2, unit, ierr, debug): a = (x1(n1), x2(n2),
// v(n1, n2)). ierr = 1 unless ndim is 2, n1 + n2 + n1 * n2 >= 4 and n1, n2 != 0. Allocates
// w1(n1 * (n2 + 1)) and w2(n2 * (n1 + 1)) and calls ACTUAL_2DSORT with l1 = n2 + 1,
// l2 = n1 + 1 (DAT_1406fe8fc / DAT_1406fe904). Debug lines for debug > 1.
void tpr_fnc_2dsort(double* a, const std::int32_t* ndim, const std::int32_t* nkey, std::int32_t* n1,
                    std::int32_t* n2, const std::int32_t* unit, std::int32_t* ierr,
                    const std::int32_t* debug)
{
    const std::int32_t m2 = *n2;
    const std::int32_t tot = *n1 + m2 + m2 * *n1;
    if (tot < 4 || *ndim != 2) {
        *ierr = 1;
        return;
    }
    if (*n1 == 0 || m2 == 0) {
        *ierr = 1;
        return;
    }
    if (1 < *debug)
        debug_line(unit, "TPR_FNC_2DSORT : Start", 0x16);
    std::int32_t l1 = *n2 + 1;
    std::vector<double> w1 = ftn_alloc(static_cast<std::int64_t>(*n1 * l1));
    std::int32_t l2 = *n1 + 1;
    std::vector<double> w2 = ftn_alloc(static_cast<std::int64_t>(*n2 * l2));
    actual_2dsort(a, w1.data(), w2.data(), n1, n2, &l1, &l2, nkey, unit, ierr, debug);
    if (1 < *debug)
        debug_line(unit, "TPR_FNC_2DSORT : Exiting", 0x18);
}

// @src 1402375da  ACTUAL_2DSORT(a, w1, w2, n1, n2, l1, l2, nkey, unit, ierr, debug):
//   w1(1, i) = x1(i), w1(1 + j, i) = v(i, j); sort w1's n1 columns (n1 shrinks)
//   w2(1, j) = x2(j), w2(1 + i, j) = w1(1 + j, i) for the remaining i; l2 = n1 + 1; sort w2's n2
//   columns (n2 shrinks); l1 = n2 + 1; a(1 .. n1 + n2 + n1 * n2) = 0; then a = (w1(1, :),
//   w2(1, :), w2(2.., :)) with the new shape.
void actual_2dsort(double* a, double* w1, double* w2, std::int32_t* n1, std::int32_t* n2,
                   std::int32_t* l1, std::int32_t* l2, const std::int32_t* nkey,
                   const std::int32_t* unit, std::int32_t* ierr, const std::int32_t* debug)
{
    (void)ierr;
    const auto A = [&](std::int64_t i) -> double& { return a[i - 1]; };
    const auto W1 = [&](std::int64_t i) -> double& { return w1[i - 1]; };
    const auto W2 = [&](std::int64_t i) -> double& { return w2[i - 1]; };
    if (*debug == 3)
        debug_line(unit, "IN ACTUAL_2DSORT", 0x10);
    std::int32_t c = 0;
    for (std::int32_t i = 1; i <= *n1; ++i) {
        W1(++c) = A(i);
        for (std::int32_t j = 1; j <= *n2; ++j)
            W1(++c) = A(*n2 + *n1 + i + static_cast<std::int64_t>(j - 1) * *n1);
    }
    sort_dedupe_columns(w1, l1, nkey, n1);
    const std::int32_t l1_old = *l1;  // DAT_1406fe918
    c = 0;
    {
        const std::int32_t s1 = *n2 + 1;  // w1's leading dimension (n2 not yet sorted)
        const std::int32_t x2 = *l2;      // x2(1) is a(l2) = a(n1_old + 1)
        for (std::int32_t j = 1; j <= *n2; ++j) {
            W2(++c) = A(x2 + j - 1);
            for (std::int32_t i = 1; i <= *n1; ++i)
                W2(++c) = W1(1 + j + static_cast<std::int64_t>(i - 1) * s1);
        }
    }
    *l2 = *n1 + 1;
    sort_dedupe_columns(w2, l2, nkey, n2);
    const std::int32_t nx = *n1 + *n2;
    const std::int32_t tot = *n2 * *n1 + nx;
    *l1 = *n2 + 1;
    for (std::int32_t i = 1; i <= tot; ++i)
        A(i) = 0.0;
    for (std::int32_t idx = 1; idx <= nx; ++idx) {
        if (*n1 < idx) {
            const std::int32_t j = idx - *n1;
            A(idx) = W2(1 + static_cast<std::int64_t>(j - 1) * *l2);
            for (std::int32_t i = 1; i <= *n1; ++i)
                A(nx + i + static_cast<std::int64_t>(j - 1) * *n1) = W2(1 + i + static_cast<std::int64_t>(j - 1) * *l2);
        } else {
            A(idx) = W1(1 + static_cast<std::int64_t>(idx - 1) * l1_old);
        }
    }
    if (*debug == 3)
        debug_line(unit, "EXITING ACTUAL_2DSORT", 0x15);
}

// @src 140235ec0  TPR_FNC_3DSORT(a, ndim, nkey, n1, n2, n3, unit, ierr, debug): a = (x1(n1),
// x2(n2), x3(n3), v(n1, n2, n3)). ierr = 1 unless ndim is 3, n1 + n2 + n3 + n1 * n2 * n3 >= 5
// and n1, n2, n3 != 0. Allocates w1(n1 + n2 + n1 * n2) and w2(n3 + n1 * n2 * n3) and calls
// ACTUAL_3DSORT. Debug lines for debug == 3.
void tpr_fnc_3dsort(double* a, const std::int32_t* ndim, const std::int32_t* nkey, std::int32_t* n1,
                    std::int32_t* n2, std::int32_t* n3, const std::int32_t* unit, std::int32_t* ierr,
                    const std::int32_t* debug)
{
    const std::uint32_t m2 = static_cast<std::uint32_t>(*n2);
    const std::uint32_t p12 = m2 * static_cast<std::uint32_t>(*n1);
    const std::int32_t m3 = *n3;
    const std::uint32_t s12 = m2 + static_cast<std::uint32_t>(*n1);
    const std::uint32_t p123 = p12 * static_cast<std::uint32_t>(m3);
    const std::int32_t tot = static_cast<std::int32_t>(p123 + static_cast<std::uint32_t>(m3) + s12);
    if (tot < 5 || *ndim != 3) {
        *ierr = 1;
        return;
    }
    if (*n1 == 0 || m2 == 0 || m3 == 0) {
        *ierr = 1;
        return;
    }
    if (*debug == 3)
        debug_line(unit, "TPR_FNC_3DSORT : Starting", 0x19);
    std::vector<double> w1 = ftn_alloc(static_cast<std::int32_t>(s12 + p12));
    std::vector<double> w2 = ftn_alloc(*n3 + static_cast<std::int32_t>(p123));
    actual_3dsort(a, w1.data(), w2.data(), nkey, n1, n2, n3, unit, ierr, debug);
    if (*debug == 3)
        debug_line(unit, "TPR_FNC_3DSORT : Exiting", 0x18);
}

// @src 140236539  ACTUAL_3DSORT(a, w1, w2, nkey, n1, n2, n3, unit, ierr, debug):
//   for each k: w1 = (x1, x2, v(:, :, k)) with the original shape, TPR_FNC_2DSORT(w1) (n1, n2
//   reset to the original before each slice), the sorted slice written back compactly after
//   the original x1, x2, x3;
//   a(1 .. n1 + n2) = w1's sorted axes, x3 moved down behind them, the value block moved down
//   one word at a time until it follows x3;
//   w2(:, k) = (x3(k), slice k); sort w2's n3 columns by x3 (n3 shrinks); copied back.
void actual_3dsort(double* a, double* w1, double* w2, const std::int32_t* nkey, std::int32_t* n1,
                   std::int32_t* n2, std::int32_t* n3, const std::int32_t* unit, std::int32_t* ierr,
                   const std::int32_t* debug)
{
    const auto A = [&](std::int64_t i) -> double& { return a[i - 1]; };
    const auto W1 = [&](std::int64_t i) -> double& { return w1[i - 1]; };
    const auto W2 = [&](std::int64_t i) -> double& { return w2[i - 1]; };
    if (*debug == 3)
        debug_line(unit, "IN ACTUAL_3DSORT", 0x10);
    const std::int32_t n1o = *n1;
    const std::int32_t n2o = *n2;
    std::int32_t slice;  // n1 * n2 of the last sorted slice
    if (*n3 < 1) {
        slice = n2o * n1o;
    } else {
        const std::int32_t len = n2o * n1o + n1o + n2o;
        std::int32_t src = *n3 + 1;  // a(src + idx - 1) is slice k's word idx - n1 - n2 ... (idx > n1 + n2)
        std::int32_t k0 = 0;
        std::int32_t k = 1;
        do {
            *n2 = n2o;
            *n1 = n1o;
            for (std::int32_t idx = 1; idx <= len; ++idx) {
                if (n2o + n1o < idx)
                    W1(idx) = A(src + idx - 1);
                else
                    W1(idx) = A(idx);
            }
            std::int32_t two = 2;
            tpr_fnc_2dsort(w1, &two, nkey, n1, n2, unit, ierr, debug);
            const std::int32_t m1 = *n1;
            const std::int32_t m2 = *n2;
            slice = m1 * m2;
            const std::int32_t wlen = m1 + slice + m2;
            std::int32_t dst = *n3 + n2o + (n1o - (m1 + m2)) + 1 + m1 * k0 * m2;
            for (std::int32_t idx = 1; idx <= wlen; ++idx, ++dst) {
                if (m1 + m2 < idx)
                    A(dst) = W1(idx);
            }
            src = src + n2o * n1o;
            ++k0;
            ++k;
        } while (k <= *n3);
    }
    const std::int32_t nxo = n2o + n1o;
    const std::int32_t m3 = *n3;
    const std::int32_t lim = m3 + nxo;
    std::int32_t nx = *n1 + *n2;
    {
        std::int32_t from = nxo - *n1 - *n2;
        for (std::int32_t idx = 1; idx <= lim; ++idx) {
            ++from;
            // ((n1 < idx && nx < idx) || (a(idx) = w1(idx), nx < idx)) && idx <= n3 + nx
            bool move;
            if (*n1 < idx && nx < idx) {
                move = true;
            } else {
                A(idx) = W1(idx);
                move = nx < idx;
            }
            if (move && idx <= m3 + nx)
                A(idx) = A(from);
        }
    }
    const std::int32_t nx_saved = nx;  // local_a8
    if (0 < nxo - nx) {
        const std::int32_t start = *n3 + 1 + nx;
        const std::int32_t cnt = (n2o + n1o + *n3 * slice) - nx;
        for (std::int32_t pass = 1; pass <= nxo - nx; ++pass)
            for (std::int32_t m = 1; m <= cnt; ++m)
                A(start + m - 1) = A(start + m);
    }
    std::int32_t c = 0;
    if (0 < *n3) {
        std::int32_t vs = *n3 + 1 + nx;
        for (std::int32_t k = 1; k <= *n3; ++k) {
            nx = nx + 1;
            W2(++c) = A(nx);
            for (std::int32_t m = 1; m <= slice; ++m)
                W2(++c) = A(vs + m - 1);
            vs = vs + slice;
        }
    }
    std::int32_t lda = slice + 1;  // DAT_1406fe840
    sort_dedupe_columns(w2, &lda, nkey, n3);
    c = 0;
    if (0 < *n3) {
        std::int32_t vs = *n3 + 1 + nx_saved;
        std::int32_t xs = *n1 + 1 + *n2;
        for (std::int32_t k = 1; k <= *n3; ++k) {
            A(xs) = W2(++c);
            for (std::int32_t m = 1; m <= slice; ++m)
                A(vs + m - 1) = W2(++c);
            vs = vs + slice;
            xs = xs + 1;
        }
    }
    if (*debug == 3)
        debug_line(unit, "EXITING ACTUAL_3DSORT", 0x15);
}

} // namespace def::dbio::gendb
