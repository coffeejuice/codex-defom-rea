// Knobs of the P12 test doubles (doubles.cpp).
#pragma once

#include "m2m/qt4.hpp"

namespace p12test {
using m2m::qt::String;
extern int flowNumberValue;        // flowNumber(op) (0x140050b20)
extern bool startsFromKeyValue;    // startsFromKey(op) (0x14005f230)
extern String projectDirValue;     // project folder (current directory after Open)
}  // namespace p12test
