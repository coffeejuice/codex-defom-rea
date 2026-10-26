// ctest-labels: p67-int oracle
// Integration replay (branch claude/p67-3-int): OBJPOS drop and GENCTC/INICTC cases through the
// port with cmd_oracle_runner.hpp, compared with the cached oracle runs. Labels come from
// INT_ORACLE_LABELS (comma separated, e.g. "OBJPOS/7-drop-3d,GENCTC/brick_0"); without it the test
// does nothing (a stub that is reached aborts the process, so tools/int/run_int.py runs one label
// per process and collects the results).
#include "cmd_oracle_runner.hpp"

#include <defcmd/contact_cmds.hpp>
#include <defcmd/drop/globals_a.hpp>
#include <defcmd/drop/globals_c.hpp>
#include <defcmd/drop/globals_d.hpp>
#include <defcmd/drop/solver.hpp>
#include <defcmd/drop/state.hpp>
#include <defcmd/genctc3d_ray.hpp>
#include <defcmd/objpos.hpp>
#include <defmodel/globals_p1_10.hpp>
#include <testkit/check.hpp>

TEST_CASE(int_cases_match_the_oracle)
{
    const std::string labels = cmdoracle::env("INT_ORACLE_LABELS", "");
    if (labels.empty()) {
        std::printf("INT_ORACLE_LABELS not set: nothing to run\n");
        return;
    }
    keytest::legacy_printf();
    cmdoracle::Runner r;
    if (!r.ok()) {
        std::printf("no oracle cache: skipped\n");
        return;
    }
    cmdoracle::Options o;
    o.command = [](def::model::DB_STEP_DATA& m, std::FILE* f, const std::string& name) {
        if (name == "OBJPOS") return def::cmd::objpos::run(m, f);
        if (name == "INICTC") return def::cmd::cmd_inictc(&m, f);
        if (name == "GENCTC") return def::cmd::cmd_genctc(&m, f);
        return -1;
    };
    o.message = [](const std::string& l) {
        if (l.rfind("ERROR  : Volume compensation", 0) == 0) return false;
        // engine records may follow an unflushed prompt on the same stdout line in the original
        // ("Keyword File = [ ? ] ?  Interference detected, KOBJ=  1"): select them anywhere
        return l.rfind("Object ", 0) == 0 || l.rfind("Failure in drop", 0) == 0 || l.rfind("ERROR", 0) == 0 ||
               l.find(" Interference detected") != std::string::npos || l.find(" Info:") != std::string::npos ||
               l.rfind("Warning", 0) == 0 || l.rfind(" Warning", 0) == 0;
    };
    // compare such records from where they start; the exact interleaving of prompt and record is
    // checked by the def_pre end-to-end run at the P6-7 gate
    o.normalize = [](const std::string& l) {
        for (const char* k : {" Interference detected", " Info:"}) {
            const std::size_t p = l.find(k);
            if (p != std::string::npos && p > 0) return l.substr(p);
        }
        return l;
    };
    o.reset = [] {
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
    int ok = 0, bad = 0, skipped = 0;
    std::size_t pos = 0;
    while (pos <= labels.size()) {
        std::size_t comma = labels.find(',', pos);
        if (comma == std::string::npos) comma = labels.size();
        const std::string label = labels.substr(pos, comma - pos);
        pos = comma + 1;
        if (label.empty()) continue;
        const cmdoracle::Result res = r.run(label, o);
        if (!res.found) {
            std::printf("RESULT %s SKIP no-oracle-entry\n", label.c_str());
            ++skipped;
            continue;
        }
        std::printf("RESULT %s %s equal=%d differ=%d gaps=%d\n%s", label.c_str(), res.pass ? "PASS" : "FAIL",
                    res.equal, res.differ, res.gaps, res.report.c_str());
        if (res.pass) ++ok;
        else ++bad;
    }
    CHECK_EQ(bad, 0);
}

int main() { return testkit::run_all(); }
