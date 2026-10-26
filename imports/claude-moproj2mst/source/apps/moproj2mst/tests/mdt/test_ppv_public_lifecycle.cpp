#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include "../support/test_paths.hpp"

#include <windows.h>
#include <bcrypt.h>
#pragma comment(lib, "bcrypt.lib")

#include <testkit/check.hpp>

#include <cstdlib>
#include <cstdint>
#include <cwchar>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

#include "moproj2mst.h"

namespace fs = std::filesystem;

namespace {
struct ManifestEntry {
    bool directory = false;
    std::uintmax_t size = 0;
    std::string sha256;
    bool operator==(const ManifestEntry&) const = default;
};
using Manifest = std::map<std::wstring, ManifestEntry>;

std::string sha256(const unsigned char* bytes, std::size_t size)
{
    if (size > static_cast<std::size_t>(ULONG_MAX))
        throw std::runtime_error("PPV fixture exceeds BCryptHash limit");
    unsigned char digest[32]{};
    if (BCryptHash(BCRYPT_SHA256_ALG_HANDLE, nullptr, 0,
                   const_cast<PUCHAR>(bytes), static_cast<ULONG>(size), digest,
                   static_cast<ULONG>(sizeof(digest))) != 0)
        throw std::runtime_error("PPV fixture SHA-256 failed");
    static constexpr char hex[] = "0123456789abcdef";
    std::string result;
    result.reserve(64);
    for (const unsigned char byte : digest) {
        result.push_back(hex[byte >> 4]);
        result.push_back(hex[byte & 0x0f]);
    }
    return result;
}

std::string sha256File(const fs::path& path)
{
    std::ifstream input(path, std::ios::binary);
    if (!input) throw std::runtime_error("cannot hash PPV fixture: " + path.string());
    const std::vector<unsigned char> bytes((std::istreambuf_iterator<char>(input)),
                                           std::istreambuf_iterator<char>());
    return sha256(bytes.data(), bytes.size());
}

Manifest manifest(const fs::path& root)
{
    Manifest result;
    for (const auto& entry : fs::recursive_directory_iterator(root)) {
        ManifestEntry value;
        value.directory = entry.is_directory();
        if (!value.directory) {
            value.size = entry.file_size();
            value.sha256 = sha256File(entry.path());
        }
        result.emplace(entry.path().lexically_relative(root).generic_wstring(),
                       std::move(value));
    }
    return result;
}

void copyTree(const fs::path& source, const fs::path& destination)
{
    fs::create_directories(destination);
    fs::copy(source, destination,
             fs::copy_options::recursive | fs::copy_options::overwrite_existing);
}

using ConvertW = decltype(&m2m_convert_w);

ConvertW loadPublicConvert()
{
    std::wstring executable(32768, L'\0');
    const DWORD length = GetModuleFileNameW(nullptr, executable.data(),
                                            static_cast<DWORD>(executable.size()));
    if (length == 0 || length >= executable.size())
        throw std::runtime_error("cannot locate public ABI test executable");
    executable.resize(length);
    const fs::path dll = fs::path(executable).parent_path() / "moproj2mst.dll";
    HMODULE module = LoadLibraryW(dll.c_str());
    if (!module) throw std::runtime_error("cannot load public C ABI DLL beside test executable");
    auto convert = reinterpret_cast<ConvertW>(GetProcAddress(module, "m2m_convert_w"));
    if (!convert) {
        FreeLibrary(module);
        throw std::runtime_error("public C ABI DLL lacks m2m_convert_w");
    }
    return convert;
}
} // namespace

