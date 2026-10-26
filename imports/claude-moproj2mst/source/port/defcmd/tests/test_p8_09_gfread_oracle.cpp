// ctest-labels: p8-09 geolib oracle
// GFREAD of GEO files against the original DEF_PRE (packet p8-09's readers and stores through
// p8-01's GFREAD front end): tests/commands/GFREAD gfread_2d_geo, gfread_3d_geo, gfread_geo2d_*,
// gfread_geo3d_*. Every group 2 record of every object must equal the original's OUT.KEY (seed
// round-trip gaps excepted, baseline "_baseline/<seed>"), and the "Loading geometry" lines its
// stdout.
//
// Needs p8-01's handler (defcmd/cmd_p8_01.hpp): without it the test only says so. The 3D cases go
// through p8-02's 3D table 140243a70; until that is merged the test installs p8-01's load_3d hook
// with the GEO branch of 140243a70 (case 2: 140246f70(g, file, &0, 3, 0), then 14024bac0(g, obj, 3);
// the plane grouping 1402059a0 that follows writes nothing KFWRIT prints, GFREAD_STL_GEO_NAS.md
// 3.4) and 3D stores need p67-1's 140163340 / 1402009f0.
//
// Helper (the shared runner is not edited): cmd_oracle_runner.hpp expects a cached case with two
// file inputs (CMD.KEY, seed). The GFREAD cases have an inline CMD.KEY and a third input (the GEO
// file), so this test builds a private cache (temp dir, pid in the name) with one entry per case
// in that shape (stdout.txt and files/OUT.KEY copied from the real entry, CMD.KEY written out),
// copies the seed's baseline entry, and the GFREAD callback runs in a work dir that holds copies
// of the extra inputs.
#include "cmd_oracle_runner.hpp"

#include <process.h>
#include <stdlib.h>

#include <defcmd/geolib.hpp>
#if __has_include(<defrt/stdlib/stdio_compat.hpp>)
#include <defrt/stdlib/stdio_compat.hpp>
#endif
#include <testkit/check.hpp>

#if __has_include(<defcmd/cmd_p8_01.hpp>)
#include <defcmd/cmd_p8_01.hpp>
#include "../src/p8_01/ext_p8_02.hpp"
#define P809_HAVE_P8_01 1
#endif
// The shared runner with inline / extra inputs and the default dispatch (p67-gate) comes with
// defcmd/dispatch.hpp: then the cases are replayed by their own labels (second test case).
#if defined(P809_HAVE_P8_01) && __has_include(<defcmd/dispatch.hpp>)
#define P809_NEW_RUNNER 1
#endif

namespace {

namespace fs = std::filesystem;
namespace gl = def::cmd::geolib;

struct CaseDef {
    const char* stem;
    bool three_d;
};
const CaseDef kCases[] = {
    {"gfread_2d_geo", false},       {"gfread_geo2d_multi", false},    {"gfread_geo2d_topo", false},
    {"gfread_geo2d_from3d", false}, {"gfread_3d_geo", true},          {"gfread_geo3d_small", true},
    {"gfread_geo3d_badcheck", true}, {"gfread_geo3d_noelem", true},
};

struct Input {
    std::string name, source, text;
};

// a JSON string value starting at p, with the escapes n, r, t decoded (cmdoracle::json_string
// keeps only the escaped character, which turns an inline CMD.KEY into one line)
std::string json_text(const std::string& j, std::size_t p)
{
    std::string out;
    while (p < j.size() && j[p] != '"') {
        char c = j[p++];
        if (c == '\\' && p < j.size()) {
            c = j[p++];
            if (c == 'n') c = '\n';
            else if (c == 'r') c = '\r';
            else if (c == 't') c = '\t';
        }
        out += c;
    }
    return out;
}

std::vector<Input> case_inputs(const std::string& j)
{
    std::vector<Input> v;
    std::size_t pos = j.find("\"inputs\"");
    while (true) {
        std::size_t end = 0;
        const std::size_t at = j.find("\"name\": \"", pos);
        if (at == std::string::npos) break;
        Input in;
        in.name = cmdoracle::json_string(j, "name", pos, &end);
        const std::size_t next = j.find("\"name\": \"", end);
        const std::size_t s = j.find("\"source\": \"", end);
        const std::size_t t = j.find("\"text\": \"", end);
        if (s != std::string::npos && (next == std::string::npos || s < next)) in.source = cmdoracle::json_string(j, "source", end);
        if (t != std::string::npos && (next == std::string::npos || t < next)) in.text = json_text(j, t + 9);
        v.push_back(in);
        pos = end;
    }
    return v;
}

std::string fwd(std::string s)
{
    for (char& c : s)
        if (c == '\\') c = '/';
    return s;
}

// newest real cache entry of a label
fs::path real_entry(const fs::path& cache, const std::string& label)
{
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
            if (best.empty() || c > best_created) {
                best = d2.path();
                best_created = c;
            }
        }
    }
    return best;
}

