// ctest-labels: p9-01 cnvt3d
// Tier-1 tests of packet p9-01 (CNVT3D driver helpers): status/log files, the argument reading
// of the command case, the axis-mapping copies, WPAXIS records, the surface-patch geometry
// helpers, the outline file reader/writer and FIXGEO with a recorded launch, the COW list
// front / pop_front. Every test that writes files runs in its own per-process temp directory.
#define _CRT_SECURE_NO_WARNINGS
#include <defcmd/cmd_p9_01.hpp>
#include <defcmd/cnvt3d.hpp>

#include <cmath>
#include <cstdio>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <limits>
#include <sstream>
#include <string>

#include <process.h>

#include <defmodel/geoprim/fixgeo.hpp>
#include <defmodel/layout/cls_1401e7b10.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/obj_geo2d.hpp>
#include <defmodel/linked/record_e8_list.hpp>
#include <defmodel/sdctl/cow.hpp>
#include <testkit/check.hpp>

namespace fs = std::filesystem;
using namespace def::cmd::cnvt3d;

namespace {

struct TempDir {
    fs::path old, dir;
    explicit TempDir(const char* tag)
    {
        old = fs::current_path();
        dir = fs::temp_directory_path() / (std::string("p901_") + tag + "_" + std::to_string(_getpid()));
        fs::remove_all(dir);
        fs::create_directories(dir);
        fs::current_path(dir);
    }
    ~TempDir()
    {
        fs::current_path(old);
        std::error_code ec;
        fs::remove_all(dir, ec);
    }
};

std::string slurp(const char* name)
{
    std::ifstream in(name, std::ios::binary);
    std::ostringstream s;
    s << in.rdbuf();
    return s.str();
}

// a FILE* holding `text`, positioned at its start
std::FILE* text_file(const std::string& text)
{
    std::FILE* f = std::tmpfile();
    std::fputs(text.c_str(), f);
    std::rewind(f);
    return f;
}

} // namespace

TEST_CASE(log_writes_status_file_log_and_stdout)
{
    TempDir t("log");
    log_message("first", " ");
    // CONV_STATUS.DAT missing -> created ("a"); " .LOG" too
    CHECK_EQ(slurp("CONV_STATUS.DAT"), std::string("first\r\n"));
    CHECK_EQ(slurp(" .LOG"), std::string("first\r\n"));
    log_message("second", "DEFORM");
    CHECK_EQ(slurp("CONV_STATUS.DAT"), std::string("first\r\nsecond\r\n"));
    CHECK_EQ(slurp("DEFORM.LOG"), std::string("second\r\n"));
    // a ".DB" stem goes through 1401b1520 (p8-08): recorded stand-in
    ext().str_left = [](std::string& out, const std::string& s, std::int32_t n) { out = s.substr(0, static_cast<std::size_t>(n)); };
    log_message("third", "RING.db");
    CHECK_EQ(slurp("RING.LOG"), std::string("third\r\n"));
    // exactly ".DB" is not longer than the suffix: kept
    log_message("fourth", ".DB");
    CHECK_EQ(slurp(".DB.LOG"), std::string("fourth\r\n"));
    reset_ext();
    delete_status_file();
    CHECK(!fs::exists("CONV_STATUS.DAT"));
}

TEST_CASE(cnv23_file_modes)
{
    TempDir t("cnv23");
    cnv23_file(1, 7);
    CHECK_EQ(slurp("DEF_CNV23.DAT"), std::string("\tSimNo\t7\r\n"));  // "w" is text mode, as the original's
    cnv23_file(0, 0);
    CHECK(!fs::exists("DEF_CNV23.DAT"));
    cnv23_file(1, 0);
    CHECK(fs::exists("DEF_CNV23.DAT"));
    CHECK_EQ(slurp("DEF_CNV23.DAT"), std::string());
    log_begin();
    log_end();
    CHECK_EQ(slurp(" .LOG"), std::string("$BGN$\r\n$END$\r\n"));
}

TEST_CASE(command_case_reads_the_count_and_returns_without_ids)
{
    def::model::DB_STEP_DATA m(2);
    std::FILE* f = text_file(" 0\nKFWRIT\n");
    CHECK_EQ(def::cmd::cmd_cnvt3d(&m, f), 0);
    char line[64] = {0};
    std::fgets(line, sizeof line, f);  // nothing else was read
    CHECK_EQ(std::string(line), std::string("KFWRIT\n"));
    std::fclose(f);
    std::FILE* e = text_file("");
    CHECK_EQ(def::cmd::cmd_cnvt3d(&m, e), 2);  // EOF on the keyword line
    std::fclose(e);
    std::FILE* g = text_file(" 2\n");
    CHECK_EQ(def::cmd::cmd_cnvt3d(&m, g), 2);  // EOF on the id line
    std::fclose(g);
}