TEST_CASE(public_ppv_local_input_is_rewritten_by_one_public_open_save_call)
{
    const fs::path source = m2m_test::configuredPath("M2M_ORACLE_B") /
                            "93_B_MB3ACT_before";
    const fs::path sourceProject = source / "Gear_blank.moproj";
    const fs::path sourceKey = source / "Operations" / "Task00002" / "SimCtrl.KEY";
    REQUIRE(fs::is_regular_file(sourceProject));
    REQUIRE(fs::is_regular_file(sourceKey));
    CHECK_EQ(sha256File(sourceProject),
             std::string("0e3bbd030ae3eda05898a6138a223ff560bd6611631ae276a69ac87ca9694b78"));
    CHECK_EQ(sha256File(sourceKey),
             std::string("7a822135c109f96c7b7cff32752c0d1bfecebe5814f1fef431b137d79716bc9c"));
    const Manifest sourceBefore = manifest(source);

    const fs::path scratch = m2m_test::scratchOutput(
        m2m_test::scratchRoot() / "ppv_public_lifecycle" /
        (std::to_string(GetCurrentProcessId()) + "-" +
         std::to_string(GetTickCount64())));
    const fs::path project = scratch / "project";
    REQUIRE(!fs::exists(scratch));
    copyTree(source, project);
    const Manifest copiedBefore = manifest(project);
    CHECK(sourceBefore == copiedBefore);

    // The retained file has Sub_Project Active="2". Source-qualified
    // parseSubProject treats Active as a zero-based DiagramItem index, so
    // select DiagramItem 1 (Operation_Id=2) in this disposable copy to make
    // Task00002 the Open/Save owner that has a retained SimCtrl.KEY.
    const fs::path inputProject = project / "Gear_blank.moproj";
    {
        std::ifstream input(inputProject, std::ios::binary);
        REQUIRE(input.is_open());
        const std::string xml{std::istreambuf_iterator<char>(input),
                              std::istreambuf_iterator<char>()};
        const std::string from = "<Sub_Project Active=\"2\"";
        const std::string to = "<Sub_Project Active=\"1\"";
        const auto at = xml.find(from);
        REQUIRE(at != std::string::npos);
        REQUIRE(xml.find(from, at + from.size()) == std::string::npos);
        std::string selected = xml;
        selected.replace(at, from.size(), to);
        std::ofstream output(inputProject, std::ios::binary | std::ios::trunc);
        REQUIRE(output.is_open());
        output.write(selected.data(), static_cast<std::streamsize>(selected.size()));
        REQUIRE(output.good());
    }

    // Source-qualified six-field local row. After selecting the second
    // DiagramItem above, the public loader encounters Task00002/SimCtrl.KEY
    // before the PPV at the same operation path.
    const fs::path ppv = project / "Operations" / "Task00002" / "SimCtrl.PPV";
    REQUIRE(!fs::exists(ppv));
    const std::string ppvBytes = "1\r\n0 false 7 8 9.5 10.5\r\n";
    {
        std::ofstream output(ppv, std::ios::binary);
        REQUIRE(output.is_open());
        output.write(ppvBytes.data(), static_cast<std::streamsize>(ppvBytes.size()));
        REQUIRE(output.good());
    }
    CHECK_EQ(sha256File(ppv),
             std::string("c15d8ec5c4e6f46c3e1d2e3ad2e3cc7acd1fd5251a816a9aea7517bc339f0352"));
    const fs::path outputMst = project / "candidate.MST";
    REQUIRE(!fs::exists(outputMst));
    REQUIRE(fs::is_regular_file(inputProject));

    wchar_t message[1024]{};
    const int status = loadPublicConvert()(inputProject.c_str(), outputMst.c_str(),
                                           M2M_FLAG_WRITE_SAVE,
                                           message, std::size(message));
    CHECK_EQ(status, M2M_OK);
    CHECK(fs::is_regular_file(outputMst));
    const std::string expectedPpv = "1\r\n0\tfalse\t7\t8\t9.5\t10.5\t0\r\n";
    REQUIRE(fs::is_regular_file(ppv));
    std::ifstream ppvInput(ppv, std::ios::binary);
    const std::string actualPpv{std::istreambuf_iterator<char>(ppvInput),
                                std::istreambuf_iterator<char>()};
    CHECK_EQ(actualPpv, expectedPpv);
    const Manifest after = manifest(project);
    CHECK(after.contains(L"candidate.MST"));
    CHECK(after.contains(L"Operations/Task00002/SimCtrl.PPV"));
    CHECK(sourceBefore == manifest(source));
}

