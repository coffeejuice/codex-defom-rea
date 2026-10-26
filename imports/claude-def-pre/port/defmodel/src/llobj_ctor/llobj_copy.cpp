// LLOBJ_DATA "copy from another object" (FUN_1401613a0) and its LLOBJ_DATA-level helpers.
// Decompile: deform-rea-work/out/g2obj/copy_closure.c. Offsets as in the decompile.
#include <defmodel/bc/boundary_topology.hpp>
#include <defmodel/layout/obj_geo3d.hpp>
#include <defmodel/llobj_ctor/llobj_copy.hpp>

#include <cstring>

#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/llobj/cls_140182880.hpp>
#include <defmodel/llobj/cls_14017ef20.hpp>
#include <defmodel/llobj_ctor/obj_defaults.hpp>
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
std::byte* b(LLOBJ_DATA* o) { return reinterpret_cast<std::byte*>(o); }
const std::byte* b(const LLOBJ_DATA* o) { return reinterpret_cast<const std::byte*>(o); }
} // namespace

// @src 140158f70  clear the object without deleting the geometry objects: 14015fbf0(obj, &0)
void reset_keep_geometry(LLOBJ_DATA* obj)
{
    char full[24];
    full[0] = 0;
    obj->reset(full);
}

// @src 140161840  3D owner: copy the 100 geometry slots (1401618b0); 2D owner: the object at
// +0x3998 (1401619c0)
void copy_geometry(LLOBJ_DATA* dst, const LLOBJ_DATA* src)
{
    const void* owner = at<void*>(dst, 0x3a98);  // *(this + 15000)
    if (!rt::dimension_is_3(owner)) {
        if (rt::dimension_is_2(owner)) copy_u3998(dst, src);
    } else {
        copy_geo3d_slots(dst, src);
    }
}

// @src 1401618b0  reset each set geometry slot, copy +0x3990 and +0x3994, then let each set slot
// copy from src's slot (virtual slot 4 of the geometry object)
void copy_geo3d_slots(LLOBJ_DATA* dst, const LLOBJ_DATA* src)
{
    std::int32_t i[6];
    for (i[0] = 0; i[0] < 100; i[0] = i[0] + 1) {
        if (at<void*>(dst, 0x3c00 + static_cast<std::int64_t>(i[0]) * 8) != nullptr)
            llobj::ext::f1402f9340(at<void*>(dst, 0x3c00 + static_cast<std::int64_t>(i[0]) * 8));
    }
    const std::uint8_t f = get_u3990(src);
    at<std::uint8_t>(dst, 0x3990) = f;
    const std::int32_t v = get_u3994(src);
    set_u3994(dst, v);
    for (i[0] = 0; i[0] < 100; i[0] = i[0] + 1) {
        if (at<void*>(dst, 0x3c00 + static_cast<std::int64_t>(i[0]) * 8) != nullptr) {
            void* g = geo_slot(src, i);
            ext::geo3d_slot4(at<void*>(dst, 0x3c00 + static_cast<std::int64_t>(i[0]) * 8), g);
        }
    }
}

// @src 1401619c0  copy the object at +0x3998 when dst has one (1403019d0)
void copy_u3998(LLOBJ_DATA* dst, const LLOBJ_DATA* src)
{
    if (at<void*>(dst, 0x3998) != nullptr) sv_list_copy(at<void*>(dst, 0x3998), at<void*>(src, 0x3998));
}

// @src 140161ac0  +0x3aa0, +0x3aa4, byte +0x3aac, 20 qwords from +0x3ab0
void copy_u3aa0(LLOBJ_DATA* dst, const LLOBJ_DATA* src)
{
    at<std::int32_t>(dst, 0x3aa0) = at<std::int32_t>(src, 0x3aa0);
    at<std::int32_t>(dst, 0x3aa4) = at<std::int32_t>(src, 0x3aa4);
    at<std::uint8_t>(dst, 0x3aac) = at<std::uint8_t>(src, 0x3aac);
    for (std::int32_t k = 0; k < 0x14; k = k + 1)
        at<std::uint64_t>(dst, 0x3ab0 + static_cast<std::int64_t>(k) * 8) =
            at<std::uint64_t>(src, 0x3ab0 + static_cast<std::int64_t>(k) * 8);
}

// @src 140163300  layer count +0xbd8 = min(n, 100)
void set_layer_count(LLOBJ_DATA* obj, std::int32_t n)
{
    std::int32_t v = n;
    if (100 < n) v = 100;
    at<std::int32_t>(obj, 0xbd8) = v;
}

// 140165170 / 140165d30: kw-g2bc's boundary topology (defmodel/bc/boundary_topology.hpp)
std::int32_t node_array_size(LLOBJ_DATA* obj) { return bc::edge_face_slot_count(*obj); }
void sum_layer_counts(LLOBJ_DATA* obj) { bc::recount_boundary_segments(*obj); }

