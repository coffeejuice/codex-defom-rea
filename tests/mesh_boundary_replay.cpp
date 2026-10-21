#include "recovered/mesh_boundary.hpp"

#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

int main() {
    try {
        std::string mode;
        std::size_t count = 0;
        if (!(std::cin >> mode >> count) || count > 100000)
            throw std::runtime_error("invalid replay header");
        if (mode == "sort") {
            std::vector<deform::recovered::BoundaryKey> keys(count);
            for (auto& key : keys)
                if (!(std::cin >> key.sum >> key.face_index))
                    throw std::runtime_error("incomplete sort input");
            deform::recovered::sort_boundary_keys(keys);
            for (const auto& key : keys)
                std::cout << key.sum << ' ' << key.face_index << '\n';
        } else if (mode == "tetra") {
            std::vector<deform::recovered::Tetrahedron> elements(count);
            for (auto& element : elements)
                for (auto& node : element)
                    if (!(std::cin >> node)) throw std::runtime_error("incomplete connectivity");
            const auto boundary = deform::recovered::tetrahedral_boundary(elements);
            for (const auto& face : boundary) {
                for (const auto node : face.nodes) std::cout << node << ' ';
                std::cout << face.owner_element << '\n';
            }
        } else if (mode == "hexa") {
            std::vector<deform::recovered::Hexahedron> elements(count);
            for (auto& element : elements)
                for (auto& node : element)
                    if (!(std::cin >> node)) throw std::runtime_error("incomplete connectivity");
            for (const auto& face : deform::recovered::hexahedral_boundary(elements)) {
                for (const auto node : face.nodes) std::cout << node << ' ';
                std::cout << face.owner_element << '\n';
            }
        } else {
            throw std::runtime_error("unknown replay mode");
        }
        return 0;
    } catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
        return 1;
    }
}
