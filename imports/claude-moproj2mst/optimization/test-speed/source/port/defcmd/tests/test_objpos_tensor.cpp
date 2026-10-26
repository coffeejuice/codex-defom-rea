// ctest-labels: p67-1 objpos oracle
// The element-tensor rotation of OBJPOS mode 3 (FUN_140184ac0 -> FUN_140325884 / FUN_14032e585)
// against the original's KEY output, directly on the records (the STRESS / STNCMP keyword reader
// belongs to kw-g2sv, which is not on this branch yet): STRESS and STNCMP of object 1 are parsed
// from the seed (the KEY reader converts with %lf, i.e. correctly rounded, as strtod does),
// rotated through def::cmd::objpos::elems_rotate with the case's arguments, formatted with the
// writer's " %15.7E" and compared token by token with the OUT.KEY of the cached oracle runs
// OBJPOS/gk-tensor-y_3d, gk-tensor-obl_3d, v-bits-rot_3d (3D, Spike_ForgingBlow2.KEY) and
// gk-tensor_2d, v-tensor_2d (2D, the Resistance_Spot_Welding seed). Without the cache the test passes with a note.
#define _CRT_SECURE_NO_WARNINGS
#include "../../defkey/tests/key_test_util.hpp"

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include <defcmd/objpos_parts.hpp>
#include <defrt/stdlib/legacy_printf.hpp>
#include <testkit/check.hpp>

namespace fs = std::filesystem;

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

// The value tokens of the record `kw obj ...` (header excluded), per element in order.
std::vector<std::vector<std::string>> record_tokens(const std::string& text, const char* kw, int obj, int* count)
{
    std::vector<std::vector<std::string>> el;
    auto ls = lines_of(text);
    for (std::size_t i = 0; i < ls.size(); ++i) {
        char name[64] = {0};
        int o = 0, n = 0;
        if (std::sscanf(ls[i].c_str(), "%63s %d %d", name, &o, &n) < 3 || std::strcmp(name, kw) != 0 || o != obj) continue;
        *count = n;
        for (std::size_t j = i + 1; j < ls.size() && !ls[j].empty() && ls[j][0] == ' '; ++j) {
            std::istringstream is(ls[j]);
            std::string t;
            const bool head = ls[j].size() > 8 && ls[j][7] != ' ';
            if (head) {
                is >> t;  // element number
                el.emplace_back();
            }
            while (is >> t) el.back().push_back(t);
        }
        break;
    }
    return el;
}

// label -> the newest cached run (a label is re-run when its inputs change)
std::map<std::string, fs::path> scan(const fs::path& cache)
{
    std::map<std::string, std::pair<std::string, fs::path>> e;
    for (auto& d1 : fs::directory_iterator(cache / "entries")) {
        if (!d1.is_directory()) continue;
        for (auto& d2 : fs::directory_iterator(d1.path())) {
            const std::string j = slurp(d2.path() / "result.json");
            const std::size_t p = j.find("\"label\": \"OBJPOS/");
            if (p == std::string::npos) continue;
            const std::size_t a = j.find("OBJPOS/", p);
            const std::string label = j.substr(a, j.find('"', a) - a);
            const std::size_t c = j.find("\"created\": \"");
            const std::string created = c == std::string::npos ? "" : j.substr(c + 12, 19);
            auto it = e.find(label);
            if (it == e.end() || it->second.first < created) e[label] = {created, d2.path()};
        }
    }
    std::map<std::string, fs::path> r;
    for (auto& [k, v] : e) r[k] = v.second;
    return r;
}

const char* const kRsw =
    "key/2D/SI/ResistanceSpotWelding/Resistance_Spot_Welding_elstoplastic_model_with_phase_transformation_Hold_cycle_SI.KEY";

struct Case {
    const char* label;
    const char* seed;
    int dim;
    double origin[3];
    double dir[3];
    double angle;
};

