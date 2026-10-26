// ctest-labels: p8-07 oracle
// DEFAMG oracle replay (tests/commands/DEFAMG/*.json, cached DEF_PRE runs): KFREAD seed, DEFAMG,
// KFWRIT through the shared runner (fresh working directory per case, command keywords through
// def::cmd::dispatch, which reaches cmd_defamg). The port runs the real external programs
// (DEF_AMG / DEF_AMG3_GUI ...) from DEFORM_PC_PATH (default: the DEFORM install); every group 2
// record of OUT.KEY and the INFO lines of stdout are compared with the oracle.
// Cases whose path reaches a callee still ext-stubbed here are skipped with the owner.
// Without the oracle cache the test passes with a note. P807_CASE=<label> runs one case.
#define _CRT_SECURE_NO_WARNINGS
#include "cmd_oracle_runner.hpp"

#include <cstdio>
#include <cstdlib>
#include <string>

#include <defcmd/meshgen/mg_globals.hpp>
#include <testkit/check.hpp>

namespace {

struct Case {
    const char* label;
    const char* needs;  // nullptr: runs here; else the reason it is skipped
};

const Case kCases[] = {
    {"DEFAMG without object number", nullptr},
    {"DEFAMG unknown object", nullptr},
    {"DEFAMG obj1 2D with DIEGEO", nullptr},
    {"DEFAMG object number in hex (%i)", nullptr},
    {"DEFAMG obj1 2D with MGUSER windows (USRWIN.DAT)", nullptr},
    {"DEFAMG obj1 2D mesh only (no DIEGEO)", nullptr},
    {"DEFAMG obj1 3D", nullptr},
    {"DEFAMG obj1 3D with 64bit.DAT", nullptr},
    {"DEFAMG obj1 3D with MGUSER window (BLOCK3.DAT)", nullptr},
    {"DEFAMG obj1 3D with CHK_VOL.INI > 0 present (deleted by the start cleanup, T8-5)", nullptr},
};

} // namespace

TEST_CASE(defamg_oracle)
{
    cmdoracle::Runner r;
    if (!r.ok()) {
        std::printf("p807: no oracle cache, skipped\n");
        return;
    }
    if (std::getenv("DEFORM_PC_PATH") == nullptr) _putenv_s("DEFORM_PC_PATH", "C:\\Program Files\\SFTC\\DEFORM\\v11.0.2\\");
    cmdoracle::Options o;
    o.message = [](const std::string& l) {
        return l.rfind("INFO   : Generating", 0) == 0 || l.rfind("INFO   : New target", 0) == 0 ||
               l.rfind("         Success", 0) == 0 || l.rfind("ERROR: Keyword", 0) == 0;
    };
    o.reset = [] { def::cmd::reset_mg_globals(); };
    o.time_search = false;
    const char* only = std::getenv("P807_CASE");
    int run = 0, pass = 0, skipped = 0;
    for (const Case& c : kCases) {
        if (only != nullptr && std::string(only) != c.label) continue;
        if (c.needs != nullptr) {
            std::printf("p807: SKIP %s: %s\n", c.label, c.needs);
            ++skipped;
            continue;
        }
        cmdoracle::Result res = r.run(c.label, o);
        if (!res.found) {
            std::printf("p807: no oracle entry for %s\n", c.label);
            continue;
        }
        ++run;
        if (res.pass) ++pass;
        std::printf("p807: %s %s (equal %d, differ %d, baseline gaps %d)\n%s", res.pass ? "PASS" : "FAIL", c.label,
                    res.equal, res.differ, res.gaps, res.report.c_str());
        CHECK(res.pass);
    }
    std::printf("p807: %d of %d run cases pass, %d skipped\n", pass, run, skipped);
}

int main() { return testkit::run_all(); }
