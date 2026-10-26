// ctest-labels: p4-05 gendb3d
// Tier 1 for packet p4-05: the LLBND movement writer FUN_1401912a0 (OMV_DATA::WriteToDB) and its
// helpers 140134de0, 14018c850, 14018c940, 14018ca10, 14018cb10, 1401d6030.
//
// The movement sub-object, the owner, DB_STEP_DATA and the record are zeroed byte buffers with
// fields set by offset. The callees other packets own (140130290, 14012eff0, 140135bc0, 14018cde0)
// are answered by the ext hook; 140194d40 runs its default body. Expected values are worked out
// by hand from the decompile (comments next to each check).
#include <defdbio/gendb3d/obj3d.hpp>
#include <testkit/check.hpp>

#include <climits>
#include <cstdint>
#include <cstring>
#include <limits>
#include <vector>

#include <defmodel/dynary_init.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/dynary.hpp>

#include "../src/gendb3d/access.hpp"
#include "../src/gendb3d/ext_movement.hpp"
#include "../src/gendb3d/movement.hpp"

using namespace def::dbio::g3d;
using def::model::DB_STEP_DATA;
using def::model::DYNARY;

namespace {

struct Buf {
    std::vector<std::uint64_t> w;
    explicit Buf(std::size_t bytes) : w((bytes + 7) / 8, 0) {}
    void* p() { return w.data(); }
    template <class T> T& at(std::ptrdiff_t off) { return fld<T>(w.data(), off); }
};

// What the hook saw.
struct Seen {
    std::vector<std::uint64_t> calls;
    std::int32_t len_290 = 4;    // 140130290 answer
    std::int32_t len_cde0 = 16;  // 14018cde0 answer
    double frame[9] = {};        // frame handed to 140135bc0
    std::int32_t to_frame_type = 0;
};
Seen g;

bool hook(std::uint64_t addr, const std::uint64_t* a, int, std::uint64_t* r)
{
    g.calls.push_back(addr);
    switch (addr) {
    case 0x140130290:
        *r = static_cast<std::uint64_t>(g.len_290);
        return true;
    case 0x14018cde0:
        *r = static_cast<std::uint64_t>(g.len_cde0);
        return true;
    case 0x14012eff0: {  // DYNARY copy, as FUN_14012eff0 does it for the fields used here
        auto* dst = ext::as_p<DYNARY>(a[0]);
        auto* src = ext::as_p<const DYNARY>(a[1]);
        dst->type = src->type;
        dst->value = src->value;
        dst->ndim = src->ndim;
        dst->ncomp = src->ncomp;
        for (int i = 0; i < 3; ++i) {
            dst->var[i] = src->var[i];
            dst->n[i] = src->n[i];
        }
        def::model::dynary_alloc(dst);
        for (int i = 0; i < dst->ndim && dst->axis[i] && src->axis[i]; ++i)
            std::memcpy(dst->axis[i], src->axis[i], sizeof(double) * dst->n[i]);
        if (dst->values && src->values)
            std::memcpy(dst->values, src->values,
                        sizeof(double) * def::model::dynary_value_count(dst));
        return true;
    }
    case 0x140135bc0: {
        const double* f = ext::as_p<const double>(a[1]);
        for (int i = 0; i < 9; ++i) g.frame[i] = f[i];
        g.to_frame_type = ext::as_p<DYNARY>(a[0])->type;
        *r = 1;
        return true;
    }
    default:
        return false;  // 140194d40: default body
    }
}

void reset_hook()
{
    g = Seen{};
    ext::g_hook = &hook;
}

// A raw (not constructed) 1-D DYNARY in a byte slot: only read by the writer.
void set_table(void* slot, std::int32_t type, std::int32_t var0, std::int32_t n, std::int32_t ncomp,
               double* axis, double* values)
{
    auto* f = static_cast<DYNARY*>(slot);
    f->type = type;
    f->ndim = 1;
    f->var[0] = var0;
    f->n[0] = n;
    f->ncomp = ncomp;
    f->axis[0] = axis;
    f->values = values;
}

struct Env {
    Buf mv{0xa88};
    Buf owner{0x40};
    Buf m{sizeof(DB_STEP_DATA)};
    std::vector<double> rec = std::vector<double>(200, 99.0);  // sentinel: unwritten words stay 99
    std::vector<std::int32_t> iifnc = std::vector<std::int32_t>(16, 0);
    std::vector<double> llfnc = std::vector<double>(64, -5.0);
    std::int32_t rec_no = 7;
    const std::int32_t rec_type = 3;

