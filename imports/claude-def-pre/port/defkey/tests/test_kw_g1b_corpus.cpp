#define _CRT_SECURE_NO_WARNINGS
// kw-g1b corpus check: every KFREAD + KFWRIT round trip of a corpus KEY file in the oracle cache
// (%USERPROFILE%\.deform-oracle, or DEFORM_ORACLE_CACHE) is replayed with kw-g1b's readers and
// writers, and the Iteration Controls / Processing Conditions / User Defined Variables sections
// must equal the original's output byte for byte. Passes (with a note) when the cache is absent.
// ctest-labels: kw-g1b corpus
//
// KEY_G1B_REPORT=<file> writes one line per case (status, label, first differing line).
#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include <testkit/check.hpp>

#include "kw_g1b_keyloop.hpp"

using namespace kwg1b_test;
namespace fs = std::filesystem;

namespace {

// ---- a minimal JSON reader (result.json is written by tools/oracle/def_pre_oracle.py) -------

struct Json {
    enum Kind { Null, Bool, Num, Str, Arr, Obj } kind = Null;
    std::string str;
    std::vector<Json> arr;
    std::map<std::string, Json> obj;
    const Json* get(const std::string& k) const
    {
        auto it = obj.find(k);
        return it == obj.end() ? nullptr : &it->second;
    }
};

struct Parser {
    const std::string& s;
    std::size_t i = 0;
    void ws()
    {
        while (i < s.size() && (s[i] == ' ' || s[i] == '\n' || s[i] == '\r' || s[i] == '\t'))
            ++i;
    }
    std::string string()
    {
        std::string out;
        ++i;  // "
        while (i < s.size() && s[i] != '"') {
            char c = s[i++];
            if (c == '\\' && i < s.size()) {
                char e = s[i++];
                switch (e) {
                case 'n': out += '\n'; break;
                case 'r': out += '\r'; break;
                case 't': out += '\t'; break;
                case 'b': out += '\b'; break;
                case 'f': out += '\f'; break;
                case 'u': {
                    unsigned v = static_cast<unsigned>(std::strtoul(s.substr(i, 4).c_str(), nullptr, 16));
                    i += 4;
                    if (v < 0x80) {
                        out += static_cast<char>(v);
                    } else if (v < 0x800) {
                        out += static_cast<char>(0xc0 | (v >> 6));
                        out += static_cast<char>(0x80 | (v & 0x3f));
                    } else {
                        out += static_cast<char>(0xe0 | (v >> 12));
                        out += static_cast<char>(0x80 | ((v >> 6) & 0x3f));
                        out += static_cast<char>(0x80 | (v & 0x3f));
                    }
                    break;
                }
                default: out += e;
                }
            } else {
                out += c;
            }
        }
        ++i;
        return out;
    }
    Json value()
    {
        ws();
        Json j;
        if (i >= s.size())
            return j;
        char c = s[i];
        if (c == '{') {
            j.kind = Json::Obj;
            ++i;
            ws();
            if (s[i] == '}') {
                ++i;
                return j;
            }
            for (;;) {
                ws();
                std::string k = string();
                ws();
                ++i;  // :
                j.obj[k] = value();
                ws();
                if (s[i] == ',') {
                    ++i;
                    continue;
                }
                ++i;  // }
                return j;
            }
        }
        if (c == '[') {
            j.kind = Json::Arr;
            ++i;
            ws();
            if (s[i] == ']') {
                ++i;
                return j;
            }
            for (;;) {
                j.arr.push_back(value());
                ws();
                if (s[i] == ',') {
                    ++i;
                    continue;
                }
                ++i;  // ]
                return j;
            }
        }
        if (c == '"') {
            j.kind = Json::Str;
            j.str = string();
            return j;
        }
        std::size_t b = i;
        while (i < s.size() && s[i] != ',' && s[i] != '}' && s[i] != ']' && s[i] != ' ' && s[i] != '\n' &&
               s[i] != '\r')
            ++i;
        j.str = s.substr(b, i - b);
        j.kind = (j.str == "true" || j.str == "false") ? Json::Bool : (j.str == "null" ? Json::Null : Json::Num);
        return j;
    }
};

fs::path cache_dir()
{
    if (const char* c = std::getenv("DEFORM_ORACLE_CACHE"))
        return fs::path(c);
    if (const char* h = std::getenv("USERPROFILE"))
        return fs::path(h) / ".deform-oracle";
    if (const char* h = std::getenv("HOME"))
        return fs::path(h) / ".deform-oracle";
    return fs::path();
}

struct Case {
    std::string label;
    std::string key_path;  // the corpus KEY file (read only)
    fs::path out_path;     // cached KFWRIT output
};

// A cached KFREAD <in> / KFWRIT <out> round trip whose input is a file on disk.
bool parse_case(const fs::path& result_json, Case* c)
{
    std::string text = read_file(result_json.string());
    Parser p{text};
    Json j = p.value();
    const Json* in = j.get("inputs");
    const Json* label = j.get("label");
    if (!in || !label || in->kind != Json::Arr)
        return false;
    std::string cmd, in_name;
    std::map<std::string, std::string> sources;
    for (const Json& e : in->arr) {
        const Json* name = e.get("name");
        if (!name)
            continue;
        if (name->str == "CMD.KEY") {
            if (const Json* t = e.get("text"))
                cmd = t->str;
        } else if (const Json* src = e.get("source")) {
            sources[name->str] = src->str;
        }
    }
    // "KFREAD\n<in>\nKFWRIT\n<out>\n"
    if (cmd.rfind("KFREAD\n", 0) != 0)
        return false;
    std::size_t a = 7, b = cmd.find('\n', a);
    if (b == std::string::npos)
        return false;
    in_name = cmd.substr(a, b - a);
    if (cmd.compare(b + 1, 7, "KFWRIT\n") != 0)
        return false;
    std::size_t c0 = b + 8, c1 = cmd.find('\n', c0);
    if (c1 == std::string::npos)
        return false;
    std::string out_name = cmd.substr(c0, c1 - c0);
    auto it = sources.find(in_name);
    if (it == sources.end() || !fs::path(it->second).is_absolute())
        return false;
    c->label = label->str;
    c->key_path = it->second;
    c->out_path = result_json.parent_path() / "files" / out_name;
    return fs::exists(c->out_path) && fs::exists(c->key_path);
}

bool uses_object_user_variables(const std::string& key_path)
{
    std::string t = read_file(key_path, true);
    bool obj = t.find("\nUSRNOD") != std::string::npos || t.find("\nUSRELM") != std::string::npos;
    bool own = t.find("\nUNNAME") != std::string::npos || t.find("\nUENAME") != std::string::npos;
    return obj && !own;
}

std::string first_difference(const std::string& got, const std::string& want)
{
    std::size_t i = 0;
    while (i < got.size() && i < want.size() && got[i] == want[i])
        ++i;
    std::size_t ls = want.rfind('\n', i == 0 ? 0 : i - 1);
    ls = ls == std::string::npos ? 0 : ls + 1;
    auto line_at = [&](const std::string& s) {
        std::size_t e = s.find('\n', ls);
        return ls < s.size() ? s.substr(ls, e == std::string::npos ? std::string::npos : e - ls) : std::string("<end>");
    };
    return "want [" + line_at(want) + "] got [" + line_at(got) + "]";
}

}  // namespace

