// Tier 1: FUN_1403d84b0 (header read) and FUN_1403d96fc (result-set directory) with the local
// datarm23 copy, on synthetic DBs built here and on a copy of the COG test DB.
// ctest-labels: p1-13 resultset
//
// Expected values: synthetic DBs are built field by field below (the code under test only reads
// them back); the COG values are the oracle's -m header / -m stepList output for that DB
// (~/.deform-oracle entries 08444cc8.. and 50916a92..), which DEF_PRE_64 computes from these two
// routines (FUN_1401dc3f0).
#include <defdbio/dbcore/ftn_io.hpp>
#include <defdbio/dbcore/globals_p1_12.hpp>
#include <defdbio/resultset/db_result_sets.hpp>
#include <defdbio/resultset/globals_p1_13.hpp>
#include <testkit/check.hpp>

#include <cstdint>
#include <cstdio>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

namespace fs = std::filesystem;
using namespace def::dbio::resultset;

namespace {

void reset()
{
    reset_globals();
    def::dbio::dbcore::reset_globals();
    def::dbio::dbcore::close_all_units();
}

fs::path scratch_dir()
{
    fs::path d = fs::temp_directory_path() / "def_port_p1_13";
    fs::create_directories(d);
    return d;
}

// A DB file as 512-byte records of big-endian words.
struct DbImage {
    std::vector<unsigned char> bytes;

    void ensure(std::size_t rec) // 1-based record must exist
    {
        if (bytes.size() < rec * 512) {
            bytes.resize(rec * 512, 0);
        }
    }
    void put_text(std::size_t rec, std::size_t off, const char* s, std::size_t n)
    {
        ensure(rec);
        std::memcpy(&bytes[(rec - 1) * 512 + off], s, n);
    }
    void put_word(std::size_t rec, std::size_t word1, std::int32_t v) // word1 is 1-based
    {
        ensure(rec + (word1 - 1) / 128);
        const std::size_t at = (rec - 1) * 512 + (word1 - 1) * 4;
        const auto u = static_cast<std::uint32_t>(v);
        bytes[at] = static_cast<unsigned char>(u >> 24);
        bytes[at + 1] = static_cast<unsigned char>(u >> 16);
        bytes[at + 2] = static_cast<unsigned char>(u >> 8);
        bytes[at + 3] = static_cast<unsigned char>(u);
    }
    fs::path write(const char* file) const
    {
        const fs::path p = scratch_dir() / file;
        std::ofstream(p, std::ios::binary)
            .write(reinterpret_cast<const char*>(bytes.data()), static_cast<std::streamsize>(bytes.size()));
        return p;
    }
};

// A version-11 3D DB: record 1 signature, record 2 header, record 3 index, chunks from record 4
// (13 records each). Entry e (1-based) has word k = e * 100 + k, except word 2 (step) = steps[e]
// and word 4 (MAR size) = mar[e].
DbImage make_db(const std::vector<std::int32_t>& steps, const std::vector<std::int32_t>& mar,
                const char* smi1 = "11.00   ", const char* sig = "SFTC-DEF-3D-DB100-00")
{
    DbImage db;
    const auto n = static_cast<std::int32_t>(steps.size());
    db.put_text(1, 0, sig, 20);
    db.put_text(2, 0, "SYNTHETIC TITLE", 15);
    std::memset(&db.bytes[512 + 15], ' ', 80 - 15);
    db.put_text(2, 80, smi1, 8);
    db.put_text(2, 88, "06/30/14", 8);
    db.put_text(2, 96, "01/02/26", 8);
    db.put_text(2, 104, "03/04/26", 8);
    db.put_word(2, 29, 7);    // ISTART
    db.put_word(2, 30, 1);    // ISMTYP
    db.put_word(2, 31, n);    // NRSETS
    db.put_word(2, 32, 4321); // NXTREC
    db.put_word(2, 33, 2);    // IPTYPE
    const std::int32_t chunks = (n + 127) / 128;
    for (std::int32_t c = 1; c <= chunks; ++c) {
        const std::int32_t rec = 4 + (c - 1) * 13;
        db.put_word(3, static_cast<std::size_t>(c), rec);
        db.ensure(static_cast<std::size_t>(rec + 12));
        for (std::int32_t e = (c - 1) * 128 + 1; e <= n && e <= c * 128; ++e) {
            const std::int32_t at = (e - (c - 1) * 128 - 1) * 13; // word offset in the chunk
            for (std::int32_t k = 1; k <= 13; ++k) {
                std::int32_t v = e * 100 + k;
                if (k == 2) v = steps[static_cast<std::size_t>(e - 1)];
                if (k == 4) v = mar[static_cast<std::size_t>(e - 1)];
                db.put_word(static_cast<std::size_t>(rec), static_cast<std::size_t>(at + k), v);
            }
        }
    }
    db.ensure(3);
    return db;
}

// Blank-padded Fortran name, as FUN_140323bf0 hands it over.
struct FtnName {
    char buf[0x400];
    std::int32_t len = 0x400;
    explicit FtnName(const fs::path& p)
    {
        const std::string s = p.string();
        std::memset(buf, ' ', sizeof buf);
        std::memcpy(buf, s.data(), s.size());
    }
};

const std::int32_t kNoPrint = 0;

} // namespace

