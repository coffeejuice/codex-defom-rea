// m2m/data/cycle.hpp: CycleData (SfOprCycleProperties+0x38, 0x10 bytes, ctor 0x14134ef10), s1-04 2.
// Owner: P06 (Cycle.CWZ reader 0x14134ec40 sets the count). P13 (writer 0x14134dcc0) reads it.
#pragma once

namespace m2m::data {

struct CycleData {
    int count = 5;                 // +0x00 WZD_CYCLE_COUNT (0x14134ee90); flow chart CycleCount
    // +0x08 list (0x14015d170), not used by the writer
};

}  // namespace m2m::data
