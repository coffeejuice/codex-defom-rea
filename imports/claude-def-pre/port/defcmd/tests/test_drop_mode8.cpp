// ctest-labels: p67-3 drop
// Tier-1 test of the mode-8 axis point (surface mass properties 1403e8510 and its callees,
// packet p67-3 worker C; spec docs/re/commands/OBJPOS_drop_mode8.md).
//
// Golden tables kX/kF/kB (1403ea82d mode 0: the 22 doubles of the block 0x140b44d80 after one
// pass) and kJ (1403f0b89): outputs of the ORIGINAL routines in a mapped copy of DEF_PRE_64.exe
// (pure math only), printed by deform-rea-work/scripts_p673/c/m8diff.cpp, which also diffed the
// port against the original on random meshes and matrices (0 mismatches). The driver
// 1403e8510 allocates (Fortran ALLOCATE), so it was not run in the image: its cases below are
// hand-checked (unit cube: three equal moments, no axis; 1x1x3 box and hexagonal prism: axis z).
#include <defcmd/drop/globals_c.hpp>
#include <defcmd/drop/mode8.hpp>
#include <testkit/check.hpp>

#include <bit>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <vector>

using namespace def::cmd::drop;
using i32 = std::int32_t;

namespace {

// mesh 0: np 8 nf 6 type 4
const std::uint64_t kX0[] = {
0x0000000000000000ull, 0x0000000000000000ull, 0x0000000000000000ull, 0x3ff0000000000000ull,
0x0000000000000000ull, 0x0000000000000000ull, 0x3ff0000000000000ull, 0x3ff0000000000000ull,
0x0000000000000000ull, 0x0000000000000000ull, 0x3ff0000000000000ull, 0x0000000000000000ull,
0x0000000000000000ull, 0x0000000000000000ull, 0x3ff0000000000000ull, 0x3ff0000000000000ull,
0x0000000000000000ull, 0x3ff0000000000000ull, 0x3ff0000000000000ull, 0x3ff0000000000000ull,
0x3ff0000000000000ull, 0x0000000000000000ull, 0x3ff0000000000000ull, 0x3ff0000000000000ull,
};
const std::int32_t kF0[] = {1, 4, 3, 2, 5, 6, 7, 8, 1, 2, 6, 5, 2, 3, 7, 6,
3, 4, 8, 7, 4, 1, 5, 8, };
const std::uint64_t kB0[22] = {
0x3fc5555555555554ull, 0x0000000000000000ull, 0x0000000000000000ull, 0x0000000000000000ull,
0x3fc5555555555554ull, 0x0000000000000000ull, 0x0000000000000000ull, 0x0000000000000000ull,
0x3fc5555555555554ull, 0x3ff0000000000000ull, 0x0000000000000000ull, 0x0000000000000000ull,
0x0000000000000000ull, 0x3ff0000000000000ull, 0x0000000000000000ull, 0x0000000000000000ull,
0x0000000000000000ull, 0x3ff0000000000000ull, 0x3ff0000000000000ull, 0x3fe0000000000000ull,
0x3fe0000000000000ull, 0x3fe0000000000000ull,
};
// mesh 1: np 8 nf 12 type 3
const std::uint64_t kX1[] = {
0x4004000000000000ull, 0xbff4000000000000ull, 0x3fe8000000000000ull, 0x400c000000000000ull,
0xbff4000000000000ull, 0x3fe8000000000000ull, 0x400c000000000000ull, 0xbfd0000000000000ull,
0x3fe8000000000000ull, 0x4004000000000000ull, 0xbfd0000000000000ull, 0x3fe8000000000000ull,
0x4004000000000000ull, 0xbff4000000000000ull, 0x400e000000000000ull, 0x400c000000000000ull,
0xbff4000000000000ull, 0x400e000000000000ull, 0x400c000000000000ull, 0xbfd0000000000000ull,
0x400e000000000000ull, 0x4004000000000000ull, 0xbfd0000000000000ull, 0x400e000000000000ull,
};
const std::int32_t kF1[] = {1, 4, 3, 1, 1, 3, 2, 1, 5, 6, 7, 5, 5, 7, 8, 5,
1, 2, 6, 1, 1, 6, 5, 1, 2, 3, 7, 2, 2, 7, 6, 2,
3, 4, 8, 3, 3, 8, 7, 3, 4, 1, 5, 4, 4, 5, 8, 4,
};
const std::uint64_t kB1[22] = {
0x4004000000000006ull, 0xbc92000000000000ull, 0x3caa000000000000ull, 0xbc92000000000000ull,
0x4004000000000006ull, 0xbc94000000000000ull, 0x3caa000000000000ull, 0xbc94000000000000ull,
0x3fe0000000000000ull, 0x4008000000000000ull, 0x0000000000000000ull, 0x0000000000000000ull,
0x0000000000000000ull, 0x4008000000000000ull, 0x0000000000000000ull, 0x0000000000000000ull,
0x0000000000000000ull, 0x4008000000000000ull, 0x4008000000000000ull, 0x4008000000000000ull,
0xbfe8000000000001ull, 0x4002000000000001ull,
};
// mesh 2: np 14 nf 18 type 4
const std::uint64_t kX2[] = {
0x4010000000000000ull, 0xbff4000000000000ull, 0x3fe8000000000000ull, 0x400a000000000000ull,
0x3fa91b85c8473000ull, 0x3fe8000000000000ull, 0x3ffc000000000002ull, 0x3fa91b85c8473000ull,
0x3fe8000000000000ull, 0x3ff0000000000000ull, 0xbff3ffffffffffffull, 0x3fe8000000000000ull,
0x3ffbfffffffffffdull, 0xc004646e17211cc0ull, 0x3fe8000000000000ull, 0x400a000000000000ull,
0xc004646e17211cc0ull, 0x3fe8000000000000ull, 0x4010000000000000ull, 0xbff4000000000000ull,
0x4006000000000000ull, 0x400a000000000000ull, 0x3fa91b85c8473000ull, 0x4006000000000000ull,
0x3ffc000000000002ull, 0x3fa91b85c8473000ull, 0x4006000000000000ull, 0x3ff0000000000000ull,
0xbff3ffffffffffffull, 0x4006000000000000ull, 0x3ffbfffffffffffdull, 0xc004646e17211cc0ull,
0x4006000000000000ull, 0x400a000000000000ull, 0xc004646e17211cc0ull, 0x4006000000000000ull,
0x4004000000000000ull, 0xbff4000000000000ull, 0x3fe8000000000000ull, 0x4004000000000000ull,
0xbff4000000000000ull, 0x4006000000000000ull,
};
const std::int32_t kF2[] = {1, 2, 8, 7, 13, 2, 1, 13, 14, 7, 8, 14, 2, 3, 9, 8,
13, 3, 2, 13, 14, 8, 9, 14, 3, 4, 10, 9, 13, 4, 3, 13,
14, 9, 10, 14, 4, 5, 11, 10, 13, 5, 4, 13, 14, 10, 11, 14,
5, 6, 12, 11, 13, 6, 5, 13, 14, 11, 12, 14, 6, 1, 7, 12,
13, 1, 6, 13, 14, 12, 7, 14, };
const std::uint64_t kB2[22] = {
0x4022c13eadbdc1e2ull, 0xbcd9d8ada7bb1590ull, 0x3ca6c006e7825da6ull, 0xbcd9d8ada7bb1590ull,
0x4022c13eadbdc1e3ull, 0x3caec416297bcb7dull, 0x3ca6c006e7825da6ull, 0x3caec416297bcb7dull,
0x4025ebd838c9d8a5ull, 0x402761f7b40a80b0ull, 0x0000000000000000ull, 0x0000000000000000ull,
0x0000000000000000ull, 0x402761f7b40a80b0ull, 0x0000000000000000ull, 0x0000000000000000ull,
0x0000000000000000ull, 0x402761f7b40a80b0ull, 0x402761f7b40a80b0ull, 0x4004000000000001ull,
0xbff4000000000001ull, 0x3ffbfffffffffffbull,
};
// mesh 3: np 8 nf 6 type 4
const std::uint64_t kX3[] = {
0x4003f61ccf617cb2ull, 0xbff3ff448d174f30ull, 0x3fe929e38f8ef82full, 0x40120331a46a3125ull,
0xbff399492431cdd8ull, 0x3fe8012568ca1c5bull, 0x4011d2ddf0c6b72eull, 0x3fd1e4b36d3b47ddull,
0x3fe75d90bf9df21aull, 0x4003c2356969aeaaull, 0x3fd013a3e0955272ull, 0x3fe695c40f6a29dfull,
0x4003d2f0af8abe9eull, 0xbff4213df9cfa6ecull, 0x3ffc623bb0116bc9ull, 0x40121a8335fb5eccull,
0xbff3a57e6d686833ull, 0x3ffb87e22fd1cf38ull, 0x40121f4d1b595a22ull, 0x3fcfaee7062a8b90ull,
0x3ffb3cbad305aea6ull, 0x400446ee7338f1fdull, 0x3fd10ccaeaca386aull, 0x3ffc3d1d88b7be93ull,
};
const std::int32_t kF3[] = {1, 4, 3, 2, 5, 6, 7, 8, 1, 2, 6, 5, 2, 3, 7, 6,
3, 4, 8, 7, 4, 1, 5, 8, };
const std::uint64_t kB3[22] = {
0x3fea1bf261267bf4ull, 0xbf65e68ca2cdeee0ull, 0x3f81122705a8b574ull, 0xbf65e68ca2cdeee0ull,
0x3ff437375fdca4a6ull, 0x3f8d1aa271058dc5ull, 0x3f81122705a8b574ull, 0x3f8d1aa271058dc5ull,
0x3ff941c0448e55b0ull, 0x400815232b8ec6eaull, 0x0000000000000000ull, 0x0000000000000000ull,
0x0000000000000000ull, 0x400815232b8ec6eaull, 0x0000000000000000ull, 0x0000000000000000ull,
0x0000000000000000ull, 0x400815232b8ec6eaull, 0x400815232b8ec6eaull, 0x400be758557952f7ull,
0xbfdf2e49c244cfc5ull, 0x3ff3e2d1279b0353ull,
};
// jacobi 0: nrot 9
const std::uint64_t kJ0[21] = {  // a after, d, v
0x4010000000000000ull, 0x3ff0000000000000ull, 0x3fe0000000000000ull, 0x0000000000000000ull,
0x4008000000000000ull, 0xbfd0000000000000ull, 0x0000000000000000ull, 0x0000000000000000ull,
0x4000000000000000ull, 0x40129b8551bf4c38ull, 0x400584b2f6300df1ull, 0x3ffa8884cca2b342ull,
0x3feb6ffaf12b6490ull, 0x3fe00e5698638276ull, 0x3fbd46c98471b99full, 0xbfd7a7b4102b6c44ull,
0x3fe82959e32e9949ull, 0xbfe1547c7685aa51ull, 0xbfd6eabf3328d1fdull, 0x3fdb035214cd6a75ull,
0x3feaa6aa75bd37e1ull,
};
// jacobi 1: nrot 0
const std::uint64_t kJ1[21] = {  // a after, d, v
0x4000000000000000ull, 0x0000000000000000ull, 0x0000000000000000ull, 0x0000000000000000ull,
0x4000000000000000ull, 0x0000000000000000ull, 0x0000000000000000ull, 0x0000000000000000ull,
0x4014000000000000ull, 0x4000000000000000ull, 0x4000000000000000ull, 0x4014000000000000ull,
0x3ff0000000000000ull, 0x0000000000000000ull, 0x0000000000000000ull, 0x0000000000000000ull,
0x3ff0000000000000ull, 0x0000000000000000ull, 0x0000000000000000ull, 0x0000000000000000ull,
0x3ff0000000000000ull,
};
// jacobi 2: nrot 8
const std::uint64_t kJ2[21] = {  // a after, d, v
0x3f50624dd2f1a9fcull, 0x401c000000000000ull, 0xc000000000000000ull, 0x0000000000000000ull,
0x408f400000000000ull, 0x3fc0000000000000ull, 0x0000000000000000ull, 0x0000000000000000ull,
0xc044000000000000ull, 0x3faa99dc11d1b0deull, 0x408f40645f16fcecull, 0xc0440ccba3d89d44ull,
0x3feff598db9448ebull, 0xbf7c9c8d49aa5819ull, 0xbfa98ba506d0da64ull, 0x3f7cab46a7501f22ull,
0x3fefffcc9e27468cull, 0x3f1bfa237a81aabcull, 0x3fa98b62fe6789eaull, 0xbf3ddf2dcf794c23ull,
0x3feff5cc12ed3eaeull,
};
// jacobi 3: nrot 11
const std::uint64_t kJ3[21] = {  // a after, d, v
0x3ff0000000000000ull, 0x3e112e0be826d695ull, 0x0000000000000000ull, 0x0000000000000000ull,
0x3ff0000000000000ull, 0x3e212e0be826d695ull, 0x0000000000000000ull, 0x0000000000000000ull,
0x3ff0000000000000ull, 0x3feffffffeccad59ull, 0x3ff0000000000000ull, 0x3ff000000099a953ull,
0x3fd43d136029e59full, 0xbfe6a09e642a9622ull, 0x3fe43d13656af449ull, 0x3fec9f25c71ebfd0ull,
0xbe008ecb1d51c000ull, 0xbfdc9f25c044a5fcull, 0x3fd43d135ca6229bull, 0x3fe6a09e68d3e175ull,
0x3fe43d136115c04dull,
};

struct GoldMesh {
    const std::uint64_t* x;
    std::size_t nx;
    const i32* f;
    std::size_t nf4;
    const std::uint64_t* b;
};

const GoldMesh kMeshes[4] = {{kX0, sizeof kX0 / 8, kF0, sizeof kF0 / 4, kB0},
                             {kX1, sizeof kX1 / 8, kF1, sizeof kF1 / 4, kB1},
                             {kX2, sizeof kX2 / 8, kF2, sizeof kF2 / 4, kB2},
                             {kX3, sizeof kX3 / 8, kF3, sizeof kF3 / 4, kB3}};

std::vector<double> to_d(const std::uint64_t* u, std::size_t n)
{
    std::vector<double> v(n);
    for (std::size_t i = 0; i < n; ++i) v[i] = std::bit_cast<double>(u[i]);
    return v;
}

i32 mesh_type(const std::vector<i32>& f)
{
    i32 t = 3;
    for (std::size_t i = 0; i + 3 < f.size(); i += 4)
        if (f[i] != std::abs(f[i + 3])) t = 4;
    return t;
}

} // namespace

