// Copy functions of the LLOBJ_DATA sub-objects used by 1401613a0 (layers 0xbe0, node arrays
// 0xae8, properties 0x1ea0, movement 0x2160, mesh generation 0x2d10, boundary conditions 0x2e68)
// and the DYNARY copy. Decompiles: deform-rea-work/out/g2obj/copy_closure.c, copy_deep.c.
#include <defmodel/bc/boundary_topology.hpp>
#include <defmodel/bc/edge_face.hpp>
#include <defmodel/llobj_ctor/llobj_copy.hpp>
#include <defmodel/window/inter_object_ops.hpp>

#include <cstdlib>
#include <cstring>

#include <defmodel/dynary_init.hpp>
#include <defmodel/layout/cls_1401364a0.hpp>
#include <defmodel/layout/dynary.hpp>
#include <defmodel/llobj/cls_14017e740.hpp>
#include <defmodel/llobj/cls_140185570.hpp>
#include <defmodel/llobj/cls_140189440.hpp>
#include <defmodel/llobj/cls_1401956d0.hpp>
#include <defmodel/llobj/dynary_table.hpp>
#include <defrt/stdlib/refcount.hpp>

namespace def::model::llobj_copy {

namespace {
template <class T>
T& at(void* p, std::size_t off)
{
    return *reinterpret_cast<T*>(static_cast<std::byte*>(p) + off);
}
template <class T>
const T& at(const void* p, std::size_t off)
{
    return *reinterpret_cast<const T*>(static_cast<const std::byte*>(p) + off);
}
void copy32(std::byte* d, const std::byte* s, std::size_t off) { at<std::int32_t>(d, off) = at<std::int32_t>(s, off); }
void copy64(std::byte* d, const std::byte* s, std::size_t off) { at<std::uint64_t>(d, off) = at<std::uint64_t>(s, off); }
void copy8(std::byte* d, const std::byte* s, std::size_t off) { at<std::uint8_t>(d, off) = at<std::uint8_t>(s, off); }
DYNARY* dy(std::byte* p) { return reinterpret_cast<DYNARY*>(p); }
const DYNARY* dy(const std::byte* p) { return reinterpret_cast<const DYNARY*>(p); }

// str 1405e4e50, 1405e4e40, 1405e4e38 / 1405e5098, 1405e5088, 1405e507c
constexpr const char kAllocFmt[] = "%s Error when allocating %s memory of size %d\n";
constexpr const char kError[] = "ERROR    ";
constexpr const char kInt[] = "INT";
constexpr const char kDouble[] = "DOUBLE";
} // namespace

// @src 14017ec20  layer copy: free (14017e800), counts +0 and +0x10, allocate (14017e8a0), then
// the three arrays (+8: n ints, +0x18: 4 * m ints, +0x20: m ints)
void layer_copy(std::int32_t* dst, const std::int32_t* src)
{
    reinterpret_cast<Cls_14017e740*>(dst)->free_arrays();  // 14017e800
    dst[0] = src[0];
    dst[4] = src[4];
    layer_alloc(dst);
    if (0 < dst[0] && at<void*>(dst, 8) != nullptr)
        std::memcpy(at<void*>(dst, 8), at<void*>(src, 8), static_cast<std::size_t>(static_cast<std::int64_t>(dst[0]) << 2));
    if (0 < dst[4] && at<void*>(dst, 0x18) != nullptr)
        std::memcpy(at<void*>(dst, 0x18), at<void*>(src, 0x18),
                    static_cast<std::size_t>(static_cast<std::int64_t>(static_cast<std::int32_t>(dst[4] << 2)) << 2));
    if (0 < dst[4] && at<void*>(dst, 0x20) != nullptr)
        std::memcpy(at<void*>(dst, 0x20), at<void*>(src, 0x20), static_cast<std::size_t>(static_cast<std::int64_t>(dst[4]) << 2));
}

// @src 14017e8a0  layer arrays: +8 = n ints (n = +0), +0x18 = 4 * m ints and +0x20 = m ints
// (m = +0x10), each freed first
void layer_alloc(std::int32_t* p)
{
    if (0 < p[0]) {
        if (at<void*>(p, 8) != nullptr) std::free(at<void*>(p, 8));
        p[2] = 0;
        p[3] = 0;
        at<void*>(p, 8) = calloc_int(p[0]);
    }
    if (0 < p[4]) {
        if (at<void*>(p, 0x18) != nullptr) std::free(at<void*>(p, 0x18));
        p[6] = 0;
        p[7] = 0;
        at<void*>(p, 0x18) = calloc_int(static_cast<std::int32_t>(p[4] << 2));
        if (at<void*>(p, 0x20) != nullptr) std::free(at<void*>(p, 0x20));
        p[8] = 0;
        p[9] = 0;
        at<void*>(p, 0x20) = calloc_int(p[4]);
    }
}

// 14017ebc0: kw-g2bc's bc::boundary_segment_count
std::int32_t layer_count(const std::int32_t* p) { return bc::boundary_segment_count(*reinterpret_cast<const Cls_14017e740*>(p)); }

// 140256dc0 / 140257150: owner defmodel bc::calloc_ints / calloc_doubles (p8-int decision)
void* calloc_int(std::int32_t n) { return bc::calloc_ints(n); }
void* calloc_double(std::int32_t n) { return bc::calloc_doubles(n); }

// 1401858d0: kw-g2bc's bc::alloc_edge_face_arrays
void arrays_alloc(void** p, std::int32_t n, std::int32_t key) { bc::alloc_edge_face_arrays(*reinterpret_cast<Cls_140185570*>(p), n, key); }

// @src 140185a10  free (1401856e0), allocate n (1401858d0(p, n, -1)), copy each array src has
void arrays_copy(void** dst, void* const* src, std::int32_t n)
{
    reinterpret_cast<Cls_140185570*>(dst)->free_arrays();  // 1401856e0
    arrays_alloc(dst, n, -1);
    const auto n4 = static_cast<std::size_t>(static_cast<std::int64_t>(n) << 2);
    const auto n8 = static_cast<std::size_t>(static_cast<std::int64_t>(n) << 3);
    if (src[0] != nullptr) std::memcpy(dst[0], src[0], n4);
    if (src[1] != nullptr) std::memcpy(dst[1], src[1], n4);
    if (src[2] != nullptr) std::memcpy(dst[2], src[2], n8);
    if (src[3] != nullptr) std::memcpy(dst[3], src[3], n4);
    if (src[4] != nullptr) std::memcpy(dst[4], src[4], n4);
    if (src[5] != nullptr) std::memcpy(dst[5], src[5], n8);
    if (src[6] != nullptr) std::memcpy(dst[6], src[6], n4);
    if (src[7] != nullptr) std::memcpy(dst[7], src[7], n4);
    if (src[8] != nullptr) std::memcpy(dst[8], src[8], n8);
    if (src[9] != nullptr) std::memcpy(dst[9], src[9], n4);
    if (src[10] != nullptr) std::memcpy(dst[10], src[10], n4);
    if (src[11] != nullptr) std::memcpy(dst[11], src[11], n8);
}

// @src 140186590  property sub-object copy (Cls_1401860a0, LLOBJ_DATA+0x1ea0; owner at +0x2b8)
void props_copy(std::byte* dst, const std::byte* src)
{
    copy64(dst, src, 0x0);
    copy64(dst, src, 0x8);
    copy64(dst, src, 0x10);
    copy64(dst, src, 0x18);
    copy32(dst, src, 0x28);
    for (std::int32_t k = 0; k < 3; k = k + 1) copy64(dst, src, 0x30 + static_cast<std::int64_t>(k) * 8);
    copy32(dst, src, 0x48);
    copy32(dst, src, 0x4c);
    copy32(dst, src, 0x50);
    for (std::int32_t k = 0; k < 2; k = k + 1) copy64(dst, src, 0x58 + static_cast<std::int64_t>(k) * 8);
    copy32(dst, src, 0x68);
    copy64(dst, src, 0x70);
    copy32(dst, src, 0x78);
    if (rt::dimension_is_2(at<void*>(dst, 0x2b8))) {
        copy32(dst, src, 0x80);
        copy64(dst, src, 0x88);
        copy32(dst, src, 0x90);
    }
    copy32(dst, src, 0x2b0);
    copy32(dst, src, 0x7c);
    copy32(dst, src, 0x94);
    copy32(dst, src, 0x98);
    copy32(dst, src, 0x9c);
    copy32(dst, src, 0xa0);
    copy32(dst, src, 0xa4);
    copy32(dst, src, 0xa8);
    copy64(dst, src, 0xb0);
    copy64(dst, src, 0xb8);
    dynary_assign(dy(dst + 0xc0), dy(src + 0xc0));
    copy32(dst, src, 0x138);
    dynary_assign(dy(dst + 0x140), dy(src + 0x140));
    if (rt::dimension_is_3(at<void*>(dst, 0x2b8))) {
        copy64(dst, src, 0x1b8);
        for (std::int32_t k = 0; k < 3; k = k + 1) {
            copy64(dst, src, 0x1c0 + static_cast<std::int64_t>(k) * 8);
            copy64(dst, src, 0x1d8 + static_cast<std::int64_t>(k) * 8);
        }
    }
    axis_list_copy(dst + 0x200, src + 0x200);  // 1402598c0
    sf_list_copy(reinterpret_cast<SfLinkedList*>(dst + 0x218), reinterpret_cast<const SfLinkedList*>(src + 0x218)); // 14025a730
    copy64(dst, src, 0x1f0);
    copy32(dst, src, 0x1f8);
    copy32(dst, src, 0x2b4);
    dynary_assign(dy(dst + 0x238), dy(src + 0x238));
}

// @src 14019a500  DYNARY assignment: 14012eff0, returns dst
DYNARY* dynary_assign(DYNARY* dst, const DYNARY* src)
{
    dynary_copy(dst, src);
    return dst;
}

// 14012eff0 (DYNARY copy) is kw-g3's def::model::dynary_copy (window/inter_object_ops.hpp)

// @src 14018bd30  movement sub-object copy (Cls_14018b220, LLOBJ_DATA+0x2160): six 3-vectors,
// then 14018beb0
void movement_copy(std::byte* dst, const std::byte* src)
{
    for (std::int32_t k = 0; k < 3; k = k + 1) {
        const std::int64_t o = static_cast<std::int64_t>(k) * 8;
        copy64(dst, src, 0x50 + o);
        copy64(dst, src, 0x170 + o);
        copy64(dst, src, 0x188 + o);
        copy64(dst, src, 0x1a0 + o);
        copy64(dst, src, 0x630 + o);
        copy64(dst, src, 0x648 + o);
    }
    movement_copy_body(dst, src);
}

// @src 14018beb0  movement sub-object copy, the rest: strcpy of the name at +0 (140484f90), the
// scalars and the 16 DYNARY curves (14012eff0), in the binary's order
void movement_copy_body(std::byte* dst, const std::byte* src)
{
    std::strcpy(reinterpret_cast<char*>(dst), reinterpret_cast<const char*>(src));  // 140484f90
    copy32(dst, src, 0x68);
    copy64(dst, src, 0x1e8);
    copy64(dst, src, 0x1f0);
    copy64(dst, src, 0x1f8);
    copy32(dst, src, 0x200);
    dynary_copy(dy(dst + 0x208), dy(src + 0x208));
    copy64(dst, src, 0x280);
    copy64(dst, src, 0x288);
    copy64(dst, src, 0x290);
    copy64(dst, src, 0x298);
    dynary_copy(dy(dst + 0x2a0), dy(src + 0x2a0));
    copy64(dst, src, 0x318);
    copy64(dst, src, 0x320);
    copy64(dst, src, 0x328);
    copy8(dst, src, 0x330);
    dynary_copy(dy(dst + 0x338), dy(src + 0x338));
    dynary_copy(dy(dst + 0x3b0), dy(src + 0x3b0));
    copy64(dst, src, 0x428);
    copy64(dst, src, 0x430);
    copy64(dst, src, 0x438);
    copy32(dst, src, 0x440);
    copy32(dst, src, 0x444);
    dynary_copy(dy(dst + 0x448), dy(src + 0x448));
    copy32(dst, src, 0x4c4);
    copy32(dst, src, 0x4c0);
    for (std::int32_t k = 0; k < 3; k = k + 1) {
        const std::int64_t o = static_cast<std::int64_t>(k) * 8;
        copy64(dst, src, 0x4c8 + o);
        copy64(dst, src, 0x4e0 + o);
        copy64(dst, src, 0x4f8 + o);
    }
    copy32(dst, src, 0x510);
    copy32(dst, src, 0x514);
    copy32(dst, src, 0x620);
    copy32(dst, src, 0x624);
    copy32(dst, src, 0x628);
    copy64(dst, src, 0x750);
    dynary_copy(dy(dst + 0x70), dy(src + 0x70));
    dynary_copy(dy(dst + 0xe8), dy(src + 0xe8));
    dynary_copy(dy(dst + 0x590), dy(src + 0x590));
    dynary_copy(dy(dst + 0x518), dy(src + 0x518));
    copy64(dst, src, 0x608);
    copy64(dst, src, 0x610);
    copy64(dst, src, 0x618);
    copy32(dst, src, 0x160);
    copy64(dst, src, 0x168);
    dynary_copy(dy(dst + 0x660), dy(src + 0x660));
    dynary_copy(dy(dst + 0x6d8), dy(src + 0x6d8));
    copy8(dst, src, 0x8b8);
    dynary_copy(dy(dst + 0x8c0), dy(src + 0x8c0));
    copy64(dst, src, 0x938);
    copy64(dst, src, 0x940);
    copy64(dst, src, 0x948);
    copy64(dst, src, 0x950);
    copy64(dst, src, 0x958);
    for (std::int32_t k = 0; k < 2; k = k + 1) {
        const std::int64_t o = static_cast<std::int64_t>(k) * 8;
        copy64(dst, src, 0x1b8 + o);
        copy64(dst, src, 0x1c8 + o);
        copy64(dst, src, 0x1d8 + o);
    }
    copy32(dst, src, 0x960);
    dynary_copy(dy(dst + 0x968), dy(src + 0x968));
    dynary_copy(dy(dst + 0x9e0), dy(src + 0x9e0));
    copy64(dst, src, 0xa58);
    copy64(dst, src, 0xa60);
    copy64(dst, src, 0xa68);
    copy64(dst, src, 0xa70);
    copy32(dst, src, 0x758);
    copy64(dst, src, 0x880);
    dynary_copy(dy(dst + 0x790), dy(src + 0x790));
    dynary_copy(dy(dst + 0x808), dy(src + 0x808));
    for (std::int32_t k = 0; k < 3; k = k + 1) {
        const std::int64_t o = static_cast<std::int64_t>(k) * 8;
        copy64(dst, src, 0x760 + o);
        copy64(dst, src, 0x778 + o);
    }
    for (std::int32_t k = 0; k < 3; k = k + 1) {
        const std::int64_t o = static_cast<std::int64_t>(k) * 8;
        copy64(dst, src, 0x888 + o);
        copy64(dst, src, 0x8a0 + o);
    }
    copy8(dst, src, 0xa78);
}

// @src 140189b70  mesh-generation sub-object copy (Cls_140189440, LLOBJ_DATA+0x2d10; owner at +0):
// reset (140189ad0), scalars, the window list +0x40 when *shapes, the 2D (+0x70) or 3D
// (+0x78..+0x98) fields when *by_dim, then the two arrays of +0xd8 entries (14018af80)
void meshgen_copy(std::byte* dst, const std::byte* src, const char* shapes, const char* by_dim)
{
    reinterpret_cast<Cls_140189440*>(dst)->reset();  // 140189ad0
    copy64(dst, src, 0x10);
    copy64(dst, src, 0x18);
    copy64(dst, src, 0x20);
    copy64(dst, src, 0x28);
    copy64(dst, src, 0x30);
    copy64(dst, src, 0x38);
    if (*shapes != '\0') shape_list_copy(dst + 0x40, src + 0x40);
    if (*by_dim != '\0') {
        if (!rt::dimension_is_2(at<void*>(dst, 0))) {
            if (rt::dimension_is_3(at<void*>(dst, 0))) {
                copy32(dst, src, 0x78);
                copy32(dst, src, 0x7c);
                copy32(dst, src, 0x80);
                copy32(dst, src, 0x84);
                copy64(dst, src, 0x98);
                copy64(dst, src, 0x88);
                copy32(dst, src, 0x90);
            }
        } else {
            copy32(dst, src, 0x70);
        }
    }
    copy64(dst, src, 0xa0);
    copy64(dst, src, 0xa8);
    copy32(dst, src, 0xb0);
    copy32(dst, src, 0xb4);
    copy32(dst, src, 0x74);
    copy64(dst, src, 0xb8);
    copy64(dst, src, 0xc0);
    copy64(dst, src, 0xc8);
    copy64(dst, src, 0xd0);
    meshgen_alloc(dst, at<std::int32_t>(src, 0xd8));
    std::memcpy(at<void*>(dst, 0xe0), at<void*>(src, 0xe0),
                static_cast<std::size_t>(static_cast<std::int64_t>(at<std::int32_t>(dst, 0xd8)) << 3));
    std::memcpy(at<void*>(dst, 0xe8), at<void*>(src, 0xe8),
                static_cast<std::size_t>(static_cast<std::int64_t>(at<std::int32_t>(dst, 0xd8)) << 2));
    copy32(dst, src, 0xf0);
}

// @src 14018af80  +0xd8 = n; free +0xe0/+0xe8, then calloc(n, 8) and calloc(n, 4)
void meshgen_alloc(std::byte* p, std::int32_t n)
{
    at<std::int32_t>(p, 0xd8) = n;
    if (at<void*>(p, 0xe0) != nullptr) std::free(at<void*>(p, 0xe0));
    at<void*>(p, 0xe0) = nullptr;
    if (at<void*>(p, 0xe8) != nullptr) std::free(at<void*>(p, 0xe8));
    at<void*>(p, 0xe8) = nullptr;
    at<void*>(p, 0xe0) = std::calloc(static_cast<std::size_t>(static_cast<std::int64_t>(at<std::int32_t>(p, 0xd8))), 8);
    at<void*>(p, 0xe8) = std::calloc(static_cast<std::size_t>(static_cast<std::int64_t>(at<std::int32_t>(p, 0xd8))), 4);
}

// @src 140195c80  boundary-condition sub-object copy (Cls_1401956d0, LLOBJ_DATA+0x2e68): reset
// (140195a40), the seven window lists, the DYNARY list at 0 (140136560), the list at +0x18
// (140257c80) for a 3D owner (+0x198)
void bc_copy(std::byte* dst, const std::byte* src)
{
    reinterpret_cast<Cls_1401956d0*>(dst)->reset();  // 140195a40
    shape_list_copy(dst + 0x48, src + 0x48);
    shape_list_copy(dst + 0x78, src + 0x78);
    shape_list_copy(dst + 0xa8, src + 0xa8);
    shape_list_copy(dst + 0x168, src + 0x168);
    shape_list_copy(dst + 0xd8, src + 0xd8);
    shape_list_copy(dst + 0x108, src + 0x108);
    shape_list_copy(dst + 0x138, src + 0x138);
    dynary_list_copy(reinterpret_cast<Cls_1401364a0*>(dst), reinterpret_cast<const Cls_1401364a0*>(src));
    if (rt::dimension_is_3(at<void*>(dst, 0x198))) ext::f140257c80(dst + 0x18, src + 0x18);
}

// @src 140136560  DYNARY list copy: clear (140136910), then for each node of src a new node
// (140136670) copied from it (14012eff0) with its link +0x60 cleared
void dynary_list_copy(Cls_1401364a0* dst, const Cls_1401364a0* src)
{
    dst->clear();
    for (std::int32_t k = 0; k < src->size(); k = k + 1) {
        DYNARY* n = dst->add();
        DYNARY* s = src->at(k + 1);
        dynary_copy(n, s);
        llobj::dynary_set_next(n, nullptr);  // *(n + 0x60) = 0
    }
}

} // namespace def::model::llobj_copy
