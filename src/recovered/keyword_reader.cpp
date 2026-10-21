#include "keyword_reader.hpp"

#include <algorithm>
#include <cerrno>
#include <clocale>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdint>
#include <limits>
#include <span>
#include <stdexcept>
#include <utility>

namespace deform::recovered {
namespace {

static_assert(sizeof(int) == 4, "recovered keyword integer fields require 32-bit int");

bool whitespace(unsigned char byte) {
    return byte == ' ' || byte == '\t' || byte == '\n' ||
           byte == '\r' || byte == '\v' || byte == '\f';
}

void require_c_locale() {
    // Never mutate process-global locale from an internal reader.
    for (const int category : {LC_CTYPE, LC_NUMERIC}) {
        const char* locale = std::setlocale(category, nullptr);
        if (!locale || (std::strcmp(locale, "C") != 0 &&
                        std::strcmp(locale, "POSIX") != 0))
            throw std::runtime_error("keyword numeric reader requires C locale");
    }
}

void replace_commas(std::string& bytes) {
    // 0x140323970 takes strlen first, so bytes after NUL are untouched.
    for (auto& byte : bytes) {
        if (byte == '\0') break;
        if (byte == ',') byte = ' ';
    }
}

bool digit(char byte) { return byte >= '0' && byte <= '9'; }

bool decimal(std::string_view text, std::size_t& cursor, double& destination) {
    // _input_l at 0x140493dec: the float branch recognizes decimal digits
    // and e/E, not the modern scanf hexadecimal/INF/NAN grammar. Keep the
    // cursor at the first unrecognized byte; the next conversion starts there.
    while (cursor < text.size() && whitespace(static_cast<unsigned char>(text[cursor])))
        ++cursor;
    const auto start = cursor;
    if (cursor < text.size() && (text[cursor] == '+' || text[cursor] == '-')) ++cursor;
    std::size_t digits = 0;
    while (cursor < text.size() && digit(text[cursor])) {
        ++cursor;
        ++digits;
    }
    if (cursor < text.size() && text[cursor] == '.') {
        ++cursor;
        while (cursor < text.size() && digit(text[cursor])) {
            ++cursor;
            ++digits;
        }
    }
    if (digits == 0) return false;
    auto conversion_end = cursor;
    if (cursor < text.size() && (text[cursor] == 'e' || text[cursor] == 'E')) {
        ++cursor;
        if (cursor < text.size() && (text[cursor] == '+' || text[cursor] == '-')) ++cursor;
        const auto exponent_digits = cursor;
        while (cursor < text.size() && digit(text[cursor])) ++cursor;
        if (cursor != exponent_digits) conversion_end = cursor;
        // Discovery capture confirms 1e, 1e+, 1e- consume the exponent piece,
        // assign 1, and allow subsequent conversions. Convert the significand
        // only when no exponent digit exists; do not rewind the input cursor.
    }
    const std::string bounded(text.substr(start, conversion_end - start));
    char* end = nullptr;
    const int previous_errno = errno;
    errno = 0;
    const double value = std::strtod(bounded.c_str(), &end);
    const int conversion_errno = errno;
    errno = previous_errno;
    if (end != bounded.c_str() + bounded.size())
        throw std::runtime_error("bounded decimal conversion failed");
    if (conversion_errno == ERANGE || !std::isfinite(value))
        throw std::out_of_range("floating range outside keyword compatibility domain");
    destination = value;
    return true;
}

int decimal_integer_prefix(std::string_view text) {
    // 47b6c0 explicitly calls signed32 strtol(base10), not scanf %i.
    std::size_t cursor = 0;
    while (cursor < text.size() && whitespace(static_cast<unsigned char>(text[cursor]))) ++cursor;
    bool negative = false;
    if (cursor < text.size() && (text[cursor] == '+' || text[cursor] == '-'))
        negative = text[cursor++] == '-';
    const std::uint64_t limit = negative ? 2147483648ULL : 2147483647ULL;
    std::uint64_t magnitude = 0;
    while (cursor < text.size() && digit(text[cursor])) {
        const unsigned value = static_cast<unsigned>(text[cursor++] - '0');
        if (magnitude > (limit - value) / 10)
            throw std::out_of_range("dynamic integer prefix outside signed32 compatibility domain");
        magnitude = magnitude * 10 + value;
    }
    return static_cast<int>(negative ? -static_cast<std::int64_t>(magnitude)
                                     : static_cast<std::int64_t>(magnitude));
}

int decimal_conversions(std::string_view text, std::span<double> values) {
    std::size_t cursor = 0;
    int conversions = 0;
    for (auto& value : values) {
        if (!decimal(text, cursor, value)) {
            // Consuming a first sign/dot through EOF is input failure; an
            // unconsumed LF is matching failure. Earlier assignments survive.
            return conversions == 0 && cursor == text.size() ? -1 : conversions;
        }
        ++conversions;
    }
    return conversions;
}

} // namespace

KeywordReader::KeywordReader(std::string_view translated_bytes, std::size_t offset)
    : input_(translated_bytes), cursor_(offset) {
    if (offset > input_.size()) throw std::out_of_range("keyword cursor beyond input");
}

KeywordReader::TextRead KeywordReader::token() {
    while (cursor_ < input_.size() &&
           whitespace(static_cast<unsigned char>(input_[cursor_])))
        ++cursor_;
    if (cursor_ == input_.size()) return {};
    const auto start = cursor_;
    while (cursor_ < input_.size() && cursor_ - start < 39 &&
           !whitespace(static_cast<unsigned char>(input_[cursor_])))
        ++cursor_;
    return {input_.substr(start, cursor_ - start), 0};
}

KeywordReader::LineRead KeywordReader::line() {
    LineRead result;
    result.offset = cursor_;
    if (cursor_ == input_.size()) {
        result.status = 1;
    } else {
        while (cursor_ < input_.size() && cursor_ - result.offset < 1023) {
            if (input_[cursor_++] == '\n') break;
        }
        result.bytes = input_.substr(result.offset, cursor_ - result.offset);
    }
    result.cursor = cursor_;
    line_reads_.push_back(result);
    return result;
}

KeywordReader::IntegerRead KeywordReader::selector_integer() {
    require_c_locale();
    IntegerRead result;
    result.count = 1;
    while (cursor_ < input_.size() &&
           whitespace(static_cast<unsigned char>(input_[cursor_]))) ++cursor_;
    bool negative = false;
    if (cursor_ < input_.size() && (input_[cursor_] == '+' || input_[cursor_] == '-'))
        negative = input_[cursor_++] == '-';

    // _input_l 4942d3/494886: leading zero selects octal, 0x selects
    // hexadecimal. The prefix is consumed even if no hex digit follows.
    unsigned base = 10;
    bool any = false;
    if (cursor_ < input_.size() && input_[cursor_] == '0') {
        ++cursor_;
        base = 8;
        any = true;
        if (cursor_ < input_.size() && (input_[cursor_] == 'x' || input_[cursor_] == 'X')) {
            ++cursor_;
            base = 16;
            any = false;
        }
    }
    std::uint64_t magnitude = 0;
    const auto limit = static_cast<std::uint64_t>(std::numeric_limits<int>::max()) +
                      static_cast<unsigned>(negative);
    while (cursor_ < input_.size()) {
        const char byte = input_[cursor_];
        unsigned value = 16;
        if (byte >= '0' && byte <= '9') value = static_cast<unsigned>(byte - '0');
        else if (byte >= 'a' && byte <= 'f') value = static_cast<unsigned>(byte - 'a' + 10);
        else if (byte >= 'A' && byte <= 'F') value = static_cast<unsigned>(byte - 'A' + 10);
        if (value >= base) break;
        ++cursor_;
        if (magnitude > (limit - value) / base)
            throw std::out_of_range("object selector exceeds signed integer compatibility domain");
        magnitude = magnitude * base + value;
        any = true;
    }
    if (any) {
        const auto signed_value = negative ? -static_cast<std::int64_t>(magnitude)
                                           : static_cast<std::int64_t>(magnitude);
        result.values[0] = static_cast<int>(signed_value);
        result.conversions = 1;
        result.status = 0;
    } else {
        result.conversions = cursor_ == input_.size() ? -1 : 0;
        result.status = result.conversions == -1 ? 1 : 0;
    }
    return result;
}

KeywordReader::IntegerRead KeywordReader::integer(bool skip_one_empty_line) {
    return integers(1, skip_one_empty_line ? BlankRule::once : BlankRule::none);
}

KeywordReader::IntegerRead KeywordReader::selector_pair() {
    auto result = selector_integer();
    result.count = 2;
    if (result.conversions != 1) return result;
    const auto second = selector_integer();
    if (second.conversions == 1) {
        result.values[1] = second.values[0];
        result.conversions = 2;
    }
    // EOF after one assignment returns1, not EOF, for the combined scan.
    result.status = 0;
    return result;
}

KeywordReader::IntegerRead KeywordReader::integer_pair() {
    return integers(2, BlankRule::none);
}

KeywordReader::IntegerRead KeywordReader::integer_triple() {
    return integers(3, BlankRule::none);
}

KeywordReader::IntegerRead KeywordReader::connectivity3() {
    return integers(4, BlankRule::none);
}

KeywordReader::IntegerRead KeywordReader::connectivity4(bool skip_empty_lines) {
    return integers(5, skip_empty_lines ? BlankRule::repeated : BlankRule::none);
}

KeywordReader::IntegerRead KeywordReader::connectivity8(bool skip_empty_lines) {
    return integers(9, skip_empty_lines ? BlankRule::repeated : BlankRule::none);
}

KeywordReader::IntegerRead KeywordReader::integers(std::size_t count, BlankRule rule) {
    require_c_locale();
    IntegerRead result;
    result.count = count;
    auto input = line();
    if (rule == BlankRule::once) {
        if (input.status == 0 && input.bytes == "\n") input = line();
    } else if (rule == BlankRule::repeated) {
        while (input.status == 0 && input.bytes == "\n") input = line();
    }
    if (input.status != 0) return result;
    replace_commas(input.bytes);
    auto& v = result.values;
    // 413de0 / 413ee0 / 414200 / 414600. Destinations are zero before
    // conversion; partial successful conversions remain visible.
    // These fixed numeric formats target correctly typed scalars. Retain scanf
    // conversion-count semantics rather than substitute the secure CRT API.
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4996)
#endif
    switch (count) {
    case 1:
        result.conversions = std::sscanf(input.bytes.c_str(), "%i", &v[0]);
        break;
    case 2:
        result.conversions = std::sscanf(input.bytes.c_str(), "%i %i", &v[0], &v[1]);
        break;
    case 3:
        result.conversions = std::sscanf(input.bytes.c_str(), "%i %i %i", &v[0], &v[1], &v[2]);
        break;
    case 4:
        result.conversions = std::sscanf(input.bytes.c_str(), "%i %i %i %i", &v[0], &v[1], &v[2], &v[3]);
        break;
    case 5:
        result.conversions = std::sscanf(input.bytes.c_str(), "%i %i %i %i %i",
            &v[0], &v[1], &v[2], &v[3], &v[4]);
        break;
    case 9:
        result.conversions = std::sscanf(input.bytes.c_str(), "%i %i %i %i %i %i %i %i %i",
            &v[0], &v[1], &v[2], &v[3], &v[4], &v[5], &v[6], &v[7], &v[8]);
        break;
    default:
        throw std::logic_error("unsupported integer helper");
    }
#ifdef _MSC_VER
#pragma warning(pop)
#endif
    result.status = result.conversions == EOF ? 1 : 0;
    return result;
}

