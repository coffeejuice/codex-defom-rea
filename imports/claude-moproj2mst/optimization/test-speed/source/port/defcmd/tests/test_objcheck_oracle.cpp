// Oracle check of packet p4-03 (GENDB material / inter-material / object checks) over the cached
// runs of `KFREAD <key> / GENDB OUT.DB` for every corpus KEY file (cases "gendb/<path>", made by
// tools/oracle/p4_03_gendb.py; cache %USERPROFILE%\.deform-oracle or $DEFORM_ORACLE_CACHE).
//
// For each cached case: a copy of the KEY file is read through the KEY loop with all keyword
// group parsers into a DB_STEP_DATA(3); then check_material_data, check_inter_material_data and
// check_object_data run with stdout captured. Their text must equal DEF_PRE's stdout from
// "INFO   : Checking material properties" up to "INFO   : Checking inter-object data".
//
// It needs the KEY reader packets (claude/kw-merge); on a branch without them the test passes with
// a note. Calls of the checks into model code that is merged there but that defdbio cannot link
// (defkey, defmodel functions not yet wired into the thin interface) go through the ext hook.
// OBJCHECK_MAX_FILES caps the cases, OBJCHECK_VERBOSE=1 prints every mismatch,
// OBJCHECK_ONLY=<substring> selects cases by label.
// ctest-labels: p4-03 objcheck oracle
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <testkit/check.hpp>

#if __has_include(<defkey/kw_g2obj.hpp>) && __has_include(<defkey/kw_g57.hpp>) && \
    __has_include(<defmodel/bc/node_arrays.hpp>) && __has_include(<defmodel/geo_kw/mesh_model.hpp>)
#define OBJCHECK_ORACLE 1
#endif

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#ifdef OBJCHECK_ORACLE
#include <io.h>
#include <process.h>

#include <defdbio/objcheck/objcheck.hpp>
#include <defkey/group1.hpp>
#include <defkey/key_reader.hpp>
#include <defkey/kw_g2obj.hpp>
#include <defkey/kw_g3/kw_g3.hpp>
#include <defkey/kw_g4.hpp>
#include <defkey/kw_g57.hpp>
#include <defrt/ftn/unit_system.hpp>
#include <defrt/ftn/units.hpp>
#include <defdbio/gendb/ext.hpp>
#include <defmodel/bc/boundary_topology.hpp>
#include <defmodel/bc/node_arrays.hpp>
#include <defmodel/geo_kw/geo_kw.hpp>
#include <defmodel/geo_kw/mesh_model.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defrt/stdlib/stdio_compat.hpp>

#include "../../defdbio/src/objcheck/ext.hpp"
#endif

namespace fs = std::filesystem;

