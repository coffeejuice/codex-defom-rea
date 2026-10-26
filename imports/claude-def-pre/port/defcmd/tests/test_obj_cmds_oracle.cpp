// ctest-labels: p67-4 oracle
// Oracle check of WRTOBJ, SVSCAL, CPYOBJ and DOESCL (packet p67-4) over the cached runs of the
// original DEF_PRE for tests/commands/{WRTOBJ,SVSCAL,CPYOBJ,DOESCL}/cases.json (populate with
// `python -I tools/oracle/def_pre_oracle.py cases tests/commands/<CMD>/cases.json`; cache
// %USERPROFILE%\.deform-oracle or $DEFORM_ORACLE_CACHE).
//
// Each case runs in a child process (a DEF_NOT_PORTED abort fails only that case) in a fresh
// scratch directory holding copies of the case inputs. The child replays CMD.KEY through the KEY
// loop: KFREAD reads the seed with the ported group parsers (1, 2, 3, 4, 5, 7), the commands of
// this packet run through def::cmd::objcmd, and KFWRIT writes the sections whose writers are
// ported: every material block (g4::write_material) and every object block (write_object, as the
// KEY writer 1401c5af0 calls FUN_14029e250). KFREAD/KFWRIT are test scaffolding here, not ports.
//
// Checks per case, byte for byte against the original:
//   * each KFWRIT file: the object section (from the first "Data for Object" banner to the
//     "Inter-Object Data" banner) and the material section (first "Property Data of Material"
//     banner to "Inter-Material Data");
//   * every other file the original created (WRTOBJ output), whole file;
//   * stdout: the lines the port prints (command messages and KEY loop errors of CMD.KEY) appear
//     in the original's stdout in the same order.
//
// P67_4_ONLY=<substring of the label> narrows the run, P67_4_VERBOSE=1 prints the first differing
// lines, P67_4_KEEP=1 keeps the scratch directories, P67_4_NOOP=1 skips the commands (to see which
// cases detect them). Without the cache the test passes with a note.
#define _CRT_SECURE_NO_WARNINGS
#include <direct.h>
#include <process.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include <defcmd/obj_cmds.hpp>
#include <defkey/group1.hpp>
#include <defkey/key_reader.hpp>
#include <defkey/kw_g2obj.hpp>
#include <defkey/kw_g3/kw_g3.hpp>
#include <defkey/kw_g4.hpp>
#include <defkey/kw_g57.hpp>
#include <defkey/line_io.hpp>
#include <defmodel/keyword_registry.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/layout/material.hpp>
#include <defmodel/strtrim.hpp>
#include <testkit/check.hpp>

#include "../../defkey/tests/key_test_util.hpp"

namespace fs = std::filesystem;
using def::model::DB_STEP_DATA;
using def::model::LLOBJ_DATA;

