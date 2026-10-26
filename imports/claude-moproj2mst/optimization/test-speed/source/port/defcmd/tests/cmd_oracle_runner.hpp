// defcmd/tests/cmd_oracle_runner.hpp: replay of command-keyword oracle cases through the port,
// shared by the p67 packet tests (first user: test_objpos_oracle.cpp, packet p67-1).
//
// A case is a cached oracle run (tools/oracle/def_pre_oracle.py, cache %USERPROFILE%\.deform-oracle
// or $DEFORM_ORACLE_CACHE) of a command file CMD.KEY (a file or inline text) with any number of
// other inputs (files or inline text), menu 2 -> 1. The runner places every input in a fresh
// working directory and:
//   * reads CMD.KEY with the KEY loop: KFREAD reads a copy of the seed with the group 1-5/7
//     parsers, KFWRIT writes every object with the group 2 object writer, GENDB is skipped, any
//     other group 6 keyword goes to the caller's `command` callback (return 0, 2 or 3 like the
//     dispatcher case; unknown ones: return -1 and the runner skips the rest of the line);
//   * captures stdout (the commands print with the CRT's stdout);
//   * compares every group 2 record of every object with the oracle's OUT.KEY, and the message
//     lines the caller selects with stdout.txt; records that already differ when the seed alone is
//     read and written (a "_baseline/..." oracle run with the same seed source file,
//     tests/commands/_baseline) are
//     reported as baseline gaps, not failures.
//
// Usage:
//     cmdoracle::Runner r;                               // scans the cache once
//     if (!r.ok()) return;                               // no cache: skip
//     cmdoracle::Options o;
//     // o.command unset: command keywords go through def::cmd::dispatch
//     o.message = [](const std::string& l) { return l.rfind("Object ", 0) == 0; };
//     o.reset = [] { /* process-lifetime globals back to their start values */ };
//     cmdoracle::Result res = r.run("OBJPOS/1-basic_3d", o);   // res.pass, res.report
// Environment: CMD_ORACLE_VERBOSE=1 prints each mismatching record, CMD_ORACLE_GAPS=1 the keywords
// of the baseline gaps.
#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include "../../defkey/tests/key_test_util.hpp"

#include <algorithm>
#include <cctype>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <functional>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>

#include <fcntl.h>
#include <io.h>

#include <defcmd/dispatch.hpp>
#include <defrt/clock.hpp>
#include <defkey/group1.hpp>
#include <defkey/key_reader.hpp>
#include <defkey/key_writer.hpp>
#include <defkey/kw_g2obj.hpp>
#include <defkey/kw_g3/kw_g3.hpp>
#include <defkey/kw_g4.hpp>
#include <defkey/kw_g57.hpp>
#include <defmodel/keyword_registry.hpp>
#include <defmodel/layout/cls_1401dbfe0.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/llobj_data.hpp>

