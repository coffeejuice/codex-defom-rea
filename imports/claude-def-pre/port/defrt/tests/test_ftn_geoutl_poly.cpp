// Tier-1: def::rt::geoutl polygon / bilinear kernels (geoutl_poly) against the original routines.
// ctest-labels: geoutl
//
// Expected bit patterns are outputs of the original routines (1404cc289 ... 1404d5a1d), called in
// a copy of DEF_PRE_64.exe mapped into a test process at its link base (the exe itself is not
// modified or run). The full random sweep (200k calls per routine, outputs and SAVE areas bit
// for bit, NaN payloads included, 0 differences) is deform-rea-work/scripts_p67/gu-poly/polydiff.cpp;
// `polydiff gold` prints the tables below.
#include <defrt/ftn/geoutl_poly.hpp>
#include <testkit/check.hpp>

#include <bit>
#include <cstdint>

using namespace def::rt::geoutl;
using U = std::uint64_t;

namespace {

double D(U b) { return std::bit_cast<double>(b); }

// quad x(3,4) of all bilinear cases
const double X[12] = {0, 0, 0, 2, 0.1, 0, 2.2, 1.9, 0.3, -0.1, 2, 0.1};

} // namespace

TEST_CASE(shape_functions_1404d6974) {
    struct R { U st[2]; U out[3]; } rows[] = {
        {{0x3fd3333333333333ull, 0xbfdccccccccccccdull}, {0x3ff537ced916872bull, 0x3fe289374bc6a7f0ull, 0x3fb03126e978d4feull}},
        {{0x3ff0000000000000ull, 0x3ff0000000000000ull}, {0x400199999999999aull, 0x3ffe666666666666ull, 0x3fd3333333333333ull}},
        {{0xbff3333333333333ull, 0x3fe6666666666666ull}, {0xbfd3df3b645a1cacull, 0x3ffb4fdf3b645a1dull, 0x3fb16872b020c49dull}},
    };
    for (const R& r : rows) {
        const double s = D(r.st[0]), t = D(r.st[1]);
        double o[3];
        fun_1404d6974(X, o, &s, &t);
        for (int i = 0; i < 3; ++i) CHECK_BITS(o[i], D(r.out[i]));
    }
}

TEST_CASE(point_segment_1404d064d) {
    struct R { std::int32_t mode; U p[2]; U pout[2]; U r[3]; } rows[] = {
        {2, {0x3ff0000000000000ull, 0x4000000000000000ull}, {0x3ff7ffffffffffffull, 0x3fe0000000000000ull}, {0x40094c583ada5b53ull, 0xbff94c583ada5b52ull, 0x3fdfffffffffffffull}},
        {1, {0xbff0000000000000ull, 0x3fe0000000000000ull}, {0xbff0000000000000ull, 0x3fe0000000000000ull}, {0x40094c583ada5b53ull, 0xbff1e3779b97f4a8ull, 0xbfcfffffffffffffull}},
        {2, {0x4010000000000000ull, 0xbff0000000000000ull}, {0x4008000000000000ull, 0x3ff0000000000000ull}, {0x40094c583ada5b53ull, 0x4001e3779b97f4a8ull, 0x3ff1999999999999ull}},
    };
    const double a[2] = {0, 0}, b[2] = {3, 1};
    for (const R& r : rows) {
        double p[2] = {D(r.p[0]), D(r.p[1])}, len, dist, t;
        fun_1404d064d(p, a, b, &len, &dist, &t, &r.mode);
        CHECK_BITS(p[0], D(r.pout[0]));
        CHECK_BITS(p[1], D(r.pout[1]));
        CHECK_BITS(len, D(r.r[0]));
        CHECK_BITS(dist, D(r.r[1]));
        CHECK_BITS(t, D(r.r[2]));
    }
}

TEST_CASE(point_in_polygon_1404cc289) {
    // counter-clockwise square: inside gives -1, outside +1, boundary 0
    struct R { std::int32_t edge, inside; U p[2]; U q[2]; U dist; } rows[] = {
        {4, -1, {0x3fe0000000000000ull, 0x3ff0000000000000ull}, {0x0000000000000000ull, 0x3ff0000000000000ull}, {0x3fe0000000000000ull}},
        {2, 1, {0x4008000000000000ull, 0x3ff0000000000000ull}, {0x4000000000000000ull, 0x3ff0000000000000ull}, {0x3ff0000000000000ull}},
        {2, 0, {0x4000000000000000ull, 0x3ff0000000000000ull}, {0x4000000000000000ull, 0x3ff0000000000000ull}, {0x0000000000000000ull}},
        {3, 0, {0x4000000000000000ull, 0x4000000000000000ull}, {0x4000000000000000ull, 0x4000000000000000ull}, {0x0000000000000000ull}},
        {1, 1, {0xbfe0000000000000ull, 0xbfd0000000000000ull}, {0x0000000000000000ull, 0x0000000000000000ull}, {0x3fe1e3779b97f4a8ull}},
    };
    const double xy[8] = {0, 0, 2, 0, 2, 2, 0, 2};
    const std::int32_t n = 4, closed = 1, useidx = 0, ld = 2, idx[4] = {1, 2, 3, 4};
    for (const R& r : rows) {
        const double p[2] = {D(r.p[0]), D(r.p[1])};
        double q[2] = {0, 0}, d = 1e30;
        std::int32_t e = 0, in = 0;
        fun_1404cc289(xy, p, q, &d, &e, &n, &in, &closed, idx, &useidx, &ld);
        CHECK_EQ(e, r.edge);
        CHECK_EQ(in, r.inside);
        CHECK_BITS(q[0], D(r.q[0]));
        CHECK_BITS(q[1], D(r.q[1]));
        CHECK_BITS(d, D(r.dist));
    }
}

