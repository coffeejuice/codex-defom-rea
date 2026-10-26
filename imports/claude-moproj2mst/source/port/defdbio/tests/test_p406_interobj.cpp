// Tier-1 tests of the p4-06 inter-object writer (defdbio/gendb/p4_06_interobj.hpp): LLBRT record
// words (FUN_1401f7730) in 2D and 3D, the mirror record and pair loop (FUN_1401fa4f0), the boundary
// choice (FUN_1401fa840) and the nearest-point searches (FUN_140166b40, FUN_140166980, FUN_14027b620).
// Expected values are worked out by hand from the decompile / listing (comments).
// ctest-labels: p4-06 gendb
#include <defdbio/gendb/p4_06_interobj.hpp>

#include <bit>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <functional>
#include <map>
#include <memory>
#include <vector>

#include <defdbio/gendb/ext_p4_06.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/obj_geo2d.hpp>
#include <defmodel/step/db_step.hpp>
#include <testkit/check.hpp>

using namespace def::dbio::gendb;
using def::model::Cls_1401f46e0;
using def::model::Cls_1401fa020;
using def::model::DB_STEP_DATA;
using def::model::DYNARY;
using def::model::field;
using def::model::LLOBJ_DATA;
using def::model::OBJ_GEO2D;

namespace {

constexpr double S = -7.0;   // sentinel: record words the writer must leave alone

// zeroed, aligned raw storage for a class whose fields the tests set by offset
struct Raw {
    explicit Raw(std::size_t n) : words((n + 15) / 16) {}
    void* p() { return words.data(); }
    struct alignas(16) W { std::byte b[16]{}; };
    std::vector<W> words;
};

using Fake = std::function<std::uint64_t(const std::uint64_t*, int)>;
std::map<std::uint64_t, Fake> g_fakes;
std::vector<std::uint64_t> g_unexpected;

std::uint64_t hook(std::uint64_t addr, const std::uint64_t* a, int n)
{
    auto it = g_fakes.find(addr);
    if (it == g_fakes.end()) {
        g_unexpected.push_back(addr);
        return 0;
    }
    return it->second(a, n);
}

void reset_hooks()
{
    g_fakes.clear();
    g_unexpected.clear();
    ext::g_hook = &hook;
}

template <class T>
T* ptr(std::uint64_t v) { return reinterpret_cast<T*>(static_cast<std::uintptr_t>(v)); }
double dbl(std::uint64_t v) { return std::bit_cast<double>(v); }

struct Model {
    Raw raw{sizeof(DB_STEP_DATA)};
    DB_STEP_DATA* m = static_cast<DB_STEP_DATA*>(raw.p());
    explicit Model(int dim)
    {
        field<std::int32_t>(m, 0x1c) = dim;
        field<std::int32_t>(m, 0x88) = 100;      // allocated function slots (no FNCLOC warning)
        field<std::int32_t>(m, 0x8c) = 10000;    // LLFNC length
        field<std::int32_t>(m, 0x134) = 5;       // next LLWIN record
        field<std::int32_t>(m, 0x138) = 7;       // next LLWRZ row
    }
};

struct Node {
    Raw raw{sizeof(Cls_1401f46e0)};
    Cls_1401f46e0* io = static_cast<Cls_1401f46e0*>(raw.p());
    explicit Node(DB_STEP_DATA* m) { io->owner = m; }
};

// 1-D function: 2 points (x, y), ncomp 1 -> 140130290 length 2 * n1 = 4, LLFNC x0 y0 x1 y1
struct Fn1 {
    double ax[2];
    double vals[2];
    void attach(DYNARY& f, std::int32_t kind, double base)
    {
        ax[0] = base;
        ax[1] = base + 1.0;
        vals[0] = base + 0.5;
        vals[1] = base + 1.5;
        f.ndim = 1;
        f.var[0] = kind;
        f.n[0] = 2;
        f.ncomp = 1;
        f.axis[0] = ax;
        f.values = vals;
    }
};

} // namespace

// ---- 1401f7730 ------------------------------------------------------------------------------

TEST_CASE(record_flag0_writes_only_word0)
{
    reset_hooks();
    Model md(2);
    Node nd(md.m);
    std::vector<double> rec(50, S);
    std::vector<std::int32_t> iifnc(40, 0);
    std::vector<double> llfnc(100, 0.0);
    const std::int32_t i = 1, j = 2, wwin = 20;
    write_interobj_record(nd.io, &i, &j, rec.data(), iifnc.data(), llfnc.data(), nullptr, nullptr, &wwin);
    CHECK_BITS(rec[0], 0.0);
    for (int k = 1; k < 50; ++k) CHECK_BITS(rec[k], S);
    CHECK_EQ(field<std::int32_t>(md.m, 0x10c), 0);
}

