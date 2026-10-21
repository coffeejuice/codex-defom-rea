#include "recovered/planar_boundary.hpp"

#include <iostream>
#include <stdexcept>
#include <vector>

int main() {
    try {
        std::size_t count = 0;
        if (!(std::cin >> count) || count > 100000)
            throw std::runtime_error("invalid planar replay count");
        std::vector<deform::recovered::PlanarElement> elements(count);
        for (auto& element : elements)
            for (auto& node : element)
                if (!(std::cin >> node)) throw std::runtime_error("incomplete connectivity");
        const auto boundary = deform::recovered::planar_boundary(elements);
        std::cout << "{\"ranges\":[";
        for (std::size_t i = 0; i < boundary.ranges.size(); ++i) {
            if (i) std::cout << ',';
            std::cout << '[' << boundary.ranges[i][0] << ',' << boundary.ranges[i][1] << ']';
        }
        std::cout << "],\"nodes\":[";
        for (std::size_t i = 0; i < boundary.nodes.size(); ++i) {
            if (i) std::cout << ',';
            std::cout << boundary.nodes[i];
        }
        std::cout << "]}\n";
        return 0;
    } catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
        return 1;
    }
}
