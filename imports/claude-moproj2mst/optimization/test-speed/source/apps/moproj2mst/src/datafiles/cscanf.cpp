// P06 internal: the static MSVC CRT's scanf engine (_input) for the directives the readers use.
// Number text is collected the way _input collects it; the conversion of the collected text uses
// strtof / strtod (correctly rounded; the CRT's _fassign goes through a 12-byte long double, which
// only differs for inputs with more significant digits than the MDT writer's %15.7E produces).
#include <cstdint>
#include <cstdlib>
#include <cstring>

#include "cscanf.hpp"

namespace m2m::datafiles {

namespace {

bool isSpace(int c) { return c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\v' || c == '\f'; }
bool isDigit(int c) { return c >= '0' && c <= '9'; }

}  // namespace

int cscan(TextFile& f, const char* fmt, std::initializer_list<void*> args)
{
    const void* const* arg = args.begin();
    const void* const* argEnd = args.end();
    int count = 0;
    bool anyConversion = false;
    auto next = [&]() -> void* { return arg < argEnd ? const_cast<void*>(*arg++) : nullptr; };
    auto inc = [&]() { return f.getc(); };
    auto unInc = [&](int ch) { if (ch != -1) f.ungetc(); };
    auto fail = [&](bool eof) { return (eof && !anyConversion && count == 0) ? -1 : count; };

    const char* p = fmt;
    while (*p) {
        unsigned char fc = static_cast<unsigned char>(*p);
        if (isSpace(fc)) {
            while (isSpace(static_cast<unsigned char>(*p))) ++p;
            int ch;
            do { ch = inc(); } while (isSpace(ch));
            unInc(ch);
            continue;
        }
        if (fc != '%' || p[1] == '%') {
            if (fc == '%') ++p;
            int ch = inc();
            if (ch != static_cast<unsigned char>(*p)) {
                unInc(ch);
                return fail(ch == -1);
            }
            ++p;
            continue;
        }
        ++p;                                           // past '%'
        bool suppress = false;
        bool longone = false;
        if (*p == '*') { suppress = true; ++p; }
        while (isDigit(static_cast<unsigned char>(*p))) ++p;   // widths are not used by the readers
        if (*p == 'l') { longone = true; ++p; }
        char conv = *p++;

        int ch;
        if (conv != '[') {
            do { ch = inc(); } while (isSpace(ch));
            if (ch == -1) return fail(true);
        } else {
            ch = inc();
            if (ch == -1) return fail(true);
        }
        anyConversion = true;

        switch (conv) {
        case 's': {
            std::string w;
            while (ch != -1 && !isSpace(ch)) { w += static_cast<char>(ch); ch = inc(); }
            unInc(ch);
            if (!suppress) {
                if (void* t = next()) *static_cast<std::string*>(t) = w;
                ++count;
            }
            break;
        }
        case '[': {
            bool negate = false;
            if (*p == '^') { negate = true; ++p; }
            bool table[256] = {};
            if (*p == ']') { table[static_cast<unsigned char>(']')] = true; ++p; }
            while (*p && *p != ']') { table[static_cast<unsigned char>(*p)] = true; ++p; }
            if (*p == ']') ++p;
            std::string w;
            while (ch != -1 && (table[ch] != negate)) { w += static_cast<char>(ch); ch = inc(); }
            unInc(ch);
            if (w.empty()) return fail(false);
            if (!suppress) {
                if (void* t = next()) *static_cast<std::string*>(t) = w;
                ++count;
            }
            break;
        }
        case 'd': {
            bool negative = false;
            if (ch == '-') { negative = true; ch = inc(); }
            else if (ch == '+') { ch = inc(); }
            std::uint32_t number = 0;
            bool started = false;
            while (isDigit(ch)) {
                number = number * 10u + static_cast<std::uint32_t>(ch - '0');
                started = true;
                ch = inc();
            }
            unInc(ch);
            if (!started) return fail(false);
            if (negative) number = 0u - number;
            if (!suppress) {
                if (void* t = next()) {
                    std::int32_t v = static_cast<std::int32_t>(number);
                    std::memcpy(t, &v, 4);
                }
                ++count;
            }
            break;
        }
        case 'e': case 'f': case 'g': case 'E': case 'G': {
            std::string s;
            int started = 0;
            if (ch == '-') { s += '-'; ch = inc(); }
            else if (ch == '+') { ch = inc(); }
            while (isDigit(ch)) { ++started; s += static_cast<char>(ch); ch = inc(); }
            if (ch == '.') {
                ch = inc();
                s += '.';
                while (isDigit(ch)) { ++started; s += static_cast<char>(ch); ch = inc(); }
            }
            if (started && (ch == 'e' || ch == 'E')) {
                s += 'e';
                ch = inc();
                if (ch == '-') { s += '-'; ch = inc(); }
                else if (ch == '+') { ch = inc(); }
                while (isDigit(ch)) { ++started; s += static_cast<char>(ch); ch = inc(); }
            }
            unInc(ch);
            if (!started) return fail(false);
            if (!suppress) {
                if (void* t = next()) {
                    if (longone) {
                        double d = std::strtod(s.c_str(), nullptr);
                        std::memcpy(t, &d, 8);
                    } else {
                        float v = std::strtof(s.c_str(), nullptr);
                        std::memcpy(t, &v, 4);
                    }
                }
                ++count;
            }
            break;
        }
        default:
            unInc(ch);
            return count;
        }
    }
    return count;
}

}  // namespace m2m::datafiles
