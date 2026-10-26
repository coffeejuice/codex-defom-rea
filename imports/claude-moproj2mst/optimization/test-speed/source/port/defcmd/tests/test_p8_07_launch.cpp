// Launcher 140223790: program search and the system() command line (system() replaced by the test
// hook, so nothing is ever started). Expected strings from the algorithm in common.md.
// ctest-labels: p8-07 tier1
#include <defcmd/launch.hpp>
#include <testkit/check.hpp>

#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <string>

#include <process.h>

namespace fs = std::filesystem;

static std::string g_cmd;
static int capture(const char* c)
{
    g_cmd = c;
    return 0;
}

static fs::path make_install()
{
    fs::path d = fs::temp_directory_path() / ("p807_launch_" + std::to_string(_getpid()));
    fs::create_directories(d / "2D");
    fs::create_directories(d / "3D");
    std::FILE* f = std::fopen((d / "2D" / "DEF_AMG.EXE").string().c_str(), "wb");
    std::fclose(f);
    f = std::fopen((d / "3D" / "BRKMSH").string().c_str(), "wb");
    std::fclose(f);
    return d;
}

static void set_base(const std::string& b) { _putenv_s("DEFORM_PC_PATH", b.c_str()); }

TEST_CASE(split_words_quotes_and_commas)
{
    char line[] = "DEF_AMG < A.ACT,B.OUT 'x y' \"q\"";
    char* argv[24];
    const int n = def::cmd::split_words(line, " ,\t\n", argv, 21);
    CHECK_EQ(n, 6);
    CHECK_EQ(std::string(argv[0]), std::string("DEF_AMG"));
    CHECK_EQ(std::string(argv[1]), std::string("<"));
    CHECK_EQ(std::string(argv[3]), std::string("B.OUT"));
    CHECK_EQ(std::string(argv[4]), std::string("'x y'"));
    CHECK_EQ(std::string(argv[5]), std::string("\"q\""));
}

TEST_CASE(found_with_exe_suffix_uses_unsuffixed_name)
{
    const fs::path d = make_install();
    set_base(d.string());  // no trailing '\': appended
    def::cmd::g_launch_system_hook = capture;
    def::cmd::launch_program_2d(1, "DEF_AMG < DEF_MGN.ACT > DEF_MGN.OUT  \n");
    const std::string b = d.string() + "\\";
    CHECK_EQ(g_cmd, "\"\"" + b + "2D\\DEF_AMG\" < DEF_MGN.ACT > DEF_MGN.OUT\"");
    def::cmd::launch_program_3d(1, "BRKMSH > BRKMSH.MSG");
    CHECK_EQ(g_cmd, "\"\"" + b + "3D\\BRKMSH\" > BRKMSH.MSG\"");
    // not found in 3D\: base + name, no extension
    def::cmd::launch_program_3d(1, "MESH3D_GUI");
    CHECK_EQ(g_cmd, "\"\"" + b + "MESH3D_GUI\"\"");
    def::cmd::g_launch_system_hook = nullptr;
    std::error_code ec;
    fs::remove_all(d, ec);
}

TEST_CASE(base_in_2d_folder_goes_up)
{
    const fs::path d = make_install();
    set_base((d / "3D").string() + "\\");
    def::cmd::g_launch_system_hook = capture;
    def::cmd::launch_program_2d(1, "DEF_AMG");
    CHECK_EQ(g_cmd, "\"\"" + (d / "3D").string() + "\\..\\2D\\DEF_AMG\"\"");
    // absolute program names are left alone
    def::cmd::launch_program_2d(1, "C:\\X\\Y.EXE a");
    CHECK_EQ(g_cmd, std::string("\"\"C:\\X\\Y.EXE\" a\""));
    def::cmd::g_launch_system_hook = nullptr;
    std::error_code ec;
    fs::remove_all(d, ec);
}

int main() { return testkit::run_all(); }
