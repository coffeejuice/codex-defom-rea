// GEOPRIM builder and extrude primitive (packet p9-04). Spec: CNVT3D_SWEEP.md 10.4-10.6.
// Decompiles and listings: deform-rea-work/out/p904/all.c, all.asm.
#include <defcmd/geoprim/builder.hpp>

#include <defmodel/bc/boundary_topology.hpp>
#include <defmodel/geo_kw/geo_kw.hpp>
#include <defmodel/geo_kw/misc_model.hpp>
#include <defmodel/geoprim/def_geoprim.hpp>
#include <defmodel/geoprim/extrude.hpp>
#include <defmodel/geoprim/polygon.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/layout/obj_geo2d.hpp>
#include <defmodel/linked/record_e8_list.hpp>
#include <defmodel/llobj/cls_14017ef20.hpp>
#include <defmodel/llobj_ctor/llobj_ctor.hpp>
#include <defmodel/port.hpp>
#include <defkey/kw_g2geo.hpp>
#include <defrt/stdlib/legacy_printf.hpp>
#include <defrt/stdlib/refcount.hpp>

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <share.h>

// Owners' headers (switch the defaults on when merged).
#if __has_include(<defmodel/geoprim/triangulate.hpp>)
#include <defmodel/geoprim/triangulate.hpp>   // p9-05: 14041f830
#define P904_HAVE_TRIANGULATE 1
#endif
#if __has_include(<defmodel/geoprim/revolve.hpp>)
#include <defmodel/geoprim/revolve.hpp>       // p9-05: 1402ce190
#define P904_HAVE_REVOLVE 1
#endif
#if __has_include(<defcmd/objpos.hpp>)
#include <defcmd/objpos.hpp>                  // p67-1: 14021d2f0, 14021d0b0
#define P904_HAVE_OBJPOS 1
#endif
#if __has_include(<defcmd/objpos_centre.hpp>)
#include <defcmd/objpos_centre.hpp>           // p67-1: 1402009f0
#define P904_HAVE_OBJPOS_CENTRE 1
#endif
#if __has_include(<defcmd/geolib.hpp>)
#include <defcmd/geolib.hpp>                  // p8-09: 140202700, 140198270, 14023a380
#define P904_HAVE_GEOLIB 1
#endif
#if __has_include(<defcmd/vec3.hpp>)
#include <defcmd/vec3.hpp>                    // p67-1: 14023b850
#define P904_HAVE_VEC3 1
#endif
#if __has_include(<defcmd/ctc3d/cube.hpp>)
#include <defcmd/ctc3d/cube.hpp>              // P6-7 gate: 140240d80
#define P904_HAVE_CUBE 1
#endif
#if __has_include(<defmodel/node_access.hpp>)
#include <defmodel/node_access.hpp>           // P6-7 gate: 1401808d0
#define P904_HAVE_NODE_ACCESS 1
#endif
#if __has_include(<defmodel/hdrstats/object_stats.hpp>)
#include <defmodel/hdrstats/object_stats.hpp> // p4-07: 14023e670
#define P904_HAVE_HDRSTATS 1
#endif
#if __has_include(<defcmd/geoprim/revolve_bind.hpp>)
#include <defcmd/geoprim/revolve_bind.hpp>  // p9-05: bind_p905_ext()
#define P904_HAVE_P905_BIND 1
#endif
#if __has_include(<defkey/kw_g2sv.hpp>)
#include <defkey/kw_g2sv.hpp>                 // kw-g2sv: 1402a4a80
#define P904_HAVE_G2SV 1
#endif

namespace def::key::g2geo::surface {
// 1402a26c0 (defkey/src/kw_g2geo/internal.hpp, not in the public header)
int write_geometry_dispatch(def::model::DB_STEP_DATA& m, std::FILE* f, def::model::LLOBJ_DATA& obj,
                            std::uint32_t n);
} // namespace def::key::g2geo::surface

