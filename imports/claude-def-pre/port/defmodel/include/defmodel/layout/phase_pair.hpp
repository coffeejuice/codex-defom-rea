// PhasePair (no RTTI, not polymorphic; ctor 140333f60): one inter-material (phase
// transformation) record, keyword group 5, keyed by (from, to) material numbers; node of
// PhasePairList at DB_STEP_DATA+0x1358. Members are named after the group-5 keywords that use
// them (docs/re/keywords/<KW>.md, _common_g457.md section 5); every offset was checked against
// the ctor/dtor. DBREAD 140336740, GENDB 14033a5a0, KEY writer 1403cbf00 (dbmap brief); group-5
// reader 140383120 (spec).
//
//   alloc    140339df0: operator new(0x780), 140333f60(node, list->owner)
//   ctor     140333f60(this, owner) (12 DYNARYs, owner, next = 0, defaults, DYNARY types)
//   dtor     140334490 (DYNARY dtors in reverse order); the list deletes nodes through
//            14033a990 (scalar deleting dtor, called directly)
//   size     0x780 (operator new size in 140339df0); owner at 0x778 is the last field.
//   note     the ctor gives tttd_t9 type 0x37 and tttd_t1 type 0x2c; TTTD.md lists the type-9
//            table as 0x2c: the reader re-types the DYNARY, so both can be true.
#pragma once

#include <cstddef>
#include <cstdint>

#include <defmodel/layout/dynary.hpp>
#include <defmodel/layout/layout_check.hpp>

namespace def::model {

class DB_STEP_DATA;

class PhasePair {
public:
    // @src 140333f60  ctor
    explicit PhasePair(DB_STEP_DATA* owner);
    // @src 140334490  dtor
    ~PhasePair();

