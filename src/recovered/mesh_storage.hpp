#pragma once

#include "mesh_boundary.hpp"
#include "planar_boundary.hpp"
#include "state_variable.hpp"

#include <array>
#include <cstddef>
#include <cstdint>
#include <span>
#include <vector>

namespace deform::recovered {

// Internal composition of recovered mesh replacement and refresh paths, not an
// application model or public ABI. Component counts and defaults come from the
// caller; this class does not infer them from dimensions, units or keywords.
//
// Supported ownership is private storage. Original reference-counted sharing,
// detachment, allocation-failure behavior and unknown extra direct slots are
// not reconstructed. Only unit multiplicities for IDs 38/43/45 are supported;
// the caller must establish that regime from the original control fields.
class MeshStorage {
public:
    static constexpr std::array<int, 28> node_ids{
        1, 2, 3, 4, 5, 6, 8, 9, 10, 11, 12, 13, 14, 15,
        16, 24, 25, 27, 28, 26, 29, 34, 35, 17, 30, 31, 32, 33};
    static constexpr std::array<int, 25> element_ids{
        38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50,
        51, 52, 53, 54, 55, 56, 57, 58, 59, 62, 63, 60};
    static constexpr std::array<int, 7> direct_integer_ids{
        18, 19, 20, 21, 22, 23, 61};

    struct BoundaryGroup {
        std::vector<std::int32_t> nodes; // 2D chain, without separators
        std::vector<BoundaryFace> faces; // 3D oriented faces and owning elements
    };

    explicit MeshStorage(std::array<int, 3> element_multiplicities = {1, 1, 1},
                         int initial_arity = 4);

    // Clear occurs before allocation. Callers write rows directly afterwards:
    // an interrupted import leaves its already-written prefix in this storage.
    // A zero count clears storage without invoking descriptor allocation.
    // Unsupported arguments are rejected before any mutation. Resource failure
    // after clearing is not transactional and has no reference-equivalence claim.
    void begin_coordinate_replace(int count, std::span<const int> component_counts,
                                  double node8_default);
    void begin_connectivity_replace(int count, int arity,
                                    std::span<const int> component_counts,
                                    std::int32_t direct61_default);

    // 199650 -> 180150/183770: preserve storage using each container's own
    // allocation count and arity, independently of object header counts.
    // A nonpositive container count is a no-op. Descriptor allocation runs
    // without clear or replacement default fills. All extents are validated
    // before mutation; resource failure during resize is not transactional.
    void refresh_components(std::span<const int> component_counts);
    // 1815b0/181190 and184a30/184150: selective allocation without clear,
    // using stored container counts. Only recovered getter IDs are accepted.
    [[nodiscard]] std::span<double> select_node_field(int id, std::span<const int> components);
    [[nodiscard]] std::span<double> select_element_field(int id, std::span<const int> components);
    // 183840: first selective allocation, then save the root width in fc/100.
    // The subsequent getter performs a second allocation. Both skip allocation
    // for an empty container; saved widths survive ordinary free/refresh.
    void set_material_shape_width(int id, std::span<const int> components);
    [[nodiscard]] std::array<int, 2> material_shape_saved_widths() const noexcept {
        return material_shape_saved_widths_;
    }

    // The only temporary-object values read back by1d0fc0(option1) are these
    // two material-column arrays. Snapshot uses object header rows/old root
    // width; remap follows preserving allocation and writes only that prefix.
    struct MaterialColumnSnapshot {
        int rows = 0;
        int width = 0;
        std::array<std::vector<double>, 2> values;
    };
    [[nodiscard]] MaterialColumnSnapshot capture_material_columns(int old_width) const;
    void remap_material_columns(const MaterialColumnSnapshot& before, int new_width,
        std::span<const int> old_ranks, std::span<const int> new_ranks);

    // Successful ELMCON completion: sets mesh-present, then the boundary portion
    // of 0x1401fc290. Call only after all connectivity rows succeed and after
    // establishing a valid root context and permission to build boundaries.
    // Supports dimension 2/arity 4 and dimension 3/arity 4/8.
    // Marks mesh present even without connectivity. Suppression preserves
    // existing boundaries, matching root byte +1b5c in the import handler.
    // Dependent boundary-state arrays and geometric finalization remain absent.
    void finalize_connectivity(int dimension, bool build_boundaries = true);

