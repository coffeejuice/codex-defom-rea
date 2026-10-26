// DB_STEP_DATA (RTTI .?AV_DB_STEP_DATA@@): the keyword model of one step (simulation control,
// materials, objects, inter-object/inter-material data, DB address tables and buffers).
// Bases (RTTI class hierarchy, 3 entries): _DB_WRITE_VAR at 0 (primary, has the vptr) and
// _ACTION_DB_STEP at 0x140 (mdisp=320, not polymorphic).
//
//   vtable   0x14052ae10 (1 slot)
//   ctor     1401c8950(this, dim) -> 1401bae10, 1401d8380, members, owners, 1401c8c00(this, dim)
//   init     1401c8c00 (also run by slot 0 when the dimension changes)
//   free     1401cb440 (frees arrays and list contents; the dtor calls it when 0x173c == 0,
//            otherwise it only resets the lists without freeing their nodes)
//   dtor     1401cb2c0 (non-virtual: the class has no virtual dtor)
//   size     0x3408: main (FUN_14001c9f0) holds Cls_14001c770 whose DB_STEP_DATA members are
//            0x3408 apart from the following Cls_1401dbfe0 (0x190 -> 0x3598, 0x4eb0 -> 0x82b8);
//            last field written: int at 0x3400 (1401d8200). FUN_14001c500's stack copies are
//            0x3410 apart (stack alignment).
//   open     0x13a0..0x13a7, 0x1402..0x15f7, 0x2f8a..0x338f never written by ctor/init/dtor
//            (0x3008 is an 800-byte memcpy target in 140174e40 but that function's param may be
//            another class); ownership of 0x340..0x74f between ACTION_DB_STEP and this class.
#pragma once

#include <cstddef>
#include <cstdint>

#include <defmodel/layout/action_db_step.hpp>
#include <defmodel/layout/cls_14017a6c0.hpp>
#include <defmodel/layout/cls_1401e7b10.hpp>
#include <defmodel/layout/cls_1401fa020.hpp>
#include <defmodel/layout/material_list.hpp>
#include <defmodel/layout/cls_14032f100.hpp>
#include <defmodel/layout/phase_pair_list.hpp>
#include <defmodel/layout/db_write_var.hpp>
#include <defmodel/layout/layout_check.hpp>

namespace def::model {

class Cls_1401dbfe0;

class DB_STEP_DATA : public DB_WRITE_VAR, public ACTION_DB_STEP {
public:
    // @src 1401c8950  ctor; dim = 2 or 3 (callers pass 3)
    explicit DB_STEP_DATA(std::int32_t dim);
    // @src 1401cb2c0  dtor (non-virtual)
    ~DB_STEP_DATA();

    // @src 1401c8ba0  slot 0: if *dim != dimension: DB_WRITE_VAR::set_dimension, 1401cb440,
    //                 1401c8c00(this, *dim)
    void set_dimension(const std::int32_t* dim) override;

