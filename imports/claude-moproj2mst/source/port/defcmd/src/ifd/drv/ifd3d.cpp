// 3D interference kernel, C++ part (packet p67-2): 1401a3aa0 (direction normalisation, backwards
// retry), 1401a3ea0 (load target S and mover T, rotate, one-sided distance), 1401a5470 (two rigid
// surfaces, two passes), and the node/element accessors they use. Spec:
// docs/re/commands/OBJPOS_interference.md sections 5.1-5.3, 5.6; decompiles/listings in
// deform-rea-work/out/p672/all_dec.c, all_lst.txt.
#include "ifd3d.hpp"
#include <defcmd/contact_cmds.hpp>
#include <defmodel/node_access.hpp>

#include <cmath>
#include <cstdlib>
#include <cstring>
#include <vector>

#include <defrt/ftn/geoutl.hpp>
#include <defrt/stdlib/libm.hpp>
#include <defmodel/bc/node_arrays.hpp>
#include <defmodel/geo_kw/boundary.hpp>
#include <defmodel/geo_kw/geo_kw.hpp>
#include <defmodel/geo_kw/surface_model.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/layout/obj_geo3d.hpp>
#include <defrt/ftn/helpers.hpp>
#include <defrt/rdata.hpp>

namespace def::cmd::ifd {

namespace geo = def::rt::geoutl;
namespace gk = def::model::geo_kw;

namespace {

template <class T, class C>
T& at(const C* base, std::size_t off)
{
    return *reinterpret_cast<T*>(const_cast<unsigned char*>(reinterpret_cast<const unsigned char*>(base)) + off);
}

std::int32_t type_of(LLOBJ_DATA* o) { return at<std::int32_t>(o, 0xc); }
std::int32_t mesh_of(LLOBJ_DATA* o) { return at<std::int32_t>(o, 0x87c); }
bool geom_of(LLOBJ_DATA* o) { return gk::has_surface_flag(*o); }  // 140160d90

// rdata 140509c90  1.0
constexpr double kOne = def::rt::rdata::d_140509c90;
// rdata 140522fc8  -1.0 (reverse direction / result)
constexpr double kMinusOne = def::rt::rdata::d_140522fc8;

// The geometry of one body as 1401a3ea0 / 1401a5470 load it.
struct Body {
    std::int32_t nnode = 0;
    std::int32_t nface = 0;
    std::vector<double> x;           // coordinates P(3, nnode) (copy)
    std::vector<std::int32_t> faces; // F(4, nface)
};

// part of @src 1401a3ea0 (the same code is inlined twice there and twice in 1401a5470): the DIEGEO
// surface when (type == 0 && geom) || (type != 0 && geom && !mesh), else the FE mesh with its
// boundary faces from FL3 FNDBND.
bool uses_surface(LLOBJ_DATA* o)
{
    return (type_of(o) == 0 && geom_of(o)) || (type_of(o) != 0 && geom_of(o) && mesh_of(o) == 0);
}

// @src 1401b06e0  (inlined here) surface element-array handle: OBJ_GEO3D + 0x18 (elems), read
//                 through the detaching operator[] 140005100 = gk::geo3d_elems per entry
void load_body(LLOBJ_DATA* o, Body& b)
{
    if (uses_surface(o)) {
        model::OBJ_GEO3D* s = gk::surface(*o, 0);  // 140198620(obj, &0)
        b.nnode = s->node_count();                 // 1400034d0
        s = gk::surface(*o, 0);
        b.nface = s->elem_count();                 // 140003500
        b.x.assign(static_cast<std::size_t>(b.nnode) * 3, 0.0);
        b.faces.assign(static_cast<std::size_t>(b.nface) * 4, 0);
        s = gk::surface(*o, 0);
        const double* xyz = gk::geo3d_coords(s);  // 140006410
        if (b.nnode * 3 > 0) std::memcpy(b.x.data(), xyz, static_cast<std::size_t>(b.nnode * 3) * 8);
        s = gk::surface(*o, 0);
        // 1401b06e0 + 140005100: element entries (a detaching access per entry), 4th through abs
        for (std::int32_t k = 0; k < b.nface; ++k) {
            const std::int32_t* el = gk::geo3d_elems(s);
            b.faces[static_cast<std::size_t>(k) * 4 + 0] = el[k * 4 + 0];
            b.faces[static_cast<std::size_t>(k) * 4 + 1] = el[k * 4 + 1];
            b.faces[static_cast<std::size_t>(k) * 4 + 2] = el[k * 4 + 2];
            const std::int32_t v = el[k * 4 + 3];
            b.faces[static_cast<std::size_t>(k) * 4 + 3] = v < 0 ? -v : v;  // 140484ab8 abs
        }
        return;
    }
    b.nnode = at<std::int32_t>(o, 0x880);
    std::int32_t nelem = at<std::int32_t>(o, 0x884);
    // obj+0x890: 2 tetrahedra (4 nodes, 4 faces), 4 bricks (8 nodes, 6 faces); other kinds leave
    // both counts uninitialised in the binary (0 here)
    std::int32_t npe = 0;
    std::int32_t nfe = 0;
    if (at<std::int32_t>(o, 0x890) == 2) {
        npe = 4;
        nfe = 4;
    } else if (at<std::int32_t>(o, 0x890) == 4) {
        npe = 8;
        nfe = 6;
    }
    b.x.assign(static_cast<std::size_t>(b.nnode) * 3, 0.0);
    std::vector<std::int32_t> conn(static_cast<std::size_t>(nelem) * static_cast<std::size_t>(npe), 0);
    const double* xyz = node_coords(&at<unsigned char>(o, 0x8a0));
    if (b.nnode * 3 > 0) std::memcpy(b.x.data(), xyz, static_cast<std::size_t>(b.nnode * 3) * 8);
    const std::int32_t* c = elem_conn(&at<unsigned char>(o, 0x9e0));
    if (nelem * npe > 0) std::memcpy(conn.data(), c, static_cast<std::size_t>(nelem * npe) * 4);
    // FUN_140256dc0: calloc(n, 4) for the face and element-of-face arrays
    std::vector<std::int32_t> faces(static_cast<std::size_t>(nelem) * static_cast<std::size_t>(nfe) * 4 + 1, 0);
    std::vector<std::int32_t> elem(static_cast<std::size_t>(nelem) * static_cast<std::size_t>(nfe) + 1, 0);
    std::int32_t nf = 0;
    gk::fl3_fndbnd(&nelem, conn.data(), &npe, &nf, faces.data(), elem.data());  // 140354ed0 (timing 0)
    b.faces.assign(faces.begin(), faces.begin() + static_cast<std::ptrdiff_t>(nf) * 4);
    b.nface = nf;
}

} // namespace

// 1401808d0 (with 14017f020 inlined): ported in defmodel/node_access.hpp (fun_1401808d0)
double* node_coords(void* node_set) { return model::fun_1401808d0(static_cast<model::Cls_14017ef20*>(node_set)); }

// 140183cb0: ported in defcmd/contact_cmds.hpp (element_set_conn)
std::int32_t* elem_conn(void* elem_set) { return element_set_conn(elem_set); }

// @src 1401a3ea0  S = target, T = mover (spec 5.2)
void fun_1401a3ea0(DB_STEP_DATA* /*m*/, LLOBJ_DATA* s, LLOBJ_DATA* t, double* u, double tol,
                   double* out, std::int32_t* err, std::int32_t flag)
{
    double r[9] = {kOne, 0.0, 0.0, 0.0, kOne, 0.0, 0.0, 0.0, kOne};
    *err = 0;
    double a[3];
    std::memcpy(a, u, sizeof a);
    const double l = std::sqrt(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);  // 140484644
    double w[3] = {a[0] / l, a[1] / l, a[2] / l};
    geo::fun_1404cf5c0(w, r);
    Body bs;
    Body bt;
    load_body(s, bs);
    load_body(t, bt);
    std::vector<double> ys(bs.x.size(), 0.0);
    std::vector<double> yt(bt.x.size(), 0.0);
    double origin[3] = {0.0, 0.0, 0.0};
    std::int32_t k = 1;
    if (type_of(t) != 0) {
        geo::fun_1404cf5c0(w, r);
        geo::fun_1404d0939(bs.x.data(), origin, r, ys.data(), &k, &bs.nnode);
        geo::fun_1404d0939(bt.x.data(), origin, r, yt.data(), &k, &bt.nnode);
    }
    if (type_of(t) == 0) {
        double wn[3];
        for (int i = 0; i < 3; ++i) wn[i] = -w[i];  // xor with the sign bit (14051bec0)
        geo::fun_1404cf5c0(wn, r);
        geo::fun_1404d0939(bs.x.data(), origin, r, ys.data(), &k, &bs.nnode);
        geo::fun_1404d0939(bt.x.data(), origin, r, yt.data(), &k, &bt.nnode);
    }
    std::int32_t e = 0;
    double d = 0.0;
    std::int32_t minus = -1;
    double tolv = tol;
    std::int32_t flagv = flag;
    if (type_of(t) != 0) {
        std::vector<std::int32_t> work(static_cast<std::size_t>(bt.nnode) + 1, 0);
        fun_1403539a1(ys.data(), yt.data(), &tolv, bs.faces.data(), bt.faces.data(), &bs.nface,
                      &bt.nface, &bs.nnode, &bt.nnode, work.data(), &d, &k, &e, &flagv);
    }
    if (type_of(t) == 0) {
        std::vector<std::int32_t> work(static_cast<std::size_t>(bs.nnode) + 1, 0);
        fun_1403539a1(yt.data(), ys.data(), &tolv, bt.faces.data(), bs.faces.data(), &bt.nface,
                      &bs.nface, &bt.nnode, &bs.nnode, work.data(), &d, &minus, &e, &flagv);
    }
    if (e == 1) *err = 1;
    out[0] = w[0] * d;
    out[1] = w[1] * d;
    out[2] = w[2] * d;
}

// @src 1401a5470  both bodies rigid DIEGEO surfaces (spec 5.3). The binary inlines the loader of
// 1401a3ea0 again; its mesh branches cannot be reached from 1401a3aa0's test.
void fun_1401a5470(DB_STEP_DATA* /*m*/, LLOBJ_DATA* s, LLOBJ_DATA* t, double* u, double tol,
                   double* out, std::int32_t* err, std::int32_t flag)
{
    double r[9] = {kOne, 0.0, 0.0, 0.0, kOne, 0.0, 0.0, 0.0, kOne};
    *err = 0;
    double a[3];
    std::memcpy(a, u, sizeof a);
    const double l = std::sqrt(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);
    double w[3] = {a[0] / l, a[1] / l, a[2] / l};
    geo::fun_1404cf5c0(w, r);
    Body bs;
    Body bt;
    load_body(s, bs);
    load_body(t, bt);
    std::vector<double> ysw(bs.x.size(), 0.0), ytw(bt.x.size(), 0.0);
    std::vector<double> ysn(bs.x.size(), 0.0), ytn(bt.x.size(), 0.0);
    double origin[3] = {0.0, 0.0, 0.0};
    std::int32_t k = 1;
    geo::fun_1404cf5c0(w, r);
    geo::fun_1404d0939(bs.x.data(), origin, r, ysw.data(), &k, &bs.nnode);
    geo::fun_1404d0939(bt.x.data(), origin, r, ytw.data(), &k, &bt.nnode);
    double wn[3];
    for (int i = 0; i < 3; ++i) wn[i] = -w[i];  // xor with the sign bit
    geo::fun_1404cf5c0(wn, r);
    geo::fun_1404d0939(bs.x.data(), origin, r, ysn.data(), &k, &bs.nnode);
    geo::fun_1404d0939(bt.x.data(), origin, r, ytn.data(), &k, &bt.nnode);
    std::int32_t minus = -1;
    double d = 0.0;
    double d1 = 0.0;
    double d2 = 0.0;
    std::int32_t e = 0;
    std::int32_t e1 = 0;
    std::int32_t e2 = 0;
    double tolv = tol;
    std::int32_t flagv = flag;
    {
        std::vector<std::int32_t> work(static_cast<std::size_t>(bt.nnode) + 1, 0);
        fun_1403539a1(ysw.data(), ytw.data(), &tolv, bs.faces.data(), bt.faces.data(), &bs.nface,
                      &bt.nface, &bs.nnode, &bt.nnode, work.data(), &d1, &k, &e, &flagv);
    }
    if (e == 1) e1 = 1;
    e = 0;
    {
        std::vector<std::int32_t> work(static_cast<std::size_t>(bs.nnode) + 1, 0);
        fun_1403539a1(ytn.data(), ysn.data(), &tolv, bt.faces.data(), bs.faces.data(), &bt.nface,
                      &bs.nface, &bt.nnode, &bs.nnode, work.data(), &d2, &minus, &e, &flagv);
    }
    if (e == 1) e2 = 1;
    if (e1 == 0 && e2 == 0) d = (std::fabs(d2) < std::fabs(d1)) ? d2 : d1;
    if (e1 == 0 && e2 == 1) d = d1;
    if (e1 == 1 && e2 == 0) d = d2;
    if (e1 == 1 && e2 == 1) *err = 1;
    out[0] = w[0] * d;
    out[1] = w[1] * d;
    out[2] = w[2] * d;
}

// @src 1401a3aa0  spec 5.1. a = mover, b = target. The early returns leave *dist unchanged.
bool fun_1401a3aa0(DB_STEP_DATA* m, LLOBJ_DATA* a, LLOBJ_DATA* b, double* dir, double* tol,
                   double* dist, double* disp, char flag)
{
    const std::int32_t fl = flag != 0 ? 1 : 0;
    if (a == nullptr || b == nullptr) return false;
    double u[3];
    std::memcpy(u, dir, sizeof u);
    def::rt::ftn::vec3_normalize(u);  // 14023b720
    if (type_of(b) == 0 && !geom_of(b)) return false;
    if (type_of(a) == 0 && !geom_of(a)) return false;
    if (!(type_of(b) == 0 || mesh_of(b) != 0 || geom_of(b))) return false;
    if (!(type_of(a) == 0 || mesh_of(a) != 0 || geom_of(a))) return false;
    double r[3];
    std::int32_t err = 0;
    if (type_of(b) == 0 && geom_of(b) && mesh_of(b) == 0 && type_of(a) == 0 && geom_of(a) && mesh_of(a) == 0)
        fun_1401a5470(m, b, a, u, *tol, r, &err, fl);
    else
        fun_1401a3ea0(m, b, a, u, *tol, r, &err, fl);
    if (err == 0) {
        *dist = std::sqrt(r[0] * r[0] + r[1] * r[1] + r[2] * r[2]);
        if (disp != nullptr) std::memcpy(disp, r, sizeof r);
        return true;
    }
    for (int i = 0; i < 3; ++i) u[i] = u[i] * kMinusOne;  // 14023b2d0
    fun_1401a3ea0(m, b, a, u, *tol, r, &err, fl);
    if (err == 0) {
        for (int i = 0; i < 3; ++i) r[i] = r[i] * kMinusOne;
        *dist = std::sqrt(r[0] * r[0] + r[1] * r[1] + r[2] * r[2]);
        if (disp != nullptr) std::memcpy(disp, r, sizeof r);
        return true;
    }
    *dist = 0.0;
    return false;
}

} // namespace def::cmd::ifd
