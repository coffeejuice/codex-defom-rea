// Cls_14028c040 (no RTTI, not polymorphic): fracture block of a Material (Material+0x1ad0), the
// data of keywords FRCMOD and UTSDAT (docs/re/keywords/FRCMOD.md, UTSDAT.md).
//
//   ctor     14028c040 (3 DYNARYs, zeroes, types 0x1b/0x1c/0xb4)
//   dtor     14029cfa0
//   size     0x1f8: last write is the 8-byte field at 0x1f0 (param_1[0x7c]); Material's next field
//            (MSTMTR flag) is at 0x1cc8 = 0x1ad0 + 0x1f8.
#pragma once

#include <cstddef>
#include <cstdint>

#include <defmodel/layout/dynary.hpp>
#include <defmodel/layout/layout_check.hpp>

namespace def::model {

class Cls_14028c040 {
public:
    // @src 14028c040  ctor
    Cls_14028c040();
    // @src 14029cfa0  dtor
    ~Cls_14028c040();

    std::int32_t model;          // 0x000 =0; FRCMOD model M (mat+0x1ad0)
    std::byte pad_004[4];        // 0x004
    double b_t2;                 // 0x008 =0; FRCMOD b for t 2 (mat+0x1ad8)
    double b_t4;                 // 0x010 =0; t 4 (mat+0x1ae0)
    double b_t5;                 // 0x018 =0; t 5, 15 (mat+0x1ae8)
    double b_t7;                 // 0x020 =0; t 7 b (mat+0x1af0)
    double c_t7;                 // 0x028 =0; t 7 c (mat+0x1af8)
    double a;                    // 0x030 =0; every type (mat+0x1b00)
    std::int32_t user_routine;   // 0x038 =0; FRCMOD -t (mat+0x1b08)
    std::byte pad_03c[4];        // 0x03c
    DYNARY uts;                  // 0x040 type 0x1b; UTSDAT (mat+0x1b10)
    double e;                    // 0x0b8 =0; FRCMOD e, every type (mat+0x1b88)
    DYNARY g;                    // 0x0c0 type 0x1c; FRCMOD t 12 table (mat+0x1b90)
    DYNARY k;                    // 0x138 type 0xb4; FRCMOD t 17 table (mat+0x1c08)
    double c_t11[9];             // 0x1b0 =0; FRCMOD t 11 (mat+0x1c80..0x1cc0; file order in spec)
};

static_assert(sizeof(Cls_14028c040) == 0x1f8);
DEF_LAYOUT_OFFSET(Cls_14028c040, b_t2, 0x008);
DEF_LAYOUT_OFFSET(Cls_14028c040, a, 0x030);
DEF_LAYOUT_OFFSET(Cls_14028c040, user_routine, 0x038);
DEF_LAYOUT_OFFSET(Cls_14028c040, uts, 0x040);
DEF_LAYOUT_OFFSET(Cls_14028c040, e, 0x0b8);
DEF_LAYOUT_OFFSET(Cls_14028c040, g, 0x0c0);
DEF_LAYOUT_OFFSET(Cls_14028c040, k, 0x138);
DEF_LAYOUT_OFFSET(Cls_14028c040, c_t11, 0x1b0);

} // namespace def::model
