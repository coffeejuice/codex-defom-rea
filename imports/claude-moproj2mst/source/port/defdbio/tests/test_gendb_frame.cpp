// Tier 1 for the GENDB driver and frame helpers of packet p4-01: version compatibility (1401b9810),
// step placement in the directory (1401e1f60), highest version (1401ddc10), the Fortran helpers
// ADJUSTL (14044a4b1), CHARACTER assignment (1404b33e4), record 2 (1403b400b) and the step words
// (1403b3f73). Expected values are worked out by hand from the recovered code.
// ctest-labels: p4-01 gendb
#include <defdbio/dbcore/globals_p1_12.hpp>
#include <defdbio/gendb/gendb.hpp>
#include <defmodel/step/db_step.hpp>
#include <testkit/check.hpp>

#include <cstring>
#include <initializer_list>
#include <string>
#include <utility>

#include "../src/gendb/db_header_io.hpp"

using namespace def::dbio::gendb;
using def::dbio::dbcore::g_db;
using def::model::Cls_1401dbfe0;
using def::model::DB_STEP_DATA;
using def::model::DbDirRecord;
using def::model::db_info_records;

namespace {

// Zeroed storage: the helper under test reads only the dimension word.
struct Step {
    alignas(16) unsigned char raw[sizeof(DB_STEP_DATA)];
    DB_STEP_DATA& m() { return *reinterpret_cast<DB_STEP_DATA*>(raw); }
    explicit Step(std::int32_t dim)
    {
        std::memset(raw, 0, sizeof(raw));
        m().dimension = dim;
    }
};

bool compat(std::int32_t dim, std::int32_t db, std::int32_t cur)
{
    static Step s(0);
    s.m().dimension = dim;
    return versions_compatible(s.m(), &db, &cur);
}

struct Dir {
    Cls_1401dbfe0 info;
    explicit Dir(std::initializer_list<std::pair<std::int32_t, std::int32_t>> e)
    {
        info.uac8 = static_cast<std::int32_t>(e.size());
        info.uad8 = new DbDirRecord[e.size()];
        DbDirRecord* r = db_info_records(info);
        std::size_t i = 0;
        for (const auto& [mesh, step] : e) {
            r[i].mesh_no = mesh;
            r[i].step = step;
            r[i].version = static_cast<std::int32_t>(100 + i);
            i = i + 1;
        }
    }
};

} // namespace

TEST_CASE(versions_compatible_table)
{
    CHECK(compat(2, 10000, 10000));
    CHECK(compat(2, 1, 10001)); // anything goes from version 10000 on
    CHECK(compat(2, 9000, 9000));
    CHECK(compat(2, 9000, 0x2332));
    CHECK(compat(2, 9000, 0x238c));
    CHECK(compat(2, 0x2332, 0x238c));
    CHECK(!compat(2, 0x238c, 0x2332));
    CHECK(!compat(2, 6000, 0x177a)); // the 3D pairs do not count in 2D
    CHECK(compat(3, 6000, 0x177a));
    CHECK(compat(3, 6000, 0x17d4));
    CHECK(compat(3, 0x177a, 0x17d4));
    CHECK(!compat(3, 9000, 0x2332));
    CHECK(compat(3, 1234, 1234));
}

TEST_CASE(place_step_inside_and_append)
{
    Dir d({{1, 1}, {2, 5}, {3, 10}});
    std::int32_t count = 0, pos = 0, prev = 0, prev_step = 0, last_mesh = 0;
    std::int32_t nstart = 7, mesh = 3;
    CHECK_EQ(place_step(&d.info, &nstart, &count, &pos, &prev, &prev_step, &mesh, &last_mesh), 0);
    CHECK_EQ(count, 3);
    CHECK_EQ(pos, 3);
    CHECK_EQ(prev, 2);
    CHECK_EQ(prev_step, 5);
    CHECK_EQ(last_mesh, 2);
    mesh = 1; // lower MESHNO before the end of the directory
    CHECK_EQ(place_step(&d.info, &nstart, &count, &pos, &prev, &prev_step, &mesh, &last_mesh), -1);
    nstart = 20; // appended: a lower MESHNO is accepted
    CHECK_EQ(place_step(&d.info, &nstart, &count, &pos, &prev, &prev_step, &mesh, &last_mesh), 0);
    CHECK_EQ(pos, 4);
    CHECK_EQ(prev, 3);
    CHECK_EQ(prev_step, 10);
    CHECK_EQ(last_mesh, 3);
}

