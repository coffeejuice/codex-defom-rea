// Material (no RTTI, not polymorphic; ctor 14028c6f0): one material (keyword group 4), node of
// MaterialList at DB_STEP_DATA+0x1328. Members are named after the keyword that reads and
// writes them (docs/re/keywords/<KW>.md, _common_g457.md section 5). Every offset below was
// checked against the ctor/init/dtor code; the keyword names come from those specs.
// DBREAD 14028f300, GENDB 14029bdf0, KEY writer 1403cdca0 (dbmap brief); group-4 reader
// 140384a50 (spec).
//
//   alloc    14029c060: operator new(0x2218), 14028c6f0(node, list->owner)
//   ctor     14028c6f0(this, owner) (sub-object and DYNARY ctors, owner, next = 0, 14028cdc0)
//   init     14028cdc0 (number = 0, name "", defaults, DYNARY types, then 14028d030)
//   dtor     14028cb70 (free mstmtr_list, member dtors in reverse order); the list deletes
//            nodes through 14029e210 (scalar deleting dtor, called directly)
//   size     0x2218 (operator new size in 14029c060); the skip byte at 0x2210 is the last field.
//   open     0x004..0x00b and 0x1ed0 are not used by any spec; the DYNARY at 0x2058 has no
//            keyword in groups 4/5/7; 0x2211..0x2217 is padding.
#pragma once

#include <cstddef>
#include <cstdint>

#include <defmodel/layout/cls_140288ae0.hpp>
#include <defmodel/layout/cls_140289930.hpp>
#include <defmodel/layout/cls_140289b80.hpp>
#include <defmodel/layout/cls_140289fe0.hpp>
#include <defmodel/layout/cls_14028b850.hpp>
#include <defmodel/layout/cls_14028bcc0.hpp>
#include <defmodel/layout/cls_14028c040.hpp>
#include <defmodel/layout/cls_14028c500.hpp>
#include <defmodel/layout/dynary.hpp>
#include <defmodel/layout/layout_check.hpp>

namespace def::model {

class DB_STEP_DATA;

class Material {
public:
    // @src 14028c6f0  ctor
    explicit Material(DB_STEP_DATA* owner);
    // @src 14028cb70  dtor
    ~Material();