namespace cmdoracle {

namespace fs = std::filesystem;
using def::model::DB_STEP_DATA;
using def::model::LLOBJ_DATA;

inline std::string env(const char* name, const char* def)
{
    const char* v = std::getenv(name);
    return v != nullptr && *v != '\0' ? v : def;
}

inline std::string slurp(const fs::path& p)
{
    std::ifstream in(p, std::ios::binary);
    std::ostringstream s;
    s << in.rdbuf();
    return s.str();
}

// Decodes the JSON string body starting at j[p] (just after the opening quote) up to the closing
// quote: \n \r \t \b \f \" \\ \/ and \uXXXX (one byte; the oracle stores inline inputs as
// latin-1). *end = position of the closing quote.
inline std::string json_decode(const std::string& j, std::size_t p, std::size_t* end = nullptr)
{
    std::string out;
    while (p < j.size() && j[p] != '"') {
        char c = j[p++];
        if (c == '\\' && p < j.size()) {
            const char e = j[p++];
            switch (e) {
            case 'n': c = '\n'; break;
            case 'r': c = '\r'; break;
            case 't': c = '\t'; break;
            case 'b': c = '\b'; break;
            case 'f': c = '\f'; break;
            case 'u': {
                const unsigned long v = std::strtoul(j.substr(p, 4).c_str(), nullptr, 16);
                p += 4;
                c = static_cast<char>(v & 0xff);
                break;
            }
            default: c = e; break;  // \" \\ \/
            }
        }
        out += c;
    }
    if (end != nullptr) *end = p;
    return out;
}

// "key": "value" after `from`, decoded; *end = position of the closing quote
inline std::string json_string(const std::string& j, const char* key, std::size_t from = 0, std::size_t* end = nullptr)
{
    std::string pat = std::string("\"") + key + "\": \"";
    std::size_t p = j.find(pat, from);
    if (p == std::string::npos) return {};
    return json_decode(j, p + pat.size(), end);
}

// One input of a cached case: its name in the scratch directory and a source file or inline text.
struct CaseInput {
    std::string name;
    std::string source;
    std::string text;
    bool inline_text = false;
};

// Every entry of the "inputs" array of a result.json.
inline std::vector<CaseInput> case_inputs(const std::string& j)
{
    std::vector<CaseInput> v;
    std::size_t p = j.find("\"inputs\"");
    if (p == std::string::npos) return v;
    p = j.find('[', p);
    std::size_t close = p;
    int depth = 0;
    bool in_str = false;
    for (; close < j.size(); ++close) {  // the matching ']' (strings may hold brackets)
        const char c = j[close];
        if (in_str) {
            if (c == '\\') ++close;
            else if (c == '"') in_str = false;
            continue;
        }
        if (c == '"') in_str = true;
        else if (c == '[') ++depth;
        else if (c == ']' && --depth == 0) break;
    }
    const std::string arr = j.substr(p, close - p + 1);
    const std::string key = "\"name\": \"";
    std::size_t q = 0;
    while ((q = arr.find(key, q)) != std::string::npos) {
        CaseInput in;
        std::size_t e = 0;
        in.name = json_decode(arr, q + key.size(), &e);
        const std::size_t next = arr.find(key, e);
        const std::string obj = arr.substr(e, next == std::string::npos ? std::string::npos : next - e);
        if (obj.find("\"source\": \"") != std::string::npos) {
            in.source = json_string(obj, "source");
        } else if (obj.find("\"text\": \"") != std::string::npos) {
            in.text = json_string(obj, "text");
            in.inline_text = true;
        }
        v.push_back(in);
        q = e;
    }
    return v;
}

inline std::vector<std::string> lines_of(const std::string& s)
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

// (object, keyword) -> the text of its records in order. A group 2 record starts at a line whose
// first token is a group 2 registry name (object = second token); lines starting with a blank
// belong to it; OBJNAM / PRSNAM take the next line whatever it is.
using RecordMap = std::map<std::pair<int, std::string>, std::vector<std::string>>;
inline RecordMap object_records(const std::string& text)
{
    RecordMap r;
    std::vector<std::string> ls = lines_of(text);
    for (std::size_t i = 0; i < ls.size(); ++i) {
        const std::string& l = ls[i];
        if (l.empty() || l[0] == ' ' || l[0] == '*' || l[0] == '\t') continue;
        char name[64] = {0};
        int obj = 0;
        if (std::sscanf(l.c_str(), "%63s %i", name, &obj) < 2) continue;
        const def::model::KeywordRecord* k = def::model::find_keyword(name);
        if (k == nullptr || k->group != def::model::KeywordGroup::Object) continue;
        std::string text = l + "\n";
        std::size_t j = i + 1;
        if ((std::strcmp(name, "OBJNAM") == 0 || std::strcmp(name, "PRSNAM") == 0) && j < ls.size()) text += ls[j++] + "\n";
        while (j < ls.size() && !ls[j].empty() && ls[j][0] == ' ') text += ls[j++] + "\n";
        r[{obj, name}].push_back(text);
        i = j - 1;
    }
    return r;
}

struct Options {
    // a group 6 keyword other than KFREAD / KFWRIT / GENDB; -1 = not handled (line skipped).
    // Unset: every such keyword goes through def::cmd::dispatch (defcmd/dispatch.hpp).
    std::function<int(DB_STEP_DATA&, std::FILE*, const std::string&)> command;
    // which stdout lines are compared (both sides)
    std::function<bool(const std::string&)> message;
    // optional: rewrite a selected stdout line before the comparison (both sides), e.g. to cut a
    // record off an unflushed prompt that precedes it on the same line
    std::function<std::string(const std::string&)> normalize;
    // run before each replay (the original starts a new process per case)
    std::function<void()> reset;
    // The original seeds rand() with time() in some routines (14045c530, CNVT3D cap
    // triangulation): the port's clock is pinned to the original run's start second
    // (def::rt::pin_clock) and, while the case still differs, to -1/+1/-2/+2 seconds of it.
    bool time_search = true;
    // Diagnostic for cases that still differ: report whether DIEGEO surface records whose points
    // are equal would be equal with their faces compared as a set (each face rotated to its
    // smallest node, orientation kept). It never turns a failure into a pass: a capped case passes
    // only byte-exact with the pinned clock (owner decision 2026-10-21). Result::face_set flags it.
    bool face_set_fallback = true;
};

struct Result {
    bool found = false;  // oracle entry present
    bool pass = false;
    int equal = 0, differ = 0, gaps = 0;
    std::string report;  // one line per problem
    int time_offset = 0;    // clock offset (s) of the replay that was compared
    bool face_set = false;  // failed, but DIEGEO faces are equal as a set (diagnostic)
    bool stale = false;     // the oracle entry is older than the case's command file / cases json
};

// The start second of a cached run: "start_time" (recorded since 2026-10-21) or, for older
// entries, the earliest output file's modification time minus the run's duration.
inline std::int64_t entry_start_time(const fs::path& dir, const std::string& json)
{
    const std::size_t p = json.find("\"start_time\": ");
    if (p != std::string::npos) return std::strtoll(json.c_str() + p + 14, nullptr, 10);
    double dur = 0.0;
    const std::size_t d = json.find("\"duration_s\": ");
    if (d != std::string::npos) dur = std::strtod(json.c_str() + d + 14, nullptr);
    std::error_code ec;
    fs::file_time_type best = fs::last_write_time(dir / "stdout.txt", ec);
    if (fs::exists(dir / "files"))
        for (auto& f : fs::recursive_directory_iterator(dir / "files"))
            if (f.is_regular_file() && f.last_write_time() < best) best = f.last_write_time();
    const auto sys = std::chrono::clock_cast<std::chrono::system_clock>(best);
    const double t = std::chrono::duration<double>(sys.time_since_epoch()).count();
    return static_cast<std::int64_t>(t - dur);
}

// A DIEGEO surface record ("DIEGEO obj geo npts nfaces", npts point lines, nfaces face lines
// "k n1 n2 n3 n4"): its point lines and its faces as a sorted set, each face rotated to start at
// its smallest node (orientation kept; a triangle repeats its first node or ends in 0 / a
// negative number, which stays last).
inline bool diegeo_equal_as_face_set(const std::string& a, const std::string& b)
{
    auto split = [](const std::string& rec, std::vector<std::string>& pts, std::vector<std::vector<long>>& faces) {
        std::vector<std::string> ls = lines_of(rec);
        if (ls.empty()) return false;
        char kw[16] = {0};
        long obj = 0, geo = 0, np = 0, nf = 0;
        if (std::sscanf(ls[0].c_str(), "%15s %ld %ld %ld %ld", kw, &obj, &geo, &np, &nf) < 5) return false;
        if (static_cast<long>(ls.size()) < 1 + np + nf) return false;
        pts.assign(ls.begin(), ls.begin() + 1 + np);
        for (long i = 0; i < nf; ++i) {
            std::istringstream is(ls[static_cast<std::size_t>(1 + np + i)]);
            long k = 0;
            std::vector<long> f;
            is >> k;
            for (long v; is >> v;) f.push_back(v);
            std::size_t n = f.size();
            if (n == 4 && (f[3] == f[0] || f[3] <= 0)) n = 3;  // triangle: the 4th entry stays as is
            std::vector<long> head(f.begin(), f.begin() + static_cast<std::ptrdiff_t>(n));
            std::rotate(head.begin(), std::min_element(head.begin(), head.end()), head.end());
            if (n == 3 && f.size() == 4) head.push_back(f[3] == f[0] ? head[0] : f[3]);
            faces.push_back(head);
        }
        std::sort(faces.begin(), faces.end());
        return true;
    };
    std::vector<std::string> pa, pb;
    std::vector<std::vector<long>> fa, fb;
    return split(a, pa, fa) && split(b, pb, fb) && pa == pb && fa == fb;
}

class Runner {
public:
    struct Entry {
        fs::path dir;
        std::string json;
    };

