// GEOPRIM helpers shared by the extrude and the revolve (packet p9-04). Spec: CNVT3D_SWEEP.md
// 10.5, 10.6. Decompiles and listings: deform-rea-work/out/p904/all.c, all.asm.
#include <defmodel/geoprim/extrude.hpp>

#include <defmodel/geo_kw/boundary.hpp>
#include <defmodel/geo_kw/geo_kw.hpp>
#include <defmodel/geo_kw/surface_model.hpp>
#include <defmodel/geoprim/polygon.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/layout/obj_geo3d.hpp>
#include <defmodel/mesh2d/geo2d_mesh_ops.hpp>
#include <defmodel/port.hpp>
#include <defmodel/sdctl/cow.hpp>

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <vector>

namespace def::model::geoprim {

namespace {

double stub_polygon_area(std::int32_t, const double*) { DEF_NOT_PORTED(0x14023e670); }
double stub_dist2(const double*, const double*) { DEF_NOT_PORTED(0x14023b850); }
double stub_dot(const double*, const double*) { DEF_NOT_PORTED(0x14023a380); }
void stub_poly_normal(double*, const double*, std::int32_t) { DEF_NOT_PORTED(0x140240d80); }
void stub_surface_size(LLOBJ_DATA*, std::int32_t, std::int32_t, std::int32_t) { DEF_NOT_PORTED(0x1402009f0); }
std::int32_t* stub_mesh_elem_u018(OBJ_MESH2D*) { DEF_NOT_PORTED(0x140220480); }

P904Ext g_ext = {stub_polygon_area, stub_dist2, stub_dot, stub_poly_normal, stub_surface_size,
                 stub_mesh_elem_u018};

double crt_fabs(double x) { return std::fabs(x); }   // 140484558

constexpr double kOne = 1.0;     // DAT_140509c90
constexpr double kEps6 = 1e-6;   // DAT_14051f1b8
constexpr double kHund = 0.01;   // DAT_14051f258

// operator new with the callers' saturating byte count (SUB168(ZEXT816(k) * n))
template <class T>
T* alloc_n(std::int64_t n)
{
    return cow_detail::alloc<T>(static_cast<std::size_t>(n));
}

} // namespace

P904Ext& p904_ext() { return g_ext; }

// ---- accessors -------------------------------------------------------------------------------

// @src 1402e9b50
std::int32_t fun_1402e9b50(DEF_GEOPRIM* prim) { return mesh2d_count_08(prim); }
// @src 1402e9b70
std::int32_t fun_1402e9b70(DEF_GEOPRIM* prim) { return mesh2d_count_148(prim); }
// @src 1402e9b90
double* fun_1402e9b90(DEF_GEOPRIM* prim) { return ext().mesh_coords(prim); }
// @src 1402e9bb0
std::int32_t* fun_1402e9bb0(DEF_GEOPRIM* prim) { return ext().mesh_conn(prim); }
// @src 1402e9bd0
std::int32_t* fun_1402e9bd0(DEF_GEOPRIM* prim) { return p904_ext().mesh_elem_u018(prim); }
// @src 1402f07e0
bool fun_1402f07e0(const DEF_GEOPRIM* prim) { return prim->u24b9 != 0; }

// @src 1402e91d0
bool fun_1402e91d0(const IntArray* a)
{
    if (a->n < 1 || a->data == nullptr) return false;
    return a->data[0] == a->data[a->n - 1];
}

// @src 1402e9470
void fun_1402e9470(IntArrayList* l, const std::int32_t* k, const std::int32_t* count)
{
    if (0 < *k && *k <= l->n) l->items[*k - 1].resize(*count);
}

// ---- helpers ---------------------------------------------------------------------------------

// @src 1402caac0  sections uniform
// (callee 1402f0a40)  v2490 size (140003090)
// (callee 140005e70)  CowVector data for writing (detach)
bool fun_1402caac0(DEF_GEOPRIM* prim)
{
    if (prim->u2388 < kOne) prim->u2388 = kOne;   // 1402caad1: x <= 1 && x != 1
    if (!(kEps6 < crt_fabs(prim->u2388 - kOne))) return true;
    if (prim->kind != 2) return false;
    if (prim->v2490.size() != 0) return false;
    const double v = *prim->v2398.mutable_begin();
    if (!(kHund < crt_fabs(v))) return true;
    if (prim->revolve != nullptr) {
        const double w = *prim->v2390.mutable_begin();
        if (crt_fabs((w - prim->angle) - prim->revolve->start) <= kEps6) return true;
    }
    return false;
}

// @src 1402cc080  boundary chains of the 2D mesh
void fun_1402cc080(DEF_GEOPRIM* prim)
{
    const std::int32_t np = fun_1402e9b50(prim);
    std::int32_t ne = fun_1402e9b70(prim);
    const std::int32_t* conn = fun_1402e9bb0(prim);
    const std::int32_t nper = 4;   // local_58
    std::int32_t* ebuf = alloc_n<std::int32_t>(static_cast<std::int64_t>(ne * 4));
    for (std::int32_t e = 0; e < ne; ++e)
        std::memcpy(ebuf + e * nper, conn + (e << 2), static_cast<std::size_t>(nper) << 2);
    // The binary allocates np + 200 ints for the chains and keeps 200 ints of ranges on the
    // stack; FL2_FNDBND needs 6 * ne + 2 and 8 * ne. The port gives it at least that much (the
    // contents are the same; only an overflow of the original is avoided).
    const std::int64_t nn = std::max<std::int64_t>(np + 200, 6 * static_cast<std::int64_t>(ne) + 2);
    std::int32_t* nodes = alloc_n<std::int32_t>(nn);
    std::vector<std::int32_t> ranges(static_cast<std::size_t>(std::max(200, 8 * ne + 2)), 0);
    std::int32_t nchain = 0;
    (void)geo_kw::fl2_fndbnd(&ne, ebuf, &nchain, ranges.data(), nodes);   // 14032d65e
    prim->int_lists.resize(&nchain);                                      // 1402e9370
    for (std::int32_t i = 0; i < prim->int_lists.n && i < 100; ++i) {
        std::int32_t len = (ranges[static_cast<std::size_t>(i * 2 + 1)] -
                            ranges[static_cast<std::size_t>(i << 1)]) + 1;
        std::int32_t k = i + 1;
        fun_1402e9470(&prim->int_lists, &k, &len);
        std::memcpy(prim->int_lists.items[i].data, nodes + (ranges[static_cast<std::size_t>(i << 1)] - 1),
                    static_cast<std::size_t>(static_cast<std::int64_t>(len) << 2));
    }
    std::free(nodes);
    std::free(ebuf);
}

// @src 1402bc720  normal of the first 2D element points to +z
// (callee 140239700)  vec3 from 3 doubles (inlined)
bool fun_1402bc720(DEF_GEOPRIM* prim, const double* xyz)
{
    const double ez[3] = {0.0, 0.0, kOne};
    double* pts = alloc_n<double>(4 * 3);
    const std::int32_t* conn = fun_1402e9bb0(prim);
    for (std::int32_t k = 0; k < 4; ++k) {
        const std::int32_t id = conn[k] - 1;
        pts[k * 3] = xyz[id * 3];
        pts[k * 3 + 1] = xyz[id * 3 + 1];
        pts[k * 3 + 2] = xyz[id * 3 + 2];
    }
    double n[3];
    p904_ext().poly_normal(n, pts, 4);
    const double d = p904_ext().dot(n, ez);
    bool r = false;
    if (0.0 < d) r = true;   // DAT_14051bed8
    std::free(pts);
    return r;
}

namespace {
inline void swap_ac(std::int32_t* q)
{
    // 1402bca0x: three XORs of +0 and +8
    q[0] = q[0] ^ q[2];
    q[2] = q[2] ^ q[0];
    q[0] = q[0] ^ q[2];
}
} // namespace

// @src 1402bc980  shell quads
std::int32_t fun_1402bc980(DEF_GEOPRIM* prim, const std::int32_t* nsec, const double* xyz,
                           std::int32_t* quads, const char* flip, const char* closed)
{
    (void)xyz;
    std::int32_t nq = 0;
    const std::int32_t np = fun_1402e9b50(prim);
    const std::int32_t ne = fun_1402e9b70(prim);
    const std::int32_t* conn = fun_1402e9bb0(prim);
    if (*closed == 0) {
        for (std::int32_t e = 0; e < ne; ++e) {
            std::memcpy(quads + nq * 4, conn + e * 4, 0x10);
            if (*flip != 0) swap_ac(quads + nq * 4);
            ++nq;
        }
    }
    IntArray* b = prim->int_lists.items;
    std::int32_t nb = b->n;
    if (fun_1402e91d0(b)) {
        nb = nb - 1;
        b->n = b->n - 1;
    }
    std::int32_t base = np;
    for (std::int32_t s = 1; s <= *nsec; ++s) {
        for (std::int32_t j = 0; j < nb; ++j) {
            const std::int32_t b1 = b->data[j];
            const std::int32_t jn = (j == nb - 1) ? 0 : j + 1;
            const std::int32_t b2 = b->data[jn];
            std::int32_t* q = quads + nq * 4;
            if (s == 1) {
                if (*nsec == 1) {
                    q[0] = b1;
                    q[1] = b1 + np;
                    q[2] = b2 + np;
                    q[3] = b2;
                } else {
                    q[0] = b1;
                    q[1] = base + 1 + j;
                    q[2] = base + 1 + jn;
                    q[3] = b2;
                }
            } else if (s == *nsec) {
                if (*closed == 0) {
                    q[0] = ((j + base) - nb) + 1;
                    q[1] = b1 + base;
                    q[2] = b2 + base;
                    q[3] = ((jn + base) - nb) + 1;
                } else {
                    q[0] = ((j + base) - nb) + 1;
                    q[1] = b1;
                    q[2] = b2;
                    q[3] = ((jn + base) - nb) + 1;
                }
            } else {
                q[0] = (base + 1 + j) - nb;
                q[1] = q[0] + nb;
                q[2] = base + 1 + jn;
                q[3] = q[2] - nb;
            }
            if (*flip != 0) swap_ac(q);
            ++nq;
        }
        base = base + nb;
    }
    if (*closed == 0) {
        base = base - nb;
        for (std::int32_t e = 0; e < ne; ++e) {
            std::int32_t* q = quads + nq * 4;
            q[0] = conn[e * 4] + base;
            q[1] = conn[e * 4 + 1] + base;
            q[2] = conn[e * 4 + 2] + base;
            q[3] = conn[e * 4 + 3] + base;
            if (*flip == 0) swap_ac(q);
            ++nq;
        }
    }
    return nq;
}

// @src 1402dbe70  store surface 0
void fun_1402dbe70(DEF_GEOPRIM* prim, const std::int32_t* np, const double* xyz,
                   const std::int32_t* nf, const std::int32_t* quads)
{
    LLOBJ_DATA* o = prim->obj;
    if (o == nullptr) return;
    for (std::int32_t i = 0; i < geo_kw::surface_count(*o); ++i)   // 1401985a0
        geo_kw::surface(*o, i)->clear();                             // 140198620, 1402f9340
    geo_kw::set_surface_count(*o, 1);                                // 140198600
    geo_kw::set_surface_flag(*o, true);                              // 1401985e0
    p904_ext().surface_size(o, *np, *nf, 0);                         // 1402009f0
    double* c = geo_kw::geo3d_coords(geo_kw::surface(*o, 0));        // 140006410
    std::int32_t* f = geo_kw::geo3d_elems(geo_kw::surface(*o, 0));   // 140005510
    for (std::int32_t i = 0; i < *np; ++i) std::memcpy(c + i * 3, xyz + i * 3, 0x18);
    for (std::int32_t i = 0; i < *nf; ++i) std::memcpy(f + (i << 2), quads + i * 4, 0x10);
}

// @src 1402dc040  triangles -> quads
void fun_1402dc040(DEF_GEOPRIM* prim, const std::int32_t* np, const double* xyz,
                   const std::int32_t* ntri, const std::int32_t* tris)
{
    std::int32_t* q = alloc_n<std::int32_t>(static_cast<std::int64_t>(*ntri << 2));
    for (std::int32_t i = 0; i < *ntri; ++i) {
        std::memcpy(q + i * 4, tris + i * 3, 0xc);
        q[i * 4 + 3] = q[i * 4];
    }
    fun_1402dbe70(prim, np, xyz, ntri, q);
    std::free(q);
}

// @src 1402e24f0  min-area triangulation
// (callee 140159ad0)  int vector ctor, 14019a6b0 double vector ctor, 140256800 double push_back,
//                 140005e70 / 140005100 data (std::vector here)
std::int32_t fun_1402e24f0(DEF_GEOPRIM* prim, std::int32_t n, const double* pts,
                           std::int32_t* tri_out)
{
    (void)prim;
    std::vector<std::int32_t> ids;
    for (std::int32_t i = 1; i <= n; ++i) ids.push_back(i);
    ids.push_back(1);
    std::vector<double> xyz;
    xyz.push_back(0.0);
    xyz.push_back(0.0);
    xyz.push_back(0.0);
    for (std::int32_t i = 1; i <= n; ++i) {
        xyz.push_back(pts[i * 2]);
        xyz.push_back(pts[i * 2 + 1]);
        xyz.push_back(0.0);
    }
    Cls_140420750 solver;
    std::int32_t count = -1;
    if (solver.run(n, ids.data(), xyz.data())) {
        const std::vector<std::int32_t>& t = solver.result();
        count = static_cast<std::int32_t>(t.size() / 3);
        for (std::int32_t k = 0; k < count; ++k) {
            tri_out[k * 3] = t[static_cast<std::size_t>(k * 3)];
            tri_out[k * 3 + 1] = t[static_cast<std::size_t>(k * 3 + 1)];
            tri_out[k * 3 + 2] = t[static_cast<std::size_t>(k * 3 + 2)];
        }
    }
    return count;
}

} // namespace def::model::geoprim
