#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <span>
#include <vector>

namespace deform::recovered {

// Selected _OBJ_GEO3D storage and finite plane grouping, not its public ABI.
// Owned vectors replace native sharing; native allocation failures and malformed
// geometry have no compatibility claim. Build without floating reassociation/FMA.
class GeometryStorage {
public:
    struct Plane {
        std::int32_t id = 0;
        std::array<double, 3> origin{};
        std::array<double, 3> normal{};
        bool operator==(const Plane&) const = default;
    };

    // 2fcd90: positive counts resize with prefix preservation and zero growth;
    // nonpositive counts keep the corresponding old vector. Classification is
    // freshly zeroed whenever the effective face count is positive. Aux68 keeps
    // its prefix. Already-written rows survive a subsequent interrupted reader.
    void begin_replace(int coordinate_count, int face_count);
    void write_coordinate(std::size_t zero_based_row, std::span<const double, 3> row);
    void write_face(std::size_t zero_based_row, std::span<const std::int32_t, 4> row);

    // 2059a0 after the caller establishes object+3990. Empty copied_planes is
    // the selected import path (argument0). Optional input models argument1's
    // plane-list copy. IDs are retained; fresh IDs use the first positive gap.
    // Nonfinite inputs/intermediates and unsafe eligible indices throw. Such
    // failures are outside native equivalence and need not be transactional.
    void finalize_planes(std::span<const Plane> copied_planes = {});
    [[nodiscard]] double bounding_diagonal() const;

    // Virtual+0x10 (2f92b0) preserves planes; cleanup2f9340->2fcde0 clears them.
    // Only fields represented here are covered by either operation.
    void reset() noexcept;
    void deactivate() noexcept;

    [[nodiscard]] int coordinate_count() const noexcept;
    [[nodiscard]] int face_count() const noexcept;
    [[nodiscard]] std::span<double> coordinates() noexcept { return coordinates_; }
    [[nodiscard]] std::span<const double> coordinates() const noexcept { return coordinates_; }
    [[nodiscard]] std::span<std::int32_t> connectivity() noexcept { return connectivity_; }
    [[nodiscard]] std::span<const std::int32_t> connectivity() const noexcept { return connectivity_; }
    [[nodiscard]] std::span<std::int32_t> classifications() noexcept { return classifications_; }
    [[nodiscard]] std::span<const std::int32_t> classifications() const noexcept { return classifications_; }
    [[nodiscard]] std::span<std::int32_t> auxiliary68() noexcept { return auxiliary68_; }
    [[nodiscard]] std::span<const std::int32_t> auxiliary68() const noexcept { return auxiliary68_; }
    [[nodiscard]] std::span<const Plane> planes() const noexcept { return planes_; }

private:
    std::vector<double> coordinates_;
    std::vector<std::int32_t> connectivity_;
    std::vector<std::int32_t> classifications_;
    std::vector<std::int32_t> auxiliary68_;
    std::vector<Plane> planes_;
};

} // namespace deform::recovered
