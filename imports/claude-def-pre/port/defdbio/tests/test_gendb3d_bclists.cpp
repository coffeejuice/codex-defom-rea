// ctest-labels: p4-05 gendb3d
// Tier 1 for packet p4-05, bclists.cpp: write_obj_1ea0 (140187a60), write_obj_2d10 (14018a760),
// write_obj_2e68 (140196840), the LLWINDEF_DATA record writer 140260ea0 and its helpers.
//
// Sub-objects and windows are zeroed buffers set by offset; DB_STEP_DATA is a zeroed buffer of
// its size (only the dimension word +0x1c, the function counters +0x88 / +0x8c / +0x10c and the
// window cursors +0x134 / +0x138 are used). The hook answers the calls into other packets
// (counts, list nodes, the DYNARY lengths); alloc_function, write_dynary_to_db_array and the
// OBJ_GEO3D counts run for real. Expected values are worked out by hand from the decompile.
#include <testkit/check.hpp>

#include <cstdint>
#include <cstring>
#include <functional>
#include <limits>
#include <new>
#include <vector>

#include <defdbio/gendb3d/obj3d.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/dynary.hpp>
#include <defmodel/layout/obj_geo3d.hpp>
#include <defmodel/layout/sf_linked_data.hpp>
#include <defmodel/sdctl/cow.hpp>

#include "../src/gendb3d/access.hpp"
#include "../src/gendb3d/bclists.hpp"
#include "../src/gendb3d/ext.hpp"

using namespace def::dbio::g3d;
using def::model::DB_STEP_DATA;
using def::model::DYNARY;

namespace {

struct Call {
    std::uint64_t addr;
    std::vector<std::uint64_t> args;
};
std::vector<Call> g_calls;
// Per-address answer: return true and set *r when handled.
std::function<bool(std::uint64_t, const std::uint64_t*, std::uint64_t*)> g_answer;

bool hook(std::uint64_t addr, const std::uint64_t* args, int nargs, std::uint64_t* r)
{
    g_calls.push_back({addr, std::vector<std::uint64_t>(args, args + nargs)});
    if (g_answer) return g_answer(addr, args, r);
    return false;
}

int count_calls(std::uint64_t addr)
{
    int n = 0;
    for (const Call& c : g_calls) n += c.addr == addr ? 1 : 0;
    return n;
}

struct Buf {
    explicit Buf(std::size_t n) : v(n / 8 + 1, 0) {}
    void* p() { return v.data(); }
    std::vector<std::uint64_t> v;
};

// Zeroed DB_STEP_DATA with room for 1000 functions / 100000 LLFNC doubles.
struct Model {
    Buf b{sizeof(DB_STEP_DATA)};
    DB_STEP_DATA* m() { return static_cast<DB_STEP_DATA*>(b.p()); }
    explicit Model(std::int32_t dim)
    {
        fld<std::int32_t>(b.p(), 0x1c) = dim;
        fld<std::int32_t>(b.p(), 0x88) = 1000;
        fld<std::int32_t>(b.p(), 0x8c) = 100000;
    }
};

// 1-D table into a DYNARY slot: n points, ncomp values each.
void set_table(void* base, std::ptrdiff_t off, std::int32_t n, std::int32_t ncomp, double* axis,
               double* values)
{
    auto* f = reinterpret_cast<DYNARY*>(at(base, off));
    f->ndim = 1;
    f->n[0] = n;
    f->ncomp = ncomp;
    f->axis[0] = axis;
    f->values = values;
}

void reset_hook()
{
    g_calls.clear();
    g_answer = nullptr;
    ext::g_hook = &hook;
}

struct TestNode : def::model::SfLinkedData {
    void from_values(const double*) override {}
    void to_values(double* rec) const override
    {
        rec[0] = 42.0;
        rec[19] = 43.0;
    }
};

const double kNaN = std::numeric_limits<double>::quiet_NaN();

} // namespace

// ---- 140258c40 and the memcpy accessors --------------------------------------------------------

