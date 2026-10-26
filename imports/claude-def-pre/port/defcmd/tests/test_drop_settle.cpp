// ctest-labels: p67-3 drop
// Tier-1 test of the OBJPOS drop equilibrium test 140369c19 (packet p67-3, worker D): the paths
// that run without the FTN_geoutl kernels (1404cf5c0 / 1404ded0c are still stubs) and without
// contact detection: contact list, early returns, the nk >= 2 stop and the penetration return of
// the moment test. Expected values are hand-calculated from the listing (comments per case).
// The step search 14036d3fd always calls the detection or the initial drop; it is covered by
// the lead's integration test.
#include <defcmd/drop/globals_d.hpp>
#include <defcmd/drop/settle.hpp>
#include <testkit/check.hpp>

#include <cmath>
#include <limits>
#include <vector>

using namespace def::cmd::drop;

namespace {

struct Case {
    std::vector<double> x;
    std::vector<i32> bc;
    std::vector<double> nrm;
    std::vector<double> gap;
    std::vector<i32> l840;
    double d[3] = {0.0, 0.0, -1.0};
    double k[18] = {};
    double p15 = 0.002;
    i32 nn = 0;
    i32 nk = 0;
    i32 stop = 7;  // must be reset to 0 at entry
    i32 shift = 7; // must be reset to 0 at entry
    i32 verbose = 0;

    explicit Case(i32 n) : x(3 * n, 1.5), bc(3 * n, 0), nrm(9 * n, 0.0), gap(n, 0.0), l840(n, -9), nn(n) {}

    void run()
    {
        i32 zi = 0;
        double zd = 0.0;
        fun_140369c19(nullptr, nullptr, x.data(), nullptr, nullptr, nullptr, nullptr, bc.data(),
                      nullptr, nullptr, nrm.data(), nullptr, nullptr, nullptr, &zi, &zi, &nn,
                      nullptr, gap.data(), nullptr, d, &zd, &zd, &zd, &p15, &stop, &nk, k,
                      l840.data(), &shift, &verbose);
    }
};

} // namespace

// nn = 0: no contact node (0x140369c71: *nn < 1 -> return); stop, shift reset to 0
TEST_CASE(eqm_no_nodes)
{
    reset_state();
    reset_globals_d();
    Case c(1);
    c.nn = 0;
    c.run();
    CHECK_EQ(c.stop, 0);
    CHECK_EQ(c.shift, 0);
    CHECK_EQ(c.l840[0], -9);
}

// codes >= 0 everywhere: cnt = 0 -> return
TEST_CASE(eqm_no_contact)
{
    reset_state();
    Case c(3);
    c.bc = {0, 0, 0, 1, 2, 3, 0, 5, 0};
    c.run();
    CHECK_EQ(c.stop, 0);
    CHECK_EQ(c.shift, 0);
}

// cnt = 2, nk = 0: fewer than 3 contacts without constraint -> return (0x140369d5a)
TEST_CASE(eqm_two_contacts_free)
{
    reset_state();
    Case c(3);
    c.bc = {0, 0, 0, -2, 0, 0, 0, 0, -2};
    c.nk = 0;
    c.run();
    CHECK_EQ(c.stop, 0);
    CHECK_EQ(c.shift, 0);
    CHECK_EQ(c.l840[0], 2);
    CHECK_EQ(c.l840[1], 3);
}

// cnt = 1, nk = 1: one contact on a symmetry plane -> return (0x14036cc66: cnt > 1 required)
TEST_CASE(eqm_one_contact_plane)
{
    reset_state();
    Case c(2);
    c.bc = {0, -2, 0, 0, 0, 0};
    c.nk = 1;
    c.run();
    CHECK_EQ(c.stop, 0);
    CHECK_EQ(c.l840[0], 1);
}

// nk = 2 and nk = 3: any contact stops (0x140369d69). Contact list: a node is in the list when
// any of its three codes is negative, in node order: nodes 2 (slot 2), 3 (slot 3), 4 (slot 1).
TEST_CASE(eqm_constrained_stop)
{
    for (i32 nk : {2, 3}) {
        reset_state();
        Case c(4);
        c.bc = {0, 0, 0, 0, -2, 0, 0, 0, -2, -2, 0, 0};
        c.nk = nk;
        c.run();
        CHECK_EQ(c.stop, 1);
        CHECK_EQ(c.shift, 0);
        CHECK_EQ(c.l840[0], 2);
        CHECK_EQ(c.l840[1], 3);
        CHECK_EQ(c.l840[2], 4);
        CHECK_EQ(c.l840[3], -9);
    }
}

// Moment test, penetration return: reached directly for nk < 0 (neither side-test block runs;
// unreachable from OBJPOS, used here because both blocks call 1404cf5c0). gmin over the list in
// order, strict <, start 1e10: gaps of nodes 1, 2, 3 = 0.5, -0.01, -0.003 -> gmin = -0.01;
// lim = p15 * 2.0 = 0.004; gmin < 0 and |gmin| = 0.01 > 0.004 -> return before 1404cf5c0:
// stop = shift = 0 and x untouched.
TEST_CASE(eqm_too_deep_returns)
{
    reset_state();
    Case c(3);
    c.bc = {-2, 0, 0, 0, -2, 0, 0, 0, -2};
    c.gap = {0.5, -0.01, -0.003};
    c.nk = -1;
    c.run();
    CHECK_EQ(c.stop, 0);
    CHECK_EQ(c.shift, 0);
    for (double v : c.x) CHECK_BITS(v, 1.5);
}

// Same with a NaN gap first: NaN < 1e10 is false (COMISD + JP), so gmin = -0.01 again -> return.
// Node 2 not in contact: its gap (-1.0) is not looked at.
TEST_CASE(eqm_nan_gap_skipped)
{
    reset_state();
    Case c(3);
    c.bc = {-2, 0, 0, 0, 0, 0, 0, -2, 0};
    c.gap = {std::numeric_limits<double>::quiet_NaN(), -1.0, -0.01};
    c.nk = -1;
    c.run();
    CHECK_EQ(c.stop, 0);
    CHECK_EQ(c.shift, 0);
    CHECK_EQ(c.l840[0], 1);
    CHECK_EQ(c.l840[1], 3);
}

int main() { return testkit::run_all(); }
