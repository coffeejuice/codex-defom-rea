#include "legacy_motion_math.hpp"

#include <bit>
#include <cmath>
#include <cstdint>
#include <stdexcept>

namespace deform::recovered {
namespace {
// Generated from retained read_bytes at14063bb40. Indices0..18 are unreachable
// in4a0430's finite large-argument call domain and deliberately contain zero.
constexpr std::array<std::uint64_t, 139> two_over_pi = {
    0ULL, 0ULL, 0ULL, 0ULL, 0ULL, 0ULL, 0ULL, 0ULL,
    0ULL, 0ULL, 0ULL, 0ULL, 0ULL, 0ULL, 0ULL, 0ULL,
    0ULL, 0ULL, 0ULL, 0ULL, 0ULL, 0ULL, 0ULL, 0ULL,
    162ULL, 998ULL, 54ULL, 915ULL, 580ULL, 84ULL, 671ULL, 777ULL,
    855ULL, 839ULL, 851ULL, 311ULL, 448ULL, 877ULL, 553ULL, 358ULL,
    316ULL, 270ULL, 260ULL, 127ULL, 593ULL, 398ULL, 701ULL, 942ULL,
    965ULL, 390ULL, 882ULL, 283ULL, 570ULL, 265ULL, 221ULL, 184ULL,
    6ULL, 292ULL, 750ULL, 642ULL, 465ULL, 584ULL, 463ULL, 903ULL,
    491ULL, 114ULL, 786ULL, 617ULL, 830ULL, 930ULL, 35ULL, 381ULL,
    302ULL, 749ULL, 72ULL, 314ULL, 412ULL, 448ULL, 619ULL, 279ULL,
    894ULL, 260ULL, 921ULL, 117ULL, 569ULL, 525ULL, 307ULL, 637ULL,
    156ULL, 529ULL, 504ULL, 751ULL, 505ULL, 160ULL, 945ULL, 1022ULL,
    151ULL, 1023ULL, 480ULL, 358ULL, 15ULL, 956ULL, 753ULL, 98ULL,
    858ULL, 41ULL, 721ULL, 987ULL, 310ULL, 507ULL, 242ULL, 498ULL,
    777ULL, 733ULL, 244ULL, 399ULL, 870ULL, 633ULL, 510ULL, 651ULL,
    373ULL, 158ULL, 940ULL, 506ULL, 997ULL, 965ULL, 947ULL, 833ULL,
    825ULL, 990ULL, 165ULL, 164ULL, 746ULL, 431ULL, 949ULL, 1004ULL,
    287ULL, 565ULL, 464ULL,
};

struct Reduced { double head, tail; std::uint32_t quadrant; std::uint64_t initial_exponent; };

Reduced reduce_large(double x) {
    const auto bits = std::bit_cast<std::uint64_t>(x);
    const auto mantissa = (bits & 0xfffffffffffffULL) | 0x10000000000000ULL;
    const int exponent = static_cast<int>((bits >> 52) & 0x7ff) - 1023;
    const int remainder = exponent % 10;
    const int index = exponent / 10 + 18;
    if (index < 19 || index + 18 >= static_cast<int>(two_over_pi.size()))
        throw std::out_of_range("legacy motion argument-reduction table index");
    std::array<std::uint64_t, 20> limbs{};
    std::uint64_t carry = 0;
    for (int i = 18; i >= 0; --i) {
        const auto product = two_over_pi[static_cast<std::size_t>(index + i)] * mantissa + carry;
        limbs[static_cast<std::size_t>(i)] = product & 0x3ff;
        carry = product >> 10;
    }
    const auto window = ((limbs[0] << 10) | limbs[1]) >> (9 - remainder);
    const bool complement = (window & 1) != 0;
    const auto quadrant = static_cast<std::uint32_t>(((window & 7) >> 1) +
        static_cast<unsigned>(complement)) & 3;
    const auto mask = (std::uint64_t{1} << (10 - remainder)) - 1;
    std::uint64_t head = mask & (complement ? ~limbs[1] : limbs[1]);
    int consumed = 1;
    while (head < 0x20000000000000ULL) {
        if (consumed + 1 >= static_cast<int>(limbs.size()))
            throw std::out_of_range("legacy motion reduction exhausted retained limbs");
        ++consumed;
        const auto limb = limbs[static_cast<std::size_t>(consumed)];
        head = (head << 10) | (complement ? (~limb & 0x3ff) : limb);
    }
    if (consumed + 1 >= static_cast<int>(limbs.size()))
        throw std::out_of_range("legacy motion reduction missing trailing limb");
    auto next = (complement ? ~limbs[static_cast<std::size_t>(consumed + 1)]
                            : limbs[static_cast<std::size_t>(consumed + 1)]) << 54;
    int scale = remainder - consumed * 10 + 52;
    while (head > 0x1fffffffffffffULL) {
        ++scale;
        next = (head << 63) | (next >> 1);
        head >>= 1;
    }
    auto high_bits = (static_cast<std::uint64_t>(scale + 1023) << 52) |
                     (head & 0xfffffffffffffULL);
    if (complement) high_bits |= 0x8000000000000000ULL;
    const double high = std::bit_cast<double>(high_bits);
    const auto low_bits = static_cast<std::uint64_t>(scale + 970) << 52;
    const double base = std::bit_cast<double>(low_bits);
    double low = std::bit_cast<double>((next >> 12) | low_bits) + (0.0 - base);
    low = low * (complement ? -2.0 : 2.0);
    const double truncated = std::bit_cast<double>(high_bits & 0xfffffffff8000000ULL);
    const double product = high * 1.5707963267948966;
    low = (truncated * 1.5707963109016418 - product) +
          (high - truncated) * 1.5707963109016418 +
          truncated * 1.5893254712295857e-08 +
          (high - truncated) * 1.5893254712295857e-08 +
          high * 6.123233995736765e-17 + low * 1.5707963267948966;
    const double result = low + product;
    return {result, (product - result) + low, quadrant, 0x7ff};
}

Reduced reduce(double x) {
    if (x >= 500000.0) return reduce_large(x);
    const auto q = static_cast<std::uint32_t>(x * 0.6366197723675814 + 0.5);
    const double count = static_cast<double>(q);
    const double first = x - count * 1.5707963267341256;
    double tail = count * 6.077100506506192e-11;
    double head = first - tail;
    const auto initial_exponent = std::bit_cast<std::uint64_t>(std::abs(head)) >> 52;
    double work = first;
    if ((std::bit_cast<std::uint64_t>(x) >> 52) - initial_exponent > 15) {
        work = first - count * 6.077100506303966e-11;
        tail = count * 2.0222662487959506e-21 -
               ((first - work) - count * 6.077100506303966e-11);
        head = work - tail;
    }
    tail = (work - head) - tail;
    return {head, tail, q, initial_exponent};
}

double sine_polynomial(double z) {
    return ((z * 1.5918144304485914e-10 + -2.5051132068021698e-08) * z +
             2.7557316103728802e-06) * z * z * z +
           (z * -0.00019841269836761127 + 0.00833333333333095) * z +
           -0.16666666666666666;
}

double reduced_sine(double head, double tail) {
    const double z = head * head;
    return tail + (head * z * sine_polynomial(z) - z * 0.5 * tail) + head;
}

double reduced_cosine(double head, double tail) {
    const double z = head * head;
    return (((z * 2.4801587298767044e-05 + -0.0013888888888887398) * z +
             0.041666666666666664 +
             ((z * -1.138263981623609e-11 + 2.0876146382372144e-09) * z +
              -2.755731727234489e-07) * z * z * z) * z * z +
            ((((z * 0.5 - 1.0) + 1.0) - z * 0.5) - tail * head)) -
            (z * 0.5 - 1.0);
}
} // namespace

std::array<double, 2> legacy_motion_cos_sin(double x) {
    if (!std::isfinite(x)) throw std::invalid_argument("nonfinite legacy motion angle");
    const double absolute = std::abs(x);
    const auto bits = std::bit_cast<std::uint64_t>(absolute);
    if (bits < 0x3fe921fb54442d19ULL) {
        if (bits < 0x3e40000000000000ULL) return {1.0, x};
        if (bits <= 0x3f1fffffffffffffULL)
            return {1.0 - x * x * 0.5, x - x * x * x * 0.16666666666666666};
        const double z = x * x;
        const double z2 = z * z;
        const double base = z * -0.5 + 1.0;
        const double cosine = (1.0 - base) + z * -0.5 +
            (z * -0.0013888888888887398 + 0.041666666666666664) * z2 +
            (z * -2.755731727234489e-07 + 2.4801587298767044e-05) * z2 * z2 +
            z2 * z2 * z2 * (z * -1.138263981623609e-11 + 2.0876146382372144e-09) + base;
        return {cosine, x + x * z * sine_polynomial(z)};
    }
    const auto r = reduce(absolute);
    double cosine = r.head, sine = r.head;
    const bool odd = (r.quadrant & 1) != 0;
    if (r.initial_exponent < 0x3f2) {
        if (r.initial_exponent < 0x3df) {
            if (!odd) cosine = 1.0;
            else sine = 1.0;
        } else {
            if (!odd) {
                cosine = 1.0 - r.head * r.head * 0.5;
                sine = r.head - r.head * 0.16666666666666666 * r.head * r.head;
            } else {
                cosine = r.head - r.head * 0.16666666666666666 * r.head * r.head;
                sine = 1.0 - r.head * r.head * 0.5;
            }
        }
    } else {
        cosine = odd ? reduced_sine(r.head, r.tail) : reduced_cosine(r.head, r.tail);
        sine = odd ? reduced_cosine(r.head, r.tail) : reduced_sine(r.head, r.tail);
    }
    if (((r.quadrant + 1) & 2) != 0) cosine = 0.0 - cosine;
    if (((r.quadrant >> 1) & 1) != static_cast<unsigned>(x != absolute)) sine = 0.0 - sine;
    return {cosine, sine};
}
} // namespace deform::recovered
