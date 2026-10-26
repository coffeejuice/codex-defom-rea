// ctest-labels: p67-3 drop
// Tier-1 test of the OBJPOS drop kernels (packet p67-3, worker A): vector helpers, Gauss table,
// characteristic length, mass properties, node velocities, 6x6 solve, stiffness, rigid move,
// projection/mirror helpers.
//
// Expected values: the same case driver (namespace dropcase below) was run against the ORIGINAL
// routines in a mapped copy of DEF_PRE_64.exe (pure math routines only, approved method), by the
// harness deform-rea-work/scripts_p673/a/dropgold.cpp, which includes this file with
// DROP_GOLD_HARNESS defined and prints the kGold* tables pasted below. Every recorded double
// must match bit for bit.
//
// The projection helpers and the constrained mass properties (nk = 1, 2, 3) call the FTN_geoutl
// kernels 1404cf5c0 / 1404ded0c, still stubs in src/drop/geoutl_pending.cpp; their cases run
// only when DEFCMD_DROP_GEOUTL_PORTED is defined (set it to 1 when the geoutl bodies land).
#include <cmath>
#include <cstdint>
#include <cstring>
#include <string>
#include <vector>

namespace dropcase {

using i32 = std::int32_t;

struct Api {
    void (*dot)(double*, double*, double*, i32*);
    void (*cross)(double*, double*, double*);
    void (*matvec)(double*, double*, double*, i32*);
    void (*matmul)(double*, double*, double*, i32*);
    void (*copy)(double*, double*, i32*);
    void (*zero)(i32*, i32*);
    void (*gauss)();
    void (*charlen)(double*, i32*, i32*, i32*, double*);
    void (*mass)(double*, i32*, i32*, i32*, i32*, double*, i32*, double*);
    void (*integrand)(double*, double*, double*, double*, double*, i32*);
    void (*shape)(double*, double*, double*, double*, double*);
    void (*plane)(double*, double*, double*);
    void (*axis)(double*, double*, double*);
    void (*mirror_m)(double*, double*, double*);
    void (*mirror_x)(double*, double*, double*);
    void (*vel)(double*, double*, i32*);
    void (*solve)(double*, double*, i32*, i32*);
    void (*stiff)(double*, double*, double*, double*);
    void (*move)(double*, double*, double*, double*, i32*, i32*);
    double* body; // COMMON 0x140b6cca0, 60 doubles
    double* gp;   // 0x140b6cba0, 16 doubles
    double* gw;   // 0x140b6cc20, 16 doubles
};

// body offsets (doubles) in the COMMON block, state.hpp BodyCommon
enum : int { B_CT = 3, B_A = 6, B_I = 20, B_M = 38, B_C = 39, B_V = 48, B_W = 51, B_N = 60 };

struct Rec {
    std::vector<double> v;
    std::vector<std::string> lab;
    void put(const char* l, double x)
    {
        v.push_back(x);
        lab.push_back(l);
    }
    void put(const char* l, const double* x, int n)
    {
        for (int i = 0; i < n; ++i) put(l, x[i]);
    }
};

inline void clear_body(const Api& api) { std::memset(api.body, 0, B_N * sizeof(double)); }

// ---- vector helpers, copy/zero, node velocities, solve, stiffness ----
inline void run_vec(const Api& api, Rec& r)
{
    double a[4] = {1.5, -2.25, 3.1, 0.7};
    double b[4] = {0.3, 4.4, -1.9, 2.0};
    for (i32 n : {3, 4, 1, 0}) {
        double out = 99.0;
        api.dot(a, b, &out, &n);
        r.put("dot", out);
    }
    double ca[3] = {1.1, -0.3, 2.7};
    double cb[3] = {-0.9, 1.6, 0.45};
    double cc[3];
    api.cross(ca, cb, cc);
    r.put("cross", cc, 3);
    double am[9] = {0.9, -0.1, 0.3, 0.2, 1.1, -0.4, 0.05, 0.6, 0.8};
    double bm[9] = {0.7, 0.33, -1.2, 2.1, 0.01, 0.6, -0.25, 1.75, 0.4};
    double xv[3] = {1.3, -2.1, 0.7};
    double y[3] = {7.0, 7.0, 7.0};
    i32 n3 = 3;
    api.matvec(am, xv, y, &n3);
    r.put("matvec3", y, 3);
    i32 n2 = 2;
    api.matvec(am, xv, y, &n2);
    r.put("matvec2", y, 3);
    double cm[9];
    api.matmul(am, bm, cm, &n3);
    r.put("matmul", cm, 9);
    double cp[5] = {0, 0, 0, 0, 0};
    i32 n4 = 4;
    api.copy(a, cp, &n4);
    r.put("copy", cp, 5);
    i32 iz[4] = {5, 6, 7, 8};
    i32 nz = 3;
    api.zero(iz, &nz);
    for (int i = 0; i < 4; ++i) r.put("zero", static_cast<double>(iz[i]));

    // node velocities
    clear_body(api);
    const double c[3] = {0.1, 0.2, -0.3}, v[3] = {1.0, -0.5, 0.25}, w[3] = {0.3, -0.7, 1.1};
    for (int i = 0; i < 3; ++i) {
        api.body[B_C + i] = c[i];
        api.body[B_V + i] = v[i];
        api.body[B_W + i] = w[i];
    }
    double xn[9] = {1.0, 2.0, 3.0, -0.75, 0.125, 4.5, 10.3, -7.7, 0.001};
    double vn[9];
    api.vel(xn, vn, &n3);
    r.put("vel", vn, 9);

    // 6x6 solve: mass block m*I3, inertia block, small coupling, pivoting needed in column 4
    double a6[36];
    double m6[36] = {2.5,  0.0,  0.0,  0.01, 0.0,  0.0,  0.0,  2.5,  0.0,  0.0,  0.02, 0.0,
                     0.0,  0.0,  2.5,  0.0,  0.0,  0.03, 0.0,  0.0,  0.0,  1e-12, 0.4, 0.1,
                     0.0,  0.0,  0.0,  0.3,  1.7,  -0.2, 0.0,  0.0,  0.0,  0.1,  -0.2, 1.9};
    double b6[6] = {0.0, 0.0, -24500.0, 13.7, -2.2, 0.35};
    std::memcpy(a6, m6, sizeof a6);
    i32 n6 = 6;
    i32 err = -5;
    api.solve(a6, b6, &n6, &err);
    r.put("solve.err", static_cast<double>(err));
    r.put("solve.b", b6, 6);
    r.put("solve.a", a6, 36);
    // singular 3x3 (second pivot 0): err = 1, partial state
    double s3[9] = {2.0, 4.0, 1.0, 1.0, 2.0, 0.5, 3.0, 1.0, 7.0};
    double sb[3] = {1.0, 2.0, 3.0};
    err = -5;
    api.solve(s3, sb, &n3, &err);
    r.put("solve3.err", static_cast<double>(err));
    r.put("solve3.b", sb, 3);
    r.put("solve3.a", s3, 9);

    // stiffness
    double ii[9] = {3.2, 0.1, -0.05, 0.1, 2.7, 0.2, -0.05, 0.2, 4.1};
    struct Sc { double v[3], w[3], p15, wt; } sc[] = {
        {{0, 0, 0}, {0, 0, 0}, 0.002, 24500.0},
        {{0.3, -1.2, 5.5}, {0.01, 0.2, -0.03}, 0.002, 24500.0},
        {{0, 0, 0}, {0.4, -0.1, 0.25}, 0.5, 1234.5},
        {{std::nan(""), 0, 0}, {0, 0, 0}, 0.002, 24500.0},
    };
    for (auto& s : sc) {
        clear_body(api);
        for (int i = 0; i < 9; ++i) api.body[B_I + i] = ii[i];
        api.body[B_M] = 2.5;
        for (int i = 0; i < 3; ++i) {
            api.body[B_V + i] = s.v[i];
            api.body[B_W + i] = s.w[i];
        }
        double k = -1.0, damp = -2.0, wt = s.wt, p15 = s.p15;
        api.stiff(&k, &damp, &wt, &p15);
        r.put("stiff.k", k);
        r.put("stiff.damp", damp);
    }
}

// ---- Gauss table, characteristic length, shape functions, integrand ----
inline void run_table(const Api& api, Rec& r)
{
    for (int i = 0; i < 16; ++i) api.gp[i] = 5.0, api.gw[i] = 5.0;
    api.gauss();
    r.put("gauss.p", api.gp, 16);
    r.put("gauss.w", api.gw, 16);

    double x[24] = {0.0, 0.0, 0.0, 1.2, 0.1, -0.05, 1.3, 0.9, 0.02, -0.1, 1.05, 0.0,
                    0.05, -0.02, 1.4, 1.1, 0.0, 1.35, 1.25, 1.0, 1.5, 0.0, 0.95, 1.45};
    i32 f[12] = {1, 4, 3, 2, 5, 6, 7, 8, 1, 2, 6, 5};
    struct Lc { i32 npf, ng; } lc[] = {{4, 1}, {4, 2}, {3, 3}, {4, 0}};
    for (auto& c : lc) {
        double len = -1.0;
        i32 npf = c.npf, ng = c.ng;
        api.charlen(x, f, &npf, &ng, &len);
        r.put("charlen", len);
    }

    double xi = 0.3, eta = -0.6, zeta = 0.85;
    double nn[8], dn[24];
    api.shape(&xi, &eta, &zeta, nn, dn);
    r.put("shape.n", nn, 8);
    r.put("shape.dn", dn, 24);

    clear_body(api);
    api.body[B_C + 0] = 0.25;
    api.body[B_C + 1] = -0.4;
    api.body[B_C + 2] = 1.1;
    double px = 1.7, py = -0.35, pz = 2.2;
    double nrm[3] = {0.48, -0.6, 0.64};
    for (i32 kase = 0; kase <= 11; ++kase) {
        double fv = 123.0;
        api.integrand(&px, &py, &pz, &fv, nrm, &kase);
        r.put("integrand", fv);
    }
}

// Test bodies: a perturbed octahedron (triangles, with a dummy face 1 and a degenerate face) and
// a perturbed hexahedron (quads).
struct TriBody {
    double x[18] = {1.3, 0.05, -0.1, -0.9, 0.1, 0.2, 0.15, 1.6, 0.05,
                    -0.05, -1.1, 0.1, 0.2, -0.1, 2.05, -0.1, 0.12, -0.8};
    // F(4,f), first face is a dummy (f_first = 2); last face degenerate (nodes 1, 1, 3)
    i32 f[40] = {6, 6, 6, 6, 1, 3, 5, 5, 3, 2, 5, 5, 2, 4, 5, 5, 4, 1, 5, 5,
                 3, 1, 6, 6, 2, 3, 6, 6, 4, 2, 6, 6, 1, 4, 6, 6, 1, 1, 3, 3};
    i32 first = 2, nf = 9, npf = 3;
};
struct QuadBody {
    double x[24] = {0.0, 0.0, 0.0, 2.1, 0.1, -0.05, 2.3, 1.4, 0.02, -0.1, 1.55, 0.0,
                    0.05, -0.02, 3.4, 2.0, 0.0, 3.35, 2.25, 1.5, 3.5, 0.0, 1.45, 3.45};
    i32 f[24] = {1, 4, 3, 2, 5, 6, 7, 8, 1, 2, 6, 5, 2, 3, 7, 6, 3, 4, 8, 7, 4, 1, 5, 8};
    i32 first = 1, nf = 6, npf = 4;
};

inline void mass_case(const Api& api, Rec& r, double* x, i32* f, i32 first, i32 nf, i32 npf, i32 nk,
                      double* k)
{
    clear_body(api);
    std::vector<double> fv(3 * static_cast<size_t>(nf), 9.0);
    api.mass(x, f, &first, &nf, &npf, fv.data(), &nk, k);
    r.put("mass.fv", fv.data(), 3 * nf);
    r.put("mass.body", api.body, B_N);
}

inline void run_mass(const Api& api, Rec& r)
{
    api.gauss();
    TriBody t;
    QuadBody q;
    double k0[18] = {};
    mass_case(api, r, t.x, t.f, t.first, t.nf, t.npf, 0, k0);
    mass_case(api, r, q.x, q.f, q.first, q.nf, q.npf, 0, k0);
    mass_case(api, r, q.x, q.f, q.first, q.nf, q.npf, 7, k0); // only the face vectors
}

// ---- rigid move ----
inline void run_move(const Api& api, Rec& r)
{
    clear_body(api);
    double x[12] = {1.0, 2.0, 3.0, -0.5, 0.25, 4.0, 10.0, -3.0, 0.5, 0.0, 0.0, 0.0};
    double from[3] = {0.5, 0.4, 1.3}, to[3] = {0.7, 0.1, 1.0};
    double ang[3] = {0.3, -0.2, 0.1};
    i32 n = 4, track = 0;
    api.move(x, from, to, ang, &n, &track);
    r.put("move.x", x, 12);
    r.put("move.ang", ang, 3);
    for (int i = 0; i < 3; ++i) api.body[B_CT + i] = 0.1 * (i + 1);
    for (int i = 0; i < 9; ++i) api.body[B_A + i] = (i % 4 == 0 ? 1.0 : 0.0) + 0.1 * (i + 1);
    double ang2[3] = {4.0, -7.5, 0.01};
    track = 1;
    n = 3;
    api.move(x, from, to, ang2, &n, &track);
    r.put("move2.x", x, 12);
    r.put("move2.ang", ang2, 3);
    r.put("move2.body", api.body, B_N);
    double ang3[3] = {1e17, -3.1415926535898, 3.1415926535897};
    double from3[3] = {0, 0, 0};
    n = 1;
    track = 0;
    api.move(x, from3, from3, ang3, &n, &track);
    r.put("move3.x", x, 3);
    r.put("move3.ang", ang3, 3);
}

// ---- needs the FTN_geoutl kernels: projection/mirror helpers, mass with nk = 1, 2, 3 ----
inline void run_geo(const Api& api, Rec& r)
{
    double c[3] = {1.3, -0.4, 2.2};
    double p[3] = {0.1, 0.2, 0.3};
    double nn[3] = {0.48, -0.6, 0.64};
    double v[3];
    std::memcpy(v, c, sizeof v);
    api.plane(v, p, nn);
    r.put("plane", v, 3);
    std::memcpy(v, c, sizeof v);
    api.axis(v, p, nn);
    r.put("axis", v, 3);
    double m6[6] = {1.3, -0.4, 2.2, 7.0, 8.0, 9.0};
    api.mirror_m(m6, p, nn);
    r.put("mirror_m", m6, 6);
    std::memcpy(v, c, sizeof v);
    api.mirror_x(v, p, nn);
    r.put("mirror_x", v, 3);
    double z[3] = {0.0, 0.0, 0.0}; // zero direction: the previous frame is reused
    std::memcpy(v, c, sizeof v);
    api.plane(v, p, z);
    r.put("plane.zero", v, 3);

    api.gauss();
    TriBody t;
    QuadBody q;
    // nk = 1: plane x = 0.2 (|n1| == 1 zeroes I12, I13)
    double k1[18] = {0.2, 0.0, 0.0, -1.0, 0.0, 0.0};
    mass_case(api, r, t.x, t.f, t.first, t.nf, t.npf, 1, k1);
    mass_case(api, r, q.x, q.f, q.first, q.nf, q.npf, 1, k1);
    // nk = 1: oblique plane (no product zeroed)
    double k1b[18] = {0.1, 0.2, 0.3, 0.48, -0.6, 0.64};
    mass_case(api, r, q.x, q.f, q.first, q.nf, q.npf, 1, k1b);
    // nk = 2: planes y = 0.1 and z = 1.0
    double k2[18] = {0.0, 0.1, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, -1.0};
    mass_case(api, r, t.x, t.f, t.first, t.nf, t.npf, 2, k2);
    mass_case(api, r, q.x, q.f, q.first, q.nf, q.npf, 2, k2);
    // nk = 3: axis through (0.5, 0.5, 0) along (0.6, 0, 0.8) (K(:,1..2) zero here)
    double k3[18] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.5, 0.5, 0.0, 0.6, 0.0, 0.8};
    mass_case(api, r, t.x, t.f, t.first, t.nf, t.npf, 3, k3);
    mass_case(api, r, q.x, q.f, q.first, q.nf, q.npf, 3, k3);
}

} // namespace dropcase