TEST_CASE(record_e8_types)
{
    Buf r(0x100);
    for (int k = 0; k < 0x100 / 8; ++k) fld<double>(r.p(), k * 8) = 100.0 + k; // double at off = 100 + off/8
    fld<std::int32_t>(r.p(), 0x38) = 7;
    fld<std::int32_t>(r.p(), 0x60) = 9;
    fld<std::int32_t>(r.p(), 0xb0) = 11;
    fld<std::int32_t>(r.p(), 0xc8) = 12;
    fld<std::int32_t>(r.p(), 0xcc) = 13;

    double out[12];
    // type 1: +8..+0x1f -> out[0..2], +0x20.. -> out[3..5], out[7] = int +0x38, +0x40..0x58 -> out[8..11]
    fld<std::int32_t>(r.p(), 4) = 1;
    for (double& d : out) d = -1.0;
    record_e8_to_values(r.p(), out);
    CHECK_BITS(out[0], 101.0);
    CHECK_BITS(out[2], 103.0);
    CHECK_BITS(out[3], 104.0);
    CHECK_BITS(out[5], 106.0);
    CHECK_BITS(out[6], 1.0);
    CHECK_BITS(out[7], 7.0);
    CHECK_BITS(out[8], 108.0);
    CHECK_BITS(out[11], 111.0);

    // type 4: +0x68 -> out[0..2], +0x80 -> out[3..5], +0x98 -> out[9..11], out[7] = int +0xb0, out[8] = +0xb8
    fld<std::int32_t>(r.p(), 4) = 4;
    for (double& d : out) d = -1.0;
    record_e8_to_values(r.p(), out);
    CHECK_BITS(out[0], 113.0);
    CHECK_BITS(out[3], 116.0);
    CHECK_BITS(out[6], 4.0);
    CHECK_BITS(out[7], 11.0);
    CHECK_BITS(out[8], 123.0);
    CHECK_BITS(out[9], 119.0);
    CHECK_BITS(out[11], 121.0);

    // type 5: only +0x20 -> out[3..5] and out[8] = +0xb8
    fld<std::int32_t>(r.p(), 4) = 5;
    for (double& d : out) d = -1.0;
    record_e8_to_values(r.p(), out);
    CHECK_BITS(out[0], -1.0);
    CHECK_BITS(out[3], 104.0);
    CHECK_BITS(out[6], 5.0);
    CHECK_BITS(out[7], -1.0);
    CHECK_BITS(out[8], 123.0);

    // type 7: +0x20 -> out[0..2], +8 -> out[3..5], out[7] = +0x40, out[8]/[9] = ints +0xc8/+0xcc
    fld<std::int32_t>(r.p(), 4) = 7;
    for (double& d : out) d = -1.0;
    record_e8_to_values(r.p(), out);
    CHECK_BITS(out[0], 104.0);
    CHECK_BITS(out[3], 101.0);
    CHECK_BITS(out[7], 108.0);
    CHECK_BITS(out[8], 12.0);
    CHECK_BITS(out[9], 13.0);
    CHECK_BITS(out[10], 126.0);
    CHECK_BITS(out[11], 127.0);

    // type 6: out[6] only
    fld<std::int32_t>(r.p(), 4) = 6;
    for (double& d : out) d = -1.0;
    record_e8_to_values(r.p(), out);
    CHECK_BITS(out[6], 6.0);
    CHECK_BITS(out[0], -1.0);
    CHECK_BITS(out[7], -1.0);

    // memcpy accessors do nothing for a null destination
    record_e8_get_08(r.p(), nullptr);
    record_e8_get_98(r.p(), nullptr);
}

// ---- 140260ea0 ------------------------------------------------------------------------------

TEST_CASE(windef_type2_shape1_3d)
{
    reset_hook();
    g_answer = [](std::uint64_t a, const std::uint64_t*, std::uint64_t* r) {
        if (a == 0x14025c830) { *r = 0; return true; } // not 2D
        return false;
    };
    Model md(3);
    Buf w(0x900);
    double pts[24];
    for (int k = 0; k < 24; ++k) pts[k] = 0.5 * k;
    fld<std::int32_t>(w.p(), 0x174) = 2;
    fld<std::int32_t>(w.p(), 0x210) = 1;
    fld<std::int32_t>(w.p(), 0x178) = 8;
    fld<double*>(w.p(), 0x180) = pts;
    fld<double>(w.p(), 0x350) = 31.0;
    fld<double>(w.p(), 0x358) = 32.0;
    fld<double>(w.p(), 0x360) = 33.0;
    fld<std::int32_t>(w.p(), 0x368) = 2;
    fld<std::int32_t>(w.p(), 0x36c) = -5;
    fld<double>(w.p(), 0x370) = 2.5;
    double rec[20] = {};
    std::vector<double> wrz(64, -1.0);
    std::int32_t row = 4, pos = 3, obj = 1;
    windef_write_to_db_arrays(w.p(), rec, wrz.data(), nullptr, nullptr, &row, &pos, md.m(), &obj, 0);
    CHECK_BITS(rec[0], 2.0);
    CHECK_BITS(rec[14], 1.0);
    CHECK_BITS(rec[1], 3.0);
    CHECK_BITS(rec[2], 10.0); // pos - 1 + 8
    CHECK_BITS(wrz[5], -1.0);
    CHECK_BITS(wrz[6], 0.0);  // (3 - 1) * 3
    CHECK_BITS(wrz[29], 11.5); // last of 24
    CHECK_BITS(wrz[30], -1.0);
    CHECK_EQ(pos, 11);
    CHECK_BITS(rec[3], 31.0);
    CHECK_BITS(rec[5], 33.0);
    CHECK_BITS(rec[15], -5.0); // -abs(-5)
    CHECK_BITS(rec[6], 2.5);   // type 2: +0x370
    CHECK_EQ(row, 5);
    CHECK_EQ(count_calls(0x14025c830), 1);
}