namespace {

std::string env(const char* name, const char* def = "")
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

void spit(const fs::path& p, const std::string& s)
{
    std::ofstream o(p, std::ios::binary);
    o << s;
}

// "key": "value" of the first occurrence at or after `from`, JSON escapes undone; `end` gets the
// position after the value
std::string json_string(const std::string& j, const char* key, std::size_t from = 0, std::size_t* end = nullptr)
{
    std::string pat = std::string("\"") + key + "\": \"";
    std::size_t p = j.find(pat, from);
    if (p == std::string::npos) {
        if (end) *end = std::string::npos;
        return {};
    }
    p += pat.size();
    std::string out;
    while (p < j.size() && j[p] != '"') {
        if (j[p] == '\\' && p + 1 < j.size()) ++p;
        out += j[p++];
    }
    if (end) *end = p;
    return out;
}

fs::path repo_root() { return fs::absolute(fs::path(__FILE__)).parent_path().parent_path().parent_path().parent_path(); }

fs::path cache_dir()
{
    std::string c = env("DEFORM_ORACLE_CACHE");
    if (!c.empty()) return c;
    return fs::path(env("USERPROFILE")) / ".deform-oracle";
}

struct Input {
    std::string name, source;
};
struct Case {
    std::string label;
    fs::path entry;  // oracle cache entry dir
    std::string created;
    std::vector<Input> inputs;
    std::vector<std::string> files;  // outputs of the original
};

const char* kCommands[] = {"WRTOBJ/", "SVSCAL/", "CPYOBJ/", "DOESCL/"};

// All cached entries of this packet's cases (the newest per label).
std::map<std::string, Case> find_cases()
{
    std::map<std::string, Case> out;
    fs::path root = cache_dir() / "entries";
    if (!fs::exists(root)) return out;
    for (auto& d1 : fs::directory_iterator(root)) {
        if (!d1.is_directory()) continue;
        for (auto& d2 : fs::directory_iterator(d1.path())) {
            fs::path rj = d2.path() / "result.json";
            if (!fs::exists(rj)) continue;
            std::string j = slurp(rj);
            std::string label = json_string(j, "label");
            bool mine = false;
            for (const char* c : kCommands) mine = mine || label.rfind(c, 0) == 0;
            if (!mine) continue;
            Case k;
            k.label = label;
            k.entry = d2.path();
            k.created = json_string(j, "created");
            std::size_t pos = j.find("\"inputs\"");
            std::size_t stop = j.find("\"dirs\"");
            while (pos != std::string::npos && pos < stop) {
                std::size_t e = 0;
                std::string name = json_string(j, "name", pos, &e);
                if (e == std::string::npos || e > stop) break;
                std::string src = json_string(j, "source", e, &e);
                k.inputs.push_back({name, src});
                pos = e;
            }
            std::size_t fpos = j.find("\"files\"");
            if (fpos != std::string::npos) {
                std::size_t p = j.find('{', fpos) + 1;
                while (true) {
                    std::size_t q = j.find('"', p);
                    if (q == std::string::npos) break;
                    std::size_t r = j.find('"', q + 1);
                    std::string name = j.substr(q + 1, r - q - 1);
                    std::string status = json_string(j, "status", r);
                    if (status == "created" || status == "modified") k.files.push_back(name);
                    std::size_t close = j.find('}', r);  // end of this file's object
                    p = close + 1;
                    std::size_t nx = j.find_first_not_of(" \r\n\t,", p);
                    if (nx == std::string::npos || j[nx] == '}') break;
                }
            }
            auto it = out.find(label);
            if (it == out.end() || it->second.created < k.created) out[label] = k;
        }
    }
    return out;
}

// ---- the child: replay CMD.KEY -----------------------------------------------------------------

struct Hooks : def::key::KeyReaderHooks {
    DB_STEP_DATA& m;
    std::FILE* out;  // messages of this packet's commands
    std::FILE* sink;
    explicit Hooks(DB_STEP_DATA& model, std::FILE* o, std::FILE* s) : m(model), out(o), sink(s) {}
    bool has_sub_model() override { return false; }
    void set_model_dimension(int dim) override { m.dimension = dim; }
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
        case 6: break;
        default: return 3;
        }
        if (def::cmd::objcmd::handles(id)) {
            if (!env("P67_4_NOOP").empty()) {  // sensitivity check: skip the command's line only
                char line[0x400];
                int a = 0, b = 0, c = 0;
                if (std::fgets(line, 0x400, f) == nullptr) return 0;
                std::sscanf(line, "%i %i %i", &a, &b, &c);
                int more = id == 13 ? 0 : id == 12 ? (c > 0 ? c : 0) : 1;
                for (int i = 0; i < more; ++i) std::fgets(line, 0x400, f);
                return 0;
            }
            return def::cmd::objcmd::run_case(m, f, id, out);
        }
        if (id == 3) return kfread(f);
        if (id == 10) return kfwrit(f);
        // a command this harness does not provide (e.g. DBREAD): the parent reports SKIP
        if (std::FILE* u = std::fopen("port_unsupported.txt", "a")) {
            std::fprintf(u, "%s\n", def::model::keyword_name(id));
            std::fclose(u);
        }
        return 1;
    }
    // KFREAD (case 3): "%i" rest of line, file name line, then the KEY reader on that file
    int kfread(std::FILE* f)
    {
        std::int32_t n = 0;
        def::key::read_i(f, &n);
        char name[0x400];
        if (std::fgets(name, 0x400, f) == nullptr) return 2;
        def::model::trim_right_blanks(name);
        std::FILE* in = std::fopen(name, "r");
        if (in == nullptr) return 2;
        def::key::read_key_stream(in, *this, sink);
        std::fclose(in);
        return 0;
    }
    // KFWRIT (case 10): the file name (140413ab0), then the ported sections into <name>.port.*
    int kfwrit(std::FILE* f)
    {
        char name[0x400];
        if (def::key::read_string_next_line(f, name, 0x400) != 0) return 2;
        std::string base = name;
        std::FILE* fm = std::fopen((base + ".port.materials").c_str(), "w");
        for (def::model::Material* mat = m.materials.head; mat != nullptr; mat = mat->next)
            if (def::key::g4::write_material(m, fm, mat, nullptr) != 0) break;
        std::fclose(fm);
        std::FILE* fo = std::fopen((base + ".port.objects").c_str(), "w");
        for (LLOBJ_DATA* o = m.objects.head; o != nullptr; o = o->next)
            def::key::g2obj::write_object(m, fo, o, 0, false);
        std::fclose(fo);
        return 0;
    }
};