KeywordReader::CoordinateRead KeywordReader::coordinates2() { return coordinates(2); }
KeywordReader::CoordinateRead KeywordReader::coordinates3() { return coordinates(3); }

KeywordReader::FloatRead KeywordReader::real() { return reals(1); }
KeywordReader::FloatRead KeywordReader::real_pair() { return reals(2); }

KeywordReader::FloatTripleRead KeywordReader::real_triple() {
    require_c_locale();
    FloatTripleRead result;
    auto input = line();
    if (input.status != 0) return result;
    replace_commas(input.bytes);
    result.conversions = decimal_conversions(
        std::string_view(input.bytes.c_str()), result.values);
    result.status = result.conversions == -1 ? 1 : 0;
    return result;
}

KeywordReader::FloatQuadRead KeywordReader::real_quad() {
    require_c_locale();
    FloatQuadRead result;
    auto input = line();
    if (input.status != 0) return result;
    replace_commas(input.bytes);
    result.conversions = decimal_conversions(
        std::string_view(input.bytes.c_str()), result.values);
    result.status = result.conversions == -1 ? 1 : 0;
    return result;
}

KeywordReader::IntegerArrayRead KeywordReader::integer_array10() {
    require_c_locale();
    IntegerArrayRead result;
    auto input = line(); // Consumed even when the caller requests no copies.
    if (input.status != 0) return result;
    replace_commas(input.bytes);
    auto& v = result.values;
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4996)
#endif
    result.conversions = std::sscanf(input.bytes.c_str(),
        "%i %i %i %i %i %i %i %i %i %i", &v[0], &v[1], &v[2], &v[3], &v[4],
        &v[5], &v[6], &v[7], &v[8], &v[9]);
