// locator.cpp: defLOCD, the bucket locator of a cube: initialisation, divisions, cell lists and
// polygon insertion (types 1/3/5/6). Spec: docs/re/commands/OBJPOS_interference_kernels.md 2.2-2.3.
// Floating-point order follows the listing (out/p672/all_lst.txt).
#include <defcmd/ifd/cube_lib.hpp>

#include <defrt/stdlib/legacy_printf.hpp>

#include <cmath>
#include <cstdio>
#include <cstdlib>

namespace def::cmd::cube {

namespace {
std::FILE* out() { return stdout; } // FUN_14047903c() + 0x30 = &_iob[1]
} // namespace

// @src 1403ff870  locator type
std::int32_t fun_1403ff870(const Locator* l) { return l->type; }

// @src 1403fed00  defLOCD_Initialize_Obj(loc, type, tol, offset): defaults, 50^3 divisions
void fun_1403fed00(Locator* l, std::int32_t type, double tol, std::int32_t offset)
{
    if (type < 0 || 7 < type) {
        fun_14039d450("defLOCD_Initialize_Obj : Invalid locator type %d\n", type); // str 140626a80
    }
    l->type = type;
    l->ncells = 0;
    l->cell_inc = 5;
    l->u1c = 1;
    l->u20 = 0;
    l->tol = tol;
    l->tol2 = tol * tol;
    l->h[2] = 0.0;
    l->h[1] = 0.0;
    l->h[0] = 0.0;
    l->pts = nullptr;
    l->cells_a8 = nullptr;
    l->cells_poly = nullptr;
    l->cells_b8 = nullptr;
    l->cells_c0 = nullptr;
    l->offset = offset < 1 ? 1 : offset;
    for (int i = 0; i < 4; ++i) {
        l->ud0[i] = nullptr;
    }
    fun_1403ff890(l, 50, 50, 50);
    fun_1404050b0(&l->set80, 0, 100, 3);
}

// @src 1403ff890  divisions, each clamped to [1, 100]
void fun_1403ff890(Locator* l, std::int32_t nx, std::int32_t ny, std::int32_t nz)
{
    l->n[0] = nx < 1 ? 1 : (100 < nx ? 100 : nx);
    l->n[1] = ny < 1 ? 1 : (100 < ny ? 100 : ny);
    l->n[2] = nz < 1 ? 1 : (100 < nz ? 100 : nz);
}

// @src 1403ff970  defLOCD_InitPointInsertion_Obj: box from the bounds (hi <= lo -> lo + 1;
// extent < 0.001 -> hi + 0.001), cell lists per type, cell size H, insertion level
void fun_1403ff970(Locator* l, PointSet* pts, const double* bounds, std::int32_t verbose)
{
    if (verbose != 0) {
        std::fprintf(out(), "\tdefLOCD_InitPointInsertion_Obj\n"); // str 140626bf8
    }
    l->u20 = 0;
    l->pts = pts;
    for (int i = 0; i < 3; ++i) {
        l->box[i * 2] = bounds[i * 2];
        l->box[i * 2 + 1] = bounds[i * 2 + 1];
        if (l->box[i * 2] >= l->box[i * 2 + 1]) { // comisd lo, hi; jc (NaN: no change)
            l->box[i * 2 + 1] = l->box[i * 2] + 1.0; // DAT_140509c90
        }
        const double d = std::fabs(l->box[i * 2 + 1] - l->box[i * 2]); // FUN_140484558
        if (0.001 > d) { // DAT_14051f1e8; comisd eps, d; jbe
            l->box[i * 2 + 1] = l->box[i * 2 + 1] + 0.001;
        }
    }
    l->ncells = 1;
    for (int i = 0; i < 3; ++i) {
        l->ncells = l->ncells * l->n[i];
    }
    if (l->type == 0) {
        l->cells_a8 = fun_1404057a0(l->ncells);
    }
    if (l->type == 1 || l->type == 5) {
        l->cells_poly = fun_1404057a0(l->ncells);
    }
    if (l->type == 2) {
        l->cells_b8 = fun_1404057a0(l->ncells);
    }
    if (l->type == 3 || l->type == 6) {
        l->cells_a8 = fun_1404057a0(l->ncells);
        l->cells_poly = fun_1404057a0(l->ncells);
    }
    if (l->type == 4) {
        l->cells_a8 = fun_1404057a0(l->ncells);
        l->cells_b8 = fun_1404057a0(l->ncells);
    }
    if (l->type == 7) {
        l->cells_c0 = fun_1404057a0(l->ncells);
    }
    for (std::int32_t i = 0; i < l->ncells; ++i) {
        if (l->type == 0) {
            fun_1404050b0(&l->cells_a8[i], 0, l->cell_inc, 1);
        }
        if (l->type == 1 || l->type == 5) {
            fun_1404050b0(&l->cells_poly[i], 0, l->cell_inc, 1);
        }
        if (l->type == 2) {
            fun_1404050b0(&l->cells_b8[i], 0, l->cell_inc, 1);
        }
        if (l->type == 3 || l->type == 6) {
            fun_1404050b0(&l->cells_a8[i], 0, l->cell_inc, 1);
            fun_1404050b0(&l->cells_poly[i], 0, l->cell_inc, 1);
        }
        if (l->type == 4) {
            fun_1404050b0(&l->cells_a8[i], 0, l->cell_inc, 1);
            fun_1404050b0(&l->cells_b8[i], 0, l->cell_inc, 1);
        }
        if (l->type == 7) {
            fun_1404050b0(&l->cells_c0[i], 0, l->cell_inc, 2);
        }
    }
    for (int i = 0; i < 3; ++i) {
        l->h[i] = (l->box[i * 2 + 1] - l->box[i * 2]) / static_cast<double>(l->n[i]);
    }
    l->tol2 = l->tol * l->tol;
    std::int32_t nmax = 0;
    double hmin = 1e29; // DAT_14052c598
    for (int i = 0; i < 3; ++i) {
        hmin = (hmin > l->h[i]) ? l->h[i] : hmin; // comisd hmin, h; jbe
        nmax = (l->n[i] < nmax) ? nmax : l->n[i];
    }
    // FUN_140486e8c is the CRT ceil; std::ceil gives the same value for every finite and infinite
    // argument (the CRT routes NaN through its matherr handler).
    l->ins_level = static_cast<std::int32_t>(std::ceil(l->tol / hmin));
    l->ins_level = (nmax < l->ins_level) ? nmax : l->ins_level;
    if (verbose != 0) {
        // str 140626c20 .. 140626cc0
        std::fprintf(out(), "\tNumber of buckets : %d\n", l->ncells);
        std::fprintf(out(), "\tInsertion level   : %d\n", l->ins_level);
        def::rt::crt::fprintf(out(), "\tInsertion tol     : %g\n", l->tol);
        def::rt::crt::fprintf(out(), "\tH1                : %g\n", l->h[0]);
        def::rt::crt::fprintf(out(), "\tH2                : %g\n", l->h[1]);
        def::rt::crt::fprintf(out(), "\tH3                : %g\n", l->h[2]);
    }
}

// @src 1403feec0  free the locator's lists (cells of its type, then every pointer)
void fun_1403feec0(Locator* l)
{
    fun_140405150(&l->set80);
    switch (l->type) {
    case 0:
        if (l->cells_a8 != nullptr) {
            for (std::int32_t i = 0; i < l->ncells; ++i) fun_140405150(&l->cells_a8[i]);
        }
        break;
    case 1:
    case 5:
        if (l->cells_poly != nullptr) {
            for (std::int32_t i = 0; i < l->ncells; ++i) fun_140405150(&l->cells_poly[i]);
        }
        break;
    case 2:
        if (l->cells_b8 != nullptr) {
            for (std::int32_t i = 0; i < l->ncells; ++i) fun_140405150(&l->cells_b8[i]);
        }
        break;
    case 3:
    case 6:
        if (l->cells_a8 != nullptr) {
            for (std::int32_t i = 0; i < l->ncells; ++i) fun_140405150(&l->cells_a8[i]);
        }
        if (l->cells_poly != nullptr) {
            for (std::int32_t i = 0; i < l->ncells; ++i) fun_140405150(&l->cells_poly[i]);
        }
        break;
    case 4:
        if (l->cells_a8 != nullptr) {
            for (std::int32_t i = 0; i < l->ncells; ++i) fun_140405150(&l->cells_a8[i]);
        }
        if (l->cells_b8 != nullptr) {
            for (std::int32_t i = 0; i < l->ncells; ++i) fun_140405150(&l->cells_b8[i]);
        }
        break;
    case 7:
        // The binary tests the +0xb0 pointer here but frees the cells at +0xc0 (kept as is: for
        // type 7, +0xb0 is NULL, so the cell data leak).
        if (l->cells_poly != nullptr) {
            for (std::int32_t i = 0; i < l->ncells; ++i) fun_140405150(&l->cells_c0[i]);
        }
        break;
    default:
        break;
    }
    std::free(l->cells_a8);
    l->cells_a8 = nullptr;
    std::free(l->cells_poly);
    l->cells_poly = nullptr;
    std::free(l->cells_b8);
    l->cells_b8 = nullptr;
    std::free(l->cells_c0);
    l->cells_c0 = nullptr;
    for (int i = 0; i < 4; ++i) {
        std::free(l->ud0[i]);
        l->ud0[i] = nullptr;
    }
}

namespace {

// part of @src 140401050: cell index of one coordinate, the 0.999 factor within 1e-6 of hi
std::int32_t insert_index(const Locator* l, int i, double c)
{
    const double d = std::fabs(c - l->box[i * 2 + 1]); // FUN_140484558
    if (!(1e-6 >= d)) { // DAT_14051f1b8; comisd eps, d; jc (also taken for NaN)
        return static_cast<std::int32_t>(((c - l->box[i * 2]) / (l->box[i * 2 + 1] - l->box[i * 2])) *
                                         static_cast<double>(l->n[i]));
    }
    return static_cast<std::int32_t>((((c - l->box[i * 2]) * 0.999) / // DAT_140522f68
                                      (l->box[i * 2 + 1] - l->box[i * 2])) *
                                     static_cast<double>(l->n[i]));
}

// part of @src 140401050: append id to cell (ix, iy, iz), (re)initialising an empty cell
void append_to_cell(Locator* l, std::int32_t ix, std::int32_t iy, std::int32_t iz, const std::int32_t* id)
{
    const std::int32_t k = ix + iy * l->n[0] + iz * l->n[0] * l->n[1];
    CellList* c = &l->cells_poly[k];
    if (fun_1404056c0(c) == 0) {
        fun_1404050b0(c, 10, 50, 1);
    }
    fun_140405540(c, 1, id);
}

} // namespace

// @src 140401050  defLOCD_InsertPolygon_Obj(loc, nodes, np, id, verbose): index box of the
// polygon's points; cells tested with pcube when the normal is not ~0 and a span exceeds 2
std::int64_t fun_140401050(Locator* l, const std::int32_t* nodes, std::int32_t np, std::int32_t id,
                           std::int32_t verbose)
{
    if (verbose != 0) {
        std::fprintf(out(), "\tdefLOCD_InsertPolygon_Obj\n"); // str 140626f98
    }
    if (l->type != 1 && l->type != 3 && l->type != 5 && l->type != 6) {
        fun_14039d450("defLOCD_InsertPolygon_Obj : Invalid locator type %d\n", l->type); // str 140626fb8
    }
    const std::int32_t id_rec = id; // local_res20
    std::int32_t maxz = 0;
    std::int32_t maxy = 0;
    std::int32_t maxx = 0;
    std::int32_t minx = l->ncells;
    std::int32_t miny = minx;
    std::int32_t minz = minx;
    double* v = static_cast<double*>(std::calloc(static_cast<std::size_t>(static_cast<std::int64_t>(np)), 0x18));
    if (v == nullptr) {
        return 0;
    }
    for (std::int32_t j = 0; j < np; ++j) {
        const std::int32_t node = nodes[j] < 0 ? -nodes[j] : nodes[j]; // FUN_140484ab8 (abs)
        const double* p = fun_140406300(l->pts, (node - l->offset) + 1);
        v[j * 3] = p[0];
        v[j * 3 + 1] = p[1];
        v[j * 3 + 2] = p[2];
        std::int32_t idx[3];
        for (int i = 0; i < 3; ++i) {
            idx[i] = insert_index(l, i, p[i]);
        }
        if (maxx < idx[0]) maxx = idx[0];
        if (idx[0] < minx) minx = idx[0];
        if (maxy < idx[1]) maxy = idx[1];
        if (idx[1] < miny) miny = idx[1];
        if (maxz < idx[2]) maxz = idx[2];
        if (idx[2] < minz) minz = idx[2];
    }
    minx = minx < 0 ? 0 : minx;
    miny = miny < 0 ? 0 : miny;
    minz = minz < 0 ? 0 : minz;
    maxx = (l->n[0] - 1 < maxx) ? l->n[0] - 1 : maxx;
    maxy = (l->n[1] - 1 < maxy) ? l->n[1] - 1 : maxy;
    maxz = (l->n[2] - 1 < maxz) ? l->n[2] - 1 : maxz;

    const Vec3 nrm = fun_140240d80(v, np);
    const Vec3 zero{0.0, 0.0, 0.0}; // local_5a0..590
    if (fun_14023aa90(nrm, zero, 1e-4) == 0 && // DAT_14051f250
        (2 < maxx - minx || 2 < maxy - miny || 2 < maxz - minz)) {
        const double lox = l->box[0];
        const double loy = l->box[2];
        const double loz = l->box[4];
        const double hx = l->h[0];
        const double hy = l->h[1];
        const double hz = l->h[2];
        double w[150]; // local_528: 150 doubles, room for 50 points (np is 4 here)
        for (std::int32_t ix = minx; ix <= maxx; ++ix) {
            for (std::int32_t iy = miny; iy <= maxy; ++iy) {
                for (std::int32_t iz = minz; iz <= maxz; ++iz) {
                    // cell centre: lo + ((double)i + 0.5) * H   (DAT_14051f240)
                    const double cx = lox + (static_cast<double>(ix) + 0.5) * hx;
                    const double cy = loy + (static_cast<double>(iy) + 0.5) * hy;
                    const double cz = loz + (static_cast<double>(iz) + 0.5) * hz;
                    for (std::int32_t j = 0; j < np; ++j) {
                        w[j * 3] = (v[j * 3] - cx) / (1.01 * hx); // DAT_140562a90
                        w[j * 3 + 1] = (v[j * 3 + 1] - cy) / (1.01 * hy);
                        w[j * 3 + 2] = (v[j * 3 + 2] - cz) / (1.01 * hz);
                    }
                    double n[3];
                    fun_140454110(n, np, w);
                    if (fun_1404537f0(np, w, n, 0, 0) == 1) {
                        append_to_cell(l, ix, iy, iz, &id_rec);
                    }
                }
            }
        }
    } else {
        for (std::int32_t ix = minx; ix <= maxx; ++ix) {
            for (std::int32_t iy = miny; iy <= maxy; ++iy) {
                for (std::int32_t iz = minz; iz <= maxz; ++iz) {
                    append_to_cell(l, ix, iy, iz, &id_rec);
                }
            }
        }
    }
    std::free(v);
    return 0;
}

} // namespace def::cmd::cube
