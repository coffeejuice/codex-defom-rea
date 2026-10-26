// defLOCD insertion routines p67-2's library does not use: points (types 0, 3, 4, 6), control
// volumes (5, 6), edges (7), elements (2, 4), the exact polygon-box test, the surface-point map
// (packet p67-7, group "cube"; GENCTC_3d_search.md sec. 4.1, 4.2). MSVC C code: plain C order.
#include <defcmd/ctc3d/cube.hpp>

#include <cmath>
#include <cstdio>
#include <cstdlib>

#include <defrt/stdlib/legacy_printf.hpp>

namespace def::cmd::cube {

namespace {

inline i32 iabs(i32 v) { return v < 0 ? -v : v; } // 140484ab8

// cell index of x on axis k (the code repeated in every insertion routine): eps = 1e-8
// (14051bed0) for points, 1e-6 (14051f1b8) for polygons, elements and edges
inline i32 cell_index(const Locator* l, double x, i32 k, double eps)
{
    const double lo = l->box[k * 2];
    const double hi = l->box[k * 2 + 1];
    if (eps < std::fabs(x - hi)) {
        return static_cast<i32>(((x - lo) / (hi - lo)) * static_cast<double>(l->n[k]));
    }
    return static_cast<i32>((((x - lo) * 0.999) / (hi - lo)) * static_cast<double>(l->n[k])); // 140522f68
}

inline CellList* cell_at(CellList* cells, const Locator* l, i32 ix, i32 iy, i32 iz)
{
    return cells + static_cast<std::int64_t>(ix + iy * l->n[0] + iz * l->n[0] * l->n[1]);
}

struct CellRange {
    i32 lo[3];
    i32 hi[3];
};

// vertex cell range (abs node numbers), min start values lo0, clamped to [0, n_k - 1]
// (part of 140401c40, 140402c40)
CellRange vertex_range(Locator* l, const i32* conn, i32 nv, const i32 lo0[3])
{
    CellRange r{{lo0[0], lo0[1], lo0[2]}, {0, 0, 0}};
    for (i32 j = 0; j < nv; ++j) {
        const double* x = fun_140406300(l->pts, (iabs(conn[j]) - l->offset) + 1);
        i32 c[3];
        for (i32 k = 0; k < 3; ++k) {
            c[k] = cell_index(l, x[k], k, 1e-06);
        }
        for (i32 k = 0; k < 3; ++k) {
            if (r.hi[k] < c[k]) {
                r.hi[k] = c[k];
            }
            if (c[k] < r.lo[k]) {
                r.lo[k] = c[k];
            }
        }
    }
    for (i32 k = 0; k < 3; ++k) {
        r.lo[k] = r.lo[k] < 0 ? 0 : r.lo[k];
    }
    for (i32 k = 0; k < 3; ++k) {
        r.hi[k] = l->n[k] - 1 < r.hi[k] ? l->n[k] - 1 : r.hi[k];
    }
    return r;
}

void add_to_range(Locator* l, CellList* cells, const CellRange& r, i32 id)
{
    for (i32 ix = r.lo[0]; ix <= r.hi[0]; ++ix) {
        for (i32 iy = r.lo[1]; iy <= r.hi[1]; ++iy) {
            for (i32 iz = r.lo[2]; iz <= r.hi[2]; ++iz) {
                CellList* cell = cell_at(cells, l, ix, iy, iz);
                if (fun_1404056c0(cell) == 0) {
                    fun_1404050b0(cell, 10, 50, 1);
                }
                fun_140405540(cell, 1, &id);
            }
        }
    }
}

} // namespace

// @src 140233ef0  map[|conn[k]| - base] = 'T' for k = 0 .. nf*nv-1
void fun_140233ef0(char* map, const i32* conn, i32 nf, i32 nv, i32 base)
{
    for (i32 k = 0; k < nf * nv; ++k) {
        map[iabs(conn[k]) - base] = 'T';
    }
}

// @src 140400330  defLOCD_InsertPoint_Obj: id into the cell of x (no index clamp)
i32 fun_140400330(Locator* l, const double* x, i32 id, i32 verbose)
{
    if (verbose != 0) {
        std::fprintf(stdout, "\tdefLOCD_InsertPoint_Obj\n"); // str 140626da0
    }
    if (l->type != 0 && l->type != 3 && l->type != 4 && l->type != 6) {
        fun_14039d450("defLOCD_InsertPoint_Obj : Invalid locator type %d\n", l->type); // str 140626dc0
    }
    i32 c[3];
    for (i32 k = 0; k < 3; ++k) {
        c[k] = cell_index(l, x[k], k, 1e-08);
    }
    if (verbose != 0) {
        def::rt::crt::fprintf(stdout, "\tPoint  ptid %d   %15.5f %15.5f %15.5f\n", id, x[0], x[1], x[2]); // str 140626df8
        std::fprintf(stdout, "\tBucket %3d %3d %3d\n", c[0], c[1], c[2]);                              // str 140626e20
    }
    CellList* cell = cell_at(l->cells_a8, l, c[0], c[1], c[2]);
    if (fun_1404056c0(cell) == 0) {
        fun_1404050b0(cell, 10, 50, 1);
    }
    fun_140405540(cell, 1, &id);
    return 0;
}

// @src 140401c40  defLOCD_InsertPolygon_ControlVolume: every cell of the vertex range (min start
// = cell count)
i32 fun_140401c40(Locator* l, const i32* conn, i32 nv, i32 id, i32 verbose)
{
    if (verbose != 0) {
        std::fprintf(stdout, "\tdefLOCD_InsertPolygon_ControlVolume\n"); // str 140626ff0
    }
    if (l->type != 1 && l->type != 3 && l->type != 5 && l->type != 6) {
        fun_14039d450("defLOCD_InsertPolygon_ControlVolume : Invalid locator type %d\n", l->type); // str 140627018
    }
    const i32 lo0[3] = {l->ncells, l->ncells, l->ncells};
    add_to_range(l, l->cells_poly, vertex_range(l, conn, nv, lo0), id);
    return 0;
}

// @src 140402c40  defLOCD_InsertElement_Obj: every cell of the vertex range (min start = n_k)
i32 fun_140402c40(Locator* l, const i32* conn, i32 nv, i32 id, i32 verbose)
{
    if (verbose != 0) {
        std::fprintf(stdout, "\tdefLOCD_InsertElement_Obj\n"); // str 1406270b0
    }
    if (l->type != 2 && l->type != 4) {
        fun_14039d450("defLOCD_InsertElement_Obj : Invalid locator type %d\n", l->type); // str 1406270d0
    }
    const i32 lo0[3] = {l->n[0], l->n[1], l->n[2]};
    add_to_range(l, l->cells_b8, vertex_range(l, conn, nv, lo0), id);
    return 0;
}

// @src 1404021a0  defLOCD_InsertEdge_Obj: the 2-node record into the cells of its range (node
// numbers without abs); with more than 3 cells on some axis only the cells the segment meets
// (140453c40, vertices mapped to (x - centre) / (1.01 h))
i32 fun_1404021a0(Locator* l, const i32* conn, i32 verbose)
{
    if (verbose != 0) {
        std::fprintf(stdout, "\tdefLOCD_InsertEdge_Obj\n"); // str 140627058
    }
    if (l->type != 7) {
        fun_14039d450("defLOCD_InsertEdge_Obj : Invalid locator type %d\n", l->type); // str 140627078
    }
    i32 lo[3] = {l->ncells, l->ncells, l->ncells};
    i32 hi[3] = {0, 0, 0};
    double b[6];
    for (i32 j = 0; j < 2; ++j) {
        const double* x = fun_140406300(l->pts, (conn[j] - l->offset) + 1);
        b[j * 3] = x[0];
        b[j * 3 + 1] = x[1];
        b[j * 3 + 2] = x[2];
        i32 c[3];
        for (i32 k = 0; k < 3; ++k) {
            c[k] = cell_index(l, x[k], k, 1e-06);
        }
        for (i32 k = 0; k < 3; ++k) {
            if (hi[k] < c[k]) {
                hi[k] = c[k];
            }
            if (c[k] < lo[k]) {
                lo[k] = c[k];
            }
        }
    }
    for (i32 k = 0; k < 3; ++k) {
        lo[k] = lo[k] < 0 ? 0 : lo[k];
    }
    for (i32 k = 0; k < 3; ++k) {
        hi[k] = l->n[k] - 1 < hi[k] ? l->n[k] - 1 : hi[k];
    }
    auto add = [l, conn](i32 ix, i32 iy, i32 iz) {
        CellList* cell = cell_at(l->cells_c0, l, ix, iy, iz);
        if (fun_1404056c0(cell) == 0) {
            fun_1404050b0(cell, 10, 50, cell->width);
        }
        fun_140405540(cell, cell->width, conn);
    };
    if (hi[0] - lo[0] < 3 && hi[1] - lo[1] < 3 && hi[2] - lo[2] < 3) {
        for (i32 ix = lo[0]; ix <= hi[0]; ++ix) {
            for (i32 iy = lo[1]; iy <= hi[1]; ++iy) {
                for (i32 iz = lo[2]; iz <= hi[2]; ++iz) {
                    add(ix, iy, iz);
                }
            }
        }
        return 0;
    }
    const double x0 = l->box[0], y0 = l->box[2], z0 = l->box[4];
    const double hx = l->h[0], hy = l->h[1], hz = l->h[2];
    for (i32 ix = lo[0]; ix <= hi[0]; ++ix) {
        for (i32 iy = lo[1]; iy <= hi[1]; ++iy) {
            for (i32 iz = lo[2]; iz <= hi[2]; ++iz) {
                const double cx = x0 + (static_cast<double>(ix) + 0.5) * hx;
                const double cy = y0 + (static_cast<double>(iy) + 0.5) * hy;
                const double cz = z0 + (static_cast<double>(iz) + 0.5) * hz;
                double a[3], e[3];
                a[0] = (b[0] - cx) / (1.01 * hx); // 140562a90
                a[1] = (b[1] - cy) / (1.01 * hy);
                a[2] = (b[2] - cz) / (1.01 * hz);
                e[0] = (b[3] - cx) / (1.01 * hx);
                e[1] = (b[4] - cy) / (1.01 * hy);
                e[2] = (b[5] - cz) / (1.01 * hz);
                if (fun_140453c40(a, e) == 1) {
                    add(ix, iy, iz);
                }
            }
        }
    }
    return 0;
}

// @src 1404040c0  exact test of polygon conn against box [min3, max3]: vertices mapped to
// (v - (lo + 0.5*(hi - lo))) / (1.0001*(hi - lo)) (140562a98), then 140454110 + 1404537f0
i32 fun_1404040c0(Locator* l, const double* box, const i32* conn, i32 nv)
{
    if (l->type != 1 && l->type != 3 && l->type != 5 && l->type != 6) {
        fun_14039d450("defLOCD_InsertPolygon : Invalid locator type %d\n", l->type); // str 140627208
    }
    auto* v = static_cast<double*>(std::calloc(static_cast<std::size_t>(static_cast<std::int64_t>(nv)), 0x18));
    if (v == nullptr) {
        return 0;
    }
    for (i32 j = 0; j < nv; ++j) {
        const double* x = fun_140406300(l->pts, (iabs(conn[j]) - l->offset) + 1);
        v[j * 3] = x[0];
        v[j * 3 + 1] = x[1];
        v[j * 3 + 2] = x[2];
    }
    double u[151]; // local_4b8
    for (i32 j = 0; j < nv; ++j) {
        u[j * 3] = (v[j * 3] - (box[0] + 0.5 * (box[3] - box[0]))) / (1.0001 * (box[3] - box[0]));
        u[j * 3 + 1] = (v[j * 3 + 1] - (box[1] + 0.5 * (box[4] - box[1]))) / (1.0001 * (box[4] - box[1]));
        u[j * 3 + 2] = (v[j * 3 + 2] - (box[2] + 0.5 * (box[5] - box[2]))) / (1.0001 * (box[5] - box[2]));
    }
    double pn[3];
    fun_140454110(pn, nv, u);
    const i32 r = fun_1404537f0(nv, u, pn, 0, 0);
    std::free(v);
    return r;
}

} // namespace def::cmd::cube