namespace {

std::string env(const char* name, const char* dflt)
{
    const char* v = std::getenv(name);
    return v != nullptr ? std::string(v) : std::string(dflt);
}

#ifdef OBJCHECK_ORACLE
using def::model::DB_STEP_DATA;
using def::model::LLOBJ_DATA;

std::string slurp(const fs::path& p)
{
    std::ifstream in(p, std::ios::binary);
    std::stringstream ss;
    ss << in.rdbuf();
    return ss.str();
}

// Tiny JSON field reader (result.json is written by def_pre_oracle.py, one field per line).
std::string json_str(const std::string& j, const std::string& key, std::size_t from = 0)
{
    std::size_t k = j.find("\"" + key + "\"", from);
    if (k == std::string::npos) return {};
    std::size_t q = j.find('"', j.find(':', k) + 1);
    std::string out;
    for (std::size_t i = q + 1; i < j.size() && j[i] != '"'; ++i) {
        if (j[i] == '\\' && i + 1 < j.size()) ++i;
        out += j[i];
    }
    return out;
}

struct AllGroups : def::key::KeyReaderHooks {
    DB_STEP_DATA& m;
    explicit AllGroups(DB_STEP_DATA& model) : m(model) {}
    bool has_sub_model() override { return false; }
    void set_model_dimension(int dim) override { m.set_dimension(&dim); }
    void set_sub_model_dimension(int) override {}
    int parse_keyword(int group, std::FILE* f, int id) override
    {
        switch (group) {
        case 1: return def::key::read_group1(&m, f, id);
        case 2: return def::key::g2obj::read_object_keyword(m, f, id);
        case 3: return def::key::g3::read_keyword(&m, f, id);
        case 4: return def::key::g4::read_keyword(m, f, id);
        case 5: return def::key::g57::read_phase_keyword(&m, f, id);
        case 7: return def::key::g57::read_dim_keyword(&m, f, id);
        default: {
            char line[0x400];
            std::fgets(line, sizeof line, f);
            return 0;
        }
        }
    }
};

// Model functions the checks reach that are merged on this branch but not yet behind the thin
// interface's defaults.
bool wiring(std::uint64_t addr, const std::uint64_t* a, int, std::uint64_t* r)
{
    auto obj = [&](int i) -> LLOBJ_DATA& { return *reinterpret_cast<LLOBJ_DATA*>(a[i]); };
    switch (addr) {
    case 0x140160d20: *r = def::model::geo_kw::curve_count(obj(0)) > 0; return true;
    case 0x140165170: *r = static_cast<std::uint32_t>(def::model::bc::edge_face_slot_count(obj(0))); return true;
    case 0x1401fb270:
        def::model::geo_kw::material_groups(obj(0), reinterpret_cast<std::int32_t*>(a[1]),
                                            reinterpret_cast<std::int32_t*>(a[2]));
        return true;
    case 0x14017ffe0:
        def::model::bc::node_set_alloc(reinterpret_cast<void*>(a[0]), static_cast<std::int32_t>(a[1]),
                                       reinterpret_cast<const std::int32_t*>(a[2]), static_cast<std::int32_t>(a[3]));
        return true;
    case 0x1401836c0:
        def::model::bc::element_set_alloc(reinterpret_cast<void*>(a[0]), static_cast<std::int32_t>(a[1]),
                                          reinterpret_cast<const std::int32_t*>(a[2]), static_cast<std::int32_t>(a[3]),
                                          static_cast<std::int32_t>(a[4]));
        return true;
    case 0x1401d48d0: return true;   // state-variable table setup of the owner (P2); widths are set by the reader
    default: return false;
    }
}

struct Capture {
    fs::path path;
    int saved = -1;
    std::FILE* f = nullptr;
    Capture()
    {
        path = fs::temp_directory_path() / (std::to_string(::_getpid()) + "_objcheck_oracle_out.txt");
        std::fflush(stdout);
        saved = ::_dup(::_fileno(stdout));
        f = std::fopen(path.string().c_str(), "wb");
        ::_dup2(::_fileno(f), ::_fileno(stdout));
    }
    std::string done()
    {
        std::fflush(stdout);
        ::_dup2(saved, ::_fileno(stdout));
        ::_close(saved);
        std::fclose(f);
        std::string s = slurp(path);
        std::error_code ec;
        fs::remove(path, ec);
        return s;
    }
};

std::string crlf_to_lf(const std::string& s)
{
    std::string o;
    for (std::size_t i = 0; i < s.size(); ++i)
        if (!(s[i] == '\r' && i + 1 < s.size() && s[i + 1] == '\n')) o += s[i];
    return o;
}

struct Totals {
    int cases = 0, equal = 0, differ = 0, no_section = 0;
    int partial = 0, partial_equal = 0;            // the KEY loop reported keyword errors
    std::map<std::string, int> unread;             // keyword -> cases where the port's reader failed
};

void check_case(const fs::path& key, const std::string& oracle_stdout, const std::string& label, Totals& t,
                bool verbose)
{
    const std::string out = crlf_to_lf(oracle_stdout);
    const std::string a = "INFO   : Checking material properties\n";
    const std::string b = "INFO   : Checking inter-object data\n";
    std::size_t i = out.find(a);
    std::size_t j = i == std::string::npos ? i : out.find(b, i);
    if (i == std::string::npos || j == std::string::npos) {
        ++t.no_section;
        return;
    }
    const std::string want = out.substr(i, j - i);

    fs::path copy = fs::temp_directory_path() / (std::to_string(::_getpid()) + "_objcheck_oracle_in.key");
    fs::copy_file(key, copy, fs::copy_options::overwrite_existing);
    // each original run is a fresh process: the unit flag (UNIT -> 140235c30) starts at SI
    def::rt::ftn::reset_unit_globals();
    def::rt::ftn::g_conv.english = 0;
    DB_STEP_DATA& m = *new DB_STEP_DATA(3);   // leaked on purpose, as the KEY reader tests do
    std::FILE* in = std::fopen(copy.string().c_str(), "r");
    fs::path logp = fs::temp_directory_path() / (std::to_string(::_getpid()) + "_objcheck_oracle_log.txt");
    std::FILE* sink = std::fopen(logp.string().c_str(), "wb");
    AllGroups hooks(m);
    def::key::read_key_stream(in, hooks, sink);
    std::fclose(in);
    std::fclose(sink);
    // Keywords the port's reader could not read (packets not merged yet) leave the model
    // incomplete; DEF_PRE read the same file without those errors (its stdout does not show them).
    std::vector<std::string> bad;
    {
        std::istringstream log(crlf_to_lf(slurp(logp)));
        std::string ln;
        while (std::getline(log, ln)) {
            char kw[64] = {0};
            if (std::sscanf(ln.c_str(), "ERROR: Keyword %63s", kw) == 1 && out.find(ln) == std::string::npos)
                bad.push_back(kw);
        }
        std::error_code ec;
        fs::remove(logp, ec);
    }

    // Stand-in for the one model change the simulation-control check (p4-04, 1401ee940) makes
    // before these checks run: deformation on and PDIE(1) == 0 -> PDIE(1) = 1.
    {
        auto* sc = reinterpret_cast<std::int32_t*>(reinterpret_cast<char*>(&m) + 0x750);
        if (sc[0x108 / 4] == 1 && sc[0x168 / 4] == 0) sc[0x168 / 4] = 1;
    }
    // GENDB (1401bb550) runs the input counts 1401bc230 first: they clear and set the materials'
    // used flag (+4) and group flags (+8) that the material check reads (14029b5c0, 14015ff30)
    {
        std::int32_t cerr = 0;
        Capture c0;
        def::dbio::gendb::ext::input_counts(m, &cerr);
        c0.done();
    }
    std::int32_t err = 0, warn = 0;
    Capture cap;
    def::dbio::objcheck::check_material_data(&m, nullptr, &err, &warn);
    def::dbio::objcheck::check_inter_material_data(&m, nullptr, &err, &warn);
    def::dbio::objcheck::check_object_data(&m, nullptr, &err, &warn);
    const std::string got = cap.done();
    if (!bad.empty()) {
        ++t.partial;
        if (got == want) ++t.partial_equal;
        for (const std::string& k : bad) ++t.unread[k];
        return;
    }
    ++t.cases;
    if (got == want) {
        ++t.equal;
    } else {
        ++t.differ;
        if (verbose) std::printf("DIFF %s\n--- oracle\n%s--- port\n%s\n", label.c_str(), want.c_str(), got.c_str());
        else std::printf("DIFF %s\n", label.c_str());
    }
    std::error_code ec;
    fs::remove(copy, ec);
}
#endif

} // namespace

