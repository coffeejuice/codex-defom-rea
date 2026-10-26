#include "object_motion_import.hpp"
#include "legacy_motion_math.hpp"

#include <algorithm>
#include <cmath>
#include <limits>
#include <stdexcept>

namespace deform::recovered {
namespace {
int integer(double value) {
    const double result = std::trunc(value);
    if (!std::isfinite(value) || result < std::numeric_limits<int>::min() ||
        result > std::numeric_limits<int>::max())
        throw std::out_of_range("MOVCTL conversion outside defined signed32 profile");
    return static_cast<int>(result);
}
int absolute_reference(int value) {
    if (value == std::numeric_limits<int>::min())
        throw std::out_of_range("MOVCTL INT_MIN reference remains uncharacterized");
    return -value;
}
void curve(PropertyTable& table, int selector, int rows) {
    table.rank_0x04 = 1;
    table.selectors_0x10[0] = selector;
    table.counts_0x1c[0] = rows;
}
}

int read_object_motion(ObjectMotion& m, int dimension, KeywordReader& reader) {
    const auto header = reader.motion_header();
    if (header.status != 0) return 2;
    const auto& v = header.values;
    const int mode = header.selector;
    const bool planar = dimension == 2;
    if (!planar && dimension != 3)
        throw std::invalid_argument("MOVCTL dimension outside owned profile");
    int status = 0;
    const auto direction = [&](std::size_t start) {
        if (planar) {
            const auto pair = legacy_motion_cos_sin(v[start] * 0.017453292519943295);
            m.direction_0x21b0[0] = pair[0];
            m.direction_0x21b0[1] = pair[1];
        } else std::copy_n(v.begin() + start, 3, m.direction_0x21b0.begin());
    };
    const auto external = [&](int selector) {
        m.field_0x2780 = selector < 0 ? 1 : 0;
        m.field_0x2784 = selector < 0 ? absolute_reference(selector) : 0;
    };
    if (mode >= 0 && mode <= 11) m.mode_0x21c8 = mode;
    switch (mode) {
    case 0: case 7: case 8: {
        direction(1);
        auto& table = m.tables[mode == 0 ? 0 : mode == 7 ? 7 : 8];
        table.reset_type(0);
        const int selector = integer(v[0]);
        const double value = v[planar ? 2 : 4];
        if (selector == 0) table.constant_0x08 = value;
        else if (selector == 1 || selector == 2) {
            curve(table, selector == 1 ? 2 : 1, integer(value));
            status = read_property_values(reader, table);
        }
        external(selector); // Includes writes after a failed table read.
        if (mode == 7 || mode == 8) m.values_0x2768[0] = v[planar ? 3 : 5];
        if (mode == 7) m.values_0x2768[1] = v[planar ? 4 : 6];
        break;
    }
    case 1: {
        direction(1);
        auto& table = m.tables[1];
        table.reset_type(0);
        const int selector = integer(v[0]);
        const double value = v[planar ? 2 : 4];
        m.field_0x2780 = m.field_0x2784 = m.field_0x22c0 = 0;
        m.field_0x22c8 = 0.0;
        if (selector == 0) table.constant_0x08 = value;
        else if (selector == 1 || selector == 2 || selector == 4) {
            curve(table, selector == 1 ? 2 : selector == 2 ? 1 : 5, integer(value));
            status = read_property_values(reader, table);
        } else if (selector >= 1001 && selector <= 1999) {
            m.field_0x22c0 = selector - 1000;
            m.field_0x22c8 = value;
        } else if (selector < 0) {
            m.field_0x2780 = 1;
            m.field_0x2784 = absolute_reference(selector);
        }
        break;
    }
    case 2: {
        direction(0);
        std::array<double, 6> data;
        if (planar) std::copy_n(v.begin() + 1, 6, data.begin());
        else {
            const auto read = reader.real_six();
            data = read.values;
            status = read.status;
        }
        const int selector = integer(data[4]);
        const int rows = integer(data[5]);
        auto& table = m.tables[2];
        m.field_0x2350 = data[0];
        m.field_0x2348 = data[2];
        m.field_0x2360 = integer(data[3]);
        // With selector!=0/rows==0 the prior2358 is deliberately retained.
        if (selector == 0 || rows != 0) m.field_0x2358 = data[1];
        table.reset_type(0);
        if (selector != 0 || rows != 0) {
            table.rank_0x04 = 1;
            if (selector == 4) table.selectors_0x10[0] = 5;
            table.counts_0x1c[0] = rows != 0 ? rows : integer(data[1]);
            status = read_property_values(reader, table);
            if (status != 0) return 2; // Before planar near-zero cleanup.
        }
        break;
    }
    case 3: {
        direction(0);
        auto& table = m.tables[3];
        //3D resets before reading the six-value continuation.
        if (!planar) table.reset_type(0);
        std::array<double, 6> data;
        if (planar) std::copy_n(v.begin() + 1, 6, data.begin());
        else {
            const auto read = reader.real_six();
            data = read.values;
            status = read.status;
        }
        std::copy_n(data.begin(), 4, m.values_0x23e0.begin());
        if (planar) table.reset_type(0);
        const int selector = integer(data[4]);
        if (selector == 4) {
            curve(table, 5, integer(data[5]));
            status = read_property_values(reader, table);
            if (status != 0) return 2;
        }
        break;
    }
    case 4: {
        direction(0);
        double rows;
        if (planar) {
            std::copy_n(v.begin() + 1, 3, m.values_0x2478.begin());
            rows = v[4];
        } else {
            const auto read = reader.real_quad();
            std::copy_n(read.values.begin(), 3, m.values_0x2478.begin());
            rows = read.values[3];
            status = read.status;
        }
        auto& table = m.tables[4];
        table.reset_type(0);
        m.flag_0x2490 = integer(rows) >= 1;
        if (m.flag_0x2490) {
            curve(table, 10, integer(rows));
            status = read_property_values(reader, table);
        }
        break;
    }
    case 5: {
        direction(0);
        const auto tokens = reader.token_header(7);
        if (tokens.status != 0) return 2;
        const auto token = [&](std::size_t i) {
            return i < tokens.tokens.size() ? std::string_view(tokens.tokens[i]) : std::string_view{};
        };
        const int selector = KeywordReader::decimal_token_integer(token(0));
        for (std::size_t i = 0; i < 3; ++i)
            m.values_0x2588[i] = KeywordReader::legacy_token_real(token(i + 2));
        m.integers_0x25a0[0] = KeywordReader::decimal_token_integer(token(5));
        m.integers_0x25a0[1] = KeywordReader::decimal_token_integer(token(6));
        auto& table = m.tables[5];
        table.reset_type(0);
        if (selector == 0) table.constant_0x08 = KeywordReader::legacy_token_real(token(1));
        else {
            curve(table, 19, KeywordReader::decimal_token_integer(token(1)));
            status = read_property_values(reader, table);
        }
        break;
    }
    case 6: {
        auto& table = m.tables[6];
        if (planar) table.initialize(0x79);
        else {
            m.field_0x2620 = integer(v[0]);
            table.initialize(m.field_0x2620 == 1 ? 0x7a : 0x7b);
        }
        const auto offset = planar ? 0 : 1;
        const int selector = integer(v[offset]);
        m.field_0x2624 = integer(v[offset + 1]);
        table.counts_0x1c[0] = integer(v[offset + 2]);
        m.field_0x2670 = 0;
        if (selector == 1 || selector == 2) table.selectors_0x10[0] = selector == 1 ? 1 : 26;
        else if (selector < 0) {
            m.field_0x2670 = 1;
            m.field_0x2674 = absolute_reference(selector);
        } else table.selectors_0x10[0] = 1;
        if (m.field_0x2670 == 0) {
            if (!planar && m.field_0x2620 == 2) {
                const auto triple = reader.real_triple();
                std::copy(triple.values.begin(), triple.values.end(), m.values_0x2628.begin());
                const auto six = reader.real_six();
                std::copy(six.values.begin(), six.values.end(), m.values_0x2628.begin() + 3);
            }
            //Both preceding helper statuses are overwritten by this table read.
            status = read_property_values(reader, table);
        }
        break;
    }
    case 9: case 10:
        direction(0);
        m.values_0x2768[0] = v[planar ? 1 : 3];
        if (mode == 10) m.values_0x2768[2] = v[planar ? 2 : 4];
        break;
    case 11:
        std::copy_n(v.begin(), planar ? 2 : 3, m.values_0x22e8.begin());
        break;
    default:
        break; //Unknown mode succeeds;2D still cleans the retained direction.
    }
    if (planar) for (std::size_t i = 0; i < 2; ++i)
        if (std::abs(m.direction_0x21b0[i]) <= 1e-12) m.direction_0x21b0[i] = 0.0;
    return status == 0 ? 0 : 2;
}
} // namespace deform::recovered