    explicit Env(std::int32_t dim)
    {
        owner.at<std::int32_t>(0x1c) = dim;
        mv.at<void*>(0xa80) = owner.p();
        m.at<std::int32_t>(0x88) = 100;   // functions allocated (no FNCLOC warnings)
        m.at<std::int32_t>(0x8c) = 1000;  // LLFNC size
        std::memcpy(mv.p(), "DIE1", 5);
    }
    double R(std::ptrdiff_t off) const { return rec[static_cast<std::size_t>(off / 8)]; }
    void run()
    {
        write_bnd_movement(mv.p(), &rec_type, rec.data(), iifnc.data(), llfnc.data(),
                           reinterpret_cast<DB_STEP_DATA*>(m.p()), &rec_no);
    }
    bool name_at(std::ptrdiff_t off) const
    {
        const char* s = reinterpret_cast<const char*>(rec.data()) + off;
        if (std::memcmp(s, "DIE1", 4) != 0) return false;
        for (int i = 4; i < 64; ++i)
            if (s[i] != ' ') return false;
        return true;
    }
};

} // namespace

// ---- helpers --------------------------------------------------------------------------------

TEST_CASE(snap_direction_2d_snaps_within_tolerance)
{
    Buf mv(0xa88);
    mv.at<double>(0x50) = 1e-12;              // |x| <= 1e-12 -> 0
    mv.at<double>(0x58) = 1.0 - 0.5e-12;      // |1 - y| <= 1e-12 -> 1
    snap_direction_2d(mv.p());
    CHECK_BITS(mv.at<double>(0x50), 0.0);
    CHECK_BITS(mv.at<double>(0x58), 1.0);

    mv.at<double>(0x50) = 2e-12;              // neither
    mv.at<double>(0x58) = std::numeric_limits<double>::quiet_NaN();  // comparisons false
    snap_direction_2d(mv.p());
    CHECK_BITS(mv.at<double>(0x50), 2e-12);
    CHECK(mv.at<double>(0x58) != mv.at<double>(0x58));

    mv.at<double>(0x50) = -1e-13;             // negative small -> 0 (fabs)
    mv.at<double>(0x58) = 0.7;
    snap_direction_2d(mv.p());
    CHECK_BITS(mv.at<double>(0x50), 0.0);
    CHECK_BITS(mv.at<double>(0x58), 0.7);
}

TEST_CASE(normalize_vectors)
{
    Buf mv(0xa88);
    mv.at<double>(0x50) = 3.0;  // 9 + 16 + 0 = 25, sqrt 5
    mv.at<double>(0x58) = 4.0;
    normalize_direction_3d(mv.p());
    CHECK_BITS(mv.at<double>(0x50), 0.6);  // 3/5 correctly rounded
    CHECK_BITS(mv.at<double>(0x58), 0.8);
    CHECK_BITS(mv.at<double>(0x60), 0.0);

    // zero vector: untouched (ucomisd == 0)
    normalize_axis_648(mv.p());
    CHECK_BITS(mv.at<double>(0x648), 0.0);
    mv.at<double>(0x658) = 2.0;
    normalize_axis_648(mv.p());
    CHECK_BITS(mv.at<double>(0x658), 1.0);

    mv.at<double>(0x778) = 1.0;  // 1 + 4 + 4 = 9, sqrt 3
    mv.at<double>(0x780) = 2.0;
    mv.at<double>(0x788) = -2.0;
    normalize_axis_778(mv.p());
    CHECK_BITS(mv.at<double>(0x778), 1.0 / 3.0);
    CHECK_BITS(mv.at<double>(0x780), 2.0 / 3.0);
    CHECK_BITS(mv.at<double>(0x788), -2.0 / 3.0);
}

