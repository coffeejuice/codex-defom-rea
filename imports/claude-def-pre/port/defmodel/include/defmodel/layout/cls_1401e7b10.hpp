// Cls_1401e7b10 (no RTTI, not polymorphic): simulation control block S = DB_STEP_DATA+0x750
// (keyword group 1; DBREAD 1401eb870, GENDB 1401effc0, KEY writer 1403d4990, KEY reader 14039aa90).
// Members carry the keyword names of docs/re/dbmap/simctrl.md (branch claude/project-thread-
// bz5k5e), which lists S+x for every DB word; each offset there was matched against the writes in
// the ctor/init functions below (no disagreement found; fields that the mapping does not mention
// keep u<off> names).
//
//   ctor     1401e7b10 (constructs the DYNARYs and the furnace block, owner = 0)
//   init     1401e7c50 (-> 1401e7e90, 1401e8490, 1401e8650, 1401e8fb0, 1401e96b0, 1401ea6b0,
//            1401ead60); DB_STEP_DATA's ctor calls 1401e7c10(this, step) = set owner + init
//   clear    1401e7da0 (frees the DYNARYs' arrays, zeroes frqncy_rows/cols)
//   dtor     1401e7cb0 (1401e7da0, then DYNARY and furnace dtors in reverse order)
//   size     0xbd0: 1401e8fb0 writes 8 bytes at 0xbc8 (ODMAX), and DB_STEP_DATA's next field is at
//            0x1320 = 0x750 + 0xbd0.
//   open     8-byte fields only ever zeroed and not in the mapping are typed uint64_t; 0x4b0..0x4b7
//            is never written (BLZMAN at 0x4b8 is a sim-ctrl field per the mapping, so the
//            furnace block is at most 0x78 bytes); DTTYPE's mode (S+0x240) is DYNARY dttype.u58.
#pragma once

#include <cstddef>
#include <cstdint>

#include <defmodel/layout/cls_1403753f0.hpp>
#include <defmodel/layout/dynary.hpp>
#include <defmodel/layout/layout_check.hpp>

namespace def::model {

class DB_STEP_DATA;

class Cls_1401e7b10 {
public:
    // @src 1401e7b10  ctor
    Cls_1401e7b10();
    // @src 1401e7cb0  dtor
    ~Cls_1401e7b10();

