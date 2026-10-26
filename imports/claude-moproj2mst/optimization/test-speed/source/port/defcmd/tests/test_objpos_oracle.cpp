// ctest-labels: p67-1 objpos oracle
// OBJPOS against the original DEF_PRE: every cached oracle run of the packet's command files
// (tests/commands/OBJPOS/cases.json, cases_gk.json, cases_v_ov-objpos.json and the mirror cases of
// cases_ik.json, labels "OBJPOS/<case>") is replayed through the port with cmd_oracle_runner.hpp:
// the OBJPOS messages must equal the original's stdout lines and every group 2 record of every
// object the original's OUT.KEY record, except the seed's baseline round-trip gaps (state
// variables until kw-g2sv lands; their tensor part is checked by test_objpos_tensor).
// A case without an oracle entry is skipped with a note. OBJPOS_ORACLE_ONLY=<substring> limits
// the cases; CMD_ORACLE_VERBOSE / CMD_ORACLE_GAPS as in the runner.
#include "cmd_oracle_runner.hpp"

#include <defcmd/objpos.hpp>
#include <defmodel/globals_p1_10.hpp>
#include <testkit/check.hpp>

namespace {

const char* kCases[] = {
    "1-basic_2d", "1-basic_3d", "1-coupled_2d", "1-coupled_3d", "11-basic_2d", "11-basic_3d", "11-coupled_3d",
    "13-coupled_3d", "13-noref_3d", "13-sub0_3d", "13-sub1_3d", "13-sub2_3d", "14-noB_3d", "14-noref_3d",
    "14-sub1_3d", "3-basic_2d", "3-basic_3d", "3-coupled_2d", "3-coupled_3d", "3-off_3d", "3-ydir_3d",
    "3-zero_3d", "4-basic_2d", "4-basic_3d", "4-oblique_2d", "4-oblique_3d", "4-zero_3d", "5-basic_2d",
    "5-first_2d", "5-noop_3d", "6-basic_2d", "6-basic_3d", "6-coupled_2d", "err-undef_2d", "err-undef_3d",
    "err-unknown_2d", "err-unknown_3d", "gk-tensor-y_3d", "gk-tensor-obl_3d", "gk-6-tri_3d", "gk-tensor_2d",
    "gk-4-type2_2d", "ik-mirror-brick-3d", "ik-mirror-tet-3d", "v-bits-rot_3d", "v-tensor_2d",
    "v-mirror-bits_2d", "v-mirror-bits_3d", "v-dir0_3d", "v-tplogy_2d", "v-ecflx-ctrl_2d", "v-ecflx_2d"};

} // namespace

TEST_CASE(objpos_cases_match_the_oracle)
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
        // GENDB's volume-compensation note is not an OBJPOS message (the runner skips GENDB)
        if (l.rfind("ERROR  : Volume compensation", 0) == 0) return false;
        return l.rfind("Object ", 0) == 0 || l.rfind("ERROR  :", 0) == 0 || l.rfind("ERROR: Keyword OBJPOS", 0) == 0 ||
               l.rfind("       Current time", 0) == 0;
    };
    o.reset = [] {
        def::cmd::objpos::g_last_disp[0] = 0.0;  // process-lifetime globals of a fresh DEF_PRE
        def::cmd::objpos::g_last_disp[1] = 0.0;
        def::cmd::objpos::g_last_disp[2] = 0.0;
        def::model::reset_globals_p1_10();  // the point kernel's SAVE frame
    };
    const std::string only = cmdoracle::env("OBJPOS_ORACLE_ONLY", "");
    int ok = 0, bad = 0, skipped = 0;
    std::vector<std::string> labels;
    for (const char* c : kCases) labels.push_back(std::string("OBJPOS/") + c);
#if __has_include(<defcmd/objpos_ifd.hpp>)
    // mode 14 sub-mode 0 on the cogging seed (ov-misc.md): needs p67-2's interference 14019fa00
    labels.push_back("GENAXS/v_objpos14_ctl");
#endif
    for (const std::string& label : labels) {
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
    std::printf("OBJPOS oracle: %d pass, %d fail, %d skipped\n", ok, bad, skipped);
    CHECK_EQ(bad, 0);
}

int main() { return testkit::run_all(); }
