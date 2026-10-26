// ctest-labels: p-dbread dbread
// Tier 1 checks of the W1 part of DBREAD (packet p-dbread): function tables from IIFNC/LLFNC,
// the movement-table conversion, 3x3 helpers, the text byte swap, the object header, a 2D
// movement record and a 2D window. Inputs are small synthetic buffers; every expected value is
// worked out by hand from the code (comments).
#include <defdbio/dbread/w1.hpp>

#include <cstdlib>
#include <cstring>
#include <new>

#include <defmodel/dynary_init.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/dynary.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/window/llwindef_data.hpp>
#include <testkit/check.hpp>

using namespace def::dbio::dbread;
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

DB_STEP_DATA* raw_step(std::int32_t dim)
{
    auto* s = static_cast<DB_STEP_DATA*>(zeroed(sizeof(DB_STEP_DATA)));
    s->dimension = dim;
    return s;
}

} // namespace

TEST_CASE(func_record_and_1d_table)
{
    // function 1: LLFNC 1..6 (id 77), function 2: LLFNC 7.. (empty: next start is 7)
    std::int32_t iifnc[6] = {1, 77, 7, 78, 7, 0};
    double llfnc[6] = {0.0, 10.0, 1.0, 20.0, 2.0, 30.0};
    CHECK(func_record(iifnc, llfnc, 1) == llfnc);
    CHECK(func_record(iifnc, llfnc, 2) == nullptr); // 7 - 7 < 1
    CHECK(func_record(iifnc, llfnc, 0) == nullptr);

    DYNARY f;
    f.type = 0x15;
    f.ndim = 1;
    f.ncomp = 1;
    char flag = 0;
    read_func_from_db_array(&f, iifnc, llfnc, 1, &flag);
    // ncomp 1: n[0] = 6 / 2 = 3 rows (x, y)
    CHECK_EQ(f.n[0], 3);
    CHECK_EQ(f.u54, 77);
    CHECK_EQ(f.axis[0][2], 2.0);
    CHECK_EQ(f.values[0], 10.0);
    CHECK_EQ(f.values[2], 30.0);
}

TEST_CASE(func_type_7c_constant_vector)
{
    std::int32_t iifnc[4] = {1, 5, 4, 0};
    double llfnc[3] = {1.5, 2.5, 3.5};
    DYNARY f;
    def::model::dynary_init(&f, 0x7c);
    f.ncomp = 3;
    char flag = 0;
    read_func_from_db_array(&f, iifnc, llfnc, 1, &flag);
    if (f.ndim == 1) {
        CHECK_EQ(f.values[0], 1.5);
        CHECK_EQ(f.values[2], 3.5);
        CHECK_EQ(f.axis[0][0], 0.0);
    }
    CHECK_EQ(f.u54, 5);
}

TEST_CASE(func_2d_with_shape_prefix)
{
    // 2x3 table, ncomp 1: prefix (2, 3), axes (a0 a1)(b0 b1 b2), 6 values
    double llfnc[13] = {2, 3, 0.0, 1.0, 5.0, 6.0, 7.0, 1, 2, 3, 4, 5, 6};
    std::int32_t iifnc[4] = {1, 9, 14, 0};
    DYNARY f;
    f.type = 0x15;
    f.ndim = 2;
    f.ncomp = 1;
    char flag = 1;
    read_func_from_db_array(&f, iifnc, llfnc, 1, &flag);
    CHECK_EQ(f.n[0], 2);
    CHECK_EQ(f.n[1], 3);
    CHECK_EQ(f.axis[0][1], 1.0);
    CHECK_EQ(f.axis[1][2], 7.0);
    CHECK_EQ(f.values[5], 6.0);
}

