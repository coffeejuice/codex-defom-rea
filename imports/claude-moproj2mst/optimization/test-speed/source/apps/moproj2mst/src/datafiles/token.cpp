// P06: the KEY token reader 0x1409068f0 (twin of DEF_PRE 0x140411ad0, port/defcmd geolib/token.cpp).
#include <cstdlib>
#include <cstring>

#include "m2m/datafiles.hpp"

namespace m2m::datafiles {

namespace {

constexpr char kSep[] = " ,:=\n\t\r";   // 0x1427b9e90 / 0x1427b9ea0

// toupper / tolower of the static CRT in the "C" locale: only a-z / A-Z change; a byte above 0x7f
// arrives as a negative int and comes back unchanged.
char upper(char c) { return (c >= 'a' && c <= 'z') ? static_cast<char>(c - 'a' + 'A') : c; }
char lower(char c) { return (c >= 'A' && c <= 'Z') ? static_cast<char>(c - 'A' + 'a') : c; }

}  // namespace

// @src 0x140906880
char* upperInPlace(char* s)
{
    for (int i = 0; s[i] != '\0'; i = i + 1) s[i] = upper(s[i]);
    return s;
}

// @src 0x140906790
char* replaceChar(char* s, char from, char to, int exact)
{
    const char lo = lower(from);
    const char up = upper(from);
    for (int i = 0; s[i] != '\0'; i = i + 1) {
        if (exact == 1) {
            if (s[i] == from) s[i] = to;
        } else if (s[i] == lo || s[i] == up) {
            s[i] = to;
        }
    }
    return s;
}

// @src 0x140906740
TokenReader::TokenReader() : mode(0) {}

// fgets(line, 2000, f) into the fixed buffer (0x1414733ac)
static bool fgetsInto(TextFile& f, char* buf, int n)
{
    std::string s;
    if (!f.fgets(s, n)) return false;
    std::memcpy(buf, s.data(), s.size());
    buf[s.size()] = '\0';
    return true;
}

static void store(void* out, const char* s)
{
    if (out) *static_cast<std::string*>(out) = s;
}

// @src 0x1409068f0
int TokenReader::read(TextFile& f, int type, void* out, int pos)
{
    char* tok;
    int ret;
    if (type == Line) {
        if (!fgetsInto(f, line_, sizeof line_)) {
            store(out, "DONE");                                   // 0x1427b9e78
            return -2;
        }
        tok_[1] = std::strtok(line_, "\n\r");                     // 0x1427b9e80
        if (tok_[1] == nullptr) tok_[1] = line_;
        tok = tok_[1];
        next_ = 2;
        count_ = 1;
        ret = 0;
    } else if (pos == 0 || pos == kNewLine) {
        if (count_ < next_ || pos == kNewLine) {
            if (pos == kNewLine) ret = 0;
            else if (count_ < pos) ret = -1;                      // dead: pos is 0 here
            else ret = 0;
            do {
                if (!fgetsInto(f, line_, sizeof line_)) {
                    if (type == Upper) store(out, "DONE");          // 0x1427b9e84
                    return -2;
                }
                if (mode == 1) line_[63] = '\0';                  // 0x143dd869f
                if (type == Raw) {
                    tok_[1] = std::strtok(line_, "\n\r");         // 0x1427b9e98
                } else {
                    char* c = std::strstr(line_, "//");           // 0x1400b22e0, 0x1427b9e8c
                    if (c != nullptr) *c = '\0';
                    tok_[1] = std::strtok(line_, kSep);
                }
                count_ = 1;
                for (;;) {
                    // The original's table has no bound (a line of more than 2000 bytes cannot
                    // hold more than 1000 tokens); this one has room for 259.
                    char* t = std::strtok(nullptr, kSep);
                    if (count_ + 1 >= static_cast<int>(sizeof tok_ / sizeof tok_[0])) t = nullptr;
                    tok_[count_ + 1] = t;
                    if (t == nullptr) break;
                    count_ = count_ + 1;
                }
            } while (tok_[1] == nullptr || *tok_[1] == '*');
            tok = tok_[1];
            next_ = 2;
        } else {
            tok = tok_[next_];
            next_ = next_ + 1;
            ret = 0;
        }
    } else {
        if (pos < 1 || count_ < pos) return -1;
        tok = tok_[pos];
        ret = 0;
    }
    switch (type - 1) {
    case 0:
        *static_cast<std::uint8_t*>(out) = std::atol(tok) != 0;  // 0x1414737ac
        break;
    case 1:
        upperInPlace(tok);
        store(out, tok);
        break;
    case 2:
    case 3:
        store(out, tok);
        break;
    case 4:
    case 5:
    case 6:
        *static_cast<std::int32_t*>(out) = static_cast<std::int32_t>(std::atol(tok));
        break;
    case 7: {
        char* end = nullptr;                                      // 0x141475c74 (base 10)
        *static_cast<std::int32_t*>(out) = static_cast<std::int32_t>(std::strtoul(tok, &end, 10));
        break;
    }
    case 8:
        replaceChar(tok, 'D', 'E', 0);
        *static_cast<float*>(out) = static_cast<float>(std::atof(tok));   // 0x1414737a4, CVTSD2SS
        break;
    case 9: {
        replaceChar(tok, 'D', 'E', 0);
        char* end = nullptr;
        *static_cast<double*>(out) = std::strtod(tok, &end);     // 0x1414759b8
        break;
    }
    case 10:
        replaceChar(tok, 'D', 'E', 0);
        break;
    case 11:
        store(out, tok);
        break;
    default:
        break;
    }
    if (pos == kNewLine) ret = count_;
    return ret;
}

}  // namespace m2m::datafiles
