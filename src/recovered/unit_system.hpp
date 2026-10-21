#pragma once

#include "property_table.hpp"
#include <array>
#include <cstdint>
#include <optional>
#include <string>

namespace deform::recovered {

// Per-root controls are distinct from the two process-global modes.
// Offsets identify recovered storage, without asserting scientific meanings.
struct RootUnitControls {
    std::int32_t selector_0x84c = 0;
    std::array<PropertyTable, 4> tables; // b18,c10,c88,d00.
    std::array<double, 4> scalars{}; // b00,c08,d78,d80.
    RootUnitControls();
    void reset_defaults(); // 1f4100; initialize(0) clears table metadata/storage.
    [[nodiscard]] std::array<double, 8> scalar_values() const;
};

struct UnitNumericRecord {
    std::int32_t id;
    std::string text;
    std::array<std::uint64_t, 4> bits;
};
struct UnitTextRecord {
    std::int32_t id;
    std::array<std::string, 5> text;
};
struct UnitPresentationTables {
    std::array<UnitNumericRecord, 28> numeric;
    std::array<UnitTextRecord, 28> labels;
};
struct ProcessUnitState {
    std::int32_t mode = 0; // 6fe778; normalized selector2 only.
    std::int32_t display_mode = 0; // 70ba70.
    std::int32_t count = 0, limit = 0, field90 = 0, field94 = 0;
    std::optional<UnitPresentationTables> tables;
};
// One process-wide state, shared by every root. Initial explicit reset profiles
// retain their asserted node8_default until a recovered global setter runs.
[[nodiscard]] const ProcessUnitState& process_unit_state();
void select_process_unit(std::int32_t selector); // 235c30 ->3f8b70 ->3f9f20.

} // namespace deform::recovered
