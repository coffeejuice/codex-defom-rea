#include "recovered/mesh_storage.hpp"

#include <array>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>
#include <type_traits>

namespace {
using deform::recovered::MeshStorage;
using deform::recovered::StateVariable;

template<class Range>
void numbers(const Range& values) {
    std::cout << '[';
    bool first = true;
    for (const auto value : values) {
        if (!first) std::cout << ',';
        first = false;
        std::cout << value;
    }
    std::cout << ']';
}

void shape(StateVariable::Shape value) {
    std::cout << "{\"count\":" << value.count << ",\"width\":" << value.width
              << ",\"multiplicity\":" << value.multiplicity << '}';
}

void descriptors(std::span<const StateVariable> states) {
    std::cout << '[';
    bool first = true;
    for (const auto& state : states) {
        if (!first) std::cout << ',';
        first = false;
        std::cout << "{\"id\":" << state.id() << ",\"shape\":";
        shape(state.shape());
        std::cout << ",\"previous\":";
        shape(state.previous_shape());
        std::cout << ",\"values\":";
        numbers(state.values());
        std::cout << '}';
    }
    std::cout << ']';
}

void snapshot(const MeshStorage& mesh) {
    std::cout << "{\"nodes\":" << mesh.node_count()
              << ",\"elements\":" << mesh.element_count()
              << ",\"arity\":" << mesh.arity()
              << ",\"mesh_present\":" << (mesh.mesh_present() ? "true" : "false")
              << ",\"coordinates\":";
    numbers(mesh.coordinates());
    std::cout << ",\"connectivity\":";
    numbers(mesh.connectivity());
    std::cout << ",\"direct_integer_buffers\":{";
    bool first = true;
    for (const int id : MeshStorage::direct_integer_ids) {
        if (!first) std::cout << ',';
        first = false;
        std::cout << '"' << id << "\":";
        numbers(mesh.direct_integers(id));
    }
    std::cout << "},\"descriptors\":{\"node\":";
    descriptors(mesh.node_states());
    std::cout << ",\"element\":";
    descriptors(mesh.element_states());
    std::cout << "},\"boundaries\":[";
    first = true;
    for (const auto& group : mesh.boundary_groups()) {
        if (!first) std::cout << ',';
        first = false;
        std::cout << "{\"nodes\":";
        numbers(group.nodes);
        std::cout << ",\"faces\":[";
        bool first_face = true;
        for (const auto& face : group.faces) {
            if (!first_face) std::cout << ',';
            first_face = false;
            numbers(face.nodes);
        }
        std::cout << "],\"owners\":[";
        first_face = true;
        for (const auto& face : group.faces) {
            if (!first_face) std::cout << ',';
            first_face = false;
            std::cout << face.owner_element;
        }
        std::cout << "]}";
    }
    std::cout << "],\"planar_chains\":{\"ranges\":[";
    first = true;
    for (const auto& range : mesh.planar_chains().ranges) {
        if (!first) std::cout << ',';
        first = false;
        numbers(range);
    }
    std::cout << "],\"nodes\":";
    numbers(mesh.planar_chains().nodes);
    std::cout << "},\"total\":" << mesh.boundary_total() << "}\n";
}

template<class T>
void read_prefix(std::span<T> destination) {
    std::size_t count = 0;
    if (!(std::cin >> count) || count > destination.size())
        throw std::runtime_error("invalid prefix length");
    for (std::size_t i = 0; i < count; ++i) {
        T value{};
        if (!(std::cin >> value)) throw std::runtime_error("incomplete prefix");
        if constexpr (std::is_floating_point_v<T>)
            if (!std::isfinite(value)) throw std::runtime_error("nonfinite replay value");
        destination[i] = value;
    }
}
} // namespace

// Test protocol, not a keyword importer or a recovered public interface.
// Supply 79 component widths, then whitespace-delimited commands. set_* writes
// a prefix, allowing an independently prepared driver to model partial rows.
int main() {
    try {
        std::array<int, 79> components{};
        for (auto& width : components)
            if (!(std::cin >> width)) throw std::runtime_error("missing component");
        MeshStorage mesh({1, 1, 1}, 4);
        std::cout << std::setprecision(17);
        std::string operation;
        while (std::cin >> operation) {
            if (operation == "nodes") {
                int count = 0;
                double node8_default = 0;
                if (!(std::cin >> count >> node8_default))
                    throw std::runtime_error("incomplete nodes command");
                mesh.begin_coordinate_replace(count, components, node8_default);
            } else if (operation == "elements") {
                int count = 0, arity = 0;
                std::int32_t direct61_default = 0;
                if (!(std::cin >> count >> arity >> direct61_default))
                    throw std::runtime_error("incomplete elements command");
                mesh.begin_connectivity_replace(count, arity, components, direct61_default);
            } else if (operation == "set_coordinates") {
                read_prefix(mesh.coordinates());
            } else if (operation == "set_connectivity") {
                read_prefix(mesh.connectivity());
            } else if (operation == "finalize") {
                int dimension = 0;
                if (!(std::cin >> dimension)) throw std::runtime_error("missing dimension");
                mesh.finalize_connectivity(dimension);
            } else if (operation == "snapshot") {
                snapshot(mesh);
            } else if (operation == "deactivate") {
                mesh.deactivate();
            } else {
                throw std::runtime_error("unknown mesh replay operation");
            }
        }
        return 0;
    } catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
        return 1;
    }
}
