// P05: Qt 4.8 path functions (QtCore4_64.dll behaviour) and the DEF_GUI_MO path helpers.
#include <testkit/check.hpp>

#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <string>

#include "m2m/qt4path.hpp"

using m2m::qt::String;
namespace qt = m2m::qt;
namespace app = m2m::app;

namespace {
String S(const char* s) { return qt::L(s); }
std::string U(const String& s) { return s.isNull() ? std::string("<null>") : s.utf8(); }
}  // namespace

TEST_CASE(clean_path)
{
    CHECK(qt::cleanPath(String()).isNull());
    CHECK(!qt::cleanPath(S("")).isNull());
    CHECK_EQ(U(qt::cleanPath(S("C:\\PROBLEM\\X\\Objects\\Object00001.KEY"))), "C:/PROBLEM/X/Objects/Object00001.KEY");
    CHECK_EQ(U(qt::cleanPath(S("Operations/Task00004\\Step.KEY"))), "Operations/Task00004/Step.KEY");
    CHECK_EQ(U(qt::cleanPath(S("/a/b/../c"))), "/a/c");
    CHECK_EQ(U(qt::cleanPath(S("a/./b"))), "a/b");
    CHECK_EQ(U(qt::cleanPath(S("./a"))), "a");
    CHECK_EQ(U(qt::cleanPath(S("a//b"))), "a/b");
    CHECK_EQ(U(qt::cleanPath(S("a/../b"))), "b");
    CHECK_EQ(U(qt::cleanPath(S("../a"))), "../a");
    CHECK_EQ(U(qt::cleanPath(S("C:/x/y/"))), "C:/x/y");
    CHECK_EQ(U(qt::cleanPath(S("C:\\"))), "C:/");
    CHECK_EQ(U(qt::cleanPath(S("/"))), "/");
    CHECK_EQ(U(qt::cleanPath(S("//server/share/a"))), "//server/share/a");
    CHECK_EQ(U(qt::cleanPath(S("C:/a/."))), "C:/a");
    CHECK_EQ(U(qt::cleanPath(S("C:/a/b/.."))), "C:/a");
}

TEST_CASE(separators)
{
    CHECK_EQ(U(qt::convertSeparators(S("C:/a/b"))), "C:\\a\\b");
    CHECK_EQ(U(qt::fromNativeSeparators(S("C:\\a\\b"))), "C:/a/b");
    CHECK(qt::convertSeparators(String()).isNull());
}

TEST_CASE(file_info_parts)
{
    CHECK_EQ(U(qt::fileName(S("C:/P/Objects\\Object00001.KEY"))), "Object00001.KEY");
    CHECK_EQ(U(qt::fileName(S("C:/dir/"))), "<null>");   // QString::mid past the end is null
    CHECK_EQ(U(qt::fileName(S("C:a.txt"))), "a.txt");
    CHECK_EQ(U(qt::path(S("C:/x"))), "C:/");
    CHECK_EQ(U(qt::path(S("/x"))), "/");
    CHECK_EQ(U(qt::path(S("x"))), ".");
    CHECK_EQ(U(qt::path(S("C:x"))), "C:");
    CHECK_EQ(U(qt::path(S("C:/a/b.c"))), "C:/a");
    CHECK_EQ(U(qt::baseName(S("C:/a/x.tar.gz"))), "x");
    CHECK_EQ(U(qt::completeBaseName(S("C:/a/x.tar.gz"))), "x.tar");
    CHECK_EQ(U(qt::suffix(S("C:/a/x.tar.gz"))), "gz");
    CHECK_EQ(U(qt::suffix(S("C:/a/x"))), "<null>");
    CHECK_EQ(U(qt::completeBaseName(S("COG_P1_B1.moproj"))), "COG_P1_B1");
    CHECK_EQ(U(qt::completeBaseName(S("C:a.b"))), "a");
    CHECK_EQ(U(qt::completeBaseName(S("noext"))), "noext");
    CHECK(qt::isRelativePath(S("Operations/Task00001")));
    CHECK(!qt::isRelativePath(S("C:/x")));
    CHECK(!qt::isRelativePath(S("C:x")));
    CHECK(!qt::isRelativePath(S("\\x")));
}