TEST_CASE(time_axis_to_speed)
{
    // points (0,0) at t=0, (3,4) at t=2, (3,4) at t=4: speeds 5/2 and 0, last 0
    DYNARY f;
    f.type = 0x7a;
    f.ndim = 1;
    f.ncomp = 3;
    f.n[0] = 3;
    f.var[0] = 1;
    def::model::dynary_alloc(&f);
    const double t[3] = {0.0, 2.0, 4.0};
    const double v[9] = {0, 0, 0, 3, 4, 0, 3, 4, 0};
    std::memcpy(f.axis[0], t, sizeof t);
    std::memcpy(f.values, v, sizeof v);
    CHECK(dynary_time_axis_to_speed(&f));
    CHECK_EQ(f.axis[0][0], 2.5);
    CHECK_EQ(f.axis[0][1], 0.0);
    CHECK_EQ(f.axis[0][2], 0.0);
    CHECK_EQ(f.var[0], 0x1a);
    f.var[0] = 2;
    CHECK(!dynary_time_axis_to_speed(&f));
}

TEST_CASE(matrices)
{
    double a[9] = {2, 0, 0, 0, 4, 0, 0, 0, 8};
    double inv[9];
    CHECK_EQ(mat3_det(a), 64.0);
    CHECK_EQ(mat3_inverse_or_2d(a, inv), 1);
    CHECK_EQ(inv[0], 0.5);
    CHECK_EQ(inv[4], 0.25);
    CHECK_EQ(inv[8], 0.125);
    // singular: the leading 2x2 minor [[1 2][3 4]] (det -2) is inverted
    double s[9] = {1, 2, 0, 3, 4, 0, 0, 0, 0};
    CHECK_EQ(mat3_inverse_or_2d(s, inv), 2);
    CHECK_EQ(inv[0], -2.0);
    CHECK_EQ(inv[1], 1.0);
    CHECK_EQ(inv[3], 1.5);
    CHECK_EQ(inv[4], -0.5);
    CHECK_EQ(inv[8], 0.0);
    double z[9] = {};
    CHECK_EQ(mat3_inverse_or_2d(z, inv), 0);
    double m[9] = {0, -1, 0, 1, 0, 0, 0, 0, 1};
    double vv[3] = {1, 2, 3};
    double r[3];
    mat3_mul_vec(vv, r, m);
    CHECK_EQ(r[0], -2.0);
    CHECK_EQ(r[1], 1.0);
    CHECK_EQ(r[2], 3.0);
}

TEST_CASE(text_byteswap_platforms)
{
    // platform 0: 2 * 100 + 0 = 200 swaps: words reversed (DCBA HGFE), then halves swapped
    char s[9] = "ABCDEFGH";
    text_byteswap(s, 0, 1, 0);
    CHECK(std::memcmp(s, "HGFEDCBA", 8) == 0);
    char t[9] = "ABCDEFGH";
    text_byteswap(t, 0, 1, 1); // keep_order != 0: words only
    CHECK(std::memcmp(t, "DCBAHGFE", 8) == 0);
    char u[9] = "ABCDEFGH";
    text_byteswap(u, 5, 1, 0); // 205: no swap
    CHECK(std::memcmp(u, "ABCDEFGH", 8) == 0);
}

TEST_CASE(object_header_3d)
{
    DB_STEP_DATA* m = raw_step(3);
    m->u748 = 5; // no text swap
    auto* obj = static_cast<LLOBJ_DATA*>(zeroed(sizeof(LLOBJ_DATA)));
    obj->owner = m;
    std::int32_t ii[16] = {1, 2, 0, 0, 11, 20, 5, 8, 3, 4, 0, 0, 8, 0, 0, 0};
    double ll[0x40] = {};
    std::memcpy(reinterpret_cast<char*>(ll) + 0x20, "BILLET  ", 8);
    std::memset(reinterpret_cast<char*>(ll) + 0x28, ' ', 0x38);
    read_object_header(obj, ii, ll, nullptr, nullptr);
    CHECK_EQ(at<std::int32_t>(obj, 0xc), 3);      // ii[1] == 2 -> kind 3
    CHECK_EQ(at<std::int32_t>(obj, 0x87c), 1);
    CHECK_EQ(at<std::int32_t>(obj, 0x880), 10);   // 20 - 11 + 1
    CHECK_EQ(at<std::int32_t>(obj, 0x884), 4);    // 8 - 5 + 1
    CHECK_EQ(at<std::int32_t>(obj, 0x888), 2);    // 4 - 3 + 1
    CHECK_EQ(at<std::int32_t>(obj, 0x890), 4);    // ii[12] == 8: bricks
    CHECK_EQ(at<std::int32_t>(obj, 0x894), 8);
    CHECK(std::strcmp(obj->name, "BILLET") == 0);
    ii[0] = 0;
    read_object_header(obj, ii, ll, nullptr, nullptr);
    CHECK_EQ(at<std::int32_t>(obj, 0xc), 7);
}