    Runner()
    {
        cache_ = env("DEFORM_ORACLE_CACHE", "");
        if (cache_.empty()) cache_ = fs::path(env("USERPROFILE", "")) / ".deform-oracle";
        if (!fs::exists(cache_ / "entries")) return;
        ok_ = true;
        for (auto& d1 : fs::directory_iterator(cache_ / "entries")) {
            if (!d1.is_directory()) continue;
            for (auto& d2 : fs::directory_iterator(d1.path())) {
                const fs::path rj = d2.path() / "result.json";
                if (!fs::exists(rj)) continue;
                std::string j = slurp(rj);
                const std::string label = json_string(j, "label");
                // the newest run of a label wins (a label is re-run when its inputs change)
                auto it = entries_.find(label);
                if (it != entries_.end() && json_string(j, "created") < json_string(it->second.json, "created")) continue;
                entries_[label] = Entry{d2.path(), std::move(j)};
            }
        }
    }
    bool ok() const { return ok_; }

    // A baseline run (KFREAD of the seed alone, label "_baseline/...") is found by the seed's
    // source file, not by its name in the scratch directory (many cases call their seed IN.KEY).
    static std::string source_key(std::string src)
    {
        for (char& c : src) c = c == '\\' ? '/' : static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
        return src;
    }
    const Entry* baseline_for(const std::vector<CaseInput>& inputs, const std::string& seed_name) const
    {
        std::string src;
        for (const CaseInput& in : inputs)
            if (in.name == seed_name && !in.inline_text) src = source_key(in.source);
        if (src.empty()) return nullptr;
        const Entry* best = nullptr;
        for (auto& [label, e] : entries_) {
            if (label.rfind("_baseline/", 0) != 0) continue;
            for (const CaseInput& in : case_inputs(e.json)) {
                if (in.inline_text || in.name == "CMD.KEY") continue;
                if (source_key(resolve_source(in.source)) != src) continue;
                if (best == nullptr || json_string(best->json, "created") < json_string(e.json, "created")) best = &e;
            }
        }
        return best;
    }