TEST_CASE(copy_sim_ctrl_words)
{
    Buf mv(0xa88);
    Buf m(sizeof(DB_STEP_DATA));
    const std::ptrdiff_t src[6] = {0x8d0, 0x8d8, 0x8e8, 0x8f0, 0x900, 0x908};
    for (int i = 0; i < 6; ++i) m.at<double>(src[i]) = 10.0 + i;
    m.at<double>(0x8e0) = -1.0;  // skipped words
    m.at<double>(0x8f8) = -1.0;
    copy_sim_ctrl_to_movement(reinterpret_cast<DB_STEP_DATA*>(m.p()), mv.p());
    for (int i = 0; i < 6; ++i) CHECK_BITS(mv.at<double>(0x1b8 + 8 * i), 10.0 + i);
}

TEST_CASE(speed_axis_to_time)
{
    reset_hook();
    alignas(8) unsigned char slot[sizeof(DYNARY)] = {};
    double axis[3] = {2.0, 4.0, 9.0};
    // ncomp 4 -> 3 coordinates: segment 1 length 5 (3,4,0), segment 2 length 12 (0,0,12)
    double values[12] = {0, 0, 0, 100, 3, 4, 0, 200, 3, 4, 12, 300};
    set_table(slot, 0x7a, 0x1a, 3, 4, axis, values);
    auto* f = reinterpret_cast<DYNARY*>(slot);
    CHECK(dynary_speed_axis_to_time(f));
    CHECK_BITS(axis[0], 0.0);
    CHECK_BITS(axis[1], 2.5);  // 0 + 5 / 2 (old axis[0])
    CHECK_BITS(axis[2], 5.5);  // 2.5 + 12 / 4 (old axis[1])
    CHECK_EQ(f->var[0], 1);
    CHECK_EQ(g.calls.size(), std::size_t{1});
    CHECK_EQ(g.calls[0], std::uint64_t{0x14012eff0});

    // ncomp 2 -> 2 coordinates; zero speed at the segment start repeats the previous value
    double axis2[3] = {0.0, 3.0, 1.0};
    double v2[6] = {0, 0, 6, 8, 6, 11};
    set_table(slot, 0x79, 0x1a, 3, 2, axis2, v2);
    CHECK(dynary_speed_axis_to_time(f));
    CHECK_BITS(axis2[1], 0.0);  // old axis[0] = 0 -> copy
    CHECK_BITS(axis2[2], 1.0);  // 0 + 3 / 3

    // rejected: wrong type, wrong variable, no points (no copy made)
    g.calls.clear();
    set_table(slot, 0x7c, 0x1a, 3, 2, axis2, v2);
    CHECK(!dynary_speed_axis_to_time(f));
    set_table(slot, 0x7b, 0x01, 3, 2, axis2, v2);
    CHECK(!dynary_speed_axis_to_time(f));
    set_table(slot, 0x7b, 0x1a, 0, 2, axis2, v2);
    CHECK(!dynary_speed_axis_to_time(f));
    CHECK(g.calls.empty());
    ext::g_hook = nullptr;
}

// ---- the writer -----------------------------------------------------------------------------

