#pragma once

#include <bit>
#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <string_view>

namespace m2m::engine::simcontrol300::detail {

// Bounded ASCII model of the shipped Qt 4 QTextStream integer getter. This
// models the parsed bit pattern and cursor only; callers decide how getter
// failure affects the destination and stream status.
struct Qt4IntParseResult {
    std::uint64_t value = 0;
    bool getterSucceeded = false;
    std::size_t nextByte = 0;
    bool sourceQualified = true;
};

struct Qt4IntOperatorResult {
    int value = 0;
    int freshStreamStatus = 0;
};

constexpr bool qt4AsciiSpace(unsigned char ch) noexcept
{
    return (ch >= 9 && ch <= 13) || ch == 32;
}

constexpr unsigned char qt4AsciiLower(unsigned char ch) noexcept
{
    return ch >= 'A' && ch <= 'Z' ? static_cast<unsigned char>(ch + ('a' - 'A')) : ch;
}

constexpr int qt4Digit(unsigned char ch) noexcept
{
    if (ch >= '0' && ch <= '9') return ch - '0';
    const auto lower = qt4AsciiLower(ch);
    if (lower >= 'a' && lower <= 'f') return lower - 'a' + 10;
    return -1;
}

inline Qt4IntParseResult parseQt4Base0Int(std::string_view bytes,
                                          std::size_t offset = 0,
                                          bool allowExactSw2spInvalidLeaves = false) noexcept
{
    std::size_t cursor = offset;
    while (cursor < bytes.size() && qt4AsciiSpace(static_cast<unsigned char>(bytes[cursor])))
        ++cursor;
    const auto first = cursor;
    if (cursor == bytes.size())
        return {0, false, cursor};

    bool negative = false;
    if (bytes[cursor] == '+' || bytes[cursor] == '-') {
        negative = bytes[cursor] == '-';
        ++cursor;
        std::uint64_t value = 0;
        std::size_t digits = 0;
        while (cursor < bytes.size()) {
            const auto ch = static_cast<unsigned char>(bytes[cursor]);
            if (ch < '0' || ch > '9') break;
            value = value * 10u + static_cast<std::uint64_t>(ch - '0');
            ++cursor;
            ++digits;
        }
        if (digits == 0) {
            const auto ch = cursor < bytes.size() ? static_cast<unsigned char>(bytes[cursor]) : 0;
            const bool asciiInput = std::all_of(bytes.begin(), bytes.end(), [](char byte) {
                const auto value = static_cast<unsigned char>(byte);
                return value == 9 || (value >= 10 && value <= 13) || value == 32 ||
                       (value >= 0x21 && value <= 0x7e);
            });
            const bool qClass = (ch >= 0x21 && ch <= 0x7e) && !(ch >= '0' && ch <= '9');
            const bool qWhitespace = ch == 9 || (ch >= 10 && ch <= 13) || ch == 32;
            if (allowExactSw2spInvalidLeaves && offset == 0 && cursor == first + 1 &&
                (bytes[first] == '+' || bytes[first] == '-') &&
                (qClass || qWhitespace) && asciiInput)
                return {0, false, cursor, true};
            if (allowExactSw2spInvalidLeaves && offset == 0 && cursor == bytes.size() &&
                (bytes[first] == '+' || bytes[first] == '-'))
                return {0, false, cursor, true};
            return {0, false, cursor, cursor == bytes.size()};
        }
        if (negative && value != 0 && (value & (std::uint64_t{1} << 63)) == 0)
            value = std::uint64_t{0} - value;
        return {value, true, cursor};
    }

    // Qt gives an unsigned leading zero a short prefix probe. A lone zero is
    // a successful value; x/b are case-folded radix prefixes; digits 8/9
    // choose decimal instead of octal.
    if (bytes[cursor] == '0') {
        if (cursor + 1 == bytes.size())
            return {0, true, cursor + 1};

        const auto prefix = qt4AsciiLower(static_cast<unsigned char>(bytes[cursor + 1]));
        unsigned base = 0;
        if (prefix == 'x') base = 16;
        else if (prefix == 'b') base = 2;
        else if (prefix >= '0' && prefix <= '7') base = 8;
        else if (prefix >= '8' && prefix <= '9') base = 10;
        else {
            // The initial zero is a decimal digit; the lookahead is ungot.
            return {0, true, cursor + 1};
        }

        if (base == 10) {
            std::uint64_t value = 0;
            std::size_t digits = 0;
            while (cursor < bytes.size()) {
                const auto ch = static_cast<unsigned char>(bytes[cursor]);
                if (ch < '0' || ch > '9') break;
                value = value * 10u + static_cast<std::uint64_t>(ch - '0');
                ++cursor;
                ++digits;
            }
            return {value, digits != 0, digits != 0 ? cursor : first};
        }

        const auto prefixStart = cursor;
        cursor += base == 8 ? 1 : 2;
        const auto digitStart = cursor;
        std::uint64_t value = 0;
        std::size_t digits = 0;
        while (cursor < bytes.size()) {
            const auto digit = qt4Digit(static_cast<unsigned char>(bytes[cursor]));
            if (digit < 0 || static_cast<unsigned>(digit) >= base) break;
            value = value * base + static_cast<unsigned>(digit);
            ++cursor;
            ++digits;
        }
        if (digits != 0)
            return {value, true, cursor};

        // Qt restores a zero-digit binary prefix, but leaves a zero-digit hex
        // prefix consumed and ungets only the following character.
        const auto rollback = base == 2 ? prefixStart : digitStart;
        return {0, false, rollback};
    }

    if (bytes[cursor] < '0' || bytes[cursor] > '9') {
        // CP486/487 PE-qualified finite ASCII class for the one-value SW2SP
        // callsite: initial M is ungot (getter error 2); after a consumed sign,
        // Q is ungot (getter error 1). The fresh operator writes zero/status 2
        // for these admitted inputs. Internal getter codes are not modeled.
        // The caller opt-in is only used by SW2SP; keep this ASCII model
        // bounded to printable malformed bytes and default-C whitespace.
        const auto ch = static_cast<unsigned char>(bytes[cursor]);
        const bool asciiInput = std::all_of(bytes.begin(), bytes.end(), [](char byte) {
            const auto value = static_cast<unsigned char>(byte);
            return value == 9 || (value >= 10 && value <= 13) || value == 32 ||
                   (value >= 0x21 && value <= 0x7e);
        });
        const bool qClass = (ch >= 0x21 && ch <= 0x7e) && !(ch >= '0' && ch <= '9');
        const bool firstClass = qClass && ch != '+' && ch != '-';
        if (allowExactSw2spInvalidLeaves && offset == 0 && cursor == first && firstClass && asciiInput)
            return {0, false, cursor, true};
        return {0, false, cursor, false};
    }

    std::uint64_t value = 0;
    std::size_t digits = 0;
    while (cursor < bytes.size()) {
        const auto ch = static_cast<unsigned char>(bytes[cursor]);
        if (ch < '0' || ch > '9') break;
        value = value * 10u + static_cast<std::uint64_t>(ch - '0');
        ++cursor;
        ++digits;
    }
    return {value, digits != 0, cursor};
}

inline Qt4IntOperatorResult applyQt4IntOperatorToFreshStream(
    const Qt4IntParseResult& parsed, std::size_t inputSize) noexcept
{
    if (!parsed.getterSucceeded) {
        return {0, parsed.nextByte >= inputSize ? 1 : 2};
    }
    const auto low = static_cast<std::uint32_t>(parsed.value);
    const auto signedLow = std::bit_cast<std::int32_t>(low);
    return {static_cast<int>(signedLow), 0};
}

} // namespace m2m::engine::simcontrol300::detail
