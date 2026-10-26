// ctest-labels: p67-6 contact tier1
// Meshed master contact (14034fc56 / 140434952, GENCTC.md 4.5): hand-built loops, expectations
// derived by hand from the listing (comments give the derivation).
#include <defcmd/ctc2d_ftn.hpp>
#include <defcmd/ctc2d_meshed.hpp>
#include <defcmd/ctc2d_save.hpp>
#include <testkit/check.hpp>

#include <cmath>
#include <cstdint>

using namespace def::cmd::ftn;

namespace {

void reset_all()
{
    reset_ctc2d_save();
    reset_ctc2d_meshed_save();
}

// CCW unit square, nodes 1..4, plus a 5th node: the driver's measuring call reads mxy(iseg + 1)
// with the loop position as node number, so position 4 reads node 5.
double g_sq[10] = {0, 0, 1, 0, 1, 1, 0, 1, 9, 9};
std::int32_t g_sq_list[5] = {1, 2, 3, 4, 1};

// L shape, CCW: reflex corner at node 4 = (1, 1)
double g_l[14] = {0, 0, 2, 0, 2, 1, 1, 1, 1, 2, 0, 2, 9, 9};
std::int32_t g_l_list[7] = {1, 2, 3, 4, 5, 6, 1};

struct Search {
    double foot[2] = {0, 0};
    double dist = 1e10;
    std::int32_t iseg = 0;
    std::int32_t orient = 99;
};

Search search(const double* mxy, double px, double py, std::int32_t n, const std::int32_t* list,
              std::int32_t closed, std::int32_t use_list)
{
    Search r;
    const double p[2] = {px, py};
    const std::int32_t p11 = 2;
    fun_140434952(mxy, p, r.foot, &r.dist, &r.iseg, &n, &r.orient, &closed, list, &use_list,
                  &p11);
    return r;
}

// One slave node (node 1) against the master loop; returns the new xy, codes, changed.
struct Drive {
    double xy[2];
    std::int32_t bc3[3];
    std::int32_t changed = 0;
};

Drive drive(double* mxy, std::int32_t* list, std::int32_t last, double x, double y, double tol,
            std::int32_t c0, std::int32_t c1)
{
    Drive r{{x, y}, {c0, c1, 0}, 0};
    const std::int32_t n = 1;
    const std::int32_t nodes[1] = {1};
    const std::int32_t nloops = 1;
    std::int32_t loops[2] = {1, last};
    const std::int32_t mobj = 3;
    const std::int32_t do_codes = 1;
    fun_14034fc56(r.xy, &n, nodes, mxy, &nloops, loops, list, &mobj, r.bc3, &tol, &r.changed,
                  &do_codes);
    return r;
}

} // namespace

// Point below edge 1-2 (outside): seg 1 inside, h = uy*px - ux*py = 0*0.5 - 1*(-0.01) = 0.01;
// nearest node 0.5001 > 0.01 -> no corner, orient 1, foot (0.5, 0).
TEST_CASE(search_edge_outside)
{
    reset_all();
    const Search r = search(g_sq, 0.5, -0.01, 4, g_sq_list, 1, 1);
    CHECK_EQ(r.iseg, 1);
    CHECK_EQ(r.orient, 1);
    CHECK_BITS(r.dist, 0.01);
    CHECK_BITS(r.foot[0], 0.5);
    CHECK_BITS(r.foot[1], 0.0);
    CHECK_BITS(g_save_140434952.dbest, 0.01);
    CHECK_EQ(g_save_140434952.knear, 1);  // node 1 and 2 tie, strict < keeps the first
}

// Inside, near edge 1-2: h = -0.02 -> orient -1, |d| returned.
TEST_CASE(search_edge_inside)
{
    reset_all();
    const Search r = search(g_sq, 0.5, 0.02, 4, g_sq_list, 1, 1);
    CHECK_EQ(r.iseg, 1);
    CHECK_EQ(r.orient, -1);
    CHECK_BITS(r.dist, 0.02);
    CHECK_BITS(g_save_140434952.dbest, -0.02);
}

