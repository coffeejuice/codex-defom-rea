// Replay of the scripted menu 5-6 sessions (tests/menus/MENU5_6/cases.json, packet p10-02)
// through the port, against the cached oracle runs (tools/oracle/def_pre_oracle.py; cache
// %USERPROFILE%\.deform-oracle or $DEFORM_ORACLE_CACHE; no cache: the test skips).
//
// Per case: the seed is read into a fresh DB_STEP_DATA with the KEY loop (p5-01's
// ModelKeyHooks), the menu runs on a byte image of the main object's records (main+0x000..0x190
// with the ctor's values) with std::cin fed the menu's own answers, and the model is written
// with the KEY writer 1401c5af0 (p5-01). Compared:
//   * stdout of the menu, byte for byte, with the oracle transcript between the main-menu
//     answer and the next main menu (the "\n" before "*****  DEFORM Input ..." is the main
//     loop's);
//   * every keyword record of the saved KEY with the oracle's OUT.KEY; records that already
//     differ in the baseline case of the seed (menu shown and left) are counted as baseline gaps.
//
// The selection prompt 140007d30 / 140006420 is p5-01's def::pre::select_prompt (menu_shell.hpp),
// reached through p10-01's ext::select with no test hook installed.
//
// ctest-labels: p10-02 def_pre oracle
#define _CRT_SECURE_NO_WARNINGS
#include "../../defcmd/tests/cmd_oracle_runner.hpp"

#include <def_pre/ext_p10_01.hpp>
#include <def_pre/ext_p10_02.hpp>
#include <def_pre/menu56.hpp>
#include <def_pre/menu7.hpp>
#include <defcmd/dispatch.hpp>
#include <testkit/check.hpp>

#include <cstdio>
#include <cstring>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include <fcntl.h>
#include <io.h>

using namespace def::pre;
namespace fs = std::filesystem;
using def::model::DB_STEP_DATA;

