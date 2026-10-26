// defdbio/src/objcheck/volume.cpp: object volume for the TRGVOL check (140163540) and the
// PathScale Fortran kernels under it. Every floating-point expression follows the disassembly
// (out/p403/p403_all.lst); Fortran SAVE variables that only carry values inside one call are
// locals here.
// ctest-labels: p4-03
#include <defdbio/objcheck/objcheck.hpp>

#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <cstring>

#include "ext.hpp"
#include "raw.hpp"

namespace def::dbio::objcheck {

using namespace raw;

// @src 1404e29fa  triangle area (Heron). p = 3 points x,y,z. Side i joins point i and i+1 (3 -> 1).
void triangle_area(const double* p, double* area)
{
    double side[3];
    double perim = 0.0;                    // 140755468
    for (int i = 1; i <= 3; ++i) {
        int j = i + 1 > 3 ? 1 : i + 1;     // 140755474
        double d2 = 0.0;                   // 140755478
        for (int k = 1; k <= 3; ++k) {
            double d = p[3 * (i - 1) + (k - 1)] - p[3 * (j - 1) + (k - 1)];
            d2 = d * d + d2;               // 1404e2b1c..2b2a (and 2b5d..2b61)
        }
        if (0.0 > d2) d2 = 0.0;            // 1404e2b7a: COMISD 0, d2
        side[i - 1] = std::sqrt(d2);       // 140755490 + 8*(i-1)
        perim = side[i - 1] + perim;
    }
    // 1404e2b93..2bd5: h = s*0.5; ((h - s1) * h) then * (h - s2) then * (h - s3)
    double h = perim * 0.5;
    double t = (h - side[0]) * h;
    t = (h - side[1]) * t;
    t = (h - side[2]) * t;
    *area = t;
    if (0.0 > t) {
        *area = std::sqrt(0.0);
    } else {
        *area = std::sqrt(*area);
    }
}

// @src 1404cce5e  c = a x b; *len = |c|; c / |c| when |c| > 1e-16 (DAT_140bb2668), else *len = 0
void cross_normalize(const double* a, const double* b, double* c, double* len)
{
    double c0 = a[1] * b[2] - b[1] * a[2];
    c[0] = c0;
    double c1 = a[2] * b[0] - b[2] * a[0];
    double c2 = a[0] * b[1] - b[0] * a[1];
    double l = std::sqrt(c2 * c2 + (c1 * c1 + c0 * c0)); // 1404ccec4..ccedc
    c[2] = c2;
    c[1] = c1;
    *len = l;
    const double eps = 1.0000000168623835e-16; // 0x3c9cd2b2a0000000
    if (!(eps < l) && !std::isnan(l)) {        // 1404ccef3: JP/JC normalise, else fall through
        *len = 0.0;
        return;
    }
    double inv = 1.0 / *len;                   // DAT_14072ff68
    c[2] = c2 * inv;
    c[1] = c1 * inv;
    c[0] = c0 * inv;
}

// @src 1404d3d43  normal of the triangle p (3 points): (p2 - p1) x (p3 - p1), normalised
void triangle_normal(const double* p, double* n)
{
    double a[3];                 // 140730cf0
    double b[3];                 // 140730d10
    a[0] = p[3] - p[0];
    b[2] = p[8] - p[2];
    a[1] = p[4] - p[1];
    b[1] = p[7] - p[1];
    a[2] = p[5] - p[2];
    b[0] = p[6] - p[0];
    double len;                  // 140757aa0
    cross_normalize(a, b, n, &len);
}

// @src 14030355f  volume enclosed by a closed surface: sum over triangles of (p1 . n) * area / 3.
// A facet is a quad when stride == 4 and its first and fourth nodes differ; it is then split into
// four triangles around its centroid (vertex tables 1406ff020..28).
void surface_volume(const double* coords, const std::int32_t* numnp, const std::int32_t* facets,
                    const std::int32_t* stride, const std::int32_t* nfacets, double* volume)
{
    (void)numnp;
    const std::int32_t st = *stride;
    const std::int64_t ld = st < 0 ? 0 : st;
    *volume = 0.0;
    if (*nfacets < 1) return;
    auto node = [&](std::int64_t base, std::int32_t k) -> const double* {
        std::int32_t n = facets[base + k - 1];               // 1406fefe4
        return coords + 3 * (static_cast<std::int64_t>(n) - 1);
    };
    for (std::int32_t f = 1; f <= *nfacets; ++f) {           // 1406fef50
        const std::int64_t base = (static_cast<std::int64_t>(f) - 1) * ld;
        double tri[9];                                       // 1406fef60
        double area;                                         // 1406fefe8
        double nrm[3];                                       // 1406feff0
        if (st == 4 && facets[base] != facets[base + 3]) {
            double c[3] = {0.0, 0.0, 0.0};                   // 1406fefc0..d0
            for (std::int32_t k = 1; k <= 4; ++k) {
                const double* q = node(base, k);
                c[0] = q[0] + c[0];
                c[1] = q[1] + c[1];
                c[2] = q[2] + c[2];
            }
            c[0] = c[0] * 0.25;                              // DAT_140bae800
            c[1] = c[1] * 0.25;
            c[2] = 0.25 * c[2];
            for (std::int32_t t = 1; t <= 4; ++t) {          // 1406ff010
                static const std::int32_t va[5] = {0, 1, 2, 3, 4};
                static const std::int32_t vb[5] = {0, 2, 3, 4, 1};
                const std::int32_t v[3] = {va[t], vb[t], 5}; // 1406ff020..28
                for (int k = 1; k <= 3; ++k) {
                    const double* q = v[k - 1] == 5 ? c : node(base, v[k - 1]);
                    tri[3 * (k - 1) + 0] = q[0];
                    tri[3 * (k - 1) + 1] = q[1];
                    tri[3 * (k - 1) + 2] = q[2];
                }
                triangle_area(tri, &area);
                triangle_normal(tri, nrm);
                // 1403036f6..140303748: c2*n2 + (c1*n1 + c0*n0)
                double dot = tri[2] * nrm[2] + (tri[1] * nrm[1] + tri[0] * nrm[0]);
                double v3 = (dot * area) / 3.0;              // DAT_140bae7f8
                *volume = v3 + *volume;
            }
        } else {
            for (int k = 1; k <= 3; ++k) {
                const double* q = node(base, k);
                tri[3 * (k - 1) + 0] = q[0];
                tri[3 * (k - 1) + 1] = q[1];
                tri[3 * (k - 1) + 2] = q[2];
            }
            triangle_area(tri, &area);
            triangle_normal(tri, nrm);
            double dot = tri[2] * nrm[2] + (tri[1] * nrm[1] + tri[0] * nrm[0]);
            double v3 = (dot * area) / 3.0;
            *volume = v3 + *volume;
        }
    }
}

// @src 1404337cd  one quadrilateral p = 4 points (r, z): area and its volume of revolution
// 2*pi*r_c*area, computed in the frame of the edge from point 2 to point 4.
void quad_area_volume_1(const double* p, double* volume, double* area)
{
    const double x0 = p[2];
    const double y0 = p[3];
    double dx = p[6] - x0;
    double dy = p[7] - y0;
    const double a1x = p[0] - x0;
    const double a1y = p[1] - y0;
    const double a3x = p[4] - x0;
    const double a3y = p[5] - y0;
    const double len = std::sqrt(dy * dy + dx * dx);   // 140433853..865
    const double c = dx / len;                          // 14070d560
    const double s = dy / len;                          // 14070d568
    const double u1 = s * a1y + c * a1x;                // 14070d570
    const double v1 = a1y * c - a1x * s;                // 14070d578
    const double u3 = s * a3y + c * a3x;                // 14070d580
    const double v3 = a3y * c - a3x * s;                // 14070d588
    const double hl = len * 0.5;                        // DAT_140bb14b8
    const double w1 = v1 * hl;                          // 14043394a
    const double w3 = hl * v3;                          // 14043394f
    const double ar = w1 - w3;
    *area = ar;
    double nv3 = -v3;                                   // XORPD DAT_140bb14c8
    nv3 = nv3 / -3.0;                                   // DAT_140bb14d0
    double ty = (v1 / 3.0) * w1;                        // DAT_140bb14c0
    double tx = ((u1 + len) / 3.0) * w1;
    double tx3 = ((len + u3) / 3.0) * w3;
    tx = tx - tx3;
    tx = tx / ar;
    nv3 = nv3 * w3;
    ty = ty - nv3;
    ty = ty / ar;
    double r = tx * c;
    r = r + x0;
    double q = ty * s;
    r = r - q;
    r = r * ar;
    r = r * 2.0;                                        // DAT_140bb14d8
    r = r * 3.141590118408203;                          // DAT_140bb14e0 (3.14159 as REAL*4)
    *volume = r;
}

// @src 1404335e0  area and volume of revolution of numel 4-node elements (elems 1-based, 4 per
// element; coords r, z per node). Nothing is written when numnp < 4 or numel < 1.
void quad_area_volume(const double* coords, const std::int32_t* elems, const std::int32_t* numnp,
                      const std::int32_t* numel, double* volume, double* area, std::int32_t* ierr)
{
    *ierr = 1;
    if (!(3 < *numnp && 0 < *numel)) return;
    *volume = 0.0;
    *area = 0.0;
    for (std::int32_t e = 1; e <= *numel; ++e) {                 // 14070d500
        double q[8];                                             // 14070d510
        for (std::int32_t k = 1; k <= 4; ++k) {                  // 14070d504
            std::int32_t n = elems[(k + e * 4) - 5];             // 14070d508
            q[2 * (k - 1)] = coords[2 * (static_cast<std::int64_t>(n) - 1)];
            q[2 * (k - 1) + 1] = coords[2 * (static_cast<std::int64_t>(n) - 1) + 1];
        }
        double v;                                                // 14070d550
        double a;                                                // 14070d558
        quad_area_volume_1(q, &v, &a);
        double old_area = *area;
        *volume = *volume + v;
        *area = old_area + a;
    }
}

// @src 140306940  2D object: area and volume of revolution of its FE mesh
void obj_area_volume_2d(LLOBJ_DATA* obj, double* area, double* volume)
{
    std::int32_t ierr[6];
    *area = 0.0;
    *volume = 0.0;
    quad_area_volume(static_cast<const double*>(ptr(obj, 0x8a8)),
                     static_cast<const std::int32_t*>(ptr(obj, 0x9e8)), &i32(obj, 0x880),
                     &i32(obj, 0x884), volume, area, ierr);
}

// @src 140163540  object volume (2D: plane strain area, axisymmetric volume, 0 for plane stress
// (+0x850 == 4)). 3D: mode 0 from the geometry surface (u3c00[0]), mode 1 from the FE mesh's
// boundary facets (+0xbf0 / +0xbf8). Negative volumes are 0.
double obj_volume(LLOBJ_DATA* obj, std::int32_t mode)
{
    double vol = 0.0;                                            // local_68
    double res;                                                  // local_60
    void* step = owner(obj);
    if (!is2d(step)) {
        std::int32_t numnp = 0;                                  // local_40
        std::int32_t nfac = 0;                                   // local_3c[0]
        std::int32_t stride = 4;                                 // local_58
        const double* coords = nullptr;                          // local_48
        std::int32_t* conn = nullptr;                            // local_50
        if (mode == 0) {
            if (!ext::fun_140160d90(obj)) return vol;
            std::int32_t i0 = 0;
            numnp = ext::fun_1400034d0(ext::fun_140198620(obj, &i0));
            std::int32_t i1 = 0;
            nfac = ext::fun_140003500(ext::fun_140198620(obj, &i1));
            std::int32_t i2 = 0;
            coords = ext::fun_140006410(ext::fun_140198620(obj, &i2));
            // operator new(4 * (nfac << 2)) with the overflow guard of `new int[]`
            std::uint64_t bytes = static_cast<std::uint64_t>(static_cast<std::int64_t>(nfac << 2)) * 4u;
            conn = static_cast<std::int32_t*>(std::malloc(bytes == 0 ? 1 : bytes));
            std::int32_t n4 = nfac << 2;
            std::int32_t i3 = 0;
            const std::int32_t* src = ext::fun_140005510(ext::fun_140198620(obj, &i3));
            std::memcpy(conn, src, static_cast<std::size_t>(static_cast<std::int64_t>(n4) << 2));
            for (std::int32_t e = 0; e < nfac; ++e) {
                std::int32_t& v = conn[e * 4 + 3];
                if (v < 0) v = -v;
            }
        } else {
            if (mode != 1) return vol;
            if (i32(obj, 0x87c) == 0) return vol;
            numnp = i32(obj, 0x880);
            nfac = i32(obj, 0xbf0);
            coords = static_cast<const double*>(ptr(obj, 0x8a8));
            conn = static_cast<std::int32_t*>(ptr(obj, 0xbf8));
        }
        surface_volume(coords, &numnp, conn, &stride, &nfac, &vol);
        if (vol < 0.0) vol = 0.0;
        res = vol;
        if (mode == 0) std::free(conn);
    } else {
        res = 0.0;
        obj_area_volume_2d(obj, &res, &vol);
        if (i32(step, 0x850) != 2) {
            res = vol;
            if (i32(step, 0x850) == 4) {
                vol = 0.0;
                res = vol;
            }
        }
    }
    return res;
}

// @src 140304cb0  2D: number of nodes whose r is below -0.01 * max(r)
std::int32_t obj_negative_radius_nodes(LLOBJ_DATA* obj)
{
    std::int32_t n = 0;
    double rmax = 0.0;
    bool first = true;
    const double* xy = node_block_coords(add(obj, 0x8a0));
    for (std::int32_t i = 0; i < i32(obj, 0x880); ++i) {
        double r = xy[i * 2];
        if (first) {
            first = false;
            rmax = r;
        } else if (rmax < r) {
            rmax = r;
        }
    }
    for (std::int32_t i = 0; i < i32(obj, 0x880); ++i) {
        if (xy[i * 2] < -rmax * 0.01) ++n;   // XOR DAT_14051bec0, MUL DAT_14051f258
    }
    return n;
}

} // namespace def::dbio::objcheck