int child_main(const char* dir)
{
    keytest::legacy_printf();
    if (_chdir(dir) != 0) return 90;
    // what library code prints to stdout (the original prints nothing of that kind here)
    if (std::freopen("port_lib_stdout.txt", "w", stdout) == nullptr) return 92;
    DB_STEP_DATA& m = *new DB_STEP_DATA(3);
    std::FILE* out = std::fopen("port_stdout.txt", "w");
    std::FILE* sink = std::fopen("port_seed_log.txt", "w");
    Hooks hooks(m, out, sink);
    std::FILE* in = std::fopen("CMD.KEY", "r");
    if (in == nullptr) return 91;
    def::key::read_key_stream(in, hooks, out);
    std::fclose(in);
    std::fclose(out);
    std::fclose(sink);
    return 0;
}

// ---- the parent ------------------------------------------------------------------------------

// [first line of the banner whose title starts with `from`, first line of the banner `to`)
std::string section(const std::string& text, const char* from, const char* to)
{
    std::string a = std::string("*\r\n*  ") + from;
    std::string b = std::string("*\r\n*  ") + to;
    std::size_t p = text.find(a);
    if (p == std::string::npos) return {};
    std::size_t q = text.find(b, p);
    if (q == std::string::npos) q = text.size();
    return text.substr(p, q - p);
}

std::vector<std::string> lines_of(const std::string& s)
{
    std::vector<std::string> v;
    std::istringstream in(s);
    std::string l;
    while (std::getline(in, l)) {
        if (!l.empty() && l.back() == '\r') l.pop_back();
        v.push_back(l);
    }
    return v;
}

// Keywords whose writers are not merged yet: their records are dropped from both sides (and
// listed in the case notes). kw-g2sv: the state-variable keywords FUN_14029e250 writes through
// handlers / 1402a4a80 (group2-writer-map.md steps 24-47, 55-61).
const std::vector<std::string>& unported_keywords()
{
#if __has_include(<defkey/kw_g2sv.hpp>)
    static const std::vector<std::string> k;
#else
    static const std::vector<std::string> k = {
        "DRZ",    "DRMESH", "BCCDEF", "BCCDFN", "URZ",    "FRZ",    "PRZ",    "BCCTMP", "BCCTFN", "NDTMP",
        "NDHEAT", "NDFLUX", "TIMERC", "NRECVL", "USRNOD", "MTLGRP", "STRAIN", "STNCMP", "DENSTY", "DAMAGE",
        "STRESS", "YLDS",   "USRELM", "VOLFC",  "VOLFS",  "DVOLF",  "GRAIN",  "GRAIN2", "SIZESH", "TXTODF",
        "ERECVL", "TICF",   "DATOM",  "HDNEST", "HDNOBJ", "BCCCRB", "CRBFLX", "VOTAGE", "BCCRHT", "RHTFLX",
        "ZEFI",   "ZMFI",   "CURRNT", "MATAXI", "NWEAR",  "CUSVAR"};
#endif
    return k;
}

bool is_unported(const std::string& kw)
{
    for (const std::string& k : unported_keywords())
        if (k == kw) return true;
    return false;
}

struct Rec {
    std::string kw, text;
};

// Records of a KEY section: a line whose first token is a registry name starts one and the lines
// that follow up to the next such line (or a '*' line) belong to it; OBJNAM / PRSNAM take the
// next line whatever it is. Each '*' line is a record of its own.
std::vector<Rec> records_of(const std::string& text)
{
    std::vector<Rec> r;
    std::vector<std::string> ls = lines_of(text);
    for (std::size_t i = 0; i < ls.size(); ++i) {
        const std::string& l = ls[i];
        char name[64] = {0};
        bool starts = !l.empty() && l[0] != ' ' && l[0] != '*' && l[0] != '\t' && std::sscanf(l.c_str(), "%63s", name) == 1 &&
                      def::model::find_keyword(name) != nullptr;
        if (!l.empty() && l[0] == '*') {
            r.push_back({"*", l + "\n"});
            continue;
        }
        if (!starts || r.empty()) {
            if (r.empty()) r.push_back({"", ""});
            if (!starts) {
                r.back().text += l + "\n";
                continue;
            }
        }
        Rec rec{name, l + "\n"};
        std::size_t j = i + 1;
        if ((rec.kw == "OBJNAM" || rec.kw == "PRSNAM") && j < ls.size()) rec.text += ls[j++] + "\n";
        r.push_back(rec);
        i = j - 1;
    }
    return r;
}

