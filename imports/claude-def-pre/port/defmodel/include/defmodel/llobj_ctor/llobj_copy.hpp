// defmodel/llobj_ctor/llobj_copy.hpp: LLOBJ_DATA "copy from another object" (FUN_1401613a0) and
// the copy functions of its sub-objects and lists that no Phase 1 packet owns. Packet kw-g2obj.
//
// The sub-objects are byte slots of LLOBJ_DATA (layout/llobj_data.hpp) or p1-04 classes with
// byte-slot layouts, so these functions take addresses and read/write at the binary's offsets,
// like the decompile. The @src tags are on the definitions (src/llobj_ctor/llobj_copy*.cpp).
//
// Calls into functions other packets own go through the wrappers in namespace ext below (the
// llobj::ext scheme: llobj::ext::g_hook observes them in tests, otherwise the owner's code runs
// through the switch in src/llobj/ext.cpp; 14039d3a0, reached only when calloc fails, still
// stops in DEF_NOT_PORTED). Pseudo address 0x4 (this file): virtual slot 4 (vtable +0x20) of
// an _OBJ_GEO3D with one argument, called by 1401618b0.
#pragma once

#include <defmodel/window/inter_object_ops.hpp>

#include <cstddef>
#include <cstdint>

#include <defmodel/llobj/ext.hpp>

namespace def::model {
class DYNARY;
class LLOBJ_DATA;
class Cls_1401364a0;
class SfLinkedList;
class SfLinkedData;

// 1401613a0: copy src into dst (dst is cleared first; nothing more when src is null). sizes are
// the model's array sizes (passed on to the state-variable sets); p4..p9 are the binary's byte
// flags: p8 copy +0x8, p7 copy the name, p5 keep element variable 0x11, p4/p6/p9 keep node
// variables 0x3c / 0x31..0x34 / 0x28.
void llobj_copy_from(LLOBJ_DATA* dst, const LLOBJ_DATA* src, const std::int32_t* sizes,
                     const bool* p4, const bool* p5, const bool* p6, const bool* p7,
                     const bool* p8, const bool* p9);
} // namespace def::model

namespace def::model::llobj_copy {

// ---- LLOBJ_DATA level (llobj_copy.cpp) --------------------------------------------------------
void reset_keep_geometry(LLOBJ_DATA* obj);                         // 140158f70
void copy_geometry(LLOBJ_DATA* dst, const LLOBJ_DATA* src);        // 140161840
void copy_geo3d_slots(LLOBJ_DATA* dst, const LLOBJ_DATA* src);     // 1401618b0
void copy_u3998(LLOBJ_DATA* dst, const LLOBJ_DATA* src);           // 1401619c0
void copy_u3aa0(LLOBJ_DATA* dst, const LLOBJ_DATA* src);           // 140161ac0
void set_layer_count(LLOBJ_DATA* obj, std::int32_t n);             // 140163300
std::int32_t node_array_size(LLOBJ_DATA* obj);                     // 140165170
void sum_layer_counts(LLOBJ_DATA* obj);                            // 140165d30
std::uint8_t get_u3990(const LLOBJ_DATA* obj);                     // 140160d90
std::int32_t get_u3994(const LLOBJ_DATA* obj);                     // 1401985c0
void set_u3994(LLOBJ_DATA* obj, std::int32_t v);                   // 140198600
void* geo_slot(const LLOBJ_DATA* obj, const std::int32_t* i);      // 140198650
void geo3d_assign(void* dst, const void* src);                     // 140199c20

// ---- sub-objects (llobj_copy_parts.cpp) -------------------------------------------------------
void layer_copy(std::int32_t* dst, const std::int32_t* src);       // 14017ec20 (Cls_14017e740)
void layer_alloc(std::int32_t* p);                                 // 14017e8a0
std::int32_t layer_count(const std::int32_t* p);                   // 14017ebc0
void* calloc_int(std::int32_t n);                                  // 140256dc0
void* calloc_double(std::int32_t n);                               // 140257150
void arrays_alloc(void** p, std::int32_t n, std::int32_t key);     // 1401858d0 (Cls_140185570)
void arrays_copy(void** dst, void* const* src, std::int32_t n);    // 140185a10
void props_copy(std::byte* dst, const std::byte* src);             // 140186590 (Cls_1401860a0)
DYNARY* dynary_assign(DYNARY* dst, const DYNARY* src);             // 14019a500
// 14012eff0 DYNARY copy: kw-g3's def::model::dynary_copy (window/inter_object_ops.hpp)
void movement_copy(std::byte* dst, const std::byte* src);          // 14018bd30 (Cls_14018b220)
void movement_copy_body(std::byte* dst, const std::byte* src);     // 14018beb0
void meshgen_copy(std::byte* dst, const std::byte* src, const char* shapes,
                  const char* by_dim);                             // 140189b70 (Cls_140189440)
void meshgen_alloc(std::byte* p, std::int32_t n);                  // 14018af80
void bc_copy(std::byte* dst, const std::byte* src);                // 140195c80 (Cls_1401956d0)
void dynary_list_copy(Cls_1401364a0* dst, const Cls_1401364a0* src); // 140136560

// ---- lists (llobj_copy_lists.cpp) -------------------------------------------------------------
// Cls_140269250 (0x30): list of LLWINDEF_DATA (0x920 bytes, linked at +0x840)
void shape_list_copy(std::byte* dst, const std::byte* src);        // 140269340 (kw-g3 winlist_copy)
bool geo3d_has_mesh(const void* geo3d);                            // 1402f9360
// list of 0xe8-byte records linked at +0xe0 (Cls_1401860a0+0x200)
void axis_list_copy(std::byte* dst, const std::byte* src);         // 1402598c0
std::int32_t axis_list_count(const std::byte* list);               // 14019a550
std::int32_t* axis_list_add(std::byte* list, std::int32_t no);     // 140259950
std::byte* axis_record_construct(std::byte* r);                    // 140258580
void axis_record_init(std::byte* r);                               // 1402585b0
void axis_record_copy(std::byte* dst, const std::byte* src);       // 140258750
std::byte* axis_list_at(const std::byte* list, std::int32_t n);    // 140259b60
// SfLinkedList (Cls_1401860a0+0x218, an OpStopLinkedList)
void sf_list_copy(SfLinkedList* dst, const SfLinkedList* src);     // 14025a730
std::int32_t sf_list_count(const SfLinkedList* list);              // 14019a580
SfLinkedData* sf_list_add(SfLinkedList* list);                     // 14025a7d0
SfLinkedData* sf_list_at(const SfLinkedList* list, std::int32_t n); // 14025a9e0
// object at LLOBJ_DATA+0x3998 (8 bytes: pointer to an MSVC std::list body; p1-09 class)
void sv_list_copy(void* dst, const void* src);                     // 1403019d0
std::int32_t sv_list_size(const void* list);                       // 140301c60
std::uint64_t sv_list_size_t(const void* list);                    // 140301e70
std::uint64_t* sv_iter_null(std::uint64_t* it);                    // 140301fa0
std::uint64_t* sv_list_begin(const void* list, std::uint64_t* out); // 140301df0
std::uint64_t sv_list_value_at(const void* list, std::int32_t i);  // 140301b40
std::uint64_t* sv_list_iter_at(const void* list, std::uint64_t* out, std::uint64_t i); // 140301f50
void* sv_list_node_at(const void* body, std::uint64_t i);          // 1403025c0
bool sv_iter_ne(const std::uint64_t* a, const std::uint64_t* b);   // 140301fe0

} // namespace def::model::llobj_copy

