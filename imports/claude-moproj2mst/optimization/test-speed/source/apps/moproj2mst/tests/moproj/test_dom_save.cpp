#include "../support/test_paths.hpp"
// .moproj writer: QDomDocument::toString(1) round trip. A .moproj the GUI saved is
// doc.toString(1) in UTF-8 of the DOM it edited (0x1400983c0), and reading such a file back gives
// the same DOM (whitespace-only text is dropped, attributes are set in file order). So every
// GUI-saved .moproj must come back byte for byte after setContent + toString(1). Files: every
// .moproj of the oracle B folders (all saved by DEF_GUI_MO_64 v11.0.2) and of the corpus.
// ctest-labels: m2m-moproj
#include <testkit/check.hpp>

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <string>
#include <system_error>
#include <vector>

#include "m2m/qt4.hpp"
#include "m2m/xml.hpp"

namespace fs = std::filesystem;
using namespace m2m;

namespace {

std::string readAll(const fs::path& f)
{
    std::ifstream in(f, std::ios::binary);
    return std::string(std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>());
}

fs::path oracleB()
{
    return m2m_test::configuredPath("M2M_ORACLE_B");
}

// first differing line, for the log
std::string firstDiff(const std::string& a, const std::string& b)
{
    std::size_t i = 0;
    while (i < a.size() && i < b.size() && a[i] == b[i]) ++i;
    std::size_t s = a.rfind('\n', i);
    s = s == std::string::npos ? 0 : s + 1;
    auto line = [&](const std::string& t) { return t.substr(s, std::min<std::size_t>(t.find('\n', s) - s, 160)); };
    return "port: " + line(a) + "\n   file: " + line(b);
}

// The writer creates some elements with an empty text child (<SubType></SubType>); reading such an
// element back gives no text node (setContent makes none for empty character data), so its next
// toString is <SubType/>. The expected bytes take that form.
std::string emptyElementsCollapsed(const std::string& s)
{
    std::string r;
    std::size_t i = 0;
    for (;;) {
        std::size_t p = s.find("></", i);
        if (p == std::string::npos) break;
        std::size_t lt = s.rfind('<', p);
        std::size_t te = s.find('>', p + 3);
        bool collapse = false;
        if (lt != std::string::npos && lt >= i && s[lt + 1] != '/' && te != std::string::npos) {
            std::size_t ne = lt + 1;
            while (ne < p && s[ne] != ' ' && s[ne] != '\t' && s[ne] != '\n') ++ne;
            collapse = s.compare(lt + 1, ne - lt - 1, s, p + 3, te - p - 3) == 0;
        }
        if (collapse) {
            r.append(s, i, p - i);
            r += "/>";
            i = te + 1;
        } else {
            r.append(s, i, p + 1 - i);
            i = p + 1;
        }
    }
    r.append(s, i, std::string::npos);
    return r;
}

}  // namespace

TEST_CASE(gui_saved_moproj_round_trip)
{
    const fs::path ob = oracleB();
    REQUIRE(!ob.empty() && fs::exists(ob));
    int n = 0, same = 0;
    // Only <oracle_b>/<case>/*.moproj is read. Other tests and the oracle runner create and delete
    // case folders under oracle_b while this runs (ctest -j), so the walk is one level deep and
    // uses the error_code overloads: a folder that vanishes mid-walk is skipped, not a failure.
    std::vector<fs::path> files;
    std::error_code ec;
    for (fs::directory_iterator c(ob, ec), end; !ec && c != end; c.increment(ec)) {
        std::error_code ec2;
        if (!c->is_directory(ec2)) continue;
        // pre-open copies made by scripts, not by the GUI
        const std::string dir = c->path().filename().string();
        if (dir.size() > 7 && dir.compare(dir.size() - 7, 7, "_before") == 0) continue;
        for (fs::directory_iterator f(c->path(), ec2), fend; !ec2 && f != fend; f.increment(ec2)) {
            std::error_code ec3;
            if (f->is_regular_file(ec3) && f->path().extension() == ".moproj") files.push_back(f->path());
        }
    }
    std::sort(files.begin(), files.end());
    for (const fs::path& path : files) {
        const std::string file = readAll(path);
        if (file.empty() && !fs::exists(path)) continue;   // removed since the listing
        const std::string bytes = emptyElementsCollapsed(file);
        xml::Document doc;
        qt::String err;
        int line = 0, col = 0;
        if (!xml::setContent(doc, file, &err, &line, &col)) {
            std::printf("PARSE %s: %s\n", path.string().c_str(), err.utf8().c_str());
            CHECK(false);
            continue;
        }
        const std::string out = qt::String(xml::toString(doc, 1)).utf8();
        ++n;
        if (out == bytes) {
            ++same;
        } else {
            std::printf("DIFF %s (%zu vs %zu bytes)\n   %s\n", path.string().c_str(), out.size(), bytes.size(),
                        firstDiff(out, bytes).c_str());
        }
    }
    std::printf("%d of %d GUI-saved .moproj round trip\n", same, n);
    CHECK(n > 0);
    CHECK_EQ(same, n);
}

int main()
{
    return testkit::run_all();
}
