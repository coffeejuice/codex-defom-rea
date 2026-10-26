// Cls_1401dbfe0 (no RTTI, not polymorphic): companion object of a DB_STEP_DATA, linked from
// DB_STEP_DATA+0x3390 (14001c770 stores &db at step+0x3390; FUN_14001c500 does the same for its
// stack pair). DBREAD gets it as its second argument: 1401cc2a0(step, *(step+0x3390), stepno, 0)
// (KEY writer 1401c5810). Other users: GENDB 1401bb550, 14013dba0, 1401d7680, 1401b9c00,
// 1401e4d80. It holds .DB file/step-directory state (two 0x400-char buffers, a 100-entry and a
// 400-entry table, two arrays deleted with vector delete).
//
//   ctor     1401dbfe0 (1401dc030, then 0x1908 = 0, 0x1910 = 0)
//   init     1401dc030
//   dtor     1401dc1c0 (1401dc220: delete[] 0xad8/0xae8 when 0xac8 != 0, free the 100 pointers at
//            0x330; then free 0x1908)
//   size     0x1918: in main (FUN_14001c9f0) the model object Cls_14001c770 packs
//            DB_STEP_DATA(0x3408) + this + DB_STEP_DATA + this with no gaps (offsets 0x190,
//            0x3598, 0x4eb0, 0x82b8, end 0x9bd0 = next stack local); last field int at 0x1910.
//            (FUN_14001c500 spaces its two stack instances 0x1920 apart: stack alignment.)
//   open     0x1770..0x18ff never written by ctor/init/dtor; role of the tables.
#pragma once

#include <cstddef>
#include <cstdint>

#include <defmodel/layout/layout_check.hpp>

namespace def::model {

class Cls_1401dbfe0 {
public:
    // @src 1401dbfe0  ctor
    Cls_1401dbfe0();
    // @src 1401dc1c0  dtor
    ~Cls_1401dbfe0();

    struct Pair {
        std::int32_t a;  // 1401dc030 [i*2+3] = 0
        std::int32_t b;  // 1401dc030 [i*2+4] = 0
    };

    std::int32_t u000;          // 0x000 =0
    std::int32_t u004;          // 0x004 =0
    std::int32_t u008;          // 0x008 =1
    Pair u00c[100];             // 0x00c loop i<100
    void* u330[100];            // 0x330 loop i<100 = 0; each freed by 1401dc220
    char u650[0x400];           // 0x650 [0] = 0
    char ua50[0x51];            // 0xa50 [0] = 0 (end inferred from the next written byte)
    char uaa1[8];               // 0xaa1 [0] = 0
    char uaa9[8];               // 0xaa9 [0] = 0
    char uab1[8];               // 0xab1 [0] = 0 (0xab9..0xabb padding)
    std::int32_t uabc;          // 0xabc
    std::int32_t uac0;          // 0xac0
    std::int32_t uac4;          // 0xac4
    std::int32_t uac8;          // 0xac8 nonzero = arrays at 0xad8/0xae8 allocated (1401dc220)
    std::int32_t uacc;          // 0xacc
    std::int32_t uad0;          // 0xad0
    std::byte pad_ad4[4];       // 0xad4
    void* uad8;                 // 0xad8 array, delete[] (vector deleting dtor, flag 3)
    std::int32_t uae0;          // 0xae0 =-1
    std::byte pad_ae4[4];       // 0xae4
    void* uae8;                 // 0xae8 array, delete[] (vector deleting dtor, flag 3)
    std::uint64_t uaf0[400];    // 0xaf0 loop i<400 = 0 (8-byte entries)
    std::byte pad_1770[0x190];  // 0x1770 not written by ctor/init/dtor
    std::uint8_t u1900;         // 0x1900 =0 (also reset by 1401dc220)
    std::byte pad_1901[7];      // 0x1901
    void* u1908;                // 0x1908 malloc'd, freed by 1401dc1c0
    std::int32_t u1910;         // 0x1910 =0
    std::byte pad_1914[4];      // 0x1914
};

static_assert(sizeof(Cls_1401dbfe0) == 0x1918);
DEF_LAYOUT_OFFSET(Cls_1401dbfe0, u008, 0x008);
DEF_LAYOUT_OFFSET(Cls_1401dbfe0, u00c, 0x00c);
DEF_LAYOUT_OFFSET(Cls_1401dbfe0, u330, 0x330);
DEF_LAYOUT_OFFSET(Cls_1401dbfe0, u650, 0x650);
DEF_LAYOUT_OFFSET(Cls_1401dbfe0, ua50, 0xa50);
DEF_LAYOUT_OFFSET(Cls_1401dbfe0, uaa1, 0xaa1);
DEF_LAYOUT_OFFSET(Cls_1401dbfe0, uab1, 0xab1);
DEF_LAYOUT_OFFSET(Cls_1401dbfe0, uabc, 0xabc);
DEF_LAYOUT_OFFSET(Cls_1401dbfe0, uac8, 0xac8);
DEF_LAYOUT_OFFSET(Cls_1401dbfe0, uad8, 0xad8);
DEF_LAYOUT_OFFSET(Cls_1401dbfe0, uae0, 0xae0);
DEF_LAYOUT_OFFSET(Cls_1401dbfe0, uae8, 0xae8);
DEF_LAYOUT_OFFSET(Cls_1401dbfe0, uaf0, 0xaf0);
DEF_LAYOUT_OFFSET(Cls_1401dbfe0, u1900, 0x1900);
DEF_LAYOUT_OFFSET(Cls_1401dbfe0, u1908, 0x1908);
DEF_LAYOUT_OFFSET(Cls_1401dbfe0, u1910, 0x1910);

} // namespace def::model
