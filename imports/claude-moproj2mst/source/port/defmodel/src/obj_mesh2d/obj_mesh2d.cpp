// OBJ_MESH2D methods (packet p1-13). Layout: defmodel/layout/obj_mesh2d.hpp.
#include <defmodel/layout/obj_mesh2d.hpp>

#include <defmodel/llobj/cls_14017e740.hpp>
#include <defmodel/llobj/cls_14017ef20.hpp>
#include <defmodel/llobj/cls_140182880.hpp>
#include <defmodel/llobj/cls_140185570.hpp>
#include <defmodel/port.hpp>

#include <new>

#include <cstring>

namespace def::model {

namespace mesh2d {

namespace {

// Default targets: p1-04's sub-object classes (wired at the Phase 1 gate). `p` is the
// sub-object's storage inside OBJ_MESH2D.
void ctor_a(void* p) { ::new (p) Cls_14017ef20(); }
void dtor_a(void* p) { static_cast<Cls_14017ef20*>(p)->~Cls_14017ef20(); }
void clear_a(void* p) { static_cast<Cls_14017ef20*>(p)->free_arrays(); }
void copy_a(void* dst, const void* src, std::int32_t n, std::int32_t* flags, char b)
{
    static_cast<Cls_14017ef20*>(dst)->copy_from(*static_cast<const Cls_14017ef20*>(src), n, flags, b != 0);
}
void ctor_b(void* p) { ::new (p) Cls_140182880(); }
void dtor_b(void* p) { static_cast<Cls_140182880*>(p)->~Cls_140182880(); }
void clear_b(void* p) { static_cast<Cls_140182880*>(p)->free_arrays(); }
void copy_b(void* dst, const void* src, std::int32_t n1, std::int32_t n2, std::int32_t* flags, char b6, char b7,
            char b8)
{
    static_cast<Cls_140182880*>(dst)->copy_from(*static_cast<const Cls_140182880*>(src), n1, n2, flags, b6 != 0,
                                                b7 != 0, b8 != 0);
}
void ctor_c(void* p) { ::new (p) Cls_140185570(); }
void dtor_c(void* p) { static_cast<Cls_140185570*>(p)->~Cls_140185570(); }
void clear_c(void* p) { static_cast<Cls_140185570*>(p)->free_arrays(); }
void ctor_e(void* p) { ::new (p) Cls_14017e740(); }
void dtor_e(void* p) { static_cast<Cls_14017e740*>(p)->~Cls_14017e740(); }
void clear_e(void* p) { static_cast<Cls_14017e740*>(p)->free_arrays(); }

MemberExt g_member_ext = {ctor_a, dtor_a, clear_a, copy_a, ctor_b, dtor_b, clear_b,
                          copy_b, ctor_c, dtor_c, clear_c, ctor_e, dtor_e, clear_e};

// Fields of the p1-04 sub-objects that OBJ_MESH2D reads and writes directly through folded
// one-line accessors.
std::int32_t get_i32(const std::byte* p, std::size_t off)
{
    std::int32_t v;
    std::memcpy(&v, p + off, 4);
    return v;
}
void set_i32(std::byte* p, std::size_t off, std::int32_t v) { std::memcpy(p + off, &v, 4); }

constexpr std::size_t kElemSize = 0x30;
constexpr int kElems = 100;

// Handles (the pairs ctor / dtor per member, each with its block functions).

// @src 1403e2850  handle at 0x1588: new block
// @src 1403e4210  (block ctor, inlined here)
RcArray<Elem28_1403e2370>* new_h1588() { return new_rc_array<Elem28_1403e2370>(); }
// @src 1403e28c0  handle at 0x1588: release
// @src 1403e4260  (reference drop, inlined here)
// @src 1403e4ba0  (block scalar deleting dtor, inlined here)
// @src 1403e50b0  (block Destroy, inlined here)
// @src 1403e4be0  (vector deleting dtor of the 0x28 elements, inlined here)
void release_h1588(RcArray<Elem28_1403e2370>* b) { release_rc_array(b); }

// @src 1403e29f0  handle at 0x1590: new block
// @src 1403e4340  (block ctor, inlined here)
RcArray<Elem18_1403e2450>* new_h1590() { return new_rc_array<Elem18_1403e2450>(); }
// @src 1403e2a60  handle at 0x1590: release
// @src 1403e4390  (reference drop, inlined here)
// @src 1403e4c70  (block scalar deleting dtor, inlined here)
// @src 1403e5130  (block Destroy, inlined here)
// @src 1403e4cb0  (vector deleting dtor of the 0x18 elements, inlined here)
void release_h1590(RcArray<Elem18_1403e2450>* b) { release_rc_array(b); }

// @src 1403e2bb0  handle at 0x1598: new block
// @src 1403e44a0  (block ctor, inlined here)
RcArray<Face18>* new_h1598() { return new_rc_array<Face18>(); }
// @src 1403e2c20  handle at 0x1598: release
// @src 1403e44f0  (reference drop, inlined here)
// @src 1403e4d40  (block scalar deleting dtor, inlined here)
// @src 1403e5180  (block dtor: free(data), inlined here; delete[] of the Face18 array here)
void release_h1598(RcArray<Face18>* b) { release_rc_array(b); }

// @src 1403e2d50  handle at 0x15a0: new block
// @src 1403e45a0  (block ctor, inlined here)
RcArray<Elem10_1403e2610>* new_h15a0() { return new_rc_array<Elem10_1403e2610>(); }
// @src 1403e2dc0  handle at 0x15a0: release
// @src 1403e45f0  (reference drop, inlined here)
// @src 1403e4d80  (block scalar deleting dtor, inlined here)
// @src 1403e51e0  (block Destroy, inlined here)
// @src 1403e53b0  (vector deleting dtor of the 0x10 elements, inlined here)
void release_h15a0(RcArray<Elem10_1403e2610>* b) { release_rc_array(b); }

// @src 1403e2ee0  handle at 0x15a8: new block
// @src 1403e4670  (block ctor, inlined here)
RcArray<Elem20_1403e25a0>* new_h15a8() { return new_rc_array<Elem20_1403e25a0>(); }
// @src 1403e2f50  handle at 0x15a8: release
// @src 1403e46c0  (reference drop, inlined here)
// @src 1403e4dc0  (block scalar deleting dtor, inlined here)
// @src 1403e5230  (block Destroy, inlined here)
// @src 1403e4e00  (vector deleting dtor of the 0x20 elements, inlined here)
void release_h15a8(RcArray<Elem20_1403e25a0>* b) { release_rc_array(b); }

// @src 1403e3210  handle at 0x15b0: new block
// @src 1403e4820  (block ctor, inlined here)
RcArray<Elem10_1403e2750>* new_h15b0() { return new_rc_array<Elem10_1403e2750>(); }
// @src 1403e3280  handle at 0x15b0: release
// @src 1403e4870  (reference drop, inlined here)
// @src 1403e4ed0  (block scalar deleting dtor, inlined here)
// @src 1403e52c0  (block Destroy, inlined here)
// @src 1403e4f10  (vector deleting dtor of the 0x10 elements, inlined here)
void release_h15b0(RcArray<Elem10_1403e2750>* b) { release_rc_array(b); }

// @src 1403e33d0  handle at 0x15b8: new block
// @src 1403e4970  (block ctor, inlined here)
RcArray<Elem18_1403e23f0>* new_h15b8() { return new_rc_array<Elem18_1403e23f0>(); }
// @src 1403e3440  handle at 0x15b8: release
// @src 1403e49c0  (reference drop, inlined here)
// @src 1403e4fa0  (block scalar deleting dtor, inlined here)
// @src 1403e5310  (block Destroy, inlined here)
// @src 1403e4fe0  (vector deleting dtor of the 0x18 elements, inlined here)
void release_h15b8(RcArray<Elem18_1403e23f0>* b) { release_rc_array(b); }

} // namespace

MemberExt& member_ext() { return g_member_ext; }

} // namespace mesh2d

using mesh2d::member_ext;

// @src 1403ddf20  ctor
OBJ_MESH2D::OBJ_MESH2D()
{
    // vptr = OBJ_MESH2D::vftable (0x140562208), set by the compiler.
    member_ext().ctor_a(s008);
    member_ext().ctor_b(s148);
    member_ext().ctor_c(s250);
    // _eh_vector_constructor_iterator_(this+0x2c8, 0x30, 100, 14017e740, 14017e720)
    for (int i = 0; i < mesh2d::kElems; ++i) {
        member_ext().ctor_e(s2c8[i]);
    }
    h1588 = mesh2d::new_h1588();
    h1590 = mesh2d::new_h1590();
    h1598 = mesh2d::new_h1598();
    h15a0 = mesh2d::new_h15a0();
    h15a8 = mesh2d::new_h15a8();
    h15b0 = mesh2d::new_h15b0();
    h15b8 = mesh2d::new_h15b8();
    count = 0;
    reset();
}

// @src 1403de190  dtor
// @src 1403e2240  (slot 0, scalar deleting dtor: this dtor, then free; compiler-generated)
OBJ_MESH2D::~OBJ_MESH2D()
{
    clear();
    mesh2d::release_h15b8(h15b8);
    mesh2d::release_h15b0(h15b0);
    mesh2d::release_h15a8(h15a8);
    mesh2d::release_h15a0(h15a0);
    mesh2d::release_h1598(h1598);
    mesh2d::release_h1590(h1590);
    mesh2d::release_h1588(h1588);
    // _eh_vector_destructor_iterator_(this+0x2c8, 0x30, 100, 14017e720): last element first.
    for (int i = mesh2d::kElems - 1; i >= 0; --i) {
        member_ext().dtor_e(s2c8[i]);
    }
    member_ext().dtor_c(s250);
    member_ext().dtor_b(s148);
    member_ext().dtor_a(s008);
}

// @src 1403de2a0  clear, then the default flags
void OBJ_MESH2D::reset()
{
    clear();
    for (int i = 0; i < 0x4f; ++i) {
        flags[i] = 1;
    }
    flags[0] = 2; // 0x15c0
    flags[1] = 2; // 0x15c4
    flags[2] = 2; // 0x15c8
    flags[3] = 2; // 0x15cc
    flags[4] = 2; // 0x15d0
    flags[5] = 2; // 0x15d4
    flags[6] = 2; // 0x15d8
    flags[18] = 2; // 0x1608
    flags[19] = 2; // 0x160c
    flags[25] = 2; // 0x1624
    mesh2d::set_i32(s008, 0x138, 2); // FUN_14019a370 (folded setter)
    mesh2d::set_i32(s148, 0xf8, 2);  // FUN_14019a3f0 (folded setter)
    mesh2d::set_i32(s250, 0x68, 2);  // FUN_14019a4b0 (folded setter)
}

// @src 1403de3d0  clear
void OBJ_MESH2D::clear()
{
    member_ext().clear_a(s008);
    member_ext().clear_b(s148);
    member_ext().clear_c(s250);
    for (std::int32_t i = 0; i < count; ++i) {
        member_ext().clear_e(s2c8[i]);
    }
}

// @src 1403de460  copy both meshes of src
void OBJ_MESH2D::copy_from(const OBJ_MESH2D* src)
{
    clear();
    reset();
    auto* s = const_cast<OBJ_MESH2D*>(src);
    const std::int32_t na = mesh2d::get_i32(s->s008, 300); // FUN_140284aa0 -> FUN_140182840
    std::byte* a = s->mesh_a();                             // FUN_1403e2280
    member_ext().copy_a(s008, a, na, flags, 1);
    const std::int32_t nb2 = s->mesh_b_count();              // FUN_1403e2290
    const std::int32_t nb1 = mesh2d::get_i32(s->s148, 0xe8); // FUN_140284ac0 -> FUN_1401854f0
    const std::byte* b = s->s148;                            // FUN_1402b7400
    member_ext().copy_b(s148, b, nb1, nb2, flags, 1, 1, 1);
}

// @src 1403e2280  address of the first sub-object
std::byte* OBJ_MESH2D::mesh_a() { return s008; }

// @src 1403e2290  count field of the second sub-object
std::int32_t OBJ_MESH2D::mesh_b_count() const
{
    return mesh2d::get_i32(s148, 0xec); // FUN_140185530 (folded accessor)
}

} // namespace def::model
