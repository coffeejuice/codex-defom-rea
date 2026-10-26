// G0: the C ABI of moproj2mst.dll (argument checks, message buffers, stub-phase result).
// ctest-labels: abi
#include <windows.h>

#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <string>
#include <vector>

#include <testkit/check.hpp>

#include "moproj2mst.h"
#include "../support/test_paths.hpp"

static std::wstring corpusFile(const wchar_t* rel)
{
    const wchar_t* c = _wgetenv(L"M2M_CORPUS");
    return std::wstring(c ? c : L"") + L"/" + rel;
}

TEST_CASE(version)
{
    CHECK_EQ(m2m_api_version(), M2M_API_VERSION);
}

TEST_CASE(null_and_empty_paths)
{
    wchar_t msg[64];
    CHECK_EQ(m2m_convert_w(nullptr, nullptr, 0, msg, 64), M2M_E_ARG);
    CHECK(msg[0] != 0);
    CHECK_EQ(m2m_convert_w(L"", nullptr, 0, nullptr, 0), M2M_E_ARG);
    char m8[64];
    CHECK_EQ(m2m_convert_utf8(nullptr, nullptr, 0, m8, sizeof m8), M2M_E_ARG);
    CHECK_EQ(m2m_convert_utf8("\xff\xfe", nullptr, 0, m8, sizeof m8), M2M_E_ARG);   // not UTF-8
}

TEST_CASE(missing_file_is_open_error)
{
    wchar_t msg[256];
    CHECK_EQ(m2m_convert_w(L"C:/no/such/dir/x.moproj", nullptr, 0, msg, 256), M2M_E_OPEN);
    CHECK(std::wcsstr(msg, L"x.moproj") != nullptr);
}

TEST_CASE(unknown_flag_is_arg_error)
{
    std::wstring f = corpusFile(L"08_HEAT1/HEAT1.moproj");
    CHECK_EQ(m2m_convert_w(f.c_str(), L"C:/nonexistent_dir/x.MST", 0x100u, nullptr, 0), M2M_E_ARG);
}

TEST_CASE(message_truncation_keeps_nul)
{
    wchar_t msg[4] = {L'x', L'x', L'x', L'x'};
    CHECK_EQ(m2m_convert_w(L"C:/no/such/dir/x.moproj", nullptr, 0, msg, 4), M2M_E_OPEN);
    CHECK(msg[3] == L'\0');
    char m8[5];
    std::memset(m8, 'x', sizeof m8);
    CHECK_EQ(m2m_convert_utf8("C:/no/such/dir/x.moproj", nullptr, 0, m8, sizeof m8), M2M_E_OPEN);
    CHECK_EQ(m8[4], '\0');
}