#ifdef _MSC_VER
#pragma warning(pop)
#endif
    result.status = 0; // Conversion failure is ignored by the original helper.
    return result;
}

KeywordReader::RealIntegerRead KeywordReader::real_integer() {
    require_c_locale();
    RealIntegerRead result;
    auto input = line();
    if (input.status != 0) return result;
    replace_commas(input.bytes);
    const std::string_view text(input.bytes.c_str());
    std::size_t cursor = 0;
    if (!decimal(text, cursor, result.real_value)) {
        result.conversions = cursor == text.size() ? -1 : 0;
    } else {
        result.conversions = 1;
        // Reuse the recovered base0 integer grammar for the second conversion.
        // This bounded scanner has its own cursor; the outer fgets has already
        // consumed the complete line. Failure cannot erase the real assignment.
        KeywordReader remaining(text, cursor);
        const auto integer = remaining.selector_integer();
        if (integer.conversions == 1) {
            result.integer_value = integer.values[0];
            ++result.conversions;
        }
    }
    result.status = result.conversions == -1 ? 1 : 0;
    return result;
}

KeywordReader::MixedIntegerRealRead KeywordReader::integer_pair_real() {
    require_c_locale();
    MixedIntegerRealRead result;
    auto input = line();
    if (input.status != 0) return result;
    replace_commas(input.bytes);
    KeywordReader fields(std::string_view(input.bytes.c_str()));
    const auto count = fields.selector_integer();
    result.conversions = count.conversions;
    if (count.conversions == 1) {
        result.integers[0] = count.values[0];
        const auto value = fields.stream_real();
        if (value.conversions == 1) {
            ++result.conversions;
            result.real_value = value.values[0];
            const auto width = fields.selector_integer();
            if (width.conversions == 1) {
                ++result.conversions;
                result.integers[1] = width.values[0];
            }
        }
    }
    result.status = result.conversions == -1 ? 1 : 0;
    return result;
}

