#define _CRT_SECURE_NO_WARNINGS
#include <defkey/crt_scan.hpp>

#include <cstring>

namespace def::key::crt {

std::string scan_format(const char* fmt)
{
    std::string out;
    const char* p = fmt;
    while (*p != '\0') {
        if (*p != '%') {
            out += *p++;
            continue;
        }
        out += *p++; // '%'
        if (*p == '%') {
            out += *p++;
            continue;
        }
        if (*p == '*') out += *p++;
        while (*p >= '0' && *p <= '9') out += *p++;
        // Old-CRT size prefixes: dropped when another character follows (the conversion or a
        // further size letter).
        while ((*p == 'F' || *p == 'N') && p[1] != '\0') ++p;
        while (*p == 'h' || *p == 'l' || *p == 'L' || *p == 'w' || *p == 'I') {
            const bool wide_int = *p == 'I' && ((p[1] == '6' && p[2] == '4') || (p[1] == '3' && p[2] == '2'));
            out += *p++;
            if (wide_int) {
                out += *p++;
                out += *p++;
            }
        }
        if (*p == '[') {
            out += *p++;
            if (*p == '^') out += *p++;
            if (*p == ']') out += *p++;
            while (*p != '\0' && *p != ']') out += *p++;
            if (*p == ']') out += *p++;
            continue;
        }
        if (*p != '\0') out += *p++;
    }
    return out;
}

int vsscanf(const char* s, const char* fmt, std::va_list ap)
{
    const std::string f = scan_format(fmt);
    return std::vsscanf(s, f.c_str(), ap);
}

int sscanf(const char* s, const char* fmt, ...)
{
    std::va_list ap;
    va_start(ap, fmt);
    const int r = crt::vsscanf(s, fmt, ap);
    va_end(ap);
    return r;
}

int vfscanf(std::FILE* f, const char* fmt, std::va_list ap)
{
    const std::string g = scan_format(fmt);
    return std::vfscanf(f, g.c_str(), ap);
}

int fscanf(std::FILE* f, const char* fmt, ...)
{
    std::va_list ap;
    va_start(ap, fmt);
    const int r = crt::vfscanf(f, fmt, ap);
    va_end(ap);
    return r;
}

} // namespace def::key::crt
