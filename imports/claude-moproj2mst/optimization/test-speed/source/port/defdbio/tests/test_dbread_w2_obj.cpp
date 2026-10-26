// ctest-labels: p-dbread dbread
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
// Tier 1 for the W2 object-array group of packet p-dbread: 14016dfb0 / 140170ed0 (object node and
// element arrays from the A array), 1401d4400 (early exits), 1401cc1a0 (directory sizes) and
// the DB file-set lookups 1401e5d60..1401e5f00. Model, object and DB object are zeroed byte
// buffers with fields set by offset; expected values are worked out by hand from the decompile.
// 3D A slots (docs/re/dbmap/region_globals.tsv): LLCRZ 11 (width 151), LLTMP 20 (160),
// LLFLX 23 (163), LLPPR 17 (157), LLSTS 29 (169), LLEPS 30 (170).
#include <defdbio/dbread/w2.hpp>
#include <testkit/check.hpp>

#include <cstdint>
#include <cstdio>
#include <cstring>
#include <vector>

#include <defmodel/layout/cls_1401dbfe0.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/step/db_step.hpp>

using def::model::DB_STEP_DATA;
using def::model::LLOBJ_DATA;
namespace dr = def::dbio::dbread;

namespace {

struct Buf {
    std::vector<std::uint64_t> w;
    explicit Buf(std::size_t bytes) : w((bytes + 7) / 8, 0) {}
    template <class T> T* as() { return reinterpret_cast<T*>(w.data()); }
};
template <class T> T& fld(void* p, std::ptrdiff_t off) { return *reinterpret_cast<T*>(static_cast<char*>(p) + off); }

struct World {
    Buf mb{sizeof(DB_STEP_DATA)};
    Buf ob{sizeof(LLOBJ_DATA)};
    DB_STEP_DATA* m;
    LLOBJ_DATA* o;
    std::int32_t D[0x4f];
    std::int32_t a_tab[280] = {};
    std::int32_t mar_tab[140] = {};
    std::vector<double> a = std::vector<double>(1000);
    World()
    {
        m = mb.as<DB_STEP_DATA>();
        o = ob.as<LLOBJ_DATA>();
        fld<std::int32_t>(m, 0x1c) = 3;
        fld<DB_STEP_DATA*>(o, 0x3a98) = m;
        fld<std::int32_t>(o, 0x87c) = 1;
        for (auto& d : D) d = 99; // no width matches unless set
        a_tab[157] = 7;            // LLPPR / LLFLX accept a stored width 0: make it non-zero
        a_tab[163] = 7;
        for (int i = 0; i < 1000; ++i) a[i] = i;
    }
};

} // namespace

TEST_CASE(node_arrays_all_regions)
{
    World w;
    std::vector<double> crz(6, -1.0), tmp(3, -1.0);
    fld<double*>(w.o, 0x8a8) = crz.data();
    fld<double*>(w.o, 0x8e0) = tmp.data();
    fld<std::int32_t>(w.o, 0x3984) = 2; // first node (1-based)
    fld<std::int32_t>(w.o, 0x880) = 3;  // nodes
    w.D[0] = 2;
    w.a_tab[11] = 101;
    w.a_tab[151] = 2;
    w.D[8] = 1;
    w.a_tab[20] = 201;
    w.a_tab[160] = 1;
    const std::int32_t all = -1;
    dr::obj_read_node_arrays(w.o, nullptr, w.a.data(), w.mar_tab, w.a_tab, w.D, 50, &all, 0);
    // LLCRZ rows: 101 - 1 + (2 - 1) * 2 = 102, 6 words
    for (int i = 0; i < 6; ++i) CHECK_BITS(crz[i], 102.0 + i);
    // LLTMP scalars: 201 - 1 - 1 + 2 = 201, 3 words
    for (int i = 0; i < 3; ++i) CHECK_BITS(tmp[i], 201.0 + i);
}

TEST_CASE(node_arrays_single_region_starts_at_zero)
{
    World w;
    std::vector<double> tmp(3, -1.0);
    fld<double*>(w.o, 0x8a8) = tmp.data(); // non-null: no reallocation (140162ff0)
    fld<double*>(w.o, 0x8e0) = tmp.data();
    fld<std::int32_t>(w.o, 0x3984) = 4;
    fld<std::int32_t>(w.o, 0x880) = 3;
    w.D[8] = 1;
    w.a_tab[160] = 1;
    std::int32_t dim = 3;
    def::model::select_index_tables(&dim);
    const std::int32_t region = def::model::g_a_index[(0x1406e8318 - 0x1406e82d0) / 4]; // LLTMP
    CHECK_EQ(region, 20);
    dr::obj_read_node_arrays(w.o, nullptr, w.a.data(), w.mar_tab, w.a_tab, w.D, 50, &region, 0);
    // offset 0 - 1 + 4 = 3 (the buffer holds only that region)
    for (int i = 0; i < 3; ++i) CHECK_BITS(tmp[i], 3.0 + i);
}

