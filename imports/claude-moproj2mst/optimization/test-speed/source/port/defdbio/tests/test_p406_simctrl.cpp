// Tier 1 for packet p4-06, group simctrl: the simulation-control GENDB writer 1401effc0, the
// self-describing FUNCTION_DATA writer 140131600, the furnace record writer 140375bb0, the PDIE
// magnitude 1401f3bb0 / 14021e170 and the DB_WRITE_VAR getters.
//
// Models are zeroed storage of DB_STEP_DATA with the fields the writer reads set by hand; the
// dimension word m+0x1c selects the branch (1400015e0). Expected words are read off the decompile
// of 1401effc0 (word index = byte offset / 4 or / 8 in its param_2[] / param_3[] stores) and the
// listing for the DTSUB chain; IIFNC/LLFNC are worked out by hand from FNCLOC (1401bb3d0): id k
// starts at IIFNC[2k-2], attribute in IIFNC[2k-1], next start IIFNC[2k] = start + len.
// ctest-labels: p4-06 gendb
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <defdbio/gendb/p4_06_simctrl.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <testkit/check.hpp>

#include <cmath>
#include <cstring>
#include <limits>
#include <string>
#include <vector>

using namespace def::model;
using namespace def::dbio::gendb;

namespace {

struct Fx {
    std::vector<std::uint64_t> mbuf;
    std::vector<std::int32_t> iisim = std::vector<std::int32_t>(100, -99);
    std::vector<double> llsim = std::vector<double>(100, -99.0);
    std::vector<std::int32_t> iiuvl = std::vector<std::int32_t>(500, 0);
    std::vector<std::int32_t> iifnc = std::vector<std::int32_t>(100, -99);
    std::vector<double> llfnc = std::vector<double>(400, -99.0);
    DB_STEP_DATA* m;
    Cls_1401e7b10* s;

    explicit Fx(std::int32_t dim) : mbuf(sizeof(DB_STEP_DATA) / 8 + 1, 0)
    {
        m = reinterpret_cast<DB_STEP_DATA*>(mbuf.data());
        field<std::int32_t>(m, 0x1c) = dim;
        s = &m->sim_ctrl;
        s->owner = m;
        field<std::int32_t>(m, 0x88) = 50;  // NFN (FNCLOC warning limit)
        field<std::int32_t>(m, 0x8c) = 400; // LLFNC length
    }
    void run() { write_sim_ctrl_to_db(s, iisim.data(), llsim.data(), iiuvl.data(), iifnc.data(), llfnc.data()); }
    std::int32_t& w(std::size_t off) { return field<std::int32_t>(m, off); }
};

// zeroed DYNARY storage (the class ctor is not needed: the writers only read fields)
struct Dyn {
    alignas(8) std::byte raw[sizeof(DYNARY)] = {};
    DYNARY* operator->() { return reinterpret_cast<DYNARY*>(raw); }
    DYNARY* get() { return reinterpret_cast<DYNARY*>(raw); }
};

// curve shape: ndim 1, n points, ncomp 1
void curve(DYNARY* f, std::int32_t type, std::int32_t xvar, std::vector<double>& x, std::vector<double>& y)
{
    f->type = type;
    f->ndim = 1;
    f->var[0] = xvar;
    f->n[0] = static_cast<std::int32_t>(x.size());
    f->ncomp = 1;
    f->axis[0] = x.data();
    f->values = y.data();
}

void fill_counts(Fx& fx)
{
    // DB_WRITE_VAR counts 0x68..0xc0 with distinctive values (0x88 / 0x8c keep 50 / 400)
    fx.w(0x68) = 168;
    fx.w(0x6c) = 169;
    fx.w(0x70) = 170;
    fx.w(0x74) = 174;
    fx.w(0x78) = 178;
    fx.w(0x7c) = 17;
    fx.w(0x80) = 180;
    fx.w(0x84) = 184;
    fx.w(0x90) = 190;
    fx.w(0x94) = 194;
    fx.w(0x98) = 198;
    fx.w(0x9c) = 199;
    fx.w(0xbc) = 211;
    fx.w(0xc0) = 212;
}

// Sets every scalar the writer copies: doubles to 1000 + their S offset / 8 (so each is
// distinct), ints to 500 + offset / 4.
void fill_scalars(Cls_1401e7b10* s)
{
    auto* b = reinterpret_cast<std::byte*>(s);
    auto d = [b](std::size_t off) { field<double>(b, off) = 1000.0 + static_cast<double>(off / 8); };
    auto i = [b](std::size_t off) { field<std::int32_t>(b, off) = 500 + static_cast<std::int32_t>(off / 4); };
    for (std::size_t off : {0x170, 0x178, 0x180, 0x188, 0x190, 0x198, 0x1a0, 0x1a8, 0x1b0, 0x1b8, 0x1c0, 0x1d0, 0x1d8,
                            0x1e0, 0x260, 0x268, 0x270, 0x278, 0x280, 0x298, 0x2a0, 0x2a8, 0x2b0, 0x2b8, 0x2c8, 0x2d0,
                            0x2d8, 0x3a8, 0x3b0, 0x3b8, 0x4b8, 0x628, 0x630, 0x638, 0x640, 0x648, 0x658, 0x660, 0x668,
                            0x670, 0x678, 0x680, 0x9d0, 0xbc8}) {
        d(off);
    }
    for (std::size_t off : {0xf0, 0xf4, 0xf8, 0xfc, 0x100, 0x104, 0x108, 0x10c, 0x110, 0x114, 0x118, 0x160, 0x164,
                            0x168, 0x16c, 0x1c8, 0x288, 0x28c, 0x290, 0x2e0, 0x310, 0x314, 0x318, 0x31c, 0x320, 0x324,
                            0x650, 0x9b4, 0x9b8, 0x9bc, 0x9c0, 0x9c4, 0x9c8}) {
        i(off);
    }
    for (std::size_t k = 0; k < 7; ++k) {
        i(0x11c + 4 * k);
    }
    for (std::size_t k = 0; k < 10; ++k) {
        i(0x138 + 4 * k);
    }
    s->envtmp.value = 77.5;
    s->cnvcof.value = 78.5;
    s->envatm.value = 79.5;
    s->acvcof.value = 80.5;
    s->dtsub = 0.25;
    s->vfactr_mode = 0;
}

double D(std::size_t off) { return 1000.0 + static_cast<double>(off / 8); }
std::int32_t N(std::size_t off) { return 500 + static_cast<std::int32_t>(off / 4); }

} // namespace

