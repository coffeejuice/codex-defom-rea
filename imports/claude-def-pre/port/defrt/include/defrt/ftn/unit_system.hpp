// defrt/ftn/unit_system.hpp: selecting the unit system (FUN_140235c30 and the conversion tables it
// rebuilds, FUN_1403f8b70 / FUN_1403f9f20). Ported at the KEY merge; the p1-12 helpers that read
// the flag are in defrt/ftn/units.hpp.
//
// In batch mode only the flag DAT_1406fe778 (read by unit_system(), 140235c10) is consumed; the
// CONV_FAC / CONV_UNIT tables are rebuilt as the original does, but their readers (1403f8bc0,
// 1403f9740 ...) are not on any batch path (not in the batch ledger).
#pragma once

#include <cstddef>
#include <cstdint>

namespace def::rt::ftn {

// One CONV_FAC record (0x88 bytes, calloc'd by 1403fb360): quantity id, name, four factors.
struct ConvFac {
    std::int32_t id;     // 0x00
    char name[100];      // 0x04
    double f[4];         // 0x68 .. 0x80
};
static_assert(sizeof(ConvFac) == 0x88);

// One CONV_UNIT record (0x1f8 bytes, calloc'd by 1403fb3c0): quantity id and five labels.
struct ConvUnit {
    std::int32_t id;     // 0x000
    char label[5][100];  // 0x004, 0x068, 0x0cc, 0x130, 0x194
};
static_assert(sizeof(ConvUnit) == 0x1f8);

// @data 14070ba70  conversion-table globals (FUN_1403f9f20)
struct ConvGlobals {
    std::int32_t english;  // 0x14070ba70 0 = SI labels, 1 = English labels (1403f8b70)
    std::int32_t pad74;
    ConvUnit* units;       // 0x14070ba78 CONV_UNIT table
    ConvFac* factors;      // 0x14070ba80 CONV_FAC table
    std::int32_t count;    // 0x14070ba88 = 28
    std::int32_t u8c;      // 0x14070ba8c = 6000
    std::int32_t u90;      // 0x14070ba90 = 4
    std::int32_t u94;      // 0x14070ba94 = 0
};
static_assert(offsetof(ConvGlobals, units) == 0x14070ba78 - 0x14070ba70);
static_assert(offsetof(ConvGlobals, u94) == 0x14070ba94 - 0x14070ba70);
inline ConvGlobals g_conv{};

// @src 140235c30  set the unit system: 2 = English (flag 1), anything else SI (flag 0), then
// 1403f8b70(unit)
void set_unit_system(std::int32_t unit);
// @src 1403f8b70  the conversion tables' flag (2 -> 1, else 0), then rebuild them (1403f9f20)
void set_conversion_unit_system(std::int32_t unit);
// @src 1403f9f20  (re)build the CONV_FAC and CONV_UNIT tables from the static records
void build_conversion_tables();
// @src 1403fb360  calloc(n, 0x88), null for n == 0
ConvFac* alloc_conv_factors(std::int32_t n);
// @src 1403fb3c0  calloc(n, 0x1f8), null for n == 0
ConvUnit* alloc_conv_units(std::int32_t n);

} // namespace def::rt::ftn
