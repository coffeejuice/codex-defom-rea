// G0: exported C ABI full Save success followed by a separate failure in one process.
// ctest-labels: abi lifecycle-save
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <map>
#include <regex>
#include <stdexcept>
#include <string>
#include <vector>

#include <testkit/check.hpp>

#include "moproj2mst.h"
#include "../support/test_paths.hpp"

namespace {
namespace fs = std::filesystem;

class EnvironmentValue {
public:
    explicit EnvironmentValue(const wchar_t* name) : name_(name)
    {
        SetLastError(ERROR_SUCCESS);
        const DWORD needed = GetEnvironmentVariableW(name_, nullptr, 0);
        if (needed != 0) {
            prior_.resize(needed);
            const DWORD used = GetEnvironmentVariableW(name_, prior_.data(), needed);
            if (used == 0 || used >= needed) throw std::runtime_error("Cannot snapshot environment variable");
            prior_.resize(used);
            existed_ = true;
        } else if (GetLastError() != ERROR_ENVVAR_NOT_FOUND && GetLastError() != ERROR_SUCCESS) {
            throw std::runtime_error("Cannot inspect environment variable");
        }
    }
    ~EnvironmentValue() { SetEnvironmentVariableW(name_, existed_ ? prior_.c_str() : nullptr); }
    EnvironmentValue(const EnvironmentValue&) = delete;
    EnvironmentValue& operator=(const EnvironmentValue&) = delete;
    void set(const wchar_t* value)
    {
        if (!SetEnvironmentVariableW(name_, value)) throw std::runtime_error("Cannot set environment variable");
    }
private:
    const wchar_t* name_;
    std::wstring prior_;
    bool existed_ = false;
};

std::wstring currentDirectory()
{
    const DWORD length = GetCurrentDirectoryW(0, nullptr);
    if (!length) throw std::runtime_error("Cannot read current directory");
    std::wstring value(length, L'\0');
    const DWORD used = GetCurrentDirectoryW(length, value.data());
    if (!used || used >= length) throw std::runtime_error("Cannot read current directory");
    value.resize(used);
    return value;
}

std::string readBytes(const fs::path& path)
{
    std::ifstream input(path, std::ios::binary);
    if (!input) throw std::runtime_error("Cannot read comparison file: " + path.string());
    return {std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>()};
}

std::string lower(std::string value)
{
    for (char& c : value)
        if (c >= 'A' && c <= 'Z') c = static_cast<char>(c + ('a' - 'A'));
    return value;
}

using Files = std::map<std::string, fs::path>;

Files comparableFiles(const fs::path& root)
{
    Files result;
    for (const auto& entry : fs::recursive_directory_iterator(root)) {
        if (!entry.is_regular_file()) continue;
        const auto path = entry.path();
        // Match tests/s/test_save_folder.cpp::files: these are oracle-runner metadata,
        // not outputs of native File > Save.
        if (path.extension() == ".png" || path.extension() == ".oracleA" ||
            path.filename() == "view_state.json") continue;
        result.emplace(lower(fs::relative(path, root).generic_string()), path);
    }
    return result;
}

void replaceWholeFolder(std::string& bytes, const std::string& from, const std::string& to)
{
    if (from.empty()) return;
    for (std::size_t at = bytes.find(from); at != std::string::npos;) {
        const std::size_t end = at + from.size();
        if (end < bytes.size() && bytes[end] != '/' && bytes[end] != '\\') {
            at = bytes.find(from, end);
            continue;
        }
        bytes.replace(at, from.size(), to);
        at = bytes.find(from, at + to.size());
    }
}

std::string relocated(std::string bytes, const fs::path& expectedRoot, const fs::path& actualRoot)
{
    auto from = fs::absolute(expectedRoot), to = fs::absolute(actualRoot);
    replaceWholeFolder(bytes, from.generic_string(), to.generic_string());
    replaceWholeFolder(bytes, from.make_preferred().string(), to.make_preferred().string());
    return bytes;
}

std::string withoutUninitializedExpanded(const std::string& bytes)
{
    static const std::regex expanded(
        R"(Expanded="[a-z]*"( Operation_Id="[^"]*">\s*<ClassName>SfOpr(Cycle|Group)<))");
    return std::regex_replace(bytes, expanded, "$1");
}

bool bannerOnly(const std::string& actual, const std::string& expected)
{
    auto lines = [](const std::string& bytes) {
        std::vector<std::string> result;
        std::size_t begin = 0;
        for (;;) {
            const auto end = bytes.find('\n', begin);
            result.push_back(bytes.substr(begin, end == std::string::npos ? std::string::npos : end - begin));
            if (end == std::string::npos) return result;
            begin = end + 1;
        }
    };
    const auto a = lines(actual), e = lines(expected);
    if (a.size() != e.size()) return false;
    bool any = false;
    for (std::size_t i = 0; i < a.size(); ++i) {
        if (a[i] == e[i]) continue;
        if (a[i].find("DEFORM-") == std::string::npos || e[i].find("DEFORM-") == std::string::npos)
            return false;
        any = true;
    }
    return any;
}

struct Comparison {
    std::vector<std::string> rawDifferent;
    std::vector<std::string> different;
    std::vector<std::string> missing;
    std::vector<std::string> extra;
};

Comparison compareSaveFolder(const fs::path& expectedRoot, const fs::path& actualRoot)
{
    const auto expected = comparableFiles(expectedRoot), actual = comparableFiles(actualRoot);
    Comparison result;
    for (const auto& [relative, expectedPath] : expected) {
        const auto found = actual.find(relative);
        if (found == actual.end()) {
            result.missing.push_back(relative);
            continue;
        }
        const std::string expectedBytes = readBytes(expectedPath);
        const std::string actualBytes = readBytes(found->second);
        if (actualBytes == expectedBytes) continue;
        result.rawDifferent.push_back(relative);
        const auto relocatedExpected = relocated(expectedBytes, expectedRoot, actualRoot);
        if (actualBytes == relocatedExpected || bannerOnly(actualBytes, relocatedExpected)) continue;
        if (fs::path(relative).extension() == ".moproj" &&
            withoutUninitializedExpanded(actualBytes) == withoutUninitializedExpanded(relocatedExpected))
            continue;
        result.different.push_back(relative);
    }
    for (const auto& [relative, path] : actual) {
        (void)path;
        if (!expected.count(relative)) result.extra.push_back(relative);
    }
    return result;
}

std::map<std::string, std::string> saveSnapshot(const fs::path& root)
{
    std::map<std::string, std::string> result;
    for (const auto& [relative, path] : comparableFiles(root)) result.emplace(relative, readBytes(path));
    return result;
}

void printPaths(const char* category, const std::vector<std::string>& paths)
{
    for (const auto& path : paths) std::fprintf(stderr, "%s %s\n", category, path.c_str());
}
}  // namespace

