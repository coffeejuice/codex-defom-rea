// ctest-labels: geoutl
// Tier-1 tests of the basic geometry kernels (defrt/ftn/geoutl_basic.hpp): 1404cce5e, 1404d68dd,
// 1404cf5c0, 1404ded0c, 1404d71d7, 1404d0939, 1404cccfb, 1404d3d43, 1404e33b7, 1404edb2f.
//
// The golden rows are bit patterns produced by the original routines, called in a copy of
// DEF_PRE_64.exe mapped at its link base into a test process (the binary is not modified or run).
// The full random sweep (8 million calls, 0 differences apart from NaN sign/payload when two NaN
// operands meet in a commutative + or *) is deform-rea-work/scripts_p67/gu-basic/basicdiff.cpp;
// gold.cpp there prints the tables below.
#include <defrt/ftn/geoutl_basic.hpp>
#include <testkit/check.hpp>

#include <bit>
#include <cstdint>
#include <cmath>
#include <cstring>

using namespace def::rt::geoutl;

namespace {

double D(std::uint64_t u) { return std::bit_cast<double>(u); }
std::uint64_t U(double d) { return std::bit_cast<std::uint64_t>(d); }

// rows: a(3) b(3) | c(3) len g_cross_inv (g_cross_inv = 0 before the call)
const std::uint64_t kCross[][11] = {
    {0x3ff0000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x3ff0000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x3ff0000000000000, 0x3ff0000000000000, 0x3ff0000000000000},
    {0x3ff0000000000000, 0x4000000000000000, 0x4008000000000000, 0x4010000000000000, 0x4014000000000000, 0x4018000000000000, 0xbfda20bd700c2c3e, 0x3fea20bd700c2c3e, 0xbfda20bd700c2c3e, 0x401d64d51e0db1c6, 0x3fc16b28f55d72d4},
    {0x3ff0000000000000, 0x4000000000000000, 0x4008000000000000, 0x4000000000000000, 0x4010000000000000, 0x4018000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000},
    {0x3fb999999999999a, 0x3fc999999999999a, 0x3fd3333333333333, 0xbfe6666666666666, 0x3ff199999999999a, 0x3fa999999999999a, 0xbfe6493091343a47, 0xbfddf259431e2e4e, 0x3fe1692df170cd87, 0x3fdd682d160050e7, 0x4001692df170cd87},
    {0x3e112e0be826d695, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x3e112e0be826d695, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x3c32725dd1d243ac, 0x0000000000000000, 0x0000000000000000},
    {0x7ff8000000000000, 0x3ff0000000000000, 0x4000000000000000, 0x4008000000000000, 0x4010000000000000, 0x4014000000000000, 0x7ff8000000000000, 0x7ff8000000000000, 0x7ff8000000000000, 0x7ff8000000000000, 0x7ff8000000000000},
    {0x4029000000000000, 0xc00a000000000000, 0x401c000000000000, 0x40290000035afe53, 0xc009ffcb923a29c7, 0x401c0a3d70a3d70a, 0xbfd06d9955bc49c8, 0xbfeeed1f54e52d67, 0x3f83cc4ddadb2149, 0x3fc08e2fd631815a, 0x401eed2aae8211cd},
};
// rows: p(3) q(3) | u(3) len
const std::uint64_t kUnit[][10] = {
    {0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x4008000000000000, 0x4010000000000000, 0x0000000000000000, 0x3fe3333333333333, 0x3fe999999999999a, 0x0000000000000000, 0x4014000000000000},
    {0x3ff0000000000000, 0x4000000000000000, 0x4008000000000000, 0x3ff0000000000000, 0x4000000000000000, 0x4008000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000},
    {0x3fb999999999999a, 0x3fc999999999999a, 0x3fd3333333333333, 0x3ffb333333333333, 0xc00199999999999a, 0x3feccccccccccccd, 0x3fe160dfa21c7009, 0xbfea114f732aa80f, 0x3fca114f732aa80f, 0x400791c8e7102b26},
    {0x8000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x8000000000000000, 0x0000000000000000, 0x0000000000000000, 0x8000000000000000, 0x0000000000000000, 0x0000000000000000},
    {0x4014000000000000, 0x4014000000000000, 0x4014000000000000, 0x4014000000000466, 0x4014000000000000, 0x4013fffffffffeae, 0x3feea62116fa0c48, 0x0000000000000000, 0xbfd26676fde124ee, 0x3d725e8b43152e6d},
};
// rows: v(3) | m(9) (0x5555... before) zero_vector rxz
const std::uint64_t kDir[][14] = {
    {0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x5555555555555555, 0x5555555555555555, 0x5555555555555555, 0x5555555555555555, 0x5555555555555555, 0x5555555555555555, 0x5555555555555555, 0x5555555555555555, 0x5555555555555555, 0x3ff0000000000000, 0x0000000000000000},
    {0x0000000000000000, 0x3ff0000000000000, 0x0000000000000000, 0x3ff0000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x3ff0000000000000, 0x0000000000000000, 0xbff0000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000},
    {0x3fd3333333333333, 0xbfd999999999999a, 0x3ff3333333333333, 0x3fef0b6848d2af1b, 0x3fb31ab65432e1ea, 0x3fcd89d89d89d89d, 0x0000000000000000, 0x3fee72929631180d, 0xbfd3b13b13b13b14, 0xbfcf0b6848d2af1b, 0x3fd31ab65432e1ea, 0x3fed89d89d89d89d, 0x0000000000000000, 0x3ff3ca78e19fe93c},
    {0x3d719799812dea11, 0x3ff0000000000000, 0x3d719799812dea11, 0x3ff0000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x3d78e10d3a69204b, 0x3ff0000000000000, 0x0000000000000000, 0xbff0000000000000, 0x3d78e10d3a69204b, 0x0000000000000000, 0x3d78e10d3a69204b},
    {0xc000000000000000, 0x3fe0000000000000, 0x0000000000000000, 0x0000000000000000, 0x3fcf0b6848d2af1c, 0xbfef0b6848d2af1c, 0x0000000000000000, 0x3fef0b6848d2af1c, 0x3fcf0b6848d2af1c, 0x3ff0000000000000, 0x8000000000000000, 0x0000000000000000, 0x0000000000000000, 0x4000000000000000},
};
// inputs of 1404ded0c and 1404d0939 (n = 2)
const double kA6[6] = {1.5, -2.25, 0.75, 10.1, 3.3, -7.9};
const double kC3[3] = {0.5, 0.25, -1.0};
const double kR9[9] = {0.36, 0.48, -0.8, -0.8, 0.6, 0.0, 0.48, 0.64, 0.6};
// rows: mode | b(3,2)
const std::uint64_t kXform[][7] = {
    {0x3ff0000000000000, 0x4009999999999999, 0x3fb99999999999a0, 0x3fd0000000000000, 0xc0025e353f7ced91, 0x40002d0e56041892, 0xc027a3d70a3d70a4},
    {0x4000000000000000, 0xbfe47ae147ae147c, 0xc002666666666666, 0xbff451eb851eb852, 0x4028147ae147ae14, 0xc017666666666667, 0x3ff3851eb851eb84},
};
// rows: p(2) a | p(2) sin cos (SAVE)
const std::uint64_t kRot2d[][7] = {
    {0x3ff8000000000000, 0xc000000000000000, 0x3fe6666666666666, 0x40037c4f939ee7f3, 0xbfe2070707561492, 0x3fe49d6e694619b8, 0x3fe87996529f9d93},
    {0x3ff8000000000000, 0xc000000000000000, 0x4008000000000000, 0xbff33e757376b707, 0x40018887a98c16ea, 0x3fc210386db6d55b, 0xbfefae04be85e5d2},
    {0x0000000000000000, 0x3ff0000000000000, 0xbff921fb54442d18, 0x3ff0000000000000, 0x3c91a62000000000, 0xbff0000000000000, 0x3c91a62000000000},
    {0x400a000000000000, 0x3fc0000000000000, 0x4059000000000000, 0x4006ed39593f9054, 0xbff89b3b6e4d984d, 0xbfe03425b78c4db8, 0x3feb981dbf665fe0},
};
// rows: flag | b(3,2) sign (SAVE)
const std::uint64_t kRotFrame[][8] = {
    {0x3ff0000000000000, 0x4009999999999999, 0x3fb99999999999a0, 0x3fd0000000000000, 0xc0025e353f7ced91, 0x40002d0e56041892, 0xc027a3d70a3d70a4, 0x3ff0000000000000},
    {0x0000000000000000, 0x4009999999999999, 0x3fb99999999999a0, 0xbfd0000000000000, 0xc0025e353f7ced91, 0x40002d0e56041892, 0x4027a3d70a3d70a4, 0xbff0000000000000},
};
// quad of the 1404cccfb rows
const double kQ12[12] = {0, 0, 0, 2.0, 0.1, 0.05, 2.1, 1.9, -0.2, -0.1, 2.0, 0.3};
// rows: xi eta | n(3) len (SAVE)
const std::uint64_t kQuad[][6] = {
    {0x3fd3333333333333, 0xbfe3333333333333, 0x3f9d2e1b69e69883, 0x3f9efdc4d6dfb8ee, 0x3feff8eb473bffd3, 0x3fee87c79dac2aec},
    {0x0000000000000000, 0x0000000000000000, 0x3fbb45284819a437, 0xbf8acaae4fd154fa, 0x3fefd0aefec894ea, 0x3ff00d7f04b0c95f},
    {0xbff0000000000000, 0x3ff0000000000000, 0x3fcb5d98f28c7e43, 0xbfc13594f78a7ee8, 0x3feef66fb0c7de09, 0x3ff225f872330e54},
    {0x3feff7ced916872b, 0xbfd0000000000000, 0x3fb2a4be20b883a5, 0x3fc108cd4aa14e08, 0x3fefa12f58058bba, 0x3fee361afd0926f7},
};
// rows: x(3,3) | n(3) g_tri_normal_len
const std::uint64_t kTri[][13] = {
    {0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x3ff0000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x3ff0000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x3ff0000000000000, 0x3ff0000000000000},
    {0x3ff199999999999a, 0x400199999999999a, 0x400a666666666666, 0x4012cccccccccccd, 0xbfe0000000000000, 0x4000000000000000, 0x3fd3333333333333, 0x3feccccccccccccd, 0x402199999999999a, 0xbfe469b695c3a0c9, 0xbfe7271bdc52962c, 0xbfd0e21d0cbd0ab6, 0x4039edbbe441416a},
    {0x3ff0000000000000, 0x3ff0000000000000, 0x3ff0000000000000, 0x4000000000000000, 0x4000000000000000, 0x4000000000000000, 0x4008000000000000, 0x4008000000000000, 0x4008000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000},
};
// rows: a(3) b(3) | same
const std::uint64_t kSame[][7] = {
    {0x3ff0000000000000, 0x4000000000000000, 0x4008000000000000, 0x3ff0000000000000, 0x4000000000000000, 0x4008000000000000, 0x3ff0000000000000},
    {0x3ff0000000000000, 0x4000000000000000, 0x4008000000000000, 0x3ff0000a7c5ac472, 0x4000000000000000, 0x4008000000000000, 0x0000000000000000},
    {0x3ff0000000000000, 0x4000000000000000, 0x4008000000000000, 0x3ff0000000000000, 0x4000000000000000, 0x40080005c4651f3f, 0x0000000000000000},
    {0x3ff0000000000000, 0x4000000000000000, 0x7ff8000000000000, 0x3ff0000000000000, 0x4000000000000000, 0x4008000000000000, 0x3ff0000000000000},
    {0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0xbee4c305a3adef92, 0x3ee4c305a3adef92, 0x0000000000000000, 0x3ff0000000000000},
};

} // namespace