TEST_CASE(windef_type3_no_shape_3d_returns)
{
    reset_hook();
    Model md(3);
    Buf w(0x900);
    fld<std::int32_t>(w.p(), 0x174) = 3;
    fld<std::int32_t>(w.p(), 0x210) = 0;
    double rec[20] = {};
    rec[1] = -7.0;
    std::int32_t row = 4, pos = 3, obj = 1;
    windef_write_to_db_arrays(w.p(), rec, nullptr, nullptr, nullptr, &row, &pos, md.m(), &obj, 0);
    CHECK_BITS(rec[0], 3.0);
    CHECK_BITS(rec[1], -7.0);
    CHECK_EQ(row, 4);
    CHECK_EQ(pos, 3);
    CHECK_EQ(static_cast<int>(g_calls.size()), 0);
}

TEST_CASE(windef_type4_shape3_geo_and_functions)
{
    reset_hook();
    g_answer = [](std::uint64_t a, const std::uint64_t*, std::uint64_t* r) {
        if (a == 0x14025c830) { *r = 0; return true; }
        if (a == 0x140130290) { *r = 4; return true; }
        return false;
    };
    Model md(3);
    Buf w(0x900);
    fld<std::int32_t>(w.p(), 0x174) = 4;
    fld<std::int32_t>(w.p(), 0x210) = 3;
    // OBJ_GEO3D at +0x220: only its coords / elems handles are constructed
    auto* g = reinterpret_cast<def::model::OBJ_GEO3D*>(at(w.p(), 0x220));
    auto* coords = ::new (&g->coords) def::model::CowVector<double>();
    auto* elems = ::new (&g->elems) def::model::CowVector<std::int32_t>();
    coords->resize(6, 0.0);
    for (int k = 0; k < 6; ++k) coords->mutable_at(k) = 1.0 + k;
    elems->resize(4, 0);
    elems->mutable_at(0) = 1;
    elems->mutable_at(1) = 2;
    elems->mutable_at(2) = 2;
    elems->mutable_at(3) = -2;
    fld<double>(w.p(), 0x350) = 31.0;
    fld<std::int32_t>(w.p(), 0x368) = 0;
    // 140263040: +0x5e4 == 0 -> rec[6] = 0, rec[7] = +0x5e8; table +0x658 (+0x65c = its ndim 1,
    // +0x668 = its var[0] 0xc -> rec[8] = 2) as function, its number into rec[9]
    fld<double>(w.p(), 0x5e8) = 55.0;
    double ax[2] = {10.0, 20.0}, va[2] = {100.0, 200.0};
    set_table(w.p(), 0x658, 2, 1, ax, va);
    fld<std::int32_t>(w.p(), 0x668) = 0xc;

    double rec[20] = {};
    std::vector<double> wrz(64, -1.0);
    std::vector<double> llfnc(16, -1.0);
    std::int32_t iifnc[8] = {};
    std::int32_t row = 1, pos = 5, obj = 6;
    windef_write_to_db_arrays(w.p(), rec, wrz.data(), iifnc, llfnc.data(), &row, &pos, md.m(), &obj,
                              0);
    CHECK_BITS(rec[0], 4.0);
    CHECK_BITS(rec[7], 55.0);  // 2 nodes, then overwritten by 140263040 rec[7]
    CHECK_BITS(rec[8], 2.0);   // elem count 1, then overwritten by the code 2
    CHECK_BITS(rec[14], 3.0);
    CHECK_BITS(rec[1], 5.0);
    CHECK_BITS(rec[2], 8.0);   // n = 2 + ceil(4 / 3) = 4
    CHECK_BITS(wrz[11], -1.0);
    CHECK_BITS(wrz[12], 1.0);  // coords at (5 - 1) * 3
    CHECK_BITS(wrz[17], 6.0);
    CHECK_BITS(wrz[18], 1.0);  // elems at ((5 + 2) - 1) * 3
    CHECK_BITS(wrz[21], -2.0);
    CHECK_BITS(wrz[22], -1.0);
    CHECK_EQ(pos, 9);
    CHECK_BITS(rec[3], 31.0);
    CHECK_BITS(rec[15], 1.0);
    CHECK_BITS(rec[6], 0.0);
    CHECK_BITS(rec[10], 0.0);  // kind 1: not written
    CHECK_BITS(rec[9], 1.0);   // first function
    CHECK_EQ(iifnc[0], 1);
    CHECK_EQ(iifnc[1], 6);
    CHECK_EQ(iifnc[2], 5);
    CHECK_BITS(llfnc[0], 10.0);
    CHECK_BITS(llfnc[1], 100.0);
    CHECK_BITS(llfnc[2], 20.0);
    CHECK_BITS(llfnc[3], 200.0);
    CHECK_EQ(row, 2);
    elems->~CowVector();
    coords->~CowVector();
}

