#include "property_table.hpp"

#include <algorithm>
#include <bit>
#include <cmath>
#include <limits>
#include <stdexcept>

namespace deform::recovered {
namespace {
void set_constant_word(double& destination, unsigned word, std::uint32_t value) {
    const unsigned shift = word * 32;
    auto bits = std::bit_cast<std::uint64_t>(destination);
    bits = (bits & ~(std::uint64_t{0xffffffff} << shift)) |
           (static_cast<std::uint64_t>(value) << shift);
    destination = std::bit_cast<double>(bits);
}
void require_rank(std::int32_t rank) {
    if (rank < 0 || rank > 3)
        throw std::invalid_argument("property table rank outside owned three-axis domain");
}
std::int32_t truncate_count(double value) {
    const double truncated = std::trunc(value);
    if (!std::isfinite(value) || truncated < std::numeric_limits<std::int32_t>::min() ||
        truncated > std::numeric_limits<std::int32_t>::max())
        throw std::out_of_range("property table count outside signed32 conversion domain");
    return static_cast<std::int32_t>(value);
}
}

PropertyTable::PropertyTable(std::int32_t type) { initialize(type); }

void PropertyTable::release_owned() noexcept {
    for (auto& axis : axes) std::vector<double>{}.swap(axis);
    std::vector<double>{}.swap(dependent);
}

std::int32_t PropertyTable::dependent_count() const {
    require_rank(rank_0x04);
    if (rank_0x04 == 0) {
        const bool special = type_0x00 == 0xb5 || type_0x00 == 0xa7 ||
            type_0x00 == 0xa8 || type_0x00 == 0xaa || type_0x00 == 0xab;
        return values_per_point_0x40 >= 2 || special ? values_per_point_0x40 : 0;
    }
    std::int64_t count = values_per_point_0x40;
    for (std::int32_t axis = 0; axis < rank_0x04; ++axis) {
        count *= counts_0x1c[axis];
        if (count < std::numeric_limits<std::int32_t>::min() ||
            count > std::numeric_limits<std::int32_t>::max())
            throw std::overflow_error("property table dependent product exceeds signed32 domain");
    }
    return count < 0 ? 0 : static_cast<std::int32_t>(count);
}

void PropertyTable::allocate_zeroed() {
    require_rank(rank_0x04);
    release_owned();
    for (auto& count : counts_0x1c) if (count < 0) count = 0;
    if (values_per_point_0x40 < 1) values_per_point_0x40 = 1;
    const auto total = dependent_count();
    for (std::int32_t axis = 0; axis < rank_0x04; ++axis)
        if (counts_0x1c[axis] > 0)
            axes[axis].assign(static_cast<std::size_t>(counts_0x1c[axis]), 0.0);
    if (total > 0) dependent.assign(static_cast<std::size_t>(total), 0.0);
}

void PropertyTable::reset_type(std::int32_t type, bool preserve_metadata) {
    const auto old54 = preserved_0x54;
    const auto old60 = preserved_0x60;
    initialize(type);
    if (preserve_metadata) { preserved_0x54 = old54; preserved_0x60 = old60; }
}

void PropertyTable::initialize(std::int32_t type) {
    release_owned();
    type_0x00 = type;
    rank_0x04 = 0;
    constant_0x08 = 0.0;
    selectors_0x10.fill(0);
    counts_0x1c.fill(0);
    values_per_point_0x40 = 1;
    flag_0x50 = preserved_0x54 = mode_0x58 = 0;
    preserved_0x60 = 0;
    // Exact assignments from retained12d2a0, including fallthrough labels.
  switch(type) {
  case 7:
    rank_0x04 = 1;
    selectors_0x10[0] = 3;
    break;
  case 10:
    rank_0x04 = 1;
    selectors_0x10[0] = 5;
    break;
  case 0xd:
  case 0xe:
  case 0xf:
  case 0x10:
  case 0x11:
  case 0x12:
  case 0x13:
  case 0x14:
    rank_0x04 = 1;
    selectors_0x10[0] = 1;
    break;
  case 0x1c:
    rank_0x04 = 1;
    selectors_0x10[0] = 0x1f;
    break;
  case 0x27:
    rank_0x04 = 1;
    selectors_0x10[0] = 3;
    values_per_point_0x40 = 2;
    break;
  case 0x28:
    rank_0x04 = 1;
    selectors_0x10[0] = 0xc;
    values_per_point_0x40 = 2;
    break;
  case 0x29:
    rank_0x04 = 2;
    selectors_0x10[0] = 3;
    selectors_0x10[1] = 0xc;
    break;
  case 0x2c:
    rank_0x04 = 3;
    values_per_point_0x40 = 2;
    selectors_0x10[0] = 0xc;
    selectors_0x10[1] = 6;
    selectors_0x10[2] = 3;
    break;
  case 0x2d:
    rank_0x04 = 3;
    values_per_point_0x40 = 2;
    selectors_0x10[0] = 7;
    selectors_0x10[1] = 0xe;
    selectors_0x10[2] = 3;
    break;
  case 0x2e:
    rank_0x04 = 3;
    values_per_point_0x40 = 2;
    selectors_0x10[0] = 0xc;
    selectors_0x10[1] = 6;
    selectors_0x10[2] = 3;
    break;
  case 0x2f:
    rank_0x04 = 3;
    values_per_point_0x40 = 2;
    selectors_0x10[0] = 7;
    selectors_0x10[1] = 0xe;
    selectors_0x10[2] = 3;
    break;
  case 0x30:
    rank_0x04 = 3;
    values_per_point_0x40 = 2;
    selectors_0x10[0] = 0xc;
    selectors_0x10[1] = 0xf;
    selectors_0x10[2] = 0x10;
    break;
  case 0x32:
    rank_0x04 = 1;
    selectors_0x10[0] = 3;
    break;
  case 0x36:
    rank_0x04 = 1;
    selectors_0x10[0] = 3;
    break;
  case 0x37:
    rank_0x04 = 3;
    values_per_point_0x40 = 2;
    selectors_0x10[0] = 0xc;
    selectors_0x10[1] = 6;
    selectors_0x10[2] = 3;
    break;
  case 0x38:
    rank_0x04 = 1;
    selectors_0x10[0] = 3;
    break;
  case 0x43:
    rank_0x04 = 3;
    selectors_0x10[0] = 1;
    selectors_0x10[1] = 6;
    selectors_0x10[2] = 3;
    break;
  case 0x44:
    rank_0x04 = 3;
    selectors_0x10[0] = 7;
    selectors_0x10[1] = 6;
    selectors_0x10[2] = 3;
    break;
  case 0x45:
    rank_0x04 = 1;
    selectors_0x10[0] = 3;
    values_per_point_0x40 = 2;
    break;
  case 0x46:
    rank_0x04 = 1;
    selectors_0x10[0] = 3;
    values_per_point_0x40 = 2;
    break;
  case 0x47:
    rank_0x04 = 1;
    selectors_0x10[0] = 3;
    values_per_point_0x40 = 5;
    break;
  case 0x48:
    rank_0x04 = 1;
    selectors_0x10[0] = 3;
    values_per_point_0x40 = 3;
    break;
  case 0x4f:
    rank_0x04 = 3;
    selectors_0x10[0] = 7;
    selectors_0x10[1] = 8;
    selectors_0x10[2] = 3;
    break;
  case 0x50:
    rank_0x04 = 3;
    selectors_0x10[0] = 7;
    selectors_0x10[1] = 0xc;
    selectors_0x10[2] = 3;
    break;
  case 0x51:
    rank_0x04 = 3;
    selectors_0x10[0] = 7;
    selectors_0x10[1] = 8;
    selectors_0x10[2] = 3;
    break;
  case 0x52:
    rank_0x04 = 2;
    selectors_0x10[0] = 0xd;
    selectors_0x10[1] = 0xc;
    break;
  case 0x53:
    rank_0x04 = 1;
    selectors_0x10[0] = 1;
    break;
  case 0x54:
    rank_0x04 = 1;
    values_per_point_0x40 = 6;
    selectors_0x10[0] = 3;
    break;
  case 0x55:
    rank_0x04 = 1;
    values_per_point_0x40 = 4;
    selectors_0x10[0] = 3;
    break;
  case 0x56:
    rank_0x04 = 1;
    values_per_point_0x40 = 7;
    selectors_0x10[0] = 3;
    break;
  case 0x57:
    rank_0x04 = 1;
    values_per_point_0x40 = 6;
    selectors_0x10[0] = 3;
    break;
  case 0x58:
    rank_0x04 = 1;
    values_per_point_0x40 = 7;
    selectors_0x10[0] = 3;
    break;
  case 0x59:
    rank_0x04 = 1;
    values_per_point_0x40 = 6;
    selectors_0x10[0] = 3;
    break;
  case 0x5a:
    rank_0x04 = 1;
    values_per_point_0x40 = 9;
    selectors_0x10[0] = 3;
    break;
  case 0x5b:
    rank_0x04 = 1;
    values_per_point_0x40 = 6;
    selectors_0x10[0] = 3;
    break;
  case 0x5c:
    rank_0x04 = 1;
    values_per_point_0x40 = 3;
    selectors_0x10[0] = 3;
    break;
  case 0x5d:
    rank_0x04 = 0;
    break;
  case 0x5e:
  case 0x5f:
  case 0x60:
    rank_0x04 = 0;
    break;
  case 0x61:
  case 0x62:
  case 99:
  case 100:
    rank_0x04 = 0;
    break;
  case 0x65:
    values_per_point_0x40 = 6;
    break;
  case 0x66:
    values_per_point_0x40 = 3;
    break;
  case 0x67:
    values_per_point_0x40 = 1;
    break;
  case 0x68:
    rank_0x04 = 1;
    values_per_point_0x40 = 6;
    break;
  case 0x69:
    rank_0x04 = 1;
    values_per_point_0x40 = 3;
    break;
  case 0x70:
    rank_0x04 = 1;
    values_per_point_0x40 = 1;
    selectors_0x10[0] = 3;
    break;
  case 0x71:
    rank_0x04 = 1;
    values_per_point_0x40 = 1;
    selectors_0x10[0] = 3;
    break;
  case 0x72:
    rank_0x04 = 1;
    values_per_point_0x40 = 1;
    break;
  case 0x73:
    rank_0x04 = 1;
    values_per_point_0x40 = 1;
    break;
  case 0x74:
    rank_0x04 = 1;
    values_per_point_0x40 = 1;
    selectors_0x10[0] = 10;
    break;
  case 0x75:
    rank_0x04 = 1;
    values_per_point_0x40 = 1;
    selectors_0x10[0] = 10;
    break;
  case 0x79:
    rank_0x04 = 1;
    counts_0x1c[0] = 1;
    values_per_point_0x40 = 2;
    selectors_0x10[0] = 1;
    break;
  case 0x7a:
    rank_0x04 = 1;
    counts_0x1c[0] = 1;
    values_per_point_0x40 = 4;
    selectors_0x10[0] = 1;
    break;
  case 0x7b:
    rank_0x04 = 1;
    counts_0x1c[0] = 1;
    values_per_point_0x40 = 3;
    selectors_0x10[0] = 1;
    break;
  case 0x7c:
    rank_0x04 = 1;
    counts_0x1c[0] = 1;
    values_per_point_0x40 = 1;
    selectors_0x10[0] = 0x16;
    break;
  case 0x80:
    rank_0x04 = 0;
    counts_0x1c[0] = 0;
    values_per_point_0x40 = 0;
    break;
  case 0x81:
    rank_0x04 = 1;
    values_per_point_0x40 = 1;
    selectors_0x10[0] = 7;
    break;
  case 0x82:
    rank_0x04 = 1;
    values_per_point_0x40 = 1;
    selectors_0x10[0] = 3;
    break;
  case 0x83:
    rank_0x04 = 1;
    values_per_point_0x40 = 1;
    break;
  case 0x84:
    rank_0x04 = 1;
    values_per_point_0x40 = 1;
    break;
  case 0x85:
    rank_0x04 = 1;
    values_per_point_0x40 = 1;
    break;
  case 0x86:
    rank_0x04 = 1;
    values_per_point_0x40 = 1;
    break;
  case 0x87:
    rank_0x04 = 1;
    values_per_point_0x40 = 1;
    break;
  case 0x88:
    rank_0x04 = 1;
    values_per_point_0x40 = 1;
    break;
  case 0x89:
    rank_0x04 = 1;
    values_per_point_0x40 = 1;
    break;
  case 0x8a:
    rank_0x04 = 1;
    values_per_point_0x40 = 1;
    break;
  case 0x8b:
    rank_0x04 = 1;
    values_per_point_0x40 = 1;
    selectors_0x10[0] = 2;
    break;
  case 0x8c:
    rank_0x04 = 3;
    selectors_0x10[0] = 2;
    selectors_0x10[1] = 8;
    selectors_0x10[2] = 3;
    break;
  case 0x8d:
    rank_0x04 = 1;
    values_per_point_0x40 = 1;
    selectors_0x10[0] = 7;
    break;
  case 0x8e:
  case 0x8f:
  case 0x90:
  case 0x91:
  case 0x92:
  case 0x93:
  case 0x94:
  case 0x95:
  case 0x96:
  case 0x97:
  case 0x99:
  case 0x9a:
  case 0x9b:
  case 0x9c:
  case 0x9d:
  case 0x9e:
  case 0x9f:
  case 0xa0:
  case 0xa1:
  case 0xa2:
  case 0xa3:
    rank_0x04 = 1;
    counts_0x1c[0] = 0;
    values_per_point_0x40 = 0;
    selectors_0x10[0] = 0;
    break;
  case 0x98:
    rank_0x04 = 1;
    counts_0x1c[0] = 1;
    values_per_point_0x40 = 1;
    selectors_0x10[0] = 1;
    break;
  case 0xa6:
    rank_0x04 = 1;
    counts_0x1c[0] = 1;
    values_per_point_0x40 = 2;
    selectors_0x10[0] = 0;
    break;
  case 0xa7:
    rank_0x04 = 0;
    values_per_point_0x40 = 1;
    break;
  case 0xa8:
    rank_0x04 = 0;
    values_per_point_0x40 = 1;
    break;
  case 0xa9:
    rank_0x04 = 1;
    counts_0x1c[0] = 1;
    values_per_point_0x40 = 2;
    selectors_0x10[0] = 0;
    break;
  case 0xaa:
    rank_0x04 = 0;
    values_per_point_0x40 = 1;
    break;
  case 0xab:
    rank_0x04 = 0;
    values_per_point_0x40 = 1;
    break;
  case 0xac:
    rank_0x04 = 2;
    selectors_0x10[0] = 3;
    selectors_0x10[1] = 8;
    break;
  case 0xad:
    rank_0x04 = 0;
    values_per_point_0x40 = 1;
    break;
  case 0xae:
    rank_0x04 = 0;
    values_per_point_0x40 = 1;
    set_constant_word(constant_0x08, 0, 0);
    set_constant_word(constant_0x08, 1, 0x3ff00000);
    break;
  case 0xaf:
    rank_0x04 = 0;
    values_per_point_0x40 = 2;
    set_constant_word(constant_0x08, 0, 0);
    set_constant_word(constant_0x08, 1, 0);
    break;
  case 0xb0:
    rank_0x04 = 1;
    selectors_0x10[0] = 0;
    break;
  case 0xb1:
    rank_0x04 = 1;
    selectors_0x10[0] = 0;
    break;
  case 0xb2:
    rank_0x04 = 1;
    selectors_0x10[0] = 0;
    break;
  case 0xb3:
    rank_0x04 = 0;
    values_per_point_0x40 = 1;
    set_constant_word(constant_0x08, 0, 0);
    set_constant_word(constant_0x08, 1, 0);
    break;
  case 0xb4:
    rank_0x04 = 1;
    values_per_point_0x40 = 1;
    selectors_0x10[0] = 0x20;
    break;
  case 0xb5:
    rank_0x04 = 0;
    values_per_point_0x40 = 0;
    break;
  case 0xb6:
    rank_0x04 = 0;
    set_constant_word(constant_0x08, 0, 0);
    set_constant_word(constant_0x08, 1, 0);
    break;
  case 0xb7:
    rank_0x04 = 1;
    values_per_point_0x40 = 1;
    selectors_0x10[0] = 7;
    break;
  case 0xb8:
    rank_0x04 = 1;
    values_per_point_0x40 = 1;
    selectors_0x10[0] = 7;
    break;
  case 0xb9:
    rank_0x04 = 1;
    values_per_point_0x40 = 1;
    selectors_0x10[0] = 1;
    break;
  case 0xba:
    rank_0x04 = 1;
    values_per_point_0x40 = 1;
    selectors_0x10[0] = 1;
    break;
  case 0xbb:
    rank_0x04 = 1;
    values_per_point_0x40 = 1;
    selectors_0x10[0] = 1;
  }
    if (type != 0x65 && type != 0x66 && type != 0x67 && type != 0x51)
        allocate_zeroed();
}

PropertyHeaderRead read_property_header(KeywordReader& reader) {
    PropertyHeaderRead result;
    auto line = reader.line();
    if (line.status != 0) return result;
    std::replace(line.bytes.begin(), line.bytes.end(), ',', ' ');
    KeywordReader fields(std::string_view(line.bytes.c_str()));
    const auto first = fields.selector_integer();
    result.conversions = first.conversions;
    if (first.conversions == 1) {
        result.selector = first.values[0];
        for (auto& value : result.values) {
            const auto parsed = fields.stream_real();
            if (parsed.conversions != 1) break;
            value = parsed.values[0];
            ++result.conversions;
        }
    }
    result.status = result.conversions == -1 ? 1 : 0;
    return result;
}

int read_property_vector(KeywordReader& reader, PropertyTable& table) {
    if (table.rank_0x04 != 0 || table.values_per_point_0x40 <= 0) return 1;
    table.allocate_zeroed();
    for (auto& value : table.dependent) {
        const auto parsed = reader.stream_real(value);
        if (parsed.conversions == 1) value = parsed.values[0];
        if (parsed.conversions == -1) return 1;
    }
    return 0;
}

int read_property_values(KeywordReader& reader, PropertyTable& table) {
    require_rank(table.rank_0x04);
    if (table.rank_0x04 == 0 && table.values_per_point_0x40 <= 1) return 0;
    table.allocate_zeroed();
    const auto read_direct = [&reader](double& value) {
        const auto parsed = reader.stream_real(value);
        if (parsed.conversions == 1) value = parsed.values[0];
        return parsed.conversions != -1;
    };
    if (table.rank_0x04 == 0) {
        for (auto& value : table.dependent) if (!read_direct(value)) return 1;
    } else if (table.rank_0x04 == 1) {
        const auto width = static_cast<std::size_t>(table.values_per_point_0x40);
        for (std::size_t row = 0; row < table.axes[0].size(); ++row) {
            if (!read_direct(table.axes[0][row])) return 1;
            for (std::size_t column = 0; column < width; ++column)
                if (!read_direct(table.dependent[row * width + column])) return 1;
        }
    } else {
        for (std::int32_t axis = 0; axis < table.rank_0x04; ++axis)
            for (auto& value : table.axes[axis]) if (!read_direct(value)) return 1;
        // Native malloc scratch is committed only after every read avoids EOF.
        // Conversion0 there would copy an uninitialized double: excluded.
        std::vector<double> staged(table.dependent.size());
        for (auto& value : staged) {
            const auto parsed = reader.stream_real();
            if (parsed.conversions == -1) return 1;
            if (parsed.conversions != 1)
                throw std::invalid_argument("uninitialized property table staging value after matching failure");
            value = parsed.values[0];
        }
        std::copy(staged.begin(), staged.end(), table.dependent.begin());
    }
    if (table.dependent_count() > 0) (void)reader.line(); // Native ignores EOF here.
    return 0;
}

PropertyReadResult read_property(KeywordReader& reader, PropertyTable& table,
    int auxiliary_flag, double* auxiliary) {
    PropertyReadResult result;
    result.header = read_property_header(reader);
    if (result.header.status != 0) return result;
    table.reset_type(0);
    const auto selector = result.header.selector;
    const auto& values = result.header.values;
    switch (selector) {
    case 0: table.constant_0x08 = values[0]; break;
    case 1: case 2: case 3: case 6:
        table.rank_0x04 = 1;
        table.selectors_0x10[0] = selector == 1 ? 3 : selector == 2 ? 9 : selector == 3 ? 12 : 8;
        table.counts_0x1c[0] = truncate_count(values[0]);
        break;
    case 4: case 5: case 7:
        table.rank_0x04 = 2;
        table.selectors_0x10[0] = 3;
        table.selectors_0x10[1] = selector == 4 ? 12 : selector == 5 ? 8 : 4;
        table.counts_0x1c[0] = truncate_count(values[0]);
        table.counts_0x1c[1] = truncate_count(values[1]);
        break;
    default: break;
    }
    if (auxiliary_flag >= 0 && auxiliary != nullptr && selector >= 0) {
        if (selector < 4 && result.header.conversions > 2) *auxiliary = values[1];
        else if (selector == 4 && result.header.conversions > 3) *auxiliary = values[2];
    }
    result.payload_invoked = true;
    result.status = read_property_values(reader, table);
    return result;
}

} // namespace deform::recovered