TEST_CASE(public_write_save_success_then_failure_isolated)
{
    static unsigned sequence = 0;
    const fs::path root = fs::path(L"C:\\PROBLEM") /
        ("m2mA142-" + std::to_string(GetCurrentProcessId()) + "-" +
         std::to_string(GetTickCount64()) + "-" + std::to_string(++sequence));
    if (fs::exists(root)) throw std::runtime_error("Refusing to reuse ABI sequence scratch directory");

    const fs::path oracleRoot = m2m_test::configuredPath("M2M_ORACLE_B");
    const fs::path inputRoot = oracleRoot / "RS_08_HEAT1_before";
    const fs::path expectedRoot = oracleRoot / "RS_08_HEAT1";
    const fs::path firstRoot = root / "first";
    const fs::path project = firstRoot / "HEAT1.moproj";
    const fs::path output = firstRoot / "HEAT1.MST";
    if (project.native().size() >= 100 ||
        (root / "second" / "LOC_DEN_CTRL.DAT").native().size() >= 100)
        throw std::runtime_error("DEFORM disposable paths must be shorter than 100 characters");
    fs::create_directories(firstRoot);
    fs::copy(inputRoot, firstRoot, fs::copy_options::recursive);
    if (!fs::is_regular_file(project) || !fs::is_regular_file(expectedRoot / "HEAT1.MST"))
        throw std::runtime_error("RS_08_HEAT1 retained fixture is incomplete");

    EnvironmentValue inputMode(L"M2M_MO_INPUT_MODE");
    inputMode.set(L"0");
    CHECK_EQ(m2m_set_codepage(1252), M2M_OK);
    CHECK_EQ(m2m_set_gui_unit(0), M2M_OK);
    const auto initialDirectory = currentDirectory();
    wchar_t successMessage[2048] = {};
    CHECK_EQ(m2m_convert_w(project.c_str(), nullptr, M2M_FLAG_WRITE_SAVE,
                           successMessage, std::size(successMessage)), M2M_OK);
    CHECK(currentDirectory() == initialDirectory);
    CHECK(fs::is_regular_file(output));

    const auto firstSnapshot = saveSnapshot(firstRoot);
    const auto comparison = compareSaveFolder(expectedRoot, firstRoot);
    if (!comparison.rawDifferent.empty()) {
        std::fprintf(stderr, "Raw Save-folder byte differences before comparator adjustments:\n");
        printPaths("raw", comparison.rawDifferent);
    }
    printPaths("different", comparison.different);
    printPaths("missing", comparison.missing);
    printPaths("extra", comparison.extra);
    CHECK(comparison.different.empty());
    CHECK(comparison.missing.empty());
    CHECK(comparison.extra.empty());

    // Repeat the exact ABI parse-failure fixture contract already used by test_abi.cpp,
    // but only after a completed successful Save in this process.
    const fs::path secondRoot = root / "second";
    fs::create_directories(secondRoot);
    const fs::path missingProject = secondRoot / "B.moproj";
    const fs::path secondOutput = secondRoot / "B.MST";
    const fs::path remesh = secondRoot / "LOC_DEN_CTRL.DAT";
    const std::string remeshBytes = "FILTER_DISTANCE 19\nDONE\n";
    {
        std::ofstream stream(remesh, std::ios::binary | std::ios::trunc);
        if (!stream || !(stream << remeshBytes)) throw std::runtime_error("Cannot create second-call remesh input");
    }
    CHECK(!fs::exists(missingProject));
    const auto secondCwd = currentDirectory();
    wchar_t failureMessage[512] = {};
    CHECK_EQ(m2m_convert_w(missingProject.c_str(), secondOutput.c_str(), M2M_FLAG_WRITE_SAVE,
                           failureMessage, std::size(failureMessage)), M2M_E_PARSE);
    CHECK(std::wstring(failureMessage).find(L"reading project") != std::wstring::npos);
    CHECK(currentDirectory() == secondCwd);
    CHECK(!fs::exists(secondOutput));
    CHECK(readBytes(remesh) == remeshBytes);
    CHECK(saveSnapshot(firstRoot) == firstSnapshot);
}

int main()
{
    return testkit::run_all();
}
