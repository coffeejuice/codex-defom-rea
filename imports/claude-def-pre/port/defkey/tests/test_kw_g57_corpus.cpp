// ctest-labels: kw-g57 corpus
// Corpus check of the group 5/7 reader and writer against cached oracle KEY round trips.
//
// Runs only when DEF_KWG57_CORPUS_MANIFEST names a manifest written by
// tests/tools/kw_g57_corpus_manifest.py (corpus input path, cached KFWRIT output path, label);
// otherwise it passes at once. For each case the corpus KEY file is read into memory (never
// opened for writing), its comment lines and every group 5 and 7 keyword with its data lines are
// kept (a record runs until the next registry keyword or comment line), that text goes through
// the KEY loop and the port's parsers, and the port's "Inter-Material Data" section must equal
// the oracle's byte for byte (CR removed). DEF_KWG57_CORPUS_REPORT optionally names a
// file that receives one line per case.
#include "kw_g57_test_util.hpp"

#include <defmodel/keyword_registry.hpp>
#include <testkit/check.hpp>

#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

namespace {

std::vector<std::string> split_lines(const std::string& s)
{
    std::vector<std::string> v;
    std::string line;
    for (char c : s) {
        if (c == '\n') {
            v.push_back(line);
            line.clear();
        } else if (c != '\r') {
            line += c;
        }
    }
    if (!line.empty()) v.push_back(line);
    return v;
}

std::string first_token(const std::string& line)
{
    std::size_t i = line.find_first_not_of(" \t");
    if (i == std::string::npos) return "";
    std::size_t j = line.find_first_of(" \t", i);
    return line.substr(i, j == std::string::npos ? std::string::npos : j - i);
}

// comment lines + group 5/7 records of a KEY file
std::string group57_text(const std::string& key)
{
    std::string out;
    bool in3 = false;
    for (const std::string& line : split_lines(key)) {
        const std::string tok = first_token(line);
        if (!tok.empty() && tok[0] == '*') {
            out += line + "\n";
            in3 = false;
            continue;
        }
        if (const auto* rec = def::model::find_keyword(tok.c_str())) {
            in3 = rec->group == def::model::KeywordGroup::PhaseTransformation ||
                  rec->group == def::model::KeywordGroup::Dimension;
        }
        if (in3) out += line + "\n";
    }
    return out;
}

// the oracle's Inter-Material section: its banner and the pair blocks, up to the next banner
// that is not a pair banner
std::string inter_material_section(const std::string& out)
{
    const std::vector<std::string> lines = split_lines(out);
    std::string s;
    for (std::size_t i = 0; i < lines.size(); ++i) {
        if (lines[i] != "*  Inter-Material Data") continue;
        s = "*\n*  Inter-Material Data\n*\n";
        for (std::size_t j = i + 2; j < lines.size(); ++j) {
            if (lines[j] == "*" && j + 1 < lines.size() && lines[j + 1].rfind("*  ", 0) == 0 &&
                lines[j + 1].rfind("*  Inter-Material Data for", 0) != 0)
                break;
            s += lines[j] + "\n";
        }
        break;
    }
    return s;
}

} // namespace

TEST_CASE(corpus_round_trips)
{
    const char* manifest = std::getenv("DEF_KWG57_CORPUS_MANIFEST");
    if (manifest == nullptr) {
        std::printf("DEF_KWG57_CORPUS_MANIFEST not set: corpus check skipped\n");
        return;
    }
    const char* report_path = std::getenv("DEF_KWG57_CORPUS_REPORT");
    std::ofstream report;
    if (report_path != nullptr) report.open(report_path, std::ios::binary);
    std::ifstream in(manifest);
    std::string row;
    int cases = 0, same = 0;
    while (std::getline(in, row)) {
        std::istringstream fields(row);
        std::string src, out, label;
        std::getline(fields, src, '\t');
        std::getline(fields, out, '\t');
        std::getline(fields, label, '\t');
        ++cases;
        const std::string expected = inter_material_section(g57test::read_file(out));
        g57test::ModelPair models;
        g57test::read_into(models, group57_text(g57test::read_file(src)), "g57corp", nullptr);
        int st = 0;
        const std::string written =
            g57test::write_section(def::model::keyword_target_model(models.primary.get()), "g57corp", &st);
        const bool ok = st == 0 && written == expected;
        if (ok) ++same;
        if (report.is_open()) report << (ok ? "same\t" : "DIFF\t") << label << "\n";
        if (!ok) std::printf("DIFF %s\n", label.c_str());
    }
    std::printf("%d of %d corpus cases identical\n", same, cases);
    CHECK_EQ(same, cases);
}

int main() { return testkit::run_all(); }