KeywordReader::CustomHeaderRead KeywordReader::custom_variable_header() {
    require_c_locale();
    CustomHeaderRead result;
    auto input = line();
    if (input.status != 0) return result;
    replace_commas(input.bytes);
    KeywordReader fields(std::string_view(input.bytes.c_str()));
    result.conversions = 0;
    for (std::size_t index = 0; index < 3; ++index) {
        const auto integer = fields.selector_integer();
        if (integer.conversions != 1) {
            if (index == 0) result.conversions = integer.conversions;
            result.status = result.conversions == -1 ? 1 : 0;
            return result;
        }
        result.integers[index] = integer.values[0];
        ++result.conversions;
    }
    const auto value = fields.stream_real();
    if (value.conversions == 1) {
        result.default_value = value.values[0];
        ++result.conversions;
        const auto integer = fields.selector_integer();
        if (integer.conversions == 1) {
            result.integers[3] = integer.values[0];
            ++result.conversions;
        }
    }
    result.status = 0;
    return result;
}

KeywordReader::ShapeHeaderRead KeywordReader::shape_header() {
    require_c_locale();
    ShapeHeaderRead result;
    const auto input = line();
    if (input.status != 0) return result;
    KeywordReader fields(std::string_view(input.bytes.c_str()));
    const std::array<int*, 3> outputs{&result.rows, &result.selector, &result.width};
    result.conversions = 0;
    for (std::size_t index = 0; index < outputs.size(); ++index) {
        const auto value = fields.selector_integer();
        if (value.conversions != 1) {
            if (index == 0) result.conversions = value.conversions;
            return result; // Width is still zero, so native rejects this header.
        }
        *outputs[index] = value.values[0];
        ++result.conversions;
    }
    const auto value = fields.stream_real();
    if (value.conversions == 1) {
        result.default_value = value.values[0];
        ++result.conversions;
    }
    result.status = result.width < 1 ? 1 : 0;
    return result;
}

