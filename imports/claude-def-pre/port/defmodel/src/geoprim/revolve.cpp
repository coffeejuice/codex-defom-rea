// GEOPRIM revolve, geometry path (packet p9-05). Spec: docs/re/commands/GEOPRIM_REVOLVE.md.
// Decompiles and listings: deform-rea-work/out/p905/all.c, all_l.txt. The C++ of these functions
// is unoptimised in the binary (every value goes through the stack), so the decompiled expression
// order is the executed order; comparisons were checked against the listing where a NaN or an
// equality could change the branch.
#include <defmodel/geoprim/revolve.hpp>

#include <defmodel/geo_kw/geo_kw.hpp>
#include <defmodel/geo_kw/misc_model.hpp>
#include <defmodel/geoprim/extrude.hpp>
#include <defmodel/geoprim/polygon.hpp>
#include <defmodel/geoprim/triangulate.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/layout/obj_geo2d.hpp>
#include <defmodel/linked/record_e8_list.hpp>
#include <defmodel/members/ftn_geom.hpp>
#include <defmodel/port.hpp>
#include <defrt/ftn/helpers.hpp>
#if __has_include(<defmodel/geoprim/fixgeo.hpp>)
#include <defmodel/geoprim/fixgeo.hpp>   // p9-01: 140220390, 140220420, 14027ece0
#define P905_HAVE_FIXGEO 1
#endif

#include <cmath>
#include <cstdlib>
#include <cstring>
#include <new>

