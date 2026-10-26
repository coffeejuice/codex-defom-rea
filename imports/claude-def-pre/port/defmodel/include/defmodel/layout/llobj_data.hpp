// LLOBJ_DATA (RTTI .?AVLLOBJ_DATA@@, base IObjectData): one simulation object (keyword group 2),
// node of the DB_STEP_DATA object lists (Cls_14017a6c0 at step+0x1380 / +0x13a8).
// Recovered at member level only: embedded sub-objects without their own header are opaque
// byte slots named s<offset>, sized to the next known member (their real size can be smaller;
// the rest of the slot is then LLOBJ_DATA's own, unwritten by the ctor).
//
//   vtable   0x140522f00 (3 slots)
//   ctor     14015ec80(this, owner) (IObjectData ctor 140199a70, vptr, member ctors, 1401999d0,
//            140199a00, then 14015ef60(this, owner) when owner != 0)
//   dtor     14015f790 (non-virtual; 14015fbf0(this, 1), 140197ec0, member dtors reversed);
//            list nodes are deleted through 14019a290 (scalar deleting dtor, called directly)
//   size     0x3f28: the list's add function 14017a880 does operator new(0x3f28) before
//            14015ec80; last field written: bytes 0x3f20..0x3f22.
//   fixed    the first version put Cls_140197a50 and the six heap handles 0x300 too low (index
//            0x724*8 is 0x3920, not 0x3620); reported by packet p1-04 and re-checked in 14015ec80 /
//            14015fbf0. The 0x10 sub-object and name split also come from 14015fbf0.
//   open     roles of the opaque sub-objects; the gaps 0x3610..0x391f, 0x3980..0x398f,
//            0x39a8..0x39c7, 0x39f8..0x3a5f, 0x3aa0..0x3bf7; s3328/s3458 are _OBJ_GEO3D (owned by
//            packet p1-07, kept opaque here).
#pragma once

#include <cstddef>
#include <cstdint>

#include <defmodel/layout/dynary.hpp>
#include <defmodel/layout/dyn_ary_linked_list.hpp>
#include <defmodel/layout/iobject_data.hpp>
#include <defmodel/layout/layout_check.hpp>
#include <defmodel/layout/sdctl_ptr_list_obj_geo2d.hpp>
#include <defmodel/layout/state_var_base.hpp>

namespace def::model {

class DB_STEP_DATA;

class LLOBJ_DATA : public IObjectData {
public:
    // @src 14015ec80  ctor (owner may be 0)
    explicit LLOBJ_DATA(DB_STEP_DATA* owner);
    // @src 14015f790  dtor (non-virtual)
    ~LLOBJ_DATA();

    // @src 14015fa60  slot 0: strcpy(out, name)
    void get_name(char* out) const override;
    // @src 14015faa0  slot 1: strcpy(name, in)
    void set_name(const char* in) override;
    // @src 14015fbd0  slot 2: return u3c00[0]
    void* vslot2() override;

    // Methods: defmodel/src/llobj/llobj_data.cpp
    // @src 14015fbf0  clear the object; *full != 0 also deletes the u3c00 geometry objects
    void reset(const char* full);
    // @src 140162070  free u39a0
    void free_u39a0();
    // @src 140197ec0  delete the object at u3998 (14019a670(p, 1))
    void delete_u3998();
    // @src 140199b10  free u39c8, then clear_u39c8
    void free_u39c8();
    // @src 1401999d0  u39c8 = 0, u39d0 = 0
    void clear_u39c8();
    // @src 140199b60  free u39d8, zero u39d4/u39e0..u39ec, u39f0 = 1
    void reset_u39d8();
    // @src 140158f50  owner (+0x3a98)
    DB_STEP_DATA* get_owner() const;