TEST_CASE(windef_type1_2d_fric)
{
    reset_hook();
    g_answer = [](std::uint64_t a, const std::uint64_t*, std::uint64_t* r) {
        if (a == 0x14025c830) { *r = 1; return true; } // 2D
        return false;
    };
    Model md(2);
    Buf w(0x900);
    double pts[4] = {1.0, 2.0, 3.0, 4.0};
    fld<std::int32_t>(w.p(), 0x174) = 1;
    fld<std::int32_t>(w.p(), 0x178) = 2;
    fld<double*>(w.p(), 0x180) = pts;
    fld<double>(w.p(), 0x350) = 31.0;
    fld<double>(w.p(), 0x358) = 32.0;
    fld<std::int32_t>(w.p(), 0x368) = 2;
    fld<std::int32_t>(w.p(), 0x36c) = 0;
    // storeWindefFric: kind 4, +0xc = 0, mode (+0x18) 5, no curve (+0x24 = 0); second set: +0xd set
    fld<std::int32_t>(w.p(), 8) = 4;
    fld<std::int32_t>(w.p(), 0x18) = 5;
    fld<std::int32_t>(w.p(), 0x3c) = 21;
    fld<std::int32_t>(w.p(), 0x40) = 22;
    fld<std::int32_t>(w.p(), 0x44) = 23;
    fld<char>(w.p(), 0xd) = 1;
    fld<std::int32_t>(w.p(), 0x14) = 9;
    fld<double>(w.p(), 0xa0) = 0.25;
    double rec[20];
    for (double& d : rec) d = -1.0;
    std::vector<double> wrz(16, -1.0);
    std::int32_t row = 1, pos = 2, obj = 1;
    windef_write_to_db_arrays(w.p(), rec, wrz.data(), nullptr, nullptr, &row, &pos, md.m(), &obj, 7);
    CHECK_BITS(rec[0], 1.0);
    CHECK_BITS(rec[1], 2.0);
    CHECK_BITS(rec[2], 3.0);
    CHECK_BITS(wrz[1], -1.0);
    CHECK_BITS(wrz[2], 1.0); // (2 - 1) << 1
    CHECK_BITS(wrz[5], 4.0);
    CHECK_BITS(wrz[6], -1.0);
    CHECK_EQ(pos, 4);
    CHECK_BITS(rec[4], 31.0);
    CHECK_BITS(rec[5], 32.0);
    CHECK_BITS(rec[3], -7.0); // type 1, +0x36c == 0: -code2d
    CHECK_BITS(rec[6], 4.0);
    CHECK_BITS(rec[9], 5.0);
    CHECK_BITS(rec[7], -1.0);
    CHECK_BITS(rec[10], 21.0);
    CHECK_BITS(rec[12], 23.0);
    CHECK_BITS(rec[8], -1.0);
    CHECK_BITS(rec[14], -9.0);
    CHECK_BITS(rec[13], 0.25);
    CHECK_BITS(rec[16], -1.0);
    CHECK_EQ(row, 2);
}

// ---- 140187a60 ------------------------------------------------------------------------------

