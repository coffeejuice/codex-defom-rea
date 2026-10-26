// geolib token reader 140411ad0 and its helpers (packet p8-09).
// Spec: docs/re/commands/GFREAD_STL_GEO_NAS.md section 4.6. Listing: deform-rea-work/out/p809/all_l.txt.
#define _CRT_SECURE_NO_WARNINGS
#include <defcmd/geolib.hpp>

#include <cctype>
#include <cstdlib>
#include <cstring>

namespace def::cmd::geolib {

namespace {

// The reader's globals: token table DAT_14070c080 (slot k = token k of the line, 8 bytes each,
// slot 1 = DAT_14070c088) directly followed by the line buffer DAT_14070c490 (2000 bytes).
// The table has room for 130 slots; a line with more tokens overwrites the start of the line
// buffer exactly as in the binary, so the two live in one byte block and slots are written with
// memcpy.
constexpr std::size_t kTableOff = 0;                 // 0x14070c080
constexpr std::size_t kLineOff = 0x14070c490 - 0x14070c080;
constexpr std::size_t kLineSize = 2000;

// @data 14070c080  token table + line buffer (DAT_14070c080 .. DAT_14070cc60)
alignas(8) unsigned char g_block[kLineOff + kLineSize];
// @data 14070c070  tokens on the current line
std::int32_t g_count = 0;
// @data 140629420  index of the next token (initial value 1 in .data)
std::int32_t g_next = 1;

char* line() { return reinterpret_cast<char*>(g_block + kLineOff); }

char* slot(std::int64_t k)
{
    char* p;
    std::memcpy(&p, g_block + kTableOff + static_cast<std::size_t>(k) * 8, sizeof p);
    return p;
}

void set_slot(std::int64_t k, char* p)
{
    std::memcpy(g_block + kTableOff + static_cast<std::size_t>(k) * 8, &p, sizeof p);
}

} // namespace

void reset_token_reader()
{
    std::memset(g_block, 0, sizeof g_block);
    g_count = 0;
    g_next = 1;
}

// @src 140411920  ctor
TokenReader* fun_140411920(TokenReader* r)
{
    r->fixed63 = 0;
    return r;
}

// @src 140411940  dtor (empty)
void fun_140411940(TokenReader*) {}

// @src 140411970  replace from by to (exact == 1: only that character; else its lower and upper
// case forms, tolower/toupper of the argument)
char* fun_140411970(TokenReader*, char* s, char from, char to, std::int32_t exact)
{
    const int lo = std::tolower(static_cast<int>(from));
    const int up = std::toupper(static_cast<int>(from));
    for (std::int32_t i = 0; s[i] != '\0'; i = i + 1) {
        if (exact == 1) {
            if (s[i] == from) s[i] = to;
        } else if (s[i] == static_cast<char>(lo) || s[i] == static_cast<char>(up)) {
            s[i] = to;
        }
    }
    return s;
}

// @src 140411a60  upper-case in place
char* fun_140411a60(TokenReader*, char* s)
{
    for (std::int32_t i = 0; s[i] != '\0'; i = i + 1) {
        s[i] = static_cast<char>(std::toupper(static_cast<int>(s[i])));
    }
    return s;
}

// @src 140411ad0  read one token and convert it by type
std::int32_t fun_140411ad0(TokenReader* r, std::FILE* f, std::int32_t type, void* out, std::int32_t pos)
{
    static const char kSep[] = " ,:=\n\t\r";  // str 140564690 (both strtok calls)
    char* tok;
    std::int32_t ret;
    if (type == 12) {
        if (std::fgets(line(), static_cast<int>(kLineSize), f) == nullptr) {
            std::strcpy(static_cast<char*>(out), "DONE");  // str 1405646b0
            return -2;
        }
        set_slot(1, std::strtok(line(), "\n\r"));
        if (slot(1) == nullptr) set_slot(1, line());
        tok = slot(1);
        g_next = 2;
        g_count = 1;
        ret = 0;
    } else if (pos == 0 || pos == tok_new_line) {
        if (g_count < g_next || pos == tok_new_line) {
            if (pos == tok_new_line) {
                ret = 0;
            } else if (g_count < pos) {
                ret = -1;  // dead: pos is 0 here (kept as decompiled)
            } else {
                ret = 0;
            }
            do {
                if (std::fgets(line(), static_cast<int>(kLineSize), f) == nullptr) {
                    if (type == 2) std::strcpy(static_cast<char*>(out), "DONE");  // str 1405646bc
                    return -2;
                }
                if (r->fixed63 == 1) line()[63] = '\0';  // DAT_14070c4cf
                if (type == 4) {
                    set_slot(1, std::strtok(line(), "\n\r"));
                } else {
                    char* c = std::strstr(line(), "//");
                    if (c != nullptr) *c = '\0';
                    set_slot(1, std::strtok(line(), kSep));
                }
                g_count = 1;
                for (;;) {
                    set_slot(static_cast<std::int64_t>(g_count + 1), std::strtok(nullptr, kSep));
                    if (slot(static_cast<std::int64_t>(g_count + 1)) == nullptr) break;
                    g_count = g_count + 1;
                }
            } while (slot(1) == nullptr || *slot(1) == '*');
            tok = slot(1);
            g_next = 2;
        } else {
            tok = slot(g_next);
            g_next = g_next + 1;
            ret = 0;
        }
    } else {
        if (pos < 1 || g_count < pos) return -1;
        tok = slot(pos);
        ret = 0;
    }
    switch (type - 1) {
    case 0:
        *static_cast<bool*>(out) = std::atol(tok) != 0;  // 14047b6c0
        break;
    case 1:
        fun_140411a60(r, tok);
        std::strcpy(static_cast<char*>(out), tok);
        break;
    case 2:
    case 3:
        std::strcpy(static_cast<char*>(out), tok);
        break;
    case 4:
        *static_cast<std::int32_t*>(out) = static_cast<std::int32_t>(std::atol(tok));  // 14047b6c0
        break;
    case 5:
    case 6:
        *static_cast<std::int32_t*>(out) = std::atoi(tok);  // 14047b6a4
        break;
    case 7: {
        char* end = nullptr;
        *static_cast<std::uint32_t*>(out) = static_cast<std::uint32_t>(std::strtoul(tok, &end, 10));
        break;
    }
    case 8:
        fun_140411970(r, tok, 'D', 'E', 0);
        *static_cast<float*>(out) = static_cast<float>(std::atof(tok));  // 14048754c, CVTSD2SS
        break;
    case 9: {
        fun_140411970(r, tok, 'D', 'E', 0);
        char* end = nullptr;
        *static_cast<double*>(out) = std::strtod(tok, &end);  // 1404880dc
        break;
    }
    case 10:
        fun_140411970(r, tok, 'D', 'E', 0);
        break;
    case 11:
        std::strcpy(static_cast<char*>(out), tok);
        break;
    default:
        break;
    }
    if (pos == tok_new_line) ret = g_count;
    return ret;
}

} // namespace def::cmd::geolib