TEST_CASE(cross_1404cce5e)
{
    for (const auto& r : kCross) {
        reset_globals_basic();
        const double a[3] = {D(r[0]), D(r[1]), D(r[2])};
        const double b[3] = {D(r[3]), D(r[4]), D(r[5])};
        double c[3];
        double len = 0;
        fun_1404cce5e(a, b, c, &len);
        for (int k = 0; k < 3; ++k) CHECK_EQ(U(c[k]), r[6 + k]);
        CHECK_EQ(U(len), r[9]);
        CHECK_EQ(U(g_cross_inv), r[10]);
    }
    // c aliased with a: a(1), b(1) are read again after c(1) is stored
    double a[3] = {1, 2, 3};
    const double b[3] = {4, 5, 6};
    double len = 0;
    fun_1404cce5e(a, b, a, &len);
    // c(1) = 2*6 - 5*3 = -3 lands in a(1) before c(2) = 3*4 - 6*a(1) = 30 is formed;
    // c(3) = 1*5 - 4*2 = -3 uses the old a(1). |c|^2 = 9 + (900 + 9) = 918.
    const double inv = 1.0 / std::sqrt(918.0);
    CHECK_EQ(U(len), U(std::sqrt(918.0)));
    CHECK_EQ(U(a[0]), U(-3.0 * inv));
    CHECK_EQ(U(a[1]), U(30.0 * inv));
    CHECK_EQ(U(a[2]), U(-3.0 * inv));
}