// On the edge: d = 0 -> orient 0
TEST_CASE(search_on_edge)
{
    reset_all();
    const Search r = search(g_sq, 0.5, 0.0, 4, g_sq_list, 1, 1);
    CHECK_EQ(r.iseg, 1);
    CHECK_EQ(r.orient, 0);
    CHECK_BITS(r.dist, 0.0);
}

// Outside the convex corner (1, 0): segs 1 and 2 give the same |d| = node distance, the later
// segment wins the tie (<=), dnear == |dbest| -> corner rule: iseg = corner position 2,
// prev (0,0), next (1,1), cross = b0*a1 - a0*b1 = 0*0 - (-1)*1 = 1 > 0 -> orient 1.
TEST_CASE(search_convex_corner)
{
    reset_all();
    const Search r = search(g_sq, 1.01, -0.01, 4, g_sq_list, 1, 1);
    CHECK_EQ(r.iseg, 2);
    CHECK_EQ(r.orient, 1);
    const auto& s = g_save_140434952;
    CHECK_EQ(s.kprev, 1);
    CHECK_EQ(s.knext, 3);
    CHECK_EQ(s.ncorner, 2);
    CHECK_BITS(s.cross, 1.0);
    CHECK_BITS(r.foot[0], 1.0);  // foot of seg 2 (nearer end A)
    CHECK_BITS(r.foot[1], 0.0);
    CHECK_EQ(s.k, 3);
}

// Inside, at the reflex corner (1, 1) of the L: segs 3 and 4 tie at -0.0141..., corner rule
// with prev = node 3 (2,1), next = node 5 (1,2): a = (1,0), b = (0,1),
// cross = 0*0 - 1*1 = -1 < -0.05 -> orient -1.
TEST_CASE(search_reflex_corner)
{
    reset_all();
    const Search r = search(g_l, 0.99, 0.99, 6, g_l_list, 1, 1);
    CHECK_EQ(r.iseg, 4);
    CHECK_EQ(r.orient, -1);
    CHECK_BITS(g_save_140434952.cross, -1.0);
    const double e = 0.99 - 1.0;
    CHECK_BITS(r.dist, std::sqrt(e * e + e * e));
}

// Closed loop wrap: corner at position 1 takes position n as previous (140435536)
TEST_CASE(search_wrap_first_position)
{
    reset_all();
    const Search r = search(g_sq, -0.01, -0.01, 4, g_sq_list, 1, 1);
    CHECK_EQ(r.iseg, 1);
    CHECK_EQ(g_save_140434952.kprev, 4);
    CHECK_EQ(g_save_140434952.nprev, 4);
    CHECK_EQ(g_save_140434952.knext, 2);
    CHECK_EQ(r.orient, 1);  // convex
}

// Open polyline (closed != 1, no list): corner at the first node has no previous node; it is
// made by rotating next - corner = (1, 0) by +1.5707950592041016 (about (0, 1)).
TEST_CASE(search_open_start_rotation)
{
    reset_all();
    double pl[6] = {0, 0, 1, 0, 1, 1};
    const Search r = search(pl, -0.01, -0.01, 3, nullptr, 0, 0);
    const auto& s = g_save_140434952;
    CHECK_EQ(s.nseg, 2);
    CHECK_EQ(r.iseg, 1);
    CHECK_EQ(s.kprev, 0);
    CHECK_BITS(s.ang, 1.5707950592041016);
    CHECK(std::fabs(s.prev[0]) < 1e-5);
    CHECK(std::fabs(s.prev[1] - 1.0) < 1e-5);
    CHECK_BITS(s.next[0], 1.0);
    CHECK_EQ(r.orient, 1);
}

// Open polyline, corner at the last node: next made by rotating (prev node - corner) = (0, -1)
// by -1.5707950592041016 (about (-1, 0)), next about (0, 1).
TEST_CASE(search_open_end_rotation)
{
    reset_all();
    double pl[6] = {0, 0, 1, 0, 1, 1};
    const Search r = search(pl, 1.01, 1.01, 3, nullptr, 0, 0);
    const auto& s = g_save_140434952;
    CHECK_EQ(r.iseg, 3);
    CHECK_EQ(s.knext, 4);
    CHECK_BITS(s.ang, -1.5707950592041016);
    CHECK(std::fabs(s.next[0]) < 1e-5);
    CHECK(std::fabs(s.next[1] - 1.0) < 1e-5);
    CHECK_EQ(r.orient, 1);
}

