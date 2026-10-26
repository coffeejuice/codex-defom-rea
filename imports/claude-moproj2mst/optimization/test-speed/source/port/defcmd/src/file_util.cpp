// Small file helpers (packet p8-07).
#define _CRT_SECURE_NO_WARNINGS
#include <defcmd/file_util.hpp>

#include <cstdio>
#include <cstring>
#include <share.h>

namespace def::cmd {

// @src 1400b9130  (outside the ledger range; 14047b934 = _fsopen(.., _SH_DENYNO), mode str 14050c8a8)
bool write_text_file(const char* name, const char* text)
{
    if (name == nullptr) return false;
    if (std::strlen(name) == 0) return false;
    std::FILE* f = _fsopen(name, "wt", _SH_DENYNO);
    if (f == nullptr) return false;
    if (text != nullptr) {
        const std::int32_t n = static_cast<std::int32_t>(std::strlen(text));
        if (0 < n) std::fwrite(text, static_cast<std::size_t>(n), 1, f);
    }
    std::fclose(f);
    return true;
}

} // namespace def::cmd
