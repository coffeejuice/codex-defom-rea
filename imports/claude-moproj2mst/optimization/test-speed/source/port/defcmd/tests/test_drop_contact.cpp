// ctest-labels: p67-3 drop
// Tier-1 tests of the drop contact routines (packet p67-3, worker B).
//  * 140063ab3, 1400647a0 (paths without the 140065ef8 fall-back), 140066698, 140066902,
//    14007a47d: golden bits from the ORIGINAL routines, called in a copy of DEF_PRE_64.exe mapped
//    at its preferred base (harness deform-rea-work/scripts_p673/b/raygold.cpp, built /Od so the
//    harness keeps nothing in XMM6-15, which the PathScale code does not preserve).
//  * 140370a26, 140371339: hand-built arrays, expected values calculated by hand below (all
//    intermediate values exact in binary floating point).
// Routines that call the cube library or FTN_geoutl kernels (14001f54f, 140064f58, 14007ff21,
// 1400653e5, 140065ef8, 14007f8c4, 140373650, 140374252, 140374954) wait for those libraries.
#include <defcmd/drop/contact.hpp>
#include <defcmd/drop/state.hpp>
#include <defcmd/genctc3d_ray.hpp>
#include <testkit/check.hpp>

#include <bit>
#include <cstdint>

using namespace def::cmd;
using drop::i32;

namespace {
double bits(std::uint64_t u) { return std::bit_cast<double>(u); }
} // namespace

TEST_CASE(ray_plane_140063ab3_golden)
{
    double P[3] = {0.3, -1.7, 5.25}, dir[3] = {0.1, 0.2, -0.97}, n[3] = {0.05, -0.1, 0.99};
    double X1[3] = {1.1, 0.4, -0.3};
    double Q[3] = {0, 0, 0}, d = 0;
    i32 ok = -1;
    ctc3d::fun_140063ab3(P, dir, n, X1, Q, &d, &ok);
    CHECK_EQ(ok, 1);
    CHECK_BITS(Q[0], bits(0x3fec2f7a5d4ebd60ull));
    CHECK_BITS(Q[1], bits(0xbfe13aa4defc1ed8ull));
    CHECK_BITS(Q[2], bits(0xbfd88ed53b2082d0ull));
    CHECK_BITS(d, bits(0x40173b58f4a26cb8ull));
    // parallel ray: ok = 0, outputs untouched
    double dpar[3] = {1.0, 0.0, 0.0}, npar[3] = {0.0, 0.0, 1.0};
    double Q2[3] = {7, 7, 7}, d2 = 7;
    ctc3d::fun_140063ab3(P, dpar, npar, X1, Q2, &d2, &ok);
    CHECK_EQ(ok, 0);
    CHECK_BITS(d2, 7.0);
}

TEST_CASE(ray_triangle_1400647a0_golden)
{
    double X[9] = {0.0, 0.0, 0.0, 2.0, 0.1, 0.05, 0.2, 1.9, -0.03};
    double n[3] = {-0.02, 0.01, 0.9997};
    double P[3] = {0.7, 0.45, 3.3}, dir[3] = {0.013, -0.021, -0.9996};
    const std::uint64_t hit_bits[6] = {0x400a52772c37a4f5ull, 0x3fddc12c1ab42a16ull,
                                       0x3fd69acc12099476ull, 0x3fe7c4ccda5d0168ull,
                                       0x3fd860bcdffed800ull, 0x3f86a13f035c3c00ull};
    for (i32 mode = 0; mode <= 2; ++mode) { // the binary gives the same bits in modes 0, 1, 2
        double d = 10.0, s = -9, t = -9, Q[3] = {-9, -9, -9};
        i32 hit = -1, bad = 0;
        ctc3d::fun_1400647a0(P, dir, n, X, &d, &s, &t, Q, &hit, &mode, &bad);
        CHECK_EQ(hit, 1);
        CHECK_EQ(bad, 0);
        CHECK_BITS(d, bits(hit_bits[0]));
        CHECK_BITS(s, bits(hit_bits[1]));
        CHECK_BITS(t, bits(hit_bits[2]));
        CHECK_BITS(Q[0], bits(hit_bits[3]));
        CHECK_BITS(Q[1], bits(hit_bits[4]));
        CHECK_BITS(Q[2], bits(hit_bits[5]));
    }
    { // outside the triangle: hit 0, outputs untouched
        double P2[3] = {2.5, 2.5, 3.0};
        i32 mode = 1, hit = -1, bad = 0;
        double d = 10.0, s = -9, t = -9, Q[3] = {-9, -9, -9};
        ctc3d::fun_1400647a0(P2, dir, n, X, &d, &s, &t, Q, &hit, &mode, &bad);
        CHECK_EQ(hit, 0);
        CHECK_BITS(d, 10.0);
        CHECK_BITS(s, -9.0);
        CHECK_BITS(Q[2], -9.0);
    }
    { // mode 1, |dist| > |d|: hit 0
        i32 mode = 1, hit = -1, bad = 0;
        double d = 1.0, s = -9, t = -9, Q[3] = {-9, -9, -9};
        ctc3d::fun_1400647a0(P, dir, n, X, &d, &s, &t, Q, &hit, &mode, &bad);
        CHECK_EQ(hit, 0);
        CHECK_BITS(d, 1.0);
    }
    { // |b1| <= 1e-11 branch
        double X3[9] = {0.0, 0.0, 0.0, 0.0, 2.0, 0.1, 1.5, 0.3, 0.05};
        double n3[3] = {0.0, -0.05, 0.998};
        double P3[3] = {0.4, 0.6, -2.0}, dir3[3] = {0.0, 0.01, 1.0};
        i32 mode = 2, hit = -1, bad = 0;
        double d = 10.0, s = -9, t = -9, Q[3] = {-9, -9, -9};
        ctc3d::fun_1400647a0(P3, dir3, n3, X3, &d, &s, &t, Q, &hit, &mode, &bad);
        CHECK_EQ(hit, 1);
        CHECK_BITS(d, bits(0x40003fa5a98303faull));
        CHECK_BITS(s, bits(0x3fdda4b51b5a552dull));
        CHECK_BITS(t, bits(0x3fd14a39d39499c2ull));
        CHECK_BITS(Q[0], bits(0x3fd999999999999aull));
        CHECK_BITS(Q[1], bits(0x3fe3d995fc8a5c51ull));
        CHECK_BITS(Q[2], bits(0x3f9fd2d4c181fd00ull));
    }
}

