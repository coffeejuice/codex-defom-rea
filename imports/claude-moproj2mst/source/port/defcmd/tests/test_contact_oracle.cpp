// ctest-labels: p67-6 contact oracle
// Oracle check of INICTC and 2D GENCTC over the cached runs of tests/commands/INICTC and
// tests/commands/GENCTC (2D cases on SPIKE_SI.KEY; cache %USERPROFILE%/.deform-oracle or
// $DEFORM_ORACLE_CACHE, filled by tools/oracle/def_pre_oracle.py cases).
//
// Each case's command file runs through the KEY loop: KFREAD reads the seed (a copy) with the
// group 2 and group 3 parsers, INICTC/GENCTC run cmd_inictc/cmd_genctc, KFWRIT writes every
// object with write_object. The RZ and BCCDEF records of every object (the arrays these commands
// change) must equal the original's OUT.KEY byte for byte, and so must the interpreter's
// "ERROR: Keyword ..." lines in stdout. Without the cache the test passes with a note.
// CONTACT_ORACLE_VERBOSE=1 prints mismatches; CONTACT_ORACLE_ONLY=<label substring> narrows.
#define _CRT_SECURE_NO_WARNINGS
#include <defcmd/dispatch.hpp>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>

#include <io.h>
#include <process.h>
#include <stdio.h>

#include <defcmd/contact_cmds.hpp>
#include <defcmd/ctc2d_save.hpp>
#include <defkey/key_reader.hpp>
#include <defkey/group1.hpp>
#include <defkey/kw_g2obj.hpp>
#include <defkey/kw_g4.hpp>
#include <defkey/kw_g57.hpp>
#include <defkey/kw_g3/kw_g3.hpp>
#include <defkey/line_io.hpp>
#include <defmodel/keyword_registry.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/step/db_step.hpp>
#include <testkit/check.hpp>

namespace fs = std::filesystem;
using def::model::DB_STEP_DATA;
using def::model::LLOBJ_DATA;

