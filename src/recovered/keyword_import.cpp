#include "keyword_import.hpp"

#include "keyword_reader.hpp"
#include "relation_properties.hpp"
#include "fstres_properties.hpp"
#include "grndat_properties.hpp"

#include <algorithm>
#include <array>
#include <bit>
#include <cmath>
#include <limits>
#include <optional>
#include <stdexcept>
#include <utility>

namespace deform::recovered {
namespace {

bool whitespace(unsigned char byte) {
    return byte == ' ' || byte == '\t' || byte == '\n' ||
           byte == '\r' || byte == '\v' || byte == '\f';
}

std::string_view word(std::string_view text, std::size_t& cursor) {
    while (cursor < text.size() && whitespace(static_cast<unsigned char>(text[cursor])))
        ++cursor;
    const auto start = cursor;
    while (cursor < text.size() && !whitespace(static_cast<unsigned char>(text[cursor])))
        ++cursor;
    if (cursor - start > 255)
        throw std::invalid_argument("header word exceeds recovered safe buffer domain");
    return text.substr(start, cursor - start);
}

std::optional<int> header_dimension(std::string_view remainder) {
    // 1b6520 parses the remainder AFTER the '*' token, once per recognized
    // header. These spellings come from its explicit strcmp branches.
    std::size_t cursor = 0;
    const auto first = word(remainder, cursor);
    const auto second = word(remainder, cursor);
    if (first == "DEFORM") {
        if (second.empty())
            throw std::invalid_argument("incomplete DEFORM header is uncharacterized");
        if (second == "3D" || second == "F3") return 3;
        if (second == "2D" || second == "F2") return 2;
    }
    if (first == "DEFORM-3D" || first == "DEFORM-F3") return 3;
    if (first == "DEFORM-2D" || first == "DEFORM-F2" ||
        first == "DEFORM-MACH2" || first == "DEFORM-DIESTRESS2" ||
        first == "DEFORM-MACHDIST2" || first == "DEFORM-MICROSTRUCTURE2")
        return 2;
    return {};
}

std::string unsupported_controls(const RootControls& controls) {
    if (controls.dimension != 2 && controls.dimension != 3)
        return "root dimension must be 2 or 3";
    if (controls.alternate_root_enabled)
        return "alternate-root dispatch is not implemented";
    if (controls.simulation && std::any_of(controls.simulation->element_layout_known.begin(),
            controls.simulation->element_layout_known.end(), [](bool known) { return !known; }))
        return "ELMNOD control state contains indeterminate native values; reset the model before importing";
    if (controls.element_multiplicities != std::array<int, 3>{1, 1, 1})
        return "nonunit element multiplicities are not implemented";
    if (controls.component_counts[0] != controls.dimension)
        return "coordinate component count does not match explicit root dimension";
    if (std::any_of(controls.component_counts.begin(), controls.component_counts.end(),
                    [](int width) { return width < 0; }))
        return "negative component counts are outside the compatibility domain";
    if (controls.node8_default != 20.0 && controls.node8_default != 68.0)
        return "node ID 8 default must be explicitly 20 or 68";
    return {};
}

enum class FieldStorage { node_real, element_real, node_integer };

struct FieldKeyword {
    const char* name;
    int keyword_id;
    int component_id;
    FieldStorage storage;
    int reader_mode;
};

// Fixed slots in 38b8a0. Component IDs select the live root width, not a
// guessed scalar/vector shape; allocation and header row counts are distinct.
constexpr FieldKeyword field_keywords[] = {
    {"DRZ", 160, 1, FieldStorage::node_real, 6},
    {"BCCDEF", 162, 18, FieldStorage::node_integer, 0},
    {"BCCDFN", 163, 19, FieldStorage::node_integer, 0},
    {"URZ", 165, 2, FieldStorage::node_real, 6},
    {"FRZ", 166, 3, FieldStorage::node_real, 0},
    {"PRZ", 167, 5, FieldStorage::node_real, 0},
    {"BCCTMP", 168, 20, FieldStorage::node_integer, 0},
    {"BCCTFN", 169, 21, FieldStorage::node_integer, 0},
    {"NDHEAT", 171, 9, FieldStorage::node_real, 0},
    {"NDFLUX", 172, 11, FieldStorage::node_real, 0},
    {"STNCMP", 176, 40, FieldStorage::element_real, 1},
    {"DENSTY", 177, 44, FieldStorage::element_real, 0},
    {"DVOLF", 206, 51, FieldStorage::element_real, 1},
    {"TICF", 207, 52, FieldStorage::element_real, 1},
    {"VOTAGE", 213, 15, FieldStorage::node_real, 0},
    {"DATOM", 214, 13, FieldStorage::node_real, 0},
    {"HDNOBJ", 217, 56, FieldStorage::element_real, 0},
    {"BCCCRB", 219, 22, FieldStorage::node_integer, 0},
    {"BCCRHT", 220, 23, FieldStorage::node_integer, 0},
    {"CRBFLX", 221, 14, FieldStorage::node_real, 0},
    {"RHTFLX", 222, 16, FieldStorage::node_real, 0},
    {"YLDS", 233, 57, FieldStorage::element_real, 0},
    {"ZEFI", 237, 58, FieldStorage::element_real, 0},
    {"ZMFI", 238, 59, FieldStorage::element_real, 0},
    {"CURRNT", 239, 62, FieldStorage::element_real, 0},
};

const FieldKeyword* field_keyword(int id) {
    for (const auto& field : field_keywords)
        if (field.keyword_id == id) return &field;
    return nullptr;
}

bool property_table_keyword(int id) {
    switch (id) {
    case 95: case 96: case 97: case 98: case 99: case 100: case 115:
    case 121: case 122: case 125: case 126: case 131: case 133:
        return true;
    default: return false;
    }
}

bool implemented(int id, int category) {
    return (category == 1 && ((id >= 22 && id <= 38) || id == 48 || id == 49 || id == 50 || id == 51)) ||
           (category == 2 && (field_keyword(id) || id == 144 || id == 145 || id == 153 || id == 158 || id == 159 || id == 170 || id == 173 || id == 174 || id == 196 || id == 197 || id == 198 || id == 205 || id == 210 || id == 211 || id == 216 || id == 254 || id == 255 || id == 263 || id == 266)) ||
           (category == 4 && (id == 92 || id == 94 || id == 108 || id == 116 || id == 127 || id == 128 || property_table_keyword(id))) ||
           (category == 5 && id >= 134 && id <= 143);
}

int handle_configuration_keyword(int id, ObjectModel& model, KeywordReader& reader) {
    if (id >= 22 && id <= 25) {
        auto& metadata = model.metadata();
        if (id == 23) {
            // 39aa90 passes live destinations to 413fb0, which zeros them
            // before fgets. Matching failure0 and partial assignments succeed.
            metadata.cursim = {};
            const auto read = reader.integer_triple();
            std::copy_n(read.values.begin(), 3, metadata.cursim.begin());
            return read.status == 0 ? 0 : 2;
        }
        auto& destination = id == 22 ? metadata.title
            : id == 24 ? metadata.operation_name : metadata.simulation_name;
        const auto read = id == 24 ? reader.material_name(destination)
                                   : reader.name(destination);
        // The helper retains the previous string on either failed line read.
        destination = read.bytes;
        return read.status == 0 ? 0 : 2;
    }
    // 39aa90: GEOTYP is ignored in3D, leaving the remainder for token scanning.
    if (id == 28 && model.controls().dimension == 3) return 0;
    if (!model.controls().simulation)
        throw std::invalid_argument(
            "configuration refresh requires explicit reset controls and recovered material properties");
    if (id == 26) {
        // 39aa90 case4 passes root+848 directly to 413de0. The reader
        // zeroes the live destination, including EOF and scan-1 failures.
        const auto read = reader.integer();
        model.set_simulation_type(read.values[0]);
        return read.status == 0 ? 0 : 2;
    }
    if (id == 27) {
        const auto read = reader.integer();
        model.set_simulation_mode(read.values[0]); // Also after EOF/scan-1.
        return read.status == 0 ? 0 : 2;
    }
    if (id == 28) {
        const auto read = reader.integer();
        model.set_geometry_type(read.values[0]); // Also on EOF/partial conversion.
        return read.status == 0 ? 0 : 2;
    }
    if (id == 29) {
        // 39aa90 case7: supported ordinary wrapper option0 reads directly
        // into root840, including zero on failure. Mask bit2 is not exposed.
        const auto read = reader.integer();
        model.set_mesh_number(read.values[0]);
        return read.status == 0 ? 0 : 2;
    }
    if (id == 30) {
        const auto read = reader.integer(); // 39aa90 reads a temporary.
        model.set_unit(read.values[0]); // Change condition is independent of status.
        return read.status == 0 ? 0 : 2;
    }
    if (id == 31) {
        const auto read = reader.real_quad();
        model.set_tnow(read.values); // Handler mutates after failed/partial reads too.
        return read.status == 0 ? 0 : 2;
    }
    if (id == 32) {
        const auto read = reader.connectivity4(false); // 414200, skip0.
        if (read.status == 0) {
            std::array<int, 5> values;
            std::copy_n(read.values.begin(), 5, values.begin());
            model.set_trans(values);
        }
        return read.status == 0 ? 0 : 2;
    }
    if (id == 37) {
        const auto read = reader.integer(); // 39aa90 case15,413de0 skip0.
        model.set_stpinc(read.values[0]); // Live destination is zeroed on failure.
        return read.status == 0 ? 0 : 2;
    }
    if (id == 33) {
        const auto read = reader.integer(); // 413de0 skip0, live root868.
        model.set_heat_method(read.values[0]); // Zero is stored even on failure.
        return read.status == 0 ? 0 : 2;
    }
    if (id == 35) {
        const auto header = reader.integer();
        const auto payload = reader.integer_array10(); // Runs even after header failure.
        if (payload.status == 0) {
            const int copied = std::clamp(header.values[0], 0, 10);
            const int known = std::min(copied, std::max(payload.conversions, 0));
            model.set_element_layout_prefix(std::span(payload.values).first(known));
            if (known != copied) {
                model.mark_element_layout_indeterminate(known, copied);
                throw std::runtime_error("ELMNOD copies uninitialized native payload values outside the compatibility domain");
            }
        }
        model.refresh_components(); // Payload return status is deliberately ignored.
        return header.status == 0 ? 0 : 2;
    }
    if (id == 34) {
        // 414600: nine base0 integers, no blank-line retry. Only seven are
        // persistent root fields; the last two destinations are temporaries.
        const auto read = reader.connectivity8(false);
        std::array<int, 7> flags;
        std::copy_n(read.values.begin(), flags.size(), flags.begin());
        model.set_output_flags(flags); // Zeroed/partial results remain visible.
        return read.status == 0 ? 0 : 2;
    }
    if (id == 36) {
        const auto read = reader.integer();
        model.set_start_step(read.values[0], read.status == 0);
        return read.status == 0 ? 0 : 2;
    }
    if (id == 38) {
        const auto read = reader.integer_pair();
        model.set_pdie(read.values[0], read.values[1]);
        return read.status == 0 ? 0 : 2;
    }
    if (id == 48) {
        const auto read = reader.real();
        model.set_displacement_step(read.values[0]);
        return read.status == 0 ? 0 : 2;
    }
    if (id == 49) {
        const auto read = reader.real_integer();
        // The original parent never stores the helper return status; its
        // local status remains zero even after EOF or matching failure.
        model.set_step_distance(read.real_value, read.integer_value);
        return 0;
    }
    if (id == 50) {
        const auto read = reader.real_pair();
        model.set_time_steps(read.values);
        return read.status == 0 ? 0 : 2;
    }
    if (id == 51) {
        const auto header = reader.integer_pair();
        if (header.status != 0) return 2; // Existing table is untouched.
        auto& table = model.initialize_time_table(header.values[0], header.values[1]);
        // 41a800: rank1 reads each axis coordinate followed by its dependent
        // value. Matching failure0 leaves the zero/previous slot and continues;
        // input failure-1 returns immediately, preserving all earlier writes.
        for (std::size_t row = 0; row < table.axes[0].size(); ++row) {
            const auto axis = reader.stream_real(table.axes[0][row]);
            table.axes[0][row] = axis.values[0];
            if (axis.conversions == -1) return 2;
            const auto value = reader.stream_real(table.dependent[row]);
            table.dependent[row] = value.values[0];
            if (value.conversions == -1) return 2;
        }
        if (!table.dependent.empty()) (void)reader.line();
        return 0;
    }
    throw std::logic_error("unimplemented configuration keyword reached dispatch");
}

int handle_material_keyword(int id, ObjectModel& model, KeywordReader& reader) {
    if (id != 92 && id != 94 && id != 108 && id != 116 && id != 127 && id != 128 && !property_table_keyword(id))
        throw std::logic_error("unimplemented material keyword reached dispatch");
    const auto selector = reader.selector_integer();
    if (selector.conversions == -1) return 2;
    if (selector.conversions != 1)
        throw std::invalid_argument(
            "material selector matching failure uses an uninitialized native ID and is unsupported");
    auto* material = model.find_or_create_material(selector.values[0]);
    if (id == 94) return read_fstres(material->fstres, reader);
    if (id == 108) {
        const auto read = read_grndat(*material, reader);
        if (read.refresh) model.refresh_components();
        return read.status;
    }
    if (property_table_keyword(id)) {
        auto& table = material->property_tables.at(id);
        if (id == 126) {
            const auto enabled = reader.integer();
            material->coarsening_enabled_0x20d0 = enabled.values[0];
            if (enabled.values[0] <= 0) return enabled.status == 0 ? 0 : 2;
        }
        // EXPAND writes directly only when the header supplies its auxiliary
        // field. HEATCP always commits its initialized local, including EOF.
        double heat_capacity_mode = 0.0;
        double* auxiliary = id == 97 ? &material->expansion_reference_0x7d8
                                    : id == 99 ? &heat_capacity_mode : nullptr;
        const auto result = read_property(reader, table, auxiliary ? 1 : 0, auxiliary);
        if (id == 99) {
            const double value = std::trunc(heat_capacity_mode);
            if (!std::isfinite(value) || value < std::numeric_limits<std::int32_t>::min() ||
                value > std::numeric_limits<std::int32_t>::max())
                throw std::invalid_argument("HEATCP auxiliary outside finite signed32 domain");
            material->heat_capacity_mode_0x8d0 = static_cast<std::int32_t>(value);
        }
        // PMEAB changes the second selector even when the table reader fails.
        if (id == 121 && table.rank_0x04 == 2) table.selectors_0x10[1] = 20;
        return result.status == 0 ? 0 : 2;
    }
    if (id == 92) {
        const auto read = reader.material_name(material->name);
        if (read.status != 0) return 2;
        material->name = read.bytes;
        return 0;
    }

    if (id == 127) {
        const auto read = reader.integer();
        material->feature_0x2150 = read.values[0];
        return read.status == 0 ? 0 : 2;
    }
    if (id == 128) {
        const auto read = reader.integer_triple();
        material->feature_0x2154 = read.values[0];
        material->field_0x2158 = read.values[1];
        material->field_0x215c = read.values[2];
        // Native destinations are zeroed and partially written even on EOF.
        // Neither property handler refreshes the component table afterwards.
        return read.status == 0 ? 0 : 2;
    }

    // 384a50 case18 saves the old count, then413de0 zeros and writes the
    // live count field. Failure returns before touching mode or allocation.
    const auto previous_count = model.begin_material_reference_count(*material);
    const auto count = reader.integer();
    model.apply_material_reference_count(*material, previous_count,
        count.values[0], count.status);
    if (count.status != 0) return 2;

    // The native %Flf destination is one uninitialized local double reused
    // throughout this loop. A later matching failure preserves its last
    // successful value; a first matching failure has no defined value to copy.
    std::optional<double> previous_value;
    for (int index = 0; index < material->count_0x1ccc; ++index) {
        const auto read = reader.stream_real(previous_value.value_or(0.0));
        if (read.conversions == -1) return 2;
        if (read.conversions == 0 && !previous_value)
            throw std::invalid_argument(
                "first material reference matching failure uses an uninitialized native value");
        previous_value = read.values[0];
        const double reference = std::trunc(*previous_value);
        if (!std::isfinite(reference) ||
            reference < std::numeric_limits<std::int32_t>::min() ||
            reference > std::numeric_limits<std::int32_t>::max())
            throw std::invalid_argument(
                "material reference conversion outside finite signed32 domain");
        model.write_material_reference(*material, static_cast<std::size_t>(index),
            static_cast<std::int32_t>(reference));
    }
    // No line discard or eager layout refresh follows the reference list.
    return 0;
}

int handle_relation_keyword(int id, ObjectModel& model, KeywordReader& reader) {
    if (id < 134 || id > 143) throw std::logic_error("unimplemented relation keyword reached dispatch");
    const auto keys = reader.selector_pair();
    if (keys.conversions == -1) return 2;
    if (keys.conversions != 2)
        throw std::invalid_argument("incomplete relation selector uses uninitialized native keys");
    auto& relation = model.find_or_create_relation(keys.values[0], keys.values[1]);
    if (id != 134) return read_relation_property(id, relation, reader);
    const auto read = reader.integer();
    model.apply_relation_mode(relation, read.values[0]); // Includes zeroed failure output.
    return read.status == 0 ? 0 : 2;
}

int handle_object_keyword(int id, ObjectModel& model, KeywordReader& reader) {
    const auto selector = reader.selector_integer();
    if (selector.status != 0) return 2;
    ObjectRecord* object = model.find_or_create(selector.values[0]);
    if (!object) return 2;
    const auto& controls = model.controls();

    if (id == 210 || id == 211) {
        const auto header = reader.shape_header();
        if (header.status != 0) return 2;
        const int component = id == 210 ? 54 : 55;
        model.set_material_shape_width(component, header.width);
        // 417c80 rejects selector1 for these two components after writing
        // the root width, but before changing the descriptor or saved widths.
        if (header.selector != 0) return 2;
        auto values = model.prepare_material_shape(*object, component);
        const int rows = object->mesh.element_count();
        if (rows < 0 || rows > std::numeric_limits<int>::max() / header.width)
            throw std::invalid_argument("material-shape extent exceeds native signed32 domain");
        const auto extent = static_cast<std::size_t>(rows) * header.width;
        if (extent > values.size())
            throw std::invalid_argument("material-shape payload exceeds initialized storage");
        if (header.conversions > 3)
            std::fill_n(values.begin(), extent, header.default_value);
        const auto read = reader.real_rows_explicit(values, header.rows, rows, header.width);
        return read.status == 0 ? 0 : 2;
    }

    if (id == 198) {
        const auto header = reader.custom_variable_header();
        if (header.status != 0) return 2;
        auto& variable = object->custom_fields.find_or_create(header.integers[0]);
        variable.configure(header.integers[1], header.integers[3], header.integers[2],
            object->mesh.node_count(), object->mesh.element_count());
        variable.fill(header.default_value);
        const auto shape = variable.shape();
        const auto read = reader.real_rows(variable.values(), header.integers[2],
            shape.count, shape.width);
        // The root write is success-only; default and partial row mutations
        // above survive failure independently of that root field.
        if (read.status != 0) return 2;
        model.select_custom_variable(header.integers[0]);
        return 0;
    }

    if (id == 266) {
        const auto header = reader.dynamic_array_header();
        if (header.status != 0) return 2;
        auto* array = object->dynamic_arrays.select(header.integers[0]);
        if (!array) return 0;
        // The original clamps token1 to1 for every signed input value.
        array->configure(header.integers[2], header.integers[3], header.integers[4]);
        const int width = array->width();
        if (array->discriminator() == 1 && !array->integers_selected()) {
            if (width != 2 && width != 3) return 0;
            auto values = array->real_values();
            for (int row = 0; row < array->count(); ++row) {
                const auto read = width == 2 ? reader.coordinates2() : reader.coordinates3();
                // These helpers zero their destination row before scanning;
                // partial output is written even when the helper then fails.
                std::copy_n(read.values.begin(), width,
                    values.begin() + static_cast<std::size_t>(row) * width);
                if (read.status != 0) return 2;
            }
        } else if (array->discriminator() == 2 && array->integers_selected()) {
            if (width != 3 && width != 4 && width != 8) return 0;
            auto values = array->integer_values();
            for (int row = 0; row < array->count(); ++row) {
                const auto read = width == 3 ? reader.connectivity3() :
                    width == 4 ? reader.connectivity4(false) : reader.connectivity8(false);
                // The parsed label is a local; rows are placed sequentially.
                std::copy_n(read.values.begin() + 1, width,
                    values.begin() + static_cast<std::size_t>(row) * width);
                if (read.status != 0) return 2;
            }
        } else if (array->discriminator() == 3 && !array->integers_selected()) {
            array->fill_real(header.default_value);
            const auto read = reader.real_rows(array->real_values(), header.integers[5],
                array->count(), width);
            return read.status == 0 ? 0 : 2;
        }
        return 0;
    }

    if (id == 196 || id == 197) {
        const auto header = reader.integer_pair_real();
        if (header.status != 0) return 2;
        const bool nodes = id == 196;
        const int requested_width = !nodes && header.integers[1] == 0 ? 2 : header.integers[1];
        model.set_user_field_width(nodes, requested_width);
        const int count = nodes ? object->mesh.node_count() : object->mesh.element_count();
        const int width = controls.component_counts[nodes ? 17 : 60];
        if (count < 0 || width < 0 || (width != 0 && count > std::numeric_limits<int>::max() / width))
            throw std::invalid_argument("user-field default extent exceeds native signed32 domain");
        const auto extent = static_cast<std::size_t>(count) * width;
        for (std::size_t index = 0; index < extent; ++index) {
            // Native code calls the getter for every default slot. Its
            // selective allocator updates shape history even at equal size.
            auto values = model.user_field_values(*object, nodes);
            if (index >= values.size())
                throw std::invalid_argument("user-field header exceeds getter allocation");
            values[index] = header.real_value;
        }
        auto values = model.user_field_values(*object, nodes);
        const auto read = reader.real_rows(values, header.integers[0], count, width);
        return read.status == 0 ? 0 : 2;
    }

    if (id == 254) {
        auto values = model.prepare_material_axes(*object);
        const auto read = reader.real_columns_mode2(values, object->mesh.element_count(),
            model.material_axes_width());
        return read.status == 0 ? 0 : 2;
    }

    if (id == 255) {
        auto values = model.prepare_node_wear(*object);
        // Native null getter skips the entire payload and returns status0.
        if (values.empty()) return 0;
        const auto read = reader.real_columns(values, object->mesh.node_count(),
            controls.component_counts[24], 0);
        return read.status == 0 ? 0 : 2;
    }

    if (id == 263) {
        // DRMESH is ignored in2D, without consuming the payload. In3D the
        // fixed node25 slot follows the ordinary in-place mode0 reader.
        if (controls.dimension != 3) return 0;
        const auto read = reader.real_columns(object->mesh.node_values(25),
            object->mesh.node_count(), controls.component_counts[25], 0);
        return read.status == 0 ? 0 : 2;
    }

    if (id == 170) {
        auto temperature = object->mesh.node_values(8);
        const int count = object->mesh.node_count();
        if (controls.dimension == 3) {
            const auto read = reader.real_columns(temperature, count,
                controls.component_counts[8], 0);
            return read.status == 0 ? 0 : 2;
        }
        const auto header = reader.integer_pair_real();
        if (header.status != 0) return 2;
        auto difference = object->mesh.node_values(12);
        if (count < 0 || static_cast<std::size_t>(count) > temperature.size() ||
            static_cast<std::size_t>(count) > difference.size())
            throw std::invalid_argument("planar temperature prefixes exceed owned node buffers");
        // Native loops fill count scalar slots, independently of root widths.
        std::fill_n(temperature.begin(), count, header.real_value);
        std::fill_n(difference.begin(), count, 0.0);
        if (header.integers[1] != 2) {
            const auto read = reader.real_rows(temperature, header.integers[0],
                count, controls.component_counts[8]);
            return read.status == 0 ? 0 : 2;
        }
        if (count > std::numeric_limits<int>::max() / 2)
            throw std::invalid_argument("planar temperature temporary exceeds native signed32 extent");
        std::vector<double> paired(static_cast<std::size_t>(count) * 2, header.real_value);
        const auto read = reader.real_rows(paired, header.integers[0], count, 2);
        // This conversion is unconditional after4199a0, including its failure.
        for (int row = 0; row < count; ++row) {
            const auto offset = static_cast<std::size_t>(row) * 2;
            temperature[row] = paired[offset + 1];
            difference[row] = paired[offset + 1] - paired[offset];
            if (!std::isfinite(difference[row]))
                throw std::invalid_argument("temperature difference outside finite compatibility domain");
        }
        return read.status == 0 ? 0 : 2;
    }

    if (const auto* field = field_keyword(id)) {
        const int width = controls.component_counts[field->component_id];
        if (field->storage == FieldStorage::node_integer) {
            auto values = object->mesh.direct_integers(field->component_id);
            const auto read = reader.integer_columns(values, object->mesh.node_count(), width);
            object->mesh.record_direct_integer_writes(field->component_id, read.written);
            // BCCDEF's spatial conversion follows the reader even on failure.
            // It reads triples independently of the reader's width argument.
            if (id == 162 && controls.dimension == 3) {
                const auto count = static_cast<std::size_t>(object->mesh.node_count());
                if (count > static_cast<std::size_t>(std::numeric_limits<std::int32_t>::max() / 3) ||
                    count > values.size() / 3 ||
                    count > object->mesh.direct_integers_initialized_prefix(18) / 3)
                    throw std::invalid_argument("BCCDEF postprocessing requires initialized node triples");
                for (std::size_t row = 0; row < count; ++row) {
                    auto triple = values.subspan(row * 3, 3);
                    if (triple[0] < 0 && triple[0] == triple[1] && triple[1] == triple[2]) {
                        // Native SUB EDX,0xc8 wraps at32bits, without a checked
                        // overflow branch. Unsigned arithmetic avoids C++ UB.
                        triple[0] = std::bit_cast<std::int32_t>(
                            std::bit_cast<std::uint32_t>(triple[0]) - std::uint32_t{200});
                        triple[2] = 0;
                        triple[1] = 0;
                    }
                }
            }
            return read.status == 0 ? 0 : 2;
        }
        const bool node = field->storage == FieldStorage::node_real;
        auto values = node ? object->mesh.node_values(field->component_id)
                           : object->mesh.element_values(field->component_id);
        const auto read = reader.real_columns(values,
            node ? object->mesh.node_count() : object->mesh.element_count(), width, field->reader_mode);
        return read.status == 0 ? 0 : 2;
    }

    if (id == 174) {
        if (!controls.simulation)
            throw std::invalid_argument("material assignments require the initialized object profile");
        const auto read = reader.integer_columns(object->mesh.direct_integers(61),
            object->mesh.element_count(), controls.component_counts[61]);
        object->mesh.record_direct_integer_writes(61, read.written);
        // 38b8a0 always calls 202960, including after reader failure. Defaults
        // and completed rows already written by 419d40/419ea0 remain live.
        model.finalize_material_groups(*object);
        return read.status == 0 ? 0 : 2;
    }

    if (id == 205 || id == 216) {
        // 398350 selects input columns from element material assignments and
        // current references. 3981c0 reads into zeroed temporary storage; a
        // failed reader leaves the live destination unchanged.
        const auto mapping = model.material_column_mapping(*object);
        const auto read = reader.material_columns(object->mesh.element_count(), mapping.input_width);
        if (read.status != 0) return 2;
        model.apply_material_columns(*object, id == 205 ? 49 : 50, mapping, read.values);
        return 0;
    }

    if (id == 144) {
        const auto result = reader.name(object->name);
        if (result.status != 0) return 2;
        object->name = result.bytes;
        return 0;
    }
    if (id == 145) {
        const auto result = reader.integer_pair();
        if (result.status != 0) return 2;
        object->apply_type(result.values[0], result.values[1]);
        return 0;
    }
    if (id == 153) {
        if (controls.dimension == 3) {
            const auto read = reader.real_triple();
            object->stroke_0x22d0 = read.values;
            return read.status == 0 ? 0 : 2;
        }
        const auto read = reader.real_pair();
        std::copy_n(read.values.begin(), 2, object->stroke_0x22d0.begin());
        return read.status == 0 ? 0 : 2;
    }
    if (id == 158) {
        auto& segments = object->geometry_segments;
        if (controls.dimension == 2) {
            const auto header = reader.integer_pair();
            if (header.status != 0) return 2;
            if (header.values[0] != 1 && header.values[0] != 2) return 2;
            // Planar dispatch preserves existing segments, including trailing
            // groups that the helper does not replace. Any helper error maps2.
            return segments.import_segments(reader, header.values[0], header.values[1]) == 0
                ? 0 : 2;
        }
        const auto header = reader.integer_triple();
        if (header.status != 0) return 2;
        if (header.values[0] == 3)
            return segments.import_transformed(reader, header.values[1], header.values[2]) == 0
                ? 0 : 2;
        if (header.conversions == 2) {
            if (header.values[0] != 1 && header.values[0] != 2) return 2;
            // 39019e..39020d: clear only a nonempty collection, preserving its
            // matrix. The native call deliberately does not store/test EAX.
            if (!segments.segments().empty()) segments.clear();
            (void)segments.import_segments(reader, header.values[0], header.values[1]);
            return 0;
        }
        auto& geometry = object->geometry;
        geometry.begin_replace(header.values[1], header.values[2]);
        // Nonpositive requested counts retain stored lengths. The original
        // loops over those effective lengths, not the requested counts.
        for (int row = 0; row < geometry.coordinate_count(); ++row) {
            const auto read = reader.coordinates3();
            geometry.write_coordinate(static_cast<std::size_t>(row), read.values);
            if (read.status != 0) return 2;
        }
        if (header.values[0] == 1) {
            for (int row = 0; row < geometry.face_count(); ++row) {
                const auto read = reader.connectivity4(false);
                std::array<std::int32_t, 4> face;
                std::copy_n(read.values.begin() + 1, 4, face.begin());
                geometry.write_face(static_cast<std::size_t>(row), face);
                if (read.status != 0) return 2;
            }
        }
        object->flag_0x3994 = 1;
        object->flag_0x3990 = true;
        return 0;
    }
    if (id == 159) {
        const auto count = reader.integer();
        if (count.status != 0) return 2;
        object->mesh.begin_coordinate_replace(count.values[0], controls.component_counts,
                                               model.node8_default());
        const auto dimensions = static_cast<std::size_t>(controls.dimension);
        for (int row = 0; row < count.values[0]; ++row) {
            const auto result = controls.dimension == 2
                ? reader.coordinates2() : reader.coordinates3();
            auto destination = object->mesh.coordinates().subspan(
                static_cast<std::size_t>(row) * dimensions, dimensions);
            // Readers zero outputs before trying fgets. Even the failing row's
            // zero/partial outputs belong in the live destination array.
            std::copy_n(result.values.begin(), dimensions, destination.begin());
            if (result.status != 0) return 2;
        }
        return 0;
    }
    if (id == 173) {
        int count = 0;
        int arity = 4;
        if (controls.dimension == 3) {
            const auto result = reader.integer_pair();
            if (result.status != 0) return 2;
            count = result.values[0];
            arity = result.values[1];
            if (arity != 4 && arity != 8) return 2;
            // 165500 precedes count replacement/allocation in the handler.
            object->element_kind = arity == 4 ? 2 : 4;
        } else {
            const auto result = reader.integer();
            if (result.status != 0) return 2;
            count = result.values[0];
        }
        object->mesh.begin_connectivity_replace(count, arity, controls.component_counts,
            controls.simulation ? std::max(object->material_group_0x3980, 0) : controls.direct61_default);
        const auto width = static_cast<std::size_t>(arity);
        for (int row = 0; row < count; ++row) {
            const auto result = arity == 4
                ? reader.connectivity4() : reader.connectivity8();
            auto destination = object->mesh.connectivity().subspan(
                static_cast<std::size_t>(row) * width, width);
            std::copy_n(result.values.begin() + 1, width, destination.begin());
            if (result.status != 0) return 2;
        }
        // Success marks the mesh even at zero count. Boundary suppression is
        // separate from mesh marking (39aa20 reads root byte +1b5c).
        object->mesh.finalize_connectivity(controls.dimension, !controls.suppress_boundaries);
        return 0;
    }
    throw std::logic_error("unimplemented handler reached internal dispatch");
}

} // namespace

KeywordImporter::KeywordImporter(ObjectModel& model, std::span<const KeywordDefinition> catalog)
    : model_(model) {
    if (catalog.size() != 304)
        throw std::invalid_argument("complete recovered 304-row keyword catalog required");
    catalog_.reserve(catalog.size());
    for (const auto& definition : catalog) {
        if (definition.name.empty())
            throw std::invalid_argument("empty keyword catalog entry");
        catalog_.push_back({definition.id, definition.category, std::string(definition.name)});
    }
    constexpr KeywordDefinition required_keywords[] = {
        {"GEOTYP", 28, 1}, {"STNOUT", 34, 1}, {"NSTART", 36, 1}, {"PDIE", 38, 1},
        {"DSMAX", 48, 1}, {"DSMAX_NSTEP", 49, 1}, {"DTMAX", 50, 1}, {"DTTYPE", 51, 1},
        {"MTNAME", 92, 4}, {"FSTRES", 94, 4}, {"GRNDAT", 108, 4}, {"MSTMTR", 116, 4}, {"PHASTF", 134, 5},
        {"OBJNAM", 144, 2}, {"OBJTYP", 145, 2}, {"STROKE", 153, 2}, {"DIEGEO", 158, 2}, {"RZ", 159, 2}, {"ELMCON", 173, 2},
        {"NDTMP", 170, 2}, {"MTLGRP", 174, 2}, {"VOLFC", 205, 2}, {"VOLFS", 216, 2},
        {"USRNOD", 196, 2}, {"USRELM", 197, 2}, {"MATAXI", 254, 2},
        {"NWEAR", 255, 2}, {"DRMESH", 263, 2}, {"CUSVAR", 198, 2}, {"DYNARY", 266, 2}};
    for (const auto& required : required_keywords) {
        const auto* entry = lookup(required.name);
        if (!entry || entry->id != required.id || entry->category != required.category)
            throw std::invalid_argument("selected keyword catalog mapping mismatch");
    }
    for (const auto& field : field_keywords) {
        const auto* entry = lookup(field.name);
        if (!entry || entry->id != field.keyword_id || entry->category != 2)
            throw std::invalid_argument("mesh-field keyword catalog mapping mismatch");
    }
}

void KeywordImporter::set_handler_observer(HandlerObserver observer) {
    handler_observer_ = std::move(observer);
}

const KeywordImporter::Entry* KeywordImporter::lookup(std::string_view token) const noexcept {
    for (const auto& entry : catalog_)
        if (entry.name == token) return &entry;
    return nullptr;
}

KeywordImportResult KeywordImporter::import_translated(
    std::string_view bytes, bool request_finalization) {
    KeywordImportResult result;
    result.finalization_pending = request_finalization;
    if (request_finalization) model_.reset_import_flags();
    result.unsupported_reason = unsupported_controls(model_.controls());
    if (result.unsupported_reason.empty() && bytes.find('\0') != std::string_view::npos)
        result.unsupported_reason = "embedded NUL input is outside the compatibility domain";
    if (!result.unsupported_reason.empty()) {
        result.outcome = KeywordImportResult::Outcome::unsupported;
        return result;
    }
    KeywordReader reader(bytes);
    bool recognized_header = false;
    bool event_in_progress = false;
    const auto stop_unsupported = [&](std::string reason) {
        result.outcome = KeywordImportResult::Outcome::unsupported;
        result.unsupported_reason = std::move(reason);
        result.cursor = reader.cursor();
        if (event_in_progress) result.events.back().cursor = reader.cursor();
    };
    const auto finalize_handled_return = [&] {
        if (request_finalization && model_.finalize_import())
            result.finalization_pending = false;
    };
    try {
        while (true) {
            event_in_progress = false;
            const auto token = reader.token();
            if (token.status != 0) {
                result.reached_eof = true;
                result.cursor = reader.cursor();
                finalize_handled_return();
                return result;
            }
            if (token.bytes[0] == '*') {
                const auto remainder = reader.line();
                if (!recognized_header && remainder.status == 0 &&
                    !remainder.bytes.empty() && remainder.bytes[0] != '\n') {
                    if (const auto dimension = header_dimension(remainder.bytes)) {
                        model_.apply_header_dimension(*dimension);
                        recognized_header = true;
                    }
                }
                continue;
            }

            KeywordEvent event;
            event.token = token.bytes;
            event.offset = reader.cursor() - token.bytes.size();
            const auto* entry = lookup(token.bytes);
            if (entry) {
                event.id = entry->id;
                event.category = entry->category;
            }
            result.events.push_back(std::move(event));
            event_in_progress = true;
            auto& observed = result.events.back();
            if (!entry) {
                observed.skipped_line = reader.line().bytes;
                observed.unknown_warning = token.bytes != "SRFNRG";
                observed.cursor = reader.cursor();
                continue;
            }
            if (!implemented(entry->id, entry->category)) {
                stop_unsupported("recognized keyword is not implemented: " + token.bytes);
                return result;
            }
            const int status = entry->category == 1
                ? handle_configuration_keyword(entry->id, model_, reader)
                : entry->category == 4
                    ? handle_material_keyword(entry->id, model_, reader)
                    : entry->category == 5
                        ? handle_relation_keyword(entry->id, model_, reader)
                    : handle_object_keyword(entry->id, model_, reader);
            observed.handler_status = status;
            observed.cursor = reader.cursor();
            if (status != 0) result.status = status;
            if (handler_observer_) handler_observer_(observed, model_);
            if (status != 0 && status != 2 && status != 3) {
                result.cursor = reader.cursor();
                finalize_handled_return();
                return result;
            }
        }
    } catch (const std::logic_error& error) {
        // Checked mesh extents, builder domain, and unsupported configurations
        // are explicit limitations, not invented original Reading Error codes.
        stop_unsupported(error.what());
    } catch (const std::runtime_error& error) {
        stop_unsupported(error.what());
    }
    return result;
}

} // namespace deform::recovered
