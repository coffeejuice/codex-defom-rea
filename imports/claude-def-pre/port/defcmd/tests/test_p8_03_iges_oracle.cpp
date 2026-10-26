// Oracle replay of the IGES reader (packet p8-03) over the cached GFREAD cases of
// tests/commands/GFREAD (gfread_2d_igs, gfread_igs_*; cache %USERPROFILE%\.deform-oracle or
// $DEFORM_ORACLE_CACHE). Per case: the inputs are written into a per-process temp directory, the
// command file runs through the KEY loop (KFREAD of the seed with the group parsers, KFWRIT with
// the group 2 writer), GFREAD is a stand-in for p8-01's handler 1401aa9a0 / 140244500 that calls
// fun_140409af0 with the arguments of the original. Compared: every group 2 record of OUT.KEY
// (records that differ on the seed alone are baseline gaps), the "INFO   : Loading geometry" and
// "INPSEC" lines, the exit code and stderr of the cases that end the process, and byte for byte
// every file the original left in its directory (DEF_IGS.OUT, *_TMP_SFTC), and that no other
// file is left. Replays through cmdoracle::Runner; the GFREAD stand-in records the files, exit
// code and stderr. CMD_ORACLE_VERBOSE=1 prints differing records, IGES_ORACLE_ONLY=<substring>
// selects cases.
// ctest-labels: p8-03 oracle
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <testkit/check.hpp>

#include "cmd_oracle_runner.hpp"

#include <direct.h>

#include <cstring>
#include <set>
#include <stdexcept>

#include <defcmd/iges.hpp>
#include <defmodel/geo_kw/geo_kw.hpp>
#include <defmodel/layout/geo2d_and_mesh2d.hpp>
#include <defmodel/layout/obj_geo2d.hpp>
#include <defrt/stdlib/stdio_compat.hpp>

#include "../src/iges/ext_p8_01.hpp"
#include "../src/iges/ext_p8_09.hpp"
#include "../src/iges/ftn_read.hpp"

