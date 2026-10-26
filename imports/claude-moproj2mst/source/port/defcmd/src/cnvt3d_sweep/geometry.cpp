// CNVT3D geometry generation (kinds 5 and 2) and the small helpers of packet p9-03a.
// Spec: CNVT3D_SWEEP.md section 10; FP order and comparisons from the listing (all_l.txt).
#define _CRT_SECURE_NO_WARNINGS
#include <defcmd/cnvt3d_sweep.hpp>

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <type_traits>
#include <vector>

#include <defcmd/meshgen/meshgen_base.hpp>
#include <defmodel/bc/boundary_topology.hpp>
#include <defmodel/geo_kw/geo_kw.hpp>
#include <defmodel/geoprim/def_geoprim.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/geo2d_and_mesh2d.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/layout/obj_geo2d.hpp>
#include <defmodel/layout/obj_geo2d_digitized.hpp>
#include <defmodel/llobj/cls_14017e740.hpp>
#include <defrt/port.hpp>

#include <defcmd/ifd/cube_lib.hpp>
#include "internal.hpp"

namespace def::cmd::cnvt3d_sweep {

using namespace detail;
using model::Cls_14017e740;
using model::OBJ_GEO2D;
using model::OBJ_GEO2D_DIGITIZED;
using model::geoprim::DEF_GEOPRIM;

namespace {

DB_STEP_DATA* model3(MeshGenBase* cvt) { return at<DB_STEP_DATA*>(cvt, 0x3f50); }
std::int32_t work_id(MeshGenBase* cvt) { return at<std::int32_t>(cvt->u010, 8); }
Cls_14017e740& boundary0(LLOBJ_DATA* o) { return at<Cls_14017e740>(o, 0xbe0); }

// The GEOPRIM face-kind list +0x2518 holds ints (1402ce310 pushes 0 / 1 through 1401599e0, the
// list<int> push_back; p9-02 declares the element as 8 bytes): the int is the slot's low 4 bytes.
template <class T>
std::int32_t as_int(const T& v)
{
    std::int32_t i;
    std::memcpy(&i, &v, 4);
    return i;
}

} // namespace

// ---- small getters of the packet list ----------------------------------------------------------

// @src 1401590d0
void fun_1401590d0(OBJ_GEO2D_DIGITIZED* d, double tol) { d->u18 = tol; }
// @src 1401590f0
std::int32_t fun_1401590f0(const OBJ_GEO2D_DIGITIZED* d) { return d->n_points; }
// @src 140159100
double* fun_140159100(const OBJ_GEO2D_DIGITIZED* d) { return d->points; }
// @src 140159110
bool fun_140159110(const OBJ_GEO2D* g)
{
    if (g->ext == nullptr) return false;
    return 0 < fun_1401590f0(g->ext);
}
// @src 140159150
OBJ_GEO2D_DIGITIZED* fun_140159150(const OBJ_GEO2D* g) { return g->ext; }
// @src 1401591b0
bool fun_1401591b0(const DEF_GEOPRIM* p) { return p->u24fc != 0 && p->u24fd != 0; }

// ---- geometry ------------------------------------------------------------------------------------

// @src 140153710  geometry of the 3D object (CNVT3D_SWEEP.md 10.1)
void fun_140153710(MeshGenBase* cvt, std::int32_t kd, double start, double ang, LLOBJ_DATA* obj,
                   MeshGenBase* mg, double c)
{
    E().fun_140155340(cvt, obj, c);
    LLOBJ_DATA* o3 = E().fun_14019ccc0(model3(cvt), work_id(cvt));
    const char* prob = static_cast<const char*>(ptr(cvt->u3f48, 0x340));
    if (E().fun_140160d90(o3)) {
        E().fun_140148840(mg, o3, model3(cvt), prob);
        return;
    }
    if (obj == nullptr) return;
    if (at<char>(obj, 0x2c20) == 0 || at<std::int32_t>(obj, 0x87c) == 0) {
        if (!E().fun_140160cc0(obj)) {
            std::printf("2D DB is not correct!");
            return;
        }
        fun_140153c00(cvt, obj, o3, model3(cvt));
    } else {
        fun_140154270(cvt, obj, o3, model3(cvt));
    }
    DB_STEP_DATA* prev = E().fun_1401d8120(cvt->u3f48);
    if (prev != nullptr) {
        LLOBJ_DATA* po = E().fun_1401d6ea0(prev, work_id(cvt));
        if (po != nullptr) {
            const char by_dim = 1;  // local_10
            const char shapes = 1;  // local_f
            E().fun_140189b70(ptr(o3, 0x2d10), ptr(po, 0x2d10), &shapes, &by_dim);
        }
    }
    if (kd == 1) E().fun_140198d30(o3, start, ang);
    E().fun_140148840(mg, o3, model3(cvt), prob);
}

// @src 140153910  extrude (kd 2): type-3 mesh windows, z within 1e-4 of an end moves 0.001*|len|
//                 outwards (COMISD 1e-4, d; JBE: a NaN distance moves nothing)
void fun_140153910(MeshGenBase*, std::int32_t kd, double start, double len, LLOBJ_DATA* mgobj)
{
    if (mgobj == nullptr) return;
    if (!(0 < E().fun_140189580(ptr(mgobj, 0x2d10)))) return;
    if (!(at<double>(mgobj, 0x2d40) > 0.0)) return;
    for (std::int32_t k = 0; k < E().fun_140189580(ptr(mgobj, 0x2d10)); k = k + 1) {
        void* w = E().fun_140269900(ptr(mgobj, 0x2d50), k + 1);
        if (w == nullptr) continue;
        const std::int32_t t = at<std::int32_t>(w, 0x210);
        if (t == 1 || t == 2 || t != 3) continue;
        void* g = ptr(w, 0x220);
        const std::int32_t n = E().fun_1400034d0(g);
        E().fun_140003500(g);
        double* x = E().fun_140006410(g);
        E().fun_140005510(g);
        const double d = std::fabs(len) * 0.001;  // DAT_14051f1e8
        for (std::int32_t i = 0; i < n; i = i + 1) {
            if (kd == 1 || kd != 2) continue;
            double& z = x[i * 3 + 2];
            if (1e-4 > std::fabs(z - start)) {  // DAT_14051f250
                z = z - d;
            } else if (1e-4 > std::fabs((z - start) - len)) {
                z = z + d;
            }
        }
    }
}

// @src 140153c00  GEOPRIM from loop 0 of geometry 0 (CNVT3D_SWEEP.md 10.2)
bool fun_140153c00(MeshGenBase*, LLOBJ_DATA* obj, LLOBJ_DATA* o3, DB_STEP_DATA* m3)
{
    DEF_GEOPRIM prim;  // 1402b7ad0 .. 1402b8090
    const double tol = 0.01;  // DAT_14051f258 (local_80)
    const double axis_dir[3] = {0.0, 0.0, 1.0};  // local_25f0 / local_25e0
    const double axis_pt[3] = {0.0, 0.0, 0.0};   // local_a8
    const double ext_dir[3] = {0.0, 0.0, 1.0};   // local_2608 / 2600 / 25f8
    model::GEO2D_AND_MESH2D* geo = model::geo_kw::geometry(*obj, 0);  // 140197f40
    if (geo == nullptr) return false;
    if (!(0 < static_cast<std::int32_t>(geo->edges.size()))) return false;  // 140159160
    // 14027ff20(geo, 0): edge 0's curve when the edge count allows it
    OBJ_GEO2D* loop = 0 < static_cast<std::int32_t>(geo->edges.size()) ? geo->edge_curve(0) : nullptr;
    if (loop == nullptr || loop->n_points < 1) return false;
    if (!fun_140159110(loop)) loop->make_ext();  // 140277830
    fun_1401590d0(fun_140159150(loop), tol);
    fun_1402778c0(loop);
    std::int32_t npts = fun_1401590f0(fun_140159150(loop));
    double* pts = fun_140159100(fun_140159150(loop));
    // 140153e92: UCOMISD + JP/JNZ, a NaN never compares equal
    if (pts[0] == pts[npts * 2 - 2] && pts[1] == pts[npts * 2 - 1]) npts = npts - 1;
    for (std::int32_t k = 1; k <= npts; k = k + 1) {
        // 140153f3b: COMISD tol*0.1, |r|; JC skips (a NaN radius is kept)
        if (std::fabs(pts[k * 2 - 2]) <= tol * 0.1) pts[k * 2 - 2] = 0.0;
    }
    if (at<std::int32_t>(obj, 0x2be8) == 1) {
        const std::int32_t n_list = 0;  // local_74
        const std::int32_t ne = 0;      // local_70
        const std::int32_t nn = 0;      // local_6c
        prim.setup_revolve(1, 0, &at<double>(obj, 0x2bf8), &at<double>(obj, 0x2bf0), axis_dir, axis_pt,
                           &at<std::int32_t>(obj, 0x2c24), &npts, pts, &nn, nullptr, &ne, nullptr,
                           &n_list, nullptr);
    } else {
        const double d2450 = 0.0;   // local_68
        const double d2448 = 0.0;   // local_60
        const double v2498 = 0.0;   // local_58
        const double v2490 = 0.0;   // local_50
        const double d2388 = 1.0;   // local_48
        const std::int32_t i23cc = 0;  // local_40
        const char b23c9 = 0;       // local_3c
        const char b23c8 = 0;       // local_3b
        const std::int32_t n_list = 0;  // local_38
        const std::int32_t ne = 0;      // local_34
        const std::int32_t nn = 0;      // local_30
        const std::int32_t count = 2;   // local_2c
        prim.setup_extrude(1, 0, &at<double>(obj, 0x2c10), &count, &npts, pts, &nn, nullptr, &ne, nullptr,
                           &n_list, nullptr, &b23c8, &b23c9, &i23cc, &d2388, &v2490, &v2498, &d2448,
                           &d2450, ext_dir);
    }
    const char flag = 0;  // local_28
    E().fun_1402ba500(&prim, o3, &flag, m3, nullptr);
    return true;
}

// @src 140154270  GEOPRIM from the 2D mesh boundary (CNVT3D_SWEEP.md 10.3). The face-kind list the
//                 decompile shows as the uninitialised local_180 is RSP+0x2608 = prim+0x2518
//                 (prim at RSP+0xf0, 0x2520 bytes): 0 keeps a face, 1 splits it in two triangles.
bool fun_140154270(MeshGenBase*, LLOBJ_DATA* obj, LLOBJ_DATA* o3, DB_STEP_DATA* m3)
{
    DEF_GEOPRIM prim;
    const double axis_dir[3] = {0.0, 0.0, 1.0};  // local_26b8 / 26a8
    const double axis_pt[3] = {0.0, 0.0, 0.0};   // local_178 / 170 / 168
    const double ext_dir[3] = {0.0, 0.0, 1.0};   // local_26d0 / 26c8 / 26c0
    std::int32_t sec = at<std::int32_t>(obj, 0x2c24) < 1 ? at<std::int32_t>(obj, 0x2c28)
                                                         : at<std::int32_t>(obj, 0x2c24);
    Cls_14017e740& b2 = boundary0(obj);
    if (at<std::int32_t>(obj, 0x2be8) == 1) {
        const std::int32_t n_prof = 0;  // local_108
        const std::int32_t* conn = E().fun_140183cb0(ptr(obj, 0x9e0));
        const double* xy = E().fun_1401808d0(ptr(obj, 0x8a0));
        prim.setup_revolve(0, 2, &at<double>(obj, 0x2bf8), &at<double>(obj, 0x2bf0), axis_dir, axis_pt,
                           &sec, &n_prof, nullptr, &at<std::int32_t>(obj, 0x880), xy,
                           &at<std::int32_t>(obj, 0x884), conn, &b2.u000,
                           static_cast<const std::int32_t*>(b2.u008));
    } else if (at<std::int32_t>(obj, 0x2be8) == 2) {
        const double d2450 = 0.0;   // local_100
        const double d2448 = 0.0;   // local_f8
        const double v2498 = 0.0;   // local_f0
        const double v2490 = 0.0;   // local_e8
        const double d2388 = 1.0;   // local_e0
        const std::int32_t i23cc = 0;  // local_d8
        const char b23c9 = 0;       // local_d4
        const char b23c8 = 0;       // local_d3
        const std::int32_t n_prof = 0;  // local_d0
        const std::int32_t count = 2;   // local_cc
        const std::int32_t* conn = E().fun_140183cb0(ptr(obj, 0x9e0));
        const double* xy = E().fun_1401808d0(ptr(obj, 0x8a0));
        prim.setup_extrude(0, 2, &at<double>(obj, 0x2c10), &count, &n_prof, nullptr,
                           &at<std::int32_t>(obj, 0x880), xy, &at<std::int32_t>(obj, 0x884), conn,
                           &b2.u000, static_cast<const std::int32_t*>(b2.u008), &b23c8, &b23c9, &i23cc,
                           &d2388, &v2490, &v2498, &d2448, &d2450, ext_dir);
    }
    // builder context: NN * (sec + 1) ints, not initialised (1404797d8), freed at the end
    const std::uint64_t nctx = static_cast<std::uint64_t>(
        static_cast<std::int64_t>(at<std::int32_t>(obj, 0x880) * (sec + 1)));
    void* ctx = std::malloc(static_cast<std::size_t>(nctx * 4));
    const char flag = 0;  // local_c0
    E().fun_1402ba500(&prim, o3, &flag, m3, ctx);
    if (fun_1401591b0(&prim)) {
        Cls_14017e740& b3 = boundary0(o3);
        const std::int32_t nf = b3.u010;
        std::vector<std::int32_t> faces(static_cast<std::size_t>(nf) * 4, 0);  // local_158
        std::vector<std::int32_t> neg(static_cast<std::size_t>(nf), 0);        // local_138
        std::memcpy(faces.data(), b3.u018, static_cast<std::size_t>(nf * 4) * 4);
        for (std::int32_t i = 0; i < nf; i = i + 1) {
            neg[i] = 0;
            if (faces[i * 4 + 3] < 0) {
                faces[i * 4 + 3] = std::abs(faces[i * 4 + 3]);  // 140484ab8
                neg[i] = -1;
            }
        }
        const std::int32_t nodes = b3.u000;
        b3.free_arrays();
        std::vector<std::int32_t> out;  // local_130 (list<int>)
        std::int32_t t1[4] = {0, 0, 0, 0};  // local_150[0..3]
        std::int32_t t2[4] = {0, 0, 0, 0};  // local_120[0..3]
        std::int32_t cnt = 0;              // local_150[5]
        auto& kinds = prim.l2518.mutable_items();
        for (std::int32_t i = 0; i < nf; i = i + 1) {
            // 140159a40: front of the list; an empty list would read the sentinel in the binary
            if (kinds.empty()) DEF_NOT_PORTED(0x140159a40);
            const std::int32_t v = as_int(kinds.front());
            if (v == 0) {
                for (std::int32_t j = 0; j < 4; j = j + 1) out.push_back(faces[i * 4 + j]);
            } else {
                fun_14023d410(E().fun_1401808d0(ptr(o3, 0x8a0)), faces.data(), i + 1, 4, t1, t2);
                if (neg[i] == 0) {
                    t1[3] = t1[0];
                    t2[3] = t2[0];
                } else {
                    t1[3] = -t1[0];
                    t2[3] = -t2[0];
                }
                for (std::int32_t j = 0; j < 4; j = j + 1) out.push_back(t1[j]);
                cnt = cnt + 1;
                for (std::int32_t j = 0; j < 4; j = j + 1) out.push_back(t2[j]);
            }
            cnt = cnt + 1;
            kinds.pop_front();  // 140159970
        }
        b3.init();
        b3.u000 = nodes;
        b3.u010 = cnt;
        model::bc::alloc_boundary_arrays(b3);
        std::int32_t* f = static_cast<std::int32_t*>(b3.u018);
        std::size_t r = 0;
        for (std::int32_t k = 1; k <= cnt; k = k + 1) {
            for (std::int32_t j = 0; j < 4; j = j + 1) f[k * 4 - 4 + j] = out[r++];
        }
    }
    if (at<std::int32_t>(obj, 0x2be8) == 1) E().fun_14021d2f0(o3, axis_pt, axis_dir, -90.0);  // DAT_14051f200
    char one = 1;  // local_50
    E().fun_1401fc730(o3, &one);
    E().fun_1401ffd80(o3);
    std::free(ctx);
    at<std::int32_t>(o3, 0x2d90) = at<std::int32_t>(obj, 0x2d80);
    return true;
}

// ---- quad split ----------------------------------------------------------------------------------

// @src 14023cdb0  sides a2 = |ab|^2, b2 = |bc|^2, c2 = |ca|^2 (14023b850), s = (a2+b2+c2)/2;
//                 area = sqrt(s*|(s-a2)(s-b2)(s-c2)|); q = sqrt(|..|/s) / (a2*b2*c2*0.25/area)
double fun_14023cdb0(const double* a, const double* b, const double* c, double* q)
{
    auto d2 = [](const double* p, const double* r) {
        return (p[0] - r[0]) * (p[0] - r[0]) + (p[1] - r[1]) * (p[1] - r[1]) + (p[2] - r[2]) * (p[2] - r[2]);
    };
    const double ab = d2(a, b);
    const double bc = d2(b, c);
    const double ca = d2(c, a);
    const double s = ((ab + bc) + ca) / 2.0;
    const double p = std::fabs(((s - ab) * (s - bc)) * (s - ca));
    const double area = std::sqrt(s * p);
    if (s > 1e-9) {  // COMISD s, 1e-9; JBE -> q = 0 (also for NaN)
        const double r = std::sqrt(p / s);
        double rr;
        if (area > 1e-9) {
            rr = ((ab * bc) * ca * 0.25) / area;
        } else {
            rr = 999999999.9999999;  // DAT_140531760
        }
        *q = r / rr;
    } else {
        *q = 0.0;
    }
    return area;
}

// @src 14023d410  split quad f into (n0 n1 n2)(n3 n0 n2) or (n0 n1 n3)(n2 n3 n1), by the better
//                 worst-quality ratio; a triangle (nper 3) is copied with a message
void fun_14023d410(const double* xyz, const std::int32_t* faces, std::int32_t f, std::int32_t nper,
                   std::int32_t* t1, std::int32_t* t2)
{
    const std::int32_t* n = faces + (f - 1) * nper;
    if (nper == 3) {
        t2[0] = n[0];
        t1[0] = t2[0];
        t2[1] = n[1];
        t1[1] = t2[1];
        t2[2] = n[2];
        t1[2] = t2[2];
        cube::fun_14039d3a0("defMath3_TriangleSplitD : Data is already triangluated, splitting is not needed\n",
                            nullptr, nullptr, 0);
        return;
    }
    const double* p0 = xyz + (n[0] - 1) * 3;
    const double* p1 = xyz + (n[1] - 1) * 3;
    const double* p2 = xyz + (n[2] - 1) * 3;
    const double* p3 = xyz + (n[3] - 1) * 3;
    double qa, qb, qc, qd;
    fun_14023cdb0(p0, p1, p3, &qa);
    fun_14023cdb0(p2, p3, p1, &qb);
    double r1 = qb > 1e-9 ? qa / qb : 0.0;
    if (r1 > 1.0) r1 = 1.0 / r1;
    fun_14023cdb0(p0, p1, p2, &qc);
    fun_14023cdb0(p3, p0, p2, &qd);
    double r2 = qd > 1e-9 ? qc / qd : 0.0;
    if (r2 > 1.0) r2 = 1.0 / r2;
    if (r1 > r2) {  // 14023d6a9 COMISD r1, r2; JBE (a NaN takes the second split)
        t1[0] = n[0];
        t1[1] = n[1];
        t1[2] = n[3];
        t2[0] = n[2];
        t2[1] = n[3];
        t2[2] = n[1];
    } else {
        t1[0] = n[0];
        t1[1] = n[1];
        t1[2] = n[2];
        t2[0] = n[3];
        t2[1] = n[0];
        t2[2] = n[2];
    }
}

} // namespace def::cmd::cnvt3d_sweep
