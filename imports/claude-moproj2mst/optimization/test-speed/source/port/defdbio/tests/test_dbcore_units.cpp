// Tier-1 tests of the DB core building blocks: byte swapping, run-length coding, the Fortran
// record I/O, OPEN statuses, RMALLOC, NINT. Expected values are worked out by hand from the
// decompiled code (shown in the comments) or come from deformdb's codec, which was checked
// against files DEFORM wrote (docs/db-format-spec.md section 10).
// ctest-labels: p1-12 dbcore
#include <defdbio/dbcore/dbops.hpp>
#include <defdbio/dbcore/ftn_io.hpp>
#include <defdbio/dbcore/globals_p1_12.hpp>
#include <defdbio/dbcore/util.hpp>
#include <deformdb/codec.hpp>
#include <testkit/check.hpp>

#include <cmath>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <limits>
#include <random>
#include <string>
#include <vector>

#include "../src/dbcore/internal.hpp"

using namespace def::dbio::dbcore;
namespace fs = std::filesystem;

namespace {

fs::path temp_dir()
{
    static fs::path d = fs::temp_directory_path() /
                        ("p112_units_" + std::to_string(std::random_device{}()));
    fs::create_directories(d);
    return d;
}

std::vector<std::int32_t> enc_int(std::vector<std::int32_t> v)
{
    const std::int32_t n = static_cast<std::int32_t>(v.size());
    v.resize(v.size() + 3);
    std::int32_t stored = 0;
    rle_encode_int(&n, v.data(), &stored);
    v.resize(static_cast<std::size_t>(stored));
    return v;
}

} // namespace

TEST_CASE(swap_words)
{
    std::uint8_t b[8] = {1, 2, 3, 4, 5, 6, 7, 8};
    const std::int32_t two = 2;
    swap4_words(b, &two);
    CHECK(b[0] == 4 && b[1] == 3 && b[2] == 2 && b[3] == 1 && b[4] == 8 && b[7] == 5);
    std::uint8_t c[8] = {1, 2, 3, 4, 5, 6, 7, 8};
    const std::int32_t one = 1;
    swap8_words(c, &one);
    CHECK(c[0] == 8 && c[3] == 5 && c[4] == 4 && c[7] == 1);
    CHECK_EQ(platform_swap_mode(), 2);
}

TEST_CASE(mod1_maps_zero_to_modulus)
{
    const std::int32_t m = 128;
    std::int32_t i = 128;
    CHECK_EQ(mod1(&i, &m), 128);
    i = 129;
    CHECK_EQ(mod1(&i, &m), 1);
    i = 0; // the slot of an empty directory: position 128 of chunk 1
    CHECK_EQ(mod1(&i, &m), 128);
}

TEST_CASE(rle_short_arrays_get_length_and_zero_link)
{
    // FUN_14044ac31 with n < 4: s = a(1..n), n, 0.
    CHECK(enc_int({7, 7, 7}) == (std::vector<std::int32_t>{7, 7, 7, 3, 0}));
    CHECK(enc_int({}) == (std::vector<std::int32_t>{0, 0}));
}

TEST_CASE(rle_hand_example)
{
    // 1 1 1 1 1 2 3 3 4 4 4 4: run of five 1s -> (1, 5, 0) at words 1..3, link word 3;
    // 2 literal; 3 3 two literals; four 4s -> (4, 4, 3) at 7..9; then N = 12, link = 9.
    const std::vector<std::int32_t> in{1, 1, 1, 1, 1, 2, 3, 3, 4, 4, 4, 4};
    const auto s = enc_int(in);
    CHECK(s == (std::vector<std::int32_t>{1, 5, 0, 2, 3, 3, 4, 4, 3, 12, 9}));
    std::vector<std::int32_t> buf = s;
    buf.resize(in.size());
    const std::int32_t stored = static_cast<std::int32_t>(s.size());
    std::int32_t n = 0;
    rle_decode_int(&stored, buf.data(), &n);
    CHECK_EQ(n, 12);
    CHECK(std::vector<std::int32_t>(buf.begin(), buf.begin() + 12) == in);
}

