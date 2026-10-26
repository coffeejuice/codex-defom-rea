// P04 golden check against the corpus: every number token in every saved MST (oracle A) must be
// what QTextStream << double / << int prints for the value it denotes. A token t passes when
// formatDouble(v) == t for v = strtod(t) or for v = (double)(float)strtod(t) (most MST values
// come from float fields), or formatInt(v) == t for an integer token. Tokens that cannot be Qt
// number output (more than 6 significant digits, zero-padded ints from QString::arg, numbers
// inside names) and the writers' literal "0.0" text are counted separately; the first ones are
// listed when M2M_P04_VERBOSE is set.
// ctest-labels: corpus
#include <testkit/check.hpp>

#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <map>
#include <regex>
#include <sstream>
#include <string>

#include "m2m/qt4.hpp"

namespace fs = std::filesystem;
using namespace m2m::qt;

namespace {

int significant_digits(const std::string& t)
{
    int n = 0;
    bool lead = true;
    std::string m = t.substr(0, t.find('e'));
    // trailing zeros after a point do not count; leading zeros never count
    for (char c : m) {
        if (c < '0' || c > '9') continue;
        if (lead && c == '0') continue;
        lead = false;
        ++n;
    }
    return n;
}

}  // namespace

TEST_CASE(corpus_numbers)
{
    const char* corpus = std::getenv("M2M_CORPUS");
    if (!corpus || !fs::exists(corpus)) {
        std::printf("M2M_CORPUS not set or missing: skipped\n");
        return;
    }
    const bool verbose = std::getenv("M2M_P04_VERBOSE") != nullptr;
    const std::regex num(R"(^[+-]?(\d+\.?\d*|\.\d+)([eE][+-]?\d+)?$)");
    long files = 0, total = 0, literal = 0, as_double = 0, as_float = 0, as_int = 0, other = 0, bad = 0;
    std::map<std::string, int> others;
    for (const auto& e : fs::recursive_directory_iterator(corpus)) {
        if (!e.is_regular_file()) continue;
        std::string ext = e.path().extension().string();
        for (auto& c : ext) c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
        if (ext != ".MST") continue;
        ++files;
        std::ifstream in(e.path(), std::ios::binary);
        std::string line;
        while (std::getline(in, line)) {
            std::istringstream ls(line);
            std::string t;
            while (ls >> t) {
                if (!std::regex_match(t, num)) continue;
                ++total;
                if (t == "0.0") {   // literal text of the Cogging writer (s1-03 4.x, 1425e47b8 ...)
                    ++literal;
                    continue;
                }
                const double v = std::strtod(t.c_str(), nullptr);
                const bool is_int = t.find_first_of(".eE") == std::string::npos;
                if (is_int && t.size() < 11 && formatInt(std::atoi(t.c_str())) == t) {
                    ++as_int;
                } else if (formatDouble(v) == t) {
                    ++as_double;
                } else if (formatDouble(static_cast<double>(static_cast<float>(v))) == t) {
                    ++as_float;
                } else if (significant_digits(t) > 6 || (is_int && t.size() > 1 && t[0] == '0')
                           || t.find('E') != std::string::npos || t[0] == '+') {
                    ++other;
                    if (others.size() < 40) others[t]++;
                } else {
                    ++bad;
                    std::printf("MISMATCH %s: '%s' -> '%s'\n", e.path().filename().string().c_str(), t.c_str(),
                                formatDouble(v).c_str());
                }
            }
        }
    }
    std::printf("%ld MSTs, %ld number tokens: %ld int, %ld double, %ld float-widened, %ld literal "
                "\"0.0\", %ld not Qt number output, %ld mismatches\n",
                files, total, as_int, as_double, as_float, literal, other, bad);
    if (verbose)
        for (const auto& [t, n] : others) std::printf("  other: %s x%d\n", t.c_str(), n);
    CHECK(files > 0);
    CHECK_EQ(bad, 0L);
}

int main() { return testkit::run_all(); }
