// Oracle replay of the menu 3 / menu 4 sessions in tests/commands/MENU3, MENU4 (packet p10-01).
//
// Each case is a DEF_PRE session: "\n2\n1\n<seed>\n\nE\n" (read the seed KEY), then main menu 3 or
// 4 with the case's answers, then "8\nOUT.KEY\nE\nY\n" (save, exit). Until p5-01's menu loop is
// merged the port runs the menu function alone:
//   * the seed is read into a DB_STEP_DATA with the KEY loop and the group 1-5/7 parsers;
//   * the menu runs with std::cin on the answers after the "3"/"4" and stdout captured; the text
//     must equal the oracle's stdout between the main-menu prompt that got "3"/"4" and the next
//     main menu, byte for byte;
//   * the group 1 sections and the material blocks are written with the KEY writers and compared
//     record by record with the oracle's OUT.KEY; records that already differ for the baseline
//     session of the same seed (MENU3/base_2d, base_3d: read and save only) are not counted.
// The selection prompt 140007d30 is p5-01's def::pre::select_prompt (through ext::select).
// Skips (passes) when the oracle cache has no entry. P1001_VERBOSE=1 prints the differences.
// ctest-labels: p10-01 def_pre oracle
#define _CRT_SECURE_NO_WARNINGS
#include <def_pre/ext_p10_01.hpp>
#include <def_pre/forms.hpp>
#include <def_pre/menu7.hpp>
#include <testkit/check.hpp>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
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
#include <defmodel/layout/material.hpp>
#include <defrt/stdlib/stdio_compat.hpp>

namespace fs = std::filesystem;
using def::model::DB_STEP_DATA;
using def::pre::MenuRec;

namespace {

std::string env(const char* n)
{
    const char* v = std::getenv(n);
    return v != nullptr ? v : "";
}
bool verbose() { return env("P1001_VERBOSE") == "1"; }

std::string slurp(const fs::path& p)
{
    std::ifstream in(p, std::ios::binary);
    std::ostringstream s;
    s << in.rdbuf();
    return s.str();
}

// The JSON string value of "key" (first occurrence at or after `from`), escapes undone.
std::string json_str(const std::string& j, const char* key, std::size_t from = 0)
{
    const std::string pat = std::string("\"") + key + "\": \"";
    std::size_t p = j.find(pat, from);
    if (p == std::string::npos) return {};
    p += pat.size();
    std::string out;
    while (p < j.size() && j[p] != '"') {
        char c = j[p++];
        if (c == '\\' && p < j.size()) {
            const char e = j[p++];
            switch (e) {
            case 'n': c = '\n'; break;
            case 't': c = '\t'; break;
            case 'r': c = '\r'; break;
            case 'u': c = static_cast<char>(std::strtol(j.substr(p, 4).c_str(), nullptr, 16)); p += 4; break;
            default: c = e; break;
            }
        }
        out += c;
    }
    return out;
}

// label -> cache entry dir (the newest run of a label)
std::map<std::string, fs::path> scan_cache()
{
    std::map<std::string, fs::path> r;
    std::map<std::string, std::string> created;
    fs::path cache = env("DEFORM_ORACLE_CACHE");
    if (cache.empty()) cache = fs::path(env("USERPROFILE")) / ".deform-oracle";
    if (!fs::exists(cache / "entries")) return r;
    for (auto& d1 : fs::directory_iterator(cache / "entries")) {
        if (!d1.is_directory()) continue;
        for (auto& d2 : fs::directory_iterator(d1.path())) {
            const fs::path rj = d2.path() / "result.json";
            if (!fs::exists(rj)) continue;
            const std::string j = slurp(rj);
            const std::string label = json_str(j, "label");
            if (label.rfind("MENU3/", 0) != 0 && label.rfind("MENU4/", 0) != 0) continue;
            const std::string c = json_str(j, "created");
            if (r.count(label) != 0 && c < created[label]) continue;
            r[label] = d2.path();
            created[label] = c;
        }
    }
    return r;
}

// ---- model ---------------------------------------------------------------------------------

struct Hooks : def::key::KeyReaderHooks {
    DB_STEP_DATA* m;
    explicit Hooks(DB_STEP_DATA* model) : m(model) {}
    bool has_sub_model() override { return false; }
    void set_model_dimension(int dim) override
    {
        std::int32_t d = dim;
        m->set_dimension(&d);
    }
    void set_sub_model_dimension(int) override {}
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
        char buf[0x400];
        (void)std::fgets(buf, sizeof buf, f);
        return 0;
    }
};

