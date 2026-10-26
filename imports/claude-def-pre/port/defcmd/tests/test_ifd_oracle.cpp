// ctest-labels: p67-2 objpos oracle
// OBJPOS interference positioning (modes 2, 12, 102, 112; packet p67-2) against the original
// DEF_PRE: every cached oracle run of tests/commands/OBJPOS/cases_ifd.json (drop cases excluded),
// the bucket cases of cases_ik.json and the interference cases of cases_v_ov-ifd.json is replayed
// through the port with cmd_oracle_runner.hpp (OBJPOS through p67-1's frame, which calls this
// packet's interference_mode): the OBJPOS messages must equal the original's stdout lines and
// every group 2 record the original's OUT.KEY, except the seed's baseline round-trip gaps.
// IFD_ORACLE_ONLY=<substring> limits the cases; CMD_ORACLE_VERBOSE / CMD_ORACLE_GAPS as in the
// runner. A case without an oracle entry is skipped with a note.
#include "cmd_oracle_runner.hpp"

#include <defrt/ftn/geoutl.hpp>
#include <defrt/stdlib/libm.hpp>
#include <defcmd/ifd/ifd_internal.hpp>
#include <defcmd/objpos.hpp>
#include <defmodel/globals_p1_10.hpp>
#include <testkit/check.hpp>

#include <cmath>


namespace {

const char* kCases[] = {
    "2-explicit-3d", "2-depth-3d", "2-negdepth-3d", "2-movctldir-3d", "2-oblique-3d", "2-overlap-3d",
    "102-overlap-3d", "12-explicit-3d", "102-explicit-3d", "112-explicit-3d", "2-deformable-3d", "2-tet-3d",
    "2-auto-3d", "12-auto-3d", "102-auto-3d", "112-auto-3d", "2-auto-behind-3d", "2-auto-coupledexcluded-3d",
    "2-coupled-3d", "12-coupled-3d", "2-coupled-movctldir-3d", "2-nomovctldir-3d", "2-undefined-target-3d",
    "2-undefined-mover-3d", "2-nohit-3d", "2-explicit-2d", "2-area-2d", "2-movctldir-2d", "12-explicit-2d",
    "102-explicit-2d", "112-explicit-2d", "2-coupled-2d", "12-coupled-2d", "2-target0-2d",
    "12-failed-stroke-2d", "ik-m102-bucket-3d", "ik-m102-bucket2-3d", "v-ifd-area1-dir0-2d",
    "v-ifd-area1-12-2d", "v-ifd-area1-102-2d", "v-ifd-area1-112-2d", "v-ifd-area1-coupled-2d",
    "v-ifd-area1-12coupled-2d", "v-ifd-areatiny-2d", "v-ifd-rot-only-3d", "v-ifd-failstroke-area1-2d"};

} // namespace

TEST_CASE(interference_cases_match_the_oracle)
{
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
        return l.rfind("Object ", 0) == 0 || l.rfind("ERROR  :", 0) == 0 || l.rfind("ERROR: Keyword OBJPOS", 0) == 0 ||
               l.rfind("       Current time", 0) == 0 || l.rfind("  Arbitrary path", 0) == 0 ||
               l.rfind("  Current TNow", 0) == 0 || l.rfind("  Failed in", 0) == 0 ||
               l.rfind("  Calculated new", 0) == 0;
    };
    o.reset = [] {
        def::cmd::objpos::g_last_disp[0] = 0.0;  // process-lifetime globals of a fresh DEF_PRE
        def::cmd::objpos::g_last_disp[1] = 0.0;
        def::cmd::objpos::g_last_disp[2] = 0.0;
        def::model::reset_globals_p1_10();
        def::cmd::ifd::reset_ray_state();
    };
    const std::string only = cmdoracle::env("IFD_ORACLE_ONLY", "");
    int ok = 0, bad = 0, skipped = 0;
    for (const char* c : kCases) {
        const std::string label = std::string("OBJPOS/") + c;
        if (!only.empty() && label.find(only) == std::string::npos) continue;
        const cmdoracle::Result res = r.run(label, o);
        if (!res.found) {
            std::printf("%-36s no oracle entry: skipped\n", label.c_str());
            ++skipped;
            continue;
        }
        std::printf("%-36s %s  records: %d equal, %d differ, %d baseline gaps\n%s", label.c_str(),
                    res.pass ? "PASS" : "FAIL", res.equal, res.differ, res.gaps, res.report.c_str());
        if (res.pass) ++ok;
        else ++bad;
    }
    std::printf("OBJPOS interference oracle: %d pass, %d fail, %d skipped\n", ok, bad, skipped);
    CHECK_EQ(bad, 0);
}

int main() { return testkit::run_all(); }
