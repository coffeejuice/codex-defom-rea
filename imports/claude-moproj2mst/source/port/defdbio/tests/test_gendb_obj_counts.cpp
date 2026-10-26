// GENDB object counts (packet p4-02a): the Fortran table sorts (14023829e, TPR_FNC_2DSORT,
// TPR_FNC_3DSORT), DYNARY sorting and sizes (140134120, 140130230, 140135bc0), window orientation
// (140265bf0, 140240d80) and the count walk from 1401beb30 over hand-built objects. Expected
// values are worked out by hand in the comments. Model objects are zeroed storage (their ctors
// are Phase 2) with the fields each case needs.
// ctest-labels: p4-02a gendb
#include <defdbio/gendb/obj_counts.hpp>

#include <defmodel/dynary_init.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/linked/record_e8_list.hpp>
#include <testkit/check.hpp>

#include <cmath>
#include <cstdlib>
#include <cstring>
#include <limits>
#include <new>
#include <vector>

using namespace def::dbio::gendb;
using def::model::DB_STEP_DATA;
using def::model::DYNARY;
using def::model::LLOBJ_DATA;

namespace {

template <class T>
T& at(void* p, std::size_t off) { return *reinterpret_cast<T*>(static_cast<char*>(p) + off); }

void* zeroed(std::size_t n)
{
    void* p = ::operator new(n);
    std::memset(p, 0, n);
    return p;
}

double* dbl(std::initializer_list<double> v)
{
    auto* p = static_cast<double*>(std::calloc(v.size() == 0 ? 1 : v.size(), 8));
    std::size_t i = 0;
    for (double x : v) p[i++] = x;
    return p;
}

// A table in place (zeroed storage): shape, calloc'd arrays.
DYNARY* table_at(void* base, std::size_t off, std::int32_t type, std::int32_t ndim, std::int32_t ncomp,
                 std::int32_t n0, std::int32_t n1 = 0, std::int32_t n2 = 0)
{
    auto* t = static_cast<DYNARY*>(static_cast<void*>(static_cast<char*>(base) + off));
    t->type = type;
    t->ndim = ndim;
    t->ncomp = ncomp;
    t->n[0] = n0;
    t->n[1] = n1;
    t->n[2] = n2;
    def::model::dynary_alloc(t);
    return t;
}

DB_STEP_DATA* raw_step(std::int32_t dim)
{
    auto* s = static_cast<DB_STEP_DATA*>(zeroed(sizeof(DB_STEP_DATA)));
    s->dimension = dim;
    return s;
}

LLOBJ_DATA* raw_obj(DB_STEP_DATA* owner)
{
    auto* o = static_cast<LLOBJ_DATA*>(zeroed(sizeof(LLOBJ_DATA)));
    o->owner = owner;
    at<DB_STEP_DATA*>(o, 0x1ea0 + 0x2b8) = owner;   // Cls_1401860a0's owner
    at<DB_STEP_DATA*>(o, 0x2e68 + 0x198) = owner;   // Cls_1401956d0's owner
    return o;
}

} // namespace

// ---- 14023829e --------------------------------------------------------------------------------

TEST_CASE(sort_columns_stable_then_dedupe)
{
    // a(2, 4) = (3,30) (1,10) (3,31) (2,20); key row 1.
    // sorted: (1,10) (2,20) (3,30) (3,31); column 4 repeats column 3's key -> dropped.
    double a[] = {3, 30, 1, 10, 3, 31, 2, 20};
    std::int32_t lda = 2, nkey = 1, n = 4;
    sort_dedupe_columns(a, &lda, &nkey, &n);
    CHECK_EQ(n, 3);
    const double want[] = {1, 10, 2, 20, 3, 30};
    for (int i = 0; i < 6; ++i) CHECK_BITS(a[i], want[i]);
}