    // An entry is stale when a file that defines its case (the command file CMD.KEY or a
    // cases*.json beside it) changed after the oracle ran: the cached output may not belong to the
    // current command any more. Reported, not hidden (re-run the oracle for it).
    static bool stale(const fs::path& dir, const std::vector<CaseInput>& inputs)
    {
        std::error_code ec;
        const auto ran = fs::last_write_time(dir / "result.json", ec);
        if (ec) return false;
        for (const CaseInput& in : inputs) {
            if (in.name != "CMD.KEY" || in.inline_text || in.source.empty()) continue;
            const fs::path cmd = in.source;
            if (fs::exists(cmd) && fs::last_write_time(cmd, ec) > ran) return true;
            if (fs::exists(cmd.parent_path()))
                for (auto& f : fs::directory_iterator(cmd.parent_path(), ec))
                    if (f.path().extension() == ".json" && f.path().filename().string().rfind("cases", 0) == 0 &&
                        f.last_write_time(ec) > ran)
                        return true;
        }
        return false;
    }

    // The repository root (tests/commands/... sources of CMD.KEY are relative to it).
    static fs::path repo_root()
    {
        return fs::path(__FILE__).parent_path().parent_path().parent_path().parent_path();
    }

    // Where a recorded source is read from: relative sources are relative to the repository root;
    // an absolute one that no longer exists (recorded in a since-removed worktree) is read from the
    // same tests/... path in this checkout.
    static std::string resolve_source(const std::string& s)
    {
        if (s.empty()) return s;
        if (!fs::path(s).is_absolute()) return (repo_root() / s).string();
        std::error_code ec;
        if (fs::exists(s, ec)) return s;
        std::string k = s;
        for (char& c : k) if (c == '\\') c = '/';
        const std::size_t p = k.find("/tests/");
        if (p != std::string::npos) {
            const fs::path alt = repo_root() / k.substr(p + 1);
            if (fs::exists(alt, ec)) return alt.string();
        }
        return s;
    }