TEST_CASE(getters_read_db_write_var)
{
    Fx fx(3);
    fill_counts(fx);
    CHECK_EQ(dbw_u068(fx.m), 168);
    CHECK_EQ(dbw_u0bc(fx.m), 211);
    CHECK_EQ(dbw_u0c0(fx.m), 212);
    CHECK_EQ(dbw_u074(fx.m), 174);
    CHECK_EQ(dbw_u084(fx.m), 184);
    CHECK_EQ(dbw_u088(fx.m), 50);
    CHECK_EQ(dbw_u08c(fx.m), 400);
    CHECK_EQ(dbw_u090(fx.m), 190);
    CHECK_EQ(dbw_u094(fx.m), 194);
    CHECK_EQ(dbw_u09c(fx.m), 199);
}

TEST_CASE(sim_ctrl_3d_word_positions)
{
    Fx fx(3);
    fill_counts(fx);
    fill_scalars(fx.s);
    fx.s->solmtd = 2;
    fx.s->solmtd_p[0] = 2.9;
    fx.s->solmtd_p[1] = -3.7;
    fx.s->solmtd_p[2] = 4.0;
    fx.s->solmtd_p[3] = 5.5;
    fx.run();
    const auto& L = fx.llsim;
    const auto& I = fx.iisim;
    // LLSIM, 3D branch of 1401effc0
    const std::pair<int, std::size_t> lw[] = {
        {0, 0x640},  {3, 0x1d0},  {4, 0x268},  {5, 0x260},  {6, 0x1e0},  {7, 0x278},  {8, 0x280},  {9, 0x178},
        {10, 0x170}, {11, 0x180}, {12, 0x188}, {13, 0x190}, {14, 0x198}, {15, 0x1a0}, {16, 0x1a8}, {17, 0x1b0},
        {18, 0x1b8}, {19, 0x1c0}, {20, 0x2d0}, {21, 0x2d8}, {22, 0x3a8}, {23, 0x660}, {24, 0x668}, {59, 0x670},
        {60, 0x678}, {26, 0x3b8}, {27, 0x3b0}, {32, 0x4b8}, {37, 0x298}, {38, 0x270}, {39, 0x2a0}, {40, 0x2a8},
        {48, 0x628}, {49, 0x630}, {53, 0x638}, {54, 0xbc8}, {55, 0x1d8}, {50, 0x648}, {56, 0x658}, {52, 0x2c8}};
    for (const auto& [word, off] : lw) {
        CHECK_BITS(L[word], D(off));
    }
    CHECK_BITS(L[2], 0.0);                                  // no PDIE object
    CHECK_BITS(L[34], static_cast<double>(N(0x288)));       // MDSOBJ 1..3 as reals
    CHECK_BITS(L[35], static_cast<double>(N(0x28c)));
    CHECK_BITS(L[36], static_cast<double>(N(0x290)));
    CHECK_BITS(L[51], static_cast<double>(N(0x650)));       // TNOW 3
    CHECK_BITS(L[45], 1.0);
    CHECK_BITS(L[46], 2.0);
    CHECK_BITS(L[47], 3.0);
    CHECK_BITS(L[57], 0.0);                                 // no furnace
    CHECK_BITS(L[28], 0.0);                                 // ENVTMP constant
    CHECK_BITS(L[29], 77.5);
    CHECK_BITS(L[30], 0.0);
    CHECK_BITS(L[31], 78.5);
    CHECK_BITS(L[41], 0.0);
    CHECK_BITS(L[42], 79.5);
    CHECK_BITS(L[43], 0.0);
    CHECK_BITS(L[44], 80.5);
    CHECK_BITS(L[33], 0.25);                                // DTSUB in [0, 0.5) kept
    CHECK_BITS(L[1], -99.0);                                // not written
    CHECK_BITS(L[25], -99.0);
    CHECK_BITS(L[61], -99.0);                               // SOLMTD 2: params go to IISIM
    // IISIM
    const std::pair<int, std::size_t> iw[] = {
        {0, 0xf4},   {1, 0xf0},   {2, 0x160},  {4, 0x16c},  {5, 0x164},  {6, 0x1c8},  {7, 0x168},
        {35, 0x9b4}, {9, 0x2e0},  {10, 0x310}, {11, 0x314}, {13, 0x324}, {15, 0x318}, {16, 0x31c},
        {17, 0x320}, {18, 0xfc},  {43, 0x118}, {44, 0x104}, {45, 0x108}, {46, 0x10c}, {47, 0x110},
        {48, 0x114}, {49, 0xf8}};
    for (const auto& [word, off] : iw) {
        CHECK_EQ(I[word], N(off));
    }
    for (int k = 0; k < 7; ++k) {
        CHECK_EQ(I[50 + k], N(0x11c + 4 * k));
    }
    for (int k = 0; k < 10; ++k) {
        CHECK_EQ(I[57 + k], N(0x138 + 4 * k));
    }
    CHECK_EQ(I[8], 0);
    CHECK_EQ(I[12], 0);
    CHECK_EQ(I[14], 0);
    // model counts
    const std::pair<int, std::int32_t> cw[] = {{3, 168},  {19, 169}, {20, 170}, {21, 174}, {22, 178}, {23, 17},
                                               {24, 180}, {25, 184}, {26, 50},  {27, 400}, {28, 190}, {29, 194},
                                               {30, 198}, {31, 199}, {33, 211}, {34, 212}};
    for (const auto& [word, v] : cw) {
        CHECK_EQ(I[word], v);
    }
    CHECK_EQ(I[39], -99); // 3D writes NoWindows in I[34] only
    CHECK_EQ(I[32], 2);   // SOLMTD
    CHECK_EQ(I[72], 2);   // (int)2.9, truncation
    CHECK_EQ(I[73], -3);  // (int)-3.7
    CHECK_EQ(I[74], 4);
    CHECK_EQ(I[75], 5);
    CHECK_EQ(I[67], 0);   // no DTTYPE function
    CHECK_EQ(I[68], 0);
    CHECK_EQ(I[71], 0);   // TIMEID/ERECID/NRECID without points
    CHECK_EQ(I[83], 0);
    CHECK_EQ(I[84], 0);
    CHECK_EQ(I[76], 0);   // FRQNCY 0 x 0
    CHECK_EQ(I[77], 0);
    CHECK_EQ(I[78], 0);
    CHECK_EQ(I[80], 0);   // VFACTR mode
    CHECK_EQ(I[81], -99);
    CHECK_EQ(I[82], -99);
    // copies into the model (3D only)
    CHECK_EQ(fx.w(0x1378), 17);
    CHECK_EQ(fx.w(0x13f0), 180);
    CHECK_EQ(fx.w(0x13f4), 211);
    CHECK_EQ(fx.w(0x13e8), 169);
    CHECK_EQ(fx.w(0x13ec), 170);
    CHECK_EQ(fx.w(0x13fc), 50);
    CHECK_EQ(fx.w(0x10c), 0); // no function allocated
}