TEST_CASE(sort_columns_guards_and_nan)
{
    double a[] = {2, 1};
    std::int32_t lda = 1, nkey = 2, n = 2;  // nkey > lda: nothing happens
    sort_dedupe_columns(a, &lda, &nkey, &n);
    CHECK_EQ(n, 2);
    CHECK_BITS(a[0], 2.0);
    // a NaN key compares unordered: the column moves before its predecessor (COMISD, SETNC)
    const double nan = std::numeric_limits<double>::quiet_NaN();
    double b[] = {1.0, nan};
    nkey = 1;
    sort_dedupe_columns(b, &lda, &nkey, &n);
    CHECK(std::isnan(b[0]));
    CHECK_BITS(b[1], 1.0);
    CHECK_EQ(n, 2);
    // a single column is left alone
    std::int32_t one = 1;
    sort_dedupe_columns(b, &lda, &nkey, &one);
    CHECK_EQ(one, 1);
}

TEST_CASE(sort_columns_two_keys_and_middle_duplicate)
{
    // a(2, 4) by row 1 then row 2 (both stable bubble sorts): (2,1) (1,2) (1,1) (2,1)
    // by row 1: (1,2) (1,1) (2,1) (2,1); by row 2: (1,1) (2,1) (2,1) (1,2)
    // equal neighbours in rows 1..2: columns 2 and 3 -> drop 3 -> (1,1) (2,1) (1,2)
    double a[] = {2, 1, 1, 2, 1, 1, 2, 1};
    std::int32_t lda = 2, nkey = 2, n = 4;
    sort_dedupe_columns(a, &lda, &nkey, &n);
    CHECK_EQ(n, 3);
    const double want[] = {1, 1, 2, 1, 1, 2};
    for (int i = 0; i < 6; ++i) CHECK_BITS(a[i], want[i]);
}

// ---- TPR_FNC_2DSORT / TPR_FNC_3DSORT --------------------------------------------------------

TEST_CASE(sort_2d_reverses_both_axes)
{
    // x1 = (2, 1), x2 = (5, 4), v(i, j) = (a b; c d) stored v11 v21 v12 v22 = 11 21 12 22.
    // sorted: x1 = (1, 2), x2 = (4, 5), v'(i', j') = v(3 - i', 3 - j'): 22 12 21 11.
    double a[] = {2, 1, 5, 4, 11, 21, 12, 22};
    std::int32_t ndim = 2, nkey = 1, n1 = 2, n2 = 2, unit = 6, ierr = 0, dbg = 1;
    tpr_fnc_2dsort(a, &ndim, &nkey, &n1, &n2, &unit, &ierr, &dbg);
    CHECK_EQ(ierr, 0);
    CHECK_EQ(n1, 2);
    CHECK_EQ(n2, 2);
    const double want[] = {1, 2, 4, 5, 22, 12, 21, 11};
    for (int i = 0; i < 8; ++i) CHECK_BITS(a[i], want[i]);
}

TEST_CASE(sort_2d_drops_repeated_abscissae)
{
    // x1 = (1, 1, 2), x2 = (7, 3), v(:, 1) = (10, 20, 30), v(:, 2) = (40, 50, 60).
    // x1 sort keeps the first of the two 1s: x1 = (1, 2), rows (10, 40), (30, 60).
    // x2 sort: x2 = (3, 7), v' = (40, 60 | 10, 30); tail of a beyond the new size untouched
    // beyond the zeroed part 1 .. n1 + n2 + n1 n2 = 8.
    double a[] = {1, 1, 2, 7, 3, 10, 20, 30, 40, 50, 60};
    std::int32_t ndim = 2, nkey = 1, n1 = 3, n2 = 2, unit = 6, ierr = 0, dbg = 1;
    tpr_fnc_2dsort(a, &ndim, &nkey, &n1, &n2, &unit, &ierr, &dbg);
    CHECK_EQ(n1, 2);
    CHECK_EQ(n2, 2);
    const double want[] = {1, 2, 3, 7, 40, 60, 10, 30, 40, 50, 60};
    for (int i = 0; i < 11; ++i) CHECK_BITS(a[i], want[i]);
}

