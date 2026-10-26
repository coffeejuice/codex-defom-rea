// ctest-labels: p4-04 gendb oracle
// Oracle check of packet p4-04's GENDB checks over the cached gendb/<KEY> runs of the original
// DEF_PRE (CMD.KEY = "KFREAD <key> / GENDB OUT.DB"; cache %USERPROFILE%\.deform-oracle or
// $DEFORM_ORACLE_CACHE).
//
// For each cached run whose stdout reached the check chain: a copy of the input KEY is read into
// a fresh DB_STEP_DATA(3) through the KEY loop with the merged group parsers (groups 1-5, 7),
// then the check chain 1401bebf0 runs with stdout captured. The lines of check 1 (from "INFO   :
// Checking simulation controls" up to "INFO   : Checking material properties") and of check 5
// (from "INFO   : Checking inter-object data" up to the next "INFO   :" line) must equal the
// original's. Checks 2-4 are p4-03's and are not compared.
//
// Known gap until p4-02b lands: the input check 1401bc230 that GENDB runs first is not ported;
// the one value check 5 needs from it, the object count at model+0x7c, is set here to the
// largest object number. Runs where 1401bc230 already failed print no check output and are
// skipped. P404_ORACLE_MAX (default all) caps the number of runs, P404_ORACLE_VERBOSE=1 prints
// every mismatch.
#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include <io.h>
#include <process.h>

#include <defdbio/gendb/checks_p4_04.hpp>
#include <defkey/group1.hpp>
#include <defkey/key_reader.hpp>
#include <defkey/kw_g2obj.hpp>
#include <defkey/kw_g3/kw_g3.hpp>
#include <defkey/kw_g4.hpp>
#include <defkey/kw_g57.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/step/db_step.hpp>
#include <defrt/stdlib/stdio_compat.hpp>
#include <testkit/check.hpp>

namespace fs = std::filesystem;
using def::model::DB_STEP_DATA;
using def::model::LLOBJ_DATA;