TEST_CASE(obj_1ea0_3d)
{
    reset_hook();
    Buf s(0x2c0);
    Buf rec3(0x100);
    fld<std::int32_t>(rec3.p(), 4) = 3;
    fld<double>(rec3.p(), 8) = 1.5;
    fld<std::int32_t>(rec3.p(), 0x60) = 4;
    TestNode node;
    g_answer = [&](std::uint64_t a, const std::uint64_t* args, std::uint64_t* r) {
        if (a == 0x140130290) { *r = 4; return true; }
        if (a == 0x14019a530) { *r = 1; return true; }
        if (a == 0x14019a560) { *r = 1; return true; }
        if (a == 0x140259b60) { *r = ext::u(rec3.p()); return args[1] == 1; }
        if (a == 0x14025a9e0) { *r = ext::u(static_cast<def::model::SfLinkedData*>(&node)); return args[1] == 1; }
        return false;
    };
    Model md(3);
    fld<double>(s.p(), 0x0) = 1.0;
    fld<double>(s.p(), 0x8) = 2.0;
    fld<double>(s.p(), 0x10) = 3.0;
    fld<double>(s.p(), 0x18) = 4.0;
    fld<std::int32_t>(s.p(), 0x28) = 2;
    fld<std::int32_t>(s.p(), 0x48) = 12;
    fld<std::int32_t>(s.p(), 0x4c) = 3;
    fld<std::int32_t>(s.p(), 0x50) = 6;
    fld<double>(s.p(), 0x58) = 5.5;
    fld<std::int32_t>(s.p(), 0x7c) = 14;
    fld<std::int32_t>(s.p(), 0x9c) = 15;
    fld<std::int32_t>(s.p(), 0xa0) = 16;
    fld<double>(s.p(), 0xc8) = 6.5; // +0xc4 == 0
    fld<std::int32_t>(s.p(), 0x138) = 1;
    double ax[2] = {1.0, 2.0}, va[2] = {3.0, 4.0};
    set_table(s.p(), 0x140, 2, 1, ax, va); // +0x144 = 1
    fld<double>(s.p(), 0x1f0) = 5.0;
    fld<std::int32_t>(s.p(), 0x1f8) = -1;
    fld<std::int32_t>(s.p(), 0x2b0) = 17;
    fld<std::int32_t>(s.p(), 0x2b4) = 18;
    fld<std::int32_t>(s.p(), 0x278) = 7;

    std::int32_t iiobj[32];
    for (auto& v : iiobj) v = -1;
    std::vector<double> llobj(0x60, -1.0);
    std::vector<double> llfnc(64, -1.0);
    std::int32_t iifnc[16] = {};
    const std::int32_t obj = 9;
    write_obj_1ea0(s.p(), iiobj, llobj.data(), iifnc, llfnc.data(), md.m(), &obj);
    CHECK_BITS(llobj[0], 4.0);
    CHECK_BITS(llobj[1], 2.0);
    CHECK_BITS(llobj[2], 1.0);
    CHECK_BITS(llobj[3], 3.0);
    CHECK_BITS(llobj[0x34], 15.0);
    CHECK_BITS(llobj[0x40], 11.0); // kind 1 -> 10, then + 1 for the curve
    CHECK_BITS(llobj[0x41], 1.0);  // function 1
    CHECK_BITS(llfnc[0], 1.0);
    CHECK_BITS(llfnc[3], 4.0);
    CHECK_BITS(llobj[0x43], 0.0);
    CHECK_BITS(llobj[0x44], 6.5);
    CHECK_BITS(llobj[0x45], 5.5);
    CHECK_BITS(llobj[0x48], -6.0);
    CHECK_BITS(llobj[0x49], 17.0);
    CHECK_EQ(iiobj[14], 16);
    CHECK_BITS(llobj[0x2a], 2.0);
    CHECK_BITS(llobj[0x2b], 12.0);
    CHECK_BITS(llobj[0x2c], 0.0);
    CHECK_BITS(llobj[0x2e], 0.0);
    CHECK_BITS(llobj[0x2f], 0.0);
    CHECK_BITS(fld<double>(s.p(), 0x1f0), 0.0);
    CHECK_EQ(iiobj[10], 18);
    // record function 2: 12 doubles at LLFNC 5
    CHECK_EQ(iiobj[19], 2);
    CHECK_EQ(iiobj[20], 2);
    CHECK_EQ(iifnc[3], 1);
    CHECK_EQ(iifnc[4], 17);
    CHECK_BITS(llfnc[4], 1.5);
    CHECK_BITS(llfnc[10], 3.0);
    CHECK_BITS(llfnc[11], 4.0);
    // SfLinkedData function 3: 20 doubles at LLFNC 17
    CHECK_EQ(iiobj[21], 3);
    CHECK_EQ(iiobj[22], 3);
    CHECK_EQ(iifnc[6], 37);
    CHECK_BITS(llfnc[16], 42.0);
    CHECK_BITS(llfnc[35], 43.0);
    CHECK_BITS(llobj[0x51], 14.0);
    CHECK_BITS(llobj[0x56], 0.0);
    CHECK_BITS(llobj[0x57], 0.0);
    CHECK_EQ(md.m()->u10c, 3);
    CHECK_EQ(iiobj[12], -1); // 2D-only word untouched
    CHECK_BITS(llobj[0x11], -1.0);
}