// @src 140160d90  byte +0x3990
std::uint8_t get_u3990(const LLOBJ_DATA* obj) { return at<std::uint8_t>(obj, 0x3990); }
// @src 1401985c0  int +0x3994
std::int32_t get_u3994(const LLOBJ_DATA* obj) { return at<std::int32_t>(obj, 0x3994); }
// @src 140198600  int +0x3994 = v
void set_u3994(LLOBJ_DATA* obj, std::int32_t v) { at<std::int32_t>(obj, 0x3994) = v; }
// @src 140198650  geometry slot +0x3c00 + 8 * *i
void* geo_slot(const LLOBJ_DATA* obj, const std::int32_t* i)
{
    return at<void*>(obj, 0x3c00 + static_cast<std::int64_t>(*i) * 8);
}

// @src 140199c20  thunk: _OBJ_GEO3D copy 1402f9b60 (p1-07)
void geo3d_assign(void* dst, const void* src) { ext::f1402f9b60(dst, src); }

namespace ext {
void geo3d_slot4(void* obj, void* arg)
{
    if (llobj::ext::g_hook) {
        call(0x4, u(obj), u(arg));  // pseudo address 0x4: virtual slot 4 (vtable +0x20)
        return;
    }
    // slot 4 of OBJ_GEO3D is copy_from (1402f9dc0 -> 1402f9b60, p1-07)
    static_cast<OBJ_GEO3D*>(obj)->copy_from(static_cast<OBJ_GEO_BASE*>(arg));
}
} // namespace ext

} // namespace def::model::llobj_copy

namespace def::model {

// @src 1401613a0  copy src into dst (cleared first by 140158f70)
void llobj_copy_from(LLOBJ_DATA* dst, const LLOBJ_DATA* src, const std::int32_t* sizes,
                     const bool* p4, const bool* p5, const bool* p6, const bool* p7,
                     const bool* p8, const bool* p9)
{
    using namespace llobj_copy;
    std::byte* d = reinterpret_cast<std::byte*>(dst);
    const std::byte* s = reinterpret_cast<const std::byte*>(src);
    auto i32 = [](const std::byte* p, std::size_t off) -> std::int32_t {
        return *reinterpret_cast<const std::int32_t*>(p + off);
    };
    auto set32 = [](std::byte* p, std::size_t off, std::int32_t v) {
        *reinterpret_cast<std::int32_t*>(p + off) = v;
    };

    reset_keep_geometry(dst);
    if (src == nullptr) return;
    if (*p8) set32(d, 0x8, i32(s, 0x8));
    set32(d, 0xc, i32(s, 0xc));
    if (*p7) std::strncpy(reinterpret_cast<char*>(d + 0x828), reinterpret_cast<const char*>(s + 0x828), 0x40);
    set32(d, 0x87c, i32(s, 0x87c));
    set32(d, 0x880, i32(s, 0x880));
    set32(d, 0x884, i32(s, 0x884));
    set32(d, 0x888, i32(s, 0x888));
    set32(d, 0x88c, i32(s, 0x88c));
    set32(d, 0x3984, i32(s, 0x3984));
    set32(d, 0x3988, i32(s, 0x3988));
    set32(d, 0x3588, i32(s, 0x3588));
    set32(d, 0x358c, i32(s, 0x358c));
    set32(d, 0x398c, i32(s, 0x398c));
    set32(d, 0x3980, i32(s, 0x3980));
    llobj_ctor::set_element_kind(d, i32(s, 0x890));  // 140165500
    copy_geometry(dst, src);                          // 140161840
    geo3d_assign(d + 0x3328, s + 0x3328);             // 140199c20
    geo3d_assign(d + 0x3458, s + 0x3458);
    set_layer_count(dst, i32(s, 0xbd8));              // 140163300
    for (std::int32_t k = 0; k < i32(d, 0xbd8); k = k + 1) {
        layer_copy(reinterpret_cast<std::int32_t*>(d + 0xbe0 + static_cast<std::int64_t>(k) * 0x30),
                   reinterpret_cast<const std::int32_t*>(s + 0xbe0 + static_cast<std::int64_t>(k) * 0x30));
    }
    // 140180390
    reinterpret_cast<Cls_14017ef20*>(d + 0x8a0)->copy_from(*reinterpret_cast<const Cls_14017ef20*>(s + 0x8a0),
                                                         i32(d, 0x880), sizes, *p5);
    // 140183900
    reinterpret_cast<Cls_140182880*>(d + 0x9e0)->copy_from(*reinterpret_cast<const Cls_140182880*>(s + 0x9e0),
                                                         i32(d, 0x884), i32(d, 0x894), sizes, *p4, *p6, *p9);
    const std::int32_t n = node_array_size(dst);      // 140165170
    arrays_copy(reinterpret_cast<void**>(d + 0xae8), reinterpret_cast<void* const*>(s + 0xae8), n); // 140185a10
    props_copy(d + 0x1ea0, s + 0x1ea0);               // 140186590
    movement_copy(d + 0x2160, s + 0x2160);            // 14018bd30
    char by_dim = 1;
    char shapes = 1;
    meshgen_copy(d + 0x2d10, s + 0x2d10, &shapes, &by_dim);  // 140189b70(dst, src, &local_13, &local_14)
    bc_copy(d + 0x2e68, s + 0x2e68);                  // 140195c80
    copy_u3998(dst, src);                             // 1401619c0
    copy_u3aa0(dst, src);                             // 140161ac0
}

} // namespace def::model