void copy_entry_files(const fs::path& from, const fs::path& to)
{
    fs::create_directories(to / "files");
    std::error_code ec;
    fs::copy_file(from / "stdout.txt", to / "stdout.txt", fs::copy_options::overwrite_existing, ec);
    if (fs::exists(from / "files" / "OUT.KEY"))
        fs::copy_file(from / "files" / "OUT.KEY", to / "files" / "OUT.KEY", fs::copy_options::overwrite_existing);
}

#ifdef P809_HAVE_P8_01
// GEO branch (type 2) of p8-02's 3D table 140243a70, for the replays only (see the header)
bool load_3d_geo_only(def::cmd::gfread::Loader*, const char* file, def::model::LLOBJ_DATA* obj, std::int32_t type)
{
    if (type != 2) DEF_NOT_PORTED(0x140243a70);
    gl::DEF_AMGGEO g;
    const char flag = 0;
    bool r = gl::fun_140246f70(&g, file, &flag, 3, nullptr);
    if (r) r = gl::fun_14024bac0(&g, obj, 3);  // then 1402059a0(obj, 0): no KEY effect
    return r;
}
#endif

} // namespace

TEST_CASE(gfread_geo_cases_match_the_oracle)
{
#if !defined(P809_HAVE_P8_01)
    std::printf("p8-01 (defcmd/cmd_p8_01.hpp) not merged: GFREAD replays skipped\n");
#elif defined(P809_NEW_RUNNER)
    std::printf("shared runner with inline inputs present: see gfread_cases_by_label\n");
#else
    keytest::legacy_printf();
    const fs::path real = fs::path(cmdoracle::env("DEFORM_ORACLE_CACHE", "").empty()
                                       ? fs::path(cmdoracle::env("USERPROFILE", "")) / ".deform-oracle"
                                       : fs::path(cmdoracle::env("DEFORM_ORACLE_CACHE", "")));
    if (!fs::exists(real / "entries")) {
        std::printf("no oracle cache: skipped\n");
        return;
    }
    const std::string pid = std::to_string(_getpid());
    const fs::path mine = fs::temp_directory_path() / ("p809_cache_" + pid);
    const fs::path work = fs::temp_directory_path() / ("p809_work_" + pid);
    std::error_code ec;
    fs::remove_all(mine, ec);
    fs::remove_all(work, ec);
    fs::create_directories(mine / "entries" / "00");
    fs::create_directories(work);
    const fs::path dir = testkit::repo_root() / "tests" / "commands" / "GFREAD";
    std::map<std::string, std::vector<Input>> extra;  // stem -> inputs other than CMD.KEY and seed
    std::vector<std::string> labels;
    for (const CaseDef& c : kCases) {
        const std::string j = cmdoracle::slurp(dir / (std::string(c.stem) + ".json"));
        const std::string label = cmdoracle::json_string(j, "label");
        const fs::path e = real_entry(real, label);
        if (e.empty()) {
            std::printf("%-24s no oracle entry: skipped\n", c.stem);
            continue;
        }
        std::vector<Input> ins = case_inputs(j);
        std::string cmd_text, seed_src;
        std::vector<Input> rest;
        for (const Input& in : ins) {
            if (in.name == "CMD.KEY") cmd_text = in.text;
            else if (in.name == "IN.KEY") seed_src = in.source;
            else rest.push_back(in);
        }
        const fs::path cmd = mine / (std::string(c.stem) + "_CMD.KEY");
        {
            std::ofstream f(cmd, std::ios::binary);
            f << cmd_text;
        }
        const std::string seed_name = fs::path(seed_src).filename().string();
        const fs::path d = mine / "entries" / "00" / c.stem;
        copy_entry_files(e, d);
        {
            std::ofstream f(d / "result.json", std::ios::binary);
            f << "{\n \"label\": \"" << c.stem << "\",\n \"created\": \"" << cmdoracle::json_string(cmdoracle::slurp(e / "result.json"), "created")
              << "\",\n \"inputs\": [\n  {\"name\": \"CMD.KEY\", \"source\": \"" << fwd(cmd.string()) << "\"},\n  {\"name\": \""
              << seed_name << "\", \"source\": \"" << fwd(seed_src) << "\"}\n ]\n}\n";
        }
        // the seed's baseline entry
        const std::string blabel = "_baseline/" + fs::path(seed_name).stem().string();
        const fs::path bd = mine / "entries" / "00" / ("base_" + fs::path(seed_name).stem().string());
        if (!fs::exists(bd)) {
            const fs::path be = real_entry(real, blabel);
            if (!be.empty()) {
                copy_entry_files(be, bd);
                fs::copy_file(be / "result.json", bd / "result.json", fs::copy_options::overwrite_existing);
            }
        }
        extra[c.stem] = rest;
        labels.push_back(c.stem);
    }
    _putenv_s("DEFORM_ORACLE_CACHE", mine.string().c_str());
    cmdoracle::Runner r;
    _putenv_s("DEFORM_ORACLE_CACHE", "");
    REQUIRE(r.ok());

    std::string current;
    cmdoracle::Options o;
    o.command = [&](def::model::DB_STEP_DATA& m, std::FILE* f, const std::string& name) {
        if (name != "GFREAD") return -1;
        for (const Input& in : extra[current])
            fs::copy_file(in.source, work / in.name, fs::copy_options::overwrite_existing);
        const fs::path back = fs::current_path();
        fs::current_path(work);
        const int rc = def::cmd::cmd_gfread(&m, f);
        std::fflush(stdout);
        fs::current_path(back);
        return rc;
    };
    o.message = [](const std::string& l) { return l.rfind("INFO   : Loading geometry", 0) == 0 || l.rfind("ERROR", 0) == 0; };
    o.reset = [] { gl::reset_token_reader(); };
    def::cmd::gfread::ext::g_load_3d_hook = load_3d_geo_only;  // until p8-02 is merged

    int ok = 0, bad = 0;
    for (const CaseDef& c : kCases) {
        if (extra.find(c.stem) == extra.end()) continue;
#if !__has_include(<defcmd/objpos_centre.hpp>)
        if (c.three_d) {
            std::printf("%-24s 3D store needs p67-1 (140163340, 1402009f0): skipped\n", c.stem);
            continue;
        }
#endif
        current = c.stem;
        const cmdoracle::Result res = r.run(c.stem, o);
        std::printf("%-24s %s  records: %d equal, %d differ, %d baseline gaps\n%s", c.stem, res.pass ? "PASS" : "FAIL",
                    res.equal, res.differ, res.gaps, res.report.c_str());
        if (res.pass) ++ok;
        else ++bad;
    }
    def::cmd::gfread::ext::g_load_3d_hook = nullptr;
    std::printf("GFREAD GEO oracle: %d pass, %d fail\n", ok, bad);
    fs::remove_all(mine, ec);
    fs::remove_all(work, ec);
    CHECK_EQ(bad, 0);
#endif
}

