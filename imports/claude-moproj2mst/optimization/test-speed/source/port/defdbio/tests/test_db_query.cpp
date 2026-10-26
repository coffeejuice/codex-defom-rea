// ctest-labels: mdt dbquery
// Tier-1 tests of the DB step queries and the multi-file DB chain (packet mdt-dbquery):
// 1401e3ee0, 1401ddf00, 1401ddf60, 1401df660, 1401de240, 1401de110, 1401ddfe0, 1401de420,
// 1401de530, 140323ce0, 1401e4d80, 1401e4c10, 1401e51d0, 1401e4e80, 1403b18c3.
// Expected values: worked out by hand from the decompiled code for a hand-built directory/cache,
// and for the COG test DB (C:\PROBLEM\TEST_REA\COG_P1_B1\COG_P1_B1.DB, used through copies) from
// its directory as deformdb's dbinfo prints it (steps, SimNo, MAR sizes; listed below) and from
// the step headers read with deformdb (word H[10], the operation number of the cache record).
#define _CRT_SECURE_NO_WARNINGS
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <random>
#include <string>

#include <deformdb/database.hpp>
#include <defdbio/db_query.hpp>
#include <defdbio/dbcore/globals_p1_12.hpp>
#include <defdbio/step/db_header_cache.hpp>
#include <defdbio/step/db_info_read.hpp>
#include <defdbio/step/globals_p67_5.hpp>
#include <defmodel/layout/cls_1401dbfe0.hpp>
#include <defmodel/step/db_step.hpp>
#include <testkit/check.hpp>

using def::model::Cls_1401dbfe0;
using def::model::DbDirRecord;
using def::model::DbInfoEntryE0;
namespace q = def::dbio::query;
namespace st = def::dbio::step;
namespace fs = std::filesystem;

namespace {

const fs::path kCog = R"(C:\PROBLEM\TEST_REA\COG_P1_B1\COG_P1_B1.DB)";

fs::path temp_dir()
{
    static fs::path d = fs::temp_directory_path() / ("mdt_dbq_" + std::to_string(std::random_device{}()));
    fs::create_directories(d);
    return d;
}

// COG_P1_B1 directory (dbinfo): 32 entries; steps by index
//   0:-1 1:1 2:-2 3:2 4:-3 5:3 6:-4 7..13: 4..10 14:11 15:-12 16:12 17:-13 18..28: 13..23 29:24
//   30:-25 31:25; SimNo 1 1 2 2 3 3 4 4x8 5 5 6x13 7 7; MAR > 0 (stored) at 0..6, 14..17, 29..31.
constexpr int kCogEntries = 32;
constexpr int kCogStored = 14; // 7 + 4 + 3

struct Row {
    int step, stored, sim, op;
};

// A hand-built directory + cache: dir step/stored/SimNo, cache +0x38 = op, +0x00 NOBJ = 10 + i,
// +0x04 PDIE = i, +0x08 = i + 0.25, +0x10 = i + 0.75, +0x28 = 100 + i.
Cls_1401dbfe0* make_db(const Row* rows, int n)
{
    auto* db = new Cls_1401dbfe0();
    db->uac8 = n;
    auto* r = new DbDirRecord[static_cast<std::size_t>(n)];
    auto* c = new DbInfoEntryE0[static_cast<std::size_t>(n)];
    for (int i = 0; i < n; ++i) {
        st::init_cache_entry(c[i]);
        r[i].step = rows[i].step;
        r[i].stored = rows[i].stored;
        r[i].sim_no = rows[i].sim;
        r[i].dim = 3;
        st::entry_i32(c[i], 0x38) = rows[i].op;
        st::entry_i32(c[i], 0x00) = 10 + i;
        st::entry_i32(c[i], 0x04) = i;
        st::entry_f64(c[i], 0x08) = i + 0.25;
        st::entry_f64(c[i], 0x10) = i + 0.75;
        st::entry_f64(c[i], 0x28) = 100.0 + i;
    }
    db->uad8 = r;
    db->uae8 = c;
    db->u1900 = 1;
    return db;
}

DbDirRecord* recs(Cls_1401dbfe0& db) { return static_cast<DbDirRecord*>(db.uad8); }

Cls_1401dbfe0* load(const fs::path& p)
{
    auto* db = new Cls_1401dbfe0();
    CHECK_EQ(def::dbio::load_db_info(*db, p.string().c_str(), false), 0);
    return db;
}

} // namespace