TEST_CASE(props_copy_axis_swaps_unless_flag)
{
    std::byte s[0x48] = {}, d[0x48] = {};
    double v[3] = {1.0, 2.0, 3.0};
    std::memcpy(s + 0x30, v, 24);
    s[0x18] = std::byte{5};
    s[0x28] = std::byte{9};
    props_copy_axis(d, s, 0);
    double out[3];
    std::memcpy(out, d + 0x30, 24);
    CHECK_EQ(out[0], 1.0);
    CHECK_EQ(out[1], 3.0);
    CHECK_EQ(out[2], 2.0);
    CHECK(d[0x18] == std::byte{5});
    CHECK(d[0x28] == std::byte{9});
    props_copy_axis(d, s, 1);
    std::memcpy(out, d + 0x30, 24);
    CHECK_EQ(out[1], 2.0);
}

TEST_CASE(sim_ctrl_axes_swap_and_codes)
{
    static std::byte sb[sizeof(def::model::Cls_1401e7b10)], db[sizeof(def::model::Cls_1401e7b10)];
    auto* s = reinterpret_cast<def::model::Cls_1401e7b10*>(sb);
    auto* d = reinterpret_cast<def::model::Cls_1401e7b10*>(db);
    double a[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::memcpy(sb + 0x180, a, sizeof a);
    std::int32_t code = 2;
    std::memcpy(sb + 0x290, &code, 4);
    sim_ctrl_copy_axes(d, s, 0);
    double o[9];
    std::memcpy(o, db + 0x180, sizeof o);
    CHECK_EQ(o[0], 1.0);
    CHECK_EQ(o[1], 3.0);
    CHECK_EQ(o[2], 2.0);
    CHECK_EQ(o[4], 6.0);
    CHECK_EQ(o[8], 8.0);
    std::int32_t c = 0;
    std::memcpy(&c, db + 0x290, 4);
    CHECK_EQ(c, 3);
    sim_ctrl_copy_axes(d, s, 1);
    std::memcpy(o, db + 0x180, sizeof o);
    CHECK_EQ(o[1], 2.0);
    std::memcpy(&c, db + 0x290, 4);
    CHECK_EQ(c, 2);
}

TEST_CASE(wpaxis_kind4_and_axis)
{
    def::model::linked::Cls_140258580List l;
    const char no = 0, yes = 1;
    CHECK(wpaxis_find_kind4(&l, &no) == nullptr);
    CHECK_EQ(wpaxis_axis(&l), 0);
    def::model::linked::Cls_140258580* r = wpaxis_find_kind4(&l, &yes);
    REQUIRE(r != nullptr);
    CHECK_EQ(l.count, 1);
    CHECK(wpaxis_find_kind4(&l, &no) == r);
    const double z[3] = {0.0, 0.0, 5.0};  // normalised by the setter
    wpaxis_set_80(r, z);
    CHECK_EQ(wpaxis_axis(&l), 3);
    const double y[3] = {0.0, 2.0, 0.0};
    wpaxis_set_80(r, y);
    CHECK_EQ(wpaxis_axis(&l), 2);
    const double x[3] = {3.0, 0.0, 0.0};
    wpaxis_set_80(r, x);
    CHECK_EQ(wpaxis_axis(&l), 1);
    const double q[3] = {1.0, 1.0, 0.0};
    wpaxis_set_80(r, q);
    CHECK_EQ(wpaxis_axis(&l), 0);
    const double nan3[3] = {std::numeric_limits<double>::quiet_NaN(), 0.0, 0.0};
    std::memcpy(reinterpret_cast<std::byte*>(r) + 0x80, nan3, 24);
    CHECK_EQ(wpaxis_record_axis(r), 0);
}

TEST_CASE(plane_distance_and_triangle_normal)
{
    const double p[3] = {1.0, 2.0, 3.0}, n[3] = {0.0, 0.0, 1.0}, q[3] = {0.0, 0.0, 1.0};
    CHECK_EQ(plane_distance(p, n, q), 2.0);
    const double a[3] = {1, 0, 0}, b[3] = {0, 0, 0}, c[3] = {0, 1, 0};
    double nn[3];
    triangle_normal(a, b, c, nn);  // (c-b) x (a-b) = y x x = -z
    CHECK_EQ(nn[0], 0.0);
    CHECK_EQ(nn[1], 0.0);
    CHECK_EQ(nn[2], -1.0);
    triangle_normal(a, a, a, nn);  // zero: left unnormalised
    CHECK_EQ(nn[2], 0.0);
}

TEST_CASE(fixgeo_options_and_outline_files)
{
    TempDir t("fixgeo");
    def::model::geoprim::FixgeoOptions o;
    CHECK_EQ(o.tol_a, 0.001);
    CHECK_EQ(o.angle_a, 20.0);
    CHECK_EQ(o.angle_b, 300.0);
    CHECK_EQ(o.i20, 0);
    def::model::OBJ_GEO2D g;
    g.curve_type = 1;
    g.n_points = 3;
    g.allocate_points();
    const double pts[9] = {0.0, 0.0, 0.0, 2.0, 0.0, 0.0, 2.0, 1.0, 0.0};
    std::memcpy(g.points, pts, sizeof pts);
    CHECK_EQ(def::model::geoprim::write_geo_file(&g, nullptr), -2);
    CHECK_EQ(def::model::geoprim::write_geo_file(&g, "A.GEO"), 0);
    const std::string txt = slurp("A.GEO");
    CHECK(txt.rfind("  2\r\n  3   1\r\n 1    0.0000000000e+000    0.0000000000e+000", 0) == 0);
    def::model::OBJ_GEO2D h;
    CHECK_EQ(def::model::geoprim::read_geo_file(&h, "A.GEO"), 0);
    CHECK_EQ(h.curve_type, 1);
    CHECK_EQ(h.n_points, 3);
    CHECK_EQ(h.points[3], 2.0);
    CHECK_EQ(def::model::geoprim::negative_x_check(&h), 0);
    h.points[0] = -0.0005;  // within 0.0005 * width 2
    CHECK_EQ(def::model::geoprim::negative_x_check(&h), 1);
    h.points[0] = -0.5;
    CHECK_EQ(def::model::geoprim::negative_x_check(&h), -1);
    CHECK_EQ(def::model::geoprim::read_geo_file(&h, "MISSING.GEO"), -3);
}

namespace {
std::string g_cmd;
void fake_fixgeo(std::int32_t mode, const char* cmd)
{
    g_cmd = std::to_string(mode) + ":" + cmd;
    // the program reads FIXGEO.INI / FIXGEO_BUF.GEO; it writes NEWGEO.GEO (one point moved)
    std::ifstream ini("FIXGEO.INI", std::ios::binary);
    std::ostringstream s;
    s << ini.rdbuf();
    g_cmd += "|" + s.str();
    std::ofstream o("NEWGEO.GEO");
    o << "  2\n  2   1\n 1 1.0 0.0 0.0\n 2 3.0 1.0 0.0\n";
}
} // namespace

TEST_CASE(fixgeo_runs_the_program_and_reads_newgeo)
{
    TempDir t("fixgeo2");
    def::model::geoprim::g_fixgeo_launch = fake_fixgeo;
    def::model::OBJ_GEO2D g;
    g.curve_type = 1;
    g.n_points = 1;
    g.allocate_points();
    def::model::geoprim::FixgeoOptions o;
    std::int32_t neg = 99;
    CHECK_EQ(def::model::geoprim::fixgeo(&g, &o, &neg, 0, 1), 1);
    CHECK_EQ(g_cmd.substr(0, g_cmd.find('|')), std::string("1:FIXGEO.EXE > DEF_GEO.TMP"));
    CHECK_EQ(g_cmd.substr(g_cmd.find('|') + 1),
             std::string("FIXGEO_BUF.GEO\r\n0.001000  0.174533  5.235988\r\n0  0\r\n0  0\r\n"));
    CHECK_EQ(g.n_points, 2);
    CHECK_EQ(g.points[3], 3.0);
    CHECK_EQ(neg, 0);
    CHECK(!fs::exists("FIXGEO.INI"));
    CHECK(!fs::exists("FIXGEO_BUF.GEO"));
    CHECK(fs::exists("NEWGEO.GEO"));
    fs::remove("NEWGEO.GEO");
    def::model::geoprim::g_fixgeo_launch = [](std::int32_t, const char*) {};
    CHECK_EQ(def::model::geoprim::fixgeo(&g, &o, &neg, 1, 0), 0);
    def::model::geoprim::g_fixgeo_launch = nullptr;
}

TEST_CASE(cow_list_front_and_pop_front)
{
    def::model::CowList<void*> a;
    int x = 0, y = 0;
    a.push_back(&x);
    a.push_back(&y);
    def::model::CowList<void*> b(a);  // shared
    CHECK(a.mutable_front() == &x);   // detaches a
    CHECK_EQ(b.use_count(), 1);
    a.pop_front();
    CHECK_EQ(a.size(), static_cast<std::size_t>(1));
    CHECK(a.mutable_front() == &y);
    CHECK_EQ(b.size(), static_cast<std::size_t>(2));
}

TEST_CASE(geo23_and_window_shape)
{
    std::int32_t m = 0;
    CHECK(!geo23_active(&m));
    m = 2;
    CHECK(geo23_active(&m));
    m = -1;
    CHECK(!geo23_active(&m));
    const char* p = "X";
    CHECK(key_name(p) == p);
}

int main() { return testkit::run_all(); }