TEST_CASE(sort_2d_and_3d_argument_errors)
{
    double a[8] = {};
    std::int32_t ndim = 3, nkey = 1, n1 = 2, n2 = 2, n3 = 1, unit = 6, ierr = 0, dbg = 1;
    tpr_fnc_2dsort(a, &ndim, &nkey, &n1, &n2, &unit, &ierr, &dbg);
    CHECK_EQ(ierr, 1);  // ndim must be 2
    ierr = 0;
    ndim = 2;
    n1 = 1;
    n2 = 1;  // 1 + 1 + 1 < 4
    tpr_fnc_2dsort(a, &ndim, &nkey, &n1, &n2, &unit, &ierr, &dbg);
    CHECK_EQ(ierr, 1);
    ierr = 0;
    ndim = 3;
    n1 = 1;
    n2 = 1;
    n3 = 1;  // 1 + 1 + 1 + 1 < 5
    tpr_fnc_3dsort(a, &ndim, &nkey, &n1, &n2, &n3, &unit, &ierr, &dbg);
    CHECK_EQ(ierr, 1);
}

TEST_CASE(sort_3d_reverses_x1_and_x3)
{
    // x1 = (2, 1), x2 = (1), x3 = (7, 6); v(:, 1, 1) = (11, 21), v(:, 1, 2) = (12, 22).
    // sorted: x1 = (1, 2), x3 = (6, 7), v'(i, 1, k) = v(3 - i, 1, 3 - k): 22 12 21 11.
    double a[] = {2, 1, 1, 7, 6, 11, 21, 12, 22};
    std::int32_t ndim = 3, nkey = 1, n1 = 2, n2 = 1, n3 = 2, unit = 6, ierr = 0, dbg = 1;
    tpr_fnc_3dsort(a, &ndim, &nkey, &n1, &n2, &n3, &unit, &ierr, &dbg);
    CHECK_EQ(ierr, 0);
    CHECK_EQ(n1, 2);
    CHECK_EQ(n2, 1);
    CHECK_EQ(n3, 2);
    const double want[] = {1, 2, 1, 6, 7, 22, 12, 21, 11};
    for (int i = 0; i < 9; ++i) CHECK_BITS(a[i], want[i]);
}

TEST_CASE(sort_3d_compacts_after_dropping_x1)
{
    // x1 = (1, 1, 2), x2 = (5), x3 = (9, 8); v(:, 1, 1) = (10, 20, 30), v(:, 1, 2) = (40, 50, 60).
    // each slice drops the second x1 = 1: slices (10, 30) and (40, 60), n1 = 2.
    // a = (1, 2 | 5 | 9, 8 | 10, 30, 40, 60) after compaction, then the x3 sort swaps the slices:
    // (1, 2, 5, 8, 9, 40, 60, 10, 30).
    double a[] = {1, 1, 2, 5, 9, 8, 10, 20, 30, 40, 50, 60};
    std::int32_t ndim = 3, nkey = 1, n1 = 3, n2 = 1, n3 = 2, unit = 6, ierr = 0, dbg = 1;
    tpr_fnc_3dsort(a, &ndim, &nkey, &n1, &n2, &n3, &unit, &ierr, &dbg);
    CHECK_EQ(n1, 2);
    CHECK_EQ(n2, 1);
    CHECK_EQ(n3, 2);
    const double want[] = {1, 2, 5, 8, 9, 40, 60, 10, 30};
    for (int i = 0; i < 9; ++i) CHECK_BITS(a[i], want[i]);
}

// ---- DYNARY helpers --------------------------------------------------------------------------

TEST_CASE(dynary_padded_words)
{
    DYNARY t;
    t.n[0] = 3;
    t.ncomp = 2;  // 3 * 3 = 9 -> 10
    CHECK_EQ(dynary_words_1d_padded(&t), 10);
    t.ncomp = 1;  // 3 * 2 = 6
    CHECK_EQ(dynary_words_1d_padded(&t), 6);
    t.n[0] = -3;  // -9: C remainder -1, not padded
    t.ncomp = 2;
    CHECK_EQ(dynary_words_1d_padded(&t), -9);
}