std::string tmp(const std::string& base)
{
    return (fs::temp_directory_path() / (std::to_string(_getpid()) + "_p1001_" + base)).string();
}

DB_STEP_DATA* read_seed(const std::string& src)
{
    const std::string seed = tmp("seed.key");
    fs::copy_file(src, seed, fs::copy_options::overwrite_existing);
    auto* m = new DB_STEP_DATA(3); // leaked: the dtor is not what this test checks
    Hooks h(m);
    std::FILE* f = std::fopen(seed.c_str(), "r");
    std::FILE* sink = std::fopen("NUL", "w");
    def::key::read_key_stream(f, h, sink);
    std::fclose(f);
    std::fclose(sink);
    std::error_code ec;
    fs::remove(seed, ec);
    return m;
}

std::string write_sections(DB_STEP_DATA* m)
{
    const std::string p = tmp("out.key");
    std::FILE* f = std::fopen(p.c_str(), "wb");
    def::key::write_group1(m, f);
    for (def::model::Material* mat = m->materials.head; mat != nullptr; mat = mat->next) {
        def::key::g4::write_material(*m, f, mat, nullptr);
    }
    std::fclose(f);
    std::string s = slurp(p);
    std::error_code ec;
    fs::remove(p, ec);
    return s;
}

// The menu function with stdin = `answers`, stdout captured (text mode, as the original's).
std::string run_menu(DB_STEP_DATA* m, char menu, const std::string& answers)
{
    MenuRec rec[5] = {{1, 5, nullptr, m}, {1, 5, nullptr, nullptr}, {1, 17, nullptr, nullptr},
                      {1, 10, nullptr, nullptr}, {1, 5, nullptr, nullptr}};
    rec[0].data = reinterpret_cast<std::byte*>(m) + 0x750;
    MenuRec mat_rec{1, 9, nullptr, m};
    std::istringstream in(answers);
    std::streambuf* old = std::cin.rdbuf(in.rdbuf());
    std::cin.clear();
    const std::string cap = tmp("stdout.txt");
    std::fflush(stdout);
    const int saved = _dup(_fileno(stdout));
    std::FILE* t = std::fopen(cap.c_str(), "w"); // text mode, as a console's stdout (LF -> CRLF)
    _dup2(_fileno(t), _fileno(stdout));
    if (menu == '3') def::pre::menu3_sim_controls(rec);
    else def::pre::menu4_material(&mat_rec);
    std::fflush(stdout);
    _dup2(saved, _fileno(stdout));
    _close(saved);
    std::fclose(t);
    std::cin.rdbuf(old);
    std::cin.clear();
    std::string s = slurp(cap);
    std::error_code ec;
    fs::remove(cap, ec);
    return s;
}

// The oracle's text of the menu call: after the prompt of the 2nd main menu (the 1st follows
// start-up, the 2nd follows the seed read) up to the "\r\n" before the next main menu.
std::string oracle_segment(const std::string& out)
{
    const std::string head = "*****  DEFORM Input Preparation Main Menu  *****";
    const std::string prompt = " Enter Selection (1-9, R, E, /): ";
    std::size_t h = out.find(head);
    h = out.find(head, h + 1);
    const std::size_t a = out.find(prompt, h) + prompt.size();
    const std::size_t b = out.find(head, a);
    return out.substr(a, b - 2 - a);
}

