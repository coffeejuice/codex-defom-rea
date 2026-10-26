// geolib: float allocation wrappers, the float point list and the surface checker's setup
// (ctor, dtor, node links) (packet p8-09). Spec: docs/re/commands/GFREAD_STL_GEO_NAS.md 2.6/2.7.
// Decompile / listing: deform-rea-work/out/p809/all.c, all_l.txt.
#include <defcmd/geolib.hpp>

#include <cstdlib>

#include "ext_p67.hpp"

namespace def::cmd::geolib {

// @src 140257080  calloc n floats
float* fun_140257080(std::int32_t n)
{
    if (n == 0) return nullptr;
    void* p = std::calloc(static_cast<std::size_t>(static_cast<std::int64_t>(n)), 4);
    if (p == nullptr) {
        // str 1405e5000, 1405e4ff0, 1405e4fe8
        ext::alloc_message("%s Error when allocating %s memory of size %d\n", "ERROR    ", "FLOAT", n);
    }
    return static_cast<float*>(p);
}

// @src 1402570e0  realloc to n floats (n == 0: null, p is neither freed nor kept)
float* fun_1402570e0(float* p, std::int32_t n)
{
    if (n == 0) return nullptr;
    void* q = std::realloc(p, static_cast<std::size_t>(static_cast<std::int64_t>(n) << 2));
    if (q == nullptr) {
        // str 1405e5048, 1405e5038, 1405e5030
        ext::alloc_message("%s Error when re-allocating %s memory of size %d\n", "ERROR    ", "FLOAT", n);
    }
    return static_cast<float*>(q);
}

// @src 140410fd0
void fun_140410fd0(FltPointList* l, std::int32_t cap, std::int32_t grow)
{
    l->count = 0;
    l->cap = cap;
    l->grow = grow;
    l->data = nullptr;
    l->dirty = 1;
    for (std::int32_t i = 0; i < 6; i = i + 1) l->bounds[i] = 0.0f;  // stored as int 0
    if (0 < cap) l->data = fun_140257080(l->cap * 3);
}

// @src 140411080
void fun_140411080(FltPointList* l)
{
    l->count = 0;
    l->cap = 0;
    l->dirty = 1;
    l->grow = 100;
    if (l->data != nullptr) std::free(l->data);
    l->data = nullptr;
}

// @src 140411300
void fun_140411300(FltPointList* l)
{
    l->cap = l->cap + l->grow;
    l->data = fun_1402570e0(l->data, l->cap * 3);
}

// @src 1404113b0
std::int32_t fun_1404113b0(FltPointList* l, float x, float y, float z)
{
    if (l->count < l->cap - 1) {
        const std::int32_t i = l->count;
        l->data[i * 3] = x;
        l->data[i * 3 + 1] = y;
        l->data[i * 3 + 2] = z;
        l->count = l->count + 1;
        l->dirty = 1;
        return l->count;
    }
    fun_140411300(l);
    return fun_1404113b0(l, x, y, z);
}

// @src 1404115d0
std::int32_t fun_1404115d0(FltPointList* l, const float* p)
{
    return fun_1404113b0(l, p[0], p[1], p[2]);
}

// @src 140411620
std::int32_t fun_140411620(const FltPointList* l)
{
    return l->count;
}

// @src 14040e620
void fun_14040e620(SurfCheck* s, FltPointList* pts, CellList* cells, float small_area)
{
    s->pts = pts;
    s->cells = cells;
    s->links = nullptr;
    s->patches = nullptr;
    s->ua0 = nullptr;
    s->nlinks = 0;
    s->uac = 0;
    s->npatch = 0;
    s->ub4 = 0;
    s->ub8 = 0;
    s->ubc = 0;
    s->depth_limit = 1000;
    s->uc4 = 0;
    s->small_area = small_area;
    s->ucc = 0;
    s->ud8 = 0;
    s->udc = 0;
    for (std::int32_t i = 0; i < 5; i = i + 1) {
        s->ue0[i] = 0;
        s->u108[i] = 0;
    }
    ext::cell_init(&s->list20, 0, 10, 1);
    ext::cell_init(&s->list40, 0, 10, 1);
    ext::cell_init(&s->list80, 0, 10, 1);
    ext::cell_init(&s->list60, 0, 5, 1);
}

// @src 140459d00
void fun_140459d00(std::byte* patch)
{
    ext::cell_free(reinterpret_cast<CellList*>(patch));
    ext::cell_free(reinterpret_cast<CellList*>(patch + 0x20));
    ext::cell_free(reinterpret_cast<CellList*>(patch + 0x40));
    ext::cell_free(reinterpret_cast<CellList*>(patch + 0x60));
}

// @src 14040e810
void fun_14040e810(SurfCheck* s)
{
    ext::cell_free(&s->list20);
    ext::cell_free(&s->list40);
    ext::cell_free(&s->list80);
    ext::cell_free(&s->list60);
    for (std::int32_t i = 0; i < s->nlinks; i = i + 1) ext::cell_free(&s->links[i]);
    if (s->links != nullptr) std::free(s->links);
    s->links = nullptr;
    for (std::int32_t i = 0; i < s->npatch; i = i + 1) {
        fun_140459d00(s->patches + static_cast<std::int64_t>(i) * 0x88);
    }
    if (s->patches != nullptr) std::free(s->patches);
    s->patches = nullptr;
    if (s->ua0 != nullptr) std::free(s->ua0);
    s->ua0 = nullptr;
    s->pts = nullptr;
    s->cells = nullptr;
}

// @src 14040fde0
void fun_14040fde0(SurfCheck* s)
{
    FltPointList* pts = s->pts;
    CellList* cells = s->cells;
    const std::int32_t ncell = ext::cell_count(cells);
    const std::int32_t width = ext::cell_width(cells);
    const std::int32_t npts = fun_140411620(pts);
    s->nlinks = npts;
    s->links = ext::cell_array(npts);
    for (std::int32_t i = 0; i < npts; i = i + 1) ext::cell_init(&s->links[i], 0, 2, 1);
    for (std::int32_t c = 0; c < ncell; c = c + 1) {
        const std::int32_t* rec = ext::cell_get(cells, c + 1);
        for (std::int32_t k = 0; k < width; k = k + 1) {
            CellList* L = &s->links[rec[k] - 1];
            const std::int32_t id = c + 1;
            ext::cell_append(L, 1, &id);
        }
    }
}

} // namespace def::cmd::geolib
