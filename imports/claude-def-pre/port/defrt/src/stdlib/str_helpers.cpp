// defrt/stdlib/str_helpers.cpp: std::string helpers 1401b0870, 1401b0940 and the path-name
// wrapper 1401b19c0 / 1401b1a10 / 1401b1a30 (packet p8-05), from the disassembly
// (deform-rea-work/out/p8mg/p8-05/str.asm).
#include <defrt/stdlib/str_helpers.hpp>

#include <cstdio>
#include <cstring>
#include <utility>

namespace def::rt {

// @src 1401b0870  sscanf(s.c_str(), "%d", &v) with v = 0 first (1401b0882); returns v
std::int32_t str_to_int(std::string s)
{
    std::int32_t v = 0;
    std::sscanf(s.c_str(), "%d", &v); // str 140524d18
    return v;
}

// @src 1401b0940  prefix compare; exact -> strncmp(s, p, strlen(p)), else case-folded copies
bool str_starts_with(std::string s, const char* p, bool exact)
{
    const std::int32_t n = static_cast<std::int32_t>(std::strlen(p));
    if (exact) {
        return std::strncmp(s.c_str(), p, static_cast<std::size_t>(n)) == 0;
    }
    std::string a = s;                    // 1401b0a11 copy
    std::string b(p);                     // 1401b0a24
    a.resize(static_cast<std::size_t>(n)); // 1401b0a34 (pads with NULs)
    // 1401b0a43 / 1401b0a67: two malloc(n + 1) buffers, strcpy of both c_str(); bytes after the
    // copied NUL are never compared (strncmp stops at the first NUL), so they are zeroed here.
    std::string x(static_cast<std::size_t>(n) + 1, '\0');
    std::string y(static_cast<std::size_t>(n) + 1, '\0');
    std::strcpy(x.data(), a.c_str());
    std::strcpy(y.data(), b.c_str());
    for (std::int32_t i = 0; i < n; i = i + 1) {
        char& cx = x[static_cast<std::size_t>(i)];
        if (cx >= 'A' && cx <= 'Z') cx = static_cast<char>(cx + 0x20);
        char& cy = y[static_cast<std::size_t>(i)];
        if (cy >= 'A' && cy <= 'Z') cy = static_cast<char>(cy + 0x20);
    }
    return std::strncmp(x.c_str(), y.c_str(), static_cast<std::size_t>(n)) == 0;
}

// @src 1401b1060  trim (owner p4-01; def_pre's trim / con_trim call this body)
std::string str_trim(std::string s)
{
    const auto blank = [](char c) {
        return c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r' || c == ' ' || c == '?';
    };
    const std::int32_t n = static_cast<std::int32_t>(std::strlen(s.c_str()));
    std::int32_t first = 0;
    std::int32_t last = n - 1;
    for (std::int32_t i = 0; i < n; i = i + 1) {
        if (!blank(s[static_cast<std::size_t>(i)])) {
            first = i;
            break;
        }
    }
    for (std::int32_t i = n - 1; 0 <= i; i = i - 1) {
        if (!blank(s[static_cast<std::size_t>(i)])) {
            last = i;
            break;
        }
    }
    std::int32_t len = last - first + 1;
    if (len < 0) {
        len = 0;
    }
    return s.substr(static_cast<std::size_t>(first), static_cast<std::size_t>(len)); // 140006760
}

// @src 1401b19c0  ctor: default-construct, assign the by-value argument, destroy it
Cls_1401b19c0::Cls_1401b19c0(std::string s) : path(std::move(s)) {}

Cls_1401b19c0::~Cls_1401b19c0() = default;

// @src 1401b1a30  stem (the 1401b1a6e copy of the string is unused)
std::string Cls_1401b19c0::stem() const
{
    const char* s = path.c_str();
    const std::int32_t len = static_cast<std::int32_t>(std::strlen(s));
    std::int32_t end = len - 1;   // [rsp+0x2c]
    std::int32_t start = 0;       // [rsp+0x28]
    for (std::int32_t i = len - 1; i >= 0; i = i - 1) {
        if (s[i] == '.') {
            end = i - 1;
            break;
        }
    }
    for (std::int32_t i = end; i >= 0; i = i - 1) {
        if (s[i] == '\\' || s[i] == '/') {
            start = i + 1;
            break;
        }
    }
    std::int32_t count = end - start + 1;
    if (count < 0) count = 0;
    // 1401b1b88: assign(str, start, count); start <= len here
    return path.substr(static_cast<std::size_t>(start), static_cast<std::size_t>(count));
}

} // namespace def::rt