TEST_CASE(sim_ctrl_2d_word_positions)
{
    Fx fx(2);
    fill_counts(fx);
    fill_scalars(fx.s);
    fx.s->solmtd = 6;
    fx.s->solmtd_p[0] = 2.9;
    fx.s->solmtd_p[1] = -3.7;
    fx.s->solmtd_p[2] = 4.0;
    fx.s->solmtd_p[3] = 5.5;
    fx.s->vfactr_mode = 1;
    fx.s->vfactr_n = 3; // 2D does not write the VFACTR list
    fx.run();
    const auto& L = fx.llsim;
    const auto& I = fx.iisim;
    const std::pair<int, std::size_t> lw[] = {
        {0, 0x640},  {42, 0x648}, {39, 0x658}, {2, 0x1d0},  {9, 0x1d8},  {3, 0x268},  {4, 0x260},  {5, 0x1e0},
        {6, 0x270},  {26, 0x2a0}, {27, 0x2a8}, {7, 0x2d0},  {8, 0x2d8},  {10, 0x3a8}, {12, 0x660}, {13, 0x668},
        {50, 0x670}, {51, 0x678}, {14, 0x680}, {15, 0x3b8}, {16, 0x3b0}, {20, 0x170}, {21, 0x178}, {22, 0x4b8},
        {24, 0x278}, {25, 0x280}, {31, 0x298}, {40, 0x628}, {41, 0x630}, {46, 0x638}, {45, 0x9d0}, {47, 0x2b0},
        {48, 0x2b8}};
    for (const auto& [word, off] : lw) {
        CHECK_BITS(L[word], D(off));
    }
    CHECK_BITS(L[43], static_cast<double>(N(0x650)));
    CHECK_BITS(L[28], static_cast<double>(N(0x288)));
    CHECK_BITS(L[29], static_cast<double>(N(0x28c)));
    CHECK_BITS(L[30], static_cast<double>(N(0x290)));
    CHECK_BITS(L[44], static_cast<double>(N(0x9c8)));
    CHECK_BITS(L[32], 1.0);
    CHECK_BITS(L[33], 2.0);
    CHECK_BITS(L[34], 3.0);
    CHECK_BITS(L[23], 0.0);
    CHECK_BITS(L[17], 77.5);
    CHECK_BITS(L[18], 0.0);
    CHECK_BITS(L[19], 78.5);
    CHECK_BITS(L[35], 0.0);
    CHECK_BITS(L[36], 79.5);
    CHECK_BITS(L[37], 0.0);
    CHECK_BITS(L[38], 80.5);
    CHECK_BITS(L[49], 0.25);
    CHECK_BITS(L[61], 2.9); // SOLMTD 6: reals
    CHECK_BITS(L[62], -3.7);
    CHECK_BITS(L[63], 4.0);
    CHECK_BITS(L[64], 5.5);
    CHECK_BITS(L[1], -99.0);
    CHECK_BITS(L[11], -99.0);
    CHECK_BITS(L[57], -99.0);
    const std::pair<int, std::size_t> iw[] = {
        {0, 0xf4},   {1, 0xf8},   {2, 0x100},  {3, 0xf0},   {4, 0x160},  {6, 0x16c},  {7, 0x164},  {8, 0x1c8},
        {9, 0x168},  {16, 0x2e0}, {17, 0x310}, {19, 0x314}, {22, 0x318}, {23, 0x31c}, {24, 0x320}, {25, 0xfc},
        {36, 0x9b4}, {40, 0x324}, {41, 0x9b8}, {42, 0x9bc}, {43, 0x118}, {44, 0x104}, {45, 0x108}, {46, 0x10c},
        {47, 0x110}, {50, 0x114}, {48, 0x9c0}, {49, 0x9c4}};
    for (const auto& [word, off] : iw) {
        CHECK_EQ(I[word], N(off));
    }
    for (int k = 0; k < 7; ++k) {
        CHECK_EQ(I[52 + k], N(0x11c + 4 * k));
    }
    for (int k = 0; k < 10; ++k) {
        CHECK_EQ(I[59 + k], N(0x138 + 4 * k));
    }
    const std::pair<int, std::int32_t> cw[] = {{5, 168},  {26, 169}, {27, 170}, {28, 174}, {29, 178}, {30, 17},
                                               {31, 180}, {32, 184}, {33, 50},  {34, 400}, {37, 211}, {39, 212}};
    for (const auto& [word, v] : cw) {
        CHECK_EQ(I[word], v);
    }
    CHECK_EQ(I[11], 0);
    CHECK_EQ(I[12], 1);
    CHECK_EQ(I[13], 1);
    CHECK_EQ(I[18], 1);
    CHECK_EQ(I[15], 6);
    CHECK_EQ(I[72], -99); // SOLMTD 6 writes no ints
    CHECK_EQ(I[51], 1);   // VFACTR mode
    CHECK_EQ(I[81], -99); // no VFACTR list in 2D
    CHECK_EQ(I[82], -99);
    CHECK_EQ(I[69], 0);
    CHECK_EQ(I[70], 0);
    for (int k : {10, 14, 20, 21, 35, 38, 79, 85}) {
        CHECK_EQ(I[k], -99);
    }
    CHECK_EQ(fx.w(0x1378), 0); // the 2D branch copies no counts
    CHECK_EQ(fx.w(0x13fc), 0);
}

