// Phase 4 gate ports (defdbio/gendb/gate_ports.hpp). Decompiles: deform-rea-work/out/p4g3d_new.c
// (1401d2bb0 from the listing: both paths load AL = 1).
#include <defdbio/gendb/gate_ports.hpp>

#include <cstdint>
#include <cstdlib>
#include <cstring>

#include <defmodel/geo_kw/misc_model.hpp>
#include <defmodel/geo_kw/surface_model.hpp>
#include <defmodel/layout/obj_geo3d.hpp>
#include <defmodel/mesh_gen/sf_list_ops.hpp>
#include <defrt/port.hpp>
#include <defrt/stdlib/refcount.hpp>

#include "raw.hpp"

namespace def::dbio::gendb {

using model::DYNARY;
using model::OBJ_GEO3D;
using raw::at;

namespace {
std::size_t size_of_i32(std::int32_t v) { return static_cast<std::size_t>(static_cast<std::int64_t>(v)); }
std::int32_t shl2(std::int32_t v) { return static_cast<std::int32_t>(static_cast<std::uint32_t>(v) << 2); }
} // namespace

// @src 140130200  type 0x7c: ncomp (+0x40), else n[0] (+0x1c) << 1
std::int32_t dynary_words_1d(const DYNARY* t)
{
    std::int32_t r;
    if (t->type == 0x7c)
        r = t->ncomp;
    else
        r = static_cast<std::int32_t>(static_cast<std::uint32_t>(t->n[0]) << 1);
    return r;
}

// @src 140130160
std::int32_t dynary_words_2d(const DYNARY* t)
{
    std::int32_t a, b, c;
    if (t->n[0] < 1)
        a = 1;
    else
        a = t->n[0];
    if (t->n[1] < 1)
        b = 1;
    else
        b = t->n[1];
    if (t->ncomp < 1)
        c = 1;
    else
        c = t->ncomp;
    return t->n[0] + t->n[1] + a * b * c;
}

// @src 14012fff0
std::int32_t dynary_words_3d(const DYNARY* t)
{
    std::int32_t a, b, c;
    if (t->n[0] < 1)
        a = 1;
    else
        a = t->n[0];
    if (t->n[1] < 1)
        b = 1;
    else
        b = t->n[1];
    if (t->n[2] < 1)
        c = 1;
    else
        c = t->n[2];
    return t->n[0] + t->n[1] + t->n[2] + a * b * c;
}

// @src 140130290
std::int32_t dynary_words(const DYNARY* t)
{
    std::int32_t r = 0;
    if (t->ndim == 0) {
        if (t->type == 0xa7 || t->type == 0xa8 || t->type == 0xaa || t->type == 0xab || t->type == 0xad
            || t->type == 0xb3 || t->type == 0xb5)
            r = t->ncomp;
        else
            r = 0;
    } else if (t->ndim == 1) {
        r = dynary_words_1d(t);
    } else if (t->ndim == 2) {
        r = dynary_words_2d(t);
    } else if (t->ndim == 3) {
        r = dynary_words_3d(t);
    }
    return r;
}

// @src 14019a530  14019a550(self + 0x200)
std::int32_t body_force_list_count(void* self)
{
    return model::geo_kw::wpaxis_count(*static_cast<model::linked::Cls_140258580List*>(raw::ptr(self, 0x200)));
}

// @src 14019a560  14019a580(self + 0x218)
std::int32_t list218_count(void* self)
{
    return model::sf_list_count(static_cast<const model::SfLinkedList*>(raw::ptr(self, 0x218)));
}

// @src 14025c830  the low byte of the argument is cleared when +0x888 is null (returns false)
bool window_owner_is_2d(void* w)
{
    if (at<void*>(w, 0x888) == nullptr) return false;
    return def::rt::dimension_is_2(at<void*>(w, 0x888));  // 1400015e0
}

// @src 140194d40
std::uint64_t movement_frame(const void* mv, double* frame)
{
    frame[0] = at<double>(mv, 0x4e0);
    frame[3] = at<double>(mv, 0x4e8);
    frame[6] = at<double>(mv, 0x4f0);
    frame[1] = at<double>(mv, 0x4f8);
    frame[4] = at<double>(mv, 0x500);
    frame[7] = at<double>(mv, 0x508);
    frame[2] = at<double>(mv, 0x4c8);
    frame[5] = at<double>(mv, 0x4d0);
    frame[8] = at<double>(mv, 0x4d8);
    return 1;
}

// @src 1401d2bb0  1401d2bba tests the byte at +0x1b59; 1401d2bc5 and 1401d2bc9 both set AL = 1
bool step_fun_1401d2bb0(const model::DB_STEP_DATA* m)
{
    if (at<std::uint8_t>(m, 0x1b59) == 0) return true;
    return true;
}

// @src 1401d5820  the reads below 1401d2bb0 == 0 are unreachable in this build (1401d2bb0
// always returns 1): MAR groups 0..3 (1401cf040 with 0..3), 4 / 5 (1401d5620), the cached arrays
// +0x2248 / +0x2250 / +0x2258 (1401cf040 with 0xd / 0xe / 0xf), group 6 (1401cf040 with 7..0x16).
void* step_read_mar_on_demand(model::DB_STEP_DATA* m, const std::int32_t* what)
{
    void* r = nullptr;
    if (!step_fun_1401d2bb0(m)) {
        (void)what;
        DEF_NOT_PORTED(0x1401cf040);  // unreachable: see above
    } else {
        r = nullptr;
    }
    return r;
}

// @src 1402f97c0  14019a740(+0x10, *n * 3, 0)
void geo3d_resize_coords(OBJ_GEO3D* g, const std::int32_t* n)
{
    double zero = 0.0;
    g->coords.resize(size_of_i32(static_cast<std::int32_t>(static_cast<std::uint32_t>(*n) * 3u)), zero);
}

// @src 1402f9810  old element count first (140003500); the flags are reallocated by 1402f9970
// and the old ones copied (min(old, new) ints), the tail zeroed, the old block freed. When the
// new count is < 1, 1402f9970 returns 0, +0x20 becomes 0 and the old block is not freed.
void geo3d_resize_elements(OBJ_GEO3D* g, const std::int32_t* n)
{
    const std::int32_t old = g->elem_count();
    std::int32_t zero = 0;
    g->elems.resize(size_of_i32(shl2(*n)), zero);  // 140159b60
    void* src = g->elem_flags;
    g->elem_flags = g->alloc_elem_flags();  // 1402f9970
    if (g->elem_flags != nullptr) {
        std::int32_t k = old;
        if (*n < old) k = *n;
        std::memcpy(g->elem_flags, src, size_of_i32(shl2(k)));
        if (old < *n)
            std::memset(g->elem_flags + old, 0, size_of_i32(shl2(*n - old)));  // 140478d30
        std::free(src);
    }
    std::int32_t zero2 = 0;
    g->elem_ids.resize(size_of_i32(*n), zero2);  // 140159b60
}

// @src 1402f9780
void geo3d_resize(OBJ_GEO3D* g, const std::int32_t* nodes, const std::int32_t* elems)
{
    geo3d_resize_coords(g, nodes);
    geo3d_resize_elements(g, elems);
}

// @src 1402fcfe0  then 140159ab0(+0x40): the list size
std::int32_t geo3d_surface_patch_count(OBJ_GEO3D* g, const char* rebuild)
{
    if (*rebuild != '\0') model::geo_kw::geo3d_rebuild_patches(g);  // 1402fced0
    return model::geo_kw::geo3d_patch_count(g);
}

} // namespace def::dbio::gendb
