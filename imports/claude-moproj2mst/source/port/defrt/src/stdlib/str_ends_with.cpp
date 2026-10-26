// FUN_1401b0cc0 (defrt/stdlib/str_ends_with.hpp). Decompile deform-rea-work/out/p8mg/p8-08/h4.c.
#define _CRT_SECURE_NO_WARNINGS
#include <defrt/stdlib/str_ends_with.hpp>

#include <cstdlib>
#include <cstring>

namespace def::rt {

// @src 1401b0cc0
bool str_ends_with(std::string s, const char* p)
{
    const int n = static_cast<int>(std::strlen(p));
    std::string a = s;                        // 140005dd0 copy
    const std::string b(p);                   // 1400066a0
    const int d = static_cast<int>(a.size()) - n;
    if (d < 1) {
        return false;
    }
    a = a.substr(a.size() - static_cast<std::size_t>(n));  // 140006760, 1400066f0
    char* x = static_cast<char*>(std::malloc(static_cast<std::size_t>(n) + 1));  // 1404797d8
    char* y = static_cast<char*>(std::malloc(static_cast<std::size_t>(n) + 1));
    std::strcpy(x, a.c_str());
    std::strcpy(y, b.c_str());
    for (int i = 0; i < n; ++i) {
        if ('@' < x[i] && x[i] < '[') x[i] = static_cast<char>(x[i] + ' ');
        if ('@' < y[i] && y[i] < '[') y[i] = static_cast<char>(y[i] + ' ');
    }
    const bool eq = std::strncmp(x, y, static_cast<std::size_t>(n)) == 0;
    std::free(x);
    std::free(y);
    return eq;
}

} // namespace def::rt
