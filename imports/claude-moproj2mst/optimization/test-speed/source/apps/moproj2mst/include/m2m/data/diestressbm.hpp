// m2m/data/diestressbm.hpp: SfOprDiestressBMProperties wizard block (PROPS+0x38), s1-05 3.
// Owner: P06 (DieStressBmWizard.Wzd reader 0x1402a1df0 fills it). P16 (writers) reads it.
// Offsets are relative to PROPS+0x38 (PROPS offset = value + 0x38).
#pragma once

#include <array>
#include <cstdint>

#include "m2m/qt4.hpp"

namespace m2m::data {

struct DiestressBMData {
    int objTotalNum = 0;                    // +0x000 nObjTotalNum
    std::array<std::uint8_t, 16> interpolate{};   // +0x004 + k bIntepolate
    std::array<int, 15> objNo{};            // +0x014 + 4k nObjNo
    std::array<int, 16> refNo{};            // +0x050 + 4k nRefNo
    std::array<double, 15> tolerance{};     // +0x090 + 8k dTolerance
    qt::String newDbName;                   // +0x108 qsNewDbName
    std::uint8_t useNewDb = 0;              // +0x110 bUseNewDb
    std::uint8_t f111 = 0;                  // +0x111 (MDT L6: 0x1402a15d0 sets 1; ctor value open)
    // Constructor defaults are not decompiled yet (open item).
};

}  // namespace m2m::data