TEST_CASE(dynary_sort_1d_sorts_dedupes_and_flushes)
{
    // points (3, 30) (1, 1e-31) (2, 20) (2, 25): sorted (1, ~0) (2, 20) (2, 25) (3, 30), the
    // second x = 2 dropped; 1e-31 is stored as 0 by 140130800.
    void* mem = zeroed(sizeof(DYNARY));
    DYNARY* t = table_at(mem, 0, 0x10, 1, 1, 4);
    const double x[] = {3, 1, 2, 2}, v[] = {30, 1e-31, 20, 25};
    for (int i = 0; i < 4; ++i) {
        t->axis[0][i] = x[i];
        t->values[i] = v[i];
    }
    dynary_sort(t);
    CHECK_EQ(t->n[0], 3);
    const double wx[] = {1, 2, 3}, wv[] = {0, 20, 30};
    for (int i = 0; i < 3; ++i) {
        CHECK_BITS(t->axis[0][i], wx[i]);
        CHECK_BITS(t->values[i], wv[i]);
    }
    def::model::dynary_free_arrays(t);
    ::operator delete(mem);
}

TEST_CASE(dynary_sort_2d_with_two_components)
{
    // n = (2, 1), ncomp 2: values hold 2 * 2 words, only the first n1 * n2 = 2 take part.
    // x1 = (2, 1), x2 = (5); comp 1 = (11, 21), comp 2 = (12, 22).
    // sorted: x1 = (1, 2), comp 1 = (21, 11); comp 2 copied back unsorted: (12, 22).
    void* mem = zeroed(sizeof(DYNARY));
    DYNARY* t = table_at(mem, 0, 0x10, 2, 2, 2, 1);
    t->axis[0][0] = 2;
    t->axis[0][1] = 1;
    t->axis[1][0] = 5;
    const double v[] = {11, 21, 12, 22};
    for (int i = 0; i < 4; ++i) t->values[i] = v[i];
    // n1 + n2 + n1 n2 = 2 + 1 + 2 = 5 >= 4: sorted
    dynary_sort(t);
    CHECK_EQ(t->n[0], 2);
    CHECK_EQ(t->n[1], 1);
    CHECK_BITS(t->axis[0][0], 1.0);
    CHECK_BITS(t->axis[0][1], 2.0);
    CHECK_BITS(t->axis[1][0], 5.0);
    const double w[] = {21, 11, 12, 22};
    for (int i = 0; i < 4; ++i) CHECK_BITS(t->values[i], w[i]);
    def::model::dynary_free_arrays(t);
    ::operator delete(mem);
}

TEST_CASE(dynary_sort_3d_second_component)
{
    // n = (2, 1, 2), ncomp 2: each component sorted on its own with the same axes.
    // comp 1 as in sort_3d_reverses_x1_and_x3 (11 21 12 22 -> 22 12 21 11), comp 2 = comp 1 + 100.
    void* mem = zeroed(sizeof(DYNARY));
    DYNARY* t = table_at(mem, 0, 0x10, 3, 2, 2, 1, 2);
    t->axis[0][0] = 2;
    t->axis[0][1] = 1;
    t->axis[1][0] = 1;
    t->axis[2][0] = 7;
    t->axis[2][1] = 6;
    const double v[] = {11, 21, 12, 22, 111, 121, 112, 122};
    for (int i = 0; i < 8; ++i) t->values[i] = v[i];
    dynary_sort(t);
    const double w[] = {22, 12, 21, 11, 122, 112, 121, 111};
    for (int i = 0; i < 8; ++i) CHECK_BITS(t->values[i], w[i]);
    CHECK_BITS(t->axis[2][0], 6.0);
    def::model::dynary_free_arrays(t);
    ::operator delete(mem);
}

