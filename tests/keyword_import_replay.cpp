#include "recovered/keyword_catalog.hpp"
#include "recovered/keyword_import.hpp"

#include <cmath>
#include <bit>
#include <charconv>
#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>

namespace {
using namespace deform::recovered;

unsigned nibble(char byte) {
    if (byte >= '0' && byte <= '9') return static_cast<unsigned>(byte - '0');
    if (byte >= 'a' && byte <= 'f') return static_cast<unsigned>(byte - 'a' + 10);
    if (byte >= 'A' && byte <= 'F') return static_cast<unsigned>(byte - 'A' + 10);
    throw std::runtime_error("invalid import hex");
}

std::string unhex(std::string_view text) {
    if (text == "-") return {};
    if (text.size() % 2) throw std::runtime_error("odd import hex length");
    std::string bytes;
    for (std::size_t i = 0; i < text.size(); i += 2)
        bytes.push_back(static_cast<char>((nibble(text[i]) << 4) | nibble(text[i + 1])));
    return bytes;
}

std::string hex(std::string_view bytes) {
    constexpr char digits[] = "0123456789abcdef";
    std::string text;
    for (const unsigned char byte : bytes) {
        text.push_back(digits[byte >> 4]);
        text.push_back(digits[byte & 15]);
    }
    return text;
}

void emit_quoted(std::string_view bytes) {
    constexpr char digits[] = "0123456789abcdef";
    std::cout << '"';
    for (const unsigned char byte : bytes) {
        if (byte == '"' || byte == '\\') std::cout << '\\' << static_cast<char>(byte);
        else if (byte < 32 || byte >= 127)
            std::cout << "\\u00" << digits[byte >> 4] << digits[byte & 15];
        else std::cout << static_cast<char>(byte);
    }
    std::cout << '"';
}

template<class Range>
void numbers(const Range& values) {
    std::cout << '[';
    bool first = true;
    for (const auto value : values) {
        if constexpr (std::is_floating_point_v<decltype(value)>)
            if (!std::isfinite(value)) throw std::runtime_error("nonfinite snapshot");
        if (!first) std::cout << ',';
        first = false;
        std::cout << value;
    }
    std::cout << ']';
}

template<class Range>
void known_numbers(const Range& values, std::size_t prefix) {
    std::cout << '[';
    for (std::size_t i = 0; i < values.size(); ++i) {
        if (i) std::cout << ',';
        if (i < prefix) std::cout << values[i];
        else std::cout << "null";
    }
    std::cout << ']';
}

void shape(StateVariable::Shape value) {
    std::cout << "{\"count\":" << value.count << ",\"width\":" << value.width
              << ",\"multiplicity\":" << value.multiplicity << '}';
}

void descriptors(std::span<const StateVariable> states) {
    std::cout << '[';
    bool first = true;
    for (const auto& state : states) {
        if (!first) std::cout << ',';
        first = false;
        std::cout << "{\"id\":" << state.id() << ",\"shape\":";
        shape(state.shape());
        std::cout << ",\"previous\":";
        shape(state.previous_shape());
        std::cout << ",\"values\":";
        numbers(state.values());
        std::cout << '}';
    }
    std::cout << ']';
}

template<class T, std::size_t Extent>
void raw_values(std::span<T, Extent> values) {
    using Bits = std::conditional_t<sizeof(T) == 8, std::uint64_t, std::uint32_t>;
    std::string bytes;
    bytes.reserve(values.size_bytes());
    for (const auto value : values) {
        const auto bits = std::bit_cast<Bits>(value);
        for (std::size_t byte = 0; byte < sizeof(T); ++byte)
            bytes.push_back(static_cast<char>((bits >> (byte * 8)) & 0xff));
    }
    emit_quoted(hex(bytes));
}

void custom_fields(const CustomFields& fields) {
    std::cout << '[';
    bool first = true;
    for (const auto& entry : fields.records()) {
        if (!first) std::cout << ',';
        first = false;
        std::cout << "{\"custom_id\":" << entry->custom_id() << ",\"id\":78,\"kind\":"
                  << entry->kind() << ",\"shape\":";
        shape(entry->shape());
        std::cout << ",\"previous\":";
        shape(entry->previous_shape());
        std::cout << ",\"values_hex\":";
        raw_values(entry->values());
        std::cout << '}';
    }
    std::cout << ']';
}

void dynamic_arrays(const DynamicArrays& arrays) {
    std::cout << '[';
    std::size_t index = 0;
    for (const auto& entry : arrays.records()) {
        if (index) std::cout << ',';
        std::cout << "{\"index\":" << ++index << ",\"mode_0x0c\":" << entry->mode()
                  << ",\"type_0x18\":" << entry->selector()
                  << ",\"integer_0x1c\":" << (entry->integers_selected() ? 1 : 0)
                  << ",\"discriminator_0x20\":" << entry->discriminator()
                  << ",\"count_0x24\":" << entry->count() << ",\"width_0x28\":" << entry->width()
                  << ",\"real_values_hex\":";
        raw_values(entry->real_values());
        std::cout << ",\"integer_values_hex\":";
        raw_values(entry->integer_values());
        std::cout << '}';
    }
    std::cout << ']';
}

// Same normalized mesh fields as mesh_storage_replay, without native addresses.
void mesh_fields(const MeshStorage& mesh) {
    std::cout << "\"nodes\":" << mesh.node_count()
              << ",\"elements\":" << mesh.element_count()
              << ",\"arity\":" << mesh.arity()
              << ",\"node_allocation_count\":" << mesh.node_allocation_count()
              << ",\"element_allocation_count\":" << mesh.element_allocation_count()
              << ",\"allocation_arity\":" << mesh.allocation_arity()
              << ",\"saved_shape_width_0xadc\":" << mesh.material_shape_saved_widths()[0]
              << ",\"saved_texture_width_0xae0\":" << mesh.material_shape_saved_widths()[1]
              << ",\"mesh_present\":" << (mesh.mesh_present() ? "true" : "false")
              << ",\"coordinates\":";
    known_numbers(mesh.coordinates(), mesh.coordinates_initialized_prefix());
    std::cout << ",\"connectivity\":";
    known_numbers(mesh.connectivity(), mesh.connectivity_initialized_prefix());
    std::cout << ",\"direct_integer_buffers\":{";
    bool first = true;
    for (const int id : MeshStorage::direct_integer_ids) {
        if (!first) std::cout << ',';
        first = false;
        emit_quoted(std::to_string(id));
        std::cout << ':';
        known_numbers(mesh.direct_integers(id), mesh.direct_integers_initialized_prefix(id));
    }
    std::cout << "},\"initialized_prefixes\":{\"coordinates\":"
              << mesh.coordinates_initialized_prefix()
              << ",\"connectivity\":" << mesh.connectivity_initialized_prefix();
    for (const int id : MeshStorage::direct_integer_ids)
        std::cout << ",\"" << id << "\":" << mesh.direct_integers_initialized_prefix(id);
    std::cout << "},\"descriptors\":{\"node\":";
    descriptors(mesh.node_states());
    std::cout << ",\"element\":";
    descriptors(mesh.element_states());
    std::cout << "},\"boundaries\":[";
    first = true;
    for (const auto& group : mesh.boundary_groups()) {
        if (!first) std::cout << ',';
        first = false;
        std::cout << "{\"nodes\":";
        numbers(group.nodes);
        std::cout << ",\"faces\":[";
        bool first_face = true;
        for (const auto& face : group.faces) {
            if (!first_face) std::cout << ',';
            first_face = false;
            numbers(face.nodes);
        }
        std::cout << "],\"owners\":[";
        first_face = true;
        for (const auto& face : group.faces) {
            if (!first_face) std::cout << ',';
            first_face = false;
            std::cout << face.owner_element;
        }
        std::cout << "]}";
    }
    std::cout << "],\"planar_chains\":{\"ranges\":[";
    first = true;
    for (const auto& range : mesh.planar_chains().ranges) {
        if (!first) std::cout << ',';
        first = false;
        numbers(range);
    }
    std::cout << "],\"nodes\":";
    numbers(mesh.planar_chains().nodes);
    std::cout << "},\"total\":" << mesh.boundary_total();
}

void finalization_controls(const std::optional<SimulationControls>& controls) {
    if (!controls) {
        std::cout << "null";
        return;
    }
    const auto& simulation = *controls;
    std::cout << "{\"step_number_0x8b0\":" << simulation.step_number_0x8b0
              << ",\"saved_step_0x744\":" << simulation.saved_step_0x744
              << ",\"selected_object_0x8b8\":" << simulation.selected_object_0x8b8
              << ",\"time_step_0x920\":" << simulation.time_step_0x920
              << ",\"time_step_secondary_0x928\":" << simulation.time_step_secondary_0x928
              << ",\"step_increment_0x930\":" << simulation.step_increment_0x930
              << ",\"control_0x93c\":" << simulation.control_0x93c
              << ",\"mode_0x990\":" << simulation.mode_0x990
              << ",\"pdie_secondary_0x1104\":" << simulation.pdie_secondary_0x1104 << '}';
}

void time_table(const std::optional<SimulationControls>& controls) {
    if (!controls) {
        std::cout << "null";
        return;
    }
    const auto& simulation = *controls;
    const auto& table = simulation.time_table;
    if (!std::isfinite(table.constant_0x08)) throw std::runtime_error("nonfinite table scalar");
    std::cout << "{\"type_0x00\":" << table.type_0x00
              << ",\"rank_0x04\":" << simulation.control_0x93c
              << ",\"constant_0x08\":" << table.constant_0x08
              << ",\"selectors_0x10\":";
    numbers(table.selectors_0x10);
    std::cout << ",\"counts_0x1c\":";
    numbers(table.counts_0x1c);
    std::cout << ",\"values_per_point_0x40\":" << table.values_per_point_0x40
              << ",\"flag_0x50\":" << table.flag_0x50
              << ",\"preserved_0x54\":" << table.preserved_0x54
              << ",\"mode_0x58\":" << simulation.mode_0x990
              << ",\"preserved_0x60_hex\":";
    std::string raw60;
    for (unsigned i = 0; i < 8; ++i)
        raw60.push_back(static_cast<char>((table.preserved_0x60 >> (8 * i)) & 0xff));
    emit_quoted(hex(raw60));
    std::cout << ",\"axes\":[";
    for (std::size_t i = 0; i < table.axes.size(); ++i) {
        if (i) std::cout << ',';
        numbers(table.axes[i]);
    }
    std::cout << "],\"dependent\":";
    numbers(table.dependent);
    std::cout << '}';
}

void stroke_fields(const std::array<double, 3>& values) {
    std::cout << "\"stroke_0x22d0\":";
    numbers(values);
    std::string raw;
    for (double value : values) {
        const auto bits = std::bit_cast<std::uint64_t>(value);
        for (unsigned i = 0; i < 8; ++i)
            raw.push_back(static_cast<char>((bits >> (8 * i)) & 0xff));
    }
    std::cout << ",\"stroke_0x22d0_hex\":";
    emit_quoted(hex(raw));
}

template<class Range>
void double_hex(const Range& values) {
    std::string raw;
    for (const double value : values) {
        const auto bits = std::bit_cast<std::uint64_t>(value);
        for (unsigned i = 0; i != 8; ++i)
            raw.push_back(static_cast<char>((bits >> (8 * i)) & 0xff));
    }
    emit_quoted(hex(raw));
}

void geometry_fields(const GeometryStorage& geometry, bool enabled, int flag) {
    std::cout << "{\"enabled_0x3990\":" << (enabled ? 1 : 0)
              << ",\"flag_0x3994\":" << flag
              << ",\"vertices\":" << geometry.coordinate_count()
              << ",\"face_count\":" << geometry.face_count() << ",\"coordinates\":";
    numbers(geometry.coordinates());
    std::cout << ",\"coordinates_hex\":";
    double_hex(geometry.coordinates());
    std::cout << ",\"faces\":";
    numbers(geometry.connectivity());
    std::cout << ",\"classification\":";
    numbers(geometry.classifications());
    std::cout << ",\"auxiliary68\":";
    numbers(geometry.auxiliary68());
    std::cout << ",\"planes\":[";
    bool first = true;
    for (const auto& plane : geometry.planes()) {
        if (!first) std::cout << ',';
        first = false;
        std::cout << "{\"id\":" << plane.id << ",\"point\":";
        numbers(plane.origin);
        std::cout << ",\"normal\":";
        numbers(plane.normal);
        std::cout << ",\"point_hex\":";
        double_hex(plane.origin);
        std::cout << ",\"normal_hex\":";
        double_hex(plane.normal);
        std::cout << '}';
    }
    std::cout << "]}";
}

void segment_fields(const GeometrySegments& geometry) {
    std::cout << "{\"containers\":[{\"segments\":[";
    bool first = true;
    for (const auto& segment : geometry.segments()) {
        if (!first) std::cout << ',';
        first = false;
        std::cout << "{\"id\":" << segment.id << ",\"format\":" << segment.format
                  << ",\"count\":" << segment.count << ",\"flag_0x28\":"
                  << (segment.flag_0x28 ? 1 : 0) << ",\"values\":";
        numbers(segment.values);
        std::cout << ",\"values_hex\":";
        double_hex(segment.values);
        std::cout << '}';
    }
    std::cout << "],\"matrix\":";
    numbers(geometry.matrix());
    std::cout << ",\"matrix_hex\":";
    double_hex(geometry.matrix());
    std::cout << ",\"flag_0xb8\":" << (geometry.flag_0xb8() ? 1 : 0) << "}]}";
}

void property_table(const PropertyTable& table) {
    if (!std::isfinite(table.constant_0x08)) throw std::runtime_error("nonfinite property scalar");
    std::cout << "{\"type_0x00\":" << table.type_0x00
              << ",\"rank_0x04\":" << table.rank_0x04
              << ",\"constant_0x08\":" << table.constant_0x08
              << ",\"selectors_0x10\":";
    numbers(table.selectors_0x10);
    std::cout << ",\"counts_0x1c\":";
    numbers(table.counts_0x1c);
    std::cout << ",\"values_per_point_0x40\":" << table.values_per_point_0x40
              << ",\"flag_0x50\":" << table.flag_0x50
              << ",\"preserved_0x54\":" << table.preserved_0x54
              << ",\"mode_0x58\":" << table.mode_0x58
              << ",\"preserved_0x60_hex\":";
    raw_values(std::span(&table.preserved_0x60, 1));
    std::cout << ",\"axes\":[";
    for (std::size_t axis = 0; axis < 3; ++axis) {
        if (axis) std::cout << ',';
        numbers(table.axes[axis]);
    }
    std::cout << "],\"dependent\":";
    numbers(table.dependent);
    std::cout << ",\"raw_hex\":{\"constant\":";
    raw_values(std::span(&table.constant_0x08, 1));
    std::cout << ",\"axes\":[";
    for (std::size_t axis = 0; axis < 3; ++axis) {
        if (axis) std::cout << ',';
        raw_values(std::span(table.axes[axis]));
    }
    std::cout << "],\"dependent\":";
    raw_values(std::span(table.dependent));
    std::cout << "}}";
}

void root_table_state(const PropertyTable& table, int next_index = -1) {
    // Metadata order: type,rank,selectors[3],counts[3],width,flag50,field54,mode58.
    const std::array<int, 12> metadata{table.type_0x00, table.rank_0x04,
        table.selectors_0x10[0], table.selectors_0x10[1], table.selectors_0x10[2],
        table.counts_0x1c[0], table.counts_0x1c[1], table.counts_0x1c[2],
        table.values_per_point_0x40, table.flag_0x50, table.preserved_0x54, table.mode_0x58};
    std::cout << "{\"metadata\":";
    numbers(metadata);
    std::cout << ",\"constant_hex\":";
    raw_values(std::span(&table.constant_0x08, 1));
    if (next_index < 0) {
        std::cout << ",\"preserved60_hex\":";
        raw_values(std::span(&table.preserved_0x60, 1));
    } else std::cout << ",\"next_index\":" << next_index;
    std::cout << ",\"axes_hex\":[";
    for (std::size_t i = 0; i < 3; ++i) {
        if (i) std::cout << ',';
        raw_values(std::span(table.axes[i]));
    }
    std::cout << "],\"dependent_hex\":";
    raw_values(std::span(table.dependent));
    std::cout << '}';
}

void object_motion_fields(const ObjectMotion& m) {
    const std::array<int, 11> integers{m.mode_0x21c8, m.field_0x22c0,
        m.field_0x2360, m.integers_0x25a0[0], m.integers_0x25a0[1],
        m.field_0x2620, m.field_0x2624, m.field_0x2670, m.field_0x2674,
        m.field_0x2780, m.field_0x2784};
    std::vector<double> reals;
    const auto append = [&](const auto& values) {
        reals.insert(reals.end(), values.begin(), values.end());
    };
    append(m.direction_0x21b0);
    reals.push_back(m.field_0x22c8);
    append(m.values_0x22e8);
    reals.insert(reals.end(), {m.field_0x2348, m.field_0x2350, m.field_0x2358});
    append(m.values_0x23e0);
    append(m.values_0x2478);
    append(m.values_0x2588);
    append(m.values_0x2628);
    append(m.values_0x2768);
    std::cout << "{\"integers\":";
    numbers(integers);
    std::cout << ",\"flag2490\":" << (m.flag_0x2490 ? 1 : 0) << ",\"reals_hex\":";
    raw_values(std::span(reals));
    std::cout << ",\"tables\":[";
    for (std::size_t i = 0; i < m.tables.size(); ++i) {
        if (i) std::cout << ',';
        root_table_state(m.tables[i]);
    }
    std::cout << "]}";
}

void relation_property_fields(const RelationRecord& relation) {
    const auto offset_key = [](int offset) {
        std::ostringstream key;
        key << "0x" << std::hex << offset;
        emit_quoted(key.str());
    };
    std::cout << ",\"property_tables\":{";
    bool first = true;
    for (const auto& [offset, table] : relation.property_tables) {
        if (!first) std::cout << ',';
        first = false;
        offset_key(offset);
        std::cout << ':';
        property_table(table);
    }
    std::cout << "},\"scalar_fields\":{";
    first = true;
    const auto scalar = [&](int offset, auto value) {
        if (!first) std::cout << ',';
        first = false;
        offset_key(offset);
        if constexpr (std::is_floating_point_v<decltype(value)>) {
            if (!std::isfinite(value)) throw std::runtime_error("nonfinite relation scalar");
            std::cout << ":{\"type\":\"binary64\",\"value\":" << value << ",\"raw_hex\":";
            raw_values(std::span(&value, 1));
        } else {
            std::cout << ":{\"type\":\"int32\",\"value\":" << value;
        }
        std::cout << '}';
    };
    scalar(0x6c, relation.field_0x6c);
    scalar(0xf0, relation.field_0xf0);
    scalar(0x470, relation.field_0x470);
    scalar(0x5e0, relation.field_0x5e0);
    scalar(0x5e4, relation.field_0x5e4);
    scalar(0x660, relation.field_0x660);
    scalar(0x768, relation.field_0x768);
    scalar(0x70, relation.field_0x70);
    scalar(0x170, relation.field_0x170);
    scalar(0x178, relation.field_0x178);
    scalar(0x3e8, relation.field_0x3e8);
    scalar(0x3f0, relation.field_0x3f0);
    const auto coefficients = [&](int offset, const auto& values) {
        for (const double value : values) {
            scalar(offset, value);
            offset += 8;
        }
    };
    coefficients(0x270, relation.coefficients_0x270);
    coefficients(0x340, relation.coefficients_0x340);
    coefficients(0x360, relation.coefficients_0x360);
    coefficients(0x390, relation.coefficients_0x390);
    coefficients(0x3b0, relation.coefficients_0x3b0);
    coefficients(0x668, relation.coefficients_0x668);
    std::cout << '}';
}

void fstres_fields(const deform::recovered::FstresProperties& state) {
    std::cout << "{\"mode_0x50\":" << state.mode_0x50 << ",\"coefficients_0x368\":";
    numbers(state.coefficients_0x368);
    std::cout << ",\"coefficients_0x368_hex\":";
    raw_values(std::span<const double>(state.coefficients_0x368));
    const auto key = [](int offset) {
        std::ostringstream name;
        name << "0x" << std::hex << offset;
        emit_quoted(name.str());
    };
    std::cout << ",\"property_tables\":{";
    const std::array<std::pair<int, const deform::recovered::PropertyTable*>, 10> tables{{
        {0x78, &state.table_0x78}, {0x138, &state.table_0x138},
        {0x1b0, &state.table_0x1b0}, {0x228, &state.table_0x228},
        {0x388, &state.table_0x388}, {0x400, &state.table_0x400},
        {0x480, &state.table_0x480}, {0x4f8, &state.table_0x4f8},
        {0x570, &state.table_0x570}, {0x5e8, &state.table_0x5e8}}};
    bool first = true;
    for (const auto& [offset, table] : tables) {
        if (!first) std::cout << ',';
        first = false;
        key(offset);
        std::cout << ':';
        property_table(*table);
    }
    std::cout << "},\"scalar_fields\":{";
    first = true;
    const auto scalar = [&](int offset, auto value) {
        if (!first) std::cout << ',';
        first = false;
        key(offset);
        if constexpr (std::is_floating_point_v<decltype(value)>) {
            if (!std::isfinite(value)) throw std::runtime_error("nonfinite FSTRES scalar");
            std::cout << ":{\"type\":\"binary64\",\"value\":" << value << ",\"raw_hex\":";
            raw_values(std::span(&value, 1));
        } else std::cout << ":{\"type\":\"int32\",\"value\":" << value;
        std::cout << '}';
    };
    scalar(0x478, state.field_0x478);
    scalar(0x660, state.field_0x660);
    const auto coefficients = [&](int offset, const auto& values) {
        for (const double value : values) { scalar(offset, value); offset += 8; }
    };
    coefficients(0x58, state.coefficients_0x58);
    coefficients(0xf0, state.coefficients_0xf0);
    coefficients(0x118, state.coefficients_0x118);
    coefficients(0x2a0, state.coefficients_0x2a0);
    coefficients(0x2d8, state.coefficients_0x2d8);
    coefficients(0x310, state.coefficients_0x310);
    coefficients(0x340, state.coefficients_0x340);
    std::cout << "}}";
}

void grndat_record(const deform::recovered::GrndatRecord& record) {
    std::cout << "{\"count_0x00\":" << record.count_0x00 << ",\"inline_0x08\":";
    raw_values(std::span(record.inline_0x08));
    std::cout << ",\"scalar_0x68\":";
    raw_values(std::span(&record.scalar_0x68, 1));
    std::cout << ",\"integer_0x70\":" << record.integer_0x70
              << ",\"skip_first_two_0x1e0\":" << (record.skip_first_two_0x1e0 ? 1 : 0)
              << ",\"tables\":[";
    for (std::size_t i = 0; i < record.tables.size(); ++i) {
        if (i) std::cout << ',';
        property_table(record.tables[i]);
    }
    std::cout << "]}";
}

void grndat_fields(const deform::recovered::GrndatProperties& p) {
    std::cout << "{\"flags_0x1078\":"; numbers(p.flags_0x1078);
    std::cout << ",\"tables_0x1088\":[";
    for (std::size_t i = 0; i < p.tables_0x1088.size(); ++i) {
        if (i) std::cout << ',';
        property_table(p.tables_0x1088[i]);
    }
    const std::array<double, 2> scalars{p.scalar_0x14c0, p.scalar_0x14c8};
    std::cout << "],\"scalars_0x14c0\":"; raw_values(std::span(scalars));
    std::cout << ",\"secondary_0x14d0\":" << p.secondary_0x14d0 << ",\"embedded\":";
    grndat_record(p.embedded_0x14d8);
    std::cout << ",\"lists\":[";
    for (std::size_t i = 0; i < p.lists.size(); ++i) {
        if (i) std::cout << ',';
        std::cout << '[';
        bool first = true;
        for (const auto& record : p.lists[i]) {
            if (!first) std::cout << ',';
            first = false;
            grndat_record(record);
        }
        std::cout << ']';
    }
    std::cout << "],\"coefficients_0x17b8\":"; raw_values(std::span(p.coefficients_0x17b8));
    std::cout << ",\"secondary_0x17ec\":" << p.secondary_0x17ec << '}';
}

void material_fields(const MaterialRecord& material) {
    std::cout << "{\"id\":" << material.id << ",\"name_hex\":";
    emit_quoted(hex(material.name));
    std::cout << ",\"field_0x4\":" << material.field_0x04
              << ",\"field_0x8\":" << material.field_0x08
              << ",\"field_0x17e8\":" << material.feature_0x17e8
              << ",\"mode_0x1cc8\":" << material.mode_0x1cc8
              << ",\"reference_count_0x1ccc\":" << material.count_0x1ccc
              << ",\"field_0x1e40\":" << material.feature_0x1e40
              << ",\"field_0x2150\":" << material.feature_0x2150
              << ",\"field_0x2154\":" << material.feature_0x2154
              << ",\"field_0x2158\":" << material.field_0x2158
              << ",\"field_0x215c\":" << material.field_0x215c
              << ",\"reference_ids\":";
    numbers(material.reference_ids());
    std::cout << ",\"reference_allocation\":{\"allocated\":"
              << (material.has_reference_allocation() ? "true" : "false")
              << ",\"capacity\":" << material.retained_reference_ids().size()
              << ",\"values\":";
    numbers(material.retained_reference_ids());
    std::cout << "},\"expansion_reference_0x7d8\":" << material.expansion_reference_0x7d8
              << ",\"expansion_reference_0x7d8_hex\":";
    raw_values(std::span(&material.expansion_reference_0x7d8, 1));
    std::cout << ",\"heat_capacity_mode_0x8d0\":" << material.heat_capacity_mode_0x8d0
              << ",\"coarsening_enabled_0x20d0\":" << material.coarsening_enabled_0x20d0
              << ",\"direct_fields\":{\"scalars_0x1ec8_hex\":";
    const auto& direct = material.direct;
    const std::array scalars{direct.frae2h_0x1ec8, direct.field_0x1ed0,
        direct.tmpsol_0x1ed8, direct.tmpliq_0x1ee0, direct.matden_0x1ee8};
    raw_values(std::span(scalars));
    std::cout << ",\"gbmobi_0x1868_hex\":";
    raw_values(std::span(direct.gbmobi_0x1868));
    std::cout << "},\"curve_imports\":{\"difcoe_mode_0x950\":"
              << material.curves.difcoe_mode_0x950 << ",\"difcoe_reference_0xac0\":"
              << material.curves.difcoe_reference_0xac0 << ",\"tables\":{";
    bool first_curve = true;
    const auto curve = [&](const char* key, const PropertyTable& table) {
        if (!first_curve) std::cout << ',';
        first_curve = false;
        emit_quoted(key); std::cout << ':'; property_table(table);
    };
    curve("0x1ef0", material.curves.fperv[0]);
    curve("0x1f68", material.curves.fperv[1]);
    curve("0x958", material.curves.difcoe[0]);
    curve("0x9d0", material.curves.difcoe[1]);
    curve("0xa48", material.curves.difcoe[2]);
    curve("0x2160", material.curves.difbnd_0x2160);
    std::cout << "}},\"frcmod\":{\"mode_0x1ad0\":" << material.frcmod.mode_0x1ad0
              << ",\"reference_0x1b08\":" << material.frcmod.reference_0x1b08
              << ",\"scalars_0x1ad8_hex\":";
    raw_values(std::span(material.frcmod.scalars_0x1ad8));
    std::cout << ",\"scalar_0x1b88_hex\":";
    raw_values(std::span(&material.frcmod.scalar_0x1b88, 1));
    std::cout << ",\"coefficients_0x1c80_hex\":";
    raw_values(std::span(material.frcmod.coefficients_0x1c80));
    std::cout << ",\"tables\":{";
    first_curve = true;
    curve("0x1b10", material.frcmod.tables[0]);
    curve("0x1b90", material.frcmod.tables[1]);
    curve("0x1c08", material.frcmod.tables[2]);
    std::cout << "}},\"hardening_imports\":{\"hdnrul_0x668\":"
              << material.hardening.hdnrul_0x668 << ",\"tables\":{";
    first_curve = true;
    curve("0xf10", material.hardening.hdnpha_0xf10);
    curve("0xf88", material.hardening.hdntim_0xf88);
    curve("0x1000", material.hardening.jominy_0x1000);
    std::cout << "}},\"creep\":{\"mode_0xac8\":" << material.creep.mode_0xac8
              << ",\"reference_0xda0\":" << material.creep.reference_0xda0
              << ",\"axis_count_0xda4\":" << material.creep.axis_count_0xda4
              << ",\"tables\":{";
    first_curve = true;
    constexpr std::array<const char*, 9> creep_slots{
        "0xad0", "0xb48", "0xbc0", "0xc38", "0xcb0", "0xd28", "0xda8", "0xe20", "0xe98"};
    for (std::size_t i = 0; i < creep_slots.size(); ++i)
        curve(creep_slots[i], material.creep.tables[i]);
    std::cout << "}},\"retained_tables\":{";
    first_curve = true;
    constexpr std::array<const char*, 4> retained_slots{"0x18f0", "0x1968", "0x19e0", "0x1a58"};
    for (std::size_t i = 0; i < retained_slots.size(); ++i)
        curve(retained_slots[i], material.retained_tables[i]);
    std::cout << "},\"property_tables\":{";
    bool first_table = true;
    for (const auto& [id, table] : material.property_tables) {
        if (!first_table) std::cout << ',';
        first_table = false;
        emit_quoted(std::to_string(id));
        std::cout << ':';
        property_table(table);
    }
    std::cout << "},\"fstres\":";
    fstres_fields(material.fstres);
    std::cout << ",\"grndat\":{\"selector_0x17e8\":" << material.feature_0x17e8
              << ",\"secondary_0x17ec\":" << material.grndat.secondary_0x17ec
              << ",\"property_tables\":{\"0x1740\":";
    property_table(material.grndat.table_0x1740);
    std::cout << "}},\"grndat_extended\":";
    grndat_fields(material.grndat);
    std::cout << '}';
}

template<class Range>
void materials(const Range& records) {
    std::cout << '[';
    bool first = true;
    for (const auto& record : records) {
        if (!first) std::cout << ',';
        first = false;
        if constexpr (requires { *record; }) material_fields(*record);
        else material_fields(record);
    }
    std::cout << ']';
}

void unit_controls(const RootUnitControls& unit) {
    std::cout << "{\"selector\":" << unit.selector_0x84c << ",\"scalar_hex\":[";
    const auto values = unit.scalar_values();
    for (std::size_t i = 0; i < values.size(); ++i) {
        if (i) std::cout << ',';
        raw_values(std::span(&values[i], 1));
    }
    std::cout << "],\"tables\":[";
    bool first = true;
    for (const auto& t : unit.tables) {
        if (!first) std::cout << ',';
        first = false;
        std::cout << "{\"type\":" << t.type_0x00 << ",\"rank\":" << t.rank_0x04
                  << ",\"constant_hex\":";
        raw_values(std::span(&t.constant_0x08, 1));
        std::cout << ",\"selectors\":"; numbers(t.selectors_0x10);
        std::cout << ",\"counts\":"; numbers(t.counts_0x1c);
        std::cout << ",\"width\":" << t.values_per_point_0x40 << ",\"flag50\":" << t.flag_0x50
                  << ",\"metadata54\":" << t.preserved_0x54 << ",\"mode58\":" << t.mode_0x58
                  << ",\"metadata60_hex\":";
        raw_values(std::span(&t.preserved_0x60, 1));
        std::cout << ",\"storage_null\":[";
        for (const auto& axis : t.axes) std::cout << (axis.empty() ? "true," : "false,");
        std::cout << (t.dependent.empty() ? "true,true]}" : "false,true]}");
    }
    std::cout << "]}";
}

void global_units() {
    const auto& unit = process_unit_state();
    std::cout << "{\"mode\":" << unit.mode << ",\"display_mode\":" << unit.display_mode
              << ",\"count\":" << unit.count << ",\"limit\":" << unit.limit
              << ",\"field90\":" << unit.field90 << ",\"field94\":" << unit.field94
              << ",\"table_a\":";
    if (!unit.tables) std::cout << "null";
    else {
        std::cout << '[';
        bool first = true;
        for (const auto& row : unit.tables->numeric) {
            if (!first) std::cout << ',';
            first = false;
            std::cout << "{\"id\":" << row.id << ",\"text\":"; emit_quoted(row.text);
            std::cout << ",\"raw64\":[";
            for (std::size_t i = 0; i < row.bits.size(); ++i) {
                if (i) std::cout << ',';
                raw_values(std::span(&row.bits[i], 1));
            }
            std::cout << "]}";
        }
        std::cout << ']';
    }
    std::cout << ",\"table_b\":";
    if (!unit.tables) std::cout << "null";
    else {
        std::cout << '[';
        bool first = true;
        for (const auto& row : unit.tables->labels) {
            if (!first) std::cout << ',';
            first = false;
            std::cout << "{\"id\":" << row.id << ",\"text\":[";
            for (std::size_t i = 0; i < row.text.size(); ++i) {
                if (i) std::cout << ',';
                emit_quoted(row.text[i]);
            }
            std::cout << "]}";
        }
        std::cout << ']';
    }
    std::cout << '}';
}

void state(const ObjectModel& model) {
    const auto& controls = model.controls();
    std::cout << "{\"dimension\":" << controls.dimension << ",\"suppress_boundaries\":"
              << (controls.suppress_boundaries ? 1 : 0)
              << ",\"alternate_enabled\":" << (controls.alternate_root_enabled ? 1 : 0)
              << ",\"node8_default\":" << model.node8_default()
              << ",\"direct61_default\":" << controls.direct61_default << ",\"components\":";
    numbers(controls.component_counts);
    std::cout << ",\"unit\":";
    if (controls.simulation) unit_controls(controls.simulation->units);
    else std::cout << "null";
    std::cout << ",\"global_units\":";
    global_units();
    std::cout << ",\"tnow\":";
    if (!controls.simulation) std::cout << "null";
    else {
        const auto& s = *controls.simulation;
        std::cout << "{\"field_0xd90_hex\":"; raw_values(std::span(&s.field_0xd90, 1));
        std::cout << ",\"field_0xd98_hex\":"; raw_values(std::span(&s.field_0xd98, 1));
        std::cout << ",\"field_0xda0\":" << s.field_0xda0 << ",\"field_0xda8_hex\":";
        raw_values(std::span(&s.field_0xda8, 1)); std::cout << '}';
    }
    std::cout << ",\"custom_last_id_0x33b8\":" << model.custom_last_id();
    std::cout << ",\"root_names\":{\"custom\":[";
    bool first_custom_name = true;
    for (const auto& record : model.custom_names()) {
        if (!first_custom_name) std::cout << ',';
        first_custom_name = false;
        std::cout << "{\"id\":" << record->id << ",\"name\":";
        emit_quoted(record->name);
        std::cout << ",\"labels\":[";
        for (std::size_t i = 0; i < record->labels.size(); ++i) {
            if (i) std::cout << ',';
            emit_quoted(record->labels[i]);
        }
        std::cout << "]}";
    }
    std::cout << "],\"user_text\":";
    if (!controls.simulation) std::cout << "null";
    else {
        std::cout << '[';
        const auto& records = controls.simulation->general.user_definitions_0xdd8;
        for (std::size_t i = 0; i < records.size(); ++i) {
            if (i) std::cout << ',';
            std::cout << "{\"known_prefix\":" << records[i].known_prefix << ",\"bytes_hex\":";
            emit_quoted(hex(std::string_view(reinterpret_cast<const char*>(records[i].bytes.data()),
                records[i].known_prefix)));
            std::cout << '}';
        }
        std::cout << ']';
    }
    std::cout << '}';
    std::cout << ",\"trans\":";
    if (!controls.simulation) std::cout << "null";
    else {
        const auto& s = *controls.simulation;
        const std::array<std::int32_t, 5> fields{s.flag_0x854, s.flag_0x858,
            s.material_gate_0x85c, s.flag_0x860, s.grain_gate_0x864};
        numbers(fields);
    }
    std::cout << ",\"element_layout_0x888\":";
    if (!controls.simulation) std::cout << "null";
    else {
        std::cout << '[';
        for (std::size_t i = 0; i < 10; ++i) {
            if (i) std::cout << ',';
            if (controls.simulation->element_layout_known[i])
                std::cout << controls.simulation->element_layout_0x888[i];
            else std::cout << "null";
        }
        std::cout << ']';
    }
    std::cout << ",\"element_layout_known\":";
    if (!controls.simulation) std::cout << "null";
    else {
        std::cout << '[';
        for (std::size_t i = 0; i < 10; ++i) {
            if (i) std::cout << ',';
            std::cout << (controls.simulation->element_layout_known[i] ? "true" : "false");
        }
        std::cout << ']';
    }
    std::cout << ",\"grain_mode_0x898\":";
    if (controls.simulation && controls.simulation->element_layout_known[4])
        std::cout << controls.simulation->grain_mode_0x898();
    else std::cout << "null";
    std::cout << ",\"simulation_mode\":";
    if (!controls.simulation) std::cout << "null";
    else std::cout << "{\"mode_0x844\":" << controls.simulation->mode_0x844
                   << ",\"flag_0x854\":" << controls.simulation->flag_0x854
                   << ",\"flag_0x858\":" << controls.simulation->flag_0x858 << '}';
    std::cout << ",\"mesh_number_0x840\":";
    if (controls.simulation) std::cout << controls.simulation->mesh_number_0x840;
    else std::cout << "null";
    std::cout << ",\"environment\":";
    if (!controls.simulation) std::cout << "null";
    else {
        const auto& simulation = *controls.simulation;
        std::cout << "{\"enabled\":" << (simulation.furnace.enabled_0xb10 ? 1 : 0)
                  << ",\"tables\":[";
        for (std::size_t i = 0; i < simulation.units.tables.size(); ++i) {
            if (i) std::cout << ',';
            root_table_state(simulation.units.tables[i]);
        }
        std::cout << "],\"parameters\":[";
        const auto& parameters = simulation.furnace.parameters;
        for (std::size_t i = 0; i < parameters.size(); ++i) {
            if (i) std::cout << ',';
            std::cout << "{\"table\":";
            root_table_state(parameters[i].table,
                i + 1 == parameters.size() ? 0 : static_cast<int>(i + 2));
            std::cout << ",\"known_bytes_hex\":";
            const auto& known = parameters[i].known_dependent_bytes;
            emit_quoted(hex(std::string(known.begin(), known.end())));
            std::cout << '}';
        }
        std::cout << "]}";
    }
    std::cout << ",\"root_fields\":";
    if (!controls.simulation) std::cout << "null";
    else {
        const auto& g = controls.simulation->general;
        const auto& u = controls.simulation->units;
        const std::array<double, 11> reals{
            g.geometry_errors_0xdb0[0], g.geometry_errors_0xdb0[1],
            g.geometry_errors_0xdb0[2], g.geometry_errors_0xdb0[3],
            g.peninf_0xaf8, u.scalars[0], g.tintgf_0xb08, u.scalars[1],
            u.scalars[2], u.scalars[3], g.ehrate_0xd88};
        std::cout << "{\"reals_hex\":";
        raw_values(std::span(reals));
        std::cout << ",\"name\":";
        emit_quoted(g.frnnam_0xb90);
        std::cout << ",\"unit_selector\":" << u.selector_0x84c << '}';
    }
    std::cout << ",\"root_record_tables\":";
    if (!controls.simulation) std::cout << "null";
    else {
        const auto& r = controls.simulation->record_tables;
        std::cout << "{\"frequency_rows\":" << r.frequency_rows_0xa78
                  << ",\"frequency_columns\":" << r.frequency_columns_0xa7c
                  << ",\"frequency\":";
        root_table_state(r.frequency_0xa80);
        std::cout << ",\"records\":[";
        for (std::size_t i = 0; i < r.records.size(); ++i) {
            if (i) std::cout << ',';
            root_table_state(r.records[i]);
        }
        std::cout << "]}";
    }
    std::cout << ",\"iteration\":";
    if (!controls.simulation) std::cout << "null";
    else {
        const auto& i = controls.simulation->iteration;
        const std::array<int, 9> words{i.itrmth_0xa30, i.solmtd_0xa34, i.parameter_count_0xa38,
            i.itrmxd_0xa60, i.defbwd_0xa64, i.solmtt_0xa68, i.itrmxt_0xa6c,
            i.tmpbwd_0xa70, i.latent_0xa74};
        std::array<double, 6> reals;
        std::copy(i.cvgerr_0xa20.begin(), i.cvgerr_0xa20.end(), reals.begin());
        std::copy(i.parameters_0xa40.begin(), i.parameters_0xa40.end(), reals.begin() + 2);
        std::cout << "{\"integers\":";
        numbers(words);
        std::cout << ",\"reals_hex\":";
        raw_values(std::span(reals));
        std::cout << '}';
    }
    std::cout << ",\"dimensional_step\":";
    if (!controls.simulation) std::cout << "null";
    else {
        const auto& d = controls.simulation->dimensional;
        const std::array<double, 3> reals{d.dtsub_0xa10, d.dplen_0xa18, d.odmax_0x1318};
        std::cout << "{\"vfactr_mode\":" << d.vfactr_mode_0x1128 << ",\"reals_hex\":";
        raw_values(std::span(reals));
        std::cout << ",\"planar\":";
        if (!d.planar) std::cout << "null";
        else {
            std::cout << "{\"osctrl\":";
            numbers(d.planar->osctrl_0x1108);
            std::cout << ",\"cnterr_int\":" << d.planar->cnterr_0x1118 << ",\"cnterr_real_hex\":";
            raw_values(std::span(&d.planar->cnterr_0x1120, 1));
            std::cout << '}';
        }
        std::cout << ",\"spatial\":";
        if (!d.spatial) std::cout << "null";
        else {
            std::cout << "{\"active_count\":" << d.spatial->active_count_0x112c << ",\"table\":";
            property_table(d.spatial->table_0x1130);
            std::cout << '}';
        }
        std::cout << '}';
    }
    std::cout << ",\"time_step_imports\":";
    if (!controls.simulation) std::cout << "null";
    else {
        const auto& s = controls.simulation->time_step_imports;
        std::array<double, 9> values{s.slderr_0x9b8};
        std::copy(s.dtpmax.begin(), s.dtpmax.end(), values.begin() + 1);
        std::copy(s.dvmax_0x9c8.begin(), s.dvmax_0x9c8.end(), values.begin() + 6);
        values[8] = s.mdsobj_0x9e8;
        std::cout << "{\"reals_hex\":";
        raw_values(std::span(values));
        std::cout << ",\"mdsobj\":";
        numbers(s.mdsobj_0x9d8);
        std::cout << '}';
    }
    std::cout << ",\"stopping_vectors_hex\":";
    if (!controls.simulation) std::cout << "null";
    else {
        const auto& s = controls.simulation->stopping;
        std::array<double, 9> values;
        std::copy(s.smax_0x8d0.begin(), s.smax_0x8d0.end(), values.begin());
        std::copy(s.vmin_0x8e8.begin(), s.vmin_0x8e8.end(), values.begin() + 3);
        std::copy(s.lmax_0x900.begin(), s.lmax_0x900.end(), values.begin() + 6);
        raw_values(std::span(values));
    }
    std::cout << ",\"stopping_controls\":";
    if (!controls.simulation) std::cout << "null";
    else {
        const auto& s = controls.simulation->stopping;
        const std::array<double, 3> reals{s.emax_0x8c0, s.tmax_0x8c8, s.demax_0x9b0};
        std::cout << "{\"nstep\":" << s.nstep_0x8bc << ",\"stpdef\":" << s.stpdef_0x918
                  << ",\"reals_hex\":";
        raw_values(std::span(reals));
        std::cout << '}';
    }
    std::cout << ",\"stpinc_0x8b4\":";
    if (controls.simulation) std::cout << controls.simulation->stpinc_0x8b4;
    else std::cout << "null";
    std::cout << ",\"heat_method_0x868\":";
    if (controls.simulation) std::cout << controls.simulation->method_0x868;
    else std::cout << "null";
    std::cout << ",\"simulation_type_0x848\":";
    if (controls.simulation) std::cout << controls.simulation->type_0x848;
    else std::cout << "null";
    std::cout << ",\"geometry_mode\":";
    if (controls.simulation) std::cout << controls.simulation->geometry_type;
    else std::cout << "null";
    std::cout << ",\"output_flags\":";
    if (controls.simulation) numbers(controls.simulation->output_flags);
    else std::cout << "null";
    std::cout << ",\"finalization_controls\":";
    finalization_controls(controls.simulation);
    std::cout << ",\"time_table\":";
    time_table(controls.simulation);
    std::cout << ",\"user_fields\":";
    if (!controls.simulation) std::cout << "null";
    else {
        const auto& simulation = *controls.simulation;
        std::cout << "{\"node_count\":" << simulation.user_node_width_0x3398
                  << ",\"element_count\":" << simulation.user_element_width_0x33a8;
        const auto records = [](const auto& definitions) {
            std::cout << '[';
            bool first = true;
            for (const auto& record : definitions) {
                if (!first) std::cout << ',';
                first = false;
                emit_quoted(hex(std::string_view(reinterpret_cast<const char*>(record.data()), record.size())));
            }
            std::cout << ']';
        };
        std::cout << ",\"node_records_hex\":";
        records(simulation.user_node_definitions);
        std::cout << ",\"element_records_hex\":";
        records(simulation.user_element_definitions);
        std::cout << '}';
    }
    std::cout << ",\"material_count\":" << model.material_count() << ",\"materials\":";
    materials(model.materials());
    std::cout << ",\"material_gate_0x85c\":";
    if (controls.simulation) std::cout << controls.simulation->material_gate_0x85c;
    else std::cout << "null";
    std::cout << ",\"relation_count_0x13f4\":" << model.relation_count()
              << ",\"relation_list\":{\"count\":" << model.relations().size() << ",\"records\":[";
    bool first_relation = true;
    for (const auto& relation : model.relations()) {
        if (!first_relation) std::cout << ',';
        first_relation = false;
        std::cout << "{\"key0\":" << relation->key0 << ",\"key1\":" << relation->key1
                  << ",\"mode_0x08\":" << relation->mode_0x08
                  << ",\"active_0x0c\":" << relation->active_0x0c
                  << ",\"field_0x10\":" << relation->field_0x10
                  << ",\"field_0x14\":" << relation->field_0x14 << ",\"name_hex\":";
        emit_quoted(hex(relation->name));
        std::cout << ",\"name_terminator_hex\":\"00\"";
        relation_property_fields(*relation);
        std::cout << '}';
    }
    std::cout << "]}";
    std::cout << ",\"import_flag_0x2f88\":" << (model.import_flag_0x2f88() ? 1 : 0)
              << ",\"import_flag_0x2f89\":" << (model.import_flag_0x2f89() ? 1 : 0)
              << ",\"object_count\":" << model.objects().size() << ",\"objects\":[";
    bool first = true;
    for (const auto& object : model.objects()) {
        if (!first) std::cout << ',';
        first = false;
        std::cout << "{\"id\":" << object->id << ",\"type\":" << object->type
                  << ",\"secondary\":" << object->secondary
                  << ",\"material_group_0x3980\":" << object->material_group_0x3980
                  << ",\"flag_0x3990\":" << (object->flag_0x3990 ? 1 : 0)
                  << ",\"element_kind\":" << object->element_kind << ",\"name_hex\":";
        emit_quoted(hex(object->name));
        std::cout << ",\"prsnam_0x2160_hex\":";
        emit_quoted(hex(object->prsnam_0x2160));
        std::cout << ",\"motion\":";
        object_motion_fields(object->motion);
        std::cout << ",\"direct_imports\":{\"scalars_0x1ea0_hex\":";
        raw_values(std::span(object->direct.scalars_0x1ea0));
        std::cout << ",\"planar\":";
        if (!object->direct.planar) std::cout << "null";
        else {
            const auto& p = *object->direct.planar;
            std::cout << "{\"penvol_mode_0x1f20\":" << p.penvol_mode_0x1f20
                      << ",\"zstr_mode_0x1f30\":" << p.zstr_mode_0x1f30
                      << ",\"zstr_0x1f28_hex\":";
            raw_values(std::span(&p.zstr_0x1f28, 1));
            std::cout << '}';
        }
        std::cout << '}';
        std::cout << ",\"temperature_limit_0x1f10_hex\":";
        raw_values(std::span(&object->direct.temperature_limit_0x1f10, 1));
        std::cout << ",\"remeshing_scalars_hex\":";
        raw_values(std::span(object->remeshing_scalars.values));
        std::cout << ",\"rotation_symmetry\":";
        if (!object->direct.rotation_symmetry) std::cout << "null";
        else {
            const auto& r = *object->direct.rotation_symmetry;
            std::cout << "{\"scalar_0x2058_hex\":"; raw_values(std::span(&r.scalar_0x2058, 1));
            std::cout << ",\"vector_0x2060_hex\":"; raw_values(std::span(r.vector_0x2060));
            std::cout << ",\"vector_0x2078_hex\":"; raw_values(std::span(r.vector_0x2078));
            std::cout << '}';
        }
        std::cout << ",\"target_volume\":{\"mode_0x2098\":"
                  << object->direct.target_volume_mode_0x2098 << ",\"value_0x2090_hex\":";
        raw_values(std::span(&object->direct.target_volume_0x2090, 1));
        std::cout << '}';
        std::cout << ",\"direct_integer_imports\":";
        numbers(object->direct.integers);
        const auto& range = object->direct.output_range;
        std::cout << ",\"output_range\":{\"mode_0x1eec\":" << range.mode_0x1eec
                  << ",\"reference_0x1ef0\":" << range.reference_0x1ef0
                  << ",\"option_0x1f08\":" << range.option_0x1f08 << ",\"values_0x1ef8_hex\":";
        raw_values(std::span(range.values_0x1ef8));
        std::cout << '}';
        std::cout << ',';
        stroke_fields(object->stroke_0x22d0);
        std::cout << ",\"geometry\":";
        geometry_fields(object->geometry, object->flag_0x3990, object->flag_0x3994);
        std::cout << ",\"geometry_segments\":";
        segment_fields(object->geometry_segments);
        std::cout << ",\"custom_fields\":";
        custom_fields(object->custom_fields);
        std::cout << ",\"dynamic_arrays\":";
        dynamic_arrays(object->dynamic_arrays);
        std::cout << ',';
        mesh_fields(object->mesh);
        std::cout << '}';
    }
    std::cout << "]}";
}

// The observer runs synchronously in this single-threaded replay process.
// Reuse the complete serializer so later output cannot read mutated objects.
std::string snapshot_state(const ObjectModel* model) {
    if (!model) return "null";
    std::ostringstream output;
    struct RestoreBuffer {
        std::streambuf* previous;
        ~RestoreBuffer() { std::cout.rdbuf(previous); }
    } restore{std::cout.rdbuf(output.rdbuf())};
    state(*model);
    return output.str();
}

void result(const KeywordImportResult& value) {
    std::cout << "{\"outcome\":";
    emit_quoted(value.outcome == KeywordImportResult::Outcome::parsed ? "parsed" : "unsupported");
    std::cout << ",\"status\":" << value.status << ",\"cursor\":" << value.cursor
              << ",\"reached_eof\":" << (value.reached_eof ? "true" : "false")
              << ",\"finalization_pending\":" << (value.finalization_pending ? "true" : "false")
              << ",\"unsupported_reason\":";
    emit_quoted(value.unsupported_reason);
    std::cout << ",\"events\":[";
    bool first = true;
    for (const auto& event : value.events) {
        if (!first) std::cout << ',';
        first = false;
        std::cout << "{\"token\":";
        emit_quoted(event.token);
        std::cout << ",\"id\":" << event.id << ",\"category\":" << event.category
                  << ",\"offset\":" << event.offset << ",\"cursor\":" << event.cursor
                  << ",\"handler_status\":";
        if (event.handler_status) std::cout << *event.handler_status;
        else std::cout << "null";
        std::cout << ",\"unknown_warning\":" << (event.unknown_warning ? "true" : "false")
                  << ",\"skipped_line\":";
        emit_quoted(event.skipped_line);
        if (!event.diagnostics.empty()) {
            std::cout << ",\"diagnostics\":[";
            for (std::size_t i = 0; i < event.diagnostics.size(); ++i) {
                if (i) std::cout << ',';
                emit_quoted(event.diagnostics[i]);
            }
            std::cout << ']';
        }
        std::cout << '}';
    }
    std::cout << "]}";
}

} // namespace