TEST_CASE(record_2d_constants)
{
    reset_hooks();
    Model md(2);
    Node nd(md.m);
    Cls_1401f46e0* io = nd.io;
    io->u170 = 1;                                   // master
    field<std::int32_t>(io, 0x0) = 3;               // friction type 3 -> r2 = 3.0
    field<std::int32_t>(io, 0x10) = 0;              // code 0: constant
    field<double>(io, 0x20) = 0.3;                  // value (DYNARY 0x18 + 8)
    io->u178 = 1;                                   // SEPRES code 1
    io->u180 = std::bit_cast<std::uint64_t>(0.01);
    io->u1b8.value = 5.0;                           // IHTCOF ndim 0
    io->u238 = std::bit_cast<std::uint64_t>(2.5);   // SEPDEN
    io->u240.value = 0.7;                           // FRCROT ndim 0
    io->u2b8.value = 9.0;                           // INTRST ndim 0
    io->u37c = 2;
    io->u380 = 3;
    io->u384 = 4;
    io->u388 = std::bit_cast<std::uint64_t>(1.5);
    io->u340 = 0;                                   // WMODEL none
    std::vector<double> rec(50, S);
    std::vector<std::int32_t> iifnc(40, 0);
    std::vector<double> llfnc(100, 0.0);
    const std::int32_t i = 1, j = 2, wwin = 20;
    write_interobj_record(io, &i, &j, rec.data(), iifnc.data(), llfnc.data(), nullptr, nullptr, &wwin);
    CHECK(g_unexpected.empty());
    CHECK_BITS(rec[0], 1.0);
    CHECK_BITS(rec[1], S);          // 2D never writes word 1
    CHECK_BITS(rec[2], 3.0);
    CHECK_BITS(rec[3], 0.3);
    CHECK_BITS(rec[4], S);          // no function (ndim 0)
    CHECK_BITS(rec[5], 0.0);        // code 0
    CHECK_BITS(rec[6], 5.0);
    CHECK_BITS(rec[7], 0.0);
    CHECK_BITS(rec[8], S);          // IHTCOF kind word only for functions / refs
    for (int k = 9; k <= 0xc; ++k) CHECK_BITS(rec[k], S);
    CHECK_BITS(rec[0xd], 0.0);
    CHECK_BITS(rec[0xe], 0.7);
    CHECK_BITS(rec[0xf], 0.0);      // no windows
    CHECK_BITS(rec[0x10], 1.0);
    CHECK_BITS(rec[0x11], 0.01);
    CHECK_BITS(rec[0x12], 2.5);
    CHECK_BITS(rec[0x13], 0.0);
    CHECK_BITS(rec[0x14], 9.0);
    CHECK_BITS(rec[0x15], S);
    CHECK_BITS(rec[0x16], 0.0);
    CHECK_BITS(rec[0x17], 0.0);
    CHECK_BITS(rec[0x18], 0.0);     // extras zeroed
    CHECK_BITS(rec[0x19], 0.0);
    CHECK_BITS(rec[0x1a], 0.0);
    CHECK_BITS(rec[0x1b], 2.0);
    CHECK_BITS(rec[0x1c], 3.0);
    CHECK_BITS(rec[0x1d], 4.0);
    CHECK_BITS(rec[0x1e], 1.5);
    CHECK_BITS(rec[0x1f], 0.0);     // WMODEL 0
    for (int k = 0x20; k < 50; ++k) CHECK_BITS(rec[k], S);   // type 3: no second set
    CHECK_EQ(field<std::int32_t>(md.m, 0x10c), 0);           // no function allocated
}

