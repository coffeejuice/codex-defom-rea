#include "recovered/keyword_reader.hpp"
#include "recovered/property_table.hpp"
#include "recovered/relation_properties.hpp"

#include <cmath>
#include <bit>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

namespace {

unsigned nibble(char value) {
    if (value >= '0' && value <= '9') return static_cast<unsigned>(value - '0');
    if (value >= 'a' && value <= 'f') return static_cast<unsigned>(value - 'a' + 10);
    if (value >= 'A' && value <= 'F') return static_cast<unsigned>(value - 'A' + 10);
    throw std::runtime_error("invalid input hex");
}

std::string unhex(std::string_view text) {
    if (text == "-") return {};
    if (text.size() % 2 != 0) throw std::runtime_error("odd input hex length");
    std::string bytes;
    bytes.reserve(text.size() / 2);
    for (std::size_t i = 0; i < text.size(); i += 2)
        bytes.push_back(static_cast<char>((nibble(text[i]) << 4) | nibble(text[i + 1])));
    return bytes;
}

std::string hex(std::string_view bytes) {
    constexpr char digits[] = "0123456789abcdef";
    std::string result;
    result.reserve(bytes.size() * 2);
    for (const unsigned char byte : bytes) {
        result.push_back(digits[byte >> 4]);
        result.push_back(digits[byte & 15]);
    }
    return result;
}

void replay(std::string_view command) {
    std::istringstream arguments{std::string(command)};
    std::string kind, encoded, previous, extra;
    int skip = 0;
    std::size_t offset = 0;
    if (!(arguments >> kind >> skip >> offset >> encoded >> previous) ||
        (arguments >> extra) || (skip != 0 && skip != 1))
        throw std::runtime_error("expected KIND SKIP OFFSET INPUT_HEX PREVIOUS_NAME_HEX");
    const auto input = unhex(encoded);
    deform::recovered::KeywordReader reader(input, offset);
    int status = 0;
    std::ostringstream fields;
    fields << std::setprecision(17);
    if (kind == "relation1i2r" || kind == "relation2i2r" || kind == "relation4r" || kind == "relation5r" || kind == "relation7r") {
        if (skip != 0 || previous != "-")
            throw std::runtime_error("relation headers require SKIP0 and no prior state");
        using deform::recovered::RelationHeaderKind;
        const auto header_kind = kind == "relation1i2r" ? RelationHeaderKind::integer_two_reals
            : kind == "relation2i2r" ? RelationHeaderKind::two_integers_two_reals
            : kind == "relation4r" ? RelationHeaderKind::four_reals
            : kind == "relation5r" ? RelationHeaderKind::five_reals : RelationHeaderKind::seven_reals;
        const auto result = deform::recovered::read_relation_header(reader, header_kind);
        status = result.status;
        fields << ",\"conversions\":" << result.conversions << ",\"integers\":[";
        for (int index = 0; index < result.integer_count; ++index) {
            if (index) fields << ',';
            fields << result.integers[index];
        }
        fields << "],\"reals\":[";
        for (int index = 0; index < result.real_count; ++index) {
            if (index) fields << ',';
            fields << result.reals[index];
        }
        fields << ']';
    } else if (kind == "propertyheader") {
        if (skip != 0 || previous != "-")
            throw std::runtime_error("propertyheader requires SKIP0 and no prior state");
        const auto result = deform::recovered::read_property_header(reader);
        status = result.status;
        fields << ",\"conversions\":" << result.conversions
               << ",\"selector\":" << result.selector << ",\"values\":[";
        for (std::size_t index = 0; index < result.values.size(); ++index) {
            if (index) fields << ',';
            fields << result.values[index];
        }
        fields << ']';
    } else if (kind == "integercolumns") {
        std::istringstream dimensions(previous);
        int elements = 0, width = 0;
        char first_separator = 0, second_separator = 0;
        std::string encoded_values;
        if (!(dimensions >> elements >> first_separator >> width >> second_separator >> encoded_values) ||
            first_separator != ':' || second_separator != ':' || (dimensions >> extra) || skip != 0 ||
            elements < 0 || width < 0 || (width && elements > std::numeric_limits<int>::max() / width))
            throw std::runtime_error("integercolumns requires SKIP0 and ELEMENTS:WIDTH:INT32_HEX parameters");
        const auto extent = static_cast<std::size_t>(elements) * width;
        std::vector<int> values(extent);
        if (encoded_values != "-") {
            const auto raw = unhex(encoded_values);
            if (raw.size() != extent * 4)
                throw std::runtime_error("integercolumns initial values extent mismatch");
            for (std::size_t i = 0; i < extent; ++i) {
                std::uint32_t bits = 0;
                for (unsigned byte = 0; byte < 4; ++byte)
                    bits |= static_cast<std::uint32_t>(static_cast<unsigned char>(raw[i * 4 + byte])) << (byte * 8);
                values[i] = std::bit_cast<std::int32_t>(bits);
            }
        }
        const auto result = reader.integer_columns(values, elements, width);
        status = result.status;
        fields << ",\"conversions\":" << result.header_conversions
               << ",\"declared_rows\":" << result.declared_rows
               << ",\"default_value\":" << result.default_value
               << ",\"default_assigned\":" << (result.default_assigned ? "true" : "false")
               << ",\"completed_rows\":" << result.completed_rows
               << ",\"row_id_initialized\":" << (result.row_id_initialized ? "true" : "false")
               << ",\"row_id\":";
        if (result.row_id_initialized) fields << result.row_id;
        else fields << "null";
        const auto array = [&fields](std::string_view key, const auto& items) {
            fields << ",\"" << key << "\":[";
            for (std::size_t i = 0; i < items.size(); ++i) {
                if (i) fields << ',';
                fields << items[i];
            }
            fields << ']';
        };
        array("values", values);
        array("row_buffer", result.row_buffer);
        fields << ",\"written\":[";
        for (std::size_t i = 0; i < result.written.size(); ++i) {
            if (i) fields << ',';
            fields << (result.written[i] ? "true" : "false");
        }
        fields << ']';
    } else if (kind == "materialcolumns" || kind == "realcolumns" || kind == "realrows" ||
               kind == "explicitrows" || kind == "mode2columns") {
        std::istringstream dimensions(previous);
        int elements = 0, width = 0, mode = 1;
        char separator = 0;
        if (!(dimensions >> elements >> separator >> width) || separator != ':' || skip != 0 ||
            elements < 0 || width < 0 || (width && elements > std::numeric_limits<int>::max() / width))
            throw std::runtime_error("column replay requires SKIP0 and bounded ELEMENTS:WIDTH parameters");
        std::vector<double> initial_values;
        if (kind != "materialcolumns") {
            char mode_separator = 0, seed_separator = 0;
            std::string seed;
            if (kind == "mode2columns") {
                if (!(dimensions >> seed_separator >> seed) || seed_separator != ':')
                    throw std::runtime_error("mode2columns requires ROWS:INITIAL_WIDTH:DOUBLE_HEX");
            } else if (!(dimensions >> mode_separator >> mode >> seed_separator >> seed) ||
                mode_separator != ':' || seed_separator != ':')
                throw std::runtime_error("realcolumns/realrows/explicitrows require ROWS:WIDTH:MODE_OR_DECLARED:DOUBLE_HEX");
            const auto raw = unhex(seed);
            const auto extent = static_cast<std::size_t>(elements) * width;
            if (raw.size() % 8 != 0 || (kind == "realcolumns" && raw.size() != extent * 8))
                throw std::runtime_error("column initial binary64 extent mismatch");
            initial_values.resize(raw.size() / 8);
            for (std::size_t index = 0; index < initial_values.size(); ++index) {
                std::uint64_t bits = 0;
                for (unsigned byte = 0; byte < 8; ++byte)
                    bits |= static_cast<std::uint64_t>(static_cast<unsigned char>(raw[index * 8 + byte])) << (byte * 8);
                initial_values[index] = std::bit_cast<double>(bits);
                if (!std::isfinite(initial_values[index]))
                    throw std::runtime_error("nonfinite realcolumns initial destination");
            }
        }
        if (dimensions >> extra) throw std::runtime_error("unexpected column replay parameters");
        const auto result = kind == "realcolumns" ? reader.real_columns(initial_values, elements, width, mode)
            : kind == "realrows" ? reader.real_rows(initial_values, mode, elements, width)
            : kind == "explicitrows" ? reader.real_rows_explicit(initial_values, mode, elements, width)
            : kind == "mode2columns" ? reader.real_columns_mode2(initial_values, elements, width)
            : reader.material_columns(elements, width);
        status = result.status;
        fields << ",\"conversions\":" << result.header_conversions
               << ",\"declared_rows\":" << result.declared_rows
               << ",\"explicit_width\":" << result.explicit_width
               << ",\"default_value\":" << result.default_value
               << ",\"default_assigned\":" << (result.default_assigned ? "true" : "false")
               << ",\"completed_rows\":" << result.completed_rows
               << ",\"row_id_initialized\":" << (result.row_id_initialized ? "true" : "false")
               << ",\"row_id\":";
        if (result.row_id_initialized) fields << result.row_id;
        else fields << "null";
        const auto array = [&fields](std::string_view key, const auto& values) {
            fields << ",\"" << key << "\":[";
            for (std::size_t i = 0; i < values.size(); ++i) {
                if (i) fields << ',';
                if (!std::isfinite(values[i]))
                    throw std::runtime_error("nonfinite material-column replay value");
                fields << values[i];
            }
            fields << ']';
        };
        array("values", result.values);
        array("row_buffer", result.row_buffer);
        if (kind != "materialcolumns") {
            array("default_values", result.default_values);
            fields << ",\"written\":[";
            for (std::size_t index = 0; index < result.written.size(); ++index) {
                if (index) fields << ',';
                fields << (result.written[index] ? "true" : "false");
            }
            fields << ']';
            // Raw output makes signed zero testable without JSON numeric coercion.
            std::string raw;
            for (const double value : result.values) {
                const auto bits = std::bit_cast<std::uint64_t>(value);
                for (unsigned byte = 0; byte < 8; ++byte)
                    raw.push_back(static_cast<char>((bits >> (byte * 8)) & 255));
            }
            fields << ",\"values_hex\":\"" << hex(raw) << '\"';
            if (kind == "realrows" || kind == "explicitrows" || kind == "mode2columns") {
                raw.clear();
                for (const double value : initial_values) {
                    const auto bits = std::bit_cast<std::uint64_t>(value);
                    for (unsigned byte = 0; byte < 8; ++byte)
                        raw.push_back(static_cast<char>((bits >> (byte * 8)) & 255));
                }
                fields << ",\"storage_hex\":\"" << hex(raw) << '\"'
                       << ",\"width_after\":" << width;
            }

        }

    } else if (kind == "token" || kind == "name" || kind == "materialname") {
        const auto result = kind == "token" ? reader.token()
            : kind == "materialname" ? reader.material_name(unhex(previous))
            : reader.name(unhex(previous));
        status = result.status;
        fields << ",\"conversions\":null,\"values\":[],\""
               << (kind == "token" ? "token_hex" : "name_hex")
               << "\":\"" << hex(result.bytes) << '"';
    } else if (kind == "real4") {
        const auto result = reader.real_quad();
        status = result.status;
        fields << ",\"conversions\":" << result.conversions << ",\"values\":[";
        std::string raw;
        for (std::size_t i = 0; i < result.count; ++i) {
            if (i) fields << ',';
            if (!std::isfinite(result.values[i]))
                throw std::runtime_error("nonfinite replay value outside compatibility domain");
            fields << result.values[i];
            const auto bits = std::bit_cast<std::uint64_t>(result.values[i]);
            for (std::size_t byte = 0; byte < 8; ++byte)
                raw.push_back(static_cast<char>((bits >> (byte * 8)) & 0xff));
        }
        fields << "],\"values_hex\":\"" << hex(raw) << '\"';
    } else if (kind == "real3") {
        const auto result = reader.real_triple();
        status = result.status;
        fields << ",\"conversions\":" << result.conversions << ",\"values\":[";
        for (std::size_t i = 0; i < result.count; ++i) {
            if (i) fields << ',';
            if (!std::isfinite(result.values[i]))
                throw std::runtime_error("nonfinite replay value outside compatibility domain");
            fields << result.values[i];
        }
        fields << ']';
    } else if (kind == "dynamicheader") {
        const auto result = reader.dynamic_array_header();
        status = result.status;
        fields << ",\"conversions\":null,\"token_count\":" << result.token_count
               << ",\"values\":[";
        for (std::size_t index = 0; index < result.integers.size(); ++index) {
            if (index) fields << ',';
            fields << result.integers[index];
        }
        fields << ',' << result.default_value << "],\"tokens_hex\":[";
        for (std::size_t index = 0; index < result.tokens.size(); ++index) {
            if (index) fields << ',';
            fields << '\"' << hex(result.tokens[index]) << '\"';
        }
        fields << ']';
    } else if (kind == "shapeheader") {
        const auto result = reader.shape_header();
        status = result.status;
        fields << ",\"conversions\":" << result.conversions << ",\"values\":["
               << result.rows << ',' << result.selector << ',' << result.width
               << ',' << result.default_value << ']';
    } else if (kind == "customheader") {
        const auto result = reader.custom_variable_header();
        status = result.status;
        fields << ",\"conversions\":" << result.conversions << ",\"values\":["
               << result.integers[0] << ',' << result.integers[1] << ','
               << result.integers[2] << ',' << result.default_value << ','
               << result.integers[3] << ']';
    } else if (kind == "intrealint") {
        const auto result = reader.integer_pair_real();
        status = result.status;
        fields << ",\"conversions\":" << result.conversions
               << ",\"values\":[" << result.integers[0] << ',' << result.real_value
               << ',' << result.integers[1] << ']';
    } else if (kind == "realint") {
        const auto result = reader.real_integer();
        status = result.status;
        if (!std::isfinite(result.real_value))
            throw std::runtime_error("nonfinite replay value outside compatibility domain");
        fields << ",\"conversions\":" << result.conversions
               << ",\"values\":[" << result.real_value << ',' << result.integer_value << ']';
    } else if (kind == "real1" || kind == "real2" || kind == "streamreal") {
        double prior = 0.0;
        if (kind == "streamreal" && previous != "-") {
            const auto raw = unhex(previous);
            if (raw.size() != 8)
                throw std::runtime_error("streamreal previous value must be eight little-endian bytes");
            std::uint64_t bits = 0;
            for (std::size_t i = 0; i < raw.size(); ++i)
                bits |= static_cast<std::uint64_t>(static_cast<unsigned char>(raw[i])) << (8 * i);
            prior = std::bit_cast<double>(bits);
        }
        const auto result = kind == "streamreal" ? reader.stream_real(prior)
            : kind == "real1" ? reader.real() : reader.real_pair();
        status = result.status;
        fields << ",\"conversions\":" << result.conversions << ",\"values\":[";
        for (std::size_t i = 0; i < result.count; ++i) {
            if (i) fields << ',';
            if (!std::isfinite(result.values[i]))
                throw std::runtime_error("nonfinite replay value outside compatibility domain");
            fields << result.values[i];
        }
        fields << ']';
    } else if (kind == "matrix4") {
        const auto result = reader.matrix_row();
        status = result.status;
        fields << ",\"conversions\":" << result.conversions << ",\"values\":[" << result.label;
        for (const auto value : result.values) {
            if (!std::isfinite(value))
                throw std::runtime_error("nonfinite replay value outside compatibility domain");
            fields << ',' << value;
        }
        fields << ']';
    } else if (kind == "coord2" || kind == "coord3") {
        const auto result = kind == "coord2" ? reader.coordinates2() : reader.coordinates3();
        status = result.status;
        fields << ",\"conversions\":" << result.conversions << ",\"values\":[" << result.label;
        for (std::size_t i = 0; i < result.dimensions; ++i) {
            if (!std::isfinite(result.values[i]))
                throw std::runtime_error("nonfinite replay value outside compatibility domain");
            fields << ',' << result.values[i];
        }
        fields << ']';
    } else {
        deform::recovered::KeywordReader::IntegerRead result;
        if (kind == "selector") result = reader.selector_integer();
        else if (kind == "selectorpair") result = reader.selector_pair();
        else if (kind == "int1") result = reader.integer(skip != 0);
        else if (kind == "int2") result = reader.integer_pair();
        else if (kind == "int3") result = reader.integer_triple();
        else if (kind == "int4") result = reader.connectivity3();
        else if (kind == "int5") result = reader.connectivity4(skip != 0);
        else if (kind == "int9") result = reader.connectivity8(skip != 0);
        else throw std::runtime_error("unknown reader kind");
        status = result.status;
        fields << ",\"conversions\":" << result.conversions << ",\"values\":[";
        for (std::size_t i = 0; i < result.count; ++i) {
            if (i) fields << ',';
            fields << result.values[i];
        }
        fields << ']';
    }
    std::cout << "{\"kind\":\"" << kind << "\",\"status\":" << status
              << ",\"cursor\":" << reader.cursor() << ",\"consumed_hex\":\""
              << hex(std::string_view(input).substr(offset, reader.cursor() - offset))
              << '"' << fields.str() << ",\"lines\":[";
    bool first = true;
    for (const auto& line : reader.line_reads()) {
        if (!first) std::cout << ',';
        first = false;
        std::cout << "{\"offset\":" << line.offset << ",\"cursor\":" << line.cursor
                  << ",\"hex\":\"" << hex(line.bytes) << "\",\"status\":" << line.status << '}';
    }
    std::cout << "]}\n";
}

} // namespace