    Result run(const std::string& label, const Options& o) const
    {
        Result res;
        auto it = entries_.find(label);
        if (it == entries_.end()) return res;
        res.found = true;
        const Entry& e = it->second;
        std::vector<CaseInput> inputs = case_inputs(e.json);
        std::string cmd_tag = label.substr(label.find('/') + 1);
        for (CaseInput& in : inputs) {
            if (!in.inline_text) in.source = resolve_source(in.source);
        }
        // the tag goes into temp file names: replace every character Windows rejects in a name
        for (char& c : cmd_tag)
            if (static_cast<unsigned char>(c) < 0x20 || std::strchr("/\\:<>\"|?* ", c) != nullptr) c = '_';
        const std::string seed_name = kfread_name(inputs);
        const std::int64_t start = entry_start_time(e.dir, e.json);
        // baseline gaps
        std::set<std::pair<int, std::string>> gaps;
        const Entry* bit = baseline_for(inputs, seed_name);
        if (bit != nullptr) {
            std::vector<CaseInput> base_in = inputs;
            for (CaseInput& in : base_in) {
                if (in.name != "CMD.KEY") continue;
                in.inline_text = true;
                in.source.clear();
                in.text = "KFREAD 0\n" + seed_name + "\nKFWRIT\nOUT.KEY\n";
            }
            const Replay base = replay(base_in, "base_" + cmd_tag, o, start);
            RecordMap a = object_records(slurp(bit->dir / "files" / "OUT.KEY"));
            RecordMap b = object_records(base.out);
            for (auto& [k, v] : a)
                if (b[k] != v) gaps.insert(k);
            for (auto& [k, v] : b)
                if (a[k] != v) gaps.insert(k);
        }
        const RecordMap a0 = object_records(slurp(e.dir / "files" / "OUT.KEY"));
        std::vector<std::string> om;
        for (auto& l : lines_of(slurp(e.dir / "stdout.txt")))
            if (o.message && o.message(l)) om.push_back(o.normalize ? o.normalize(l) : l);
        const bool verbose = env("CMD_ORACLE_VERBOSE", "0") == "1";
        std::map<std::string, int> gap_kws;
        // one comparison of a replay with the original
        auto evaluate = [&](const Replay& port, bool face_set) {
            Result r;
            r.found = true;
            gap_kws.clear();
            std::vector<std::string> pm;
            for (auto& l : lines_of(port.msgs))
                if (o.message && o.message(l)) pm.push_back(o.normalize ? o.normalize(l) : l);
            bool pass = true;
            if (pm != om) {
                pass = false;
                r.report += "  messages differ\n";
                for (auto& l : om) r.report += "    orig: " + l + "\n";
                for (auto& l : pm) r.report += "    port: " + l + "\n";
            }
            RecordMap a = a0;
            RecordMap b = object_records(port.out);
            std::set<std::pair<int, std::string>> keys;
            for (auto& [k, v] : a) keys.insert(k);
            for (auto& [k, v] : b) keys.insert(k);
            for (auto& k : keys) {
                if (a[k] == b[k]) {
                    ++r.equal;
                    continue;
                }
                if (gaps.count(k) != 0) {
                    ++r.gaps;
                    ++gap_kws[k.second];
                    continue;
                }
                if (face_set && k.second == "DIEGEO" && a[k].size() == b[k].size()) {
                    bool same = true;
                    for (std::size_t i = 0; i < a[k].size() && same; ++i)
                        same = diegeo_equal_as_face_set(a[k][i], b[k][i]);
                    if (same) {
                        ++r.equal;
                        r.face_set = true;
                        r.report += "  record DIEGEO object " + std::to_string(k.first) + ": faces equal as a set only\n";
                        continue;
                    }
                }
                ++r.differ;
                r.report += "  record " + k.second + " object " + std::to_string(k.first) + " differs\n";
                if (verbose) {
                    r.report += "    orig:\n";
                    for (auto& s : a[k]) r.report += s;
                    r.report += "    port:\n";
                    for (auto& s : b[k]) r.report += s;
                }
            }
            r.pass = pass && r.differ == 0;
            return r;
        };
        static const int kOffsets[] = {0, -1, 1, -2, 2};
        const int tries = o.time_search ? 5 : 1;
        Replay first;
        for (int t = 0; t < tries; ++t) {
            const Replay port = replay(inputs, cmd_tag, o, start + kOffsets[t]);
            if (t == 0) first = port;
            res = evaluate(port, false);
            res.time_offset = kOffsets[t];
            if (res.pass) break;
        }
        if (!res.pass && o.face_set_fallback) {
            // diagnostic only (owner decision): the case still FAILS; the report says whether the
            // remaining difference is the DIEGEO face order alone
            const Result r = evaluate(first, true);
            if (r.pass) {
                res.face_set = true;
                res.report += "  diagnostic: equal when DIEGEO faces are compared as a set (cap order only); still a failure\n";
            }
        }
        if (stale(e.dir, inputs)) {
            res.stale = true;
            res.report += "  warning: oracle entry older than the case files (re-run the oracle for this label)\n";
        }
        if (env("CMD_ORACLE_GAPS", "0") == "1") {
            res.report += "  baseline gaps:";
            for (auto& [kw, n] : gap_kws) res.report += " " + kw + "(" + std::to_string(n) + ")";
            res.report += "\n";
        }
        return res;
    }

private:
    struct Replay {
        std::string out;
        std::string msgs;
    };