TEST_CASE(element_stress_from_eight_point_rows)
{
    World w;
    std::vector<double> sts(4, -1.0), eps(6, -1.0);
    fld<double*>(w.o, 0x9e8) = sts.data(); // non-null: no reallocation
    fld<double*>(w.o, 0xa00) = sts.data();
    fld<double*>(w.o, 0xa08) = eps.data();
    fld<std::int32_t>(w.o, 0x3988) = 1; // first element
    fld<std::int32_t>(w.o, 0x884) = 2;  // elements
    w.D[0x26] = 2;                      // 2 stress components per element (one point)
    w.a_tab[29] = 301;
    w.a_tab[169] = 16;                  // stored: 8 points x 2
    w.D[0x27] = 3;
    w.a_tab[30] = 401;
    w.a_tab[170] = 3;
    const std::int32_t all = -1;
    dr::obj_read_element_arrays(w.o, nullptr, w.a.data(), w.mar_tab, w.a_tab, w.D, nullptr, &all);
    // k = 1 (no element-set owner), 16 != 1 * 2 and 16 / 2 == 8: first 2 words of each 16-word row
    CHECK_BITS(sts[0], 300.0);
    CHECK_BITS(sts[1], 301.0);
    CHECK_BITS(sts[2], 316.0);
    CHECK_BITS(sts[3], 317.0);
    // LLEPS: 401 - 1 + 0 * 3 = 400, 6 words
    for (int i = 0; i < 6; ++i) CHECK_BITS(eps[i], 400.0 + i);
}

TEST_CASE(single_a_array_early_exits)
{
    World w;
    std::int32_t r = -1;
    const char elem = 0;
    CHECK_EQ(dr::read_single_a_array(w.m, &r, &elem), 0u);
    r = 280; // == DAT_1406e84d0
    CHECK_EQ(dr::read_single_a_array(w.m, &r, &elem), 0u);
    r = 20;
    w.m->u26c4[20] = 1; // already read
    CHECK_EQ(dr::read_single_a_array(w.m, &r, &elem), 0u);
    w.m->u26c4[20] = 0; // no DB object, no objects: timers only, returns 1
    CHECK_EQ(dr::read_single_a_array(w.m, &r, &elem), 1u);
}

TEST_CASE(step_dir_sizes_and_version)
{
    World w;
    Buf dbb(sizeof(def::model::Cls_1401dbfe0));
    auto* db = dbb.as<def::model::Cls_1401dbfe0>();
    std::int32_t recs[2][10] = {};
    recs[1][2] = 5;     // hsize
    recs[1][3] = 600;   // MAR size
    recs[1][4] = 7000;  // A size
    recs[1][8] = 11000; // version
    db->uad8 = recs;
    w.m->u740 = 1;
    std::int32_t h = 0, ms = 0, as = 0, v = 0;
    CHECK_EQ(dr::step_dir_sizes(w.m, nullptr, 0, &h, &ms, &as, &v), 0u);
    CHECK_EQ(dr::step_dir_sizes(w.m, db, 0, &h, &ms, &as, &v), 1u);
    CHECK_EQ(h, 5);
    CHECK_EQ(ms, 600);
    CHECK_EQ(as, 7000);
    CHECK_EQ(v, 11000);
    CHECK_EQ(w.m->step_version, 11000);
}

TEST_CASE(db_file_set_lookup)
{
    Buf dbb(sizeof(def::model::Cls_1401dbfe0));
    auto* db = dbb.as<def::model::Cls_1401dbfe0>();
    char n0[] = "A.DB", n1[] = "B.DB";
    std::strcpy(db->u650, "ONE.DB");
    char out[64];
    std::int32_t first = -5;
    db->u008 = 1; // single file
    dr::db_file_for_step(db, out, &first, 30);
    CHECK(std::strcmp(out, "ONE.DB") == 0);
    CHECK_EQ(first, 0);
    db->u008 = 2;
    db->u00c[0] = {1, 10};
    db->u00c[1] = {11, 40};
    db->u330[0] = n0;
    db->u330[1] = n1;
    dr::db_file_for_step(db, out, &first, 30);
    CHECK(std::strcmp(out, "B.DB") == 0);
    CHECK_EQ(first, 11);
    CHECK_EQ(dr::db_file_index(db, 99), 0); // not found: entry 0
    CHECK_EQ(dr::db_file_first(db, 2), 0);  // out of range
    dr::db_file_name(db, out, -1);
    CHECK(std::strcmp(out, "ONE.DB") == 0);
}

int main()
{
    std::setvbuf(stdout, nullptr, _IONBF, 0);
    return testkit::run_all();
}
