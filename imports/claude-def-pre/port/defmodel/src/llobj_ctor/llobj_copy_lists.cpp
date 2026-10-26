// List copies used by 1401613a0: the window lists (Cls_140269250 of LLWINDEF_DATA records), the
// 0xe8-byte record list and the SfLinkedList of the property sub-object, and the std::list
// object at LLOBJ_DATA+0x3998. Decompiles: deform-rea-work/out/g2obj/copy_deep.c, copy_deep2.c.
#include <defmodel/mesh_gen/sf_list_ops.hpp>
#include <defmodel/llobj_ctor/llobj_copy.hpp>

#include <cstdlib>
#include <cstring>
#include <new>

#include <defmodel/dynary_init.hpp>
#include <defmodel/layout/dynary.hpp>
#include <defmodel/layout/sf_linked_data.hpp>
#include <defmodel/layout/sf_linked_list.hpp>
#include <defmodel/window/window_list.hpp>
#include <defrt/rdata.hpp>

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

// rdata 14051f210  360.0 (bytes 00 00 00 00 00 80 76 40)
constexpr double k360 = def::rt::rdata::d_14051f210;
} // namespace

// ---- Cls_140269250: list of LLWINDEF_DATA ------------------------------------------------------

// 140269340 (window list copy) and the LLWINDEF_DATA record functions it uses are kw-g3's
// (defmodel/window/window_list.hpp, llwindef_data.hpp); this forwards to them.
void shape_list_copy(std::byte* dst, const std::byte* src)
{
    winlist_copy(reinterpret_cast<Cls_140269250*>(dst), reinterpret_cast<const Cls_140269250*>(src));
}

// @src 1402f9360  the _OBJ_GEO3D has points (1400034d0) and facets (140003500)
bool geo3d_has_mesh(const void* geo3d)
{
    if (0 < ext::f1400034d0(geo3d) && 0 < ext::f140003500(geo3d)) return true;
    return false;
}

// ---- list of 0xe8-byte records (Cls_1401860a0+0x200) -------------------------------------------

// @src 1402598c0  record list copy: reset (140259c70), then per src record a new one
// (140259950(dst, 0)) copied from the n-th (140259b60, 140258750) with its link +0xe0 cleared
void axis_list_copy(std::byte* dst, const std::byte* src)
{
    llobj::ext::f140259c70(dst);
    for (std::int32_t k = 0; k < axis_list_count(src); k = k + 1) {
        std::int32_t* r = axis_list_add(dst, 0);
        const std::byte* s = axis_list_at(src, k + 1);
        axis_record_copy(reinterpret_cast<std::byte*>(r), s);
        at<std::uint64_t>(r, 0xe0) = 0;
    }
}

// @src 14019a550  count (+0x10)
std::int32_t axis_list_count(const std::byte* list) { return at<std::int32_t>(list, 0x10); }

// @src 140259950  append a new record (operator new(0xe8), 140258580); its int +0 = no, or the
// new count when no is 0
std::int32_t* axis_list_add(std::byte* list, std::int32_t no)
{
    void* mem = ::operator new(0xe8);  // 1404797d8
    std::int32_t* r;
    if (mem == nullptr)
        r = nullptr;
    else
        r = reinterpret_cast<std::int32_t*>(axis_record_construct(static_cast<std::byte*>(mem)));
    if (at<std::int32_t>(list, 0x10) == 0) {
        at<std::int32_t*>(list, 8) = r;
        at<std::int32_t*>(list, 0) = r;
    } else {
        at<std::int32_t*>(at<std::byte*>(list, 8), 0xe0) = r;
        at<std::int32_t*>(list, 8) = r;
    }
    at<std::int32_t>(list, 0x10) = at<std::int32_t>(list, 0x10) + 1;
    std::int32_t v = no;
    if (no == 0) v = at<std::int32_t>(list, 0x10);
    *r = v;
    return r;
}

// @src 140258580  ctor: 1402585b0, returns this
std::byte* axis_record_construct(std::byte* r)
{
    axis_record_init(r);
    return r;
}

