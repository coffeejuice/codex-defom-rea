// ctest-labels: p67-3 drop oracle
// OBJPOS drop (3D modes 7, 8, 9) against the original DEF_PRE: the cached oracle runs of
// tests/commands/OBJPOS/cases_drop.json and the mode 7-9 cases of cases_ifd.json are replayed
// through the port with cmd_oracle_runner.hpp (p67-1's OBJPOS frame calls drop_mode). The drop
// messages must equal the original's stdout lines and every group 2 record of every object the
// original's OUT.KEY record, except the seed's baseline round-trip gaps. A case without an oracle
// entry is skipped with a note. DROP_ORACLE_ONLY=<substring> limits the cases.
// Until the 3D contact engine (packet p67-7) has bodies, cases that reach the contact search stop
// in a DEF_NOT_PORTED stub; set DROP_ORACLE_RUN=1 to run them.
#include "cmd_oracle_runner.hpp"

#include <defcmd/drop/globals_a.hpp>
#include <defcmd/drop/globals_c.hpp>
#include <defcmd/drop/globals_d.hpp>
#include <defcmd/drop/solver.hpp>
#include <defcmd/drop/state.hpp>
#include <defcmd/genctc3d_ray.hpp>
#include <defcmd/objpos.hpp>
#include <defmodel/globals_p1_10.hpp>
#include <testkit/check.hpp>

namespace {

const char* kCases[] = {"drop-gap05-3d", "drop-inplace-3d", "drop-nohit-3d", "7-drop-3d", "7-default-tol-3d",
                        "7-tilted-3d", "8-drop-axis-3d", "9-drop-vector-3d", "9-drop-zerovector-3d",
                        "7-drop-coupled-3d", "7-drop-rigid-3d", "7-drop-up-3d", "7-drop-zerodir-3d",
                        "7-undefined-3d", "7-in-2d"};

} // namespace

TEST_CASE(drop_cases_match_the_oracle)
{
    if (cmdoracle::env("DROP_ORACLE_RUN", "0") != "1") {
        std::printf("drop oracle replay waits for the p67-7 engine bodies: skipped (DROP_ORACLE_RUN=1 runs it)\n");
        return;
    }
    keytest::legacy_printf();
    cmdoracle::Runner r;
    if (!r.ok()) {
        std::printf("no oracle cache: skipped\n");
        return;
    }
    cmdoracle::Options o;
    // command keywords go through the dispatcher (cmd_oracle_runner.hpp default)
    o.message = [](const std::string& l) {
        if (l.rfind("ERROR  : Volume compensation", 0) == 0) return false;
        return l.rfind("Object ", 0) == 0 || l.rfind("Failure in drop", 0) == 0 || l.rfind("ERROR  :", 0) == 0 ||
               l.rfind("ERROR: Keyword OBJPOS", 0) == 0;
    };
    o.reset = [] {
        // process-lifetime state of a fresh DEF_PRE
        def::cmd::objpos::g_last_disp[0] = 0.0;
        def::cmd::objpos::g_last_disp[1] = 0.0;
        def::cmd::objpos::g_last_disp[2] = 0.0;
        def::model::reset_globals_p1_10();
        def::cmd::drop::reset_state();
        def::cmd::drop::reset_globals_a();
        def::cmd::drop::reset_globals_c();
        def::cmd::drop::reset_globals_d();
        def::cmd::ctc3d::reset_ray_statics();
        for (double& v : def::cmd::drop::g_nr1_buf) v = 0.0;
        def::cmd::drop::g_nr1_cnt = 0;
        def::cmd::ctc3d::g_no_diegeo = 0;
    };
    const std::string only = cmdoracle::env("DROP_ORACLE_ONLY", "");
    int ok = 0, bad = 0, skipped = 0;
    for (const char* c : kCases) {
        const std::string label = std::string("OBJPOS/") + c;
        if (!only.empty() && label.find(only) == std::string::npos) continue;
        const cmdoracle::Result res = r.run(label, o);
        if (!res.found) {
            std::printf("%-28s no oracle entry: skipped\n", label.c_str());
            ++skipped;
            continue;
        }
        std::printf("%-28s %s  records: %d equal, %d differ, %d baseline gaps\n%s", label.c_str(),
                    res.pass ? "PASS" : "FAIL", res.equal, res.differ, res.gaps, res.report.c_str());
        if (res.pass) ++ok;
        else ++bad;
    }
    std::printf("OBJPOS drop oracle: %d pass, %d fail, %d skipped\n", ok, bad, skipped);
    CHECK_EQ(bad, 0);
}

int main() { return testkit::run_all(); }