    std::int32_t number;          // 0x0000 material number (key of 14029c2c0); 14028cdc0 =0
    std::byte pad_0004[8];        // 0x0004 not written
    char name[0x41];              // 0x000c MTNAME (FUN_140413ab0(fp, +0xc, 0x41)); init strcpy ""
    std::byte pad_004d[3];        // 0x004d
    Cls_140288ae0 fstres;         // 0x0050 FSTRES block
    std::int32_t hdnrul;          // 0x0668 HDNRUL; init =0
    std::byte pad_066c[4];        // 0x066c
    DYNARY young;                 // 0x0670 YOUNG
    DYNARY poison;                // 0x06e8 POISON
    DYNARY expand;                // 0x0760 EXPAND
    double expand_ref;            // 0x07d8 EXPAND extra (reference temperature); init =0
    DYNARY thrcnd;                // 0x07e0 THRCND
    DYNARY heatcp;                // 0x0858 HEATCP
    std::int32_t heatcp_extra;    // 0x08d0 HEATCP extra, stored as (int)tmp; init =0
    std::byte pad_08d4[4];        // 0x08d4
    DYNARY emsvty;                // 0x08d8 EMSVTY
    Cls_140289930 difcoe;         // 0x0950 DIFCOE block
    Cls_140289b80 creep;          // 0x0ac8 CREEP block
    DYNARY hdnpha;                // 0x0f10 HDNPHA
    DYNARY hdntim;                // 0x0f88 HDNTIM; init type 0x53
    DYNARY jominy;                // 0x1000 JOMINY; init type 0x52
    Cls_14028b850 grndat;         // 0x1078 GRNDAT block
    DYNARY gbengy;                // 0x17f0 GBENGY; init type 0x5d
    double gbmobi_p;              // 0x1868 GBMOBI p; init =0
    double gbmobi_q;              // 0x1870 GBMOBI q; init =0
    DYNARY nucsiz;                // 0x1878 NUCSIZ; init type 0xb6
    Cls_14028bcc0 disloc;         // 0x18f0 BURGRS / ALPHA / NDISFM / RECVRY
    Cls_14028c040 frcmod;         // 0x1ad0 FRCMOD / UTSDAT block
    std::int32_t mstmtr_flag;     // 0x1cc8 MSTMTR: 1 = mixture material; init =0
    std::int32_t mstmtr_count;    // 0x1ccc MSTMTR n; init =0
    std::int32_t* mstmtr_list;    // 0x1cd0 MSTMTR entries (calloc(n, 4)); freed by the dtor
    DYNARY elrst;                 // 0x1cd8 ELRST
    DYNARY pmeab;                 // 0x1d50 PMEAB
    DYNARY pmitt;                 // 0x1dc8 PMITT
    Cls_140289fe0 aniso;          // 0x1e40 ANISO block
    double frae2h;                // 0x1ec8 FRAE2H; init DAT_14051f220 (0.9 per spec)
    std::uint64_t u1ed0;          // 0x1ed0 init =0; no keyword
    double tmpsol;                // 0x1ed8 TMPSOL; init =0
    double tmpliq;                // 0x1ee0 TMPLIQ; init =0
    double matden;                // 0x1ee8 MATDEN (3D only on write); init =0
    DYNARY fperv1;                // 0x1ef0 FPERV first table; init type 0
    DYNARY fperv2;                // 0x1f68 FPERV second table; init type 0
    DYNARY masden;                // 0x1fe0 MASDEN
    DYNARY u2058;                 // 0x2058 no keyword found
    std::int32_t coarse_flag;     // 0x20d0 COARSE flag; init =0
    std::byte pad_20d4[4];        // 0x20d4
    DYNARY coarse;                // 0x20d8 COARSE
    std::int32_t sizemd;          // 0x2150 SIZEMD; init =0
    Cls_14028c500 txture;         // 0x2154 TXTURE a, b, c
    DYNARY difbnd;                // 0x2160 DIFBND; init type 0xaf
    std::int32_t sinter_model;    // 0x21d8 SINTER model; init =0
    std::byte pad_21dc[4];        // 0x21dc
    double sinter_p[4];           // 0x21e0 SINTER p1..p4; init =0
    Material* next;               // 0x2200 list link
    DB_STEP_DATA* owner;          // 0x2208 model back-pointer (ctor param; read by 14028cdc0)
    std::uint8_t skip_key;        // 0x2210 1 = skip in KEY output (tested by 1403d4970); init =0
    std::byte pad_2211[7];        // 0x2211
};

static_assert(sizeof(Material) == 0x2218);
DEF_LAYOUT_OFFSET(Material, name, 0x000c);
DEF_LAYOUT_OFFSET(Material, fstres, 0x0050);
DEF_LAYOUT_OFFSET(Material, hdnrul, 0x0668);
DEF_LAYOUT_OFFSET(Material, young, 0x0670);
DEF_LAYOUT_OFFSET(Material, poison, 0x06e8);
DEF_LAYOUT_OFFSET(Material, expand, 0x0760);
DEF_LAYOUT_OFFSET(Material, expand_ref, 0x07d8);
DEF_LAYOUT_OFFSET(Material, thrcnd, 0x07e0);
DEF_LAYOUT_OFFSET(Material, heatcp, 0x0858);
DEF_LAYOUT_OFFSET(Material, heatcp_extra, 0x08d0);
DEF_LAYOUT_OFFSET(Material, emsvty, 0x08d8);
DEF_LAYOUT_OFFSET(Material, difcoe, 0x0950);
DEF_LAYOUT_OFFSET(Material, creep, 0x0ac8);
DEF_LAYOUT_OFFSET(Material, hdnpha, 0x0f10);
DEF_LAYOUT_OFFSET(Material, hdntim, 0x0f88);
DEF_LAYOUT_OFFSET(Material, jominy, 0x1000);
DEF_LAYOUT_OFFSET(Material, grndat, 0x1078);
DEF_LAYOUT_OFFSET(Material, gbengy, 0x17f0);
DEF_LAYOUT_OFFSET(Material, gbmobi_p, 0x1868);
DEF_LAYOUT_OFFSET(Material, nucsiz, 0x1878);
DEF_LAYOUT_OFFSET(Material, disloc, 0x18f0);
DEF_LAYOUT_OFFSET(Material, frcmod, 0x1ad0);
DEF_LAYOUT_OFFSET(Material, mstmtr_flag, 0x1cc8);
DEF_LAYOUT_OFFSET(Material, mstmtr_list, 0x1cd0);
DEF_LAYOUT_OFFSET(Material, elrst, 0x1cd8);
DEF_LAYOUT_OFFSET(Material, pmeab, 0x1d50);
DEF_LAYOUT_OFFSET(Material, pmitt, 0x1dc8);
DEF_LAYOUT_OFFSET(Material, aniso, 0x1e40);
DEF_LAYOUT_OFFSET(Material, frae2h, 0x1ec8);
DEF_LAYOUT_OFFSET(Material, tmpsol, 0x1ed8);
DEF_LAYOUT_OFFSET(Material, tmpliq, 0x1ee0);
DEF_LAYOUT_OFFSET(Material, matden, 0x1ee8);
DEF_LAYOUT_OFFSET(Material, fperv1, 0x1ef0);
DEF_LAYOUT_OFFSET(Material, fperv2, 0x1f68);
DEF_LAYOUT_OFFSET(Material, masden, 0x1fe0);
DEF_LAYOUT_OFFSET(Material, u2058, 0x2058);
DEF_LAYOUT_OFFSET(Material, coarse_flag, 0x20d0);
DEF_LAYOUT_OFFSET(Material, coarse, 0x20d8);
DEF_LAYOUT_OFFSET(Material, sizemd, 0x2150);
DEF_LAYOUT_OFFSET(Material, txture, 0x2154);
DEF_LAYOUT_OFFSET(Material, difbnd, 0x2160);
DEF_LAYOUT_OFFSET(Material, sinter_model, 0x21d8);
DEF_LAYOUT_OFFSET(Material, sinter_p, 0x21e0);
DEF_LAYOUT_OFFSET(Material, next, 0x2200);
DEF_LAYOUT_OFFSET(Material, owner, 0x2208);
DEF_LAYOUT_OFFSET(Material, skip_key, 0x2210);

} // namespace def::model