// @src 1402585b0  record defaults: all zero except the double +0xb8 = 360.0 (DAT_14051f210)
void axis_record_init(std::byte* r)
{
    std::int32_t* p = reinterpret_cast<std::int32_t*>(r);
    p[0] = 0;
    p[1] = 0;
    for (std::int32_t k = 0; k < 3; k = k + 1) {
        at<std::uint64_t>(p, static_cast<std::int64_t>(k) * 8 + 8) = 0;
        at<std::uint64_t>(p, static_cast<std::int64_t>(k) * 8 + 0x20) = 0;
    }
    p[0xe] = 0;
    at<std::uint64_t>(p, 0x40) = 0;
    at<std::uint64_t>(p, 0x48) = 0;
    at<std::uint64_t>(p, 0x50) = 0;
    at<std::uint64_t>(p, 0x58) = 0;
    p[0x18] = 0;
    for (std::int32_t k = 0; k < 3; k = k + 1) {
        at<std::uint64_t>(p, static_cast<std::int64_t>(k) * 8 + 0x68) = 0;
        at<std::uint64_t>(p, static_cast<std::int64_t>(k) * 8 + 0x80) = 0;
        at<std::uint64_t>(p, static_cast<std::int64_t>(k) * 8 + 0x98) = 0;
    }
    p[0x2c] = 0;
    at<double>(p, 0xb8) = k360;
    at<std::uint64_t>(p, 0xc0) = 0;
    p[0x32] = 0;
    p[0x33] = 0;
    at<std::uint64_t>(p, 0xd0) = 0;
    at<std::uint64_t>(p, 0xd8) = 0;
    at<std::uint64_t>(p, 0xe0) = 0;
}

// @src 140258750  record copy (every field except the link +0xe0)
void axis_record_copy(std::byte* dst, const std::byte* src)
{
    copy32(dst, src, 0x0);
    copy32(dst, src, 0x4);
    for (std::int32_t k = 0; k < 3; k = k + 1) {
        copy64(dst, src, static_cast<std::int64_t>(k) * 8 + 8);
        copy64(dst, src, static_cast<std::int64_t>(k) * 8 + 0x20);
    }
    copy32(dst, src, 0x38);
    copy64(dst, src, 0x40);
    copy64(dst, src, 0x48);
    copy64(dst, src, 0x50);
    copy64(dst, src, 0x58);
    copy32(dst, src, 0x60);
    for (std::int32_t k = 0; k < 3; k = k + 1) {
        copy64(dst, src, static_cast<std::int64_t>(k) * 8 + 0x68);
        copy64(dst, src, static_cast<std::int64_t>(k) * 8 + 0x80);
        copy64(dst, src, static_cast<std::int64_t>(k) * 8 + 0x98);
    }
    copy32(dst, src, 0xb0);
    copy64(dst, src, 0xb8);
    copy64(dst, src, 0xc0);
    copy32(dst, src, 0xc8);
    copy32(dst, src, 0xcc);
    copy64(dst, src, 0xd0);
    copy64(dst, src, 0xd8);
}

// @src 140259b60  n-th record (1-based; the head for n <= 1)
std::byte* axis_list_at(const std::byte* list, std::int32_t n)
{
    std::int32_t k = 0;
    std::byte* r = at<std::byte*>(list, 0);
    while (r != nullptr && (k = k + 1, k < n)) r = at<std::byte*>(r, 0xe0);
    return r;
}

// ---- SfLinkedList (Cls_1401860a0+0x218) --------------------------------------------------------

// @src 14025a730  list copy: clear (14025ab20), then per src node a new node (14025a7d0) that
// copies the n-th src node through its virtual slot 1, link cleared
void sf_list_copy(SfLinkedList* dst, const SfLinkedList* src)
{
    llobj::ext::f14025ab20(dst);
    for (std::int32_t k = 0; k < def::model::sf_list_count(src); k = k + 1) {
        SfLinkedData* r = sf_list_add(dst);
        SfLinkedData* s = sf_list_at(src, k + 1);
        r->copy_from(s);  // (**(code **)(*r + 8))(r, s)
        r->next = nullptr;
    }
}

// 14019a580: kw-g2mg's def::model::sf_list_count (mesh_gen/sf_list_ops.hpp)
std::int32_t sf_list_count(const SfLinkedList* list) { return def::model::sf_list_count(list); }