TEST_CASE(queries_on_hand_built_cache)
{
    const Row rows[] = {
        {-1, 1, 1, 1}, {1, 1, 1, 1},  {-2, 1, 2, 2}, {2, 0, 2, 2},
        {3, 1, 2, 2},  {4, 1, 3, 7},  {5, 1, 4, 9},  {-6, 1, 4, 9},
    };
    Cls_1401dbfe0* db = make_db(rows, 8);
    CHECK(q::header_cache_built(*db));
    // 1401ddf00 / 1401ddf60: the stored flag does not matter, first match wins
    CHECK_EQ(q::entry_step(*db, 3), 2);
    CHECK_EQ(q::entry_step(*db, -1), 0);
    CHECK_EQ(q::entry_step(*db, 8), 0);
    CHECK_EQ(q::step_index(*db, 2), 3);
    CHECK_EQ(q::step_index(*db, -2), 2);
    CHECK_EQ(q::step_index(*db, 7), -1);
    // op 1: steps -1, 1 -> first -1, last 1, count |1| - (-1 - (-1)) + 1 = 2
    CHECK_EQ(q::op_first_step(*db, 1), -1);
    CHECK_EQ(q::op_last_step(*db, 1), 1);
    CHECK_EQ(q::op_step_count(*db, 1), 2);
    // op 2: stored -2, 3 (2 is not stored) -> count 3 - (-1 - (-2)) + 1 = 3
    CHECK_EQ(q::op_first_step(*db, 2), -2);
    CHECK_EQ(q::op_last_step(*db, 2), 3);
    CHECK_EQ(q::op_step_count(*db, 2), 3);
    // op 7: one entry -> first == last -> 1
    CHECK_EQ(q::op_step_count(*db, 7), 1);
    // op 9: 5 then -6 -> |-6| - 5 + 1 = 2
    CHECK_EQ(q::op_step_count(*db, 9), 2);
    // op 3 matches by cache +0x38 only (entry 5 has SimNo 3 but op 7): not found
    CHECK_EQ(q::op_first_step(*db, 3), 0);
    CHECK_EQ(q::op_last_step(*db, 3), 0);
    CHECK_EQ(q::op_step_count(*db, 3), 0);
    // 1401df660 outputs: first match entry 2, last match entry 4
    {
        int dim = -9, nobj = -9, pdie = -9, f = -9, l = -9;
        double t0 = -9, t1 = -9, s0 = -9, s1 = -9;
        CHECK(q::op_first_last(*db, 2, &dim, &nobj, &pdie, &f, &l, &t0, &t1, &s0, &s1));
        CHECK_EQ(dim, 3);
        CHECK_EQ(nobj, 12);
        CHECK_EQ(pdie, 2);
        CHECK_EQ(f, -2);
        CHECK_EQ(l, 3);
        CHECK_BITS(t0, 2.25);
        CHECK_BITS(t1, 4.75);
        CHECK_BITS(s0, 102.0);
        CHECK_BITS(s1, 104.0);
        // single match: the "last" outputs stay untouched except the step
        t1 = -9;
        s1 = -9;
        CHECK(q::op_first_last(*db, 7, &dim, &nobj, &pdie, &f, &l, &t0, &t1, &s0, &s1));
        CHECK_EQ(f, 4);
        CHECK_EQ(l, 4);
        CHECK_BITS(t1, -9.0);
        CHECK_BITS(s1, -9.0);
        CHECK(!q::op_first_last(*db, 3, &dim, &nobj, &pdie, &f, &l, &t0, &t1, &s0, &s1));
    }
    // 1401de420 / 1401de530: by SimNo
    CHECK_EQ(q::sim_first_step(*db, 2), -2);
    CHECK_EQ(q::sim_last_step(*db, 2), 3);
    CHECK_EQ(q::sim_first_step(*db, 3), 4);
    CHECK_EQ(q::sim_last_step(*db, 3), 4);
    CHECK_EQ(q::sim_first_step(*db, 7), 0);
    CHECK_EQ(q::sim_last_step(*db, 7), 0);
    // no cache: 1401df660 fails; the sim queries do not build one
    def::model::delete_entries_e0(static_cast<DbInfoEntryE0*>(db->uae8));
    db->uae8 = nullptr;
    CHECK_EQ(q::op_first_step(*db, 1), 0);  // cache flag still 1, so nothing is rebuilt
    CHECK_EQ(q::op_step_count(*db, 1), 0);
    CHECK_EQ(q::sim_first_step(*db, 1), 0);
    CHECK_EQ(q::sim_last_step(*db, 1), 0);
    delete db;

    // no directory at all
    Cls_1401dbfe0 empty;
    CHECK(!q::header_cache_built(empty));
    CHECK_EQ(q::step_index(empty, 1), -1);
    CHECK_EQ(q::entry_step(empty, 0), 0);
}