namespace {

namespace fs = std::filesystem;
using cmdoracle::env;
using cmdoracle::slurp;
using def::model::DB_STEP_DATA;
using def::model::LLOBJ_DATA;

struct Exit {
    std::string err;
    int code;
};

struct Entry {
    fs::path dir;
    std::string json;
};

// the GFREAD IGES case labels of this repository (the shared cache also holds cases of other
// packets, e.g. p8-09's gfread_igs_multicurve, which need hooks this test does not have)
const std::set<std::string>& repo_labels()
{
    static const std::set<std::string> s = [] {
        std::set<std::string> r;
        for (auto& f : fs::directory_iterator(cmdoracle::Runner::repo_root() / "tests" / "commands" / "GFREAD"))
            if (f.path().extension() == ".json") {
                // every "label" of the file (gfread_igs_install.json holds ten cases)
                const std::string j = slurp(f.path());
                for (std::size_t p = j.find("\"label\""); p != std::string::npos; p = j.find("\"label\"", p + 1))
                    r.insert(cmdoracle::json_string(j, "label", p));
            }
        return r;
    }();
    return s;
}

// label -> oracle entry directory (newest run), for the files, exit code and stderr the runner
// does not compare
std::map<std::string, Entry> scan_cache(const fs::path& cache)
{
    std::map<std::string, Entry> m;
    for (auto& d1 : fs::directory_iterator(cache / "entries")) {
        if (!d1.is_directory()) continue;
        for (auto& d2 : fs::directory_iterator(d1.path())) {
            const fs::path rj = d2.path() / "result.json";
            if (!fs::exists(rj)) continue;
            std::string j = slurp(rj);
            const std::string label = cmdoracle::json_string(j, "label");
            if (label.rfind("GFREAD IGES", 0) != 0 || repo_labels().count(label) == 0) continue;
            auto it = m.find(label);
            if (it != m.end() && cmdoracle::json_string(j, "created") < cmdoracle::json_string(it->second.json, "created"))
                continue;
            m[label] = Entry{d2.path(), std::move(j)};
        }
    }
    return m;
}

// what the GFREAD stand-in observed in the case's working directory
struct CaseState {
    std::string err;
    int code = 0;
    std::map<std::string, std::string> files; // files GFREAD left (name -> bytes)
    bool stopped = false;                      // the process ended inside GFREAD
};
CaseState g_case;

std::set<std::string> dir_names()
{
    std::set<std::string> s;
    for (auto& e : fs::directory_iterator(fs::current_path())) s.insert(e.path().filename().string());
    return s;
}

// stand-in for the dispatcher case + 1401aa9a0 + 140243830 + 140244500 (p8-01)
int gfread(DB_STEP_DATA& m, std::FILE* f, const std::string& name)
{
    if (name != "GFREAD") return -1;
    char buf[0x400] = {};
    (void)std::fgets(buf, sizeof buf, f);
    const std::int32_t obj_no = std::atoi(buf);
    (void)std::fgets(buf, sizeof buf, f);
    std::string file = buf;
    while (!file.empty() && (file.back() == '\n' || file.back() == '\r' || file.back() == ' ')) file.pop_back();
    LLOBJ_DATA* obj = def::key::g2obj::find_object(m, obj_no);
    if (obj == nullptr) return 0;
    const std::set<std::string> before = dir_names();
    int rc = 0;
    try {
        def::cmd::iges::Uic2CObjectIgesFile iges(nullptr);
        double x = 0.0;
        const int r = def::cmd::iges::fun_140409af0(&iges, file.c_str(), "", obj, &x, 2, 0, 1, 300);
        if (r == 0) *reinterpret_cast<std::int32_t*>(reinterpret_cast<unsigned char*>(obj) + 0xbc8) = iges.u1c;
        std::fprintf(stdout, r == -1 ? "INFO   : Loading geometry - Failed\n" : "INFO   : Loading geometry - Done\n");
    } catch (const Exit& e) {
        g_case.err = e.err;
        g_case.code = e.code;
        g_case.stopped = true;
        rc = 99; // the process ends here: stop the KEY loop
    }
    for (const std::string& n : dir_names())
        if (before.count(n) == 0) g_case.files[n] = slurp(n);
    return rc;
}

void install_hooks()
{
#ifndef DEF_P803_HAVE_P801
    auto& h = def::cmd::iges::ext::g_p801;
    h.f140197bc0 = [](LLOBJ_DATA* o) -> std::int64_t {
        o->u3a60.clear();
        return 0;
    };
    h.f14019a650 = [](LLOBJ_DATA* o, std::int32_t n) { std::memcpy(reinterpret_cast<unsigned char*>(o) + 0xbc8, &n, 4); };
    h.f1404098d0 = [](LLOBJ_DATA* o, double v) { std::memcpy(reinterpret_cast<unsigned char*>(o) + 0x3a48, &v, 8); };
    h.f1404098f0 = [](const LLOBJ_DATA* o) {
        std::int32_t v;
        std::memcpy(&v, reinterpret_cast<const unsigned char*>(o) + 0x3a50, 4);
        return v;
    };
    h.f140409930 = [](const LLOBJ_DATA* o) {
        double v;
        std::memcpy(&v, reinterpret_cast<const unsigned char*>(o) + 0x3a58, 8);
        return v;
    };
    h.f14040cde0 = [](def::model::OBJ_GEO2D* c, double* d) { c->points = d; };
    h.f14040ce20 = [](def::cmd::iges::Uic2CObjectIgesFile* p, std::int32_t v) { p->u18 = v; };
    h.f14040ce40 = [](def::cmd::iges::Uic2CObjectIgesFile* p) { p->u18 = 1; };
    h.f14040ce60 = [](def::cmd::iges::Uic2CObjectIgesFile* p, std::int32_t v) { p->u1c = v; };
#endif
#ifndef DEF_P803_HAVE_P809
    auto& g = def::cmd::iges::ext::g_p809;
    g.f140198270 = [](LLOBJ_DATA* o) {
        def::model::geo_kw::ensure_geo_list(*o);
        def::model::geo_kw::geometry(*o, 0)->add_edge_curve();
    };
    g.f140198230 = [](LLOBJ_DATA* o, std::int32_t n) {
        while (def::model::geo_kw::curve_count(*o) < n) def::model::geo_kw::geometry(*o, 0)->add_edge_curve();
        if (def::model::geo_kw::curve_count(*o) > n) throw std::runtime_error("test hook 140198230: shrink not implemented");
    };
#endif
    def::cmd::iges::ftnrd::g_exit_hook = [](const std::string& b, int code) { throw Exit{b, code}; };
}

bool message_line(const std::string& l)
{
    // the INPSEC record is written straight to the descriptor behind a prompt line of the menu, so
    // it is checked apart (see the test case)
    return l.find("Loading geometry") != std::string::npos;
}

} // namespace

