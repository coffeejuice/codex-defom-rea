#include "hardening_import.hpp"
#include "relation_properties.hpp"

#include <cmath>
#include <limits>
#include <stdexcept>

namespace deform::recovered {
namespace {
int integer(double value) {
    const auto integral = std::trunc(value);
    if (!std::isfinite(value) || integral < std::numeric_limits<int>::min() ||
        integral > std::numeric_limits<int>::max())
        throw std::invalid_argument("hardening table conversion outside finite signed32 domain");
    return static_cast<int>(integral);
}
int payload(KeywordReader& reader, PropertyTable& table) {
    return read_property_values(reader, table) == 0 ? 0 : 2;
}
}

bool hardening_keyword(int keyword) noexcept {
    return keyword == 109 || keyword == 114 || keyword == 117 || keyword == 118 ||
        keyword == 119 || keyword == 120;
}

int read_hardening(int keyword, MaterialRecord& material, KeywordReader& reader) {
    auto& fields = material.hardening;
    if (keyword == 109) return reader.integer().status == 0 ? 0 : 2;
    if (keyword == 120) {
        const auto header = reader.integer();
        fields.hdnrul_0x668 = header.values[0]; //Live zeroing survives read failure.
        return header.status == 0 ? 0 : 2;
    }
    if (keyword == 117) {
        const auto header = reader.integer();
        if (header.status != 0) return 2;
        auto& table = fields.hdntim_0xf88;
        table.reset_type(0x53);
        table.counts_0x1c[0] = header.values[0];
        return payload(reader, table);
    }
    if (keyword == 118) {
        const auto header = reader.integer_triple();
        if (header.status != 0) return 2;
        auto& table = fields.jominy_0x1000;
        table.reset_type(0x52); //Even disabled/unrecognized modes reset old data.
        if (header.values[0] != 1) return 0;
        table.counts_0x1c[0] = header.values[1];
        table.counts_0x1c[1] = header.values[2];
        return payload(reader, table);
    }
    if (keyword == 119) {
        const auto header = reader.integer_real();
        if (header.status != 0) return 2;
        auto& table = material.frcmod.tables[0]; //Canonical shared1b10 storage.
        table.reset_type(0x1b);
        if (header.integer_value == 0) {
            table.rank_0x04 = 0;
            table.constant_0x08 = header.real_value;
        }
        else if (header.integer_value == 1) {
            table.rank_0x04 = 1;
            table.selectors_0x10[0] = 3;
            table.counts_0x1c[0] = integer(header.real_value);
            return payload(reader, table);
        }
        return 0;
    }
    if (keyword != 114) throw std::logic_error("unexpected hardening keyword");
    const auto header = read_relation_header(reader, RelationHeaderKind::integer_two_reals);
    if (header.status != 0) return 2;
    auto& table = fields.hdnpha_0xf10;
    table.reset_type(0x23);
    const int selector = header.integers[0];
    if (selector == 0) {
        table.rank_0x04 = 0;
        table.constant_0x08 = header.reals[0];
    }
    else if (selector >= 1 && selector <= 3) {
        table.rank_0x04 = 1;
        table.selectors_0x10[0] = selector == 1 ? 12 : selector == 2 ? 3 : 9;
        table.counts_0x1c[0] = integer(header.reals[0]);
    } else if (selector == 4) {
        table.rank_0x04 = 2;
        table.selectors_0x10[0] = 3;
        table.selectors_0x10[1] = 12;
        table.counts_0x1c[0] = integer(header.reals[0]);
        table.counts_0x1c[1] = integer(header.reals[1]);
    } else return 3; //Source-specific status after the destructive reset.
    return payload(reader, table);
}
}
