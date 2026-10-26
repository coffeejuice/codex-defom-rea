// ctest-labels: abi lifecycle-failure
#include <windows.h>
#include <testkit/check.hpp>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <string>
#include "moproj2mst.h"
#include "../support/test_paths.hpp"

namespace {
std::wstring currentDirectory()
{
    const DWORD length = GetCurrentDirectoryW(0, nullptr);
    std::wstring result(length, L'\0');
    const DWORD used = GetCurrentDirectoryW(length, result.data());
    result.resize(used);
    return result;
}
struct Scratch {
    std::filesystem::path directory, project, output, remesh;
    Scratch() {
        static unsigned sequence = 0;
        directory = m2m_test::scratchOutput(m2m_test::scratchRoot() /
            ("of-" + std::to_string(GetCurrentProcessId()) + "-" + std::to_string(++sequence)));
        std::filesystem::create_directories(directory);
        project = directory / "project.moproj";
        output = directory / "out.MST";
        remesh = directory / "LOC_DEN_CTRL.DAT";
    }
    void writeRemesh(const std::string& bytes) {
        std::ofstream(remesh, std::ios::binary | std::ios::trunc) << bytes;
    }
    std::string remeshBytes() const {
        std::ifstream f(remesh, std::ios::binary);
        return {std::istreambuf_iterator<char>(f), std::istreambuf_iterator<char>()};
    }
    int run(unsigned flags, wchar_t* message = nullptr, unsigned size = 0) const {
        return m2m_convert_w(project.c_str(), output.c_str(), flags, message, size);
    }
};
struct LockedFile {
    HANDLE value = INVALID_HANDLE_VALUE;
    explicit LockedFile(const std::filesystem::path& path) {
        value = CreateFileW(path.c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING,
                            FILE_ATTRIBUTE_NORMAL, nullptr);
    }
    ~LockedFile() { if (value != INVALID_HANDLE_VALUE) CloseHandle(value); }
};
}

TEST_CASE(abi_full_open_missing_project_reaches_later_save_rejection)
{
    Scratch disk;
    const std::string content = "FILTER_DISTANCE 7\nDONE\n";
    disk.writeRemesh(content);
    const auto initialDirectory = currentDirectory();
    CHECK_EQ(disk.run(0), M2M_E_OPEN); // compatibility for legacy profile
    CHECK_EQ(disk.run(M2M_FLAG_WRITE_SAVE), M2M_E_PARSE); // failed openedOk at Save
    CHECK(!std::filesystem::exists(disk.project));
    CHECK(!std::filesystem::exists(disk.output));
    CHECK(disk.remeshBytes() == content); // failed Save never reaches remesh Save
    CHECK(currentDirectory() == initialDirectory); // adapter restoration, not original Close
}

TEST_CASE(abi_full_open_missing_project_does_not_mask_later_remesh_reader_failure)
{
    Scratch disk;
    const std::string content = "INTERNAL_ELEMENT_SIZE_CONTROL_METHOD 4\n\x80\n";
    disk.writeRemesh(content);
    wchar_t message[512] = {};
    CHECK_EQ(disk.run(0), M2M_E_OPEN);
    CHECK_EQ(disk.run(M2M_FLAG_WRITE_SAVE, message, 512), M2M_E_UNSUPPORTED);
    CHECK(std::wstring(message).find(L"Open remesh") != std::wstring::npos);
    CHECK(!std::filesystem::exists(disk.output));
    CHECK(disk.remeshBytes() == content);
}

TEST_CASE(abi_full_open_sharing_denied_project_uses_actual_reader_result)
{
    Scratch disk;
    std::ofstream(disk.project, std::ios::binary) << "<Project><Unit Name=\"SI\"/></Project>";
    disk.writeRemesh("FILTER_DISTANCE 11\nDONE\n");
    const auto initialDirectory = currentDirectory();
    {
        LockedFile lock(disk.project);
        REQUIRE(lock.value != INVALID_HANDLE_VALUE);
        CHECK_EQ(disk.run(0), M2M_E_OPEN);
        CHECK_EQ(disk.run(M2M_FLAG_WRITE_SAVE), M2M_E_PARSE);
    }
    CHECK(!std::filesystem::exists(disk.output));
    CHECK(currentDirectory() == initialDirectory);
}

TEST_CASE(abi_full_open_rejected_suffix_skips_remesh_tail_for_utf8_entry)
{
    Scratch disk;
    disk.project = disk.directory / "missing.txt";
    disk.writeRemesh("INTERNAL_ELEMENT_SIZE_CONTROL_METHOD 4\n\x80\n");
    const auto input8 = disk.project.u8string(), output8 = disk.output.u8string();
    const std::string input(input8.begin(), input8.end()), output(output8.begin(), output8.end());
    char message[256] = {};
    CHECK_EQ(m2m_convert_utf8(input.c_str(), output.c_str(), M2M_FLAG_WRITE_SAVE,
                            message, sizeof message), M2M_E_PARSE);
    CHECK(std::string(message).find("Open remesh") == std::string::npos);
    CHECK(!std::filesystem::exists(disk.output));
}

int main() { return testkit::run_all(); }
