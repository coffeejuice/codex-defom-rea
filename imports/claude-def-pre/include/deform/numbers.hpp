#pragma once

// Number and token helpers for KEY text: Fortran-style output such as
// "1.0752490170E+000", "-7", "1.0D-03" and overflow fields ("********").

#include <cstdint>
#include <optional>
#include <string_view>

namespace dk {

[[nodiscard]] std::optional<std::int64_t> parse_int(std::string_view token) noexcept;

// Accepts E/e/D/d exponents and a leading '+'. Fortran overflow fields made of
// '*' parse as NaN; anything else that is not a number returns nullopt.
[[nodiscard]] std::optional<double> parse_real(std::string_view token) noexcept;

[[nodiscard]] bool is_int_token(std::string_view token) noexcept;
[[nodiscard]] bool is_number_token(std::string_view token) noexcept;

// Whitespace tokenizer over a text range. Line breaks count as whitespace.
class TokenCursor {
public:
    TokenCursor() = default;
    explicit TokenCursor(std::string_view text) noexcept
        : p_(text.data()), end_(text.data() + text.size())
    {
    }

    // Next token, or false at the end.
    bool next(std::string_view &token) noexcept
    {
        while (p_ < end_ && is_space(*p_)) {
            ++p_;
        }
        if (p_ >= end_) {
            return false;
        }
        const char *start = p_;
        while (p_ < end_ && !is_space(*p_)) {
            ++p_;
        }
        token = std::string_view(start, static_cast<std::size_t>(p_ - start));
        return true;
    }

    // Skips the rest of the current line (including the newline).
    void skip_line() noexcept
    {
        while (p_ < end_ && *p_ != '\n') {
            ++p_;
        }
        if (p_ < end_) {
            ++p_;
        }
    }

    // True when only whitespace remains up to the next newline.
    [[nodiscard]] bool at_line_end() const noexcept
    {
        const char *q = p_;
        while (q < end_ && *q != '\n' && is_space(*q)) {
            ++q;
        }
        return q >= end_ || *q == '\n';
    }

    // Returns the remainder of the current line (without CR/LF) and advances
    // past it.
    std::string_view take_line() noexcept
    {
        const char *start = p_;
        while (p_ < end_ && *p_ != '\n') {
            ++p_;
        }
        const char *stop = p_;
        if (p_ < end_) {
            ++p_;
        }
        if (stop > start && stop[-1] == '\r') {
            --stop;
        }
        return std::string_view(start, static_cast<std::size_t>(stop - start));
    }

    [[nodiscard]] bool done() const noexcept { return p_ >= end_; }
    [[nodiscard]] const char *position() const noexcept { return p_; }

    // Skips whitespace; true if any non-whitespace remains.
    [[nodiscard]] bool has_more() noexcept
    {
        while (p_ < end_ && is_space(*p_)) {
            ++p_;
        }
        return p_ < end_;
    }

private:
    static constexpr bool is_space(char c) noexcept
    {
        return c == ' ' || c == '\t' || c == '\r' || c == '\n' || c == '\f' || c == '\v';
    }
    const char *p_ = nullptr;
    const char *end_ = nullptr;
};

} // namespace dk
