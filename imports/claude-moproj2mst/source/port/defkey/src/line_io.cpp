#define _CRT_SECURE_NO_WARNINGS
#include <defkey/line_io.hpp>

#include <cstdlib>
#include <cstring>

#include <defkey/crt_scan.hpp>
#include <defkey/globals_key_common.hpp>

namespace def::key {

void reset_globals()
{
    std::memset(g_token_line, 0, sizeof g_token_line);
    for (char*& p : g_tokens) p = nullptr;
}

// @src 140323970
void replace_char(char* s, char from, char to)
{
    const int len = static_cast<int>(std::strlen(s));
    for (int i = 0; i < len; ++i) {
        if (s[i] == from) s[i] = to;
    }
}

namespace {

// part of @src 140413ee0 and the other scalar readers: fgets, ',' -> ' ', sscanf. Returns the
// sscanf result, or -2 for EOF at fgets.
int scan_line(std::FILE* f, char* buf, const char* fmt, ...)
{
    if (std::fgets(buf, kLineSize, f) == nullptr) return -2;
    replace_char(buf, ',', ' ');
    std::va_list ap;
    va_start(ap, fmt);
    const int r = crt::vsscanf(buf, fmt, ap);
    va_end(ap);
    return r;
}

// fgets, also skipping lines that are exactly "\n" (once, or while `repeat`). false on EOF.
bool get_line(std::FILE* f, char* buf, bool skip, bool repeat)
{
    if (std::fgets(buf, kLineSize, f) == nullptr) return false;
    if (skip) {
        while (std::strlen(buf) == 1 && buf[0] == '\n') {
            if (std::fgets(buf, kLineSize, f) == nullptr) return false;
            if (!repeat) break;
        }
    }
    return true;
}

int status(int r) { return (r == -2 || r == -1) ? 1 : 0; }

} // namespace

// @src 140413de0
int read_i(std::FILE* f, std::int32_t* a, bool skip_blank)
{
    char buf[kLineSize];
    *a = 0;
    if (!get_line(f, buf, skip_blank, false)) return 1;
    replace_char(buf, ',', ' ');
    return crt::sscanf(buf, "%i", a) == -1 ? 1 : 0;
}

// @src 140413ee0
int read_ii(std::FILE* f, std::int32_t* a, std::int32_t* b)
{
    char buf[kLineSize];
    *b = 0;
    *a = 0;
    return status(scan_line(f, buf, "%i %i", a, b));
}

// @src 140413fb0
int read_iii(std::FILE* f, std::int32_t* a, std::int32_t* b, std::int32_t* c, int* nconv)
{
    char buf[kLineSize];
    *c = 0;
    *b = 0;
    *a = 0;
    const int r = scan_line(f, buf, "%i %i %i", a, b, c);
    if (r == -2) return 1;
    if (nconv != nullptr) *nconv = r;
    return r == -1 ? 1 : 0;
}

// @src 140414100
int read_i4(std::FILE* f, std::int32_t* a, std::int32_t* b, std::int32_t* c, std::int32_t* d)
{
    char buf[kLineSize];
    *c = 0;
    *b = 0;
    *a = 0;
    *d = 0;
    return status(scan_line(f, buf, "%i %i %i %i", a, b, c, d));
}

// @src 140414200
int read_i5(std::FILE* f, std::int32_t* a, std::int32_t* b, std::int32_t* c, std::int32_t* d,
            std::int32_t* e, bool skip_blank_lines)
{
    char buf[kLineSize];
    *e = 0;
    *d = 0;
    *c = 0;
    *b = 0;
    *a = 0;
    if (!get_line(f, buf, skip_blank_lines, true)) return 1;
    replace_char(buf, ',', ' ');
    return crt::sscanf(buf, "%i %i %i %i %i", a, b, c, d, e) == -1 ? 1 : 0;
}

// @src 1404144b0
int read_i7(std::FILE* f, std::int32_t* const o[7])
{
    char buf[kLineSize];
    for (int i = 0; i < 7; ++i) *o[i] = 0;
    return status(scan_line(f, buf, "%i %i %i %i %i %i %i", o[0], o[1], o[2], o[3], o[4], o[5], o[6]));
}

// @src 140414600
int read_i9(std::FILE* f, std::int32_t* const o[9], bool skip_blank_lines)
{
    char buf[kLineSize];
    for (int i = 0; i < 9; ++i) *o[i] = 0;
    if (!get_line(f, buf, skip_blank_lines, true)) return 1;
    replace_char(buf, ',', ' ');
    return crt::sscanf(buf, "%i %i %i %i %i %i %i %i %i", o[0], o[1], o[2], o[3], o[4], o[5], o[6],
                       o[7], o[8]) == -1
               ? 1
               : 0;
}

// @src 1404147e0
int read_i10(std::FILE* f, std::int32_t* dst, const int* n)
{
    char buf[kLineSize];
    const int count = *n < 0xb ? *n : 10;
    std::int32_t v[10] = {}; // uninitialised stack in the original
    if (std::fgets(buf, kLineSize, f) == nullptr) return 1;
    replace_char(buf, ',', ' ');
    crt::sscanf(buf, "%i %i %i %i %i %i %i %i %i %i", &v[0], &v[1], &v[2], &v[3], &v[4], &v[5], &v[6],
                &v[7], &v[8], &v[9]);
    for (int i = 0; i < count; ++i) dst[i] = v[i];
    return 0;
}

// @src 140414990
int read_d(std::FILE* f, double* a)
{
    char buf[kLineSize];
    *a = 0;
    return status(scan_line(f, buf, "%Flf", a));
}

// @src 140414a40
int read_dd(std::FILE* f, double* a, double* b)
{
    char buf[kLineSize];
    *b = 0;
    *a = 0;
    return status(scan_line(f, buf, "%Flf %Flf", a, b));
}

// @src 140414c20
int read_d3(std::FILE* f, double* a, double* b, double* c)
{
    char buf[kLineSize];
    *b = 0;
    *a = 0;
    *c = 0;
    return status(scan_line(f, buf, "%Flf %Flf %Flf", a, b, c));
}

// @src 140414d10
int read_d3_not_one(std::FILE* f, double* a, double* b, double* c)
{
    char buf[kLineSize];
    *c = 0;
    *b = 0;
    *a = 0;
    const int r = scan_line(f, buf, "%Flf %Flf %Flf", a, b, c);
    if (r == -2 || r == -1) return 1;
    if (r == 1) {
        *c = 0;
        *b = 0;
        *a = 0;
    }
    return 0;
}

// @src 140414e40
int read_d4(std::FILE* f, double* a, double* b, double* c, double* d)
{
    char buf[kLineSize];
    *b = 0;
    *a = 0;
    *d = 0;
    *c = 0;
    return status(scan_line(f, buf, "%Flf %Flf %Flf %Flf", a, b, c, d));
}

// @src 140414f50
int read_d5(std::FILE* f, double* const o[5])
{
    char buf[kLineSize];
    for (int i = 0; i < 5; ++i) *o[i] = 0;
    return status(scan_line(f, buf, "%Flf %Flf %Flf %Flf %Flf", o[0], o[1], o[2], o[3], o[4]));
}

// @src 140415080
int read_d6(std::FILE* f, double* const o[6])
{
    char buf[kLineSize];
    for (int i = 0; i < 6; ++i) *o[i] = 0;
    return status(scan_line(f, buf, "%Flf %Flf %Flf %Flf %Flf %Flf", o[0], o[1], o[2], o[3], o[4], o[5]));
}

// @src 1404151c0
int read_d7(std::FILE* f, double* const o[7])
{
    char buf[kLineSize];
    for (int i = 0; i < 7; ++i) *o[i] = 0;
    return status(scan_line(f, buf, "%Flf %Flf %Flf %Flf %Flf %Flf %Flf", o[0], o[1], o[2], o[3], o[4],
                            o[5], o[6]));
}

// @src 140415320
int read_d9(std::FILE* f, double* const o[9])
{
    char buf[kLineSize];
    for (int i = 0; i < 9; ++i) *o[i] = 0;
    return status(scan_line(f, buf, "%Flf %Flf %Flf %Flf %Flf %Flf %Flf %Flf %Flf", o[0], o[1], o[2],
                            o[3], o[4], o[5], o[6], o[7], o[8]));
}

// @src 1404154c0
int read_i_d(std::FILE* f, std::int32_t* a, double* b)
{
    char buf[kLineSize];
    *a = 0;
    *b = 0;
    return status(scan_line(f, buf, "%i %Flf", a, b));
}

// @src 140415590
int read_i_d_i(std::FILE* f, std::int32_t* a, double* b, std::int32_t* c)
{
    char buf[kLineSize];
    *a = 0;
    *b = 0;
    *c = 0;
    return status(scan_line(f, buf, "%i %Flf %i", a, b, c));
}

// @src 140415680
int read_i_dd(std::FILE* f, std::int32_t* a, double* b, double* c)
{
    char buf[kLineSize];
    *a = 0;
    *c = 0;
    *b = 0;
    return status(scan_line(f, buf, "%i %Flf %Flf", a, b, c));
}

// @src 140415880
int read_i_d3(std::FILE* f, std::int32_t* a, double* b, double* c, double* d)
{
    char buf[kLineSize];
    *a = 0;
    *b = 0;
    *c = 0;
    *d = 0;
    return status(scan_line(f, buf, "%i %Flf %Flf %Flf", a, b, c, d));
}

// @src 140415990
int read_i_d4(std::FILE* f, std::int32_t* a, double* const d[4])
{
    char buf[kLineSize];
    *a = 0;
    for (int i = 0; i < 4; ++i) *d[i] = 0;
    return status(scan_line(f, buf, "%i %Flf %Flf %Flf %Flf", a, d[0], d[1], d[2], d[3]));
}

// @src 140415ac0
int read_i_d5(std::FILE* f, std::int32_t* a, double* const d[5])
{
    char buf[kLineSize];
    *a = 0;
    for (int i = 0; i < 5; ++i) *d[i] = 0;
    return status(scan_line(f, buf, "%i %Flf %Flf %Flf %Flf %Flf", a, d[0], d[1], d[2], d[3], d[4]));
}

// @src 140415d70
int read_i_d7(std::FILE* f, std::int32_t* a, double* const d[7])
{
    char buf[kLineSize];
    *a = 0;
    for (int i = 0; i < 7; ++i) *d[i] = 0;
    return status(scan_line(f, buf, "%i %Flf %Flf %Flf %Flf %Flf %Flf %Flf", a, d[0], d[1], d[2], d[3],
                            d[4], d[5], d[6]));
}

// @src 140415f00
int read_ii_d(std::FILE* f, std::int32_t* a, std::int32_t* b, double* c)
{
    char buf[kLineSize];
    *b = 0;
    *a = 0;
    *c = 0;
    return status(scan_line(f, buf, "%i %i %Flf", a, b, c));
}

// @src 140415ff0
int read_ii_dd(std::FILE* f, std::int32_t* a, std::int32_t* b, double* c, double* d)
{
    char buf[kLineSize];
    *b = 0;
    *a = 0;
    *d = 0;
    *c = 0;
    return status(scan_line(f, buf, "%i %i %Flf %Flf", a, b, c, d));
}

// @src 140416100
int read_ii_d3(std::FILE* f, std::int32_t* a, std::int32_t* b, double* c, double* d, double* e)
{
    char buf[kLineSize];
    *b = 0;
    *a = 0;
    *e = 0;
    *d = 0;
    *c = 0;
    return status(scan_line(f, buf, "%i %i %Flf %Flf %Flf", a, b, c, d, e));
}

// @src 140416220
int read_ii_d4(std::FILE* f, std::int32_t* a, std::int32_t* b, double* const d[4])
{
    char buf[kLineSize];
    *b = 0;
    *a = 0;
    for (int i = 0; i < 4; ++i) *d[i] = 0;
    return status(scan_line(f, buf, "%i %i %Flf %Flf %Flf %Flf", a, b, d[0], d[1], d[2], d[3]));
}

// @src 1404164e0
int read_iii_d(std::FILE* f, std::int32_t* a, std::int32_t* b, std::int32_t* c, double* d)
{
    char buf[kLineSize];
    *c = 0;
    *b = 0;
    *a = 0;
    *d = 0;
    return status(scan_line(f, buf, "%i %i %i %Flf", a, b, c, d));
}

// @src 1404165e0
int read_iii_dd(std::FILE* f, std::int32_t* a, std::int32_t* b, std::int32_t* c, double* d, double* e)
{
    char buf[kLineSize];
    *a = 0;
    *b = 0;
    *c = 0;
    *d = 0;
    *e = 0;
    return status(scan_line(f, buf, "%i %i %i %Flf %Flf", a, b, c, d, e));
}

// @src 140416700
int read_iii_d_i(std::FILE* f, std::int32_t* a, std::int32_t* b, std::int32_t* c, double* d,
                 std::int32_t* e)
{
    char buf[kLineSize];
    *c = 0;
    *b = 0;
    *a = 0;
    *d = 0;
    *e = 0;
    return status(scan_line(f, buf, "%i %i %i %Flf %i", a, b, c, d, e));
}

// @src 140416aa0
int read_d_i(std::FILE* f, double* a, std::int32_t* b)
{
    char buf[kLineSize];
    *b = 0;
    *a = 0;
    return status(scan_line(f, buf, "%Flf %i", a, b));
}

namespace {
// part of @src 140413ab0 / 140413c60: cut at '\n', strncpy n-1, trim trailing blanks
void copy_trimmed(char* dst, char* line, int n)
{
    char* nl = std::strchr(line, '\n');
    if (nl != nullptr) *nl = '\0';
    std::strncpy(dst, line, static_cast<std::size_t>(static_cast<std::int64_t>(n - 1)));
    int last = -1;
    for (int i = n - 2; -1 < i; --i) {
        if (dst[i] != '\0' && dst[i] != ' ' && dst[i] != '\n' && dst[i] != '\r') {
            last = i;
            break;
        }
    }
    dst[last + 1] = '\0';
}
} // namespace

// @src 140413ab0
int read_string_next_line(std::FILE* f, char* dst, int n)
{
    char buf[kLineSize];
    if (std::fgets(buf, kLineSize, f) == nullptr) return 1;
    if (std::fgets(buf, kLineSize, f) == nullptr) return 1;
    copy_trimmed(dst, buf, n);
    return 0;
}

// @src 140413c60
int read_string_line(std::FILE* f, char* dst, int n)
{
    char buf[kLineSize];
    if (std::fgets(buf, kLineSize, f) == nullptr) return 1;
    copy_trimmed(dst, buf, n);
    return 0;
}

// @src 140413900
int read_tokens(std::FILE* f, int* count, int max_tokens)
{
    if (std::fgets(g_token_line, kLineSize, f) == nullptr) return 1;
    int lim = max_tokens;
    if (0x7f < max_tokens) lim = 0x80;
    for (int i = 0; i < lim; ++i) g_tokens[i] = nullptr;
    *count = 0;
    g_tokens[0] = std::strtok(g_token_line, " ,\n\t\r");
    *count = *count + 1;
    for (int i = 1; i < lim && i < 0x80; ++i) {
        g_tokens[i] = std::strtok(nullptr, " ,\n\t\r");
        if (g_tokens[i] == nullptr) break;
        *count = *count + 1;
    }
    return 0;
}

// @src 140413a50  (both fallbacks are empty strings: str 14056548c, 140565554)
const char* token(int i)
{
    if (i < 0 || 0x7f < i) return "";
    if (g_tokens[i] == nullptr) return "";
    return g_tokens[i];
}

std::int32_t token_int(int i) { return static_cast<std::int32_t>(std::strtol(token(i), nullptr, 10)); }

double token_double(int i) { return std::atof(token(i)); }

} // namespace def::key
