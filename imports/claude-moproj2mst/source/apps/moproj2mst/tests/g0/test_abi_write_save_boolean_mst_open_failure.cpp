// G0: exported C ABI full Save when the default MST destination denies write-open.
// ctest-labels: abi lifecycle-save lifecycle-failure
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <stdexcept>
#include <string>

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

class DenyWriteOpen {
public:
    explicit DenyWriteOpen(const fs::path& path)
        : handle_(CreateFileW(path.c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr,
                              OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr))
    {
        if (handle_ == INVALID_HANDLE_VALUE)
            throw std::runtime_error("Cannot acquire read handle for denied-write MST fixture");
    }
    ~DenyWriteOpen() { if (handle_ != INVALID_HANDLE_VALUE) CloseHandle(handle_); }
    DenyWriteOpen(const DenyWriteOpen&) = delete;
    DenyWriteOpen& operator=(const DenyWriteOpen&) = delete;
private:
    HANDLE handle_ = INVALID_HANDLE_VALUE;
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
    if (!input) throw std::runtime_error("Cannot read expected disposable output: " + path.string());
    return {std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>()};
}
} // namespace

TEST_CASE(public_write_save_denied_mst_open_keeps_prior_save_effects)
{
    static unsigned sequence = 0;
    const fs::path problemRoot = fs::path(L"C:\\PROBLEM");
    if (!fs::is_directory(problemRoot))
        throw std::runtime_error("Required short test root C:\\PROBLEM is unavailable");
    const fs::path root = problemRoot /
        ("m2mA143-" + std::to_string(GetCurrentProcessId()) + "-" +
         std::to_string(GetTickCount64()) + "-" + std::to_string(++sequence));
    if (fs::exists(root)) throw std::runtime_error("Refusing to reuse ABI MST-open scratch directory");

    const fs::path fixture = m2m_test::configuredPath("M2M_ORACLE_B") / "133_B_DBSTEP_BOOL3_before";
    for (const auto& entry : fs::recursive_directory_iterator(fixture)) {
        const fs::path generated = root / fs::relative(entry.path(), fixture);
        if (generated.native().size() >= 100)
            throw std::runtime_error("Refusing generated project path at or above 100 characters: " + generated.string());
    }
    for (const auto& generated : {root / "COG_P1_B1.moproj", root / "COG_P1_B1.MST",
                                  root / "Operations" / "Task00002" / "Boolean.BWZ",
                                  root / "Operations" / "Task00002" / "DEF_MACHDIST_CTRL2.INI"}) {
        if (generated.native().size() >= 100)
            throw std::runtime_error("Refusing generated project path at or above 100 characters: " + generated.string());
    }
    fs::create_directories(root);
    fs::copy(fixture, root, fs::copy_options::recursive);

    const fs::path project = root / "COG_P1_B1.moproj";
    const fs::path mst = root / "COG_P1_B1.MST";
    const fs::path task = root / "Operations" / "Task00002";
    const fs::path booleanBwz = task / "Boolean.BWZ";
    const fs::path booleanIni = task / "DEF_MACHDIST_CTRL2.INI";
    if (!fs::is_regular_file(project) || !fs::is_regular_file(mst))
        throw std::runtime_error("133_B_DBSTEP_BOOL3 retained fixture is incomplete");
    if (fs::exists(booleanBwz) || fs::exists(booleanIni))
        throw std::runtime_error("Expected Boolean step-4 outputs are already present in copied fixture");

    const std::string originalProject = readBytes(project);
    const std::string retainedMst = readBytes(mst);
    EnvironmentValue inputMode(L"M2M_MO_INPUT_MODE");
    inputMode.set(L"0");
    CHECK_EQ(m2m_set_codepage(1252), M2M_OK);
    CHECK_EQ(m2m_set_gui_unit(0), M2M_OK);
    CHECK_EQ(m2m_set_deform_dir_w(nullptr), M2M_OK);

    const auto initialDirectory = currentDirectory();
    wchar_t message[2048] = {};
    {
        DenyWriteOpen denyMst(mst); // Read access is allowed; a later WRITE open is denied by share mode.
        CHECK_EQ(m2m_convert_w(project.c_str(), nullptr, M2M_FLAG_WRITE_SAVE,
                               message, std::size(message)), M2M_E_WRITE);
        CHECK(std::wstring(message).find(L"cannot open MST ") == 0);
        CHECK(readBytes(mst) == retainedMst);
        CHECK(readBytes(booleanBwz).find("WZD_BOOLEAN_CUTTER_OBJECT_NO") != std::string::npos);
        CHECK(readBytes(booleanIni).find("WZD_MACHDIST_OPERATION_BOOLEAN") != std::string::npos);
        CHECK(readBytes(project) != originalProject);
        CHECK(currentDirectory() == initialDirectory);
    }

    // Releasing the sharing-denial handle is sufficient cleanup. The copied project and
    // all generated effects stay in CTest's disposable scratch for diagnosis.
}

int main()
{
    return testkit::run_all();
}