TEST_CASE(record_2d_functions_windows_type4)
{
    reset_hooks();
    Model md(2);
    Node nd(md.m);
    Cls_1401f46e0* io = nd.io;
    io->u170 = 1;
    field<std::int32_t>(io, 0x0) = 4;               // type 4: two sets
    field<std::int32_t>(io, 0x10) = 1;              // set 1 code 1 (1-variable function)
    Fn1 f1, frot, fiht, fint;
    f1.attach(field<DYNARY>(io, 0x18), 1, 10.0);
    field<std::uint8_t>(io, 0x5) = 1;               // set 2 negative: ref 3
    field<std::int32_t>(io, 0xc) = 3;
    field<double>(io, 0x98) = 0.25;
    frot.attach(io->u240, 4, 20.0);                 // FRCROT kind 4: mode word not written
    fiht.attach(io->u1b8, 3, 30.0);                 // IHTCOF kind 3 -> r8 = 3.0
    fint.attach(io->u2b8, 3, 40.0);                 // INTRST kind 3 -> r19 = 1.0
    io->u340 = 1;                                   // WMODEL 4 reals
    io->u348[0] = std::bit_cast<std::uint64_t>(1.25);
    io->u348[1] = std::bit_cast<std::uint64_t>(2.25);
    io->u348[2] = std::bit_cast<std::uint64_t>(3.25);
    io->u348[3] = std::bit_cast<std::uint64_t>(4.25);
    // two windows in the list at +0x188 (head at +0, next at +0x840, count at +0x14)
    Raw w1(0x848), w2(0x848);
    field<void*>(io, 0x188) = w1.p();
    field<void*>(w1.p(), 0x840) = w2.p();
    field<std::int32_t>(io, 0x188 + 0x14) = 2;
    std::vector<double> llwin(400, 0.0), llwrz(100, 0.0);
    std::vector<std::uint64_t> win_args;
    g_fakes[0x140260ea0] = [&](const std::uint64_t* a, int n) -> std::uint64_t {
        CHECK_EQ(n, 10);
        win_args.push_back(a[0]);
        win_args.push_back(a[1]);
        win_args.push_back(a[9]);
        CHECK(ptr<double>(a[2]) == llwrz.data());
        CHECK(ptr<DB_STEP_DATA>(a[7]) == md.m);
        *ptr<std::int32_t>(a[5]) += 1;   // window cursor
        *ptr<std::int32_t>(a[6]) += 2;   // LLWRZ cursor
        return 0;
    };
    std::vector<double> rec(50, S);
    std::vector<std::int32_t> iifnc(40, 0);
    std::vector<double> llfnc(100, S);
    const std::int32_t i = 1, j = 2, wwin = 20;
    write_interobj_record(io, &i, &j, rec.data(), iifnc.data(), llfnc.data(), llwin.data(), llwrz.data(), &wwin);
    CHECK(g_unexpected.empty());
    // function numbers in binary order: set 1 (1), FRCROT (2), IHTCOF (3), INTRST (4), WMODEL (5);
    // each 1-D function has length 4, so the LLFNC starts are 1, 5, 9, 13, 17
    CHECK_BITS(rec[2], 4.0);
    CHECK_BITS(rec[5], 1.0);
    CHECK_BITS(rec[3], S);          // code 1: value not written
    CHECK_BITS(rec[4], 1.0);
    CHECK_BITS(llfnc[0], 10.0);
    CHECK_BITS(llfnc[1], 10.5);
    CHECK_BITS(llfnc[2], 11.0);
    CHECK_BITS(llfnc[3], 11.5);
    CHECK_BITS(rec[0x18], 0.0);
    CHECK_BITS(rec[0x23], -3.0);    // set 2 ref
    CHECK_BITS(rec[0x21], 0.25);
    CHECK_BITS(rec[0x22], S);
    CHECK_BITS(rec[0x24], S);       // negative set: extras untouched
    // windows: records 5 and 6 (cursor 5, W 20 -> llwin + 80, + 100), other object = slave 2
    REQUIRE(win_args.size() == 6);
    CHECK(ptr<void>(win_args[0]) == w1.p());
    CHECK(ptr<double>(win_args[1]) == llwin.data() + 80);
    CHECK_EQ(static_cast<std::int32_t>(win_args[2]), 2);
    CHECK(ptr<void>(win_args[3]) == w2.p());
    CHECK(ptr<double>(win_args[4]) == llwin.data() + 100);
    CHECK_BITS(rec[0x16], 5.0);
    CHECK_BITS(rec[0x17], 6.0);
    CHECK_BITS(rec[0xf], 1.0);
    CHECK_EQ(field<std::int32_t>(md.m, 0x134), 7);
    CHECK_EQ(field<std::int32_t>(md.m, 0x138), 11);
    CHECK_BITS(rec[0xd], S);        // FRCROT kind 4: mode word left
    CHECK_BITS(rec[0xe], 2.0);
    CHECK_BITS(llfnc[4], 20.0);
    CHECK_BITS(rec[8], 3.0);
    CHECK_BITS(rec[7], 3.0);
    CHECK_BITS(rec[6], S);
    CHECK_BITS(llfnc[8], 30.0);
    CHECK_BITS(rec[0x13], 1.0);
    CHECK_BITS(rec[0x14], 4.0);
    CHECK_BITS(llfnc[12], 40.0);
    CHECK_BITS(rec[0x1f], 1.0);
    CHECK_BITS(rec[0x20], 5.0);
    CHECK_BITS(llfnc[16], 1.25);
    CHECK_BITS(llfnc[17], 2.25);
    CHECK_BITS(llfnc[18], 3.25);
    CHECK_BITS(llfnc[19], 4.25);
    CHECK_BITS(llfnc[20], S);
    // IIFNC (1bb3d0): [0] = 1, [2k-1] = 0, [2k] = [2k-2] + 4
    CHECK_EQ(iifnc[0], 1);
    CHECK_EQ(iifnc[2], 5);
    CHECK_EQ(iifnc[10], 21);
    CHECK_EQ(field<std::int32_t>(md.m, 0x10c), 5);
}

TEST_CASE(record_2d_slave_flag_kind_variants)
{
    reset_hooks();
    Model md(2);
    Node nd(md.m);
    Cls_1401f46e0* io = nd.io;
    io->u170 = 2;                                   // slave -> r0 = 2.0; window object = master
    field<std::int32_t>(io, 0x0) = 1;
    field<std::uint8_t>(io, 0x4) = 1;               // set 1 negative ref 6
    field<std::int32_t>(io, 0x8) = 6;
    field<double>(io, 0x20) = 0.125;
    Fn1 frot, fiht, fint;
    frot.attach(io->u240, 1, 1.0);                  // FRCROT kind 1 -> r13 = 1.0
    fiht.attach(io->u1b8, 4, 2.0);                  // IHTCOF kind 4 -> 2.0, then ref override
    io->u234 = 1;
    io->u230 = 9;
    fint.attach(io->u2b8, 4, 3.0);                  // INTRST kind 4 -> 2.0
    io->u178 = 2;
    io->u340 = 3;                                   // WMODEL ref 8
    io->u378 = 8;
    Raw w1(0x848);
    field<void*>(io, 0x188) = w1.p();
    field<std::int32_t>(io, 0x188 + 0x14) = 1;
    std::int32_t objid = -1;
    g_fakes[0x140260ea0] = [&](const std::uint64_t* a, int) -> std::uint64_t {
        objid = static_cast<std::int32_t>(a[9]);
        return 0;   // cursor not advanced: no record counted
    };
    std::vector<double> rec(50, S);
    std::vector<std::int32_t> iifnc(40, 0);
    std::vector<double> llfnc(100, 0.0);
    std::vector<double> llwin(400, 0.0);
    const std::int32_t i = 3, j = 4, wwin = 20;
    write_interobj_record(io, &i, &j, rec.data(), iifnc.data(), llfnc.data(), llwin.data(), nullptr, &wwin);
    CHECK(g_unexpected.empty());
    CHECK_BITS(rec[0], 2.0);
    CHECK_BITS(rec[5], -6.0);
    CHECK_BITS(rec[3], 0.125);
    CHECK_BITS(rec[0x18], S);       // negative set: extras untouched
    CHECK_EQ(objid, 3);             // flag 2: master number
    CHECK_BITS(rec[0x16], 0.0);     // window cursor unchanged -> 0 / 0
    CHECK_BITS(rec[0x17], 0.0);
    CHECK_BITS(rec[0xf], 0.0);
    CHECK_BITS(rec[0x10], 2.0);
    CHECK_BITS(rec[0xd], 1.0);
    CHECK_BITS(rec[0xe], 1.0);      // first function
    CHECK_BITS(rec[7], 2.0);
    CHECK_BITS(rec[8], -9.0);       // ref overrides the kind word 2.0
    CHECK_BITS(rec[0x13], 2.0);
    CHECK_BITS(rec[0x14], 3.0);
    CHECK_BITS(rec[0x1f], -8.0);
    CHECK_BITS(rec[0x20], S);
    CHECK_EQ(field<std::int32_t>(md.m, 0x10c), 3);
}