// Driver: node outside within tol -> moved to the foot, code y set to -mobj
TEST_CASE(drive_contact_outside)
{
    reset_all();
    const Drive r = drive(g_sq, g_sq_list, 5, 0.5, -0.01, 0.05, 0, 0);
    CHECK_EQ(r.changed, 1);
    CHECK_BITS(r.xy[0], 0.5);
    CHECK_BITS(r.xy[1], 0.0);
    CHECK_EQ(r.bc3[0], 0);
    CHECK_EQ(r.bc3[1], -3);
    CHECK_EQ(g_save_14034fc56.bseg, 1);
    CHECK_EQ(g_save_14034fc56.count, 4);
    CHECK_BITS(g_save_14034fc56.dbest, 0.01);
}

// Driver: penetrating node (d = -0.02, |d| <= tol) -> moved; c1 < 0 and c0 < 1: c0 = c1, c1 = K
TEST_CASE(drive_contact_inside)
{
    reset_all();
    const Drive r = drive(g_sq, g_sq_list, 5, 0.5, 0.02, 0.05, 0, -1);
    CHECK_EQ(r.changed, 1);
    CHECK_BITS(r.xy[1], 0.0);
    CHECK_EQ(r.bc3[0], -1);
    CHECK_EQ(r.bc3[1], -3);
    CHECK_BITS(g_save_14034fc56.dbest, -0.02);
}

// Driver: beyond tolerance -> nothing changes
TEST_CASE(drive_beyond_tol)
{
    reset_all();
    const Drive r = drive(g_sq, g_sq_list, 5, 0.5, -0.2, 0.05, 0, 0);
    CHECK_EQ(r.changed, 0);
    CHECK_BITS(r.xy[0], 0.5);
    CHECK_BITS(r.xy[1], -0.2);
    CHECK_EQ(r.bc3[1], 0);
}

// Driver: convex corner, final move with mnodes(2), mnodes(3) onto end A = (1, 0);
// codes: c1 > 0, c0 < 1 -> c0 = K
TEST_CASE(drive_convex_corner)
{
    reset_all();
    const Drive r = drive(g_sq, g_sq_list, 5, 1.01, -0.01, 0.05, 0, 2);
    CHECK_EQ(r.changed, 1);
    CHECK_BITS(r.xy[0], 1.0);
    CHECK_BITS(r.xy[1], 0.0);
    CHECK_EQ(r.bc3[0], -3);
    CHECK_EQ(r.bc3[1], 2);
}

// Driver: reflex corner from inside: orient -1 makes the best distance -|d|; moved to (1, 1)
TEST_CASE(drive_reflex_corner)
{
    reset_all();
    const Drive r = drive(g_l, g_l_list, 7, 0.99, 0.99, 0.05, -3, 0);
    CHECK_EQ(r.changed, 1);
    CHECK_BITS(r.xy[0], 1.0);
    CHECK_BITS(r.xy[1], 1.0);
    CHECK_EQ(g_save_14034fc56.orient, -1);
    CHECK(g_save_14034fc56.dbest < 0.0);
    CHECK_EQ(r.bc3[0], -3);  // c0 == K: codes unchanged
    CHECK_EQ(r.bc3[1], 0);
}

// Driver: both codes positive -> warning 1001, codes unchanged, node still moved
TEST_CASE(drive_codes_locked)
{
    reset_all();
    const Drive r = drive(g_sq, g_sq_list, 5, 0.5, -0.01, 0.05, 1, 1);
    CHECK_EQ(r.changed, 1);
    CHECK_BITS(r.xy[1], 0.0);
    CHECK_EQ(r.bc3[0], 1);
    CHECK_EQ(r.bc3[1], 1);
}

int main() { return testkit::run_all(); }