KeywordReader::DynamicHeaderRead KeywordReader::dynamic_array_header() {
    require_c_locale();
    DynamicHeaderRead result;
    const auto input = line();
    if (input.status != 0) return result;
    if (input.bytes.find('\0') != std::string::npos)
        throw std::invalid_argument("embedded NUL outside dynamic header compatibility domain");
    const auto delimiter = [](char value) {
        return value == ' ' || value == ',' || value == '\n' || value == '\t' || value == '\r';
    };
    std::size_t cursor = 0;
    while (result.tokens.size() < 10) {
        while (cursor < input.bytes.size() && delimiter(input.bytes[cursor])) ++cursor;
        if (cursor == input.bytes.size()) break;
        const auto start = cursor;
        while (cursor < input.bytes.size() && !delimiter(input.bytes[cursor])) ++cursor;
        result.tokens.emplace_back(input.bytes.substr(start, cursor - start));
    }
    // 413900 counts its first strtok attempt even when it returned null;
    // 413a50 supplies an empty string for that and all missing token slots.
    if (result.tokens.empty()) result.tokens.emplace_back();
    result.token_count = static_cast<int>(result.tokens.size());
    for (std::size_t index = 0; index < 5; ++index)
        result.integers[index] = decimal_integer_prefix(index < result.tokens.size()
            ? std::string_view(result.tokens[index]) : std::string_view{});
    result.integers[5] = result.token_count < 6 ? result.integers[3]
        : decimal_integer_prefix(result.tokens[5]);
    if (result.token_count > 6) {
        // __strgtold12_l accepts D/d as well as E/e and falls back to the
        // significand for an incomplete exponent. Its invalid initial token
        // produces positive zero. No modern hexadecimal/INF/NAN grammar.
        auto normalized = result.tokens[6];
        for (auto& value : normalized) {
            if (value == 'D') value = 'E';
            else if (value == 'd') value = 'e';
        }
        std::size_t numeric_cursor = 0;
        (void)decimal(normalized, numeric_cursor, result.default_value);
    }
    result.status = 0;
    return result;
}

KeywordReader::FloatRead KeywordReader::stream_real(double previous_value) {
    require_c_locale();
    if (!std::isfinite(previous_value))
        throw std::invalid_argument("nonfinite stream destination outside compatibility domain");
    FloatRead result;
    result.count = 1;
    result.values[0] = previous_value;
    if (decimal(input_, cursor_, result.values[0])) result.conversions = 1;
    else result.conversions = cursor_ == input_.size() ? -1 : 0;
    result.status = result.conversions == -1 ? 1 : 0;
    return result;
}

KeywordReader::MaterialColumnRead KeywordReader::material_columns(int element_count, int input_width) {
    if (element_count < 0 || input_width < 0 ||
        (input_width != 0 && element_count > std::numeric_limits<int>::max() / input_width))
        throw std::out_of_range("material-column scratch extent outside compatibility domain");
    std::vector<double> scratch(static_cast<std::size_t>(element_count) * input_width, 0.0);
    return real_columns(scratch, element_count, input_width, 1);
}

