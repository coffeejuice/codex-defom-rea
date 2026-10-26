// ctest-labels: p67-5 commands
// Smoke test of the p67-5 test support: the baseline cases of tests/commands/_baseline (seed
// KFREAD + KFWRIT, no command) read through the port's KEY loop give the original's group 2
// records for the keywords the p67-5 commands change.
#include "p67_5_support.hpp"

#include <testkit/check.hpp>

TEST_CASE(baseline_seeds_round_trip)
{
    unsigned long long& o = *__local_stdio_printf_options();
    o = (o | _CRT_INTERNAL_PRINTF_LEGACY_THREE_DIGIT_EXPONENTS | _CRT_INTERNAL_PRINTF_LEGACY_MSVCRT_COMPATIBILITY) &
        ~static_cast<unsigned long long>(_CRT_INTERNAL_PRINTF_STANDARD_ROUNDING);
    if (p675::repo_root().empty()) {
        std::printf("no repository root: skipped\n");
        return;
    }
    // BCCDEF / BCCTMP are written by the state-variable writer of kw-g2sv (not merged yet)
    const std::set<std::string> kws = {"RZ", "ELMCON", "WPAXIS", "ECCTMP"};
    int checked = 0;
    for (const p675::Case& c : p675::load_cases("_baseline")) {
        if (c.label != "_baseline/SPIKE_SI" && c.label != "_baseline/UPSET_Brick" && c.label != "_baseline/_TEMP_STEP")
            continue;
        p675::fs::path e = p675::oracle_entry(c.label);
        if (e.empty()) {
            std::printf("%s: not in the oracle cache, skipped\n", c.label.c_str());
            continue;
        }
        auto& m = *new p675::DB_STEP_DATA(3);
        p675::CmdRunner run(m, c);
        {
            p675::StdoutCapture cap;
            run.run();
        }
        std::string mine = p675::write_model_key(m);
        std::string orig = p675::slurp(e / "files" / "OUT.KEY");
        int bad = p675::compare_records(orig, mine, kws, true, c.label);
        std::printf("%s: %d mismatching records\n", c.label.c_str(), bad);
        CHECK_EQ(bad, 0);
        ++checked;
    }
    std::printf("%d baseline cases checked\n", checked);
}

int main() { return testkit::run_all(); }
