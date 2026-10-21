#include "planar_boundary.hpp"

#include <algorithm>
#include <limits>
#include <stdexcept>

namespace deform::recovered {

PlanarBoundary planar_boundary(std::span<const PlanarElement> elements) {
    if (elements.size() >
        static_cast<std::size_t>(std::numeric_limits<std::int32_t>::max()) / 12)
        throw std::length_error("planar boundary stream exceeds signed-int domain");
    using Edge = std::array<std::int32_t, 2>;
    std::vector<Edge> edges;
    edges.reserve(elements.size() * 4);
    for (const auto& element : elements) {
        if (std::any_of(element.begin(), element.end(), [](auto node) { return node <= 0; }))
            throw std::invalid_argument("nonpositive planar node ID outside recovered domain");
        for (std::size_t i = 0; i < 4; ++i) {
            if (i == 3 && element[0] == element[3]) break;
            edges.push_back({element[i], element[(i + 1) % 4]});
        }
    }
    // 0x14032da62 pairs reversed directed edges only, then compacts in place.
    std::size_t surviving = 0;
    for (std::size_t i = 0; i < edges.size(); ++i) {
        if (edges[i][0] == 0) continue;
        bool paired = false;
        for (auto j = i + 1; j < edges.size(); ++j) {
            if (edges[j][0] == edges[i][1] && edges[j][1] == edges[i][0]) {
                edges[j][0] = 0;
                paired = true;
                break;
            }
        }
        if (!paired) edges[surviving++] = edges[i];
    }
    edges.resize(surviving);

    PlanarBoundary result;
    for (std::size_t start = 0; start < edges.size(); ++start) {
        if (edges[start][0] == 0) continue;
        const auto first = static_cast<std::int32_t>(result.nodes.size() + 1);
        result.nodes.push_back(edges[start][0]);
        auto current = start;
        for (;;) {
            const auto endpoint = edges[current][1];
            edges[current][0] = 0;
            result.nodes.push_back(endpoint);
            // Always restart after the outer starting edge, even if the
            // previous link was farther along. Input ordering is observable.
            auto next = start + 1;
            while (next < edges.size() && edges[next][0] != endpoint) ++next;
            if (next == edges.size()) break;
            current = next;
        }
        result.ranges.push_back({first, static_cast<std::int32_t>(result.nodes.size())});
        result.nodes.push_back(0);
    }
    if (result.nodes.empty()) result.nodes.push_back(-1);
    else result.nodes.back() = -1;
    return result;
}

} // namespace deform::recovered