TEST_CASE(unit_1404d68dd)
{
    for (const auto& r : kUnit) {
        const double p[3] = {D(r[0]), D(r[1]), D(r[2])};
        const double q[3] = {D(r[3]), D(r[4]), D(r[5])};
        double u[3];
        double len = 0;
        fun_1404d68dd(p, q, u, &len);
        for (int k = 0; k < 3; ++k) CHECK_EQ(U(u[k]), r[6 + k]);
        CHECK_EQ(U(len), r[9]);
    }
}

TEST_CASE(direction_1404cf5c0)
{
    for (const auto& r : kDir) {
        reset_globals_basic();
        const double v[3] = {D(r[0]), D(r[1]), D(r[2])};
        double m[9];
        for (double& x : m) x = D(0x5555555555555555ull);
        fun_1404cf5c0(v, m);
        for (int k = 0; k < 9; ++k) CHECK_EQ(U(m[k]), r[3 + k]);
        CHECK_EQ(U(static_cast<double>(g_dir_matrix.zero_vector)), r[12]);
        CHECK_EQ(U(g_dir_matrix.rxz), r[13]);
    }
}

TEST_CASE(transform_1404ded0c)
{
    for (const auto& r : kXform) {
        reset_globals_basic();
        const std::int32_t mode = static_cast<std::int32_t>(D(r[0]));
        const std::int32_t n = 2;
        double b[6];
        fun_1404ded0c(kA6, b, kC3, kR9, &n, &mode);
        for (int k = 0; k < 6; ++k) CHECK_EQ(U(b[k]), r[1 + k]);
        CHECK_EQ(g_xform.i, 3);
        CHECK_EQ(g_xform.j, 4);
        CHECK_EQ(g_xform.k, 4);
    }
}

