// Cls_14028b850 (no RTTI, not polymorphic): grain/recrystallization block of a Material
// (Material+0x1078), data of keyword GRNDAT (docs/re/keywords/GRNDAT.md, "Model mapping").
// Holds three parts with their own ctors, defined here as nested classes:
//   Cls_14028a140 (+0x000, 0x458)  type-1 data: 4 flags, 9 DYNARYs, g2, g1
//   Cls_14028b640 (+0x458, 0x270)  type-2 data: c, main record, 5 record lists
//   +0x6c8 DYNARY built by 14028a3e0 (type 0xac), +0x740 double[6] built by 14028a440
//
//   ctor     14028b850 (14028a140, 14028b640(+0x458), 14028a3e0(+0x6c8), 14028a440(+0x740),
//            type = 0, ref = 0)
//   dtor     14028b8e0 (14028a4b0(+0x740) empty, 14028a420(+0x6c8) = DYNARY dtor,
//            14028b6f0(+0x458), 14028a320(+0))
//   size     0x778: last write is the int at 0x774; Material's next member is at
//            0x17f0 = 0x1078 + 0x778.
#pragma once

#include <cstddef>
#include <cstdint>

#include <defmodel/layout/cls_14028a4c0.hpp>
#include <defmodel/layout/dynary.hpp>
#include <defmodel/layout/layout_check.hpp>

namespace def::model {

// GRNDAT type 1 part (Material+0x1078).
class Cls_14028a140 {
public:
    // @src 14028a140  ctor (flags = 0, DYNARY types 0x55, 0x54, 0x56..0x5c, g = 0)
    Cls_14028a140();
    // @src 14028a320  dtor
    ~Cls_14028a140();

    std::int32_t flag_d3;        // 0x000 (mat+0x1078) spec: DRX on (inferred)
    std::int32_t flag_d5;        // 0x004 (mat+0x107c) spec: MDRX on (inferred)
    std::int32_t flag_d7;        // 0x008 (mat+0x1080) spec: SRX on (inferred)
    std::int32_t flag_d9;        // 0x00c (mat+0x1084) spec: grain growth on (inferred)
    DYNARY d[9];                 // 0x010 D1..D9, types 0x55,0x54,0x56,0x57,0x58,0x59,0x5a,0x5b,0x5c
    double g2;                   // 0x448 (mat+0x14c0) =0
    double g1;                   // 0x450 (mat+0x14c8) =0
};

static_assert(sizeof(Cls_14028a140) == 0x458);
DEF_LAYOUT_OFFSET(Cls_14028a140, d, 0x010);
DEF_LAYOUT_OFFSET(Cls_14028a140, g2, 0x448);
DEF_LAYOUT_OFFSET(Cls_14028a140, g1, 0x450);

// GRNDAT type 2 part (Material+0x14d0).
class Cls_14028b640 {
public:
    // @src 14028b640  ctor (record ctor, 5 list ctors (FID-labelled
    //                 COleControlSiteOrWnd::COleControlSiteOrWnd), c = 0, record.is_main = 1)
    Cls_14028b640();
    // @src 14028b6f0  dtor (14028af40 on each list, 14028a530 on the record)
    ~Cls_14028b640();

    struct RecordList {          // cleared by 14028af40
        Cls_14028a4c0* head;
        Cls_14028a4c0* tail;
        std::int32_t count;
        std::byte pad[4];
    };

    std::int32_t c;              // 0x000 (mat+0x14d0) GRNDAT 3rd int, type 2
    std::byte pad_004[4];        // 0x004
    Cls_14028a4c0 main;          // 0x008 (mat+0x14d8) main record
    RecordList l3;               // 0x1f8 (mat+0x16c8) spec: L3
    RecordList l2;               // 0x210 (mat+0x16e0) L2
    RecordList l1;               // 0x228 (mat+0x16f8) L1 (first in the file)
    RecordList l4;               // 0x240 (mat+0x1710) L4
    RecordList l5;               // 0x258 (mat+0x1728) L5
};

static_assert(sizeof(Cls_14028b640) == 0x270);
DEF_LAYOUT_OFFSET(Cls_14028b640, main, 0x008);
DEF_LAYOUT_OFFSET(Cls_14028b640, l3, 0x1f8);
DEF_LAYOUT_OFFSET(Cls_14028b640, l5, 0x258);

class Cls_14028b850 {
public:
    // @src 14028b850  ctor
    Cls_14028b850();
    // @src 14028b8e0  dtor
    ~Cls_14028b850();

    Cls_14028a140 type1;         // 0x000 (mat+0x1078)
    Cls_14028b640 type2;         // 0x458 (mat+0x14d0)
    DYNARY table4;               // 0x6c8 (mat+0x1740) 14028a3e0: type 0xac; GRNDAT type 4 table
    double p[6];                 // 0x740 (mat+0x17b8) 14028a440 zeroes; GRNDAT type 5 p1..p6
    std::int32_t type;           // 0x770 (mat+0x17e8) =0; GRNDAT type
    std::int32_t ref;            // 0x774 (mat+0x17ec) =0; -type for type < 0
};

static_assert(sizeof(Cls_14028b850) == 0x778);
DEF_LAYOUT_OFFSET(Cls_14028b850, type2, 0x458);
DEF_LAYOUT_OFFSET(Cls_14028b850, table4, 0x6c8);
DEF_LAYOUT_OFFSET(Cls_14028b850, p, 0x740);
DEF_LAYOUT_OFFSET(Cls_14028b850, type, 0x770);
DEF_LAYOUT_OFFSET(Cls_14028b850, ref, 0x774);

} // namespace def::model
