// ctest-labels: p-dbread dbread
// Tier 1 for packet p-dbread, helper W3: ReadStepHeader (1401cca80), ReadArrays (1401cd1f0), the
// header cache (1401dd4d0 / 1401dfb80), the directory accessors, DBMADR and the Fortran wrappers.
//
//   * hand-derived checks of the small functions (values traced from the decompiles / listings
//     in deform-rea-work/out/dbread/w3, table words from the binary's .rdata);
//   * real DBs, always copies: every step with arrays of the oracle-written DBs under
//     %USERPROFILE%\.deform-oracle\entries (and the COG test DB when present) is read through
//     read_step_header + read_arrays; header words, MAR, A and the address tables must equal what
//     the independent deformdb reader reports (its StepModel for the address tables of steps
//     >= 11000). Skipped when no DB is present.
//
// The functions W3 calls that belong to W2 (src/dbread/pending_w3.hpp) are defined here as test
// doubles that follow their decompiles (out/dbread/w3/callees.c); 140324020 comes from W1's
// w1_wiring.cpp. TEST-ONLY: remove this block (W3_TEST_DOUBLES) when the lead forwards the
// pending declarations to the W2 ports.
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <defdbio/dbread/w3.hpp>
#include <defdbio/dbcore/globals_p1_12.hpp>
#include <defdbio/step/db_info_read.hpp>
#include <defmodel/hdrstats/globals_p4_07.hpp>
#include <defmodel/hdrstats/object_stats.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/step/db_step.hpp>
#include <deformdb/database.hpp>
#include <deformdb/model.hpp>
#include <testkit/check.hpp>

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <memory>
#include <random>
#include <set>
#include <utility>
#include <string>
#include <vector>

#include "../src/dbread/pending_w3.hpp"
#include "../src/dbread/w3_internal.hpp"

using namespace def::dbio::dbread;
using def::model::Cls_1401dbfe0;
using def::model::DB_STEP_DATA;
namespace fs = std::filesystem;