// @src 14025a7d0  new node from the list's virtual slot 1, linked at the tail
SfLinkedData* sf_list_add(SfLinkedList* list)
{
    SfLinkedData* r = list->create_item();
    if (list->count == 0) {
        list->tail = r;
        list->head = r;
    } else {
        list->tail->next = r;
        list->tail = r;
    }
    list->count = list->count + 1;
    return r;
}

// @src 14025a9e0  n-th node (1-based; the head for n <= 1)
SfLinkedData* sf_list_at(const SfLinkedList* list, std::int32_t n)
{
    std::int32_t k = 0;
    SfLinkedData* r = list->head;
    while (r != nullptr && (k = k + 1, k < n)) r = r->next;
    return r;
}

// ---- the std::list object at LLOBJ_DATA+0x3998 (body: +8 head node, +0x10 size; node +0 next) --

// @src 1403019d0  copy: clear dst (140301d10), add as many elements as src has (140301ad0), then
// walk dst and copy element k of src into each (140280490)
void sv_list_copy(void* dst, const void* src)
{
    if (src != nullptr) {
        llobj::ext::f140301d10(dst);
        const std::int32_t n = sv_list_size(src);
        for (std::int32_t k = 0; k < n; k = k + 1) llobj::ext::f140301ad0(dst);
        std::uint64_t it;
        sv_iter_null(&it);
        std::int32_t k = 0;
        std::uint64_t first;
        it = *sv_list_begin(dst, &first);
        while (true) {
            std::uint64_t last;
            const std::uint64_t* e = ext::f140301e30(dst, &last);
            if (!sv_iter_ne(&it, e)) break;
            const std::uint64_t v = sv_list_value_at(src, k);
            const std::uint64_t d = ext::f140302020(&it);
            ext::f140280490(d, v);
            k = k + 1;
            std::uint64_t old;
            ext::f140302040(&it, &old, 0);
        }
    }
}

// @src 140301c60  size (140301e70), as int
std::int32_t sv_list_size(const void* list) { return static_cast<std::int32_t>(sv_list_size_t(list)); }

// @src 140301e70  size: body +0x10
std::uint64_t sv_list_size_t(const void* list) { return at<std::uint64_t>(at<void*>(list, 0), 0x10); }

// @src 140301fa0  iterator ctor: 0
std::uint64_t* sv_iter_null(std::uint64_t* it)
{
    *it = 0;
    return it;
}

// @src 140301df0  begin: 140302090, iterator at the head's next node (1403020c0)
std::uint64_t* sv_list_begin(const void* list, std::uint64_t* out)
{
    ext::f140302090(list);
    ext::f1403020c0(out, at<void*>(at<void*>(at<void*>(list, 0), 8), 0));
    return out;
}

// @src 140301b40  element i (0 <= i <= size, size != 0), else 0
std::uint64_t sv_list_value_at(const void* list, std::int32_t i)
{
    std::int32_t n = sv_list_size(list);
    if (n != 0 && -1 < i && (n = sv_list_size(list), i <= n)) {
        std::uint64_t tmp[3];
        sv_list_iter_at(list, tmp, static_cast<std::uint64_t>(static_cast<std::int64_t>(i)));
        return ext::f140302020(tmp);
    }
    return 0;
}

// @src 140301f50  iterator at node i: 140302090, node (1403025c0), 1403020c0
std::uint64_t* sv_list_iter_at(const void* list, std::uint64_t* out, std::uint64_t i)
{
    ext::f140302090(list);
    void* node = sv_list_node_at(at<void*>(list, 0), i);
    ext::f1403020c0(out, node);
    return out;
}

// @src 1403025c0  node i: the head's next, followed i times
void* sv_list_node_at(const void* body, std::uint64_t i)
{
    void* node = at<void*>(at<void*>(body, 8), 0);
    for (std::uint64_t k = 0; k < i; k = k + 1) node = at<void*>(node, 0);
    return node;
}

// iterator != as FUN_140301fe0 (p1-09) does it, local copy (compares the node pointers; identical-code-folded with
// ATL::CFileTimeSpan::operator!=)
bool sv_iter_ne(const std::uint64_t* a, const std::uint64_t* b) { return *a != *b; }


} // namespace def::model::llobj_copy
