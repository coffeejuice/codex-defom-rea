// Long-path form for Win32 file calls (include/m2m/longpath.hpp). Port addition, no @src.
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "m2m/longpath.hpp"

namespace m2m {

namespace {

constexpr size_t kShortLimit = 248;   // CreateDirectoryW's limit; MAX_PATH - 12

bool hasDevicePrefix(const std::wstring& p)
{
    return p.size() >= 4 && (p[0] == L'\\' || p[0] == L'/') && (p[1] == L'\\' || p[1] == L'/') &&
           (p[2] == L'?' || p[2] == L'.') && (p[3] == L'\\' || p[3] == L'/');
}

}  // namespace

std::wstring fullPathName(const std::wstring& path)
{
    if (path.empty()) return {};
    std::wstring buf(MAX_PATH, L'\0');
    for (int tries = 0; tries < 4; ++tries) {
        const DWORD n = GetFullPathNameW(path.c_str(), static_cast<DWORD>(buf.size()), buf.data(), nullptr);
        if (n == 0) return {};
        if (n < buf.size()) {
            buf.resize(n);
            return buf;
        }
        buf.assign(n + 1, L'\0');   // n = the size needed, terminator included
    }
    return {};
}

std::wstring win32Path(const std::wstring& path)
{
    if (path.empty() || hasDevicePrefix(path)) return path;
    if (path.size() < kShortLimit) {
        // A relative name can still resolve to a long full path.
        bool relative = !(path.size() >= 2 && path[1] == L':') && !(path[0] == L'\\' || path[0] == L'/');
        if (!relative) return path;
    }
    const std::wstring full = fullPathName(path);
    if (full.empty() || full.size() < kShortLimit) return path;
    if (full.size() >= 2 && full[0] == L'\\' && full[1] == L'\\')
        return L"\\\\?\\UNC\\" + full.substr(2);
    return L"\\\\?\\" + full;
}

std::wstring win32Path(const std::u16string& path)
{
    return win32Path(std::wstring(path.begin(), path.end()));
}

}  // namespace m2m