TEST_CASE(writer_2d_type0_const_speed)
{
    reset_hook();
    Env e(2);
    e.mv.at<double>(0x50) = 1e-13;        // snapped to 0
    e.mv.at<double>(0x58) = 1.0 - 1e-13;  // snapped to 1
    e.mv.at<std::int32_t>(0x68) = 0;
    e.mv.at<std::int32_t>(0x74) = 0;
    e.mv.at<double>(0x78) = 2.5;          // speed
    e.mv.at<double>(0x170) = 11.0;
    e.mv.at<double>(0x178) = 12.0;
    e.mv.at<std::int32_t>(0x620) = 1;     // follow object |-7|
    e.mv.at<std::int32_t>(0x624) = -7;
    for (int i = 0; i < 6; ++i)
        e.m.at<double>(0x8d0 + std::ptrdiff_t{8} * (i < 2 ? i : i < 4 ? i + 1 : i + 2)) = 20.0 + i;
    e.mv.at<std::int32_t>(0x628) = 0;     // ANGMOV mode 0, constant
    e.mv.at<double>(0x668) = 33.0;
    e.mv.at<double>(0x630) = 34.0;
    e.mv.at<double>(0x638) = 35.0;
    e.mv.at<double>(0x750) = 36.0;
    double sx[2] = {0.0, 1.0}, sv[2] = {5.0, 6.0};
    set_table(static_cast<char*>(e.mv.p()) + 0x8c0, 0x7c, 1, 2, 1, sx, sv);
    e.mv.at<char>(0x8b8) = 1;             // SPDLMT curve
    e.mv.at<double>(0x938) = 40.0;
    e.mv.at<double>(0x940) = 41.0;
    e.mv.at<double>(0x948) = 42.0;
    e.mv.at<double>(0x950) = 43.0;
    e.mv.at<double>(0x958) = 44.0;
    e.mv.at<double>(0x888) = 50.0;
    e.mv.at<double>(0x890) = 51.0;
    e.mv.at<double>(0x898) = 52.0;        // not in 2D
    e.mv.at<double>(0x8a0) = 53.0;
    e.mv.at<double>(0x8a8) = 54.0;
    e.m.at<std::int32_t>(0x850) = 0;      // no ANGMVY words
    e.run();

    CHECK(e.name_at(0x1c0));
    CHECK_BITS(e.R(0x50), 0.0);
    CHECK_BITS(e.R(0x58), 1.0);
    CHECK_BITS(e.R(0x38), 0.0);
    CHECK_BITS(e.R(0x48), 2.5);
    CHECK_BITS(e.R(0x60), -1.0);  // 0.0 by type 0, then -1.0 by the follow flag
    CHECK_BITS(e.R(0x70), 7.0);
    CHECK_BITS(e.R(0x90), 11.0);
    CHECK_BITS(e.R(0x98), 12.0);
    CHECK_BITS(e.R(0xb0), 0.0);   // 2.5 * 0
    CHECK_BITS(e.R(0xb8), 2.5);   // 2.5 * 1
    CHECK_BITS(e.R(0xa0), 99.0);  // type 1 words not written
    for (int i = 0; i < 6; ++i) CHECK_BITS(e.R(0xc0 + 8 * i), 20.0 + i);
    CHECK_BITS(e.R(0xf0), 0.0);
    CHECK_BITS(e.R(0xf8), 0.0);
    CHECK_BITS(e.R(0x118), 0.0);
    CHECK_BITS(e.R(0x100), 33.0);
    CHECK_BITS(e.R(0x120), 99.0);
    CHECK_BITS(e.R(0x108), 34.0);
    CHECK_BITS(e.R(0x110), 35.0);
    CHECK_BITS(e.R(0x128), 36.0);
    CHECK_BITS(e.R(0x40), 1.0);   // SPDLMT: first function number
    CHECK_BITS(e.R(0x1b0), 40.0);
    CHECK_BITS(e.R(0x1b8), 41.0);
    CHECK_BITS(e.R(0x198), 42.0);
    CHECK_BITS(e.R(0x1a0), 43.0);
    CHECK_BITS(e.R(0x1a8), 44.0);
    CHECK_BITS(e.R(0x130), 99.0); // ANGMVY skipped
    CHECK_BITS(e.R(0x208), 50.0);
    CHECK_BITS(e.R(0x210), 51.0);
    CHECK_BITS(e.R(0x218), 53.0);
    CHECK_BITS(e.R(0x220), 54.0);
    // the 3D-only name position is untouched
    CHECK_BITS(e.R(0x290), 99.0);
    // alloc_function: iifnc pair (id = rec_no, start 1), end 1 + 4; LLFNC rows (x, v)
    CHECK_EQ(e.iifnc[0], 1);
    CHECK_EQ(e.iifnc[1], 7);
    CHECK_EQ(e.iifnc[2], 5);
    CHECK_BITS(e.llfnc[0], 0.0);
    CHECK_BITS(e.llfnc[1], 5.0);
    CHECK_BITS(e.llfnc[2], 1.0);
    CHECK_BITS(e.llfnc[3], 6.0);
    CHECK_EQ(g.calls.size(), std::size_t{1});
    CHECK_EQ(g.calls[0], std::uint64_t{0x140130290});
    ext::g_hook = nullptr;
}

