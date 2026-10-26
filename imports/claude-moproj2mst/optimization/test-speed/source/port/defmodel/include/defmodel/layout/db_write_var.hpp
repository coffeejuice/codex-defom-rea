// DB_WRITE_VAR (RTTI .?AV_DB_WRITE_VAR@@): base of DB_STEP_DATA, holds the DB header/version
// state and the GENDB output buffers.
//
//   vtable   0x140528290 (1 slot)            ctor  1401bae10 (inits via 1401baf30)
//   dtor     1401baf10 (non-virtual: only restores the vptr; the class has no virtual dtor)
//   size     0x140: DB_STEP_DATA's second base ACTION_DB_STEP sits at 0x140 (RTTI base class
//            descriptor of _DB_STEP_DATA: mdisp=320), and 1401baf30 writes up to 0x13c.
//   open     8-byte fields at 0xe8/0xf0/0xf8 are only zeroed (pointer or double unknown);
//            0x4c and 0x0a..0x0b are padding by alignment (never written).
#pragma once

#include <cstddef>
#include <cstdint>

#include <defmodel/layout/layout_check.hpp>

namespace def::model {

class DB_WRITE_VAR {
public:
    // @src 1401bae10  ctor (sets vptr, small fields, then calls 1401baf30)
    DB_WRITE_VAR();
    // @src 1401baf10  dtor (non-virtual)
    ~DB_WRITE_VAR();

    // @src 1401c5700  slot 0: stores *dim at 0x1c (getter 140158e90 reads it back)
    virtual void set_dimension(const std::int32_t* dim);

    // vptr at 0x000
    std::uint8_t u008;        // 0x008 1401bae10 =1; setter 1401c5720; GENDB direct-write flag (mapping: 0 = write nothing)
    std::uint8_t u009;        // 0x009 1401bae10 =1
    std::int32_t version;     // 0x00c 1401baf30 =0x44c (1100)
    char date[0xc];           // 0x010 1401baf30 strcpy "06/30/14"
    std::int32_t dimension;   // 0x01c 1401bae10 =3; set_dimension, getter 140158e90
    std::int32_t u020;        // 0x020 1401bae10 =0
    std::int32_t u024;        // 0x024
    std::int32_t u028;        // 0x028
    std::int32_t u02c;        // 0x02c
    std::int32_t u030;        // 0x030 MAR buffer length (GENDB, per DBREAD/GENDB brief)
    std::int32_t u034;        // 0x034 A buffer length (GENDB)
    std::int32_t u038;        // 0x038
    std::int32_t u03c;        // 0x03c
    std::int32_t u040;        // 0x040 1401bae10 =0
    std::int32_t u044;        // 0x044
    std::int32_t u048;        // 0x048
    void* u050;               // 0x050 GENDB MAR buffer, freed by 1401bb320
    void* u058;               // 0x058 GENDB A buffer, freed by 1401bb320
    void* u060;               // 0x060 GENDB step header buffer, freed by 1401bb320
    std::int32_t u068;        // 0x068 1401baf30 =0
    std::int32_t u06c;        // 0x06c setter 1401c5740; NUMNP for GENDB (mapping, IISIM[19])
    std::int32_t u070;        // 0x070 setter 1401c5760; NUMEL for GENDB (mapping)
    std::int32_t u074[21];    // 0x074..0x0c4 1401baf30 zeroes each; GENDB counts (mapping: 0x78 materials,
                              // 0x7c objects, 0x80 boundaries, 0x88 NFN, 0x8c LLFNC length, 0xbc relations, 0xc0 windows)
    std::int32_t u0c8;        // 0x0c8 setter 1401c5780
    std::int32_t u0cc;        // 0x0cc setter 1401c57a0
    std::int32_t u0d0;        // 0x0d0 setter 14019a310
    std::int32_t u0d4;        // 0x0d4 setter 14019a330
    std::int32_t u0d8;        // 0x0d8
    std::int32_t u0dc;        // 0x0dc
    std::int32_t u0e0;        // 0x0e0 1401baf30 =0x10
    std::int32_t u0e4;        // 0x0e4
    std::uint64_t u0e8;       // 0x0e8 zeroed (8 bytes)
    std::uint64_t u0f0;       // 0x0f0 zeroed (8 bytes)
    std::uint64_t u0f8;       // 0x0f8 zeroed (8 bytes)
    std::int32_t u100;        // 0x100
    std::int32_t u104;        // 0x104
    std::int32_t u108;        // 0x108
    std::int32_t u10c;        // 0x10c GENDB counter (mapping)
    std::int32_t u110;        // 0x110 setter 14019a2d0
    std::int32_t u114;        // 0x114 setter 140199d00
    std::int32_t u118;        // 0x118 setter 140199d40
    std::int32_t u11c;        // 0x11c setter 14019a0e0
    std::int32_t u120;        // 0x120
    std::int32_t u124;        // 0x124 setter 14019a120
    std::int32_t u128;        // 0x128 setter 14019a160
    std::int32_t u12c;        // 0x12c setter 14019a1a0
    std::int32_t u130;        // 0x130 setter 14019a1e0
    std::int32_t u134;        // 0x134 GENDB window-row cursor (mapping)
    std::int32_t u138;        // 0x138 GENDB window-row cursor (mapping)
    std::int32_t u13c;        // 0x13c
};

static_assert(sizeof(DB_WRITE_VAR) == 0x140);
DEF_LAYOUT_OFFSET(DB_WRITE_VAR, u008, 0x008);
DEF_LAYOUT_OFFSET(DB_WRITE_VAR, version, 0x00c);
DEF_LAYOUT_OFFSET(DB_WRITE_VAR, date, 0x010);
DEF_LAYOUT_OFFSET(DB_WRITE_VAR, dimension, 0x01c);
DEF_LAYOUT_OFFSET(DB_WRITE_VAR, u030, 0x030);
DEF_LAYOUT_OFFSET(DB_WRITE_VAR, u048, 0x048);
DEF_LAYOUT_OFFSET(DB_WRITE_VAR, u050, 0x050);
DEF_LAYOUT_OFFSET(DB_WRITE_VAR, u068, 0x068);
DEF_LAYOUT_OFFSET(DB_WRITE_VAR, u074, 0x074);
DEF_LAYOUT_OFFSET(DB_WRITE_VAR, u0c8, 0x0c8);
DEF_LAYOUT_OFFSET(DB_WRITE_VAR, u0e8, 0x0e8);
DEF_LAYOUT_OFFSET(DB_WRITE_VAR, u100, 0x100);
DEF_LAYOUT_OFFSET(DB_WRITE_VAR, u110, 0x110);
DEF_LAYOUT_OFFSET(DB_WRITE_VAR, u13c, 0x13c);

} // namespace def::model
