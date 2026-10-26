// Qt 4.8.4 qdtoa (qlocale_tools.cpp), modes 2 and 3: the digit generator behind
// QTextStream << double and QString::number(double) (owner: P04).
//
// Ported from a copy of QtCore4_64.dll 4.8.4 (VAs in that DLL). It is the old BSD version of
// David Gay's dtoa with IEEE_Arith, no ROUND_BIASED, no Honor/Check_FLT_ROUNDS, denormals
// supported, and one Qt change: an empty digit string becomes "0" with decpt 1.
//
// The three digit paths are kept as in the binary, because they do not always give the same
// string: the floating-point "quick" path and the big-integer path strip trailing zeros, while
// the "small integer" path keeps them when it rounds down (1000005.0 -> "100000", decpt 7, which
// doubleToString prints as 1.00000e+06). The big-integer arithmetic is our own (exact), so only
// the ratio b/S matters, not dtoa's internal scaling shifts.
#include <cstdint>
#include <cstring>
#include <vector>

#include "m2m/common.hpp"
#include "m2m/qt4num.hpp"

namespace m2m::qt {
namespace {

constexpr int kBias = 1023;
constexpr int kP = 53;
constexpr int kTenPmax = 22;
constexpr int kQuickMax = 14;
constexpr int kIntMax = 14;
constexpr int kBletch = 0x10;
constexpr int kNBigtens = 5;

// tens / bigtens at 0x671f2150 / 0x671f2208 in the DLL copy.
constexpr double kTens[] = {1e0,  1e1,  1e2,  1e3,  1e4,  1e5,  1e6,  1e7,  1e8,  1e9,  1e10, 1e11,
                            1e12, 1e13, 1e14, 1e15, 1e16, 1e17, 1e18, 1e19, 1e20, 1e21, 1e22};
constexpr double kBigtens[] = {1e16, 1e32, 1e64, 1e128, 1e256};

std::uint64_t bits(double d)
{
    std::uint64_t u;
    std::memcpy(&u, &d, sizeof u);
    return u;
}

double from_bits(std::uint64_t u)
{
    double d;
    std::memcpy(&d, &u, sizeof d);
    return d;
}

std::uint32_t word0(double d) { return static_cast<std::uint32_t>(bits(d) >> 32); }
std::uint32_t word1(double d) { return static_cast<std::uint32_t>(bits(d)); }
double set_word0(double d, std::uint32_t w0)
{
    return from_bits((static_cast<std::uint64_t>(w0) << 32) | word1(d));
}

// Unsigned big integer, 32-bit limbs, little endian, no leading zero limbs.
struct Big {
    std::vector<std::uint32_t> x;

