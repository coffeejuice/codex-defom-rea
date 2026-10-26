// The part of the VS2008 CRT scanf that GENSUB's converter FUN_14037e5c0 uses (packet p67-5):
// _input_l for %d %s %f, and the float conversion behind scanf("%f") (_fassign_l flag 0 ->
// _atoflt_l -> __strgtold12_l / __mtold12 / __multtenpow12 / __ld12mul / _ld12tof).
//
// Why not UCRT sscanf / strtof: the old CRT converts the digit string to an 80-bit mantissa,
// multiplies by its own 80-bit powers of ten (rounded) and then rounds the 96-bit result to 24
// bits with exact ties going DOWN (_RoundMan rounds up only when a tail bit below the round bit is
// set). A correctly rounded strtof rounds ties to even, so "16777219" gives 16777218 here and
// 16777220 in the UCRT. Ported from the decompiles and listings of the binary's statically linked
// CRT (deform-rea-work/out/p675/gensub/input.c, fassign.c, crt2.lst); the power-of-ten tables
// are copied from 14063c0a0 / 14063c200 and the float format descriptor from 14063c080.
#include <defrt/stdlib/legacy_printf.hpp>
#include <defcmd/p67_5_gensub.hpp>

#include <cctype>
#include <cstring>
#include <stdexcept>
#include <string>

