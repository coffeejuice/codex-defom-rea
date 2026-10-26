// ctest-labels: p67-7 ctc3d
// Tier-1 tests of the SRCNT group (packet p67-7, group "srcnt"): the box diagonal, the
// DIE_000.GEO name buffer, the DIE_00k.GEO record formats against the oracle's DIE_001.GEO
// (s3d_diegeo_dat), SRCNT2.DAT reading, and the routines' paths that need no cube.
#include "../src/ctc3d/srcnt/srcnt_internal.hpp"

#include <defcmd/ctc3d/common.hpp>
#include <defcmd/ctc3d/srcnt.hpp>
#include <defrt/ftn/fio.hpp>
#include <testkit/check.hpp>

#include <cstdio>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#ifdef _WIN32
#include <process.h>
#define DEF_GETPID _getpid
#else
#include <unistd.h>
#define DEF_GETPID getpid
#endif

using namespace def::cmd::ctc3d;
namespace sd = def::cmd::ctc3d::srcnt_detail;
namespace fio = def::rt::ftn::fio;
namespace fs = std::filesystem;

namespace {

fs::path temp_dir()
{
    fs::path d = fs::temp_directory_path() / ("ctc3d_srcnt_" + std::to_string(DEF_GETPID()));
    fs::create_directories(d);
    return d;
}

std::string read_all(const fs::path& p)
{
    std::ifstream f(p, std::ios::binary);
    std::ostringstream s;
    s << f.rdbuf();
    return s.str();
}

void write_all(const fs::path& p, const std::string& s)
{
    std::ofstream f(p, std::ios::binary);
    f << s;
}

} // namespace

TEST_CASE(box_diagonal)
{
    reset_image();
    const double box[6] = {-1.5, 0.25, 3.0, 10.0, 7.75, 4.5};
    double d = 0.0;
    fun_14003b0c6(box, &d);
    const double dx = box[3] - box[0], dy = box[4] - box[1], dz = box[5] - box[2];
    const double s = dz * dz + (dy * dy + dx * dx);
    CHECK_BITS(d, std::sqrt(s));
    CHECK_BITS(m<double>(0x140655a30), dx);
    CHECK_BITS(m<double>(0x140655a38), dy);
    CHECK_BITS(m<double>(0x140655a40), dz);
}

TEST_CASE(partner_negative_node_flag_without_partners)
{
    reset_image();
    dat_14077867c = 0;
    i32 obj = 2, flag = 7;
    fun_14003b130(&obj, &flag);
    CHECK_EQ(flag, 0);
}

TEST_CASE(die_name_buffer)
{
    reset_image();
    auto name = [] { return std::string(sd::die_name(), 11); };
    CHECK_EQ(name(), std::string("DIE_000.GEO"));
    sd::die_name_set(1);
    CHECK_EQ(name(), std::string("DIE_001.GEO"));
    sd::die_name_reset(1);
    CHECK_EQ(name(), std::string("DIE_001.GEO"));
    sd::die_name_set(3);
    CHECK_EQ(name(), std::string("DIE_003.GEO"));
    sd::die_name_set(12);
    CHECK_EQ(name(), std::string("DIE_012.GEO"));
    sd::die_name_reset(12);
    CHECK_EQ(name(), std::string("DIE_002.GEO")); // only the tens digit is reset
    sd::die_name_set(150);                          // >= 100: unchanged
    CHECK_EQ(name(), std::string("DIE_002.GEO"));
    sd::die_name_reset(150);
    CHECK_EQ(name(), std::string("DIE_002.GEO"));
    sd::die_name_set(-1); // I1 of -1 overflows
    CHECK_EQ(name(), std::string("DIE_00*.GEO"));
}

