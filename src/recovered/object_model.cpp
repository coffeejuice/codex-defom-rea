#include "object_model.hpp"
#include "component_layout.hpp"

#include <algorithm>
#include <bit>
#include <cmath>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <utility>

namespace deform::recovered {
namespace {

ComponentLayoutInputs default_material_layout(int dimension,
    const SimulationControls& simulation, std::int32_t material_count = 0,
    bool material_columns = false, int size_width = 0, int texture_width = 0,
    std::int32_t relation_count = 0) {
    ComponentLayoutInputs input{};
    input.dimension = dimension;
    input.root_0x850 = simulation.geometry_type;
    input.root_0x1320 = material_count;
    input.root_0x13f4 = relation_count;
    input.root_0x3398 = simulation.user_node_width_0x3398;
    input.root_0x33a8 = simulation.user_element_width_0x33a8;
    std::copy_n(simulation.output_flags.begin(), 5, input.root_0x86c_to_0x87c.begin());
    input.root_0x880 = simulation.output_flags[5];
    input.root_0x884 = simulation.output_flags[6];
    input.root_0x33c8 = input.root_0x33d8 = 1;
    // The remaining inputs retain the reset profile, including zero
    // 11c4/123c/12b4 observed in root-reset-contract-audit.json. This recipe
    // is not an inference from an arbitrary component table or material list.
    // 1d64f0: query24 true;49 is any material1cc8==1 (root85c stays0);
    // 53/29 false (root864=0);54/55 are the recovered material queries.
    // Query46 is dimension2 or a material1e40 flag; those flags are allzero.
    input.feature24.enabled = true;
    input.feature49.enabled = material_columns;
    input.feature46.enabled = dimension == 2;
    input.feature54 = {size_width > 0, size_width};
    input.feature55 = {texture_width > 0, texture_width};
    return input;
}

void require_simulation_profile(const RootControls& controls) {
    if (!controls.simulation)
        throw std::invalid_argument(
            "configuration refresh requires explicit reset controls and selected material properties");
}

void normalize_step_mode(SimulationControls& simulation) {
    // 1ee510/1ee5a0: an explicit nonzero93c suppresses automatic mode changes.
    if (simulation.control_0x93c == 0)
        simulation.mode_0x990 = simulation.step_increment_0x930 <= 0.0 ? 1 : 0;
}

} // namespace

RelationRecord::RelationRecord(int first, int second)
    : key0(first), key1(second),
      field_0x170(std::bit_cast<double>(std::uint64_t{0x3f847ae147ae147b})),
      field_0x178(std::bit_cast<double>(std::uint64_t{0x3fefae147ae147ae})) {
    // 333f60: common12d050(type0), followed by the listed typed initializers.
    constexpr std::array<std::pair<int, int>, 12> profiles{{
        {0x78, 0x2c}, {0xf8, 0x37}, {0x180, 0xb3}, {0x1f8, 0x30},
        {0x2c8, 0x32}, {0x3f8, 0x36}, {0x478, 0}, {0x4f0, 0},
        {0x568, 0}, {0x5e8, 0}, {0x678, 0x6d}, {0x6f0, 0x6e}}};
    for (const auto& [offset, type] : profiles)
        property_tables.try_emplace(offset, type);
}

MaterialRecord::MaterialRecord(int material_id) : id(material_id) {
    // 28c6f0 constructs generic tables;28cdc0 changes GBENGY/NUCSIZ types.
    for (const int keyword : {95, 96, 97, 98, 99, 100, 115, 121, 122, 125, 126})
        property_tables.try_emplace(keyword, 0);
    property_tables.try_emplace(131, 0x5d);
    property_tables.try_emplace(133, 0xb6);
}

int MaterialRecord::texture_value_count() const noexcept {
    // 28c5a0: the second selector must be1; recognized resolutions1..4
    // have fixed counts. Unknown modes return0 and write the native stdout diagnostic.
    if (feature_0x2154 == 0) return 0;
    if (feature_0x2154 < 1 || feature_0x2154 > 3) {
        std::cout << "Error in TEXTURE_DATA::countNumODFs()\n";
        return 0;
    }
    if (field_0x2158 != 1 || field_0x215c < 1 || field_0x215c > 4) return 0;
    constexpr std::array<int, 4> first{10, 76, 600, 4784};
    constexpr std::array<int, 4> third{7, 50, 388, 3080};
    if (feature_0x2154 == 1 || feature_0x2154 == 2) return first[field_0x215c - 1];
    if (feature_0x2154 == 3) return third[field_0x215c - 1];
    return 0;
}

RootControls initialized_root_controls(int dimension, double node8_default,
    bool suppress_boundaries) {
    if (dimension != 2 && dimension != 3)
        throw std::invalid_argument("unsupported root dimension");
    if (node8_default != 20.0 && node8_default != 68.0)
        throw std::invalid_argument("unsupported process temperature default");
    RootControls result{};
    result.dimension = dimension;
    result.simulation.emplace();
    result.component_counts = initialize_layout(default_material_layout(dimension, *result.simulation));
    result.node8_default = node8_default;
    result.direct61_default = 0; // New object 15f0b0 stores3980=0.
    result.suppress_boundaries = suppress_boundaries;
    return result;
}

ObjectRecord::ObjectRecord(int object_id, int dimension)
    : id(object_id), element_kind(dimension == 2 ? 0 : 2), mesh({1, 1, 1}, 4) {
    if (object_id < 1) throw std::invalid_argument("object ID must be positive");
    if (dimension != 2 && dimension != 3)
        throw std::invalid_argument("unsupported object dimension");
    // 15f0b0: 2D initializes kind 0/arity 4; 3D calls 165500(kind 2).
}

void ObjectRecord::apply_type(int file_type, int secondary_value) {
    if (file_type >= 1 && file_type <= 5) type = file_type - 1;
    else if (file_type == 7) type = 6;
    else if (file_type != 6) type = 7;
    secondary = secondary_value;
    if (type == 7) {
        // 161260 plus object-deactivation-observations: identity, secondary
        // and selected element kind/arity survive; name and mesh payload clear.
        mesh.deactivate();
        geometry.deactivate();
        // 15fbf0:301d10 removes the containers, then301ad0 constructs a new
        // default one. Unlike import-time clear(), this resets the matrix too.
        geometry_segments = GeometrySegments{};
        flag_0x3990 = false; // 15fbf0 leaves3994 unchanged.
        name.clear();
    }
}

ObjectModel::ObjectModel(RootControls controls) : controls_(std::move(controls)) {}

void ObjectModel::set_dimension(int dimension) {
    if (dimension != 2 && dimension != 3)
        throw std::invalid_argument("unsupported root dimension");
    if (dimension == controls_.dimension) return;
    auto reset = initialized_root_controls(dimension, controls_.node8_default,
        controls_.suppress_boundaries);
    reset.alternate_root_enabled = controls_.alternate_root_enabled;
    // Native setter writes the dimension before clearing the lists. Internal
    // ownership releases every represented mesh/descriptor with its object.
    controls_.dimension = dimension;
    // 1cb440 destroys materials before object lists;1c8c00 resets1320.
    materials_.clear();
    material_count_ = 0;
    relations_.clear();
    relation_count_ = 0;
    objects_.clear();
    controls_ = std::move(reset);
    ++generation_;
}

void ObjectModel::link_root(ObjectModel* root) {
    if (root == this) throw std::invalid_argument("self-linked root is unsupported");
    linked_root_ = root;
    if (linked_root_) linked_root_->set_dimension(controls_.dimension == 3 ? 2 : 3);
}

void ObjectModel::apply_header_dimension(int dimension) {
    auto* linked = linked_root_;
    set_dimension(dimension);
    if (linked) linked->set_dimension(dimension == 3 ? 2 : 3);
}

void ObjectModel::set_geometry_type(int value) {
    require_simulation_profile(controls_);
    controls_.simulation->geometry_type = value;
    refresh_components();
}

void ObjectModel::set_output_flags(const std::array<int, 7>& values) {
    require_simulation_profile(controls_);
    controls_.simulation->output_flags = values;
    refresh_components();
}

void ObjectModel::refresh_components() {
    refresh_components_impl({}, false);
}

void ObjectModel::set_user_field_width(bool nodes, int width) {
    require_simulation_profile(controls_);
    if (width < 0)
        throw std::invalid_argument("negative user-field metadata count is outside the recovered domain");
    auto& simulation = *controls_.simulation;
    auto& stored = nodes ? simulation.user_node_width_0x3398 : simulation.user_element_width_0x33a8;
    if (stored == width) return;
    auto& definitions = nodes ? simulation.user_node_definitions : simulation.user_element_definitions;
    definitions.resize(static_cast<std::size_t>(width));
    stored = width;
    refresh_components();
}

std::span<double> ObjectModel::user_field_values(ObjectRecord& object, bool nodes) {
    if (find(object.id) != &object)
        throw std::invalid_argument("user-field getter requires the owned object");
    // The following native1d48d0 call is a no-op:1d2b70 returns true on both
    // branches in this binary. Do not invent property-table cleanup here.
    return nodes ? object.mesh.select_node_field(17, controls_.component_counts)
                 : object.mesh.select_element_field(60, controls_.component_counts);
}

std::span<double> ObjectModel::prepare_material_axes(ObjectRecord& object) {
    if (find(object.id) != &object)
        throw std::invalid_argument("material-axis getter requires the owned object");
    controls_.component_counts[46] = controls_.dimension == 3 ? 9 : 1;
    return object.mesh.select_element_field(46, controls_.component_counts);
}

std::span<double> ObjectModel::prepare_node_wear(ObjectRecord& object) {
    if (find(object.id) != &object)
        throw std::invalid_argument("wear getter requires the owned object");
    controls_.component_counts[24] = 5;
    return object.mesh.select_node_field(24, controls_.component_counts);
}

void ObjectModel::set_material_shape_width(int component_id, int width) {
    if ((component_id != 54 && component_id != 55) || width < 1)
        throw std::invalid_argument("invalid material-shape width update");
    controls_.component_counts[component_id] = width;
}

std::span<double> ObjectModel::prepare_material_shape(ObjectRecord& object, int component_id) {
    if (find(object.id) != &object || (component_id != 54 && component_id != 55))
        throw std::invalid_argument("material-shape getter requires an owned object and component54/55");
    object.mesh.set_material_shape_width(component_id, controls_.component_counts);
    return object.mesh.select_element_field(component_id, controls_.component_counts);
}

bool ObjectModel::material_columns_enabled() const noexcept {
    return (controls_.simulation && controls_.simulation->material_gate_0x85c != 0) ||
        std::any_of(materials_.begin(), materials_.end(),
        [](const auto& material) { return material->mode_0x1cc8 == 1; });
}

RelationRecord* ObjectModel::find_relation(int key0, int key1) noexcept {
    for (const auto& relation : relations_)
        if (relation->key0 == key0 && relation->key1 == key1) return relation.get();
    return nullptr;
}

RelationRecord& ObjectModel::find_or_create_relation(int key0, int key1) {
    if (auto* relation = find_relation(key0, key1)) return *relation;
    if (relations_.size() >= static_cast<std::size_t>(std::numeric_limits<std::int32_t>::max()))
        throw std::overflow_error("relation list count exceeds native signed32 domain");
    relations_.push_back(std::make_unique<RelationRecord>(key0, key1));
    return *relations_.back(); // No material existence check or eager refresh.
}

void ObjectModel::apply_relation_mode(RelationRecord& relation, std::int32_t mode) {
    if (find_relation(relation.key0, relation.key1) != &relation)
        throw std::invalid_argument("relation update requires the owned record");
    relation.mode_0x08 = mode;
    relation.active_0x0c = mode != 0 ? 1 : 0;
    if (mode == 0) return;
    relation_count_ = static_cast<std::int32_t>(std::count_if(relations_.begin(), relations_.end(),
        [](const auto& item) { return item->active_0x0c == 1; }));
    refresh_components(); // 1d0fc0(root,0); no semantic relation-column permutation.
}

void ObjectModel::refresh_components_impl(std::span<const int> old_material_ids,
    bool remap_materials) {
    require_simulation_profile(controls_);
    const bool enabled = material_columns_enabled();
    const bool remap = remap_materials && enabled && !old_material_ids.empty();
    const int old_width = controls_.component_counts[49];
    std::vector<int> old_ranks, new_ranks;
    std::vector<std::optional<MeshStorage::MaterialColumnSnapshot>> copies(objects_.size());
    if (remap) {
        if (old_width < 0 || static_cast<std::size_t>(old_width) > old_material_ids.size())
            throw std::invalid_argument("old material width exceeds the saved ID map");
        old_ranks.reserve(old_material_ids.size());
        new_ranks.reserve(old_material_ids.size());
        // 29ca10 stores insertion-order IDs with their strictly-less-ID rank.
        // Only old root width entries are later consumed by1d0fc0.
        for (const int id : old_material_ids) {
            old_ranks.push_back(static_cast<int>(std::count_if(old_material_ids.begin(), old_material_ids.end(),
                [id](int other) { return other < id; })));
            new_ranks.push_back(find_material(id) ? static_cast<int>(std::count_if(materials_.begin(), materials_.end(),
                [id](const auto& other) { return other->id < id; })) : -1);
        }
        for (std::size_t index = 0; index < objects_.size(); ++index)
            if (objects_[index]->type != 7)
                copies[index] = objects_[index]->mesh.capture_material_columns(old_width);
    }
    // As in1d0fc0, update the root before resizing each active object's
    // containers; option1 remaps that object immediately after its refresh.
    std::int64_t size_width = 0, texture_width = 0;
    constexpr std::array<int, 7> size_counts{0, 1, 4, 3, 2, 4, 1};
    for (const auto& material : materials_) {
        const auto mode = material->feature_0x2150;
        if (mode > 0 && mode < static_cast<int>(size_counts.size())) size_width += size_counts[mode];
        // 1d6b30 traverses each immediate reference in order, including
        // duplicates. A nonzero material mode suppresses its own texture.
        if (material->mode_0x1cc8 == 0) texture_width += material->texture_value_count();
        else for (const int reference_id : material->reference_ids()) {
            if (const auto* reference = find_material(reference_id))
                texture_width += reference->texture_value_count();
        }
    }
    if (size_width > std::numeric_limits<int>::max() || texture_width > std::numeric_limits<int>::max())
        throw std::overflow_error("material component count exceeds native signed32 domain");
    controls_.component_counts = refresh_layout(default_material_layout(controls_.dimension,
        *controls_.simulation, material_count_, enabled, static_cast<int>(size_width),
        static_cast<int>(texture_width), relation_count_));
    for (std::size_t index = 0; index < objects_.size(); ++index) {
        const auto& object = objects_[index];
        if (object->type != 7) {
            object->mesh.refresh_components(controls_.component_counts);
            if (copies[index])
                object->mesh.remap_material_columns(*copies[index], controls_.component_counts[49], old_ranks, new_ranks);
        }
    }
}

void ObjectModel::set_start_step(std::int32_t value, bool read_succeeded) {
    require_simulation_profile(controls_);
    controls_.simulation->step_number_0x8b0 = value;
    if (read_succeeded) import_flag_0x2f88_ = true;
}

void ObjectModel::set_displacement_step(double value) {
    require_simulation_profile(controls_);
    auto& simulation = *controls_.simulation;
    simulation.step_increment_0x930 = value;
    if (value > 0.0) {
        simulation.time_step_0x920 = 0.0;
        normalize_step_mode(simulation);
    }
}

void ObjectModel::set_time_steps(const std::array<double, 2>& values) {
    require_simulation_profile(controls_);
    auto& simulation = *controls_.simulation;
    simulation.time_step_0x920 = values[0];
    simulation.time_step_secondary_0x928 = values[1];
    if (values[0] > 0.0) {
        simulation.step_increment_0x930 = 0.0; // 1f4390(simulation,0).
        normalize_step_mode(simulation);
    }
}

void ObjectModel::reset_import_flags() noexcept {
    import_flag_0x2f88_ = false;
    import_flag_0x2f89_ = false;
}

void ObjectModel::set_step_distance(double value, int count) {
    require_simulation_profile(controls_);
    auto& simulation = *controls_.simulation;
    const auto* object = find(simulation.selected_object_0x8b8);
    if (!object || count == 0 || value == 0.0) return;
    const double reference = object->stroke_0x22d0[2];
    if (!std::isfinite(value) || !std::isfinite(reference))
        throw std::invalid_argument("nonfinite step-distance values are outside the compatibility domain");
    // 484558 clears the sign bit for finite binary64 values. The parent uses
    // SUBSD followed by DIVSD with the signed integer converted to double.
    const double difference = std::fabs(value) - std::fabs(reference);
    simulation.step_increment_0x930 = difference / static_cast<double>(count);
    // Unlike DSMAX, this does not clear920 or normalize990.
}

void ObjectModel::set_pdie(int selected_object, int secondary) {
    require_simulation_profile(controls_);
    auto& simulation = *controls_.simulation;
    simulation.selected_object_0x8b8 = selected_object;
    simulation.pdie_secondary_0x1104 = secondary;
}

TimeTableControls& ObjectModel::initialize_time_table(int selector, int count) {
    require_simulation_profile(controls_);
    auto& simulation = *controls_.simulation;
    if (simulation.control_0x93c != 0 && simulation.control_0x93c != 1)
        throw std::invalid_argument("higher-rank time tables are outside the compatibility domain");
    auto& table = simulation.time_table;
    if (table.type_0x00 != 0x80 || table.values_per_point_0x40 != 1)
        throw std::invalid_argument("only the reset type80 time-table profile is supported");
    const auto field54 = table.preserved_0x54;
    const auto field60 = table.preserved_0x60;
    table = TimeTableControls{};
    table.preserved_0x54 = field54;
    table.preserved_0x60 = field60;
    // 12d2a0 resets these aliased fields before the switch writes them.
    simulation.control_0x93c = 0;
    simulation.mode_0x990 = 0;
    if (selector == 0) {
        normalize_step_mode(simulation);
        return table;
    }
    simulation.control_0x93c = 1;
    table.counts_0x1c[0] = std::max(count, 0); // 12fc00 clamps negatives.
    table.selectors_0x10[0] = selector == 2 || selector == 4 ? 2 : 1;
    simulation.mode_0x990 = selector == 3 || selector == 4 ? 1 : 0;
    const auto rows = static_cast<std::size_t>(table.counts_0x1c[0]);
    table.axes[0].resize(rows, 0.0);
    table.dependent.resize(rows, 0.0);
    return table;
}

bool ObjectModel::finalize_import() {
    if (!controls_.simulation) return false;
    auto& simulation = *controls_.simulation;
    // 1ee510/1ee5a0 precede the material and object loops in1d7f40.
    normalize_step_mode(simulation);
    finalize_material_references(); // Immediate insertion-order1d6250 writes.
    // 1d63c0 visits type!=7 objects;
    //2059a0 groups slot0 geometry only when3990 is enabled.
    for (const auto& object : objects_)
        if (object->type != 7 && object->flag_0x3990)
            object->geometry.finalize_planes();
    if (import_flag_0x2f88_) {
        if (simulation.step_number_0x8b0 >= 0) {
            // Widen before adding so INT_MAX becomes INT_MIN without C++ UB.
            simulation.step_number_0x8b0 = static_cast<std::int32_t>(
                -(static_cast<std::int64_t>(simulation.step_number_0x8b0) + 1));
        }
        simulation.saved_step_0x744 = simulation.step_number_0x8b0;
    }
    return true;
}

ObjectRecord* ObjectModel::find(int id) noexcept {
    for (const auto& object : objects_)
        if (object->id == id) return object.get();
    return nullptr;
}

const ObjectRecord* ObjectModel::find(int id) const noexcept {
    for (const auto& object : objects_)
        if (object->id == id) return object.get();
    return nullptr;
}

ObjectRecord* ObjectModel::find_or_create(int id) {
    if (id < 1) return nullptr;
    if (auto* object = find(id)) return object;
    // Existing selected material properties do not change the represented
    // owner-bound constructor fields. Subsequent mesh imports use the current
    // root component table, including its material-dependent widths.
    // 19ccc0 appends placeholders without running deactivation on their new
    // descriptors. Do not replace this with apply_type(0), which clears history.
    while (objects_.size() < static_cast<std::size_t>(id)) {
        const int next_id = static_cast<int>(objects_.size()) + 1;
        auto object = std::make_unique<ObjectRecord>(next_id, controls_.dimension);
        if (next_id != id) object->type = 7;
        objects_.push_back(std::move(object));
    }
    return objects_.back().get();
}

MaterialRecord* ObjectModel::find_material(int id) noexcept {
    for (const auto& material : materials_)
        if (material->id == id) return material.get();
    return nullptr;
}

const MaterialRecord* ObjectModel::find_material(int id) const noexcept {
    for (const auto& material : materials_)
        if (material->id == id) return material.get();
    return nullptr;
}

MaterialRecord* ObjectModel::find_or_create_material(int id) {
    if (auto* material = find_material(id)) return material;
    require_simulation_profile(controls_);
    if (material_count_ == std::numeric_limits<std::int32_t>::max())
        throw std::out_of_range("material count exceeds signed32 compatibility domain");
    std::vector<int> old_ids;
    old_ids.reserve(materials_.size());
    for (const auto& existing : materials_) old_ids.push_back(existing->id);
    auto material = std::make_unique<MaterialRecord>(id);
    auto* result = material.get();
    materials_.push_back(std::move(material));
    ++material_count_;
    // 1d0fc0(root,1) copies active objects to a temporary list, refreshes the
    // originals, then consults query49 before restoring material columns.
    // Temporary copies leave owned sources intact. Their only values read
    // back here are element49/50 columns when mode1 enables query49.
    // Scratch ID/rank arrays and copies are released before the name read.
    // Append/count/refresh remain if the name later fails.
    refresh_components_impl(old_ids, true);
    return result;
}

std::span<const std::int32_t> MaterialRecord::reference_ids() const {
    if (count_0x1ccc <= 0) return {};
    if (static_cast<std::size_t>(count_0x1ccc) > reference_allocation_.size())
        throw std::invalid_argument("logical material reference count exceeds owned allocation");
    return std::span<const std::int32_t>(reference_allocation_).first(static_cast<std::size_t>(count_0x1ccc));
}

std::int32_t ObjectModel::begin_material_reference_count(MaterialRecord& material) {
    require_simulation_profile(controls_);
    if (find_material(material.id) != &material)
        throw std::invalid_argument("material reference destination belongs to another root");
    const auto previous = material.count_0x1ccc;
    material.count_0x1ccc = 0; // 413de0 zeros its direct destination before fgets.
    return previous;
}

void ObjectModel::apply_material_reference_count(MaterialRecord& material,
    std::int32_t previous_count, std::int32_t parsed_count, int reader_status) {
    require_simulation_profile(controls_);
    if (find_material(material.id) != &material)
        throw std::invalid_argument("material reference destination belongs to another root");
    material.count_0x1ccc = parsed_count;
    if (reader_status != 0) return;
    if (parsed_count > 0) {
        material.mode_0x1cc8 = 1;
        // Native frees before calloc. Allocation failure has no compatibility
        // claim; committed mode/count and released old storage are retained.
        std::vector<std::int32_t>().swap(material.reference_allocation_);
        material.reference_allocation_.resize(static_cast<std::size_t>(parsed_count), 0);
    } else if (material.mode_0x1cc8 == 1 && material.has_reference_allocation() && previous_count > 0) {
        if (static_cast<std::size_t>(previous_count) > material.reference_allocation_.size())
            throw std::invalid_argument("previous material reference count exceeds retained allocation");
        for (std::int32_t index = 0; index < previous_count; ++index)
            if (auto* target = find_material(material.reference_allocation_[static_cast<std::size_t>(index)]))
                target->mode_0x1cc8 = 0;
        material.mode_0x1cc8 = 0;
    }
}

void ObjectModel::write_material_reference(MaterialRecord& material, std::size_t index,
    std::int32_t reference_id) {
    if (find_material(material.id) != &material || material.count_0x1ccc <= 0 ||
        index >= static_cast<std::size_t>(material.count_0x1ccc) || index >= material.reference_allocation_.size())
        throw std::out_of_range("material reference write is outside its logical allocation");
    material.reference_allocation_[index] = reference_id;
}

MaterialColumnMapping ObjectModel::material_column_mapping(const ObjectRecord& object) const {
    require_simulation_profile(controls_);
    if (find(object.id) != &object || !object.mesh.mesh_present())
        throw std::invalid_argument("material-column mapping requires an owned live mesh");
    const auto rows = object.mesh.element_count();
    const auto element_ids = object.mesh.direct_integers(61);
    if (rows < 0 || element_ids.size() < static_cast<std::size_t>(rows) ||
        object.mesh.direct_integers_initialized_prefix(61) < static_cast<std::size_t>(rows))
        throw std::invalid_argument("material-column mapping requires initialized element IDs");
    MaterialColumnMapping result;
    result.columns.assign(materials_.size(), -1);
    std::vector<int> marks(materials_.size(), 0);
    const auto rank = [this](int id) -> int {
        if (!find_material(id)) return -1;
        return static_cast<int>(std::count_if(materials_.begin(), materials_.end(),
            [id](const auto& material) { return material->id < id; }));
    };
    int previous_id = 0;
    for (int row = 0; row < rows; ++row) {
        const int id = element_ids[static_cast<std::size_t>(row)];
        if (id == previous_id) continue;
        previous_id = id;
        const int material_rank = rank(id);
        if (material_rank < 0 || marks[static_cast<std::size_t>(material_rank)] != 0) continue;
        const auto* material = find_material(id);
        if (material->mode_0x1cc8 != 1) continue;
        marks[static_cast<std::size_t>(material_rank)] = 1;
        for (const int reference : material->reference_ids()) {
            const int reference_rank = rank(reference);
            if (reference_rank >= 0) marks[static_cast<std::size_t>(reference_rank)] = 2;
        }
    }
    for (std::size_t index = 0; index < marks.size(); ++index)
        if (marks[index] == 2) result.columns[index] = result.input_width++;
    return result;
}

void ObjectModel::apply_material_columns(ObjectRecord& object, int descriptor_id,
    const MaterialColumnMapping& mapping, std::span<const double> values) {
    require_simulation_profile(controls_);
    if (find(object.id) != &object || (descriptor_id != 49 && descriptor_id != 50))
        throw std::invalid_argument("unsupported material-column destination");
    const int rows = object.mesh.element_count();
    const int width = controls_.component_counts[static_cast<std::size_t>(descriptor_id)];
    const auto checked_extent = [rows](int columns) {
        if (rows < 0 || columns < 0 || (columns != 0 && rows > std::numeric_limits<int>::max() / columns))
            throw std::invalid_argument("material-column extent exceeds the selected native domain");
        return static_cast<std::size_t>(rows) * static_cast<std::size_t>(columns);
    };
    const auto input_size = checked_extent(mapping.input_width);
    const auto destination_size = checked_extent(width);
    auto destination = object.mesh.element_values(descriptor_id);
    if (values.size() != input_size || destination.size() < destination_size ||
        mapping.columns.size() != materials_.size() || mapping.columns.size() < static_cast<std::size_t>(width))
        throw std::invalid_argument("material-column commit is outside owned storage");
    for (int column = 0; column < width; ++column)
        if (mapping.columns[static_cast<std::size_t>(column)] >= mapping.input_width)
            throw std::invalid_argument("material-column input map is outside its row width");
    for (int row = 0; row < rows; ++row)
        for (int column = 0; column < width; ++column) {
            const int input_column = mapping.columns[static_cast<std::size_t>(column)];
            if (input_column >= 0)
                destination[static_cast<std::size_t>(row) * width + column] =
                    values[static_cast<std::size_t>(row) * mapping.input_width + input_column];
        }
}

void ObjectModel::finalize_material_groups(ObjectRecord& object) {
    require_simulation_profile(controls_);
    if (find(object.id) != &object)
        throw std::invalid_argument("material-group destination belongs to another root");
    const auto values = object.mesh.direct_integers(61);
    // 202960 temporarily sets3980=0, so1fb270 always takes1fb440. Absent
    // mesh or absent direct storage leaves the output zero and restores old3980.
    if (!object.mesh.mesh_present() || values.empty()) return;
    const int rows = object.mesh.element_count();
    if (rows < 0 || values.size() < static_cast<std::size_t>(rows) ||
        object.mesh.direct_integers_initialized_prefix(61) < static_cast<std::size_t>(rows))
        throw std::invalid_argument("material-group postprocess requires initialized element IDs");
    std::vector<std::pair<std::int32_t, int>> counts;
    for (int row = 0; row < rows; ++row) {
        const auto id = values[static_cast<std::size_t>(row)];
        const auto found = std::find_if(counts.begin(), counts.end(),
            [id](const auto& item) { return item.first == id; });
        if (found == counts.end()) counts.emplace_back(id, 1);
        else ++found->second;
    }
    std::int32_t selected = 0;
    int maximum = 0;
    for (const auto& [id, count] : counts)
        if (count > maximum) {
            maximum = count;
            selected = id;
        }
    if (selected != 0) object.material_group_0x3980 = selected;
}

void ObjectModel::finalize_material_references() {
    // 1d6250 uses current modes/counts, not a graph snapshot. In particular a
    // self-reference clears this source's count and stops its own next test.
    for (const auto& source : materials_) {
        if (source->mode_0x1cc8 != 1) continue;
        for (std::int32_t index = 0; index < source->count_0x1ccc; ++index) {
            if (static_cast<std::size_t>(index) >= source->reference_allocation_.size())
                throw std::invalid_argument("material finalizer reference is outside retained allocation");
            auto* target = find_material(source->reference_allocation_[static_cast<std::size_t>(index)]);
            if (!target || target->mode_0x1cc8 == 2) continue;
            if (target->count_0x1ccc > 0) {
                std::vector<std::int32_t>().swap(target->reference_allocation_);
                target->count_0x1ccc = 0;
            }
            target->mode_0x1cc8 = 2;
        }
    }
}

} // namespace deform::recovered
