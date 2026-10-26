// ctest-labels: p67-7 srcnt4
// SRCNT4 subtree (GENCTC_3d_srcnt4.md): the parts that run without the cube library bodies.
// Expected values are hand calculations (min/max/add/sub only, exact in double).
#include <defcmd/genctc3d.hpp>
#include <defrt/ftn/fio.hpp>
#include <testkit/check.hpp>

#include <cmath>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <limits>
#include <string>

#include <process.h>

using namespace def::cmd::ctc3d;
namespace fs = std::filesystem;

TEST_CASE(a4be_shared_negative_code)
{
    reset_image();
    i32 a[3] = {0, -3, 5};
    i32 b[3] = {-3, 0, 0};
    i32 found = 7;
    fun_14003a4be(a, b, &found);
    CHECK_EQ(found, 1);
    CHECK_EQ(m<i32>(0x140655964), 2); // i at the match
    CHECK_EQ(m<i32>(0x140655968), 1); // j at the match

    i32 c[3] = {4, 0, -2};
    i32 d[3] = {4, 0, 2}; // equal but not negative, and -2 vs 2
    fun_14003a4be(c, d, &found);
    CHECK_EQ(found, 0);
    CHECK_EQ(m<i32>(0x140655964), 4);
    CHECK_EQ(m<i32>(0x140655968), 4);
}

TEST_CASE(ae1_init_accumulate_widen)
{
    reset_image();
    double box[6] = {0, 0, 0, 0, 0, 0};
    double x[9] = {1.0, -2.0, 3.0, /**/ 4.0, 5.0, -6.0, /**/ 0.5, 0.25, 0.125};
    double margin = 0.25;
    i32 n = 1;
    i32 mode = 1;
    i32 idx1[1] = {1};
    fun_140030ae1(x, box, &margin, &n, idx1, &mode);
    CHECK_EQ(box[0], 10000.0);
    CHECK_EQ(box[2], 10000.0);
    CHECK_EQ(box[3], -10000.0);
    CHECK_EQ(box[5], -10000.0);
    CHECK_EQ(m<i32>(0x14064f5d0), 4);

    // mode 0: points |idx|, 0 skipped, no margin
    i32 idx[3] = {-2, 0, 1};
    n = 3;
    mode = 0;
    fun_140030ae1(x, box, &margin, &n, idx, &mode);
    CHECK_EQ(box[0], 1.0);
    CHECK_EQ(box[1], -2.0);
    CHECK_EQ(box[2], -6.0);
    CHECK_EQ(box[3], 4.0);
    CHECK_EQ(box[4], 5.0);
    CHECK_EQ(box[5], 3.0);
    CHECK_EQ(m<i32>(0x14064f5d4), 4);
    CHECK_EQ(m<i32>(0x14064f5d8), 1);

    // mode -1: point 3 added, then widened by the margin
    i32 idx3[1] = {3};
    n = 1;
    mode = -1;
    fun_140030ae1(x, box, &margin, &n, idx3, &mode);
    CHECK_EQ(box[0], 0.25);
    CHECK_EQ(box[1], -2.25);
    CHECK_EQ(box[2], -6.25);
    CHECK_EQ(box[3], 4.25);
    CHECK_EQ(box[4], 5.25);
    CHECK_EQ(box[5], 3.25);
}

TEST_CASE(ae1_nan_coordinate_ignored)
{
    reset_image();
    const double nan = std::numeric_limits<double>::quiet_NaN();
    double box[6] = {1.0, 1.0, 1.0, 2.0, 2.0, 2.0};
    double x[3] = {nan, 0.0, 3.0};
    double margin = 0.0;
    i32 n = 1;
    i32 mode = 0;
    i32 idx[1] = {1};
    fun_140030ae1(x, box, &margin, &n, idx, &mode);
    CHECK_EQ(box[0], 1.0); // SETA is false for an unordered compare
    CHECK_EQ(box[3], 2.0);
    CHECK_EQ(box[1], 0.0);
    CHECK_EQ(box[5], 3.0);
}

namespace {

fs::path temp_dir()
{
    const fs::path d = fs::temp_directory_path() / ("defport_srcnt4_" + std::to_string(_getpid()));
    fs::create_directories(d);
    return d;
}

void call_srcnt4()
{
    i32 obj = 2, inst = 1, ndof = 3, nn = 0, nf = 0, p9 = 0, dbg = 0, dobj = 0, dinst = 0, dpoly = 0;
    i32 bcc[3] = {0, 0, 0};
    double xx[3] = {0, 0, 0};
    double tol = 0.1;
    fun_14002f091(&obj, &inst, bcc, &ndof, &nn, &nf, xx, &tol, &p9, &dbg, &dobj, &dinst, &dpoly);
}

} // namespace

TEST_CASE(srcnt4_skip_pntr_and_flag)
{
    const fs::path old = fs::current_path();
    const fs::path d = temp_dir();
    fs::current_path(d);

    // SKIP_PNTR.DAT present: skipped before any work
    reset_image();
    { std::ofstream(d / "SKIP_PNTR.DAT") << "x\n"; }
    m<i32>(0x14064f380) = 77;
    m<i32>(0x14064f3e0) = 55;
    call_srcnt4();
    CHECK_EQ(m<i32>(0x14064f380), 77);
    CHECK_EQ(m<i32>(0x14064f3e0), 55);
    def::rt::ftn::fio::reset(); // unit 61 is left open by SRCNT4
    fs::remove(d / "SKIP_PNTR.DAT");

    // no file but DAT_1405c16fc == 1: skipped as well
    reset_image();
    m<i32>(0x1405c16fc) = 1;
    m<i32>(0x14064f380) = 77;
    m<i32>(0x14064f3e0) = 55;
    call_srcnt4();
    CHECK_EQ(m<i32>(0x14064f380), 77);
    CHECK_EQ(m<i32>(0x14064f3e0), 55);
    def::rt::ftn::fio::reset();

    fs::current_path(old);
    std::error_code ec;
    fs::remove_all(d, ec);
}

int main() { return testkit::run_all(); }