    // 0x000 DB_WRITE_VAR (0x140), 0x140 ACTION_DB_STEP (0x200)
    char u340[0x400];          // 0x340 ctor strcpy "DEFORM"; KEY command (140378e00) strncpy 0x400
    std::int32_t u740;         // 0x740 1401c8c00 =0; 1401cb440 =0; read by 140378e00
    std::int32_t u744;         // 0x744 =-1; step number passed to DBREAD 1401cc2a0 (1401c5810)
    std::int32_t u748;         // 0x748 =0; platform code for text byte swap (FUN_140324020, mapping)
    std::byte pad_74c[4];      // 0x74c
    Cls_1401e7b10 sim_ctrl;    // 0x750 simulation control (owner set by 1401e7c10)
    std::int32_t n_materials;  // 0x1320 number of materials (140384a50 stores the list count; read by 1401d09c0)
    std::byte pad_1324[4];     // 0x1324
    MaterialList materials;    // 0x1328 material list (owner by 1401d8300)
    PhasePairList phase_pairs; // 0x1358 phase-pair (inter-material) list (owner by 1401d8320)
    std::int32_t u1378;        // 0x1378 number of objects (mapping); zeroed by 1401cb440
    std::byte pad_137c[4];     // 0x137c
    Cls_14017a6c0 objects;     // 0x1380 object list (owner by 1401d8340)
    std::byte pad_13a0[8];     // 0x13a0 never written
    Cls_14017a6c0 objects2;    // 0x13a8 second object list (owner by 1401d8340)
    Cls_1401fa020 inter_obj;   // 0x13c8 inter-object list (owner by 1401d8360)
    std::int32_t u13e8;        // 0x13e8 NUMNP (mapping); multiplied by u3398 in GENDB 1401c4a30
    std::int32_t u13ec;        // 0x13ec NUMEL (mapping); multiplied by u33a8 in 1401c4be0
    std::int32_t u13f0;        // 0x13f0 boundary count (mapping)
    std::int32_t n_phase_on;   // 0x13f4 pairs with phastf_on == 1 (1401d6450, PHASTF.md); read by 1401d09c0
    std::int32_t u13f8;        // 0x13f8 number of windows (mapping)
    std::int32_t u13fc;        // 0x13fc NFN, functions in IIFNC (mapping)
    std::uint8_t u1400;        // 0x1400 =0
    std::uint8_t u1401;        // 0x1401 =0
    std::byte pad_1402[0x1f6]; // 0x1402 never written by ctor/init/dtor
    std::int32_t u15f8[0x4f];  // 0x15f8 state-variable layout table (1401d09c0 fills; 1401d0fc0 rebuilds)
    std::int32_t u1734;        // 0x1734 =0; from step header double [8] (1401cca80)
    std::int32_t u1738;        // 0x1738 =1; ==3 tested by main's INI loader 140009f20
    std::uint8_t u173c;        // 0x173c =0; nonzero = the dtor does not free list contents
    std::byte pad_173d[3];     // 0x173d
    std::int32_t u1740;        // 0x1740 ctor =0; set by KEY reader 1401b5f00
    std::int32_t u1744;        // 0x1744 ctor =0; set by KEY writer 1401c5810
    char u1748[0x400];         // 0x1748 ctor strcpy "DEFORM", then [0] = 0
    std::int32_t u1b48;        // 0x1b48 1401ca960 =0
    std::int32_t u1b4c;        // 0x1b4c =0
    std::uint64_t u1b50;       // 0x1b50 =0
    std::uint8_t u1b58;        // 0x1b58 ctor =1
    std::uint8_t u1b59;        // 0x1b59 ctor =1
    std::uint8_t u1b5a;        // 0x1b5a ctor =0
    std::uint8_t u1b5b;        // 0x1b5b ctor =0
    std::uint8_t u1b5c;        // 0x1b5c setter 140158fa0 (ctor passes 0)
    std::uint8_t u1b5d;        // 0x1b5d init =1
    std::byte pad_1b5e[2];     // 0x1b5e
    std::int32_t u1b60;        // 0x1b60 setter 1401cc120
    std::int32_t step_version; // 0x1b64 setter 1401cc160, getter 1401cc180
    std::int32_t u1b68;        // 0x1b68 MAR length (1401cd1f0)
    std::int32_t u1b6c;        // 0x1b6c A length (1401cd1f0)
    std::int32_t u1b70;        // 0x1b70 set by 1401cca80
    std::int32_t u1b74;        // 0x1b74 element count of a calloc(n, 8) array (1401cd1f0)
    void* u1b78;               // 0x1b78 A array (DBREAD), freed by 1401cb440
    double* u1b80;             // 0x1b80 step header (doubles), freed by 1401cb440
    void* u1b88;               // 0x1b88 MAR array (DBREAD), freed by 1401cb440
    std::int32_t u1b90[0x8c];  // 0x1b90 MAR address table: 70 starts + 70 widths (1401cb170)
    std::int32_t u1dc0[0x118]; // 0x1dc0 A address table: 140 starts + 140 widths (1401cb170)
    void* u2220;               // 0x2220 IIFNC region (DBREAD)
    void* u2228;               // 0x2228 LLFNC region
    void* u2230;               // 0x2230 LLWIN region
    void* u2238;               // 0x2238 LLWRZ region
    void* u2240;               // 0x2240 freed by 1401cb440
    void* u2248;               // 0x2248 freed by 1401cb440
    void* u2250;               // 0x2250 freed by 1401cb440
    void* u2258;               // 0x2258 freed by 1401cb440
    std::int32_t u2260[0x8c];  // 0x2260 1401cb1f0
    std::int32_t u2490;        // 0x2490 1401cb1f0
    std::int32_t u2494[0x8c];  // 0x2494 1401cb1f0
    std::int32_t u26c4[0x118]; // 0x26c4 1401cb1f0
    std::int32_t u2b24;        // 0x2b24 1401cb1f0
    std::int32_t u2b28[0x118]; // 0x2b28 1401cb1f0
    std::uint8_t u2f88;        // 0x2f88 1401d7f10 =0
    std::uint8_t u2f89;        // 0x2f89 1401d7ef0/1401d7f10 =0
    std::byte pad_2f8a[0x406]; // 0x2f8a never written by ctor/init/dtor
    Cls_1401dbfe0* db;         // 0x3390 companion DB object (14001c770, FUN_14001c500)
    std::int32_t u3398;        // 0x3398 number of strings in u33a0
    std::byte pad_339c[4];     // 0x339c
    char** u33a0;              // 0x33a0 UNNAME node names (mapping; 1401cb9e0 alloc, 1401cb910 free)
    std::int32_t u33a8;        // 0x33a8 number of strings in u33b0
    std::byte pad_33ac[4];     // 0x33ac
    char** u33b0;              // 0x33b0 UENAME element names (mapping; 1401cb730/1401cb7d0 alloc, 1401cb660 free)
    Cls_14032f100 u33b8;       // 0x33b8 CVNAME container (mapping: KEY-only); int + heap list
    std::int32_t u33c8;        // 0x33c8 init =1; nodal DAMAGE width (mapping)
    std::byte pad_33cc[0xc];   // 0x33cc
    std::int32_t u33d8;        // 0x33d8 init =1; element DAMAGE width (mapping)
    std::byte pad_33dc[0xc];   // 0x33dc
    DB_STEP_DATA* companion;   // 0x33e8 other-dimension model (DIM.md; 1401d8000 sets, 1401d8050 forces it to the other dim)
    std::uint8_t use_companion; // 0x33f0 DIM flag: 1401d81b0 returns companion when set; 1401d7fd0 =0
    std::byte pad_33f1[7];     // 0x33f1
    std::uint64_t u33f8;       // 0x33f8 1401d8200 =0
    std::int32_t u3400;        // 0x3400 1401d8200 =0
    std::byte pad_3404[4];     // 0x3404
};

static_assert(sizeof(DB_STEP_DATA) == 0x3408);
DEF_LAYOUT_OFFSET(DB_STEP_DATA, text, 0x140);
DEF_LAYOUT_OFFSET(DB_STEP_DATA, u340, 0x340);
DEF_LAYOUT_OFFSET(DB_STEP_DATA, u740, 0x740);
DEF_LAYOUT_OFFSET(DB_STEP_DATA, u744, 0x744);
DEF_LAYOUT_OFFSET(DB_STEP_DATA, sim_ctrl, 0x750);
DEF_LAYOUT_OFFSET(DB_STEP_DATA, n_materials, 0x1320);
DEF_LAYOUT_OFFSET(DB_STEP_DATA, materials, 0x1328);
DEF_LAYOUT_OFFSET(DB_STEP_DATA, phase_pairs, 0x1358);
DEF_LAYOUT_OFFSET(DB_STEP_DATA, u1378, 0x1378);
DEF_LAYOUT_OFFSET(DB_STEP_DATA, objects, 0x1380);
DEF_LAYOUT_OFFSET(DB_STEP_DATA, objects2, 0x13a8);
DEF_LAYOUT_OFFSET(DB_STEP_DATA, inter_obj, 0x13c8);
DEF_LAYOUT_OFFSET(DB_STEP_DATA, u13e8, 0x13e8);
DEF_LAYOUT_OFFSET(DB_STEP_DATA, u1400, 0x1400);
DEF_LAYOUT_OFFSET(DB_STEP_DATA, u15f8, 0x15f8);
DEF_LAYOUT_OFFSET(DB_STEP_DATA, u1734, 0x1734);
DEF_LAYOUT_OFFSET(DB_STEP_DATA, u173c, 0x173c);
DEF_LAYOUT_OFFSET(DB_STEP_DATA, u1740, 0x1740);
DEF_LAYOUT_OFFSET(DB_STEP_DATA, u1748, 0x1748);
DEF_LAYOUT_OFFSET(DB_STEP_DATA, u1b48, 0x1b48);
DEF_LAYOUT_OFFSET(DB_STEP_DATA, u1b58, 0x1b58);
DEF_LAYOUT_OFFSET(DB_STEP_DATA, u1b5d, 0x1b5d);
DEF_LAYOUT_OFFSET(DB_STEP_DATA, u1b60, 0x1b60);
DEF_LAYOUT_OFFSET(DB_STEP_DATA, step_version, 0x1b64);
DEF_LAYOUT_OFFSET(DB_STEP_DATA, u1b78, 0x1b78);
DEF_LAYOUT_OFFSET(DB_STEP_DATA, u1b88, 0x1b88);
DEF_LAYOUT_OFFSET(DB_STEP_DATA, u1b90, 0x1b90);
DEF_LAYOUT_OFFSET(DB_STEP_DATA, u1dc0, 0x1dc0);
DEF_LAYOUT_OFFSET(DB_STEP_DATA, u2220, 0x2220);
DEF_LAYOUT_OFFSET(DB_STEP_DATA, u2258, 0x2258);
DEF_LAYOUT_OFFSET(DB_STEP_DATA, u2260, 0x2260);
DEF_LAYOUT_OFFSET(DB_STEP_DATA, u2490, 0x2490);
DEF_LAYOUT_OFFSET(DB_STEP_DATA, u2494, 0x2494);
DEF_LAYOUT_OFFSET(DB_STEP_DATA, u26c4, 0x26c4);
DEF_LAYOUT_OFFSET(DB_STEP_DATA, u2b24, 0x2b24);
DEF_LAYOUT_OFFSET(DB_STEP_DATA, u2b28, 0x2b28);
DEF_LAYOUT_OFFSET(DB_STEP_DATA, u2f88, 0x2f88);
DEF_LAYOUT_OFFSET(DB_STEP_DATA, db, 0x3390);
DEF_LAYOUT_OFFSET(DB_STEP_DATA, u3398, 0x3398);
DEF_LAYOUT_OFFSET(DB_STEP_DATA, u33a0, 0x33a0);
DEF_LAYOUT_OFFSET(DB_STEP_DATA, u33b0, 0x33b0);
DEF_LAYOUT_OFFSET(DB_STEP_DATA, u33b8, 0x33b8);
DEF_LAYOUT_OFFSET(DB_STEP_DATA, u33c8, 0x33c8);
DEF_LAYOUT_OFFSET(DB_STEP_DATA, u33d8, 0x33d8);
DEF_LAYOUT_OFFSET(DB_STEP_DATA, companion, 0x33e8);
DEF_LAYOUT_OFFSET(DB_STEP_DATA, use_companion, 0x33f0);
DEF_LAYOUT_OFFSET(DB_STEP_DATA, u33f8, 0x33f8);
DEF_LAYOUT_OFFSET(DB_STEP_DATA, u3400, 0x3400);

} // namespace def::model
