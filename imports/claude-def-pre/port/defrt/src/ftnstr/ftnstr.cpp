// C <-> Fortran fixed-length string helpers (packet p1-10). See defrt/ftnstr/ftnstr.hpp.
#define _CRT_SECURE_NO_WARNINGS // strncpy / getenv are what the original and the test use
#include <defrt/ftnstr/ftnstr.hpp>

#include <cstring>

namespace def::rt {

namespace {

bool is_trim_char(char c)
{
    return c == ' ' || c == '\t' || c == '\r' || c == '\n' || c == '\0';
}

} // namespace

// @src 1403239f0  trim trailing blanks/tabs/CR/LF/NULs of a Fortran field (FUN_1403239f0)
void trim_fortran(char* s, std::int32_t n)
{
    // The original loop always reads s[n-1] first, also when n <= 0 (s[-1]); for n <= 0 the
    // result is s[0] = 0 whatever it reads, so the port skips that read.
    std::int32_t end = n; // iVar1: one past the last kept character
    if (n > 0) {
        std::int32_t i = n;
        std::int32_t k;
        do {
            end = i;
            k = end - 1;
            if (!is_trim_char(s[k]))
                break;
            i = k;
        } while (0 < k);
    }
    std::int32_t m = n;
    if (end < n)
        m = end;
    std::int32_t cut = (m < 0) ? 0 : m;
    s[cut] = '\0';
}

// @src 140323b90  blank-pad a C string to n characters (FUN_140323b90)
void blank_pad(char* s, std::int32_t n)
{
    for (std::int32_t i = static_cast<std::int32_t>(std::strlen(s)); i < n; ++i)
        s[i] = ' ';
}

// @src 140323bf0  C string to a blank-padded Fortran field (FUN_140323bf0)
std::uint32_t to_fortran(char* dst, const char* src, std::int32_t n)
{
    std::size_t len = std::strlen(src);
    std::strncpy(dst, src, static_cast<std::size_t>(static_cast<std::int64_t>(n)));
    // 140323b90 call. When src filled all n bytes, dst has no terminator and the original's
    // strlen runs past the field; its result is then >= n and no padding happens. strnlen gives
    // the same result without the overread.
    std::size_t have = n > 0 ? strnlen(dst, static_cast<std::size_t>(n)) : 0;
    for (std::int32_t i = static_cast<std::int32_t>(have); i < n; ++i)
        dst[i] = ' ';
    return static_cast<std::uint32_t>(len & 0xffffffffu);
}

} // namespace def::rt
