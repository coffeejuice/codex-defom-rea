// Test support for packet p67-5 (TCYCLE, GENAXS, GENSUB, GENBCC): run a command file of
// tests/commands/<CMD>/ on the port and fetch the original's results for the same case from the
// oracle cache (%USERPROFILE%\.deform-oracle or $DEFORM_ORACLE_CACHE), where
// `tools/oracle/def_pre_oracle.py cases tests/commands/<CMD>/cases.json` put them.
//
// The command file is interpreted with the port's KEY loop (read_key_stream): groups 1-5 and 7
// go to the ported group parsers; group 6 (commands) goes to CmdRunner::command(), which handles
// KFREAD (reads the seed through the same loop), KFWRIT / GENDB (only consume their file name
// line: the test writes what it compares itself) and hands everything else to `on_command`.
// All file names are per-process temp names in the working directory (keytest::tmp_name).
#pragma once

#define _CRT_SECURE_NO_WARNINGS
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
#include <process.h>

#include <defkey/group1.hpp>
#include <defkey/key_reader.hpp>
#include <defkey/kw_g2obj.hpp>
#include <defkey/kw_g3/kw_g3.hpp>
#include <defkey/kw_g4.hpp>
#include <defkey/kw_g57.hpp>
#include <defmodel/keyword_registry.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/step/db_step.hpp>

namespace p675 {

namespace fs = std::filesystem;
using def::model::DB_STEP_DATA;
using def::model::LLOBJ_DATA;

template <class T, class C>
T& at(C& base, std::size_t off)
{
    return *reinterpret_cast<T*>(reinterpret_cast<unsigned char*>(&base) + off);
}

inline std::string env(const char* name, const char* def)
{
    const char* v = std::getenv(name);
    return v != nullptr && *v != '\0' ? v : def;
}

inline std::string tmp_name(const std::string& base)
{
    return std::to_string(static_cast<unsigned long>(_getpid())) + "_" + base;
}

inline std::string slurp(const fs::path& p)
{
    std::ifstream in(p, std::ios::binary);
    std::ostringstream s;
    s << in.rdbuf();
    return s.str();
}

inline void spit(const fs::path& p, const std::string& s)
{
    std::ofstream o(p, std::ios::binary);
    o << s;
}

// ---- repository and oracle ----------------------------------------------------------------------

// The repository root: $P675_REPO, else the first parent of the working directory that has
// tests/commands.
inline fs::path repo_root()
{
    std::string r = env("P675_REPO", "");
    if (!r.empty()) return r;
    for (fs::path p = fs::current_path(); !p.empty(); p = p.parent_path()) {
        if (fs::exists(p / "tests" / "commands")) return p;
        if (p == p.parent_path()) break;
    }
    return {};
}

// "key": "value" of the first occurrence after `from`, JSON backslash escapes undone
inline std::string json_string(const std::string& j, const char* key, std::size_t from = 0)
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

inline fs::path oracle_cache()
{
    fs::path cache = env("DEFORM_ORACLE_CACHE", "");
    if (cache.empty()) cache = fs::path(env("USERPROFILE", "")) / ".deform-oracle";
    return cache;
}

// Cache entry directory of the case labelled `label` ("TCYCLE/corpus"), empty if not cached.
// Holds result.json, stdout.txt, stderr.txt and files/<output>.
inline fs::path oracle_entry(const std::string& label)
{
    static std::map<std::string, fs::path> index;
    static bool built = false;
    if (!built) {
        built = true;
        fs::path e = oracle_cache() / "entries";
        if (fs::exists(e)) {
            for (auto& d1 : fs::directory_iterator(e)) {
                if (!d1.is_directory()) continue;
                for (auto& d2 : fs::directory_iterator(d1.path())) {
                    fs::path rj = d2.path() / "result.json";
                    if (!fs::exists(rj)) continue;
                    std::string j = slurp(rj);
                    std::string l = json_string(j, "label");
                    if (!l.empty() && l.find('/') != std::string::npos && l.rfind("kfrw/", 0) != 0 &&
                        l.rfind("m/", 0) != 0)
                        index[l] = d2.path();
                }
            }
        }
    }
    auto it = index.find(label);
    return it == index.end() ? fs::path() : it->second;
}

// One case of tests/commands/<CMD>/cases.json.
struct Case {
    std::string label;                       // "<CMD>/<option>"
    std::map<std::string, fs::path> inputs;  // name in the run directory -> source file
};

// All cases of tests/commands/<cmd>/cases.json (relative sources resolved against that folder).
inline std::vector<Case> load_cases(const std::string& cmd)
{
    std::vector<Case> out;
    fs::path dir = repo_root() / "tests" / "commands" / cmd;
    std::string j = slurp(dir / "cases.json");
    std::size_t p = 0;
    for (;;) {
        std::size_t l = j.find("\"label\": \"", p);
        if (l == std::string::npos) break;
        std::size_t next = j.find("\"label\": \"", l + 1);
        Case c;
        c.label = json_string(j, "label", l);
        std::size_t q = l;
        for (;;) {
            std::size_t n = j.find("\"name\": \"", q);
            if (n == std::string::npos || (next != std::string::npos && n > next)) break;
            std::string name = json_string(j, "name", n);
            std::string src = json_string(j, "source", n);
            fs::path sp = src;
            if (sp.is_relative()) sp = dir / sp;
            c.inputs[name] = sp;
            q = n + 1;
        }
        out.push_back(c);
        p = l + 1;
    }
    return out;
}

// ---- stdout capture -----------------------------------------------------------------------------

// Redirects the C runtime's stdout (fd 1) into a temp file while alive; text() returns what was
// written.
class StdoutCapture {
public:
    StdoutCapture() : name_(tmp_name("p675_stdout.txt"))
    {
        std::fflush(stdout);
        saved_ = _dup(1);
        int fd = _open(name_.c_str(), _O_WRONLY | _O_CREAT | _O_TRUNC | _O_BINARY, 0666);
        _dup2(fd, 1);
        _close(fd);
    }
    ~StdoutCapture()
    {
        restore();
        std::remove(name_.c_str());
    }
    std::string text()
    {
        restore();
        return slurp(name_);
    }

private:
    void restore()
    {
        if (saved_ < 0) return;
        std::fflush(stdout);
        _dup2(saved_, 1);
        _close(saved_);
        saved_ = -1;
    }
    std::string name_;
    int saved_ = -1;
};

// ---- the command-file runner --------------------------------------------------------------------

inline void trim_line(char* s)
{
    std::size_t n = std::strlen(s);
    while (n > 0 && (s[n - 1] == '\n' || s[n - 1] == '\r' || s[n - 1] == ' ' || s[n - 1] == '\t')) s[--n] = 0;
    char* b = s;
    while (*b == ' ' || *b == '\t') ++b;
    if (b != s) std::memmove(s, b, std::strlen(b) + 1);
}

struct CmdRunner : def::key::KeyReaderHooks {
    DB_STEP_DATA& m;
    const Case& c;
    // A command other than KFREAD / KFWRIT / GENDB: (keyword id, stream after the keyword).
    // Returns the dispatcher's status (0 or 2); return -1 for a command the test cannot run (the
    // case is then reported as skipped).
    std::function<int(int, std::FILE*)> on_command;
    std::vector<std::string> unsupported;  // commands on_command refused
    std::vector<fs::path> temps;