TEST_CASE(surface_integrals_golden)
{
    for (const GoldMesh& gm : kMeshes) {
        std::vector<double> x = to_d(gm.x, gm.nx);
        std::vector<i32> f(gm.f, gm.f + gm.nf4);
        i32 first = 1, nf = static_cast<i32>(f.size() / 4), mode = 0, dbg = 0, type = mesh_type(f);
        double gp[16], gw[16], p9[24] = {};
        gauss_tables(gp, gw);
        std::vector<double> nrm(3 * nf + 3), area(nf + 1);
        reset_globals_c();
        surface_integrals(x.data(), f.data(), &first, &nf, &type, nrm.data(), area.data(), &mode, p9, &dbg, gp, gw);
        for (int i = 0; i < 22; ++i) CHECK_BITS(g_massprop[i], std::bit_cast<double>(gm.b[i]));
    }
}

TEST_CASE(jacobi_golden)
{
    const double mats[4][9] = {{4, 1, 0.5, 1, 3, -0.25, 0.5, -0.25, 2},
                               {2, 0, 0, 0, 2, 0, 0, 0, 5},
                               {1e-3, 7, -2, 7, 1e3, 0.125, -2, 0.125, -40},
                               {1, 1e-9, 0, 1e-9, 1, 2e-9, 0, 2e-9, 1}};
    const std::uint64_t* gold[4] = {kJ0, kJ1, kJ2, kJ3};
    const i32 nrot_gold[4] = {9, 0, 8, 11};
    for (int c = 0; c < 4; ++c) {
        double a[9], d[3], v[9];
        std::memcpy(a, mats[c], sizeof a);
        i32 n = 3, np = 3, nrot = -1;
        jacobi_eigen(a, &n, &np, d, v, &nrot);
        CHECK_EQ(nrot, nrot_gold[c]);
        for (int i = 0; i < 9; ++i) CHECK_BITS(a[i], std::bit_cast<double>(gold[c][i]));
        for (int i = 0; i < 3; ++i) CHECK_BITS(d[i], std::bit_cast<double>(gold[c][9 + i]));
        for (int i = 0; i < 9; ++i) CHECK_BITS(v[i], std::bit_cast<double>(gold[c][12 + i]));
    }
}