TEST_CASE(record_2d_friction_codes_5_to_8_and_wmodel_other)
{
    for (std::int32_t code = 5; code <= 8; ++code) {
        reset_hooks();
        Model md(2);
        Node nd(md.m);
        Cls_1401f46e0* io = nd.io;
        io->u170 = 1;
        field<std::int32_t>(io, 0x0) = 2;
        field<std::int32_t>(io, 0x10) = code;
        field<std::int32_t>(io, 0x34) = 11;
        field<std::int32_t>(io, 0x38) = 12;
        field<std::int32_t>(io, 0x3c) = 13;
        for (int k = 0; k < 6; ++k) field<double>(io, 0x108 + 8 * k) = 100.0 + k;
        io->u340 = 4;                               // other mode: FNCLOC of length 0, code word left
        std::vector<double> rec(50, S);
        std::vector<std::int32_t> iifnc(40, 0);
        std::vector<double> llfnc(100, S);
        const std::int32_t i = 1, j = 2, wwin = 20;
        write_interobj_record(io, &i, &j, rec.data(), iifnc.data(), llfnc.data(), nullptr, nullptr, &wwin);
        CHECK_BITS(rec[2], 2.0);
        CHECK_BITS(rec[5], static_cast<double>(code));
        CHECK_BITS(rec[3], S);
        const double e0 = code == 5 ? 11.0 : 100.0 + 2 * (code - 6);
        const double e1 = code == 5 ? 12.0 : 101.0 + 2 * (code - 6);
        const double e2 = code == 5 ? 13.0 : 0.0;
        CHECK_BITS(rec[0x18], e0);
        CHECK_BITS(rec[0x19], e1);
        CHECK_BITS(rec[0x1a], e2);
        CHECK_BITS(rec[0x1f], S);
        CHECK_BITS(rec[0x20], 1.0);                 // function 1 allocated with length 0
        CHECK_EQ(iifnc[2], 1);
        CHECK_BITS(llfnc[0], S);
    }
}

TEST_CASE(record_3d)
{
    reset_hooks();
    Model md(3);
    Node nd(md.m);
    Cls_1401f46e0* io = nd.io;
    io->u170 = 2;
    io->u174 = 0;                                   // -> r1 = 1.0
    io->u178 = 2;
    io->u180 = std::bit_cast<std::uint64_t>(0.5);
    field<std::int32_t>(io, 0x0) = 4;
    field<std::int32_t>(io, 0x10) = 5;              // set 1: 3-variable dims
    field<std::int32_t>(io, 0x34) = 2;
    field<std::int32_t>(io, 0x38) = 3;
    field<std::int32_t>(io, 0x3c) = 4;
    field<std::int32_t>(io, 0x14) = 6;              // set 2: pair 0x138 / 0x140
    field<double>(io, 0x138) = 7.5;
    field<double>(io, 0x140) = 8.5;
    io->u1b8.value = 6.0;
    io->u234 = 1;
    io->u230 = 5;
    io->u2b8.value = 0.75;
    io->u330 = 1;
    io->u334 = 3;
    io->u338 = std::bit_cast<std::uint64_t>(0.125);
    io->u340 = 2;
    io->u348[4] = std::bit_cast<std::uint64_t>(-1.5);   // 0x368
    io->u348[5] = std::bit_cast<std::uint64_t>(-2.5);   // 0x370
    io->u37c = 1;
    io->u380 = 2;
    io->u384 = 3;
    io->u388 = std::bit_cast<std::uint64_t>(4.5);
    Raw w1(0x848);
    field<void*>(io, 0x188) = w1.p();
    field<std::int32_t>(io, 0x188 + 0x14) = 1;
    std::int32_t objid = -1;
    double* winrec = nullptr;
    std::vector<double> llwin(400, 0.0);
    g_fakes[0x140260ea0] = [&](const std::uint64_t* a, int) -> std::uint64_t {
        winrec = ptr<double>(a[1]);
        objid = static_cast<std::int32_t>(a[9]);
        *ptr<std::int32_t>(a[5]) += 1;
        *ptr<std::int32_t>(a[6]) += 8;
        return 0;
    };
    std::vector<double> rec(50, S);
    std::vector<std::int32_t> iifnc(40, 0);
    std::vector<double> llfnc(100, S);
    const std::int32_t i = 1, j = 2, wwin = 20;
    write_interobj_record(io, &i, &j, rec.data(), iifnc.data(), llfnc.data(), llwin.data(), nullptr, &wwin);
    CHECK(g_unexpected.empty());
    CHECK_BITS(rec[0], 2.0);
    CHECK_BITS(rec[1], 1.0);
    CHECK_BITS(rec[2], 4.0);
    CHECK_BITS(rec[3], S);
    CHECK_BITS(rec[4], S);
    CHECK_BITS(rec[5], 5.0);
    CHECK_BITS(rec[6], 6.0);
    CHECK_BITS(rec[7], 0.0);
    CHECK_BITS(rec[8], -5.0);
    for (int k = 9; k <= 0xb; ++k) CHECK_BITS(rec[k], S);
    CHECK_BITS(rec[0xc], 2.0);
    CHECK_BITS(rec[0xd], 0.5);
    CHECK_BITS(rec[0xe], 2.0);
    CHECK_BITS(rec[0xf], 3.0);
    CHECK_BITS(rec[0x10], 4.0);
    CHECK_BITS(rec[0x11], S);
    CHECK_BITS(rec[0x12], S);
    CHECK_BITS(rec[0x13], 0.0);
    CHECK_BITS(rec[0x14], 0.75);
    for (int k = 0x15; k <= 0x1e; ++k) CHECK_BITS(rec[k], S);
    CHECK_BITS(rec[0x1f], 1.0);
    CHECK_BITS(rec[0x20], 3.0);
    CHECK_BITS(rec[0x21], 0.125);
    CHECK_BITS(rec[0x22], 2.0);
    CHECK_BITS(rec[0x23], 1.0);     // first function number
    CHECK_BITS(llfnc[0], -1.5);
    CHECK_BITS(llfnc[1], -2.5);
    CHECK_BITS(llfnc[2], S);
    CHECK(winrec == llwin.data() + 80);
    CHECK_EQ(objid, 0);             // 3D passes 0
    CHECK_BITS(rec[0x24], 5.0);
    CHECK_BITS(rec[0x25], 5.0);
    CHECK_EQ(field<std::int32_t>(md.m, 0x134), 6);
    CHECK_EQ(field<std::int32_t>(md.m, 0x138), 15);
    CHECK_BITS(rec[0x26], 1.0);
    CHECK_BITS(rec[0x27], 2.0);
    CHECK_BITS(rec[0x28], 3.0);
    CHECK_BITS(rec[0x29], 4.5);
    CHECK_BITS(rec[0x2a], S);       // set 2 code 6: value not written
    CHECK_BITS(rec[0x2b], S);
    CHECK_BITS(rec[0x2c], 6.0);
    CHECK_BITS(rec[0x2d], 7.5);
    CHECK_BITS(rec[0x2e], 8.5);
    CHECK_BITS(rec[0x2f], 0.0);
    CHECK_BITS(rec[0x30], S);
    CHECK_BITS(rec[0x31], S);
}