TEST_CASE(unicode_diagnostic_truncation_preserves_scalar_boundary)
{
    // Build the supplementary character as UTF-16 code units so the test source
    // does not depend on the compiler's source-file encoding or active locale.
    std::wstring filename = L"missing-";
    filename.push_back(static_cast<wchar_t>(0xd83d));
    filename.push_back(static_cast<wchar_t>(0xde00));
    filename += L".moproj";
    const std::wstring missing = (m2m_test::scratchRoot() / filename).wstring();
    REQUIRE(GetFileAttributesW(missing.c_str()) == INVALID_FILE_ATTRIBUTES);

    const int pathBytes = WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS,
        missing.c_str(), -1, nullptr, 0, nullptr, nullptr);
    REQUIRE(pathBytes > 1);
    std::string missing8(static_cast<std::size_t>(pathBytes), '\0');
    CHECK(WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, missing.c_str(), -1,
        missing8.data(), pathBytes, nullptr, nullptr) == pathBytes);
    missing8.resize(static_cast<std::size_t>(pathBytes - 1)); // exclude the path NUL

    wchar_t fullW[2048] = {};
    char full8[4096] = {};
    CHECK_EQ(m2m_convert_w(missing.c_str(), nullptr, 0, fullW, _countof(fullW)), M2M_E_OPEN);
    CHECK_EQ(m2m_convert_utf8(missing8.c_str(), nullptr, 0, full8, sizeof full8), M2M_E_OPEN);
    CHECK(std::wcsstr(fullW, filename.c_str()) != nullptr);
    CHECK(std::strstr(full8, "missing-") != nullptr);

    const std::wstring wideFull(fullW);
    const std::string utf8Full(full8);
    const std::size_t wideScalar = wideFull.find(std::wstring({
        static_cast<wchar_t>(0xd83d), static_cast<wchar_t>(0xde00)}));
    const std::size_t utf8Scalar = utf8Full.find("\xf0\x9f\x98\x80");
    REQUIRE(wideScalar != std::wstring::npos);
    REQUIRE(utf8Scalar != std::string::npos);

    // Capacity includes the terminating NUL. These capacities would otherwise
    // leave only the high surrogate or the UTF-8 leading byte in the buffer.
    std::vector<wchar_t> shortW(wideScalar + 2, L'x');
    std::vector<char> short8(utf8Scalar + 2, 'x');
    CHECK_EQ(m2m_convert_w(missing.c_str(), nullptr, 0, shortW.data(), shortW.size()), M2M_E_OPEN);
    CHECK_EQ(m2m_convert_utf8(missing8.c_str(), nullptr, 0, short8.data(), short8.size()), M2M_E_OPEN);
    CHECK(shortW[wideScalar] == L'\0');
    CHECK(short8[utf8Scalar] == '\0');
    CHECK(shortW[wideScalar - 1] != static_cast<wchar_t>(0xd83d));
    CHECK(static_cast<unsigned char>(short8[utf8Scalar - 1]) < 0x80 ||
          static_cast<unsigned char>(short8[utf8Scalar - 1]) >= 0xc0);
    CHECK(std::wstring(shortW.data()) == wideFull.substr(0, wideScalar));
    CHECK(std::string(short8.data()) == utf8Full.substr(0, utf8Scalar));

    const int decodedChars = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS,
        full8, -1, nullptr, 0);
    REQUIRE(decodedChars > 0);
    std::wstring decodedFull(static_cast<std::size_t>(decodedChars), L'\0');
    CHECK(MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, full8, -1,
        decodedFull.data(), decodedChars) == decodedChars);
    decodedFull.resize(static_cast<std::size_t>(decodedChars - 1));
    CHECK(decodedFull == wideFull);

    const int decodedShortChars = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS,
        short8.data(), -1, nullptr, 0);
    REQUIRE(decodedShortChars > 0);
    std::wstring decodedShort(static_cast<std::size_t>(decodedShortChars), L'\0');
    CHECK(MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, short8.data(), -1,
        decodedShort.data(), decodedShortChars) == decodedShortChars);
    decodedShort.resize(static_cast<std::size_t>(decodedShortChars - 1));
    CHECK(decodedShort == std::wstring(shortW.data()));
}

TEST_CASE(gui_unit)
{
    CHECK_EQ(m2m_set_gui_unit(1), M2M_OK);
    CHECK_EQ(m2m_set_gui_unit(3), M2M_E_ARG);
    CHECK_EQ(m2m_set_gui_unit(0), M2M_OK);
}

TEST_CASE(codepage)
{
    CHECK_EQ(m2m_set_codepage(1252), M2M_OK);
    CHECK_EQ(m2m_set_codepage(932), M2M_OK);
    CHECK_EQ(m2m_set_codepage(4294967u), M2M_E_ARG);
    CHECK_EQ(m2m_set_codepage(1252), M2M_OK);
}