TEST_CASE(edge_helpers_140066698_140066902_14007a47d_golden)
{
    double A[6] = {0.1, 0.2, 0.7, 1.3, 0.9, -0.2}, B[6] = {1.0, -0.5, 0.3, 0.2, 1.4, 0.8};
    double out[3] = {-9, -9, -9}, par[2] = {-9, -9};
    i32 n = 3, ok = -1;
    ctc3d::fun_140066698(A, B, out, par, &n, &ok);
    CHECK_EQ(ok, 1);
    CHECK_BITS(out[0], bits(0x3fe2bfd1d90f24c7ull));
    CHECK_BITS(out[1], bits(0x3fdef0db38f8114full));
    CHECK_BITS(out[2], bits(0x3ff06212bfd1d90full)); // r * b1 + A(3): the binary's third "direction"
    CHECK_BITS(par[0], bits(0x3fd9ea5dbf193d4cull));
    CHECK_BITS(par[1], bits(0x3fe09039b0ad1207ull));
    i32 ok2 = -1;
    ctc3d::fun_140066902(&par[0], &par[1], &ok2);
    CHECK_EQ(ok2, 1);
    CHECK_BITS(par[0], bits(0x3fd9ea5dbf193d4cull));
    double a = -5e-9, b = 1.000000005;
    i32 ok3 = -1;
    ctc3d::fun_140066902(&a, &b, &ok3);
    CHECK_EQ(ok3, 1);
    CHECK_BITS(a, 0.0);
    CHECK_BITS(b, 1.0);
    double a4 = -2e-8, b4 = 0.5;
    i32 ok4 = -1;
    ctc3d::fun_140066902(&a4, &b4, &ok4); // beyond the snap band: not snapped, not inside
    CHECK_EQ(ok4, 0);
    CHECK_BITS(a4, -2e-8);
    double p[3] = {0.3, -1.1, 2.0}, q[3] = {1.7, 0.25, -0.6}, dist = -1;
    ctc3d::fun_14007a47d(p, q, &dist);
    CHECK_BITS(dist, bits(0x4009f9b20ab29d96ull));
    double z = -1;
    ctc3d::fun_14007a47d(p, p, &z);
    CHECK_BITS(z, 0.0);
}