TEST_CASE(absolute_path)
{
    CHECK_EQ(U(qt::absolutePath(S("c:/p/q.db"))), "C:/p");
    CHECK_EQ(U(qt::absolutePath(S("C:\\p\\.\\q.db"))), "C:/p");
    CHECK_EQ(U(qt::absolutePath(S(""))), "");
    CHECK(!qt::absolutePath(S("")).isNull());
    {
        qt::CurrentDirScope cd(S("C:/Windows"));
        REQUIRE(cd.ok());
        CHECK_EQ(U(qt::currentPath()), "C:/Windows");
        CHECK_EQ(U(qt::absolutePath(S("Objects/x.KEY"))), "C:/Windows/Objects");
        CHECK_EQ(U(qt::absolutePath(S("X.DB"))), "C:/Windows");
        // REL of s1-04: the DB is relative, so paths are relative to the current directory.
        CHECK_EQ(U(app::relToDb(S("X.DB"), S("C:/Windows/Operations/Task00004\\Step.KEY"))),
                 "Operations/Task00004/Step.KEY");
    }
}

TEST_CASE(relative_file_path)
{
    CHECK_EQ(U(qt::relativeFilePath(S("C:/PROBLEM/X"), S("C:/PROBLEM/X/Objects\\Object00001.KEY"))),
             "Objects/Object00001.KEY");
    CHECK_EQ(U(qt::relativeFilePath(S("c:/problem/x"), S("C:/PROBLEM/X/Objects/A.KEY"))), "Objects/A.KEY");
    CHECK_EQ(U(qt::relativeFilePath(S("C:/a/c"), S("C:/a/b"))), "../b");
    CHECK_EQ(U(qt::relativeFilePath(S("C:/a"), S("D:/a/b"))), "D:/a/b");
    CHECK_EQ(U(qt::relativeFilePath(S("C:/a"), S("rel/x"))), "rel/x");
    CHECK_EQ(U(qt::relativeFilePath(S("C:/a"), S("C:/a"))), "<null>");   // nothing appended: null
    CHECK(qt::relativeFilePath(S("C:/a"), String()).isNull());
}

TEST_CASE(app_helpers)
{
    CHECK(app::stripDot(S("")).isNull());
    CHECK_EQ(U(app::stripDot(S(".DB"))), "DB");
    CHECK_EQ(U(app::stripDot(S("moproj"))), "moproj");
    CHECK_EQ(U(app::stripDot(S("."))), "");
    CHECK(app::endsWithSuffix(S("C:/P/COG.MOPROJ"), S("moproj")));
    CHECK(app::endsWithSuffix(S("C:/P/COG.db"), S(".DB")));
    CHECK(!app::endsWithSuffix(S("C:/P/COGDB"), S(".DB")));
    CHECK_EQ(U(app::changeSuffix(S("C:\\P\\COG_P1_B1.moproj"), S(".DB"))), "C:/P/COG_P1_B1.DB");
    CHECK_EQ(U(app::changeSuffix(S("C:/P/COG_P1_B1"), S("moproj"))), "C:/P/COG_P1_B1.moproj");
    // 0x1408192e0 applies completeBaseName to the base a second time.
    CHECK_EQ(U(app::changeSuffix(S("C:/P/a.b.moproj"), S(".DB"))), "C:/P/a.DB");
    CHECK_EQ(U(app::joinBaseSuffix(S("C:/P"), S("COG"), S(".MST"))), "C:/P/COG.MST");
    CHECK_EQ(U(app::appendSuffix(S("COG"), S(".MST"))), "COG.MST");
    CHECK_EQ(U(app::appendSuffix(S("COG.mst"), S(".MST"))), "COG.mst");
}

// Every project-relative path in the saved corpus MSTs is regenerated the way the writers make it:
// the stored absolute path "<prj>/<dir>\<file>" (s1-01 4) through REL of a DB in the project folder.
TEST_CASE(corpus_paths)
{
    const char* corpus = std::getenv("M2M_CORPUS");
    if (!corpus || !*corpus || !std::filesystem::is_directory(corpus)) {
        std::printf("corpus not found, skipped\n");
        return;
    }
    int n = 0;
    for (const auto& c : std::filesystem::directory_iterator(corpus)) {
        if (!c.is_directory()) continue;
        for (const auto& f : std::filesystem::directory_iterator(c.path())) {
            if (f.path().extension() != ".MST") continue;
            const String prj = qt::cleanPath(String(c.path().u16string()));
            std::ifstream in(f.path(), std::ios::binary);
            std::string line;
            while (std::getline(in, line)) {
                if (line.find('/') == std::string::npos || line.find_first_of(" \t") != std::string::npos) continue;
                const size_t cut = line.rfind('/');
                const String stored = prj + u'/' + qt::L(line.substr(0, cut).c_str()) + qt::kSep +
                                      qt::L(line.substr(cut + 1).c_str());
                const String rel = app::relToDb(prj + qt::L("/X.DB"), stored);
                CHECK_EQ(U(rel), line);
                ++n;
            }
        }
    }
    std::printf("%d corpus paths regenerated\n", n);
    CHECK(n > 0);
}

int main()
{
    return testkit::run_all();
}
