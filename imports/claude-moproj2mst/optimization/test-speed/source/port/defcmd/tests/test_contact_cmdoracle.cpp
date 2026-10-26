// ctest-labels: p67-6 contact oracle
// INICTC / 2D GENCTC through the shared command replay harness (cmd_oracle_runner.hpp, packet
// p67-1): every group 2 record of every object against the oracle's OUT.KEY, baseline gaps
// reported separately, and the interpreter's INICTC/GENCTC message lines. The kernels' Fortran
// warnings (stderr) and the BCCDEF values (its writer, kw-g2sv, is not ported yet) are checked by
// test_contact_oracle.cpp. Until the harness is merged into this branch the test is a no-op.
#if __has_include("cmd_oracle_runner.hpp")
#include "cmd_oracle_runner.hpp"
#define DEF_HAVE_CMDORACLE 1
#endif

#include <cstdio>
#include <string>

#include <defcmd/contact_cmds.hpp>
#include <defcmd/ctc2d_save.hpp>
#include <testkit/check.hpp>

TEST_CASE(contact_commands_through_the_shared_harness)
{
#ifdef DEF_HAVE_CMDORACLE
    cmdoracle::Runner r;
    if (!r.ok()) return;
    cmdoracle::Options o;
    o.command = [](def::model::DB_STEP_DATA& m, std::FILE* f, const std::string& name) {
        if (name == "INICTC") return def::cmd::cmd_inictc(&m, f);
        if (name == "GENCTC") return def::cmd::cmd_genctc(&m, f);
        return -1;
    };
    o.message = [](const std::string& l) {
        return l.find("INICTC") != std::string::npos || l.find("GENCTC") != std::string::npos;
    };
    o.reset = [] { def::cmd::ftn::reset_ctc2d_save(); };
    const char* labels[] = {
        "INICTC/spike_all", "GENCTC/noline", "GENCTC/spike_0", "GENCTC/spike_large", "GENCTC/spike_small",
        "GENCTC/spike_tiny", "GENCTC/text", "GENCTC/spike_move", "GENCTC/spike_warn", "GENCTC/spike_meshed",
        "GENCTC/spike_meshed_0", "GENCTC/spike_huge", "GENCTC/spike_bcc", "GENCTC/m_valve_si_0",
        "GENCTC/m_valve_si_big", "GENCTC/m_hammer_si_0", "GENCTC/m_hammer_si_big",
        "GENCTC/m_plate_bend_ep_si_0", "GENCTC/m_plate_bend_ep_si_big", "GENCTC/m_extrusion_ep_si_0",
        "GENCTC/m_extrusion_ep_si_big", "GENCTC/m_sheet_si_0", "GENCTC/m_sheet_si_big", "GENCTC/m_frac_si_0",
        "GENCTC/m_frac_si_big"};
    int failed = 0;
    for (const char* l : labels) {
        cmdoracle::Result res = r.run(l, o);
        std::printf("%-30s %s equal %d differ %d gaps %d\n", l, res.pass ? "ok  " : "FAIL", res.equal, res.differ,
                    res.gaps);
        if (!res.pass) {
            std::printf("%s", res.report.c_str());
            ++failed;
        }
    }
    CHECK_EQ(failed, 0);
#else
    std::printf("cmd_oracle_runner.hpp not on this branch yet: skipped\n");
#endif
}

int main() { return testkit::run_all(); }
