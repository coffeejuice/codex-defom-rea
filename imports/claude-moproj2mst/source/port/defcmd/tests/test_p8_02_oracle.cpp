// ctest-labels: p8-02
// p8-02 oracle replays: the GFREAD STL and NASTRAN cases of tests/commands/GFREAD (gfread_3d_stl,
// gfread_stl_* and gfread_nas_*) through cmdoracle::Runner (cmd_oracle_runner.hpp from
// claude/p67-gate @21cd8f1) and the port's 3D table def::cmd::stlnas::fun_140243a70.
//
// GFREAD itself (dispatcher case, FUN_1401aa9a0, FUN_140243830) is p8-01's: the Options::command
// callback is a test copy of its 3D path, so the cases exercise p8-02's code only.
// Small helpers of this file (the runner lacks them):
//   * case_label(): the cache holds some of these cases under an older label than the case file
//     (the oracle reuses an entry by its input hash); the label is looked up by the inputs.
//   * baseline gaps: the runner looks for "_baseline/<seed stem>" and these cases call the seed
//     IN.KEY; the gaps are taken from running "_baseline/UPSET_Brick" through the runner (by
//     keyword: a keyword that differs for an object of the seed alone, such as HDNEST, which no port
//     writer emits yet, is also a gap for the GFREAD object).
//   * DEF_STL.OUT: the original deletes it right after writing it and it carries GetTickCount
//     times, so the oracle has no copy; the 1401b85d0 seam is hooked to capture the port's file
//     before the deletion and its fixed lines are checked.
//   * gfread_stl_alldegen ends the process with exit(1): it runs in a child process.
// The STL bounds message prints three undefined numbers in the original (no arguments passed);
// its dx/dy/dz lines are not compared.
//
// Needs the owners' code (p8-09 geolib, p9-02, p67-1/2/3/5, P4 gate) and the dispatcher header
// the runner includes (p5-01): without them the test reports "skipped" and passes.
#define _CRT_SECURE_NO_WARNINGS
#include <testkit/check.hpp>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <set>
#include <string>
#include <vector>

#include "../src/p8_02/ext_p67.hpp"
#include "../src/p8_02/ext_p8_09.hpp"

#if __has_include(<defcmd/dispatch.hpp>) && defined(DEF_P802_HAS_CUBE) && defined(DEF_P802_HAS_P671) && \
    defined(DEF_P802_HAS_P673) && defined(DEF_P802_HAS_OBJ3D) && defined(DEF_P802_HAS_P675) &&            \
    defined(DEF_P802_HAS_GEOLIB)
#define P802_ORACLE 1
#endif

#if defined(P802_ORACLE)
#include "cmd_oracle_runner.hpp"

#include <defcmd/stl_nas.hpp>
#include <defdbio/step/db_info_read.hpp>
#include <defkey/kw_g2obj.hpp>
#include <defkey/line_io.hpp>
#include <defmodel/geo_kw/geo_kw.hpp>
#include <defmodel/llobj_ctor/llobj_ctor.hpp>
#include <defmodel/strtrim.hpp>

namespace fs = std::filesystem;
using def::model::DB_STEP_DATA;
using def::model::LLOBJ_DATA;