// A real corpus project converts to M2M_OK only once the packets merge; until then the seam stubs
// make it M2M_E_UNSUPPORTED. Either way nothing is written next to the project.
TEST_CASE(corpus_project_result)
{
    std::wstring f = corpusFile(L"08_HEAT1/HEAT1.moproj");
    if (GetFileAttributesW(f.c_str()) == INVALID_FILE_ATTRIBUTES) {
        std::fprintf(stderr, "corpus not found, skipped\n");
        return;
    }
    wchar_t tmp[MAX_PATH], out[MAX_PATH];
    GetTempPathW(MAX_PATH, tmp);
    GetTempFileNameW(tmp, L"m2m", 0, out);
    DeleteFileW(out);
    wchar_t msg[512];
    int rc = m2m_convert_w(f.c_str(), out, 0, msg, 512);
    CHECK(rc == M2M_OK || rc == M2M_E_UNSUPPORTED);
    if (rc == M2M_E_UNSUPPORTED) {
        CHECK(std::wcsstr(msg, L"not ported yet") != nullptr);
        CHECK(GetFileAttributesW(out) == INVALID_FILE_ATTRIBUTES);
    }
    DeleteFileW(out);
}

TEST_CASE(sequential_public_calls_keep_single_project_failure_state_isolated)
{
    namespace fs = std::filesystem;
    static unsigned sequence = 0;
    const fs::path root = m2m_test::scratchOutput(m2m_test::scratchRoot() /
        ("abi-sequential-" + std::to_string(GetCurrentProcessId()) + "-" +
         std::to_string(++sequence)));
    const fs::path projectA = root / "A" / "A.moproj";
    const fs::path projectB = root / "B" / "B.moproj";
    const fs::path outputA = root / "A" / "A.MST";
    const fs::path outputB = root / "B" / "B.MST";
    const fs::path remeshA = root / "A" / "LOC_DEN_CTRL.DAT";
    const fs::path remeshB = root / "B" / "LOC_DEN_CTRL.DAT";
    fs::create_directories(projectA.parent_path());
    fs::create_directories(projectB.parent_path());

    // Both disposable projects deliberately omit the .moproj. Existing ABI failure
    // evidence establishes that WRITE_SAVE continues through Open's remesh input,
    // then reports M2M_E_PARSE at the later openedOk Save decision without writing MST.
    const std::string bytesA = "FILTER_DISTANCE 7\nDONE\n";
    const std::string bytesB = "FILTER_DISTANCE 19\nDONE\n";
    std::ofstream(remeshA, std::ios::binary) << bytesA;
    std::ofstream(remeshB, std::ios::binary) << bytesB;
    auto read = [](const fs::path& path) {
        std::ifstream stream(path, std::ios::binary);
        return std::string(std::istreambuf_iterator<char>(stream),
                           std::istreambuf_iterator<char>());
    };

    wchar_t messageA[256] = {};
    wchar_t messageB[256] = {};
    CHECK_EQ(m2m_convert_w(projectA.c_str(), outputA.c_str(), M2M_FLAG_WRITE_SAVE,
                           messageA, sizeof messageA / sizeof messageA[0]), M2M_E_PARSE);
    CHECK(std::wstring(messageA).find(L"reading project") != std::wstring::npos);
    CHECK(!fs::exists(outputA));
    CHECK(read(remeshA) == bytesA); // failed Save does not apply remesh Save effects

    CHECK_EQ(m2m_convert_w(projectB.c_str(), outputB.c_str(), M2M_FLAG_WRITE_SAVE,
                           messageB, sizeof messageB / sizeof messageB[0]), M2M_E_PARSE);
    CHECK(std::wstring(messageB).find(L"reading project") != std::wstring::npos);
    CHECK(!fs::exists(outputB));
    CHECK(read(remeshB) == bytesB);

    // This is an adapter-level repeated failure-path check. It does not assert that
    // native process-global state resets between projects; each invocation is scoped
    // as a separate single-project conversion, and successful full Save is tested elsewhere.
}

int main()
{
    return testkit::run_all();
}
