// p8-02: the surface checker behind DEF_STL.OUT: patch walk and orientation (changes the cell
// list), areas, patch boundaries and the summary text. The checker's ctor, dtor and node links
// (14040e620, 14040e810, 14040fde0) are p8-09's. Spec: docs/re/commands/GFREAD_STL_GEO_NAS.md 2.7.
#include <cmath>
#include <cstdio>
#include <cstdlib>

#include <defmodel/bc/boundary_topology.hpp>
#include <defrt/stdlib/legacy_printf.hpp>

#include "ext_p67.hpp"
#include "ext_p8_09.hpp"
#include "stl_nas_internal.hpp"

namespace def::cmd::stlnas {

namespace crt = def::rt::crt;

// @src 14040e980
void fun_14040e980(SurfCheck* s, std::FILE* f, i32 level)
{
    i32 free_edges = 0;  // local_14
    for (i32 i = 0; i < s->npatchdata; i = i + 1) free_edges = free_edges + s->patches[i].ncurves;
    crt::fprintf(f, "\n\tSurface information\n");
    crt::fprintf(f, "\tNumber of points              : %d\n", ext::fun_140411620(s->points));
    crt::fprintf(f, "\tNumber of polys               : %d\n", ext::fun_1404056c0(s->cells));
    crt::fprintf(f, "\tNo of surfaces                : %d\n", s->npatch);
    crt::fprintf(f, "\tNo of free edges              : %d\n", free_edges);
    crt::fprintf(f, "\tPoly with invalid edges       : %d\n", ext::fun_1404056c0(&s->invalid));
    crt::fprintf(f, "\tPoly with invalid orientation : %d\n", ext::fun_1404056c0(&s->reversed));
    crt::fprintf(f, "\tPoly with small area          : %d\n", ext::fun_1404056c0(&s->small));
    crt::fprintf(f, "\tSurface area                  : %g\n", static_cast<double>(s->area));
    crt::fprintf(f, "\tMin polygon area              : %g\n", static_cast<double>(s->min_area));
    crt::fprintf(f, "\tMax polygon area              : %g\n", static_cast<double>(s->max_area));
    crt::fprintf(f, "\tMin edge length               : %g\n", static_cast<double>(s->min_edge));
    crt::fprintf(f, "\tMax edge length               : %g\n", static_cast<double>(s->max_edge));
    crt::fprintf(f, "\tRecursion depth limit         : %d\n", s->limit);
    crt::fprintf(f, "\tNo. exceed max depth          : %d\n", s->exceed);
    crt::fprintf(f, "\n");
    if (level != 0) {
        if (0 < ext::fun_1404056c0(&s->invalid)) {
            crt::fprintf(f, "\nPolygons have edges shared by more than one other polygon \n");
            const i32* d = ext::fun_140405720(&s->invalid, 1);
            const i32 n = ext::fun_1404056c0(&s->invalid);
            for (i32 i = 0; i < n; i = i + 1) {
                crt::fprintf(f, "%d ", d[i]);
                if (i % 10 == 0) crt::fprintf(f, "\n");
            }
            crt::fprintf(f, "\n");
        }
        if (0 < ext::fun_1404056c0(&s->reversed)) {
            crt::fprintf(f, "\nBadly oriented polygons which were reversed\n");
            const i32* d = ext::fun_140405720(&s->reversed, 1);
            const i32 n = ext::fun_1404056c0(&s->reversed);
            for (i32 i = 0; i < n; i = i + 1) {
                crt::fprintf(f, "%d ", d[i]);
                if (i % 10 == 0) crt::fprintf(f, "\n");
            }
            crt::fprintf(f, "\n");
        }
        if (0 < ext::fun_1404056c0(&s->small)) {
            crt::fprintf(f, "\nPolygons with area less than %g\n", static_cast<double>(s->small_area));
            const i32* d = ext::fun_140405720(&s->small, 1);
            const i32 n = ext::fun_1404056c0(&s->small);
            for (i32 i = 0; i < n; i = i + 1) {
                crt::fprintf(f, "%d ", d[i]);
                if (i % 10 == 0) crt::fprintf(f, "\n");
            }
            crt::fprintf(f, "\n");
        }
        crt::fprintf(f, "\n");
    }
    if (level == 2) {
        crt::fprintf(f, "\tSurf PtLinks\n");
        for (i32 i = 0; i < s->nlinks; i = i + 1) {
            crt::fprintf(f, "\tLinks %3d : ", ext::fun_1404056c0(s->links + i));
            const i32* d = ext::fun_140405720(s->links + i, 1);
            for (i32 k = 0; k < ext::fun_1404056c0(s->links + i); k = k + 1) crt::fprintf(f, "%d ", d[k]);
            crt::fprintf(f, "\n");
        }
        crt::fprintf(f, "\n");
    }
    if (level != 0) {
        crt::fprintf(f, "Surf patches\n");
        if (level == 1) {
            for (i32 i = 0; i < s->npatchdata; i = i + 1) {
                const i32 npoly = ext::fun_1404056c0(&s->patches[i].polys);
                crt::fprintf(f, "Patch %6d  NoCurves %6d  NoPoly  %6d\n", i + 1, s->patches[i].ncurves, npoly);
            }
            crt::fprintf(f, "\n\n");
        } else {
            for (i32 i = 0; i < s->npatchdata; i = i + 1) {
                crt::fprintf(f, "Patch %d : \n", i + 1);
                fun_140459d50(f, s->patches + i, level);
            }
            crt::fprintf(f, "\n\n");
        }
    }
}

// @src 14040f4e0
i32 fun_14040f4e0(SurfCheck* s, std::FILE* f, void*, i32 verbose)
{
    i32 reversed_any = 0;  // local_1c
    s->t_start[0] = fun_140222b60();
    s->patch = def::model::bc::calloc_ints(ext::fun_1404056c0(s->cells));  // 140256dc0
    if (verbose != 0) crt::fprintf(f, "\t...Removing extra points   \n");
    s->npatch = 0;
    if (verbose != 0) crt::fprintf(f, "\t...Building surface links  ");
    s->t_start[1] = fun_140222b60();
    ext::fun_14040fde0(s);
    s->t_end[1] = fun_140222b80();
    if (verbose != 0) {
        crt::fprintf(f, "(%g ms)\n", static_cast<double>(s->t_end[1] - s->t_start[1]));
        crt::fprintf(f, "\t...Finding surface patches  ");
    }
    s->t_start[2] = fun_140222b60();
    i32 p = 1;  // local_24
    i32 seed = 0;  // local_20
    while (fun_14040f9f0(s, &seed)) {
        ext::fun_1404050b0(&s->pending, 0, 5, 1);
        s->depth = 0;
        fun_14040fa70(s, seed, p, verbose);
        s->maxdepth = s->depth < s->maxdepth ? s->maxdepth : s->depth;
        if (0 < ext::fun_1404056c0(&s->pending)) {
            for (i32 j = 0; j < ext::fun_1404056c0(&s->pending); j = j + 1) {
                s->depth = 0;
                const i32* d = ext::fun_140405720(&s->pending, 1);
                fun_14040fa70(s, d[j], p, verbose);
                s->maxdepth = s->depth < s->maxdepth ? s->maxdepth : s->depth;
            }
        }
        p = p + 1;
        ext::fun_140405150(&s->pending);
    }
    s->npatch = p - 1;
    s->t_end[2] = fun_140222b80();
    if (verbose != 0) {
        crt::fprintf(f, "(%g ms)\n", static_cast<double>(s->t_end[2] - s->t_start[2]));
        crt::fprintf(f, "\t...Calculating area ");
    }
    s->t_start[3] = fun_140222b60();
    s->area = static_cast<float>(fun_140410760(s));
    s->t_end[3] = fun_140222b80();
    if (verbose != 0) {
        crt::fprintf(f, "(%g ms)\n", static_cast<double>(s->t_end[3] - s->t_start[3]));
        crt::fprintf(f, "\t...Checking for redundant polygons  ");
    }
    s->t_start[4] = fun_140222b60();
    fun_140410750(s);
    s->t_end[4] = fun_140222b80();
    if (verbose != 0) {
        crt::fprintf(f, "(%g ms)\n", static_cast<double>(s->t_end[4] - s->t_start[4]));
        crt::fprintf(f, "\t...Extracting patch boundaries  ");
    }
    s->t_start[5] = fun_140222b60();
    fun_140410130(s);
    s->t_end[5] = fun_140222b80();
    if (verbose != 0) crt::fprintf(f, "(%g ms)\n", static_cast<double>(s->t_end[5] - s->t_start[5]));
    s->t_end[0] = fun_140222b80();
    if (0 < ext::fun_1404056c0(&s->reversed)) reversed_any = 1;
    return reversed_any;
}

// @src 14040f9f0
bool fun_14040f9f0(SurfCheck* s, i32* seed)
{
    const i32 n = ext::fun_1404056c0(s->cells);
    for (i32 i = 0; i < n; i = i + 1) {
        if (s->patch[i] == 0) {
            *seed = i + 1;
            return true;
        }
    }
    return false;
}

// @src 14040fa70
void fun_14040fa70(SurfCheck* s, i32 cell, i32 patch, i32 verbose)
{
    const i32 w = fun_140405700(s->cells);  // local_2c
    CellList* cells = s->cells;             // local_18
    s->patch[cell - 1] = patch;
    s->visits = s->visits + 1;
    s->depth = s->depth + 1;
    if (s->limit < s->depth) {
        i32 rec = cell;  // local_34
        ext::fun_140405540(&s->pending, 1, &rec);
        s->exceed = s->exceed + 1;
        return;  // depth is not decremented
    }
    for (i32 k = 0; k < w; k = k + 1) {
        CellList nb;  // local_68
        ext::fun_1404050b0(&nb, 0, 5, 1);
        const i32* c = ext::fun_140405720(cells, cell);
        const i32 a = c[k];            // local_28
        const i32 b = c[(k + 1) % w];  // local_30
        if (a == b) continue;
        fun_14040ff60(s, cell, a, b, &nb);
        if (ext::fun_1404056c0(&nb) < 2) {
            if (ext::fun_1404056c0(&nb) == 1) {
                const i32* n0 = ext::fun_140405720(&nb, 1);  // local_40
                if (s->patch[*n0 - 1] == 0) {
                    const i32* m = ext::fun_140405720(cells, *n0);  // local_20
                    i32 j = 0;
                    while (j < w && m[j] != b) j = j + 1;
                    if (m[(j + 1) % w] != a) {
                        i32 rec = *n0;
                        ext::fun_140405540(&s->reversed, 1, &rec);
                        fun_140405800(cells, *n0);
                    }
                    fun_14040fa70(s, *n0, patch, verbose);
                }
            }
        } else {
            i32 rec = cell;
            ext::fun_140405540(&s->invalid, 1, &rec);
        }
        ext::fun_140405150(&nb);
    }
    s->depth = s->depth - 1;
}

// @src 14040ff60
void fun_14040ff60(SurfCheck* s, i32 cell, i32 a, i32 b, CellList* out)
{
    const i32 w = fun_140405700(s->cells);
    CellList* cells = s->cells;
    CellList* la = s->links + (a - 1);
    const i32 n = ext::fun_1404056c0(la);
    const i32* l = ext::fun_140405720(la, 1);
    for (i32 i = 0; i < n; i = i + 1) {
        if (l[i] == cell) continue;
        const i32* m = ext::fun_140405720(cells, l[i]);
        i32 j = 0;
        while (j < w && m[j] != b) j = j + 1;
        if (j < w) {
            bool found = false;  // local_c
            i32 cand = l[i];     // local_2c
            for (i32 k = 0; k < ext::fun_1404056c0(out); k = k + 1) {
                if (*ext::fun_140405720(out, k + 1) == cand) {
                    found = true;
                    break;
                }
            }
            if (!found) ext::fun_140405540(out, 1, &cand);
        }
    }
}

// @src 140410130
void fun_140410130(SurfCheck* s)
{
    s->patches = fun_140459fe0(s->npatch);
    s->npatchdata = s->npatch;
    for (i32 i = 0; i < s->npatch; i = i + 1) fun_140459c70(s->patches + i);
    const i32 n = ext::fun_1404056c0(s->cells);
    for (i32 i = 0; i < n; i = i + 1) fun_140459fb0(s->patches + (s->patch[i] - 1), i + 1);
    for (i32 i = 0; i < s->npatch; i = i + 1) fun_140410260(s, i + 1);
}

// @src 140410260
void fun_140410260(SurfCheck* s, i32 patch)
{
    PatchData* pd = s->patches + (patch - 1);  // local_30
    CellList edges;                            // local_68: free edges (a, b)
    ext::fun_1404050b0(&edges, 0, 100, 2);
    const i32 w = fun_140405700(s->cells);       // local_34
    const i32 npoly = ext::fun_1404056c0(&pd->polys);  // local_48
    const i32* polys = ext::fun_140405720(&pd->polys, 1);  // local_28
    for (i32 i = 0; i < npoly; i = i + 1) {
        const i32* c = ext::fun_140405720(s->cells, polys[i]);  // local_90
        for (i32 k = 0; k < w; k = k + 1) {
            CellList nb;  // local_b0
            ext::fun_1404050b0(&nb, 0, 5, 1);
            i32 e[2] = {c[k], c[(k + 1) % w]};  // local_40, local_3c
            fun_14040ff60(s, polys[i], e[0], e[1], &nb);
            if (ext::fun_1404056c0(&nb) == 0) {
                ext::fun_140405540(&edges, 2, e);
            } else {
                const i32* n0 = ext::fun_140405720(&nb, 1);
                if (s->patch[*n0 - 1] != patch) ext::fun_140405540(&edges, 2, e);
            }
            ext::fun_140405150(&nb);
        }
    }
    if (ext::fun_1404056c0(&edges) == 0) {
        ext::fun_140405150(&edges);
        pd->ncurves = 0;
        return;
    }
    pd->ncurves = 0;
    i32 counter[2] = {0, 0};  // local_b8
    i32 endv[2] = {0, 0};     // local_70
    i32* d = ext::fun_140405720(&edges, 1);  // local_20
    const i32 n = ext::fun_1404056c0(&edges);  // local_74
    for (i32 k = 0; k < n; k = k + 1) {
        if (d[k * 2] == 0) continue;
        ext::fun_140405540(&pd->starts, 1, counter);
        ext::fun_140405540(&pd->points, 1, d + k * 2);
        const i32 next = k + 1;  // local_38
        i32 cur = k;             // local_78
        counter[0] = counter[0] + 1;
        while (next < n) {
            ext::fun_140405540(&pd->points, 1, d + (cur * 2 + 1));
            d[cur * 2] = 0;
            i32 j = next;  // local_18
            while (j < n && d[j * 2] != d[cur * 2 + 1]) j = j + 1;
            if (n <= j) {
                endv[0] = counter[0];
                counter[0] = counter[0] + 1;
                ext::fun_140405540(&pd->ends, 1, endv);
                break;
            }
            cur = j;
            counter[0] = counter[0] + 1;
        }
    }
    fun_1404053b0(&pd->points);
    fun_1404053b0(&pd->starts);
    fun_1404053b0(&pd->ends);
    pd->ncurves = ext::fun_1404056c0(&pd->ends);
    ext::fun_140405150(&edges);
}

// @src 140410750  redundant-polygon check: empty
void fun_140410750(SurfCheck*) {}

// @src 140410760
double fun_140410760(SurfCheck* s)
{
    double min_edge = 1.0E29;   // local_70, DAT_14052c598
    double max_edge = -1.0E29;  // local_a8, DAT_14052c5a0
    double min_area = 1.0E29;   // local_c0
    double max_area = -1.0E29;  // local_c8
    const i32 n = ext::fun_1404056c0(s->cells);  // local_68
    i32 w = fun_140405700(s->cells);             // local_88
    double sum = 0.0;                            // local_d8
    double mn = 0.0, mx = 0.0;                   // local_80, local_78
    if (w == 3) {
        for (i32 i = 0; i < n; i = i + 1) {
            const i32* c = ext::fun_140405720(s->cells, i + 1);
            const double a = fun_140410d40(s->points, c, &mn, &mx);  // local_60
            min_edge = mn <= min_edge ? mn : min_edge;
            max_edge = max_edge <= mx ? mx : max_edge;
            min_area = min_area < a ? min_area : a;
            max_area = a < max_area ? max_area : a;
            if (a < static_cast<double>(s->small_area)) {
                i32 rec = i + 1;
                ext::fun_140405540(&s->small, 1, &rec);
            }
            sum = sum + a;
        }
    } else {
        for (i32 i = 0; i < n; i = i + 1) {
            const i32* c = ext::fun_140405720(s->cells, i + 1);
            i32 t[3] = {c[0], c[1], c[2]};  // local_98..local_90
            const double a = fun_140410d40(s->points, t, &mn, &mx);
            min_edge = mn <= min_edge ? mn : min_edge;
            const double max1 = max_edge <= mx ? mx : max_edge;
            t[0] = c[0];
            t[1] = c[2];
            t[2] = c[3];
            const double b = fun_140410d40(s->points, t, &mn, &mx);
            min_edge = mn <= min_edge ? mn : min_edge;
            max_edge = max1 <= mx ? mx : max1;
            min_area = a + b <= min_area ? a + b : min_area;
            max_area = max_area <= a + b ? a + b : max_area;
            if (a + b < static_cast<double>(s->small_area)) {
                i32 rec = i + 1;
                ext::fun_140405540(&s->small, 1, &rec);
            }
            sum = sum + a + b;
        }
    }
    s->min_edge = static_cast<float>(min_edge);
    s->max_edge = static_cast<float>(max_edge);
    s->min_area = static_cast<float>(min_area);
    s->max_area = static_cast<float>(max_area);
    return sum;
}

// @src 140410d40  Heron area of a triangle, edge lengths sqrt((double)(float)d2)
double fun_140410d40(PointList* pts, const i32* tri, double* min_edge, double* max_edge)
{
    double sum = 0.0;          // local_38
    double mn = 1.0E29;        // local_40
    double mx = -1.0E29;       // local_48
    const float* p[3] = {fun_140411640(pts, tri[0]), fun_140411640(pts, tri[1]), fun_140411640(pts, tri[2])};
    double e[3];  // local_78[3..5]
    for (i32 k = 0; k < 3; k = k + 1) {
        const i32 j = (k + 1) % 3;
        double d = static_cast<double>(fun_140411880(p[k], p[j]));
        if (d < 0.0) d = 0.0;
        e[k] = std::sqrt(d);  // 140484644
        mn = e[k] <= mn ? e[k] : mn;
        mx = mx <= e[k] ? e[k] : mx;
        sum = sum + e[k];
    }
    sum = sum / 2.0;  // DAT_140509fb8
    const double v = std::fabs(((sum * (sum - e[0])) * (sum - e[1])) * (sum - e[2]));  // 140484558
    double r = 0.0;
    if (0.0 < v) r = std::sqrt(v);
    *min_edge = mn;
    *max_edge = mx;
    return r;
}

// @src 140459c70
void fun_140459c70(PatchData* p)
{
    p->ncurves = 0;
    ext::fun_1404050b0(&p->polys, 0, 10, 1);
    ext::fun_1404050b0(&p->points, 0, 10, 1);
    ext::fun_1404050b0(&p->starts, 0, 5, 1);
    ext::fun_1404050b0(&p->ends, 0, 5, 1);
}

// @src 140459d50
void fun_140459d50(std::FILE* f, PatchData* p, i32 level)
{
    crt::fprintf(f, "Number of polygons : %d\n", ext::fun_1404056c0(&p->polys));
    if (0 < ext::fun_1404056c0(&p->polys)) {
        const i32* d = ext::fun_140405720(&p->polys, 1);
        for (i32 i = 0; i < ext::fun_1404056c0(&p->polys); i = i + 1) crt::fprintf(f, "%d ", d[i]);
        crt::fprintf(f, "\n\n");
    }
    crt::fprintf(f, "Number of curves   : %d\n", p->ncurves);
    if (0 < p->ncurves && level == 2) {
        crt::fprintf(f, "Starting  ");
        const i32* d = ext::fun_140405720(&p->starts, 1);
        for (i32 i = 0; i < p->ncurves; i = i + 1) crt::fprintf(f, "%d ", d[i]);
        crt::fprintf(f, "\nEnding    ");
        d = ext::fun_140405720(&p->ends, 1);
        for (i32 i = 0; i < p->ncurves; i = i + 1) crt::fprintf(f, "%d ", d[i]);
        crt::fprintf(f, "\nPoints    ");
        d = ext::fun_140405720(&p->points, 1);
        for (i32 i = 0; i < ext::fun_1404056c0(&p->points); i = i + 1) crt::fprintf(f, "%d ", d[i]);
        crt::fprintf(f, "\n\n");
    }
}

// @src 140459fb0
i32 fun_140459fb0(PatchData* p, i32 cell)
{
    i32 rec[6] = {cell, 0, 0, 0, 0, 0};  // local_res10
    ext::fun_140405540(&p->polys, 1, rec);
    return 0;
}

// @src 140459fe0
PatchData* fun_140459fe0(i32 n)
{
    if (n == 0) return nullptr;
    void* p = std::calloc(static_cast<std::size_t>(static_cast<std::int64_t>(n)), 0x88);
    if (p == nullptr) {
        // str 140639740, 140639730, 140639720
        ext::fun_14039d3a0("%s Error when allocating %s memory of size %d\n", "ERROR    ", "PATCH_DATA", n);
    }
    return static_cast<PatchData*>(p);
}

}  // namespace def::cmd::stlnas