// stdin: legacy DIMENSION NODE8_DEFAULT DIRECT61_DEFAULT SUPPRESS_BOUNDARIES,
// then79 component integers; or initialized DIMENSION NODE8_DEFAULT SUPPRESS.
// The latter explicitly selects the recovered empty reset-root profile.
// Follow with sequential "import HEX_BYTES" commands (or "link DIMENSION").
// '-' represents an empty import. Inputs must already be text-translated.
// One model persists for the complete request stream. This is an oracle driver,
// not a file-opening or deployment interface.
int main() {
    try {
        RootControls controls{};
        int suppress = 0;
        std::string first;
        if (!(std::cin >> first)) throw std::runtime_error("missing root profile");
        if (first == "initialized") {
            if (!(std::cin >> controls.dimension >> controls.node8_default >> suppress) ||
                (suppress != 0 && suppress != 1))
                throw std::runtime_error("invalid initialized root controls");
            controls = initialized_root_controls(controls.dimension, controls.node8_default,
                                                 suppress != 0);
        } else {
            const auto parsed = std::from_chars(first.data(), first.data() + first.size(),
                                                controls.dimension);
            if (parsed.ec != std::errc{} || parsed.ptr != first.data() + first.size() ||
                !(std::cin >> controls.node8_default >> controls.direct61_default >> suppress) ||
                (suppress != 0 && suppress != 1))
                throw std::runtime_error("missing or invalid explicit root controls");
            controls.suppress_boundaries = suppress != 0;
            for (auto& width : controls.component_counts)
                if (!(std::cin >> width)) throw std::runtime_error("missing component count");
        }
        ObjectModel model(controls);
        KeywordImporter importer(model, keyword_catalog);
        struct StrokeSnapshot {
            int id;
            int type;
            std::array<double, 3> values;
        };
        struct HandlerSnapshot {
            KeywordEvent event;
            int dimension;
            std::optional<SimulationControls> controls;
            bool flag88;
            bool flag89;
            std::vector<StrokeSnapshot> object_strokes;
            struct GeometrySnapshot {
                int id;
                int type;
                bool enabled;
                int flag;
                GeometryStorage data;
            };
            std::vector<GeometrySnapshot> object_geometries;
            struct SegmentSnapshot {
                int id;
                int type;
                GeometrySegments data;
            };
            std::vector<SegmentSnapshot> object_segments;
            std::int32_t material_count = 0;
            std::vector<MaterialRecord> materials;
            std::array<int, 79> components;
            std::string state_json;
            std::string linked_state_json;
        };
        std::vector<HandlerSnapshot> handler_states;
        importer.set_handler_observer([&](const KeywordEvent& event, const ObjectModel& observed) {
            HandlerSnapshot snapshot{event, observed.controls().dimension,
                observed.controls().simulation, observed.import_flag_0x2f88(),
                observed.import_flag_0x2f89(), {}};
            for (const auto& object : observed.objects())
                snapshot.object_strokes.push_back({object->id, object->type, object->stroke_0x22d0});
            for (const auto& object : observed.objects())
                snapshot.object_geometries.push_back({object->id, object->type,
                    object->flag_0x3990, object->flag_0x3994, object->geometry});
            for (const auto& object : observed.objects())
                snapshot.object_segments.push_back({object->id, object->type, object->geometry_segments});
            snapshot.material_count = observed.material_count();
            for (const auto& material : observed.materials())
                snapshot.materials.push_back(*material);
            snapshot.components = observed.controls().component_counts;
            snapshot.state_json = snapshot_state(&observed);
            snapshot.linked_state_json = snapshot_state(observed.linked_root());
            handler_states.push_back(std::move(snapshot));
        });
        std::unique_ptr<ObjectModel> linked;
        std::map<int, const ObjectRecord*> identities;
        std::map<int, const MaterialRecord*> material_identities;
        auto identity_generation = model.generation();
        std::cout << std::setprecision(17);
        std::string operation, encoded;
        while (std::cin >> operation) {
            if (operation == "link") {
                int dimension;
                if (!(std::cin >> dimension)) throw std::runtime_error("missing link dimension");
                linked = std::make_unique<ObjectModel>(
                    initialized_root_controls(dimension, controls.node8_default));
                model.link_root(linked.get());
                continue;
            }
            if (operation != "import" || !(std::cin >> encoded))
                throw std::runtime_error("expected import HEX_BYTES");
            // Emit the pre-import state before mutation; the complete line is
            // terminated only after the result and post-import state are ready.
            std::cout << "{\"before_state\":";
            state(model);
            std::cout << ",\"before_linked_state\":";
            if (linked) state(*linked); else std::cout << "null";
            handler_states.clear();
            // Keep application stdout separate from this driver's JSON protocol.
            // These are logical stream bytes before Windows text translation.
            // snapshot_state temporarily redirects the stream again in the
            // handler observer and restores this buffer on return.
            std::ostringstream native_stdout;
            auto* protocol_stream = std::cout.rdbuf(native_stdout.rdbuf());
            KeywordImportResult imported;
            try {
                imported = importer.import_translated(unhex(encoded));
            } catch (...) {
                std::cout.rdbuf(protocol_stream);
                throw;
            }
            std::cout.rdbuf(protocol_stream);
            if (identity_generation != model.generation()) {
                identities.clear();
                material_identities.clear();
                identity_generation = model.generation();
            }
            for (const auto& object : model.objects()) {
                const auto [position, inserted] = identities.emplace(object->id, object.get());
                if (!inserted && position->second != object.get())
                    throw std::runtime_error("object identity changed");
            }
            for (const auto& material : model.materials()) {
                const auto [position, inserted] = material_identities.emplace(material->id, material.get());
                if (!inserted && position->second != material.get())
                    throw std::runtime_error("material identity changed");
            }
            std::cout << ",\"native_stdout_hex\":";
            emit_quoted(hex(native_stdout.str()));
            std::cout << ",\"result\":";
            result(imported);
            std::cout << ",\"handler_states\":[";
            bool first_handler = true;
            for (const auto& snapshot : handler_states) {
                if (!first_handler) std::cout << ',';
                first_handler = false;
                std::cout << "{\"id\":" << snapshot.event.id
                          << ",\"category\":" << snapshot.event.category
                          << ",\"status\":" << *snapshot.event.handler_status
                          << ",\"cursor\":" << snapshot.event.cursor
                          << ",\"dimension\":" << snapshot.dimension
                          << ",\"finalization_controls\":";
                finalization_controls(snapshot.controls);
                std::cout << ",\"time_table\":";
                time_table(snapshot.controls);
                std::cout << ",\"import_flag_0x2f88\":" << (snapshot.flag88 ? 1 : 0)
                          << ",\"import_flag_0x2f89\":" << (snapshot.flag89 ? 1 : 0)
                          << ",\"object_strokes\":[";
                bool first_stroke = true;
                for (const auto& object : snapshot.object_strokes) {
                    if (!first_stroke) std::cout << ',';
                    first_stroke = false;
                    std::cout << "{\"id\":" << object.id << ",\"type\":" << object.type << ',';
                    stroke_fields(object.values);
                    std::cout << '}';
                }
                std::cout << "],\"object_geometries\":[";
                bool first_geometry = true;
                for (const auto& object : snapshot.object_geometries) {
                    if (!first_geometry) std::cout << ',';
                    first_geometry = false;
                    std::cout << "{\"id\":" << object.id << ",\"type\":" << object.type << ",\"geometry\":";
                    geometry_fields(object.data, object.enabled, object.flag);
                    std::cout << '}';
                }
                std::cout << "],\"object_segments\":[";
                bool first_segment = true;
                for (const auto& object : snapshot.object_segments) {
                    if (!first_segment) std::cout << ',';
                    first_segment = false;
                    std::cout << "{\"id\":" << object.id << ",\"type\":" << object.type
                              << ",\"geometry_segments\":";
                    segment_fields(object.data);
                    std::cout << '}';
                }
                std::cout << "],\"material_count\":" << snapshot.material_count
                          << ",\"materials\":";
                materials(snapshot.materials);
                std::cout << ",\"components\":";
                numbers(snapshot.components);
                std::cout << ",\"state\":" << snapshot.state_json
                          << ",\"linked_state\":" << snapshot.linked_state_json << '}';
            }
            std::cout << ']';
            std::cout << ",\"state\":";
            state(model);
            std::cout << ",\"stable_identity\":true,\"stable_object_identity\":true,\"stable_material_identity\":true,\"generation\":" << model.generation()
                      << ",\"linked_generation\":";
            if (linked) std::cout << linked->generation(); else std::cout << "null";
            std::cout << ",\"linked_state\":";
            if (linked) state(*linked); else std::cout << "null";
            std::cout << "}\n";
        }
        return 0;
    } catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
        return 1;
    }
}
