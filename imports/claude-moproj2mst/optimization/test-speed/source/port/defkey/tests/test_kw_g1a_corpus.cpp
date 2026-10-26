// ctest-labels: kw-g1a keywords
// Corpus parse check for kw-g1a. Runs only when KWG1A_CORPUS names a folder prepared by
// tests/data/kw_g1a/corpus_expected.py (copies of the corpus KEY files plus DEF_PRE's KFWRIT
// output for each); otherwise it passes without doing anything, so tier 1 stays self-contained.
//
// Each copy is read through the group 1 parser from a fresh model (dimension comments switch the
// model like the original; keywords that are not kw-g1a's are skipped with their line), the two
// sections are written, and the text must equal DEF_PRE's. kw-g1a's own "Reading Error" lines must
// equal DEF_PRE's too.
#include "kw_g1a_support.hpp"

#include <cstdlib>
#include <fstream>
#include <vector>

using namespace kwg1a_test;

namespace {

std::string first_difference(const std::string& a, const std::string& b)
{
    std::istringstream x(a), y(b);
    std::string lx, ly;
    int n = 0;
    while (true) {
        bool gx = static_cast<bool>(std::getline(x, lx));
        bool gy = static_cast<bool>(std::getline(y, ly));
        ++n;
        if (!gx && !gy) return "";
        if (!gx || !gy || lx != ly) {
            return "line " + std::to_string(n) + ": port [" + (gx ? lx : "<eof>") + "] original [" +
                   (gy ? ly : "<eof>") + "]";
        }
    }
}

std::string g1a_errors(const std::string& msgs)
{
    static const char* kws[] = {"TITLE", "CURSIM", "OPRNAM", "SIMNAM", "STYPE", "SMODE", "GEOTYP",
                                "MESHNO", "UNIT", "TNOW", "TRANS", "HTMTHD", "STNOUT", "ELMNOD",
                                "NSTART", "STPINC", "PDIE", "OSCTRL", "NSTEP", "EMAX", "TMAX",
                                "SMAX", "VMIN", "LMAX", "STPDEF", "DEMAX", "DSMAX", "DSMAX_NSTEP",
                                "DTMAX", "DTTYPE", "SLDERR", "DTPMAX", "DVMAX", "MDSOBJ", "CNTERR",
                                "VFACTR", "DTSUB", "DPLEN", "ODMAX"};
    std::istringstream in(msgs);
    std::string ln, out;
    while (std::getline(in, ln)) {
        if (ln.rfind("ERROR: Keyword ", 0) != 0) continue;
        std::string kw = ln.substr(15, ln.find(' ', 15) - 15);
        for (const char* k : kws) {
            if (kw == k) {
                out += ln + "\n";
                break;
            }
        }
    }
    return out;
}

} // namespace

TEST_CASE(corpus_sections_match_def_pre)
{
    const char* root = std::getenv("KWG1A_CORPUS");
    if (root == nullptr) {
        std::printf("KWG1A_CORPUS not set: corpus check skipped\n");
        return;
    }
    std::filesystem::path dir(root);
    std::ifstream manifest(dir / "manifest.tsv");
    REQUIRE(manifest.good());
    std::string row;
    int total = 0, bad = 0;
    while (std::getline(manifest, row)) {
        std::string n = row.substr(0, row.find('\t'));
        std::string id = row.substr(row.find('\t') + 1);
        auto base = dir / "files" / n;
        std::FILE* f = std::fopen((base.string() + ".KEY").c_str(), "r");
        REQUIRE(f != nullptr);
        Model mdl(3);
        std::string msgs = run_key(mdl, f);
        std::fclose(f);
        std::string out = write_sections(mdl.m);
        std::string d = first_difference(out, read_file(base.string() + ".expected"));
        std::string e = g1a_errors(msgs);
        std::string ee = read_file(base.string() + ".msgs");
        ++total;
        if (!d.empty() || e != ee) {
            ++bad;
            std::fprintf(stderr, "%s %s\n", n.c_str(), id.c_str());
            if (!d.empty()) std::fprintf(stderr, "    %s\n", d.c_str());
            if (e != ee)
                std::fprintf(stderr, "    messages: port [%s] original [%s]\n", e.c_str(), ee.c_str());
        }
    }
    std::printf("corpus: %d files, %d differ\n", total, bad);
    CHECK_EQ(bad, 0);
}

int main() { return testkit::run_all(); }
