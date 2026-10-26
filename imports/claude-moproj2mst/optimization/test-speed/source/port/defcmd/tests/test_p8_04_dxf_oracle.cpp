// ctest-labels: p8-04
// Oracle replay of the GFREAD DXF cases (tests/commands/GFREAD/gfread_2d_dxf.json,
// gfread_dxf_*.json) through cmdoracle::Runner, the GFREAD command of p8-01 and this packet's
// reader.
//
// Needs p8-01's defcmd/cmd_p8_01.hpp (cmd_gfread), p5-01's defcmd/dispatch.hpp (the runner
// includes it) and the oracle cache; skips otherwise. The cases whose reader result is decided
// before the Fortran conversion (binary, empty, arc radius 0) always run; the others reach p8-03's
// pass 2 (FUN_14040c220) and run only with P804_ORACLE_FULL=1 (after p8-03 is merged).
//
// Baseline gaps: the runner looks for "_baseline/<stem of the KFREAD file name>", here
// "_baseline/IN", which does not exist (these cases name the RING_SI.KEY seed IN.KEY). This test
// therefore treats a differing record of an object other than 1 as a seed round-trip gap when the
// original's record equals the one in its plain RING_SI round trip ("_baseline/RING_SI"), i.e.
// when GFREAD did not touch it in the original. Records of object 1 must match.
#define _CRT_SECURE_NO_WARNINGS
#include <testkit/check.hpp>

#include <cstdio>
#include <string>

#if __has_include(<defcmd/cmd_p8_01.hpp>) && __has_include(<defcmd/dispatch.hpp>)
#include "cmd_oracle_runner.hpp"

#include <defcmd/cmd_p8_01.hpp>
#include <defcmd/dxf.hpp>
#define P804_HAVE_GFREAD 1
#endif

#ifdef P804_HAVE_GFREAD
namespace {

namespace fs = std::filesystem;

// cache directory of the newest entry with this label
fs::path entry_dir(const std::string& label)
{
    fs::path cache = cmdoracle::env("DEFORM_ORACLE_CACHE", "");
    if (cache.empty()) cache = fs::path(cmdoracle::env("USERPROFILE", "")) / ".deform-oracle";
    fs::path best;
    std::string best_created;
    if (!fs::exists(cache / "entries")) return best;
    for (auto& d1 : fs::directory_iterator(cache / "entries")) {
        if (!d1.is_directory()) continue;
        for (auto& d2 : fs::directory_iterator(d1.path())) {
            const fs::path rj = d2.path() / "result.json";
            if (!fs::exists(rj)) continue;
            const std::string j = cmdoracle::slurp(rj);
            if (cmdoracle::json_string(j, "label") != label) continue;
            const std::string c = cmdoracle::json_string(j, "created");
            if (best.empty() || best_created < c) {
                best = d2.path();
                best_created = c;
            }
        }
    }
    return best;
}

} // namespace
#endif

TEST_CASE(dxf_oracle_cases)
{
#ifndef P804_HAVE_GFREAD
    std::printf("  skipped: p8-01's defcmd/cmd_p8_01.hpp or p5-01's defcmd/dispatch.hpp is not in this build\n");
#else
    cmdoracle::Runner r;
    if (!r.ok()) {
        std::printf("  skipped: no oracle cache\n");
        return;
    }
    const fs::path base_dir = entry_dir("_baseline/RING_SI");
    const cmdoracle::RecordMap base =
        base_dir.empty() ? cmdoracle::RecordMap{} : cmdoracle::object_records(cmdoracle::slurp(base_dir / "files" / "OUT.KEY"));
    cmdoracle::Options o;
    o.command = [](def::model::DB_STEP_DATA& m, std::FILE* f, const std::string& name) {
        return name == "GFREAD" ? def::cmd::cmd_gfread(&m, f) : -1;
    };
    o.message = [](const std::string& l) { return l.rfind("INFO   : Loading geometry", 0) == 0; };
    o.reset = [] { def::cmd::dxf::reset_globals(); };
    const bool full = cmdoracle::env("P804_ORACLE_FULL", "0") == "1";
    const fs::path gf = cmdoracle::Runner::repo_root() / "tests" / "commands" / "GFREAD";
    std::vector<fs::path> files = {gf / "gfread_2d_dxf.json"};
    for (auto& d : fs::directory_iterator(gf))
        if (d.path().filename().string().rfind("gfread_dxf_", 0) == 0) files.push_back(d.path());
    int pass = 0, fail = 0, not_run = 0, gaps = 0;
    for (const fs::path& cf : files) {
        const std::string tag = cf.stem().string();
        const std::string label = cmdoracle::json_string(cmdoracle::slurp(cf), "label");
        const bool early = tag == "gfread_dxf_binary" || tag == "gfread_dxf_empty" || tag == "gfread_dxf_arc_r0";
        if (!early && !full) {
            ++not_run;
            continue;
        }
        const cmdoracle::Result res = r.run(label, o);
        if (!res.found) {
            std::printf("  %-36s no oracle entry\n", tag.c_str());
            ++not_run;
            continue;
        }
        const cmdoracle::RecordMap orig = cmdoracle::object_records(cmdoracle::slurp(entry_dir(label) / "files" / "OUT.KEY"));
        bool ok = true;
        std::istringstream rep(res.report);
        std::string line;
        while (std::getline(rep, line)) {
            char kw[64] = {0};
            int obj = 0;
            if (std::sscanf(line.c_str(), "  record %63s object %d differs", kw, &obj) == 2) {
                const std::pair<int, std::string> k{obj, kw};
                auto a = orig.find(k);
                auto b = base.find(k);
                const bool untouched = (a == orig.end() && b == base.end()) ||
                                       (a != orig.end() && b != base.end() && a->second == b->second);
                if (obj != 1 && untouched) {
                    ++gaps;
                    continue;
                }
                ok = false;
                std::printf("  %s:%s\n", tag.c_str(), line.c_str() + 1);
            } else if (line.find("messages differ") != std::string::npos || line.find("orig:") != std::string::npos ||
                       line.find("port:") != std::string::npos) {
                ok = false;
                std::printf("  %s:%s\n", tag.c_str(), line.c_str() + 1);
            }
        }
        std::printf("  %-36s %s (%d records equal)\n", tag.c_str(), ok ? "same" : "DIFFERENT", res.equal);
        ok ? ++pass : ++fail;
        CHECK(ok);
    }
    std::printf("  %d same, %d different, %d not run (need p8-03 pass 2: P804_ORACLE_FULL=1); %d seed round-trip gap "
                "records skipped\n", pass, fail, not_run, gaps);
#endif
}

int main() { return testkit::run_all(); }