KeywordReader::MaterialColumnRead KeywordReader::real_columns(std::span<double> destination,
    int row_count, int width, int mode) {
    require_c_locale();
    if (mode != 0 && mode != 1 && mode != 6)
        throw std::invalid_argument("real-column mode outside recovered compatibility domain");
    // This native rejection precedes both fgets and any destination access;
    // no declared extent is required. Report the supplied initialized span.
    if (mode == 6 && width > 10) {
        MaterialColumnRead rejected;
        rejected.values.assign(destination.begin(), destination.end());
        rejected.written.assign(destination.size(), false);
        return rejected;
    }
    if (row_count < 0 || width < 0 ||
        (width != 0 && row_count > std::numeric_limits<int>::max() / width))
        throw std::out_of_range("real-column extent outside compatibility domain");
    const auto extent = static_cast<std::size_t>(row_count) * width;
    if (destination.size() < extent)
        throw std::out_of_range("real-column header extent exceeds owned destination");
    MaterialColumnRead result;
    result.written.assign(extent, false);
    const auto finish = [&]() {
        result.values.assign(destination.begin(), destination.begin() + extent);
        return result;
    };
    const auto header = line();
    if (header.status != 0) return finish();
    KeywordReader fields(header.bytes); // No comma normalization or blank skipping.
    const auto rows = fields.selector_integer();
    result.header_conversions = rows.conversions;
    if (rows.conversions != 1) return finish();
    result.declared_rows = rows.values[0];
    bool read_defaults = true;
    if (mode == 1) {
        const auto explicit_width = fields.selector_integer();
        read_defaults = explicit_width.conversions == 1;
        if (read_defaults) {
            ++result.header_conversions;
            result.explicit_width = explicit_width.values[0];
        }
    }
    if (read_defaults) {
        const int maximum = mode == 6 ? 10 : 1;
        for (int index = 0; index < maximum; ++index) {
            const auto value = fields.stream_real();
            if (value.conversions != 1) break;
            ++result.header_conversions;
            result.default_values.push_back(value.values[0]);
        }
    }
    if (!result.default_values.empty()) {
        result.default_assigned = true;
        result.default_value = result.default_values.front();
        const bool per_column = result.default_values.size() == static_cast<std::size_t>(width);
        for (std::size_t index = 0; index < extent; ++index)
            destination[index] = per_column ? result.default_values[index % width] : result.default_value;
        std::fill(result.written.begin(), result.written.end(), true);
    }
    if (result.explicit_width < 0)
        throw std::out_of_range("negative real-column row allocation");
    const bool implicit_width = result.explicit_width == 0;
    const int row_width = implicit_width ? width : result.explicit_width;
    if (implicit_width && result.declared_rows < 1) {
        result.status = 0;
        return finish();
    }
    result.row_buffer.assign(static_cast<std::size_t>(row_width), 0.0);
    for (int row = 0; row < result.declared_rows; ++row) {
        const auto identifier = selector_integer();
        if (identifier.conversions == -1) return finish();
        if (identifier.conversions == 1) {
            result.row_id = identifier.values[0];
            result.row_id_initialized = true;
        } else if (!result.row_id_initialized) {
            throw std::runtime_error("first real-column row ID is uninitialized after matching failure");
        }
        for (double& retained : result.row_buffer) {
            const auto value = stream_real(retained);
            if (value.conversions == -1) return finish();
            retained = value.values[0];
        }
        if (result.row_id > 0 && result.row_id <= row_count) {
            const auto offset = static_cast<std::size_t>(result.row_id - 1) * width;
            const int copied_width = std::min(width, row_width);
            std::copy_n(result.row_buffer.begin(), copied_width, destination.begin() + offset);
            std::fill_n(result.written.begin() + offset, copied_width, true);
        }
        ++result.completed_rows;
    }
    if (implicit_width && line().status != 0) return finish();
    result.status = 0;
    return finish();
}

KeywordReader::MaterialColumnRead KeywordReader::real_rows(std::span<double> destination,
    int declared_rows, int row_count, int width) {
    return real_rows_impl(destination, declared_rows, row_count, width, true);
}

KeywordReader::MaterialColumnRead KeywordReader::real_rows_explicit(std::span<double> destination,
    int declared_rows, int row_count, int width) {
    if (width < 1)
        throw std::invalid_argument("explicit real-row width must be positive");
    auto result = real_rows_impl(destination, declared_rows, row_count, width, false);
    result.explicit_width = width;
    return result;
}

KeywordReader::MaterialColumnRead KeywordReader::real_rows_impl(std::span<double> destination,
    int declared_rows, int row_count, int width, bool trailing_line) {
    require_c_locale();
    MaterialColumnRead result;
    result.declared_rows = declared_rows;
    // Raw4199a0 returns before allocating or using any destination when there
    // are no records. Explicit419b90 still allocates its row buffer.
    if (trailing_line && declared_rows < 1) {
        result.status = 0;
        result.values.assign(destination.begin(), destination.end());
        result.written.assign(destination.size(), false);
        return result;
    }
    if (row_count < 0 || width < 0 ||
        (width && row_count > std::numeric_limits<int>::max() / width))
        throw std::out_of_range("real-row extent outside compatibility domain");
    const auto extent = static_cast<std::size_t>(row_count) * width;
    if (destination.size() < extent)
        throw std::out_of_range("real-row extent exceeds owned destination");
    result.written.assign(extent, false);
    result.row_buffer.assign(static_cast<std::size_t>(width), 0.0);
    const auto finish = [&]() {
        result.values.assign(destination.begin(), destination.begin() + extent);
        return result;
    };
    for (int row = 0; row < declared_rows; ++row) {
        const auto identifier = selector_integer();
        if (identifier.conversions == -1) return finish();
        if (identifier.conversions == 1) {
            result.row_id = identifier.values[0];
            result.row_id_initialized = true;
        } else if (!result.row_id_initialized) {
            throw std::runtime_error("first real-row ID is uninitialized after matching failure");
        }
        for (double& retained : result.row_buffer) {
            const auto value = stream_real(retained);
            if (value.conversions == -1) return finish();
            retained = value.values[0];
        }
        if (result.row_id > 0 && result.row_id <= row_count) {
            const auto offset = static_cast<std::size_t>(result.row_id - 1) * width;
            std::copy(result.row_buffer.begin(), result.row_buffer.end(), destination.begin() + offset);
            std::fill_n(result.written.begin() + offset, width, true);
        }
        ++result.completed_rows;
    }
    if (trailing_line && line().status != 0) return finish();
    result.status = 0;
    return finish();
}

