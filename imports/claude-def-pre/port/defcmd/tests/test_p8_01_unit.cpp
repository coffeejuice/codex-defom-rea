// ctest-labels: p8-01
// Tier-1 tests of packet p8-01 that need no oracle cache: GFREAD extension and type codes
// (GFREAD.md section 3), the loader defaults, the 2D front end 140244010 / 140244200 / 140244500 /
// 1402442f0 through the seams' test hooks (the readers are p8-03 / p8-04 / p8-09), the 3D
// dispatch, the Uic2CObject member bodies on raw storage, the object-field accessors, and the
// argument grammar of MORP, ARMCTL, EXECMD and GFREAD (MORP.md, ARMCTL.md, EXECMD.md).
#include "p8_01_support.hpp"

#include "../src/p8_01/ext_p8_03.hpp"
#include "../src/p8_01/ext_p8_04.hpp"
#include "../src/p8_01/ext_p8_09.hpp"

#include <bit>

#include <defmodel/llobj_ctor/llobj_ctor.hpp>
#include <testkit/check.hpp>

namespace {

namespace gf = def::cmd::gfread;
using def::model::DB_STEP_DATA;
using def::model::LLOBJ_DATA;

template <class T>
T& at(void* p, std::size_t off)
{
    return *reinterpret_cast<T*>(static_cast<unsigned char*>(p) + off);
}

std::string ext_of(const char* file, bool* ok)
{
    char ex[16] = {};
    *ok = gf::fun_140243370(file, ex);
    return ex;
}

DB_STEP_DATA* model(int dim)
{
    auto* m = new DB_STEP_DATA(3);  // leaked (as the runner's)
    std::int32_t d = dim;
    m->set_dimension(&d);
    return m;
}

// a working directory of its own for the commands that write files
struct TmpCwd {
    std::filesystem::path home, dir;
    explicit TmpCwd(const std::string& tag)
    {
        home = std::filesystem::current_path();
        dir = std::filesystem::temp_directory_path() / keytest::tmp_name("p801u_" + tag);
        std::filesystem::create_directories(dir);
        std::filesystem::current_path(dir);
    }
    ~TmpCwd()
    {
        std::filesystem::current_path(home);
        std::error_code ec;
        std::filesystem::remove_all(dir, ec);
    }
};

std::vector<std::string> g_seq;  // calls seen by the reader hooks, in order

} // namespace

TEST_CASE(extension_and_type)
{
    bool ok = false;
    CHECK_EQ(ext_of("A.STL", &ok), std::string("STL"));
    CHECK(ok);
    CHECK_EQ(ext_of("ab.cd", &ok), std::string("cd"));  // '.' at len-3
    CHECK_EQ(ext_of("X.GEOM", &ok), std::string("GEOM"));
    CHECK_EQ(ext_of("x.abcde", &ok), std::string("abcde"));  // '.' at len-6: 5 letters
    CHECK(ok);
    (void)ext_of("x.abcdef", &ok);  // '.' at len-7 needs len > 7: "x.abcdef" has 8
    CHECK(ok);
    (void)ext_of("x.abcdefg", &ok);  // '.' at len-8: not tried
    CHECK(!ok);
    (void)ext_of("abc", &ok);
    CHECK(!ok);
    (void)ext_of("a.b", &ok);  // len 3: '.' at len-2 is never tried
    CHECK(!ok);
    (void)ext_of(".STL", &ok);  // '.' at len-4 needs len > 4
    CHECK(!ok);
    // the first dot from the right end side wins in the order len-3, len-4, ...
    CHECK_EQ(ext_of("a.b.KEY", &ok), std::string("KEY"));

    const struct {
        const char* e;
        int t;
    } t[] = {{"KEY", 0},  {"key", 0}, {"Key", -1}, {"DB", 1},   {"db", 1},   {"GEO", 2},  {"geom", 2}, {"STL", 3},
             {"stl", 3},  {"Stl", -1}, {"PDA", 4}, {"pat", 4},  {"UNV", 5},  {"unv", 5},  {"NAS", 10}, {"nas", 10},
             {"IGS", 6},  {"igs", 6},  {"DXF", 7}, {"dxfx", 7}, {"IGES", -1}, {"x", -1}};
    for (auto& c : t) CHECK_EQ(gf::fun_140243500(c.e), c.t);
}

