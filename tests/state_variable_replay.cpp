#include "recovered/state_variable.hpp"

#include <array>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>

// Test driver, not an application compatibility interface. The Python verifier
// supplies independently captured original descriptor states and compares output.
int main() {
    try {
        int id = 0, policy = 0;
        if (!(std::cin >> id >> policy)) throw std::runtime_error("missing descriptor");
        if (policy < 0 || policy > 2) throw std::runtime_error("invalid policy");
        deform::recovered::StateVariable variable(
            id, static_cast<deform::recovered::StateVariable::Policy>(policy));
        std::array<int, 79> components{};
        for (auto& width : components)
            if (!(std::cin >> width)) throw std::runtime_error("missing component");
        std::cout << std::setprecision(17);
        std::string operation;
        while (std::cin >> operation) {
            if (operation == "clear") {
                variable.clear();
            } else if (operation == "allocate") {
                int count = 0, selector = 0, multiplier = 0;
                if (!(std::cin >> count >> selector >> multiplier))
                    throw std::runtime_error("incomplete allocation");
                variable.allocate(count, components, selector,
                    multiplier < 0 ? std::nullopt : std::optional<int>{multiplier});
            } else if (operation == "fill") {
                double value = 0;
                if (!(std::cin >> value)) throw std::runtime_error("incomplete fill");
                for (auto& item : variable.values()) item = value;
            } else if (operation == "snapshot") {
                const auto now = variable.shape(), old = variable.previous_shape();
                std::cout << now.count << ' ' << now.width << ' ' << now.multiplicity
                          << ' ' << old.count << ' ' << old.width << ' '
                          << old.multiplicity << ' ' << variable.values().size();
                for (const auto value : variable.values()) std::cout << ' ' << value;
                std::cout << '\n';
            } else {
                throw std::runtime_error("unknown replay operation");
            }
        }
        return 0;
    } catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
        return 1;
    }
}