TEST_CASE(obj_1ea0_2d)
{
    reset_hook();
    g_answer = [](std::uint64_t a, const std::uint64_t*, std::uint64_t* r) {
        if (a == 0x140130290) { *r = 2; return true; }
        if (a == 0x14019a530) { *r = 2; return true; }
        if (a == 0x140186d00) { *r = 1; return true; } // != 2: no record functions
        if (a == 0x14019a560) { *r = 0; return true; }
        return false;
    };
    Model md(2);
    fld<std::int32_t>(md.b.p(), 0x750 + 0x10c) = 77;
    Buf s(0x2c0);
    fld<std::int32_t>(s.p(), 0x28) = 1;
    fld<double>(s.p(), 0x30) = 0.1;
    fld<double>(s.p(), 0x38) = 0.2;
    fld<std::int32_t>(s.p(), 0x4c) = 2;
    fld<std::int32_t>(s.p(), 0x80) = 3;
    fld<double>(s.p(), 0x88) = 0.3;
    fld<std::int32_t>(s.p(), 0x90) = 4;
    fld<std::int32_t>(s.p(), 0x7c) = 5;
    double ax1[1] = {7.0}, va1[1] = {8.0};
    set_table(s.p(), 0xc0, 1, 1, ax1, va1); // +0xc4 = 1
    fld<std::int32_t>(s.p(), 0xd0) = 0x1c;
    fld<std::int32_t>(s.p(), 0x138) = 5;   // kind outside 0..2: word 0x40 untouched
    fld<double>(s.p(), 0x148) = 9.5;       // +0x144 == 0
    fld<double>(s.p(), 0x1f0) = -0.0;
    fld<std::int32_t>(s.p(), 0x1f8) = -3;  // -> 0; -0.0 compares equal and is kept
    double ax2[1] = {1.0}, va2[8] = {2, 3, 4, 5, 6, 7, 8, 9};
    set_table(s.p(), 0x238, 1, 8, ax2, va2); // +0x278 = 8

    std::int32_t iiobj[32];
    for (auto& v : iiobj) v = -1;
    std::vector<double> llobj(0x60, -1.0);
    std::vector<double> llfnc(64, -1.0);
    std::int32_t iifnc[16] = {};
    const std::int32_t obj = 4;
    write_obj_1ea0(s.p(), iiobj, llobj.data(), iifnc, llfnc.data(), md.m(), &obj);
    CHECK_BITS(llobj[0x40], -1.0);
    CHECK_BITS(llobj[0x41], 9.5);
    CHECK_BITS(llobj[0x43], 2.0);
    CHECK_BITS(llobj[0x44], 1.0);  // function 1 (length 2 at LLFNC 1)
    CHECK_BITS(llfnc[0], 7.0);
    CHECK_BITS(llfnc[1], 8.0);
    CHECK_BITS(llobj[0x48], 2.0);
    CHECK_EQ(iiobj[10], 0);
    CHECK_EQ(iiobj[11], 0);
    CHECK_EQ(iiobj[12], 77);
    CHECK_EQ(iiobj[13], 0);
    CHECK_BITS(llobj[0x11], 3.0);
    CHECK_BITS(llobj[0x12], 4.0);
    CHECK_BITS(llobj[0x13], 0.3);
    CHECK_BITS(llobj[0x37], 1.0);
    CHECK_BITS(llobj[0x32], 0.1);
    CHECK_BITS(llobj[0x33], 0.2);
    CHECK_BITS(llobj[0x53], 5.0);
    CHECK_BITS(llobj[0x54], 0.0);
    CHECK_BITS(llobj[0x55], -0.0);
    CHECK_EQ(iiobj[15], 0);
    CHECK_EQ(iiobj[16], 0);
    CHECK_EQ(iiobj[17], 0);
    CHECK_EQ(iiobj[18], 0);
    CHECK_BITS(llobj[0x56], 8.0);
    CHECK_BITS(llobj[0x57], 2.0); // function 2: length 8 from +0x278, at LLFNC 3
    CHECK_EQ(iifnc[3], 4);
    CHECK_EQ(iifnc[4], 11);
    CHECK_BITS(llfnc[2], 1.0);
    CHECK_BITS(llfnc[3], 2.0);
    CHECK_BITS(llfnc[10], 9.0);
    CHECK_BITS(llobj[0x2a], -1.0); // 3D-only word untouched
    CHECK_EQ(count_calls(0x140259b60), 0);
}

