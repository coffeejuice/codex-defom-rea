// ctest-labels: p8-01 oracle
// Packet p8-01 against the original DEF_PRE: the cached oracle runs of tests/commands/GFREAD
// (driver cases), MORP, GENINT, IMPLUB, IMPEQP, ARMCTL and EXECMD are replayed through the port
// with cmdoracle::Runner, the command keywords through p5-01's dispatcher (p8_01_support.hpp): the
// command messages and every group 2 record of OUT.KEY (except the seed's own round-trip gaps), and
// in addition the group 3 records, BCCDEF and the files the commands write, byte for byte. External
// programs are p8-07's launcher: the launch calls are checked (command line and dirKind as the
// case body passes them; MORP.md section 5, EXECMD.md section 3), the programs are not run;
// EXECMD mode 1 runs its `echo` through system() and the file it writes is compared.
// A case whose oracle entry is missing is skipped; a case that needs another packet's body counts
// as blocked (GFREAD .UNV: p8-02's 3D table). P801_ORACLE_ONLY=<substring> limits the cases.
#include "p8_01_support.hpp"

#include <testkit/check.hpp>

namespace {

using Launch = std::tuple<int, std::string, int>;

struct Case {
    const char* label;
    std::vector<Launch> launches;      // expected launcher calls
    std::vector<std::string> systems;  // expected system() calls
    std::set<std::string> external;    // files written by external programs
    bool needs_p8_02 = false;
};

bool message(const std::string& l)
{
    static const char* k[] = {"INFO   : Loading geometry", "ERROR  : Reading GFREAD", "ERROR: Keyword file",
                              "ERROR: Keyword GFREAD",       "ERROR: Keyword MORP",     "ERROR: Keyword GENINT",
                              "ERROR: Keyword IMPLUB",       "ERROR: Keyword IMPEQP",   "ERROR: Keyword ARMCTL",
                              "ERROR: Keyword EXECMD",       "WARNING: Keyword X.STL"};
    for (const char* p : k)
        if (l.rfind(p, 0) == 0) return true;
    return l.find(" does not exist.") != std::string::npos;
}

std::vector<Case> cases()
{
    return {
        {"GFREAD mixed-case extension", {}, {}, {}},
        {"GFREAD missing file", {}, {}, {}},
        {"GFREAD id 0", {}, {}, {}},
        {"GFREAD .KEY is a no-op success (3D)", {}, {}, {}},
        {"GFREAD .STL on a 2D object is a no-op success", {}, {}, {}},
        {"GFREAD UNV via STB2KEY3 (3D)", {}, {}, {"STB3.TMP"}, true},
        {"MORP n=1 m=3 2D", {{1, "DEF_MORP2.EXE", 2}}, {}, {"A1"}},  // A1: written by DEF_MORP2
        {"MORP n=2 m=0 3D", {{1, "DEF_MORP3.EXE morphing DEF_MORP3.INI", 3}}, {}, {}},
        {"MORP too few string lines", {}, {}, {}},
        {"GENINT 2D with corpus InterObject.KEY", {}, {}, {}},
        {"GENINT 3D with corpus InterObject.KEY", {}, {}, {}},
        {"GENINT missing template file", {}, {}, {}},
        {"IMPLUB 1 2 0 (2D)", {}, {}, {}},
        {"IMPLUB 1 2 1 (2D)", {}, {}, {}},
        {"IMPLUB 1 2 0 (3D)", {}, {}, {}},
        {"IMPLUB 1 2 1 (3D)", {}, {}, {}},
        {"IMPLUB 1 2 0 (2D, -1 codes)", {}, {}, {}},
        {"IMPLUB 1 2 1 (2D, -1 codes)", {}, {}, {}},
        {"IMPLUB 1 2 0 (3D, -201 codes)", {}, {}, {}},
        {"IMPLUB 1 2 1 (3D, -1 codes)", {}, {}, {}},
        {"IMPEQP 2 equipment (2D)", {}, {}, {}},
        {"IMPEQP 5 creates object slots (2D)", {}, {}, {}},
        {"IMPEQP missing file", {}, {}, {}},
        {"ARMCTL 1 0 5", {}, {}, {}},
        {"ARMCTL with no values", {}, {}, {}},
        {"ARMCTL 7,abc,-3 9", {}, {}, {}},
        {"EXECMD 0 relative program", {{1, "cmd /c echo m0> EXEC0.TXT\n", 0}}, {}, {"EXEC0.TXT"}},
        {"EXECMD 1 (system)", {}, {"echo m1> EXEC1.TXT\n"}, {}},
        {"EXECMD 2 absolute program", {{1, "C:\\Windows\\System32\\cmd.exe /c echo m2> EXEC2.TXT\n", 2}}, {}, {"EXEC2.TXT"}},
        {"EXECMD 3 DEFORM program search", {{1, "FIXGEO > EXEC3.TXT\n", 3}}, {}, {"EXEC3.TXT"}},
    };
}

} // namespace