    struct Hooks : def::key::KeyReaderHooks {
        DB_STEP_DATA*& m;
        const Options& o;
        fs::path out_path;
        Hooks(DB_STEP_DATA*& model, const Options& opt) : m(model), o(opt) {}
        bool has_sub_model() override { return false; }
        void set_model_dimension(int dim) override
        {
            std::int32_t d = dim;
            m->set_dimension(&d);
        }
        void set_sub_model_dimension(int) override {}
        static void skip_line(std::FILE* f)
        {
            char buf[0x400];
            (void)std::fgets(buf, sizeof buf, f);
        }
        int parse_keyword(int group, std::FILE* f, int id) override
        {
            switch (group) {
            case 1: return def::key::read_group1(m, f, id);
            case 2: return def::key::g2obj::read_object_keyword(*m, f, id);
            case 3: return def::key::g3::read_keyword(m, f, id);
            case 4: return def::key::g4::read_keyword(*m, f, id);
            case 5: return def::key::g57::read_phase_keyword(m, f, id);
            case 7: return def::key::g57::read_dim_keyword(m, f, id);
            default: break;
            }
            std::string n = def::key::keyword_label(id);
            while (!n.empty() && n.back() == ' ') n.pop_back();
            if (n == "KFREAD") {
                skip_line(f);  // "0"
                char nm[0x400] = {0};  // the file name, read from the case's working directory
                if (std::fgets(nm, sizeof nm, f) == nullptr) return 2;
                std::string name = nm;
                while (!name.empty() && (name.back() == '\n' || name.back() == '\r' || name.back() == ' '))
                    name.pop_back();
                std::FILE* s = std::fopen(name.c_str(), "r");
                if (s == nullptr) return 2;
                std::FILE* sink = std::fopen("NUL", "w");
                def::key::read_key_stream(s, *this, sink);
                std::fclose(s);
                std::fclose(sink);
                return 0;
            }
            if (n == "KFWRIT") {
                skip_line(f);
                skip_line(f);
                std::FILE* out = std::fopen(out_path.string().c_str(), "wb");
                for (LLOBJ_DATA* ob = *reinterpret_cast<LLOBJ_DATA**>(reinterpret_cast<unsigned char*>(m) + 0x1380);
                     ob != nullptr; ob = *reinterpret_cast<LLOBJ_DATA**>(reinterpret_cast<unsigned char*>(ob) + 0x3940)) {
                    def::key::g2obj::write_object(*m, out, ob, 0, false);
                }
                std::fclose(out);
                return 0;
            }
            if (n == "GENDB") {
                skip_line(f);
                skip_line(f);
                return 0;
            }
            // the command keywords go through the dispatcher FUN_140378e00 (p5-01) unless the test
            // supplies its own routing
            if (!o.command) return def::cmd::dispatch(m, f, id);
            const int rc = o.command(*m, f, n);
            if (rc >= 0) return rc;
            skip_line(f);
            return 0;
        }
    };

