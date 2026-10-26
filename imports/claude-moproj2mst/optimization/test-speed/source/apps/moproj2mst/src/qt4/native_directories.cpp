#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <algorithm>
#include <string>
#include <vector>
#include "m2m/common.hpp"
#include "m2m/native_directories.hpp"
#include "m2m/qt4path.hpp"

namespace m2m::qt {
namespace {
// QtCore 670ffbf0 then 670f7fc0. Qt always adds the extended prefix,
// including for short paths. The independent port long-path helper does not.
std::wstring nativeName(const std::wstring& input)
{
    if (input.starts_with(L"\\\\.\\")) return input;
    std::vector<wchar_t> buffer(std::max<std::size_t>(260, input.size() + 1));
    wchar_t* leaf = nullptr;
    DWORD length = GetFullPathNameW(input.c_str(), static_cast<DWORD>(buffer.size()), buffer.data(), &leaf);
    if (length > buffer.size()) {
        buffer.resize(length);
        length = GetFullPathNameW(input.c_str(), static_cast<DWORD>(buffer.size()), buffer.data(), &leaf);
    }
    // An unstable process cwd/oversized second return cannot occur for the
    // absolute Save paths; do not read outside storage on an adapter violation.
    if (length >= buffer.size())
        throw Error(M2M_E_UNSUPPORTED, "Qt directory full-path buffer changed during resolution");
    std::wstring full = length ? std::wstring(buffer.data(), length) : std::wstring();
    if (!input.empty() && input.back() == L' ') full += L' '; // 670ffd55
    std::wstring prefix = L"\\\\?\\";
    if (full.starts_with(L"\\\\") && full.size() > 2 && full[2] != L'.') {
        prefix += L"UNC\\";
        full.erase(0, 2);
    }
    return prefix + full;
}

// 670fed20 and the 670fe9d0 access/sharing-error fallback.
bool directoryExists(std::wstring path, bool& exists)
{
    if (path.size() == 2 && path[1] == L':') path += L'\\';
    DWORD attributes = GetFileAttributesW(nativeName(path).c_str());
    if (attributes == INVALID_FILE_ATTRIBUTES) {
        const DWORD error = GetLastError();
        if (error == ERROR_ACCESS_DENIED || error == ERROR_SHARING_VIOLATION) {
            auto name = nativeName(path);
            while (!name.empty() && name.back() == L'\\') name.pop_back();
            if (!name.empty() && name.back() != L':') {
                WIN32_FIND_DATAW data{};
                const HANDLE find = FindFirstFileW(name.c_str(), &data);
                if (find != INVALID_HANDLE_VALUE) {
                    FindClose(find);
                    attributes = data.dwFileAttributes;
                }
            }
        }
    }
    exists = attributes != INVALID_FILE_ATTRIBUTES;
    return exists && (attributes & FILE_ATTRIBUTE_DIRECTORY) != 0;
}
}

bool nativeMkpathAbsolute(const String& input)
{
    if (input.isEmpty()) return false; // QDir::mkpath 670ac0ca
    const auto normalized = convertSeparators(cleanPath(input)).u16();
    const bool drive = normalized.size() >= 3 && normalized[1] == u':' && normalized[2] == u'\\';
    const bool unc = normalized.starts_with(u"\\\\");
    if (!drive && !unc)
        throw Error(M2M_E_UNSUPPORTED, "native Save directory requires an absolute filesystem path");
    const std::wstring path(normalized.begin(), normalized.end());
    std::size_t previous = std::wstring::npos;
    if (unc) {
        const auto server = path.find_first_not_of(L'\\');
        if (server != std::wstring::npos) previous = path.find(L'\\', server);
    }
    for (;;) {
        auto next = path.find(L'\\', previous == std::wstring::npos ? 0 : previous + 1);
        if (next == std::wstring::npos) {
            next = path.size();
            if (previous == next) return true;
        }
        if (next != 0) {
            const auto component = path.substr(0, next);
            bool exists = false;
            if (!directoryExists(component, exists)) {
                if (exists || !CreateDirectoryW(nativeName(component).c_str(), nullptr)) return false;
            }
        }
        previous = next;
    }
}
}