TEST_CASE(p8_01_cases_match_the_oracle)
{
    keytest::legacy_printf();
    cmdoracle::Runner r;
    if (!r.ok()) {
        std::printf("no oracle cache: skipped\n");
        return;
    }
    p801test::EntryDirs dirs;
    cmdoracle::Options o;
    o.command = p801test::route_command;
    o.message = message;
    const std::string only = cmdoracle::env("P801_ORACLE_ONLY", "");
    int ok = 0, bad = 0, skipped = 0, blocked = 0;
    for (const Case& c : cases()) {
        if (!only.empty() && std::string(c.label).find(only) == std::string::npos) continue;
#ifdef DEF_P801_HAVE_P8_02
        // with p8-02's 3D table the .UNV case runs STB2KEY3 through the launcher
        if (c.needs_p8_02 && cmdoracle::env("P801_REAL_LAUNCH", "0") != "1") {
            std::printf("%-44s needs the real launcher (P801_REAL_LAUNCH=1): skipped\n", c.label);
            ++skipped;
            continue;
        }
#endif
        p801test::g_calls = p801test::Calls{};
        p801test::g_calls.run_echo = true;
        p801test::install_hooks();
        const cmdoracle::Result res = r.run(c.label, o);
        if (!res.found) {
            std::printf("%-44s no oracle entry: skipped\n", c.label);
            ++skipped;
            continue;
        }
        const p801test::Calls calls = p801test::g_calls;  // the baseline replay runs no command
        const std::filesystem::path dir = dirs.find(c.label);
        std::set<std::string> inputs;
        for (const cmdoracle::CaseInput& in : cmdoracle::case_inputs(cmdoracle::slurp(dir / "result.json")))
            inputs.insert(in.name);
        const p801test::Extra x = p801test::compare_extra(dir, inputs, c.external);
        bool pass = res.pass && x.pass;
        std::string extra = x.report;
        if (calls.launches != c.launches) {
            pass = false;
            extra += "  launches differ:";
            for (auto& [m, s, d] : calls.launches) extra += " (" + std::to_string(m) + ", '" + s + "', " + std::to_string(d) + ")";
            extra += "\n";
        }
        if (calls.systems != c.systems) {
            pass = false;
            extra += "  system() calls differ\n";
        }
        const bool is_blocked = !calls.blocked_3d.empty();
        if (is_blocked && !c.needs_p8_02) {
            pass = false;
            extra += "  unexpected 3D reader call\n";
        }
        const char* verdict = pass ? "PASS" : (is_blocked ? "BLOCKED (p8-02 3D table)" : "FAIL");
        std::printf("%-44s %s  records: %d equal, %d differ, %d baseline gaps\n%s%s", c.label, verdict,
                    res.equal + x.equal, res.differ + x.differ, res.gaps, res.report.c_str(), extra.c_str());
        if (pass) {
            ++ok;
        } else if (is_blocked && c.needs_p8_02) {
            ++blocked;
        } else {
            ++bad;
        }
    }
    std::printf("p8-01 oracle: %d pass, %d fail, %d blocked, %d skipped\n", ok, bad, blocked, skipped);
    CHECK_EQ(bad, 0);
}

#if __has_include(<defcmd/iges.hpp>) && __has_include(<defcmd/dxf.hpp>) && __has_include(<defcmd/geolib.hpp>) &&     __has_include(<defcmd/stl_nas.hpp>)
// Integration build only (the readers of p8-02/03/04/09 present): every other GFREAD case of
// tests/commands/GFREAD goes through the p8-01 driver and 2D front end into the owners' readers.
// The 3D .PDA/.UNV cases need the real launcher (P801_REAL_LAUNCH=1, DEFORM_PC_PATH).
TEST_CASE(gfread_reader_cases_through_the_front_end)
{
    keytest::legacy_printf();
    cmdoracle::Runner r;
    if (!r.ok()) return;
    p801test::EntryDirs dirs;
    cmdoracle::Options o;
    o.command = p801test::route_command;
    o.message = message;
    std::set<std::string> mine;
    for (const Case& c : cases()) mine.insert(c.label);
    int ok = 0, bad = 0;
    for (auto& e : std::filesystem::directory_iterator(cmdoracle::Runner::repo_root() / "tests" / "commands" / "GFREAD")) {
        if (e.path().extension() != ".json") continue;
        // P801_FRONT_SKIP=<stem>,<stem>: cases that stop in another packet's DEF_NOT_PORTED
        if (("," + cmdoracle::env("P801_FRONT_SKIP", "") + ",").find("," + e.path().stem().string() + ",") != std::string::npos)
            continue;
        const std::string label = cmdoracle::json_string(cmdoracle::slurp(e.path()), "label");
        if (mine.count(label) != 0) continue;
        // the process ends, as in the original (Fortran runtime error / STOP, exit(1) of the STL
        // cell list): not replayable in this process
        if (label == "GFREAD IGES letters in a D-entry field" || label == "GFREAD IGES parameter data cut short" ||
            label == "GFREAD STL all triangles degenerate")
            continue;
        p801test::g_calls = p801test::Calls{};
        p801test::install_hooks();
        const cmdoracle::Result res = r.run(label, o);
        if (!res.found) continue;
        const std::filesystem::path dir = dirs.find(label);
        std::set<std::string> inputs;
        for (const cmdoracle::CaseInput& in : cmdoracle::case_inputs(cmdoracle::slurp(dir / "result.json")))
            inputs.insert(in.name);
        const p801test::Extra x = p801test::compare_extra(dir, inputs, {"STB3.TMP", "PDA3.TMP"});
        const bool pass = res.pass && x.pass;
        std::printf("%-44s %s  records: %d equal, %d differ\n%s%s", label.c_str(), pass ? "PASS" : "FAIL",
                    res.equal + x.equal, res.differ + x.differ, res.report.c_str(), x.report.c_str());
        (pass ? ok : bad)++;
    }
    std::printf("GFREAD reader cases through p8-01: %d pass, %d fail\n", ok, bad);
    CHECK_EQ(bad, 0);
}
#endif

int main() { return testkit::run_all(); }