// keyword records: a line whose first token is a registry keyword starts one; the key is
// "<name> <second token>" plus an occurrence count
std::map<std::string, std::string> records(const std::string& text)
{
    std::map<std::string, std::string> r;
    std::map<std::string, int> seen;
    std::istringstream in(text);
    std::string line, key;
    while (std::getline(in, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();
        char a[64] = {0}, b[64] = {0};
        const int n = std::sscanf(line.c_str(), "%63s %63s", a, b);
        if (n >= 1 && line[0] != ' ' && line[0] != '*' && def::model::find_keyword(a) != nullptr) {
            std::string k = std::string(a) + " " + (n == 2 ? b : "");
            k += "#" + std::to_string(seen[k]++);
            key = k;
        }
        if (line.rfind("*", 0) == 0) continue;
        if (!key.empty()) r[key] += line + "\n";
    }
    return r;
}

struct Seed {
    std::string name, src;
};

Seed seed_of(const std::string& json)
{
    const std::size_t p = json.find("\"inputs\"");
    return {json_str(json, "name", p), json_str(json, "source", p)};
}

int run_case(const fs::path& json_file, const std::map<std::string, fs::path>& cache, int& skipped)
{
    const std::string j = slurp(json_file);
    const std::string label = json_str(j, "label");
    auto it = cache.find(label);
    if (it == cache.end()) {
        ++skipped;
        std::printf("  %s: no oracle entry, skipped\n", label.c_str());
        return 0;
    }
    const std::string stdin_text = json_str(j, "stdin");
    Seed seed = seed_of(j);
    if (!fs::path(seed.src).is_absolute()) seed.src = (json_file.parent_path() / seed.src).string();
    const std::string prefix = "\n2\n1\n" + seed.name + "\n\nE\n";
    const std::string suffix = "8\nOUT.KEY\nE\nY\n";
    const std::string part = stdin_text.substr(prefix.size(), stdin_text.size() - prefix.size() - suffix.size());
    if (part.empty()) return 0; // baseline
    const char menu = part[0];
    const std::string answers = part.substr(2);
    int bad = 0;

    // text
    DB_STEP_DATA* m = read_seed(seed.src);
    const std::string got = run_menu(m, menu, answers);
    const std::string want = oracle_segment(slurp(it->second / "stdout.txt"));
    if (got != want) {
        ++bad;
        std::size_t k = 0;
        while (k < got.size() && k < want.size() && got[k] == want[k]) ++k;
        std::printf("  %s: stdout differs at byte %zu\n    orig: %s\n    port: %s\n", label.c_str(), k,
                    want.substr(k > 80 ? k - 80 : 0, 200).c_str(), got.substr(k > 80 ? k - 80 : 0, 200).c_str());
    }

    // KEY records
    const std::string base_label = seed.name == "RING_SI.KEY"         ? "MENU3/base_2d"
                                   : seed.name == "UPSET_Brick.KEY"   ? "MENU3/base_3d"
                                                                      : "MENU4/base_2dn";
    auto bit = cache.find(base_label);
    std::set<std::string> gaps;
    if (bit != cache.end()) {
        auto ob = records(slurp(bit->second / "files" / "OUT.KEY"));
        auto pb = records(write_sections(read_seed(seed.src)));
        for (auto& [k, v] : pb)
            if (ob[k] != v) gaps.insert(k);
    }
    auto oc = records(slurp(it->second / "files" / "OUT.KEY"));
    auto pc = records(write_sections(m));
    int equal = 0, unverified = 0;
    for (auto& [k, v] : pc) {
        if (gaps.count(k) != 0) {
            ++unverified;
            if (verbose()) std::printf("    gap %s\n", k.c_str());
            continue;
        }
        if (oc.count(k) == 0 || oc[k] != v) {
            ++bad;
            std::printf("  %s: record %s differs\n    orig: %s    port: %s", label.c_str(), k.c_str(),
                        oc.count(k) != 0 ? oc[k].c_str() : "(none)\n", v.c_str());
        }
        else {
            ++equal;
        }
    }
    std::printf("  %s: stdout %s, records %d equal, %d baseline gaps, %d differ\n", label.c_str(),
                got == want ? "equal" : "DIFFERS", equal, unverified, bad - (got != want ? 1 : 0));
    return bad;
}

} // namespace

TEST_CASE(menu3_menu4_oracle_sessions)
{
    def::rt::init_stdio_compat();
    def::pre::ext::g_hooks.select = nullptr;   // p5-01 select_prompt (140007d30)
    const auto cache = scan_cache();
    int bad = 0, skipped = 0, cases = 0;
    for (const char* dir : {"MENU3", "MENU4"}) {
        const fs::path d = testkit::repo_root() / "tests" / "commands" / dir;
        if (!fs::exists(d)) continue;
        for (auto& e : fs::directory_iterator(d)) {
            if (e.path().extension() != ".json") continue;
            ++cases;
            const int b = run_case(e.path(), cache, skipped);
            if (b != 0) ++bad;
        }
    }
    std::printf("p10-01 menu cases: %d, failing %d, skipped %d\n", cases, bad, skipped);
    CHECK_EQ(bad, 0);
}

int main() { return testkit::run_all(); }
