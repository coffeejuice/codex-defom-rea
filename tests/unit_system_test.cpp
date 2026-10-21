#include "recovered/object_model.hpp"
#include <stdexcept>

using namespace deform::recovered;

namespace {
void require(bool condition, const char* reason) {
    if (!condition) throw std::runtime_error(reason);
}
}

int main() {
    // Source-contract check for owned buffers not populated by the native
    // discovery fixtures. This does not claim native populated-buffer parity.
    ObjectModel first(initialized_root_controls(3, 20));
    ObjectModel independent(initialized_root_controls(2, 20));
    require(!process_unit_state().tables, "presentation must remain lazy");
    first.set_unit(2);
    require(independent.node8_default() == 68, "process units must reach unlinked roots");
    require(independent.controls().simulation->units.selector_0x84c == 0,
            "global update must not rewrite independent root selector");

    auto seeded = initialized_root_controls(3, 20);
    auto& units = seeded.simulation->units;
    units.selector_0x84c = 1;
    units.scalars[0] = 19;
    for (auto& table : units.tables) {
        table.rank_0x04 = 1;
        table.selectors_0x10[0] = 3;
        table.counts_0x1c[0] = 2;
        table.axes[0] = {1, 2};
        table.dependent = {7, 8};
        table.preserved_0x54 = 71;
        table.preserved_0x60 = 0x123456789abcdef0;
    }
    ObjectModel root(std::move(seeded));
    root.set_unit(1); // Equal raw selector: do not repair different global mode.
    require(root.controls().simulation->units.scalars[0] == 19, "same unit must retain edits");
    require(root.controls().simulation->units.tables[0].dependent.size() == 2,
            "same unit must retain populated table");
    require(process_unit_state().mode == 1, "same root selector must not update globals");
    root.set_unit(3);
    require(root.controls().simulation->units.tables[0].preserved_0x54 == 71,
            "invalid selector must retain metadata");
    root.set_unit(2);
    for (const auto& table : root.controls().simulation->units.tables) {
        require(table.rank_0x04 == 0 && table.type_0x00 == 0, "reset table type/rank");
        require(table.axes[0].empty() && table.dependent.empty(), "reset owned buffers");
        require(table.preserved_0x54 == 0 && table.preserved_0x60 == 0,
                "UNIT must use destructive initialize, not preserving reset_type");
    }
    root.set_dimension(2);
    require(root.controls().simulation->units.selector_0x84c == 2,
            "dimension reset must preserve raw selector");
    require(root.controls().simulation->units.tables[0].constant_0x08 == 68,
            "dimension reset must regenerate selected defaults");
}