TEST_CASE(rotate_1404d71d7)
{
    for (const auto& r : kRot2d) {
        double p[2] = {D(r[0]), D(r[1])};
        const double a = D(r[2]);
        fun_1404d71d7(p, &a);
        CHECK_EQ(U(p[0]), r[3]);
        CHECK_EQ(U(p[1]), r[4]);
        CHECK_EQ(U(g_rot2d.sin_a), r[5]);
        CHECK_EQ(U(g_rot2d.cos_a), r[6]);
    }
}

TEST_CASE(frame_1404d0939)
{
    for (const auto& r : kRotFrame) {
        reset_globals_basic();
        const std::int32_t flag = static_cast<std::int32_t>(D(r[0]));
        const std::int32_t n = 2;
        double b[6];
        fun_1404d0939(kA6, kC3, kR9, b, &flag, &n);
        for (int k = 0; k < 6; ++k) CHECK_EQ(U(b[k]), r[1 + k]);
        CHECK_EQ(U(g_rot_frame.sign), r[7]);
        CHECK_EQ(g_rot_frame.i, 3);
    }
    // in place (a = b), as the drop solver calls it
    double ab[6];
    std::memcpy(ab, kA6, sizeof ab);
    const std::int32_t flag = 1;
    const std::int32_t n = 2;
    fun_1404d0939(ab, kC3, kR9, ab, &flag, &n);
    for (int k = 0; k < 6; ++k) CHECK_EQ(U(ab[k]), kRotFrame[0][1 + k]);
}

TEST_CASE(quad_normal_1404cccfb)
{
    for (const auto& r : kQuad) {
        reset_globals_basic();
        const double xi = D(r[0]);
        const double eta = D(r[1]);
        double nn[3];
        fun_1404cccfb(kQ12, &xi, &eta, nn);
        for (int k = 0; k < 3; ++k) CHECK_EQ(U(nn[k]), r[2 + k]);
        CHECK_EQ(U(g_quad_normal.len), r[5]);
    }
}

TEST_CASE(tri_normal_1404d3d43)
{
    for (const auto& r : kTri) {
        reset_globals_basic();
        double x[9];
        for (int k = 0; k < 9; ++k) x[k] = D(r[k]);
        double nn[3];
        fun_1404d3d43(x, nn);
        for (int k = 0; k < 3; ++k) CHECK_EQ(U(nn[k]), r[9 + k]);
        CHECK_EQ(U(g_tri_normal_len), r[12]);
    }
}

TEST_CASE(same_point_1404e33b7)
{
    for (const auto& r : kSame) {
        const double a[3] = {D(r[0]), D(r[1]), D(r[2])};
        const double b[3] = {D(r[3]), D(r[4]), D(r[5])};
        std::int32_t s = 9;
        fun_1404e33b7(a, b, &s);
        CHECK_EQ(U(static_cast<double>(s)), r[6]);
    }
    const std::int32_t code = -1;
    fun_1404edb2f(&code); // bare RET
}

int main() { return testkit::run_all(); }
