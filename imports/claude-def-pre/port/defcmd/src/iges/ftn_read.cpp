// Fortran formatted READ for the IGES library (packet p8-03), see ftn_read.hpp.
#define _CRT_SECURE_NO_WARNINGS
#include "ftn_read.hpp"

#include <bit>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <map>

#include <defrt/stdlib/libm.hpp>

#include <fcntl.h>
#include <io.h>

namespace def::cmd::iges::ftnrd {

std::function<void(const std::string& stderr_bytes, int code)> g_exit_hook;

namespace {

struct Unit {
    std::string data;    // file contents in text mode (CR LF -> LF)
    std::size_t pos = 0; // next byte
};

std::map<int, Unit>& units()
{
    static std::map<int, Unit> u;
    return u;
}

bool blank(char c) { return c == ' '; }

} // namespace

bool open_old(int unit, const char* name, std::size_t len)
{
    std::string n(name, len);
    while (!n.empty() && n.back() == ' ') n.pop_back();
    if (n.empty()) return false;
    std::FILE* f = std::fopen(n.c_str(), "rb");
    if (f == nullptr) return false;
    std::string raw;
    char buf[65536];
    std::size_t k;
    while ((k = std::fread(buf, 1, sizeof buf, f)) > 0) raw.append(buf, k);
    std::fclose(f);
    Unit u;
    u.data.reserve(raw.size());
    for (std::size_t i = 0; i < raw.size(); ++i) {
        if (raw[i] == '\r' && i + 1 < raw.size() && raw[i + 1] == '\n') continue;
        u.data += raw[i];
    }
    units()[unit] = std::move(u);
    return true;
}

void rewind(int unit)
{
    auto it = units().find(unit);
    if (it != units().end()) it->second.pos = 0;
}

void close(int unit) { units().erase(unit); }

int read_record(int unit, std::string& rec)
{
    auto it = units().find(unit);
    if (it == units().end()) {
        // reading a unit that is not open does not happen on the IGES paths
        runtime_error("Unconnected or illegal unit specified", std::to_string(unit).c_str());
    }
    Unit& u = it->second;
    if (u.pos >= u.data.size()) return 2;
    std::size_t e = u.data.find('\n', u.pos);
    if (e == std::string::npos) {
        rec = u.data.substr(u.pos);
        u.pos = u.data.size();
    } else {
        rec = u.data.substr(u.pos, e - u.pos);
        u.pos = e + 1;
    }
    return 0;
}

void reset() { units().clear(); }

std::string field(const std::string& rec, int c, int w)
{
    std::string f(static_cast<std::size_t>(w), ' ');
    for (int i = 0; i < w; ++i) {
        const std::size_t p = static_cast<std::size_t>(c - 1 + i);
        if (p < rec.size()) f[static_cast<std::size_t>(i)] = rec[p];
    }
    return f;
}

bool read_int(const std::string& f, std::int32_t* out)
{
    std::int64_t v = 0;
    bool neg = false, sign_seen = false, digit_seen = false;
    for (char c : f) {
        if (blank(c)) continue;
        if ((c == '+' || c == '-') && !sign_seen && !digit_seen) {
            sign_seen = true;
            neg = c == '-';
            continue;
        }
        if (c < '0' || '9' < c) return false;
        digit_seen = true;
        v = v * 10 + (c - '0');
    }
    *out = static_cast<std::int32_t>(neg ? -v : v);
    return true;
}

bool read_real_e0(const std::string& f, double* out)
{
    std::string t;
    for (char c : f)
        if (!blank(c)) t += c;
    if (t.empty()) {
        *out = 0.0;
        return true;
    }
    // Accepted shape (kept from the probes; the runtime's own check is FUN_1404c7130 mode 5):
    // [sign] digits [. digits] [ (E|D|Q [sign] | sign) digits ], at least one mantissa digit.
    auto digit = [&t](std::size_t k) { return k < t.size() && '0' <= t[k] && t[k] <= '9'; };
    std::size_t i = 0;
    if (t[i] == '+' || t[i] == '-') ++i;
    std::size_t nm = 0;
    for (; digit(i); ++i) ++nm;
    if (i < t.size() && t[i] == '.')
        for (++i; digit(i); ++i) ++nm;
    if (nm == 0) return false;
    if (i < t.size()) {
        const char c = t[i];
        if (c == 'E' || c == 'e' || c == 'D' || c == 'd' || c == 'Q' || c == 'q') {
            ++i;
            if (i < t.size() && (t[i] == '+' || t[i] == '-')) ++i;
        } else if (c == '+' || c == '-') {
            ++i;
        } else {
            return false;
        }
        std::size_t nd = 0;
        for (; digit(i); ++i) ++nd;
        if (nd == 0) return false;
    }
    if (i != t.size()) return false;
    // The value comes from the runtime's REAL*8 conversion of the compacted field (T8-2).
    *out = ew0_value(t.c_str());
    return true;
}

namespace {

// @src 1404ca0d4  in place: the 64-bit integer in the slot -> double, low bits dropped (no rounding)
std::uint64_t u64_to_double_bits(std::uint64_t v)
{
    std::uint32_t hi = static_cast<std::uint32_t>(v >> 32);
    std::uint32_t lo = static_cast<std::uint32_t>(v);
    if (hi == 0 && lo == 0) return v;
    std::uint32_t e = 0x433;
    if (hi > 0x100000u) {
        while (hi > 0x1fffffu) {
            lo = (lo >> 1) + (hi << 31);
            hi >>= 1;
            ++e;
        }
    } else {
        while (hi < 0x100000u) {
            --e;
            hi = (lo >> 31) + hi * 2u;
            lo += lo;
        }
    }
    hi &= ~0x100000u;
    return (static_cast<std::uint64_t>((e << 20) | hi) << 32) | lo;
}

} // namespace

// @src 1404c971c  REAL*8 case (kind not 9/12), third argument 0: FUN_1404c736c gives 0 for
// Ew.0 without a scale factor. Digits are gathered in a 64-bit integer while its high word is at
// most 0x19999999 (later integer digits only raise the exponent, later fraction digits are
// dropped), the integer is cut to 53 bits, then scaled once by 10**|e| from FUN_1404b47fc (ps_powi).
double ew0_value(const char* s)
{
    const char* p = s;
    while (*p == ' ' || *p == '\t') ++p;
    std::uint32_t sign = 0;
    int c = static_cast<signed char>(*p++);
    if (c == '+' || c == '-') {
        if (c == '-') sign = 0x80000000u;
        c = static_cast<signed char>(*p++);
    }
    if (c == 'I' || c == 'i') return std::bit_cast<double>(static_cast<std::uint64_t>(sign | 0x7ff00000u) << 32);
    if (c == 'N' || c == 'n')
        return std::bit_cast<double>((static_cast<std::uint64_t>(sign | 0x7fff0000u) << 32) | 0xffffffffu);
    constexpr std::uint32_t lim = 0x19999999u;
    std::uint64_t u = 0;
    std::uint32_t e = 0;     // ESI
    std::int32_t zeros = 0;  // R9D: fraction zeros not yet applied
    auto hi = [&u] { return static_cast<std::uint32_t>(u >> 32); };
    while (c >= '0' && c <= '9') {
        if (hi() > lim) ++e;
        else u = static_cast<std::uint64_t>(c & 0xf) + u * 10u;
        c = static_cast<signed char>(*p++);
    }
    if (c == '.') {
        for (c = static_cast<signed char>(*p++); c >= '0' && c <= '9'; c = static_cast<signed char>(*p++)) {
            if (hi() > lim) continue;
            if (c == '0') {
                ++zeros;
                continue;
            }
            for (; zeros > 0; --zeros) {
                if (hi() <= lim) {
                    u *= 10u;
                    --e;
                }
            }
            if (hi() <= lim) {
                --e;
                u = static_cast<std::uint64_t>(c & 0xf) + u * 10u;
            }
        }
    }
    if (c >= 'a' && c <= 'z') c &= 0xdf;
    if (c == 'E' || c == 'D') c = static_cast<signed char>(*p++);
    bool eneg = false;
    if (c == '-') {
        eneg = true;
        c = static_cast<signed char>(*p++);
    } else if (c == '+') {
        c = static_cast<signed char>(*p++);
    }
    std::uint32_t ex = 0; // R12D
    for (; c >= '0' && c <= '9'; c = static_cast<signed char>(*p++)) ex = static_cast<std::uint32_t>(c & 0xf) + ex * 10u;
    double x = std::bit_cast<double>(u64_to_double_bits(u));
    if (eneg) ex = 0u - ex;
    e += ex; // + the third argument, 0
    std::int32_t es = static_cast<std::int32_t>(e);
    if (es < -307) {
        const std::int32_t n = -(es + 307);
        es = -307;
        x = x / def::rt::libm::ps_powi(10.0, n);
    }
    if (es != 0) {
        const double t = def::rt::libm::ps_powi(10.0, es < 0 ? -es : es);
        if (es > 0) x = t * x;
        else x = x / t;
    }
    return std::bit_cast<double>(std::bit_cast<std::uint64_t>(x) | (static_cast<std::uint64_t>(sign) << 32));
}

namespace {

[[noreturn]] void leave(const std::string& bytes, int code)
{
    if (g_exit_hook) {
        g_exit_hook(bytes, code);
        std::abort(); // the hook must not return
    }
    std::fflush(nullptr);
    if (!bytes.empty()) {
        const int old = _setmode(2, _O_BINARY);
        (void)_write(2, bytes.data(), static_cast<unsigned>(bytes.size()));
        if (old != -1) _setmode(2, old);
    }
    std::exit(code);
}

} // namespace

void runtime_error(const char* what, const char* unit_text)
{
    std::string b = "? FORTRAN Runtime Error:\r\n? ";
    b += what;
    b += "\r\n? READ(UNIT=";
    b += unit_text;
    b += ",...\n";
    leave(b, -1);
}

void stop() { leave(std::string(), 0); }

} // namespace def::cmd::iges::ftnrd
