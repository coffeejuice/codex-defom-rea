// The MSVC 2008 iostream pieces in the p9-03 packet list (std::ios_base::setf / precision, the
// ostream inserter for double and its num_put call). The NBCD.OUT writer of the revolve sweep
// (p9-03b) prints with them: setf(left), precision(8), setf(scientific) -> "%.8e" text with the
// old CRT's three exponent digits (def::rt::crt::snprintf).
#define _CRT_SECURE_NO_WARNINGS
#include <defcmd/cnvt3d_sweep.hpp>

#include <cstdio>
#include <string>

#include <defrt/stdlib/legacy_printf.hpp>

namespace def::cmd::cnvt3d_sweep {

namespace {
constexpr std::int32_t kUppercase = 0x4;
constexpr std::int32_t kShowpoint = 0x10;
constexpr std::int32_t kShowpos = 0x20;
constexpr std::int32_t kLeft = 0x40;
constexpr std::int32_t kInternal = 0x100;
constexpr std::int32_t kScientific = 0x1000;
constexpr std::int32_t kFixed = 0x2000;
constexpr std::int32_t kFloatfield = kScientific | kFixed;
} // namespace

// @src 140159050  std::ios_base::setf(int)
std::int32_t fun_140159050(IosState& s, std::int32_t f)
{
    const std::int32_t old = s.flags;
    s.flags = s.flags | (f & 0xffff);
    return old;
}

// @src 140159090  std::ios_base::precision(streamsize)
std::int64_t fun_140159090(IosState& s, std::int64_t n)
{
    const std::int64_t old = s.precision;
    s.precision = n;
    return old;
}

// @src 14015a260  num_put<char>::put(.., double) -> do_put (library): format from _Ffmt
//                 ('%' ['+'] ['#'] ".*" f|e|E|g|G), precision 6 when <= 0 and not fixed, then
//                 _Fput pads to width with fill (left: after, internal: after the sign, else
//                 before) and resets width to 0
std::string fun_14015a260(IosState& s, double v)
{
    char fmt[8];
    std::size_t k = 0;
    fmt[k++] = '%';
    if (s.flags & kShowpos) fmt[k++] = '+';
    if (s.flags & kShowpoint) fmt[k++] = '#';
    fmt[k++] = '.';
    fmt[k++] = '*';
    const std::int32_t ff = s.flags & kFloatfield;
    if (ff == kFixed) {
        fmt[k++] = 'f';
    } else if (ff == kScientific) {
        fmt[k++] = (s.flags & kUppercase) ? 'E' : 'e';
    } else {
        fmt[k++] = (s.flags & kUppercase) ? 'G' : 'g';
    }
    fmt[k] = '\0';
    const std::int64_t prec = (s.precision <= 0 && !(s.flags & kFixed)) ? 6 : s.precision;
    char buf[512];
    def::rt::crt::snprintf(buf, sizeof buf, fmt, static_cast<int>(prec), v);
    std::string t(buf);
    const std::int64_t w = s.width;
    s.width = 0;
    if (w > static_cast<std::int64_t>(t.size())) {
        const std::size_t pad = static_cast<std::size_t>(w) - t.size();
        const std::int32_t adj = s.flags & (kLeft | kInternal | 0x80);
        if (adj == kLeft) {
            t.append(pad, s.fill);
        } else if (adj == kInternal && !t.empty() && (t[0] == '+' || t[0] == '-')) {
            t.insert(1, pad, s.fill);
        } else {
            t.insert(0, pad, s.fill);
        }
    }
    return t;
}

// @src 1401595b0  basic_ostream<char>::operator<<(double): sentry, num_put put, badbit on failure
void fun_1401595b0(std::FILE* f, IosState& s, double v)
{
    const std::string t = fun_14015a260(s, v);
    std::fwrite(t.data(), 1, t.size(), f);
}

} // namespace def::cmd::cnvt3d_sweep