namespace def::cmd::gensub {

namespace {

// _LDBL12: bytes 0-1 low mantissa word ("XT"), 2-5 MANLO, 6-9 MANHI, 10-11 sign | exponent.
struct L12 {
    unsigned char b[12] = {};
    std::uint16_t u16(int o) const
    {
        std::uint16_t v;
        std::memcpy(&v, b + o, 2);
        return v;
    }
    std::uint32_t u32(int o) const
    {
        std::uint32_t v;
        std::memcpy(&v, b + o, 4);
        return v;
    }
    void set16(int o, std::uint16_t v) { std::memcpy(b + o, &v, 2); }
    void set32(int o, std::uint32_t v) { std::memcpy(b + o, &v, 4); }
};

// 14063c0a0 / 14063c200 (_pow10pos / _pow10neg): def::rt::crt::pow10_table (legacy_printf.cpp)

// @data 14063c080  float format descriptor of _ld12tof: max, min, precision, exponent width,
// format width, bias
constexpr int kFmtMax = 128, kFmtMin = -127, kFmtPrec = 24, kFmtExpWidth = 8, kFmtBias = 127;

// 96-bit helpers on (lo, mid, hi) = dwords at 0, 4, 8
void shl96(L12& x)
{
    const std::uint32_t lo = x.u32(0), mid = x.u32(4), hi = x.u32(8);
    x.set32(0, lo << 1);
    x.set32(4, (mid << 1) | (lo >> 31));
    x.set32(8, (hi << 1) | (mid >> 31));
}

void shr96(L12& x)
{
    const std::uint32_t lo = x.u32(0), mid = x.u32(4), hi = x.u32(8);
    x.set32(8, hi >> 1);
    x.set32(4, (mid >> 1) | (hi << 31));
    x.set32(0, (lo >> 1) | (mid << 31));
}

// @src 1404b0c14  __mtold12: digit values (one per byte) -> normalised 80-bit mantissa, exponent
void mtold12(const signed char* digits, int n, L12& x)
{
    std::memset(x.b, 0, 12);
    std::uint16_t expn = 0x404e;
    // 96-bit value as w[0] (low) .. w[2] (high); all arithmetic mod 2^96
    std::uint32_t w[3] = {0, 0, 0};
    auto add96 = [](std::uint32_t a[3], const std::uint32_t b[3]) {
        std::uint64_t c = 0;
        for (int k = 0; k < 3; ++k) {
            c += static_cast<std::uint64_t>(a[k]) + b[k];
            a[k] = static_cast<std::uint32_t>(c);
            c >>= 32;
        }
    };
    auto shl96n = [](std::uint32_t a[3], int s) {
        a[2] = (a[2] << s) | (a[1] >> (32 - s));
        a[1] = (a[1] << s) | (a[0] >> (32 - s));
        a[0] <<= s;
    };
    for (; n != 0; --n) {
        // x*4 + x, *2, + digit: 96-bit additions with the carries the listing propagates
        const std::uint32_t t[3] = {w[0], w[1], w[2]};
        shl96n(w, 2);
        add96(w, t);
        shl96n(w, 1);
        const std::uint32_t d[3] = {static_cast<std::uint32_t>(static_cast<std::int32_t>(*digits)), 0, 0};
        add96(w, d);
        ++digits;
    }
    x.set32(0, w[0]);
    x.set32(4, w[1]);
    x.set32(8, w[2]);
    while (x.u32(8) == 0) {
        const std::uint32_t lo = x.u32(0), mid = x.u32(4);
        x.set32(8, mid >> 16);
        x.set32(4, (mid << 16) | (lo >> 16));
        x.set32(0, lo << 16);
        expn = static_cast<std::uint16_t>(expn - 16);
    }
    while ((x.u32(8) & 0x8000) == 0) {
        shl96(x);
        expn = static_cast<std::uint16_t>(expn - 1);
    }
    x.set16(10, expn);
}

// part of @src 1404a1544  __ld12mul (inlined in __strgtold12_l): px *= py, 80-bit rounding
void ld12mul(L12& px, const L12& py)
{
    L12 t;
    std::uint16_t expx = px.u16(10), expy = py.u16(10);
    const std::uint16_t sign = static_cast<std::uint16_t>((expx ^ expy) & 0x8000);
    expx &= 0x7fff;
    expy &= 0x7fff;
    std::uint16_t expsum = static_cast<std::uint16_t>(expx + expy);
    if (expx >= 0x7fff || expy >= 0x7fff || expsum > 0xbffd) {
        px.set32(0, 0);
        px.set32(4, 0);
        px.set32(8, sign != 0 ? 0xffff8000u : 0x7fff8000u);
        return;
    }
    if (expsum <= 0x3fbf) {
        std::memset(px.b, 0, 12);
        return;
    }
    if (expx == 0) {
        expsum = static_cast<std::uint16_t>(expsum + 1);
        if ((px.u32(8) & 0x7fffffff) == 0 && px.u32(4) == 0 && px.u32(0) == 0) {
            px.set16(10, 0);  // a zero stays zero, with a positive sign
            return;
        }
    }
    if (expy == 0) {
        expsum = static_cast<std::uint16_t>(expsum + 1);
        if ((py.u32(8) & 0x7fffffff) == 0 && py.u32(4) == 0 && py.u32(0) == 0) {
            std::memset(px.b, 0, 12);
            return;
        }
    }
    int roffs = 0;
    for (int i = 0; i < 5; ++i) {
        int poffs = i << 1;
        int qoffs = 8;
        for (int j = 5 - i; j > 0; --j) {
            const std::uint32_t prod = static_cast<std::uint32_t>(px.u16(poffs)) * py.u16(qoffs);
            const std::uint32_t r = t.u32(roffs);
            const std::uint32_t s = r + prod;
            t.set32(roffs, s);
            if (s < r || s < prod) t.set16(roffs + 4, static_cast<std::uint16_t>(t.u16(roffs + 4) + 1));
            poffs += 2;
            qoffs -= 2;
        }
        roffs += 2;
    }
    expsum = static_cast<std::uint16_t>(expsum - 0x3ffe);
    while (static_cast<std::int16_t>(expsum) > 0 && (t.u32(8) & 0x80000000u) == 0) {
        shl96(t);
        expsum = static_cast<std::uint16_t>(expsum - 1);
    }
    if (static_cast<std::int16_t>(expsum) <= 0) {
        expsum = static_cast<std::uint16_t>(expsum - 1);
        int sticky = 0;
        while (static_cast<std::int16_t>(expsum) < 0) {
            if (t.u16(0) & 1) ++sticky;
            shr96(t);
            expsum = static_cast<std::uint16_t>(expsum + 1);
        }
        if (sticky != 0) t.set16(0, static_cast<std::uint16_t>(t.u16(0) | 1));
    }
    if (t.u16(0) > 0x8000 || (t.u32(0) & 0x1ffff) == 0x18000) {
        if (t.u32(2) == 0xffffffffu) {
            t.set32(2, 0);
            if (t.u32(6) == 0xffffffffu) {
                t.set32(6, 0);
                if (t.u16(10) == 0xffff) {
                    t.set16(10, 0x8000);
                    expsum = static_cast<std::uint16_t>(expsum + 1);
                } else {
                    t.set16(10, static_cast<std::uint16_t>(t.u16(10) + 1));
                }
            } else {
                t.set32(6, t.u32(6) + 1);
            }
        } else {
            t.set32(2, t.u32(2) + 1);
        }
    }
    if (expsum >= 0x7fff) {
        px.set32(0, 0);
        px.set32(4, 0);
        px.set32(8, sign != 0 ? 0xffff8000u : 0x7fff8000u);
        return;
    }
    std::memcpy(px.b, t.b + 2, 10);
    px.set16(10, static_cast<std::uint16_t>(expsum | sign));
}

// part of @src 1404a1544  __multtenpow12 (inlined): x *= 10^pow with the 80-bit tables
void multtenpow12(L12& x, int pow)
{
    if (pow == 0) return;
    int table = 0;
    if (pow < 0) {
        pow = -pow;
        table = 1;
    }
    x.set16(0, 0);  // mult12 == 0: the low mantissa word is cleared
    int group = -1;
    while (pow != 0) {
        ++group;
        const int last3 = pow & 7;
        pow >>= 3;
        if (last3 == 0) continue;
        L12 y;
        std::memcpy(y.b, def::rt::crt::pow10_table(table != 0) + (group * 7 + last3 - 1), 12);
        if (y.u16(0) >= 0x8000) y.set32(2, y.u32(2) - 1);  // "unround" the table entry
        ld12mul(x, y);
    }
}

bool dig19(char c) { return static_cast<unsigned char>(c - '1') < 9; }
bool is_exp_char(char c) { return c == 'D' || c == 'E' || c == 'd' || c == 'e'; }

// @src 1404a1544  __strgtold12_l(pld12, &end, str, mult12 = 0, scale = 0, decpt = 0,
// implicit_E = 0) as _atoflt_l calls it; locale decimal point '.'
void strgtold12(L12& out, const char* str)
{
    signed char man[26] = {};
    int manlen = 0, manexp = 0, expval = 0, expsign = 1;
    bool found_digit = false;
    std::uint16_t sign = 0;
    const char* p = str;
    while (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r') ++p;
    int state = 0;
    signed char* manp = man;
    auto store_digits_int = [&](char& c) {
        while (c > '/' && c < ':') {
            if (manlen < 25) {
                ++manlen;
                *manp++ = static_cast<signed char>(c - '0');
            } else {
                ++manexp;
            }
            c = *p++;
        }
    };
    for (bool done = false; !done;) {
        char c = *p++;
        switch (state) {
        case 0:
            if (dig19(c)) {
                state = 3;
                --p;
            } else if (c == '.') {
                state = 5;
            } else if (c == '+') {
                state = 2;
                sign = 0;
            } else if (c == '-') {
                state = 2;
                sign = 0x8000;
            } else if (c == '0') {
                state = 1;
            } else {
                done = true;
            }
            break;
        case 1:
            found_digit = true;
            if (dig19(c)) {
                state = 3;
                --p;
            } else if (c == '.') {
                state = 4;
            } else if (c == '+' || c == '-') {
                state = 11;
                --p;
            } else if (c == '0') {
                state = 1;
            } else if (is_exp_char(c)) {
                state = 6;
            } else {
                done = true;
            }
            break;
        case 2:
            if (dig19(c)) {
                state = 3;
                --p;
            } else if (c == '.') {
                state = 5;
            } else if (c == '0') {
                state = 1;
            } else {
                done = true;
            }
            break;
        case 3:
        case 4:
            if (state == 3) {
                store_digits_int(c);
                if (c == '.') {
                    found_digit = true;
                    state = 4;
                    break;
                }
            } else {
                if (manlen == 0)
                    while (c == '0') {
                        --manexp;
                        c = *p++;
                    }
                while (c > '/' && c < ':') {
                    if (manlen < 25) {
                        ++manlen;
                        *manp++ = static_cast<signed char>(c - '0');
                        --manexp;
                    }
                    c = *p++;
                }
            }
            found_digit = true;
            if (c == '+' || c == '-') {
                state = 11;
                --p;
            } else if (is_exp_char(c)) {
                state = 6;
            } else {
                done = true;
            }
            break;
        case 5:
            if (static_cast<unsigned char>(c - '0') > 9) {
                done = true;
            } else {
                state = 4;
                --p;
            }
            break;
        case 6:
            if (dig19(c)) {
                state = 9;
                --p;
            } else if (c == '+') {
                state = 7;
            } else if (c == '-') {
                expsign = -1;
                state = 7;
            } else if (c == '0') {
                state = 8;
            } else {
                done = true;
            }
            break;
        case 7:
            if (dig19(c)) {
                state = 9;
                --p;
            } else if (c == '0') {
                state = 8;
            } else {
                done = true;
            }
            break;
        case 8:
            while (c == '0') c = *p++;
            if (dig19(c)) {
                state = 9;
                --p;
            } else {
                done = true;
            }
            break;
        case 9:
            while (c >= '0' && c <= '9') {
                expval = c - '0' + expval * 10;
                if (expval >= 0x1451) {
                    expval = 0x1451;
                    while (c > '/' && c < ':') c = *p++;
                    break;
                }
                c = *p++;
            }
            done = true;
            break;
        default:  // 11: a sign after the mantissa, implicit_E == 0
            done = true;
            break;
        }
    }
    std::memset(out.b, 0, 12);
    if (found_digit) {
        if (manlen > 24) {
            // 1404a1983: the 24th digit (index 23) is tested and incremented
            if (man[23] >= 5) man[23] = static_cast<signed char>(man[23] + 1);
            --manp;
            manlen = 24;
            ++manexp;
        }
        if (manlen != 0) {
            while (*--manp == 0) {
                --manlen;
                ++manexp;
            }
            mtold12(man, manlen, out);
            if (expsign < 0) expval = -expval;
            expval += manexp;
            if (expval > 0x1450) {
                std::memset(out.b, 0, 12);
                out.set32(6, 0x80000000u);
                out.set16(10, 0x7fff);
            } else if (expval < -0x1450) {
                std::memset(out.b, 0, 12);
            } else {
                multtenpow12(out, expval);
            }
        }
    }
    out.set16(10, static_cast<std::uint16_t>(out.u16(10) | sign));
}

void shr_man(std::uint32_t man[3], int n)
{
    const int words = n / 32;
    const int bits = n % 32;
    std::uint32_t carry = 0;
    for (int k = 0; k < 3; ++k) {
        const std::uint32_t w = man[k];
        man[k] = (w >> bits) | carry;
        carry = bits == 0 ? 0 : (w & ~(0xffffffffu << bits)) << (32 - bits);
    }
    for (int l = 2; l >= 0; --l) man[l] = l < words ? 0 : man[l - words];
}

// part of @src 1404b04c0  _RoundMan: round at bit `prec` (ties down), returns the carry out
int round_man(std::uint32_t man[3], int prec)
{
    const int idx = prec / 32;
    const int bit = 31 - prec % 32;
    int carry = 0;
    if ((man[idx] >> bit) & 1) {
        bool up = (man[idx] & ~(0xffffffffu << bit)) != 0;
        for (int k = idx + 1; !up && k < 3; ++k)
            if (man[k] != 0) up = true;
        if (up) {
            const int i2 = (prec - 1) / 32;
            const std::uint32_t add = 1u << (31 - (prec - 1) % 32);
            const std::uint32_t s = man[i2] + add;
            carry = (s < man[i2] || s < add) ? 1 : 0;
            man[i2] = s;
            for (int l = i2 - 1; l >= 0 && carry != 0; --l) {
                const std::uint32_t s2 = man[l] + 1;
                carry = (s2 < man[l] || s2 == 0) ? 1 : 0;
                man[l] = s2;
            }
        }
    }
    man[idx] &= 0xffffffffu << bit;
    for (int k = idx + 1; k < 3; ++k) man[k] = 0;
    return carry;
}

// @src 1404b04c0  _ld12tof (_ld12cvt with the float descriptor 14063c080)
float ld12tof(const L12& x)
{
    const std::uint16_t e16 = x.u16(10);
    const int uexp = e16 & 0x7fff;
    const int exp = uexp - 0x3fff;
    std::uint32_t man[3] = {x.u32(6), x.u32(2), static_cast<std::uint32_t>(x.u16(0)) << 16};
    std::uint32_t bexp = 0;
    if (exp == -0x3fff) {
        if (man[0] != 0 || man[1] != 0 || man[2] != 0) {
            man[0] = 0;
            man[1] = 0;
        }
    } else {
        const std::uint32_t saved[3] = {man[0], man[1], man[2]};
        const int carry = round_man(man, kFmtPrec);
        const int e = carry != 0 ? uexp - 0x3ffe : exp;
        if (e < kFmtMin - kFmtPrec) {
            man[0] = 0;
            man[1] = 0;
        } else if (kFmtMin < e) {
            if (e < kFmtMax) {
                man[0] &= 0x7fffffffu;
                bexp = static_cast<std::uint32_t>(kFmtBias + e);
                shr_man(man, kFmtExpWidth);
            } else {
                man[0] = 0x80000000u;
                man[1] = 0;
                man[2] = 0;
                shr_man(man, kFmtExpWidth);
                bexp = static_cast<std::uint32_t>(kFmtBias + kFmtMax);
            }
        } else {
            man[0] = saved[0];
            man[1] = saved[1];
            man[2] = saved[2];
            shr_man(man, kFmtMin - exp);
            round_man(man, kFmtPrec);
            shr_man(man, kFmtExpWidth + 1);
        }
    }
    const std::uint32_t bits = (bexp << (31 - kFmtExpWidth)) | ((e16 & 0x8000) != 0 ? 0x80000000u : 0) | man[0];
    float f;
    std::memcpy(&f, &bits, 4);
    return f;
}

// ---- _input_l ------------------------------------------------------------------------------------

struct Source {
    std::FILE* f = nullptr;
    const char* s = nullptr;
    std::size_t pos = 0, len = 0;
    int get()
    {
        if (f != nullptr) return std::getc(f);
        if (pos >= len) {
            ++pos;
            return EOF;
        }
        return static_cast<unsigned char>(s[pos++]);
    }
    void unget(int c)
    {
        if (f != nullptr)
            std::ungetc(c, f);
        else
            --pos;
    }
};

bool sp(int c) { return std::isspace(c) != 0; }
bool dg(int c) { return c >= '0' && c <= '9'; }

// @src 140493dec  _input_l (the conversions the converter's formats use; a width, %c, %[, %n,
// %i/%o/%x/%u/%p or an I64/ll size throws std::logic_error)
int input(Source& in, const char* format, void* const* args)
{
    const unsigned char* p = reinterpret_cast<const unsigned char*>(format);
    int count = 0;          // fields assigned
    signed char match = 0;  // directives completed
    int c = 0;              // the last character read (uVar20)
    int ai = 0;
    while (*p != 0) {
        if (sp(*p)) {
            for (;;) {
                const int ch = in.get();
                if (ch == EOF) break;
                if (!sp(ch & 0xff)) {
                    in.unget(ch);
                    break;
                }
            }
            do ++p;
            while (sp(*p));
            continue;
        }
        if (*p == '%' && p[1] != '%') {
            bool suppress = false;
            int longone = 1;
            const unsigned char* q = p + 1;
            for (;; ++q) {
                if (*q == '*') {
                    suppress = true;
                } else if (*q == 'F' || *q == 'N') {
                } else if (*q == 'l') {
                    ++longone;
                } else if (*q == 'h') {
                    --longone;
                } else if (dg(*q)) {
                    throw std::logic_error("gensub scan: width not ported");
                } else {
                    break;
                }
            }
            const int conv = *q | 0x20;
            void* target = suppress ? nullptr : args[ai++];
            // leading white space
            int ch;
            do {
                ch = in.get();
                if (ch == EOF) break;
            } while (sp(ch & 0xff));
            if (ch == EOF) {
                c = EOF;
                goto end;
            }
            if (conv == 'd') {
                bool neg = false;
                if (ch == '-') {
                    neg = true;
                    ch = in.get();
                } else if (ch == '+') {
                    ch = in.get();
                }
                std::uint32_t num = 0;
                int ndig = 0;
                while (dg(ch & 0xff) && ch != EOF) {
                    num = num * 10 + static_cast<std::uint32_t>(ch - '0');
                    ++ndig;
                    ch = in.get();
                }
                if (ch != EOF) in.unget(ch);
                if (neg) num = 0u - num;
                c = ch;
                if (ndig == 0) goto end;
                if (!suppress) {
                    ++count;
                    if (longone == 0)
                        *static_cast<std::uint16_t*>(target) = static_cast<std::uint16_t>(num);
                    else
                        *static_cast<std::uint32_t*>(target) = num;
                }
            } else if (conv == 's') {
                in.unget(ch);
                char* dst = static_cast<char*>(target);
                std::size_t n = 0;
                for (;;) {
                    ch = in.get();
                    if (ch == EOF) break;
                    if ((ch > 8 && ch < 0xe) || ch == ' ') {
                        in.unget(ch);
                        break;
                    }
                    if (!suppress) dst[n] = static_cast<char>(ch);
                    ++n;
                }
                c = ch;
                if (n == 0) goto end;
                if (!suppress) {
                    ++count;
                    dst[n] = 0;
                }
            } else if (conv == 'e' || conv == 'f' || conv == 'g') {
                std::string buf;
                if (ch == '-') {
                    buf += '-';
                    ch = in.get();
                } else if (ch == '+') {
                    ch = in.get();
                }
                int ndig = 0;
                while (ch != EOF && dg(ch & 0xff)) {
                    ++ndig;
                    buf += static_cast<char>(ch);
                    ch = in.get();
                }
                if (static_cast<char>(ch) == '.') {
                    ch = in.get();
                    buf += '.';
                    while (ch != EOF && dg(ch & 0xff)) {
                        ++ndig;
                        buf += static_cast<char>(ch);
                        ch = in.get();
                    }
                }
                int total = ndig;
                if (ndig != 0 && (ch == 'e' || ch == 'E')) {
                    buf += 'e';
                    ch = in.get();
                    if (ch == '-') {
                        buf += '-';
                        ch = in.get();
                    } else if (ch == '+') {
                        ch = in.get();
                    }
                    while (ch != EOF && dg(ch & 0xff)) {
                        ++total;
                        buf += static_cast<char>(ch);
                        ch = in.get();
                    }
                }
                if (ch != EOF) in.unget(ch);
                c = ch;
                if (total == 0) goto end;
                if (!suppress) {
                    ++count;
                    // 14048df54 _fassign_l: flag 0 -> float, else double
                    if (longone - 1 == 0)
                        *static_cast<float*>(target) = crt_atoflt(buf.c_str());
                    else
                        throw std::logic_error("gensub scan: %lf not ported");
                }
            } else {
                throw std::logic_error("gensub scan: conversion not ported");
            }
            ++match;
            p = q + 1;
            if (c == EOF) goto end;
            continue;
        }
        // a literal character (or "%%")
        {
            const unsigned char want = *p;
            p += (*p == '%') ? 2 : 1;
            const int ch = in.get();
            c = ch;
            if (ch != want) {
                if (ch != EOF) in.unget(ch);
                goto end;
            }
            if (c == EOF) goto end;
        }
    }
end:
    if (c == EOF) return (count != 0 || match != 0) ? count : -1;
    return count;
}

} // namespace

// @src 1404ab960  _atoflt_l (the _ld12tof variant _fassign_l calls for flag 0)
// @src 14048df54  _fassign_l (flag 0 part)
float crt_atoflt(const char* text)
{
    L12 x;
    strgtold12(x, text);
    return ld12tof(x);
}

// @src 140484924  fscanf (vfscanf 1404847b8 -> _input_l)
int crt_fscanf(std::FILE* f, const char* fmt, void* const* args)
{
    Source s;
    s.f = f;
    return input(s, fmt, args);
}

// @src 14047c354  sscanf (vscan_fn 14047c2b0 -> _input_l on the string)
int crt_sscanf(const char* str, const char* fmt, void* const* args)
{
    Source s;
    s.s = str;
    s.len = std::strlen(str);
    return input(s, fmt, args);
}

} // namespace def::cmd::gensub