TEST_CASE(dttype_code_mapping)
{
    // (mode S+0x240, xvar S+0x1f8) -> code: mode 0: 1->1, 2->2, other->1; mode 1: 1->3, 2->4, other->1
    const int cases[][3] = {{0, 1, 1}, {0, 2, 2}, {0, 5, 1}, {1, 1, 3}, {1, 2, 4}, {1, 7, 1}};
    for (std::int32_t dim : {3, 2}) {
        for (const auto& c : cases) {
            Fx fx(dim);
            std::vector<double> x = {0.0, 1.0}, y = {0.5, 0.25};
            curve(&fx.s->dttype, 0x80, c[1], x, y);
            fx.s->dttype.u58 = c[0];
            fx.run();
            const int w = dim == 3 ? 67 : 69;
            CHECK_EQ(fx.iisim[w], c[2]);
            CHECK_EQ(fx.iisim[w + 1], 1); // first function
            // len = 2 * npts * ncomp = 4: IIFNC = {1, 0, 5}; data interleaved x, y
            CHECK_EQ(fx.iifnc[0], 1);
            CHECK_EQ(fx.iifnc[1], 0);
            CHECK_EQ(fx.iifnc[2], 5);
            CHECK_BITS(fx.llfnc[0], 0.0);
            CHECK_BITS(fx.llfnc[1], 0.5);
            CHECK_BITS(fx.llfnc[2], 1.0);
            CHECK_BITS(fx.llfnc[3], 0.25);
        }
    }
}