namespace def::cmd::geoprim {

namespace mg = def::model::geoprim;
namespace gk = def::model::geo_kw;
using def::model::Cls_14017ef20;
using def::model::OBJ_GEO2D;
using def::model::linked::Cls_140258580;
using def::model::linked::Cls_140258580List;

namespace {

// ---- defaults of BuilderExt ----
std::int32_t d_cap_triangulate(std::int32_t nc, const std::int32_t* cntr, const double* pts,
                               std::int32_t* tris)
{
#ifdef P904_HAVE_TRIANGULATE
    return mg::fun_14041f830(nc, cntr, pts, tris);
#else
    (void)nc, (void)cntr, (void)pts, (void)tris;
    DEF_NOT_PORTED(0x14041f830);
#endif
}
bool d_revolve(DEF_GEOPRIM* prim)
{
#ifdef P904_HAVE_REVOLVE
    return mg::fun_1402ce190(prim);
#else
    (void)prim;
    DEF_NOT_PORTED(0x1402ce190);
#endif
}
void d_rotate(LLOBJ_DATA* o, const double* origin, const double* dir, double angle)
{
#ifdef P904_HAVE_OBJPOS
    def::cmd::objpos::rotate(o, origin, dir, angle);
#else
    (void)o, (void)origin, (void)dir, (void)angle;
    DEF_NOT_PORTED(0x14021d2f0);
#endif
}
void d_translate(LLOBJ_DATA* o, const double* d, const char* a, const char* b)
{
#ifdef P904_HAVE_OBJPOS
    def::cmd::objpos::translate(o, d, a, b);
#else
    (void)o, (void)d, (void)a, (void)b;
    DEF_NOT_PORTED(0x14021d0b0);
#endif
}
void d_mesh_alloc(LLOBJ_DATA* o, std::int32_t nn, std::int32_t ne, std::int32_t t,
                  const std::int32_t* dims, std::int32_t dim)
{
#ifdef P904_HAVE_GEOLIB
    def::cmd::geolib::fun_140202700(o, nn, ne, t, dims, dim);
#else
    (void)o, (void)nn, (void)ne, (void)t, (void)dims, (void)dim;
    DEF_NOT_PORTED(0x140202700);
#endif
}
void d_add_curve(LLOBJ_DATA* o)
{
#ifdef P904_HAVE_GEOLIB
    def::cmd::geolib::fun_140198270(o);
#else
    (void)o;
    DEF_NOT_PORTED(0x140198270);
#endif
}
double* d_node_coords(Cls_14017ef20* set)
{
#ifdef P904_HAVE_NODE_ACCESS
    return def::model::fun_1401808d0(set);
#else
    (void)set;
    DEF_NOT_PORTED(0x1401808d0);
#endif
}
std::int32_t d_remove_if_exists(const char* name)
{
    (void)name;
    DEF_NOT_PORTED(0x1401b85d0);   // p4-01 (defdbio gendb driver, not exported yet)
}
int d_write_state_var(DB_STEP_DATA& m, std::FILE* f, LLOBJ_DATA& obj, int code, std::uint32_t n, int k)
{
#ifdef P904_HAVE_G2SV
    return def::key::g2sv::write_state_var(m, f, obj, code, n, k);
#else
    (void)m, (void)f, (void)obj, (void)code, (void)n, (void)k;
    DEF_NOT_PORTED(0x1402a4a80);
#endif
}

BuilderExt g_ext = {d_cap_triangulate, d_revolve, d_rotate, d_translate, d_mesh_alloc,
                    d_add_curve, d_node_coords, d_remove_if_exists, d_write_state_var};

// ---- defaults of def::model::geoprim::P904Ext ----
#ifdef P904_HAVE_HDRSTATS
double b_polygon_area(std::int32_t n, const double* pts)
{
    return def::model::p407::polygon_area(n, reinterpret_cast<const def::model::p407::Vec3*>(pts));
}
#endif
#ifdef P904_HAVE_VEC3
double b_dist2(const double* a, const double* b) { return def::cmd::v3::dist2(a, b); }
#endif
#ifdef P904_HAVE_GEOLIB
double b_dot(const double* a, const double* b) { return def::cmd::geolib::fun_14023a380(a, b); }
#endif
#ifdef P904_HAVE_CUBE
void b_poly_normal(double* out, const double* pts, std::int32_t n)
{
    // 140240d80 returns the unit normal by value (hidden return pointer in the binary)
    const def::cmd::cube::Vec3 v = def::cmd::ctc3d::fun_140240d80(pts, n);
    out[0] = v.x;
    out[1] = v.y;
    out[2] = v.z;
}
#endif
#ifdef P904_HAVE_OBJPOS_CENTRE
void b_surface_size(LLOBJ_DATA* o, std::int32_t nn, std::int32_t nf, std::int32_t bdry)
{
    def::cmd::objpos::size_surface(o, nn, nf, bdry);
}
#endif

template <class T>
T& rec(void* base, std::size_t off)
{
    return *reinterpret_cast<T*>(static_cast<std::byte*>(base) + off);
}
Cls_140258580List& wpaxis_list(LLOBJ_DATA& o) { return rec<Cls_140258580List>(&o, 0x20a0); }

constexpr double kEps12 = 1e-12;   // DAT_14051f260
constexpr double k09 = 0.9;        // DAT_14051f220
constexpr double km09 = -0.9;      // DAT_140541470
constexpr double kOne = 1.0;       // DAT_140509c90
constexpr double kMinusOne = -1.0; // DAT_140522fc8
constexpr double k90 = 90.0;       // DAT_14051f208
constexpr double km90 = -90.0;     // DAT_14051f200

template <class T>
T* alloc_n(std::int64_t n)
{
    return def::model::cow_detail::alloc<T>(static_cast<std::size_t>(n));
}

// @src 1402bd210  shape-rolling extrude: not-needed in batch (docs/re/status/p9-shape-roll.tsv;
// +0x23a4 is 1 only after setups outside the batch scope). Reaching it is a port error.
[[noreturn]] bool fun_1402bd210(DEF_GEOPRIM*)
{
    DEF_NOT_PORTED(0x1402bd210);
}

} // namespace

BuilderExt& builder_ext() { return g_ext; }

void bind_ext()
{
    mg::P904Ext& e = mg::p904_ext();
#ifdef P904_HAVE_HDRSTATS
    e.polygon_area = b_polygon_area;
#endif
#ifdef P904_HAVE_VEC3
    e.dist2 = b_dist2;
#endif
#ifdef P904_HAVE_GEOLIB
    e.dot = b_dot;
#endif
#ifdef P904_HAVE_CUBE
    e.poly_normal = b_poly_normal;
#endif
#ifdef P904_HAVE_OBJPOS_CENTRE
    e.surface_size = b_surface_size;
#endif
    (void)e;
}

// @src 1402ba500  builder
// (callee 1404797d8)  operator new (DB_STEP_DATA 0x3408, LLOBJ_DATA 0x3f28)
bool fun_1402ba500(DEF_GEOPRIM* prim, LLOBJ_DATA* o3, const char* write_key, DB_STEP_DATA* model3,
                   void* ctx)
{
    bind_ext();
#ifdef P904_HAVE_P905_BIND
    bind_p905_ext();   // p9-05's hooks of the revolve
#endif
    if (model3 == nullptr) {
        prim->model = new DB_STEP_DATA(3);   // 1401c8950
        prim->owns_model = 1;
    } else {
        if (!def::rt::dimension_is_3(model3)) return false;   // 1400015f0
        prim->model = model3;
        prim->owns_model = 0;
    }
    if (o3 == nullptr) {
        prim->obj = new LLOBJ_DATA(nullptr);                            // 14015ec80
        def::model::llobj_ctor::set_owner(prim->obj, prim->model);      // 14015ef60
        prim->owns_obj = 1;
    } else {
        prim->obj = o3;
        prim->owns_obj = 0;
    }
    prim->ctx = (ctx == nullptr) ? nullptr : ctx;
    bool ok = fun_1402ba850(prim);
    if (prim->kind == 1) ok = fun_1402baa60(prim);
    else if (prim->kind == 2) ok = g_ext.revolve(prim);
    if (*write_key != 0 && ok) fun_1402dc560(prim);
    if (prim->owns_obj != 0) {
        if (prim->obj != nullptr) mg::ext().obj_delete(prim->obj);      // 14019a290
        prim->obj = nullptr;
    }
    if (prim->owns_model != 0) {
        if (prim->model != nullptr) mg::ext().model_delete(prim->model);   // 140158fc0
        prim->model = nullptr;
    }
    return ok;
}

// @src 1402ba850  profile -> curve 0
bool fun_1402ba850(DEF_GEOPRIM* prim)
{
    if (0 < prim->profile.n) {
        OBJ_GEO2D* c = gk::curve(*prim->obj, 0);   // 1401982c0
        if (c == nullptr) {
            g_ext.add_curve(prim->obj);            // 140198270
            c = gk::curve(*prim->obj, 0);
        }
        c->n_points = prim->profile.n + 1;
        c->curve_type = 1;
        c->clear_data();                           // vtable +0x18 (slot 3)
        const double* P = prim->profile.pts;
        std::int32_t i = 0;
        for (; i < prim->profile.n; ++i) {
            c->points[i * 3] = P[(i + 1) * 2];
            c->points[i * 3 + 1] = P[(i + 1) * 2 + 1];
            c->points[i * 3 + 2] = 0.0;
        }
        c->points[i * 3] = P[2];
        c->points[i * 3 + 1] = P[3];
        c->points[i * 3 + 2] = 0.0;
    }
    return false;   // AL = 0 on both paths
}

// @src 1402baa60  extrude primitive
bool fun_1402baa60(DEF_GEOPRIM* prim)
{
    bool ok = false;
    if (prim->mode_geo != 0) ok = fun_1402bb460(prim);
    if (prim->mode_mesh != 0) {
        ok = fun_1402bbb10(prim);
        if (std::strlen(prim->u1b30) != 0) fun_1402bc580(prim);
    }
    return ok;
}

// @src 1402bbb10  mesh path dispatch
bool fun_1402bbb10(DEF_GEOPRIM* prim)
{
    if (prim->mode_mesh == 1) return fun_1402bd210(prim);
    if (prim->mode_mesh == 2) return fun_1402bbb60(prim);
    return false;
}

// @src 1402bb460  geometry path
bool fun_1402bb460(DEF_GEOPRIM* prim)
{
    // 1402bb48a: COMISD 1e-12, len; JA -> false (len < 1e-12 or NaN)
    if (!(kEps12 < prim->u2360 || kEps12 == prim->u2360)) return false;
    const std::int32_t n = prim->profile.n;
    if (n < 1 || prim->profile.pts == nullptr) return false;
    std::int32_t* T = alloc_n<std::int32_t>(static_cast<std::int64_t>(n << 2) * 3);
    std::int32_t ntri;
    if (!mg::fun_1402e7f40(&prim->profile)) {
        ntri = 0;
    } else if (!mg::fun_1402f07e0(prim)) {
        ntri = g_ext.cap_triangulate(1, &prim->profile.n, prim->profile.pts, T);
    } else {
        ntri = mg::fun_1402e24f0(prim, prim->profile.n, prim->profile.pts, T);
    }
    double* X = alloc_n<double>(static_cast<std::int64_t>(n << 1) * 3);
    for (std::int32_t i = 0; i < prim->profile.n; ++i) {
        std::memcpy(X + i * 3, prim->profile.pts + (i + 1) * 2, 0x10);
        X[i * 3 + 2] = prim->u2360;
        std::memcpy(X + (i + prim->profile.n) * 3, prim->profile.pts + (i + 1) * 2, 0x10);
        X[(i + prim->profile.n) * 3 + 2] = 0.0;
    }
    for (std::int32_t i = 0; i < ntri; ++i) {
        std::int32_t* d = T + (i + ntri) * 3;
        const std::int32_t* s = T + i * 3;
        d[0] = s[2] + prim->profile.n;
        d[1] = s[1] + prim->profile.n;
        d[2] = s[0] + prim->profile.n;
    }
    ntri = ntri << 1;
    for (std::int32_t i = 0; i < prim->profile.n; ++i) {
        const std::int32_t np = prim->profile.n;
        std::int32_t* t = T + ntri * 3;
        if (i == np - 1) {
            if (prim->profile.u10 != 0) {
                t[0] = i + 1;
                t[1] = i + 1 + np;
                t[2] = np + 1;
                t[3] = i + 1;
                t[4] = np + 1;
                t[5] = 1;
                ntri = ntri + 2;
            }
        } else {
            t[0] = i + 1;
            t[1] = i + 1 + np;
            t[2] = i + 2 + np;
            t[3] = i + 1;
            t[4] = i + 2 + np;
            t[5] = i + 2;
            ntri = ntri + 2;
        }
    }
    std::int32_t nx = prim->profile.n << 1;
    mg::fun_1402dc040(prim, &nx, X, &ntri, T);
    const char zero = 0;
    fun_1402c1130(prim, &zero);
    Cls_140258580* w = gk::wpaxis_get(wpaxis_list(*prim->obj), 1);   // 140259bc0
    rec<std::int32_t>(w, 4) = 1;
    std::memcpy(&rec<double>(w, 8), X, 0x18);
    std::memcpy(&rec<double>(w, 0x20), X + prim->profile.n * 3, 0x18);
    std::free(T);
    std::free(X);
    return true;
}

// @src 1402bbb60  mesh-boundary path
bool fun_1402bbb60(DEF_GEOPRIM* prim)
{
    prim->u23e4 = 0;
    prim->u23e0 = 0;
    prim->u23e8 = 0;
    prim->u23ec = 0;
    prim->u2360 = prim->extrude->length;
    if (!(kEps12 < prim->u2360 || kEps12 == prim->u2360)) return false;
    const std::int32_t np = mg::fun_1402e9b50(prim);
    const double* xy = mg::fun_1402e9b90(prim);
    if (np < 1 || xy == nullptr) return false;
    if (prim->sections < 1) return false;
    if (prim->int_lists.n < 1) mg::fun_1402cc080(prim);
    if (prim->int_lists.n < 1) {
        def::rt::crt::fprintf(stdout, "cross section mesh boundary can not be constructed.\n");
        return false;
    }
    const std::int32_t ns = prim->sections;
    const double len = prim->u2360;
    double* X = alloc_n<double>(static_cast<std::int64_t>((prim->sections + 1) * np) * 3);
    std::int32_t* ctx = static_cast<std::int32_t*>(prim->ctx);
    const double* first = nullptr;   // local_d8
    for (std::int32_t k = 0; k < np; ++k) {
        if (k == 0) first = X;
        std::memcpy(X + k * 3, xy + (k << 1), 0x10);
        X[k * 3 + 2] = 0.0;
        if (prim->ctx != nullptr) ctx[k] = k;
    }
    std::int32_t e = np;
    prim->u23e4 = np;
    prim->u23e0 = np;
    prim->u23e8 = np;
    mg::IntArray* B = prim->int_lists.items;
    std::int32_t nb = B->n;
    if (mg::fun_1402e91d0(B)) {
        nb = nb - 1;
        B->n = B->n - 1;
    }
    const std::int32_t nsec = prim->sections;   // local_bc
    const double step = len / static_cast<double>(ns);   // 1402bbd37: hoisted division
    for (std::int32_t s = 1; s < nsec; ++s) {
        const double ds = static_cast<double>(s);
        for (std::int32_t j = 0; j < nb; ++j) {
            const std::int32_t src = B->data[j] - 1;
            std::memcpy(X + e * 3, X + src * 3, 0x10);
            const double dz = step * ds;                  // 1402bbf8f
            X[e * 3 + 2] = X[src * 3 + 2] + dz;           // 1402bc03a
            if (prim->ctx != nullptr) ctx[e] = src;
            ++e;
        }
    }
    prim->u23e4 = (nsec / 2) * nb + prim->u23e4;
    prim->u23e8 = nsec * nb + prim->u23e8;
    prim->u23ec = nb;
    const double top = prim->u2360;
    const double* last = nullptr;   // local_40
    for (std::int32_t k = 0; k < np; ++k) {
        if (k == 0) last = X + e * 3;
        std::memcpy(X + e * 3, X + k * 3, 0x10);
        X[e * 3 + 2] = X[k * 3 + 2] + top;
        if (prim->ctx != nullptr) ctx[e] = k;
        ++e;
    }
    const std::int32_t nel = mg::fun_1402e9b70(prim);
    std::int32_t nq = nb * prim->sections + nel * 2;
    std::int32_t* Q = alloc_n<std::int32_t>(static_cast<std::int64_t>(nq * 4));
    const char flip = mg::fun_1402bc720(prim, X) ? 1 : 0;
    const char closed = 0;
    nq = mg::fun_1402bc980(prim, &prim->sections, X, Q, &flip, &closed);
    LLOBJ_DATA* o = prim->obj;
    g_ext.mesh_alloc(o, e, 0, 4, prim->model->u15f8, 0);   // 140202700
    for (std::int32_t k = 0; k < e; ++k) {
        double* c = g_ext.node_coords(reinterpret_cast<Cls_14017ef20*>(o->s8a0));   // 1401808d0
        std::memcpy(c + k * 3, X + k * 3, 0x18);
    }
    def::model::bc::set_boundary_count(*o, 1);          // 140163300
    def::model::bc::set_boundary_face_count(*o, nq, 0); // 140200a70
    for (std::int32_t k = 0; k < nq; ++k) {
        std::int32_t* faces = rec<std::int32_t*>(o, 0xbf8);
        std::memcpy(faces + (k << 2), Q + k * 4, 0x10);
    }
    Cls_140258580* w = gk::wpaxis_get(wpaxis_list(*o), 1);
    rec<std::int32_t>(w, 4) = 1;
    std::memcpy(&rec<double>(w, 8), first, 0x18);
    std::memcpy(&rec<double>(w, 0x20), last, 0x18);
    std::free(Q);
    std::free(X);
    return true;
}

// @src 1402bc580  axis file
void fun_1402bc580(DEF_GEOPRIM* prim)
{
    if (std::strlen(prim->u1b30) == 0 || prim->obj == nullptr || prim->obj->u87c[0] == 0 ||
        prim->kind != 1)
        return;
    std::FILE* f = _fsopen(prim->u1b30, "wt", _SH_DENYNO);   // 14047b934, str 14053e0c4
    if (f == nullptr) return;
    def::rt::crt::fprintf(f, "0\t0\t0\n");
    const double* a = g_ext.node_coords(reinterpret_cast<Cls_14017ef20*>(prim->obj->s8a0));
    const double* b = g_ext.node_coords(reinterpret_cast<Cls_14017ef20*>(prim->obj->s8a0));
    const double* c = g_ext.node_coords(reinterpret_cast<Cls_14017ef20*>(prim->obj->s8a0));
    def::rt::crt::fprintf(f, "%lf\t%lf\t%lf\n", c[0], b[1], a[2]);
    const double* d = prim->extrude->dir;
    def::rt::crt::fprintf(f, "%lf\t%lf\t%lf\n", d[0], d[1], d[2]);
    def::rt::crt::fprintf(f, "0.0\t0.0\n");
    std::fclose(f);
}

// @src 1402c1130  orientation
void fun_1402c1130(DEF_GEOPRIM* prim, const char* unused)
{
    (void)unused;
    const double origin[3] = {0.0, 0.0, 0.0};   // local_190
    const double ex[3] = {kOne, 0.0, 0.0};      // local_160
    const double ey[3] = {0.0, kOne, 0.0};      // local_1a8
    const double ez[3] = {0.0, 0.0, kOne};      // local_178
    if (prim->extrude == nullptr) return;
    const double* d = prim->extrude->dir;
    mg::P904Ext& v = mg::p904_ext();
    if (!(std::fabs(v.dot(d, ez)) <= k09)) return;
    double axis[3] = {0.0, 0.0, 0.0};   // local_148..138
    LLOBJ_DATA* o = prim->obj;
    if (v.dot(d, ex) <= k09) {
        if (km09 < v.dot(d, ex)) {
            if (v.dot(d, ey) <= k09) {
                if (v.dot(d, ey) <= km09) {
                    axis[0] = kMinusOne;
                    g_ext.rotate(o, origin, axis, km90);
                }
            } else {
                axis[0] = kMinusOne;
                g_ext.rotate(o, origin, axis, km90);
                axis[0] = 0.0;
                axis[1] = kMinusOne;   // set, not used
                const double t[3] = {0.0, prim->extrude->length, 0.0};
                const char fa = 0, fb = 1;
                g_ext.translate(o, t, &fa, &fb);
            }
        } else {
            axis[1] = kOne;
            g_ext.rotate(o, origin, axis, k90);
            axis[0] = kOne;
            axis[1] = 0.0;
            g_ext.rotate(o, origin, axis, k90);
            const double t[3] = {-prim->extrude->length, 0.0, 0.0};   // XOR sign mask
            const char fa = 0, fb = 1;
            g_ext.translate(o, t, &fa, &fb);
        }
    } else {
        axis[1] = kOne;
        g_ext.rotate(o, origin, axis, k90);
        axis[0] = kOne;
        axis[1] = 0.0;
        g_ext.rotate(o, origin, axis, k90);
    }
}

// @src 1402dc560  DEF_GEOPRIM.KEY
void fun_1402dc560(DEF_GEOPRIM* prim)
{
    g_ext.remove_if_exists(prim->key_name);   // 1401b85d0
    LLOBJ_DATA* o = prim->obj;
    if (o == nullptr) return;
    if (!(gk::has_surface_flag(*o) || o->u87c[0] != 0)) return;
    if (prim->model == nullptr) return;
    std::FILE* f = _fsopen(prim->key_name, "wt", _SH_DENYNO);   // str 14053eb8c
    if (f == nullptr) return;
    DB_STEP_DATA& m = *prim->model;
    if (gk::has_surface_flag(*o)) def::key::g2geo::surface::write_geometry_dispatch(m, f, *o, 1);
    if (o->u87c[0] != 0) {
        def::key::g2geo::write_nodes(m, f, *o, 1);      // 1402a2db0
        def::key::g2geo::write_elements(m, f, *o, 1);   // 1402a30c0
        g_ext.write_state_var(m, f, *o, 0x3d, 1, 0);    // 1402a4a80
    }
    if (prim->u23c8 != 0) {
        g_ext.write_state_var(m, f, *o, 0x12, 1, 0);
        g_ext.write_state_var(m, f, *o, 0x13, 1, 0);
        if (prim->u24d8 != 0) g_ext.write_state_var(m, f, *o, 0x44, 1, 0);
    }
    std::fclose(f);
}

} // namespace def::cmd::geoprim
