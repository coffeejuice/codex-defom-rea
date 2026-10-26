// LLWINDEF_DATA (RTTI .?AVLLWINDEF_DATA@@, vtable 0x140535fe0, one slot): a contact window, node
// of the window list Cls_140269250 (inter-object record +0x188 and several object classes).
// Owner: packet kw-g3 (coordinator decision); bodies in src/window/llwindef_data.cpp.
// Spec: docs/re/keywords/FRCWIN.md.
//
//   ctor     14025c100(this, owner): Cls_1401fb020 at 8 (called before the vptr is stored, so it
//            is a non-polymorphic base in the original; modelled as the member `fric`, same
//            layout), _OBJ_GEO3D at 0x220 (1402f9020), ten DYNARY at 0x380.., owner at 0x888,
//            then 14025c350(this, 0)
//   dtor     14025c6e0 (14025d0a0, the ten DYNARY dtors 0x7c0 down to 0x380, 1402f91f0 on 0x220,
//            1401fb1a0); slot 0 is the scalar deleting dtor 14026a380
//   size     0x920: operator new(0x920) in 1402694a0
//   geo3d    _OBJ_GEO3D at 0x220 (p1-07's class, layout/obj_geo3d.hpp): built by the ctor
//            (1402f9020), reset by 14025c350 (vtable slot 2), cleared by 14025d0a0 (1402f9340),
//            copied by 14025c9b0 (1402f9360 / 1402f9b60), destroyed by the dtor (1402f91f0). Window
//            shape 3 keeps its points there; the KEY reader / writer (14041b7d0 / 1402f4890,
//            kw-g3) fill and write its nodes and elements (only MGUSER in 3D sets shape 3).
//   open     0x848..0x887 is not touched by any function ported here.
#pragma once

#include <cstddef>
#include <cstdint>

#include <defmodel/layout/cls_1401fb020.hpp>
#include <defmodel/layout/dynary.hpp>
#include <defmodel/layout/layout_check.hpp>
#include <defmodel/layout/obj_geo3d.hpp>

namespace def::model {

class DB_STEP_DATA;

class LLWINDEF_DATA {
public:
    // @src 14025c100  ctor
    explicit LLWINDEF_DATA(DB_STEP_DATA* owner);
    // @src 14025c6e0  dtor (virtual: slot 0 is the scalar deleting dtor 14026a380)
    virtual ~LLWINDEF_DATA();

    LLWINDEF_DATA(const LLWINDEF_DATA&) = delete;
    LLWINDEF_DATA& operator=(const LLWINDEF_DATA&) = delete;

