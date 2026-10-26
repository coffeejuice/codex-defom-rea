// Cls_1401fb020 (no RTTI, not polymorphic): leading sub-object (offset 0) of the inter-object
// record Cls_1401f46e0. Two curves of type 0x15 and twelve 8-byte values. Whether it is a base
// class or a first member cannot be told (no RTTI, no vtable); it is modelled as a member.
//
//   ctor     1401fb020 (curves at 0x18/0x90, fields, 14012d2a0(curve, 0x15) twice)
//   dtor     1401fb1a0 (called last by the record's dtor 1401f4950)
//   size     0x168: last write is the 8-byte field at 0x160 (param_1[0x58]); the record's own
//            fields start at 0x168 (1401f46e0).
#pragma once

#include <cstddef>
#include <cstdint>

#include <defmodel/layout/dynary.hpp>
#include <defmodel/layout/layout_check.hpp>

namespace def::model {

class Cls_1401fb020 {
public:
    // @src 1401fb020  ctor
    Cls_1401fb020();
    // @src 1401fb1a0  dtor
    ~Cls_1401fb020();

    std::int32_t u000;        // 0x000 =1
    std::uint8_t u004;        // 0x004 =0
    std::uint8_t u005;        // 0x005 =0
    std::int32_t u008;        // 0x008 =0
    std::int32_t u00c;        // 0x00c =0
    std::int32_t u010;        // 0x010 =0
    std::int32_t u014;        // 0x014 =0
    DYNARY u018;       // 0x018 curve type 0x15
    DYNARY u090;       // 0x090 curve type 0x15
    std::uint64_t u108[12];   // 0x108 =0 each
};

static_assert(sizeof(Cls_1401fb020) == 0x168);
DEF_LAYOUT_OFFSET(Cls_1401fb020, u004, 0x004);
DEF_LAYOUT_OFFSET(Cls_1401fb020, u008, 0x008);
DEF_LAYOUT_OFFSET(Cls_1401fb020, u014, 0x014);
DEF_LAYOUT_OFFSET(Cls_1401fb020, u018, 0x018);
DEF_LAYOUT_OFFSET(Cls_1401fb020, u090, 0x090);
DEF_LAYOUT_OFFSET(Cls_1401fb020, u108, 0x108);

} // namespace def::model
