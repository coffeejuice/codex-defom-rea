#include "grndat_properties.hpp"
#include "relation_properties.hpp"

#include <algorithm>
#include <bit>
#include <cmath>
#include <iostream>
#include <limits>
#include <stdexcept>

namespace deform::recovered {
namespace {
int extent(double value) {
    const auto integral = std::trunc(value);
    if (!std::isfinite(value) || integral < std::numeric_limits<int>::min() ||
        integral > std::numeric_limits<int>::max())
        throw std::out_of_range("GRNDAT table extent outside finite signed32 conversion domain");
    return static_cast<int>(integral);
}

int record_table(KeywordReader& reader, PropertyTable& table) {
    // 41c070: no typed reset; selector starts -1, numeric locals start zero.
    auto line = reader.line();
    if (line.status == 0 && line.bytes == "\n") line = reader.line();
    if (line.status != 0) return 1;
    std::replace(line.bytes.begin(), line.bytes.end(), ',', ' ');
    KeywordReader fields(line.bytes);
    const auto read = fields.selector_integer();
    int selector = -1;
    std::array<double, 3> values{};
    if (read.conversions == 1) {
        selector = read.values[0];
        for (auto& value : values) {
            const auto r = fields.stream_real();
            if (r.conversions != 1) break;
            value = r.values[0];
        }
    }
    if (selector == 0) {
        table.rank_0x04 = 0; table.constant_0x08 = values[0];
    } else if (selector == 1 || selector == 2) {
        table.rank_0x04 = 1; table.selectors_0x10[0] = selector == 1 ? 3 : 23;
        table.counts_0x1c[0] = extent(values[0]);
    } else if (selector == 3) {
        table.rank_0x04 = 2; table.selectors_0x10[0] = 3; table.selectors_0x10[1] = 23;
        table.counts_0x1c[0] = extent(values[0]); table.counts_0x1c[1] = extent(values[1]);
    }
    return selector > 0 ? read_property_values(reader, table) : 0;
}

int record(KeywordReader& reader, GrndatRecord& destination) {
    const auto count = reader.selector_integer();
    if (count.conversions == -1) return 1;
    destination.count_0x00 = count.values[0];
    if (destination.count_0x00 > 12) {
        destination.count_0x00 = 12;
        std::cout << "Error - SubbdryVarSize is incorrect.\n";
    }
    for (int i = 0; i < destination.count_0x00; ++i) {
        const auto r = reader.stream_real(destination.inline_0x08[i]);
        destination.inline_0x08[i] = r.values[0];
        if (r.conversions == -1) return 1;
    }
    // Aggregate fscanf(%i %Flf): a failed first conversion leaves both old
    // fields; EOF after a successful int is a successful partial aggregate.
    const auto integer = reader.selector_integer();
    if (integer.conversions == -1) return 1;
    if (integer.conversions == 1) {
        destination.integer_0x70 = integer.values[0];
        destination.scalar_0x68 = reader.stream_real(destination.scalar_0x68).values[0];
    }
    if (!destination.skip_first_two_0x1e0) {
        if (record_table(reader, destination.tables[0]) != 0) return 1;
        if (record_table(reader, destination.tables[1]) != 0) return 1;
    }
    return record_table(reader, destination.tables[2]);
}
}

GrndatReadResult read_grndat(MaterialRecord& material, KeywordReader& reader) {
    const auto header = reader.integer_pair();
    if (header.status != 0) return {2, false};
    const auto selector = header.values[0];
    material.feature_0x17e8 = selector;
    auto& p = material.grndat;
    if (selector == 1) {
        constexpr std::array<int, 9> types{0x55, 0x54, 0x56, 0x57, 0x58, 0x59, 0x5a, 0x5b, 0x5c};
        constexpr std::array<int, 9> flags{-1, -1, 0, -1, 1, -1, 2, -1, 3};
        for (std::size_t i = 0; i < types.size(); ++i) {
            const auto h = reader.integer_pair();
            if (h.status != 0) return {2, false};
            auto& t = p.tables_0x1088[i];
            t.reset_type(types[i]);
            const int subselector = h.values[0];
            if (subselector == 1) t.selectors_0x10[0] = 3;
            else if (subselector == 2) t.selectors_0x10[0] = 8;
            else if (subselector == 3 && (i == 3 || i == 5 || i == 7 || i == 8))
                t.selectors_0x10[0] = 33;
            if (flags[i] >= 0) {
                auto& flag = p.flags_0x1078[flags[i]];
                if (subselector == 0) flag = 0;
                else if (subselector == 1 || subselector == 2 || (i == 8 && subselector == 3)) flag = 1;
            }
            t.counts_0x1c[0] = h.values[1];
        }
        for (auto& t : p.tables_0x1088)
            if (read_property_values(reader, t) != 0) return {2, false};
        const auto pair = reader.real_pair();
        p.scalar_0x14c8 = pair.values[0]; p.scalar_0x14c0 = pair.values[1];
        return {pair.status == 0 ? 0 : 2, true};
    }
    if (selector == 2) {
        p.secondary_0x14d0 = header.values[1];
        if (record(reader, p.embedded_0x14d8) != 0) return {2, false};
        for (int index : {2, 1, 0, 3, 4}) {
            auto& list = p.lists[index];
            list.clear();
            const auto count = reader.integer(true); // Return status deliberately ignored.
            for (int i = 0; i < count.values[0]; ++i) {
                list.emplace_back(); // Native node is linked before parsing begins.
                if (record(reader, list.back()) != 0) return {2, false};
            }
        }
        return {0, true};
    }
    if (selector == 4) {
        const auto r = read_property(reader, p.table_0x1740, 0, nullptr);
        return {r.status == 0 ? 0 : 2, true};
    }
    if (selector == 5) {
        const auto r = read_relation_header(reader, RelationHeaderKind::six_reals);
        std::copy_n(r.reals.begin(), 6, p.coefficients_0x17b8.begin());
        return {r.status == 0 ? 0 : 2, true};
    }
    if (selector < 0) {
        p.secondary_0x17ec = std::bit_cast<std::int32_t>(
            std::uint32_t{0} - std::bit_cast<std::uint32_t>(selector));
        material.feature_0x17e8 = -1;
    }
    return {0, true};
}
}
