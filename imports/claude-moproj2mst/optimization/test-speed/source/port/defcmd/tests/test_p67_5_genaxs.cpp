// ctest-labels: p67-5 commands
// GENAXS against the original: every case of tests/commands/GENAXS/cases.json is run through the
// port's KEY loop and compared with the oracle's OUT.KEY record by record (group 1 and every
// object block), or, for the cases where the original crashes (exit code 0xC0000005), the port
// must end the same way.
//
// The object translation FUN_14021d0b0 and OBJPOS (objpos14) are p67-1's (defcmd/objpos.hpp);
// every record is compared.
#include "p67_5_support.hpp"

#include <defcmd/dispatch.hpp>
#include <cmath>
#include <stdexcept>

#include <defcmd/p67_5.hpp>
#include <defcmd/objpos.hpp>
#include <defmodel/geo_kw/geo_kw.hpp>
#include <defmodel/layout/obj_geo3d.hpp>
#include <defrt/stdlib/crt_emulation.hpp>
#include <testkit/check.hpp>

using namespace p675;

namespace {


struct AbnormalExit {
    std::uint32_t status;
};

void throw_exit(std::uint32_t status) { throw AbnormalExit{status}; }

// Compares the records the port writes (group 1 sections and object blocks) with the original's,
// for the record keys `want(kw, obj)` accepts (obj -1 for non-object keywords). Keys only the
// original has are compared too when they are WPAXIS records (the command's own output). Known
// gaps of the port's writer that this packet does not own are left out: BCCDEF / BCCTMP (state
// variable writer of kw-g2sv, not merged yet).
template <class Want>
int compare_port_written(const std::string& orig, const std::string& mine, bool verbose, const std::string& what, Want want)
{
    auto a = records_of(orig);
    auto b = records_of(mine);
    std::set<std::string> keys;
    for (auto& [k, v] : b) keys.insert(k);
    for (auto& [k, v] : a)
        if (k.rfind("WPAXIS ", 0) == 0) keys.insert(k);
    int bad = 0;
    for (const std::string& k : keys) {
        const std::size_t sp = k.find(' ');
        const std::string kw = k.substr(0, sp);
        const int obj = sp == std::string::npos ? -1 : std::atoi(k.c_str() + sp + 1);
        if (kw == "BCCDEF" || kw == "BCCTMP") continue;
        if (!want(kw, obj) || a[k] == b[k]) continue;
        ++bad;
        if (verbose) {
            std::printf("MISMATCH %s %s\n  orig:", what.c_str(), k.c_str());
            for (auto& s : a[k]) std::printf("\n%s", s.c_str());
            std::printf("\n  port:");
            for (auto& s : b[k]) std::printf("\n%s", s.c_str());
            std::printf("\n");
        }
    }
    return bad;
}

} // namespace

TEST_CASE(genaxs_cases_against_the_oracle)
{
    unsigned long long& opt = *__local_stdio_printf_options();
    opt = (opt | _CRT_INTERNAL_PRINTF_LEGACY_THREE_DIGIT_EXPONENTS | _CRT_INTERNAL_PRINTF_LEGACY_MSVCRT_COMPATIBILITY) &
          ~static_cast<unsigned long long>(_CRT_INTERNAL_PRINTF_STANDARD_ROUNDING);
    if (repo_root().empty()) {
        std::printf("no repository root: skipped\n");
        return;
    }
    def::rt::set_abnormal_exit_hook(throw_exit);
    const bool verbose = env("P675_VERBOSE", "0") == "1";
    const int id = kw_id("GENAXS");
    const int objpos_id = kw_id("OBJPOS");
    int full = 0, partial = 0, crashed = 0, missing = 0, failed = 0, skipped = 0;
    for (const Case& c : load_cases("GENAXS")) {
        fs::path e = oracle_entry(c.label);
        if (e.empty()) {
            std::printf("%-20s not in the oracle cache\n", c.label.c_str());
            ++missing;
            continue;
        }
        std::string rj = slurp(e / "result.json");
        const bool orig_crash = rj.find("\"exit_code\": 3221225477") != std::string::npos;
        def::cmd::objpos::g_last_disp[0] = def::cmd::objpos::g_last_disp[1] = def::cmd::objpos::g_last_disp[2] = 0.0;
        auto& m = *new DB_STEP_DATA(3);  // leaked with its objects, as in the group 2 tests
        CmdRunner run(m, c);
        run.on_command = [&](int kid, std::FILE* f) {
            if (kid == objpos_id || kid == id) return def::cmd::dispatch(&m, f, kid);  // FUN_140378e00
            return -1;
        };
        std::uint32_t port_exit = 0;
        try {
            StdoutCapture cap;
            run.run();
        } catch (const AbnormalExit& x) {
            port_exit = x.status;
        }
        if (orig_crash || port_exit != 0) {
            const bool ok = orig_crash && port_exit == def::rt::kStatusAccessViolation;
            std::printf("%-20s %s (original crashes: %d, port exit 0x%08X)\n", c.label.c_str(),
                        ok ? "crash, as the original" : "MISMATCH", orig_crash ? 1 : 0, port_exit);
            CHECK(ok);
            if (ok) ++crashed; else ++failed;
            continue;
        }
        if (!run.unsupported.empty()) {  // OBJPOS (p67-1) before GENAXS in objpos14
            std::printf("%-20s needs %s: skipped\n", c.label.c_str(), run.unsupported[0].c_str());
            ++skipped;
            continue;
        }
        const std::string orig = slurp(e / "files" / "OUT.KEY");
        const std::string mine = write_model_key(m);
        const int bad = compare_port_written(orig, mine, verbose, c.label, [](const std::string&, int) { return true; });
        ++full;
        std::printf("%-20s full, %d mismatching records\n", c.label.c_str(), bad);
        CHECK_EQ(bad, 0);
        if (bad != 0) ++failed;
    }
    std::printf("GENAXS: %d full, %d partial, %d crashes as the original, %d skipped, %d not cached, %d failed\n", full,
                partial, crashed, skipped, missing, failed);
}

int main() { return testkit::run_all(); }