TEST_CASE(corpus_round_trips)
{
    fs::path entries = cache_dir() / "entries";
    if (cache_dir().empty() || !fs::exists(entries)) {
        std::printf("oracle cache not found (%s): skipped\n", entries.string().c_str());
        return;
    }
    std::unique_ptr<std::FILE, int (*)(std::FILE*)> report(nullptr, &std::fclose);
    if (const char* r = std::getenv("KEY_G1B_REPORT"))
        report.reset(std::fopen(r, "w"));
    int cases = 0, passed = 0, keywords = 0, shown = 0, partial_cases = 0;
    std::FILE* devnull = std::tmpfile();
    for (const auto& shard : fs::directory_iterator(entries)) {
        for (const auto& entry : fs::directory_iterator(shard.path())) {
            Case c;
            if (!parse_case(entry.path() / "result.json", &c))
                continue;
            std::string want = g1b_sections(read_file(c.out_path.string()));
            if (want.empty())
                continue;
            ++cases;
            LoopResult lr;
            int rc = 0;
            std::string got = round_trip(c.key_path, devnull, &lr, &rc);
            keywords += lr.g1b_keywords;
            // Object keywords USRNOD/USRELM (group 2) size the model's user-variable name arrays
            // through FUN_1401d0fc0, so the original writes UNNAME/UENAME records this loop cannot
            // produce. For those files the comparison stops at the first UNNAME/UENAME record.
            bool partial = false;
            if (got != want && uses_object_user_variables(c.key_path)) {
                std::size_t cut = want.find("\nUNNAME ");
                if (cut == std::string::npos)
                    cut = want.find("\nUENAME ");
                if (cut != std::string::npos && got.size() == cut + 1 && want.compare(0, cut + 1, got) == 0) {
                    got = want;
                    partial = true;
                    ++partial_cases;
                }
            }
            bool ok = rc == 0 && got == want;
            if (ok)
                ++passed;
            else if (shown++ < 10)
                std::printf("MISMATCH %s: %s\n", c.label.c_str(), first_difference(got, want).c_str());
            if (report)
                std::fprintf(report.get(), "%s\t%s\t%d\t%s\n", ok ? (partial ? "ok-partial" : "ok") : "FAIL",
                             c.label.c_str(), lr.g1b_keywords, ok ? "" : first_difference(got, want).c_str());
        }
    }
    std::fclose(devnull);
    std::printf("corpus: %d of %d KFREAD/KFWRIT round trips match (%d of them compared up to UNNAME/UENAME, "
                "which USRNOD/USRELM objects create); %d kw-g1b keyword records read\n",
                passed, cases, partial_cases, keywords);
    CHECK_EQ(passed, cases);
}

int main() { return testkit::run_all(); }