    CmdRunner(DB_STEP_DATA& model, const Case& kase) : m(model), c(kase) {}
    ~CmdRunner()
    {
        for (auto& t : temps) {
            std::error_code ec;
            fs::remove(t, ec);
        }
    }

    bool has_sub_model() override { return false; }
    void set_model_dimension(int dim) override
    {
        at<std::int32_t>(m, 0x1c) = dim;
        def::model::step_setup_array_sizes(m, dim);  // 1401d09c0 (the model reset on a dimension change)
    }
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
        case 6: return command(f, id);
        default: return 3;
        }
    }

    // Reads one line (rest of the current one) into buf.
    static void line(std::FILE* f, char* buf, int n)
    {
        buf[0] = 0;
        if (std::fgets(buf, n, f) == nullptr) buf[0] = 0;
        trim_line(buf);
    }

    int command(std::FILE* f, int id)
    {
        const char* name = def::model::keyword_name(id);
        std::string kw = name != nullptr ? name : "";
        char buf[1024];
        if (kw == "KFREAD") {
            line(f, buf, sizeof buf);  // option
            line(f, buf, sizeof buf);  // file name
            auto it = c.inputs.find(buf);
            fs::path src = it != c.inputs.end() ? it->second : fs::path(buf);
            fs::path copy = tmp_name(std::string("p675_seed_") + fs::path(buf).filename().string());
            fs::copy_file(src, copy, fs::copy_options::overwrite_existing);
            temps.push_back(copy);
            std::FILE* in = std::fopen(copy.string().c_str(), "r");
            if (in == nullptr) return 2;
            struct Closer {
                std::FILE* f;
                ~Closer() { std::fclose(f); }
            } closer{in};
            return def::key::read_key_stream(in, *this, stdout);
        }
        if (kw == "KFWRIT" || kw == "GENDB") {
            line(f, buf, sizeof buf);  // rest of the keyword's line
            line(f, buf, sizeof buf);  // file name
            return 0;
        }
        if (!on_command) {
            unsupported.push_back(kw);
            line(f, buf, sizeof buf);
            return 0;
        }
        int st = on_command(id, f);
        if (st < 0) {
            unsupported.push_back(kw);
            return 0;
        }
        return st;
    }

    // Runs the command file of the case (input CMD.KEY). The model's messages go to stdout.
    int run()
    {
        fs::path cmd = tmp_name("p675_CMD.KEY");
        fs::copy_file(c.inputs.at("CMD.KEY"), cmd, fs::copy_options::overwrite_existing);
        temps.push_back(cmd);
        std::FILE* in = std::fopen(cmd.string().c_str(), "r");
        struct Closer {
            std::FILE* f;
            ~Closer() { std::fclose(f); }  // also when a test hook throws out of the command
        } closer{in};
        return def::key::read_key_stream(in, *this, stdout);
    }
};

