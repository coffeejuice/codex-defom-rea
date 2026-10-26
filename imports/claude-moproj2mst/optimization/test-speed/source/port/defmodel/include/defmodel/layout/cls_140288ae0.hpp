// Cls_140288ae0 (no RTTI, not polymorphic): flow-stress block of a Material (Material+0x50), the
// data of keyword FSTRES (docs/re/keywords/FSTRES.md; offsets there are absolute material
// offsets = this + 0x50).
//
//   ctor     140288ae0 (10 DYNARYs, model = 1, zeroes, DYNARY types)
//   dtor     14029cdc0 (DYNARY dtors in reverse order)
//   size     0x618: Material's next field (HDNRUL int) is at 0x668 = 0x50 + 0x618; the last write
//            is the int at 0x610 (param_1[0x184]).
//   open     meaning of the coefficient sets for FSTRES types 5, 9, 10, 11 (spec: "unknown").
#pragma once

#include <cstddef>
#include <cstdint>

#include <defmodel/layout/dynary.hpp>
#include <defmodel/layout/layout_check.hpp>

namespace def::model {

class Cls_140288ae0 {
public:
    // @src 140288ae0  ctor
    Cls_140288ae0();
    // @src 14029cdc0  dtor
    ~Cls_140288ae0();

    std::int32_t model;          // 0x000 =1; FSTRES model code M (KEY type -> M per spec)
    std::byte pad_004[4];        // 0x004
    double c_t1[4];              // 0x008 =0; FSTRES t1 coefficients (mat+0x58..0x70)
    DYNARY table;                // 0x028 type 0x4f; FSTRES t2/t3 table (mat+0x78), flag in u50
    double c_t4[5];              // 0x0a0 =0; FSTRES t4 (mat+0xf0..0x110)
    double c_t5[4];              // 0x0c8 =0; FSTRES t5 (mat+0x118..0x130)
    DYNARY h;                    // 0x0e8 type 0x25; FSTRES t6 "Coefficient (H)" (mat+0x138)
    DYNARY y;                    // 0x160 type 0x24; FSTRES t6 "Coefficient (Y)" (mat+0x1b0)
    DYNARY s;                    // 0x1d8 type 0x50; FSTRES t7/t8 "Strain-Atm-Temp" (mat+0x228)
    double c_t9[14];             // 0x250 =0; FSTRES t9 (mat+0x2a0..0x308)
    double c_t10[6];             // 0x2c0 =0; FSTRES t10 (mat+0x310..0x338)
    double c_t11[5];             // 0x2f0 =0; FSTRES t11 (mat+0x340..0x360)
    double c_t14[4];             // 0x318 =0; FSTRES t14 (mat+0x368..0x380)
    DYNARY bmd_n;                // 0x338 type 0xae; FSTRES t14 "Bird-Mukherjee-Dorn n" (mat+0x388)
    DYNARY f;                    // 0x3b0 type 0x26; FSTRES t12 "Initial Flow Stress" (mat+0x400)
    std::int32_t cnt;            // 0x428 =0; FSTRES t13 count (mat+0x478)
    std::byte pad_42c[4];        // 0x42c
    DYNARY a;                    // 0x430 type 0xa6; FSTRES t13 A (mat+0x480)
    DYNARY b;                    // 0x4a8 type 0xa7; FSTRES t13 B (mat+0x4f8)
    DYNARY c;                    // 0x520 type 0xa8; FSTRES t13 C (mat+0x570)
    DYNARY p;                    // 0x598 type 0x51; FSTRES t16 "Strain-StrainRate-Temp" (mat+0x5e8)
    std::int32_t user_routine;   // 0x610 =0; FSTRES t<0 routine number (mat+0x660)
    std::byte pad_614[4];        // 0x614
};

static_assert(sizeof(Cls_140288ae0) == 0x618);
DEF_LAYOUT_OFFSET(Cls_140288ae0, c_t1, 0x008);
DEF_LAYOUT_OFFSET(Cls_140288ae0, table, 0x028);
DEF_LAYOUT_OFFSET(Cls_140288ae0, c_t4, 0x0a0);
DEF_LAYOUT_OFFSET(Cls_140288ae0, c_t5, 0x0c8);
DEF_LAYOUT_OFFSET(Cls_140288ae0, h, 0x0e8);
DEF_LAYOUT_OFFSET(Cls_140288ae0, y, 0x160);
DEF_LAYOUT_OFFSET(Cls_140288ae0, s, 0x1d8);
DEF_LAYOUT_OFFSET(Cls_140288ae0, c_t9, 0x250);
DEF_LAYOUT_OFFSET(Cls_140288ae0, c_t10, 0x2c0);
DEF_LAYOUT_OFFSET(Cls_140288ae0, c_t11, 0x2f0);
DEF_LAYOUT_OFFSET(Cls_140288ae0, c_t14, 0x318);
DEF_LAYOUT_OFFSET(Cls_140288ae0, bmd_n, 0x338);
DEF_LAYOUT_OFFSET(Cls_140288ae0, f, 0x3b0);
DEF_LAYOUT_OFFSET(Cls_140288ae0, cnt, 0x428);
DEF_LAYOUT_OFFSET(Cls_140288ae0, a, 0x430);
DEF_LAYOUT_OFFSET(Cls_140288ae0, b, 0x4a8);
DEF_LAYOUT_OFFSET(Cls_140288ae0, c, 0x520);
DEF_LAYOUT_OFFSET(Cls_140288ae0, p, 0x598);
DEF_LAYOUT_OFFSET(Cls_140288ae0, user_routine, 0x610);

} // namespace def::model