// Independent requests, one line each:
// KIND SKIP OFFSET INPUT_HEX PREVIOUS_NAME_HEX
// KIND = token|selector|int1|int2|int3|int4|int5|int9|real1|real2|real3|real4|realint|intrealint|customheader|dynamicheader|streamreal|coord2|coord3|matrix4|name|materialname
// SKIP = 0|1 (only int1/int5/int9 use it); OFFSET = initial byte cursor;
// '-' means empty hex. Previous name is used only by name; streamreal instead
// uses that slot for eight little-endian binary64 bytes ('-' means +0).
// materialcolumns uses SKIP0 and ELEMENTS:INPUT_WIDTH in the final slot;
// output includes scratch values, retained row buffer and header/row metadata.
// integercolumns uses SKIP0 and ELEMENTS:WIDTH:INT32_HEX in the final slot;
// '-' for INT32_HEX explicitly seeds zero values. Output includes changed
// destination values and a written mask, even when status is failure.
// realcolumns uses SKIP0 and ROWS:WIDTH:MODE:DOUBLE_HEX (modes0/1/6).
// The seed is the exact initialized little-endian binary64 destination; only
// zero extent accepts empty hex. Result includes raw values_hex and written.
// shapeheader uses no extra parameters: shapeheader 0 OFFSET INPUT_HEX -.
// realrows/explicitrows use ROWS:WIDTH:DECLARED_ROWS:DOUBLE_HEX; mode2columns uses
// ROWS:INITIAL_WIDTH:DOUBLE_HEX. Their seeds may include retained physical tails;
// storage_hex reports the whole destination and width_after the referenced width.
// Each output is one JSON object, with byte-exact line-attempt observations.
// This protocol supplies test inputs; it does not parse a complete keyword file.
int main() {
    try {
        std::cout << std::setprecision(17);
        std::string command;
        while (std::getline(std::cin, command)) {
            if (command.empty()) continue;
            replay(command);
        }
        return 0;
    } catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
        return 1;
    }
}