// ---- 14018a760 ------------------------------------------------------------------------------

namespace {
// type 10 window with shape 0xb: rec[0] = 9, rec[7] = 1, rec[14] = 11, n = +0x178 coordinates
void make_window(Buf& w, double v370, double* pts)
{
    fld<std::int32_t>(w.p(), 0x174) = 10;
    fld<std::int32_t>(w.p(), 0x210) = 0xb;
    fld<std::int32_t>(w.p(), 0x178) = 1;
    fld<double*>(w.p(), 0x180) = pts;
    fld<double>(w.p(), 0x370) = v370;
}
} // namespace

TEST_CASE(obj_2d10_3d_windows)
{
    reset_hook();
    Buf w1(0x900), w2(0x900), w3(0x900);
    double p1[3] = {1, 2, 3}, p2[3] = {4, 5, 6};
    make_window(w1, 1.0, p1);
    make_window(w2, 0.0, p2);  // not > 0: skipped
    make_window(w3, kNaN, p2); // NaN: skipped
    void* wins[3] = {w1.p(), w2.p(), w3.p()};
    g_answer = [&](std::uint64_t a, const std::uint64_t* args, std::uint64_t* r) {
        if (a == 0x140189580) { *r = 3; return true; }
        if (a == 0x140269900) { *r = ext::u(wins[args[1] - 1]); return true; }
        if (a == 0x14025c830) { *r = 0; return true; }
        return false;
    };
    Model md(3);
    md.m()->u134 = 3;
    md.m()->u138 = 10;
    Buf s(0x158);
    fld<double>(s.p(), 0xb8) = 1.25;
    fld<double>(s.p(), 0x30) = 2.25;
    fld<std::int32_t>(s.p(), 0x90) = 6;
    fld<std::int32_t>(s.p(), 0x80) = 8;
    fld<double>(s.p(), 0xa8) = 3.25;
    std::vector<double> llobj(0x60, -1.0);
    std::vector<double> llwin(100, -1.0);
    std::vector<double> wrz(64, -1.0);
    const char flag = 1;
    const std::int32_t obj = 2;
    write_obj_2d10(s.p(), nullptr, llobj.data(), nullptr, nullptr, llwin.data(), 20, wrz.data(),
                   &flag, md.m(), &obj);
    CHECK_BITS(llobj[0xa0 / 8], 1.25);
    CHECK_BITS(llobj[0xc0 / 8], 2.25);
    CHECK_BITS(llobj[0xe8 / 8], 6.0);
    CHECK_BITS(llobj[0x1b8 / 8], 8.0);
    CHECK_BITS(llobj[0x198 / 8], 3.25);
    CHECK_BITS(llobj[0xf0 / 8], -1.0); // 2D word untouched
    // window 1 into LLWIN row 3 (offset 20 * 2), coordinates at LLWRZ 10
    CHECK_BITS(llwin[39], -1.0);
    CHECK_BITS(llwin[40], 9.0);
    CHECK_BITS(llwin[41], 10.0);
    CHECK_BITS(llwin[42], 10.0);
    CHECK_BITS(llwin[47], 1.0);
    CHECK_BITS(llwin[54], 11.0);
    CHECK_BITS(llwin[60], -1.0);
    CHECK_EQ(fld<std::int32_t>(w1.p(), 0x378), 1);
    CHECK_BITS(wrz[27], 1.0);
    CHECK_BITS(wrz[29], 3.0);
    CHECK_BITS(wrz[30], -1.0);
    CHECK_BITS(llobj[200 / 8], 3.0);
    CHECK_BITS(llobj[0xd0 / 8], 3.0);
    CHECK_EQ(md.m()->u134, 4);
    CHECK_EQ(md.m()->u138, 11);
    CHECK_EQ(count_calls(0x140189580), 5); // condition, then every loop test
    CHECK_EQ(count_calls(0x140269900), 3);
    CHECK_EQ(count_calls(0x14019a5c0), 1);
}