// With the p67-gate runner: every case by its label through def::cmd::dispatch, plus the
// edge-count shrink case gfread_igs_multicurve (IGES into an object with 8 curves; needs p8-03, p8-04).
TEST_CASE(gfread_cases_by_label)
{
#ifndef P809_NEW_RUNNER
    std::printf("shared runner without inline inputs: skipped\n");
#else
    keytest::legacy_printf();
    cmdoracle::Runner r;
    if (!r.ok()) {
        std::printf("no oracle cache: skipped\n");
        return;
    }
#if __has_include(<defrt/stdlib/stdio_compat.hpp>)
    def::rt::init_stdio_compat();
#endif
    std::vector<std::string> stems;
    for (const CaseDef& c : kCases) stems.push_back(c.stem);
#if __has_include(<defcmd/iges.hpp>) && __has_include(<defcmd/dxf.hpp>)
    stems.push_back("gfread_igs_multicurve");
#else
    std::printf("gfread_igs_multicurve     needs p8-03 (IGES) and p8-04 (the DXF stack object in 140244500): skipped\n");
#endif
    cmdoracle::Options o;
    o.message = [](const std::string& l) { return l.rfind("INFO   : Loading geometry", 0) == 0 || l.rfind("ERROR", 0) == 0; };
    o.reset = [] { gl::reset_token_reader(); };
#if !__has_include(<defcmd/stl_nas.hpp>)
    def::cmd::gfread::ext::g_load_3d_hook = load_3d_geo_only;  // until p8-02 is merged
#endif
    const fs::path dir = testkit::repo_root() / "tests" / "commands" / "GFREAD";
    int ok = 0, bad = 0, skipped = 0;
    for (const std::string& stem : stems) {
        const std::string label = cmdoracle::json_string(cmdoracle::slurp(dir / (stem + ".json")), "label");
        const cmdoracle::Result res = r.run(label, o);
        if (!res.found) {
            std::printf("%-24s no oracle entry: skipped\n", stem.c_str());
            ++skipped;
            continue;
        }
        std::printf("%-24s %s  records: %d equal, %d differ, %d baseline gaps\n%s", stem.c_str(), res.pass ? "PASS" : "FAIL",
                    res.equal, res.differ, res.gaps, res.report.c_str());
        if (res.pass) ++ok;
        else ++bad;
    }
    def::cmd::gfread::ext::g_load_3d_hook = nullptr;
    std::printf("GFREAD oracle (by label): %d pass, %d fail, %d skipped\n", ok, bad, skipped);
    CHECK_EQ(bad, 0);
#endif
}

int main() { return testkit::run_all(); }