TEST_CASE(mass_properties_hand_checked)
{
    for (int c = 0; c < 3; ++c) {
        const GoldMesh& gm = kMeshes[c];
        std::vector<double> x = to_d(gm.x, gm.nx);
        std::vector<i32> f(gm.f, gm.f + gm.nf4);
        i32 np = static_cast<i32>(x.size() / 3), nf = static_cast<i32>(f.size() / 4), err = -5, dbg = 0;
        double cc[3] = {9, 9, 9}, a[3] = {9, 9, 9};
        reset_globals_c();
        mass_properties(&np, &nf, x.data(), f.data(), cc, a, &err, &dbg);
        // c is the centroid of the first pass (the golden block, bit for bit)
        for (int i = 0; i < 3; ++i) CHECK_BITS(cc[i], std::bit_cast<double>(gm.b[19 + i]));
        if (c == 0) {
            // unit cube: centroid (0.5, 0.5, 0.5); I11 = I22 = I33 -> no strict minimum, no axis
            CHECK_BITS(cc[0], 0.5);
            CHECK_EQ(err, 1);
            for (double v : a) CHECK_BITS(v, 0.0);
        } else {
            // 1x1x3 box at (2.5, -1.25, 0.75) (triangles) and hexagonal prism, height along z:
            // I11 = I22 != I33, the axis is the z direction (sign from the eigen vectors)
            CHECK_EQ(err, 0);
            CHECK(std::fabs(std::fabs(a[2]) - 1.0) < 1e-9);
            CHECK(std::fabs(a[0]) < 1e-6 && std::fabs(a[1]) < 1e-6);
        }
        if (c == 1) {
            CHECK(std::fabs(cc[0] - 3.0) < 1e-12 && std::fabs(cc[1] + 0.75) < 1e-12 && std::fabs(cc[2] - 2.25) < 1e-12);
        }
    }
}

TEST_CASE(small_routines)
{
    double a[3] = {1, 2, 3}, b[3] = {4, 5, 6}, c[3], r = 7;
    cross3(a, b, c);
    CHECK_BITS(c[0], -3.0);
    CHECK_BITS(c[1], 6.0);
    CHECK_BITS(c[2], -3.0);
    i32 n = 3;
    dot_n(a, b, &r, &n);
    CHECK_BITS(r, 32.0);
    // brick functions at a corner of the top face: N(5) = 1, others 0
    double xi = -1, eta = -1, zeta = 1, nn[8], dn[24];
    brick_shape(&xi, &eta, &zeta, nn, dn);
    CHECK_BITS(nn[4], 1.0);
    CHECK_BITS(nn[6], 0.0);
}

int main() { return testkit::run_all(); }
