// Cls_1401f46e0 (no RTTI, not polymorphic): one inter-object record (keyword group 3: contact
// between two objects), node of the DB_STEP_DATA inter-object list Cls_1401fa020 (step+0x13c8).
// DBREAD 1401f58a0, GENDB 1401fa4f0, KEY writer 1403c9ad0 (dbmap brief).
//
//   ctor     1401f46e0(this, owner) (Cls_1401fb020 at 0, Cls_140269250 at 0x188, 3 curves,
//            owner, next = 0, fields, curve types 0x17/0x16, 14026a120(+0x188, owner))
//   dtor     1401f4950 (Ghidra FID mislabels it ~CBitmapButton; curve dtors, 140269320 on 0x188,
//            1401fb1a0 on 0); deleted through 1401fb200 (scalar deleting dtor, called directly)
//   size     0x3a0: the list's add function 1401faaa0 does operator new(0x3a0); owner at 0x398
//            is the last field.
//   0x188    Cls_140269250, the list of LLWINDEF_DATA (contact window) nodes: 1402694a0 allocates
//            0x920-byte nodes with the LLWINDEF_DATA ctor 14025c100.
//   order    C++ member construction/destruction order is the original's: 1401fb020, 140269250,
//            the three curves (ctor); curves 0x2b8, 0x240, 0x1b8, 140269320, 1401fb1a0 (dtor).
#pragma once

#include <cstddef>
#include <cstdint>

#include <defmodel/layout/dynary.hpp>
#include <defmodel/layout/cls_1401fb020.hpp>
#include <defmodel/layout/cls_140269250.hpp>
#include <defmodel/layout/layout_check.hpp>

namespace def::model {

class DB_STEP_DATA;

class Cls_1401f46e0 {
public:
    // @src 1401f46e0  ctor
    explicit Cls_1401f46e0(DB_STEP_DATA* owner);
    // @src 1401f4950  dtor
    ~Cls_1401f46e0();

    Cls_1401fb020 u000;       // 0x000 leading sub-object
    std::int32_t u168;        // 0x168 =0
    std::int32_t u16c;        // 0x16c =0
    std::int32_t u170;        // 0x170 =0
    std::int32_t u174;        // 0x174 =1
    std::int32_t u178;        // 0x178 =0
    std::byte pad_17c[4];     // 0x17c
    std::uint64_t u180;       // 0x180 =0
    Cls_140269250 u188;       // 0x188 window list (owner set at the end of the ctor)
    DYNARY u1b8;       // 0x1b8 curve type 0x17
    std::int32_t u230;        // 0x230 =0
    std::int32_t u234;        // 0x234 =0
    std::uint64_t u238;       // 0x238 =0
    DYNARY u240;       // 0x240 curve type 0x16
    DYNARY u2b8;       // 0x2b8 curve
    std::int32_t u330;        // 0x330 =0
    std::int32_t u334;        // 0x334 =1
    std::uint64_t u338;       // 0x338 =0
    std::int32_t u340;        // 0x340 =0
    std::byte pad_344[4];     // 0x344
    std::uint64_t u348[6];    // 0x348 =0 each
    std::int32_t u378;        // 0x378 =0
    std::int32_t u37c;        // 0x37c =0
    std::int32_t u380;        // 0x380 =0
    std::int32_t u384;        // 0x384 =0
    std::uint64_t u388;       // 0x388 =0
    Cls_1401f46e0* next;      // 0x390 list link
    DB_STEP_DATA* owner;      // 0x398 ctor param
};

static_assert(sizeof(Cls_1401f46e0) == 0x3a0);
DEF_LAYOUT_OFFSET(Cls_1401f46e0, u168, 0x168);
DEF_LAYOUT_OFFSET(Cls_1401f46e0, u178, 0x178);
DEF_LAYOUT_OFFSET(Cls_1401f46e0, u180, 0x180);
DEF_LAYOUT_OFFSET(Cls_1401f46e0, u188, 0x188);
DEF_LAYOUT_OFFSET(Cls_1401f46e0, u1b8, 0x1b8);
DEF_LAYOUT_OFFSET(Cls_1401f46e0, u230, 0x230);
DEF_LAYOUT_OFFSET(Cls_1401f46e0, u238, 0x238);
DEF_LAYOUT_OFFSET(Cls_1401f46e0, u240, 0x240);
DEF_LAYOUT_OFFSET(Cls_1401f46e0, u2b8, 0x2b8);
DEF_LAYOUT_OFFSET(Cls_1401f46e0, u330, 0x330);
DEF_LAYOUT_OFFSET(Cls_1401f46e0, u338, 0x338);
DEF_LAYOUT_OFFSET(Cls_1401f46e0, u340, 0x340);
DEF_LAYOUT_OFFSET(Cls_1401f46e0, u348, 0x348);
DEF_LAYOUT_OFFSET(Cls_1401f46e0, u378, 0x378);
DEF_LAYOUT_OFFSET(Cls_1401f46e0, u388, 0x388);
DEF_LAYOUT_OFFSET(Cls_1401f46e0, next, 0x390);
DEF_LAYOUT_OFFSET(Cls_1401f46e0, owner, 0x398);

} // namespace def::model
