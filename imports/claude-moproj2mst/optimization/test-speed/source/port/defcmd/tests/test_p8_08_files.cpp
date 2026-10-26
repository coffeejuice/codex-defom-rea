// Packet p8-08, tier 1: the exchange files DEFINT / DEFFRZ write for DEF_INT and DEF_FRZ, the
// command lines they launch, the clean-up and the string helpers. The model is a 2D object read
// from a small KEY text; system() is replaced by the launcher's test hook, which snapshots the
// exchange files at launch time (nothing is started). Expected bytes come from the format strings
// of the binary (DEFINT.md section 4, DEFFRZ.md section 4) and the CRT behaviour cited in
// interp.cpp (the "%10d %\n" line of the .RMS file prints "%10d " then "\n").
// ctest-labels: p8-08 tier1
#define _CRT_SECURE_NO_WARNINGS
#include <defcmd/cmd_p8_08.hpp>
#include <defcmd/interp.hpp>
#include <defcmd/launch.hpp>
#include <defcmd/meshgen/meshgen_base.hpp>
#include <defcmd/meshgen/p8_seams.hpp>
#include <testkit/check.hpp>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <direct.h>
#include <filesystem>
#include <fstream>
#include <map>
#include <process.h>
#include <sstream>
#include <string>

#include <defkey/kw_g2obj.hpp>
#include <defkey/group1.hpp>
#include <defkey/key_reader.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/llobj/object_list.hpp>
#include <defrt/stdlib/str_ends_with.hpp>

namespace fs = std::filesystem;
using def::model::DB_STEP_DATA;
using def::model::LLOBJ_DATA;

namespace {

std::string slurp(const fs::path& p)
{
    std::ifstream in(p, std::ios::binary);
    std::ostringstream s;
    s << in.rdbuf();
    return s.str();
}

struct Hooks : def::key::KeyReaderHooks {
    DB_STEP_DATA* m;
    explicit Hooks(DB_STEP_DATA* model) : m(model) {}
    bool has_sub_model() override { return false; }
    void set_model_dimension(int dim) override
    {
        std::int32_t d = dim;
        m->set_dimension(&d);
    }
    void set_sub_model_dimension(int) override {}
    int parse_keyword(int group, std::FILE* f, int id) override
    {
        if (group == 1) return def::key::read_group1(m, f, id);
        if (group == 2) return def::key::g2obj::read_object_keyword(*m, f, id);
        char buf[0x400];
        (void)std::fgets(buf, sizeof buf, f);
        return 0;
    }
};

// a unit square object 1 (4 nodes, 1 quad) in a 2D model
DB_STEP_DATA* make_model_2d()
{
    DB_STEP_DATA* m = new DB_STEP_DATA(2);
    const char* key =
        "OBJTYP       1       2       0\n"
        "RZ           1       4\n"
        "       1  0.0000000E+000  0.0000000E+000\n"
        "       2  1.0000000E+000  0.0000000E+000\n"
        "       3  1.0000000E+000  1.2500000E+000\n"
        "       4  0.0000000E+000  1.2500000E+000\n"
        "ELMCON       1       1\n"
        "       1       1       2       3       4\n";
    const std::string path = "p808_model.key";
    {
        std::ofstream o(path, std::ios::binary);
        o << key;
    }
    std::FILE* f = std::fopen(path.c_str(), "r");
    std::FILE* sink = std::fopen("NUL", "w");
    Hooks h(m);
    def::key::read_key_stream(f, h, sink);
    std::fclose(f);
    std::fclose(sink);
    std::remove(path.c_str());
    return m;
}

// launch hook: remember the command and the exchange files present at that moment
std::string g_cmd;
std::map<std::string, std::string> g_files;
int capture(const char* c)
{
    g_cmd = c;
    g_files.clear();
    for (auto& e : fs::directory_iterator(fs::current_path())) {
        if (e.is_regular_file()) g_files[e.path().filename().string()] = slurp(e.path());
    }
    return 0;
}

struct TempDir {
    fs::path old, dir;
    TempDir()
    {
        old = fs::current_path();
        dir = fs::temp_directory_path() / ("p808_files_" + std::to_string(_getpid()));
        fs::create_directories(dir);
        fs::current_path(dir);
        _putenv_s("DEFORM_PC_PATH", dir.string().c_str());
        def::cmd::g_launch_system_hook = capture;
    }
    ~TempDir()
    {
        def::cmd::g_launch_system_hook = nullptr;
        fs::current_path(old);
        std::error_code ec;
        fs::remove_all(dir, ec);
    }
    static std::size_t file_count()
    {
        std::size_t n = 0;
        for (auto& e : fs::directory_iterator(fs::current_path())) n += e.is_regular_file() ? 1 : 0;
        return n;
    }
};

const char* kRms1 =
    "         4          1\r\n"
    "         1       0.0000000       0.0000000\r\n"
    "         2       1.0000000       0.0000000\r\n"
    "         3       1.0000000       1.2500000\r\n"
    "         4       0.0000000       1.2500000\r\n"
    "         1 \r\n"  // "%10d %\n": '%' + '\n' prints '\n'
    "         1          1          2          3          4 \r\n";

const char* kDmesh2 =
    "         4\r\n"
    "         1       0.0000000       0.0000000\r\n"
    "         2       1.0000000       0.0000000\r\n"
    "         3       1.0000000       1.2500000\r\n"
    "         4       0.0000000       1.2500000\r\n"
    "         1     4\r\n"
    "         1          1          2          3          4 \r\n";

} // namespace