TEST_CASE(record_3d_option_wmodel_ref_curve_friction)
{
    reset_hooks();
    Model md(3);
    Node nd(md.m);
    Cls_1401f46e0* io = nd.io;
    io->u170 = 1;
    io->u174 = 1;                                   // -> r1 = 0
    io->u178 = 5;                                   // unknown SEPRES code: word left
    field<std::int32_t>(io, 0x0) = 1;
    field<std::int32_t>(io, 0x10) = 2;
    Fn1 f1;
    f1.attach(field<DYNARY>(io, 0x18), 4, 50.0);
    io->u340 = 3;
    io->u378 = 12;
    std::vector<double> rec(50, S);
    std::vector<std::int32_t> iifnc(40, 0);
    std::vector<double> llfnc(100, S);
    const std::int32_t i = 1, j = 2, wwin = 20;
    write_interobj_record(io, &i, &j, rec.data(), iifnc.data(), llfnc.data(), nullptr, nullptr, &wwin);
    CHECK_BITS(rec[0], 1.0);
    CHECK_BITS(rec[1], 0.0);
    CHECK_BITS(rec[0xc], S);
    CHECK_BITS(rec[5], 2.0);
    CHECK_BITS(rec[4], 1.0);
    CHECK_BITS(llfnc[0], 50.0);
    CHECK_BITS(llfnc[3], 51.5);
    CHECK_BITS(rec[0xe], 0.0);
    CHECK_BITS(rec[0x1f], 0.0);     // CNTMTH off
    CHECK_BITS(rec[0x20], S);
    CHECK_BITS(rec[0x22], -12.0);
    CHECK_BITS(rec[0x23], S);
    CHECK_BITS(rec[0x24], 0.0);     // no windows
    CHECK_BITS(rec[0x25], 0.0);
}

// ---- 14027b620 ------------------------------------------------------------------------------

TEST_CASE(nearest_point_curve_type1)
{
    OBJ_GEO2D g;
    g.curve_type = 1;               // dof 3: x, y, r
    g.n_points = 3;
    double* p = g.allocate_points();
    const double rows[9] = {10.0, 0.0, 9.0, 3.0, 4.0, 9.0, 4.0, 3.0, 1.0};
    std::memcpy(p, rows, sizeof rows);
    double d = S, px = S, py = S;
    CHECK(geo2d_nearest_point(&g, 0.0, 0.0, &d, &px, &py));
    // squared distances 100, 25 (3, 4), 25 (4, 3): the first wins (strict <), sqrt(25) = 5
    CHECK_BITS(d, 5.0);
    CHECK_BITS(px, 3.0);
    CHECK_BITS(py, 4.0);
    // exact hit: squared distance 0 -> *dist stays 0
    CHECK(geo2d_nearest_point(&g, 10.0, 0.0, &d, &px, &py));
    CHECK_BITS(d, 0.0);
    CHECK_BITS(px, 10.0);
}

