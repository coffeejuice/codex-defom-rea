#include "furnace_model.hpp"

#include <algorithm>
#include <bit>
#include <cmath>
#include <cstring>
#include <limits>
#include <optional>
#include <span>
#include <stdexcept>

namespace deform::recovered {
namespace {
#include "furnace_source_data.inc"

std::optional<std::size_t> flat_index(const PropertyTable& table, int row, int column) {
    //130530, initialized rank1 profiles used by the furnace defaults.
    if (table.rank_0x04 != 1 || row < 0 || column < 0 ||
        row >= std::max(1, table.counts_0x1c[0]) ||
        column >= std::max(1, table.values_per_point_0x40)) return {};
    const auto index = static_cast<std::int64_t>(row) * table.values_per_point_0x40 + column;
    if (index < 0 || static_cast<std::uint64_t>(index) >= table.dependent.size()) return {};
    return static_cast<std::size_t>(index);
}

double get(const PropertyTable& table, int row, int column) {
    const auto index = flat_index(table, row, column);
    return index ? table.dependent[*index] : 0.0;
}

void put(PropertyTable& table, double value, int row, int column) {
    const auto index = flat_index(table, row, column);
    if (!index) return;
    const double magnitude = std::abs(value);
    table.dependent[*index] = magnitude > 1e-30 && magnitude < 1e30 ? value : 0.0;
}

double factor(int quantity, int direction) {
    if (quantity < 0 || quantity >= static_cast<int>(unit_factors.size()) ||
        (direction != 1 && direction != 2))
        throw std::invalid_argument("unrecovered furnace unit factor profile");
    return std::bit_cast<double>(unit_factors[quantity][direction - 1]);
}

double temperature(int direction, double value) {
    return direction == 1 ? value * 1.8 + 32.0 : (value - 32.0) / 1.8;
}

void property_units(PropertyTable& table, int quantity, int direction,
                    bool axes, bool reciprocal = false) {
    //12e400/12e6d0; only the rank1 owned default profiles are consumed here.
    if (table.rank_0x04 != 1 || table.counts_0x1c[0] < 1 ||
        table.values_per_point_0x40 != 1) return;
    if (axes) {
        const int selector = table.selectors_0x10[0];
        if (selector < 0 || selector >= static_cast<int>(axis_unit_codes.size()))
            throw std::invalid_argument("unrecovered furnace axis unit selector");
        const int unit = axis_unit_codes[selector];
        for (double& value : table.axes[0])
            value = unit == 9 ? temperature(direction, value) : value * factor(unit, direction);
    }
    const auto convert = [&](double value) {
        if (quantity == 9) {
            const double converted = temperature(direction, value);
            return reciprocal ? 1.0 / converted : converted;
        }
        const double scale = factor(quantity, direction);
        return value * (reciprocal || table.type_0x00 == 0x2a ? 1.0 / scale : scale);
    };
    table.constant_0x08 = convert(table.constant_0x08);
    for (double& value : table.dependent) value = convert(value);
}

void convert_defaults(FurnaceCollection& collection, int direction) {
    const auto referenced = [&](int index) -> PropertyTable& {
        auto* parameter = collection.lookup(index);
        if (!parameter) throw std::out_of_range("furnace default reference outside collection");
        return parameter->table;
    };
    for (std::size_t index = 0; index < collection.parameters.size(); ++index) {
        auto& table = collection.parameters[index].table;
        const int width = table.values_per_point_0x40;
        const auto scale = [&](int row, int column, int quantity) {
            put(table, get(table, row, column) * factor(quantity, direction), row, column);
        };
        const auto temp = [&](int row, int column) {
            put(table, temperature(direction, get(table, row, column)), row, column);
        };
        const auto references = [&](int row) {
            for (int column = 2; column <= 5; ++column) {
                const int id = native_furnace_integer(get(table, row, column));
                property_units(referenced(id), column == 5 ? 0x2a : 0x15 + column,
                               direction, true);
            }
        };
        switch (index + 1) {
        case 1: scale(0, 3, 0x28); break;
        case 2: scale(0, 0, 0x29); temp(0, 1); temp(0, 2); break;
        case 3:
            for (int column = 0; column < 6; ++column) scale(0, column, 1);
            break;
        case 4: temp(0, 3); break;
        case 5:
            put(table, get(table, 0, 1) * factor(0x28, direction) / factor(4, direction), 0, 1);
            temp(0, 2);
            break;
        case 6:
            scale(0, 0, 3); scale(1, 3 - width, 4);
            //3769f0 writes both converted values into the same original slot.
            put(table, temperature(direction, get(table, 1, 4 - width)), 1, 3 - width);
            put(table, temperature(direction, get(table, 1, 5 - width)), 1, 3 - width);
            break;
        case 7:
            //The source computes a heat conversion at0/3, then discards it.
            scale(0, 5, 1); scale(0, 6, 1); scale(1, 10 - width, 4);
            temp(1, 11 - width); temp(1, 12 - width); scale(2, 21 - width * 2, 0x29);
            references(2); references(3);
            {
                auto& target = referenced(native_furnace_integer(get(table, 4, 2)));
                property_units(target, 5, direction, true);
                property_units(target, 3, direction, false, true);
            }
            break;
        case 8:
            scale(2, 2, 0x14); scale(3, 0, 0x18); temp(3, 2); scale(3, 3, 0x14);
            put(table, get(table, 4, 3) * factor(0x27, direction) / factor(4, direction), 4, 3);
            temp(5, 0); temp(5, 1); temp(5, 2); scale(5, 4, 0x13); scale(6, 0, 1);
            put(table, get(table, 6, 1) / factor(9, direction), 6, 1);
            temp(9, 1); temp(9, 3); scale(10, 0, 0x14); scale(10, 1, 0x14);
            break;
        case 9:
            for (int row = 0; row < table.counts_0x1c[0]; ++row) scale(row, 1, 6);
            break;
        case 11:
            table.constant_0x08 = temperature(direction, table.constant_0x08);
            property_units(table, 9, direction, true);
            break;
        case 14:
            for (int row = 0; row < table.counts_0x1c[0]; ++row) {
                for (int column = 12; column < 18; ++column) scale(row, column, 1);
                for (int column = 18; column < width; ++column) scale(row, column, 3);
            }
            break;
        case 15:
        case 17:
            for (int row = 0; row < table.counts_0x1c[0]; ++row) {
                scale(row, 1, index + 1 == 15 ? 0x29 : 1);
                references(row);
            }
            break;
        default: break;
        }
    }
}
}

int native_furnace_integer(double value) noexcept {
    return !std::isfinite(value) || value < -2147483648.0 || value >= 2147483648.0
        ? std::numeric_limits<std::int32_t>::min() : static_cast<int>(value);
}

FurnaceParameter* FurnaceCollection::lookup(int index) noexcept {
    if (parameters.empty()) return nullptr;
    const auto selected = static_cast<std::size_t>(std::max(1, index) - 1);
    return selected < parameters.size() ? &parameters[selected] : nullptr;
}

void FurnaceCollection::reset_entries(int requested) {
    parameters.clear();
    const int count = std::max(21, requested);
    parameters.reserve(static_cast<std::size_t>(count));
    for (int index = 0; index < count; ++index)
        parameters.emplace_back(index < 21 ? 0x8e + index : 0xa3);
}

void FurnaceCollection::initialize_defaults(int process_unit_mode) {
    enabled_0xb10 = false;
    reset_entries(22);
    for (std::size_t index = 0; index < parameters.size(); ++index) {
        auto& table = parameters[index].table;
        table.rank_0x04 = 1;
        table.selectors_0x10[0] = 0;
        table.counts_0x1c[0] = index < 20 ? default_layout[index][3] : 0;
        table.values_per_point_0x40 = index < 20 ? default_layout[index][4] : 0;
        if (index < 10 || index == 13) table.allocate_zeroed();
        if (index == 10) table.selectors_0x10[0] = 1;
    }
    for (const auto& value : default_writes)
        put(parameters[static_cast<std::size_t>(value.parameter - 1)].table,
            std::bit_cast<double>(value.bits), value.row, value.column);
    if (process_unit_mode == 0) convert_defaults(*this, 2);
}

FurnaceHeaderRead read_furnace_header(KeywordReader& reader) {
    FurnaceHeaderRead result;
    auto line = reader.line();
    if (line.status != 0) return result;
    std::replace(line.bytes.begin(), line.bytes.end(), ',', ' ');
    KeywordReader fields(std::string_view(line.bytes.c_str()));
    int conversions = 0;
    for (auto& value : result.integers) {
        const auto parsed = fields.selector_integer();
        if (parsed.conversions != 1) {
            result.conversions = conversions ? conversions : parsed.conversions;
            result.status = result.conversions == -1 ? 1 : 0;
            return result;
        }
        value = parsed.values[0]; ++conversions;
    }
    for (auto& value : result.reals) {
        const auto parsed = fields.stream_real();
        if (parsed.conversions != 1) break;
        value = parsed.values[0]; ++conversions;
    }
    result.conversions = conversions;
    result.status = 0;
    return result;
}

int read_furnace_parameter(KeywordReader& reader, FurnaceParameter& parameter) {
    const auto header = read_furnace_header(reader); //Status deliberately ignored.
    auto& table = parameter.table;
    table.rank_0x04 = header.integers[1];
    if (table.rank_0x04 == 0) {
        table.values_per_point_0x40 = header.integers[2];
        table.allocate_zeroed();
        parameter.known_dependent_bytes.clear();
        if (table.values_per_point_0x40 < 2) table.constant_0x08 = header.reals[0];
    } else if (table.rank_0x04 == 1) {
        table.selectors_0x10[0] = header.integers[2];
        table.counts_0x1c[0] = native_furnace_integer(header.reals[0]);
        if (table.counts_0x1c[0] == 0) {
            table.constant_0x08 = header.reals[1];
            table.values_per_point_0x40 = 0;
        } else table.values_per_point_0x40 = native_furnace_integer(header.reals[1]);
        table.allocate_zeroed();
        parameter.known_dependent_bytes.clear();
    } else return 1;
    const auto read = [&](double& value) {
        const auto parsed = reader.stream_real(value);
        if (parsed.conversions == 1) value = parsed.values[0];
        return parsed.conversions != -1;
    };
    if (table.rank_0x04 == 0) {
        for (auto& value : table.dependent) if (!read(value)) return 1;
    } else {
        const auto width = static_cast<std::size_t>(table.values_per_point_0x40);
        for (std::size_t row = 0; row < table.axes[0].size(); ++row) {
            if (!read(table.axes[0][row])) return 1;
            for (std::size_t column = 0; column < width; ++column)
                if (!read(table.dependent[row * width + column])) return 1;
        }
    }
    if (table.dependent_count() > 0) (void)reader.line();
    return 0;
}

int read_furnace_text(KeywordReader& reader, FurnaceParameter& parameter) {
    const auto header = read_furnace_header(reader);
    auto& table = parameter.table;
    table.rank_0x04 = header.integers[1];
    if (table.rank_0x04 == 0) return 0;
    if (table.rank_0x04 != 1) return 1;
    table.selectors_0x10[0] = header.integers[2];
    table.counts_0x1c[0] = native_furnace_integer(header.reals[0]);
    table.values_per_point_0x40 = native_furnace_integer(header.reals[1]);
    table.allocate_zeroed();
    auto bytes = std::as_writable_bytes(std::span(table.dependent));
    parameter.known_dependent_bytes.assign(bytes.size(), std::uint8_t{1});
    std::optional<std::string> previous;
    for (int row = 0; row < table.counts_0x1c[0]; ++row) {
        const auto offset = static_cast<std::size_t>(row) * 64;
        if (offset + 64 > bytes.size())
            throw std::invalid_argument("native furnace text would write beyond its dependent allocation");
        const auto parsed = reader.furnace_text_line(previous.value_or(std::string{}));
        if (parsed.status == 0) previous = parsed.bytes;
        if (!previous) {
            std::fill_n(parameter.known_dependent_bytes.begin() + offset, 64, std::uint8_t{0});
            continue;
        }
        std::fill_n(bytes.begin() + offset, 64, std::byte{0x20});
        std::memcpy(bytes.data() + offset, previous->data(), previous->size());
    }
    return 0;
}

} // namespace deform::recovered