TEST_CASE(strip_extension_cuts_at_last_dot)
{
    char a[] = "C:\\a.b\\NAME.DB";
    q::strip_extension(a);
    CHECK_EQ(std::string(a), std::string("C:\\a.b\\NAME"));
    char b[] = "NOEXT";
    q::strip_extension(b);
    CHECK_EQ(std::string(b), std::string("NOEXT"));
    char c[] = "x.y.z";
    q::strip_extension(c);
    CHECK_EQ(std::string(c), std::string("x.y"));
}

TEST_CASE(cog_queries)
{
    if (!fs::exists(kCog)) {
        std::printf("COG DB missing: skipped\n");
        return;
    }
    const fs::path copy = temp_dir() / "COGQ.DB";
    fs::copy_file(kCog, copy, fs::copy_options::overwrite_existing);
    st::reset_globals_p67_5();
    Cls_1401dbfe0* db = load(copy);
    CHECK_EQ(db->uac8, kCogEntries);
    CHECK_EQ(db->uacc, kCogStored);
    CHECK(!q::header_cache_built(*db));
    // the directory queries need no cache
    CHECK_EQ(q::step_index(*db, -13), 17);
    CHECK_EQ(q::step_index(*db, 25), 31);
    CHECK_EQ(q::step_index(*db, 26), -1);
    CHECK_EQ(q::entry_step(*db, 29), 24);
    // no cache yet: the sim queries return 0
    CHECK_EQ(q::sim_first_step(*db, 6), 0);
    // an op query builds the cache (1401dd250)
    (void)q::op_first_step(*db, 1);
    CHECK(q::header_cache_built(*db));
    // SimNo: stored entries 0,1 (sim 1), 6 and 14 (sim 4), 17 and 29 (sim 6), 30, 31 (sim 7)
    CHECK_EQ(q::sim_first_step(*db, 1), -1);
    CHECK_EQ(q::sim_last_step(*db, 1), 1);
    CHECK_EQ(q::sim_first_step(*db, 4), -4);
    CHECK_EQ(q::sim_last_step(*db, 4), 11);
    CHECK_EQ(q::sim_first_step(*db, 6), -13);
    CHECK_EQ(q::sim_last_step(*db, 6), 24);
    CHECK_EQ(q::sim_first_step(*db, 8), 0);
    // operations: H[10] of each stored entry, read with deformdb
    auto ref = deformdb::Database::open(copy);
    deformdb::ReadOptions ro;
    ro.arrays = false;
    ro.dynamic = false;
    int opno[kCogEntries] = {};
    for (int i = 0; i < kCogEntries; ++i) {
        auto s = ref->read_step(ref->entries()[i], ro);
        opno[i] = static_cast<int>(s.header[10]);
    }
    int checked = 0;
    for (int op = 0; op <= 9; ++op) {
        int first = 0, last = 0;
        bool found = false;
        for (int i = 0; i < kCogEntries; ++i) {
            if (ref->entries()[i].mar_size < 1 || opno[i] != op) continue;
            if (!found) first = ref->entries()[i].step;
            last = ref->entries()[i].step;
            found = true;
        }
        int count = 0;
        if (found) {
            count = first == last ? 1 : (last < 1 ? -last : last) - (first < 1 ? -1 - first : first) + 1;
        }
        std::printf("op %d: found %d steps %d..%d count %d\n", op, found ? 1 : 0, first, last, count);
        CHECK_EQ(q::op_first_step(*db, op), first);
        CHECK_EQ(q::op_last_step(*db, op), last);
        CHECK_EQ(q::op_step_count(*db, op), count);
        checked += found ? 1 : 0;
    }
    CHECK(checked > 0);
    delete db;
    { std::error_code rm_ec; fs::remove(copy, rm_ec); }
}