KeywordReader::MaterialColumnRead KeywordReader::real_columns_mode2(std::span<double> destination,
    int row_count, int& width) {
    require_c_locale();
    MaterialColumnRead result;
    const auto untouched = [&]() {
        result.values.assign(destination.begin(), destination.end());
        result.written.assign(destination.size(), false);
        return result;
    };
    if (width > 10) return untouched();
    const auto header = line();
    if (header.status != 0) return untouched(); // No sscanf, no width assignment.
    KeywordReader fields(header.bytes);
    const auto rows = fields.selector_integer();
    result.header_conversions = rows.conversions;
    if (rows.conversions == 1) {
        result.declared_rows = rows.values[0];
        const auto parsed_width = fields.selector_integer();
        if (parsed_width.conversions == 1) {
            ++result.header_conversions;
            result.explicit_width = parsed_width.values[0];
            for (int index = 0; index < 10; ++index) {
                const auto value = fields.stream_real();
                if (value.conversions != 1) break;
                ++result.header_conversions;
                result.default_values.push_back(value.values[0]);
            }
        }
    }
    width = result.explicit_width; // Native store precedes conversion-count test.
    if (result.header_conversions < 1) return untouched();
    if (row_count < 0 || width < 0 ||
        (width && row_count > std::numeric_limits<int>::max() / width))
        throw std::out_of_range("mode2 effective extent outside compatibility domain");
    const auto extent = static_cast<std::size_t>(row_count) * width;
    if (destination.size() < extent)
        throw std::out_of_range("mode2 effective width exceeds owned destination");
    if (!result.default_values.empty()) {
        result.default_assigned = true;
        result.default_value = result.default_values.front();
        const bool per_column = result.default_values.size() == static_cast<std::size_t>(width);
        for (std::size_t index = 0; index < extent; ++index)
            destination[index] = per_column ? result.default_values[index % width] : result.default_value;
    }
    auto records = real_rows_impl(destination.first(extent), result.declared_rows, row_count, width, width == 0);
    records.header_conversions = result.header_conversions;
    records.explicit_width = result.explicit_width;
    records.default_values = std::move(result.default_values);
    records.default_value = result.default_value;
    records.default_assigned = result.default_assigned;
    if (records.default_assigned) std::fill(records.written.begin(), records.written.end(), true);
    return records;
}

KeywordReader::IntegerColumnRead KeywordReader::integer_columns(std::span<int> destination,
    int element_count, int width) {
    require_c_locale();
    if (element_count < 0 || width < 0 ||
        (width != 0 && element_count > std::numeric_limits<int>::max() / width))
        throw std::out_of_range("integer-column extent outside compatibility domain");
    const auto extent = static_cast<std::size_t>(element_count) * width;
    if (destination.size() < extent)
        throw std::out_of_range("integer-column header extent exceeds owned destination");
    IntegerColumnRead result;
    result.written.assign(extent, false);
    const auto header = line();
    if (header.status != 0) return result;
    KeywordReader fields(header.bytes); // No comma conversion and no blank skipping.
    const auto rows = fields.selector_integer();
    result.header_conversions = rows.conversions;
    if (rows.conversions == -1) return result;
    if (rows.conversions == 0)
        throw std::runtime_error("integer-column row count is uninitialized after header matching failure");
    result.declared_rows = rows.values[0];
    const auto value = fields.selector_integer();
    if (value.conversions == 1) {
        result.header_conversions = 2;
        result.default_assigned = true;
        result.default_value = value.values[0];
        std::fill_n(destination.begin(), extent, result.default_value);
        std::fill(result.written.begin(), result.written.end(), true);
    }
    if (result.declared_rows < 1) {
        result.status = 0;
        return result;
    }
    result.row_buffer.assign(static_cast<std::size_t>(width), 0);
    for (int row = 0; row < result.declared_rows; ++row) {
        const auto identifier = selector_integer();
        if (identifier.conversions == -1) return result;
        if (identifier.conversions == 1) {
            result.row_id = identifier.values[0];
            result.row_id_initialized = true;
        } else if (!result.row_id_initialized) {
            throw std::runtime_error("first integer-column row ID is uninitialized after matching failure");
        }
        for (int& retained : result.row_buffer) {
            const auto item = selector_integer();
            if (item.conversions == -1) return result;
            if (item.conversions == 1) retained = item.values[0];
        }
        if (result.row_id > 0 && result.row_id <= element_count) {
            const auto offset = static_cast<std::size_t>(result.row_id - 1) * width;
            std::copy(result.row_buffer.begin(), result.row_buffer.end(), destination.begin() + offset);
            std::fill_n(result.written.begin() + offset, width, true);
        }
        ++result.completed_rows;
    }
    if (line().status != 0) return result;
    result.status = 0;
    return result;
}