TEST_CASE(dtsub_clamp_cases)
{
    // listing 1401f1f49 (3D) / 1401f1429 (2D): [0, 0.5) kept; [0.5, 1] -> 1; == 2 -> 2;
    // otherwise 1.0 (3D) or 0.0 (2D); NaN fails every COMISD and takes "otherwise".
    const double nan = std::numeric_limits<double>::quiet_NaN();
    const double in[] = {-1.0, 0.3, 0.7, 2.0, 5.0, 0.0, 0.5, 1.0, nan};
    const double out3[] = {1.0, 0.3, 1.0, 2.0, 1.0, 0.0, 1.0, 1.0, 1.0};
    const double out2[] = {0.0, 0.3, 1.0, 2.0, 0.0, 0.0, 1.0, 1.0, 0.0};
    for (int k = 0; k < 9; ++k) {
        Fx f3(3);
        f3.s->dtsub = in[k];
        f3.run();
        CHECK_BITS(f3.llsim[33], out3[k]);
        CHECK_BITS(f3.s->dtsub, out3[k]);
        Fx f2(2);
        f2.s->dtsub = in[k];
        f2.run();
        CHECK_BITS(f2.llsim[49], out2[k]);
        CHECK_BITS(f2.s->dtsub, out2[k]);
    }
}

TEST_CASE(curves_allocated_in_order_3d)
{
    Fx fx(3);
    std::vector<double> ex = {0.0, 10.0}, ey = {20.0, 30.0};
    std::vector<double> cx = {5.0}, cy = {6.0};
    std::vector<double> ax = {1.0, 2.0, 3.0}, ay = {7.0, 8.0, 9.0};
    std::vector<double> dx = {0.0, 4.0}, dy = {0.1, 0.2};
    curve(&fx.s->envtmp, 0x7d, 1, ex, ey);
    curve(&fx.s->cnvcof, 0x7e, 3, cx, cy);
    curve(&fx.s->acvcof, 0x7f, 12, ax, ay);
    curve(&fx.s->dttype, 0x80, 2, dx, dy);
    fx.run();
    // order: ENVTMP (len 4), CNVCOF (2), ACVCOF (6), DTTYPE (4)
    const std::int32_t iifnc[] = {1, 0, 5, 0, 7, 0, 13, 0, 17};
    for (int k = 0; k < 9; ++k) {
        CHECK_EQ(fx.iifnc[k], iifnc[k]);
    }
    CHECK_EQ(fx.iifnc[9], -99);
    const double llfnc[] = {0.0, 20.0, 10.0, 30.0, 5.0, 6.0, 1.0, 7.0, 2.0, 8.0, 3.0, 9.0, 0.0, 0.1, 4.0, 0.2};
    for (int k = 0; k < 16; ++k) {
        CHECK_BITS(fx.llfnc[k], llfnc[k]);
    }
    CHECK_BITS(fx.llfnc[16], -99.0);
    CHECK_BITS(fx.llsim[28], 1.0);  // ENVTMP fid
    CHECK_BITS(fx.llsim[29], -99.0); // constant not written with a curve
    CHECK_BITS(fx.llsim[30], 2.0);  // CNVCOF fid
    CHECK_BITS(fx.llsim[43], 2.0);  // ACVCOF type from xvar 12
    CHECK_BITS(fx.llsim[44], 3.0);  // ACVCOF fid
    CHECK_BITS(fx.llsim[41], 0.0);  // ENVATM constant flag
    CHECK_EQ(fx.iisim[67], 2);      // DTTYPE mode 0, xvar 2
    CHECK_EQ(fx.iisim[68], 4);
    CHECK_EQ(fx.w(0x10c), 4);
}

