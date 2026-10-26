// The VS2008 CRT's floating-point formatting (_fltout2, $I10_OUTPUT with the inlined
// __multtenpow12 / ld12mul, _fptostr, _cftoe/_cftof/_cftog, _cropzeros, _forcdecpt) as compiled
// into DEF_PRE_64, and a printf front end that uses it. Packet p1-01.
//
// The Ghidra decompile of $I10_OUTPUT merges the 96-bit temporaries badly; the listing
// (1404b2074 normalise loop, 1404b20ca sticky shift, 1404b2121 rounding test) shows full 96-bit
// shifts, which is what this port does.
#include <defrt/stdlib/legacy_printf.hpp>

#include <cctype>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <string>

namespace def::rt::crt {

namespace {

// @data 14063c0a0  _pow10pos (29 _LDBL12 entries: 1e1..1e7, 1e8..1e56, 1e64..1e448, ...)
const Ldbl12 kPow10Pos[29] = {
    {{0x0000, 0x0000, 0x0000, 0x0000, 0xa000, 0x4002}},
    {{0x0000, 0x0000, 0x0000, 0x0000, 0xc800, 0x4005}},
    {{0x0000, 0x0000, 0x0000, 0x0000, 0xfa00, 0x4008}},
    {{0x0000, 0x0000, 0x0000, 0x0000, 0x9c40, 0x400c}},
    {{0x0000, 0x0000, 0x0000, 0x0000, 0xc350, 0x400f}},
    {{0x0000, 0x0000, 0x0000, 0x0000, 0xf424, 0x4012}},
    {{0x0000, 0x0000, 0x0000, 0x8000, 0x9896, 0x4016}},
    {{0x0000, 0x0000, 0x0000, 0x2000, 0xbebc, 0x4019}},
    {{0x0000, 0x0000, 0x0400, 0xc9bf, 0x8e1b, 0x4034}},
    {{0x0000, 0xa100, 0xcced, 0x1bce, 0xd3c2, 0x404e}},
    {{0xf020, 0xb59e, 0x2b70, 0xada8, 0x9dc5, 0x4069}},
    {{0x5dd0, 0x25fd, 0x1ae5, 0x4f8e, 0xeb19, 0x4083}},
    {{0x9671, 0x95d7, 0x0e43, 0x8d05, 0xaf29, 0x409e}},
    {{0xbff9, 0x44a0, 0x81ed, 0x8f12, 0x8281, 0x40b9}},
    {{0x3cbf, 0xa6d5, 0xffcf, 0x1f49, 0xc278, 0x40d3}},
    {{0xc66f, 0x8ce0, 0x80e9, 0x47c9, 0x93ba, 0x41a8}},
    {{0x85bc, 0x556b, 0x3927, 0xf78d, 0xe070, 0x427c}},
    {{0xddbc, 0xde8e, 0x9df9, 0xebfb, 0xaa7e, 0x4351}},
    {{0xe6a1, 0xe376, 0xf2cc, 0x2f29, 0x8184, 0x4426}},
    {{0x1028, 0xaa17, 0xaef8, 0xe310, 0xc4c5, 0x44fa}},
    {{0xa7eb, 0xf3d4, 0xebf7, 0x4ae1, 0x957a, 0x45cf}},
    {{0xcc65, 0x91c7, 0xa60e, 0xa0ae, 0xe319, 0x46a3}},
    {{0x650d, 0x0c17, 0x8175, 0x7586, 0xc976, 0x4d48}},
    {{0x4258, 0xa7e4, 0x3993, 0x353b, 0xb2b8, 0x53ed}},
    {{0xa74d, 0x5de5, 0xc53d, 0x3b5d, 0x9e8b, 0x5a92}},
    {{0x5dff, 0xf0a6, 0x20a1, 0x54c0, 0x8ca5, 0x6137}},
    {{0xfdd1, 0x5a8b, 0xd88b, 0x5d25, 0xf989, 0x67db}},
    {{0x95aa, 0xf3f8, 0xbf27, 0xc8a2, 0xdd5d, 0x6e80}},
    {{0xc94c, 0x979b, 0x8a20, 0x5202, 0xc460, 0x7525}},
};

// @data 14063c200  _pow10neg (29 _LDBL12 entries: 1e-1..1e-7, 1e-8..1e-56, ...)
const Ldbl12 kPow10Neg[29] = {
    {{0xcccd, 0xcccd, 0xcccc, 0xcccc, 0xcccc, 0x3ffb}},
    {{0x3d71, 0xd70a, 0x70a3, 0x0a3d, 0xa3d7, 0x3ff8}},
    {{0x645a, 0xdf3b, 0x8d4f, 0x6e97, 0x8312, 0x3ff5}},
    {{0xd3c3, 0x652c, 0xe219, 0x1758, 0xd1b7, 0x3ff1}},
    {{0x0fd0, 0x8423, 0x1b47, 0xac47, 0xa7c5, 0x3fee}},
    {{0xa640, 0x69b6, 0xaf6c, 0xbd05, 0x8637, 0x3feb}},
    {{0x3d33, 0x42bc, 0xe57a, 0x94d5, 0xd6bf, 0x3fe7}},
    {{0xfdc2, 0xcefd, 0x8461, 0x7711, 0xabcc, 0x3fe4}},
    {{0x4c2f, 0xe15b, 0xc44d, 0x94be, 0xe695, 0x3fc9}},
    {{0xc492, 0x3b53, 0x4475, 0x14cd, 0x9abe, 0x3faf}},
    {{0x67de, 0x94ba, 0x4539, 0x1ead, 0xcfb1, 0x3f94}},
    {{0x2324, 0xe2c6, 0xbabc, 0x313b, 0x8b61, 0x3f7a}},
    {{0x5561, 0xc159, 0xb17e, 0x7c53, 0xbb12, 0x3f5f}},
    {{0xeed7, 0x8d2f, 0xbe06, 0x8592, 0xfb15, 0x3f44}},
    {{0x3f24, 0xe9a5, 0xa539, 0xea27, 0xa87f, 0x3f2a}},
    {{0xac7d, 0xe4a1, 0x64bc, 0x467c, 0xddd0, 0x3e55}},
    {{0x7b63, 0xcc06, 0x5423, 0x8377, 0x91ff, 0x3d81}},
    {{0xfa91, 0x193a, 0x637a, 0x4325, 0xc031, 0x3cac}},
    {{0x8921, 0x38d1, 0x4782, 0xb897, 0xfd00, 0x3bd7}},
    {{0x88dc, 0x0858, 0xb11b, 0xe3e8, 0xa686, 0x3b03}},
    {{0x84c6, 0x4245, 0xb607, 0x7599, 0xdb37, 0x3a2e}},
    {{0x7133, 0xd21c, 0xdb23, 0xee32, 0x9049, 0x395a}},
    {{0x87a6, 0xc0be, 0xda57, 0x82a5, 0xa2a6, 0x32b5}},
    {{0x68e2, 0x11b2, 0x52a7, 0x449f, 0xb759, 0x2c10}},
    {{0x4925, 0x2de4, 0x3436, 0x534f, 0xceae, 0x256b}},
    {{0x598f, 0xa404, 0xdec0, 0x7dc2, 0xe8fb, 0x1ec6}},
    {{0xe79e, 0x5a88, 0x9157, 0xbf3c, 0x8350, 0x1822}},
    {{0x4b4e, 0x6265, 0x83fd, 0xaf8f, 0x9406, 0x117d}},
    {{0x2de4, 0x9fde, 0xd2ce, 0x04c8, 0xa6dd, 0x0ad8}},
};
} // namespace

const Ldbl12* pow10_table(bool neg) { return neg ? kPow10Neg : kPow10Pos; }

namespace {

// 0.1 as _LDBL12, built on the stack by $I10_OUTPUT (1404b1c9a: cccccccc cccccccc 3ffbcccc)
const Ldbl12 kOneTenth = {{0xcccc, 0xcccc, 0xcccc, 0xcccc, 0xcccc, 0x3ffb}};

std::uint32_t lo32(const Ldbl12& x) { return x.w[0] | (std::uint32_t{x.w[1]} << 16); }
std::uint32_t med32(const Ldbl12& x) { return x.w[2] | (std::uint32_t{x.w[3]} << 16); }
std::uint32_t hi32(const Ldbl12& x) { return x.w[4] | (std::uint32_t{x.w[5]} << 16); }
std::uint32_t manlo(const Ldbl12& x) { return x.w[1] | (std::uint32_t{x.w[2]} << 16); }
std::uint32_t manhi(const Ldbl12& x) { return x.w[3] | (std::uint32_t{x.w[4]} << 16); }

void set_lo32(Ldbl12& x, std::uint32_t v)
{
    x.w[0] = static_cast<std::uint16_t>(v);
    x.w[1] = static_cast<std::uint16_t>(v >> 16);
}
void set_med32(Ldbl12& x, std::uint32_t v)
{
    x.w[2] = static_cast<std::uint16_t>(v);
    x.w[3] = static_cast<std::uint16_t>(v >> 16);
}
void set_hi32(Ldbl12& x, std::uint32_t v)
{
    x.w[4] = static_cast<std::uint16_t>(v);
    x.w[5] = static_cast<std::uint16_t>(v >> 16);
}
void set_manlo(Ldbl12& x, std::uint32_t v)
{
    x.w[1] = static_cast<std::uint16_t>(v);
    x.w[2] = static_cast<std::uint16_t>(v >> 16);
}
void set_manhi(Ldbl12& x, std::uint32_t v)
{
    x.w[3] = static_cast<std::uint16_t>(v);
    x.w[4] = static_cast<std::uint16_t>(v >> 16);
}

// shl_12 / shr_12 / add_12: over the whole 96 bits
void shl_12(Ldbl12& x)
{
    const std::uint32_t lo = lo32(x), med = med32(x), hi = hi32(x);
    set_hi32(x, (hi << 1) | (med >> 31));
    set_med32(x, (med << 1) | (lo >> 31));
    set_lo32(x, lo << 1);
}

void shr_12(Ldbl12& x)
{
    const std::uint32_t lo = lo32(x), med = med32(x), hi = hi32(x);
    set_lo32(x, (lo >> 1) | (med << 31));
    set_med32(x, (med >> 1) | (hi << 31));
    set_hi32(x, hi >> 1);
}

void add_12(Ldbl12& x, const Ldbl12& y)
{
    std::uint64_t s = std::uint64_t{lo32(x)} + lo32(y);
    set_lo32(x, static_cast<std::uint32_t>(s));
    s = (s >> 32) + med32(x) + med32(y);
    set_med32(x, static_cast<std::uint32_t>(s));
    s = (s >> 32) + hi32(x) + hi32(y);
    set_hi32(x, static_cast<std::uint32_t>(s));
}

bool is_zero_12(const Ldbl12& x)
{
    return (hi32(x) & 0x7fffffffu) == 0 && med32(x) == 0 && lo32(x) == 0;
}

void put_inf_12(Ldbl12& x, std::uint16_t sign)
{
    set_lo32(x, 0);
    set_med32(x, 0);
    set_hi32(x, 0x7fff8000u | (sign ? 0x80000000u : 0u));
}

void put_zero_12(Ldbl12& x)
{
    set_lo32(x, 0);
    set_med32(x, 0);
    set_hi32(x, 0);
}

constexpr std::uint16_t kLdMaxExp = 0x7fff;
constexpr std::uint16_t kLdBiasM1 = 0x3ffe;

void zero_result(Fos* fos, bool negative)
{
    fos->exp = 0;
    fos->len = 1;
    fos->sign = negative ? '-' : ' ';
    fos->man[0] = '0';
    fos->man[1] = '\0';
}

} // namespace

// @src 1404abb94  __dtold: double -> 80-bit, in _LDBL12 words 1..5
Ldbl12 dtold(double x)
{
    std::uint64_t bits;
    std::memcpy(&bits, &x, 8);
    const std::uint32_t dlo = static_cast<std::uint32_t>(bits);
    const std::uint32_t dhi = static_cast<std::uint32_t>(bits >> 32);
    const std::uint16_t exp = static_cast<std::uint16_t>((dhi >> 20) & 0x7ff);
    const std::uint16_t sign = static_cast<std::uint16_t>((dhi >> 16) & 0x8000);
    const std::uint32_t mhi = dhi & 0xfffff;
    std::uint32_t msb = 0x80000000u;
    std::uint16_t ldexp;
    Ldbl12 r = {{0, 0, 0, 0, 0, 0}};
    if (exp == 0x7ff) {
        ldexp = kLdMaxExp;
    } else if (exp == 0) {
        if (mhi == 0 && dlo == 0) {
            r.w[5] = sign; // 1404abc5a: a zero keeps only its sign
            return r;
        }
        ldexp = static_cast<std::uint16_t>(0 - 0x3ff + 0x3fff + 1); // denormal
        msb = 0;
    } else {
        ldexp = static_cast<std::uint16_t>(static_cast<int>(exp) - 0x3ff + 0x3fff);
    }
    std::uint32_t hi = (mhi << 11) | (dlo >> 21) | msb;
    std::uint32_t lo = dlo << 11;
    if (msb == 0) {
        while ((hi & 0x80000000u) == 0) {
            hi = (hi << 1) | (lo >> 31);
            lo <<= 1;
            --ldexp;
        }
    }
    set_manlo(r, lo);
    set_manhi(r, hi);
    r.w[5] = static_cast<std::uint16_t>(sign | ldexp);
    return r;
}

// @src 1404b1c60  ld12mul, inlined twice into $I10_OUTPUT (1404b1f18 and 1404b2270)
void ld12mul(Ldbl12* px, const Ldbl12* py)
{
    std::uint16_t expx = px->w[5];
    std::uint16_t expy = py->w[5];
    const std::uint16_t sign = (expx ^ expy) & 0x8000;
    expx &= 0x7fff;
    expy &= 0x7fff;
    std::uint16_t expsum = static_cast<std::uint16_t>(expx + expy);
    if (expx >= kLdMaxExp || expy >= kLdMaxExp || expsum > kLdMaxExp + kLdBiasM1) {
        put_inf_12(*px, sign);
        return;
    }
    if (expsum <= kLdBiasM1 - 63) {
        put_zero_12(*px);
        return;
    }
    if (expx == 0) {
        ++expsum;
        if (is_zero_12(*px)) {
            px->w[5] = 0;
            return;
        }
    }
    if (expy == 0) {
        ++expsum;
        if (is_zero_12(*py)) {
            put_zero_12(*px);
            return;
        }
    }
    // tempman: the top 96 bits of the 160-bit product of the two 80-bit mantissas (words 0..4);
    // partial products below word 4 of the product are not formed, as in the CRT
    std::uint16_t t[6] = {0, 0, 0, 0, 0, 0};
    for (int i = 0; i < 5; ++i) {
        int p = i;
        int q = 4;
        for (int j = 5 - i; j > 0; --j) {
            const std::uint32_t prod = std::uint32_t{px->w[p]} * py->w[q];
            const std::uint32_t r0 = t[i] | (std::uint32_t{t[i + 1]} << 16);
            const std::uint32_t sum = r0 + prod;
            t[i] = static_cast<std::uint16_t>(sum);
            t[i + 1] = static_cast<std::uint16_t>(sum >> 16);
            if (sum < r0 || sum < prod) {
                ++t[i + 2]; // 16-bit increment, no further carry
            }
            ++p;
            --q;
        }
    }
    Ldbl12 tm = {{t[0], t[1], t[2], t[3], t[4], t[5]}};
    expsum = static_cast<std::uint16_t>(expsum - kLdBiasM1);
    while (static_cast<std::int16_t>(expsum) > 0 && (hi32(tm) & 0x80000000u) == 0) {
        shl_12(tm);
        --expsum;
    }
    if (static_cast<std::int16_t>(expsum) <= 0) {
        --expsum;
        int sticky = 0;
        while (static_cast<std::int16_t>(expsum) < 0) {
            if (tm.w[0] & 1) {
                ++sticky;
            }
            shr_12(tm);
            ++expsum;
        }
        if (sticky) {
            tm.w[0] |= 1;
        }
    }
    if (tm.w[0] > 0x8000 || (lo32(tm) & 0x1ffff) == 0x18000) {
        if (manlo(tm) == 0xffffffffu) {
            set_manlo(tm, 0);
            if (manhi(tm) == 0xffffffffu) {
                set_manhi(tm, 0);
                if (tm.w[5] == 0xffff) {
                    tm.w[5] = 0x8000;
                    ++expsum;
                } else {
                    ++tm.w[5];
                }
            } else {
                set_manhi(tm, manhi(tm) + 1);
            }
        } else {
            set_manlo(tm, manlo(tm) + 1);
        }
    }
    if (expsum >= 0x7fff) {
        put_inf_12(*px, sign);
        return;
    }
    px->w[0] = tm.w[1];
    set_manlo(*px, med32(tm));
    set_manhi(*px, hi32(tm));
    px->w[5] = static_cast<std::uint16_t>(expsum | sign);
}

// @src 1404b1c60  __multtenpow12, inlined into $I10_OUTPUT (1404b1e7c..1404b224e)
void multtenpow12(Ldbl12* pld12, int pow, bool mult12)
{
    if (pow == 0) {
        return;
    }
    const Ldbl12* table = kPow10Pos;
    if (pow < 0) {
        pow = -pow;
        table = kPow10Neg;
    }
    if (!mult12) {
        pld12->w[0] = 0;
    }
    int group = -8; // _pow10pos - 8 entries, + 7 entries per group of three exponent bits
    while (pow) {
        group += 7;
        const int last3 = pow & 7;
        pow >>= 3;
        if (last3 == 0) {
            continue;
        }
        Ldbl12 unround = table[group + last3];
        if (unround.w[0] >= 0x8000) {
            set_manlo(unround, manlo(unround) - 1); // the stored entry was rounded up
        }
        ld12mul(pld12, &unround);
    }
}

// @src 1404b1c60  $I10_OUTPUT: ndigits significant digits of an 80-bit value
int i10_output(const Ldbl12& ld, int ndigits, unsigned flags, Fos* fos)
{
    std::uint16_t expn = ld.w[5];
    const std::uint32_t mhi = manhi(ld);
    const std::uint32_t mlo = manlo(ld);
    const bool negative = (expn & 0x8000) != 0;
    expn &= 0x7fff;
    fos->sign = negative ? '-' : ' ';
    if (expn == 0 && mhi == 0 && mlo == 0) {
        zero_result(fos, negative);
        return 1;
    }
    if (expn == 0x7fff) {
        fos->exp = 1;
        const char* text;
        if ((mhi == 0x80000000u && mlo == 0) || ((mhi >> 30) & 1)) {
            if (negative && mhi == 0xc0000000u && mlo == 0) {
                text = "1#IND";
            } else if (mhi == 0x80000000u && mlo == 0) {
                text = "1#INF";
            } else {
                text = "1#QNAN";
            }
        } else {
            text = "1#SNAN";
        }
        std::strcpy(fos->man, text);
        fos->len = static_cast<std::uint8_t>(std::strlen(text));
        return 0;
    }
    // 1404b1e4d: estimate of the decimal exponent
    const std::uint32_t est = ((std::uint32_t{expn} >> 8) + (mhi >> 24) * 2) * 0x4d +
                              static_cast<std::uint32_t>(-0x134312f4) +
                              std::uint32_t{expn} * 0x4d10;
    std::int16_t r = static_cast<std::int16_t>(est >> 16);

    Ldbl12 ld12 = {{0, 0, 0, 0, 0, expn}};
    set_manlo(ld12, mlo);
    set_manhi(ld12, mhi);
    multtenpow12(&ld12, -r, true);
    if (ld12.w[5] >= 0x3fff) {
        ++r;
        ld12mul(&ld12, &kOneTenth);
    }
    fos->exp = r;

    int nd = ndigits;
    if (flags & 1) { // SO_FFORMAT
        nd += r;
        if (nd <= 0) {
            zero_result(fos, negative);
            return 1;
        }
    }
    if (nd > 21) {
        nd = 21;
    }
    // to fixed point with the binary point above bit 88
    const int ib = static_cast<int>(ld12.w[5]) - 0x3ffe;
    ld12.w[5] = 0;
    for (int i = 0; i < 8; ++i) {
        shl_12(ld12);
    }
    if (ib < 0) {
        int n = -ib & 0xff;
        while (n > 0) {
            shr_12(ld12);
            --n;
        }
    }
    char* p = fos->man;
    for (int i = nd + 1; i > 0; --i) {
        const Ldbl12 tmp = ld12;
        shl_12(ld12);
        shl_12(ld12);
        add_12(ld12, tmp);
        shl_12(ld12); // * 10
        *p++ = static_cast<char>((ld12.w[5] >> 8) + '0');
        ld12.w[5] &= 0x00ff;
    }
    char* q = p - 2; // the last of the nd digits; p[-1] decides the rounding
    if (p[-1] > '4') {
        while (q >= fos->man && *q == '9') {
            *q = '0';
            --q;
        }
        if (q < fos->man) {
            ++q;
            ++fos->exp;
        }
        ++*q;
    } else {
        while (q >= fos->man && *q == '0') {
            --q;
        }
        if (q < fos->man) {
            zero_result(fos, negative);
            return 1;
        }
    }
    fos->len = static_cast<std::uint8_t>(q - fos->man + 1);
    fos->man[fos->len] = '\0';
    return 1;
}

// @src 1404abc6c  _fltout2: 17 significant digits of a double
StrFlt* fltout2(double x, StrFlt* flt, char* result, std::size_t size)
{
    Fos fos;
    const Ldbl12 ld = dtold(x);
    flt->flag = i10_output(ld, 17, 0, &fos);
    flt->sign = fos.sign;
    flt->decpt = fos.exp;
    std::strncpy(result, fos.man, size); // strcpy_s(result, size, fos.man)
    result[size - 1] = '\0';
    flt->mantissa = result;
    return flt;
}

// @src 1404abab8  _fptostr: round the mantissa string to `digits` digits, half up
int fptostr(char* buf, std::size_t size, int digits, StrFlt* flt)
{
    (void)size; // the callers' buffers are large enough (the CRT's EINVAL/ERANGE checks)
    const char* mantissa = flt->mantissa;
    buf[0] = '0';
    char* p = buf;
    for (;;) {
        ++p;
        if (digits < 1) {
            break;
        }
        char c;
        if (*mantissa == '\0') {
            c = '0';
        } else {
            c = *mantissa++;
        }
        *p = c;
        --digits;
    }
    *p = '\0';
    if (digits >= 0 && *mantissa > '4') {
        while (*--p == '9') {
            *p = '0';
        }
        ++*p;
    }
    if (buf[0] == '1') {
        ++flt->decpt;
    } else {
        std::memmove(buf, buf + 1, std::strlen(buf + 1) + 1);
    }
    return 0;
}

namespace {

constexpr char kDecimalPoint = '.'; // *(localeinfo->decimal_point) in the "C" locale

void shift_right(char* s, std::size_t n)
{
    std::memmove(s + n, s, std::strlen(s) + 1);
}

// @src 14048dfe4  _cftoe2_l: lay out d.ddde+xxx from the rounded digits
void cftoe2(char* buf, int ndec, bool caps, const StrFlt* pflt, bool g_fmt)
{
    if (g_fmt) {
        char* p = buf + (pflt->sign == '-');
        if (ndec > 0) {
            shift_right(p, 1);
        }
    }
    char* p = buf;
    if (pflt->sign == '-') {
        *p++ = '-';
    }
    if (ndec > 0) {
        *p = p[1];
        ++p;
        *p = kDecimalPoint;
    }
    p = p + (g_fmt ? 0 : 1) + ndec;
    std::strcpy(p, "e+000");
    if (caps) {
        *p = 'E';
    }
    if (*pflt->mantissa != '0') {
        int exp = pflt->decpt - 1;
        if (exp < 0) {
            exp = -exp;
            p[1] = '-';
        }
        if (exp >= 100) {
            p[2] = static_cast<char>(p[2] + exp / 100);
            exp %= 100;
        }
        if (exp >= 10) {
            p[3] = static_cast<char>(p[3] + exp / 10);
            exp %= 10;
        }
        p[4] = static_cast<char>(p[4] + exp);
    }
    // _outputformat & _TWO_DIGIT_EXPONENT (DAT_14072f414) is never set by DEF_PRE
}

// @src 14048e7ac  _cftof2_l: insert the decimal point and the leading zeros
void cftof2(char* buf, int ndec, const StrFlt* pflt, bool g_fmt)
{
    const int magnitude = pflt->decpt - 1;
    if (g_fmt && magnitude == ndec) {
        char* q = buf + magnitude + (pflt->sign == '-');
        q[0] = '0';
        q[1] = '\0';
    }
    if (pflt->sign == '-') {
        *buf++ = '-';
    }
    if (pflt->decpt <= 0) {
        shift_right(buf, 1);
        *buf++ = '0';
    } else {
        buf += pflt->decpt;
    }
    if (ndec > 0) {
        shift_right(buf, 1);
        *buf++ = kDecimalPoint;
        if (pflt->decpt < 0) {
            int n = -pflt->decpt;
            if (!g_fmt && ndec < n) {
                n = ndec;
            }
            if (n != 0) {
                shift_right(buf, static_cast<std::size_t>(n));
            }
            std::memset(buf, '0', static_cast<std::size_t>(n));
        }
    }
}

// @src 14048e23c  _cftoe_l
void cftoe(double v, char* buf, int ndec, bool caps)
{
    StrFlt flt;
    char mant[24];
    fltout2(v, &flt, mant, sizeof mant);
    fptostr(buf + (ndec > 0) + (flt.sign == '-'), 0, ndec + 1, &flt);
    cftoe2(buf, ndec, caps, &flt, false);
}

// @src 14048e970  _cftof_l
void cftof(double v, char* buf, int ndec)
{
    StrFlt flt;
    char mant[24];
    fltout2(v, &flt, mant, sizeof mant);
    fptostr(buf + (flt.sign == '-'), 0, flt.decpt + ndec, &flt);
    cftof2(buf, ndec, &flt, false);
}

// @src 14048ea6c  _cftog_l
void cftog(double v, char* buf, int ndec, bool caps)
{
    StrFlt flt;
    char mant[24];
    fltout2(v, &flt, mant, sizeof mant);
    const int magnitude = flt.decpt - 1;
    char* digits = buf + (flt.sign == '-');
    fptostr(digits, 0, ndec, &flt);
    const int g_magnitude = flt.decpt - 1; // 14048eb17: a temporary, decpt itself is kept
    if (g_magnitude < -4 || g_magnitude >= ndec) {
        cftoe2(buf, ndec, caps, &flt, true);
    } else {
        if (magnitude < g_magnitude) {
            // 14048eb30: rounding added a digit, drop the last one
            char* p = digits;
            while (*p++) {
            }
            p[-2] = '\0';
        }
        cftof2(buf, ndec, &flt, true);
    }
}

// @src 14048de9c  _cropzeros_l: remove trailing zeros (and a bare point) of a %g result
void cropzeros(char* buf)
{
    while (*buf && *buf != kDecimalPoint) {
        ++buf;
    }
    if (*buf) {
        char* p1;
        char c;
        do {
            ++buf;
            c = *buf;
            p1 = buf;
        } while (c != '\0' && c != 'e' && c != 'E');
        char* p2;
        do {
            p2 = p1;
            p1 = p2 - 1;
        } while (*p1 == '0');
        if (*p1 == kDecimalPoint) {
            p1 = p2 - 2;
        }
        do {
            c = *buf;
            ++p1;
            ++buf;
            *p1 = c;
        } while (c != '\0');
    }
}

// @src 14048de1c  _forcdecpt_l: "%#.0e/f/g" keeps a decimal point
void forcdecpt(char* buf)
{
    if (std::tolower(static_cast<unsigned char>(*buf)) != 'e') {
        do {
            ++buf;
        } while (std::isdigit(static_cast<unsigned char>(*buf)));
    }
    if (std::tolower(static_cast<unsigned char>(*buf)) == 'x') {
        buf += 2;
    }
    char hold = *buf;
    *buf++ = kDecimalPoint;
    char next;
    do {
        next = *buf;
        *buf = hold;
        hold = next;
    } while (*buf++);
}

[[noreturn]] void unsupported(const char* what)
{
    std::fprintf(stderr, "def::rt::crt printf: unsupported conversion %s\n", what);
    std::abort();
}

} // namespace

// part of @src 14048ccb4  (_output_l, the e/E/f/g/G case at 14048d05f; _cfltcvt_l 14048ebcc)
std::string format_double(double value, char conv, int precision, bool alternate)
{
    bool caps = false;
    char fmt = conv;
    if (fmt == 'E' || fmt == 'G') {
        caps = true;
        fmt = static_cast<char>(fmt + 0x20);
    }
    if (precision < 0) {
        precision = 6;
    } else if (precision == 0) {
        if (fmt == 'g') {
            precision = 1;
        }
    } else if (precision > 0x200) {
        precision = 0x200;
    }
    if (!std::isfinite(value)) {
        // the UCRT in legacy mode prints the old "1.#INF00" texts (stdio_compat.hpp)
        char spec[16];
        std::snprintf(spec, sizeof spec, alternate ? "%%#.%d%c" : "%%.%d%c", precision, conv);
        char out[64];
        std::snprintf(out, sizeof out, spec, value);
        return out;
    }
    std::string buf(static_cast<std::size_t>(precision) + 0x15d + 32, '\0');
    char* b = buf.data();
    if (fmt == 'e') {
        cftoe(value, b, precision, caps);
    } else if (fmt == 'f') {
        cftof(value, b, precision);
    } else {
        cftog(value, b, precision, caps);
    }
    if (alternate && precision == 0) {
        forcdecpt(b);
    }
    if (fmt == 'g' && !alternate) {
        cropzeros(b);
    }
    buf.resize(std::strlen(b));
    return buf;
}

// @src 14048ccb4  _output_l: format parsing, flags, width and sign padding of e/E/f/g/G
// (other conversions are handed to the UCRT one at a time)
std::string vformat(const char* fmt, std::va_list ap)
{
    std::string out;
    const char* s = fmt;
    while (*s) {
        if (*s != '%') {
            out.push_back(*s++);
            continue;
        }
        ++s;
        if (*s == '%') {
            out.push_back('%');
            ++s;
            continue;
        }
        bool left = false, plus = false, space = false, alt = false, zero = false;
        for (;; ++s) {
            if (*s == '-') {
                left = true;
            } else if (*s == '+') {
                plus = true;
            } else if (*s == ' ') {
                space = true;
            } else if (*s == '#') {
                alt = true;
            } else if (*s == '0') {
                zero = true;
            } else {
                break;
            }
        }
        int width = 0;
        if (*s == '*') {
            width = va_arg(ap, int);
            if (width < 0) {
                left = true;
                width = -width;
            }
            ++s;
        } else {
            while (*s >= '0' && *s <= '9') {
                width = width * 10 + (*s++ - '0');
            }
        }
        int precision = -1;
        if (*s == '.') {
            ++s;
            precision = 0;
            if (*s == '*') {
                precision = va_arg(ap, int);
                if (precision < 0) {
                    precision = -1;
                }
                ++s;
            } else {
                while (*s >= '0' && *s <= '9') {
                    precision = precision * 10 + (*s++ - '0');
                }
            }
        }
        enum Size { kInt, kLong, kLongLong, kShort, kChar, kPtrSize };
        Size size = kInt;
        const char* size_start = s;
        for (;;) {
            if (*s == 'h') {
                size = size == kShort ? kChar : kShort;
                ++s;
            } else if (*s == 'l') {
                size = size == kLong ? kLongLong : kLong;
                ++s;
            } else if (*s == 'L' || *s == 'w') {
                ++s;
            } else if (s[0] == 'I' && s[1] == '6' && s[2] == '4') {
                size = kLongLong;
                s += 3;
            } else if (s[0] == 'I' && s[1] == '3' && s[2] == '2') {
                size = kInt;
                s += 3;
            } else if (*s == 'I') {
                size = kPtrSize;
                ++s;
            } else {
                break;
            }
        }
        const std::string size_text(size_start, s);
        const char conv = *s;
        if (conv == '\0') {
            break; // an unfinished conversion at the end prints nothing
        }
        ++s;
        if (conv == 'e' || conv == 'E' || conv == 'f' || conv == 'g' || conv == 'G') {
            const double v = va_arg(ap, double);
            std::string text = format_double(v, conv, precision, alt);
            std::string prefix;
            if (!text.empty() && text[0] == '-') {
                prefix = "-";
                text.erase(0, 1);
            } else if (plus) {
                prefix = "+";
            } else if (space) {
                prefix = " ";
            }
            const int pad = width - static_cast<int>(text.size() + prefix.size());
            if (!left && !zero && pad > 0) {
                out.append(static_cast<std::size_t>(pad), ' ');
            }
            out += prefix;
            if (zero && !left && pad > 0) {
                out.append(static_cast<std::size_t>(pad), '0');
            }
            out += text;
            if (left && pad > 0) {
                out.append(static_cast<std::size_t>(pad), ' ');
            }
            continue;
        }
        // every other conversion: the UCRT, one conversion at a time
        std::string spec = "%";
        if (left) spec += '-';
        if (plus) spec += '+';
        if (space) spec += ' ';
        if (alt) spec += '#';
        if (zero) spec += '0';
        if (width) spec += std::to_string(width);
        if (precision >= 0) spec += "." + std::to_string(precision);
        spec += size_text;
        spec += conv;
        std::string piece;
        const auto put = [&piece, &spec](auto value) {
            const int len = std::snprintf(nullptr, 0, spec.c_str(), value);
            if (len > 0) {
                piece.assign(static_cast<std::size_t>(len) + 1, '\0');
                std::snprintf(piece.data(), piece.size(), spec.c_str(), value);
                piece.resize(static_cast<std::size_t>(len));
            }
        };
        switch (conv) {
        case 'd':
        case 'i':
        case 'o':
        case 'u':
        case 'x':
        case 'X':
            if (size == kLongLong || size == kPtrSize) {
                put(va_arg(ap, long long));
            } else if (size == kLong) {
                put(va_arg(ap, long));
            } else {
                put(va_arg(ap, int));
            }
            break;
        case 'c':
        case 'C':
            put(va_arg(ap, int));
            break;
        case 's':
        case 'S':
        case 'p':
        case 'Z':
            put(va_arg(ap, void*));
            break;
        case 'a':
        case 'A':
            put(va_arg(ap, double));
            break;
        case 'n':
            unsupported("%n");
        default:
            // a character that is no conversion: the state table (__lookuptable 14056fb30, used
            // at 14048ce9d) goes back to the normal state, which writes that character alone;
            // the '%' and what was parsed after it are dropped, no argument is consumed
            // (beyond a '*' width/precision already read). "%10d %\n" prints "<n> \n".
            piece.assign(1, conv);
            break;
        }
        out += piece;
    }
    return out;
}

std::string format(const char* fmt, ...)
{
    std::va_list ap;
    va_start(ap, fmt);
    std::string s = vformat(fmt, ap);
    va_end(ap);
    return s;
}

int vsnprintf(char* buf, std::size_t n, const char* fmt, std::va_list ap)
{
    const std::string s = vformat(fmt, ap);
    if (n > 0) {
        const std::size_t k = s.size() < n - 1 ? s.size() : n - 1;
        std::memcpy(buf, s.data(), k);
        buf[k] = '\0';
    }
    return static_cast<int>(s.size());
}

int snprintf(char* buf, std::size_t n, const char* fmt, ...)
{
    std::va_list ap;
    va_start(ap, fmt);
    const int r = crt::vsnprintf(buf, n, fmt, ap);
    va_end(ap);
    return r;
}

int sprintf(char* buf, const char* fmt, ...)
{
    std::va_list ap;
    va_start(ap, fmt);
    const std::string s = vformat(fmt, ap);
    va_end(ap);
    std::memcpy(buf, s.c_str(), s.size() + 1);
    return static_cast<int>(s.size());
}

int vfprintf(std::FILE* f, const char* fmt, std::va_list ap)
{
    const std::string s = vformat(fmt, ap);
    return static_cast<int>(std::fwrite(s.data(), 1, s.size(), f));
}

int fprintf(std::FILE* f, const char* fmt, ...)
{
    std::va_list ap;
    va_start(ap, fmt);
    const int r = crt::vfprintf(f, fmt, ap);
    va_end(ap);
    return r;
}

int printf(const char* fmt, ...)
{
    std::va_list ap;
    va_start(ap, fmt);
    const int r = crt::vfprintf(stdout, fmt, ap);
    va_end(ap);
    return r;
}

} // namespace def::rt::crt