namespace {

std::string env(const char* name, const char* def)
{
    const char* v = std::getenv(name);
    return v != nullptr && *v != '\0' ? v : def;
}

std::string slurp(const fs::path& p)
{
    std::ifstream in(p, std::ios::binary);
    std::ostringstream s;
    s << in.rdbuf();
    return s.str();
}

std::string tmp(const std::string& base)
{
    return (fs::temp_directory_path() / (std::to_string(_getpid()) + "_p4_04_" + base)).string();
}

// JSON string values of `key` in order, backslash escapes undone (enough for result.json)
std::vector<std::string> json_strings(const std::string& j, const char* key)
{
    std::vector<std::string> r;
    const std::string pat = std::string("\"") + key + "\": \"";
    for (std::size_t p = j.find(pat); p != std::string::npos; p = j.find(pat, p)) {
        p += pat.size();
        std::string out;
        while (p < j.size() && j[p] != '"') {
            if (j[p] == '\\' && p + 1 < j.size()) {
                ++p;
                out += j[p] == 'n' ? '\n' : j[p];
                ++p;
                continue;
            }
            out += j[p++];
        }
        r.push_back(out);
    }
    return r;
}

std::vector<std::string> lines_of(const std::string& s)
{
    std::vector<std::string> v;
    std::size_t a = 0;
    while (a < s.size()) {
        std::size_t b = s.find('\n', a);
        if (b == std::string::npos) b = s.size();
        std::string l = s.substr(a, b - a);
        if (!l.empty() && l.back() == '\r') l.pop_back();
        v.push_back(l);
        a = b + 1;
    }
    return v;
}

// lines after the first `start` line up to (not including) the first line accepted by stop
template <class Stop>
std::vector<std::string> section(const std::vector<std::string>& ls, const char* start, Stop stop, bool* found)
{
    std::vector<std::string> r;
    *found = false;
    std::size_t i = 0;
    for (; i < ls.size(); ++i)
        if (ls[i] == start) break;
    if (i == ls.size()) return r;
    *found = true;
    for (++i; i < ls.size() && !stop(ls[i]); ++i) r.push_back(ls[i]);
    return r;
}

std::vector<std::string> check1(const std::vector<std::string>& ls, bool* f)
{
    return section(ls, "INFO   : Checking simulation controls",
                   [](const std::string& l) { return l == "INFO   : Checking material properties"; }, f);
}

std::vector<std::string> check5(const std::vector<std::string>& ls, bool* f)
{
    return section(ls, "INFO   : Checking inter-object data",
                   [](const std::string& l) { return l.rfind("INFO   :", 0) == 0; }, f);
}

DB_STEP_DATA* parser_model(DB_STEP_DATA& m) // 1401d81b0
{
    return m.use_companion != 0 && m.companion != nullptr ? m.companion : &m;
}

struct Hooks : def::key::KeyReaderHooks {
    DB_STEP_DATA& m;
    explicit Hooks(DB_STEP_DATA& model) : m(model) {}
    bool has_sub_model() override { return false; }
    void set_model_dimension(int dim) override
    {
        const std::int32_t d = dim;
        m.set_dimension(&d);
    }
    void set_sub_model_dimension(int) override {}
    int parse_keyword(int group, std::FILE* f, int id) override
    {
        DB_STEP_DATA* p = parser_model(m);
        switch (group) {
        case 1: return def::key::read_group1(p, f, id);
        case 2: return def::key::g2obj::read_object_keyword(*p, f, id);
        case 3: return def::key::g3::read_keyword(p, f, id);
        case 4: return def::key::g4::read_keyword(*p, f, id);
        case 5: return def::key::g57::read_phase_keyword(p, f, id);
        case 7: return def::key::g57::read_dim_keyword(&m, f, id);
        default: return 0; // group 6: commands, not part of a model read
        }
    }
};

template <class F>
std::string capture(F f)
{
    const std::string path = tmp("stdout.txt");
    std::fflush(stdout);
    const int saved = _dup(_fileno(stdout));
    FILE* t = std::fopen(path.c_str(), "w");
    _dup2(_fileno(t), _fileno(stdout));
    f();
    std::fflush(stdout);
    _dup2(saved, _fileno(stdout));
    _close(saved);
    std::fclose(t);
    std::string s = slurp(path);
    std::error_code ec;
    fs::remove(path, ec);
    return s;
}

struct Totals {
    int runs = 0, skipped = 0, compared = 0, bad1 = 0, bad5 = 0;
};

void check_run(const fs::path& dir, Totals& tot, bool verbose)
{
    const std::string rj = slurp(dir / "result.json");
    const std::vector<std::string> names = json_strings(rj, "name");
    const std::vector<std::string> sources = json_strings(rj, "source");
    const std::vector<std::string> orig = lines_of(slurp(dir / "stdout.txt"));
    bool f1 = false, f5 = false;
    const std::vector<std::string> want1 = check1(orig, &f1);
    const std::vector<std::string> want5 = check5(orig, &f5);
    ++tot.runs;
    // one KEY input with a corpus source, and the chain reached in the original
    if (sources.size() != 1 || !f1 || !f5 || !fs::exists(sources[0])) {
        ++tot.skipped;
        return;
    }
    // the input is read from a copy (corpus files stay untouched)
    const std::string copy = tmp("input.key");
    fs::copy_file(sources[0], copy, fs::copy_options::overwrite_existing);
    DB_STEP_DATA& m = *new DB_STEP_DATA(3); // leaked on purpose, as in the keyword oracle tests
    std::FILE* in = std::fopen(copy.c_str(), "r");
    std::FILE* sink = std::fopen("NUL", "w");
    Hooks hooks(m);
    def::key::read_key_stream(in, hooks, sink);
    std::fclose(in);
    std::fclose(sink);
    std::error_code ec;
    fs::remove(copy, ec);

    DB_STEP_DATA& s = *parser_model(m);
    std::int32_t nobj = 0; // stand-in for 1401bc230's object count at +0x7c
    for (LLOBJ_DATA* o = s.objects.head; o != nullptr; o = o->next) {
        const std::int32_t n = def::model::field<std::int32_t>(o, 8);
        if (nobj < n) nobj = n;
    }
    s.u074[2] = nobj;
    std::int32_t err = 0, warn = 0;
    const std::vector<std::string> got = lines_of(capture([&] { def::dbio::gendb::check_chain(s, &err, &warn); }));
    bool g1 = false, g5 = false;
    const std::vector<std::string> got1 = check1(got, &g1);
    const std::vector<std::string> got5 = check5(got, &g5);
    ++tot.compared;
    const bool ok1 = got1 == want1, ok5 = got5 == want5;
    if (!ok1) ++tot.bad1;
    if (!ok5) ++tot.bad5;
    if (verbose && (!ok1 || !ok5)) {
        std::printf("MISMATCH %s (%s)\n", sources[0].c_str(), dir.filename().string().c_str());
        const auto dump = [](const char* what, const std::vector<std::string>& a, const std::vector<std::string>& b) {
            std::printf("  %s orig:\n", what);
            for (auto& l : a) std::printf("    |%s\n", l.c_str());
            std::printf("  %s port:\n", what);
            for (auto& l : b) std::printf("    |%s\n", l.c_str());
        };
        if (!ok1) dump("check 1", want1, got1);
        if (!ok5) dump("check 5", want5, got5);
    }
}

} // namespace

TEST_CASE(oracle_gendb_check_messages)
{
    def::rt::init_stdio_compat();
    fs::path cache = env("DEFORM_ORACLE_CACHE", "");
    if (cache.empty()) cache = fs::path(env("USERPROFILE", "")) / ".deform-oracle";
    if (!fs::exists(cache / "entries")) {
        std::printf("no oracle cache at %s: skipped\n", cache.string().c_str());
        return;
    }
    const int max_runs = std::atoi(env("P404_ORACLE_MAX", "100000").c_str());
    const bool verbose = env("P404_ORACLE_VERBOSE", "0") == "1";
    Totals tot;
    for (auto& d1 : fs::directory_iterator(cache / "entries")) {
        if (!d1.is_directory()) continue;
        for (auto& d2 : fs::directory_iterator(d1.path())) {
            const fs::path rj = d2.path() / "result.json";
            if (!fs::exists(rj) || tot.runs >= max_runs) continue;
            const std::string j = slurp(rj);
            if (j.find("\"label\": \"gendb/") == std::string::npos) continue;
            check_run(d2.path(), tot, verbose);
        }
    }
    std::printf("gendb runs %d, skipped %d, compared %d, check 1 mismatches %d, check 5 mismatches %d\n",
                tot.runs, tot.skipped, tot.compared, tot.bad1, tot.bad5);
    CHECK(tot.compared > 0);
    CHECK_EQ(tot.bad1, 0);
    CHECK_EQ(tot.bad5, 0);
}

int main() { return testkit::run_all(); }
