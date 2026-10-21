#include "unit_system.hpp"
#include <bit>

namespace deform::recovered {
namespace {
ProcessUnitState process_units;
#include "unit_source_data.inc"
constexpr std::array<std::array<std::uint64_t, 8>, 2> profiles{{
    {0x3ff0000000000000,0x3dcf2a6880c4f0aa,0x4034000000000000,0x3f947ae147ae147b,
     0x3ffb0a3d70a3d70a,0x3f30624dd2f1a9fc,0x3e15a589e1f37f03,0x3d03d0dac864deb1},
    {0x3fbb645a1cac0831,0x3cedc71df2f4abbd,0x4051000000000000,0x3ee025e7f1158172,
     0x3ffb0a3d70a3d70a,0x3ee4f8b588e368f1,0x3e61204d7839eab6,0x3d4faa7ab552a552}
}};
}

RootUnitControls::RootUnitControls() { reset_defaults(); }

void RootUnitControls::reset_defaults() {
    for (auto& table : tables) table.initialize(0);
    const auto& p = profiles[selector_0x84c == 2 ? 1 : 0];
    scalars = {std::bit_cast<double>(p[0]),std::bit_cast<double>(p[1]),
               std::bit_cast<double>(p[6]),std::bit_cast<double>(p[7])};
    for (std::size_t i = 0; i < tables.size(); ++i)
        tables[i].constant_0x08 = std::bit_cast<double>(p[i+2]);
}

std::array<double, 8> RootUnitControls::scalar_values() const {
    return {scalars[0],scalars[1],tables[0].constant_0x08,tables[1].constant_0x08,
            tables[2].constant_0x08,tables[3].constant_0x08,scalars[2],scalars[3]};
}

const ProcessUnitState& process_unit_state() { return process_units; }

void select_process_unit(std::int32_t selector) {
    process_units.mode = selector == 2 ? 1 : 0;
    process_units.display_mode = process_units.mode;
    process_units.limit = 6000;
    process_units.count = 28;
    process_units.field94 = 0;
    process_units.field90 = 4;
    if (!process_units.tables) process_units.tables.emplace();
    auto& tables = *process_units.tables;
    tables.numeric = source_numeric;
    tables.labels = source_labels;
    for (auto& record : tables.numeric) record.bits[3] = record.bits[0];
    for (auto& record : tables.labels) {
        const auto value = record.text[process_units.display_mode == 0 ? 3 : 2];
        record.text[1] = value;
        record.text[4] = value;
    }
}
} // namespace deform::recovered