TEST_CASE(header_of_synthetic_db)
{
    reset();
    const fs::path p = make_db({-1, 1, -2, 2}, {0, 9, 0, 9}).write("hdr.DB");
    FtnName name(p);
    DbHeader h;
    std::memset(&h, 0x5a, sizeof h);
    std::int32_t err = -1;
    read_db_header(name.buf, &name.len, &h, &err, &kNoPrint);
    REQUIRE(err == 0);
    CHECK_EQ(std::string(h.title, 15), std::string("SYNTHETIC TITLE"));
    CHECK_EQ(std::string(h.smi1, 8), std::string("11.00   "));
    CHECK_EQ(std::string(h.smi2, 8), std::string("06/30/14"));
    CHECK_EQ(std::string(h.smi3, 8), std::string("01/02/26"));
    CHECK_EQ(std::string(h.smi4, 8), std::string("03/04/26"));
    CHECK_EQ(h.istart, 7);
    CHECK_EQ(h.ismtyp, 1);
    CHECK_EQ(h.nrsets, 4);
    CHECK_EQ(h.nxtrec, 4321);
    CHECK_EQ(h.family, 23);    // "SFTC-DEF-3D-DB100-00"
    CHECK_EQ(h.version, 11000); // "11.00   " in the family-23 table of FUN_1403b32b5
    CHECK_EQ(dat_140707dd0, 3);
    CHECK_EQ(dat_140707e64, 11);
    CHECK_EQ(dat_140707e60, 16); // last non-blank of the title is column 15
    CHECK_EQ(def::dbio::dbcore::g_db.h.w33, 2); // IPTYPE, word 33 of record 2
}

TEST_CASE(header_open_failure_is_99_and_leaves_outputs)
{
    reset();
    FtnName name(scratch_dir() / "does_not_exist.DB");
    DbHeader h;
    std::memset(&h, 0x5a, sizeof h);
    std::int32_t err = -1;
    read_db_header(name.buf, &name.len, &h, &err, &kNoPrint);
    CHECK_EQ(err, 99);
    CHECK_EQ(h.nrsets, 0x5a5a5a5a);
}

TEST_CASE(header_short_file_is_0x65)
{
    reset();
    DbImage db;
    db.put_text(1, 0, "SFTC", 4);
    db.bytes.resize(100); // record 1 incomplete: the signature read fails
    FtnName name(db.write("short.DB"));
    DbHeader h;
    std::memset(&h, 0x5a, sizeof h);
    std::int32_t err = -1;
    read_db_header(name.buf, &name.len, &h, &err, &kNoPrint);
    CHECK_EQ(err, 0x65);
    CHECK_EQ(h.version, 0x5a5a5a5a);
}

TEST_CASE(header_legacy_version_range)
{
    // Family 2 below " 3.1.1  " is 100, below " 5.0    " is 0x12d (FUN_1403b600f).
    reset();
    FtnName a(make_db({1}, {1}, " 3.0    ", "SFTC-DEFORM-2D-DB450").write("old2d_a.DB"));
    DbHeader h;
    std::int32_t err = -1;
    read_db_header(a.buf, &a.len, &h, &err, &kNoPrint);
    CHECK_EQ(err, 100);
    FtnName b(make_db({1}, {1}, " 4.0    ", "SFTC-DEFORM-2D-DB450").write("old2d_b.DB"));
    read_db_header(b.buf, &b.len, &h, &err, &kNoPrint);
    CHECK_EQ(err, 0x12d);
    FtnName c(make_db({1}, {1}, " 8.10   ", "SFTC-DEFORM-2D-DB450").write("old2d_c.DB"));
    read_db_header(c.buf, &c.len, &h, &err, &kNoPrint);
    CHECK_EQ(err, 0);
    CHECK_EQ(h.family, 2);
    CHECK_EQ(h.version, 8100);
}

TEST_CASE(result_sets_two_chunks_and_counts)
{
    reset();
    // 130 entries: two directory chunks. Steps alternate -e / e, MAR size 0 on every third.
    std::vector<std::int32_t> steps, mar;
    std::int32_t want_rst = 0, want_rms = 0;
    for (std::int32_t e = 1; e <= 130; ++e) {
        steps.push_back(e % 2 ? -e : e);
        mar.push_back(e % 3 ? 50 + e : 0);
        want_rst += e % 3 ? 1 : 0;
        want_rms += e % 2 ? 1 : 0;
    }
    FtnName name(make_db(steps, mar).write("dir.DB"));
    DbHeader h;
    std::int32_t err = -1;
    read_db_header(name.buf, &name.len, &h, &err, &kNoPrint);
    REQUIRE(err == 0);

    std::int32_t count = h.nrsets;
    const std::int32_t ncols = 10;
    std::vector<std::int32_t> words(static_cast<std::size_t>(count) * 10, -7);
    read_result_sets(name.buf, &name.len, &count, words.data(), &ncols, nullptr, &err, &kNoPrint);
    REQUIRE(err == 0);
    for (std::int32_t e = 1; e <= 130; ++e) {
        for (std::int32_t k = 1; k <= 10; ++k) {
            std::int32_t want = e * 100 + k;
            if (k == 2) want = steps[static_cast<std::size_t>(e - 1)];
            if (k == 4) want = mar[static_cast<std::size_t>(e - 1)];
            CHECK_EQ(words[static_cast<std::size_t>((e - 1) * 10 + k - 1)], want);
        }
    }
    CHECK_EQ(g_numrst, want_rst);
    CHECK_EQ(g_numrms, want_rms);
    CHECK_EQ(dat_140707ed8, 131);
    CHECK_EQ(dat_140707edc, 129 * 10);
    CHECK_EQ(dat_140707e68, 3);
}

