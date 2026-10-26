// ctest-labels: p8-06
// DEFAMG 3D oracle cases (tests/commands/DEFAMG/defamg_3d*.json) through the port with the shared
// cmd_oracle_runner (KEY loop, def::cmd::dispatch for DEFAMG -> defamg_object (p8-07) -> 3D
// surface step (p8-07) and volume step (p8-06)). The external programs are not run: the
// launcher's system() hook replays the capture runs (p806_capture.hpp; every exchange file before
// each launch must match byte for byte). OUT.KEY records and the INFO messages are compared by the
// runner. Skipped while p8-07's 140141580 still reaches the p67-5 ext stub 140202a20, or when a cache is
// missing.
#include "p806_capture.hpp"

#include <defcmd/launch.hpp>
#include <defcmd/meshgen/mg_globals.hpp>
#include <defcmd/p8_06_ext.hpp>
#include <testkit/check.hpp>

namespace {

namespace fs = std::filesystem;
using namespace p806cap;

// p8-07's NBCD.OUT reader 140141580 needs p67-5's 140202a20 (ext stub in p8_07_ext.cpp until the
// integration): skip while the stub is there
bool needs_p67_5()
{
    const std::string s = cmdoracle::slurp(cmdoracle::Runner::repo_root() / "port" / "defcmd" / "src" / "p8_07_ext.cpp");
    return s.find("DEF_NOT_PORTED(0x140202a20)") != std::string::npos;
}

void run(const char* label, const std::vector<std::string>& launches)
{
    if (needs_p67_5()) {
        std::printf("SKIP %s: needs p67-5 140202a20 (min edge length, called by p8-07 140141580)\n", label);
        return;
    }
    cmdoracle::Runner r;
    const fs::path cap = find_entry(label);
    if (!r.ok() || cap.empty()) {
        std::printf("SKIP %s: no oracle or capture entry\n", label);
        return;
    }
    _putenv("DEFORM_PC_PATH=");
    Replay rp;
    rp.capture = cap / "files" / "_ext_capture";
    g_replay = &rp;
    def::cmd::g_launch_system_hook = &system_hook;
    // cleared on every exit, also when r.run throws (rp is a local)
    struct Unhook {
        ~Unhook()
        {
            def::cmd::g_launch_system_hook = nullptr;
            g_replay = nullptr;
        }
    } unhook;
    cmdoracle::Options o;
    o.reset = [&rp] {
        def::cmd::reset_mg_globals();
        rp.count.clear();
        rp.launched.clear();
        rp.problems.clear();
    };
    o.time_search = false;
    o.message = [](const std::string& l) { return l.rfind("INFO   :", 0) == 0 || l.rfind("         Success", 0) == 0; };
    const cmdoracle::Result res = r.run(label, o);
    if (!res.pass || !rp.problems.empty()) std::printf("%s:\n%s%s", label, res.report.c_str(), rp.problems.c_str());
    CHECK(res.found);
    CHECK(rp.launched == launches);
    CHECK(res.pass);
    CHECK(rp.problems.empty());
}

} // namespace

TEST_CASE(defamg_3d)
{
    run("DEFAMG obj1 3D", {"DEF_AMG3_GUI", "MESH3D_GUI", "BRKMSH"});
}

TEST_CASE(defamg_3d_block3)
{
    run("DEFAMG obj1 3D with MGUSER window (BLOCK3.DAT)", {"DEF_AMG3_GUI", "MESH3D_GUI", "BRKMSH"});
}

TEST_CASE(defamg_3d_chkvol_deleted)
{
    run("DEFAMG obj1 3D with CHK_VOL.INI > 0 present (deleted by the start cleanup, T8-5)",
        {"DEF_AMG3_GUI", "MESH3D_GUI", "BRKMSH"});
}

int main() { return testkit::run_all(); }
