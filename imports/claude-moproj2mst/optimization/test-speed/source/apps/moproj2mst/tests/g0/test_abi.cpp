// G0: the C ABI of moproj2mst.dll (argument checks, message buffers, stub-phase result).
// ctest-labels: abi
#include <windows.h>

#include <cstdlib>
#include <cstring>
#include <string>

#include <testkit/check.hpp>

#include "moproj2mst.h"

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

int main()
{
    return testkit::run_all();
}