TEST_CASE(string_helpers)
{
    CHECK(def::rt::str_ends_with("HAMMER_SI.DB", ".DB"));
    CHECK(def::rt::str_ends_with("hammer.db", ".DB"));
    CHECK(!def::rt::str_ends_with(".DB", ".DB"));  // strictly longer
    CHECK(!def::rt::str_ends_with("A.KEY", ".DB"));
    CHECK_EQ(def::cmd::fun_1401b17b0(), std::string("\\"));
    CHECK_EQ(def::cmd::fun_1401b1520("HAMMER_SI.DB", 9), std::string("HAMMER_SI"));
    CHECK_EQ(def::cmd::fun_1401b1520("ABC", 0), std::string("ABC"));
    CHECK_EQ(def::cmd::fun_1401b1520("ABC", 3), std::string("ABC"));
}

TEST_CASE(rms_file_bytes_mode_1_and_2)
{
    TempDir t;
    DB_STEP_DATA* m = make_model_2d();
    LLOBJ_DATA* o = def::model::object_list_find(&m->objects, 1);
    CHECK(o != nullptr);
    {
        def::cmd::MeshGenBase mg;
        def::cmd::meshgen_bind(mg, o, m, "PROB");
        mg.u3f40 = 1;
        CHECK(def::cmd::write_mesh_2d(mg, "A.RMS"));
        CHECK_EQ(slurp("A.RMS"), std::string(kRms1));
        mg.u3f40 = 2;
        CHECK(def::cmd::write_mesh_2d(mg, "DMESH.DAT"));
        CHECK_EQ(slurp("DMESH.DAT"), std::string(kDmesh2));
    }
}

TEST_CASE(defint_2d_exchange_files_and_cleanup)
{
    TempDir t;
    DB_STEP_DATA* m = make_model_2d();
    std::strcpy(m->u340, "PROB");
    std::strcpy(m->u1748, "C:\\x/y\\HAMMER_SI.DB");
    m->u744 = 17;
    std::fflush(stdout);
    const std::uint32_t id = 1;
    CHECK(def::cmd::defint_object(*m, &id));  // DEF_INT "runs" (hook), no _REM.KEY -> Failure
    CHECK_EQ(g_cmd, "\"\"" + t.dir.string() + "\\DEF_INT\" < DEF_INT.ACT > DEF_INT.OUT\"");
    CHECK_EQ(g_files["DEF_INT.ACT"], std::string("\r\n5\r\nY\r\n1\r\nPROB.RMS\r\n\r\n6\r\n\r\n"));
    CHECK_EQ(g_files["DEF_REM.INI"], std::string("C:\\x/y\\HAMMER_SI.DB\r\n17\r\n2\r\n1\r\n"));
    CHECK_EQ(g_files["PROB.RMS"], std::string(kRms1));
    CHECK_EQ(g_files["DEFORM_UIRUNREMESH.DAT"], std::string("2D"));
    CHECK_EQ(g_files.size(), static_cast<std::size_t>(4));
    CHECK_EQ(TempDir::file_count(), static_cast<std::size_t>(0));  // nothing left behind
}

