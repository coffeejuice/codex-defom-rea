#include "../support/test_paths.hpp"
// .moproj writer: qCompress (zlib 1.2.5 deflate, level 6). Oracle: the blobs DEF_GUI_MO_64 wrote
// into its saved .moproj files (QtCore4's own qCompress). Every <Data>, <ObjPropertiesKey>,
// <SimCtrlKey> and <InterObjectKey> blob of the oracle B and corpus .moproj files is inflated and
// compressed again; the bytes must equal the blob. With M2M_DEFLATE_VECTORS=<dir> the test also
// checks every <name>.in / <name>.out pair there (made with another zlib, e.g. Python's).
// ctest-labels: m2m-moproj
#include <testkit/check.hpp>

#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <set>
#include <string>

#include "m2m/blob.hpp"
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

void collect(const xml::Node& n, std::set<std::string>& out)
{
    for (const auto& c : n.children) {
        if (c->kind != xml::Node::Element) continue;
        const std::string t = c->tag.utf8();
        if (t == "Data" || t == "ObjPropertiesKey" || t == "SimCtrlKey" || t == "InterObjectKey") {
            const std::string h = c->text().utf8();
            if (!h.empty() && h.find('<') == std::string::npos) out.insert(h);
        }
        collect(*c, out);
    }
}

}  // namespace

TEST_CASE(gui_blobs_recompress)
{
    const char* corpus = std::getenv("M2M_CORPUS");
    REQUIRE(corpus != nullptr);
    const fs::path roots[] = {fs::path(corpus), m2m_test::configuredPath("M2M_ORACLE_B")};
    std::set<std::string> blobs;
    for (const fs::path& r : roots) {
        if (!fs::exists(r)) continue;
        for (const auto& e : fs::recursive_directory_iterator(r)) {
            if (!e.is_regular_file() || e.path().extension() != ".moproj") continue;
            xml::Document doc;
            if (!xml::setContent(doc, readAll(e.path()), nullptr, nullptr, nullptr) || !doc.root) continue;
            collect(*doc.root, blobs);
        }
    }
    int n = 0, same = 0;
    std::size_t biggest = 0;
    for (const std::string& h : blobs) {
        const std::string z = qt::fromHex(h);
        const std::string raw = qt::qUncompress(z);
        if (raw.empty()) continue;                       // not a blob (or one the GUI could not read)
        ++n;
        if (raw.size() > biggest) biggest = raw.size();
        const std::string again = qt::qCompress(raw);
        if (again == z) ++same;
        else if (n - same <= 5) std::printf("DIFF blob of %zu bytes (%zu vs %zu compressed)\n", raw.size(), again.size(), z.size());
    }
    std::printf("%d of %d blobs recompress identically (largest %zu bytes)\n", same, n, biggest);
    CHECK(n > 0);
    CHECK_EQ(same, n);
    CHECK(qt::toHex(qt::qCompress(qt::fromHex(blobs.empty() ? std::string() : *blobs.begin()))).size() > 0);
}

TEST_CASE(vectors)
{
    const char* dir = std::getenv("M2M_DEFLATE_VECTORS");
    if (!dir) {
        std::puts("SKIP optional deflate vectors: M2M_DEFLATE_VECTORS unset");
        return;
    }
    int n = 0, same = 0;
    for (const auto& e : fs::directory_iterator(dir)) {
        if (e.path().extension() != ".in") continue;
        fs::path o = e.path();
        o.replace_extension(".out");
        const std::string in = readAll(e.path()), want = readAll(o);
        ++n;
        if (qt::qCompress(in).substr(4) == want) ++same;
        else std::printf("DIFF %s (%zu bytes)\n", e.path().filename().string().c_str(), in.size());
    }
    std::printf("%d of %d vectors\n", same, n);
    CHECK_EQ(same, n);
}

int main()
{
    return testkit::run_all();
}