namespace {

// ---- small functions ------------------------------------------------------------------------------

TEST_CASE(ftn_compare_blank_padding)
{
    CHECK_EQ(ftn_compare("ABC", "ABC", 3, 3), 0);
    CHECK_EQ(ftn_compare("ABC", "ABC  ", 3, 5), 0);  // the rest of b is blank
    CHECK_EQ(ftn_compare("ABC  ", "ABC", 5, 3), 0);
    CHECK_EQ(ftn_compare("ABC", "ABD", 3, 3), -1);   // 'C' < 'D'
    CHECK_EQ(ftn_compare("ABD", "ABC", 3, 3), 1);
    CHECK_EQ(ftn_compare("AB", "ABX", 2, 3), -1);    // b goes on with 'X' (>= 0x21)
    CHECK_EQ(ftn_compare("AB", "AB\x01", 2, 3), 1);  // b goes on with a control character
    CHECK_EQ(ftn_compare("ABX", "AB", 3, 2), 1);
    CHECK_EQ(ftn_compare("AB\x01", "AB", 3, 2), -1);
    CHECK_EQ(ftn_compare("", "", 0, 0), 0);
}

TEST_CASE(dbmadr_copies_iad1_and_iad2)
{
    // MAR(1) = 1, MAR(2) = 5, MAR(3) = 8: I1 = 4, I2 = 3; NADS1 = MAR(1..4), NADS2 = MAR(5..7)
    const std::int32_t mar[10] = {1, 5, 8, 40, 50, 60, 70, 80, 90, 100};
    std::int32_t n1[6] = {-1, -1, -1, -1, -1, -1};
    std::int32_t n2[6] = {-1, -1, -1, -1, -1, -1};
    const std::int32_t verbose = 0;
    dbmadr(mar, n1, n2, &verbose);
    CHECK_EQ(n1[0], 1);
    CHECK_EQ(n1[3], 40);
    CHECK_EQ(n1[4], -1);
    CHECK_EQ(n2[0], 50);
    CHECK_EQ(n2[2], 70);
    CHECK_EQ(n2[3], -1);
    // I1 <= 0: NADS2 starts at MAR(1)
    const std::int32_t mar2[4] = {5, 3, 5, 9};
    std::int32_t m1[2] = {-1, -1};
    std::int32_t m2[3] = {-1, -1, -1};
    dbmadr(mar2, m1, m2, &verbose);
    CHECK_EQ(m1[0], -1);
    CHECK_EQ(m2[0], 5);
    CHECK_EQ(m2[1], 3);
    CHECK_EQ(m2[2], -1);
}

TEST_CASE(directory_accessors_and_header_layout)
{
    Cls_1401dbfe0 db;
    CHECK_EQ(dir_dim(&db, 0), 3);       // no directory
    CHECK_EQ(dir_version(&db, 0), 10000);
    auto* rec = new def::model::DbDirRecord[3];
    rec[0].dim = 2;
    rec[0].version = 9100;
    rec[1].dim = 3;
    rec[1].version = 2500;
    rec[2].dim = 3;
    rec[2].version = 11000;
    db.uad8 = rec;
    db.uac8 = 3;
    CHECK_EQ(dir_dim(&db, 0), 2);
    CHECK_EQ(dir_dim(&db, 3), 3);       // out of range
    CHECK_EQ(dir_dim(&db, -1), 3);
    CHECK_EQ(dir_version(&db, 1), 2500);
    CHECK_EQ(dir_version(&db, 3), 10000);
    std::int32_t f = 0, s = 0;
    std::int32_t i0 = 0, i1 = 1, i2 = 2;
    header_layout(&db, &i0, &f, &s);
    CHECK_EQ(f, 34);
    CHECK_EQ(s, 100);
    header_layout(&db, &i1, &f, &s);
    CHECK_EQ(f, 8);
    CHECK_EQ(s, 100);
    // version > 10999 takes F / S from the cache record when > 0
    auto* e = new def::model::DbInfoEntryE0[3];
    for (int k = 0; k < 3; ++k) {
        hdr_cache_record_init(&e[k]);
    }
    db.uae8 = e;
    header_layout(&db, &i2, &f, &s);
    CHECK_EQ(f, 34);
    CHECK_EQ(s, 200);
    *reinterpret_cast<std::int32_t*>(reinterpret_cast<char*>(&e[2]) + 0x30) = 100;
    *reinterpret_cast<std::int32_t*>(reinterpret_cast<char*>(&e[2]) + 0x34) = 400;
    header_layout(&db, &i2, &f, &s);
    CHECK_EQ(f, 100);
    CHECK_EQ(s, 400);
    def::model::db_info_reset(db); // deletes both arrays
}

TEST_CASE(cache_record_and_block_ctors)
{
    std::vector<unsigned char> r(0xe0, 0xcc);
    hdr_cache_record_init(r.data());
    CHECK_EQ(*reinterpret_cast<std::int32_t*>(&r[0x30]), 0);
    CHECK(std::strcmp(reinterpret_cast<char*>(&r[0x4c]), "OPERATION 1") == 0);
    CHECK(std::strcmp(reinterpret_cast<char*>(&r[0x8d]), "SIMULATION 1") == 0);
    CHECK_EQ(r[0x40], 0xcc); // +0x40..0x4b untouched
    std::vector<unsigned char> b(2 * 0xcd0, 0xcc);
    vector_ctor_iterator(b.data(), 0xcd0, 2, &hdr_object_block_init);
    CHECK_EQ(b[0xcd0 + 8 * 399], 0);
    CHECK_EQ(b[0xcd0 + 0xc80], 0);
    CHECK_EQ(b[0xcd0 + 0xc81], 0xcc);
}

TEST_CASE(remap_maps_match_rdata)
{
    using def::dbio::dbread::w3::remap;
    // words read from DEF_PRE_64.exe .rdata (python dump of the maps, out/dbread/w3)
    CHECK_EQ(remap(0x1405d6de0, 17), 58);  // 3D 2300 MAR
    CHECK_EQ(remap(0x1405d6f20, 2), 3);    // 3D 2300 A
    CHECK_EQ(remap(0x1405d7460, 23), 58);  // 3D 4000 MAR starts
    CHECK_EQ(remap(0x1405d7460, 35), 23);
    CHECK_EQ(remap(0x1405d7460, 40), 70);  // widths from k + 40
    CHECK_EQ(remap(0x1405d75a0, 56), 126); // 3D 4000 A
    CHECK_EQ(remap(0x1405d5f00, 14), 62);  // 2D 6000 MAR
    CHECK_EQ(remap(0x1405d5ff0, 109), 39); // 2D 6000 A (the 39 in the middle of the table)
    CHECK_EQ(remap(0x1405d6220, 30), 70);  // 2D 7000 MAR widths from k + 30
    CHECK_EQ(remap(0x1405d6310, 139), 268);
    // past the end of a map the original reads the next one: 3D 4000 MAR word 80 is A map word 0
    CHECK_EQ(remap(0x1405d7460, 80), 0);
    CHECK_EQ(remap(0x1405d7460, 81), 1);
}

TEST_CASE(realloc_helpers)
{
    CHECK(realloc_ints(nullptr, 0) == nullptr);
    std::int32_t* p = realloc_ints(nullptr, 4);
    REQUIRE(p != nullptr);
    p[3] = 7;
    p = realloc_ints(p, 100);
    CHECK_EQ(p[3], 7);
    std::free(p);
    double* d = realloc_doubles(nullptr, 3);
    REQUIRE(d != nullptr);
    std::free(d);
}

// ---- real DBs -------------------------------------------------------------------------------------

fs::path temp_dir()
{
    static fs::path d = fs::temp_directory_path() / ("dbread_w3_" + std::to_string(std::random_device{}()));
    fs::create_directories(d);
    return d;
}

std::vector<fs::path> source_dbs()
{
    std::vector<fs::path> r;
    std::error_code ec;
    const fs::path cog = R"(C:\PROBLEM\TEST_REA\COG_P1_B1\COG_P1_B1.DB)";
    if (fs::exists(cog, ec)) {
        r.push_back(cog);
    }
    const char* home = std::getenv("USERPROFILE");
    if (home == nullptr) {
        return r;
    }
    const fs::path root = fs::path(home) / ".deform-oracle" / "entries";
    if (!fs::exists(root, ec)) {
        return r;
    }
    std::vector<fs::path> o;
    for (auto it = fs::recursive_directory_iterator(root, ec); it != fs::recursive_directory_iterator(); it.increment(ec)) {
        if (ec) {
            break;
        }
        const auto& p = it->path();
        if (it->is_regular_file(ec) && p.extension() == ".DB" && fs::file_size(p, ec) < 8u * 1024 * 1024) {
            o.push_back(p);
        }
    }
    std::sort(o.begin(), o.end());
    // a spread of at most 60 files keeps the test in seconds
    const std::size_t stride = o.size() / 60 + 1;
    for (std::size_t i = 0; i < o.size(); i += stride) {
        r.push_back(o[i]);
    }
    return r;
}

struct Totals {
    int dbs = 0, steps = 0, tables = 0, v2 = 0, v3 = 0;
    std::set<std::pair<int, int>> kinds; // (dim, version) of the steps read
};

void check_db(const fs::path& src, int k, Totals& t)
{
    const fs::path p = temp_dir() / ("w3_" + std::to_string(k) + ".DB");
    fs::copy_file(src, p, fs::copy_options::overwrite_existing);
    fs::permissions(p, fs::perms::owner_write, fs::perm_options::add);
    auto ref = deformdb::Database::open(p);
    REQUIRE(ref != nullptr);

    def::dbio::dbcore::reset_globals();
    auto db = std::make_unique<Cls_1401dbfe0>();
    const std::string ps = p.string();
    REQUIRE(def::dbio::load_db_info(*db, ps.c_str(), false) == 0);
    REQUIRE(db->uac8 == static_cast<std::int32_t>(ref->entries().size()));
    ++t.dbs;
    for (std::int32_t idx = 0; idx < db->uac8; ++idx) {
        const deformdb::StepEntry& e = ref->entries()[static_cast<std::size_t>(idx)];
        if (e.mar_size < 1) {
            continue;
        }
        auto m = std::make_unique<DB_STEP_DATA>(e.dim == 2 ? 2 : 3);
        m->u740 = idx;
        m->step_version = e.version; // 1401cc160 in DBREAD phase 3
        const std::int32_t rh = read_step_header(m.get(), db.get());
        CHECK_EQ(rh, 0);
        if (rh != 0) {
            continue;
        }
        const deformdb::StepData sd = ref->read_step(e);
        REQUIRE(m->u1b70 == e.header_size);
        bool same_h = true;
        for (std::size_t i = 0; i < sd.header.size(); ++i) {
            same_h = same_h && testkit::same_bits(m->u1b80[i], sd.header[i]);
        }
        CHECK(same_h);
        CHECK_EQ(m->u1378, static_cast<std::int32_t>(sd.header[0]));
        CHECK_EQ(m->sim_ctrl.cursim[0], e.sim_no);
        CHECK_EQ(m->sim_ctrl.cursim[1], static_cast<std::int32_t>(sd.header[10]));
        CHECK(std::strcmp(m->sim_ctrl.title, db->ua50) == 0);
        // header cache record of this entry (1401dfb80)
        const char* r = static_cast<const char*>(db->uae8) + static_cast<std::ptrdiff_t>(idx) * 0xe0;
        CHECK_EQ(db->uae0, idx);
        CHECK_EQ(*reinterpret_cast<const std::int32_t*>(r), static_cast<std::int32_t>(sd.header[0]));
        CHECK_EQ(*reinterpret_cast<const std::int32_t*>(r + 0x30), static_cast<std::int32_t>(sd.header[12]));
        CHECK(testkit::same_bits(*reinterpret_cast<const double*>(r + 8), sd.header[2]));

        const std::int32_t ra = read_arrays(m.get(), db.get());
        CHECK_EQ(ra, 0);
        if (ra != 0) {
            continue;
        }
        ++t.steps;
        (e.dim == 2 ? t.v2 : t.v3)++;
        t.kinds.insert({e.dim, e.version});
        const auto* mar = static_cast<const std::int32_t*>(m->u1b88);
        const auto* a = static_cast<const double*>(m->u1b78);
        const std::size_t nm = std::min<std::size_t>(sd.mar.size(), static_cast<std::size_t>(m->u1b68));
        const std::size_t na = std::min<std::size_t>(sd.a.size(), static_cast<std::size_t>(m->u1b6c));
        CHECK(nm > 0 && na > 0);
        CHECK(std::equal(sd.mar.begin(), sd.mar.begin() + static_cast<std::ptrdiff_t>(nm), mar));
        bool same_a = true;
        for (std::size_t i = 0; i < na; ++i) {
            same_a = same_a && testkit::same_bits(a[i], sd.a[i]);
        }
        CHECK(same_a);
        if (10999 < e.version) {
            const deformdb::StepModel model(sd);
            if (model.supported()) {
                bool same_t = true;
                for (int i = 1; i <= 70; ++i) {
                    same_t = same_t && m->u1b90[i - 1] == model.mar_start(i) &&
                             m->u1b90[70 + i - 1] == model.mar_width(i);
                }
                for (int i = 1; i <= 140; ++i) {
                    same_t = same_t && m->u1dc0[i - 1] == model.a_start(i) &&
                             m->u1dc0[140 + i - 1] == model.a_width(i);
                }
                CHECK(same_t);
                ++t.tables;
            }
            else if (sd.mar.size() > 420 && sd.mar[0] == 1 && sd.mar[1] == 141 && sd.mar[2] == 421) {
                // no named IIAD1 / IIAD2 in deformdb for this layout (2D): the spec layout, IIAD1 =
                // MAR(1..140), IIAD2 = MAR(141..420), is what DBMADR copies verbatim for >= 11000
                bool same_t = true;
                for (int i = 0; i < 140; ++i) {
                    same_t = same_t && m->u1b90[i] == sd.mar[static_cast<std::size_t>(i)];
                }
                for (int i = 0; i < 280; ++i) {
                    same_t = same_t && m->u1dc0[i] == sd.mar[static_cast<std::size_t>(140 + i)];
                }
                CHECK(same_t);
                ++t.tables;
            }
            // dynamic-array descriptor read by op 5 (1403dd16d)
            CHECK_EQ(def::dbio::dbcore::g_db.dyn_hdr[0], sd.dynamic_descriptor[0]);
            CHECK_EQ(def::dbio::dbcore::g_db.dyn_hdr[10], sd.dynamic_descriptor[10]);
        }
    }
}

TEST_CASE(real_dbs_match_deformdb)
{
    Totals t;
    int k = 0;
    for (const auto& src : source_dbs()) {
        check_db(src, k++, t);
    }
    std::printf("DBs %d, steps read %d (2D %d, 3D %d), address tables compared %d\n", t.dbs,
                t.steps, t.v2, t.v3, t.tables);
    for (const auto& kv : t.kinds) {
        std::printf("  dim %d version %d\n", kv.first, kv.second);
    }
}

} // namespace

int main() { return testkit::run_all(); }