TEST_CASE(public_ppv_root_only_input_preserves_file_at_local_empty_save_gate)
{
    const fs::path source = m2m_test::configuredPath("M2M_ORACLE_B") /
                            "93_B_MB3ACT_before";
    const fs::path sourceProject = source / "Gear_blank.moproj";
    const fs::path sourceKey = source / "Operations" / "Task00002" / "SimCtrl.KEY";
    REQUIRE(fs::is_regular_file(sourceProject));
    REQUIRE(fs::is_regular_file(sourceKey));
    CHECK_EQ(sha256File(sourceProject),
             std::string("0e3bbd030ae3eda05898a6138a223ff560bd6611631ae276a69ac87ca9694b78"));
    CHECK_EQ(sha256File(sourceKey),
             std::string("7a822135c109f96c7b7cff32752c0d1bfecebe5814f1fef431b137d79716bc9c"));
    const Manifest sourceBefore = manifest(source);

    const fs::path scratch = m2m_test::scratchOutput(
        m2m_test::scratchRoot() / "ppv_public_root_only" /
        (std::to_string(GetCurrentProcessId()) + "-" +
         std::to_string(GetTickCount64())));
    const fs::path project = scratch / "project";
    REQUIRE(!fs::exists(scratch));
    copyTree(source, project);
    CHECK(sourceBefore == manifest(project));

    const fs::path inputProject = project / "Gear_blank.moproj";
    {
        std::ifstream input(inputProject, std::ios::binary);
        REQUIRE(input.is_open());
        const std::string xml{std::istreambuf_iterator<char>(input),
                              std::istreambuf_iterator<char>()};
        // Active is a zero-based DiagramItem index; operation 2 owns the
        // retained Task00002 SimCtrl.KEY and has operation 1 as predecessor.
        const std::string from = "<Sub_Project Active=\"2\"";
        const std::string to = "<Sub_Project Active=\"1\"";
        const auto at = xml.find(from);
        REQUIRE(at != std::string::npos);
        REQUIRE(xml.find(from, at + from.size()) == std::string::npos);
        std::string selected = xml;
        selected.replace(at, from.size(), to);
        std::ofstream output(inputProject, std::ios::binary | std::ios::trunc);
        REQUIRE(output.is_open());
        output.write(selected.data(), static_cast<std::streamsize>(selected.size()));
        REQUIRE(output.good());
    }

    const fs::path ppv = project / "Operations" / "Task00002" / "SimCtrl.PPV";
    const std::string rootOnlyInput = "1\r\n0 true 7 8 9.5 10.5\r\n";
    {
        std::ofstream output(ppv, std::ios::binary);
        REQUIRE(output.is_open());
        output.write(rootOnlyInput.data(),
                     static_cast<std::streamsize>(rootOnlyInput.size()));
        REQUIRE(output.good());
    }
    const fs::path outputMst = project / "candidate.MST";
    REQUIRE(!fs::exists(outputMst));
    wchar_t message[1024]{};
    const int status = loadPublicConvert()(inputProject.c_str(), outputMst.c_str(),
                                           M2M_FLAG_WRITE_SAVE,
                                           message, std::size(message));
    CHECK_EQ(status, M2M_OK);
    CHECK(fs::is_regular_file(outputMst));
    REQUIRE(fs::is_regular_file(ppv));
    std::ifstream ppvInput(ppv, std::ios::binary);
    const std::string afterPpv{std::istreambuf_iterator<char>(ppvInput),
                               std::istreambuf_iterator<char>()};
    // Native 0x140d4afe0 exits on local-list-is-empty before QFile open;
    // Save ignores that false return after writing SimCtrl.KEY.
    CHECK_EQ(afterPpv, rootOnlyInput);
    CHECK(sourceBefore == manifest(source));
}