TEST_CASE(loader_defaults)
{
    gf::Loader l{};
    int cb = 0;
    gf::fun_1402437c0(&l, &cb);
    CHECK_EQ(std::bit_cast<std::uint64_t>(l.tol), 0x3ee4f8b588e368f1ull);  // DAT_140522f98
    CHECK_EQ(l.scale, 1.0);
    CHECK(l.callback == &cb);
    gf::fun_140243810(&l);
}

TEST_CASE(object_fields)
{
    DB_STEP_DATA* m = model(2);
    LLOBJ_DATA* o = def::model::llobj_ctor::get_or_create(m, 2);
    REQUIRE(o != nullptr);
    gf::fun_14019a650(o, 7);
    CHECK_EQ(at<std::int32_t>(o, 0xbc8), 7);
    gf::fun_140256670(o, 6);
    CHECK_EQ(gf::fun_140256690(o), 6);
    gf::fun_1404098d0(o, 2.5);
    CHECK_EQ(at<double>(o, 0x3a48), 2.5);
    at<std::int32_t>(o, 0x3a50) = 3;
    at<double>(o, 0x3a58) = 0.125;
    CHECK_EQ(gf::fun_1404098f0(o), 3);
    CHECK_EQ(gf::fun_140409910(o), 3);
    CHECK_EQ(gf::fun_140409930(o), 0.125);
    alignas(8) unsigned char curve[0x30] = {};
    double pts[3] = {};
    gf::fun_14040cde0(reinterpret_cast<def::model::OBJ_GEO2D*>(curve), pts);
    CHECK(at<double*>(curve, 0x18) == pts);
}

TEST_CASE(uic2c_member_bodies_on_raw_storage)
{
    alignas(8) unsigned char b[0x28];
    std::memset(b, 0xcc, sizeof b);
    auto* iges = reinterpret_cast<gf::Uic2CObjectIgesFile*>(b);
    gf::fun_1404099d0(iges, nullptr);
    CHECK_EQ(at<std::int32_t>(b, 8), 0);
    CHECK_EQ(std::string(at<char*>(b, 0x10)), std::string(""));
    CHECK_EQ(at<std::int32_t>(b, 0x18), 0);
    CHECK_EQ(at<std::int32_t>(b, 0x1c), 0);
    CHECK_EQ(at<std::uint64_t>(b, 0x20), 0ull);
    gf::fun_14040ce40(iges);
    CHECK_EQ(at<std::int32_t>(b, 0x18), 1);
    gf::fun_14040ce60(iges, 4);
    CHECK_EQ(gf::fun_1402566b0(iges), 4);
    auto* base = reinterpret_cast<def::cmd::Uic2CObject*>(b);
    gf::fun_140454a90(base, "short");
    CHECK_EQ(std::string(at<char*>(b, 0x10)), std::string("short"));
    const std::string longname(200, 'n');
    gf::fun_140454a90(base, longname.c_str());  // > 0x7f: new buffer of len+1
    CHECK_EQ(std::string(at<char*>(b, 0x10)), longname);
    gf::fun_140454a90(base, "");
    CHECK_EQ(std::string(at<char*>(b, 0x10)), std::string(""));
    gf::fun_140409a90(iges);

    std::memset(b, 0xcc, sizeof b);
    auto* dxf = reinterpret_cast<gf::Uic2CObjectDxfFile*>(b);
    gf::fun_1404092b0(dxf, "name");
    CHECK_EQ(std::string(at<char*>(b, 0x10)), std::string("name"));
    CHECK_EQ(at<std::int32_t>(b, 0x1c), 0);
    at<std::int32_t>(b, 0x1c) = 9;
    CHECK_EQ(gf::fun_1402566c0(dxf), 9);
    gf::fun_140409370(dxf);
}

