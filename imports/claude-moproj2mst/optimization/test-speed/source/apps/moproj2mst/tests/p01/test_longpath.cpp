#include "../support/test_paths.hpp"
// G1 (P01): paths longer than MAX_PATH. The corpus case 13_XY is copied under a directory whose
// path is over 260 characters. The conversion runs Save step 2, which loads the active operation's
// KEY files through the engine's reader (_fsopen, as in the GUI), so there it refuses with
// M2M_E_UNSUPPORTED naming the limit. A short project still writes an explicit long output path,
// with the bytes of the saved MST.
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <string>

#include <testkit/check.hpp>

#include "m2m/convert.hpp"
#include "m2m/longpath.hpp"

namespace fs = std::filesystem;

namespace {

std::u16string u16(const std::wstring& w) { return std::u16string(w.begin(), w.end()); }

std::string readAll(const std::wstring& path)
{
    std::ifstream f(fs::path(m2m::win32Path(path)), std::ios::binary);
    return std::string(std::istreambuf_iterator<char>(f), std::istreambuf_iterator<char>());
}

// A fresh directory under %TEMP% whose full path is longer than `minLen` characters.
std::wstring longDir(size_t minLen)
{
    wchar_t tmp[MAX_PATH + 1];
    GetTempPathW(MAX_PATH + 1, tmp);
    std::wstring d = std::wstring(tmp) + L"m2m_longpath_" + std::to_wstring(GetCurrentProcessId());
    int i = 0;
    while (d.size() <= minLen) d += L"\\segment_" + std::to_wstring(i++) + L"_abcdefghijklmnopqrstuvwxyz0123456789";
    return d;
}

// Removes this process's %TEMP%\m2m_longpath_<pid> tree. The root itself is short, so
// win32Path leaves it unprefixed and remove_all would then fail on the long children (the
// folders were never removed): the \\?\ prefix is added here explicitly.
void removeRoot()
{
    wchar_t tmp[MAX_PATH + 1];
    GetTempPathW(MAX_PATH + 1, tmp);
    std::wstring root = std::wstring(tmp) + L"m2m_longpath_" + std::to_wstring(GetCurrentProcessId());
    if (root.rfind(L"\\\\?\\", 0) != 0) root = L"\\\\?\\" + root;
    std::error_code ec;
    fs::remove_all(fs::path(root), ec);
}

}  // namespace

TEST_CASE(win32_path_forms)
{
    CHECK(m2m::win32Path(std::wstring(L"C:\\short\\a.txt")) == L"C:\\short\\a.txt");
    CHECK(m2m::win32Path(std::wstring(L"\\\\?\\C:\\x")) == L"\\\\?\\C:\\x");
    const std::wstring tail(300, L'a');
    CHECK(m2m::win32Path(L"C:/dir/" + tail) == L"\\\\?\\C:\\dir\\" + tail);
    CHECK(m2m::win32Path(L"C:\\dir\\.\\sub\\..\\" + tail) == L"\\\\?\\C:\\dir\\" + tail);
    CHECK(m2m::win32Path(L"\\\\server\\share\\" + tail) == L"\\\\?\\UNC\\server\\share\\" + tail);
}

TEST_CASE(convert_under_long_path)
{
    const char* c = std::getenv("M2M_CORPUS");
    REQUIRE(c != nullptr);
    const fs::path src = fs::path(c) / "13_XY";
    REQUIRE(fs::exists(src / "XY.moproj"));
    const std::wstring dir = longDir(300);
    const fs::path osDir(m2m::win32Path(dir));
    std::error_code ec;
    // a folder left by an earlier process with the same id (Windows reuses ids) makes the copy
    // below fail on its existing files: start from an empty one
    removeRoot();
    fs::create_directories(osDir, ec);
    REQUIRE(!ec);
    fs::copy(src, osDir, fs::copy_options::recursive, ec);
    REQUIRE(!ec);
    fs::remove(osDir / "XY.MST", ec);
    const std::string expected = readAll(src.wstring() + L"\\XY.MST");
    REQUIRE(!expected.empty());

    const std::wstring moproj = dir + L"\\XY.moproj";
    CHECK(moproj.size() > MAX_PATH);
    // next to the project: the engine's KEY reader cannot open the operation's KEY files there
    m2m::Result r = m2m::convert(u16(moproj), u"", 0, 0);
    CHECK_EQ(r.code, M2M_E_UNSUPPORTED);
    CHECK(r.message.find("MAX_PATH (260)") != std::string::npos);
    CHECK(r.message.find("KEY reader (1401b5f00") != std::string::npos);
    CHECK(!fs::exists(osDir / "XY.MST"));
    // Full Save additionally reaches original process-global remesh loading
    // during Open. Its bounded native directory profile refuses earlier.
    r = m2m::convert(u16(moproj), u"", M2M_FLAG_WRITE_SAVE, 0);
    CHECK_EQ(r.code, M2M_E_UNSUPPORTED);
    CHECK(r.message.find("remesh") != std::string::npos);
    CHECK(!fs::exists(osDir / "XY.MST"));
    // a short project, an explicit long output path with '/' separators
    std::wstring out = dir + L"\\out\\XY_copy.MST";
    fs::create_directories(fs::path(m2m::win32Path(dir + L"\\out")), ec);
    for (auto& ch : out)
        if (ch == L'\\') ch = L'/';
    const fs::path shortCopy = m2m_test::scratchRoot() / ("m2m_longpath_src_" + std::to_string(GetCurrentProcessId()));
    fs::remove_all(shortCopy, ec);
    fs::copy(src, shortCopy, fs::copy_options::recursive, ec);
    REQUIRE(!ec);
    r = m2m::convert((shortCopy / "XY.moproj").u16string(), u16(out), 0, 0);
    CHECK_EQ(r.code, M2M_OK);
    if (r.code != M2M_OK) std::fprintf(stderr, "%s\n", r.message.c_str());
    CHECK(readAll(out) == expected);
    fs::remove_all(shortCopy, ec);
    // the temporary files are gone
    int tmpFiles = 0;
    for (const auto& e : fs::recursive_directory_iterator(osDir))
        if (e.path().extension() == L".tmp") ++tmpFiles;
    CHECK_EQ(tmpFiles, 0);
    removeRoot();
}

int main() { return testkit::run_all(); }