namespace {

std::string g_stl_out;  // DEF_STL.OUT as written by the port (captured before the deletion)
bool g_stl_out_seen = false;

bool capture_delete(const char* name)
{
    if (std::strcmp(name, "DEF_STL.OUT") == 0) {
        g_stl_out.clear();
        for (char ch : cmdoracle::slurp(name))  // written in text mode: CRLF -> LF
            if (ch != 13) g_stl_out += ch;
        g_stl_out_seen = true;
    }
    return def::cmd::stlnas::ext::default_1401b85d0(name);
}

void reset()
{
    def::cmd::stlnas::ext::reset_ext_p67();
    def::cmd::stlnas::ext::reset_ext_p8_09();
    def::cmd::stlnas::ext::fun_1401b85d0 = &capture_delete;
    def::cmd::geolib::reset_token_reader();
}

// FUN_140243500 (p8-01): extension -> type code
std::int32_t type_of(const std::string& file)
{
    std::string ext = file.substr(file.rfind('.') + 1);
    for (auto& c : ext) c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
    if (ext == "GEO") return 2;
    if (ext == "STL") return 3;
    if (ext == "PDA" || ext == "PAT") return 4;
    if (ext == "UNV") return 5;
    if (ext == "NAS") return 10;
    return -1;
}

// test copy of p8-01's GFREAD (cmd_gfread, FUN_1401aa9a0, FUN_140243830) for 3D objects
int gfread(DB_STEP_DATA& m, std::FILE* f)
{
    std::int32_t id = 0;
    if (def::key::read_i(f, &id, false) != 0) return 2;
    if (def::model::llobj_ctor::get_or_create(&m, id) == nullptr) return 2;
    char name[0x400];
    if (std::fgets(name, 0x400, f) == nullptr) return 2;
    def::model::trim_right_blanks(name);
    LLOBJ_DATA* obj = def::key::g2obj::find_object(m, id);
    if (obj == nullptr) return 0;
    if (!def::dbio::file_exists(name)) {
        std::fprintf(stdout, "ERROR  : Reading GFREAD\n");
        std::fprintf(stdout, "         %s does not exist.\n", name);
        return 0;
    }
    const def::cmd::stlnas::LoaderView loader{1.0E-5, 1.0, nullptr};
    const std::int32_t type = type_of(name);
    bool ok = false;
    if (type != -1 && def::model::geo_kw::object_dim(*obj) == 3) {
        ok = def::cmd::stlnas::fun_140243a70(&loader, name, obj, type);
        if (ok) *reinterpret_cast<std::int32_t*>(reinterpret_cast<unsigned char*>(obj) + 0x3a40) = type;
    }
    std::fprintf(stdout, ok ? "INFO   : Loading geometry - Done\n" : "INFO   : Loading geometry - Failed\n");
    return 0;
}

bool message_line(const std::string& l)
{
    if (l.rfind("\tdx :", 0) == 0 || l.rfind("\tdy :", 0) == 0 || l.rfind("\tdz :", 0) == 0) return false;
    return l.rfind("INFO   :", 0) == 0 || l.rfind("ERROR", 0) == 0 || l.rfind("*** ", 0) == 0 ||
           l.rfind("defCELL", 0) == 0 || l.rfind("defFPT", 0) == 0 || (!l.empty() && l[0] == '\t');
}

cmdoracle::Options options()
{
    cmdoracle::Options o;
    o.command = [](DB_STEP_DATA& m, std::FILE* f, const std::string& n) { return n == "GFREAD" ? gfread(m, f) : -1; };
    o.message = &message_line;
    o.reset = &reset;
    return o;
}

// the label of the newest cache entry whose inputs (names, inline texts, file names) are the case's
std::string case_label(const std::string& id, fs::path* dir = nullptr)
{
    const fs::path p = testkit::repo_root() / "tests" / "commands" / "GFREAD" / (id + ".json");
    const std::vector<cmdoracle::CaseInput> want = cmdoracle::case_inputs(cmdoracle::slurp(p));
    std::string cache = cmdoracle::env("DEFORM_ORACLE_CACHE", "");
    if (cache.empty()) cache = (fs::path(cmdoracle::env("USERPROFILE", "")) / ".deform-oracle").string();
    const fs::path root = fs::path(cache) / "entries";
    if (!fs::exists(root) || want.empty()) return {};
    std::string best, best_created;
    for (auto& d1 : fs::directory_iterator(root)) {
        if (!d1.is_directory()) continue;
        for (auto& d2 : fs::directory_iterator(d1.path())) {
            const fs::path rj = d2.path() / "result.json";
            if (!fs::exists(rj)) continue;
            const std::string j = cmdoracle::slurp(rj);
            const std::vector<cmdoracle::CaseInput> have = cmdoracle::case_inputs(j);
            if (have.size() != want.size()) continue;
            bool same = true;
            for (std::size_t i = 0; i < want.size() && same; ++i) {
                same = have[i].name == want[i].name && have[i].text == want[i].text &&
                       fs::path(have[i].source).filename() == fs::path(want[i].source).filename();
            }
            const std::string created = cmdoracle::json_string(j, "created");
            if (same && (best.empty() || created > best_created)) {
                best = cmdoracle::json_string(j, "label");
                best_created = created;
                if (dir != nullptr) *dir = d2.path();
            }
        }
    }
    return best;
}

// "  record KW object N differs" lines of a runner report
std::set<std::pair<int, std::string>> differing(const std::string& report)
{
    std::set<std::pair<int, std::string>> s;
    for (const std::string& l : cmdoracle::lines_of(report)) {
        char kw[64] = {0};
        int obj = 0;
        if (std::sscanf(l.c_str(), "  record %63s object %d differs", kw, &obj) == 2) s.insert({obj, kw});
    }
    return s;
}

const cmdoracle::Runner& runner()
{
    static cmdoracle::Runner r;
    return r;
}

std::set<std::string> baseline_gap_keywords()
{
    static std::set<std::string> kws;
    static bool done = false;
    if (!done) {
        done = true;
        const cmdoracle::Result b = runner().run("_baseline/UPSET_Brick", options());
        for (auto& k : differing(b.report)) kws.insert(k.second);
    }
    return kws;
}

// replays the case; true when the messages agree and every differing record is a baseline gap
bool run_case(const std::string& id, bool expect_stl_out)
{
    const std::string label = case_label(id);
    if (label.empty() || !runner().ok()) {
        std::printf("  %s: no oracle entry, skipped\n", id.c_str());
        return true;
    }
    const std::set<std::string> gap_kws = baseline_gap_keywords();
    const cmdoracle::Result res = runner().run(label, options());
    const std::string stl_out = g_stl_out;
    const bool stl_out_seen = g_stl_out_seen;
    CHECK(res.found);
    bool ok = res.report.find("messages differ") == std::string::npos;
    std::string fails;
    for (auto& k : differing(res.report)) {
        if (gap_kws.count(k.second) != 0) continue;
        ok = false;
        fails += "  record " + k.second + " object " + std::to_string(k.first) + " differs\n";
    }
    if (!ok) testkit::fail(__FILE__, __LINE__, id + " (" + label + "):\n" + res.report + fails);
    if (expect_stl_out) {
        CHECK(stl_out_seen);
        CHECK(stl_out.rfind("\t...Removing extra points   \n\t...Building surface links  (", 0) == 0);
        CHECK(stl_out.find("\n\tSurface information\n") != std::string::npos);
        CHECK(stl_out.find("\tRecursion depth limit         : 1000\n") != std::string::npos);
        CHECK(!fs::exists("DEF_STL.OUT"));
    }
    std::printf("  %s: %s (%d records equal, %d differing, %d baseline gaps)\n", id.c_str(), ok ? "pass" : "FAIL",
                res.equal, res.differ, res.gaps);
    return ok;
}

std::string g_exe;

}  // namespace

