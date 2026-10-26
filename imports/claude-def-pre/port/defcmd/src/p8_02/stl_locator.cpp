// p8-02: the float point list helpers (defFPT trim / get / bounds / distance), the defCELL helpers
// of this packet (print, trim, width, reverse) and the point locator defLOC used by the STL vertex
// merge. Spec: docs/re/commands/GFREAD_STL_GEO_NAS.md sections 2.2, 2.5, 2.6.
// FP order from the listing (deform-rea-work/out/p802/all_l.txt).
#include <cmath>
#include <cstdio>
#include <cstdlib>

#include <defrt/stdlib/legacy_printf.hpp>

#include "ext_p67.hpp"
#include "ext_p8_09.hpp"
#include "stl_nas_internal.hpp"

namespace def::cmd::stlnas {

namespace {
CellList* bucket(Locator* l, i32 b)
{
    return l->buckets + b;
}
}  // namespace

// ---- defFPT ------------------------------------------------------------------------------------

// @src 140411360
void fun_140411360(PointList* l)
{
    l->cap = l->count;
    l->data = ext::fun_1402570e0(l->data, l->cap * 3);
}

// @src 140411640
float* fun_140411640(PointList* l, i32 k)
{
    if (k < 1 || l->count < k) {
        ext::fun_14039d450("defFPT_GetPoint : %d not valid, Max %d\n", k, l->count);  // str 1406293f8
    }
    return l->data + (k - 1) * 3;
}

// @src 1404116a0
float* fun_1404116a0(PointList* l)
{
    if (l->dirty != 0) {
        l->bounds[4] = 1.0E29f;  // DAT_1405646a4
        l->bounds[2] = 1.0E29f;
        l->bounds[0] = 1.0E29f;
        l->bounds[5] = -1.0E29f;  // DAT_1405646a8
        l->bounds[3] = -1.0E29f;
        l->bounds[1] = -1.0E29f;
        for (i32 i = 0; i < ext::fun_140411620(l); i = i + 1) {
            const float* p = fun_140411640(l, i + 1);
            for (i32 c = 0; c < 3; c = c + 1) {
                if (p[c] < l->bounds[c * 2]) l->bounds[c * 2] = p[c];
                if (l->bounds[c * 2 + 1] < p[c]) l->bounds[c * 2 + 1] = p[c];
            }
        }
        l->dirty = 0;
    }
    return l->bounds;
}

// @src 140411850
float* fun_140411850(PointList* l)
{
    fun_1404116a0(l);
    return l->bounds;
}

// @src 140411880  ((a0-b0)^2 + (a1-b1)^2) + (a2-b2)^2, all float
float fun_140411880(const float* a, const float* b)
{
    float s = (a[0] - b[0]) * (a[0] - b[0]);
    s = s + (a[1] - b[1]) * (a[1] - b[1]);
    return s + (a[2] - b[2]) * (a[2] - b[2]);
}

// ---- defCELL -----------------------------------------------------------------------------------

// @src 1404051b0
void fun_1404051b0(CellList* c, i32 all)
{
    def::rt::crt::fprintf(stdout, "\n\tCell list info\n");                       // str 140627360
    def::rt::crt::fprintf(stdout, "\t\tNumber of cells  : %d\n", c->count);      // str 140627378
    def::rt::crt::fprintf(stdout, "\t\tMax cells        : %d\n", c->cap);        // str 140627398
    def::rt::crt::fprintf(stdout, "\t\tCell size        : %d\n", c->width);      // str 1406273b8
    def::rt::crt::fprintf(stdout, "\t\tMemory increment : %d\n", c->inc);        // str 1406273d8
    if (all != 0) {
        for (i32 i = 0; i < c->count; i = i + 1) {
            def::rt::crt::fprintf(stdout, "\t%6d  ", i + 1);  // str 1406273f4
            for (i32 k = 0; k < c->width; k = k + 1) {
                def::rt::crt::fprintf(stdout, " %7d ", c->data[i * c->width + k]);  // str 1406273fc
            }
            def::rt::crt::fprintf(stdout, "\n");
        }
    }
    def::rt::crt::fprintf(stdout, "\n");
}

// @src 1404053b0
void fun_1404053b0(CellList* c)
{
    c->cap = c->count;
    if (0 < c->cap) c->data = ext::fun_140256e20(c->data, c->cap * c->width);
}

// @src 140405700
i32 fun_140405700(const CellList* c)
{
    return c->width2;
}

// @src 140405800
void fun_140405800(CellList* c, i32 k)
{
    if (k < 1 || c->count < k) {
        ext::fun_14039d450("defCELL_ReverseCellData : %d not valid, Max %d\n", k, c->count);  // str 140627500
    }
    i32* r = ext::fun_140405720(c, k);
    const i32 w = c->width;
    for (i32 i = 0; i < w / 2; i = i + 1) {
        const i32 t = r[i];
        r[i] = r[(w - i) - 1];
        r[(w - i) - 1] = t;
    }
}

// ---- defLOC ------------------------------------------------------------------------------------

// @src 14040ce80
void fun_14040ce80(Locator* l, float tol)
{
    l->dims[0] = 100;
    l->dims[1] = 100;
    l->dims[2] = 100;
    l->nb = 0;
    l->inc = 5;
    l->u18 = 1;
    l->count = 0;
    l->tol = tol;
    l->tol2 = tol * tol;
    l->h[2] = 0.0f;
    l->h[1] = 0.0f;
    l->h[0] = 0.0f;
    l->buckets = nullptr;
    l->points = nullptr;
    l->buckets2 = nullptr;
    ext::fun_1404050b0(&l->neigh, 0, 100, 3);
}

// @src 14040cf80
void fun_14040cf80(Locator* l)
{
    ext::fun_140405150(&l->neigh);
    for (i32 i = 0; i < l->nb; i = i + 1) {
        ext::fun_140405150(l->buckets + i);
        ext::fun_140405150(l->buckets2 + i);
    }
    if (l->buckets != nullptr) std::free(l->buckets);
    l->buckets = nullptr;
    if (l->buckets2 != nullptr) std::free(l->buckets2);
    l->buckets2 = nullptr;
}

// @src 14040d270
void fun_14040d270(Locator* l, PointList* pts, const float* bounds, i32 verbose)
{
    if (verbose != 0) def::rt::crt::fprintf(stdout, "\tdefLOC_InitPointInsertion\n");  // str 1406286c0
    l->count = 0;
    l->points = pts;
    for (i32 i = 0; i < 3; i = i + 1) {
        l->range[i * 2] = bounds[i * 2];
        l->range[i * 2 + 1] = bounds[i * 2 + 1];
        if (l->range[i * 2 + 1] <= l->range[i * 2]) {
            l->range[i * 2 + 1] = l->range[i * 2] + 1.0f;  // DAT_14053176c
        }
    }
    l->nb = 1;
    for (i32 i = 0; i < 3; i = i + 1) l->nb = l->nb * l->dims[i];
    l->buckets = ext::fun_1404057a0(l->nb);
    l->buckets2 = ext::fun_1404057a0(l->nb);
    for (i32 i = 0; i < l->nb; i = i + 1) {
        ext::fun_1404050b0(l->buckets + i, 0, l->inc, 1);
        ext::fun_1404050b0(l->buckets2 + i, 0, l->inc, 1);
    }
    for (i32 i = 0; i < 3; i = i + 1) {
        l->h[i] = (l->range[i * 2 + 1] - l->range[i * 2]) / static_cast<float>(l->dims[i]);
    }
    l->tol2 = l->tol * l->tol;
    i32 maxdim = 0;       // local_30
    float minh = 1.0E29f;  // local_34, DAT_1405646a4
    for (i32 i = 0; i < 3; i = i + 1) {
        if (minh > l->h[i]) minh = l->h[i];          // COMISS minh, h; JBE keeps minh
        if (!(maxdim > l->dims[i])) maxdim = l->dims[i];  // CMP; JLE takes dims[i]
    }
    l->level = static_cast<i32>(std::ceil(static_cast<double>(l->tol) / static_cast<double>(minh)));  // 140486e8c
    if (maxdim < l->level) l->level = maxdim;
    if (verbose != 0) {
        def::rt::crt::fprintf(stdout, "\tNumber of buckets : %d\n", l->nb);                    // str 1406286e0
        def::rt::crt::fprintf(stdout, "\tInsertion level   : %d\n", l->level);                 // str 140628700
        def::rt::crt::fprintf(stdout, "\tInsertion tol     : %f\n", static_cast<double>(l->tol));  // str 140628720
        def::rt::crt::fprintf(stdout, "\tH1                : %f\n", static_cast<double>(l->h[0]));
        def::rt::crt::fprintf(stdout, "\tH2                : %f\n", static_cast<double>(l->h[1]));
        def::rt::crt::fprintf(stdout, "\tH3                : %f\n", static_cast<double>(l->h[2]));
    }
}

namespace {
// the bucket index of p (listing 0x14040d7c1..0x14040d855, the same in 14040dc00)
void bucket_index(const Locator* l, const float* p, i32* idx)
{
    for (i32 i = 0; i < 3; i = i + 1) {
        const float a = p[i] - l->range[i * 2];                     // SUBSS
        const double t = static_cast<double>(a) * 0.999;            // CVTPS2PD, MULSD DAT_140522f68
        const float w = l->range[i * 2 + 1] - l->range[i * 2];      // SUBSS
        const float q = static_cast<float>(t / static_cast<double>(w));  // DIVSD, CVTSD2SS
        idx[i] = static_cast<i32>(q * static_cast<float>(l->dims[i]));  // CVTDQ2PS, MULSS, CVTTSS2SI
    }
}
}  // namespace

// @src 14040d750
i32 fun_14040d750(Locator* l, const float* p, i32 verbose)
{
    if (verbose != 0) def::rt::crt::fprintf(stdout, "\tdefLOC_InsertPoint\n");  // str 1406287a0
    i32 idx[4];  // local_58
    bucket_index(l, p, idx);
    if (verbose != 0) {
        def::rt::crt::fprintf(stdout, "\tPoint  %15.5f %15.5f %15.5f\n", static_cast<double>(p[0]),
                              static_cast<double>(p[1]), static_cast<double>(p[2]));            // str 1406287b8
        def::rt::crt::fprintf(stdout, "\tBucket %3d %3d %3d\n", idx[0], idx[1], idx[2]);  // str 1406287d8
    }
    const i32 home = idx[0] + idx[1] * l->dims[0] + idx[2] * l->dims[0] * l->dims[1];  // local_64
    CellList* home_list = bucket(l, home);
    for (i32 r = 0;; r = r + 1) {
        if (l->level < r) {
            l->count = l->count + 1;
            i32 id[4] = {l->count, 0, 0, 0};  // local_78
            ext::fun_140405540(home_list, 1, id);
            ext::fun_1404115d0(l->points, p);
            if (verbose != 0) def::rt::crt::fprintf(stdout, "\tNew point %d added\n", l->count);  // str 1406287f0
            return l->count;
        }
        fun_14040dfd0(l, idx, l->dims, r, verbose);
        for (i32 k = 0; k < ext::fun_1404056c0(&l->neigh); k = k + 1) {
            const i32* b = ext::fun_140405720(&l->neigh, k + 1);
            const i32 bi = b[0] + b[1] * l->dims[0] + b[2] * l->dims[0] * l->dims[1];  // local_40
            if (ext::fun_1404056c0(bucket(l, bi)) != 0) {
                const i32 n = ext::fun_1404056c0(bucket(l, bi));  // local_68
                for (i32 j = 0; j < n; j = j + 1) {
                    const i32* e = ext::fun_140405720(bucket(l, bi), j + 1);
                    const float* q = fun_140411640(l->points, *e);
                    if (fun_140411880(p, q) <= l->tol2) return *e;
                }
            }
        }
    }
}

// @src 14040dc00
i32 fun_14040dc00(Locator* l, const float* p, i32 verbose)
{
    if (verbose != 0) def::rt::crt::fprintf(stdout, "\tdefLOC_InsertPointExact\n");  // str 140628808
    i32 idx[4];
    bucket_index(l, p, idx);
    if (verbose != 0) {
        def::rt::crt::fprintf(stdout, "\tPoint  %15.5f %15.5f %15.5f\n", static_cast<double>(p[0]),
                              static_cast<double>(p[1]), static_cast<double>(p[2]));            // str 140628828
        def::rt::crt::fprintf(stdout, "\tBucket %3d %3d %3d\n", idx[0], idx[1], idx[2]);  // str 140628848
    }
    const i32 home = idx[0] + idx[1] * l->dims[0] + idx[2] * l->dims[0] * l->dims[1];  // local_5c
    CellList* hl = bucket(l, home);
    if (ext::fun_1404056c0(hl) == 0) {
        ext::fun_1404050b0(hl, 10, 0x32, 1);
    } else {
        const i32 n = ext::fun_1404056c0(hl);
        for (i32 j = 0; j < n; j = j + 1) {
            const i32* e = ext::fun_140405720(hl, j + 1);
            const float* q = fun_140411640(l->points, *e);
            if (p[0] == q[0] && p[1] == q[1] && p[2] == q[2]) return *e;
        }
    }
    l->count = l->count + 1;
    i32 id[4] = {l->count, 0, 0, 0};  // local_70
    ext::fun_140405540(hl, 1, id);
    ext::fun_1404115d0(l->points, p);
    if (verbose != 0) def::rt::crt::fprintf(stdout, "\tNew point %d added\n", l->count);  // str 140628860
    return l->count;
}

// @src 14040dfd0
void fun_14040dfd0(Locator* l, const i32* idx, const i32* dims, i32 ring, i32 verbose)
{
    if (verbose != 0) def::rt::crt::fprintf(stdout, "\tdefLOC_GetBucketNeighbors\n");  // str 140628878
    ext::fun_140405150(&l->neigh);
    ext::fun_1404050b0(&l->neigh, 0, 100, 3);
    if (ring == 0) {
        ext::fun_140405540(&l->neigh, 3, idx);
        return;
    }
    i32 lo[4];  // local_28
    i32 hi[4];  // local_40
    for (i32 i = 0; i < 3; i = i + 1) {
        const i32 a = idx[i] - ring;
        const i32 b = idx[i] + ring;
        lo[i] = a < 1 ? 0 : a;
        hi[i] = (dims[i] - 1 <= b) ? dims[i] - 1 : b;
    }
    for (i32 x = lo[0]; x <= hi[0]; x = x + 1) {
        for (i32 y = lo[1]; y <= hi[1]; y = y + 1) {
            for (i32 z = lo[2]; z <= hi[2]; z = z + 1) {
                if (x == idx[0] + ring || x == idx[0] - ring || y == idx[1] + ring || y == idx[1] - ring ||
                    z == idx[2] + ring || z == idx[2] - ring) {
                    const i32 rec[3] = {x, y, z};  // local_58
                    ext::fun_140405540(&l->neigh, 3, rec);
                }
            }
        }
    }
    if (verbose != 0) {
        def::rt::crt::fprintf(stdout, "\tdefLOC_GetBucketNeighbors\n");  // str 140628898
        fun_1404051b0(&l->neigh, 1);
    }
}

}  // namespace def::cmd::stlnas