TEST_CASE(nearest_point_curve_type2)
{
    OBJ_GEO2D g;
    g.curve_type = 2;               // dof 6: type, x1, y1, x2, y2, a
    g.n_points = 2;
    double* p = g.allocate_points();
    const double rows[12] = {1.0, 0.0, 10.0, 6.0, 8.0, 0.0, 2.0, 20.0, 0.0, 1.0, 1.0, 0.5};
    std::memcpy(p, rows, sizeof rows);
    double d = S, px = S, py = S;
    CHECK(geo2d_nearest_point(&g, 0.0, 0.0, &d, &px, &py));
    // candidates (0,10) 100, (6,8) 100, (20,0) 400, (1,1) 2 -> (1,1), sqrt(2)
    CHECK_BITS(d, std::sqrt(2.0));
    CHECK_BITS(px, 1.0);
    CHECK_BITS(py, 1.0);
    // (6, 9): (0,10) 37, (6,8) 1 -> (6,8), distance 1
    CHECK(geo2d_nearest_point(&g, 6.0, 9.0, &d, &px, &py));
    CHECK_BITS(d, 1.0);
    CHECK_BITS(px, 6.0);
    CHECK_BITS(py, 8.0);
    OBJ_GEO2D empty;
    empty.curve_type = 1;
    d = S;
    CHECK(!geo2d_nearest_point(&empty, 0.0, 0.0, &d, &px, &py));
    CHECK_BITS(d, 0.0);
}

// ---- 140166b40 / 140166d30 / 140166980 / 1401667e0 / 1401fa840 --------------------------------

namespace {

struct Obj {
    Raw raw{sizeof(LLOBJ_DATA)};
    LLOBJ_DATA* o = static_cast<LLOBJ_DATA*>(raw.p());
    Obj(std::int32_t id, std::int32_t first, std::int32_t last)
    {
        field<std::int32_t>(o, 8) = id;
        field<std::int32_t>(o, 0x3588) = first;
        field<std::int32_t>(o, 0x358c) = last;
    }
};

// meshed object: boundaries k with first node nodes[k] (1-based) and coordinates xy
struct Mesh {
    std::vector<double> xy;
    std::vector<std::int32_t> first;
    void attach(LLOBJ_DATA* o)
    {
        field<std::int32_t>(o, 0x87c) = 1;
        field<std::int32_t>(o, 0x880) = 1;
        field<std::int32_t>(o, 0xbd8) = static_cast<std::int32_t>(first.size());
        field<double*>(o, 0x8a8) = xy.data();
        for (std::size_t k = 0; k < first.size(); ++k) {
            field<std::int32_t*>(o, 0xbe0 + 0x30 * k + 8) = &first[k];
        }
    }
};

// rigid geometry hooks: curves per object
std::map<const void*, std::vector<OBJ_GEO2D*>> g_curves;

void install_rigid_hooks()
{
    g_fakes[0x140160cc0] = [](const std::uint64_t* a, int) -> std::uint64_t {
        return g_curves.count(ptr<void>(a[0])) ? 1 : 0;
    };
    g_fakes[0x140160d20] = g_fakes[0x140160cc0];
    g_fakes[0x140198310] = [](const std::uint64_t* a, int) -> std::uint64_t {
        return g_curves[ptr<void>(a[0])].size();
    };
    g_fakes[0x1401982c0] = [](const std::uint64_t* a, int) -> std::uint64_t {
        auto& v = g_curves[ptr<void>(a[0])];
        const std::int32_t k = *ptr<std::int32_t>(a[1]);
        return k < static_cast<std::int32_t>(v.size()) ? ext::u(v[k]) : 0;
    };
}

OBJ_GEO2D* points_curve(std::vector<std::unique_ptr<OBJ_GEO2D>>& keep, std::initializer_list<double> xy)
{
    keep.push_back(std::make_unique<OBJ_GEO2D>());
    OBJ_GEO2D* g = keep.back().get();
    g->curve_type = 1;
    g->n_points = static_cast<std::int32_t>(xy.size() / 2);
    double* p = g->allocate_points();
    std::int32_t i = 0;
    for (auto it = xy.begin(); it != xy.end(); it += 2, ++i) {
        p[3 * i] = *it;
        p[3 * i + 1] = *(it + 1);
    }
    return g;
}

} // namespace

TEST_CASE(nearest_boundary_rigid_and_meshed)
{
    reset_hooks();
    g_curves.clear();
    install_rigid_hooks();
    std::vector<std::unique_ptr<OBJ_GEO2D>> keep;
    Obj rigid(1, 4, 6);
    g_curves[rigid.o] = {points_curve(keep, {100.0, 0.0}), points_curve(keep, {1.0, 1.0}),
                         points_curve(keep, {1.0, 1.0})};
    std::int32_t b = -1;
    double d = S;
    // curve distances 97, sqrt(13), sqrt(13): curve 1 (strict <); boundary 4 + 1. The rigid branch's
    // nearest point stays (0, 0) (140166d30 does not write it): distance sqrt(3*3 + 4*4) = 5
    CHECK(nearest_boundary(rigid.o, 3.0, 4.0, &b, &d));
    CHECK_EQ(b, 5);
    CHECK_BITS(d, 5.0);
    // meshed: 140166e60 gives boundary 2 and the point (2, 3): distance sqrt(1 + 1)
    Obj meshed(2, 7, 9);
    field<std::int32_t>(meshed.o, 0x87c) = 1;
    g_fakes[0x140166e60] = [&](const std::uint64_t* a, int n) -> std::uint64_t {
        CHECK_EQ(n, 7);
        CHECK(ptr<void>(a[0]) == meshed.o);
        CHECK_BITS(dbl(a[1]), 3.0);
        CHECK_BITS(dbl(a[2]), 4.0);
        *ptr<std::int32_t>(a[3]) = 77;
        *ptr<std::int32_t>(a[4]) = 2;
        *ptr<double>(a[5]) = 2.0;
        *ptr<double>(a[6]) = 3.0;
        return 1;
    };
    CHECK(nearest_boundary(meshed.o, 3.0, 4.0, &b, &d));
    CHECK_EQ(b, 9);
    CHECK_BITS(d, std::sqrt(2.0));
    // no geometry: false, *bnd = 0, dist untouched
    Obj none(3, 1, 2);
    d = S;
    CHECK(!nearest_boundary(none.o, 3.0, 4.0, &b, &d));
    CHECK_EQ(b, 0);
    CHECK_BITS(d, S);
    CHECK(g_unexpected.empty());
}