namespace {

// ---- the main object's record area (Cls_14001c770 ctor 14001c770 values) ----------------------
struct Records {
    alignas(8) unsigned char b[0x190] = {};
    template <class T>
    T& at(std::size_t off) { return *reinterpret_cast<T*>(b + off); }
    explicit Records(DB_STEP_DATA* m)
    {
        at<std::int32_t>(0x008) = 1;
        at<std::int32_t>(0x00c) = 10;
        at<void*>(0x018) = m;
        at<std::int32_t>(0x0c0) = 1;
        at<std::int32_t>(0x0c4) = 4;
        at<void*>(0x0d0) = m;
        at<double>(0x100) = -1.0;
        at<std::int32_t>(0x108) = 1;
        at<std::int32_t>(0x10c) = 2;
        std::uint64_t u = 0x3f1a36e2eb1c432dull; // 1e-4
        std::memcpy(b + 0x110, &u, 8);
        at<double>(0x140) = 1.0;
        at<std::int32_t>(0x150) = 1;
        std::memcpy(b + 0x158, &u, 8);
        at<std::int32_t>(0x160) = 1;
        at<std::int32_t>(0x164) = 1000;
    }
    MenuRec* menu5() { return reinterpret_cast<MenuRec*>(b + 0x008); }
    MenuRec* menu6() { return reinterpret_cast<MenuRec*>(b + 0x0c0); }
};

// keyword records of a KEY text: (keyword + second token, occurrence) -> text
std::map<std::string, std::string> key_records(const std::string& text)
{
    std::map<std::string, std::string> r;
    std::map<std::string, int> seen;
    const std::vector<std::string> ls = cmdoracle::lines_of(text);
    for (std::size_t i = 0; i < ls.size(); ++i) {
        const std::string& l = ls[i];
        if (l.empty() || l[0] == ' ' || l[0] == '*' || l[0] == '\t') continue;
        char a[64] = {0}, b[64] = {0};
        std::sscanf(l.c_str(), "%63s %63s", a, b);
        std::string k = std::string(a) + " " + b;
        std::string body = l + "\n";
        std::size_t j = i + 1;
        if ((std::strcmp(a, "OBJNAM") == 0 || std::strcmp(a, "PRSNAM") == 0) && j < ls.size()) body += ls[j++] + "\n";
        while (j < ls.size() && !ls[j].empty() && (ls[j][0] == ' ' || ls[j][0] == '\t')) body += ls[j++] + "\n";
        r[k + "#" + std::to_string(seen[k]++)] = body;
        i = j - 1;
    }
    return r;
}

struct Case {
    std::string label, seed, seed_src, stdin_text;
    int menu = 0;
};

std::vector<Case> load_cases(const fs::path& file)
{
    std::vector<Case> v;
    const std::string j = cmdoracle::slurp(file);
    std::size_t pos = 0;
    while (true) {
        std::size_t end = 0;
        const std::string label = cmdoracle::json_string(j, "label", pos, &end);
        if (label.empty()) break;
        Case c;
        c.label = label;
        c.seed_src = cmdoracle::json_string(j, "source", end, &end);
        const std::size_t rp = j.find("\"replay\"", end);
        const std::size_t mp = j.find("\"menu\": ", rp);
        c.menu = std::atoi(j.c_str() + mp + 8);
        c.seed = cmdoracle::json_string(j, "seed", rp, &end);
        c.stdin_text = cmdoracle::json_string(j, "stdin", rp, &end);
        // json_string keeps escapes undone character by character: turn "n" after a backslash back
        std::string s;
        const std::string raw_pat = "\"stdin\": \"";
        std::size_t q = j.find(raw_pat, rp) + raw_pat.size();
        while (j[q] != '"') {
            if (j[q] == '\\') {
                ++q;
                s += j[q] == 'n' ? '\n' : j[q];
            } else {
                s += j[q];
            }
            ++q;
        }
        c.stdin_text = s;
        pos = q;
        v.push_back(c);
    }
    return v;
}

std::map<std::string, fs::path> scan_cache()
{
    std::map<std::string, fs::path> m;
    std::map<std::string, std::string> created;
    fs::path cache = cmdoracle::env("DEFORM_ORACLE_CACHE", "");
    if (cache.empty()) cache = fs::path(cmdoracle::env("USERPROFILE", "")) / ".deform-oracle";
    if (!fs::exists(cache / "entries")) return m;
    for (auto& d1 : fs::directory_iterator(cache / "entries")) {
        if (!d1.is_directory()) continue;
        for (auto& d2 : fs::directory_iterator(d1.path())) {
            const fs::path rj = d2.path() / "result.json";
            if (!fs::exists(rj)) continue;
            const std::string j = cmdoracle::slurp(rj);
            const std::string label = cmdoracle::json_string(j, "label");
            if (label.rfind("MENU5_6/", 0) != 0) continue;
            const std::string c = cmdoracle::json_string(j, "created");
            if (m.count(label) != 0 && c < created[label]) continue;
            m[label] = d2.path();
            created[label] = c;
        }
    }
    return m;
}

// the menu's part of the oracle transcript
std::string oracle_slice(const std::string& out)
{
    const std::string hdr = "*****  DEFORM Input Preparation Main Menu  *****";
    const std::string prompt = " Enter Selection (1-9, R, E, /): ";
    std::size_t h0 = out.find(hdr);
    std::size_t h1 = out.find(hdr, h0 + 1);
    if (h1 == std::string::npos) return {};
    std::size_t a = out.find(prompt, h1);
    if (a == std::string::npos) return {};
    a += prompt.size();
    std::size_t b = out.find("\n" + hdr, a);
    if (b == std::string::npos) return {};
    return out.substr(a, b - a);
}

struct Run {
    std::string out, key;
};

Run replay(const Case& c)
{
    const fs::path tmp = fs::temp_directory_path();
    const fs::path cap = tmp / keytest::tmp_name("menu56_stdout.txt");
    const fs::path keyp = tmp / keytest::tmp_name("menu56_out.key");
    const fs::path seedp = tmp / keytest::tmp_name("menu56_" + c.seed); // a copy, never the corpus file
    fs::copy_file(c.seed_src, seedp, fs::copy_options::overwrite_existing);
    auto* m = new DB_STEP_DATA(3); // leaked, as the other replay tests
    {
        // the KEY loop's messages (SOLMTD defaults, ...) go to stdout: not part of the comparison
        std::fflush(stdout);
        const int keep = _dup(_fileno(stdout));
        std::FILE* nul = std::fopen("NUL", "w");
        _dup2(_fileno(nul), _fileno(stdout));
        def::cmd::ModelKeyHooks h(m);
        std::FILE* f = std::fopen(seedp.string().c_str(), "r");
        std::FILE* sink = std::fopen("NUL", "w");
        def::key::read_key_stream(f, h, sink);
        std::fclose(f);
        std::fclose(sink);
        std::fflush(stdout);
        _dup2(keep, _fileno(stdout));
        _close(keep);
        std::fclose(nul);
    }
    Records rec(m);
    std::istringstream in(c.stdin_text);
    std::streambuf* old = std::cin.rdbuf(in.rdbuf());
    std::cin.clear();
    std::fflush(stdout);
    const int saved = _dup(_fileno(stdout));
    std::FILE* t = std::fopen(cap.string().c_str(), "wb");
    _dup2(_fileno(t), _fileno(stdout));
    if (c.menu == 5) {
        menu5_objects(rec.menu5());
    } else {
        menu6_inter_object(rec.menu6());
    }
    std::fflush(stdout);
    _dup2(saved, _fileno(stdout));
    _close(saved);
    std::fclose(t);
    std::cin.rdbuf(old);
    std::cin.clear();
    std::FILE* k = std::fopen(keyp.string().c_str(), "w");
    def::cmd::write_key_stream(m, k);
    std::fclose(k);
    Run r{cmdoracle::slurp(cap), cmdoracle::slurp(keyp)};
    std::error_code ec;
    fs::remove(seedp, ec);
    fs::remove(cap, ec);
    fs::remove(keyp, ec);
    std::string o; // the text-mode capture: CRLF -> LF
    for (char ch : r.out) if (ch != '\r') o += ch;
    r.out = o;
    std::string kk;
    for (char ch : r.key) if (ch != '\r') kk += ch;
    r.key = kk;
    return r;
}

std::string lf(const std::string& s)
{
    std::string o;
    for (char ch : s) if (ch != '\r') o += ch;
    return o;
}

std::string first_diff(const std::string& a, const std::string& b)
{
    std::size_t i = 0;
    while (i < a.size() && i < b.size() && a[i] == b[i]) ++i;
    const std::size_t from = i < 200 ? 0 : i - 200;
    return "    at byte " + std::to_string(i) + "\n    orig: [" + a.substr(from, 400) + "]\n    port: [" +
           b.substr(from, 400) + "]\n";
}

} // namespace

