// Cls_14001c770 (no RTTI, not polymorphic): main's interactive-session object (local
// local_a7e8 of main FUN_14001c9f0): menu state for TEXT_PRE plus two keyword models (step A and
// step B), each with its companion DB object. main calls 140009f20(this) (INI/DEF_MULTI.INI) and
// the menu loop 14001ad90(this).
//
//   ctor     14001c770 (menu defaults, 1401c8950(+0x190, 3), 1401dbfe0(+0x3598),
//            1401c8950(+0x4eb0, 3), 1401dbfe0(+0x82b8), links step->db, 1401d8000(A, B))
//   dtor     none: inlined in main (1401dc1c0(+0x82b8), 1401cb2c0(+0x4eb0), 1401dc1c0(+0x3598),
//            1401cb2c0(+0x190))
//   size     0x9bd0: main's frame puts the next local (local_c18) at 0xa7e8 - 0x9bd0; the last
//            member Cls_1401dbfe0 (0x1918) ends there (0x82b8 + 0x1918 = 0x9bd0).
//   open     0x000..0x18f is a run of small menu records {int on, int n_items, ptr, ptr} (0x18
//            bytes) that the menu loop 14001ad90 fills with model pointers; their classes and
//            the 8-byte fields that are only zeroed are not recovered.
#pragma once

#include <cstddef>
#include <cstdint>

#include <defmodel/layout/cls_1401dbfe0.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/layout_check.hpp>

namespace def::model {

class Cls_14001c770 {
public:
    // @src 14001c770  ctor
    Cls_14001c770();
    // no binary dtor: inlined in main FUN_14001c9f0
    ~Cls_14001c770();

