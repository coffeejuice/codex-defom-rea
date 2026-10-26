// defrt/ftn/units.hpp: unit-conversion helpers of the Fortran side (packet p1-12; in defrt because
// defmodel code calls them).
#pragma once

#include <cstddef>
#include <cstdint>

namespace def::rt::ftn {

// Globals of the unit system that these helpers read; declared here (packet p1-12) until the
// module that initialises them declares them (port-conventions section 5).
// @data 1406fe620  user unit factors, indexed by quantity (FUN_140235d00 mode 3)
// @data 1406fe668  temperature factor of the user unit system (FUN_140235d80 mode 3)
// @data 1406fe778  English/SI flag read by FUN_140235c10
struct UnitGlobals {
    double factor[9];
    double temp_factor;
    std::byte pad_50[0x158 - 0x50];
    std::int32_t unit_flag;
};
static_assert(offsetof(UnitGlobals, temp_factor) == 0x1406fe668 - 0x1406fe620);
static_assert(offsetof(UnitGlobals, unit_flag) == 0x1406fe778 - 0x1406fe620);
inline UnitGlobals g_units{};

// @src 140235c10  1 when the unit flag DAT_1406fe778 is 0, else 2
std::int32_t unit_system();
// @src 140235d00  conversion factor of quantity q: column 1 or 2 of the table at 140531490,
// or the user factor (mode 3); 1.0 for any other mode
double unit_factor(std::int32_t q, std::int32_t mode);
// @src 140235d80  temperature: mode 1 C->F, 2 F->C, 3 by the user unit system
double convert_temperature(std::int32_t mode, double t);

// Resets g_units (tests).
void reset_unit_globals();

} // namespace def::rt::ftn