    // vptr at 0x000 (IObjectData)
    std::byte pad_008[8];             // 0x008 not written by the ctor
    // 0x010..0x823: sub-object built by 140199970 (writes +0, +4, +0x808..+0x810); kept flat
    std::int32_t u010;                // 0x010 140199970 =0; 14015fbf0 =0
    std::int32_t u014;                // 0x014 =0
    char u018[0x400];                 // 0x018 14015fbf0 strcpy DAT_14051f7d6
    char u418[0x400];                 // 0x418 14015fbf0 strcpy DAT_14051f7d7
    std::int32_t u818;                // 0x818 =0
    std::int32_t u81c;                // 0x81c =0
    std::int32_t u820;                // 0x820 =0
    std::byte pad_824[4];             // 0x824
    char name[0x54];                  // 0x828 object name (slots 0/1); ends where 0x87c starts
    std::int32_t u87c[5];             // 0x87c 14015fbf0 =0 each (0x87c..0x88c)
    std::byte pad_890[0x10];          // 0x890
    std::byte s8a0[0x140];            // 0x8a0 Cls_14017ef20 (writes +0x130, +0x138; owner 14017efc0)
    std::byte s9e0[0x108];            // 0x9e0 Cls_140182880 (writes +0xf0, +0xf8; owner 140182920)
    std::byte sae8[0x70];             // 0xae8 Cls_140185570 (writes +0x60, +0x68; owner 1401855d0)
    std::byte sb58[0x18];             // 0xb58 Cls_140185c60 (140199a90 owner, 140199ab0 this)
    StateVarBase ub70;                // 0xb70 StateVarBase(-1, 0)
    DynAryLinkedList uba8;            // 0xba8 dynamic arrays
    std::byte pad_bc8[0x18];          // 0xbc8 not written
    std::byte sbe0[100][0x30];        // 0xbe0 100 x Cls_14017e740 (eh vector ctor, dtor 14017e720)
    std::byte s1ea0[0x2c0];           // 0x1ea0 Cls_1401860a0 (3 curves, writes +0x2b8; owner 1401861c0)
    std::byte s2160[0xa88];           // 0x2160 Cls_14018b220 (16 curves, writes +0xa80; owner 14018b370)
    std::byte s2be8[0x128];           // 0x2be8 Cls_1401889a0 (writes +0x120; owner 1401890e0)
    std::byte s2d10[0x158];           // 0x2d10 Cls_140189440 (3 Cls_140269250; owner 1401894d0)
    std::byte s2e68[0x4c0];           // 0x2e68 Cls_1401956d0 (writes up to +0x198; owner 1401957b0)
    std::byte s3328[0x130];           // 0x3328 _OBJ_GEO3D (ctor 1402f9020, vtable 0x140548d18)
    std::byte s3458[0x140];           // 0x3458 _OBJ_GEO3D (same class; slot 0x10 larger)
    DYNARY u3598;              // 0x3598 curve
    std::byte pad_3610[0x310];        // 0x3610 not written by ctor/dtor
    std::byte s3920[0x20];            // 0x3920 Cls_140197a50 (-> 140197b00; 14015fbf0 calls 140197b40)
    LLOBJ_DATA* next;                 // 0x3940 list link (14017a880, 14017ab70)
    std::byte pad_3948[8];            // 0x3948
    void* u3950;                      // 0x3950 heap handle (14019a8a0: new 0x20, 14019b090)
    void* u3958;                      // 0x3958 heap handle (14019a8a0)
    void* u3960;                      // 0x3960 heap handle (14019a8a0)
    void* u3968;                      // 0x3968 heap handle (140159ad0: new 0x20, 14015a380)
    void* u3970;                      // 0x3970 heap handle (14019a960: new 0x20, 14019b190)
    void* u3978;                      // 0x3978 heap handle (14019aa20: new 0x20, 14019b2c0)
    std::byte pad_3980[0x10];         // 0x3980
    std::uint8_t u3990;               // 0x3990 14015fbf0 =0
    std::byte pad_3991[7];            // 0x3991
    void* u3998;                      // 0x3998 ctor =0; 14015fbf0: if set, 140301d10/140301ad0 on it
    std::uint64_t u39a0;              // 0x39a0 ctor =0
    std::byte pad_39a8[0x20];         // 0x39a8
    std::uint64_t u39c8;              // 0x39c8 1401999d0 =0
    std::int32_t u39d0;               // 0x39d0 1401999d0 =0
    std::int32_t u39d4;               // 0x39d4 140199a00 =0
    std::uint64_t u39d8;              // 0x39d8 140199a00 =0
    std::int32_t u39e0;               // 0x39e0 =0
    std::int32_t u39e4;               // 0x39e4 =0
    std::int32_t u39e8;               // 0x39e8 =0
    std::int32_t u39ec;               // 0x39ec =0
    std::int32_t u39f0;               // 0x39f0 =1
    std::byte pad_39f4[0x6c];         // 0x39f4
    SdctlPtrList_OBJ_GEO2D u3a60;     // 0x3a60 2D geometry list
    DB_STEP_DATA* owner;              // 0x3a98 set by 14015ef60; read as *(this+15000)
    std::byte pad_3aa0[0x158];        // 0x3aa0
    std::uint64_t u3bf8;              // 0x3bf8 ctor =0
    void* u3c00[100];                 // 0x3c00 ctor loop =0; slot 2 returns u3c00[0]
    std::uint8_t u3f20;               // 0x3f20 ctor =1
    std::uint8_t u3f21;               // 0x3f21 =0
    std::uint8_t u3f22;               // 0x3f22 =0
    std::byte pad_3f23[5];            // 0x3f23
};

static_assert(sizeof(LLOBJ_DATA) == 0x3f28);
DEF_LAYOUT_OFFSET(LLOBJ_DATA, u010, 0x010);
DEF_LAYOUT_OFFSET(LLOBJ_DATA, u018, 0x018);
DEF_LAYOUT_OFFSET(LLOBJ_DATA, u418, 0x418);
DEF_LAYOUT_OFFSET(LLOBJ_DATA, u818, 0x818);
DEF_LAYOUT_OFFSET(LLOBJ_DATA, u87c, 0x87c);
DEF_LAYOUT_OFFSET(LLOBJ_DATA, name, 0x828);
DEF_LAYOUT_OFFSET(LLOBJ_DATA, s8a0, 0x8a0);
DEF_LAYOUT_OFFSET(LLOBJ_DATA, s9e0, 0x9e0);
DEF_LAYOUT_OFFSET(LLOBJ_DATA, sae8, 0xae8);
DEF_LAYOUT_OFFSET(LLOBJ_DATA, sb58, 0xb58);
DEF_LAYOUT_OFFSET(LLOBJ_DATA, ub70, 0xb70);
DEF_LAYOUT_OFFSET(LLOBJ_DATA, uba8, 0xba8);
DEF_LAYOUT_OFFSET(LLOBJ_DATA, sbe0, 0xbe0);
DEF_LAYOUT_OFFSET(LLOBJ_DATA, s1ea0, 0x1ea0);
DEF_LAYOUT_OFFSET(LLOBJ_DATA, s2160, 0x2160);
DEF_LAYOUT_OFFSET(LLOBJ_DATA, s2be8, 0x2be8);
DEF_LAYOUT_OFFSET(LLOBJ_DATA, s2d10, 0x2d10);
DEF_LAYOUT_OFFSET(LLOBJ_DATA, s2e68, 0x2e68);
DEF_LAYOUT_OFFSET(LLOBJ_DATA, s3328, 0x3328);
DEF_LAYOUT_OFFSET(LLOBJ_DATA, s3458, 0x3458);
DEF_LAYOUT_OFFSET(LLOBJ_DATA, u3598, 0x3598);
DEF_LAYOUT_OFFSET(LLOBJ_DATA, s3920, 0x3920);
DEF_LAYOUT_OFFSET(LLOBJ_DATA, u3950, 0x3950);
DEF_LAYOUT_OFFSET(LLOBJ_DATA, u3978, 0x3978);
DEF_LAYOUT_OFFSET(LLOBJ_DATA, u3990, 0x3990);
DEF_LAYOUT_OFFSET(LLOBJ_DATA, next, 0x3940);
DEF_LAYOUT_OFFSET(LLOBJ_DATA, u3998, 0x3998);
DEF_LAYOUT_OFFSET(LLOBJ_DATA, u39c8, 0x39c8);
DEF_LAYOUT_OFFSET(LLOBJ_DATA, u39d4, 0x39d4);
DEF_LAYOUT_OFFSET(LLOBJ_DATA, u39d8, 0x39d8);
DEF_LAYOUT_OFFSET(LLOBJ_DATA, u39f0, 0x39f0);
DEF_LAYOUT_OFFSET(LLOBJ_DATA, u3a60, 0x3a60);
DEF_LAYOUT_OFFSET(LLOBJ_DATA, owner, 0x3a98);
DEF_LAYOUT_OFFSET(LLOBJ_DATA, u3bf8, 0x3bf8);
DEF_LAYOUT_OFFSET(LLOBJ_DATA, u3c00, 0x3c00);
DEF_LAYOUT_OFFSET(LLOBJ_DATA, u3f20, 0x3f20);

} // namespace def::model