namespace {

template <class T, class C>
T& at(C& base, std::size_t off)
{
    return *reinterpret_cast<T*>(reinterpret_cast<unsigned char*>(&base) + off);
}

std::string env(const char* name, const char* def)
{
    const char* v = std::getenv(name);
    return v != nullptr && *v != '\0' ? v : def;
}

std::string tmp(const std::string& base) { return (fs::temp_directory_path() / (std::to_string(_getpid()) + "_" + base)).string(); }

std::string slurp(const fs::path& p)
{
    std::ifstream in(p, std::ios::binary);
    std::ostringstream s;
    s << in.rdbuf();
    return s.str();
}

std::string json_string(const std::string& j, const char* key, std::size_t from = 0)
{
    std::string pat = std::string("\"") + key + "\": \"";
    std::size_t p = j.find(pat, from);
    if (p == std::string::npos) return {};
    p += pat.size();
    std::string out;
    while (p < j.size() && j[p] != '"') {
        if (j[p] == '\\' && p + 1 < j.size()) ++p;
        out += j[p++];
    }
    return out;
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

// "KW obj" record with its continuation lines, by object and keyword
using Records = std::map<int, std::map<std::string, std::string>>;
Records records_of(const std::string& text, const std::set<std::string>& kws)
{
    Records r;
    std::vector<std::string> ls = lines_of(text);
    for (std::size_t i = 0; i < ls.size(); ++i) {
        char name[64] = {0};
        int obj = 0;
        if (ls[i].empty() || ls[i][0] == ' ' || ls[i][0] == '*') continue;
        if (std::sscanf(ls[i].c_str(), "%63s %i", name, &obj) < 2 || kws.count(name) == 0) continue;
        std::string t = ls[i] + "\n";
        std::size_t j = i + 1;
        while (j < ls.size() && !ls[j].empty() && ls[j][0] == ' ') t += ls[j++] + "\n";
        r[obj][name] += t;
        i = j - 1;
    }
    return r;
}

std::vector<std::string> error_lines(const std::string& text)
{
    std::vector<std::string> v;
    for (auto& l : lines_of(text))
        if (l.find("INICTC") != std::string::npos || l.find("GENCTC") != std::string::npos) v.push_back(l);
    return v;
}

// value check of an original BCCDEF record ("BCCDEF obj n 0", then n lines "node c0 c1") against
// the codes (2 per node) the port holds
bool bcc_match(const std::string& text, const std::vector<std::int32_t>& v)
{
    std::vector<std::string> ls = lines_of(text);
    int obj = 0, n = 0, z = 0;
    if (ls.empty() || std::sscanf(ls[0].c_str(), "%*s %d %d %d", &obj, &n, &z) != 3) return false;
    std::vector<std::int32_t> want(v.size(), 0);
    for (std::size_t i = 1; i < ls.size(); ++i) {
        int node = 0, c0 = 0, c1 = 0;
        if (std::sscanf(ls[i].c_str(), "%d %d %d", &node, &c0, &c1) != 3) continue;
        if (node < 1 || 2 * static_cast<std::size_t>(node) > want.size()) return false;
        want[2 * (node - 1)] = c0;
        want[2 * (node - 1) + 1] = c1;
    }
    return want == v;
}

struct Hooks : def::key::KeyReaderHooks {
    DB_STEP_DATA& m;
    fs::path dir;     // folder of the case inputs (KFREAD names are relative to it)
    std::string out;  // KFWRIT result
    std::map<int, std::vector<std::int32_t>> bcc;  // BCCDEF codes at KFWRIT, by object
    std::FILE* msg_out = stdout;
    explicit Hooks(DB_STEP_DATA& model, fs::path d) : m(model), dir(std::move(d)) {}
    bool has_sub_model() override { return false; }
    void set_model_dimension(int dim) override
    {
        at<std::int32_t>(m, 0x1c) = dim;
        def::model::step_setup_array_sizes(m, dim);
    }
    void set_sub_model_dimension(int) override {}
    std::string next_line(std::FILE* f)
    {
        char buf[0x400] = {0};
        if (std::fgets(buf, sizeof buf, f) == nullptr) return {};
        std::string s = buf;
        while (!s.empty() && (s.back() == '\n' || s.back() == '\r' || s.back() == ' ')) s.pop_back();
        return s;
    }
    int parse_keyword(int group, std::FILE* f, int id) override
    {
        if (group == 2) return def::key::g2obj::read_object_keyword(m, f, id);
        if (group == 3) return def::key::g3::read_keyword(&m, f, id);
        if (group == 1) return def::key::read_group1(&m, f, id);
        if (group == 4) return def::key::g4::read_keyword(m, f, id);
        if (group == 5) return def::key::g57::read_phase_keyword(&m, f, id);
        if (group == 7) return def::key::g57::read_dim_keyword(&m, f, id);
        if (group != 6) return 0;
        if (id == 5 || id == 6) return def::cmd::dispatch(&m, f, id);  // FUN_140378e00
        if (id == def::model::keyword_id("KFREAD")) {
            next_line(f);
            fs::path seed = dir / next_line(f);
            fs::path copy = tmp("contact_oracle_seed.key");
            fs::copy_file(seed, copy, fs::copy_options::overwrite_existing);
            std::FILE* in = std::fopen(copy.string().c_str(), "r");
            int r = def::key::read_key_stream(in, *this, msg_out);
            std::fclose(in);
            fs::remove(copy);
            return r == 0 ? 0 : 0;
        }
        if (id == def::model::keyword_id("KFWRIT")) {
            next_line(f);
            next_line(f);
            fs::path o = tmp("contact_oracle_out.key");
            std::FILE* w = std::fopen(o.string().c_str(), "wb");
            for (LLOBJ_DATA* p = at<LLOBJ_DATA*>(m, 0x1380); p != nullptr; p = at<LLOBJ_DATA*>(*p, 0x3940)) {
                def::key::g2obj::write_object(m, w, p, 0, false);
                // BCCDEF's writer (state-variable writer 1402a4a80, kw-g2sv) is not ported yet:
                // keep the codes for a value comparison
                std::vector<std::int32_t>& v = bcc[at<std::int32_t>(*p, 8)];
                const std::int32_t* a = at<std::int32_t*>(*p, 0x990);
                const std::int32_t n = at<std::int32_t>(*p, 0x880) * at<std::int32_t>(m, 0x1640);
                if (a != nullptr) v.assign(a, a + n);
            }
            std::fclose(w);
            out = slurp(o);
            fs::remove(o);
            return 0;
        }
        return 0;
    }
};

struct Case {
    std::string label;
    fs::path cmd;   // command file
    fs::path seed_dir;
};

// the 2D cases of a cases.json (inputs: seed SPIKE_SI.KEY + CMD.KEY)
std::vector<Case> cases_of(const fs::path& json)
{
    std::vector<Case> v;
    std::string j = slurp(json);
    std::size_t p = 0;
    while ((p = j.find("\"label\"", p)) != std::string::npos) {
        Case c;
        c.label = json_string(j, "label", p);
        std::size_t q = j.find("\"label\"", p + 1);
        std::string body = j.substr(p, q == std::string::npos ? std::string::npos : q - p);
        p += 7;
        if (body.find("/key/2D/") == std::string::npos) continue;  // 2D seeds only
        std::size_t c1 = body.find("\"CMD.KEY\"");
        if (c1 == std::string::npos) continue;
        c.cmd = json.parent_path() / json_string(body, "source", c1);
        c.seed_dir = fs::path(json_string(body, "source")).parent_path();
        if (slurp(c.cmd).find("OBJPOS") != std::string::npos) continue;  // OBJPOS: packet p67-1/2
        v.push_back(c);
    }
    return v;
}

std::map<std::string, fs::path> oracle_dirs(const fs::path& cache)
{
    std::map<std::string, fs::path> m;
    for (auto& d1 : fs::directory_iterator(cache / "entries")) {
        if (!d1.is_directory()) continue;
        for (auto& d2 : fs::directory_iterator(d1.path())) {
            fs::path rj = d2.path() / "result.json";
            if (!fs::exists(rj)) continue;
            std::string l = json_string(slurp(rj), "label");
            if (l.rfind("INICTC/", 0) == 0 || l.rfind("GENCTC/", 0) == 0) m[l] = d2.path();
        }
    }
    return m;
}

fs::path repo_root()
{
    fs::path p = fs::path(__FILE__).parent_path();
    while (!p.empty() && !fs::exists(p / "tests" / "commands")) p = p.parent_path();
    return p;
}

} // namespace

TEST_CASE(contact_commands_match_the_oracle)
{
    unsigned long long& o = *__local_stdio_printf_options();
    o = (o | _CRT_INTERNAL_PRINTF_LEGACY_THREE_DIGIT_EXPONENTS | _CRT_INTERNAL_PRINTF_LEGACY_MSVCRT_COMPATIBILITY) &
        ~static_cast<unsigned long long>(_CRT_INTERNAL_PRINTF_STANDARD_ROUNDING);
    fs::path cache = env("DEFORM_ORACLE_CACHE", "");
    if (cache.empty()) cache = fs::path(env("USERPROFILE", "")) / ".deform-oracle";
    if (!fs::exists(cache / "entries")) {
        std::printf("no oracle cache at %s: skipped\n", cache.string().c_str());
        return;
    }
    const bool verbose = env("CONTACT_ORACLE_VERBOSE", "0") == "1";
    const std::string only = env("CONTACT_ORACLE_ONLY", "");
    const std::set<std::string> kws = {"RZ", "BCCDEF"};
    auto dirs = oracle_dirs(cache);
    fs::path root = repo_root();
    int ncase = 0, nbad = 0, nmissing = 0;
    for (const char* json : {"INICTC/cases.json", "GENCTC/cases.json", "GENCTC/cases_p676.json", "GENCTC/cases_p676_more.json"}) {
        for (const Case& c : cases_of(root / "tests" / "commands" / json)) {
            if (!only.empty() && c.label.find(only) == std::string::npos) continue;
            if (dirs.count(c.label) == 0 || !fs::exists(dirs[c.label] / "files" / "OUT.KEY")) {
                ++nmissing;
                std::printf("%-24s no oracle output\n", c.label.c_str());
                continue;
            }
            ++ncase;
            def::cmd::ftn::reset_ctc2d_save();
            DB_STEP_DATA& m = *new DB_STEP_DATA(3);
            Hooks h(m, c.seed_dir);
            // the command file's own folder holds no seed: KFREAD names it relative to the corpus
            fs::path cmdcopy = tmp("contact_oracle_cmd.key");
            fs::copy_file(c.cmd, cmdcopy, fs::copy_options::overwrite_existing);
            fs::path msg = tmp("contact_oracle_msg.txt");
            std::FILE* mf = std::fopen(msg.string().c_str(), "w");
            std::FILE* in = std::fopen(cmdcopy.string().c_str(), "r");
            h.msg_out = mf;
            // the kernels' Fortran warnings go to stderr (unit 0): capture fd 2
            fs::path errp = tmp("contact_oracle_err.txt");
            std::fflush(stderr);
            const int saved = _dup(2);
            std::FILE* ef = std::fopen(errp.string().c_str(), "w");
            _dup2(_fileno(ef), 2);
            def::key::read_key_stream(in, h, mf);
            std::fflush(stderr);
            _dup2(saved, 2);
            _close(saved);
            std::fclose(ef);
            const std::string errs = slurp(errp);
            fs::remove(errp);
            std::fclose(in);
            std::fclose(mf);
            const std::string msgs = slurp(msg);
            fs::remove(msg);
            fs::remove(cmdcopy);

            Records mine = records_of(h.out, kws);
            Records orig = records_of(slurp(dirs[c.label] / "files" / "OUT.KEY"), kws);
            int bad = 0, cmp = 0;
            for (auto& [obj, recs] : orig) {
                for (auto& [kw, text] : recs) {
                    ++cmp;
                    // BCCDEF: byte compare once its writer (kw-g2sv) writes it, value compare before
                    if (kw == "BCCDEF" && mine[obj][kw].empty() ? bcc_match(text, h.bcc[obj]) : mine[obj][kw] == text)
                        continue;
                    ++bad;
                    if (verbose)
                        std::printf("MISMATCH %s obj %d %s\n  orig: %.300s\n  port: %.300s\n", c.label.c_str(), obj,
                                    kw.c_str(), text.c_str(), mine[obj][kw].c_str());
                }
            }
            for (auto& [obj, recs] : mine)
                for (auto& [kw, text] : recs)
                    if (kw != "BCCDEF" && orig[obj].count(kw) == 0) {
                        ++bad;
                        if (verbose) std::printf("EXTRA %s obj %d %s\n", c.label.c_str(), obj, kw.c_str());
                    }
            const auto e1 = error_lines(msgs);
            const auto e2 = error_lines(slurp(dirs[c.label] / "stdout.txt"));
            if (errs != slurp(dirs[c.label] / "stderr.txt")) {
                ++bad;
                if (verbose) std::printf("STDERR differs in %s:\n%s\n", c.label.c_str(), errs.c_str());
            }
            if (e1 != e2) {
                ++bad;
                if (verbose) std::printf("MESSAGES differ in %s (%zu vs %zu lines)\n", c.label.c_str(), e1.size(), e2.size());
            }
            std::printf("%-24s records %d, mismatched %d\n", c.label.c_str(), cmp, bad);
            if (bad != 0) ++nbad;
        }
    }
    std::printf("contact oracle: %d cases, %d mismatched, %d without oracle output\n", ncase, nbad, nmissing);
    CHECK_EQ(nbad, 0);
}

int main() { return testkit::run_all(); }
