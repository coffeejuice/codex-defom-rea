// Knobs of the P15 test doubles (doubles.cpp).
#pragma once

#include <map>
#include <string>

namespace p15test {
extern int flowNumberValue;            // flowNumber(op)
extern int nextSimValue;               // nextSim(...) result
extern int nextSimArg;                 // last ord passed to nextSim
extern bool is2DValue;                 // is2D(op)
extern bool unitOk;                    // evalUnitString ok flag
extern std::map<std::u16string, double> unitValues;   // evalUnitString text -> value
}  // namespace p15test