TEST_CASE(curves_allocated_in_order_2d)
{
    Fx fx(2);
    std::vector<double> dx = {0.0, 4.0}, dy = {0.1, 0.2};
    std::vector<double> ex = {0.0, 10.0}, ey = {20.0, 30.0};
    std::vector<double> vx = {1.5}, vy = {2.5};
    curve(&fx.s->dttype, 0x80, 1, dx, dy);
    fx.s->dttype.u58 = 1;
    curve(&fx.s->envtmp, 0x7d, 1, ex, ey);
    curve(&fx.s->envatm, 0x7e, 1, vx, vy);
    fx.run();
    // 2D order: DTTYPE (4), ENVTMP (4), ENVATM (2)
    const std::int32_t iifnc[] = {1, 0, 5, 0, 9, 0, 11};
    for (int k = 0; k < 7; ++k) {
        CHECK_EQ(fx.iifnc[k], iifnc[k]);
    }
    const double llfnc[] = {0.0, 0.1, 4.0, 0.2, 0.0, 20.0, 10.0, 30.0, 1.5, 2.5};
    for (int k = 0; k < 10; ++k) {
        CHECK_BITS(fx.llfnc[k], llfnc[k]);
    }
    CHECK_EQ(fx.iisim[69], 3);
    CHECK_EQ(fx.iisim[70], 1);
    CHECK_BITS(fx.llsim[23], 2.0); // ENVTMP fid
    CHECK_BITS(fx.llsim[17], -99.0);
    CHECK_BITS(fx.llsim[35], 1.0); // ENVATM flag
    CHECK_BITS(fx.llsim[36], 3.0); // ENVATM fid
}

TEST_CASE(timeid_self_describing_record)
{
    for (std::int32_t dim : {3, 2}) {
        Fx fx(dim);
        std::vector<double> x = {1.0, 2.0, 3.0}, y = {4.0, 5.0, 6.0};
        curve(&fx.s->timeid, 0xb0, 1, x, y);
        fx.s->erecid.n[0] = 0; // no points: id 0, nothing allocated
        fx.run();
        // 140130380: 140130230 = 3 * (1 + 1) = 6 (even) + 10 = 16
        CHECK_EQ(fx.iisim[71], 1);
        CHECK_EQ(fx.iisim[83], 0);
        CHECK_EQ(fx.iisim[84], 0);
        CHECK_EQ(fx.iifnc[0], 1);
        CHECK_EQ(fx.iifnc[1], 0);
        CHECK_EQ(fx.iifnc[2], 17);
        const double head[] = {10.0, 1.0, 1.0, 3.0, 1.0};
        for (int k = 0; k < 5; ++k) {
            CHECK_BITS(fx.llfnc[k], head[k]);
        }
        for (int k = 5; k < 10; ++k) {
            CHECK_BITS(fx.llfnc[k], -99.0); // header words 5..9 untouched for ndim 1
        }
        const double data[] = {1.0, 4.0, 2.0, 5.0, 3.0, 6.0};
        for (int k = 0; k < 6; ++k) {
            CHECK_BITS(fx.llfnc[10 + k], data[k]);
        }
    }
}

TEST_CASE(selfdesc_other_shapes)
{
    std::vector<double> out(40, -99.0);
    Dyn f0;
    f0->ndim = 0;
    f0->ncomp = 3;
    f0->value = 4.5;
    write_func_selfdesc(f0.get(), out.data());
    CHECK_BITS(out[0], 10.0);
    CHECK_BITS(out[1], 0.0);
    CHECK_BITS(out[2], 3.0);
    CHECK_BITS(out[3], 4.5);
    CHECK_BITS(out[4], -99.0);
    // ndim 1 with no points: word 4 is the constant instead of ncomp
    std::fill(out.begin(), out.end(), -99.0);
    Dyn f1;
    f1->ndim = 1;
    f1->var[0] = 3;
    f1->n[0] = 0;
    f1->ncomp = 2;
    f1->value = 6.25;
    write_func_selfdesc(f1.get(), out.data());
    CHECK_BITS(out[2], 3.0);
    CHECK_BITS(out[3], 0.0);
    CHECK_BITS(out[4], 6.25);
    CHECK_BITS(out[10], -99.0);
    // ndim 2 (140131360): x1, x2 back to back, then the value block
    std::fill(out.begin(), out.end(), -99.0);
    std::vector<double> a1 = {1.0, 2.0}, a2 = {3.0, 4.0, 5.0}, v = {11, 12, 13, 14, 15, 16};
    Dyn f2;
    f2->ndim = 2;
    f2->var[0] = 3;
    f2->var[1] = 7;
    f2->n[0] = 2;
    f2->n[1] = 3;
    f2->ncomp = 1;
    f2->axis[0] = a1.data();
    f2->axis[1] = a2.data();
    f2->values = v.data();
    write_func_selfdesc(f2.get(), out.data());
    const double head[] = {10.0, 2.0, 3.0, 7.0, 2.0, 3.0, 1.0};
    for (int k = 0; k < 7; ++k) {
        CHECK_BITS(out[k], head[k]);
    }
    const double data[] = {1, 2, 3, 4, 5, 11, 12, 13, 14, 15, 16};
    for (int k = 0; k < 11; ++k) {
        CHECK_BITS(out[10 + k], data[k]);
    }
    CHECK_BITS(out[21], -99.0);
    // ndim 3: only words 0 and 1
    std::fill(out.begin(), out.end(), -99.0);
    f2->ndim = 3;
    write_func_selfdesc(f2.get(), out.data());
    CHECK_BITS(out[1], 3.0);
    CHECK_BITS(out[2], -99.0);
    write_func_selfdesc(f2.get(), nullptr); // no-op
}