TEST_CASE(cog_part_chain)
{
    if (!fs::exists(kCog)) {
        std::printf("COG DB missing: skipped\n");
        return;
    }
    def::dbio::dbcore::reset_globals();
    const fs::path dir = temp_dir() / "chain";
    fs::create_directories(dir);
    const fs::path main = dir / "X.DB";
    fs::copy_file(kCog, main, fs::copy_options::overwrite_existing);

    // first step -1: no previous part
    {
        Cls_1401dbfe0* db = load(main);
        CHECK(!q::has_previous_part(*db));
        CHECK_EQ(q::prepend_previous_part(*db), 5);
        // the chain of a single file: one part, entries 1..32, steps -1..25
        CHECK_EQ(q::setup_part_chain(*db), 0);
        CHECK_EQ(db->u008, 1);
        CHECK_EQ(db->u00c[0].a, 0);
        CHECK_EQ(db->u00c[0].b, 31);
        CHECK_EQ(std::string(static_cast<const char*>(db->u330[0])), main.string());
        CHECK_EQ(q::db_part_count(), 1);
        CHECK_EQ(q::db_part_words(1)[0], 1);
        CHECK_EQ(q::db_part_words(1)[1], 32);
        CHECK_EQ(q::db_part_words(1)[2], -1);
        CHECK_EQ(q::db_part_words(1)[3], 25);
        CHECK_EQ(q::db_part_entries(), 32);
        // name 1: the path blank-padded to 200 characters
        std::string nm(q::db_part_name(1), 200);
        std::string want = main.string();
        want.resize(200, ' ');
        CHECK_EQ(nm, want);
        CHECK(db->uae8 == nullptr);
        delete db;
    }
    // previous-part lookup with the first step patched in memory (the file stays the COG copy,
    // whose last step is 25)
    {
        Cls_1401dbfe0* db = load(main);
        recs(*db)[0].step = 26;
        CHECK(!q::has_previous_part(*db)); // X25.DB / X-26.DB missing
        const fs::path p25 = dir / "X25.DB";
        fs::copy_file(kCog, p25, fs::copy_options::overwrite_existing);
        CHECK(q::has_previous_part(*db)); // last step 25 == 25
        recs(*db)[0].step = 27;
        const fs::path p26 = dir / "X26.DB";
        fs::copy_file(kCog, p26, fs::copy_options::overwrite_existing);
        CHECK(q::has_previous_part(*db)); // 25 == 26 - 1
        recs(*db)[0].step = -30;
        const fs::path p29 = dir / "X29.DB";
        fs::copy_file(kCog, p29, fs::copy_options::overwrite_existing);
        CHECK(!q::has_previous_part(*db)); // |-30| - 1 = 29: 25 is neither 29 nor 28
        recs(*db)[0].step = 31;
        const fs::path pm31 = dir / "X-31.DB";
        fs::copy_file(kCog, pm31, fs::copy_options::overwrite_existing);
        CHECK(!q::has_previous_part(*db)); // X30 missing, X-31: 25 is neither -31 nor -32
        { std::error_code rm_ec; fs::remove(p26, rm_ec); }
        { std::error_code rm_ec; fs::remove(p29, rm_ec); }
        { std::error_code rm_ec; fs::remove(pm31, rm_ec); }
        delete db;
    }
    // chain set-up with X25.DB in front: the 32 entries of X25 are prepended, then its first step
    // (-1) ends the loop with 5 (returned as 0)
    {
        Cls_1401dbfe0* db = load(main);
        recs(*db)[0].step = 26;
        CHECK_EQ(q::setup_part_chain(*db), 0);
        CHECK_EQ(db->u008, 2);
        CHECK_EQ(db->uac8, 64);
        CHECK_EQ(db->uacc, 2 * kCogStored);
        CHECK_EQ(db->u00c[0].a, 0);
        CHECK_EQ(db->u00c[0].b, 31);
        CHECK_EQ(db->u00c[1].a, 32);
        CHECK_EQ(db->u00c[1].b, 63);
        CHECK_EQ(std::string(static_cast<const char*>(db->u330[0])), (dir / "X25.DB").string());
        CHECK_EQ(std::string(static_cast<const char*>(db->u330[1])), main.string());
        CHECK_EQ(recs(*db)[0].step, -1);
        CHECK_EQ(recs(*db)[17].step, -13);
        CHECK_EQ(recs(*db)[17].sim_no, 6);
        CHECK_EQ(recs(*db)[31].step, 25);
        CHECK_EQ(recs(*db)[32].step, 26);
        CHECK_EQ(recs(*db)[63].step, 25);
        CHECK_EQ(q::db_part_count(), 2);
        CHECK_EQ(q::db_part_words(1)[0], 1);
        CHECK_EQ(q::db_part_words(1)[1], 32);
        CHECK_EQ(q::db_part_words(1)[2], -1);
        CHECK_EQ(q::db_part_words(1)[3], 25);
        CHECK_EQ(q::db_part_words(2)[0], 33);
        CHECK_EQ(q::db_part_words(2)[1], 64);
        CHECK_EQ(q::db_part_words(2)[2], 26);
        CHECK_EQ(q::db_part_words(2)[3], 25);
        CHECK_EQ(q::db_part_entries(), 64);
        CHECK_EQ(q::step_index(*db, 26), 32);
        delete db;
    }
    // `-N` part: X-26.DB only; its last entry (step 25) is dropped: 31 entries prepended, 13 of
    // them stored
    {
        { std::error_code rm_ec; fs::remove(dir / "X25.DB", rm_ec); }
        fs::copy_file(kCog, dir / "X-26.DB", fs::copy_options::overwrite_existing);
        Cls_1401dbfe0* db = load(main);
        recs(*db)[0].step = 26;
        // the part-0 set-up of 1401e4e80 (1401e51d0 copies part 0's name, so it must exist)
        db->u008 = 1;
        db->u00c[0].a = 0;
        db->u00c[0].b = db->uac8 - 1;
        db->u330[0] = std::malloc(0x400);
        std::strcpy(static_cast<char*>(db->u330[0]), db->u650);
        CHECK_EQ(q::prepend_previous_part(*db), 0);
        CHECK_EQ(db->u008, 2);
        CHECK_EQ(db->uac8, 63);
        CHECK_EQ(db->uacc, kCogStored + 13);
        CHECK_EQ(db->u00c[0].b, 30);
        CHECK_EQ(db->u00c[1].a, 31);
        CHECK_EQ(db->u00c[1].b, 62);
        CHECK_EQ(recs(*db)[30].step, -25);
        CHECK_EQ(recs(*db)[31].step, 26);
        CHECK_EQ(std::string(static_cast<const char*>(db->u330[0])), (dir / "X-26.DB").string());
        CHECK_EQ(std::string(static_cast<const char*>(db->u330[1])), main.string());
        CHECK_EQ(q::prepend_previous_part(*db), 5); // first step now -1
        delete db;
    }
    std::error_code ec;
    fs::remove_all(dir, ec);
}

int main() { return testkit::run_all(); }
