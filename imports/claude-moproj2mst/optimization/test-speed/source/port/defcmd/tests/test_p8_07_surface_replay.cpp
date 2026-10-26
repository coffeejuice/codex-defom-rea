// ctest-labels: p8-07 oracle
// 3D surface step of DEFAMG (14013ee00 / 14013f410 / 140143920 / 14013f8f0 / 14013f010) against
// capture runs of the original (deform-rea-work/out/p8mg/p8-06/capcache, made by p8-06 with the
// extprobe wrappers: the working directory before and after every external program). The seed is
// read with read_keyword_file (1401b5f00), DEFAMG runs with the launcher's system() replaced by
// a replay: at each launch the exchange files the port wrote are compared byte for byte with the
// original's "before" copy, then the program's "after" files are copied in. The run stops at the
// BRKMSH launch (the NBCD.OUT read-back after it needs p67-5 140202a20). The surface mesh read
// back from MESH3D.INP is checked through the MESH3D.INP the volume step writes before MESH3D_GUI.
// Skipped without the capture cache (P807_CAPCACHE overrides its path).
#define _CRT_SECURE_NO_WARNINGS
#include "cmd_oracle_runner.hpp"

#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <set>
#include <stdexcept>
#include <string>
#include <vector>

#include <process.h>

#include <defcmd/key_file.hpp>
#include <defcmd/launch.hpp>
#include <defcmd/meshgen/mg_globals.hpp>
#include <defcmd/meshgen/p8_seams.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <testkit/check.hpp>

namespace fs = std::filesystem;

namespace {

struct Stop {};

fs::path g_capture;          // <entry>/files/_ext_capture
int g_launch = 0;            // launches seen
std::vector<std::string> g_problems;

const char* program_of(const std::string& cmd)
{
    if (cmd.find("DEF_AMG3_GUI") != std::string::npos) return "1_3D_DEF_AMG3_GUI";
    if (cmd.find("MESH3D_GUI") != std::string::npos) return "1_3D_MESH3D_GUI";
    if (cmd.find("BRKMSH") != std::string::npos) return "1_3D_BRKMSH";
    return nullptr;
}

int replay_system(const char* command)
{
    const std::string cmd(command);
    const char* prog = program_of(cmd);
    ++g_launch;
    if (prog == nullptr) {
        g_problems.push_back("unexpected launch: " + cmd);
        throw Stop{};
    }
    const fs::path step = g_capture / prog;
    // files the original had before the launch, minus the case inputs and what the original's
    // start-up or the shell redirection made (NEWEQ.MSG: DEF_PRE start-up 14001ce7b)
    static const std::set<std::string> ignore = {"CMD.KEY", "IN.KEY", "NEWEQ.MSG", "DEF_MGN.OUT", "BRKMSH.MSG"};
    std::set<std::string> want;
    for (auto& e : fs::directory_iterator(step / "before")) want.insert(e.path().filename().string());
    for (const std::string& n : want) {
        if (ignore.count(n) != 0) continue;
        if (!fs::exists(n)) {
            g_problems.push_back(std::string(prog) + ": " + n + " missing");
            continue;
        }
        if (cmdoracle::slurp(n) != cmdoracle::slurp(step / "before" / n))
            g_problems.push_back(std::string(prog) + ": " + n + " differs");
    }
    for (auto& e : fs::directory_iterator(".")) {
        const std::string n = e.path().filename().string();
        if (want.count(n) == 0 && ignore.count(n) == 0 && n != "CHK_VOL.INI")
            g_problems.push_back(std::string(prog) + ": extra file " + n);
    }
    std::printf("p807 replay: %s checked\n", prog);
    if (std::string(prog) == "1_3D_BRKMSH") throw Stop{};
    // the program's effect: the directory becomes its "after" state (it may delete its inputs)
    std::set<std::string> after;
    for (auto& e : fs::directory_iterator(step / "after")) after.insert(e.path().filename().string());
    std::vector<fs::path> gone;
    for (auto& e : fs::directory_iterator(".")) {
        const std::string n = e.path().filename().string();
        if (after.count(n) == 0 && ignore.count(n) == 0 && n != "CHK_VOL.INI") gone.push_back(e.path());
    }
    for (auto& p : gone) fs::remove(p);
    for (auto& e : fs::directory_iterator(step / "after")) {
        const std::string n = e.path().filename().string();
        if (n == "CMD.KEY" || n == "IN.KEY") continue;
        fs::copy_file(e.path(), n, fs::copy_options::overwrite_existing);
    }
    return 0;
}

} // namespace

TEST_CASE(surface_step_replay)
{
    const fs::path cache = cmdoracle::env("P807_CAPCACHE", "C:/Users/alext/Projects/deform-rea-work/out/p8mg/p8-06/capcache");
    if (!fs::exists(cache / "entries")) {
        std::printf("p807: no capture cache, skipped\n");
        return;
    }
    if (std::getenv("DEFORM_PC_PATH") == nullptr) _putenv_s("DEFORM_PC_PATH", "C:\\Program Files\\SFTC\\DEFORM\\v11.0.2\\");
    const fs::path home = fs::current_path();
    int ran = 0;
    for (auto& d1 : fs::directory_iterator(cache / "entries")) {
        for (auto& d2 : fs::directory_iterator(d1.path())) {
            const std::string j = cmdoracle::slurp(d2.path() / "result.json");
            const std::string label = cmdoracle::json_string(j, "label");
            // mesh-only seeds need p67-1 1401fc730 before the surface step
            if (label.rfind("DEFAMG obj1 3D", 0) != 0 || label.find("64bit.DAT") != std::string::npos) continue;  // capture runs only
            g_capture = d2.path() / "files" / "_ext_capture";
            const std::size_t at = j.find("\"name\": \"IN.KEY\"");
            const std::string seed = cmdoracle::json_string(j, "source", at);
            const fs::path work = fs::temp_directory_path() / ("p807_surf_" + std::to_string(_getpid()) + "_" + std::to_string(ran));
            fs::create_directories(work);
            fs::copy_file(seed, work / "IN.KEY", fs::copy_options::overwrite_existing);
            fs::current_path(work);
            def::cmd::reset_mg_globals();
            g_launch = 0;
            g_problems.clear();
            auto* m = new def::model::DB_STEP_DATA(3);  // leaked, as in the shared runner
            const char reset = 0;
            const std::int32_t flags = 0;
            std::FILE* sink = std::fopen("NUL", "w");
            const int rc = def::cmd::read_keyword_file(m, "IN.KEY", &reset, &flags);
            if (sink != nullptr) std::fclose(sink);
            CHECK_EQ(rc, 0);
            def::cmd::g_launch_system_hook = &replay_system;
            const std::uint32_t id = 1;
            try {
                def::cmd::defamg_object(*m, &id);
                g_problems.push_back("run did not reach BRKMSH");
            } catch (const Stop&) {
            }
            def::cmd::g_launch_system_hook = nullptr;
            fs::current_path(home);
            std::error_code ec;
            fs::remove_all(work, ec);
            ++ran;
            std::printf("p807: %s %s (%d launches)\n", g_problems.empty() ? "PASS" : "FAIL", label.c_str(), g_launch);
            for (auto& p : g_problems) std::printf("    %s\n", p.c_str());
            CHECK(g_problems.empty());
        }
    }
    std::printf("p807: %d surface-step replays\n", ran);
}

int main() { return testkit::run_all(); }