TEST_CASE(result_sets_count_zero_takes_nrsets)
{
    reset();
    FtnName name(make_db({5, -6, 7}, {1, 1, 0}).write("zero.DB"));
    DbHeader h;
    std::int32_t err = -1;
    read_db_header(name.buf, &name.len, &h, &err, &kNoPrint);
    REQUIRE(err == 0);
    std::int32_t count = 0;
    const std::int32_t ncols = 10;
    std::vector<std::int32_t> words(30, 0);
    read_result_sets(name.buf, &name.len, &count, words.data(), &ncols, nullptr, &err, &kNoPrint);
    CHECK_EQ(err, 0);
    CHECK_EQ(count, 3);
    CHECK_EQ(words[21], 7);
    CHECK_EQ(g_numrst, 2);
    CHECK_EQ(g_numrms, 1);
}

TEST_CASE(result_sets_missing_chunk_is_0x65)
{
    reset();
    DbImage db = make_db({1, 2}, {1, 1});
    FtnName name(db.write("chunk.DB"));
    DbHeader h;
    std::int32_t err = -1;
    read_db_header(name.buf, &name.len, &h, &err, &kNoPrint);
    REQUIRE(err == 0);
    db.bytes.resize(3 * 512 + 100); // the chunk at record 4 is gone
    db.write("chunk.DB");
    std::int32_t count = 2;
    const std::int32_t ncols = 10;
    std::vector<std::int32_t> words(20, 0);
    g_numrst = -5;
    read_result_sets(name.buf, &name.len, &count, words.data(), &ncols, nullptr, &err, &kNoPrint);
    CHECK_EQ(err, 0x65);
    CHECK_EQ(g_numrst, -5); // the counts are not touched on an error
    std::int32_t err2 = -1;
    FtnName none(scratch_dir() / "does_not_exist.DB");
    read_result_sets(none.buf, &none.len, &count, words.data(), &ncols, nullptr, &err2, &kNoPrint);
    CHECK_EQ(err2, 99);
}

TEST_CASE(cog_test_db_matches_oracle)
{
    const fs::path src = "C:/PROBLEM/TEST_REA/COG_P1_B1/COG_P1_B1.DB";
    if (!fs::exists(src)) {
        std::printf("COG test DB not present, case skipped\n");
        return;
    }
    reset();
    const fs::path copy = scratch_dir() / "COG_P1_B1.DB";
    fs::copy_file(src, copy, fs::copy_options::overwrite_existing);
    FtnName name(copy);
    DbHeader h;
    std::int32_t err = -1;
    read_db_header(name.buf, &name.len, &h, &err, &kNoPrint);
    REQUIRE(err == 0);
    // -m header: DB Version 11000, dates, SimType 1, NoRecords 32.
    CHECK_EQ(h.version, 11000);
    CHECK_EQ(std::string(h.smi2, 8), std::string("06/30/14"));
    CHECK_EQ(std::string(h.smi3, 8), std::string("10/09/26"));
    CHECK_EQ(std::string(h.smi4, 8), std::string("10/09/26"));
    CHECK_EQ(h.ismtyp, 1);
    CHECK_EQ(h.nrsets, 32);

    std::int32_t count = h.nrsets;
    const std::int32_t ncols = 10;
    std::vector<std::int32_t> words(static_cast<std::size_t>(count) * 10, 0);
    read_result_sets(name.buf, &name.len, &count, words.data(), &ncols, nullptr, &err, &kNoPrint);
    REQUIRE(err == 0);
    // -m stepList: the steps of the entries whose MAR size (word 4) is at least 1, in order.
    const std::vector<std::int32_t> want = {-1, 1, -2, 2, -3, 3, -4, 11, -12, 12, -13, 24, -25, 25};
    std::vector<std::int32_t> got;
    for (std::int32_t e = 0; e < count; ++e) {
        if (words[static_cast<std::size_t>(e * 10 + 3)] >= 1) {
            got.push_back(words[static_cast<std::size_t>(e * 10 + 1)]);
        }
    }
    CHECK(got == want);
    // NoStoredSteps 14 counts MAR size >= 1, NUMRST counts MAR size != 0; sizes are lengths.
    CHECK_EQ(g_numrst, 14);
    fs::remove(copy);
}

int main() { return testkit::run_all(); }