namespace {
// One free body (body 1) with two nodes and a fixed body: quad face 1 (z = 0, 0..2 x 0..2) and
// triangle face 2 (nodes 1, 2, 3, 1).
struct Scene {
    double rt[200] = {};
    double pt[20] = {};
    i32 it[20] = {};
    double x[6] = {0.5, 0.5, 0.25, 1.5, 0.5, -0.125};
    double xu[12] = {0, 0, 0, 2, 0, 0, 2, 2, 0, 0, 2, 0};
    i32 f[4] = {};
    i32 fu[8] = {1, 2, 3, 4, 1, 2, 3, 1};
    i32 bc[6] = {-2, 0, 0, -2, 0, -2};
    i32 sw[12] = {};
    i32 fid[6] = {1, 0, 0, 0, 0, 2};
    double nrm[18] = {};
    double xe[12] = {};
    i32 flags[2] = {1, 2};
    double vn[6] = {1.0, 0.0, 0.0, 0.0, 0.0, 0.0};
    i32 nb = 1, nb2 = 1;
    i32 nodes[2] = {1, 2};
    Scene()
    {
        it[1] = 1; // free
        it[8] = 1; // RT group range 1..1
        it[9] = 1;
        rt[1] = 2.0; // node count
        rt[2] = 1.0; // first node
        nrm[2] = -1.0;       // NRM(:,1,1) = (0, 0, -1)
        nrm[9 + 6 + 2] = -1.0; // NRM(:,3,2)
        xe[0] = -0.5;        // XE(:,1,1)
        xe[1] = -0.5;
        xe[6 + 4] = 0.25;    // XE(:,3,2)
        xe[6 + 5] = 0.5;
    }
};
} // namespace

TEST_CASE(gap_140370a26_hand)
{
    Scene s;
    double gap[2] = {7, 7};
    drop::fun_140370a26(s.rt, s.pt, s.x, s.xu, s.it, s.f, s.fu, s.bc, s.sw, s.fid, s.nrm, s.xe,
                        s.flags, s.vn, gap, &s.nb, &s.nb2, s.nodes);
    // node 1: quad at (-0.5, -0.5): a = 1.5*0.25 = 0.375, b = 0.5*0.25 = 0.125;
    // N = (0.375*1.5, 0.125*1.5, 0.5*0.125, 0.5*0.375) = (0.5625, 0.1875, 0.0625, 0.1875);
    // Q = (0.5, 0.5, 0); D = Q - X = (0, 0, -0.25), |D| = 0.25, D·N = 0.25 > 0 and > 1e-5:
    // released (BC(1,1) = 0), GAP = +0.25
    CHECK_EQ(s.bc[0], 0);
    CHECK_BITS(gap[0], 0.25);
    // node 2: slot 3 (last negative), triangle face 2 at (0.25, 0.5): N = (0.25, 0.5, 0.25),
    // Q = (1.5, 0.5, 0); D = (0, 0, 0.125), D·N = -0.125: stays in contact, GAP = -0.125
    CHECK_EQ(s.bc[3], -2);
    CHECK_EQ(s.bc[5], -2);
    CHECK_BITS(gap[1], -0.125);
}

TEST_CASE(forces_140371339_hand)
{
    Scene s;
    s.bc[3] = s.bc[5] = 0; // only node 1 in contact
    s.x[0] = 1.0;          // X(:,1) = (1, 0.5, 0.25)
    drop::reset_state();
    drop::g_body.c[0] = 0.5;
    drop::g_body.c[1] = 0.5;
    drop::g_body.c[2] = 1.0;
    double f6[6] = {1, 1, 1, 1, 1, 1};
    double w = 10.0, d[3] = {0.0, 0.0, -1.0}, kc = 20.0, damp = 0.0, p15 = 0.002;
    double fn[12] = {};
    drop::fun_140371339(s.rt, s.pt, s.x, s.xu, s.it, s.f, s.fu, s.bc, s.sw, s.fid, s.nrm, s.xe,
                        s.flags, s.vn, &s.nb, &s.nb2, s.nodes, f6, &w, d, &kc, &damp, &p15, fn);
    // fm = 20*0.1 = 2.0; N = (0,0,-1): Fn = -(N*fm) = (-0, -0, 2); friction term 0*fm = 0.
    // r = X - c = (0.5, 0, -0.75); M = r x Fn = (0, -(2*0.5), 0) = (0, -1, 0)
    CHECK(fn[0] == 0.0);
    CHECK(fn[1] == 0.0);
    CHECK_BITS(fn[2], 2.0);
    CHECK(fn[3] == 0.0);
    CHECK_BITS(fn[4], -1.0);
    CHECK(fn[5] == 0.0);
    // |S·d| = 2, scale W/2 = 5: F6 = S*5 + 1 = (1, 1, 11, 1, -4, 1)
    CHECK(f6[0] == 1.0);
    CHECK(f6[1] == 1.0);
    CHECK_BITS(f6[2], 11.0);
    CHECK(f6[3] == 1.0);
    CHECK_BITS(f6[4], -4.0);
    CHECK(f6[5] == 1.0);
    // node 2 has no contact: FN(:,2) untouched
    CHECK_BITS(fn[6], 0.0);
    drop::reset_state();
}

int main() { return testkit::run_all(); }
