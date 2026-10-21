#pragma once

#include <array>
#include <cstdint>
#include <span>
#include <vector>

namespace deform::recovered {

using PlanarElement = std::array<std::int32_t, 4>;

struct PlanarBoundary {
    // Inclusive one-based ranges in nodes, excluding inter-chain separators.
    std::vector<std::array<std::int32_t, 2>> ranges;
    // Zero separators between chains; final separator is -1. Empty result: {-1}.
    std::vector<std::int32_t> nodes;
};

PlanarBoundary planar_boundary(std::span<const PlanarElement> elements);

} // namespace deform::recovered