TEST_CASE(stl_cases)
{
    run_case("gfread_3d_stl", true);
    run_case("gfread_stl_ascii_merge", true);
    run_case("gfread_stl_binary_merge", true);
    run_case("gfread_stl_bucket_edge", true);
    run_case("gfread_stl_bigbounds", false);
    run_case("gfread_stl_bigbounds_noname", false);
}

TEST_CASE(stl_ascii_merge_def_stl_out)
{
    if (!run_case("gfread_stl_ascii_merge", true)) return;
    if (!g_stl_out_seen) return;
    // T3 of the case is reversed by the orientation pass (spec 2.7 and the case notes)
    CHECK(g_stl_out.find("\tPoly with invalid orientation : 1\n") != std::string::npos);
    CHECK(g_stl_out.find("\nBadly oriented polygons which were reversed\n") != std::string::npos);
    CHECK(g_stl_out.find("\tNumber of polys               : 6\n") != std::string::npos);
    CHECK(g_stl_out.find("\tNumber of points              : 10\n") != std::string::npos);
    CHECK(g_stl_out.find("Surf patches\nPatch      1  NoCurves ") != std::string::npos);
}

TEST_CASE(stl_alldegen_exits_1)
{
    fs::path entry;
    const std::string label = case_label("gfread_stl_alldegen", &entry);
    if (label.empty()) return;
    const std::string cmd = "\"\"" + g_exe + "\" --child gfread_stl_alldegen\"";
    const int rc = std::system(cmd.c_str());
    CHECK_EQ(rc, 1);
    // the child's stdout capture: the runner's per-process file (child pid + case tag in its name)
    fs::path cap;
    for (auto& e : fs::directory_iterator(fs::temp_directory_path())) {
        const std::string n = e.path().filename().string();
        if (n.find("_cmdoracle_stdout_") != std::string::npos && n.find("all_triangles_degenerate") != std::string::npos &&
            (cap.empty() || fs::last_write_time(e.path()) > fs::last_write_time(cap)))
            cap = e.path();
    }
    REQUIRE(!cap.empty());
    std::vector<std::string> pm, om;
    for (auto& l : cmdoracle::lines_of(cmdoracle::slurp(cap)))
        if (message_line(l)) pm.push_back(l);
    for (auto& l : cmdoracle::lines_of(cmdoracle::slurp(entry / "stdout.txt")))
        if (message_line(l)) om.push_back(l);
    std::error_code ec;
    fs::remove(cap, ec);
    if (pm != om) {
        std::string s = "alldegen: messages differ\n";
        for (auto& l : om) s += "    orig: " + l + "\n";
        for (auto& l : pm) s += "    port: " + l + "\n";
        testkit::fail(__FILE__, __LINE__, s);
    }
    CHECK(!pm.empty());
    std::printf("  gfread_stl_alldegen: exit code %d, %zu message lines\n", rc, pm.size());
}

TEST_CASE(nas_cases)
{
    run_case("gfread_nas_tet_fixed", false);
    run_case("gfread_nas_tet_free", false);
    run_case("gfread_nas_hex", false);
    run_case("gfread_nas_tria", false);
    run_case("gfread_nas_none", false);
}

int main(int argc, char** argv)
{
    g_exe = argv[0];
    if (argc >= 3 && std::strcmp(argv[1], "--child") == 0) {
        (void)runner().run(case_label(argv[2]), options());  // exits with code 1 inside
        return 0;
    }
    return testkit::run_all();
}
#else
TEST_CASE(skipped_without_owners)
{
    std::printf("p8-02 oracle replays skipped: the owners' headers (p8-09, p67-1/2/3/5, p5-01 dispatch) are not in "
                "this build\n");
}

int main() { return testkit::run_all(); }
#endif