TEST_CASE(public_ppv_readonly_save_open_failure_keeps_key_save_success)
{
    const fs::path source = m2m_test::configuredPath("M2M_ORACLE_B") /
                            "93_B_MB3ACT_before";
    const fs::path sourceProject = source / "Gear_blank.moproj";
    const fs::path sourceKey = source / "Operations" / "Task00002" / "SimCtrl.KEY";
    REQUIRE(fs::is_regular_file(sourceProject));
    REQUIRE(fs::is_regular_file(sourceKey));
    CHECK_EQ(sha256File(sourceProject),
             std::string("0e3bbd030ae3eda05898a6138a223ff560bd6611631ae276a69ac87ca9694b78"));
    CHECK_EQ(sha256File(sourceKey),
             std::string("7a822135c109f96c7b7cff32752c0d1bfecebe5814f1fef431b137d79716bc9c"));
    const Manifest sourceBefore = manifest(source);

    const fs::path scratch = m2m_test::scratchOutput(
        m2m_test::scratchRoot() / "ppv_public_open_failure" /
        (std::to_string(GetCurrentProcessId()) + "-" +
         std::to_string(GetTickCount64())));
    const fs::path project = scratch / "project";
    REQUIRE(!fs::exists(scratch));
    copyTree(source, project);
    CHECK(sourceBefore == manifest(project));

    const fs::path inputProject = project / "Gear_blank.moproj";
    {
        std::ifstream input(inputProject, std::ios::binary);
        REQUIRE(input.is_open());
        const std::string xml{std::istreambuf_iterator<char>(input),
                              std::istreambuf_iterator<char>()};
        const std::string from = "<Sub_Project Active=\"2\"";
        const std::string to = "<Sub_Project Active=\"1\"";
        const auto at = xml.find(from);
        REQUIRE(at != std::string::npos);
        REQUIRE(xml.find(from, at + from.size()) == std::string::npos);
        std::string selected = xml;
        selected.replace(at, from.size(), to);
        std::ofstream output(inputProject, std::ios::binary | std::ios::trunc);
        REQUIRE(output.is_open());
        output.write(selected.data(), static_cast<std::streamsize>(selected.size()));
        REQUIRE(output.good());
    }

    const fs::path ppv = project / "Operations" / "Task00002" / "SimCtrl.PPV";
    const std::string localInput = "1\r\n0 false 7 8 9.5 10.5\r\n";
    {
        std::ofstream output(ppv, std::ios::binary);
        REQUIRE(output.is_open());
        output.write(localInput.data(), static_cast<std::streamsize>(localInput.size()));
        REQUIRE(output.good());
    }
    const DWORD originalAttributes = GetFileAttributesW(ppv.c_str());
    REQUIRE(originalAttributes != INVALID_FILE_ATTRIBUTES);
    REQUIRE(SetFileAttributesW(ppv.c_str(), originalAttributes | FILE_ATTRIBUTE_READONLY));
    struct RestorePpvAttributes {
        fs::path path;
        DWORD value;
        ~RestorePpvAttributes() { SetFileAttributesW(path.c_str(), value); }
    } restoreAttributes{ppv, originalAttributes};

    const fs::path key = project / "Operations" / "Task00002" / "SimCtrl.KEY";
    HANDLE keyHandle = CreateFileW(key.c_str(), FILE_WRITE_ATTRIBUTES,
                                   FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
                                   nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
    REQUIRE(keyHandle != INVALID_HANDLE_VALUE);
    SYSTEMTIME oldSystemTime{2010, 1, 5, 1, 0, 0, 0, 0};
    FILETIME oldFileTime{};
    REQUIRE(SystemTimeToFileTime(&oldSystemTime, &oldFileTime));
    const BOOL setTime = SetFileTime(keyHandle, nullptr, nullptr, &oldFileTime);
    CloseHandle(keyHandle);
    REQUIRE(setTime);

    const fs::path outputMst = project / "candidate.MST";
    REQUIRE(!fs::exists(outputMst));
    wchar_t message[1024]{};
    const int status = loadPublicConvert()(inputProject.c_str(), outputMst.c_str(),
                                           M2M_FLAG_WRITE_SAVE,
                                           message, std::size(message));
    CHECK_EQ(status, M2M_OK);
    CHECK(fs::is_regular_file(outputMst));

    std::ifstream ppvInput(ppv, std::ios::binary);
    REQUIRE(ppvInput.is_open());
    const std::string afterPpv{std::istreambuf_iterator<char>(ppvInput),
                               std::istreambuf_iterator<char>()};
    CHECK_EQ(afterPpv, localInput);
    const DWORD afterAttributes = GetFileAttributesW(ppv.c_str());
    CHECK(afterAttributes != INVALID_FILE_ATTRIBUTES);
    CHECK((afterAttributes & FILE_ATTRIBUTE_READONLY) != 0);

    FILETIME keyLastWrite{};
    keyHandle = CreateFileW(key.c_str(), FILE_READ_ATTRIBUTES,
                            FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
                            nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
    REQUIRE(keyHandle != INVALID_HANDLE_VALUE);
    const BOOL gotTime = GetFileTime(keyHandle, nullptr, nullptr, &keyLastWrite);
    CloseHandle(keyHandle);
    REQUIRE(gotTime);
    CHECK(CompareFileTime(&keyLastWrite, &oldFileTime) > 0);

    // Restore scratch metadata; the converter must not be able to leave the
    // injected input read-only merely because its Save open failed.
    REQUIRE(SetFileAttributesW(ppv.c_str(), originalAttributes));
    CHECK(sourceBefore == manifest(source));
}

TEST_CASE(public_ppv_save_continues_when_simctrl_key_open_fails)
{
    const fs::path source = m2m_test::configuredPath("M2M_ORACLE_B") /
                            "93_B_MB3ACT_before";
    const fs::path sourceProject = source / "Gear_blank.moproj";
    const fs::path sourceKey = source / "Operations" / "Task00002" / "SimCtrl.KEY";
    REQUIRE(fs::is_regular_file(sourceProject));
    REQUIRE(fs::is_regular_file(sourceKey));
    CHECK_EQ(sha256File(sourceProject),
             std::string("0e3bbd030ae3eda05898a6138a223ff560bd6611631ae276a69ac87ca9694b78"));
    CHECK_EQ(sha256File(sourceKey),
             std::string("7a822135c109f96c7b7cff32752c0d1bfecebe5814f1fef431b137d79716bc9c"));
    const Manifest sourceBefore = manifest(source);

    const fs::path scratch = m2m_test::scratchOutput(
        m2m_test::scratchRoot() / "ppv_key_open_failure" /
        (std::to_string(GetCurrentProcessId()) + "-" +
         std::to_string(GetTickCount64())));
    const fs::path project = scratch / "project";
    REQUIRE(!fs::exists(scratch));
    copyTree(source, project);
    CHECK(sourceBefore == manifest(project));

    const fs::path inputProject = project / "Gear_blank.moproj";
    {
        std::ifstream input(inputProject, std::ios::binary);
        REQUIRE(input.is_open());
        const std::string xml{std::istreambuf_iterator<char>(input),
                              std::istreambuf_iterator<char>()};
        const std::string from = "<Sub_Project Active=\"2\"";
        const std::string to = "<Sub_Project Active=\"1\"";
        const auto at = xml.find(from);
        REQUIRE(at != std::string::npos);
        REQUIRE(xml.find(from, at + from.size()) == std::string::npos);
        std::string selected = xml;
        selected.replace(at, from.size(), to);
        std::ofstream output(inputProject, std::ios::binary | std::ios::trunc);
        REQUIRE(output.is_open());
        output.write(selected.data(), static_cast<std::streamsize>(selected.size()));
        REQUIRE(output.good());
    }

    const fs::path ppv = project / "Operations" / "Task00002" / "SimCtrl.PPV";
    const std::string localInput = "1\r\n0 false 7 8 9.5 10.5\r\n";
    {
        std::ofstream output(ppv, std::ios::binary);
        REQUIRE(output.is_open());
        output.write(localInput.data(), static_cast<std::streamsize>(localInput.size()));
        REQUIRE(output.good());
    }
    const fs::path key = project / "Operations" / "Task00002" / "SimCtrl.KEY";
    const std::string keyBefore = sha256File(key);
    const fs::path outputMst = project / "candidate.MST";
    REQUIRE(!fs::exists(outputMst));

    // A read-capable handle denies new write access to KEY while still
    // permitting the converter's Open phase to read it. PPV remains writable.
    HANDLE keyLock = CreateFileW(key.c_str(), GENERIC_READ | GENERIC_WRITE,
                                 FILE_SHARE_READ, nullptr, OPEN_EXISTING,
                                 FILE_ATTRIBUTE_NORMAL, nullptr);
    REQUIRE(keyLock != INVALID_HANDLE_VALUE);
    wchar_t message[1024]{};
    const int status = loadPublicConvert()(inputProject.c_str(), outputMst.c_str(),
                                           M2M_FLAG_WRITE_SAVE,
                                           message, std::size(message));
    CHECK_EQ(status, M2M_OK);
    CHECK(fs::is_regular_file(outputMst));
    CHECK_EQ(sha256File(key), keyBefore);
    CloseHandle(keyLock);

    const std::string expectedPpv = "1\r\n0\tfalse\t7\t8\t9.5\t10.5\t0\r\n";
    REQUIRE(fs::is_regular_file(ppv));
    std::ifstream ppvInput(ppv, std::ios::binary);
    const std::string afterPpv{std::istreambuf_iterator<char>(ppvInput),
                               std::istreambuf_iterator<char>()};
    CHECK_EQ(afterPpv, expectedPpv);
    CHECK(sourceBefore == manifest(source));
}

TEST_CASE(public_ppv_malformed_second_row_keeps_first_row_through_save)
{
    const fs::path source = m2m_test::configuredPath("M2M_ORACLE_B") /
                            "93_B_MB3ACT_before";
    const fs::path sourceProject = source / "Gear_blank.moproj";
    const fs::path sourceKey = source / "Operations" / "Task00002" / "SimCtrl.KEY";
    REQUIRE(fs::is_regular_file(sourceProject));
    REQUIRE(fs::is_regular_file(sourceKey));
    CHECK_EQ(sha256File(sourceProject),
             std::string("0e3bbd030ae3eda05898a6138a223ff560bd6611631ae276a69ac87ca9694b78"));
    CHECK_EQ(sha256File(sourceKey),
             std::string("7a822135c109f96c7b7cff32752c0d1bfecebe5814f1fef431b137d79716bc9c"));
    const Manifest sourceBefore = manifest(source);

    const fs::path scratch = m2m_test::scratchOutput(
        m2m_test::scratchRoot() / "ppv_public_malformed_row" /
        (std::to_string(GetCurrentProcessId()) + "-" +
         std::to_string(GetTickCount64())));
    const fs::path project = scratch / "project";
    REQUIRE(!fs::exists(scratch));
    copyTree(source, project);
    CHECK(sourceBefore == manifest(project));

    const fs::path inputProject = project / "Gear_blank.moproj";
    {
        std::ifstream input(inputProject, std::ios::binary);
        REQUIRE(input.is_open());
        const std::string xml{std::istreambuf_iterator<char>(input),
                              std::istreambuf_iterator<char>()};
        const std::string from = "<Sub_Project Active=\"2\"";
        const std::string to = "<Sub_Project Active=\"1\"";
        const auto at = xml.find(from);
        REQUIRE(at != std::string::npos);
        REQUIRE(xml.find(from, at + from.size()) == std::string::npos);
        std::string selected = xml;
        selected.replace(at, from.size(), to);
        std::ofstream output(inputProject, std::ios::binary | std::ios::trunc);
        REQUIRE(output.is_open());
        output.write(selected.data(), static_cast<std::streamsize>(selected.size()));
        REQUIRE(output.good());
    }

    const fs::path ppv = project / "Operations" / "Task00002" / "SimCtrl.PPV";
    const std::string malformedInput =
        "2\r\n0 false 7 8 9.5 10.5\r\nshort row\r\n";
    {
        std::ofstream output(ppv, std::ios::binary);
        REQUIRE(output.is_open());
        output.write(malformedInput.data(),
                     static_cast<std::streamsize>(malformedInput.size()));
        REQUIRE(output.good());
    }

    const fs::path outputMst = project / "candidate.MST";
    REQUIRE(!fs::exists(outputMst));
    wchar_t message[1024]{};
    const int status = loadPublicConvert()(inputProject.c_str(), outputMst.c_str(),
                                           M2M_FLAG_WRITE_SAVE,
                                           message, std::size(message));
    CHECK_EQ(status, M2M_OK);
    CHECK(fs::is_regular_file(outputMst));
    REQUIRE(fs::is_regular_file(ppv));
    std::ifstream ppvInput(ppv, std::ios::binary);
    const std::string afterPpv{std::istreambuf_iterator<char>(ppvInput),
                               std::istreambuf_iterator<char>()};
    CHECK_EQ(afterPpv, "1\r\n0\tfalse\t7\t8\t9.5\t10.5\t0\r\n");
    CHECK(sourceBefore == manifest(source));
}

int main() { return testkit::run_all(); }
