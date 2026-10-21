#include "creep_properties.hpp"

#include <cmath>
#include <limits>
#include <stdexcept>

namespace deform::recovered {
namespace {
int signed_count(double value) {
    const double integral = std::trunc(value);
    if (!std::isfinite(value) || integral < std::numeric_limits<int>::min() ||
        integral > std::numeric_limits<int>::max())
        throw std::invalid_argument("CREEP axis conversion outside finite signed32 domain");
    return static_cast<int>(integral);
}
int checked(std::int64_t value) {
    if (value < std::numeric_limits<int>::min() || value > std::numeric_limits<int>::max())
        throw std::invalid_argument("CREEP derived width overflow remains uncharacterized");
    return static_cast<int>(value);
}
}

int read_creep(CreepProperties& p, KeywordReader& reader) {
    const auto header = reader.integer_triple();
    if (header.status != 0) return 2;
    const int selector = header.values[0];
    const auto payload = [&reader](PropertyTable& table) {
        return read_property_values(reader, table) == 0 ? 0 : 2;
    };
    if (selector == 0) p.mode_0xac8 = 0;
    else if (selector >= 1 && selector <= 4) {
        p.mode_0xac8 = selector;
        auto& table = p.tables[static_cast<std::size_t>(selector + 1)];
        table.reset_type(0x44 + selector);
        table.counts_0x1c[0] = header.values[1];
        return payload(table);
    } else if (selector == 5 || selector == 6) {
        p.mode_0xac8 = selector; //Mode changes even when the second header fails.
        const auto counts = reader.integer_triple();
        if (counts.status != 0) return 2;
        auto& table = p.tables[selector == 5 ? 1 : 0];
        table.reset_type(selector == 5 ? 0x44 : 0x43);
        table.counts_0x1c = {counts.values[0], counts.values[1], counts.values[2]};
        return payload(table);
    } else if (selector == 7 || selector == 8) {
        p.mode_0xac8 = selector;
        const auto count = reader.integer();
        p.axis_count_0xda4 = count.values[0]; //Live zeroing survives failure.
        if (count.status != 0) return 2;
        auto& dimensions = p.tables[6];
        dimensions.reset_type(0xa9);
        dimensions.counts_0x1c[0] = p.axis_count_0xda4;
        if (payload(dimensions) != 0) return 2;
        int sum = 0, product = 1;
        for (const auto value : dimensions.axes[0]) {
            const int n = signed_count(value);
            sum = checked(static_cast<std::int64_t>(sum) + n);
            product = checked(static_cast<std::int64_t>(product) * n);
        }
        auto& coordinates = p.tables[7];
        coordinates.reset_type(0xaa);
        coordinates.values_per_point_0x40 = sum;
        if (payload(coordinates) != 0) return 2;
        auto& values = p.tables[8];
        values.reset_type(0xab);
        values.values_per_point_0x40 = product;
        return payload(values);
    } else if (selector < 0) {
        p.mode_0xac8 = 9;
        if (selector == std::numeric_limits<int>::min())
            throw std::invalid_argument("CREEP INT_MIN reference negation remains uncharacterized");
        p.reference_0xda0 = -selector;
    }
    return 0; //Unknown positive selectors preserve the complete prior state.
}
}
