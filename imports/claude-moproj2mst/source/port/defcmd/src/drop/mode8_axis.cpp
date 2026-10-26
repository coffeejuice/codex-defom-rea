// Mode-8 axis point of the OBJPOS drop (packet p67-3): 1401fe7d0 (C++ wrapper) and 1403e8510
// (Fortran driver of the surface mass properties). Spec: docs/re/commands/OBJPOS_drop_mode8.md.
// Floating-point order from the listing (deform-rea-work/out/p673/c/L_m8.txt).
#include <defcmd/drop/globals_c.hpp>
#include <defcmd/drop/mode8.hpp>
#include <defcmd/objpos_centre.hpp>
#include <defcmd/objpos_mirror.hpp>

#include <cmath>
#include <cstdint>
#include <cstring>
#include <vector>

#include <defmodel/geo_kw/geo_kw.hpp>
#include <defmodel/geo_kw/surface_model.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/layout/obj_geo3d.hpp>
#include <defrt/port.hpp>

namespace def::cmd::drop {

namespace gk = def::model::geo_kw;

namespace {

// FUN_140484ab8 / the inline abs of 1403e8510: two's complement negation, abs(INT_MIN) = INT_MIN
inline std::int32_t iabs(std::int32_t v)
{
    return v < 0 ? static_cast<std::int32_t>(0u - static_cast<std::uint32_t>(v)) : v;
}

} // namespace

// @src 1401fe7d0  surface of obj: a mesh object (+0x87c != 0) without a surface (+0x3990) gets a
// temporary one (1401fc730 with compact = 1, removed again by 1401ffed0); neither -> false.
// Surface 0 (140198620) missing -> false (a temporary surface then stays). Otherwise the face
// array is copied with every node number made positive (operator new 1404797d8, abs 140484ab8,
// free), 1403e8510(&nP, &nF, X, F', c', a', &err, &0), c = c', a = a'; returns err == 0.
bool axis_point(model::LLOBJ_DATA* obj, double* c, double* a)
{
    bool made = false;     // local_84
    std::int32_t err = 0;  // local_90
    std::int32_t dbg = 0;  // local_88
    const std::int32_t mesh = obj->u87c[0];
    if (mesh == 0 || gk::has_surface_flag(*obj)) {
        if (mesh == 0 && !gk::has_surface_flag(*obj)) return false;
    } else {
        char compact = 1; // local_28
        objpos::fun_1401fc730(obj, &compact);
        made = true;
    }
    model::OBJ_GEO3D* s = gk::surface(*obj, 0); // local_24 = 0
    if (s == nullptr) return false;
    std::int32_t np = s->node_count();  // 1400034d0
    std::int32_t nf = s->elem_count();  // 140003500
    double* x = gk::geo3d_coords(s);    // 140006410
    std::int32_t* f = gk::geo3d_elems(s); // 140005510
    std::vector<std::int32_t> fa(static_cast<std::size_t>(nf > 0 ? nf : 0) * 4 + 1);
    for (std::int32_t i = 0; i < nf * 4; ++i) fa[static_cast<std::size_t>(i)] = iabs(f[i]);
    double cl[3], al[3]; // local_68, local_40
    mass_properties(&np, &nf, x, fa.data(), cl, al, &err, &dbg);
    std::memcpy(c, cl, 0x18);
    std::memcpy(a, al, 0x18);
    if (made) objpos::fun_1401ffed0(obj);
    return err == 0;
}

// @src 1403e8510  type = 3 unless some face has F(1,f) != |F(4,f)| (then 4); *err = 1;
// work arrays normals(3,nF), areas(nF), Y(3,nP); Gauss tables; first pass 1403ea82d (mode 0) on X;
// c = centroid; principal frame: Jacobi on the inertia tensor, vectors V1..V3 (columns of the
// eigenvector matrix), V1 negated when (V1 x V2).V3 < 0; Y(j,i) = sum_k (X(k,i) - C(k))*V(k,j)
// from 0.0; second pass on Y; then with d12 = |I11-I22|, d31 = |I33-I11|, d23 = |I22-I33| (of the
// second pass) a = V3 if d12 is strictly the smallest, V1 if d23 is, V2 if d31 is (*err = 0);
// else a = 0.
void mass_properties(std::int32_t* np, std::int32_t* nf, double* x, std::int32_t* f, double* c,
                     double* a, std::int32_t* err, std::int32_t* dbg)
{
    double* const b = g_massprop;
    std::int32_t nfc = *nf;  // 0x140708188
    std::int32_t first = 1;  // 0x140708184
    std::int32_t type = 3;   // 0x14070818c
    std::int32_t mode = 0;   // 0x140708180
    *err = 1;
    for (std::int32_t i = 1; i <= *nf; ++i) {
        const std::int32_t* fc = f + (static_cast<std::int64_t>(i) - 1) * 4;
        if (fc[0] != iabs(fc[3])) type = 4;
    }
    if (0 < *dbg) DEF_NOT_PORTED(0x1403e8510); // NUMSN / RZ / NUMSE / NBDRY WRITEs
    const std::int64_t nfa = *nf > 0 ? *nf : 0;
    const std::int64_t npa = *np > 0 ? *np : 0;
    std::vector<double> nrm(static_cast<std::size_t>(nfa * 3 + 1));  // 0x140708120 (3, nF)
    std::vector<double> y(static_cast<std::size_t>(npa * 3 + 1));    // 0x140708070 (3, nP)
    std::vector<double> area(static_cast<std::size_t>(nfa + 1));     // 0x1407080d0 (nF)
    double gp[16], gw[16]; // 0x1407081a0, 0x140708220
    gauss_tables(gp, gw);
    double p9[10] = {};    // 0x1407082a0 (unused in mode 0)
    surface_integrals(x, f, &first, &nfc, &type, nrm.data(), area.data(), &mode, p9, dbg, gp, gw);
    c[0] = b[19];
    c[1] = b[20];
    c[2] = b[21];

    double m[9]; // 0x1407082f0, rows of the symmetric tensor
    m[0] = b[0];
    m[1] = b[3];
    m[2] = b[6];
    m[3] = b[3];
    m[4] = b[4];
    m[5] = b[7];
    m[6] = b[6];
    m[7] = b[7];
    m[8] = b[8];
    std::int32_t n3 = 3, np3 = 3; // 0x140708340, 0x140708344
    double d[3], v[9];            // 0x140708350, 0x140708380
    std::int32_t nrot = 0;        // 0x1407083d0
    jacobi_eigen(m, &n3, &np3, d, v, &nrot);
    if (0 < *dbg) DEF_NOT_PORTED(0x1403e8510); // "Principal", "vec n" WRITEs

    double vv[9]; // 0x1407083e0: V1 (0..2), V2 (3..5), V3 (6..8)
    for (int i = 0; i < 9; ++i) vv[i] = v[i];
    double w[3];  // 0x140708430
    cross3(vv, vv + 3, w);
    std::int32_t three = 3; // local_b8
    double s = 0.0;         // 0x140708450
    dot_n(vv + 6, w, &s, &three);
    if (0.0 > s) {
        for (int i = 0; i < 3; ++i) {
            std::uint64_t u;
            std::memcpy(&u, &vv[i], 8);
            u ^= 0x8000000000000000ull; // XORPD DAT_140bb1000
            std::memcpy(&vv[i], &u, 8);
        }
    }
    if (0 < *dbg) DEF_NOT_PORTED(0x1403e8510); // "vec n" WRITEs

    for (std::int32_t i = 1; i <= *np; ++i) {
        for (int j = 0; j < 3; ++j) {
            double acc = 0.0;
            for (int k = 0; k < 3; ++k)
                acc = acc + (x[(static_cast<std::int64_t>(i) - 1) * 3 + k] - b[19 + k]) * vv[j * 3 + k];
            y[(static_cast<std::size_t>(i) - 1) * 3 + j] = acc;
        }
    }
    surface_integrals(y.data(), f, &first, &nfc, &type, nrm.data(), area.data(), &mode, p9, dbg, gp,
                      gw);
    // 1404b6d74 x3: DEALLOCATE normals, Y, areas

    a[0] = a[1] = a[2] = 0.0;
    const double d12 = std::fabs(b[0] - b[4]); // ANDPD DAT_140bb1048
    const double d31 = std::fabs(b[8] - b[0]);
    const double d23 = std::fabs(b[4] - b[8]);
    if (d23 > d12 && d31 > d12) { // 1403ea3c0: JP/JBE skip
        a[0] = vv[6];
        a[1] = vv[7];
        a[2] = vv[8];
        *err = 0;
    }
    if (d12 > d23 && d31 > d23) {
        a[0] = vv[0];
        a[1] = vv[1];
        a[2] = vv[2];
        *err = 0;
    }
    if (d12 > d31 && d23 > d31) {
        a[0] = vv[3];
        a[1] = vv[4];
        a[2] = vv[5];
        *err = 0;
    }
    if (0 < *dbg) DEF_NOT_PORTED(0x1403e8510); // "CENTER", "AXIS" WRITEs
}

} // namespace def::cmd::drop