TEST_CASE(menu56_replay_against_oracle)
{
    const std::string env_repo = cmdoracle::env("DEF_PORT_REPO", "");
    const fs::path repo = !env_repo.empty()
                              ? fs::path(env_repo)
                              : fs::weakly_canonical(fs::path(__FILE__)).parent_path().parent_path().parent_path().parent_path();
    const std::vector<Case> cases = load_cases(repo / "tests" / "menus" / "MENU5_6" / "cases.json");
    CHECK(!cases.empty());
    const auto cache = scan_cache();
    if (cache.empty()) {
        std::printf("no oracle cache entries for MENU5_6: skipped\n");
        return;
    }
    def::pre::ext::g_hooks.select = nullptr;   // p5-01 select_prompt (140007d30)
    const bool verbose = cmdoracle::env("MENU56_VERBOSE", "0") == "1";
    const std::string only = cmdoracle::env("MENU56_ONLY", "");
    // baseline gaps per seed: records that differ between port and oracle with no edit
    std::map<std::string, std::set<std::string>> gaps;
    for (const Case& c : cases) {
        if (c.label.find("_baseline_") == std::string::npos) continue;
        auto it = cache.find(c.label);
        if (it == cache.end()) continue;
        const Run r = replay(c);
        auto a = key_records(lf(cmdoracle::slurp(it->second / "files" / "OUT.KEY")));
        auto b = key_records(r.key);
        for (auto& [k, v] : a) if (b[k] != v) gaps[c.seed].insert(k);
        for (auto& [k, v] : b) if (a[k] != v) gaps[c.seed].insert(k);
    }
    int pass = 0, fail = 0, skipped = 0;
    std::string report;
    for (const Case& c : cases) {
        if (!only.empty() && c.label.find(only) == std::string::npos) continue;
        auto it = cache.find(c.label);
        if (it == cache.end()) {
            ++skipped;
            continue;
        }
        // BCC generation runs GENCTC 3D (1401a6c80, p67-6) with p67-7's cube routines: in the
        // build since the p9/p10 integration (the MENU56_GENCTC opt-in is gone)
        const Run r = replay(c);
        const std::string want = oracle_slice(lf(cmdoracle::slurp(it->second / "stdout.txt")));
        bool ok = true;
        std::string why;
        if (want != r.out) {
            ok = false;
            why += "  stdout differs\n" + first_diff(want, r.out);
        }
        auto a = key_records(lf(cmdoracle::slurp(it->second / "files" / "OUT.KEY")));
        auto b = key_records(r.key);
        std::set<std::string> keys;
        for (auto& [k, v] : a) keys.insert(k);
        for (auto& [k, v] : b) keys.insert(k);
        int differ = 0;
        for (auto& k : keys) {
            if (a[k] == b[k] || gaps[c.seed].count(k) != 0) continue;
            ++differ;
            why += "  record " + k + " differs\n";
            if (verbose) why += "    orig: " + a[k] + "    port: " + b[k];
        }
        if (differ != 0) ok = false;
        if (ok) {
            ++pass;
        } else {
            ++fail;
            report += c.label + "\n" + why;
        }
    }
    def::pre::ext::g_hooks.select = nullptr;
    std::printf("menu 5-6 replay: %d pass, %d fail, %d skipped; baseline gaps 3d %zu, 2d %zu\n%s", pass, fail,
                skipped, gaps["UPSET_Brick.KEY"].size(), gaps["SPIKE_SI.KEY"].size(), report.c_str());
    CHECK_EQ(fail, 0);
}

int main() { return testkit::run_all(); }