TEST_CASE(writer_2d_type6_curve_and_angmvy)
{
    reset_hook();
    Env e(2);
    e.mv.at<std::int32_t>(0x68) = 6;
    e.mv.at<std::int32_t>(0x510) = 0;
    e.mv.at<std::int32_t>(0x4c4) = -4;
    double ax[2] = {2.0, 0.0};
    double vv[4] = {0.0, 0.0, 6.0, 8.0};  // segment length 10, old axis[0] = 2 -> 5
    // fn@0x448: mv+0x458 is its var[0] (0x1a != 1: transformed copy), mv+0x464 its n[0] (2)
    set_table(static_cast<char*>(e.mv.p()) + 0x448, 0x79, 0x1a, 2, 2, ax, vv);
    g.len_cde0 = 6;
    e.m.at<std::int32_t>(0x850) = 1;      // ANGMVY: mode 1, 0x960 == 1, dimension 2
    e.mv.at<std::int32_t>(0x960) = 1;
    e.mv.at<std::int32_t>(0x9e4) = 0;
    e.mv.at<double>(0x9e8) = 61.0;
    e.mv.at<double>(0xa70) = 62.0;
    e.mv.at<std::int32_t>(0x628) = 1;     // ANGMOV mode 1, curve with xt 10
    double cx[2] = {0.0, 1.0}, cv[2] = {7.0, 8.0};
    // fn@0x6d8: mv+0x6dc is its ndim (1), mv+0x6e8 its var[0] (10)
    set_table(static_cast<char*>(e.mv.p()) + 0x6d8, 0x7c, 10, 2, 1, cx, cv);
    e.run();

    CHECK_BITS(e.R(0x38), 6.0);
    CHECK_BITS(e.R(0x60), 1.0);
    CHECK_BITS(e.R(0x78), 2.0);   // (double) mv+0x464 = n[0] of fn@0x448
    CHECK_BITS(e.R(0x70), 1.0);   // function 1
    CHECK_BITS(e.R(0x68), 5.0);
    CHECK_BITS(e.R(0x80), -4.0);
    // rows (x, v0, v1) of the transformed copy; the object's own table is unchanged
    CHECK_BITS(e.llfnc[0], 0.0);
    CHECK_BITS(e.llfnc[1], 0.0);
    CHECK_BITS(e.llfnc[2], 0.0);
    CHECK_BITS(e.llfnc[3], 5.0);
    CHECK_BITS(e.llfnc[4], 6.0);
    CHECK_BITS(e.llfnc[5], 8.0);
    CHECK_BITS(ax[1], 0.0);
    // ANGMOV: function 2 (start 1 + 6 = 7, length 4)
    CHECK_BITS(e.R(0xf0), 1.0);
    CHECK_BITS(e.R(0x118), 2.0);
    CHECK_BITS(e.R(0x120), 2.0);
    CHECK_BITS(e.R(0xf8), 0.0);   // cleared before the mode test
    CHECK_EQ(e.iifnc[2], 7);
    CHECK_EQ(e.iifnc[3], 7);
    CHECK_EQ(e.iifnc[4], 11);
    CHECK_BITS(e.llfnc[6], 0.0);
    CHECK_BITS(e.llfnc[7], 7.0);
    CHECK_BITS(e.llfnc[8], 1.0);
    CHECK_BITS(e.llfnc[9], 8.0);
    // ANGMVY
    CHECK_BITS(e.R(0x130), 1.0);
    CHECK_BITS(e.R(0x138), 1.0);
    CHECK_BITS(e.R(0x140), 0.0);
    CHECK_BITS(e.R(0x148), 61.0);
    CHECK_BITS(e.R(0x170), 62.0);
    const std::vector<std::uint64_t> want = {0x14018cde0, 0x14012eff0, 0x14012eff0, 0x140130290};
    CHECK(g.calls == want);  // curve length, copy, the transform's own copy, ANGMOV length
    ext::g_hook = nullptr;
}