    explicit Big(std::uint64_t v = 0)
    {
        while (v) {
            x.push_back(static_cast<std::uint32_t>(v));
            v >>= 32;
        }
    }
    bool zero() const { return x.empty(); }
    void trim()
    {
        while (!x.empty() && x.back() == 0) x.pop_back();
    }
    void muladd(std::uint32_t m, std::uint32_t a)
    {
        std::uint64_t carry = a;
        for (auto& w : x) {
            std::uint64_t t = static_cast<std::uint64_t>(w) * m + carry;
            w = static_cast<std::uint32_t>(t);
            carry = t >> 32;
        }
        if (carry) x.push_back(static_cast<std::uint32_t>(carry));
        trim();
    }
    void pow5(int n)
    {
        for (; n >= 13; n -= 13) muladd(1220703125u, 0);   // 5^13
        static constexpr std::uint32_t p5[] = {1, 5, 25, 125, 625, 3125, 15625, 78125, 390625,
                                               1953125, 9765625, 48828125, 244140625};
        if (n > 0) muladd(p5[n], 0);
    }
    void shl(int n)
    {
        if (zero() || n <= 0) return;
        int words = n / 32, b = n % 32;
        std::vector<std::uint32_t> r(static_cast<size_t>(words), 0);
        std::uint32_t carry = 0;
        for (auto w : x) {
            r.push_back(b ? (w << b) | carry : w);
            carry = b ? w >> (32 - b) : 0;
        }
        if (carry) r.push_back(carry);
        x.swap(r);
    }
    static int cmp(const Big& a, const Big& b)
    {
        if (a.x.size() != b.x.size()) return a.x.size() < b.x.size() ? -1 : 1;
        for (size_t i = a.x.size(); i-- > 0;)
            if (a.x[i] != b.x[i]) return a.x[i] < b.x[i] ? -1 : 1;
        return 0;
    }
    void sub(const Big& o)   // *this >= o
    {
        std::int64_t borrow = 0;
        for (size_t i = 0; i < x.size(); ++i) {
            std::int64_t t = static_cast<std::int64_t>(x[i]) - borrow - (i < o.x.size() ? o.x[i] : 0);
            borrow = t < 0;
            x[i] = static_cast<std::uint32_t>(t + (borrow << 32));
        }
        trim();
    }
};

// quorem(b, S): the next digit; dtoa guarantees b < 10*S here.
int quorem(Big& b, const Big& S)
{
    int q = 0;
    while (Big::cmp(b, S) >= 0) {
        b.sub(S);
        ++q;
    }
    return q;
}

// The string under construction, as dtoa's s0/s pointers.
struct Out {
    std::string s;
    int k = 0;
};

// bump_up / roundoff of the quick and small-integer paths: drop trailing '9's, add one.
void bump_up(Out& o)
{
    while (!o.s.empty() && o.s.back() == '9') {
        if (o.s.size() == 1) {
            ++o.k;
            o.s.back() = '0';
            break;
        }
        o.s.pop_back();
    }
    ++o.s.back();
}

void strip_zeros(Out& o)
{
    while (!o.s.empty() && o.s.back() == '0') o.s.pop_back();
}

}  // namespace

// @src 0x67057450 qdtoa / 0x67055fd0 _qdtoa
Digits qdtoa(double din, int mode, int ndigits)
{
    Digits r;
    double d = din;
    if (word0(d) & 0x80000000u) {
        r.sign = true;
        d = set_word0(d, word0(d) & 0x7fffffffu);
    }
    if ((word0(d) & 0x7ff00000u) == 0x7ff00000u) {
        r.decpt = 9999;
        r.digits = (!word1(d) && !(word0(d) & 0xfffff)) ? "Infinity" : "NaN";
        return r;
    }
    if (d == 0.0) {
        r.decpt = 1;
        r.digits = "0";
        return r;
    }
    if (mode < 0 || mode > 9) mode = 0;
    if (mode != 2 && mode != 3) M2M_STUB(0x67055fd0, "qdtoa mode other than 2/3");

    // d2b: mantissa without trailing zero bits, be = exponent of its lowest bit, bbits = bit count.
    const std::uint64_t u = bits(d);
    const int de = static_cast<int>((u >> 52) & 0x7ff);
    std::uint64_t mant = u & ((1ull << 52) - 1);
    if (de) mant |= 1ull << 52;
    int lo0 = 0;
    while (!((mant >> lo0) & 1)) ++lo0;
    int be, bbits;
    if (de) {
        be = de - kBias - (kP - 1) + lo0;
        bbits = kP - lo0;
    } else {
        be = de - kBias - (kP - 1) + 1 + lo0;
        std::uint64_t m = mant >> lo0;
        bbits = 0;
        while (m) {
            ++bbits;
            m >>= 1;
        }
    }

    int i;
    double d2;
    if (de) {
        d2 = set_word0(d, (word0(d) & 0xfffffu) | 0x3ff00000u);
        i = de - kBias;
    } else {
        i = bbits + be + (kBias + (kP - 1) - 1);
        std::uint32_t x = i > 32 ? (word0(d) << (64 - i)) | (word1(d) >> (i - 32))
                                 : word1(d) << (32 - i);
        d2 = static_cast<double>(x);
        d2 = set_word0(d2, word0(d2) - 31u * 0x100000u);
        i -= (kBias + (kP - 1) - 1) + 1;
    }
    double ds = (d2 - 1.5) * 0.289529654602168 + 0.1760912590558 + i * 0.301029995663981;
    int k = static_cast<int>(ds);
    if (ds < 0. && ds != k) k--;
    bool k_check = true;
    if (k >= 0 && k <= kTenPmax) {
        if (d < kTens[k]) k--;
        k_check = false;
    }
    int j = bbits - i - 1;
    int b2, s2, b5, s5;
    if (j >= 0) {
        b2 = 0;
        s2 = j;
    } else {
        b2 = -j;
        s2 = 0;
    }
    if (k >= 0) {
        b5 = 0;
        s5 = k;
        s2 += k;
    } else {
        b2 -= k;
        b5 = -k;
        s5 = 0;
    }

    int ilim, ilim1;
    if (mode == 2) {
        if (ndigits <= 0) ndigits = 1;
        ilim = ilim1 = ndigits;
    } else {
        ilim = ndigits + k + 1;
        ilim1 = ilim - 1;
    }

    Out o;
    o.k = k;
    auto finish = [&]() {
        if (o.s.empty()) {   // Qt: an empty result becomes "0"
            o.s = "0";
            o.k = 0;
        }
        r.digits = o.s;
        r.decpt = o.k + 1;
        return r;
    };

    // ---- quick path: floating-point digits (try_quick; mode 2/3 never set leftright) ----
    if (ilim >= 0 && ilim <= kQuickMax) {
        int ii = 0;
        double dq = d;
        int kq = k;
        int ilimq = ilim;
        int ieps = 2;
        bool failed = false;
        if (kq > 0) {
            double dsq = kTens[kq & 0xf];
            int jj = kq >> 4;
            if (jj & kBletch) {
                jj &= kBletch - 1;
                dq /= kBigtens[kNBigtens - 1];
                ieps++;
            }
            for (; jj; jj >>= 1, ii++)
                if (jj & 1) {
                    ieps++;
                    dsq *= kBigtens[ii];
                }
            dq /= dsq;
        } else if (int j1 = -kq) {
            dq *= kTens[j1 & 0xf];
            for (int jj = j1 >> 4; jj; jj >>= 1, ii++)
                if (jj & 1) {
                    ieps++;
                    dq *= kBigtens[ii];
                }
        }
        if (k_check && dq < 1. && ilimq > 0) {
            if (ilim1 <= 0) {
                failed = true;
            } else {
                ilimq = ilim1;
                kq--;
                dq *= 10.;
                ieps++;
            }
        }
        if (!failed) {
            double eps = ieps * dq + 7.;
            eps = set_word0(eps, word0(eps) - (kP - 1) * 0x100000u);
            if (ilimq == 0) {
                dq -= 5.;
                if (dq > eps) {   // one_digit
                    o.s = "1";
                    o.k = kq + 1;
                    return finish();
                }
                if (dq < -eps) {   // no_digits
                    o.k = -1 - ndigits;
                    return finish();
                }
            } else {
                // Generate ilim digits, then fix them up.
                Out q;
                q.k = kq;
                for (int n = 1;; n++, dq *= 10.) {
                    int L = static_cast<int>(dq);
                    dq -= L;
                    q.s += static_cast<char>('0' + L);
                    if (n == ilimq) {
                        const double e = eps * kTens[ilimq - 1];
                        if (dq > 0.5 + e) {
                            bump_up(q);
                            o = q;
                            return finish();
                        }
                        if (dq < 0.5 - e) {
                            strip_zeros(q);
                            o = q;
                            return finish();
                        }
                        break;   // fast_failed
                    }
                }
            }
        }
        // fast_failed: s, d, k and ilim are as before the quick path
    }

    // ---- small integer ----
    if (be >= 0 && k <= kIntMax) {
        double dd = d;
        const double dsi = kTens[k];
        if (ndigits < 0 && ilim <= 0) {
            if (ilim < 0 || dd <= 5 * dsi) {
                o.k = -1 - ndigits;
                return finish();
            }
            o.s = "1";
            o.k = k + 1;
            return finish();
        }
        for (int n = 1;; n++, dd *= 10.) {
            int L = static_cast<int>(dd / dsi);
            dd -= L * dsi;
            o.s += static_cast<char>('0' + L);
            if (!dd) break;
            if (n == ilim) {
                dd += dd;
                if (dd > dsi || (dd == dsi && (L & 1))) bump_up(o);
                break;
            }
        }
        return finish();
    }

    // ---- big integers: b / S = d / 10^k exactly ----
    Big b(mant >> lo0);
    b.pow5(b5);
    b.shl(b2);
    Big S(1);
    S.pow5(s5);
    S.shl(s2);
    if (k_check && Big::cmp(b, S) < 0) {
        o.k = --k;
        b.muladd(10, 0);
        ilim = ilim1;
    }
    if (ilim <= 0 && mode > 2) {
        Big S5 = S;
        S5.muladd(5, 0);
        if (ilim < 0 || Big::cmp(b, S5) <= 0) {   // no_digits
            o.k = -1 - ndigits;
            return finish();
        }
        o.s = "1";   // one_digit
        o.k = k + 1;
        return finish();
    }
    int dig = 0;
    for (int n = 1;; n++) {
        dig = quorem(b, S);
        o.s += static_cast<char>('0' + dig);
        if (n >= ilim) break;
        b.muladd(10, 0);
    }
    // Round off last digit
    b.shl(1);
    const int c = Big::cmp(b, S);
    if (c > 0 || (c == 0 && (dig & 1))) {
        while (o.s.back() == '9') {
            if (o.s.size() == 1) {
                ++o.k;
                o.s = "1";
                return finish();
            }
            o.s.pop_back();
        }
        ++o.s.back();
    } else {
        strip_zeros(o);
    }
    return finish();
}

}  // namespace m2m::qt
