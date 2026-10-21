#include "recovered/component_layout.hpp"

#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>

namespace {
std::int32_t read_integer() {
    std::int64_t value;
    if (!(std::cin >> value) ||
        value < std::numeric_limits<std::int32_t>::min() ||
        value > std::numeric_limits<std::int32_t>::max())
        throw std::runtime_error("Expected a signed int32 input");
    return static_cast<std::int32_t>(value);
}
deform::recovered::FeatureQueryResult read_feature() {
    const auto enabled = read_integer();
    if (enabled != 0 && enabled != 1)
        throw std::runtime_error("Feature enabled input must be 0 or 1");
    return {enabled != 0, read_integer()};
}
} // namespace

// Stdin: KIND (initialize|refresh); dimension;
// seven (enabled,width) pairs for IDs24,49,53,29,54,55,46;
// root86c,870,874,878,87c; root880,884; root1320,13f4,3398,33a8;
// root850; root888,88c,890; root33c8,33d8,11c4,123c,12b4.
// Exactly35 whitespace-separated integers after KIND, for either kind.
// Widths for IDs24/49/46 are unused. Stdout: JSON array of79 integers.
int main() {
    try {
        deform::recovered::ComponentLayoutInputs input;
        std::string kind;
        if (!(std::cin >> kind) || (kind != "initialize" && kind != "refresh"))
            throw std::runtime_error("Expected initialize or refresh");
        input.dimension = read_integer();
        input.feature24 = read_feature();
        input.feature49 = read_feature();
        input.feature53 = read_feature();
        input.feature29 = read_feature();
        input.feature54 = read_feature();
        input.feature55 = read_feature();
        input.feature46 = read_feature();
        for (auto& flag : input.root_0x86c_to_0x87c) flag = read_integer();
        input.root_0x880 = read_integer();
        input.root_0x884 = read_integer();
        input.root_0x1320 = read_integer();
        input.root_0x13f4 = read_integer();
        input.root_0x3398 = read_integer();
        input.root_0x33a8 = read_integer();
        input.root_0x850 = read_integer();
        input.root_0x888 = read_integer();
        input.root_0x88c = read_integer();
        input.root_0x890 = read_integer();
        input.root_0x33c8 = read_integer();
        input.root_0x33d8 = read_integer();
        input.root_0x11c4 = read_integer();
        input.root_0x123c = read_integer();
        input.root_0x12b4 = read_integer();
        std::string extra;
        if (std::cin >> extra) throw std::runtime_error("Unexpected trailing input");
        const auto counts = kind == "initialize"
            ? deform::recovered::initialize_layout(input)
            : deform::recovered::refresh_layout(input);
        std::cout << '[';
        for (std::size_t i = 0; i < counts.size(); ++i) {
            if (i) std::cout << ',';
            std::cout << counts[i];
        }
        std::cout << "]\n";
        return 0;
    } catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
        return 1;
    }
}