namespace def::model::llobj_copy::ext {
using llobj::ext::call;
using llobj::ext::u;

// ---- p1-01 (COW vectors of _OBJ_GEO3D) --------------------------------------------------------
// 1400034d0: point count ((end - begin) / 8 / 3 of the vector at +0x10)
inline std::int32_t f1400034d0(const void* geo3d) { return static_cast<std::int32_t>(call(0x1400034d0, u(geo3d))); }
// 140003500: facet count ((end - begin) / 4 / 4 of the vector at +0x18)
inline std::int32_t f140003500(const void* geo3d) { return static_cast<std::int32_t>(call(0x140003500, u(geo3d))); }

// ---- p1-06 (linked lists) ---------------------------------------------------------------------
// 140257c80: copy of the list at Cls_1401956d0+0x18 (records of 0x40 bytes)
inline void f140257c80(void* dst, const void* src) { call(0x140257c80, u(dst), u(src)); }

// ---- p1-07 (_OBJ_GEO3D) -----------------------------------------------------------------------
// 1402f9b60: _OBJ_GEO3D copy (assign from src)
inline void f1402f9b60(void* dst, const void* src) { call(0x1402f9b60, u(dst), u(src)); }
// 140280490: copy of a state-variable list element (dst element, src element)
inline void f140280490(std::uint64_t dst, std::uint64_t src) { call(0x140280490, dst, src); }
// virtual slot 4 (vtable +0x20) of an _OBJ_GEO3D with one argument (pseudo address 0x4)
void geo3d_slot4(void* obj, void* arg);

// ---- p1-09 (std::list iterator helpers of the object at LLOBJ_DATA+0x3998) ---------------------
// 140301e30: end iterator into *out, returns out
inline std::uint64_t* f140301e30(const void* list, std::uint64_t* out)
{
    return reinterpret_cast<std::uint64_t*>(call(0x140301e30, u(list), u(out)));
}
// 140302020: dereference an iterator (the element)
inline std::uint64_t f140302020(const std::uint64_t* it) { return call(0x140302020, u(it)); }
// 140302040: ++it (post-increment form: old value into *out, third argument 0)
inline void f140302040(std::uint64_t* it, std::uint64_t* out, std::int32_t z)
{
    call(0x140302040, u(it), u(out), static_cast<std::uint32_t>(z));
}
// 140302090: detach the COW list body when it is shared (refs > 1 -> 140302210)
inline void f140302090(const void* list) { call(0x140302090, u(list)); }
// 1403020c0: iterator ctor: *out = node, returns out
inline void f1403020c0(std::uint64_t* out, void* node) { call(0x1403020c0, u(out), u(node)); }

// ---- area kw_group_handlers_dds (Phase 2, no Phase 1 packet; not cpp_data_model) --------------
// 14039d3a0: message printf (fmt, three varargs) through 14039d570(0, fmt, va)
inline void f14039d3a0(const char* fmt, const char* a, const char* b, std::int32_t n)
{
    call(0x14039d3a0, u(fmt), u(a), u(b), static_cast<std::uint32_t>(n));
}


} // namespace def::model::llobj_copy::ext