// An element set as 140184ac0 reads it: data slots at +0x20 (STRESS), +0x30 (STNCMP), +0xb8
// (YLDS), +0x60 (MATAXI), count +0xe8, owner +0xf0 (widths at owner+0x15f8: +0x98 STRESS,
// +0xa0 STNCMP, +0xe4 YLDS, +0xb8 MATAXI; GEOTYP at owner+0x850), dimension +0xf8.
int check(const Case& c, const fs::path& oracle_dir, const fs::path& corpus)
{
    const std::string seed = slurp(corpus / c.seed);
    const std::string out = slurp(oracle_dir / "files" / "OUT.KEY");
    alignas(16) unsigned char es[0x108] = {};
    std::vector<unsigned char> owner(0x1800, 0);
    int n_s = 0, n_e = 0, n_o = 0;
    auto s_tok = record_tokens(seed, "STRESS", 1, &n_s);
    auto e_tok = record_tokens(seed, "STNCMP", 1, &n_e);
    const int ws = static_cast<int>(s_tok.empty() ? 0 : s_tok[0].size());
    const int we = static_cast<int>(e_tok.empty() ? 0 : e_tok[0].size());
    std::vector<double> s(static_cast<std::size_t>(n_s * ws)), e(static_cast<std::size_t>(n_e * we));
    for (int i = 0; i < n_s; ++i)
        for (int k = 0; k < ws; ++k) s[i * ws + k] = std::strtod(s_tok[i][k].c_str(), nullptr);
    for (int i = 0; i < n_e; ++i)
        for (int k = 0; k < we; ++k) e[i * we + k] = std::strtod(e_tok[i][k].c_str(), nullptr);
    *reinterpret_cast<double**>(es + 0x20) = s.data();
    *reinterpret_cast<double**>(es + 0x30) = e.data();
    *reinterpret_cast<std::int32_t*>(es + 0xe8) = n_s;
    *reinterpret_cast<unsigned char**>(es + 0xf0) = owner.data();
    *reinterpret_cast<std::int32_t*>(es + 0xf8) = c.dim;
    *reinterpret_cast<std::int32_t*>(owner.data() + 0x15f8 + 0x98) = ws;
    *reinterpret_cast<std::int32_t*>(owner.data() + 0x15f8 + 0xa0) = we;
    CHECK_EQ(n_s, n_e);
    def::cmd::objpos::elems_rotate(es, c.origin, c.dim == 3 ? c.dir : nullptr, c.angle);
    int bad = 0, total = 0;
    for (const char* kw : {"STRESS", "STNCMP"}) {
        auto o_tok = record_tokens(out, kw, 1, &n_o);
        const std::vector<double>& v = std::strcmp(kw, "STRESS") == 0 ? s : e;
        const int w = std::strcmp(kw, "STRESS") == 0 ? ws : we;
        for (int i = 0; i < n_o && i < n_s; ++i) {
            for (int k = 0; k < w; ++k) {
                // the writer's precision (STRESS/STNCMP print 10 decimals)
                const std::string& ot = o_tok[i][k];
                const int prec = static_cast<int>(ot.find('E') - ot.find('.') - 1);
                char buf[64];
                def::rt::crt::snprintf(buf, sizeof buf, "%.*E", prec, v[i * w + k]);
                std::string t = buf;
                t.erase(0, t.find_first_not_of(' '));
                ++total;
                if (t != o_tok[i][k]) {
                    if (bad < 5) std::printf("  %s %s elem %d comp %d: port %s orig %s\n", c.label, kw, i + 1, k, t.c_str(), o_tok[i][k].c_str());
                    ++bad;
                }
            }
        }
    }
    std::printf("%-26s %d of %d values equal\n", c.label, total - bad, total);
    return bad;
}

} // namespace

TEST_CASE(tensor_rotation_matches_the_oracle_records)
{
    keytest::legacy_printf();
    fs::path cache = env("DEFORM_ORACLE_CACHE", "");
    if (cache.empty()) cache = fs::path(env("USERPROFILE", "")) / ".deform-oracle";
    const fs::path corpus = env("DEFORM_CORPUS", "C:/Users/alext/Projects/DEFORM_corpus");
    if (!fs::exists(cache / "entries") || !fs::exists(corpus)) {
        std::printf("no oracle cache or corpus: skipped\n");
        return;
    }
    const auto entries = scan(cache);
    const Case cases[] = {
        {"OBJPOS/gk-tensor-y_3d", "key/3D/EN/Forging/Spike_ForgingBlow2.KEY", 3, {0, 0, 0}, {0, 1, 0}, 30.0},
        {"OBJPOS/gk-tensor-obl_3d", "key/3D/EN/Forging/Spike_ForgingBlow2.KEY", 3, {1, 2, 3}, {1, 1, 1}, -37.5},
        {"OBJPOS/gk-tensor_2d", kRsw, 2, {0, 0, 0}, {0, 0, 0}, 30.0},
        {"OBJPOS/v-tensor_2d", kRsw, 2, {0, 0, 0}, {0, 0, 0}, 40.6},
        {"OBJPOS/v-dir0_3d", "key/3D/EN/Forging/Spike_ForgingBlow2.KEY", 3, {0, 0, 0}, {0, 0, 0}, 30.0},
        {"OBJPOS/v-bits-rot_3d", "key/3D/EN/Forging/Spike_ForgingBlow2.KEY", 3, {1, 2, 3}, {1, 1, 1}, 40.6},
    };
    int bad = 0;
    for (const Case& c : cases) {
        auto it = entries.find(c.label);
        if (it == entries.end()) {
            std::printf("%s: no oracle entry, skipped\n", c.label);
            continue;
        }
        bad += check(c, it->second, corpus);
    }
    CHECK_EQ(bad, 0);
}

int main() { return testkit::run_all(); }