// The ids of the command keywords, from the registry.
inline int kw_id(const char* name) { return def::model::keyword_id(name); }

// ---- KEY output and record comparison -----------------------------------------------------------

// The port's KEY text of the parts this packet checks: group 1 sections, then every object's
// block (write_object), in list order.
inline std::string write_model_key(DB_STEP_DATA& m)
{
    fs::path p = tmp_name("p675_port_out.key");
    std::FILE* out = std::fopen(p.string().c_str(), "wb");
    def::key::write_group1(&m, out);
    for (LLOBJ_DATA* o = at<LLOBJ_DATA*>(m, 0x1380); o != nullptr; o = at<LLOBJ_DATA*>(*o, 0x3940))
        def::key::g2obj::write_object(m, out, o, 0, false);
    std::fclose(out);
    std::string s = slurp(p);
    fs::remove(p);
    return s;
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

// Records of registry keywords: a line whose first token is a keyword name starts one; following
// lines that start with a blank belong to it; OBJNAM / PRSNAM take the next line whatever it is.
// Key: "<KW>" for groups other than 2, "<KW> <object>" for group 2. Values in file order.
inline std::map<std::string, std::vector<std::string>> records_of(const std::string& text)
{
    std::map<std::string, std::vector<std::string>> r;
    std::vector<std::string> ls = lines_of(text);
    for (std::size_t i = 0; i < ls.size(); ++i) {
        const std::string& l = ls[i];
        if (l.empty() || l[0] == ' ' || l[0] == '*' || l[0] == '\t') continue;
        char name[64] = {0};
        int obj = 0;
        int n = std::sscanf(l.c_str(), "%63s %i", name, &obj);
        if (n < 1) continue;
        const def::model::KeywordRecord* k = def::model::find_keyword(name);
        if (k == nullptr) continue;
        std::string key = name;
        if (k->group == def::model::KeywordGroup::Object) key += " " + std::to_string(n == 2 ? obj : 0);
        std::string rec = l + "\n";
        std::size_t j = i + 1;
        if ((std::strcmp(name, "OBJNAM") == 0 || std::strcmp(name, "PRSNAM") == 0) && j < ls.size()) rec += ls[j++] + "\n";
        while (j < ls.size() && !ls[j].empty() && (ls[j][0] == ' ' || ls[j][0] == '\t')) rec += ls[j++] + "\n";
        r[key].push_back(rec);
        i = j - 1;
    }
    return r;
}

// Compares the records of the keywords in `kws` (names; empty = all keywords present in either
// text). Returns the number of mismatching record keys; prints each when verbose.
inline int compare_records(const std::string& orig, const std::string& port, const std::set<std::string>& kws,
                           bool verbose, const std::string& what)
{
    auto a = records_of(orig);
    auto b = records_of(port);
    std::set<std::string> keys;
    for (auto& [k, v] : a) keys.insert(k);
    for (auto& [k, v] : b) keys.insert(k);
    int bad = 0;
    for (const std::string& k : keys) {
        std::string kw = k.substr(0, k.find(' '));
        if (!kws.empty() && kws.count(kw) == 0) continue;
        const auto& x = a[k];
        const auto& y = b[k];
        if (x == y) continue;
        ++bad;
        if (verbose) {
            std::printf("MISMATCH %s %s\n  orig:", what.c_str(), k.c_str());
            for (auto& s : x) std::printf("\n%s", s.c_str());
            std::printf("\n  port:");
            for (auto& s : y) std::printf("\n%s", s.c_str());
            std::printf("\n");
        }
    }
    return bad;
}

// Lines of the original's stdout that a command prints (everything between "*** INFO: Reading
// Keyword File" and "*** INFO: Keyword File Read", minus blank lines). The menu text around it is
// the menus' business, not the command's.
inline std::string keyword_phase_stdout(const std::string& stdout_text)
{
    std::string out;
    bool in = false;
    for (const std::string& l : lines_of(stdout_text)) {
        if (l.find("*** INFO: Reading Keyword File") != std::string::npos) {
            in = true;
            continue;
        }
        if (l.find("*** INFO: Keyword File Read") != std::string::npos) break;
        if (in && !l.empty()) out += l + "\n";
    }
    return out;
}

} // namespace p675
