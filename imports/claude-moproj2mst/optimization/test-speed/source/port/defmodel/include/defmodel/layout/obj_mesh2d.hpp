// OBJ_MESH2D (RTTI .?AVOBJ_MESH2D@@, no base): a 2D mesh held by pointer at +0x28 of the 2D
// geometry object (FUN_140280490 copies it). The Phase 1 packet map calls it the result-set
// object Cls_1403ddf20. Owner p1-13; p1-08 keeps vtable slot 1 (class name).
//
//   vtable   0x140562208 (2 slots: 1403e2240 scalar deleting dtor, 1402f0610 class name)
//   ctor     1403ddf20 (vptr, three p1-04 sub-objects, 100 x Cls_14017e740 by eh vector ctor,
//            seven array handles, count 0x2c0 = 0, then reset 1403de2a0)
//   dtor     1403de190 (non-virtual body of slot 0: clear 1403de3d0, members in reverse)
//   size     0x1700: FUN_140280490 does operator new(0x1700) before the ctor.
//   methods  1403de2a0 reset, 1403de3d0 clear, 1403de460 copy_from,
//            1403e2280 / 1403e2290 accessors (FUN_140284ac0 and FUN_1402b7400 of p1-08/p1-07
//            are folded copies of accessors that copy_from also calls on OBJ_MESH2D).
//   open     what the three sub-objects and the element arrays hold (p1-04 owns
//            Cls_14017ef20, Cls_140182880, Cls_140185570, Cls_14017e740); 0x2c4 and 0x16fc are
//            alignment as far as Phase 1 code shows.
#pragma once

#include <cstddef>
#include <cstdint>

#include <defmodel/layout/layout_check.hpp>
#include <defmodel/obj_mesh2d/rc_array.hpp>

namespace def::model {

class OBJ_MESH2D {
public:
    // @src 1403ddf20  ctor
    OBJ_MESH2D();
    // @src 1403de190  dtor body; slot 0 (1403e2240) is the scalar deleting dtor around it
    virtual ~OBJ_MESH2D();
    // @src 1402f0610  slot 1: class name "OBJ_MESH2D" (p1-08; stub in src/layout/obj_mesh2d.cpp)
    virtual const char* class_name() const;

    // @src 1403de2a0  clear, then the default flags
    void reset();
    // @src 1403de3d0  clear the sub-objects and the first `count` elements
    void clear();
    // @src 1403de460  reset, then copy both meshes of `src` (the original takes three more
    // arguments that it does not read)
    void copy_from(const OBJ_MESH2D* src);
    // @src 1403e2280  address of the first sub-object
    std::byte* mesh_a();
    // @src 1403e2290  the int at +0xec of the second sub-object (FUN_140185530, folded)
    std::int32_t mesh_b_count() const;

    // vptr at 0x000
    std::byte s008[0x140];            // 0x008 Cls_14017ef20 (p1-04; ctor writes +0x130, +0x138)
    std::byte s148[0x108];            // 0x148 Cls_140182880 (p1-04; ctor writes +0xf0, +0xf8)
    std::byte s250[0x70];             // 0x250 Cls_140185570 (p1-04; ctor writes +0x60, +0x68)
    std::int32_t count;               // 0x2c0 elements of s2c8 in use; ctor =0
    std::byte pad_2c4[4];             // 0x2c4
    std::byte s2c8[100][0x30];        // 0x2c8 100 x Cls_14017e740 (ctor 14017e740, dtor 14017e720)
    mesh2d::RcArray<mesh2d::Elem28_1403e2370>* h1588; // 0x1588 handle 1403e2850 / 1403e28c0
    mesh2d::RcArray<mesh2d::Elem18_1403e2450>* h1590; // 0x1590 handle 1403e29f0 / 1403e2a60
    mesh2d::RcArray<mesh2d::Face18>* h1598;           // 0x1598 handle 1403e2bb0 / 1403e2c20
    mesh2d::RcArray<mesh2d::Elem10_1403e2610>* h15a0; // 0x15a0 handle 1403e2d50 / 1403e2dc0
    mesh2d::RcArray<mesh2d::Elem20_1403e25a0>* h15a8; // 0x15a8 handle 1403e2ee0 / 1403e2f50
    mesh2d::RcArray<mesh2d::Elem10_1403e2750>* h15b0; // 0x15b0 handle 1403e3210 / 1403e3280
    mesh2d::RcArray<mesh2d::Elem18_1403e23f0>* h15b8; // 0x15b8 handle 1403e33d0 / 1403e3440
    std::int32_t flags[0x4f];         // 0x15c0 reset: all 1, entries 0..6, 18, 19, 25 = 2
    std::byte pad_16fc[4];            // 0x16fc
};

static_assert(sizeof(OBJ_MESH2D) == 0x1700);
DEF_LAYOUT_OFFSET(OBJ_MESH2D, s008, 0x008);
DEF_LAYOUT_OFFSET(OBJ_MESH2D, s148, 0x148);
DEF_LAYOUT_OFFSET(OBJ_MESH2D, s250, 0x250);
DEF_LAYOUT_OFFSET(OBJ_MESH2D, count, 0x2c0);
DEF_LAYOUT_OFFSET(OBJ_MESH2D, s2c8, 0x2c8);
DEF_LAYOUT_OFFSET(OBJ_MESH2D, h1588, 0x1588);
DEF_LAYOUT_OFFSET(OBJ_MESH2D, h1598, 0x1598);
DEF_LAYOUT_OFFSET(OBJ_MESH2D, h15b8, 0x15b8);
DEF_LAYOUT_OFFSET(OBJ_MESH2D, flags, 0x15c0);

namespace mesh2d {

// Calls OBJ_MESH2D makes into packet p1-04 (its sub-objects). p1-04 has not landed yet: each
// pointer starts at a stub that stops in def::rt::not_ported; p1-04's integration points them
// at its functions (tests point them at recorders). `p` is the sub-object's address.
struct MemberExt {
    void (*ctor_a)(void* p);         // FUN_14017ef20  Cls_14017ef20 ctor
    void (*dtor_a)(void* p);         // FUN_14017ef80
    void (*clear_a)(void* p);        // FUN_14017f130
    // FUN_140180390(dst, src, n, flags, keep_order)
    void (*copy_a)(void* dst, const void* src, std::int32_t n, std::int32_t* flags, char b);
    void (*ctor_b)(void* p);         // FUN_140182880  Cls_140182880 ctor
    void (*dtor_b)(void* p);         // FUN_1401828e0
    void (*clear_b)(void* p);        // FUN_140182a50
    // FUN_140183900(dst, src, n1, n2, flags, b6, b7, b8)
    void (*copy_b)(void* dst, const void* src, std::int32_t n1, std::int32_t n2, std::int32_t* flags,
                   char b6, char b7, char b8);
    void (*ctor_c)(void* p);         // FUN_140185570  Cls_140185570 ctor
    void (*dtor_c)(void* p);         // FUN_1401855b0
    void (*clear_c)(void* p);        // FUN_1401856e0
    void (*ctor_e)(void* p);         // FUN_14017e740  Cls_14017e740 ctor (0x30)
    void (*dtor_e)(void* p);         // FUN_14017e720
    void (*clear_e)(void* p);        // FUN_14017e800
};
MemberExt& member_ext();

} // namespace mesh2d

} // namespace def::model