TEST_CASE(obj_2d10_2d_no_windows)
{
    reset_hook();
    g_answer = [](std::uint64_t a, const std::uint64_t*, std::uint64_t* r) {
        if (a == 0x140189580) { *r = 0; return true; }
        return false;
    };
    Model md(2);
    md.m()->u134 = 3;
    Buf s(0x158);
    fld<std::int32_t>(s.p(), 0x70) = 3;
    fld<double>(s.p(), 0x30) = 2.25;
    fld<double>(s.p(), 0xa8) = 3.25;
    std::vector<double> llobj(0x60, -1.0);
    const char flag = 1;
    const std::int32_t obj = 2;
    write_obj_2d10(s.p(), nullptr, llobj.data(), nullptr, nullptr, nullptr, 20, nullptr, &flag,
                   md.m(), &obj);
    CHECK_BITS(llobj[0xf0 / 8], 3.0);
    CHECK_BITS(llobj[0x128 / 8], 2.25);
    CHECK_BITS(llobj[0x158 / 8], 3.25);
    CHECK_BITS(llobj[200 / 8], 0.0);
    CHECK_BITS(llobj[0xd0 / 8], 0.0);
    CHECK_BITS(llobj[0x160 / 8], -1.0);
    CHECK_BITS(llobj[0xc0 / 8], -1.0);
    CHECK_EQ(md.m()->u134, 3);
    CHECK_EQ(count_calls(0x14019a5c0), 0);
}

// ---- 140196840 ------------------------------------------------------------------------------

TEST_CASE(obj_2e68_lists)
{
    reset_hook();
    Buf w1(0x900), w2(0x900), w3(0x900);
    double p1[3] = {1, 2, 3}, p2[3] = {4, 5, 6};
    make_window(w1, 0.0, p1);
    make_window(w3, 0.0, p2);
    // w2: type 0 (skipped in the +0x138 list)
    Buf s(0x1a0);
    Buf dim(0x40);
    fld<std::int32_t>(dim.p(), 0x1c) = 3;
    fld<void*>(s.p(), 0x198) = dim.p();
    g_answer = [&](std::uint64_t a, const std::uint64_t* args, std::uint64_t* r) {
        if (a == 0x140195b30) { *r = 1; return true; }
        if (a == 0x140195c20) { *r = 2; return true; }
        if (a == 0x140195b60 || a == 0x140195b90 || a == 0x140195bc0 || a == 0x140195bf0 ||
            a == 0x140195c50) { *r = 0; return true; }
        if (a == 0x140269900) {
            if (args[0] == ext::u(at(s.p(), 0x48))) *r = ext::u(w1.p());
            else *r = ext::u(args[1] == 1 ? w2.p() : w3.p()); // +0x138 list
            return true;
        }
        if (a == 0x14025c830) { *r = 0; return true; }
        return false;
    };
    Model md(3);
    md.m()->u134 = 1;
    md.m()->u138 = 1;
    std::vector<double> llobj(0x60, -1.0);
    std::vector<double> llwin(100, -1.0);
    std::vector<double> wrz(64, -1.0);
    const char f1 = 1, f2 = 1, f3 = 1;
    const std::int32_t obj = 2;
    write_obj_2e68(s.p(), nullptr, llobj.data(), nullptr, nullptr, llwin.data(), 20, wrz.data(), &f1,
                   &f2, &f3, md.m(), &obj);
    CHECK_BITS(llobj[0x288 / 8], -1.0); // 2D pair not written
    CHECK_BITS(llobj[0x290 / 8], 1.0);  // +0x48 list: row 1
    CHECK_BITS(llobj[0x298 / 8], 1.0);
    CHECK_BITS(llobj[0x1e0 / 8], 0.0);
    CHECK_BITS(llobj[0x1e8 / 8], 0.0);
    CHECK_BITS(llobj[0x2c0 / 8], 0.0);
    CHECK_BITS(llobj[0x2c8 / 8], 0.0);
    CHECK_BITS(llobj[600 / 8], 0.0);
    CHECK_BITS(llobj[0x270 / 8], 0.0);
    CHECK_BITS(llobj[0x278 / 8], 2.0);  // +0x138 list: w2 skipped, w3 at row 2
    CHECK_BITS(llobj[0x280 / 8], 2.0);
    CHECK_BITS(llwin[0], 9.0);
    CHECK_BITS(llwin[1], 1.0);
    CHECK_BITS(llwin[20], 9.0);
    CHECK_BITS(llwin[21], 2.0);
    CHECK_BITS(llwin[40], -1.0);
    CHECK_BITS(wrz[0], 1.0);
    CHECK_BITS(wrz[3], 4.0);
    CHECK_EQ(md.m()->u134, 3);
    CHECK_EQ(md.m()->u138, 3);
    CHECK_EQ(count_calls(0x14019a5c0), 2);
}

int main() { return testkit::run_all(); }