TEST_CASE(gendb_checks_match_oracle)
{
#ifndef OBJCHECK_ORACLE
    std::printf("KEY reader packets not on this branch: skipped\n");
#else
    def::rt::init_stdio_compat();
    fs::path cache = env("DEFORM_ORACLE_CACHE", "");
    if (cache.empty()) cache = fs::path(env("USERPROFILE", "")) / ".deform-oracle";
    if (!fs::exists(cache / "entries")) {
        std::printf("no oracle cache at %s: skipped\n", cache.string().c_str());
        return;
    }
    const int max_files = std::atoi(env("OBJCHECK_MAX_FILES", "100000").c_str());
    const bool verbose = env("OBJCHECK_VERBOSE", "0") == "1";
    const std::string only = env("OBJCHECK_ONLY", "");
    def::dbio::objcheck::ext::g_hook = &wiring;
    Totals t;
    std::map<std::string, std::pair<fs::path, fs::path>> cases;   // label -> (source, entry dir)
    for (auto& d1 : fs::directory_iterator(cache / "entries")) {
        if (!d1.is_directory()) continue;
        for (auto& d2 : fs::directory_iterator(d1.path())) {
            fs::path rj = d2.path() / "result.json";
            if (!fs::exists(rj)) continue;
            std::string j = slurp(rj);
            std::string label = json_str(j, "label");
            if (label.rfind("gendb/key/", 0) != 0) continue;
            if (!only.empty() && label.find(only) == std::string::npos) continue;
            std::string src = json_str(j, "source", j.find("\"inputs\""));
            if (src.empty() || !fs::exists(src)) continue;
            cases[label] = {src, d2.path()};
        }
    }
    for (auto& [label, c] : cases) {
        if (t.cases + t.partial >= max_files) break;
        check_case(c.first, slurp(c.second / "stdout.txt"), label, t, verbose);
    }
    def::dbio::objcheck::ext::g_hook = nullptr;
    std::printf("gendb checks: %d complete models, %d equal, %d differ; %d without the check section; "
                "%d models the port's KEY reader could not fully read (%d of them equal anyway)\n",
                t.cases, t.equal, t.differ, t.no_section, t.partial, t.partial_equal);
    for (auto& [k, n] : t.unread) std::printf("  unread keyword %s in %d cases\n", k.c_str(), n);
    CHECK_EQ(t.differ, 0);
#endif
}

int main() { return testkit::run_all(); }