KeywordReader::FloatRead KeywordReader::reals(std::size_t count) {
    require_c_locale();
    FloatRead result;
    result.count = count;
    auto input = line();
    if (input.status != 0) return result;
    replace_commas(input.bytes);
    result.conversions = decimal_conversions(std::string_view(input.bytes.c_str()),
        std::span<double>(result.values).first(count));
    result.status = result.conversions == -1 ? 1 : 0;
    return result;
}

KeywordReader::CoordinateRead KeywordReader::coordinates(std::size_t dimensions) {
    require_c_locale();
    CoordinateRead result;
    result.dimensions = dimensions;
    auto input = line();
    if (input.status != 0) return result;
    replace_commas(input.bytes);
    // Preserve the integer label's established %i behavior and exact consumed
    // prefix, then use the legacy decimal lexer for each floating conversion.
    // A whitespace-token split would incorrectly accept data after suffixes.
    int consumed = 0;
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4996)
#endif
    result.conversions = std::sscanf(input.bytes.c_str(), "%i%n", &result.label, &consumed);
#ifdef _MSC_VER
#pragma warning(pop)
#endif
    if (result.conversions == 1) {
        std::size_t cursor = static_cast<std::size_t>(consumed);
        const std::string_view text(input.bytes.c_str());
        for (std::size_t i = 0; i < dimensions; ++i) {
            if (!decimal(text, cursor, result.values[i])) break;
            ++result.conversions;
        }
    }
    result.status = result.conversions == EOF ? 1 : 0;
    return result;
}

KeywordReader::MatrixRowRead KeywordReader::matrix_row() {
    require_c_locale();
    MatrixRowRead result;
    auto input = line();
    if (input.status != 0) return result;
    replace_commas(input.bytes);
    int consumed = 0;
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4996)
#endif
    result.conversions = std::sscanf(input.bytes.c_str(), "%i%n", &result.label, &consumed);
#ifdef _MSC_VER
#pragma warning(pop)
#endif
    if (result.conversions == 1) {
        std::size_t cursor = static_cast<std::size_t>(consumed);
        const std::string_view text(input.bytes.c_str());
        for (auto& value : result.values) {
            if (!decimal(text, cursor, value)) break;
            ++result.conversions;
        }
    }
    result.status = result.conversions == EOF ? 1 : 0;
    return result;
}

KeywordReader::TextRead KeywordReader::name(std::string_view previous_name) {
    return name_with_capacity(previous_name, 81);
}

KeywordReader::TextRead KeywordReader::material_name(std::string_view previous_name) {
    return name_with_capacity(previous_name, 65);
}

KeywordReader::TextRead KeywordReader::name_with_capacity(
    std::string_view previous_name, std::size_t capacity) {
    // 413ab0: discard remainder, read name, then strncpy/trim capacity-1.
    TextRead result{std::string(previous_name), 1};
    if (line().status != 0) return result;
    auto input = line();
    if (input.status != 0) return result;
    const auto end = input.bytes.find_first_of(std::string_view("\0\n", 2));
    if (end != std::string::npos) input.bytes.resize(end);
    if (input.bytes.size() >= capacity) input.bytes.resize(capacity - 1);
    while (!input.bytes.empty()) {
        const char last = input.bytes.back();
        if (last != '\0' && last != ' ' && last != '\r' && last != '\n') break;
        input.bytes.pop_back();
    }
    result.bytes = std::move(input.bytes);
    result.status = 0;
    return result;
}

} // namespace deform::recovered
