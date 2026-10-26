// cube_lib.cpp: cfnCUBE, the bucket search grid as the 3D interference kernel uses it: create the
// cube table, build a polygon cube (type 1), query a box, copy the sorted result, clean up.
// Spec: docs/re/commands/OBJPOS_interference_kernels.md section 2. Build types other than 1 are
// completed with the type-specific insertion routines of the p67-7 cube group (ctc3d/cube.hpp).
#include <defcmd/ifd/cube_lib.hpp>

#include <defcmd/ifd/cube.hpp>
#include <defcmd/ctc3d/cube.hpp> // p67-7 cube: the insertion routines of the other types
#include <defrt/ftn/geoutl.hpp>
#include <defrt/stdlib/libm.hpp>
#include <defrt/port.hpp>
#include <defrt/stdlib/legacy_printf.hpp>

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace def::cmd::cube {

namespace {

std::FILE* out() { return stdout; } // FUN_14047903c() + 0x30 = &_iob[1]

// int32 products wrap as the binary's imul does
std::int32_t mul32(std::int32_t a, std::int32_t b)
{
    return static_cast<std::int32_t>(static_cast<std::uint32_t>(a) * static_cast<std::uint32_t>(b));
}

double spacing_pow(double x, double y)
{
    return def::rt::libm::crt_pow(x, y); // FUN_140483f60
}

} // namespace

// @src 140231b10  calloc n cubes (0x1a0 bytes), ids (-1, -1); NULL for n < 1
Cube* fun_140231b10(std::int32_t n)
{
    if (n < 1) {
        return nullptr;
    }
    Cube* p = static_cast<Cube*>(std::calloc(static_cast<std::size_t>(static_cast<std::int64_t>(n)), 0x1a0));
    if (p == nullptr) {
        // str 1405e1cc8, 1405e1cb8, 1405e1ca8 (the loop below then faults, as in the binary)
        fun_14039d3a0("%s Error when allocating %s memory of size %d\n", "ERROR    ", "OBJ_CUBE_DATA", n);
    }
    for (std::int32_t i = 0; i < n; ++i) {
        p[i].sub = -1;
        p[i].id = -1;
    }
    return p;
}

// @src 140225640  cfnCUBE_CreateCubes_Obj(&nobj, &verbose)
void fun_140225640(std::int32_t* nobj, std::int32_t* verbose)
{
    const std::int32_t v = *verbose;
    if (v != 0) {
        std::fprintf(out(), "cfnCUBE_CreateCubes_Obj : Entering function\n"); // str 1405df040
    }
    if (*nobj < 1) {
        fun_14039d450("cfnCUBE_CreateCubes_Obj : Illegal value for number of objects %d\n", *nobj); // str 1405df070
    }
    if (g_cube_size != 0) {
        fun_140225820(verbose);
    }
    g_cubes = fun_140231b10(*nobj);
    g_cube_size = *nobj;
    if (v != 0) {
        std::fprintf(out(), "cfnCUBE_CreateCubes_Obj : Exiting function\n"); // str 1405df0b8
    }
}

// @src 140225820  cfnCUBE_CleanCubes_AllObj(&verbose): clean every used cube (elements too), free
void fun_140225820(std::int32_t* verbose)
{
    std::int32_t v = *verbose;   // local_14[0]
    std::int32_t clean_elems = 1; // local_14[1]
    if (v != 0) {
        std::fprintf(out(), "cfnCUBE_CleanCubes_AllObj : Entering function\n"); // str 1405df1f8
    }
    for (std::int32_t i = 0; i < g_cube_used; ++i) {
        fun_140225af0(&i, &clean_elems, &v);
    }
    if (g_cubes != nullptr) {
        std::free(g_cubes);
    }
    g_cubes = nullptr;
    g_cube_size = 0;
    g_cube_used = 0;
    if (v != 0) {
        std::fprintf(out(), "cfnCUBE_CleanCubes_AllObj : Exiting function\n"); // str 1405df228
    }
}

// @src 140225af0  cfnCUBE_Clean_ObjArray(&idx, &clean_elems, &verbose): drop the borrowed point
// and polygon data, free the result list, the element arrays (when asked) and the locator
void fun_140225af0(std::int32_t* idx, std::int32_t* clean_elems, std::int32_t* verbose)
{
    const std::int32_t v = *verbose;
    if (v != 0) {
        std::fprintf(out(), "cfnCUBE_Clean_ObjArray : Entering function\n"); // str 1405df2a0
    }
    const std::int32_t i = *idx;
    if (i < 0 || g_cube_used <= i) {
        fun_14039d450("cfnCUBE_Clean_ObjArray : Illegal object bucket array number %d\n", i); // str 1405df2d0
    }
    Cube* c = &g_cubes[i];
    c->points.cap = 0;
    c->points.count = 0;
    c->points.data = nullptr;
    c->polys.count = 0;
    c->polys.cap = 0;
    c->polys.data = nullptr;
    fun_140405150(&c->result);
    if (*clean_elems != 0) {
        fun_1404047a0(&c->a180);
        fun_1404047a0(&c->a190);
    }
    fun_1403feec0(&c->loc);
    if (v != 0) {
        std::fprintf(out(), "...leaving function\n"); // str 1405df310
    }
}

// part of @src 140226150: division count bc from the polygon count
std::int32_t division_count(std::int32_t npoly)
{
    std::int32_t bc = 50;
    if (npoly < 10) {
        bc = 5;
    } else if (npoly < 100) {
        bc = 10;
    } else if (npoly < 1000) {
        bc = 20;
    } else if (npoly < 10000) {
        bc = static_cast<std::int32_t>(0.0033 * static_cast<double>(npoly - 1000)) + 20; // DAT_140530da0
    } else {
        bc = static_cast<std::int32_t>(static_cast<double>(npoly - 10000) * 0.0005) + 50; // DAT_140530d98
    }
    return bc;
}

// part of @src 140226150: divisions per axis from the extents, bc, nPoly and the pow result
void divisions(const double* a, std::int32_t bc, std::int32_t npoly, double pw, std::int32_t* n, double* sp)
{
    *sp = pw / static_cast<double>(bc);
    for (int i = 0; i < 3; ++i) {
        std::int32_t k = static_cast<std::int32_t>(a[i] / *sp);
        k = (5 < k) ? k : 5;
        n[i] = (k < npoly) ? k : npoly;
    }
    std::int32_t prod = mul32(mul32(n[0], n[1]), n[2]);
    std::int32_t cube3 = mul32(mul32(bc, bc), bc);
    if (49 < bc) {
        while (static_cast<double>(prod / cube3) > 1.1) { // DAT_140530d88; int division
            for (int i = 0; i < 3; ++i) {
                n[i] = (5 < n[i] - 1) ? n[i] - 1 : 5;
            }
            cube3 = mul32(mul32(bc, bc), bc);
            prod = mul32(mul32(n[0], n[1]), n[2]);
        }
    }
}

// @src 140226150  cfnCUBE_BuildBucket(&id, &sub, &type, &nPts, pts, &nPoly, &npp, polys, &offset,
// &create, &verbose)
void fun_140226150(std::int32_t* id, std::int32_t* sub, std::int32_t* type, std::int32_t* npts,
                   double* pts, std::int32_t* npoly, std::int32_t* npp, std::int32_t* polys,
                   std::int32_t* offset, std::int32_t* create, std::int32_t* verbose)
{
    std::int32_t v = *verbose; // local_d8[0]
    const std::int32_t np = *npp;
    const std::int32_t t = *type;
    const std::int32_t off = *offset;
    const std::int32_t loc_verbose = 0; // local_c0
    std::int32_t bc = 50;
    const float tol = 1e-6f; // DAT_140530da8
    if (t < 0 || 7 < t) {
        fun_14039d450("cfnCUBE_BuildBucket : Illegal cube type %d\n", t); // str 1405df3b0
    }
    if (t == 7 && *npp != 2) {
        fun_14039d450("cfnCUBE_BuildBucket : Illegal NONP %d\n", *npp); // str 1405df3e0
    }
    if (v != 0) {
        std::fprintf(out(), "cfnCUBE_BuildBucket : Entering function\n"); // str 1405df408
    }
    if (g_cube_size <= g_cube_used) {
        // str 1405df438
        fun_14039d450("cfnCUBE_BuildBucket : CubeNo %d exceeds the Maximum CubeNo %d\n", g_cube_used + 1,
                      g_cube_size);
    }
    std::int32_t slot = -1;
    for (std::int32_t i = 0; i < g_cube_used; ++i) {
        if (g_cubes[i].id == *id && g_cubes[i].sub == *sub) {
            slot = i;
            if (g_cubes[i].points.data != nullptr || g_cubes[i].polys.data != nullptr) {
                fun_140225af0(&i, create, &v);
            }
            break;
        }
    }
    if (slot == -1 && *create == 0) {
        // str 1405df480
        fun_14039d450("cfnCUBE_BuildBucket : Illegal combination of object number (%d) and entity set number "
                      "(%d)\nYou are trying to update a non-existing bucket.\n",
                      *id, *sub);
    }
    if (slot == -1) {
        slot = g_cube_used;
        g_cube_used = g_cube_used + 1;
    }
    Cube* c = &g_cubes[slot];
    c->id = *id;
    c->sub = *sub;
    fun_140405c80(&c->points, 0, 1000);
    fun_1404050b0(&c->polys, 0, 1000, np);
    if (t == 7) {
        fun_1404050b0(&c->result, 0, 50, 2);
    } else {
        fun_1404050b0(&c->result, 0, 50, 1);
    }
    c->points.cap = *npts;
    c->points.count = *npts;
    c->points.data = pts;
    c->polys.count = *npoly;
    c->polys.cap = *npoly;
    c->polys.data = polys;
    if (*create == 0) {
        if (fun_140404780(&c->a180) != *npts) {
            // str 1405df510
            fun_14039d450("cfnCUBE_BuildBucket : The original number of point (%d) is different than the "
                          "current value (%d). \n",
                          fun_140404780(&c->a180), *npts);
        }
        if (fun_140404780(&c->a190) != *npoly) {
            // str 1405df590
            fun_14039d450("cfnCUBE_BuildBucket : The original number of polygons/elements (%d) is different "
                          "than the current value (%d). \n",
                          fun_140404780(&c->a190), *npoly);
        }
    } else {
        fun_1404044c0(&c->a180, *npts);
        fun_1404044c0(&c->a190, *npoly);
    }
    const bool polygonal = t == 1 || t == 3 || t == 5 || t == 6 || t == 7;
    if (polygonal) {
        bc = division_count(*npoly);
        if (v != 0) {
            std::fprintf(out(), "...setting division size %d\n", bc); // str 1405df600
        }
    }
    fun_1403fed00(&c->loc, t, static_cast<double>(tol), off);
    if (polygonal) {
        const double* b = fun_140406510(&c->points);
        double box[6]; // adStack_68
        double a[3];   // local_a8
        for (int i = 0; i < 3; ++i) {
            box[i * 2] = b[i * 2];
            box[i * 2 + 1] = b[i * 2 + 1];
            if (box[i * 2] >= box[i * 2 + 1]) { // comisd lo, hi; jc (NaN: no change)
                box[i * 2 + 1] = box[i * 2] + 1.0; // DAT_140509c90
            }
            const double d = std::fabs(box[i * 2 + 1] - box[i * 2]); // FUN_140484558
            if (0.001 > d) { // DAT_14051f1e8
                box[i * 2 + 1] = box[i * 2 + 1] + 0.001;
            }
            a[i] = std::fabs(box[i * 2 + 1] - box[i * 2]);
        }
        const double vol = std::fabs(a[0] * a[1] * a[2]);
        const double pw = spacing_pow(vol, 0.3333333); // DAT_140530d90
        std::int32_t n[3];
        double sp = 0.0;
        divisions(a, bc, *npoly, pw, n, &sp);
        if (v != 0) {
            const std::int32_t prod = mul32(mul32(n[0], n[1]), n[2]);
            const std::int32_t cube3 = mul32(mul32(bc, bc), bc);
            def::rt::crt::fprintf(out(), "...spacing            %g %g %g\n", a[0], a[1], a[2]); // str 1405df620
            def::rt::crt::fprintf(out(), "...calculated spacing %g\n", sp);                    // str 1405df640
            std::fprintf(out(), "...divisions in x     %d\n", n[0]);                           // str 1405df660
            std::fprintf(out(), "...divisions in y     %d\n", n[1]);
            std::fprintf(out(), "...divisions in z     %d\n", n[2]);
            std::fprintf(out(), "...max buckets        %d\n", prod);                           // str 1405df6c0
            def::rt::crt::fprintf(out(), "...limit of buck/max  %g\n",
                                  static_cast<double>(prod / cube3)); // str 1405df6e0
        }
        fun_1403ff890(&c->loc, n[0], n[1], n[2]);
    }
    const double* bounds = fun_140406510(&c->points);
    fun_1403ff970(&c->loc, &c->points, bounds, loc_verbose);
    if (v != 0) {
        const double* bx = c->loc.box; // cube + 0x58
        std::fprintf(out(), "\tPoint data set bounds\n");                                // str 1405df578
        def::rt::crt::fprintf(out(), "\tMin   %g %g %g\n", bx[0], bx[2], bx[4]);         // str 1405df700
        def::rt::crt::fprintf(out(), "\tMax   %g %g %g\n", bx[1], bx[3], bx[5]);         // str 1405df718
    }
    if (v != 0) {
        std::fprintf(out(), "...constructing buckets from point list\n"); // str 1405df730
    }
    if (t == 0) {
        // 140226fc2..14022703f: every point into its cell (FUN_140400330, p67-7 cube)
        const std::int32_t k = fun_1404062e0(&c->points);
        for (std::int32_t i = 0; i < k; ++i) {
            fun_140400330(&c->loc, fun_140406300(&c->points, i + 1), i + 1, 0);
        }
    }
    // 140227041..14022705b: the type-0 loop above falls through to the test t == 0 || 3 || 4 || 6,
    // so type 0 inserts every surface point a second time (the binary's behaviour, kept)
    if (t == 0 || t == 3 || t == 4 || t == 6) {
        // 14022707c..14022717f: map of points used by some polygon (FUN_140256b30, memset 'F',
        // FUN_140233ef0), then those points into their cells (FUN_140400330); p67-7 cube
        const std::int32_t k = fun_1404062e0(&c->points);
        char* map = fun_140256b30(k);
        std::memset(map, 'F', static_cast<std::size_t>(static_cast<std::int64_t>(k))); // 140478d30
        fun_140233ef0(map, polys, *npoly, *npp, off);
        for (std::int32_t i = 0; i < k; ++i) {
            if (map[i] != 'F') {
                fun_140400330(&c->loc, fun_140406300(&c->points, i + 1), i + 1, 0);
            }
        }
        if (map != nullptr) {
            std::free(map);
        }
    }
    if (v != 0) {
        std::fprintf(out(), "...constructing buckets from polygon list\n"); // str 1405df760
    }
    if (t == 1 || t == 3) {
        const std::int32_t k = fun_1404056c0(&c->polys);
        for (std::int32_t i = 0; i < k; ++i) {
            const std::int32_t* rec = fun_140405720(&c->polys, i + 1);
            if (0 < *rec) {
                fun_140401050(&c->loc, rec, np, i + 1, 0);
            }
        }
    }
    if (t == 5 || t == 6) {
        // 140227333: FUN_140401c40 for every polygon with rec[0] > 0 (p67-7 cube)
        const std::int32_t k = fun_1404056c0(&c->polys);
        for (std::int32_t i = 0; i < k; ++i) {
            const std::int32_t* rec = fun_140405720(&c->polys, i + 1);
            if (0 < *rec) {
                fun_140401c40(&c->loc, rec, np, i + 1, 0);
            }
        }
    }
    if (t == 7) {
        // 1402273d9: FUN_1404021a0 for every record, with the caller's verbose flag (p67-7 cube)
        const std::int32_t k = fun_1404056c0(&c->polys);
        for (std::int32_t i = 0; i < k; ++i) {
            fun_1404021a0(&c->loc, fun_140405720(&c->polys, i + 1), v);
        }
    }
    if (t == 2 || t == 4) {
        // 14022757b / 14022762c: FUN_140402c40 for every element; verbose >= 2 lists them (p67-7 cube)
        if (v < 2) {
            const std::int32_t k = fun_1404056c0(&c->polys);
            for (std::int32_t i = 0; i < k; ++i) {
                fun_140402c40(&c->loc, fun_140405720(&c->polys, i + 1), np, i + 1, 0);
            }
        } else {
            std::fprintf(out(), "...   inserting elements into locator  max %d\n", fun_1404056c0(&c->polys)); // str 1405df790
            const std::int32_t k = fun_1404056c0(&c->polys);
            for (std::int32_t i = 0; i < k; ++i) {
                const std::int32_t* rec = fun_140405720(&c->polys, i + 1);
                if (2 < v) {
                    std::fprintf(out(), "...  inserting element %d : ", i + 1); // str 1405df7c0
                    for (std::int32_t j = 0; j < np; ++j) {
                        std::fprintf(out(), "%d-", rec[j]); // str 1405df0e4
                    }
                    std::fprintf(out(), "\n"); // str 1405df2cc
                }
                fun_140402c40(&c->loc, rec, np, i + 1, 0);
            }
        }
    }
    if (v != 0) {
        std::fprintf(out(), "...leaving function\n"); // str 1405df7e0
    }
}

// @src 140225ff0  build with create = 1 (140226150)
void fun_140225ff0(std::int32_t* id, std::int32_t* sub, std::int32_t* type, std::int32_t* npts,
                   double* pts, std::int32_t* npoly, std::int32_t* npp, std::int32_t* polys,
                   std::int32_t* offset, std::int32_t* verbose)
{
    std::int32_t create = 1;
    fun_140226150(id, sub, type, npts, pts, npoly, npp, polys, offset, &create, verbose);
}

// @src 140228c60  cfnCUBE_SearchPoly_Obj(&id, &sub, box, &nc, &exact, &verbose): ids stored in
// the cells covering the box (clamped to the locator box) into the cube's result list
void fun_140228c60(std::int32_t* id, std::int32_t* sub, double* box, std::int32_t* nc,
                   std::int32_t* exact, std::int32_t* verbose)
{
    const std::int32_t v = *verbose;
    const std::int32_t oid = *id;
    const std::int32_t osub = *sub;
    std::int32_t slot = -1;
    for (std::int32_t i = 0; i < g_cube_used; ++i) {
        if (g_cubes[i].id == oid && g_cubes[i].sub == osub) {
            slot = i;
            break;
        }
    }
    if (slot == -1) {
        *nc = 0;
        // str 1405e0310
        fun_14039d450("cfnCUBE_SearchPoly_Obj : Illegal combination of object number (%d) and entity set "
                      "number (%d)\n",
                      oid, osub);
    }
    Cube* c = &g_cubes[slot];
    if (fun_1403ff870(&c->loc) != 1 && fun_1403ff870(&c->loc) != 3 && fun_1403ff870(&c->loc) != 5 &&
        fun_1403ff870(&c->loc) != 6) {
        fun_14039d450("defCUBE_SearchPoly_Obj : Locator type not for polygon seraching\n"); // str 1405e0370
    }
    if (v != 0) {
        std::fprintf(out(), "...cfnCUBE_SearchPoly_Obj\n"); // str 1405e03b8
    }
    const std::int32_t width = fun_1404056e0(&c->polys); // local_b0, for the exact test
    Locator* l = &c->loc;
    fun_140405150(&c->result);
    double lbox[6]; // local_a0..78: lo x, lo y, lo z, hi x, hi y, hi z
    lbox[0] = l->box[0];
    lbox[1] = l->box[2];
    lbox[2] = l->box[4];
    lbox[3] = l->box[1];
    lbox[4] = l->box[3];
    lbox[5] = l->box[5];
    if (v != 0) {
        std::fprintf(out(), "... Input Box :\n"); // str 1405dfbd8
        def::rt::crt::fprintf(out(), "min(%f, %f, %f), max(%f, %f, %f)\n", box[0], box[1], box[2], box[3], box[4],
                              box[5]); // str 1405dfde8
        std::fprintf(out(), "... Bucket Box :\n"); // str 1405dffa8
        def::rt::crt::fprintf(out(), "min(%f, %f, %f), max(%f, %f, %f)\n", lbox[0], lbox[1], lbox[2], lbox[3],
                              lbox[4], lbox[5]); // str 1405e0138
    }
    if (fun_140240b40(box, lbox) == 0) {
        if (v != 0) {
            std::fprintf(out(), "There is no intersection between two boxes, RETURN\n"); // str 1405e02d8
        }
        *nc = 0;
        return;
    }
    std::int32_t maxz = 0;
    std::int32_t maxy = 0;
    std::int32_t maxx = 0;
    std::int32_t minx = l->n[0] - 1;
    std::int32_t miny = l->n[1] - 1;
    std::int32_t minz = l->n[2] - 1;
    for (int corner = 0; corner < 2; ++corner) {
        std::int32_t idx[3];
        for (int i = 0; i < 3; ++i) {
            const double lo = l->box[i * 2];
            const double hi = l->box[i * 2 + 1];
            const double ci = box[corner * 3 + i];
            // comisd lo, c; jbe / comisd c, hi; jbe: a NaN coordinate stays NaN
            const double cv = (lo > ci) ? lo : ((ci > hi) ? hi : ci);
            const double d = std::fabs(cv - hi); // FUN_140484558
            if (!(1e-8 >= d)) { // DAT_14051bed0; comisd eps, d; jc (also taken for NaN)
                idx[i] = static_cast<std::int32_t>(((cv - lo) / (hi - lo)) * static_cast<double>(l->n[i]));
            } else {
                idx[i] = static_cast<std::int32_t>((((cv - lo) * 0.999) / (hi - lo)) * // DAT_140522f68
                                                   static_cast<double>(l->n[i]));
            }
        }
        if (maxx < idx[0]) maxx = idx[0];
        if (idx[0] < minx) minx = idx[0];
        if (maxy < idx[1]) maxy = idx[1];
        if (idx[1] < miny) miny = idx[1];
        if (maxz < idx[2]) maxz = idx[2];
        if (idx[2] < minz) minz = idx[2];
    }
    // No clamping of the index range here (the coordinates are clamped to the box instead).
    for (std::int32_t ix = minx; ix < maxx + 1; ++ix) {
        for (std::int32_t iy = miny; iy < maxy + 1; ++iy) {
            for (std::int32_t iz = minz; iz < maxz + 1; ++iz) {
                const std::int32_t k = ix + iy * l->n[0] + iz * l->n[0] * l->n[1];
                CellList* cell = &l->cells_poly[k];
                const std::int32_t cnt = fun_1404056c0(cell);
                for (std::int32_t j = 0; j < cnt; ++j) {
                    const std::int32_t pid = cell->data[j];
                    if (*exact == 0 || fun_1403ff870(&c->loc) == 5 || fun_1403ff870(&c->loc) == 6) {
                        fun_140405540(&c->result, 1, &pid);
                    } else {
                        // 140229675: exact box test of the polygon (FUN_1404040c0, p67-7 cube)
                        const std::int32_t* rec = fun_140405720(&c->polys, pid);
                        if (fun_1404040c0(&c->loc, box, rec, width) != 0) {
                            fun_140405540(&c->result, 1, &pid);
                        }
                    }
                }
            }
        }
    }
    *nc = fun_1404056c0(&c->result);
    if (v != 0) {
        std::fprintf(out(), "...leaving function\n"); // str 1405e03d8
    }
}

// @src 140231d00  qsort comparator: ascending ints
int fun_140231d00(const void* a, const void* b)
{
    const std::int32_t x = *static_cast<const std::int32_t*>(a);
    const std::int32_t y = *static_cast<const std::int32_t*>(b);
    if (y < x) {
        return 1;
    }
    if (x == y) {
        return 0;
    }
    return -1;
}

// @src 140229f30  cfnCUBE_GetPoly_Obj(&id, &sub, list, &verbose): copy the result list of the
// last query into list and sort it ascending
void fun_140229f30(std::int32_t* id, std::int32_t* sub, std::int32_t* list, std::int32_t* verbose)
{
    const std::int32_t v = *verbose;
    std::int32_t slot = -1;
    for (std::int32_t i = 0; i < g_cube_used; ++i) {
        if (g_cubes[i].id == *id && g_cubes[i].sub == *sub) {
            slot = i;
            break;
        }
    }
    if (slot == -1) {
        // str 1405e0570
        fun_14039d450("cfnCUBE_GetPoly_Obj : Illegal combination of object number (%d) and entity set number "
                      "(%d) \n",
                      *id, *sub);
    }
    Cube* c = &g_cubes[slot];
    if (fun_1403ff870(&c->loc) != 1 && fun_1403ff870(&c->loc) != 3 && fun_1403ff870(&c->loc) != 5 &&
        fun_1403ff870(&c->loc) != 6) {
        fun_14039d450("defCUBE_GetPoly_Obj : Locator type not for polygon seraching\n"); // str 1405e05d0
    }
    if (v != 0) {
        std::fprintf(out(), "...constructing polygon list from bucketdata list\n"); // str 1405e0610
    }
    const std::int32_t n = fun_1404056c0(&c->result);
    const std::int32_t* src = nullptr; // local_10 (uninitialised in the binary when n < 1)
    if (0 < n) {
        src = fun_140405720(&c->result, 1);
        std::memcpy(list, src, static_cast<std::size_t>(static_cast<std::int64_t>(n) << 2));
    }
    if (v != 0) {
        std::fprintf(out(), "...sorting list of valid search results\n"); // str 1405e0648
    }
    // The binary calls the CRT qsort (not stable) with 140231d00. For ints that compare equal only
    // when they are equal, any correct sort gives the same array, so std::sort is equivalent here.
    if (0 < n) {
        std::sort(list, list + n, [](std::int32_t a, std::int32_t b) { return fun_140231d00(&a, &b) < 0; });
    }
    if (v != 0) {
        std::fprintf(out(), "...leaving function, found %d elements\n", n); // str 1405e0678
    }
    if (2 < v) {
        std::fprintf(out(), "... element list\n\n"); // str 1405e06a0
        for (std::int32_t i = 0; i < n; ++i) {
            std::fprintf(out(), "%d-", src[i]); // str 1405df3dc (the unsorted result list)
        }
        std::fprintf(out(), "\n"); // str 1405df50c
    }
}

} // namespace def::cmd::cube