TEST_CASE(boundary_first_point_cases)
{
    reset_hooks();
    g_curves.clear();
    install_rigid_hooks();
    std::vector<std::unique_ptr<OBJ_GEO2D>> keep;
    Obj rigid(1, 1, 2);
    g_curves[rigid.o] = {points_curve(keep, {2.0, 3.0, 4.0, 5.0})};
    double pt[2] = {S, S};
    const std::int32_t k1 = 1;
    boundary_first_point(rigid.o, pt, &k1);   // curve 0 regardless of idx
    CHECK_BITS(pt[0], 2.0);
    CHECK_BITS(pt[1], 3.0);
    keep.push_back(std::make_unique<OBJ_GEO2D>());
    OBJ_GEO2D* seg = keep.back().get();
    seg->curve_type = 2;
    seg->n_points = 1;
    double* p = seg->allocate_points();
    p[0] = 1.0;
    p[1] = 6.0;
    p[2] = 7.0;
    g_curves[rigid.o] = {seg};
    boundary_first_point(rigid.o, pt, &k1);   // segment rows: words 1, 2
    CHECK_BITS(pt[0], 6.0);
    CHECK_BITS(pt[1], 7.0);
    Obj none(2, 1, 1);
    boundary_first_point(none.o, pt, &k1);
    CHECK_BITS(pt[0], 0.0);
    CHECK_BITS(pt[1], 0.0);
    Obj meshed(3, 1, 2);
    Mesh mesh{{0.0, 0.0, 1.0, 2.0, 3.0, 4.0}, {3, 2}};
    mesh.attach(meshed.o);
    boundary_first_point(meshed.o, pt, &k1);  // boundary 1: node 2 -> (1, 2)
    CHECK_BITS(pt[0], 1.0);
    CHECK_BITS(pt[1], 2.0);
    const std::int32_t k2 = 2;
    pt[0] = S;
    boundary_first_point(meshed.o, pt, &k2);  // out of range: unchanged
    CHECK_BITS(pt[0], S);
    CHECK(g_unexpected.empty());
}

namespace {

// 140166e60 fake for a meshed slave: boundary 0 for x < 5 (point (x + 3, y)), else boundary 1
// (point (x + 1, y))
void install_mesh_search(const void* slave)
{
    g_fakes[0x140166e60] = [slave](const std::uint64_t* a, int) -> std::uint64_t {
        CHECK(ptr<void>(a[0]) == slave);
        const double x = dbl(a[1]);
        const bool left = x < 5.0;
        *ptr<std::int32_t>(a[4]) = left ? 0 : 1;
        *ptr<double>(a[5]) = left ? x + 3.0 : x + 1.0;
        *ptr<double>(a[6]) = dbl(a[2]);
        return 1;
    };
}

} // namespace

TEST_CASE(choose_boundary_records_branches)
{
    reset_hooks();
    g_curves.clear();
    install_rigid_hooks();
    std::vector<std::unique_ptr<OBJ_GEO2D>> keep;
    std::int32_t bm = -1, bs = -1;
    const std::int32_t flag1 = 1, flag2 = 2;
    // both single: direct
    {
        Obj a(1, 3, 3), b(2, 5, 5);
        choose_boundary_records(nullptr, a.o, b.o, &flag1, &bm, &bs, nullptr, nullptr);
        CHECK_EQ(bm, 3);
        CHECK_EQ(bs, 5);
    }
    // master single (rigid, first point (0, 0)), slave rigid with 2 curves: nearest curve 1
    {
        Obj a(1, 2, 2), b(2, 10, 11);
        g_curves[a.o] = {points_curve(keep, {0.0, 0.0})};
        g_curves[b.o] = {points_curve(keep, {9.0, 9.0}), points_curve(keep, {1.0, 0.0})};
        choose_boundary_records(nullptr, a.o, b.o, &flag1, &bm, &bs, nullptr, nullptr);
        CHECK_EQ(bm, 2);
        CHECK_EQ(bs, 11);
    }
    // slave single (rigid, first point (8, 8)), master rigid: nearest curve 0
    {
        Obj a(1, 20, 21), b(2, 4, 4);
        g_curves[b.o] = {points_curve(keep, {8.0, 8.0})};
        g_curves[a.o] = {points_curve(keep, {9.0, 9.0}), points_curve(keep, {1.0, 0.0})};
        choose_boundary_records(nullptr, a.o, b.o, &flag2, &bm, &bs, nullptr, nullptr);
        CHECK_EQ(bm, 20);
        CHECK_EQ(bs, 4);
    }
    // both multiple, flag 1: master meshed, boundary k first points (0, 0) and (10, 0);
    // slave search: k 0 -> slave boundary 0 at distance 3, k 1 -> boundary 1 at distance 1
    {
        Obj a(1, 30, 31), b(2, 40, 41);
        Mesh mesh{{0.0, 0.0, 10.0, 0.0}, {1, 2}};
        mesh.attach(a.o);
        field<std::int32_t>(b.o, 0x87c) = 1;
        install_mesh_search(b.o);
        choose_boundary_records(nullptr, a.o, b.o, &flag1, &bm, &bs, nullptr, nullptr);
        CHECK_EQ(bm, 31);
        CHECK_EQ(bs, 41);
    }
    // both multiple, flag 2: the search runs over the slave's boundaries against the master
    {
        Obj a(1, 30, 31), b(2, 40, 41);
        Mesh mesh{{0.0, 0.0, 10.0, 0.0}, {2, 1}};   // slave k 0 at (10, 0), k 1 at (0, 0)
        mesh.attach(b.o);
        field<std::int32_t>(a.o, 0x87c) = 1;
        install_mesh_search(a.o);
        choose_boundary_records(nullptr, a.o, b.o, &flag2, &bm, &bs, nullptr, nullptr);
        CHECK_EQ(bs, 40);   // slave boundary k 0 (distance 1)
        CHECK_EQ(bm, 31);   // master boundary 1
    }
    // both multiple, searching object not meshed: nothing found
    {
        Obj a(1, 30, 31), b(2, 40, 41);
        choose_boundary_records(nullptr, a.o, b.o, &flag1, &bm, &bs, nullptr, nullptr);
        CHECK_EQ(bm, 0);
        CHECK_EQ(bs, 0);
    }
    CHECK(g_unexpected.empty());
}

