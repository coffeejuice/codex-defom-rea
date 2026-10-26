// Polygon normals and areas (ER1, ER2), validity flags (EI2, NI3): GENCTC_3d_search.md sec. 5.4,
// 5.5. Group "surface", packet p67-7. SAVE locals live in the static image at their addresses.
#include <defcmd/ctc3d/cube.hpp>
#include <defcmd/ctc3d/search.hpp>
#include <defcmd/ctc3d/surface.hpp>

#include <defrt/ftn/geoutl.hpp>
#include <defrt/ftn/sse_ops.hpp>

#include <cstring>

#include "surf_util.hpp"

namespace def::cmd::ctc3d {

using namespace surf;
namespace sse = def::rt::sse;

// @src 140029d47  NI3 on-surface flags and EI2 polygon status
void fun_140029d47(i32* obj, i32* inst, const i32* npts, const i32* npoly)
{
    i32& one = I(0x14064c194);   // count 1
    i32& val = I(0x14064c198);   // value written
    i32& i = I(0x14064c19c);     // loop index; conn(k) = I(0x14064c19c + 4k)
    i32* conn = IP(0x14064c1a0); // 4..12 node numbers
    i32& cnt = I(0x14064c1d0);
    double& area = D(0x14064c1d8);
    i32& acnt = I(0x14064c1e0);
    i32& k = I(0x14064c1e4);
    i32& node = I(0x14064c1e8);

    one = 1;
    val = 0;
    i = 1;
    if (*npts > 0) {
        do {
            i32 fld = 3;
            i32 ent = i;
            fun_140228690(obj, inst, &ent, &fld, &val, &one); // NI3 = 0
            i = i + 1;
        } while (i <= *npts);
    }
    i = 1;
    if (*npoly > 0) {
        do {
            i32 ent = i;
            fun_140227e00(obj, inst, &ent, conn, &cnt);
            // 140029fde: sign bit of conn(cnt), read at 14064c19c + 4*cnt (cnt = 0 reads i)
            val = (I(0x14064c19c + 4ull * static_cast<std::uint32_t>(cnt)) < 0) ? 0 : 1;
            i32 fld = 2;
            ent = i;
            fun_140228540(obj, inst, &ent, &fld, &area, &acnt); // ER2
            // 14002a06b: COMISD 1.000000013351432e-10 (140bad130), ER2; NaN keeps the flag
            if (val > 0 && 1.000000013351432e-10 >= area) {
                val = -1000;
            }
            fld = 2;
            ent = i;
            fun_1402280d0(obj, inst, &ent, &fld, &val, &one); // EI2
            const i32 n = cnt;
            k = 1;
            if (n > 0) {
                do {
                    val = 1;
                    i32 f3 = 3;
                    node = iabs(I(0x14064c19c + 4ull * static_cast<std::uint32_t>(k)));
                    fun_140228690(obj, inst, &node, &f3, &val, &one); // NI3 = 1
                    k = k + 1;
                } while (k <= n);
            }
            i = i + 1;
        } while (i <= *npoly);
    }
}

// @src 14003c506  NI3 = 0 at nodes whose polygons are all disabled (EI2 0) or interfering (-2000)
void fun_14003c506(i32* obj, i32* inst, const i32* npts)
{
    i32& i = I(0x14065711c);
    i32* list = IP(0x140657120); // NI1, list(k) = I(0x14065711c + 4k)
    i32& n = I(0x14066a9a0);
    i32& bad = I(0x14066a9a4);
    i32& k = I(0x14066a9a8);
    i32& poly = I(0x14066a9ac);
    i32& ei2 = I(0x14066a9b0);
    i32& ecnt = I(0x14066a9b4);

    i = 1;
    if (*npts > 0) {
        do {
            i32 fld = 1;
            i32 ent = i;
            fun_140228830(obj, inst, &ent, &fld, list, &n);
            bad = 0;
            k = 1;
            bool clear = false;
            if (n < 1) {
                clear = (n == 0);
            } else {
                do {
                    poly = I(0x14065711c + 4ull * static_cast<std::uint32_t>(k));
                    i32 f2 = 2;
                    fun_140228260(obj, inst, &poly, &f2, &ei2, &ecnt);
                    if (ei2 == 0 || ei2 == -2000) {
                        bad = bad + 1;
                    }
                    k = k + 1;
                } while (k <= n);
                clear = (bad == n);
            }
            if (clear) {
                ei2 = 0;
                i32 f3 = 3;
                i32 e = i;
                i32 one = 1;
                fun_140228690(obj, inst, &e, &f3, &ei2, &one); // NI3 = 0
            }
            i = i + 1;
        } while (i <= *npts);
    }
}

// @src 140075036  triangle area from the angle at vertex 2: |((sqrt(1 - c*c) * l1) * l2) * 0.5|
void fun_140075036(const double* x, double* area, const i32* conn, const i32* mode)
{
    i32& i1 = I(0x1406ae4c8);
    i32& i2 = I(0x1406ae4cc);
    i32& i3 = I(0x1406ae4d0);
    double* u = DP(0x1406ae4e0);
    double& l1 = D(0x1406ae500);
    double* w = DP(0x1406ae510);
    double& l2 = D(0x1406ae530);
    double& c = D(0x1406ae538);

    if (*mode == 1) {
        i1 = iabs(conn[0]);
        i2 = iabs(conn[1]);
        i3 = iabs(conn[2]);
    } else {
        i1 = 1;
        i2 = 2;
        i3 = 3;
    }
    const double* x2 = x + 3 * (static_cast<std::ptrdiff_t>(i2) - 1);
    def::rt::geoutl::fun_1404d68dd(x + 3 * (static_cast<std::ptrdiff_t>(i1) - 1), x2, u, &l1);
    def::rt::geoutl::fun_1404d68dd(x + 3 * (static_cast<std::ptrdiff_t>(i3) - 1), x2, w, &l2);
    c = sse::add(sse::mul(u[2], w[2]), sse::add(sse::mul(u[1], w[1]), sse::mul(u[0], w[0])));
    if (-1.0 > c) { // 140bad880
        c = -1.0;
    }
    if (c > 1.0) { // 140bad138
        c = 1.0;
    }
    double t = sse::sqrt(sse::sub(1.0, sse::mul(c, c)));
    t = sse::mul(sse::mul(sse::mul(t, l1), l2), 0.5); // 140bad260
    std::uint64_t b;
    std::memcpy(&b, &t, 8);
    b &= 0x7fffffffffffffffull; // ANDPD 140bad200
    std::memcpy(area, &b, 8);
}

// @src 1400407e7  ER1 unit normal and ER2 area of every polygon
void fun_1400407e7(i32* obj, i32* inst, i32* fnorm, i32* farea, const i32* /*npts*/,
                   const i32* npoly, const i32* /*zero*/, i32* flag)
{
    i32& cnt = I(0x14066ba84);
    i32& p = I(0x14066ba88);
    double* x = DP(0x14066ba90); // x(3, 4)
    i32* conn = IP(0x14066baf0);
    double* n = DP(0x14066bb20);
    double& area = D(0x14066bb40);
    double& area2 = D(0x14066bb48);

    cnt = 4;
    p = 1;
    *flag = 0;
    if (*npoly > 0) {
        do {
            i32 ent = p;
            fun_14004f363(obj, inst, &ent, x, conn, &cnt);
            fun_140066a50(x, n, flag);
            i32 three = 3;
            ent = p;
            fun_1402283b0(obj, inst, &ent, fnorm, n, &three);
            i32 mode = 0;
            area = 0.0;
            fun_140075036(x, &area, conn, &mode);
            if (cnt == 4 && conn[0] != conn[3] && conn[3] > 0) {
                // second triangle (1, 3, 4): X2 := X3, X3 := X4
                double t[6];
                std::memcpy(t, x + 6, sizeof t);
                std::memcpy(x + 3, t, sizeof t);
                i32 mode2 = 0;
                fun_140075036(x, &area2, conn, &mode2);
                area = sse::add(area, area2);
            }
            i32 one = 1;
            ent = p;
            fun_1402283b0(obj, inst, &ent, farea, &area, &one);
            p = p + 1;
        } while (p <= *npoly);
    }
}

} // namespace def::cmd::ctc3d