    // 0x000 vptr
    Cls_1401fb020 fric;          // 0x008 friction block (FRCWIN kind 1..4 at fric.u000)
    std::int32_t index;          // 0x170 1-based position in the list (140269bc0)
    std::int32_t u174;           // 0x174 14025c350 param (1 when reset by FRCWIN)
    std::int32_t npts;           // 0x178 point count (140263cb0)
    std::byte pad_17c[4];        // 0x17c
    double* pts;                 // 0x180 npts * (2 in 2D | 3 in 3D) doubles (malloc)
    double u188[4];              // 0x188 2x2 block (copied by 14025c9b0)
    double u1a8[2];              // 0x1a8
    double u1b8;                 // 0x1b8 shape 2: second end-face radius (face 3 when >= 1e-20)
    double u1c0;                 // 0x1c0 shape 2: end-face radius
    double u1c8[6];              // 0x1c8 3x2 block
    double u1f8[3];              // 0x1f8
    std::int32_t shape;          // 0x210 3D shape (1 box, 2 cylinder, 3, 11..15)
    std::int32_t nface;          // 0x214 number of face centres (140263dc0)
    double* faces;               // 0x218 nface * 3 doubles (malloc)
    OBJ_GEO3D geo3d;             // 0x220 _OBJ_GEO3D (shape 3 points, see the header comment)
    double u350[3];              // 0x350 FRCWIN: three doubles (3D) / two doubles (2D)
    std::int32_t orient;         // 0x368 0, 1 or 2
    std::int32_t orient_id;      // 0x36c object number (orient 2)
    std::uint64_t u370;          // 0x370 =0, copied by 14025c9b0
    std::int32_t u378;           // 0x378 =0, copied
    std::byte pad_37c[4];        // 0x37c
    DYNARY t380[4];              // 0x380, 0x3f8, 0x470, 0x4e8
    std::uint8_t u560;           // 0x560
    std::byte pad_561[7];        // 0x561
    DYNARY t568[6];              // 0x568, 0x5e0, 0x658, 0x6d0, 0x748, 0x7c0
    std::int32_t u838;           // 0x838
    std::byte pad_83c[4];        // 0x83c
    LLWINDEF_DATA* next;         // 0x840 list link
    std::byte pad_848[0x40];     // 0x848 not touched here
    DB_STEP_DATA* owner;         // 0x888 ctor param (the list's owner)
    std::uint64_t u890;          // 0x890 from the list's +0x28 (14025c8e0)
    std::int32_t dim;            // 0x898 owner dimension (14025c8b0); 2 = 2D window
    std::byte pad_89c[0x920 - 0x89c];
};

static_assert(sizeof(LLWINDEF_DATA) == 0x920);
DEF_LAYOUT_OFFSET(LLWINDEF_DATA, fric, 0x008);
DEF_LAYOUT_OFFSET(LLWINDEF_DATA, index, 0x170);
DEF_LAYOUT_OFFSET(LLWINDEF_DATA, npts, 0x178);
DEF_LAYOUT_OFFSET(LLWINDEF_DATA, pts, 0x180);
DEF_LAYOUT_OFFSET(LLWINDEF_DATA, u1b8, 0x1b8);
DEF_LAYOUT_OFFSET(LLWINDEF_DATA, u1c0, 0x1c0);
DEF_LAYOUT_OFFSET(LLWINDEF_DATA, u1c8, 0x1c8);
DEF_LAYOUT_OFFSET(LLWINDEF_DATA, u1f8, 0x1f8);
DEF_LAYOUT_OFFSET(LLWINDEF_DATA, shape, 0x210);
DEF_LAYOUT_OFFSET(LLWINDEF_DATA, faces, 0x218);
DEF_LAYOUT_OFFSET(LLWINDEF_DATA, geo3d, 0x220);
DEF_LAYOUT_OFFSET(LLWINDEF_DATA, u350, 0x350);
DEF_LAYOUT_OFFSET(LLWINDEF_DATA, orient, 0x368);
DEF_LAYOUT_OFFSET(LLWINDEF_DATA, u370, 0x370);
DEF_LAYOUT_OFFSET(LLWINDEF_DATA, t380, 0x380);
DEF_LAYOUT_OFFSET(LLWINDEF_DATA, u560, 0x560);
DEF_LAYOUT_OFFSET(LLWINDEF_DATA, t568, 0x568);
DEF_LAYOUT_OFFSET(LLWINDEF_DATA, u838, 0x838);
DEF_LAYOUT_OFFSET(LLWINDEF_DATA, next, 0x840);
DEF_LAYOUT_OFFSET(LLWINDEF_DATA, owner, 0x888);
DEF_LAYOUT_OFFSET(LLWINDEF_DATA, dim, 0x898);

// Window operations (methods in the binary, free functions here):
void window_init(LLWINDEF_DATA* w, std::int32_t kind);           // 14025c350
void window_free(LLWINDEF_DATA* w);                              // 14025d0a0
void window_reset(LLWINDEF_DATA* w, std::int32_t kind);          // 14025d1a0
void window_copy(LLWINDEF_DATA* dst, const LLWINDEF_DATA* src);  // 14025c9b0
void window_set_shape(LLWINDEF_DATA* w, std::int32_t code);      // 140263450
bool window_is_2d(const LLWINDEF_DATA* w);                       // 14025c930
bool window_is_3d(const LLWINDEF_DATA* w);                       // 14025c970
std::int32_t window_alloc_points(LLWINDEF_DATA* w, std::int32_t n);  // 140263cb0
void window_free_points(LLWINDEF_DATA* w);                       // 140263360
void window_free_faces(LLWINDEF_DATA* w);                        // 1402633d0
std::int32_t window_alloc_faces(LLWINDEF_DATA* w, std::int32_t n);   // 140263dc0
void window_build_faces(LLWINDEF_DATA* w);                       // 140267760
const double* window_face(LLWINDEF_DATA* w, std::int32_t i);     // 140264090

// @data 1406feac0  result of FUN_140267560 (window rim point); the original returns a pointer to
// this static, so each call overwrites the previous result.
inline double g_dat_1406feac0[3] = {0.0, 0.0, 0.0};

} // namespace def::model