TEST_CASE(deffrz_2d_exchange_files)
{
    TempDir t;
    DB_STEP_DATA* m = make_model_2d();
    std::strcpy(m->u340, "PROB");
    std::strcpy(m->u1748, "HAMMER_SI.DB");
    m->u744 = 5;
    const std::uint32_t a = 1, b = 1;
    const double v = 0.5;
    CHECK(def::cmd::deffrz_object(*m, &a, &b, &v));
    CHECK_EQ(g_cmd, "\"\"" + t.dir.string() + "\\DEF_FRZ\" > DEF_FRZ2.OUT\"");
    CHECK_EQ(g_files["FRZ1.INI"], std::string("HAMMER_SI.DB\r\n5\r\n1\r\n         1          1  \r\n0.500000\r\n"));
    CHECK_EQ(g_files["DMESH.DAT"], std::string(kDmesh2));
    CHECK_EQ(g_files.size(), static_cast<std::size_t>(2));
    CHECK_EQ(TempDir::file_count(), static_cast<std::size_t>(0));
}

TEST_CASE(scratch_key_3d_mesh_part)
{
    TempDir t;
    DB_STEP_DATA* m = new DB_STEP_DATA(3);
    const char* key =
        "RZ           1       4\n"
        "       1  0.0000000E+000  0.0000000E+000  0.0000000E+000\n"
        "       2  1.0000000E+000  0.0000000E+000  0.0000000E+000\n"
        "       3  0.0000000E+000  1.0000000E+000  0.0000000E+000\n"
        "       4  0.0000000E+000  0.0000000E+000  2.5000000E-001\n"
        "ELMCON       1       1       4\n"
        "       1       1       2       3       4\n";
    {
        std::ofstream o("p808_model3.key", std::ios::binary);
        o << key;
    }
    std::FILE* f = std::fopen("p808_model3.key", "r");
    std::FILE* sink = std::fopen("NUL", "w");
    Hooks h(m);
    def::key::read_key_stream(f, h, sink);
    std::fclose(f);
    std::fclose(sink);
    LLOBJ_DATA* o = def::model::object_list_find(&m->objects, 1);
    CHECK(o != nullptr);
    def::cmd::MeshGenBase mg;
    def::cmd::meshgen_bind(mg, o, m, "PROB");
    LLOBJ_DATA* objs[1] = {o};
    mg.u4780 = objs;
    mg.u3f40 = 1;
    CHECK(def::cmd::write_mesh_3d(mg, "SCRATCH.KEY"));
    const std::string got = slurp("SCRATCH.KEY");
    const std::string want =
        "RZ          1          4\r\n"
        "         1       0.0000000       0.0000000       0.0000000\r\n"
        "         2       1.0000000       0.0000000       0.0000000\r\n"
        "         3       0.0000000       1.0000000       0.0000000\r\n"
        "         4       0.0000000       0.0000000       0.2500000\r\n"
        "ELMCON          1          1     4\r\n"
        "         1          1          2          3          4 \r\n"
        "MTLGRP ";  // then the MTLGRP block of 1402f4df0 (kw-g2sv's tests)
    CHECK_EQ(got.substr(0, want.size()), want);
}

TEST_CASE(cleanup_removes_exchange_files)
{
    TempDir t;
    const char* names[] = {"DEF_AMG.INI", "NBCD.DAT", "DEF_AMG3.INI", "crossed.faces.0", "DEF_INT.OUT", "KEEP.TXT"};
    for (const char* n : names) {
        std::ofstream o(n);
        o << "x";
    }
    {
        def::cmd::MeshGenBase mg;
        def::cmd::exchange_cleanup(mg, false);  // NBCD.DAT kept
    }
    CHECK(fs::exists("NBCD.DAT"));
    CHECK(fs::exists("KEEP.TXT"));
    CHECK_EQ(TempDir::file_count(), static_cast<std::size_t>(2));
    {
        def::cmd::MeshGenBase mg;
        def::cmd::exchange_cleanup(mg, true);
    }
    CHECK(!fs::exists("NBCD.DAT"));
    CHECK_EQ(TempDir::file_count(), static_cast<std::size_t>(1));
}

int main() { return testkit::run_all(); }