// The section text without the records of unported keywords; `dropped` collects their names.
std::string filtered(const std::string& text, std::vector<std::string>* dropped)
{
    std::string out;
    for (const Rec& rec : records_of(text)) {
        if (is_unported(rec.kw)) {
            if (dropped && std::find(dropped->begin(), dropped->end(), rec.kw) == dropped->end()) dropped->push_back(rec.kw);
            continue;
        }
        out += rec.text;
    }
    return out;
}

void show_diff(const std::string& what, const std::string& a, const std::string& b)
{
    std::vector<std::string> la = lines_of(a), lb = lines_of(b);
    std::size_t n = std::max(la.size(), lb.size());
    int shown = 0;
    for (std::size_t i = 0; i < n && shown < 6; ++i) {
        std::string x = i < la.size() ? la[i] : "<none>";
        std::string y = i < lb.size() ? lb[i] : "<none>";
        if (x != y) {
            std::printf("    %s line %zu\n      oracle: %s\n      port:   %s\n", what.c_str(), i + 1, x.c_str(), y.c_str());
            ++shown;
        }
    }
    if (la.size() != lb.size()) std::printf("    %s: %zu oracle lines, %zu port lines\n", what.c_str(), la.size(), lb.size());
}

struct Result {
    bool ok = true;
    bool skipped = false;
    std::vector<std::string> notes;
};

