// ctest-labels: kw-g4b
// Group 4 round trips against the original program: every kfrw/ corpus case (KFREAD of a corpus
// KEY file, then KFWRIT) whose material blocks were extracted by kw_g4_cases.py. The input is the
// file's comment lines and group 4 blocks; the expected text is the material part of the
// original's KFWRIT_OUT.KEY, byte for byte (CR LF, three-digit exponents).
//
// Case directory: $KW_G4_CASES, else %USERPROFILE%/.deform-port-runs/kw_g4 (made by
// `python -I port/defkey/tests/kw_g4_cases.py`). Without it the test reports SKIP and passes.
#include "kw_g4_support.hpp"

#include <testkit/check.hpp>

#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>

namespace fs = std::filesystem;

namespace {

std::string slurp(const fs::path& p)
{
    std::ifstream in(p, std::ios::binary);
    std::ostringstream s;
    s << in.rdbuf();
    return s.str();
}

fs::path case_dir()
{
    if (const char* d = std::getenv("KW_G4_CASES")) return d;
    const char* home = std::getenv("USERPROFILE");
    return fs::path(home != nullptr ? home : ".") / ".deform-port-runs" / "kw_g4";
}

// first differing line, for the failure report
std::string first_diff(const std::string& a, const std::string& b)
{
    std::size_t i = 0;
    while (i < a.size() && i < b.size() && a[i] == b[i]) ++i;
    std::size_t s = a.rfind('\n', i == 0 ? 0 : i - 1);
    s = (s == std::string::npos) ? 0 : s + 1;
    auto line = [&](const std::string& t) {
        std::size_t e = t.find('\n', s);
        return t.substr(s, (e == std::string::npos ? t.size() : e) - s);
    };
    return "got [" + line(a) + "] want [" + line(b) + "]";
}

} // namespace

TEST_CASE(corpus_material_round_trips)
{
    keytest::legacy_printf();
    const fs::path dir = case_dir();
    std::ifstream index(dir / "index.tsv");
    if (!index) {
        std::printf("SKIP: no case index in %s\n", dir.string().c_str());
        return;
    }
    std::FILE* log = std::fopen(keytest::tmp_name("kw_g4_corpus.log").c_str(), "w");
    int n = 0, pass = 0;
    std::string row;
    while (std::getline(index, row)) {
        if (row.empty()) continue;
        const std::string num = row.substr(0, row.find('\t'));
        const std::string id = row.substr(row.find('\t') + 1, row.rfind('\t') - row.find('\t') - 1);
        const std::string in = slurp(dir / (num + ".in"));
        const std::string exp = slurp(dir / (num + ".exp"));
        kwg4::Model model;
        kwg4::Hooks hooks(model.m());
        std::fprintf(log, "== %s\n", id.c_str());
        kwg4::read_key_text(model.m(), in, hooks, log);
        const std::string got = kwg4::text_output("kw_g4_out.KEY", [&](std::FILE* f) {
            kwg4::write_all_materials(f, model.m());
        });
        ++n;
        if (got == exp && hooks.other_groups == 0) {
            ++pass;
        }
        else {
            CHECK(false);
            std::fprintf(stderr, "FAIL %s: %s\n", id.c_str(), first_diff(got, exp).c_str());
        }
    }
    std::fclose(log);
    std::remove(keytest::tmp_name("kw_g4_corpus.log").c_str());
    std::printf("%d of %d corpus cases reproduce the original's material blocks\n", pass, n);
}

int main() { return testkit::run_all(); }