    // The KEY file KFREAD reads in CMD.KEY (the line after "KFREAD"); else the first other input.
    static std::string kfread_name(const std::vector<CaseInput>& inputs)
    {
        for (const CaseInput& in : inputs) {
            if (in.name != "CMD.KEY") continue;
            const std::vector<std::string> ls = lines_of(in.inline_text ? in.text : slurp(in.source));
            for (std::size_t i = 0; i + 1 < ls.size(); ++i)
                if (ls[i].rfind("KFREAD", 0) == 0) {
                    std::string n = ls[i + 1];
                    while (!n.empty() && n.back() == ' ') n.pop_back();
                    return n;
                }
        }
        for (const CaseInput& in : inputs)
            if (in.name != "CMD.KEY") return in.name;
        return {};
    }

    // Every input is placed (copied, or written from inline text) in a fresh working directory;
    // CMD.KEY is read there with that directory as the current directory (commands open their
    // files by name, as DEF_PRE does in its scratch directory). The directory is removed after.
    static Replay replay(const std::vector<CaseInput>& inputs, const std::string& tag, const Options& o,
                         std::int64_t clock)
    {
        if (o.reset) o.reset();
        def::rt::pin_clock(clock);  // def::rt::now() as the original's time() during its run
        const fs::path tmp = fs::temp_directory_path();
        const fs::path work = tmp / keytest::tmp_name("cmdoracle_" + tag);
        const fs::path outp = tmp / keytest::tmp_name("cmdoracle_out_" + tag + ".key");
        const fs::path cap = tmp / keytest::tmp_name("cmdoracle_stdout_" + tag + ".txt");
        std::error_code ec;
        fs::remove_all(work, ec);
        fs::create_directories(work);
        for (const CaseInput& in : inputs) {
            const fs::path dst = work / in.name;
            fs::create_directories(dst.parent_path(), ec);
            if (in.inline_text) {
                std::ofstream f(dst, std::ios::binary);
                f << in.text;
            } else {
                fs::copy_file(in.source, dst, fs::copy_options::overwrite_existing);
            }
        }
        const fs::path old_cwd = fs::current_path();
        fs::current_path(work);
        DB_STEP_DATA* m = new DB_STEP_DATA(3);  // leaked: its dtor is not what these tests check
        m->db = new def::model::Cls_1401dbfe0();  // main always links one (14001c770); DBREAD needs it
        std::fflush(stdout);
        const int saved = _dup(_fileno(stdout));
        std::FILE* t = std::fopen(cap.string().c_str(), "wb");
        _dup2(_fileno(t), _fileno(stdout));
        {
            Hooks h(m, o);
            h.out_path = outp;
            if (std::FILE* f = std::fopen("CMD.KEY", "r")) {
                def::key::read_key_stream(f, h, stdout);
                std::fclose(f);
            }
        }
        std::fflush(stdout);
        _dup2(saved, _fileno(stdout));
        _close(saved);
        std::fclose(t);
        fs::current_path(old_cwd);
        def::rt::pin_clock(-1);
        Replay r{slurp(outp), slurp(cap)};
        fs::remove_all(work, ec);
        fs::remove(outp, ec);
        fs::remove(cap, ec);
        return r;
    }

    fs::path cache_;
    bool ok_ = false;
    std::map<std::string, Entry> entries_;
};

} // namespace cmdoracle