    // Mesh-owned portion of object deactivation (0x140161260/0x14015fbf0).
    // Keeps selected arity, descriptor shape history and active group count,
    // while releasing each group's payload. Identity/type/name live outside
    // this component; this is not complete application-object cleanup.
    void deactivate();

    [[nodiscard]] int node_count() const noexcept { return node_count_; }
    [[nodiscard]] int element_count() const noexcept { return element_count_; }
    [[nodiscard]] int arity() const noexcept { return arity_; }
    [[nodiscard]] int node_allocation_count() const noexcept { return node_allocation_count_; }
    [[nodiscard]] int element_allocation_count() const noexcept { return element_allocation_count_; }
    [[nodiscard]] int allocation_arity() const noexcept { return allocation_arity_; }
    [[nodiscard]] bool mesh_present() const noexcept { return mesh_present_; }
    // Original realloc leaves newly grown direct-buffer tails unspecified.
    // Only these linear prefixes have reference-equivalent initialized values.
    // Vector tails use zero placeholders, which must NOT be interpreted as
    // original values. Mutable spans remain available, but writes through them
    // do not automatically extend this conservative validity accounting.
    [[nodiscard]] std::size_t coordinates_initialized_prefix() const noexcept {
        return coordinates_initialized_prefix_;
    }
    [[nodiscard]] std::size_t connectivity_initialized_prefix() const noexcept {
        return connectivity_initialized_prefix_;
    }
    [[nodiscard]] std::size_t direct_integers_initialized_prefix(int id) const;
    // Extend the conservative prefix from actual in-place reader writes.
    // Written islands beyond an unknown gap do not imply a known whole tail.
    void record_direct_integer_writes(int id, const std::vector<bool>& written);
    [[nodiscard]] std::span<double> coordinates() noexcept { return coordinates_; }
    [[nodiscard]] std::span<const double> coordinates() const noexcept { return coordinates_; }
    [[nodiscard]] std::span<std::int32_t> connectivity() noexcept { return connectivity_; }
    [[nodiscard]] std::span<const std::int32_t> connectivity() const noexcept { return connectivity_; }
    [[nodiscard]] std::span<std::int32_t> direct_integers(int id);
    [[nodiscard]] std::span<const std::int32_t> direct_integers(int id) const;
    [[nodiscard]] std::span<double> node_values(int id);
    [[nodiscard]] std::span<double> element_values(int id);
    [[nodiscard]] std::span<const StateVariable> node_states() const noexcept { return node_states_; }
    [[nodiscard]] std::span<const StateVariable> element_states() const noexcept { return element_states_; }
    [[nodiscard]] std::span<const BoundaryGroup> boundary_groups() const noexcept { return boundary_groups_; }
    [[nodiscard]] const PlanarBoundary& planar_chains() const noexcept { return planar_chains_; }
    [[nodiscard]] int boundary_total() const noexcept { return boundary_total_; }

private:
    int node_count_ = 0;
    int element_count_ = 0;
    int arity_ = 4; // Object's selected arity, not the container's allocated width.
    int node_allocation_count_ = 0; // Node container +0x12c.
    int element_allocation_count_ = 0; // Element container +0xe8.
    int allocation_arity_ = 0; // Element container +0xec; free resets this.
    std::array<int, 2> material_shape_saved_widths_{}; // Element container fc/100.
    bool mesh_present_ = false;
    std::vector<double> coordinates_;
    std::vector<std::int32_t> connectivity_;
    std::array<std::vector<std::int32_t>, 7> direct_integers_;
    std::size_t coordinates_initialized_prefix_ = 0;
    std::size_t connectivity_initialized_prefix_ = 0;
    std::array<std::size_t, 7> direct_integers_initialized_prefix_{};
    std::vector<StateVariable> node_states_;
    std::vector<StateVariable> element_states_;
    std::vector<BoundaryGroup> boundary_groups_;
    PlanarBoundary planar_chains_;
    int boundary_total_ = 0;
};

} // namespace deform::recovered