TEST_CASE(front_end_2d_geo_reads_twice_and_always_succeeds)
{
    g_seq.clear();
    gf::ext::g_geo_hooks.ctor = [](void*) { g_seq.push_back("ctor"); };
    gf::ext::g_geo_hooks.dtor = [](void*) { g_seq.push_back("dtor"); };
    gf::ext::g_geo_hooks.read = [](void*, const char* file, char* flag, std::int32_t mode, LLOBJ_DATA* obj) {
        g_seq.push_back(std::string("read ") + file + " " + std::to_string(*flag) + " " + std::to_string(mode) +
                        (obj != nullptr ? " obj" : " 0"));
        return mode == 3;
    };
    gf::ext::g_geo_hooks.store = [](void*, LLOBJ_DATA*, std::int32_t dim) {
        g_seq.push_back("store " + std::to_string(dim));
        return false;
    };
    DB_STEP_DATA* m = model(2);
    LLOBJ_DATA* o = def::model::llobj_ctor::get_or_create(m, 2);
    gf::Loader l{};
    gf::fun_1402437c0(&l, nullptr);
    CHECK(gf::fun_140244010(&l, "A.GEO", o, 2));
    const std::vector<std::string> want = {"ctor", "read A.GEO 0 3 0", "store 2", "ctor", "read A.GEO 0 4 obj", "dtor", "dtor"};
    CHECK(g_seq == want);
    g_seq.clear();
    CHECK(!gf::fun_140244010(&l, "A.GEO", nullptr, 2));  // obj == 0 after the first read
    const std::vector<std::string> want0 = {"ctor", "read A.GEO 0 3 0", "store 2", "ctor", "dtor", "dtor"};
    CHECK(g_seq == want0);
    for (int t : {0, 1, 3, 4, 5, 10, -1}) CHECK(gf::fun_140244010(&l, "A.X", o, t));
    gf::ext::g_geo_hooks = {};
}

TEST_CASE(front_end_2d_iges_dxf)
{
    static std::int32_t rc = 0;
    g_seq.clear();
    gf::ext::g_iges_read_hook = [](gf::Uic2CObjectIgesFile* self, const char* file, const char* prefix, LLOBJ_DATA*,
                                   double* x, std::int32_t a, char b, std::int32_t c, std::int32_t mode) {
        g_seq.push_back(std::string("iges ") + (file ? file : "(0)") + " [" + (prefix ? prefix : "(0)") + "] " +
                        std::to_string(*x) + " " + std::to_string(a) + " " + std::to_string(b) + " " + std::to_string(c) +
                        " " + std::to_string(mode));
        gf::fun_14040ce60(self, 5);
        return rc;
    };
    gf::ext::g_dxf_read_hook = [](gf::Uic2CObjectDxfFile* self, const char* file, const char* prefix, LLOBJ_DATA*,
                                  std::int32_t a, std::int32_t mode) {
        g_seq.push_back(std::string("dxf ") + (file ? file : "(0)") + " [" + (prefix ? prefix : "(0)") + "] " +
                        std::to_string(a) + " " + std::to_string(mode));
        at<std::int32_t>(self, 0x1c) = 6;
        return rc;
    };
    DB_STEP_DATA* m = model(2);
    LLOBJ_DATA* o = def::model::llobj_ctor::get_or_create(m, 2);
    gf::Loader l{};
    gf::fun_1402437c0(&l, nullptr);
    at<std::int32_t>(o, 0xbc8) = 0;
    rc = 0;
    CHECK(gf::fun_140244010(&l, "A.IGS", o, 6));
    CHECK_EQ(at<std::int32_t>(o, 0xbc8), 5);
    CHECK(gf::fun_140244010(&l, "A.DXF", o, 7));
    CHECK_EQ(at<std::int32_t>(o, 0xbc8), 6);
    const std::vector<std::string> want = {"iges A.IGS [] 0.000000 2 0 1 300", "dxf A.DXF [] 2 300"};
    CHECK(g_seq == want);
    // other negative codes count as success, the count is not copied; -1 fails
    rc = -3;
    at<std::int32_t>(o, 0xbc8) = 0;
    CHECK(gf::fun_140244010(&l, "A.IGS", o, 6));
    CHECK_EQ(at<std::int32_t>(o, 0xbc8), 0);
    rc = -1;
    CHECK(!gf::fun_140244010(&l, "A.DXF", o, 7));
    CHECK(!gf::fun_140244200(&l, nullptr, "A.DXF", 7, 300));
    // mode 400: the reader from obj+0x3a40 with file 0 and prefix 0; other modes: 300 then 400
    g_seq.clear();
    rc = 0;
    gf::fun_140256670(o, 7);
    CHECK(gf::fun_140244200(&l, o, "B.IGS", 6, 400));
    CHECK(gf::fun_140244200(&l, o, "B.IGS", 6, 0));
    const std::vector<std::string> want4 = {"dxf (0) [(0)] 2 400", "iges B.IGS [] 0.000000 2 0 1 300",
                                            "dxf (0) [(0)] 2 400"};
    CHECK(g_seq == want4);
    gf::fun_140256670(o, 3);
    CHECK_EQ(gf::fun_1402442f0(&l, o), -1);
    CHECK_EQ(gf::fun_1402442f0(&l, nullptr), -1);
    CHECK_EQ(gf::fun_140244500(&l, o, "C.STL", 3), -1);
    gf::ext::g_iges_read_hook = nullptr;
    gf::ext::g_dxf_read_hook = nullptr;
}

