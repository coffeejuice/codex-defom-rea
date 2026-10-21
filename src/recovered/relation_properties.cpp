#include "relation_properties.hpp"

#include <algorithm>
#include <bit>
#include <cmath>
#include <limits>
#include <span>
#include <stdexcept>

namespace deform::recovered {
namespace {
int integer_from_real(double value) {
    const double integral = std::trunc(value);
    if (!std::isfinite(value) || integral < std::numeric_limits<int>::min() ||
        integral > std::numeric_limits<int>::max())
        throw std::out_of_range("relation property conversion outside finite signed32 domain");
    return static_cast<int>(integral);
}

int payload(KeywordReader& reader, PropertyTable& table) {
    return read_property_values(reader, table) == 0 ? 0 : 2;
}

// The aggregate native fscanf stops at the first failed assignment. An EOF
// after an earlier conversion is a successful partial aggregate, not -1.
int coefficient_stream(KeywordReader& reader, std::span<double> destination) {
    int conversions = 0;
    for (auto& value : destination) {
        const auto read = reader.stream_real(value);
        if (read.conversions != 1)
            return read.conversions == -1 && conversions == 0 ? 2 : 0;
        value = read.values[0];
        ++conversions;
    }
    return 0;
}

int count_triple_and_payload(KeywordReader& reader, PropertyTable& table) {
    const auto counts = reader.integer_triple();
    // This call writes the live count destinations, including partial failure.
    std::copy_n(counts.values.begin(), 3, table.counts_0x1c.begin());
    return counts.status == 0 ? payload(reader, table) : 2;
}

void select_two_axis_profile(PropertyTable& table, int selector,
    double first, double second, int second_axis) {
    switch (selector) {
    case 0:
        table.rank_0x04 = 0;
        table.constant_0x08 = first;
        break;
    case 1: case 2:
        table.rank_0x04 = 1;
        table.selectors_0x10[0] = selector == 1 ? 3 : second_axis;
        table.counts_0x1c[0] = integer_from_real(first);
        break;
    case 3:
        table.rank_0x04 = 2;
        table.selectors_0x10[0] = 3;
        table.selectors_0x10[1] = second_axis;
        table.counts_0x1c[0] = integer_from_real(first);
        table.counts_0x1c[1] = integer_from_real(second);
        break;
    default: break; // Native preserves whatever the preceding reset left.
    }
}

int tttd(RelationRecord& relation, KeywordReader& reader) {
    const auto header = reader.integer_pair();
    if (header.status != 0) return 2;
    const int mode = header.values[0];
    if (header.values[1] >= 0 && header.values[1] <= 2)
        relation.field_0x10 = header.values[1];
    if (mode == 1) {
        const auto options = reader.integer_triple();
        if (options.status != 0) return 2;
        const auto coefficients = reader.real_pair();
        if (coefficients.status != 0) return 2;
        auto& table = relation.property_tables.at(0x78);
        if (options.values[0] >= 0 && options.values[0] <= 3) {
            relation.field_0x14 = options.values[0];
            table.reset_type(0x2c + options.values[0]);
        }
        relation.field_0x6c = options.values[1];
        table.values_per_point_0x40 = options.values[1];
        relation.field_0xf0 = options.values[2];
        if (relation.field_0x6c == 1) {
            relation.field_0x70 = coefficients.values[0];
            relation.field_0x170 = coefficients.values[1];
        } else {
            relation.field_0x170 = coefficients.values[0];
            relation.field_0x178 = coefficients.values[1];
        }
        return count_triple_and_payload(reader, table);
    }
    if (mode == 2) {
        relation.field_0x14 = 4;
        const auto counts = reader.connectivity3();
        if (counts.status != 0) return 2;
        auto& table = relation.property_tables.at(0x1f8);
        table.reset_type(0x30);
        std::copy_n(counts.values.begin() + 1, 3, table.counts_0x1c.begin());
        if (counts.values[0] != 2) return 3;
        table.values_per_point_0x40 = 2;
        return payload(reader, table);
    }
    if (mode == 3) {
        relation.field_0x14 = 5;
        return coefficient_stream(reader, relation.coefficients_0x270);
    }
    if (mode == 4) {
        relation.field_0x14 = 6;
        const auto coefficients = read_relation_header(reader, RelationHeaderKind::four_reals);
        std::copy_n(coefficients.reals.begin(), 4, relation.coefficients_0x340.begin());
        if (coefficients.status != 0) return 2;
        const auto count = reader.integer();
        if (count.status != 0) return 2;
        auto& table = relation.property_tables.at(0x2c8);
        table.reset_type(0x32);
        table.counts_0x1c[0] = count.values[0];
        return payload(reader, table);
    }
    if (mode == 5) {
        relation.field_0x14 = 7;
        return coefficient_stream(reader, relation.coefficients_0x360);
    }
    if (mode == 6) {
        relation.field_0x14 = 8;
        const auto coefficients = read_relation_header(reader, RelationHeaderKind::four_reals);
        std::copy_n(coefficients.reals.begin(), 4, relation.coefficients_0x390.begin());
        return coefficients.status == 0 ? 0 : 2;
    }
    if (mode == 7) {
        relation.field_0x14 = 9;
        return coefficient_stream(reader, relation.coefficients_0x3b0);
    }
    if (mode == 8) {
        relation.field_0x14 = 10;
        const auto count = reader.integer();
        if (count.status != 0) return 2;
        auto& table = relation.property_tables.at(0x3f8);
        table.reset_type(0x36);
        table.counts_0x1c[0] = count.values[0];
        return payload(reader, table);
    }
    if (mode == 9) {
        const auto options = reader.integer_pair();
        if (options.status != 0) return 2;
        const auto coefficients = reader.real_pair();
        if (coefficients.status != 0) return 2;
        relation.field_0x14 = 11;
        auto& table = relation.property_tables.at(0xf8);
        table.reset_type(0x2c);
        relation.field_0x6c = options.values[1];
        table.values_per_point_0x40 = options.values[1];
        if (relation.field_0x6c == 1) {
            relation.field_0x70 = coefficients.values[0];
            relation.field_0x170 = coefficients.values[1];
        } else {
            relation.field_0x3e8 = coefficients.values[0];
            relation.field_0x3f0 = coefficients.values[1];
        }
        return count_triple_and_payload(reader, table);
    }
    if (mode >= 10 && mode <= 13) {
        relation.field_0x14 = mode + 2;
        const auto width = reader.integer();
        if (width.status != 0) return 2;
        auto& table = relation.property_tables.at(0x180);
        table.values_per_point_0x40 = width.values[0];
        // No reset: width<=1 leaves existing dependent storage untouched.
        return payload(reader, table);
    }
    if (mode < 0) {
        relation.field_0x14 = 16;
        // 484ab8 uses 32-bit NEG; INT_MIN stays INT_MIN without C++ overflow.
        relation.field_0x470 = std::bit_cast<std::int32_t>(
            std::uint32_t{0} - std::bit_cast<std::uint32_t>(mode));
    }
    return 0;
}
} // namespace

RelationHeaderRead read_relation_header(KeywordReader& reader, RelationHeaderKind kind) {
    RelationHeaderRead result;
    switch (kind) {
    case RelationHeaderKind::integer_two_reals: result.integer_count = 1; result.real_count = 2; break;
    case RelationHeaderKind::two_integers_two_reals: result.integer_count = 2; result.real_count = 2; break;
    case RelationHeaderKind::four_reals: result.real_count = 4; break;
    case RelationHeaderKind::five_reals: result.real_count = 5; break;
    case RelationHeaderKind::seven_reals: result.real_count = 7; break;
    }
    auto line = reader.line();
    if (line.status != 0) return result;
    std::replace(line.bytes.begin(), line.bytes.end(), ',', ' ');
    KeywordReader fields(std::string_view(line.bytes.c_str()));
    result.conversions = 0;
    const auto failed = [&result](int conversions) {
        if (conversions == -1 && result.conversions == 0) result.conversions = -1;
        result.status = result.conversions == -1 ? 1 : 0;
    };
    for (int index = 0; index < result.integer_count; ++index) {
        const auto read = fields.selector_integer();
        if (read.conversions != 1) { failed(read.conversions); return result; }
        result.integers[index] = read.values[0];
        ++result.conversions;
    }
    for (int index = 0; index < result.real_count; ++index) {
        const auto read = fields.stream_real();
        if (read.conversions != 1) { failed(read.conversions); return result; }
        result.reals[index] = read.values[0];
        ++result.conversions;
    }
    result.status = 0;
    return result;
}

int read_relation_property(int keyword, RelationRecord& relation, KeywordReader& reader) {
    if (keyword == 135) return tttd(relation, reader);
    if (keyword == 136) {
        const auto read = reader.name(relation.name);
        relation.name = read.bytes;
        return read.status == 0 ? 0 : 2;
    }
    if (keyword == 143) {
        const auto read = reader.integer();
        relation.field_0x768 = read.values[0];
        return read.status == 0 ? 0 : 2;
    }
    if (keyword == 139) {
        const auto header = read_relation_header(reader, RelationHeaderKind::two_integers_two_reals);
        if (header.status != 0) return 2;
        relation.field_0x5e4 = header.integers[0] == 0 ? 0 : 1;
        if (relation.field_0x5e4 == 0) return 0;
        auto& table = relation.property_tables.at(0x5e8);
        // TRNSFP intentionally retains prior table state before selection.
        select_two_axis_profile(table, header.integers[1], header.reals[0], header.reals[1], 12);
        return payload(reader, table);
    }
    if (keyword == 141) {
        const auto header = read_relation_header(reader, RelationHeaderKind::five_reals);
        // The first two helper outputs are live relation destinations even on EOF.
        relation.coefficients_0x668[0] = header.reals[0];
        relation.coefficients_0x668[1] = header.reals[1];
        if (header.status != 0) return 2;
        relation.field_0x660 = 1;
        const int selector = integer_from_real(header.reals[2]);
        auto& table = relation.property_tables.at(0x678);
        table.reset_type(0x6d);
        if (selector == 0) {
            table.rank_0x04 = 0;
            table.constant_0x08 = header.reals[3];
        } else if (selector == 1) {
            table.rank_0x04 = 1;
            table.selectors_0x10[0] = 3;
            table.counts_0x1c[0] = integer_from_real(header.reals[3]);
        }
        return payload(reader, table);
    }
    if (keyword == 137 || keyword == 138 || keyword == 140 || keyword == 142) {
        const auto header = read_relation_header(reader, RelationHeaderKind::integer_two_reals);
        if (header.status != 0) return 2;
        const int offset = keyword == 137 ? 0x478 : keyword == 138 ? 0x4f0 : keyword == 140 ? 0x568 : 0x6f0;
        const int type = keyword == 137 ? 0x1f : keyword == 138 ? 0x20 : keyword == 140 ? 0x6c : 0x6e;
        auto& table = relation.property_tables.at(offset);
        table.reset_type(type);
        int selector = header.integers[0];
        if (keyword == 140) {
            relation.field_0x5e0 = selector;
            if (selector >= 4 && selector <= 7) selector -= 4;
        }
        select_two_axis_profile(table, selector, header.reals[0], header.reals[1], keyword == 142 ? 23 : 12);
        return payload(reader, table);
    }
    throw std::logic_error("unimplemented relation property reached dispatch");
}

} // namespace deform::recovered
