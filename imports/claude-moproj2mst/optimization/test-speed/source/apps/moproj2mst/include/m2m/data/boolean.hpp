// m2m/data/boolean.hpp: BooleanData (SfOprBooleanProperties+0x38, 0x18 bytes, ctor 0x1411e23f0),
// s1-04 1.1. Owner: P06 (Boolean.BWZ reader 0x1411e1dd0 fills it). P12 (writer) reads it.
#pragma once

#include <cstdint>

namespace m2m::data {

struct BooleanData {
    // +0x00 KEY token reader base (0x140906740): datafiles::TokenReader, made by the reader itself
    int workpiece = 1;             // +0x04 WZD_MACHDIST_WORKPIECE_OBJECT_NO
    int cutter = 0;                // +0x08 WZD_BOOLEAN_CUTTER_OBJECT_NO
    std::uint8_t remesh = 0;       // +0x0c WZD_BOOLEAN_REMESH (token type 1)
    // +0x10 owner SfOprBooleanProperties* (Props::owner here)
};

}  // namespace m2m::data