TEST_CASE(rotate_velocity_table)
{
    // type 0x7b, 2 points, 3 components -> type 0x7a, 4 components:
    // (w0, w1, w2) = m * (v1, v2, 1), fourth = v3. m = (1 2 3; 4 5 6; 7 8 9):
    // point 1 v = (1, 2, 9): (1 + 4 + 3, 4 + 10 + 6, 7 + 16 + 9) = (8, 20, 32), 9
    void* mem = zeroed(sizeof(DYNARY));
    DYNARY* t = table_at(mem, 0, 0x7b, 1, 3, 2);
    t->axis[0][0] = 0.5;
    t->axis[0][1] = 1.5;
    const double v[] = {1, 2, 9, 0, 1, -1};
    for (int i = 0; i < 6; ++i) t->values[i] = v[i];
    const double m[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    CHECK_EQ(dynary_rotate_velocity(t, m), 1);
    CHECK_EQ(t->type, 0x7a);
    CHECK_EQ(t->ncomp, 4);
    CHECK_EQ(t->n[0], 2);
    CHECK_BITS(t->axis[0][1], 1.5);
    const double w[] = {8, 20, 32, 9, 5, 11, 17, -1};  // point 2: m * (0, 1, 1) = (5, 11, 17)
    for (int i = 0; i < 8; ++i) CHECK_BITS(t->values[i], w[i]);
    // padded words: 2 * 5 = 10
    CHECK_EQ(dynary_words_1d_padded(t), 10);
    // not a velocity table: unchanged
    CHECK_EQ(dynary_rotate_velocity(t, m), 0);
    CHECK_EQ(t->type, 0x7a);
    def::model::dynary_free_arrays(t);
    ::operator delete(mem);
}

// ---- windows -----------------------------------------------------------------------------------

TEST_CASE(polygon_normal_of_triangle_and_square)
{
    const double tri[] = {0, 0, 0, 1, 0, 0, 0, 1, 0};
    double n[3];
    polygon_normal(n, tri, 3);
    CHECK_BITS(n[0], 0.0);
    CHECK_BITS(n[1], 0.0);
    CHECK_BITS(n[2], 1.0);
    // clockwise square in the xy plane: sum of the fan cross products (0, 0, -4) -> (0, 0, -1)
    const double sq[] = {0, 0, 0, 0, 2, 0, 1, 2, 0, 1, 0, 0};
    polygon_normal(n, sq, 4);
    CHECK_BITS(n[2], -1.0);
    // degenerate: zero vector
    const double line[] = {0, 0, 0, 1, 0, 0, 2, 0, 0};
    polygon_normal(n, line, 3);
    CHECK_BITS(n[0], 0.0);
    CHECK_BITS(n[2], 0.0);
}

TEST_CASE(orient_window_2d_and_3d)
{
    void* w = zeroed(0x920);
    at<std::int32_t>(w, 0x898) = 2;  // 2D window
    at<std::int32_t>(w, 0x178) = 3;
    at<double*>(w, 0x180) = dbl({0, 0, 0, 1, 1, 0});  // clockwise: area -0.5
    std::uint8_t no = 0, yes = 1;
    CHECK_EQ(orient_window(w, &no), 0);
    CHECK_BITS(at<double*>(w, 0x180)[2], 0.0);  // unchanged
    CHECK_EQ(orient_window(w, &yes), 0);
    const double r2[] = {1, 0, 0, 1, 0, 0};
    for (int i = 0; i < 6; ++i) CHECK_BITS(at<double*>(w, 0x180)[i], r2[i]);
    CHECK_EQ(orient_window(w, &yes), 1);  // now counter-clockwise
    std::free(at<double*>(w, 0x180));

    at<std::int32_t>(w, 0x898) = 3;  // 3D
    at<double*>(w, 0x180) = dbl({0, 0, 0, 0, 1, 0, 1, 0, 0});  // normal (0, 0, -1)
    CHECK_EQ(orient_window(w, &yes), 0);
    const double r3[] = {1, 0, 0, 0, 1, 0, 0, 0, 0};
    for (int i = 0; i < 9; ++i) CHECK_BITS(at<double*>(w, 0x180)[i], r3[i]);
    CHECK_EQ(orient_window(w, &no), 1);
    std::free(at<double*>(w, 0x180));
    at<std::int32_t>(w, 0x178) = 0;
    CHECK_EQ(orient_window(w, &yes), 0);
    ::operator delete(w);
}

TEST_CASE(count_window_kinds)
{
    DB_STEP_DATA* s3 = raw_step(3);
    DB_STEP_DATA* s2 = raw_step(2);
    void* w = zeroed(0x920);
    std::int32_t nfn = 0, lfnc = 0, nwin = 0, lwin = 0;
    // kind 5: counted without words; 1D table at 0x380 (n 4 -> 8 words), 2D table at 0x3f8
    // (n (2, 2): sorted, 2 + 2 + 4 = 8 words), 1D table at 0x568 of type 0x7c (ncomp 3 -> 3 words)
    at<std::int32_t>(w, 0x174) = 5;
    table_at(w, 0x380, 0x10, 1, 1, 4);
    DYNARY* t2 = table_at(w, 0x3f8, 0x10, 2, 1, 2, 2);
    t2->axis[0][0] = 1;
    t2->axis[0][1] = 2;
    t2->axis[1][0] = 1;
    t2->axis[1][1] = 2;
    table_at(w, 0x568, 0x7c, 1, 3, 1);
    count_window(w, &nfn, &lfnc, &nwin, &lwin, s3);
    CHECK_EQ(nfn, 3);
    CHECK_EQ(lfnc, 8 + 8 + 3);
    CHECK_EQ(nwin, 1);
    CHECK_EQ(lwin, 0);
    // kind 1, inactive in 3D (+0x210 == 0): nothing
    at<std::int32_t>(w, 0x174) = 1;
    count_window(w, &nfn, &lfnc, &nwin, &lwin, s3);
    CHECK_EQ(nwin, 1);
    // kind 1, active in 3D: 8 words; 3D table at 0x20 (n (10, 10, 1): 21 + 100 = 121 -> capped 100)
    at<std::int32_t>(w, 0x210) = 1;
    table_at(w, 0x20, 0x10, 3, 1, 10, 10, 1);
    count_window(w, &nfn, &lfnc, &nwin, &lwin, s3);
    CHECK_EQ(nwin, 2);
    CHECK_EQ(lwin, 8);
    CHECK_EQ(nfn, 4);
    CHECK_EQ(lfnc, 19 + 100);
    // kind 10 in 2D with 3 points: one window of 3 words
    at<std::int32_t>(w, 0x174) = 10;
    at<std::int32_t>(w, 0x178) = 3;
    count_window(w, &nfn, &lfnc, &nwin, &lwin, s2);
    CHECK_EQ(nwin, 3);
    CHECK_EQ(lwin, 11);
    for (std::size_t off : {0x20u, 0x380u, 0x3f8u, 0x568u})
        def::model::dynary_free_arrays(static_cast<DYNARY*>(static_cast<void*>(static_cast<char*>(w) + off)));
    ::operator delete(w);
    ::operator delete(s3);
    ::operator delete(s2);
}

// ---- the walk from 1401beb30 -----------------------------------------------------------------

TEST_CASE(count_object_tables_3d)
{
    reset_obj_count_ext();
    DB_STEP_DATA* s = raw_step(3);
    at<std::int32_t>(s, 0x78) = 7;  // words of the +0x3590 table
    LLOBJ_DATA* o = raw_obj(s);
    LLOBJ_DATA* skip = raw_obj(s);
    s->objects.head = skip;
    skip->next = o;
    at<std::int32_t>(skip, 0xc) = 7;  // type 7: not counted
    at<std::int32_t>(o, 0xc) = 1;     // deformable
    // Cls_14018b220: case 0 table at 0x70, 1D, n 5: 10 words
    table_at(o, 0x2160 + 0x70, 0x10, 1, 1, 5);
    // Cls_1401860a0: curve at 0xc0, 1D, n 3: 6 words; two 0x218 entries: 2 x 20 words
    table_at(o, 0x1ea0 + 0xc0, 0x10, 1, 1, 3);
    at<std::int32_t>(o, 0x1ea0 + 0x218 + 0x18) = 2;
    // Cls_1401956d0: window list at 0x78 with one kind-5 window (table at 0x380: 8 words)
    void* w = zeroed(0x920);
    at<std::int32_t>(w, 0x174) = 5;
    table_at(w, 0x380, 0x10, 1, 1, 4);
    at<void*>(o, 0x2e68 + 0x78) = w;
    at<std::int32_t>(o, 0x2e68 + 0x78 + 0x14) = 1;
    // the +0x3590 table
    at<std::int32_t>(o, 0x3590) = 1;
    std::int32_t nfn = 100, lfnc = 1000, nwin = 10, lwin = 20, err = 0;
    count_object_tables(s, &nfn, &lfnc, &nwin, &lwin, &err);
    CHECK_EQ(nfn, 100 + 1 + 1 + 2 + 1 + 1);
    CHECK_EQ(lfnc, 1000 + 10 + 6 + 40 + 8 + 7);
    CHECK_EQ(nwin, 11);
    CHECK_EQ(lwin, 20);
    CHECK_EQ(err, 0);

    // a rigid object (+0xc == 0) counts its windows only when not "only deformation" and the
    // mesh options 0x880 > 3, 0x884 > 0; sim control +0x108 == 1 alone means only deformation
    at<std::int32_t>(o, 0xc) = 0;
    at<std::int32_t>(o, 0x880) = 4;
    at<std::int32_t>(o, 0x884) = 1;
    at<std::int32_t>(&s->sim_ctrl, 0x108) = 1;
    nfn = lfnc = nwin = lwin = 0;
    count_object_tables(s, &nfn, &lfnc, &nwin, &lwin, &err);
    CHECK_EQ(nfn, 4);
    CHECK_EQ(lfnc, 56);
    CHECK_EQ(nwin, 0);
    at<std::int32_t>(&s->sim_ctrl, 0x10c) = 1;  // another mode on
    nfn = lfnc = nwin = lwin = 0;
    count_object_tables(s, &nfn, &lfnc, &nwin, &lwin, &err);
    CHECK_EQ(nfn, 6);
    CHECK_EQ(lfnc, 56 + 8 + 7);
    CHECK_EQ(nwin, 1);

    def::model::dynary_free_arrays(static_cast<DYNARY*>(static_cast<void*>(static_cast<char*>(w) + 0x380)));
    def::model::dynary_free_arrays(static_cast<DYNARY*>(static_cast<void*>(reinterpret_cast<char*>(o) + 0x2160 + 0x70)));
    def::model::dynary_free_arrays(static_cast<DYNARY*>(static_cast<void*>(reinterpret_cast<char*>(o) + 0x1ea0 + 0xc0)));
    ::operator delete(w);
    ::operator delete(o);
    ::operator delete(skip);
    ::operator delete(s);
}

namespace {
std::vector<std::pair<LLOBJ_DATA*, std::int32_t>> g_dropped;
bool hook_drop(LLOBJ_DATA* o, std::int32_t f)
{
    g_dropped.emplace_back(o, f);
    return true;
}
std::int32_t hook_geo2d_count(LLOBJ_DATA*) { return 2; }
} // namespace

TEST_CASE(count_object_2d_multiplies_and_drops_body_forces)
{
    reset_obj_count_ext();
    obj_count_ext().obj_drop_bad_geo2d = hook_drop;
    obj_count_ext().obj_geo2d_count = hook_geo2d_count;
    g_dropped.clear();
    DB_STEP_DATA* s = raw_step(2);
    LLOBJ_DATA* o = raw_obj(s);
    s->objects.head = o;
    // the selector 0x21c8 (= Cls_14018b220+0x68) is 0 and the byte at 0x2a18 set: cleared
    // (only selectors 7..10 keep it)
    at<std::uint8_t>(o, 0x2a18) = 1;
    // case 0 table, n 5 (10 words), counted once per 2D geometry (2)
    table_at(o, 0x2160 + 0x70, 0x10, 1, 1, 5);
    // 0x960 == 0 and 0x96c == 1: the 2D-only table at 0x968, type 0x7c ncomp 4: 4 words x 2
    at<std::int32_t>(o, 0x2160 + 0x96c) = 1;
    at<std::int32_t>(o, 0x2160 + 0x968) = 0x7c;
    at<std::int32_t>(o, 0x2160 + 0x968 + 0x40) = 4;
    // body forces: records of kind 5 and 3; 1 of kind 5/6 != 2 records -> list emptied, 1 x 12
    auto* bf = reinterpret_cast<def::model::linked::Cls_140258580List*>(reinterpret_cast<char*>(o) + 0x1ea0 + 0x200);
    auto* r1 = static_cast<def::model::linked::Cls_140258580*>(zeroed(sizeof(def::model::linked::Cls_140258580)));
    auto* r2 = static_cast<def::model::linked::Cls_140258580*>(zeroed(sizeof(def::model::linked::Cls_140258580)));
    at<std::int32_t>(r1, 4) = 5;
    at<std::int32_t>(r2, 4) = 3;
    r1->next = r2;
    bf->head = r1;
    bf->tail = r2;
    bf->count = 2;
    std::int32_t nfn = 0, lfnc = 0, nwin = 0, lwin = 0, err = 0;
    count_object_tables(s, &nfn, &lfnc, &nwin, &lwin, &err);
    CHECK_EQ(g_dropped.size(), std::size_t{1});
    CHECK(g_dropped[0].first == o);
    CHECK_EQ(g_dropped[0].second, 1);
    CHECK_EQ(at<std::uint8_t>(o, 0x2a18), 0);
    CHECK_EQ(nfn, 2 + 2 + 1);
    CHECK_EQ(lfnc, 20 + 8 + 12);
    CHECK_EQ(bf->count, 0);
    CHECK(bf->head == nullptr);
    def::model::dynary_free_arrays(static_cast<DYNARY*>(static_cast<void*>(reinterpret_cast<char*>(o) + 0x2160 + 0x70)));
    ::operator delete(o);
    ::operator delete(s);
    reset_obj_count_ext();
}

TEST_CASE(dynary_list_counts_take_the_largest_id)
{
    // two tables: 1D n 2 (4 words), 3D n (1, 1, 2) (4 + 2 = 6 words); ids (+0x54) 0 and 5:
    // functions = entry value + max(count 2, 5) = 5
    std::vector<void*> nodes = {zeroed(sizeof(DYNARY)), zeroed(sizeof(DYNARY))};
    table_at(nodes[0], 0, 0x10, 1, 1, 2);
    table_at(nodes[1], 0, 0x10, 3, 1, 1, 1, 2);
    at<std::int32_t>(nodes[1], 0x54) = 5;
    at<void*>(nodes[0], 0x60) = nodes[1];
    void* list = zeroed(0x18);
    at<void*>(list, 0) = nodes[0];
    at<std::int32_t>(list, 0x10) = 2;
    std::int32_t nfn = 3, lfnc = 0;
    count_dynary_list(list, &nfn, &lfnc);
    CHECK_EQ(nfn, 3 + 5);
    CHECK_EQ(lfnc, 4 + 6);
    for (void* p : nodes) {
        def::model::dynary_free_arrays(static_cast<DYNARY*>(p));
        ::operator delete(p);
    }
    ::operator delete(list);
}

int main() { return testkit::run_all(); }
