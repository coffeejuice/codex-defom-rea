// Writer round trips on synthetic steps (no DEFORM files needed): 2D steps,
// 3D region steps with dynamic arrays, header-only entries, and directories that
// need more than one directory chunk and more than one index record.
#include <cstdio>
#include <filesystem>
#include <system_error>
#include <random>
#include <string>
#include <vector>

#include "deformdb/database.hpp"
#include "deformdb/writer.hpp"

using namespace deformdb;

static int failures = 0;
#define CHECK(c)                                                                         \
    do {                                                                                 \
        if (!(c)) { std::printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #c); ++failures; } \
    } while (0)

static std::mt19937 rng(7);

static StepData make_2d(int step) {
    StepData s;
    s.entry.mesh_no = 1;
    s.entry.step = step;
    s.entry.sim_no = 1;
    s.entry.sim_mode = 2;
    s.entry.dim = 2;
    s.entry.version = 9000;
    s.header.assign(34, 0.0);
    s.header[0] = 1;
    s.header[2] = 0.5 * step;
    s.mar.resize(700);
    for (auto& v : s.mar) v = (rng() % 3 == 0) ? static_cast<std::int32_t>(rng() % 50) : 0;
    s.a.resize(900);
    for (auto& v : s.a) v = (rng() % 3 == 0) ? (rng() % 1000) / 7.0 : 1.25;
    return s;
}

// 3D v11000 step with a 3-region MAR table, a 2-region A table (one region empty)
// and one integer plus one real dynamic array.
static StepData make_3d(int step) {
    StepData s;
    s.entry.mesh_no = 2;
    s.entry.step = step;
    s.entry.sim_no = 3;
    s.entry.sim_mode = 5;
    s.entry.dim = 3;
    s.entry.version = 11000;
    s.header.assign(500, 0.0);
    s.header[0] = 2;
    s.mar.resize(300);
    for (auto& v : s.mar) v = static_cast<std::int32_t>(rng() % 4);
    s.a.resize(400);
    for (auto& v : s.a) v = (rng() % 2) ? 0.0 : (rng() % 100) * 0.5;
    // R, word 2, then (length, start, stored, record) per region.
    s.mar_table = {3, 200, 100, 1, 0, 0, 0, 101, 0, 0, 200, 101, 0, 0};
    s.a_table = {2, 400, 400, 1, 0, 0, 0, 401, 0, 0};
    s.dynamic_descriptor = {1, 50, 0, 500, 0, 0, 0, 0, 0, 0, 1, 60, 0, 2000, 0, 0, 0, 0, 0, 0};
    std::array<std::int32_t, 10> irow{40, 0, 0, 0, 1, 1, 2, 4, 0, 0};
    std::array<std::int32_t, 10> rrow{60, 0, 0, 0, 1, 1, 1, 3, 0, 0};
    s.dynamic_int_rows = {irow};
    s.dynamic_real_rows = {rrow};
    DynamicIntArray di;
    di.slot = 1;
    for (int k = 0; k < 40; ++k) di.values.push_back(k / 8);
    DynamicRealArray dr;
    dr.slot = 1;
    for (int k = 0; k < 60; ++k) dr.values.push_back(k % 5 == 0 ? 1.0 : 0.0);
    s.dynamic_ints = {di};
    s.dynamic_reals = {dr};
    return s;
}

int main() {
    namespace fs = std::filesystem;
    // One directory per run: tier-1 runs of several worktrees share the temp directory, and a
    // fixed name let one run delete or overwrite the files another run was reading.
    const fs::path dir = fs::temp_directory_path() /
                         ("deformdb_test_writer_" + std::to_string(std::random_device{}()));
    fs::create_directories(dir);

    {   // mixed steps
        const fs::path p = dir / "mixed.DB";
        std::vector<StepData> in = {make_2d(-1), make_2d(1), make_3d(2), make_3d(3)};
        {
            WriterHeader h;
            h.title = "WRITER TEST";
            h.release_date = "06/30/14";
            h.today = "01/02/26";
            auto w = Writer::create(p, h);
            w->append_step(in[0]);
            w->append_step(in[1], true);
            w->append_step(in[2]);
            w->append_step(in[3], true);
        }
        auto db = Database::open(p);
        CHECK(db->header().title == "WRITER TEST");
        CHECK(db->header().entry_count == 4);
        CHECK(db->header().dates[1] == "01/02/26");
        CHECK(db->entries().size() == 4);
        auto s0 = db->read_step(db->entries()[0]);
        CHECK(s0.mar == in[0].mar);
        CHECK(s0.a == in[0].a);
        CHECK(s0.header[2] == in[0].header[2]);
        CHECK(s0.header[8] == 2.0);  // 2D platform code
        CHECK(db->entries()[1].mar_size == 0 && db->entries()[1].a_size == 0);
        auto s1 = db->read_step(db->entries()[1]);
        CHECK(s1.header.size() == 34 && s1.header[6] == 0.0 && s1.header[7] == 0.0);
        auto s2 = db->read_step(db->entries()[2]);
        CHECK(s2.mar == in[2].mar);
        CHECK(s2.a == in[2].a);
        CHECK(s2.header[5] == 2.0 && s2.header[6] == 300.0 && s2.header[7] == 400.0);
        CHECK(db->entries()[2].mar_size == 14 && db->entries()[2].a_size == 10);
        CHECK(s2.dynamic_ints.size() == 1 && s2.dynamic_ints[0].values == in[2].dynamic_ints[0].values);
        CHECK(s2.dynamic_reals.size() == 1 && s2.dynamic_reals[0].values == in[2].dynamic_reals[0].values);
        CHECK(s2.dynamic_ints.size() == 1 && s2.dynamic_ints[0].descriptor[6] == 2);
        CHECK(db->entries()[3].mar_size == 0 && db->entries()[3].rec_dynamic == 0);
    }

    {   // 16 300 header-only entries: 128 directory chunks, two index records
        const fs::path p = dir / "many.DB";
        StepData s;
        s.entry.mesh_no = 1;
        s.entry.dim = 3;
        s.entry.version = 11000;
        s.header.assign(9, 0.0);
        const int n = 16300;
        {
            WriterHeader h;
            h.today = "01/02/26";
            auto w = Writer::create(p, h);
            for (int k = 1; k <= n; ++k) {
                s.entry.step = k;
                s.header[2] = k;
                w->append_step(s, true);
            }
        }
        auto db = Database::open(p);
        CHECK(static_cast<int>(db->entries().size()) == n);
        bool ok = true;
        for (int k : {1, 128, 129, 16256, 16257, 16300}) {
            const auto& e = db->entries()[static_cast<std::size_t>(k - 1)];
            auto st = db->read_step(e);
            ok = ok && e.step == k && st.header[2] == k;
        }
        CHECK(ok);
    }

    // Best-effort cleanup. The throwing overload ended the test with an uncaught exception
    // (abort, exit 0xC0000409) when a scanner still held one of the fresh files under parallel
    // ctest; the results above are already decided.
    std::error_code ec;
    fs::remove_all(dir, ec);
    std::printf(failures ? "%d failures\n" : "all writer tests passed\n", failures);
    return failures ? 1 : 0;
}
