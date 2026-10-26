// Cls_1403753f0 (no RTTI, not polymorphic): "Furnace model" block of the simulation control
// (Cls_1401e7b10+0x440): a name and a list of curves (140375600 appends 21+ curves of types
// 0x8e..0xa3 to the list).
//
//   ctor     1403753f0 (list ctor at 0x40, then 140375470: zero bytes 0x58..0x6f,
//            sprintf(this, "Furnace model"))
//   dtor     140375430 (1403754e0 -> 140375520 clears the list, then 140136510 list dtor)
//   size     0x70 (lower bound, taken as the size): 140375470 writes bytes up to 0x6f; the owner
//            writes an 8-byte value at its own offset 0x4b8 = 0x440 + 0x78, and its next member
//            (a curve) starts at 0x4c0 = 0x440 + 0x80.
//   defaults 140375e90: per-curve shape from the table at 0x140550ec0 (kFurnaceCurveShape),
//            constant values for curves 1, 4, 5, 6 and 8, then unit conversion to English when
//            the model is not SI (140235c10 != 2).
//   open     the real size is 0x70, 0x78 or 0x80: if it is larger, Cls_1401e7b10::u4b0/u4b8
//            belong to this class. Byte offsets are unaffected.
#pragma once

#include <cstddef>
#include <cstdint>

#include <defmodel/layout/dynary.hpp>
#include <defmodel/layout/cls_1401364a0.hpp>
#include <defmodel/layout/layout_check.hpp>

namespace def::model {

class Cls_1403753f0 {
public:
    // @src 1403753f0  ctor
    Cls_1403753f0();
    // @src 140375430  dtor
    ~Cls_1403753f0();
    Cls_1403753f0(const Cls_1403753f0&) = delete;
    Cls_1403753f0& operator=(const Cls_1403753f0&) = delete;

    // Methods: defmodel/src/members/furnace.cpp (p1-10).
    void init_name();                                  // 140375470
    void release_curves();                             // 1403754e0
    void clear_curves();                               // 140375520
    void setup_curves(const std::int32_t* n);          // 140375600 (21 fixed curves + extras)
    std::int32_t curve_count();                        // 140375950
    DYNARY* curve(const std::int32_t* i);       // 140375980 (1-based)
    void set_defaults();                               // 140375e90 (SI defaults, then units)
    void convert_units(std::int32_t mode);             // 1403769f0

    char name[0x40];          // 0x00 sprintf "Furnace model" (140375470)
    Cls_1401364a0 curves;     // 0x40 curve list (140375600, 140136670)
    std::uint8_t u58[0x18];   // 0x58 each byte zeroed by 140375470 (flags?)
};

static_assert(sizeof(Cls_1403753f0) == 0x70);
DEF_LAYOUT_OFFSET(Cls_1403753f0, name, 0x00);
DEF_LAYOUT_OFFSET(Cls_1403753f0, curves, 0x40);
DEF_LAYOUT_OFFSET(Cls_1403753f0, u58, 0x58);

} // namespace def::model
