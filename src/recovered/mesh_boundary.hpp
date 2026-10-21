#pragma once

#include <array>
#include <cstdint>
#include <span>
#include <vector>

namespace deform::recovered {

using Tetrahedron = std::array<std::int32_t, 4>;
using Hexahedron = std::array<std::int32_t, 8>;

struct BoundaryFace {
    // The legacy triangle record repeats its first node in the fourth slot.
    std::array<std::int32_t, 4> nodes;
    std::int32_t owner_element;
    bool operator==(const BoundaryFace&) const = default;
};

struct BoundaryKey {
    std::int32_t sum;
    std::int32_t face_index;
    bool operator==(const BoundaryKey&) const = default;
};

// Internal compatibility helper: reproduces the embedded sort's equal-key
// permutation, which influences ownership after pairwise face cancellation.
void sort_boundary_keys(std::span<BoundaryKey> keys);

// Recovered four-node, 3D branch only. Other element kinds remain separate work.
std::vector<BoundaryFace> tetrahedral_boundary(std::span<const Tetrahedron> elements);

// Eight-slot branch, including recovered repeated/zero-node classification.
std::vector<BoundaryFace> hexahedral_boundary(std::span<const Hexahedron> elements);

} // namespace deform::recovered