TEST_CASE(usrdef_text_blanks)
{
    for (std::int32_t dim : {3, 2}) {
        Fx fx(dim);
        std::strcpy(fx.s->usrdef[0], "ABC");
        std::memset(fx.s->usrdef[1], 'X', 80);
        fx.s->usrdef[1][80] = '\0';
        fx.s->usrdef[2][0] = '\0';
        std::strcpy(fx.s->usrdef[9], "last one");
        fx.run();
        const char* t = reinterpret_cast<const char*>(fx.iiuvl.data());
        CHECK(std::string(t, 80) == "ABC" + std::string(77, ' '));
        CHECK(std::string(t + 80, 80) == std::string(80, 'X'));
        CHECK(std::string(t + 160, 80) == std::string(80, ' '));
        CHECK(std::string(t + 720, 80) == "last one" + std::string(72, ' '));
        CHECK_EQ(fx.iiuvl[200], 0); // words 200.. untouched
    }
}

TEST_CASE(furnace_two_zones_3d)
{
    Fx fx(3);
    fx.s->furnace_on = 1;
    std::strcpy(fx.s->furnace.name, "Furnace model");
    Dyn z1, z2;
    std::vector<double> x1 = {100.0}, y1 = {500.0};
    std::vector<double> x2 = {0.0, 60.0}, y2 = {20.0, 900.0};
    curve(z1.get(), 0x8e, 1, x1, y1);
    curve(z2.get(), 0x8f, 1, x2, y2);
    field<DYNARY*>(z1.get(), 0x60) = z2.get();
    fx.s->furnace.curves.head = z1.get();
    fx.s->furnace.curves.tail = z2.get();
    fx.s->furnace.curves.count = 2;
    fx.run();
    CHECK_BITS(fx.llsim[57], 10.0);
    CHECK_BITS(fx.llsim[58], 1.0);   // record fid
    CHECK_BITS(fx.llsim[28], -99.0); // ENVTMP words not written
    CHECK_BITS(fx.llsim[29], -99.0);
    // record fid 1, len 20; zone 1: 1*(1+1) = 2 + 10 = 12; zone 2: 2*2 = 4 + 10 = 14
    const std::int32_t iifnc[] = {1, 0, 21, 1, 33, 2, 47};
    for (int k = 0; k < 7; ++k) {
        CHECK_EQ(fx.iifnc[k], iifnc[k]);
    }
    CHECK_BITS(fx.llfnc[0], 20.0);
    const char* name = reinterpret_cast<const char*>(fx.llfnc.data() + 1);
    CHECK(std::string(name, 64) == "Furnace model" + std::string(51, ' '));
    CHECK_BITS(fx.llfnc[9], 2.0);  // first zone fid
    CHECK_BITS(fx.llfnc[10], 3.0); // last zone fid
    CHECK_BITS(fx.llfnc[11], -99.0);
    // zone 1 at LLFNC[20]: 10, ndim 1, xvar 1, npts 1, ncomp 1, data at [30]
    CHECK_BITS(fx.llfnc[20], 10.0);
    CHECK_BITS(fx.llfnc[21], 1.0);
    CHECK_BITS(fx.llfnc[23], 1.0);
    CHECK_BITS(fx.llfnc[30], 100.0);
    CHECK_BITS(fx.llfnc[31], 500.0);
    // zone 2 at LLFNC[32], data at [42]
    CHECK_BITS(fx.llfnc[32], 10.0);
    CHECK_BITS(fx.llfnc[35], 2.0);
    CHECK_BITS(fx.llfnc[42], 0.0);
    CHECK_BITS(fx.llfnc[43], 20.0);
    CHECK_BITS(fx.llfnc[44], 60.0);
    CHECK_BITS(fx.llfnc[45], 900.0);
    CHECK_BITS(fx.llfnc[46], -99.0);
    CHECK_EQ(fx.w(0x10c), 3);
}