#ifndef DROP_GOLD_HARNESS

#include "../src/drop/kernels_mass_internal.hpp"

#include <defcmd/drop/globals_a.hpp>
#include <defcmd/drop/kernels.hpp>
#include <testkit/check.hpp>

#include <bit>
#include <cstdio>

namespace {

using namespace def::cmd::drop;

dropcase::Api port_api()
{
    dropcase::Api a{};
    a.dot = fun_140367f0b;
    a.cross = fun_140366881;
    a.matvec = fun_140367df1;
    a.matmul = fun_140367c86;
    a.copy = fun_140375025;
    a.zero = fun_1403735bd;
    a.gauss = fun_140365e0b;
    a.charlen = fun_1403750cf;
    a.mass = fun_140362db9;
    a.integrand = fun_1403653fe;
    a.shape = fun_1403660b9;
    a.plane = fun_140365a10;
    a.axis = fun_140365b02;
    a.mirror_m = fun_140365bfc;
    a.mirror_x = fun_140365d0c;
    a.vel = fun_1403668de;
    a.solve = fun_140366ac3;
    a.stiff = fun_140367fe2;
    a.move = fun_140367448;
    a.body = reinterpret_cast<double*>(&g_body);
    a.gp = g_gauss_p;
    a.gw = g_gauss_w;
    return a;
}

// ---- golden tables (generated by dropgold.cpp) ----
static const std::uint64_t kGoldVec[] = {
    0xc02eae147ae147aeull, // dot
    0xc02be147ae147ae1ull, // dot
    0x3fdcccccccccccccull, // dot
    0x0000000000000000ull, // dot
    0xc011d1eb851eb852ull, // cross
    0xc007666666666667ull, // cross
    0x3ff7d70a3d70a3d8ull, // cross
    0x3fe91eb851eb8520ull, // matvec3
    0xc00028f5c28f5c2aull, // matvec3
    0x3ffca3d70a3d70a4ull, // matvec3
    0x3fe147ae147ae149ull, // matvec2
    0xbfe199999999999aull, // matvec2
    0x3ffca3d70a3d70a4ull, // matvec2
    0x3fe45a1cac083126ull, // matmul
    0xbfdb53f7ced91686ull, // matmul
    0xbfec395810624dd3ull, // matmul
    0x3ffec083126e978eull, // matmul
    0x3fc49ba5e353f7ceull, // matmul
    0x3ff1b22d0e560418ull, // matmul
    0x3fc28f5c28f5c290ull, // matmul
    0x4001851eb851eb86ull, // matmul
    0xbfdd1eb851eb851eull, // matmul
    0x3ff8000000000000ull, // copy
    0xc002000000000000ull, // copy
    0x4008cccccccccccdull, // copy
    0x3fe6666666666666ull, // copy
    0x0000000000000000ull, // copy
    0x0000000000000000ull, // zero
    0x0000000000000000ull, // zero
    0x0000000000000000ull, // zero
    0x4020000000000000ull, // zero
    0xc00a51eb851eb852ull, // vel
    0xbfdffffffffffffcull, // vel
    0x3ff6b851eb851eb8ull, // vel
    0xc0023851eb851eb8ull, // vel
    0xc007000000000000ull, // vel
    0xbfd7851eb851eb84ull, // vel
    0x4022f566cf41f214ull, // vel
    0x4025426809d4951aull, // vel
    0x4014147ae147ae15ull, // vel
    0x0000000000000000ull, // solve.err
    0x0000000000000000ull, // solve.b
    0x0000000000000000ull, // solve.b
    0xc0c3240000000000ull, // solve.b
    0x4055e924924a4d50ull, // solve.b
    0xc011249249291b33ull, // solve.b
    0x4062bb6db6db6217ull, // solve.b
    0x4004000000000000ull, // solve.a
    0x0000000000000000ull, // solve.a
    0x0000000000000000ull, // solve.a
    0x3f847ae147ae147bull, // solve.a
    0x0000000000000000ull, // solve.a
    0x0000000000000000ull, // solve.a
    0x0000000000000000ull, // solve.a
    0x4004000000000000ull, // solve.a
    0x0000000000000000ull, // solve.a
    0x0000000000000000ull, // solve.a
    0x3f947ae147ae147bull, // solve.a
    0x0000000000000000ull, // solve.a
    0x0000000000000000ull, // solve.a
    0x0000000000000000ull, // solve.a
    0x4004000000000000ull, // solve.a
    0x0000000000000000ull, // solve.a
    0x0000000000000000ull, // solve.a
    0x3f9eb851eb851eb8ull, // solve.a
    0x0000000000000000ull, // solve.a
    0x0000000000000000ull, // solve.a
    0x0000000000000000ull, // solve.a
    0x3fd999999999999aull, // solve.a
    0x3d719799812dea11ull, // solve.a
    0x3fb999999999999aull, // solve.a
    0x0000000000000000ull, // solve.a
    0x0000000000000000ull, // solve.a
    0x0000000000000000ull, // solve.a
    0x3ffb333333333333ull, // solve.a
    0xbfe4000000000000ull, // solve.a
    0x3fd3333333320822ull, // solve.a
    0x0000000000000000ull, // solve.a
    0x0000000000000000ull, // solve.a
    0x0000000000000000ull, // solve.a
    0xbfc999999999999aull, // solve.a
    0x3fff333333333333ull, // solve.a
    0x3ff09374bc699e80ull, // solve.a
    0x3ff0000000000000ull, // solve3.err
    0x4000000000000000ull, // solve3.b
    0x0000000000000000ull, // solve3.b
    0x4004000000000000ull, // solve3.b
    0x4010000000000000ull, // solve3.a
    0x4000000000000000ull, // solve3.a
    0x3ff0000000000000ull, // solve3.a
    0x4000000000000000ull, // solve3.a
    0x0000000000000000ull, // solve3.a
    0x0000000000000000ull, // solve3.a
    0x3ff0000000000000ull, // solve3.a
    0x4004000000000000ull, // solve3.a
    0x401b000000000000ull, // solve3.a
    0x40fde84000000000ull, // stiff.k
    0x404bab808749b402ull, // stiff.damp
    0x4187c4e727fffffeull, // stiff.k
    0x409ba1dcf00d9174ull, // stiff.damp
    0x40b8242c28f5c28full, // stiff.k
    0x4028dc25e30123bdull, // stiff.damp
    0x7ff8000000000000ull, // stiff.k
    0xc000000000000000ull, // stiff.damp
};
static const std::uint64_t kGoldTable[] = {
    0x0000000000000000ull, // gauss.p
    0xbfe279a74590331eull, // gauss.p
    0xbfe8c97ef43f7244ull, // gauss.p
    0xbfeb8e6dbcf63989ull, // gauss.p
    0x0000000000000000ull, // gauss.p
    0x3fe279a74590331eull, // gauss.p
    0x0000000000000000ull, // gauss.p
    0xbfd5c23fd9dd3df7ull, // gauss.p
    0x0000000000000000ull, // gauss.p
    0x0000000000000000ull, // gauss.p
    0x3fe8c97ef43f7244ull, // gauss.p
    0x3fd5c23fd9dd3df7ull, // gauss.p
    0x0000000000000000ull, // gauss.p
    0x0000000000000000ull, // gauss.p
    0x0000000000000000ull, // gauss.p
    0x3feb8e6dbcf63989ull, // gauss.p
    0x4000000000000000ull, // gauss.w
    0x3ff0000000000000ull, // gauss.w
    0x3fe1c71c71c71c76ull, // gauss.w
    0x3fd64340f7e7b66eull, // gauss.w
    0x0000000000000000ull, // gauss.w
    0x3ff0000000000000ull, // gauss.w
    0x3fec71c71c71c71dull, // gauss.w
    0x3fe4de5f840c24c9ull, // gauss.w
    0x0000000000000000ull, // gauss.w
    0x0000000000000000ull, // gauss.w
    0x3fe1c71c71c71c76ull, // gauss.w
    0x3fe4de5f840c24c9ull, // gauss.w
    0x0000000000000000ull, // gauss.w
    0x0000000000000000ull, // gauss.w
    0x0000000000000000ull, // gauss.w
    0x3fd64340f7e7b66eull, // gauss.w
    0x3ffc05bb6b6f476dull, // charlen
    0x3ff9ecc8b9474fe8ull, // charlen
    0x3ff950198f105776ull, // charlen
    0x40f86a0000000000ull, // charlen
    0x3f95810624dd2f1bull, // shape.n
    0x3fa3f7ced916872cull, // shape.n
    0x3f83f7ced916872cull, // shape.n
    0x3f75810624dd2f1bull, // shape.n
    0x3fd09374bc6a7efaull, // shape.n
    0x3fdec8b439581063ull, // shape.n
    0x3fbec8b439581063ull, // shape.n
    0x3fb09374bc6a7efaull, // shape.n
    0xbf9eb851eb851ebaull, // shape.dn
    0xbf8ae147ae147ae2ull, // shape.dn
    0xbfc1eb851eb851ebull, // shape.dn
    0x3f9eb851eb851ebaull, // shape.dn
    0xbf98f5c28f5c28f7ull, // shape.dn
    0xbfd0a3d70a3d70a4ull, // shape.dn
    0x3f7eb851eb851ebaull, // shape.dn
    0x3f98f5c28f5c28f7ull, // shape.dn
    0xbfb0a3d70a3d70a4ull, // shape.dn
    0xbf7eb851eb851ebaull, // shape.dn
    0x3f8ae147ae147ae2ull, // shape.dn
    0xbfa1eb851eb851ebull, // shape.dn
    0xbfd7ae147ae147afull, // shape.dn
    0xbfc4b851eb851eb8ull, // shape.dn
    0x3fc1eb851eb851ebull, // shape.dn
    0x3fd7ae147ae147afull, // shape.dn
    0xbfd33d70a3d70a3eull, // shape.dn
    0x3fd0a3d70a3d70a4ull, // shape.dn
    0x3fb7ae147ae147afull, // shape.dn
    0x3fd33d70a3d70a3eull, // shape.dn
    0x3fb0a3d70a3d70a4ull, // shape.dn
    0xbfb7ae147ae147afull, // shape.dn
    0x3fc4b851eb851eb8ull, // shape.dn
    0x3fa1eb851eb851ebull, // shape.dn
    0x405ec00000000000ull, // integrand
    0x3fea1cac083126e9ull, // integrand
    0x3fe631f8a0902ddfull, // integrand
    0xbfa2d0e560418936ull, // integrand
    0x3ff8c7e28240b781ull, // integrand
    0x3fd22bc5c83d62abull, // integrand
    0x3fe8b1fc1f591a07ull, // integrand
    0x3fdf3760bf5d7881ull, // integrand
    0xbf99d5e4a383276dull, // integrand
    0xbfe1c30d306a2b17ull, // integrand
    0x3f4b089a02752554ull, // integrand
    0x405ec00000000000ull, // integrand
};
static const std::uint64_t kGoldMass[] = {
    0x3fe7f17c832e75baull, // mass.fv
    0x3fe077bc1342dcfdull, // mass.fv
    0x3fdacc464d628b66ull, // mass.fv
    0xbfe65b82432bda57ull, // mass.fv
    0x3fe12a4755252a6eull, // mass.fv
    0x3fde4c72fa815fdcull, // mass.fv
    0xbfe7ab6e4d69fbe2ull, // mass.fv
    0xbfe1c755fe259b31ull, // mass.fv
    0x3fd84dbcaf7cd2a3ull, // mass.fv
    0x3fe4acfbb5bdee7eull, // mass.fv
    0xbfe6b51258c33b1dull, // mass.fv
    0x3fd1fcf318beebcaull, // mass.fv
    0x3fdb4c70dc860fd4ull, // mass.fv
    0x3fd94ae9cb804cd4ull, // mass.fv
    0xbfea08b1f8bfa595ull, // mass.fv
    0xbfe697a3012789f0ull, // mass.fv
    0x3fdc1214eb5fcfcfull, // mass.fv
    0xbfe1cb0c6f35b684ull, // mass.fv
    0xbfe63c6ac0ad696cull, // mass.fv
    0xbfdc7d1d0114b281ull, // mass.fv
    0xbfe212dd5f45d100ull, // mass.fv
    0x3fd69187c526d5d7ull, // mass.fv
    0xbfe179c05a5c5f5aull, // mass.fv
    0xbfe850e7ce301293ull, // mass.fv
    0x0000000000000000ull, // mass.fv
    0x0000000000000000ull, // mass.fv
    0x0000000000000000ull, // mass.fv
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x3fdb3fc29f9be952ull, // mass.body
    0xbfa34618115b5176ull, // mass.body
    0xbfac13877073961aull, // mass.body
    0xbfa34618115b5176ull, // mass.body
    0x3fd7b1c49a3f4336ull, // mass.body
    0x3fa543ef045d107aull, // mass.body
    0xbfac13877073961aull, // mass.body
    0x3fa543ef045d107aull, // mass.body
    0x3fd5b2d54c78cd9dull, // mass.body
    0x4006eb9af72015d9ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x4006eb9af72015d9ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x4006eb9af72015d9ull, // mass.body
    0x4006eb9af72015d9ull, // mass.body
    0x3fbfce8928eef611ull, // mass.body
    0x3fb8b8aeddbc7b9cull, // mass.body
    0x3fcefb0be22cfed8ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0xbf7f08b9cab1a5b8ull, // mass.fv
    0x3f9bdb479ac815d3ull, // mass.fv
    0xbfeff84b808a9ef1ull, // mass.fv
    0x3f66bf1cb3005872ull, // mass.fv
    0xbfb1247bffd8a91eull, // mass.fv
    0x3fefe6c69111c4a0ull, // mass.fv
    0x3f9d28439d69e24aull, // mass.fv
    0xbfeff97ec3399722ull, // mass.fv
    0xbf91d88f898921d7ull, // mass.fv
    0x3fef949358a17c2aull, // mass.fv
    0xbfc474794cad938cull, // mass.fv
    0x3f960f34db923f51ull, // mass.fv
    0x3f948db4e5a6b5baull, // mass.fv
    0x3feff3b56480d145ull, // mass.fv
    0xbeea12d813478e00ull, // mass.fv
    0xbfeff30fed8ddda7ull, // mass.fv
    0xbfa95ce342df3dd0ull, // mass.fv
    0x3f9572b0c9127596ull, // mass.fv
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x40291f1757f8f106ull, // mass.body
    0xbf8e95679da182e0ull, // mass.body
    0x3f96a4f27f532b8eull, // mass.body
    0xbf8e95679da182e0ull, // mass.body
    0x402ddc7cf19c7ee5ull, // mass.body
    0xbfc0f084eba803f9ull, // mass.body
    0x3f96a4f27f532b8eull, // mass.body
    0xbfc0f084eba803f9ull, // mass.body
    0x4018bf69da9fd10full, // mass.body
    0x4025b97b4a2339c1ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x4025b97b4a2339c1ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x4025b97b4a2339c1ull, // mass.body
    0x4025b97b4a2339c1ull, // mass.body
    0x3ff0e3a3a5f061bcull, // mass.body
    0x3fe8da5185dc61bbull, // mass.body
    0x3ffb1ef7fbe9cd8eull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0xbf7f08b9cab1a5b8ull, // mass.fv
    0x3f9bdb479ac815d3ull, // mass.fv
    0xbfeff84b808a9ef1ull, // mass.fv
    0x3f66bf1cb3005872ull, // mass.fv
    0xbfb1247bffd8a91eull, // mass.fv
    0x3fefe6c69111c4a0ull, // mass.fv
    0x3f9d28439d69e24aull, // mass.fv
    0xbfeff97ec3399722ull, // mass.fv
    0xbf91d88f898921d7ull, // mass.fv
    0x3fef949358a17c2aull, // mass.fv
    0xbfc474794cad938cull, // mass.fv
    0x3f960f34db923f51ull, // mass.fv
    0x3f948db4e5a6b5baull, // mass.fv
    0x3feff3b56480d145ull, // mass.fv
    0xbeea12d813478e00ull, // mass.fv
    0xbfeff30fed8ddda7ull, // mass.fv
    0xbfa95ce342df3dd0ull, // mass.fv
    0x3f9572b0c9127596ull, // mass.fv
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
};
static const std::uint64_t kGoldMove[] = {
    0x3fe575cfd6d2ed34ull, // move.x
    0x3ff20dc2c40b0e44ull, // move.x
    0x40093c4b536c1722ull, // move.x
    0xbfe5d5ac85b9792aull, // move.x
    0xbfef820fd914bcb9ull, // move.x
    0x400a497a93d5b6ceull, // move.x
    0x40253a9e091535fcull, // move.x
    0xbffee9cf971f7950ull, // move.x
    0x3ff2751435fec2baull, // move.x
    0x3fdeca04c4fc9d6eull, // move.x
    0x3fb4808f8e66a8e6ull, // move.x
    0xbfdbabea1d76e838ull, // move.x
    0x3fd3333333333333ull, // move.ang
    0xbfc999999999999aull, // move.ang
    0x3fb999999999999aull, // move.ang
    0xbfec5655531a35f2ull, // move2.x
    0xbfeafbfc40cf87a8ull, // move2.x
    0x3ffc573c3883a134ull, // move2.x
    0x3fb461b1f733fa50ull, // move2.x
    0xc00283acda953aaeull, // move2.x
    0xbf96d8d837e9b680ull, // move2.x
    0x4017dc58ad1284e5ull, // move2.x
    0xbff42ce44c87a2dbull, // move2.x
    0x4023af52c78f1c2cull, // move2.x
    0x3fdeca04c4fc9d6eull, // move2.x
    0x3fb4808f8e66a8e6ull, // move2.x
    0xbfdbabea1d76e838ull, // move2.x
    0x3feb7812aeef4b60ull, // move2.ang
    0xbff37812aeef4b60ull, // move2.ang
    0x3f847ae147ae147bull, // move2.ang
    0x0000000000000000ull, // move2.body
    0x0000000000000000ull, // move2.body
    0x0000000000000000ull, // move2.body
    0x3ff4f65e068eb382ull, // move2.body
    0x3fe76e3ce02ba882ull, // move2.body
    0x3fd620e2993f3ef4ull, // move2.body
    0x3ffa821d53be9598ull, // move2.body
    0x3fe78aa23958b20full, // move2.body
    0x3ff48a44e9ce26c0ull, // move2.body
    0x3fd3469069f479d6ull, // move2.body
    0x3ff5847afdedf7e8ull, // move2.body
    0x3ff094217cab782bull, // move2.body
    0x3fbe94e59c0dc0d0ull, // move2.body
    0xbfb933510c5eb5f2ull, // move2.body
    0x3ff6db0d2d4740f6ull, // move2.body
    0x0000000000000000ull, // move2.body
    0x0000000000000000ull, // move2.body
    0x0000000000000000ull, // move2.body
    0x0000000000000000ull, // move2.body
    0x0000000000000000ull, // move2.body
    0x0000000000000000ull, // move2.body
    0x0000000000000000ull, // move2.body
    0x0000000000000000ull, // move2.body
    0x0000000000000000ull, // move2.body
    0x0000000000000000ull, // move2.body
    0x0000000000000000ull, // move2.body
    0x0000000000000000ull, // move2.body
    0x0000000000000000ull, // move2.body
    0x0000000000000000ull, // move2.body
    0x0000000000000000ull, // move2.body
    0x0000000000000000ull, // move2.body
    0x0000000000000000ull, // move2.body
    0x0000000000000000ull, // move2.body
    0x0000000000000000ull, // move2.body
    0x0000000000000000ull, // move2.body
    0x0000000000000000ull, // move2.body
    0x0000000000000000ull, // move2.body
    0x0000000000000000ull, // move2.body
    0x0000000000000000ull, // move2.body
    0x0000000000000000ull, // move2.body
    0x0000000000000000ull, // move2.body
    0x0000000000000000ull, // move2.body
    0x0000000000000000ull, // move2.body
    0x0000000000000000ull, // move2.body
    0x0000000000000000ull, // move2.body
    0x0000000000000000ull, // move2.body
    0x0000000000000000ull, // move2.body
    0x0000000000000000ull, // move2.body
    0x0000000000000000ull, // move2.body
    0x0000000000000000ull, // move2.body
    0x0000000000000000ull, // move2.body
    0x0000000000000000ull, // move2.body
    0x0000000000000000ull, // move2.body
    0x0000000000000000ull, // move2.body
    0x0000000000000000ull, // move2.body
    0x0000000000000000ull, // move2.body
    0x0000000000000000ull, // move2.body
    0x0000000000000000ull, // move2.body
    0x0000000000000000ull, // move2.body
    0x0000000000000000ull, // move2.body
    0x3fec5655531a38b5ull, // move3.x
    0x3feafbfc40cf84bdull, // move3.x
    0x3ffc573c3883a132ull, // move3.x
    0x0000000000000000ull, // move3.ang
    0x0000000000000000ull, // move3.ang
    0x400921fb54442c46ull, // move3.ang
};
[[maybe_unused]] static const std::uint64_t kGoldGeo[] = {
    0x3fd1172ef0ae5360ull, // plane
    0x3fec84b5dcc63f14ull, // plane
    0x3fea53b8e4b87bdeull, // plane
    0x3ff2209aaa3ad18eull, // axis
    0xbff1758e219652beull, // axis
    0x3ffad6238da3c212ull, // axis
    0x3feee8d10f51ac9eull, // mirror_m
    0xbffc84b5dcc63f15ull, // mirror_m
    0x3ff27913e81450f0ull, // mirror_m
    0x401c000000000000ull, // mirror_m
    0x4020000000000000ull, // mirror_m
    0x4022000000000000ull, // mirror_m
    0xbfe8826aa8eb4638ull, // mirror_x
    0x4001758e219652beull, // mirror_x
    0xbfe1bef49cf56eacull, // mirror_x
    0x3fd1172ef0ae5360ull, // plane.zero
    0x3fec84b5dcc63f14ull, // plane.zero
    0x3fea53b8e4b87bdeull, // plane.zero
    0x3fe7f17c832e75baull, // mass.fv
    0x3fe077bc1342dcfdull, // mass.fv
    0x3fdacc464d628b66ull, // mass.fv
    0xbfe65b82432bda57ull, // mass.fv
    0x3fe12a4755252a6eull, // mass.fv
    0x3fde4c72fa815fdcull, // mass.fv
    0xbfe7ab6e4d69fbe2ull, // mass.fv
    0xbfe1c755fe259b31ull, // mass.fv
    0x3fd84dbcaf7cd2a3ull, // mass.fv
    0x3fe4acfbb5bdee7eull, // mass.fv
    0xbfe6b51258c33b1dull, // mass.fv
    0x3fd1fcf318beebcaull, // mass.fv
    0x3fdb4c70dc860fd4ull, // mass.fv
    0x3fd94ae9cb804cd4ull, // mass.fv
    0xbfea08b1f8bfa595ull, // mass.fv
    0xbfe697a3012789f0ull, // mass.fv
    0x3fdc1214eb5fcfcfull, // mass.fv
    0xbfe1cb0c6f35b684ull, // mass.fv
    0xbfe63c6ac0ad696cull, // mass.fv
    0xbfdc7d1d0114b281ull, // mass.fv
    0xbfe212dd5f45d100ull, // mass.fv
    0x3fd69187c526d5d7ull, // mass.fv
    0xbfe179c05a5c5f5aull, // mass.fv
    0xbfe850e7ce301293ull, // mass.fv
    0x0000000000000000ull, // mass.fv
    0x0000000000000000ull, // mass.fv
    0x0000000000000000ull, // mass.fv
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x3fdb3fc29f9be952ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x3fd8bf26bec9b3ccull, // mass.body
    0x3fa543ef045d107aull, // mass.body
    0x0000000000000000ull, // mass.body
    0x3fa543ef045d107aull, // mass.body
    0x3fd6c03771033e33ull, // mass.body
    0x4006eb9af72015d9ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x4006eb9af72015d9ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x4006eb9af72015d9ull, // mass.body
    0x4006eb9af72015d9ull, // mass.body
    0x3fc999999999999aull, // mass.body
    0x3fb8b8aeddbc7b9cull, // mass.body
    0x3fcefb0be22cfed8ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0xbf7f08b9cab1a5b8ull, // mass.fv
    0x3f9bdb479ac815d3ull, // mass.fv
    0xbfeff84b808a9ef1ull, // mass.fv
    0x3f66bf1cb3005872ull, // mass.fv
    0xbfb1247bffd8a91eull, // mass.fv
    0x3fefe6c69111c4a0ull, // mass.fv
    0x3f9d28439d69e24aull, // mass.fv
    0xbfeff97ec3399722ull, // mass.fv
    0xbf91d88f898921d7ull, // mass.fv
    0x3fef949358a17c2aull, // mass.fv
    0xbfc474794cad938cull, // mass.fv
    0x3f960f34db923f51ull, // mass.fv
    0x3f948db4e5a6b5baull, // mass.fv
    0x3feff3b56480d145ull, // mass.fv
    0xbeea12d813478e00ull, // mass.fv
    0xbfeff30fed8ddda7ull, // mass.fv
    0xbfa95ce342df3dd0ull, // mass.fv
    0x3f9572b0c9127596ull, // mass.fv
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x40291f1757f8f106ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x4036e1c65d667b11ull, // mass.body
    0xbfc0f084eba803f9ull, // mass.body
    0x0000000000000000ull, // mass.body
    0xbfc0f084eba803f9ull, // mass.body
    0x402c46c4b6805fc2ull, // mass.body
    0x4025b97b4a2339c1ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x4025b97b4a2339c1ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x4025b97b4a2339c1ull, // mass.body
    0x4025b97b4a2339c1ull, // mass.body
    0x3fc999999999999aull, // mass.body
    0x3fe8da5185dc61bbull, // mass.body
    0x3ffb1ef7fbe9cd8eull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0xbf7f08b9cab1a5b8ull, // mass.fv
    0x3f9bdb479ac815d3ull, // mass.fv
    0xbfeff84b808a9ef1ull, // mass.fv
    0x3f66bf1cb3005872ull, // mass.fv
    0xbfb1247bffd8a91eull, // mass.fv
    0x3fefe6c69111c4a0ull, // mass.fv
    0x3f9d28439d69e24aull, // mass.fv
    0xbfeff97ec3399722ull, // mass.fv
    0xbf91d88f898921d7ull, // mass.fv
    0x3fef949358a17c2aull, // mass.fv
    0xbfc474794cad938cull, // mass.fv
    0x3f960f34db923f51ull, // mass.fv
    0x3f948db4e5a6b5baull, // mass.fv
    0x3feff3b56480d145ull, // mass.fv
    0xbeea12d813478e00ull, // mass.fv
    0xbfeff30fed8ddda7ull, // mass.fv
    0xbfa95ce342df3dd0ull, // mass.fv
    0x3f9572b0c9127596ull, // mass.fv
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x4035098fae62f558ull, // mass.body
    0x400a5df37e746a68ull, // mass.body
    0xc00b65ff56df59fdull, // mass.body
    0x400a5df37e746a68ull, // mass.body
    0x4035f8ec07020c81ull, // mass.body
    0x4010cd00ced53fa3ull, // mass.body
    0xc00b65ff56df59fdull, // mass.body
    0x4010cd00ced53fa3ull, // mass.body
    0x40295d82a8595ed3ull, // mass.body
    0x4025b97b4a2339c1ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x4025b97b4a2339c1ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x4025b97b4a2339c1ull, // mass.body
    0x4025b97b4a2339c1ull, // mass.body
    0x3fe2556479fc8b1full, // mass.body
    0x3ff61456861cd415ull, // mass.body
    0x3ff0d30b6ffc52a7ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x3fe7f17c832e75baull, // mass.fv
    0x3fe077bc1342dcfdull, // mass.fv
    0x3fdacc464d628b66ull, // mass.fv
    0xbfe65b82432bda57ull, // mass.fv
    0x3fe12a4755252a6eull, // mass.fv
    0x3fde4c72fa815fdcull, // mass.fv
    0xbfe7ab6e4d69fbe2ull, // mass.fv
    0xbfe1c755fe259b31ull, // mass.fv
    0x3fd84dbcaf7cd2a3ull, // mass.fv
    0x3fe4acfbb5bdee7eull, // mass.fv
    0xbfe6b51258c33b1dull, // mass.fv
    0x3fd1fcf318beebcaull, // mass.fv
    0x3fdb4c70dc860fd4ull, // mass.fv
    0x3fd94ae9cb804cd4ull, // mass.fv
    0xbfea08b1f8bfa595ull, // mass.fv
    0xbfe697a3012789f0ull, // mass.fv
    0x3fdc1214eb5fcfcfull, // mass.fv
    0xbfe1cb0c6f35b684ull, // mass.fv
    0xbfe63c6ac0ad696cull, // mass.fv
    0xbfdc7d1d0114b281ull, // mass.fv
    0xbfe212dd5f45d100ull, // mass.fv
    0x3fd69187c526d5d7ull, // mass.fv
    0xbfe179c05a5c5f5aull, // mass.fv
    0xbfe850e7ce301293ull, // mass.fv
    0x0000000000000000ull, // mass.fv
    0x0000000000000000ull, // mass.fv
    0x0000000000000000ull, // mass.fv
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x40009307e190545aull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x400021366fa78570ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x3fd5b362d6629edbull, // mass.body
    0x4006eb9af72015d9ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x4006eb9af72015d9ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x4006eb9af72015d9ull, // mass.body
    0x4006eb9af72015d9ull, // mass.body
    0x3fbfce8928eef611ull, // mass.body
    0x3fb999999999999aull, // mass.body
    0x3ff0000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0xbf7f08b9cab1a5b8ull, // mass.fv
    0x3f9bdb479ac815d3ull, // mass.fv
    0xbfeff84b808a9ef1ull, // mass.fv
    0x3f66bf1cb3005872ull, // mass.fv
    0xbfb1247bffd8a91eull, // mass.fv
    0x3fefe6c69111c4a0ull, // mass.fv
    0x3f9d28439d69e24aull, // mass.fv
    0xbfeff97ec3399722ull, // mass.fv
    0xbf91d88f898921d7ull, // mass.fv
    0x3fef949358a17c2aull, // mass.fv
    0xbfc474794cad938cull, // mass.fv
    0x3f960f34db923f51ull, // mass.fv
    0x3f948db4e5a6b5baull, // mass.fv
    0x3feff3b56480d145ull, // mass.fv
    0xbeea12d813478e00ull, // mass.fv
    0xbfeff30fed8ddda7ull, // mass.fv
    0xbfa95ce342df3dd0ull, // mass.fv
    0x3f9572b0c9127596ull, // mass.fv
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x4036ca41520357f9ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x40343120318cf4c7ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x40264f5cc7e03cc8ull, // mass.body
    0x4025b97b4a2339c1ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x4025b97b4a2339c1ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x4025b97b4a2339c1ull, // mass.body
    0x4025b97b4a2339c1ull, // mass.body
    0x3ff0e3a3a5f061bcull, // mass.body
    0x3fb999999999999aull, // mass.body
    0x3ff0000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x3fe7f17c832e75baull, // mass.fv
    0x3fe077bc1342dcfdull, // mass.fv
    0x3fdacc464d628b66ull, // mass.fv
    0xbfe65b82432bda57ull, // mass.fv
    0x3fe12a4755252a6eull, // mass.fv
    0x3fde4c72fa815fdcull, // mass.fv
    0xbfe7ab6e4d69fbe2ull, // mass.fv
    0xbfe1c755fe259b31ull, // mass.fv
    0x3fd84dbcaf7cd2a3ull, // mass.fv
    0x3fe4acfbb5bdee7eull, // mass.fv
    0xbfe6b51258c33b1dull, // mass.fv
    0x3fd1fcf318beebcaull, // mass.fv
    0x3fdb4c70dc860fd4ull, // mass.fv
    0x3fd94ae9cb804cd4ull, // mass.fv
    0xbfea08b1f8bfa595ull, // mass.fv
    0xbfe697a3012789f0ull, // mass.fv
    0x3fdc1214eb5fcfcfull, // mass.fv
    0xbfe1cb0c6f35b684ull, // mass.fv
    0xbfe63c6ac0ad696cull, // mass.fv
    0xbfdc7d1d0114b281ull, // mass.fv
    0xbfe212dd5f45d100ull, // mass.fv
    0x3fd69187c526d5d7ull, // mass.fv
    0xbfe179c05a5c5f5aull, // mass.fv
    0xbfe850e7ce301293ull, // mass.fv
    0x0000000000000000ull, // mass.fv
    0x0000000000000000ull, // mass.fv
    0x0000000000000000ull, // mass.fv
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x3ff18da16070b9d0ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x3fee11e044249a46ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x3ff2b77f5c519ad2ull, // mass.body
    0x4006eb9af72015d9ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x4006eb9af72015d9ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x4006eb9af72015d9ull, // mass.body
    0x4006eb9af72015d9ull, // mass.body
    0x3fdec728cbe2dd8cull, // mass.body
    0x3fe0000000000000ull, // mass.body
    0xbf9a11ef026d89a0ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0xbf7f08b9cab1a5b8ull, // mass.fv
    0x3f9bdb479ac815d3ull, // mass.fv
    0xbfeff84b808a9ef1ull, // mass.fv
    0x3f66bf1cb3005872ull, // mass.fv
    0xbfb1247bffd8a91eull, // mass.fv
    0x3fefe6c69111c4a0ull, // mass.fv
    0x3f9d28439d69e24aull, // mass.fv
    0xbfeff97ec3399722ull, // mass.fv
    0xbf91d88f898921d7ull, // mass.fv
    0x3fef949358a17c2aull, // mass.fv
    0xbfc474794cad938cull, // mass.fv
    0x3f960f34db923f51ull, // mass.fv
    0x3f948db4e5a6b5baull, // mass.fv
    0x3feff3b56480d145ull, // mass.fv
    0xbeea12d813478e00ull, // mass.fv
    0xbfeff30fed8ddda7ull, // mass.fv
    0xbfa95ce342df3dd0ull, // mass.fv
    0x3f9572b0c9127596ull, // mass.fv
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x402d5b685209783bull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x40327fd593957da5ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x402296a8079e82eaull, // mass.body
    0x4025b97b4a2339c1ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x4025b97b4a2339c1ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x4025b97b4a2339c1ull, // mass.body
    0x4025b97b4a2339c1ull, // mass.body
    0x3ff837dae7db3e2cull, // mass.body
    0x3fe0000000000000ull, // mass.body
    0x3ff59fce8a79a83bull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
    0x0000000000000000ull, // mass.body
};
// ---- end golden tables ----

template <std::size_t N>
void compare(void (*run)(const dropcase::Api&, dropcase::Rec&), const std::uint64_t (&gold)[N])
{
    reset_state();
    reset_globals_a();
    dropcase::Rec r;
    run(port_api(), r);
    CHECK_EQ(r.v.size(), N);
    for (std::size_t i = 0; i < r.v.size() && i < N; ++i) {
        const double g = std::bit_cast<double>(gold[i]);
        if (!testkit::same_bits(r.v[i], g))
            std::printf("  mismatch #%zu %s: port %.17g, binary %.17g\n", i, r.lab[i].c_str(), r.v[i], g);
        CHECK_BITS(r.v[i], g);
    }
}

} // namespace

TEST_CASE(drop_vec_vel_solve_stiff) { compare(dropcase::run_vec, kGoldVec); }
TEST_CASE(drop_gauss_charlen_shape_integrand) { compare(dropcase::run_table, kGoldTable); }
TEST_CASE(drop_mass_nk0) { compare(dropcase::run_mass, kGoldMass); }
TEST_CASE(drop_move) { compare(dropcase::run_move, kGoldMove); }
#if defined(DEFCMD_DROP_GEOUTL_PORTED) && DEFCMD_DROP_GEOUTL_PORTED
TEST_CASE(drop_projection_and_mass_nk123) { compare(dropcase::run_geo, kGoldGeo); }
#endif

int main() { return testkit::run_all(); }

#endif // DROP_GOLD_HARNESS
