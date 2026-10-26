// ctest-labels: p-dbread dbread
// Tier-1 tests of the old-format DB operations of helper W2 (1403bef07, 1403c0e60, 1403dc957,
// 140444458, 140469eb0). The DB file is written by the port itself (op 4) into a temp dir and
// read back with ops 0..3; expected values follow from the code by hand (comments).
#include <defdbio/dbcore/ftn_io.hpp>
#include <defdbio/dbcore/globals_p1_12.hpp>
#include <defdbio/dbcore/util.hpp>
#include <defdbio/dbread/w2.hpp>
#include <testkit/check.hpp>

#include <cstring>
#include <filesystem>
#include <random>
#include <string>
#include <vector>

#include "../src/dbread/globals_w2.hpp"

using namespace def::dbio;
using namespace def::dbio::dbread;
namespace fs = std::filesystem;

namespace {

fs::path temp_dir()
{
    static fs::path d = fs::temp_directory_path() / ("w2_dbops_" + std::to_string(std::random_device{}()));
    fs::create_directories(d);
    return d;
}

void reset_all()
{
    dbcore::reset_globals();
    reset_globals_w2();
}

} // namespace

TEST_CASE(copy_chars_and_copy_char)
{
    reset_all();
    char dst[8] = {'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x'};
    const std::int32_t n = 5;
    copy_chars("abcdefgh", dst, &n);
    CHECK(std::memcmp(dst, "abcdexxx", 8) == 0);
    CHECK_EQ(g_copy_index, 6); // the global index ends one past *n
    const std::int32_t zero = 0;
    copy_chars("zz", dst, &zero);
    CHECK_EQ(g_copy_index, 1);
    CHECK(dst[0] == 'a');
    char c = 0;
    copy_char(&c, "Q", 99);
    CHECK(c == 'Q');
}

TEST_CASE(chunk_io_old_positions)
{
    reset_all();
    // mode 1 on the loaded record does no I/O: slot 130 -> record 2, position 2
    g_old_db.cur_rec = 2;
    std::int32_t mode = 1, unit = 77, slot = 130, rec = 0, pos = 0, ierr = 5;
    chunk_io_old(&mode, &unit, &slot, &rec, &pos, &ierr);
    CHECK_EQ(rec, 2);
    CHECK_EQ(pos, 2);
    CHECK_EQ(ierr, 0);
    slot = 128; // record 1, position 128 (mod1 maps 0 to 128)
    g_old_db.cur_rec = 1;
    chunk_io_old(&mode, &unit, &slot, &rec, &pos, &ierr);
    CHECK_EQ(rec, 1);
    CHECK_EQ(pos, 128);
}

