#include "deform/numbers.hpp"

#include <charconv>
#include <cmath>
#include <limits>

namespace dk {

std::optional<std::int64_t> parse_int(std::string_view t) noexcept
{
    if (!t.empty() && t.front() == '+') {
        t.remove_prefix(1);
    }
    std::int64_t v = 0;
    const auto [ptr, ec] = std::from_chars(t.data(), t.data() + t.size(), v);
    if (ec != std::errc{} || ptr != t.data() + t.size() || t.empty()) {
        return std::nullopt;
    }
    return v;
}

std::optional<double> parse_real(std::string_view t) noexcept
{
    if (t.empty()) {
        return std::nullopt;
    }
    if (t.front() == '+') {
        t.remove_prefix(1);
        if (t.empty()) {
            return std::nullopt;
        }
    }
    double v = 0.0;
    auto [ptr, ec] = std::from_chars(t.data(), t.data() + t.size(), v);
    if (ec == std::errc{} && ptr == t.data() + t.size()) {
        return v;
    }
    if (ec == std::errc::result_out_of_range && ptr == t.data() + t.size()) {
        // Denormal/huge values: fall back to strtod semantics.
        char buf[64];
        if (t.size() < sizeof(buf)) {
            t.copy(buf, t.size());
            buf[t.size()] = '\0';
            return std::strtod(buf, nullptr);
        }
    }
    // Fortran D exponent.
    if (t.size() < 64) {
        char buf[64];
        bool changed = false;
        for (std::size_t i = 0; i < t.size(); ++i) {
            char c = t[i];
            if (c == 'D' || c == 'd') {
                c = 'E';
                changed = true;
            }
            buf[i] = c;
        }
        if (changed) {
            auto [p2, ec2] = std::from_chars(buf, buf + t.size(), v);
            if (ec2 == std::errc{} && p2 == buf + t.size()) {
                return v;
            }
        }
    }
    // Fortran field overflow.
    bool stars = true;
    for (char c : t) {
        if (c != '*') {
            stars = false;
            break;
        }
    }
    if (stars) {
        return std::numeric_limits<double>::quiet_NaN();
    }
    return std::nullopt;
}

bool is_int_token(std::string_view t) noexcept
{
    if (!t.empty() && (t.front() == '+' || t.front() == '-')) {
        t.remove_prefix(1);
    }
    if (t.empty()) {
        return false;
    }
    for (char c : t) {
        if (c < '0' || c > '9') {
            return false;
        }
    }
    return true;
}

bool is_number_token(std::string_view t) noexcept
{
    return is_int_token(t) || parse_real(t).has_value();
}

} // namespace dk