TEST_CASE(rle_matches_deformdb_codec)
{
    std::mt19937 rng(12);
    for (int trial = 0; trial < 400; ++trial) {
        const int n = static_cast<int>(rng() % 60);
        std::vector<std::int32_t> v(static_cast<std::size_t>(n));
        for (auto& x : v) {
            x = static_cast<std::int32_t>(rng() % 3 == 0 ? rng() % 5 : 0);
        }
        const auto mine = enc_int(v);
        const auto ref = deformdb::rle_encode(std::span<const std::int32_t>(v));
        CHECK(mine == ref);
        // Decode in place and compare with deformdb's decoder.
        std::vector<std::int32_t> buf = mine;
        if (buf.size() < v.size()) {
            buf.resize(v.size());
        }
        const std::int32_t stored = static_cast<std::int32_t>(mine.size());
        std::int32_t out = 0;
        rle_decode_int(&stored, buf.data(), &out);
        CHECK_EQ(out, n);
        CHECK(std::vector<std::int32_t>(buf.begin(), buf.begin() + n) == v);
    }
    for (int trial = 0; trial < 200; ++trial) {
        const int n = static_cast<int>(rng() % 50);
        std::vector<double> v(static_cast<std::size_t>(n));
        for (auto& x : v) {
            x = rng() % 3 == 0 ? (rng() % 7) * 0.25 : 1.5;
        }
        std::vector<double> buf = v;
        buf.resize(v.size() + 3);
        std::int32_t stored = 0;
        rle_encode_real(&n, buf.data(), &stored);
        buf.resize(static_cast<std::size_t>(stored));
        const auto ref = deformdb::rle_encode(std::span<const double>(v));
        CHECK(buf.size() == ref.size());
        bool same = true;
        for (std::size_t i = 0; i < ref.size() && i < buf.size(); ++i) {
            same = same && testkit::same_bits(buf[i], ref[i]);
        }
        CHECK(same);
    }
}

TEST_CASE(rle_real_negative_zero_joins_positive_run)
{
    // Runs compare with ==: -0.0 continues a run of +0.0 and decodes as +0.0.
    std::vector<double> v{0.0, -0.0, 0.0, 0.0, 5.0};
    const std::int32_t n = 5;
    std::vector<double> buf = v;
    buf.resize(8);
    std::int32_t stored = 0;
    rle_encode_real(&n, buf.data(), &stored);
    std::int32_t out = 0;
    rle_decode_real(&stored, buf.data(), &out);
    CHECK_EQ(out, 5);
    CHECK(!std::signbit(buf[1]));
}

TEST_CASE(nint_and_truncation)
{
    using detail::nint;
    CHECK_EQ(nint(2.5), 3);
    CHECK_EQ(nint(-2.5), -3);
    CHECK_EQ(nint(1234567.49), 1234567);
    CHECK_EQ(nint(std::numeric_limits<double>::quiet_NaN()), std::numeric_limits<std::int32_t>::min());
    CHECK_EQ(nint(1e20), std::numeric_limits<std::int32_t>::min());
}

TEST_CASE(block_io_records_and_errors)
{
    const fs::path p = temp_dir() / "blk.DB";
    const std::string ps = p.string();
    std::int32_t ios = -1;
    const std::int32_t open4 = 4;
    const std::int32_t unit = 41;
    fortran_open(&open4, &unit, ps.c_str(), "UNKNOWN", &ios, static_cast<std::int32_t>(ps.size()), 7);
    REQUIRE(ios == 0);
    std::vector<std::int32_t> w(300);
    for (int i = 0; i < 300; ++i) {
        w[static_cast<std::size_t>(i)] = i + 1;
    }
    const std::int32_t wr = 2;
    const std::int32_t rd = 1;
    const std::int32_t n = 300;
    std::int32_t rec = 5;
    std::int32_t err = -1;
    block_io_int(&wr, &unit, w.data(), &n, &rec, &err);
    CHECK_EQ(err, 0);
    CHECK_EQ(rec, 7); // 300 words: records 5, 6, 7; *rec ends on the last one
    CHECK_EQ(static_cast<long long>(fs::file_size(p)), 7LL * 512);
    std::vector<std::int32_t> r(300, 0);
    rec = 5;
    block_io_int(&rd, &unit, r.data(), &n, &rec, &err);
    CHECK_EQ(err, 0);
    CHECK(r == w);
    // Record 8 does not exist: error, *rec stays on it.
    const std::int32_t one = 1;
    rec = 8;
    block_io_int(&rd, &unit, r.data(), &one, &rec, &err);
    CHECK_EQ(err, 1);
    CHECK_EQ(rec, 8);
    // n < 1 clears the error and does nothing.
    const std::int32_t zero = 0;
    block_io_int(&rd, &unit, r.data(), &zero, &rec, &err);
    CHECK_EQ(err, 0);
    // Record 1 lies in the gap the first write left: zeros.
    rec = 1;
    block_io_int(&rd, &unit, r.data(), &one, &rec, &err);
    CHECK_EQ(err, 0);
    CHECK_EQ(r[0], 0);
    const std::int32_t close0 = 0;
    fortran_open(&close0, &unit, "", "", &ios, 0, 0);
    CHECK(!unit_is_open(unit));
    // A record that is only partly in the file cannot be read.
    { std::ofstream(p, std::ios::binary | std::ios::app).write("abc", 3); }
    const std::int32_t open5 = 5;
    fortran_open(&open5, &unit, ps.c_str(), "OLD", &ios, static_cast<std::int32_t>(ps.size()), 3);
    REQUIRE(ios == 0);
    rec = 8;
    block_io_int(&rd, &unit, r.data(), &one, &rec, &err);
    CHECK_EQ(err, 1);
    // ACTION='READ': a write fails.
    rec = 1;
    block_io_int(&wr, &unit, w.data(), &one, &rec, &err);
    CHECK_EQ(err, 1);
    fortran_open(&close0, &unit, "", "", &ios, 0, 0);
}

