// ctest-labels: p67-7 ctc3d
// Tier-1 tests of the GENCTC 3D decision group (packet p67-7, group "decide") on hand-made inputs.
// Only routines (or early paths) that do not reach the cube library are exercised here; expected
// values are worked out from the listings by hand (comments).
#include <defcmd/ctc3d/common.hpp>
#include <defcmd/ctc3d/decide.hpp>
#include <testkit/check.hpp>

using namespace def::cmd::ctc3d;

// 14004c429: pairs (1,2), (2,3), (1,3) of equal negative codes
TEST_CASE(c429_pairs)
{
    reset_image();
    i32 obj = 77;
    i32 nd = -5;
    i32 pos = -5;
    {
        i32 c[3] = {-2, -2, 0};
        i32 r = 1;
        fun_14004c429(c, &obj, &nd, &r, &pos);
        CHECK_EQ(nd, 1);
        CHECK_EQ(obj, 2);
        CHECK_EQ(pos, 1);
        r = 2;
        fun_14004c429(c, &obj, &nd, &r, &pos);
        CHECK_EQ(pos, 2);
        r = 3; // r not in the pair
        fun_14004c429(c, &obj, &nd, &r, &pos);
        CHECK_EQ(pos, 0);
    }
    {
        i32 c[3] = {-1, -3, -3};
        i32 r = 3;
        obj = 0;
        fun_14004c429(c, &obj, &nd, &r, &pos);
        CHECK_EQ(nd, 1);
        CHECK_EQ(obj, 3);
        CHECK_EQ(pos, 2);
    }
    {
        // all three equal: three pairs, count 3, pos from the last matching pair (1,3): r=3 -> 2
        i32 c[3] = {-4, -4, -4};
        i32 r = 3;
        fun_14004c429(c, &obj, &nd, &r, &pos);
        CHECK_EQ(nd, 3);
        CHECK_EQ(obj, 4);
        CHECK_EQ(pos, 2);
    }
    {
        // (1,3) equal and (1,2)... impossible to get count 2 without all equal; count 2 needs two
        // pairs: c1 == c2 and c2 == c3 imply c1 == c3. Equal positive codes are no pair.
        i32 c[3] = {1, 1, 1};
        i32 r = 2;
        obj = 9;
        fun_14004c429(c, &obj, &nd, &r, &pos);
        CHECK_EQ(nd, 0);
        CHECK_EQ(pos, 0);
        CHECK_EQ(obj, 9);
    }
}

// 14003abfd: code of node list(1) shared by all neighbours list(2..n)
TEST_CASE(abfd_shared_code)
{
    reset_image();
    // 4 nodes, ndof 3, BCCDEF rows
    i32 bc[12] = {1, 0, 0,  //
                  1, 0, -2, //
                  1, 5, 0,  //
                  0, 0, -2};
    i32 nd = 3;
    i32 code = -9;
    i32 dir = -9;
    {
        i32 list[3] = {1, 2, 3}; // node 1 (x fixed), neighbours 2 and 3 both x fixed
        i32 n = 3;
        fun_14003abfd(bc, list, &n, &nd, &code, &dir);
        CHECK_EQ(code, 1);
        CHECK_EQ(dir, 1);
    }
    {
        i32 list[3] = {1, 2, 4}; // node 4 not x fixed: not shared
        i32 n = 3;
        fun_14003abfd(bc, list, &n, &nd, &code, &dir);
        CHECK_EQ(code, 0);
        CHECK_EQ(dir, 0);
    }
    {
        i32 list[2] = {4, 2}; // contact code -2 found in any direction of node 2
        i32 n = 2;
        fun_14003abfd(bc, list, &n, &nd, &code, &dir);
        CHECK_EQ(code, -2);
        CHECK_EQ(dir, 0);
    }
    {
        // single-node list: n == 1 returns the first nonzero code at once (dir = the stale
        // static 140655a08 for code 1; 1 from the first case above)
        i32 list[1] = {1};
        i32 n = 1;
        fun_14003abfd(bc, list, &n, &nd, &code, &dir);
        CHECK_EQ(code, 1);
        CHECK_EQ(dir, 1);
    }
    {
        i32 zero[3] = {0, 0, 0};
        i32 list[2] = {1, 1};
        i32 n = 2;
        fun_14003abfd(zero, list, &n, &nd, &code, &dir);
        CHECK_EQ(code, 0);
        CHECK_EQ(dir, 0);
    }
}