    std::int32_t u000;          // 0x000 =0, then =1 at the end of the ctor
    std::int32_t u004;          // 0x004 =0, then =9
    std::int32_t u008;          // 0x008 =1   (record at 0x008 -> 14001a050(this+8))
    std::int32_t u00c;          // 0x00c =10
    std::byte pad_010[8];       // 0x010
    void* u018;                 // 0x018 = &step_a (14001ad90)
    std::int32_t u020;          // 0x020 =0
    std::byte pad_024[0xc];     // 0x024
    std::int32_t u030;          // 0x030 =1
    std::int32_t u034;          // 0x034 =5
    void* u038;                 // 0x038 = &step_a.sim_ctrl (this+0x8e0, 14001ad90)
    void* u040;                 // 0x040 = &step_a (14001ad90)
    std::int32_t u048;          // 0x048 =1
    std::int32_t u04c;          // 0x04c =5
    std::byte pad_050[0x10];    // 0x050
    std::int32_t u060;          // 0x060 =1
    std::int32_t u064;          // 0x064 =0x11
    std::byte pad_068[0x10];    // 0x068
    std::int32_t u078;          // 0x078 =1
    std::int32_t u07c;          // 0x07c =10
    std::byte pad_080[0x10];    // 0x080
    std::int32_t u090;          // 0x090 =1
    std::int32_t u094;          // 0x094 =5
    std::byte pad_098[0x10];    // 0x098
    std::int32_t u0a8;          // 0x0a8 =1   (record at 0x0a8 -> 1400157c0(this+0xa8))
    std::int32_t u0ac;          // 0x0ac =9
    std::byte pad_0b0[8];       // 0x0b0
    void* u0b8;                 // 0x0b8 = &step_a (14001ad90)
    std::int32_t u0c0;          // 0x0c0 =1   (record at 0x0c0 -> 140019c20(this+0xc0))
    std::int32_t u0c4;          // 0x0c4 =4
    std::byte pad_0c8[8];       // 0x0c8
    void* u0d0;                 // 0x0d0 = &step_a (14001ad90)
    std::uint64_t u0d8;         // 0x0d8 =0
    std::uint64_t u0e0;         // 0x0e0 =0
    std::uint64_t u0e8;         // 0x0e8 =0
    std::uint64_t u0f0;         // 0x0f0 =0
    std::uint64_t u0f8;         // 0x0f8 =0
    double u100;                // 0x100 =-1.0
    std::int32_t u108;          // 0x108 =1
    std::int32_t u10c;          // 0x10c =2
    double u110;                // 0x110 =1e-4 (0x3f1a36e2eb1c432d)
    std::uint64_t u118;         // 0x118 =0
    std::uint64_t u120;         // 0x120 =0
    std::uint64_t u128;         // 0x128 =0
    std::uint64_t u130;         // 0x130 =0
    std::uint64_t u138;         // 0x138 =0
    double u140;                // 0x140 =1.0
    std::uint64_t u148;         // 0x148 =0
    std::int32_t u150;          // 0x150 =1
    std::byte pad_154[4];       // 0x154
    double u158;                // 0x158 =1e-4
    std::int32_t u160;          // 0x160 =1
    std::int32_t u164;          // 0x164 =1000
    std::int32_t u168;          // 0x168 =0
    std::int32_t u16c;          // 0x16c =0
    std::int32_t u170;          // 0x170 =1
    std::int32_t u174;          // 0x174 =9
    void* u178;                 // 0x178 = &step_a (14001ad90)
    void* u180;                 // 0x180 = &db_a (this+0x3598, 14001ad90)
    std::int32_t u188;          // 0x188 =1   (record at 0x188 -> 14000afd0(this+0x188))
    std::int32_t u18c;          // 0x18c =10
    DB_STEP_DATA step_a;        // 0x190 1401c8950(.., 3); main passes it to the KEY/INI code
    Cls_1401dbfe0 db_a;         // 0x3598 step_a.db = &db_a
    DB_STEP_DATA step_b;        // 0x4eb0 1401c8950(.., 3); step_a.companion = &step_b (1401d8000)
    Cls_1401dbfe0 db_b;         // 0x82b8 step_b.db = &db_b
};

static_assert(sizeof(Cls_14001c770) == 0x9bd0);
DEF_LAYOUT_OFFSET(Cls_14001c770, u008, 0x008);
DEF_LAYOUT_OFFSET(Cls_14001c770, u018, 0x018);
DEF_LAYOUT_OFFSET(Cls_14001c770, u020, 0x020);
DEF_LAYOUT_OFFSET(Cls_14001c770, u030, 0x030);
DEF_LAYOUT_OFFSET(Cls_14001c770, u038, 0x038);
DEF_LAYOUT_OFFSET(Cls_14001c770, u048, 0x048);
DEF_LAYOUT_OFFSET(Cls_14001c770, u060, 0x060);
DEF_LAYOUT_OFFSET(Cls_14001c770, u078, 0x078);
DEF_LAYOUT_OFFSET(Cls_14001c770, u090, 0x090);
DEF_LAYOUT_OFFSET(Cls_14001c770, u0a8, 0x0a8);
DEF_LAYOUT_OFFSET(Cls_14001c770, u0b8, 0x0b8);
DEF_LAYOUT_OFFSET(Cls_14001c770, u0c0, 0x0c0);
DEF_LAYOUT_OFFSET(Cls_14001c770, u0d0, 0x0d0);
DEF_LAYOUT_OFFSET(Cls_14001c770, u100, 0x100);
DEF_LAYOUT_OFFSET(Cls_14001c770, u110, 0x110);
DEF_LAYOUT_OFFSET(Cls_14001c770, u140, 0x140);
DEF_LAYOUT_OFFSET(Cls_14001c770, u150, 0x150);
DEF_LAYOUT_OFFSET(Cls_14001c770, u158, 0x158);
DEF_LAYOUT_OFFSET(Cls_14001c770, u160, 0x160);
DEF_LAYOUT_OFFSET(Cls_14001c770, u178, 0x178);
DEF_LAYOUT_OFFSET(Cls_14001c770, u188, 0x188);
DEF_LAYOUT_OFFSET(Cls_14001c770, step_a, 0x190);
DEF_LAYOUT_OFFSET(Cls_14001c770, db_a, 0x3598);
DEF_LAYOUT_OFFSET(Cls_14001c770, step_b, 0x4eb0);
DEF_LAYOUT_OFFSET(Cls_14001c770, db_b, 0x82b8);

} // namespace def::model