    std::int32_t from;           // 0x000 =0; TTTN/PHASTF from-material (key, 14033a040)
    std::int32_t to;             // 0x004 =0; to-material (key)
    std::int32_t phastf;         // 0x008 =0; PHASTF raw value (2 enables PHEQVF output)
    std::int32_t phastf_on;      // 0x00c =0; PHASTF v != 0 (counted into DB_STEP_DATA+0x13f4)
    std::int32_t tttd_kind;      // 0x010 =0; TTTD line-1 kind
    std::int32_t tttd_type;      // 0x014 =0; TTTD internal kind
    char name[0x51];             // 0x018 TTTN name line (read, never written); ctor sprintf " "
    std::byte pad_069[3];        // 0x069
    std::int32_t tttd_nc;        // 0x06c =2; TTTD type 1/9 nc
    double tttd_f1;              // 0x070 =0; TTTD f1 when nc == 1
    DYNARY tttd_t1;              // 0x078 type 0x2c; TTTD type 1 table
    std::int32_t tttd_x;         // 0x0f0 =0; TTTD type 1 x
    std::byte pad_0f4[4];        // 0x0f4
    DYNARY tttd_t9;              // 0x0f8 type 0x37; TTTD type 9 table ("Phase Compositions")
    double tttd_f1b;             // 0x170 DAT_14051f258; TTTD f1/f2 (spec default 0.01)
    double tttd_f2;              // 0x178 DAT_1405501c8; TTTD f2 (spec default 0.99)
    DYNARY tttd_vec;             // 0x180 type 0xb3; TTTD types 10..13 vector
    DYNARY tttd_t2;              // 0x1f8 type 0x30; TTTD type 2 table
    double tttd_c3[11];          // 0x270 =0; TTTD type 3 coefficients
    DYNARY tttd_t4;              // 0x2c8 type 0x32; TTTD type 4 f(T)
    double tttd_c4[4];           // 0x340 =0; TTTD type 4
    double tttd_c5[6];           // 0x360 =0; TTTD type 5
    double tttd_c6[4];           // 0x390 =0; TTTD type 6
    double tttd_c7[7];           // 0x3b0 =0; TTTD type 7
    double tttd_f9[2];           // 0x3e8 =0; TTTD type 9 f1, f2 (nc != 1)
    DYNARY tttd_t8;              // 0x3f8 type 0x36; TTTD type 8 f(T)
    std::int32_t tttd_ref;       // 0x470 =0; TTTD negative type (abs value)
    std::byte pad_474[4];        // 0x474
    DYNARY phaslh;               // 0x478 PHASLH (re-typed 0x1f by the reader)
    DYNARY phasvl;               // 0x4f0 PHASVL (re-typed 0x20)
    DYNARY pheqvf;               // 0x568 PHEQVF table (re-typed 0x6c)
    std::int32_t pheqvf_t;       // 0x5e0 =0; PHEQVF t
    std::int32_t trnsfp_on;      // 0x5e4 =0; TRNSFP on
    DYNARY trnsfp;               // 0x5e8 TRNSFP table (type stays 0)
    std::int32_t coarsn_on;      // 0x660 =0; COARSN present
    std::byte pad_664[4];        // 0x664
    double coarsn_c1;            // 0x668 =0; COARSN c1
    double coarsn_c2;            // 0x670 =0; COARSN c2
    DYNARY coarsn;               // 0x678 type 0x6d; COARSN table
    DYNARY ipsnrg;               // 0x6f0 type 0x6e; IPSNRG
    std::int32_t varsel;         // 0x768 =0; VARSEL
    std::byte pad_76c[4];        // 0x76c
    PhasePair* next;             // 0x770 list link
    DB_STEP_DATA* owner;         // 0x778 ctor param
};

static_assert(sizeof(PhasePair) == 0x780);
DEF_LAYOUT_OFFSET(PhasePair, to, 0x004);
DEF_LAYOUT_OFFSET(PhasePair, phastf_on, 0x00c);
DEF_LAYOUT_OFFSET(PhasePair, tttd_type, 0x014);
DEF_LAYOUT_OFFSET(PhasePair, name, 0x018);
DEF_LAYOUT_OFFSET(PhasePair, tttd_nc, 0x06c);
DEF_LAYOUT_OFFSET(PhasePair, tttd_f1, 0x070);
DEF_LAYOUT_OFFSET(PhasePair, tttd_t1, 0x078);
DEF_LAYOUT_OFFSET(PhasePair, tttd_x, 0x0f0);
DEF_LAYOUT_OFFSET(PhasePair, tttd_t9, 0x0f8);
DEF_LAYOUT_OFFSET(PhasePair, tttd_f1b, 0x170);
DEF_LAYOUT_OFFSET(PhasePair, tttd_vec, 0x180);
DEF_LAYOUT_OFFSET(PhasePair, tttd_t2, 0x1f8);
DEF_LAYOUT_OFFSET(PhasePair, tttd_c3, 0x270);
DEF_LAYOUT_OFFSET(PhasePair, tttd_t4, 0x2c8);
DEF_LAYOUT_OFFSET(PhasePair, tttd_c4, 0x340);
DEF_LAYOUT_OFFSET(PhasePair, tttd_c5, 0x360);
DEF_LAYOUT_OFFSET(PhasePair, tttd_c6, 0x390);
DEF_LAYOUT_OFFSET(PhasePair, tttd_c7, 0x3b0);
DEF_LAYOUT_OFFSET(PhasePair, tttd_f9, 0x3e8);
DEF_LAYOUT_OFFSET(PhasePair, tttd_t8, 0x3f8);
DEF_LAYOUT_OFFSET(PhasePair, tttd_ref, 0x470);
DEF_LAYOUT_OFFSET(PhasePair, phaslh, 0x478);
DEF_LAYOUT_OFFSET(PhasePair, phasvl, 0x4f0);
DEF_LAYOUT_OFFSET(PhasePair, pheqvf, 0x568);
DEF_LAYOUT_OFFSET(PhasePair, pheqvf_t, 0x5e0);
DEF_LAYOUT_OFFSET(PhasePair, trnsfp_on, 0x5e4);
DEF_LAYOUT_OFFSET(PhasePair, trnsfp, 0x5e8);
DEF_LAYOUT_OFFSET(PhasePair, coarsn_on, 0x660);
DEF_LAYOUT_OFFSET(PhasePair, coarsn_c1, 0x668);
DEF_LAYOUT_OFFSET(PhasePair, coarsn, 0x678);
DEF_LAYOUT_OFFSET(PhasePair, ipsnrg, 0x6f0);
DEF_LAYOUT_OFFSET(PhasePair, varsel, 0x768);
DEF_LAYOUT_OFFSET(PhasePair, next, 0x770);
DEF_LAYOUT_OFFSET(PhasePair, owner, 0x778);

} // namespace def::model
