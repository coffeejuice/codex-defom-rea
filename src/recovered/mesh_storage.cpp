#include "mesh_storage.hpp"

#include <algorithm>
#include <limits>
#include <stdexcept>
#include <utility>

namespace deform::recovered {
namespace {

std::size_t extent(int count, int width) {
    if (count < 0 || width < 0)
        throw std::invalid_argument("negative mesh extent");
    if (width != 0 && count > std::numeric_limits<int>::max() / width)
        throw std::length_error("mesh extent exceeds legacy int range");
    return static_cast<std::size_t>(count) * static_cast<std::size_t>(width);
}

template<class T>
void release(std::vector<T>& values) {
    std::vector<T>().swap(values);
}

template<std::size_t N>
void validate_components(int count, std::span<const int> components,
                         const std::array<int, N>& ids) {
    for (const int id : ids) {
        if (static_cast<std::size_t>(id) >= components.size())
            throw std::out_of_range("mesh component index");
        (void)extent(count, components[static_cast<std::size_t>(id)]);
    }
}

StateVariable& find_state(std::vector<StateVariable>& states, int id) {
    const auto found = std::find_if(states.begin(), states.end(),
        [id](const StateVariable& state) { return state.id() == id; });
    if (found == states.end()) throw std::out_of_range("unregistered mesh state ID");
    return *found;
}

std::size_t direct_index(int id) {
    const auto found = std::find(MeshStorage::direct_integer_ids.begin(),
                                 MeshStorage::direct_integer_ids.end(), id);
    if (found == MeshStorage::direct_integer_ids.end())
        throw std::out_of_range("unregistered direct integer ID");
    return static_cast<std::size_t>(found - MeshStorage::direct_integer_ids.begin());
}

template<std::size_t N>
std::vector<std::array<std::int32_t, N>> rows(std::span<const std::int32_t> flat) {
    std::vector<std::array<std::int32_t, N>> result(flat.size() / N);
    for (std::size_t i = 0; i < result.size(); ++i)
        std::copy_n(flat.begin() + i * N, N, result[i].begin());
    return result;
}

} // namespace

MeshStorage::MeshStorage(std::array<int, 3> element_multiplicities, int initial_arity)
    : arity_(initial_arity) {
    if (element_multiplicities != std::array<int, 3>{1, 1, 1})
        throw std::invalid_argument("nonunit element multiplicity is unsupported");
    if (initial_arity != 4 && initial_arity != 8)
        throw std::invalid_argument("unsupported initial connectivity arity");
    // Registration order and policies: 0x14017f2f0 / 0x140182b10.
    node_states_.reserve(node_ids.size());
    for (const int id : node_ids)
        node_states_.emplace_back(id, id >= 30 && id <= 33
            ? StateVariable::Policy::explicit_selection_only
            : StateVariable::Policy::ordinary);
    element_states_.reserve(element_ids.size());
    for (const int id : element_ids)
        element_states_.emplace_back(id, id == 46
            ? StateVariable::Policy::material_axes
            : StateVariable::Policy::ordinary);
}

void MeshStorage::begin_coordinate_replace(
    int count, std::span<const int> components, double node8_default) {
    if (node8_default != 20.0 && node8_default != 68.0)
        throw std::invalid_argument("unsupported node ID 8 default");
    validate_components(count, components, std::array{0, 18, 19, 20, 21, 22, 23});
    // IDs 30..33 are skipped by selector -1, including component-table reads.
    validate_components(count, components, std::array{
        1, 2, 3, 4, 5, 6, 8, 9, 10, 11, 12, 13, 14, 15,
        16, 24, 25, 27, 28, 26, 29, 34, 35, 17});
    if (components[0] != 2 && components[0] != 3)
        throw std::invalid_argument("unsupported coordinate component count");

    // 0x1401652b0 -> 0x140162ff0 -> free, then selector -1 allocation.
    node_count_ = count;
    release(coordinates_);
    coordinates_initialized_prefix_ = 0;
    node_allocation_count_ = 0;
    for (std::size_t i = 0; i < 6; ++i) {
        release(direct_integers_[i]);
        direct_integers_initialized_prefix_[i] = 0;
    }
    for (auto& state : node_states_) state.clear();
    if (count == 0) return;
    coordinates_.resize(extent(count, components[0]), 0.0);
    coordinates_initialized_prefix_ = coordinates_.size();
    for (std::size_t i = 0; i < 6; ++i) {
        direct_integers_[i].resize(extent(count, components[direct_integer_ids[i]]), 0);
        direct_integers_initialized_prefix_[i] = direct_integers_[i].size();
    }
    node_allocation_count_ = count;
    for (auto& state : node_states_) state.allocate(count, components, -1);
    auto node8_values = node_values(8);
    std::fill(node8_values.begin(), node8_values.end(), node8_default);
}

void MeshStorage::begin_connectivity_replace(
    int count, int arity, std::span<const int> components,
    std::int32_t direct61_default) {
    if (arity != 4 && arity != 8)
        throw std::invalid_argument("unsupported connectivity arity");
    (void)extent(count, arity);
    validate_components(count, components, element_ids);
    validate_components(count, components, std::array{61});

    // 0x1401653b0 -> 0x140163090 -> free, then selector -1 allocation.
    element_count_ = count;
    arity_ = arity;
    release(connectivity_);
    release(direct_integers_[6]);
    connectivity_initialized_prefix_ = 0;
    direct_integers_initialized_prefix_[6] = 0;
    element_allocation_count_ = 0;
    allocation_arity_ = 0;
    for (auto& state : element_states_) state.clear();
    if (count == 0) return;
    connectivity_.resize(extent(count, arity), 0);
    connectivity_initialized_prefix_ = connectivity_.size();
    direct_integers_[6].resize(extent(count, components[61]), 0);
    direct_integers_initialized_prefix_[6] = direct_integers_[6].size();
    element_allocation_count_ = count;
    allocation_arity_ = arity;
    for (auto& state : element_states_) {
        // These three overrides update multiplicity history even when it is 1.
        if (state.id() == 38 || state.id() == 43 || state.id() == 45)
            state.allocate(count, components, -1, 1);
        else
            state.allocate(count, components, -1);
    }
    if (direct61_default > 0)
        std::fill(direct_integers_[6].begin(), direct_integers_[6].end(), direct61_default);
    auto ones = element_values(44);
    std::fill(ones.begin(), ones.end(), 1.0);
    // Reference explicitly zeros count*components[45], not multiplicity times
    // that extent. These are equal in this component's unit-only regime.
    auto zeros = element_values(45);
    std::fill(zeros.begin(), zeros.end(), 0.0);
}

void MeshStorage::refresh_components(std::span<const int> components) {
    // 199650 gets +12c from the node container and +e8/+ec from the element
    // container. Neither call substitutes object header counts/selected arity.
    // 180150/183770 test count>0 before touching any component-table entries.
    if (node_allocation_count_ > 0) {
        validate_components(node_allocation_count_, components,
                            std::array{0, 18, 19, 20, 21, 22, 23});
        // Selector -1 does not inspect IDs30..33.
        validate_components(node_allocation_count_, components, std::array{
            1, 2, 3, 4, 5, 6, 8, 9, 10, 11, 12, 13, 14, 15,
            16, 24, 25, 27, 28, 26, 29, 34, 35, 17});
    }
    if (element_allocation_count_ > 0) {
        (void)extent(element_allocation_count_, allocation_arity_);
        validate_components(element_allocation_count_, components, std::array{61});
        validate_components(element_allocation_count_, components, element_ids);
    }

    if (node_allocation_count_ > 0) {
        coordinates_.resize(extent(node_allocation_count_, components[0]));
        coordinates_initialized_prefix_ =
            std::min(coordinates_initialized_prefix_, coordinates_.size());
        for (std::size_t i = 0; i < 6; ++i) {
            auto& values = direct_integers_[i];
            values.resize(extent(node_allocation_count_, components[direct_integer_ids[i]]));
            direct_integers_initialized_prefix_[i] =
                std::min(direct_integers_initialized_prefix_[i], values.size());
        }
        for (auto& state : node_states_)
            state.allocate(node_allocation_count_, components, -1);
    }
    if (element_allocation_count_ > 0) {
        connectivity_.resize(extent(element_allocation_count_, allocation_arity_));
        connectivity_initialized_prefix_ =
            std::min(connectivity_initialized_prefix_, connectivity_.size());
        direct_integers_[6].resize(extent(element_allocation_count_, components[61]));
        direct_integers_initialized_prefix_[6] =
            std::min(direct_integers_initialized_prefix_[6], direct_integers_[6].size());
        for (auto& state : element_states_) {
            // Same registered allocator policies, still unit multiplicities.
            if (state.id() == 38 || state.id() == 43 || state.id() == 45)
                state.allocate(element_allocation_count_, components, -1, 1);
            else
                state.allocate(element_allocation_count_, components, -1);
        }
    }
}

MeshStorage::MaterialColumnSnapshot MeshStorage::capture_material_columns(int old_width) const {
    MaterialColumnSnapshot result;
    result.rows = element_count_;
    result.width = old_width;
    const auto size = extent(result.rows, result.width);
    for (std::size_t index = 0; index < result.values.size(); ++index) {
        const auto found = std::find_if(element_states_.begin(), element_states_.end(),
            [index](const StateVariable& state) { return state.id() == 49 + static_cast<int>(index); });
        if (found == element_states_.end() || found->values().size() < size)
            throw std::invalid_argument("material-column source extent is outside owned storage");
        const auto source = found->values().first(size);
        result.values[index].assign(source.begin(), source.end());
    }
    return result;
}

void MeshStorage::remap_material_columns(const MaterialColumnSnapshot& before, int new_width,
    std::span<const int> old_ranks, std::span<const int> new_ranks) {
    const auto old_size = extent(before.rows, before.width);
    const auto new_size = extent(element_count_, new_width);
    if (before.rows != element_count_ || old_ranks.size() < static_cast<std::size_t>(before.width) ||
        new_ranks.size() < static_cast<std::size_t>(before.width))
        throw std::invalid_argument("inconsistent material-column remap dimensions");
    std::array<std::span<double>, 2> destination{element_values(49), element_values(50)};
    for (std::size_t index = 0; index < destination.size(); ++index)
        if (before.values[index].size() != old_size || destination[index].size() < new_size)
            throw std::invalid_argument("material-column remap extent is outside owned storage");
    for (int column = 0; column < before.width; ++column) {
        if (new_ranks[column] < 0) continue; // Original missing-ID guard.
        if (new_ranks[column] >= new_width || old_ranks[column] < 0 || old_ranks[column] >= before.width)
            throw std::invalid_argument("material-column rank is outside its stored row width");
    }
    // 1d0fc0 clears both row-major arrays before copying surviving columns.
    // It uses header element count here, independently of allocation count.
    for (auto values : destination) std::fill_n(values.begin(), new_size, 0.0);
    for (int column = 0; column < before.width; ++column) {
        if (new_ranks[column] < 0) continue;
        for (int row = 0; row < element_count_; ++row) {
            const auto source = static_cast<std::size_t>(row) * before.width + old_ranks[column];
            const auto target = static_cast<std::size_t>(row) * new_width + new_ranks[column];
            destination[0][target] = before.values[0][source];
            destination[1][target] = before.values[1][source];
        }
    }
}

void MeshStorage::finalize_connectivity(int dimension, bool build_boundaries) {
    if (dimension != 2 && dimension != 3)
        throw std::invalid_argument("unsupported boundary dimension");
    if (dimension == 2 && arity_ != 4)
        throw std::invalid_argument("2D boundaries require four connectivity slots");
    mesh_present_ = true;
    if (!build_boundaries || connectivity_.empty()) return;

    // 0x1401fc290, totals from 0x140165d30 / 0x14017ebc0.
    std::vector<BoundaryGroup> groups;
    PlanarBoundary chains;
    int total = 0;
    if (dimension == 2) {
        chains = planar_boundary(rows<4>(connectivity_));
        const auto group_count = std::min<std::size_t>(100, chains.ranges.size());
        groups.resize(group_count);
        for (std::size_t i = 0; i < group_count; ++i) {
            const auto range = chains.ranges[i];
            groups[i].nodes.assign(chains.nodes.begin() + range[0] - 1,
                                   chains.nodes.begin() + range[1]);
            const auto edges = groups[i].nodes.size() - 1;
            if (edges > static_cast<std::size_t>(std::numeric_limits<int>::max() - total))
                throw std::length_error("boundary total exceeds legacy int range");
            total += static_cast<int>(edges);
        }
    } else {
        groups.resize(1);
        groups[0].faces = arity_ == 4
            ? tetrahedral_boundary(rows<4>(connectivity_))
            : hexahedral_boundary(rows<8>(connectivity_));
        if (groups[0].faces.size() > static_cast<std::size_t>(std::numeric_limits<int>::max()))
            throw std::length_error("boundary total exceeds legacy int range");
        total = static_cast<int>(groups[0].faces.size());
    }
    boundary_groups_ = std::move(groups);
    planar_chains_ = std::move(chains);
    boundary_total_ = total;
}

std::span<std::int32_t> MeshStorage::direct_integers(int id) {
    return direct_integers_[direct_index(id)];
}

std::size_t MeshStorage::direct_integers_initialized_prefix(int id) const {
    return direct_integers_initialized_prefix_[direct_index(id)];
}

void MeshStorage::record_direct_integer_writes(int id, const std::vector<bool>& written) {
    const auto index = direct_index(id);
    if (written.size() > direct_integers_[index].size())
        throw std::invalid_argument("direct integer write mask exceeds owned storage");
    auto& prefix = direct_integers_initialized_prefix_[index];
    while (prefix < written.size() && written[prefix]) ++prefix;
}

void MeshStorage::deactivate() {
    release(coordinates_);
    release(connectivity_);
    for (auto& values : direct_integers_) release(values);
    for (auto& state : node_states_) state.clear();
    for (auto& state : element_states_) state.clear();
    for (auto& group : boundary_groups_) {
        release(group.nodes);
        release(group.faces);
    }
    planar_chains_ = {};
    node_count_ = 0;
    element_count_ = 0;
    node_allocation_count_ = 0;
    element_allocation_count_ = 0;
    allocation_arity_ = 0; // 182a50; arity_ belongs to the object and survives.
    coordinates_initialized_prefix_ = 0;
    connectivity_initialized_prefix_ = 0;
    direct_integers_initialized_prefix_.fill(0);
    mesh_present_ = false;
    boundary_total_ = 0;
}
std::span<const std::int32_t> MeshStorage::direct_integers(int id) const {
    return direct_integers_[direct_index(id)];
}
std::span<double> MeshStorage::node_values(int id) {
    return find_state(node_states_, id).values();
}
std::span<double> MeshStorage::element_values(int id) {
    return find_state(element_states_, id).values();
}

std::span<double> MeshStorage::select_node_field(int id, std::span<const int> components) {
    if (id != 17 && id != 24)
        throw std::invalid_argument("node getter outside recovered selector set");
    auto& state = find_state(node_states_, id);
    if (node_allocation_count_ > 0)
        state.allocate(node_allocation_count_, components, id);
    return state.values();
}

std::span<double> MeshStorage::select_element_field(int id, std::span<const int> components) {
    if (id != 46 && id != 54 && id != 55 && id != 60)
        throw std::invalid_argument("element getter outside recovered selector set");
    auto& state = find_state(element_states_, id);
    if (element_allocation_count_ > 0)
        state.allocate(element_allocation_count_, components, id);
    return state.values();
}

void MeshStorage::set_material_shape_width(int id, std::span<const int> components) {
    if (id != 54 && id != 55)
        throw std::invalid_argument("material-shape component must be54 or55");
    if (element_allocation_count_ > 0) {
        find_state(element_states_, id).allocate(element_allocation_count_, components, id);
        material_shape_saved_widths_[id - 54] = components[id];
    }
}

} // namespace deform::recovered