TEST_CASE(movement_2d_type1_offset)
{
    DB_STEP_DATA* m = raw_step(2);
    void* mv = zeroed(0xa88);
    at<DB_STEP_DATA*>(mv, 0xa80) = m;
    double rec[0x50] = {};
    std::memset(reinterpret_cast<char*>(rec) + 0x1c0, ' ', 0x50);
    std::memcpy(reinterpret_cast<char*>(rec) + 0x1c0, "DIE", 3);
    at<double>(rec, 0x38) = 1.0;   // type 1
    at<double>(rec, 0x60) = 2.0;   // -> +0x160 = (int)rec[0x70], +0x168 = rec[0x48]
    at<double>(rec, 0x70) = 3.0;
    at<double>(rec, 0x48) = 0.25;
    at<double>(rec, 0x50) = -1.0;
    at<double>(rec, 0x58) = 0.5;
    read_movement(mv, rec, nullptr, nullptr, 5);
    CHECK(std::strcmp(static_cast<char*>(mv), "DIE") == 0);
    CHECK_EQ(at<std::int32_t>(mv, 0x68), 1);
    CHECK_EQ(at<std::int32_t>(mv, 0x160), 3);
    CHECK_EQ(at<double>(mv, 0x168), 0.25);
    CHECK_EQ(at<double>(mv, 0x50), -1.0);
    CHECK_EQ(at<double>(mv, 0x58), 0.5);
    CHECK_EQ(at<std::int32_t>(mv, 0x960), -1); // rec[0x130] == 0
    CHECK_EQ(at<std::uint8_t>(mv, 0x8b8), 0);
}

TEST_CASE(window_2d_type2)
{
    DB_STEP_DATA* m = raw_step(2);
    auto* w = new def::model::LLWINDEF_DATA(m);
    double row[16] = {};
    row[0] = 2.0;  // window type 2
    row[1] = 2.0;  // points 2..3 of LLWRZ
    row[2] = 3.0;
    row[3] = 1.0;  // orientation 0
    row[4] = 7.0;
    row[5] = 8.0;
    row[6] = 0.125;
    double llwrz[8] = {0, 0, 1, 2, 3, 4, 9, 9};
    read_window(w, row, llwrz, nullptr, nullptr);
    CHECK_EQ(w->u174, 2);
    CHECK_EQ(w->npts, 2);           // |3 - 2| + 1
    CHECK_EQ(w->pts[0], 1.0);       // LLWRZ from (2 - 1) * 2
    CHECK_EQ(w->pts[3], 4.0);
    CHECK_EQ(w->u350[0], 7.0);
    CHECK_EQ(w->u350[1], 8.0);
    CHECK_EQ(w->orient, 0);
    CHECK_EQ(at<double>(w, 0x370), 0.125);
}

TEST_CASE(body_force_type4)
{
    void* r = zeroed(0xe8);
    double v[12] = {0, 0, 5, 0, 2, 0, 4, 9, 1.5, 7, 8, 9};
    body_force_from_values(r, v);
    CHECK_EQ(at<std::int32_t>(r, 4), 4);
    CHECK_EQ(at<double>(r, 0x68), 0.0); // (0, 0, 5) / 5
    CHECK_EQ(at<double>(r, 0x70), 0.0);
    CHECK_EQ(at<double>(r, 0x78), 1.0);
    CHECK_EQ(at<double>(r, 0x80), 0.0); // (0, 2, 0) / 2
    CHECK_EQ(at<double>(r, 0x88), 1.0);
    CHECK_EQ(at<double>(r, 0x98), 7.0); // v[9..11]
    CHECK_EQ(at<std::int32_t>(r, 0xb0), 9);
    CHECK_EQ(at<double>(r, 0xb8), 1.5);
}

int main() { return testkit::run_all(); }
