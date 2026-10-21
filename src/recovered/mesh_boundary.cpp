#include "mesh_boundary.hpp"

#include <algorithm>
#include <cstddef>
#include <limits>
#include <stdexcept>
#include <utility>

namespace deform::recovered {
namespace {

// From the reference's qsort at 0x140484bd0 and shortsort at 0x140484b10.
// Index arithmetic replaces byte-pointer arithmetic; comparisons use only sum.
void legacy_sort(std::span<BoundaryKey> data, std::ptrdiff_t low,
                 std::ptrdiff_t high) {
    if (low >= high) return;
    auto at = [&](std::ptrdiff_t index) -> BoundaryKey& {
        return data[static_cast<std::size_t>(index)];
    };
    if (high - low + 1 <= 8) {
        for (; low < high; --high) {
            auto largest = low;
            for (auto candidate = low + 1; candidate <= high; ++candidate)
                if (at(candidate).sum > at(largest).sum) largest = candidate;
            std::swap(at(largest), at(high));
        }
        return;
    }

    auto middle = low + (high - low + 1) / 2;
    if (at(low).sum > at(middle).sum) std::swap(at(low), at(middle));
    if (at(low).sum > at(high).sum) std::swap(at(low), at(high));
    if (at(middle).sum > at(high).sum) std::swap(at(middle), at(high));
    auto left = low, right = high;
    for (;;) {
        if (left < middle) {
            do { ++left; }
            while (left < middle && at(left).sum <= at(middle).sum);
        }
        if (left >= middle) {
            do { ++left; }
            while (left <= high && at(left).sum <= at(middle).sum);
        }
        do { --right; }
        while (right > middle && at(right).sum > at(middle).sum);
        if (left > right) break;
        std::swap(at(left), at(right));
        if (middle == right) middle = left;
    }

    ++right;
    if (middle < right) {
        do { --right; }
        while (right > middle && at(right).sum == at(middle).sum);
    }
    if (right <= middle) {
        do { --right; }
        while (right > low && at(right).sum == at(middle).sum);
    }
    // The reference processes the smaller partition first using an explicit
    // stack. These disjoint partitions have no observable cross-partition state.
    legacy_sort(data, low, right);
    legacy_sort(data, left, high);
}

std::int32_t sum_of(const std::array<std::int32_t, 4>& face) {
    const auto sum = static_cast<std::int64_t>(face[0]) + face[1] + face[2];
    if (sum > std::numeric_limits<std::int32_t>::max())
        throw std::overflow_error("face node sum exceeds recovered signed-int domain");
    return static_cast<std::int32_t>(sum);
}

} // namespace

void sort_boundary_keys(std::span<BoundaryKey> keys) {
    if (keys.size() > static_cast<std::size_t>(std::numeric_limits<std::int32_t>::max()))
        throw std::length_error("too many boundary sort records");
    if (!keys.empty()) legacy_sort(keys, 0, static_cast<std::ptrdiff_t>(keys.size()) - 1);
}

std::vector<BoundaryFace> tetrahedral_boundary(std::span<const Tetrahedron> elements) {
    if (elements.size() >
        static_cast<std::size_t>(std::numeric_limits<std::int32_t>::max()) / 4)
        throw std::length_error("too many tetrahedra");
    // First 12 int32 entries at 0x140605090, selected by arity 4.
    constexpr std::array<std::array<std::size_t, 3>, 4> local_faces{{
        {0, 2, 1}, {0, 1, 3}, {0, 3, 2}, {1, 2, 3}
    }};
    std::vector<BoundaryFace> candidates;
    std::vector<BoundaryKey> keys;
    candidates.reserve(elements.size() * 4);
    keys.reserve(elements.size() * 4);
    for (std::size_t element = 0; element < elements.size(); ++element) {
        const auto& nodes = elements[element];
        if (std::any_of(nodes.begin(), nodes.end(), [](auto node) { return node <= 0; }))
            throw std::invalid_argument("nonpositive tetrahedral node ID outside recovered domain");
        for (const auto& local : local_faces) {
            const std::array<std::int32_t, 4> face{
                nodes[local[0]], nodes[local[1]], nodes[local[2]], nodes[local[0]]
            };
            keys.push_back({sum_of(face), static_cast<std::int32_t>(candidates.size() + 1)});
            candidates.push_back({face, static_cast<std::int32_t>(element + 1)});
        }
    }
    sort_boundary_keys(keys);
    std::vector<BoundaryFace> result;
    for (std::size_t i = 0; i < keys.size(); ++i) {
        auto& face = candidates[static_cast<std::size_t>(keys[i].face_index - 1)];
        if (face.nodes[0] == 0) continue;
        bool paired = false;
        for (auto j = i + 1; j < keys.size() && keys[j].sum == keys[i].sum; ++j) {
            auto& other = candidates[static_cast<std::size_t>(keys[j].face_index - 1)];
            const bool same = std::all_of(face.nodes.begin(), face.nodes.begin() + 3,
                [&](auto node) {
                    return std::find(other.nodes.begin(), other.nodes.begin() + 3, node) !=
                           other.nodes.begin() + 3;
                });
            if (same) {
                other.nodes[0] = 0;
                paired = true;
                break;
            }
        }
        if (!paired) result.push_back(face);
    }
    // The wrapper orders equal-sum output by first node, then second node,
    // swapping the owner alongside its oriented face. Do not canonicalize faces.
    for (std::size_t i = 0; i < result.size(); ++i) {
        const auto sum = sum_of(result[i].nodes);
        for (auto j = i + 1; j < result.size() && sum_of(result[j].nodes) == sum; ++j) {
            if (result[j].nodes[0] < result[i].nodes[0] ||
                (result[j].nodes[0] == result[i].nodes[0] &&
                 result[j].nodes[1] < result[i].nodes[1]))
                std::swap(result[i], result[j]);
        }
    }
    return result;
}

} // namespace deform::recovered
