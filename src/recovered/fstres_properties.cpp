#include "fstres_properties.hpp"
#include "relation_properties.hpp"

#include <algorithm>
#include <bit>
#include <cmath>
#include <limits>
#include <span>
#include <stdexcept>

namespace deform::recovered {
namespace {
int payload(KeywordReader& reader, PropertyTable& table) {
    return read_property_values(reader, table) == 0 ? 0 : 2;
}

int coefficients(KeywordReader& reader, RelationHeaderKind kind,
    std::span<double> destination) {
    const auto read = read_relation_header(reader, kind);
    // Live native destinations are zeroed even on EOF. Mode10 intentionally
    // consumes seven outputs while persisting only the first six.
    std::copy_n(read.reals.begin(), destination.size(), destination.begin());
    return read.status == 0 ? 0 : 2;
}

std::int32_t truncate_integer(double value) {
    const auto integral = std::trunc(value);
    if (!std::isfinite(value) || integral < std::numeric_limits<std::int32_t>::min() ||
        integral > std::numeric_limits<std::int32_t>::max())
        throw std::out_of_range("FSTRES table extent outside finite signed32 conversion domain");
    return static_cast<std::int32_t>(integral);
}

std::int32_t checked_extent(std::int64_t value) {
    if (value < std::numeric_limits<std::int32_t>::min() ||
        value > std::numeric_limits<std::int32_t>::max())
        throw std::out_of_range("FSTRES derived extent overflows signed32 domain");
    return static_cast<std::int32_t>(value);
}

int selected_table(KeywordReader& reader, PropertyTable& table, int type,
    int selector, int second_axis) {
    const auto header = reader.real_pair();
    if (header.status != 0) return 2; // Local outputs; prior table untouched.
    table.reset_type(type);
    switch (selector) {
    case 0:
        table.rank_0x04 = 0;
        table.constant_0x08 = header.values[0];
        break;
    case 1: case 2:
        table.rank_0x04 = 1;
        table.selectors_0x10[0] = selector == 1 ? 3 : second_axis;
        table.counts_0x1c[0] = truncate_integer(header.values[0]);
        break;
    case 3:
        table.rank_0x04 = 2;
        table.selectors_0x10[0] = 3;
        table.selectors_0x10[1] = second_axis;
        table.counts_0x1c[0] = truncate_integer(header.values[0]);
        table.counts_0x1c[1] = truncate_integer(header.values[1]);
        break;
    default: break; // Unknown selector keeps the typed reset profile.
    }
    return payload(reader, table);
}

int linked_tables(FstresProperties& properties, KeywordReader& reader) {
    const auto count = reader.integer();
    properties.field_0x478 = count.values[0];
    if (count.status != 0) return 2;
    auto& source = properties.table_0x480;
    source.reset_type(0xa6);
    source.counts_0x1c[0] = properties.field_0x478;
    if (payload(reader, source) != 0) return 2;
    std::int32_t sum = 0, product = 1;
    for (const double coordinate : source.axes[0]) {
        const auto integer = truncate_integer(coordinate);
        sum = checked_extent(static_cast<std::int64_t>(sum) + integer);
        product = checked_extent(static_cast<std::int64_t>(product) * integer);
    }
    auto& summed = properties.table_0x4f8;
    summed.reset_type(0xa7);
    summed.values_per_point_0x40 = sum;
    if (payload(reader, summed) != 0) return 2;
    auto& multiplied = properties.table_0x570;
    multiplied.reset_type(0xa8);
    multiplied.values_per_point_0x40 = product;
    return payload(reader, multiplied);
}
} // namespace

int read_fstres(FstresProperties& properties, KeywordReader& reader) {
    const auto header = reader.integer_triple();
    if (header.status != 0) return 2;
    const auto mode = header.values[0];
    switch (mode) {
    case 1:
        properties.mode_0x50 = 0;
        return coefficients(reader, RelationHeaderKind::four_reals, properties.coefficients_0x58);
    case 2: case 3: {
        properties.mode_0x50 = 1;
        auto& table = properties.table_0x78;
        table.reset_type(0x4f);
        const auto counts = reader.integer_triple();
        std::copy_n(counts.values.begin(), 3, table.counts_0x1c.begin());
        if (counts.status != 0) return 2;
        const auto status = payload(reader, table);
        table.flag_0x50 = mode == 2 ? 1 : 0; // Also after payload failure.
        return status;
    }
    case 4:
        properties.mode_0x50 = 2;
        return coefficients(reader, RelationHeaderKind::five_reals, properties.coefficients_0xf0);
    case 5:
        properties.mode_0x50 = 3;
        return coefficients(reader, RelationHeaderKind::four_reals, properties.coefficients_0x118);
    case 6:
        properties.mode_0x50 = 4;
        if (selected_table(reader, properties.table_0x1b0, 0x24, header.values[1], 12) != 0)
            return 2;
        return selected_table(reader, properties.table_0x138, 0x25, header.values[2], 12);
    case 7: case 8: {
        properties.mode_0x50 = 5;
        auto& table = properties.table_0x228;
        table.reset_type(0x50);
        const auto counts = reader.integer_triple();
        std::copy_n(counts.values.begin(), 3, table.counts_0x1c.begin());
        if (counts.status != 0) return 2;
        const auto status = payload(reader, table);
        table.flag_0x50 = mode == 7 ? 1 : 0;
        return status;
    }
    case 9:
        properties.mode_0x50 = 6;
        (void)coefficients(reader, RelationHeaderKind::seven_reals, properties.coefficients_0x2a0);
        return coefficients(reader, RelationHeaderKind::seven_reals, properties.coefficients_0x2d8);
    case 10:
        properties.mode_0x50 = 7;
        return coefficients(reader, RelationHeaderKind::seven_reals, properties.coefficients_0x310);
    case 11:
        properties.mode_0x50 = 8;
        return coefficients(reader, RelationHeaderKind::five_reals, properties.coefficients_0x340);
    case 12:
        properties.mode_0x50 = 9;
        return selected_table(reader, properties.table_0x400, 0x26, header.values[1], 8);
    case 13: case 15:
        properties.mode_0x50 = mode == 13 ? 10 : 12;
        return linked_tables(properties, reader);
    case 14: {
        properties.mode_0x50 = 11;
        (void)coefficients(reader, RelationHeaderKind::four_reals, properties.coefficients_0x368);
        const auto table = read_property(reader, properties.table_0x388, 0, nullptr);
        return table.status == 0 ? 0 : 2;
    }
    case 16: {
        properties.mode_0x50 = 13;
        auto& table = properties.table_0x5e8;
        table.reset_type(0x51);
        const auto width = reader.integer();
        table.values_per_point_0x40 = width.values[0];
        if (width.status != 0) return 2;
        const auto counts = reader.integer_triple();
        std::copy_n(counts.values.begin(), 3, table.counts_0x1c.begin());
        if (counts.status != 0) return 2;
        const auto status = payload(reader, table);
        table.flag_0x50 = 0;
        return status;
    }
    default:
        if (mode < 0) {
            properties.mode_0x50 = 14;
            properties.field_0x660 = std::bit_cast<std::int32_t>(
                std::uint32_t{0} - std::bit_cast<std::uint32_t>(mode));
        } else if (mode == 0) properties.mode_0x50 = -1;
        return 0;
    }
}

} // namespace deform::recovered