TEST_CASE(load_by_extension_sets_the_type)
{
    static std::vector<int> types;
    gf::ext::g_load_3d_hook = [](gf::Loader*, const char*, LLOBJ_DATA*, std::int32_t t) {
        types.push_back(t);
        return t != 5;
    };
    DB_STEP_DATA* m = model(3);
    LLOBJ_DATA* o = def::model::llobj_ctor::get_or_create(m, 2);
    at<std::int32_t>(o, 0x39c0) = 3;  // a 3D object (140158ca0)
    gf::Loader l{};
    gf::fun_1402437c0(&l, nullptr);
    std::int32_t type = 99;
    gf::fun_140256670(o, 0);
    CHECK(gf::fun_140243830(&l, "x.nas", o, &type));
    CHECK_EQ(type, 10);
    CHECK_EQ(gf::fun_140256690(o), 10);
    CHECK(!gf::fun_140243830(&l, "x.unv", o, &type));  // reader failed: +0x3a40 kept
    CHECK_EQ(type, 5);
    CHECK_EQ(gf::fun_140256690(o), 10);
    type = 99;
    CHECK(!gf::fun_140243830(&l, "x.Stl", o, &type));  // -1: no reader call
    CHECK_EQ(type, -1);
    type = 99;
    CHECK(!gf::fun_140243830(&l, "nodot", o, &type));  // no extension: type untouched
    CHECK_EQ(type, 99);
    CHECK(types == std::vector<int>({10, 5}));
    gf::ext::g_load_3d_hook = nullptr;
}

