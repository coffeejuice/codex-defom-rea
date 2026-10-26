// P07 corpus test: for every oracle-A case of $DEF_PORT_REPO/docs/re/moproj2mst/runner/cases.tsv
// found under $M2M_CORPUS, the header + main block (0x140131680, 0x1401318b0) equal the saved MST's
// first bytes and the trailer (0x140131d10) equals its end. Operation bodies are not written.
// Needs the open path (P01-P03) and the stream/paths (P04/P05).
#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "m2m/mst/emit.hpp"
#include "m2m/qt4path.hpp"

using namespace m2m;
using namespace m2m::mst;

namespace {

int g_fail = 0;

void check(bool ok, const std::string& what)
{
    if (!ok) {
        ++g_fail;
        std::printf("FAIL %s\n", what.c_str());
    }
}

std::filesystem::path envPath(const wchar_t* name)
{
    const wchar_t* v = _wgetenv(name);
    return v ? std::filesystem::path(v) : std::filesystem::path();
}

std::string readFile(const std::filesystem::path& f)
{
    std::ifstream in(f, std::ios::binary);
    std::ostringstream s;
    s << in.rdbuf();
    return s.str();
}

std::vector<std::string> splitTab(const std::string& line)
{
    std::vector<std::string> v;
    size_t a = 0;
    for (;;) {
        size_t b = line.find('\t', a);
        v.push_back(line.substr(a, b == std::string::npos ? std::string::npos : b - a));
        if (b == std::string::npos) break;
        a = b + 1;
    }
    return v;
}

std::filesystem::path findFile(const std::filesystem::path& dir, const std::string& name)
{
    std::error_code ec;
    for (auto it = std::filesystem::recursive_directory_iterator(dir, ec);
         !ec && it != std::filesystem::recursive_directory_iterator(); it.increment(ec))
        if (it->is_regular_file() && it->path().filename().string() == name) return it->path();
    return {};
}

}  // namespace

int main()
{
    const auto repo = envPath(L"DEF_PORT_REPO");
    const auto corpusDir = envPath(L"M2M_CORPUS");
    if (repo.empty() || corpusDir.empty()) {
        std::printf("FAIL DEF_PORT_REPO and M2M_CORPUS must be set\n");
        return 1;
    }
    std::ifstream in(repo / "docs/re/moproj2mst/runner/cases.tsv", std::ios::binary);
    std::string line;
    if (!std::getline(in, line)) {
        std::printf("FAIL cannot read cases.tsv\n");
        return 1;
    }
    if (!line.empty() && line.back() == '\r') line.pop_back();
    const auto cols = splitTab(line);
    auto col = [&](const char* n) {
        for (size_t i = 0; i < cols.size(); ++i)
            if (cols[i] == n) return i;
        return size_t(-1);
    };
    const size_t cId = col("case_id"), cMoproj = col("moproj"), cMst = col("mst"), cOracle = col("oracle");
    int pass = 0, skip = 0;
    while (std::getline(in, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();
        const auto f = splitTab(line);
        if (f.size() < cols.size()) continue;
        if (f[cOracle].rfind("A-excluded", 0) == 0) {          // cases.tsv exclusion column
            ++skip;
            std::printf("SKIP %s (%s)\n", f[cId].c_str(), f[cOracle].c_str());
            continue;
        }
        if (f[cOracle] != "A") continue;
        const auto dir = corpusDir / f[cId];
        const auto moproj = findFile(dir, f[cMoproj]);
        const auto mstFile = findFile(dir, f[cMst]);
        if (moproj.empty() || mstFile.empty()) {
            ++skip;
            std::printf("SKIP %s (not in the corpus copy)\n", f[cId].c_str());
            continue;
        }
        const std::string oracle = readFile(mstFile);
        try {
            Ctx ctx;
            Project p;
            const String file{moproj.generic_u16string()};
            // as G0's driver (src/core/convert.cpp): data root, ctor, registry, open, activate 0
            const String db = app::changeSuffix(file, qt::L(".DB"));
            initProject(p, qt::absolutePath(db), qt::completeBaseName(db));
            makeRegistry(p);
            if (!openProject(p, file, ctx) || !p.openedOk) throw Error(M2M_E_PARSE, "open failed");
            activateSubProject(p, 0, ctx);
            Stream ts(1252);
            State st;
            W w{ts, st, ctx};
            writeHeader(w, p);
            writeMainBlock(w, p);
            const std::string head = ts.bytes();
            Stream tt(1252);
            W wt{tt, st, ctx};
            writeTrailer(wt, p);
            const std::string tail = tt.bytes();
            // After the main block comes a SIMULATION header (starts with ENDL) or the trailer.
            const bool okHead = oracle.size() > head.size() && oracle.compare(0, head.size(), head) == 0 &&
                                oracle[head.size()] == '\n';
            const bool okTail = oracle.size() >= tail.size() &&
                                oracle.compare(oracle.size() - tail.size(), tail.size(), tail) == 0;
            check(okHead, f[cId] + " header/main block");
            check(okTail, f[cId] + " trailer");
            if (okHead && okTail) ++pass;
        } catch (const std::exception& e) {
            check(false, f[cId] + ": " + e.what());
        }
    }
    std::printf("p07 corpus: %d pass, %d failures, %d skipped\n", pass, g_fail, skip);
    return g_fail || pass == 0 ? 1 : 0;
}
