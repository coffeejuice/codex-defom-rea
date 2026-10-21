#include "mesh_boundary.hpp"

#include <algorithm>
#include <limits>
#include <stdexcept>

namespace deform::recovered {
namespace {

struct ElementClass {
    int nodes = 8;
    int faces = 6;
    int width = 4;
};

ElementClass classify(const Hexahedron& element) {
    const auto zeros = std::count(element.begin(), element.end(), 0);
    std::ptrdiff_t multiplicity = 0;
    for (auto node : element)
        multiplicity = std::max(multiplicity,
                               std::count(element.begin(), element.end(), node));
    // Branch precedence from 0x14035672f is observable for mixed degeneracies.
    if (zeros == 2 || multiplicity == 2) return {6, 5, 4};
    if (zeros == 4 || multiplicity == 4) return {4, 4, 3};
    return {};
}

std::int32_t face_sum(const std::array<std::int32_t, 4>& nodes) {
    std::int64_t sum = 0;
    for (auto node : nodes) sum += node;
    if (sum > std::numeric_limits<std::int32_t>::max())
        throw std::overflow_error("hexahedral face sum exceeds recovered signed-int domain");
    return static_cast<std::int32_t>(sum);
}

struct Candidate {
    std::array<std::int32_t, 4> nodes;
    ElementClass type;
};

} // namespace

std::vector<BoundaryFace> hexahedral_boundary(std::span<const Hexahedron> elements) {
    if (elements.size() >
        static_cast<std::size_t>(std::numeric_limits<std::int32_t>::max()) / 6)
        throw std::length_error("too many eight-slot elements");

    // Tables selected at 0x140355c23: 0x140604fd0, 0x140604f10, 0x140605030.
    constexpr std::array<std::array<std::size_t, 4>, 6> standard{{
        {0,3,2,1}, {4,5,6,7}, {0,1,5,4},
        {1,2,6,5}, {2,3,7,6}, {3,0,4,7}
    }};
    constexpr std::array<std::array<std::size_t, 4>, 6> wedge{{
        {0,2,1,0}, {3,4,5,3}, {0,1,4,3},
        {1,2,5,4}, {2,0,3,5}, {2,2,5,5}
    }};
    constexpr std::array<std::array<std::size_t, 3>, 4> tetra{{
        {0,2,1}, {0,1,3}, {0,3,2}, {1,2,3}
    }};
    std::vector<Candidate> candidates;
    std::vector<BoundaryKey> keys;
    candidates.reserve(elements.size() * 6);
    keys.reserve(elements.size() * 6);
    for (const auto& element : elements) {
        if (std::any_of(element.begin(), element.end(), [](auto node) { return node < 0; }))
            throw std::invalid_argument("negative node ID outside recovered domain");
        const auto type = classify(element);
        // The original visits six table rows even for collapsed tetrahedra.
        // Rows five/six contain zero indices: all three fetched values coincide
        // and are always rejected by its duplicate-pair filter. Omit those reads.
        const auto rows = type.nodes == 4 ? 4 : 6;
        for (int row = 0; row < rows; ++row) {
            std::array<std::int32_t, 4> face{};
            if (type.nodes == 4) {
                for (int i = 0; i < 3; ++i) face[i] = element[tetra[row][i]];
            } else {
                const auto& local = type.nodes == 6 ? wedge[row] : standard[row];
                for (int i = 0; i < 4; ++i) face[i] = element[local[i]];
            }
            int duplicate_pairs = 0;
            for (int i = 0; i < 4; ++i)
                for (int j = i + 1; j < 4; ++j)
                    if (face[i] == face[j]) ++duplicate_pairs;
            if (duplicate_pairs >= 2 || std::count(face.begin(), face.end(), 0) >= 2)
                continue;
            keys.push_back({face_sum(face),
                            static_cast<std::int32_t>(candidates.size() + 1)});
            candidates.push_back({face, type});
        }
    }
    sort_boundary_keys(keys);
    std::vector<BoundaryFace> result;
    for (std::size_t i = 0; i < keys.size(); ++i) {
        const auto index = static_cast<std::size_t>(keys[i].face_index - 1);
        auto& candidate = candidates[index];
        if (candidate.nodes[0] == 0) continue;
        bool paired = false;
        for (auto j = i + 1; j < keys.size() && keys[j].sum == keys[i].sum; ++j) {
            auto& other = candidates[static_cast<std::size_t>(keys[j].face_index - 1)];
            const int other_width =
                candidate.type.width == 4 && other.type.width == 4 ? 4 : 3;
            bool same = true;
            for (int k = 0; k < candidate.type.width; ++k)
                if (std::find(other.nodes.begin(), other.nodes.begin() + other_width,
                              candidate.nodes[k]) == other.nodes.begin() + other_width)
                    same = false;
            if (same) {
                other.nodes[0] = 0;
                paired = true;
                break;
            }
        }
        if (paired) continue;
        auto face = candidate.nodes;
        if (candidate.type.width == 3) face[3] = face[0];
        else if (candidate.type.nodes == 6) {
            int destination = 0;
            for (auto node : candidate.nodes)
                if (node > 0) face[destination++] = node;
            if (destination < 4) face[3] = face[0];
        }
        // Preserve the original compressed-index formula, including mixed
        // classes; substituting the source element number changes behavior.
        result.push_back({face, static_cast<std::int32_t>(
            index / static_cast<std::size_t>(candidate.type.faces) + 1)});
    }
    for (std::size_t i = 0; i < result.size(); ++i) {
        const auto sum = face_sum(result[i].nodes);
        for (auto j = i + 1; j < result.size() && face_sum(result[j].nodes) == sum; ++j) {
            if (result[j].nodes[0] < result[i].nodes[0] ||
                (result[j].nodes[0] == result[i].nodes[0] &&
                 result[j].nodes[1] < result[i].nodes[1]))
                std::swap(result[i], result[j]);
        }
    }
    return result;
}

} // namespace deform::recovered