TEST_CASE(open_status_old_of_missing_file)
{
    const std::string ps = (temp_dir() / "missing.DB").string();
    std::int32_t ios = 0;
    const std::int32_t open5 = 5;
    const std::int32_t unit = 42;
    // FUN_140407e81: STATUS='OLD' and no file -> 0x65 from the INQUIRE, no OPEN.
    fortran_open(&open5, &unit, ps.c_str(), "OLD", &ios, static_cast<std::int32_t>(ps.size()), 3);
    CHECK_EQ(ios, 0x65);
    CHECK(!unit_is_open(unit));
    // Modes 7..9 only clear IOSTAT.
    const std::int32_t m7 = 7;
    ios = 5;
    fortran_open(&m7, &unit, ps.c_str(), "OLD", &ios, static_cast<std::int32_t>(ps.size()), 3);
    CHECK_EQ(ios, 0); // the INQUIRE is for modes 1..6 only
}

TEST_CASE(blank_name_in_inquire_is_a_runtime_fault)
{
    struct Fault {};
    set_runtime_fault_hook([](std::uint32_t) { throw Fault{}; });
    std::int32_t ios = 0;
    const std::int32_t open5 = 5;
    const std::int32_t unit = 43;
    bool faulted = false;
    try {
        fortran_open(&open5, &unit, "    ", "OLD", &ios, 4, 3);
    } catch (const Fault&) {
        faulted = true;
    }
    set_runtime_fault_hook(nullptr);
    CHECK(faulted);
}

TEST_CASE(rmalloc_indices_find_their_blocks)
{
    reset_globals();
    std::int32_t total = 0;
    const std::int32_t n = 10;
    const std::int64_t idx = rmalloc(&n, kRmIntBase, &total);
    CHECK_EQ(total, 10);
    CHECK_EQ(g_rm_total, 10);
    CHECK_EQ(g_rm_allocs, 1);
    CHECK(idx > 0 && idx < 0x7fffffff);
    auto* p = static_cast<std::int32_t*>(rm_word(kRmIntBase, idx));
    REQUIRE(p != nullptr);
    CHECK_EQ(p[9], 0); // calloc
    CHECK(rm_word(kRmIntBase, idx + 9) == p + 9);
    CHECK(rm_word(kRmIntBase, idx + 10) == nullptr);
    rfree(p, &n, &total);
    CHECK_EQ(total, 0);
    CHECK_EQ(g_rm_frees, 1);
    CHECK(rm_word(kRmIntBase, idx) == nullptr);
    // RFREE(p, tot, tot) as FUN_1403bd3b5 calls it leaves the global total unchanged.
    std::int32_t tot2 = 0;
    const std::int32_t m = 6;
    void* q = rmalloc_scratch(&m, &tot2);
    const std::int32_t before = g_rm_total;
    rfree(q, &tot2, &tot2);
    CHECK_EQ(tot2, 0);
    CHECK_EQ(g_rm_total, before);
}

TEST_CASE(dispatcher_maps_region_ops)
{
    // op < -29: op 3 for MAR region |op+30| only; op >= 30: A region op-30 only. Without a DB
    // open the step read fails at the chunk read; the selectors are set before that.
    reset_globals();
    const std::int32_t unit = 44; // not open: the chunk read fails
    std::int32_t op = -35;
    std::int32_t slot = 1;
    std::int32_t step = 0, nh = 0, nmar = 0, na = 0, ierr = 0;
    db_operation(&unit, &op, &slot, &step, &nh, nullptr, &nmar, nullptr, &na, nullptr, &ierr);
    CHECK_EQ(op, 3);
    CHECK_EQ(g_db.sel_mar, 5);
    CHECK_EQ(g_db.sel_a, -1);
    CHECK_EQ(ierr, 0x65);
    op = 33;
    g_db.cur_chunk = 0;
    db_operation(&unit, &op, &slot, &step, &nh, nullptr, &nmar, nullptr, &na, nullptr, &ierr);
    CHECK_EQ(op, 3);
    CHECK_EQ(g_db.sel_mar, -1);
    CHECK_EQ(g_db.sel_a, 3);
}

int main()
{
    const int r = testkit::run_all();
    std::error_code ec;
    fs::remove_all(temp_dir(), ec);
    return r;
}