Result run_case(const Case& k, const char* self, bool verbose)
{
    Result r;
    std::string safe = k.label;
    for (char& c : safe)
        if (c == '/' || c == '\\') c = '_';
    for (const Input& in : k.inputs) {
        fs::path src = in.source;
        if (src.is_relative()) src = repo_root() / src;
        if (!fs::exists(src)) {
            // a cached case whose command file is not in this tree (e.g. added on another branch)
            r.skipped = true;
            r.notes.push_back("input not in this tree: " + in.source);
            return r;
        }
    }
    fs::path dir = fs::temp_directory_path() / ("p67_4_" + std::to_string(_getpid()) + "_" + safe);
    fs::remove_all(dir);
    fs::create_directories(dir);
    for (const Input& in : k.inputs) {
        fs::path src = in.source;
        if (src.is_relative()) src = repo_root() / src;
        fs::copy_file(src, dir / in.name, fs::copy_options::overwrite_existing);
    }
    std::string d = dir.string();
    intptr_t rc = _spawnl(_P_WAIT, self, self, "--case", d.c_str(), nullptr);
    if (rc != 0) {
        r.ok = false;
        r.notes.push_back("child exit code " + std::to_string(rc));
    }
    if (fs::exists(dir / "port_unsupported.txt")) {
        r = Result{};
        r.skipped = true;
        r.notes.push_back("needs a command the harness does not provide: " +
                          lines_of(slurp(dir / "port_unsupported.txt"))[0]);
        if (env("P67_4_KEEP").empty()) fs::remove_all(dir);
        return r;
    }
    fs::path files = k.entry / "files";
    for (const std::string& name : k.files) {
        std::string orig = slurp(files / name);
        if (fs::exists(dir / (name + ".port.objects"))) {
            // a KFWRIT file
            struct S {
                const char* tag;
                const char* from;
                const char* to;
            } secs[] = {{"objects", "Data for Object", "Inter-Object Data"},
                        {"materials", "Property Data of Material", "Inter-Material Data"}};
            for (const S& s : secs) {
                std::vector<std::string> dropped;
                std::string o = filtered(section(orig, s.from, s.to), &dropped);
                std::string p = filtered(slurp(dir / (name + ".port." + s.tag)), nullptr);
                if (!dropped.empty()) {
                    std::string n = name + " " + s.tag + " not checked:";
                    for (const std::string& kw : dropped) n += " " + kw;
                    r.notes.push_back(n);
                }
                if (o != p) {
                    r.ok = false;
                    r.notes.push_back(name + " " + s.tag + " differ");
                    if (verbose) show_diff(name + " " + s.tag, o, p);
                }
            }
        } else {
            if (!fs::exists(dir / name)) {
                r.ok = false;
                r.notes.push_back(name + " not written by the port");
                continue;
            }
            std::string p = slurp(dir / name);
            std::vector<std::string> dropped;
            if (!unported_keywords().empty()) {
                orig = filtered(orig, &dropped);
                p = filtered(p, nullptr);
            }
            if (!dropped.empty()) {
                std::string n = name + " not checked:";
                for (const std::string& kw : dropped) n += " " + kw;
                r.notes.push_back(n);
            }
            if (orig != p) {
                r.ok = false;
                r.notes.push_back(name + " differs");
                if (verbose) show_diff(name, orig, p);
            }
        }
    }
    // files the port wrote that the original did not
    for (auto& e : fs::directory_iterator(dir)) {
        std::string n = e.path().filename().string();
        if (n.rfind("port_", 0) == 0 || n.find(".port.materials") != std::string::npos) continue;
        const std::string suffix = ".port.objects";  // a KFWRIT of the port: its file name
        if (n.size() > suffix.size() && n.compare(n.size() - suffix.size(), suffix.size(), suffix) == 0)
            n = n.substr(0, n.size() - suffix.size());
        bool input = false;
        for (const Input& in : k.inputs) input = input || in.name == n;
        bool known = false;
        for (const std::string& f : k.files) known = known || f == n;
        if (!input && !known) {
            r.ok = false;
            r.notes.push_back(n + " written by the port only");
        }
    }
    // stdout: the message lines (command messages, KEY loop errors and warnings) of both sides,
    // in order. The original's menu text is not matched by the patterns; the port's KEY loop
    // messages of the seed (KFREAD) went to port_seed_log.txt and are not compared.
    auto messages = [](const std::string& text) {
        static const char* pat[] = {"Scale ", "Assign ", "Error on reading DOESCL", "VarFieldWidth", "ERROR: Keyword",
                                    "WARNING: Keyword"};
        std::vector<std::string> v;
        for (const std::string& l : lines_of(text)) {
            for (const char* p : pat) {
                std::size_t q = l.find(p);
                if (q != std::string::npos) {
                    v.push_back(l.substr(q));
                    break;
                }
            }
        }
        return v;
    };
    std::vector<std::string> om = messages(slurp(k.entry / "stdout.txt"));
    std::vector<std::string> pm = messages(slurp(dir / "port_stdout.txt"));
    if (om != pm) {
        r.ok = false;
        r.notes.push_back("stdout messages differ (" + std::to_string(om.size()) + " oracle, " +
                          std::to_string(pm.size()) + " port)");
        if (verbose) {
            for (const std::string& l : om) std::printf("    oracle: %s\n", l.c_str());
            for (const std::string& l : pm) std::printf("    port:   %s\n", l.c_str());
        }
    }
    if (r.ok && env("P67_4_KEEP").empty()) fs::remove_all(dir);
    else r.notes.push_back("scratch " + d);
    return r;
}

} // namespace

int main(int argc, char** argv)
{
    if (argc == 3 && std::strcmp(argv[1], "--case") == 0) return child_main(argv[2]);
    std::map<std::string, Case> cases = find_cases();
    if (cases.empty()) {
        std::printf("p67-4 oracle: no cached cases under %s; skipped\n", cache_dir().string().c_str());
        return 0;
    }
    std::string only = env("P67_4_ONLY");
    bool verbose = !env("P67_4_VERBOSE").empty();
    int pass = 0, fail = 0;
    for (auto& [label, k] : cases) {
        if (!only.empty() && label.find(only) == std::string::npos) continue;
        Result r;
        try {
            r = run_case(k, argv[0], verbose);
        } catch (const std::exception& e) {
            r.ok = false;
            r.notes.push_back(std::string("exception: ") + e.what());
        }
        std::printf("%s %s", r.skipped ? "SKIP" : r.ok ? "PASS" : "FAIL", label.c_str());
        for (const std::string& n : r.notes) std::printf(" | %s", n.c_str());
        std::printf("\n");
        std::fflush(stdout);
        if (!r.skipped) (r.ok ? pass : fail)++;
    }
    std::printf("p67-4 oracle: %d passed, %d failed\n", pass, fail);
    return fail == 0 ? 0 : 1;
}