TEST_CASE(morp_writes_the_ini_and_launches)
{
    TmpCwd cwd("morp");
    p801test::g_calls = {};
    p801test::install_hooks();
    DB_STEP_DATA* m2 = model(2);
    {
        keytest::InFile f("morp_in.key", " 1,3\n0.5\nA1\n  A2  \nA3\nA4\nA5\nA6\nNEXT\n");
        CHECK_EQ(def::cmd::cmd_morp(m2, f.get()), 0);
        CHECK_EQ(f.rest(), std::string("NEXT\n"));
    }
    CHECK_EQ(cmdoracle::slurp("DEF_MORP2.INI"), std::string("3\r\n1\r\n0.500000\r\nA1\r\n  A2\r\nA3\r\nA4\r\nA5\r\nA6\r\n"));
    DB_STEP_DATA* m3 = model(3);
    {
        keytest::InFile f("morp_in3.key", "0\nS1\nS2\n");  // n = 0, m = 0: two string lines
        CHECK_EQ(def::cmd::cmd_morp(m3, f.get()), 0);
    }
    CHECK_EQ(cmdoracle::slurp("DEF_MORP3.INI"), std::string("0\r\n0\r\nS1\r\nS2\r\n"));
    const std::vector<std::tuple<int, std::string, int>> want = {{1, "DEF_MORP2.EXE", 2},
                                                                 {1, "DEF_MORP3.EXE morphing DEF_MORP3.INI", 3}};
    CHECK(p801test::g_calls.launches == want);
    {
        keytest::InFile f("morp_eof.key", "1 1\n");  // EOF on the double line
        CHECK_EQ(def::cmd::cmd_morp(m2, f.get()), 2);
    }
    {
        keytest::InFile f("morp_eof2.key", "1 1\n2\nonly\n");  // EOF in the strings
        CHECK_EQ(def::cmd::cmd_morp(m2, f.get()), 2);
    }
    CHECK_EQ(p801test::g_calls.launches.size(), std::size_t(2));
}

TEST_CASE(armctl_file)
{
    TmpCwd cwd("armctl");
    DB_STEP_DATA* m = model(2);
    {
        keytest::InFile f("a.key", " 12abc,-7 0x10 99\n");
        CHECK_EQ(def::cmd::cmd_armctl(m, f.get()), 0);
    }
    CHECK_EQ(cmdoracle::slurp("DEF_ARM_CTL.DAT"), std::string("12\r\n-7\r\n0\r\n"));  // atol: decimal only
    {
        keytest::InFile f("b.key", "");
        CHECK_EQ(def::cmd::cmd_armctl(m, f.get()), 2);
    }
}

TEST_CASE(execmd_modes)
{
    p801test::g_calls = {};
    p801test::install_hooks();
    p801test::g_calls.run_echo = false;
    DB_STEP_DATA* m = model(2);
    const char* lines[] = {"1\n  echo x \n", "2\nA b,c\n", "3\nB\n", "4\nC\n", "0\nD\n", "x\nE\n", "7\nF"};
    for (const char* t : lines) {
        keytest::InFile f("e.key", t);
        CHECK_EQ(def::cmd::cmd_execmd(m, f.get()), 0);
    }
    CHECK(p801test::g_calls.systems == std::vector<std::string>({"  echo x \n"}));
    const std::vector<std::tuple<int, std::string, int>> want = {
        {1, "A b,c\n", 2}, {1, "B\n", 3}, {1, "C\n", 4}, {1, "D\n", 0}, {1, "E\n", 0}, {1, "F", 0}};
    CHECK(p801test::g_calls.launches == want);
    keytest::InFile f("e.key", "1\n");
    CHECK_EQ(def::cmd::cmd_execmd(m, f.get()), 2);
}

TEST_CASE(gfread_grammar_and_messages)
{
    TmpCwd cwd("gfread");
    DB_STEP_DATA* m = model(2);
    {
        keytest::InFile f("g.key", "0\nX.STL\n");  // id < 1: 2 before the name line
        CHECK_EQ(def::cmd::cmd_gfread(m, f.get()), 2);
        CHECK_EQ(f.rest(), std::string("X.STL\n"));
    }
    {
        keytest::InFile f("g.key", "\n");
        CHECK_EQ(def::cmd::cmd_gfread(m, f.get()), 2);
    }
    {
        keytest::InFile f("g.key", "3\n");  // name line at EOF; object 3 already created
        CHECK_EQ(def::cmd::cmd_gfread(m, f.get()), 2);
        CHECK(def::key::g2obj::find_object(*m, 3) != nullptr);
    }
}

int main() { return testkit::run_all(); }