    char title[0x51];         // 0x000 TITLE; 1401e7e90 strcpy "SIMULATION"
    char oprnam[0x41];        // 0x051 OPRNAM; "OPERATION 1"
    char simnam[0x51];        // 0x092 SIMNAM; "SIMULATION 1" (0xe3 is padding)
    std::int32_t cursim[3];   // 0x0e4 CURSIM v1..v3; init 1, 0, 0
    std::int32_t meshno;      // 0x0f0 MESHNO =1
    std::int32_t smode;       // 0x0f4 SMODE =1
    std::int32_t stype;       // 0x0f8 STYPE =1
    std::int32_t unit;        // 0x0fc UNIT (==2 selects the unit constants in 1401ea6b0)
    std::int32_t geotyp;      // 0x100 GEOTYP (2D) =1
    std::int32_t trans[5];    // 0x104 TRANS 1..5 (init loop zeroes 6 ints from 0x104; trans[1]=1)
    std::int32_t htmthd;      // 0x118 HTMTHD (6th int of that loop)
    std::int32_t stnout[7];   // 0x11c STNOUT 1..7 (stnout[4] at 0x12c = 1)
    std::int32_t elmnod[10];  // 0x138 ELMNOD 1..10, zeroed loop
    std::int32_t nstart;      // 0x160 NSTART; 1401e8490 =-1
    std::int32_t stpinc;      // 0x164 STPINC =10
    std::int32_t pdie1;       // 0x168 PDIE 1 =0
    std::int32_t nstep;       // 0x16c NSTEP =100 then =1
    double emax;              // 0x170 EMAX; 1401e8fb0 =0
    double tmax;              // 0x178 TMAX
    double smax[3];           // 0x180 SMAX 1..3 (3D)
    double vmin[3];           // 0x198 VMIN 1..3 (3D)
    double lmax[3];           // 0x1b0 LMAX 1..3 (3D)
    std::int32_t stpdef;      // 0x1c8 STPDEF; 1401e8650 =1
    std::byte pad_1cc[4];     // 0x1cc
    double dtmax1;            // 0x1d0 DTMAX 1
    double dtmax2;            // 0x1d8 DTMAX 2
    double dsmax;             // 0x1e0 DSMAX; setter 1401f4390
    DYNARY dttype;            // 0x1e8 DTTYPE function, type 0x80; mode at +0x58 (S+0x240)
    double demax;             // 0x260 DEMAX; _DAT_14051bef8 in 2D, else 0
    double slderr;            // 0x268 SLDERR
    double dtpmax1;           // 0x270 DTPMAX 1
    double dvmax[2];          // 0x278 DVMAX 1, 2
    std::int32_t mdsobj[3];   // 0x288 MDSOBJ 1..3 (int); 1401e8fb0
    std::byte pad_294[4];     // 0x294
    double mdsobj4;           // 0x298 MDSOBJ 4
    double dtpmax2_5[4];      // 0x2a0 DTPMAX 2..5 (4, 5 2D only)
    double dtsub;             // 0x2c0 DTSUB; _DAT_140509c90 in 3D, else 0
    double dplen;             // 0x2c8 DPLEN (3D); DAT_14051f240
    double cvgerr[2];         // 0x2d0 CVGERR 1, 2; 1401e96b0 2D/3D constants
    std::int32_t itrmth;      // 0x2e0 ITRMTH; 1 (2D) / 2 (3D)
    std::int32_t solmtd;      // 0x2e4 SOLMTD =1
    std::int32_t u2e8;        // 0x2e8 =0 (not in the mapping)
    std::byte pad_2ec[4];     // 0x2ec
    double solmtd_p[4];       // 0x2f0 SOLMTD parameters
    std::int32_t itrmxd;      // 0x310 ITRMXD =200
    std::int32_t defbwd;      // 0x314 DEFBWD =1
    std::int32_t solmtt;      // 0x318 SOLMTT; 1 (2D) / 2 (3D)
    std::int32_t itrmxt;      // 0x31c ITRMXT =200
    std::int32_t tmpbwd;      // 0x320 TMPBWD =1
    std::int32_t latent;      // 0x324 LATENT =0
    std::int32_t frqncy_rows; // 0x328 FRQNCY rows; 1401e9850 =6
    std::int32_t frqncy_cols; // 0x32c FRQNCY cols; 1401e9850 =1
    DYNARY frqncy;            // 0x330 FRQNCY table, type 0xb5, 6x1 values (1401e9850)
    double peninf;            // 0x3a8 PENINF; DAT_140522f50
    double unte2h;            // 0x3b0 UNTE2H; unit-dependent constant
    double tintgf;            // 0x3b8 TINTGF; DAT_14052e8d0
    std::uint8_t furnace_on;  // 0x3c0 ENVTMP furnace flag =0 (FRNNAM written only when set)
    std::byte pad_3c1[7];     // 0x3c1
    DYNARY envtmp;            // 0x3c8 ENVTMP (const at 0x3d0 set by 1401ea6b0)
    Cls_1403753f0 furnace;    // 0x440 FRNNAM furnace model (140375600(.., 0x16), 140375e90)
    std::byte pad_4b0[8];     // 0x4b0 never written
    double blzman;            // 0x4b8 BLZMAN; unit-dependent constant
    DYNARY cnvcof;            // 0x4c0 CNVCOF (const at 0x4c8)
    DYNARY envatm;            // 0x538 ENVATM (const at 0x540 = DAT_14052e8a0)
    DYNARY acvcof;            // 0x5b0 ACVCOF (const at 0x5b8)
    double envmpr;            // 0x628 ENVMPR; unit-dependent constant
    double envmpt;            // 0x630 ENVMPT
    double ehrate;            // 0x638 EHRATE =0
    double tnow1;             // 0x640 TNOW 1 (global time); 1401ead60 =0
    double tnow2;             // 0x648 TNOW 2
    std::int32_t tnow3;       // 0x650 TNOW 3 (int)
    std::byte pad_654[4];     // 0x654
    double tnow4;             // 0x658 TNOW 4
    double geoerr[4];         // 0x660 GEOERR 1..4 (DAT_14051f1b8, DAT_14051f250, 0, 0)
    double u680;              // 0x680 DAT_140524cd8 in 2D (left as is in 3D); not in the mapping
    char usrdef[10][0x51];    // 0x688 USRDEF 1..10, each strcpy DAT_14052c7da
    std::int32_t pdie2;       // 0x9b4 PDIE 2 (0x9b2..0x9b3 padding)
    std::int32_t osctrl[4];   // 0x9b8 OSCTRL 1..4 (2D); zeroed in 2D init
    std::int32_t cnterr1;     // 0x9c8 CNTERR 1 (int, 2D)
    std::byte pad_9cc[4];     // 0x9cc
    double cnterr2;           // 0x9d0 CNTERR 2 (2D)
    std::int32_t vfactr_mode; // 0x9d8 VFACTR mode
    std::int32_t vfactr_n;    // 0x9dc VFACTR count
    DYNARY vfactr;            // 0x9e0 VFACTR list, type 0xb5 (init in 3D only)
    DYNARY timeid;            // 0xa58 TIMEID, type 0xb0
    DYNARY erecid;            // 0xad0 ERECID, type 0xb1
    DYNARY nrecid;            // 0xb48 NRECID, type 0xb2
    DB_STEP_DATA* owner;      // 0xbc0 set by 1401e7c10; 2D/3D tests read it (1400015e0/1400015f0)
    double odmax;             // 0xbc8 ODMAX (3D); 1401e8fb0 =0
};

static_assert(sizeof(Cls_1401e7b10) == 0xbd0);
DEF_LAYOUT_OFFSET(Cls_1401e7b10, oprnam, 0x051);
DEF_LAYOUT_OFFSET(Cls_1401e7b10, simnam, 0x092);
DEF_LAYOUT_OFFSET(Cls_1401e7b10, cursim, 0x0e4);
DEF_LAYOUT_OFFSET(Cls_1401e7b10, meshno, 0x0f0);
DEF_LAYOUT_OFFSET(Cls_1401e7b10, unit, 0x0fc);
DEF_LAYOUT_OFFSET(Cls_1401e7b10, geotyp, 0x100);
DEF_LAYOUT_OFFSET(Cls_1401e7b10, trans, 0x104);
DEF_LAYOUT_OFFSET(Cls_1401e7b10, htmthd, 0x118);
DEF_LAYOUT_OFFSET(Cls_1401e7b10, stnout, 0x11c);
DEF_LAYOUT_OFFSET(Cls_1401e7b10, elmnod, 0x138);
DEF_LAYOUT_OFFSET(Cls_1401e7b10, nstart, 0x160);
DEF_LAYOUT_OFFSET(Cls_1401e7b10, pdie1, 0x168);
DEF_LAYOUT_OFFSET(Cls_1401e7b10, emax, 0x170);
DEF_LAYOUT_OFFSET(Cls_1401e7b10, smax, 0x180);
DEF_LAYOUT_OFFSET(Cls_1401e7b10, lmax, 0x1b0);
DEF_LAYOUT_OFFSET(Cls_1401e7b10, stpdef, 0x1c8);
DEF_LAYOUT_OFFSET(Cls_1401e7b10, dtmax1, 0x1d0);
DEF_LAYOUT_OFFSET(Cls_1401e7b10, dsmax, 0x1e0);
DEF_LAYOUT_OFFSET(Cls_1401e7b10, dttype, 0x1e8);
DEF_LAYOUT_OFFSET(Cls_1401e7b10, demax, 0x260);
DEF_LAYOUT_OFFSET(Cls_1401e7b10, slderr, 0x268);
DEF_LAYOUT_OFFSET(Cls_1401e7b10, dtpmax1, 0x270);
DEF_LAYOUT_OFFSET(Cls_1401e7b10, dvmax, 0x278);
DEF_LAYOUT_OFFSET(Cls_1401e7b10, mdsobj, 0x288);
DEF_LAYOUT_OFFSET(Cls_1401e7b10, mdsobj4, 0x298);
DEF_LAYOUT_OFFSET(Cls_1401e7b10, dtpmax2_5, 0x2a0);
DEF_LAYOUT_OFFSET(Cls_1401e7b10, dtsub, 0x2c0);
DEF_LAYOUT_OFFSET(Cls_1401e7b10, dplen, 0x2c8);
DEF_LAYOUT_OFFSET(Cls_1401e7b10, cvgerr, 0x2d0);
DEF_LAYOUT_OFFSET(Cls_1401e7b10, itrmth, 0x2e0);
DEF_LAYOUT_OFFSET(Cls_1401e7b10, solmtd, 0x2e4);
DEF_LAYOUT_OFFSET(Cls_1401e7b10, solmtd_p, 0x2f0);
DEF_LAYOUT_OFFSET(Cls_1401e7b10, itrmxd, 0x310);
DEF_LAYOUT_OFFSET(Cls_1401e7b10, latent, 0x324);
DEF_LAYOUT_OFFSET(Cls_1401e7b10, frqncy_rows, 0x328);
DEF_LAYOUT_OFFSET(Cls_1401e7b10, frqncy, 0x330);
DEF_LAYOUT_OFFSET(Cls_1401e7b10, peninf, 0x3a8);
DEF_LAYOUT_OFFSET(Cls_1401e7b10, furnace_on, 0x3c0);
DEF_LAYOUT_OFFSET(Cls_1401e7b10, envtmp, 0x3c8);
DEF_LAYOUT_OFFSET(Cls_1401e7b10, furnace, 0x440);
DEF_LAYOUT_OFFSET(Cls_1401e7b10, blzman, 0x4b8);
DEF_LAYOUT_OFFSET(Cls_1401e7b10, cnvcof, 0x4c0);
DEF_LAYOUT_OFFSET(Cls_1401e7b10, envatm, 0x538);
DEF_LAYOUT_OFFSET(Cls_1401e7b10, acvcof, 0x5b0);
DEF_LAYOUT_OFFSET(Cls_1401e7b10, envmpr, 0x628);
DEF_LAYOUT_OFFSET(Cls_1401e7b10, ehrate, 0x638);
DEF_LAYOUT_OFFSET(Cls_1401e7b10, tnow1, 0x640);
DEF_LAYOUT_OFFSET(Cls_1401e7b10, tnow3, 0x650);
DEF_LAYOUT_OFFSET(Cls_1401e7b10, tnow4, 0x658);
DEF_LAYOUT_OFFSET(Cls_1401e7b10, geoerr, 0x660);
DEF_LAYOUT_OFFSET(Cls_1401e7b10, u680, 0x680);
DEF_LAYOUT_OFFSET(Cls_1401e7b10, usrdef, 0x688);
DEF_LAYOUT_OFFSET(Cls_1401e7b10, pdie2, 0x9b4);
DEF_LAYOUT_OFFSET(Cls_1401e7b10, osctrl, 0x9b8);
DEF_LAYOUT_OFFSET(Cls_1401e7b10, cnterr1, 0x9c8);
DEF_LAYOUT_OFFSET(Cls_1401e7b10, cnterr2, 0x9d0);
DEF_LAYOUT_OFFSET(Cls_1401e7b10, vfactr_mode, 0x9d8);
DEF_LAYOUT_OFFSET(Cls_1401e7b10, vfactr, 0x9e0);
DEF_LAYOUT_OFFSET(Cls_1401e7b10, timeid, 0xa58);
DEF_LAYOUT_OFFSET(Cls_1401e7b10, erecid, 0xad0);
DEF_LAYOUT_OFFSET(Cls_1401e7b10, nrecid, 0xb48);
DEF_LAYOUT_OFFSET(Cls_1401e7b10, owner, 0xbc0);
DEF_LAYOUT_OFFSET(Cls_1401e7b10, odmax, 0xbc8);

} // namespace def::model