TEST_CASE(iges_cases_match_oracle)
{
    def::rt::init_stdio_compat();
    cmdoracle::Runner runner;
    if (!runner.ok()) {
        std::printf("no oracle cache: skipped\n");
        return;
    }
    fs::path cache = env("DEFORM_ORACLE_CACHE", "");
    if (cache.empty()) cache = fs::path(env("USERPROFILE", "")) / ".deform-oracle";
    install_hooks();
    const auto entries = scan_cache(cache);
    const std::string only = env("IGES_ORACLE_ONLY", "");
    cmdoracle::Options o;
    o.command = gfread;
    o.message = message_line;
    o.reset = [] {
        def::cmd::iges::reset_globals();
        g_case = CaseState{};
    };
    int pass = 0, fail = 0, n = 0;
    for (const auto& [label, e] : entries) {
        if (!only.empty() && label.find(only) == std::string::npos) continue;
        ++n;
        const cmdoracle::Result res = runner.run(label, o);
        std::string rep = res.report;
        if (!res.found) rep += "  no oracle entry\n";
        // exit code and stderr of the cases that end the process
        const std::size_t pe = e.json.find("\"exit_code\": ");
        const long long oc = pe == std::string::npos ? 0 : std::atoll(e.json.c_str() + pe + 13);
        const int want_code = static_cast<int>(static_cast<std::uint32_t>(oc));
        if (want_code != g_case.code) rep += "  exit code " + std::to_string(g_case.code) + " != " + std::to_string(want_code) + "\n";
        const bool o_inpsec = slurp(e.dir / "stdout.txt").find(" INPSEC : STOPPING\r\n") != std::string::npos;
        const bool p_inpsec = g_case.stopped && g_case.code == 0 && g_case.err.empty();
        if (o_inpsec != p_inpsec) rep += "  INPSEC : STOPPING differs\n";
        const std::string oerr = fs::exists(e.dir / "stderr.txt") ? slurp(e.dir / "stderr.txt") : std::string();
        if (oerr != g_case.err) rep += "  stderr differs: [" + g_case.err + "]\n";
        // files left by GFREAD, byte for byte (NEWEQ.MSG is main's unit 1, OUT.KEY is compared above)
        std::map<std::string, std::string> ofiles;
        if (fs::exists(e.dir / "files"))
            for (auto& f : fs::directory_iterator(e.dir / "files")) {
                const std::string nme = f.path().filename().string();
                if (nme != "OUT.KEY" && nme != "NEWEQ.MSG") ofiles[nme] = slurp(f.path());
            }
        if (ofiles != g_case.files) {
            rep += "  left-over files differ: orig";
            for (auto& [k, v] : ofiles) rep += " " + k + "(" + std::to_string(v.size()) + ")";
            rep += " / port";
            for (auto& [k, v] : g_case.files) rep += " " + k + "(" + std::to_string(v.size()) + ")";
            rep += "\n";
        }
        if (rep.empty() && res.pass) {
            ++pass;
            std::printf("PASS %s\n", label.c_str());
        } else {
            ++fail;
            std::printf("FAIL %s\n%s", label.c_str(), rep.c_str());
        }
    }
    std::printf("%d cases: %d pass, %d fail\n", n, pass, fail);
    CHECK(n > 0);
    CHECK_EQ(fail, 0);
}

int main() { return testkit::run_all(); }