TEST_CASE(place_step_negative_entry_and_first)
{
    Dir d({{1, 1}, {1, -5}, {1, 9}});
    std::int32_t count = 0, pos = 0, prev = 0, prev_step = -7, last_mesh = -7;
    std::int32_t nstart = 5, mesh = 1;
    CHECK_EQ(place_step(&d.info, &nstart, &count, &pos, &prev, &prev_step, &mesh, &last_mesh), 0);
    CHECK_EQ(pos, 3); // a positive step goes after the negative step of the same number
    CHECK_EQ(prev, 2);
    CHECK_EQ(prev_step, 1); // first entry with the mesh number of the entry before pos
    nstart = -5;
    CHECK_EQ(place_step(&d.info, &nstart, &count, &pos, &prev, &prev_step, &mesh, &last_mesh), 0);
    CHECK_EQ(pos, 2);
    nstart = 1;
    prev_step = -7;
    last_mesh = -7;
    CHECK_EQ(place_step(&d.info, &nstart, &count, &pos, &prev, &prev_step, &mesh, &last_mesh), 0);
    CHECK_EQ(pos, 1);
    CHECK_EQ(prev, 0);
    CHECK_EQ(prev_step, -7); // untouched
    CHECK_EQ(last_mesh, -7);
}

TEST_CASE(db_max_version_and_new)
{
    Cls_1401dbfe0 empty;
    CHECK_EQ(db_max_version(&empty), 10000);
    Dir d({{1, 1}, {1, 2}, {1, 3}});
    db_info_records(d.info)[1].version = 500;
    CHECK_EQ(db_max_version(&d.info), 500);
    const std::int32_t v = 0x2774;
    db_info_set_new(&d.info, "A.DBtemp", &v);
    const char* base = reinterpret_cast<const char*>(&d.info);
    CHECK_EQ(std::string(base + 0x650), std::string("A.DBtemp"));
    std::int32_t got = 0;
    std::memcpy(&got, base + 0xabc, 4);
    CHECK_EQ(got, v);
}

TEST_CASE(adjust_left_and_char_assign)
{
    char s[8] = {' ', '\0', 'a', 'b', ' ', 'c', ' ', ' '};
    CHECK_EQ(adjust_left(s, 8), 4);
    CHECK_EQ(std::string(s, 8), std::string("ab c    "));
    char b[4] = {' ', '\0', ' ', ' '};
    CHECK_EQ(adjust_left(b, 4), 0);
    CHECK_EQ(std::string(b, 4), std::string(" \0  ", 4)); // all blank: left alone
    char x[6];
    char_assign(x, "OLD", 6, 3);
    CHECK_EQ(std::string(x, 6), std::string("OLD   "));
    char_assign(x, "LONGER", 3, 6);
    CHECK_EQ(std::string(x, 3), std::string("LON"));
}

TEST_CASE(record2_and_step_words)
{
    char title[0x50];
    std::memset(title, 0, sizeof(title));
    std::memcpy(title, "  Ring test", 11);
    const char smi[33] = "11.02   " " 0.00   " "15.00   " "-1.00   ";
    const std::int32_t istart = 4, ismtyp = 2, nrsets = 6, nxtrec = 77, w33 = 1, w34 = 0;
    set_record2(title, smi, &istart, &ismtyp, &nrsets, &nxtrec, &w33, &w34);
    const std::string t(g_db.h.title, 0x50);
    CHECK_EQ(t.substr(0, 9), std::string("Ring test"));
    CHECK(t.find_first_not_of(' ', 9) == std::string::npos);
    CHECK_EQ(std::string(g_db.h.smi1, 8), std::string("11.02   "));
    CHECK_EQ(std::string(g_db.h.smi4, 8), std::string("-1.00   "));
    CHECK_EQ(g_db.h.istart, 4);
    CHECK_EQ(g_db.h.ismtyp, 2);
    CHECK_EQ(g_db.h.nrsets, 6);
    CHECK_EQ(g_db.h.nxtrec, 77);
    CHECK_EQ(g_db.h.w33, 1);
    CHECK_EQ(g_db.h.w34, 0);

    const std::int32_t mesh = 3, step = -12, interval = 5, sim = 2, mode = 1, fam = 2, ver = 0x2774;
    set_step_words(&mesh, &step, &interval, &sim, &mode, &fam, &ver);
    CHECK_EQ(g_db.put_mesh_no, 3);
    CHECK_EQ(g_db.put_step, -12);
    CHECK_EQ(g_db.put_interval, 5);
    CHECK_EQ(g_db.put_sim_no, 2);
    CHECK_EQ(g_db.put_sim_mode, 1);
    CHECK_EQ(g_db.step_family, 2);
    CHECK_EQ(g_db.step_version, 0x2774);
}

int main() { return testkit::run_all(); }