// 140046625: drop the slots of the evicted object and compact Q
TEST_CASE(e6625_evict)
{
    reset_image();
    i32 evict = 3;
    i32 ni4[6] = {-1, 0, -3, 11, 0, 33};
    i32 codes[3] = {-1, 0, -3};
    i32 dirs[2] = {3, 1}; // slot 1 -> z (object 3), slot 2 -> x (object 1)
    i32 nacc = 2;
    double q[9] = {30, 31, 32, 10, 11, 12, 90, 91, 92};
    fun_140046625(&evict, ni4, codes, dirs, &nacc, q);
    CHECK_EQ(nacc, 1);
    CHECK_EQ(evict, 0);
    CHECK_EQ(codes[2], 0);
    CHECK_EQ(codes[0], -1);
    CHECK_EQ(ni4[2], 0);
    CHECK_EQ(ni4[5], 0);
    CHECK_EQ(ni4[0], -1);
    CHECK_EQ(dirs[0], 1);
    CHECK_EQ(q[0], 10.0); // slot 2 moved to slot 1
    CHECK_EQ(q[2], 12.0);
    CHECK_EQ(q[3], 90.0); // Q(nacc+1) = Q(3) moved to Q(m+1) = Q(2)
    CHECK_EQ(q[5], 92.0);
}

// Early returns that do not reach the cube library
TEST_CASE(early_returns)
{
    reset_image();
    {
        // 14005266d: no contact code -> Pn unchanged
        i32 obj = 2, inst = 1, node = 7;
        double x[3] = {1, 2, 3};
        double pn[3] = {4, 5, 6};
        i32 codes[3] = {1, 0, 1};
        fun_14005266d(&obj, &inst, x, pn, codes, &node);
        CHECK_EQ(pn[0], 4.0);
        CHECK_EQ(pn[1], 5.0);
        CHECK_EQ(pn[2], 6.0);
        CHECK_EQ(m<i32>(0x1406767bc), 2); // nfix
        CHECK_EQ(m<i32>(0x1406767c0), 0); // nneg
        CHECK_EQ(m<i32>(0x1406767d0), 1);
        CHECK_EQ(m<i32>(0x1406767d4), 3);
    }
    {
        // 14004522c: fewer than two contact codes -> nothing, *changed = 0
        i32 obj = 2, inst = 1, node = 7, changed = 5;
        double x[3] = {0, 0, 0};
        double tol = 1.0;
        i32 row[3] = {-1, 0, 1};
        fun_14004522c(&obj, &inst, x, row, &node, &changed, &tol);
        CHECK_EQ(changed, 0);
        CHECK_EQ(m<i32>(0x14066e368), 1);
        i32 row3[3] = {-1, -2, -3};
        fun_14004522c(&obj, &inst, x, row3, &node, &changed, &tol);
        CHECK_EQ(m<i32>(0x14066e368), 3);
    }
    {
        // 14004ad46: two contact codes -> return before any search, *sep = 0
        i32 p = 1, e = 1, obj = 2, inst = 1, poly = 4, ndof = 3, dir = 3, node = 2, sep = 9;
        i32 bc[6] = {0, 0, 0, -1, 0, -3};
        double x[3] = {0, 0, 0}, q[3] = {0, 0, 1}, lc[3] = {0, 0, 0};
        double len = 1.0;
        fun_14004ad46(&p, &e, &obj, &inst, x, q, lc, &poly, bc, &ndof, &dir, &node, &len, &sep);
        CHECK_EQ(sep, 0);
        CHECK_EQ(p, 1);
        CHECK_EQ(m<i32>(0x140670ebc), 2);
        CHECK_EQ(m<i32>(0x140670ec0), 1); // scan stopped at direction x
    }
    {
        // 140049b2d: no negative code -> nothing (flag cleared)
        i32 obj = 2, inst = 1, ndof = 3, node = 1, dbg = 0, flag = 7, sep = 0;
        i32 bc[3] = {1, 0, 0};
        double x[3] = {1, 2, 3}, p[3] = {9, 9, 9};
        double len = 1.0;
        fun_140049b2d(x, &obj, &inst, bc, &ndof, &node, p, &len, &dbg, &flag, &sep);
        CHECK_EQ(flag, 0);
        CHECK_EQ(p[0], 9.0);
    }
    {
        // 140040d77: all three codes set -> nothing
        i32 obj = 2, inst = 1, ndof = 3, node = 1, dbg = 0, flag = 0, mode = 0, nacc = 4, sep = 0;
        i32 bc[3] = {1, 1, -1};
        double P[3] = {1, 2, 3}, pn[3] = {0, 0, 0}, tol = 1.0;
        i32 codes[3] = {0, 0, 0};
        fun_140040d77(P, &obj, &inst, bc, &ndof, &node, &dbg, &flag, &tol, pn, codes, &mode, &nacc, &sep);
        CHECK_EQ(nacc, 0);
        CHECK_EQ(pn[0], 0.0);
    }
}

int main() { return testkit::run_all(); }