TEST_CASE(write_and_read_back_one_step)
{
    reset_all();
    const fs::path p = temp_dir() / "OLD.DB";
    const std::string ps = p.string();
    std::int32_t unit = 3, ios = 0;
    const std::int32_t open4 = 4, open5 = 5, close0 = 0;
    dbcore::fortran_open(&open4, &unit, ps.c_str(), "UNKNOWN", &ios, static_cast<std::int32_t>(ps.size()), 7);
    CHECK_EQ(ios, 0);

    // record 2 contents: SMI1 " 2.0.1  " (>= " 1.0    ": run-length coded arrays), ISMTYP 1
    std::memcpy(dbcore::g_db.h.title, "TITLE", 5);
    std::memcpy(dbcore::g_db.h.smi1, " 2.0.1  ", 8);
    dbcore::g_db.h.ismtyp = 1;
    dbcore::g_db.put_mesh_no = 7;
    dbcore::g_db.put_step = 42;
    dbcore::g_db.put_interval = 1;

    std::vector<double> h(8, 0.0);
    h[0] = 1.25;
    std::vector<std::int32_t> mar = {5, 5, 5, 5, 9, 0, 0, 0, 0, 0};
    std::vector<double> a = {0.5, 0.5, -3.0, 0, 0, 0, 0, 0, 0, 0};
    std::int32_t op = 4, slot = 1, step = 42, nh = 8, nmar = 5, na = 3, ierr = 0;
    db_operation_old(&unit, &op, &slot, &step, &nh, h.data(), &nmar, mar.data(), &na, a.data(), &ierr);
    CHECK_EQ(ierr, 0);
    CHECK_EQ(dbcore::g_db.h.nrsets, 1);
    CHECK_EQ(g_old_db.index[0], 4);         // slot 1: directory record 1 at file record 4
    CHECK_EQ(g_old_db.entry(1, 6), 12);     // header at record 12 (0xc)
    CHECK_EQ(g_old_db.entry(1, 7), 13);     // 8 header reals fill one record
    CHECK_BITS(h[5], 2.0);                  // platform swap mode
    CHECK_BITS(h[6], 5.0);                  // *nmar before coding
    CHECK_BITS(h[7], 3.0);
    CHECK_EQ(mar[0], 5);                    // the caller's arrays come back decoded
    CHECK_EQ(mar[4], 9);
    CHECK_BITS(a[2], -3.0);
    CHECK_EQ(g_old_compressed, 1);
    dbcore::fortran_open(&close0, &unit, "", "", &ios, 0, 0);

    // read back: op 0, op 1, op 2 by slot, op 3 by step search
    reset_all();
    dbcore::fortran_open(&open5, &unit, ps.c_str(), "OLD", &ios, static_cast<std::int32_t>(ps.size()), 3);
    CHECK_EQ(ios, 0);
    op = 0;
    ierr = 0;
    db_operation_old(&unit, &op, &slot, &step, &nh, h.data(), &nmar, mar.data(), &na, a.data(), &ierr);
    CHECK_EQ(ierr, 0);
    CHECK_EQ(dbcore::g_db.h.nrsets, 1);
    CHECK_EQ(dbcore::g_db.h.ismtyp, 1);
    CHECK_EQ(g_old_db.index[0], 4);

    std::vector<std::int32_t> dir(10, -1);
    std::int32_t count = 0;
    op = 1;
    db_operation_old(&unit, &op, &slot, &step, &nh, h.data(), &count, dir.data(), &na, a.data(), &ierr);
    CHECK_EQ(ierr, 0);
    CHECK_EQ(count, 1);
    CHECK_EQ(dir[0], 7);  // mesh
    CHECK_EQ(dir[1], 42); // step
    CHECK_EQ(dir[2], 8);  // header words
    CHECK_EQ(dir[5], -1); // 5 words per entry only

    std::vector<double> h2(8, 0.0);
    op = 2;
    slot = 1;
    nh = 0;
    db_operation_old(&unit, &op, &slot, &step, &nh, h2.data(), &nmar, mar.data(), &na, a.data(), &ierr);
    CHECK_EQ(ierr, 0);
    CHECK_EQ(nh, 8);
    CHECK_BITS(h2[0], 1.25);
    CHECK_BITS(h2[6], 5.0);

    std::vector<std::int32_t> mar2(16, 0);
    std::vector<double> a2(16, 0.0);
    std::int32_t nmar2 = 0, na2 = 0;
    op = 3;
    slot = 0;
    step = 42;
    db_operation_old(&unit, &op, &slot, &step, &nh, h2.data(), &nmar2, mar2.data(), &na2, a2.data(), &ierr);
    CHECK_EQ(ierr, 0);
    CHECK_EQ(slot, 1);
    CHECK_EQ(nmar2, 5);
    CHECK_EQ(mar2[3], 5);
    CHECK_EQ(mar2[4], 9);
    CHECK_EQ(na2, 3);
    CHECK_BITS(a2[0], 0.5);
    CHECK_BITS(a2[2], -3.0);

    // an unknown step: 0xc9
    step = 43;
    slot = 0;
    nmar2 = 0;
    na2 = 0;
    db_operation_old(&unit, &op, &slot, &step, &nh, h2.data(), &nmar2, mar2.data(), &na2, a2.data(), &ierr);
    CHECK_EQ(ierr, 0xc9);
    dbcore::fortran_open(&close0, &unit, "", "", &ios, 0, 0);
}

TEST_CASE(read_a_region_missing_file)
{
    reset_all();
    const std::string ps = (temp_dir() / "NOPE.DB").string();
    char file[200];
    std::memset(file, ' ', 200);
    std::memcpy(file, ps.data(), ps.size());
    std::int32_t nchar = static_cast<std::int32_t>(ps.size()), slot = 1, nh = 0, nmar = 0, na = 0;
    std::int32_t region = 5, version = 11000, ierr = 0, iprint = 0;
    double h[8] = {};
    std::int32_t mar[4] = {};
    double a[4] = {};
    read_a_region_from_file(file, &nchar, &slot, &nh, h, &nmar, mar, &na, a, &region, &version, &ierr, &iprint);
    CHECK_EQ(ierr, 99);
    CHECK_EQ(g_rdarya.unit, 3);
}

int main() { return testkit::run_all(); }