TEST_CASE(writer_2d_type6_die_follow)
{
    reset_hook();
    Env e(2);
    e.mv.at<std::int32_t>(0x68) = 6;
    e.mv.at<std::int32_t>(0x510) = 1;
    e.mv.at<std::int32_t>(0x514) = -12;
    e.mv.at<std::int32_t>(0x628) = 5;     // neither ANGMOV mode: 0xf0 stays 1.0
    e.m.at<std::int32_t>(0x850) = 3;
    e.mv.at<std::int32_t>(0x960) = -1;
    e.run();
    CHECK_BITS(e.R(0x60), -1.0);
    CHECK_BITS(e.R(0x70), 12.0);
    CHECK_BITS(e.R(0xf0), 1.0);
    CHECK_BITS(e.R(0x130), 0.0);
    CHECK_BITS(e.R(0x138), 99.0);
    CHECK(g.calls.empty());
    ext::g_hook = nullptr;
}

TEST_CASE(writer_3d_type1_and_axes)
{
    reset_hook();
    Env e(3);
    e.mv.at<double>(0x58) = 3.0;  // direction (0, 3, 4) -> (0, 0.6, 0.8)
    e.mv.at<double>(0x60) = 4.0;
    e.mv.at<std::int32_t>(0x68) = 1;
    e.mv.at<std::int32_t>(0x160) = 5;
    e.mv.at<double>(0x168) = 70.0;
    e.mv.at<double>(0x180) = 71.0;
    e.mv.at<double>(0x640) = 72.0;
    e.mv.at<double>(0x650) = 2.0;  // CNTRAX axis -> (0, 1, 0)
    e.mv.at<double>(0x788) = -5.0; // CNTRA2 axis -> (0, 0, -1)
    e.mv.at<double>(0x760) = 73.0;
    e.mv.at<double>(0x880) = 74.0;
    e.mv.at<double>(0x898) = 75.0;
    e.mv.at<double>(0x8b0) = 76.0;
    e.mv.at<std::int32_t>(0x758) = 0;
    e.mv.at<double>(0x798) = 77.0;
    e.run();
    CHECK(e.name_at(0x290));
    CHECK_BITS(e.R(0x1c0), 73.0);  // CNTRA2 centre, not the 2D name
    CHECK_BITS(e.R(0x60), 0.0);
    CHECK_BITS(e.R(0x68), 0.6);
    CHECK_BITS(e.R(0x70), 0.8);
    CHECK_BITS(e.R(0x78), 1.0);
    CHECK_BITS(e.R(0x80), -1005.0);  // -(5 + 1000)
    CHECK_BITS(e.R(0x88), 70.0);
    CHECK_BITS(e.R(0xa0), 71.0);
    // type 1 and 0xec == 0: the force words are written although 0x160 != 0 (0xf0 = 0)
    CHECK_BITS(e.R(0xa8), 0.0);
    CHECK_BITS(e.R(0xb0), 0.0);
    CHECK_BITS(e.R(0xb8), 0.0);
    CHECK_BITS(e.R(0xd8), 0.0);      // ANGMOV mode 0, constant
    CHECK_BITS(e.R(0xe0), 0.0);
    CHECK_BITS(e.R(0x130), 0.0);
    CHECK_BITS(e.R(0x118), 72.0);
    CHECK_BITS(e.R(0xf0), 0.0);
    CHECK_BITS(e.R(0xf8), 1.0);
    CHECK_BITS(e.R(0x100), 0.0);
    CHECK_BITS(e.R(0x190), 0.0);
    CHECK_BITS(e.R(0x198), 0.0);
    CHECK_BITS(e.R(0x1e8), 77.0);
    CHECK_BITS(e.R(0x1a8), 0.0);
    CHECK_BITS(e.R(0x1b0), 0.0);
    CHECK_BITS(e.R(0x1b8), -1.0);
    CHECK_BITS(e.R(0x1d8), 74.0);
    CHECK_BITS(e.R(0x2e8), 75.0);
    CHECK_BITS(e.R(0x300), 76.0);
    CHECK_BITS(e.R(0x258), 99.0);    // no SPDLMT curve
    CHECK(g.calls.empty());
    ext::g_hook = nullptr;
}

