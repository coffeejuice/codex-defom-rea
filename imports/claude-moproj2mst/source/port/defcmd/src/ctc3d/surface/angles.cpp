// Dihedral angles ER3 (GENCTC_3d_search.md sec. 5.8). Group "surface", packet p67-7.
#include <defcmd/ctc3d/cube.hpp>
#include <defcmd/ctc3d/search.hpp>
#include <defcmd/ctc3d/surface.hpp>

#include <defrt/ftn/fio.hpp>
#include <defrt/ftn/geoutl.hpp>
#include <defrt/ftn/sse_ops.hpp>
#include <defrt/stdlib/libm.hpp>

#include <cstdint>
#include <cstring>
#include <vector>

#include "surf_util.hpp"

namespace def::cmd::ctc3d {

using namespace surf;
namespace sse = def::rt::sse;

// @src 14007e62b  b(1..m, 1..n) = a(1..m, 1..n), REAL*8
void fun_14007e62b(const double* a, double* b, const i32* m, const i32* n)
{
    const i32 mm = *m;
    const std::int64_t ld = mm < 0 ? 0 : mm;
    i32& j = I(0x1406d2a38);
    i32& i = I(0x1406d2a3c);
    j = 1;
    if (*n > 0) {
        do {
            i = 1;
            if (mm > 0) {
                do {
                    std::memcpy(b + (j - 1) * ld + i - 1, a + (j - 1) * ld + i - 1, 8);
                    i = i + 1;
                } while (i <= mm);
            }
            j = j + 1;
        } while (j <= *n);
    }
}

// @src 14007ea01  angle = atan2(s, c) of the 2D points p1, p2, moved to (0, 2 pi]
void fun_14007ea01(const double* p1, const double* p2, double* angle)
{
    const double x1 = p1[0];
    const double y1 = p1[1];
    const double x2 = p2[0];
    const double y2 = p2[1];
    double& r1 = D(0x1406d2a50);
    double& r2 = D(0x1406d2a58);
    r1 = sse::sqrt(sse::add(sse::mul(y1, y1), sse::mul(x1, x1)));
    r2 = sse::sqrt(sse::add(sse::mul(y2, y2), sse::mul(x2, x2)));
    *angle = 0.0;
    if (r1 == 0.0) { // JNZ/JP: a NaN length goes on
        return;
    }
    if (r2 == 0.0) {
        return;
    }
    double s = sse::sub(sse::mul(x1, y2), sse::mul(y1, x2));
    double c = sse::add(sse::mul(y2, y1), sse::mul(x2, x1));
    s = sse::div(sse::div(s, r1), r2);
    c = sse::div(sse::div(c, r1), r2);
    D(0x1406d2a60) = s;
    D(0x1406d2a68) = c;
    // 14007eb1a: FUN_1404b3878(xmm0 = s, xmm1 = c) (the decompile drops the arguments)
    const double a = def::rt::libm::ps_atan2(s, c);
    *angle = a;
    if (0.0 >= a) {
        // 3.1415926535898 (1405c8820) * 2.0 (140bad4d0) + angle
        *angle = sse::add(sse::mul(0x1.921fb54442d28p+1, 2.0), *angle);
    }
}

// @src 140062fff  angle between two polygons at their common edge, in the edge's frame
void fun_140062fff(const double* xa, const double* xb, double* angle, const i32* k,
                   const i32* nv)
{
    i32& ka = I(0x1406a13d0);
    i32& ka1 = I(0x1406a13d4);
    i32& ka2 = I(0x1406a13d8);
    i32& kb1 = I(0x1406a13e0);
    double* u = DP(0x1406a13f0);
    double& len = D(0x1406a1410);
    i32& one = I(0x1406a1418);
    double* rot = DP(0x1406a1420);
    double* pa = DP(0x1406a1470);
    double* pb = DP(0x1406a1488);
    i32& i = I(0x1406a14a0);
    double* ca = DP(0x1406a14b0);
    double* cb = DP(0x1406a14c8);
    i32& j = I(0x1406a14e0);
    double& fn = D(0x1406a14e8);
    auto X = [](const double* x, i32 v) { return x + 3 * (static_cast<std::ptrdiff_t>(v) - 1); };

    ka = k[0];
    ka1 = ka + 1;
    if (*nv < ka1) {
        ka1 = 1;
    }
    ka2 = ka1 + 1;
    if (*nv < ka2) {
        ka2 = 1;
    }
    I(0x1406a13dc) = k[1];
    kb1 = k[1] - 1;
    if (k[1] == 1) {
        kb1 = *nv;
    }
    const double* xka = X(xa, ka);
    def::rt::geoutl::fun_1404d68dd(xka, X(xa, ka1), u, &len);
    *angle = 3.140000104904175; // 140bade58
    if (1.0000000036274937e-15 >= len) { // 140bade50; NaN goes on
        return;
    }
    one = 1;
    def::rt::geoutl::fun_1404cf5c0(u, rot);
    if (*nv == 3) {
        i32 n1 = 1;
        def::rt::geoutl::fun_1404d0939(X(xa, ka2), xka, rot, pa, &one, &n1);
        n1 = 1;
        def::rt::geoutl::fun_1404d0939(X(xb, kb1), xka, rot, pb, &one, &n1);
    } else {
        for (i = 1; i <= 3; i = i + 1) {
            ca[i - 1] = 0.0;
            cb[i - 1] = 0.0;
        }
        i = 1;
        if (*nv > 0) {
            do {
                for (j = 1; j <= 3; j = j + 1) {
                    ca[j - 1] = sse::add(X(xa, i)[j - 1], ca[j - 1]);
                    cb[j - 1] = sse::add(X(xb, i)[j - 1], cb[j - 1]);
                }
                i = i + 1;
            } while (i <= *nv);
        }
        fn = static_cast<double>(*nv);
        for (i = 1; i <= 3; i = i + 1) {
            ca[i - 1] = sse::div(ca[i - 1], fn);
            cb[i - 1] = sse::div(cb[i - 1], fn);
        }
        i32 n1 = 1;
        def::rt::geoutl::fun_1404d0939(ca, xka, rot, pa, &one, &n1);
        n1 = 1;
        def::rt::geoutl::fun_1404d0939(cb, xka, rot, pb, &one, &n1);
    }
    fun_14007ea01(pa, pb, angle);
}

// @src 1400610f2  ER3: dihedral angle at every polygon edge from the edge table, 0 without
// neighbour
void fun_1400610f2(i32* obj, const i32* /*npts*/, const i32* npoly, i32* inst, i32* /*fer2*/,
                   i32* fer3, i32* fei1, i32* table, const i32* nedge)
{
    i32& nv = I(0x14069ee70);
    i32* ca = IP(0x14069ee80); // conn(k) = I(0x14069ee7c + 4k)
    i32& cca = I(0x14069eeb0);
    i32& k = I(0x14069eeb4);
    double* er3 = DP(0x14069eec0);
    double& ang = D(0x14069eee0);
    i32& e = I(0x14069eee8);
    i32& pa = I(0x14069eef4);
    i32& pb = I(0x14069eef8);
    i32* cb = IP(0x14069ef00);
    i32& ccb = I(0x14069ef30);
    i32& fa = I(0x14069ef34);
    i32& fb = I(0x14069ef38);
    double* xa = DP(0x14069ef50);
    double* xb = DP(0x14069efb0);
    double& aa = D(0x14069f010);
    i32& acnt = I(0x14069f018);
    double& ab = D(0x14069f020);
    i32* ei = IP(0x14069f040); // ei(j) = I(0x14069f03c + 4j)
    auto CA = [](i32 j) -> i32 { return I(0x14069ee7c + 4ull * static_cast<std::uint32_t>(j)); };
    auto CB = [](i32 j) -> i32 { return I(0x14069eefc + 4ull * static_cast<std::uint32_t>(j)); };
    auto EI = [](i32 j) -> i32& { return I(0x14069f03c + 4ull * static_cast<std::uint32_t>(j)); };
    auto R = [&](std::int64_t r, int f) -> i32& { return table[(r - 1) * 8 + f]; };

    nv = 4;
    i32 one1 = 1;
    fun_140227e00(obj, inst, &one1, ca, &cca);
    if (cca > 0) {
        nv = cca;
    }
    // dope 14069ee20: REAL*8 angle per edge (allocated as 2 * nedge INTEGERs)
    const i32 n2 = *nedge * 2;
    std::vector<double> edge_ang(static_cast<std::size_t>(n2 > 0 ? (n2 + 1) / 2 : 0), 0.0);
    for (k = 1; k <= 4; k = k + 1) {
        er3[k - 1] = 0.0;
    }
    ang = 0.0;
    e = 1;
    if (*nedge > 0) {
        i32 i5 = 1;
        i32 i7 = 0;
        do {
            I(0x14069eeec) = i7;
            I(0x14069eef0) = i5;
            i32 m1 = 1;
            i32 n1 = 1;
            fun_14007e62b(&ang, &edge_ang[static_cast<std::size_t>(e - 1)], &m1, &n1);
            e = e + 1;
            i5 = i5 + 2;
            i7 = i7 + 2;
        } while (e <= *nedge);
        i32 i5b = 2;
        e = 1;
        do {
            pa = R(e, 2);
            pb = R(e, 4);
            if (pa != 0 && pb != 0) {
                fun_140227e00(obj, inst, &pa, ca, &cca);
                fun_140227e00(obj, inst, &pb, cb, &ccb);
                fun_140062fb2(ca, &cca, &fa);
                fun_140062fb2(cb, &ccb, &fb);
                if (fa != 0 && fb != 0) {
                    R(e, 7) = 0;
                    if (pa == 0 || pb == 0) {
                        // str 140baddf0 (unreachable: both are != 0 here)
                        def::rt::ftn::fio::Write(msg_unit())
                            .a(" ERROR in Topology", 18)
                            .i(R(e, 0))
                            .i(R(e, 1))
                            .i(pa)
                            .i(pb)
                            .end();
                    } else {
                        nv = cca;
                        if (CA(cca) == ca[0]) {
                            nv = cca - 1;
                        }
                        I(0x14069ef3c) = 0;
                        k = 1;
                        if (nv > 0) {
                            do {
                                I(0x14069ef40) = iabs(CA(k));
                                fun_140227fc0(obj, inst, IP(0x14069ef40), xa + 3 * (k - 1));
                                I(0x14069ef40) = iabs(CB(k));
                                fun_140227fc0(obj, inst, IP(0x14069ef40), xb + 3 * (k - 1));
                                k = k + 1;
                            } while (k <= nv);
                        }
                        i32 f2 = 2;
                        fun_140228540(obj, inst, &pa, &f2, &aa, &acnt);
                        f2 = 2;
                        fun_140228540(obj, inst, &pb, &f2, &ab, &acnt);
                        // 140061b00: COMISD 1.000000013351432e-10 (140bad130), area; NaN goes on
                        if (1.000000013351432e-10 >= aa) {
                            def::rt::ftn::fio::Write(msg_unit())
                                .a("Warning -- the area of Polygon (", 32)
                                .i(pa)
                                .a(") is Zero!", 10)
                                .end();
                        } else if (1.000000013351432e-10 >= ab) {
                            def::rt::ftn::fio::Write(msg_unit())
                                .a("Warning -- the area of Polygon (", 32)
                                .i(pb)
                                .a(") is Zero!", 10)
                                .end();
                        } else {
                            I(0x14069f028) = R(e, 3);
                            I(0x14069f02c) = R(e, 5);
                            I(0x14069f030) = I(0x14069f028);
                            I(0x14069f034) = I(0x14069f02c);
                            if (pb != 0) {
                                fun_140062fff(xa, xb, &ang, IP(0x14069f030), &nv);
                                I(0x14069eef0) = i5b - 1;
                                I(0x14069eeec) = i5b - 2;
                                i32 m1 = 1;
                                i32 n1 = 1;
                                fun_14007e62b(&ang, &edge_ang[static_cast<std::size_t>(e - 1)], &m1,
                                              &n1);
                            }
                        }
                    }
                }
            }
            e = e + 1;
            i5b = i5b + 2;
        } while (e <= *nedge);
    }
    // 14006144c..: per polygon ER3(k) = angle of the edge EI1(nv + k); nv and the conn count are
    // those left by the last edge processed above (as in the original)
    const i32 cnt = cca;
    k = 1;
    if (*npoly > 0) {
        do {
            i32 ent = k;
            fun_140228260(obj, inst, &ent, fei1, ei, &acnt);
            if (nv < cnt) {
                i32& j = I(0x14069f060);
                j = cnt + 1;
                i32 dst = cnt;
                if (j <= acnt) {
                    do {
                        EI(dst) = EI(j);
                        dst = dst + 1;
                        j = j + 1;
                    } while (j <= acnt);
                }
            }
            I(0x14069f064) = nv + 1;
            const i32 last = nv * 2;
            I(0x14069f068) = last;
            i32& j = I(0x14069f06c);
            j = nv + 1;
            i32 i6 = 1;
            while (j <= last) {
                const i32 id = EI(j);
                I(0x14069f070) = id;
                I(0x14069eeec) = id * 2 - 2;
                I(0x14069eef0) = id * 2 - 1;
                if (EI(i6) == 0 || id == 0) {
                    er3[i6 - 1] = 0.0;
                } else if (id < 0 || id > static_cast<i32>(edge_ang.size())) {
                    // outside the ALLOCATEd array: the original reads heap memory; the port
                    // stores 0 (only reachable with the stale nv/count shift above)
                    er3[i6 - 1] = 0.0;
                } else {
                    i32 m1 = 1;
                    i32 n1 = 1;
                    fun_14007e62b(&edge_ang[static_cast<std::size_t>(id - 1)], &er3[i6 - 1], &n1,
                                  &m1);
                }
                j = j + 1;
                i6 = i6 + 1;
            }
            ent = k;
            fun_1402283b0(obj, inst, &ent, fer3, er3, &nv); // ER3, nv values
            k = k + 1;
        } while (k <= *npoly);
    }
}

} // namespace def::cmd::ctc3d