namespace def::model::geoprim {

namespace {

void stub_vec3_cross(double*, const double*, const double*) { DEF_NOT_PORTED(0x14023a4b0); }
double stub_vec3_length(const double*) { DEF_NOT_PORTED(0x140239a30); }
#ifdef P905_HAVE_FIXGEO
// Owner p9-01 (defmodel/geoprim/fixgeo.hpp).
void stub_par_init(void* par) { new (par) FixgeoOptions(); }                          // 140220390
void stub_par_free(void* par) { static_cast<FixgeoOptions*>(par)->~FixgeoOptions(); } // 140220420
void stub_fixgeo(OBJ_GEO2D* geo, void* par, std::int32_t* k, std::int32_t mode, char flag)
{
    fixgeo(geo, static_cast<const FixgeoOptions*>(par), k, mode, flag);   // 14027ece0
}
#else
void stub_par_init(void*) { DEF_NOT_PORTED(0x140220390); }
void stub_par_free(void*) { DEF_NOT_PORTED(0x140220420); }
void stub_fixgeo(OBJ_GEO2D*, void*, std::int32_t*, std::int32_t, char) { DEF_NOT_PORTED(0x14027ece0); }
#endif
// 140220520 only calls 140158ce0 (geo_kw::object_dim), which is ported
std::int32_t obj_dimension(LLOBJ_DATA* o) { return geo_kw::object_dim(*o); }
void stub_model_copy(DB_STEP_DATA*, std::int32_t*) { DEF_NOT_PORTED(0x1401d7320); }
void stub_mesh_alloc(LLOBJ_DATA*, std::int32_t, std::int32_t, std::int32_t, std::int32_t*, std::int32_t)
{
    DEF_NOT_PORTED(0x140202700);
}
double* stub_node_coords(void*) { DEF_NOT_PORTED(0x1401808d0); }
void stub_surface_update(void*) { DEF_NOT_PORTED(0x1402fc1b0); }

constexpr double kEps12 = 1e-12;   // DAT_14051f260
constexpr double kEps6 = 1e-6;     // DAT_14051f1b8
constexpr double kNegEps6 = -1e-6; // DAT_140541478
constexpr double kEps8 = 1e-8;     // DAT_14051bed0
constexpr double k360 = 360.0;     // DAT_14051f210
constexpr double kMinus90 = -90.0; // DAT_14051f200
constexpr double kTol01 = 0.01;    // DAT_14051f258

double fabs_(double x) { return std::fabs(x); }   // 140484558

// operator new (1404797d8) with the size saturated as the binary does (the size never overflows
// for the counts used here); every block is released with free().
template <class T>
T* alloc_array(std::int64_t n)
{
    void* p = std::malloc(static_cast<std::size_t>(n) * sizeof(T));
    if (p == nullptr && n != 0) throw std::bad_alloc();
    return static_cast<T*>(p);
}

// WPAXIS record 1 of the object: +4 = 0, +8 axis point, +0x20 axis direction.
void set_wpaxis1(DEF_GEOPRIM* prim, const double* pt, const double* dir)
{
    auto& l = *reinterpret_cast<linked::Cls_140258580List*>(reinterpret_cast<std::byte*>(prim->obj) + 0x20a0);
    linked::Cls_140258580* r = geo_kw::wpaxis_get(l, 1);   // 140259bc0
    auto* b = reinterpret_cast<std::byte*>(r);
    const std::int32_t zero = 0;
    std::memcpy(b + 4, &zero, 4);
    std::memcpy(b + 8, pt, 0x18);
    std::memcpy(b + 0x20, dir, 0x18);
}

// The 3D position of profile point (x, y) for axis kind t (decompile of 1402d7cd0 / 1402ce310).
// Returns false when the binary writes nothing (t 3 without a record of kind 1, 2 or 3).
bool map_profile_point(const DEF_GEOPRIM* prim, std::int32_t t, double x, double y, double* q)
{
    const RevolveRec* rev = prim->revolve;
    const std::int32_t kind = rev ? rev->axis_kind : 0;
    if (t == 2) {
        if (rev == nullptr || kind != 3) { q[0] = 0.0; q[1] = x; q[2] = y; }
        else { q[0] = y; q[1] = 0.0; q[2] = x; }
    } else if (t == 1) {
        if (rev == nullptr || kind != 2) { q[0] = x; q[1] = y; q[2] = 0.0; }
        else { q[0] = 0.0; q[1] = x; q[2] = y; }
    } else if (t == 0) {
        if (rev == nullptr || kind != 3) { q[0] = x; q[1] = y; q[2] = 0.0; }
        else { q[0] = y; q[1] = 0.0; q[2] = x; }
    } else if (rev == nullptr || kind != 3) {
        if (rev == nullptr || kind != 2) {
            if (rev == nullptr || kind != 1) return false;
            q[0] = x; q[1] = y; q[2] = 0.0;
        } else { q[0] = 0.0; q[1] = x; q[2] = y; }
    } else { q[0] = y; q[1] = 0.0; q[2] = x; }
    return true;
}

} // namespace

P905Ext& p905_ext()
{
    static P905Ext e{stub_vec3_cross, stub_vec3_length, stub_par_init, stub_par_free, stub_fixgeo,
                     obj_dimension, stub_model_copy, stub_mesh_alloc, stub_node_coords,
                     stub_surface_update};
    return e;
}

// ---------------------------------------------------------------------------------------------

// @src 1402ce190
bool fun_1402ce190(DEF_GEOPRIM* prim)
{
    bool ok = false;
    if (prim->revolve != nullptr && prim->revolve->axis_kind == 0) {
        double dir[3], pt[3];
        std::int32_t t[4];
        fun_1402d9bb0(prim, dir, t, pt);
        if (t[0] == 0) prim->revolve->axis_kind = 1;
        else if (t[0] == 1) prim->revolve->axis_kind = 1;
        else {
            prim->revolve->axis_kind = 2;
            prim->u24db = 1;
        }
    }
    if (prim->mode_geo != 0) ok = fun_1402d7cd0(prim);
    if (prim->mode_mesh != 0) ok = fun_1402ce290(prim);
    return ok;
}

// @src 1402ce290
bool fun_1402ce290(DEF_GEOPRIM* prim)
{
    if (prim->mode_mesh == 1) {
        // strstr(rev->u420, "ExtrCrossSection2D.KEY") ? 1402d2920 : 1402d4420: shape-rolling
        // cross sections, not reachable in batch (+0x23a4 is never 1, see the packet plan)
        if (std::strstr(prim->revolve->u420, "ExtrCrossSection2D.KEY") == nullptr)
            DEF_NOT_PORTED(0x1402d4420);
        DEF_NOT_PORTED(0x1402d2920);
    }
    if (prim->mode_mesh == 2) return fun_1402ce310(prim);
    return false;
}

// @src 1402d9bb0
void fun_1402d9bb0(DEF_GEOPRIM* prim, double* dir, std::int32_t* t, double* pt)
{
    const double px[3] = {1.0, 0.0, 0.0};      // local_80[0..2]
    const double py[3] = {0.0, 1.0, 0.0};      // local_50..40
    const double nx[3] = {-1.0, 0.0, 0.0};     // local_20..10 (DAT_140522fc8 = -1)
    const double ny[3] = {0.0, -1.0, 0.0};     // local_38..28
    RevolveRec* rev = prim->revolve;
    if (rev == nullptr) return;
    dir[0] = rev->axis_dir[0];
    dir[1] = rev->axis_dir[1];
    dir[2] = rev->axis_dir[2];
    def::rt::ftn::vec3_normalize(dir);          // 14023b720
    *t = 3;
    if (kEps6 <= fabs_(1.0 - fabs_(dir[0]))) {
        if (kEps6 <= fabs_(1.0 - fabs_(dir[1]))) {
            if (fabs_(1.0 - fabs_(dir[2])) < kEps6) *t = 2;
        } else
            *t = 1;
    } else
        *t = 0;
    if (*t == 0) std::memcpy(dir, dir[0] < 0.0 ? nx : px, 0x18);
    else if (*t == 1) std::memcpy(dir, dir[1] < 0.0 ? ny : py, 0x18);
    // (the binary's third test repeats "*t == 0", so a z axis keeps its normalised direction)

    if (*t == 2) {
        if (rev->axis_kind == 3) { pt[0] = rev->axis_pt[0]; pt[1] = 0.0; pt[2] = rev->axis_pt[2]; }
        else { pt[0] = 0.0; pt[1] = rev->axis_pt[1]; pt[2] = rev->axis_pt[2]; }
    } else if (*t == 1) {
        if (rev->axis_kind == 2) { pt[0] = 0.0; pt[1] = rev->axis_pt[1]; pt[2] = rev->axis_pt[2]; }
        else { pt[0] = rev->axis_pt[0]; pt[1] = rev->axis_pt[1]; pt[2] = 0.0; }
    } else if (*t == 0) {
        if (rev->axis_kind == 3) { pt[0] = rev->axis_pt[0]; pt[1] = 0.0; pt[2] = rev->axis_pt[2]; }
        else { pt[0] = rev->axis_pt[0]; pt[1] = rev->axis_pt[1]; pt[2] = 0.0; }
    } else {
        pt[0] = rev->axis_pt[0];
        pt[1] = rev->axis_pt[1];
        pt[2] = rev->axis_pt[2];
    }
}

// @src 1402da250
void fun_1402da250(DEF_GEOPRIM* prim, const double* dir, const double* pt)
{
    if (prim->profile.n < 1 || prim->profile.pts == nullptr) return;
    double d[3];
    std::memcpy(d, dir, 0x18);
    def::rt::ftn::vec3_normalize(d);
    const RevolveRec* rev = prim->revolve;
    if (rev != nullptr && rev->axis_kind == 2) {   // raw, not normalised
        d[0] = dir[1];
        d[1] = dir[2];
        d[2] = dir[0];
    }
    V2 a2;
    double o[2];
    if (kEps6 <= fabs_(d[2])) {
        a2 = fun_1403086e0(d[2], d[0]);
        o[0] = pt[1];
        o[1] = pt[2];
    } else {
        a2 = V2{d[0], d[1]};                        // 140308730 copy
        o[0] = pt[0];
        o[1] = pt[1];
        if (rev != nullptr && rev->axis_kind == 2) {
            o[0] = pt[1];
            o[1] = pt[2];
        }
    }
    double* P = prim->profile.pts;
    std::int32_t pos = 0, neg = 0;
    for (std::int32_t i = 1; i <= prim->profile.n; ++i) {
        const V2 v = fun_1403086e0(P[2 * i] - o[0], P[2 * i + 1] - o[1]);
        const double s = fun_140308ad0(v, a2);
        if (s <= kEps6) {
            if (s < kNegEps6) ++neg;
        } else
            ++pos;
    }
    if (pos == 0 || neg == 0) return;
    const bool neg_side = pos < neg;   // local_228: move the points on the positive side
    const bool pos_side = !neg_side;   // local_1c0
    bool moved = false;
    for (std::int32_t i = 1; i <= prim->profile.n; ++i) {
        const V2 v = fun_1403086e0(P[2 * i] - o[0], P[2 * i + 1] - o[1]);
        const double s = fun_140308ad0(v, a2);
        if ((kEps6 < s && neg_side) || (s < kNegEps6 && pos_side)) {
            const double t = v.x * a2.x + v.y * a2.y;   // 140308a90
            P[2 * i] = o[0] + t * a2.x;
            P[2 * i + 1] = o[1] + t * a2.y;
            moved = true;
        }
    }
    if (!moved) return;
    OBJ_GEO2D geo;                                   // 140275cd0
    alignas(8) std::byte par[0x30];
    p905_ext().fixgeo_par_init(par);
    geo.curve_type = 1;
    geo.n_points = prim->profile.n;
    geo.clear_data();                                // 14027f360: allocate n * dof doubles
    const std::int32_t dof = geo.dof();              // 1402760e0 (3)
    for (std::int32_t i = 0; i < prim->profile.n; ++i)
        std::memcpy(geo.points + static_cast<std::int64_t>(i * dof), prim->profile.pts + 2 * (i + 1), 0x10);
    std::int32_t k;
    p905_ext().fixgeo(&geo, par, &k, 1, 0);
    std::int32_t count = geo.n_points;
    if (prim->profile.n < count) prim->profile.resize(&count);   // 1402e3fa0
    else prim->profile.n = count;
    for (std::int32_t i = 0; i < prim->profile.n; ++i)
        std::memcpy(prim->profile.pts + 2 * (i + 1), geo.points + static_cast<std::int64_t>(i * dof), 0x10);
    p905_ext().fixgeo_par_free(par);
}                                                    // 140275e00 geo dtor

// @src 1402da9f0
void fun_1402da9f0(DEF_GEOPRIM* /*prim*/, const double* dir, const double* pt, const std::int32_t* np,
                   double* xyz, const std::int32_t* nface, std::int32_t* faces)
{
    double d[3], p[3];
    std::memcpy(d, dir, 0x18);    // 140239800 copies
    std::memcpy(p, pt, 0x18);
    const std::int32_t n = *np;
    double* tmp = alloc_array<double>(static_cast<std::int64_t>(n) * 3);
    for (std::int32_t i = 0; i < n; ++i) std::memcpy(tmp + 3 * i, xyz + 3 * i, 0x18);   // 140239700
    double nrm[3];
    p904_ext().poly_normal(nrm, tmp, n);            // 140240d80
    double c[3];
    p905_ext().vec3_cross(c, d, nrm);               // 14023a4b0
    const double ref = p904_ext().dot(c, p);        // 14023a380
    const double area = p904_ext().polygon_area(n, tmp);   // 14023e670
    bool beyond = false;                            // local_218
    for (std::int32_t i = 0; i < n; ++i) {
        const double v = p904_ext().dot(c, tmp + 3 * i);
        if ((ref < v && 0.0 < area) || (v < ref && area < 0.0)) {
            beyond = true;
            break;
        }
    }
    bool rev = false;                               // local_1d8
    if (!beyond) {
        if (0.0 < area) rev = true;
    } else if (area < 0.0)
        rev = true;
    if (rev) {
        for (std::int32_t i = 0; i < n; ++i) {
            xyz[3 * i] = tmp[3 * (n - i - 1)];
            xyz[3 * i + 1] = tmp[3 * (n - i - 1) + 1];
            xyz[3 * i + 2] = tmp[3 * (n - i - 1) + 2];
        }
        if (0 < *nface) {
            for (std::int32_t f = 0; f < *nface; ++f)
                for (std::int32_t j = 0; j < 4; ++j) faces[4 * f + j] = (n - faces[4 * f + j]) + 1;
            for (std::int32_t f = 0; f < *nface; ++f) {   // xor swap of entries 1 and 2
                faces[4 * f + 1] ^= faces[4 * f + 2];
                faces[4 * f + 2] ^= faces[4 * f + 1];
                faces[4 * f + 1] ^= faces[4 * f + 2];
            }
        }
    }
    std::free(tmp);
}

// @src 1402db0d0
std::int32_t fun_1402db0d0(DEF_GEOPRIM* /*prim*/, const double* pt, const double* dir, std::int32_t n,
                           const double* xyz, char* on_axis)
{
    double d[3];
    std::memcpy(d, dir, 0x18);
    std::int32_t count = 0;
    for (std::int32_t i = 0; i < n; ++i) {
        double v[3];
        v[0] = xyz[3 * i] - pt[0];
        v[1] = xyz[3 * i + 1] - pt[1];
        v[2] = xyz[3 * i + 2] - pt[2];
        double c[3];
        p905_ext().vec3_cross(c, d, v);
        if (kEps6 <= p905_ext().vec3_length(c)) on_axis[i] = 0;
        else {
            on_axis[i] = 1;
            ++count;
        }
    }
    return count;
}

// @src 1402db310
void fun_1402db310(DEF_GEOPRIM* prim, const std::int32_t* nsec, const char* full, const char* on_axis,
                   std::int32_t* faces, std::int32_t* nface)
{
    const std::int32_t n = prim->profile.n;
    const bool closed = prim->profile.u10 != 0;
    auto put = [&](std::int32_t a, std::int32_t b, std::int32_t c) {
        std::int32_t* f = faces + 4 * static_cast<std::int64_t>(*nface);
        f[0] = a;
        f[1] = b;
        f[2] = c;
        f[3] = f[0];
        *nface = *nface + 1;
    };
    std::int32_t nl = (*full == 0) ? *nsec : *nsec - 1;
    for (std::int32_t s = 0; s < nl; ++s) {
        const std::int32_t base = s * n;
        for (std::int32_t i = 0; i < n; ++i) {
            if (i == n - 1) {
                if (!closed) continue;
                const char c1 = on_axis[i];
                const char c2 = on_axis[0];
                if (c1 == 0) put(base + 1 + i, base + i + 1 + n, c2 == 0 ? base + 1 + n : base + 1);
                if (c2 == 0) put(c1 == 0 ? base + 1 + i : i + 1, base + 1 + n, base + 1);
            } else {
                const char c1 = on_axis[i];
                const char c2 = on_axis[i + 1];
                if (c1 == 0) put(base + 1 + i, base + i + 1 + n, c2 == 0 ? base + i + 2 + n : i + 2);
                if (c2 == 0) put(c1 == 0 ? base + 1 + i : i + 1, base + i + 2 + n, base + 2 + i);
            }
        }
    }
    if (*full != 0) {                                // the last layer joins layer 0
        nl = nl * n;
        for (std::int32_t i = 0; i < n; ++i) {
            if (i == n - 1) {
                if (!closed) continue;
                const char c1 = on_axis[i];
                const char c2 = on_axis[0];
                if (c1 == 0) put(nl + 1 + i, i + 1, 1);
                if (c2 == 0) put(c1 == 0 ? nl + 1 + i : i + 1, 1, nl + 1);
            } else {
                const char c1 = on_axis[i];
                const char c2 = on_axis[i + 1];
                if (c1 == 0) put(nl + 1 + i, i + 1, i + 2);
                if (c2 == 0) put(c1 == 0 ? nl + 1 + i : i + 1, i + 2, nl + 2 + i);
            }
        }
    }
}

// @src 1402dbb20
void fun_1402dbb20(DEF_GEOPRIM* prim, const std::int32_t* nsec, const char* full, const char* on_axis,
                   std::int32_t* faces, std::int32_t* nface)
{
    const std::int32_t n = prim->profile.n;
    for (std::int32_t s = 0; s < *nsec; ++s) {
        const std::int32_t base = s * n;
        for (std::int32_t i = 0; i < n; ++i) {
            std::int32_t* f = faces + 4 * static_cast<std::int64_t>(*nface);
            const bool last_layer = (s == *nsec - 1) && *full != 0;
            if (i == n - 1) {
                if (on_axis[i] == 0 || on_axis[0] == 0) {
                    f[0] = base + 1 + i;
                    if (last_layer) {
                        f[1] = n;
                        f[2] = 1;
                    } else {
                        f[1] = f[0] + n;
                        f[2] = f[0] + 1;
                    }
                    f[3] = base + 1;
                    *nface = *nface + 1;
                }
            } else if (on_axis[i] == 0 || on_axis[i + 1] == 0) {
                f[0] = base + 1 + i;
                if (last_layer) {
                    f[1] = i + 1;
                    f[2] = i + 2;
                } else {
                    f[1] = f[0] + n;
                    f[2] = f[1] + 1;
                }
                f[3] = f[0] + 1;
                *nface = *nface + 1;
            }
        }
    }
}

// @src 1402dc160
void fun_1402dc160(DEF_GEOPRIM* prim, const std::int32_t* np, const double* xyz, const std::int32_t* nface,
                   const std::int32_t* faces)
{
    std::int32_t* map = alloc_array<std::int32_t>(*np);
    std::memset(map, 0, static_cast<std::size_t>(*np) * 4);   // 140478d30
    for (std::int32_t f = 0; f < *nface; ++f)
        for (std::int32_t j = 0; j < 3; ++j) {
            const std::int32_t id = faces[4 * f + j];
            if (0 < id && id <= *np) map[id - 1] = 1;
        }
    std::int32_t used = 0;
    for (std::int32_t i = 0; i < *np; ++i)
        if (map[i] == 1) map[i] = ++used;
    if (used == *np) {
        std::free(map);
        fun_1402dbe70(prim, np, xyz, nface, faces);
        return;
    }
    double* pts = alloc_array<double>(static_cast<std::int64_t>(used) * 3);
    std::int32_t* nf = alloc_array<std::int32_t>(static_cast<std::int64_t>(*nface) * 4);
    // ids <= 0 are not written in the binary (heap contents); the port leaves them 0
    std::memset(nf, 0, static_cast<std::size_t>(*nface) * 16);
    for (std::int32_t i = 0; i < *np; ++i) {
        const std::int32_t m = map[i];
        if (0 < m) std::memcpy(pts + 3 * static_cast<std::int64_t>(m - 1), xyz + 3 * static_cast<std::int64_t>(i), 0x18);
    }
    for (std::int32_t f = 0; f < *nface; ++f)
        for (std::int32_t j = 0; j < 4; ++j) {
            const std::int32_t id = faces[4 * f + j];
            if (0 < id) nf[4 * f + j] = map[id - 1];
        }
    fun_1402dbe70(prim, &used, pts, nface, nf);
    std::free(map);
    std::free(pts);
    std::free(nf);
}

// @src 1402cac30
void fun_1402cac30(DEF_GEOPRIM* /*prim*/, double* a2, double* a3, const double* start, const double* angle,
                   const char* full, double* p7, double* p8, double* p9, double* p10, double* p11,
                   double* p12, double* p13, double* p14)
{
    *p8 = 0.0;
    *p10 = 0.0;
    *p11 = *start;
    *p12 = *angle;
    *p14 = 0.0;
    if (*a2 < *start) *a2 = *start;        // listing: COMISD start, a2; JBE skip
    if (!(*a2 < *start + *angle)) return;
    if (*full == 0 && *start + *angle < *a2 + *a3) *a3 = (*start + *angle) - *a2;
    if (!(kTol01 < fabs_(((*a2 + *a3) - *start) + *angle))) return;
    if (*full == 0 || ((*a2 + *a3) - k360) <= *start) {
        *p11 = *start;
        *p12 = *a2 - *start;
        *p13 = *a2 + *a3;
        *p14 = (*angle - *p12) - *a3;
        *p7 = *a2;
        *p8 = *a3;
        *p10 = 0.0;
    } else {
        *p7 = *start;
        *p8 = ((*a2 + *a3) - k360) - *start;
        *p9 = *a2;
        *p10 = (k360 + *start) - *p9;
        *p11 = *p7 + *p8;
        *p12 = *angle - *a3;
        *p14 = 0.0;
    }
}

// @src 1402caf40
void fun_1402caf40(DEF_GEOPRIM* prim, const std::int32_t* nsec, const double* /*p3*/, const double* p4,
                   const double* /*p5*/, const double* p6, const double* /*p7*/, const double* p8,
                   const double* /*p9*/, const double* p10, const double* /*p11*/, const double* p12,
                   const double* /*p13*/, const double* p14, std::int32_t* n15, double* s16,
                   std::int32_t* n17, double* s18, std::int32_t* n19, double* s20, std::int32_t* n21,
                   double* s22)
{
    const std::int32_t m = static_cast<std::int32_t>(static_cast<double>(*nsec) /
                                                     (1.0 + (*p4 / *p6) * prim->u2388));
    *n19 = static_cast<std::int32_t>((static_cast<double>(m) * *p12) / (*p6 - *p4));
    *s20 = (*n19 < 1) ? 0.0 : *p12 / static_cast<double>(*n19);
    *n21 = m - *n19;
    *s22 = (*n21 < 1) ? 0.0 : *p14 / static_cast<double>(*n21);
    const std::int32_t rest = *nsec;
    const double a = *p8;
    const double v0 = prim->v2398.mutable_at(0);   // 140005e70
    *n15 = static_cast<std::int32_t>((static_cast<double>(rest - m) * a) / v0);
    *s16 = (*n15 < 1) ? 0.0 : *p8 / static_cast<double>(*n15);
    *n17 = (rest - m) - *n15;
    *s18 = (*n17 < 1) ? 0.0 : *p10 / static_cast<double>(*n17);
}

// @src 1402d7cd0
bool fun_1402d7cd0(DEF_GEOPRIM* prim)
{
    std::int32_t t = 2;                              // local_90
    double dir[3] = {0.0, 0.0, 1.0};                 // local_f8
    double pt[3] = {0.0, 0.0, 0.0};                  // local_128
    char full = 0;                                   // local_d8
    char uniform = 1;                                // local_78
    char b8 = 0;
    double a0 = 0.0, d180 = 0.0, d168 = 0.0, de0 = 0.0, d148 = 0.0, dc0 = 0.0, dc8 = 0.0, d150 = 0.0;
    std::int32_t n188 = 0, n10c = 0, n1b8 = 0, n110 = 0;
    double s158 = 0.0, s1b0 = 0.0, s80 = 0.0, s88 = 0.0;
    double step = 0.0;                               // local_140 (unset in the binary when the
                                                     // non-uniform test turns uniform)

    if (!(kEps12 < prim->angle || kEps12 == prim->angle)) return false;
    if (prim->profile.n < 1 || prim->profile.pts == nullptr) return false;
    prim->angle = (k360 <= prim->angle) ? k360 : prim->angle;
    if (fabs_(prim->angle - k360) < kEps8) full = 1;
    std::int32_t nsec = prim->sections;              // local_d4
    if (prim->sections < 1) return false;

    double start = 0.0;                              // local_98
    if (prim->revolve == nullptr) {
        if (prim->u2340 != nullptr)
            std::memcpy(&start, reinterpret_cast<const std::byte*>(prim->u2340) + 0x858, 8);
    } else {
        fun_1402d9bb0(prim, dir, &t, pt);
        start = prim->revolve->start;
    }
    fun_1402da250(prim, dir, pt);
    uniform = fun_1402caac0(prim) ? 1 : 0;
    if (uniform == 0) {
        double* v98 = &prim->v2398.mutable_at(0);
        double* v90 = &prim->v2390.mutable_at(0);
        fun_1402cac30(prim, v90, v98, &start, &prim->angle, &full, &a0, &d180, &d168, &de0, &d148, &dc0,
                      &dc8, &d150);
        if (fabs_(prim->v2398.mutable_at(0)) <= kTol01) uniform = 1;
        if (a0 - start <= kTol01) b8 = 1;
        if (uniform == 0) {
            double* w98 = &prim->v2398.mutable_at(0);
            double* w90 = &prim->v2390.mutable_at(0);
            fun_1402caf40(prim, &nsec, w90, w98, &start, &prim->angle, &a0, &d180, &d168, &de0, &d148, &dc0,
                          &dc8, &d150, &n188, &s158, &n10c, &s1b0, &n1b8, &s80, &n110, &s88);
        }
    } else
        step = prim->angle / static_cast<double>(nsec);

    const std::int32_t n = prim->profile.n;
    std::int32_t sz = (nsec * 2 + 2) * n;            // local_1a4
    std::int32_t* tris = alloc_array<std::int32_t>(static_cast<std::int64_t>(sz * 3));
    fun_1402e7f40(&prim->profile);
    std::int32_t nface = 0;                          // local_178
    if (prim->u24fb == 0) {
        if (full == 0 && prim->mode_geo == 1) {
            if (prim->profile.u10 == 0) nface = 0;
            else if (!fun_1402f07e0(prim)) nface = fun_14041f830(1, &prim->profile.n, prim->profile.pts, tris);
            else nface = fun_1402e24f0(prim, prim->profile.n, prim->profile.pts, tris);
        } else
            nface = 0;
    }
    std::int32_t ncap = nface;                       // local_d0
    std::int32_t* faces = alloc_array<std::int32_t>(static_cast<std::int64_t>(sz << 2));
    for (std::int32_t i = 0; i < nface; ++i) {
        std::memcpy(faces + 4 * i, tris + 3 * i, 0xc);
        faces[4 * i + 3] = faces[4 * i];
    }
    sz = (nsec + 1) * prim->profile.n;
    double* xyz = alloc_array<double>(static_cast<std::int64_t>(sz) * 3);
    for (std::int32_t i = 0; i < prim->profile.n; ++i)
        map_profile_point(prim, t, prim->profile.pts[2 * (i + 1)], prim->profile.pts[2 * (i + 1) + 1], xyz + 3 * i);
    std::int32_t np = prim->profile.n;              // local_1b4
    if (kEps6 < fabs_(start))
        for (std::int32_t i = 0; i < np; ++i) fun_140325691(xyz + 3 * i, pt, dir, &start);
    if (prim->u24db != 0) {
        const double m90 = kMinus90;
        for (std::int32_t i = 0; i < np; ++i) fun_140325691(xyz + 3 * i, pt, dir, &m90);
    }
    if (prim->revolve != nullptr) fun_1402da9f0(prim, dir, pt, &np, xyz, &ncap, faces);
    char* on_axis = alloc_array<char>(prim->profile.n);
    std::memset(on_axis, 0, static_cast<std::size_t>(prim->profile.n));
    const std::int32_t naxis = fun_1402db0d0(prim, pt, dir, prim->profile.n, xyz, on_axis);
    const std::int32_t nl = (full == 0) ? nsec : nsec - 1;
    double ang = 0.0;                                // local_160
    for (std::int32_t k = 1; k <= nl; ++k) {
        if (uniform == 0) {
            if (b8 == 0) {
                if (n1b8 < k) {
                    if (n1b8 + n188 < k) {
                        if (n1b8 + n188 + n110 < k) ang = ang + s1b0;
                        else ang = ang + s88;
                    } else
                        ang = ang + s158;
                } else
                    ang = ang + s80;
            } else if (n188 < k) {
                if (n188 + n1b8 < k) {
                    if (n188 + n1b8 + n10c < k) ang = ang + s88;
                    else ang = ang + s1b0;
                } else
                    ang = ang + s80;
            } else
                ang = ang + s158;
        } else
            ang = step * static_cast<double>(k);
        for (std::int32_t i = 0; i < prim->profile.n; ++i) {
            double q[3];
            std::memcpy(q, xyz + 3 * i, 0x18);
            fun_140325691(q, pt, dir, &ang);
            std::memcpy(xyz + 3 * static_cast<std::int64_t>(np), q, 0x18);
            ++np;
        }
    }
    if (prim->mode_geo == 1) fun_1402db310(prim, &nsec, &full, on_axis, faces, &nface);
    else if (prim->mode_geo == 2) fun_1402dbb20(prim, &nsec, &full, on_axis, faces, &nface);
    if (full == 0) {                                 // the far cap: reversed, shifted ids
        const std::int32_t shift = prim->profile.n * nsec;
        for (std::int32_t i = 0; i < ncap; ++i) {
            std::int32_t* dst = faces + 4 * static_cast<std::int64_t>(nface);
            std::int32_t e = faces[4 * i];
            if (on_axis[e - 1] == 0) e = e + shift;
            dst[3] = e;
            e = faces[4 * i + 1];
            if (on_axis[e - 1] == 0) e = e + shift;
            dst[2] = e;
            e = faces[4 * i + 2];
            if (on_axis[e - 1] == 0) e = e + shift;
            dst[1] = e;
            e = faces[4 * i + 3];
            if (on_axis[e - 1] == 0) e = e + shift;
            dst[0] = e;
            ++nface;
        }
    }
    if (naxis == 0) fun_1402dbe70(prim, &np, xyz, &nface, faces);
    else fun_1402dc160(prim, &np, xyz, &nface, faces);
    if (prim->revolve == nullptr) set_wpaxis1(prim, pt, dir);
    else set_wpaxis1(prim, prim->revolve->axis_pt, prim->revolve->axis_dir);
    std::free(tris);
    std::free(faces);
    std::free(xyz);
    std::free(on_axis);
    return true;
}

// @src 1402d6bf0
char fun_1402d6bf0(DEF_GEOPRIM* prim, const double* dir, const double* pt, const double* xyz,
                   const char* invert, const char* keep)
{
    double d[3], p[3];
    std::memcpy(d, dir, 0x18);
    std::memcpy(p, pt, 0x18);
    const std::int32_t nv = 4;                       // local_27c
    double* tmp = alloc_array<double>(static_cast<std::int64_t>(nv) * 3);
    const std::int32_t ne = fun_1402e9b70(prim);
    std::int32_t* conn = fun_1402e9bb0(prim);
    for (std::int32_t i = 0; i < nv; ++i) {
        const std::int32_t k = conn[i] - 1;
        std::memcpy(tmp + 3 * i, xyz + 3 * static_cast<std::int64_t>(k), 0x18);
    }
    double nrm[3];
    p904_ext().poly_normal(nrm, tmp, nv);
    double c[3];
    p905_ext().vec3_cross(c, d, nrm);
    const double ref = p904_ext().dot(c, p);
    const double area = p904_ext().polygon_area(nv, tmp);
    bool beyond = false;                             // local_280
    for (std::int32_t i = 0; i < nv; ++i) {
        const std::int32_t k = conn[i] - 1;
        const double v = p904_ext().dot(c, xyz + 3 * static_cast<std::int64_t>(k));
        if ((ref < v && 0.0 < area) || (v < ref && area < 0.0)) {
            beyond = true;
            break;
        }
    }
    char ans = 0;                                    // local_238
    if (!beyond) {
        if (area < 0.0) ans = 1;
    } else if (0.0 < area)
        ans = 1;
    if (*invert != 0) ans = (ans == 0) ? 1 : 0;
    if (ans != 0 && *keep == 0)
        for (std::int32_t e = 0; e < ne; ++e) {
            conn[4 * e] ^= conn[4 * e + 2];
            conn[4 * e + 2] ^= conn[4 * e];
            conn[4 * e] ^= conn[4 * e + 2];
        }
    std::free(tmp);
    return ans;
}

} // namespace def::model::geoprim