TEST_CASE(frqncy_rows_cols)
{
    for (std::int32_t dim : {3, 2}) {
        Fx fx(dim);
        std::vector<double> v = {1, 0, 100, 20, 1, 1};
        fx.s->frqncy.type = 0xb5;
        fx.s->frqncy.ndim = 0;
        fx.s->frqncy.ncomp = 6;
        fx.s->frqncy.values = v.data();
        fx.s->frqncy_rows = 2;
        fx.s->frqncy_cols = 3;
        fx.run();
        CHECK_EQ(fx.iisim[76], 2);
        CHECK_EQ(fx.iisim[77], 3);
        CHECK_EQ(fx.iisim[78], 1);
        // 140130290 ndim 0, list type 0xb5: ncomp = 6 words
        CHECK_EQ(fx.iifnc[2], 7);
        for (int k = 0; k < 6; ++k) {
            CHECK_BITS(fx.llfnc[k], v[k]);
        }
        CHECK_BITS(fx.llfnc[6], -99.0);
        // rows 0: id 0, nothing allocated
        Fx g(dim);
        g.s->frqncy_rows = 0;
        g.s->frqncy_cols = 3;
        g.run();
        CHECK_EQ(g.iisim[76], 0);
        CHECK_EQ(g.iisim[77], 3);
        CHECK_EQ(g.iisim[78], 0);
        CHECK_EQ(g.w(0x10c), 0);
    }
}

TEST_CASE(vfactr_list_3d)
{
    Fx fx(3);
    std::vector<double> v = {0.5, 0.75};
    fx.s->vfactr_mode = 1;
    fx.s->vfactr_n = 2;
    fx.s->vfactr.type = 0xb5;
    fx.s->vfactr.ndim = 0;
    fx.s->vfactr.ncomp = 2;
    fx.s->vfactr.values = v.data();
    fx.run();
    CHECK_EQ(fx.iisim[80], 1);
    CHECK_EQ(fx.iisim[81], 2);
    CHECK_EQ(fx.iisim[82], 1);
    CHECK_EQ(fx.iifnc[2], 3);
    CHECK_BITS(fx.llfnc[0], 0.5);
    CHECK_BITS(fx.llfnc[1], 0.75);
}

TEST_CASE(pdie_magnitude)
{
    Fx fx(3);
    std::vector<std::uint64_t> obuf(sizeof(LLOBJ_DATA) / 8 + 1, 0);
    auto* obj = reinterpret_cast<LLOBJ_DATA*>(obuf.data());
    field<std::int32_t>(obj, 8) = 4;
    field<double>(obj, 0x2160 + 0x170) = 3.0;
    field<double>(obj, 0x2160 + 0x178) = 4.0;
    field<double>(obj, 0x2160 + 0x180) = 12.0;
    field<DB_STEP_DATA*>(obj, 0x2160 + 0xa80) = fx.m;
    fx.m->objects.head = obj;
    fx.m->objects.count = 1;
    fx.s->pdie1 = 4;
    // 3D: sqrt((9 + 16) + 144) = 13
    CHECK_BITS(sim_pdie_magnitude(fx.s), 13.0);
    fx.run();
    CHECK_BITS(fx.llsim[2], 13.0);
    // other id: no object
    fx.s->pdie1 = 5;
    CHECK_BITS(sim_pdie_magnitude(fx.s), 0.0);
    // 2D model: sqrt(9 + 16) = 5
    Fx f2(2);
    field<DB_STEP_DATA*>(obj, 0x2160 + 0xa80) = f2.m;
    CHECK_BITS(omv_magnitude(reinterpret_cast<std::byte*>(obj) + 0x2160), 5.0);
    // grouping ((a*a) + (b*b)) + (c*c) (14021e214, 14021e232): a = 1e8, b = c = 1. 1e16 + 1 is a
    // tie and rounds to even 1e16, + 1 again gives 1e16, so the result is 1e8; the other grouping
    // 1e16 + (1 + 1) = 1e16 + 2 would give sqrt(1e16 + 2) != 1e8.
    field<DB_STEP_DATA*>(obj, 0x2160 + 0xa80) = fx.m;
    field<double>(obj, 0x2160 + 0x170) = 1e8;
    field<double>(obj, 0x2160 + 0x178) = 1.0;
    field<double>(obj, 0x2160 + 0x180) = 1.0;
    CHECK_BITS(omv_magnitude(reinterpret_cast<std::byte*>(obj) + 0x2160), 1e8);
}

int main() { return testkit::run_all(); }
