// ctest-labels: units corpus kw-merge
// UNIT end to end (kw-g1a's UNIT case -> FUN_140235c30 set_unit_system -> the unit flag the
// material ctor reads through FUN_140235c10) against the original: every kw-g4b corpus case (see
// test_kw_g4_corpus.cpp) whose source KEY file sets UNIT is read with that UNIT line in front of
// its group 4 blocks, and the material blocks must equal the original's KFWRIT output. As a
// control, the same input without the UNIT line is counted when its output differs, which shows
// how many cases depend on the unit system being applied.
// Without the case directory ($KW_G4_CASES or %USERPROFILE%/.deform-port-runs/kw_g4) the test
// passes with a note.
#include "kw_g4_support.hpp"

#include <testkit/check.hpp>

#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>

#include <defkey/kw_g1a.hpp>
#include <defmodel/keyword_registry.hpp>
#include <defrt/ftn/units.hpp>
#include <defrt/ftn/unit_system.hpp>

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

// group 1 UNIT goes to kw-g1a's parser (the real UNIT case), group 4 to kw-g4b's
struct UnitHooks : kwg4::Hooks {
    def::model::DB_STEP_DATA& m;
    int unit_id = def::model::keyword_id("UNIT");
    explicit UnitHooks(def::model::DB_STEP_DATA& md) : kwg4::Hooks(md), m(md) {}
    int parse_keyword(int group, std::FILE* f, int id) override
    {
        if (group == 1 && id == unit_id) return def::key::g1a::read_keyword(m, f, id);
        return kwg4::Hooks::parse_keyword(group, f, id);
    }
};

// the first "UNIT n" line of the source, or -1
int unit_of(const std::string& text)
{
    std::istringstream in(text);
    std::string line;
    while (std::getline(in, line)) {
        if (line.rfind("UNIT", 0) == 0 && (line.size() == 4 || line[4] == ' ' || line[4] == '\t' || line[4] == '\r'))
            return std::atoi(line.c_str() + 4);
    }
    return -1;
}

// the input with "UNIT n" inserted after the leading comment lines
std::string with_unit(const std::string& in, int unit)
{
    std::size_t pos = 0;
    while (pos < in.size() && in[pos] == '*') {
        std::size_t e = in.find('\n', pos);
        if (e == std::string::npos) return in + "\nUNIT " + std::to_string(unit) + "\n";
        pos = e + 1;
    }
    return in.substr(0, pos) + "UNIT " + std::to_string(unit) + "\n" + in.substr(pos);
}

std::string run(const std::string& input)
{
    def::rt::ftn::reset_unit_globals();  // a fresh process: flag 0
    kwg4::Model model;
    UnitHooks hooks(model.m());
    std::FILE* log = std::fopen(keytest::tmp_name("kw_unit_corpus.log").c_str(), "w");
    kwg4::read_key_text(model.m(), input, hooks, log);
    std::fclose(log);
    std::remove(keytest::tmp_name("kw_unit_corpus.log").c_str());
    return kwg4::text_output("kw_unit_out.KEY", [&](std::FILE* f) { kwg4::write_all_materials(f, model.m()); });
}

} // namespace

TEST_CASE(unit_system_from_key_matches_oracle)
{
    keytest::legacy_printf();
    const fs::path dir = case_dir();
    const fs::path manifest = dir / "index.tsv";
    if (!fs::exists(manifest)) {
        std::printf("SKIP: no kw-g4 case directory at %s\n", dir.string().c_str());
        return;
    }
    const fs::path corpus = "C:/Users/alext/Projects/DEFORM_corpus";
    std::ifstream mf(manifest);
    std::string line;
    int n = 0, pass = 0, english = 0, depends = 0;
    while (std::getline(mf, line)) {
        std::istringstream ls(line);
        std::string num, label;
        std::getline(ls, num, '\t');
        std::getline(ls, label, '\t');
        if (label.rfind("kfrw/", 0) != 0) continue;
        const fs::path src = corpus / label.substr(5);
        if (!fs::exists(src)) continue;
        const int unit = unit_of(slurp(src));
        if (unit < 0) continue;
        const std::string in = slurp(dir / (num + ".in"));
        const std::string exp = slurp(dir / (num + ".exp"));
        ++n;
        if (unit == 2) ++english;
        const std::string got = run(with_unit(in, unit));
        if (got == exp) {
            ++pass;
        } else {
            CHECK(false);
            std::fprintf(stderr, "FAIL %s (UNIT %d)\n", label.c_str(), unit);
        }
        if (run(in) != exp) ++depends;
    }
    std::printf("%d of %d corpus cases with UNIT (%d English) reproduce the material blocks; "
                "%d of them differ when UNIT is not applied\n",
                pass, n, english, depends);
}

int main() { return testkit::run_all(); }