TEST_CASE(writer_3d_type1_force_and_wraparound)
{
    reset_hook();
    Env e(3);
    e.mv.at<double>(0x50) = 1.0;     // direction (1, 0, 0)
    e.mv.at<std::int32_t>(0x68) = 1;
    e.mv.at<std::int32_t>(0x160) = 0;
    e.mv.at<std::int32_t>(0xec) = 0;
    e.mv.at<double>(0xf0) = 1.5;
    e.run();
    CHECK_BITS(e.R(0x80), 0.0);
    CHECK_BITS(e.R(0x88), 1.5);
    CHECK_BITS(e.R(0xa8), 1.5);      // 1.5 * 1
    CHECK_BITS(e.R(0xb0), 0.0);
    CHECK_BITS(e.R(0xb8), 0.0);

    Env w(3);
    w.mv.at<std::int32_t>(0x68) = 1;
    w.mv.at<std::int32_t>(0x160) = INT_MAX;  // INT_MAX + 1000 wraps to INT_MIN + 999; negated
    w.run();
    CHECK_BITS(w.R(0x80), 2147482649.0);

    Env f(3);
    f.mv.at<std::int32_t>(0x68) = 9;
    f.mv.at<double>(0x608) = 80.0;
    f.mv.at<std::int32_t>(0x620) = 1;
    f.mv.at<std::int32_t>(0x624) = INT_MIN;  // abs(INT_MIN) = INT_MIN, NEG keeps it
    f.run();
    CHECK_BITS(f.R(0x78), 9.0);
    CHECK_BITS(f.R(0x150), 80.0);
    CHECK_BITS(f.R(0x80), -2147483648.0);
    ext::g_hook = nullptr;
}

TEST_CASE(writer_3d_type6_frame_header)
{
    reset_hook();
    Env e(3);
    e.mv.at<std::int32_t>(0x68) = 6;
    e.mv.at<std::int32_t>(0x510) = 0;
    e.mv.at<std::int32_t>(0x4c0) = 2;  // frame applied
    e.mv.at<std::int32_t>(0x4c4) = 9;
    for (int i = 0; i < 9; ++i) e.mv.at<double>(0x4c8 + 8 * i) = 1.0 + i;
    double ax[2] = {0.0, 2.0};
    double vv[6] = {1, 2, 3, 4, 5, 6};
    // var[0] (mv+0x458) = 1: no speed transform; n[0] (mv+0x464) = 2
    set_table(static_cast<char*>(e.mv.p()) + 0x448, 0x7b, 1, 2, 3, ax, vv);
    g.len_cde0 = 18;
    e.run();
    CHECK_BITS(e.R(0x78), 6.0);
    CHECK_BITS(e.R(0x148), 2.0);
    CHECK_BITS(e.R(0x88), 2.0);
    CHECK_BITS(e.R(0x80), 1.0);
    CHECK_BITS(e.R(0x150), 2.0);
    CHECK_BITS(e.R(0x158), 9.0);
    // 140194d40 default: frame[0]=mv+0x4e0 (4), [3]=0x4e8 (5), [6]=0x4f0 (6), [1]=0x4f8 (7),
    // [4]=0x500 (8), [7]=0x508 (9), [2]=0x4c8 (1), [5]=0x4d0 (2), [8]=0x4d8 (3)
    const double want_frame[9] = {4, 7, 1, 5, 8, 2, 6, 9, 3};
    for (int i = 0; i < 9; ++i) CHECK_BITS(g.frame[i], want_frame[i]);
    CHECK_EQ(g.to_frame_type, 0x7b);
    // LLFNC: 10.0, the raw frame mv+0x4c8..0x508, then rows (x, v[3])
    CHECK_BITS(e.llfnc[0], 10.0);
    for (int i = 0; i < 9; ++i) CHECK_BITS(e.llfnc[1 + i], 1.0 + i);
    const double rows[8] = {0, 1, 2, 3, 2, 4, 5, 6};
    for (int i = 0; i < 8; ++i) CHECK_BITS(e.llfnc[10 + i], rows[i]);
    CHECK_EQ(e.iifnc[2], 19);
    const std::vector<std::uint64_t> want = {0x14012eff0, 0x140194d40, 0x140135bc0, 0x14018cde0};
    CHECK(g.calls == want);
    ext::g_hook = nullptr;
}

TEST_CASE(writer_other_dimension_writes_nothing)
{
    reset_hook();
    Env e(1);
    e.mv.at<char>(0x8b8) = 1;
    e.run();
    for (double v : e.rec) CHECK_BITS(v, 99.0);
    CHECK(g.calls.empty());
    ext::g_hook = nullptr;
}

int main() { return testkit::run_all(); }