TEST_CASE(bilinear_inverse_1404ccf58) {
    // analytic root (1404cd2e1) for the first and last point, bisection (1404ce05b) failing for
    // the point far outside
    struct R { std::int32_t nroot, ierr; U p[2]; U st[4]; } rows[] = {
        {1, 0, {0x3feb333333333333ull, 0x3fdae147ae147ae1ull}, {0xbfc3dfce167c6390ull, 0xbfe360e42cd3514full, 0x0000000000000000ull, 0x0000000000000000ull}},
        {1, 1, {0x4014000000000000ull, 0x4014000000000000ull}, {0x0000000000000000ull, 0x0000000000000000ull, 0x0000000000000000ull, 0x0000000000000000ull}},
        {1, 0, {0x3ff199999999999aull, 0x3ff0000000000000ull}, {0x3fb1dc47711dc434ull, 0x3cc7cb08d3dcb094ull, 0x0000000000000000ull, 0x0000000000000000ull}},
    };
    reset_poly_globals();
    for (const R& r : rows) {
        const double p[2] = {D(r.p[0]), D(r.p[1])};
        double st[4] = {0, 0, 0, 0};
        std::int32_t k = 1, er = 7;
        fun_1404ccf58(X, p, st, &k, &er);
        CHECK_EQ(k, r.nroot);
        CHECK_EQ(er, r.ierr);
        for (int i = 0; i < 4; ++i) CHECK_BITS(st[i], D(r.st[i]));
    }
}

TEST_CASE(bilinear_bisection_1404ce05b) {
    struct R { std::int32_t ierr; U p[2]; U st[2]; } rows[] = {
        {0, {0x3feb333333333333ull, 0x3fdae147ae147ae1ull}, {0xbfc3dfce1db5edccull, 0xbfe360e400000000ull}},
        {0, {0x3ff8000000000000ull, 0x3ffb333333333333ull}, {0x3fd9cda871fa0074ull, 0x3fe8171400000000ull}},
    };
    for (const R& r : rows) {
        const double p[2] = {D(r.p[0]), D(r.p[1])};
        double st[2] = {0, 0};
        std::int32_t er = 7;
        fun_1404ce05b(X, p, st, &er);
        CHECK_EQ(er, r.ierr);
        CHECK_BITS(st[0], D(r.st[0]));
        CHECK_BITS(st[1], D(r.st[1]));
    }
}

TEST_CASE(pntlin_1404d6b13) {
    struct R { U p[3]; U q[3]; U r[2]; } rows[] = {
        {{0x3fe0000000000000ull, 0x3fe0000000000000ull, 0x3fe0000000000000ull}, {0x3fcb6db6db6db6dbull, 0x3fdb6db6db6db6dbull, 0x3fe4924924924924ull}, {0x3fcb6db6db6db6dbull, 0x3fd4f2ec413cb52aull}},
        {{0x4000000000000000ull, 0x4008000000000000ull, 0x4010000000000000ull}, {0x3ff0000000000000ull, 0x4000000000000000ull, 0x4008000000000000ull}, {0x3ff0000000000000ull, 0x3ffbb67ae8584caaull}},
        {{0xbff0000000000000ull, 0x0000000000000000ull, 0x3fd0000000000000ull}, {0x0000000000000000ull, 0x0000000000000000ull, 0x0000000000000000ull}, {0x0000000000000000ull, 0x3ff07e0f66afed07ull}},
    };
    const double a[3] = {0, 0, 0}, b[3] = {1, 2, 3};
    for (const R& r : rows) {
        const double p[3] = {D(r.p[0]), D(r.p[1]), D(r.p[2])};
        double q[3], t, d;
        fun_1404d6b13(a, b, p, q, &t, &d);
        for (int i = 0; i < 3; ++i) CHECK_BITS(q[i], D(r.q[i]));
        CHECK_BITS(t, D(r.r[0]));
        CHECK_BITS(d, D(r.r[1]));
    }
}

TEST_CASE(newton_1404d5a1d_linesearch_1404d6647) {
    // r = (s, t) after 1404d5a1d from st0, then the step of 1404d6647 from st0 along (0.3, -0.2)
    struct R { std::int32_t ierr; U st0[2]; U r[3]; } rows[] = {
        {1, {0x0000000000000000ull, 0x0000000000000000ull}, {0x0000000000000000ull, 0x0000000000000000ull, 0x3f50000000000000ull}},
        {1, {0x3fe0000000000000ull, 0xbfe0000000000000ull}, {0x3fe315e6dde3fe90ull, 0xbfd23e795b8e8a98ull, 0x3f50000000000000ull}},
    };
    const double c1[6] = {0.1, -0.2, 0.3, 1.2, 0.9, -0.4}, c2[6] = {-0.15, 0.25, 0.2, 1.1, -0.3, 0.35};
    const double p[3] = {1.0, 1.0, 0.2};
    for (const R& r : rows) {
        const double s0 = D(r.st0[0]), t0 = D(r.st0[1]);
        double s = s0, t = t0;
        std::int32_t er = 7;
        fun_1404d5a1d(c1, c2, &s, &t, X, p, &er);
        CHECK_EQ(er, r.ierr);
        CHECK_BITS(s, D(r.r[0]));
        CHECK_BITS(t, D(r.r[1]));
        double h = 0;
        const double ds = 0.3, dt = -0.2;
        fun_1404d6647(X, p, &s0, &t0, &ds, &dt, &h);
        CHECK_BITS(h, D(r.r[2]));
    }
}

int main() { return testkit::run_all(); }