TEST_CASE(nearest_boundary_pair_ties_keep_first)
{
    reset_hooks();
    Obj a(1, 30, 32), b(2, 40, 41);
    Mesh mesh{{0.0, 0.0, 1.0, 0.0, 2.0, 0.0}, {1, 2, 3}};
    mesh.attach(a.o);
    field<std::int32_t>(b.o, 0x87c) = 1;
    // every point lies at distance 3 (x < 5): the first boundary is kept
    install_mesh_search(b.o);
    std::int32_t ia = -1, ib = -1;
    CHECK(nearest_boundary_pair(a.o, b.o, &ia, &ib));
    CHECK_EQ(ia, 30);
    CHECK_EQ(ib, 40);
    field<std::int32_t>(a.o, 0x880) = 0;
    ia = -1;
    CHECK(!nearest_boundary_pair(a.o, b.o, &ia, &ib));
    CHECK_EQ(ia, -1);
}

// ---- 1401fa4f0 ------------------------------------------------------------------------------

TEST_CASE(list_writer_records_and_mirror)
{
    reset_hooks();
    Model md(3);
    DB_STEP_DATA* m = md.m;
    // objects 1, 2 (one boundary each: 1, 2) and 3 (empty slot, type 7, boundary 3)
    Obj o1(1, 1, 1), o2(2, 2, 2), o3(3, 3, 3);
    field<std::int32_t>(o3.o, 0xc) = 7;
    field<LLOBJ_DATA*>(m, 0x1380) = o1.o;
    field<LLOBJ_DATA*>(o1.o, 0x3940) = o2.o;
    field<LLOBJ_DATA*>(o2.o, 0x3940) = o3.o;
    field<std::int32_t>(m, 0x7c) = 3;
    // nodes (1, 1) flag 0, (1, 2) flag 1, (1, 3) flag 1 (empty object: skipped), (2, 2) flag 2
    Node n11(m), n12(m), n13(m), n22(m);
    n11.io->u168 = 1; n11.io->u16c = 1; n11.io->u170 = 0;
    n12.io->u168 = 1; n12.io->u16c = 2; n12.io->u170 = 1;
    n13.io->u168 = 1; n13.io->u16c = 3; n13.io->u170 = 1;
    n22.io->u168 = 2; n22.io->u16c = 2; n22.io->u170 = 2;
    n12.io->u2b8.value = 0.5;       // INTRST constant -> r[0x14]
    n12.io->u174 = 1;               // r1 = 0
    n22.io->u174 = 0;
    Cls_1401fa020* list = &m->inter_obj;
    list->owner = m;
    list->head = n11.io;
    n11.io->next = n12.io;
    n12.io->next = n13.io;
    n13.io->next = n22.io;
    const std::int32_t nbnd = 3, wbrt = 50, wwin = 20;
    std::vector<double> llbrt(9 * 50, 0.0);
    std::vector<std::int32_t> iifnc(40, 0);
    std::vector<double> llfnc(100, 0.0);
    write_interobj_list(list, llbrt.data(), &nbnd, iifnc.data(), llfnc.data(), nullptr, nullptr, &wbrt, &wwin,
                        nullptr, nullptr);
    CHECK(g_unexpected.empty());
    // rec(bm, bs) = 50 * ((bs - 1) * 3 + bm - 1)
    const double* r12 = llbrt.data() + 50 * 3;    // (1, 2)
    const double* r21 = llbrt.data() + 50 * 1;    // mirror (2, 1)
    const double* r22 = llbrt.data() + 50 * 4;    // (2, 2): no mirror (bm == bs)
    CHECK_BITS(r12[0], 1.0);
    CHECK_BITS(r12[1], 0.0);
    CHECK_BITS(r12[0x14], 0.5);
    CHECK_BITS(r21[0], 2.0);        // 1.0 <-> 2.0
    for (int k = 1; k < 50; ++k) CHECK_BITS(r21[k], r12[k]);
    CHECK_BITS(r22[0], 2.0);
    CHECK_BITS(r22[1], 1.0);
    // (1, 1) has flag 0 and (1, 3) / (3, 1) belong to the empty object: still zero
    for (int k = 0; k < 50; ++k) {
        CHECK_BITS(llbrt[k], 0.0);
        CHECK_BITS(llbrt[50 * 6 + k], 0.0);
        CHECK_BITS(llbrt[50 * 2 + k], 0.0);
    }
}

int main() { return testkit::run_all(); }