TEST_CASE(die_geo_records_match_oracle)
{
    const fs::path oracle = testkit::repo_root() / "port/defcmd/tests/data/ctc3d/DIE_001.GEO";
    const std::string want = read_all(oracle);
    REQUIRE(want.size() == 5424u);
    // parse the oracle's numbers
    std::istringstream in(want);
    int np = 0;
    in >> np;
    std::vector<int> pid(np);
    std::vector<double> xyz(3 * static_cast<std::size_t>(np));
    for (int i = 0; i < np; ++i) {
        in >> pid[i] >> xyz[3 * i] >> xyz[3 * i + 1] >> xyz[3 * i + 2];
    }
    int ne = 0;
    in >> ne;
    std::vector<int> e(5 * static_cast<std::size_t>(ne));
    for (int& v : e) {
        in >> v;
    }
    REQUIRE(np == 56 && ne == 54);

    const fs::path out = temp_dir() / "DIE_001.GEO";
    write_all(out, std::string(9000, 'x')); // an older, longer file: STATUS='UNKNOWN' + REWIND keeps no tail
    REQUIRE(fio::open_unknown(81, out.string().c_str()));
    fio::rewind(81);
    sd::geo_write_count(np);
    for (int i = 0; i < np; ++i) {
        sd::geo_write_point(pid[i], &xyz[3 * static_cast<std::size_t>(i)]);
    }
    sd::geo_write_count(ne);
    for (int j = 0; j < ne; ++j) {
        const int* r = &e[5 * static_cast<std::size_t>(j)];
        sd::geo_write_poly(r[0], r[1], r[2], r[3], r[4]);
    }
    fio::close(81);
    const std::string got = read_all(out);
    CHECK_EQ(got.size(), want.size());
    CHECK(got == want);
    fs::remove(out);
}

TEST_CASE(srcnt2_dat_read_once)
{
    reset_image();
    const fs::path dir = temp_dir();
    const fs::path old = fs::current_path();
    fs::current_path(dir);
    write_all(dir / "SRCNT2.DAT", "3 7\n");
    // defaults 5 / 20 (DAT_1405c66b4 / DAT_1405c66b8); with no partner polygons the routine only
    // reads the file
    CHECK_EQ(m<i32>(0x1405c66b4), 5);
    CHECK_EQ(m<i32>(0x1405c66b8), 20);
    i32 obj = 1, inst = 1, npoly = 0, oobj = 2, p7 = 0;
    double tol = 0.5;
    double box[6] = {0, 0, 0, 1, 1, 1};
    fun_14002cdee(&obj, &inst, &npoly, &tol, box, &oobj, &p7);
    CHECK_EQ(m<i32>(0x1405c66b0), 1);
    CHECK_EQ(m<i32>(0x1405c66b4), 3);
    CHECK_EQ(m<i32>(0x1405c66b8), 7);
    CHECK_BITS(m<double>(0x14064ca88), 0.5 * 20.0);
    // once per process: a changed file is not read again
    write_all(dir / "SRCNT2.DAT", "9 9\n");
    fun_14002cdee(&obj, &inst, &npoly, &tol, box, &oobj, &p7);
    CHECK_EQ(m<i32>(0x1405c66b4), 3);
    // a file that cannot be read keeps the defaults
    reset_image();
    write_all(dir / "SRCNT2.DAT", "abc\n");
    fun_14002cdee(&obj, &inst, &npoly, &tol, box, &oobj, &p7);
    CHECK_EQ(m<i32>(0x1405c66b4), 5);
    CHECK_EQ(m<i32>(0x1405c66b8), 20);
    fs::remove(dir / "SRCNT2.DAT");
    fs::current_path(old);
}

TEST_CASE(diegeo_without_partners_writes_nothing)
{
    reset_image();
    dat_14077867c = 0;
    i32 o = 0, i = 1, p = 0;
    fun_14003cd08(&o, &i, &p);
    CHECK_EQ(m<i32>(0x14066aa4c), 1);
}

TEST_CASE(symmetry_polygons_without_planes)
{
    reset_image();
    i32 obj = 1, inst = 1, npoly = 5, nplane = 0;
    double size = 4.0;
    fun_14003d2ee(&obj, &inst, &npoly, mp<double>(0x140655b00), mp<double>(0x140656460), mp<double>(0x140656dc0), &nplane, &size);
    CHECK_EQ(m<std::int64_t>(0x14066aa70), 0);
    CHECK_EQ(m<i32>(0x14066aa80), 0x18);
}

int main()
{
    int r = testkit::run_all();
    std::error_code ec;
    fs::remove_all(temp_dir(), ec);
    return r;
}
