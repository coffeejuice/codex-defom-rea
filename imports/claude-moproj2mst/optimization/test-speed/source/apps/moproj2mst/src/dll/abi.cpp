// The C ABI of include/moproj2mst.h (owner: G0). Compiled into moproj2mst.dll (M2M_BUILD_DLL)
// and into the CLI (M2M_STATIC). No exception crosses the boundary.
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <atomic>
#include <cstring>
#include <limits>
#include <mutex>
#include <string>

#include "moproj2mst.h"
#include "m2m/convert.hpp"

namespace {

std::atomic<unsigned> g_codepage{1252};
std::atomic<int> g_guiUnit{0};
std::mutex g_deformDirLock;
std::u16string g_deformDir;     // m2m_set_deform_dir_w

std::u16string deformDir()
{
    std::lock_guard<std::mutex> lock(g_deformDirLock);
    return g_deformDir;
}

// Adapter contract: snapshot once per C ABI call, never consult the original INI.
// Unset/empty means 0. Otherwise accept optional +/- and ASCII decimal digits only,
// within signed 32-bit range. Invalid input is an adapter argument error.
m2m::OriginalSettings originalSettings()
{
    std::wstring value(16, L'\0');
    for (;;) {
        SetLastError(ERROR_SUCCESS);
        const DWORD n = GetEnvironmentVariableW(L"M2M_MO_INPUT_MODE", value.data(),
                                                static_cast<DWORD>(value.size()));
        if (n == 0) {
            if (GetLastError() == ERROR_SUCCESS || GetLastError() == ERROR_ENVVAR_NOT_FOUND) return {};
            throw m2m::Error(M2M_E_ARG, "cannot read M2M_MO_INPUT_MODE");
        }
        if (n < value.size()) { value.resize(n); break; }
        value.resize(n); // required size includes NUL; retry if concurrently changed
    }
    std::size_t i = 0;
    const bool negative = value[0] == L'-';
    if (negative || value[0] == L'+') ++i;
    const auto invalid = []() -> void {
        throw m2m::Error(M2M_E_ARG, "M2M_MO_INPUT_MODE must be a signed 32-bit decimal integer");
    };
    if (i == value.size()) invalid();
    const std::uint64_t limit = negative ? 2147483648ull : 2147483647ull;
    std::uint64_t magnitude = 0;
    for (; i < value.size(); ++i) {
        if (value[i] < L'0' || value[i] > L'9') invalid();
        const auto digit = static_cast<unsigned>(value[i] - L'0');
        if (magnitude > (limit - digit) / 10) invalid();
        magnitude = magnitude * 10 + digit;
    }
    m2m::OriginalSettings result;
    result.moInputMode = negative
        ? (magnitude == 2147483648ull ? (std::numeric_limits<int>::min)() : -static_cast<int>(magnitude))
        : static_cast<int>(magnitude);
    return result;
}

void putMsgW(const std::string& utf8, wchar_t* msg, size_t chars)
{
    if (!msg || chars == 0) return;
    msg[0] = 0;
    if (utf8.empty()) return;
    int n = MultiByteToWideChar(CP_UTF8, 0, utf8.data(), static_cast<int>(utf8.size()), nullptr, 0);
    if (n <= 0) return;
    std::wstring w(static_cast<size_t>(n), L'\0');
    MultiByteToWideChar(CP_UTF8, 0, utf8.data(), static_cast<int>(utf8.size()), w.data(), n);
    size_t k = std::min(w.size(), chars - 1);
    if (k > 0 && k < w.size() && IS_HIGH_SURROGATE(w[k - 1])) --k;   // do not split a pair
    std::memcpy(msg, w.data(), k * sizeof(wchar_t));
    msg[k] = 0;
}

void putMsgUtf8(const std::string& utf8, char* msg, size_t bytes)
{
    if (!msg || bytes == 0) return;
    size_t k = std::min(utf8.size(), bytes - 1);
    while (k > 0 && k < utf8.size() && (static_cast<unsigned char>(utf8[k]) & 0xc0) == 0x80) --k;   // whole chars
    std::memcpy(msg, utf8.data(), k);
    msg[k] = 0;
}

bool fromUtf8(const char* s, std::u16string& out)
{
    out.clear();
    if (!s || !*s) return true;
    int len = static_cast<int>(std::strlen(s));
    int n = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, s, len, nullptr, 0);
    if (n <= 0) return false;
    std::wstring w(static_cast<size_t>(n), L'\0');
    MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, s, len, w.data(), n);
    out.assign(w.begin(), w.end());
    return true;
}

std::u16string fromWide(const wchar_t* s)
{
    if (!s) return {};
    std::wstring w(s);
    return std::u16string(w.begin(), w.end());
}

}  // namespace

extern "C" {

M2M_API int m2m_api_version(void)
{
    return M2M_API_VERSION;
}

M2M_API int m2m_set_codepage(unsigned codepage)
{
    if (!IsValidCodePage(codepage)) return M2M_E_ARG;
    g_codepage.store(codepage);
    return M2M_OK;
}

M2M_API int m2m_set_gui_unit(unsigned unit)
{
    if (unit > 2) return M2M_E_ARG;
    g_guiUnit.store(static_cast<int>(unit));
    return M2M_OK;
}

M2M_API int m2m_set_deform_dir_w(const wchar_t* dir)
{
    try {
        std::u16string d = fromWide(dir);
        std::lock_guard<std::mutex> lock(g_deformDirLock);
        g_deformDir = std::move(d);
        return M2M_OK;
    } catch (...) {
        return M2M_E_ARG;
    }
}

M2M_API int m2m_convert_w(const wchar_t* moproj_path, const wchar_t* out_mst_path, unsigned flags,
                          wchar_t* msg, size_t msg_chars)
{
    try {
        if (!moproj_path || !*moproj_path) {
            putMsgW("no project path", msg, msg_chars);
            return M2M_E_ARG;
        }
        m2m::Result r = m2m::convert(fromWide(moproj_path), fromWide(out_mst_path), flags, g_codepage.load(), g_guiUnit.load(), deformDir(), originalSettings());
        putMsgW(r.message, msg, msg_chars);
        return r.code;
    } catch (const m2m::Error& e) {
        putMsgW(e.what(), msg, msg_chars);
        return e.code;
    } catch (...) {
        putMsgW("internal error", msg, msg_chars);
        return M2M_E_INPUT;
    }
}

M2M_API int m2m_convert_utf8(const char* moproj_path, const char* out_mst_path, unsigned flags, char* msg,
                             size_t msg_bytes)
{
    try {
        std::u16string in, out;
        if (!moproj_path || !*moproj_path) {
            putMsgUtf8("no project path", msg, msg_bytes);
            return M2M_E_ARG;
        }
        if (!fromUtf8(moproj_path, in) || !fromUtf8(out_mst_path, out)) {
            putMsgUtf8("path is not valid UTF-8", msg, msg_bytes);
            return M2M_E_ARG;
        }
        m2m::Result r = m2m::convert(in, out, flags, g_codepage.load(), g_guiUnit.load(), deformDir(), originalSettings());
        putMsgUtf8(r.message, msg, msg_bytes);
        return r.code;
    } catch (const m2m::Error& e) {
        putMsgUtf8(e.what(), msg, msg_bytes);
        return e.code;
    } catch (...) {
        putMsgUtf8("internal error", msg, msg_bytes);
        return M2M_E_INPUT;
    }
}

}  // extern "C"
