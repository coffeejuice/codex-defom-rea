#include "retained_material_import.hpp"
#include "relation_properties.hpp"

#include <cmath>
#include <limits>
#include <stdexcept>

namespace deform::recovered {
namespace {
int count(double value) {
    const double integral = std::trunc(value);
    if (!std::isfinite(value) || integral < std::numeric_limits<int>::min() ||
        integral > std::numeric_limits<int>::max())
        throw std::invalid_argument("material table count outside finite signed32 domain");
    return static_cast<int>(integral);
}
}

bool retained_material_keyword(int keyword) noexcept {
    return keyword >= 110 && keyword <= 113;
}

int read_retained_material(int keyword, PropertyTable& table, KeywordReader& reader) {
    if (!retained_material_keyword(keyword))
        throw std::logic_error("unexpected retained material keyword");
    const auto header = read_relation_header(reader, RelationHeaderKind::integer_two_reals);
    if (header.status != 0) return 2;
    const int selector = header.integers[0];
    if (keyword == 110) {
        if (selector == 0) {
            table.rank_0x04 = 0;
            table.constant_0x08 = header.reals[0];
        } else if (selector >= 1 && selector <= 3) {
            table.rank_0x04 = 1;
            table.selectors_0x10[0] = selector == 1 ? 3 : selector == 2 ? 6 : 24;
            table.counts_0x1c[0] = count(header.reals[0]);
        } else if (selector >= 4 && selector <= 6) {
            table.rank_0x04 = 2;
            table.selectors_0x10[0] = selector == 6 ? 6 : 3;
            table.selectors_0x10[1] = selector == 4 ? 6 : 24;
            table.counts_0x1c[0] = count(header.reals[0]);
            table.counts_0x1c[1] = count(header.reals[1]);
        } else if (selector == 7) {
            const auto counts = reader.integer_triple();
            if (counts.status != 0) return 2; //No table writes before this succeeds.
            table.rank_0x04 = 3;
            table.selectors_0x10 = {3, 6, 24};
            table.counts_0x1c = {counts.values[0], counts.values[1], counts.values[2]};
        } else return 2;
    } else {
        if (selector == 0) {
            table.rank_0x04 = 0;
            table.constant_0x08 = header.reals[0];
        } else if (selector == 1 || selector == 2) {
            table.rank_0x04 = 1;
            table.selectors_0x10[0] = selector == 1 ? 3 : 8;
            table.counts_0x1c[0] = count(header.reals[0]);
        } else if (selector == 3) {
            table.rank_0x04 = 2;
            table.selectors_0x10[0] = 3;
            table.selectors_0x10[1] = 8;
            table.counts_0x1c[0] = count(header.reals[0]);
            table.counts_0x1c[1] = count(header.reals[1]);
        } else return 2;
    }
    return read_property_values(reader, table) == 0 ? 0 : 2;
}
}
