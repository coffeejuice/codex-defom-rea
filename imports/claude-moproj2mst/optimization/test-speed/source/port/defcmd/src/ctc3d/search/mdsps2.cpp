// GENCTC 3D engine, group "search": MDSPS2_REC FUN_14006798b (GENCTC_3d_search.md sec. 9) with its
// candidate reorder 14006e938, vertex re-search 14006de2d (sec. 10.7) and the helpers 14006790a,
// 14006fb9b. Control flow and every floating-point test from the listings
// (deform-rea-work/out/p677/lst/L_<addr>.txt): COMISD + JP branches decide what a NaN does, and
// they are written out where the decompile's C reads differently. The SAVE variables of these
// routines live in the static image at their binary addresses (attribute buffers keep their old
// contents when a cube Get finds no field, and some outputs of the kernels are not always
// written). The Fortran ALLOCATABLE candidate lists (descriptors 1406a3160, 1406a31b0, 1406acac0)
// are std::vector.
#include <defcmd/ctc3d/cube.hpp>
#include <defcmd/ctc3d/search.hpp>
#include <defrt/ftn/fio.hpp>
#include <defrt/ftn/geoutl.hpp>
#include <defrt/ftn/sse_ops.hpp>

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <vector>

namespace def::cmd::ctc3d {

namespace fio = def::rt::ftn::fio;
namespace geo = def::rt::geoutl;

namespace {

using def::rt::sse::add;
using def::rt::sse::div;
using def::rt::sse::mul;
using def::rt::sse::sub;

constexpr double kTiny10f = 1.000000013351432e-10;   // DAT_140bad130 (1e-10f)
constexpr double kTiny12f = 9.999999960041972e-13;   // DAT_140badf00 (1e-12f)
constexpr double kTiny8f = 9.99999993922529e-09;     // DAT_140bad7f8 (1e-8f)
constexpr double kTiny6f = 9.999999974752427e-07;    // DAT_140bad3c0 (1e-6f)
constexpr double kGrow = 1.0099999904632568;         // DAT_140badf08 (1.01f)
constexpr double kPad4f = 9.999999747378752e-05;     // DAT_140bad1f8 (1e-4f)
constexpr double kOnPoly = 1.000001;                 // DAT_140badf50
constexpr double kOneHalf = 1.5;                     // DAT_140bad1b0
constexpr double kHundredth = 0.009999999776482582;  // DAT_140bad100 (0.01f)
constexpr double kHalf = 0.5;                        // DAT_140bad260
constexpr double kTen = 10.0;                        // DAT_140badb90
constexpr double kLimHi = 0.8999999761581421;        // DAT_140bad800 (0.9f)
constexpr double kLimLo = -0.05999999865889549;      // DAT_140bad9b0 (-0.06f)
constexpr double kFlatEdge = 0.10000000149011612;    // DAT_140bad2d0 (0.1f)
constexpr double kFullTurn = 6.179999828338623;      // DAT_140bae080 (6.18f)
constexpr double kNearPi = 0.20000000298023224;      // DAT_140bada60 (0.2f)
constexpr double kPiHi = 3.1415998935699463;         // DAT_140bae088 (3.1416f)
constexpr double kPiLo = 3.140000104904175;          // DAT_140bade58 (3.14f)
constexpr double kSmallAngle = 0.019999999552965164; // DAT_140bae090 (0.02f)
constexpr double kAreaRatio = 9.999999747378752e-06; // DAT_140bae1c0 (1e-5f)

// unit of the engine's messages (NEWEQ.MSG, DAT_14077a7bc)
i32 msg_unit() { return dat_14077a7bc; }

} // namespace

// @src 14006790a  polygon presence in a box (box query of the cube)
void fun_14006790a(i32* obj, i32* inst, double* box, i32* empty)
{
    i32& n = m<i32>(0x1406a3150);
    i32 exact = 0; // [RSP+0x30]
    i32 dbg = 0;   // [RSP+0x34]
    *empty = 1;
    fun_140228c60(obj, inst, box, &n, &exact, &dbg);
    if (n != 0) {
        *empty = 0;
    }
}

// @src 14006fb9b  projection on a neighbour polygon (quad 1404d203b or triangle 1400702c8)
void fun_14006fb9b(double* x, double* nrm, i32* kind, double* p, double* s, double* t, double* q,
                   double* din, double* d, i32* trimode, i32* flag, i32* inside)
{
    *flag = 0;
    *inside = 0;
    if (*kind == 0) {
        geo::fun_1404d203b(x, p, s, t, q, d, flag);
        double* v = mp<double>(0x1406acee0);
        v[0] = sub(p[0], q[0]);
        v[1] = sub(p[1], q[1]);
        v[2] = sub(p[2], q[2]);
        double& c = m<double>(0x1406acf00);
        c = add(mul(v[2], nrm[2]), add(mul(v[1], nrm[1]), mul(v[0], nrm[0])));
        if (0.0 > c) {
            *d = -std::fabs(*d);
        }
        geo::fun_1404d3e13(s, t, inside);
    } else if (*kind == 1) {
        *d = std::fabs(*din);
        fun_1400702c8(p, x, q, d, s, t, trimode, mp<i32>(0x1406acf08), inside);
        *trimode = 1;
    }
}

// @src 14006e938  candidate reorder: EI2 == -k, then EI2 > 0, then the rest (stable)
void fun_14006e938(i32* obj, i32* inst, i32* list, i32* n, i32* k)
{
    i32& cnt = m<i32>(0x1406acb10);
    i32& i = m<i32>(0x1406acb14);
    i32& poly = m<i32>(0x1406acb18);
    i32& f = m<i32>(0x1406acb1c);
    i32& fcnt = m<i32>(0x1406acb20);
    const i32 nn = *n;
    // ALLOCATE (tmp(max(n,0))): the original's contents are not initialised; every entry is
    // written below unless *k < 0, which MDSPS2_REC never passes (k = -mode with mode < 1).
    std::vector<i32> tmp(static_cast<std::size_t>(std::max(nn, 0)), 0);
    const i32 k0 = *k;
    cnt = 0;
    auto ei2 = [&]() {
        i32 field = 2;
        fun_140228260(obj, inst, &poly, &field, &f, &fcnt);
    };
    if (0 < k0) {
        for (i = 1; i <= nn; ++i) {
            poly = list[i - 1];
            ei2();
            if (f == -k0) {
                tmp[static_cast<std::size_t>(cnt++)] = poly;
            }
        }
    }
    for (i = 1; i <= nn; ++i) {
        poly = list[i - 1];
        ei2();
        if (0 < f) {
            tmp[static_cast<std::size_t>(cnt++)] = poly;
        }
    }
    if (nn < 1) {
        return;
    }
    const i32 k1 = *k;
    for (i = 1; i <= nn; ++i) {
        poly = list[i - 1];
        ei2();
        if (f < 1 && (*k == 0 || (f != -k1 && 0 < *k))) {
            tmp[static_cast<std::size_t>(cnt++)] = poly;
        }
    }
    for (i = 1; i <= nn; ++i) {
        list[i - 1] = tmp[static_cast<std::size_t>(i - 1)];
    }
}

// @src 14006de2d  vertex re-search: ray from p to the centroid of the largest polygon at the node
void fun_14006de2d(i32* obj, i32* inst, i32* n, i32* list, i32* node, double* p, double* d)
{
    double& amax = m<double>(0x1406aa930);
    i32& pmax = m<i32>(0x1406aa938);
    i32* ni1 = mp<i32>(0x1406aa940);
    i32& ni1c = m<i32>(0x1406ac880);
    i32& i = m<i32>(0x1406ac884);
    i32& poly = m<i32>(0x1406ac888); // NI1 entry, then the largest polygon
    double& area = m<double>(0x1406ac890);
    i32& cdum = m<i32>(0x1406ac898);
    i32& ei2 = m<i32>(0x1406ac89c);
    double* xc = mp<double>(0x1406ac8a0);
    i32* connc = mp<i32>(0x1406ac900);
    i32& nc = m<i32>(0x1406ac920);
    double* c = mp<double>(0x1406ac930);
    i32& nv = m<i32>(0x1406ac950);
    i32& k = m<i32>(0x1406ac954);
    double* u = mp<double>(0x1406ac960);
    double& len = m<double>(0x1406ac980);
    i32& prev = m<i32>(0x1406ac988);
    double& best = m<double>(0x1406ac990);
    i32& win = m<i32>(0x1406ac998);
    i32& bad = m<i32>(0x1406ac99c);
    i32& qp = m<i32>(0x1406ac9a0);
    double* xq = mp<double>(0x1406ac9b0);
    i32* connq = mp<i32>(0x1406aca10);
    i32& nq = m<i32>(0x1406aca30);
    double& aq = m<double>(0x1406aca38);
    double& ratio = m<double>(0x1406aca40);
    double* nrm = mp<double>(0x1406aca50);
    i32& rmode = m<i32>(0x1406aca70);
    i32& nrmc = m<i32>(0x1406aca74);
    double& dq = m<double>(0x1406aca78);
    double& sq = m<double>(0x1406aca80);
    double& tq = m<double>(0x1406aca88);
    double* qq = mp<double>(0x1406aca90);
    i32& hit = m<i32>(0x1406acab0);
    double& dot = m<double>(0x1406acab8);

    i32 field = 1;
    amax = 0.0;
    pmax = 0;
    fun_140228830(obj, inst, node, &field, ni1, &ni1c);
    for (i = 1; i <= ni1c; ++i) {
        poly = ni1[i - 1];
        field = 2;
        fun_140228540(obj, inst, &poly, &field, &area, &cdum);
        field = 2;
        fun_140228260(obj, inst, &poly, &field, &ei2, &cdum);
        if (ei2 != 0 && area > amax) {
            pmax = poly;
            amax = area;
        }
    }
    poly = pmax;
    fun_14004f363(obj, inst, &poly, xc, connc, &nc);
    c[0] = 0.0;
    c[1] = 0.0;
    nv = nc;
    c[2] = 0.0;
    if (nc == 4) {
        const i32 a1 = connc[0] < 0 ? -connc[0] : connc[0];
        const i32 a4 = connc[3] < 0 ? -connc[3] : connc[3];
        if (a1 == a4) {
            nv = 3;
        }
    }
    for (k = 1; k <= nv; ++k) {
        c[0] = add(xc[3 * (k - 1)], c[0]);
        c[1] = add(xc[3 * (k - 1) + 1], c[1]);
        c[2] = add(xc[3 * (k - 1) + 2], c[2]);
    }
    const double dv = static_cast<double>(nv);
    c[1] = div(c[1], dv);
    c[2] = div(c[2], dv);
    c[0] = div(c[0], dv);
    u[0] = sub(c[0], p[0]);
    u[1] = sub(c[1], p[1]);
    u[2] = sub(c[2], p[2]);
    const double l = def::rt::sse::sqrt(add(mul(u[2], u[2]), add(mul(u[1], u[1]), mul(u[0], u[0]))));
    len = l;
    if (l <= 0.0) { // JNC: a NaN goes on
        return;
    }
    u[2] = div(u[2], l);
    u[1] = div(u[1], l);
    u[0] = div(u[0], l);
    prev = 0;
    win = 0;
    bad = 0;
    best = l;
    if (0 < *n) {
        for (k = 1; k <= *n; ++k) {
            qp = list[k - 1];
            if (prev == qp) {
                continue;
            }
            prev = qp;
            if (poly == qp) {
                continue;
            }
            field = 2;
            fun_140228260(obj, inst, &qp, &field, &ei2, &cdum);
            if (ei2 == 0) {
                continue;
            }
            fun_14004f363(obj, inst, &qp, xq, connq, &nq);
            if (nq == 4) {
                const i32 a1 = connq[0] < 0 ? -connq[0] : connq[0];
                const i32 a4 = connq[3] < 0 ? -connq[3] : connq[3];
                if (a1 == a4 || connq[3] == 0) {
                    nq = 3;
                }
            }
            field = 2;
            fun_140228540(obj, inst, &qp, &field, &aq, &cdum);
            bad = 0;
            ratio = div(aq, amax);
            if (ratio < kAreaRatio || aq < kTiny8f) {
                fun_140066a50(xq, nrm, &bad);
            }
            rmode = 2;
            field = 1;
            fun_140228540(obj, inst, &qp, &field, nrm, &nrmc);
            dq = l;
            if (nq == 3) {
                fun_1400647a0(p, u, nrm, xq, &dq, &sq, &tq, qq, &hit, &rmode, &bad);
            } else {
                fun_140064f58(p, u, nrm, xq, &dq, &sq, &tq, qq, &hit, &rmode, &bad);
            }
            if (dq < 0.0 || dq > l) {
                hit = 0;
            } else if (hit == 1 && best > dq) {
                win = qp;
                best = dq;
            }
        }
        if (win != 0) {
            goto sign;
        }
    }
    win = poly;
sign:
    field = 1;
    fun_140228540(obj, inst, &win, &field, nrm, &nrmc);
    dot = add(mul(u[2], nrm[2]), add(mul(u[1], nrm[1]), mul(u[0], nrm[0])));
    if (dot <= 0.0) { // JP/JC: a NaN takes the negative sign
        *d = std::fabs(*d);
    } else {
        *d = -std::fabs(*d);
    }
}

// @src 14006798b  MDSPS2_REC point-to-surface search
void fun_14006798b(i32* obj, i32* inst, i32* erfield, i32* npoly, double* p, double* st, i32* poly,
                   double* d, i32* status, double* q, double* dirn, i32* mode, i32* dbg)
{
    i32& trimode = m<i32>(0x1406a3200); // 1400702c8 mode: 0 for the first triangle of the call
    i32& a3204 = m<i32>(0x1406a3204);   // 1 when mode < 0, else mode
    i32& a3208 = m<i32>(0x1406a3208);   // mode when < 0, else 0
    i32& usedir = m<i32>(0x1406a320c);
    double& dlen = m<double>(0x1406a3210);
    double& dn = m<double>(0x1406a3218); // distance to the nearest surface node
    i32& inode = m<i32>(0x1406a3220);
    i32& ni3 = m<i32>(0x1406a3224);
    i32& ni3c = m<i32>(0x1406a3228);
    double& nr2 = m<double>(0x1406a3230);
    i32& cnt38 = m<i32>(0x1406a3238);
    i32& cnt3c = m<i32>(0x1406a323c);
    double& r2 = m<double>(0x1406a3240);
    double& base = m<double>(0x1406a3248);
    double& factor = m<double>(0x1406a3250);
    i32& pass = m<i32>(0x1406a3258);
    i32& phase = m<i32>(0x1406a325c);
    i32& src = m<i32>(0x1406a3260);
    double* box = mp<double>(0x1406a3270);
    i32& cnt = m<i32>(0x1406a32a0);
    i32* ni1 = mp<i32>(0x1406a32b0); // NI1 of the node, 7000 entries
    i32& cnt2 = m<i32>(0x1406aa010);
    i32& prevp = m<i32>(0x1406aa040);
    i32& a044 = m<i32>(0x1406aa044); // last contact polygon seen (-900 < EI2 < 0, mode < 0)
    i32& jp = m<i32>(0x1406aa048);
    i32& pp = m<i32>(0x1406aa04c);
    i32& f = m<i32>(0x1406aa050);
    i32& fcnt = m<i32>(0x1406aa054);
    double* x = mp<double>(0x1406aa060);
    i32* conn = mp<i32>(0x1406aa0c0);
    i32& nconn = m<i32>(0x1406aa0f0);
    double& w = m<double>(0x1406aa0f8);
    i32& kk = m<i32>(0x1406aa100);
    i32& tri = m<i32>(0x1406aa104);
    i32& nvert = m<i32>(0x1406aa108);
    double& rb = m<double>(0x1406aa110);
    double& rp = m<double>(0x1406aa118);
    double* pbox = mp<double>(0x1406aa120);
    i32& ov = m<i32>(0x1406aa150);
    double& area = m<double>(0x1406aa158);
    double* nrm = mp<double>(0x1406aa160);
    i32& nrmc = m<i32>(0x1406aa180);
    double& nsum = m<double>(0x1406aa188);
    double& cdir = m<double>(0x1406aa190);
    double& lim = m<double>(0x1406aa198);
    i32& qflag = m<i32>(0x1406aa1a0);
    i32& inside = m<i32>(0x1406aa1a4);
    double& ss = m<double>(0x1406aa1a8);
    double& tt = m<double>(0x1406aa1b0);
    double* qq = mp<double>(0x1406aa1c0);
    double& dist = m<double>(0x1406aa1e0);
    double* v = mp<double>(0x1406aa1f0);
    i32& found = m<i32>(0x1406aa210);
    i32& kn = m<i32>(0x1406aa214);
    i32& prevpoly = m<i32>(0x1406aa218);
    i32& prevcode = m<i32>(0x1406aa21c);
    i32& code = m<i32>(0x1406aa220); // inside code of the accepted polygon
    i32* ei1 = mp<i32>(0x1406aa230);
    i32& ei1c = m<i32>(0x1406aa260);
    i32& qn = m<i32>(0x1406aa264);
    i32& g = m<i32>(0x1406aa268);
    i32& nvfin = m<i32>(0x1406aa26c);
    i32& trifin = m<i32>(0x1406aa270);
    i32& kf = m<i32>(0x1406aa274);
    i32& nodek = m<i32>(0x1406aa278);
    double* er = mp<double>(0x1406aa280);
    i32& erc = m<i32>(0x1406aa2a0);
    double& e = m<double>(0x1406aa2a8);
    double& gdev = m<double>(0x1406aa2b0);
    i32& sgn = m<i32>(0x1406aa2b8);
    i32& vk = m<i32>(0x1406aa2bc);
    i32& vnode = m<i32>(0x1406aa2c0);
    double* nr1 = mp<double>(0x1406aa2d0);
    i32& nr1c = m<i32>(0x1406aa2f0);
    double* uu = mp<double>(0x1406aa300);
    double& ulen = m<double>(0x1406aa320);

    trimode = 0;
    a3204 = *mode;
    *poly = 0;
    a3208 = 0;
    if (*mode < 0) {
        a3204 = 1;
        a3208 = *mode;
    }
    dlen = def::rt::sse::sqrt(add(mul(dirn[2], dirn[2]), add(mul(dirn[1], dirn[1]), mul(dirn[0], dirn[0]))));
    usedir = 0;
    if (dlen >= kTiny10f) {
        usedir = 1;
    }
    if (*dbg == 1) {
        fio::Write(msg_unit()).a("Entering MDSP2S_OBJ").end();
    }
    fun_14022fcd0(obj, inst, p, d, dbg);
    fun_14022d770(obj, inst, p, &dn, reinterpret_cast<std::uint32_t*>(&inode), dbg);
    if (*mode < 0) {
        i32 field = 3;
        fun_140228830(obj, inst, &inode, &field, &ni3, &ni3c);
        if (ni3 == 0) {
            field = 2;
            fun_140228b10(obj, inst, &inode, &field, &nr2, &cnt38);
            dn = mul(def::rt::sse::sqrt(nr2), kOneHalf);
            *d = dn;
        }
    }
    if (*d > dn) {
        *d = dn;
    }
    if (*d <= kTiny12f) {
        i32 field = 2;
        fun_140228b10(obj, inst, &inode, &field, &nr2, &cnt3c);
        r2 = mul(def::rt::sse::sqrt(nr2), kHundredth);
        if (r2 > *d) {
            *d = r2;
        }
        if (*d <= kTiny12f) {
            *d = kTiny8f;
        }
    }
    const double b0 = mul(*d, kGrow);
    const i32 negmode = -*mode; // [RSP+0x270]
    factor = 1.0;
    pass = 0;
    phase = 0;
    base = b0;
    *d = b0;

    std::vector<i32> list; // ALLOCATABLE DAT_1406a3160
    i32 rawcnt = 0;        // R15D: count before 14022a5c0 (size of the re-search list)
    i32 boxcnt = 0;        // raw count of the last box query (bounds 14022a5c0's output)
    for (;;) {
        if (phase == 0) {
            *d = mul(b0, factor);
        }
        if (*dbg == 1 && phase == 0) {
            fio::Write(msg_unit()).a("...factor search = ").d(factor).end();
        }
        if (phase == 0) {
            i32 one = 1;
            i32 idx = 1;
            src = 1;
            geo::fun_1404d1b1c(p, box, d, &one, &idx);
            i32 exact = 0;
            i32 dbg0 = 0;
            fun_140228c60(obj, inst, box, &cnt, &exact, &dbg0);
            boxcnt = cnt;
        } else if (phase == 1) {
            i32 field = 1;
            src = 2;
            fun_140228830(obj, inst, &inode, &field, ni1, &cnt);
            if (7000 < cnt) {
                fio::Write(6).a("Increase NDATA sz from 7000:").i(cnt).end();
                fio::stop();
            }
        }
        rawcnt = cnt;
        cnt2 = cnt;
        if (0 < cnt) {
            list.assign(static_cast<std::size_t>(std::max(cnt, boxcnt)), 0);
            if (src == 1) {
                i32 z = 0;
                fun_14022a5c0(obj, inst, list.data(), reinterpret_cast<std::uint32_t*>(&cnt), &z);
            }
            if (src == 2) {
                for (i32 k = 0; k < cnt; ++k) {
                    list[static_cast<std::size_t>(k)] = ni1[k];
                }
            }
        } else {
            list.clear();
        }
        if (*d == 0.0) {
            *d = kTiny6f;
        }
        // DAT_1406aa020, DAT_1406aa028, DAT_1406aa030 = 0.0 here (never read)
        *status = 0;
        if (phase == 0) {
            ++pass;
        }
        prevp = 0;
        a044 = 0;
        if (*mode < 1) {
            i32 k = negmode;
            fun_14006e938(obj, inst, list.data(), &cnt, &k);
        }
        const i32 ncand = cnt;
        for (jp = 1; jp <= ncand; ++jp) {
            pp = list[static_cast<std::size_t>(jp - 1)];
            if (prevp == pp) {
                continue;
            }
            i32 field = 2;
            prevp = pp;
            fun_140228260(obj, inst, &pp, &field, &f, &fcnt);
            if (f == a3208 && a3208 < 0) {
                f = 1;
            } else if (f < 0 && -900 < f && a3204 == 1) {
                a044 = pp;
            }
            if (!(0 < *mode || (f != 0 && f != 2 && f != -1000 && (-1 < *mode || -1 < f)))) {
                continue;
            }
            fun_14004f363(obj, inst, &pp, x, conn, &nconn);
            w = 1.0;
            if (f != 1) {
                for (kk = 1; kk <= nconn; ++kk) {
                    if (conn[kk - 1] == inode) {
                        w = kOnPoly;
                    }
                }
            }
            tri = 0;
            nvert = 4;
            if (conn[0] == conn[3] || nconn == 3) {
                tri = 1;
                nvert = 3;
            }
            rb = mul(std::fabs(*d), kGrow);
            rp = mul(rb, kPad4f);
            geo::fun_1404d1b1c(x, pbox, &rp, &nvert, mp<i32>(0x1405c81c0)); // idx table 1,2,3,4
            {
                i32 one = 1;
                i32 idx = 1;
                geo::fun_1404d1b1c(p, box, &rb, &one, &idx);
            }
            geo::fun_1404d1ed7(pbox, box, &ov);
            if (ov == 0) {
                continue;
            }
            field = 2;
            fun_140228540(obj, inst, &pp, &field, &area, &cnt38);
            if (area <= kTiny12f) { // JP/JA: a NaN area goes on
                continue;
            }
            field = 1;
            fun_140228540(obj, inst, &pp, &field, nrm, &nrmc);
            nsum = add(add(std::fabs(nrm[1]), std::fabs(nrm[0])), std::fabs(nrm[2]));
            if (nsum <= kTiny10f) {
                continue;
            }
            cdir = 0.0;
            if (usedir == 1) {
                lim = dirn[3];
                cdir = add(mul(dirn[2], nrm[2]), add(mul(dirn[1], nrm[1]), mul(dirn[0], nrm[0])));
                if (lim > kLimHi) {
                    lim = kLimHi;
                }
                if (kLimLo > lim) {
                    lim = kLimLo;
                }
                if (cdir > lim) {
                    continue;
                }
            }
            qflag = 0;
            inside = 0;
            if (tri == 0) {
                geo::fun_1404d203b(x, p, &ss, &tt, qq, &dist, &qflag);
                dist = mul(dist, w);
                v[0] = sub(p[0], qq[0]);
                v[1] = sub(p[1], qq[1]);
                v[2] = sub(p[2], qq[2]);
                cdir = add(mul(v[2], nrm[2]), add(mul(v[1], nrm[1]), mul(v[0], nrm[0])));
                if (0.0 > cdir) {
                    dist = -std::fabs(dist);
                }
                geo::fun_1404d3e13(&ss, &tt, &inside);
            } else if (tri == 1) {
                dist = std::fabs(*d);
                fun_1400702c8(p, x, qq, &dist, &ss, &tt, &trimode, &found, &inside);
                dist = mul(dist, w);
                trimode = 1;
            }
            for (kn = 1; kn <= nvert; ++kn) {
                const i32 cn = conn[kn - 1];
                const i32 an = cn < 0 ? -cn : cn;
                if (inode == an && std::fabs(dist) > dn) {
                    dist = (0.0 > dist) ? -dn : dn;
                }
            }
            if (*dbg == 1) {
                fio::Write(msg_unit()).i(pp).d(ss).d(tt).d(dist).i(qflag).end();
            }
            if (std::fabs(*d) > std::fabs(dist)) {
                const i32 ins = inside;
                prevpoly = *poly;
                prevcode = code;
                *poly = pp;
                code = ins;
                if (0 < ins) {
                    field = 1;
                    fun_140228260(obj, inst, &pp, &field, ei1, &ei1c);
                    qn = ei1[ins - 1];
                    if (qn < 1) {
                        g = 0;
                        code = 0;
                    } else {
                        field = 2;
                        fun_140228260(obj, inst, &qn, &field, &g, &fcnt);
                        bool valid_q = false;
                        if (g == a3208 && a3204 == 1 && a3208 < 0) {
                            g = 1;
                            valid_q = true;
                        } else if (0 < g) {
                            valid_q = true;
                        }
                        if (valid_q && f < 1) {
                            // p itself is not valid: undo, keep the previous best
                            code = prevcode;
                            *poly = prevpoly;
                            continue;
                        }
                        if (g == 0) {
                            code = 0;
                        }
                        if (g == -1000) {
                            dist = std::fabs(dist);
                            code = 0;
                        }
                    }
                }
                nvfin = nvert;
                st[0] = ss;
                trifin = tri;
                st[1] = tt;
                q[0] = qq[0];
                q[1] = qq[1];
                q[2] = qq[2];
                *status = 1;
                *d = dist;
            }
        }
        // DEALLOCATE (list kept until the next pass)
        if (0 < *poly) {
            break;
        }
        if (pass <= 5 || (factor <= 1.0 && phase == 0)) {
            if (a044 == 0) {
                factor = add(factor, kHalf);
            } else {
                factor = mul(factor, kTen);
            }
            if (1 <= pass) {
                usedir = 0;
            }
        } else {
            if (phase != 0) {
                const i32 u = msg_unit();
                *d = std::fabs(*d);
                fio::Write(u).a("Info: No Polygon Found by Polygon Search Code").end();
                fio::Write(u).a(" ").end();
                fio::Write(u).a(" ").end();
                fio::Write(u).a("...Object             = ").i(*obj).end();
                fio::Write(u).a("...Min distance       = ").d(*d).end();
                fio::Write(u).a("...Number of polygons = ").i(*npoly).end();
                fio::Write(u).a(" ").end();
                *status = -1;
                return;
            }
            phase = 1;
        }
    }

    // ---- accepted polygon: final projection and sign (sec. 9.3) ----
    fun_14004f363(obj, inst, poly, x, conn, &nvert);
    if (phase == 1) {
        fio::Write(msg_unit())
            .a("POLY FOUND WITH POINT SRCH:              DIE IPT NDST:")
            .i(*obj)
            .i(inode)
            .i(*poly)
            .end();
    }
    const i32 nv = nvert;
    for (kf = 1; kf <= nv; ++kf) {
        nodek = conn[kf - 1];
        if (nodek < 1) {
            fio::Write(msg_unit()).a("MDSPS2_REC Fatal error").end();
        }
    }
    nvfin = nv;
    qq[2] = p[2];
    qq[1] = p[1];
    qq[0] = p[0];
    if (conn[0] == conn[3] && nv == 4) {
        nvfin = 3;
    } else if (nv == 4) {
        geo::fun_1404d6974(x, q, st, st + 1);
    }
    if (nvfin == 3) {
        const i32 three = 3;
        geo::fun_1404e34eb(x, q, st, st + 1, &three);
    }
    if (code == 0) {
        return;
    }
    if (0 < *erfield) {
        fun_140228540(obj, inst, poly, erfield, er, &erc);
        if (code < 1) {
            // foot at vertex -code
            vk = code < 0 ? -code : code;
            vnode = conn[vk - 1];
            i32 field = 1;
            fun_140228b10(obj, inst, &vnode, &field, nr1, &nr1c);
            if (std::fabs(nr1[3]) <= kTiny8f) {
                geo::fun_1404d68dd(q, p, uu, &ulen);
                cdir = add(mul(uu[2], nr1[2]), add(mul(uu[1], nr1[1]), mul(uu[0], nr1[0])));
                if (cdir > 0.0) {
                    *d = std::fabs(*d);
                    return;
                }
                // re-search over the last box list (ALLOCATABLE DAT_1406a31b0, size R15D)
                std::vector<i32> list2(static_cast<std::size_t>(std::max({rawcnt, boxcnt, 0})), 0);
                i32 z = 0;
                fun_14022a5c0(obj, inst, list2.data(), reinterpret_cast<std::uint32_t*>(&cnt), &z);
                fun_14006de2d(obj, inst, &cnt, list2.data(), &vnode, p, d);
                return;
            }
            if (nr1[3] > 0.0) {
                *d = std::fabs(*d);
            } else if (nr1[3] < 0.0) {
                *d = -std::fabs(*d);
            }
        } else {
            // foot on edge code: dihedral angle of that edge
            e = er[code - 1];
            if (std::fabs(e) <= kFlatEdge || e > kFullTurn) {
                *d = std::fabs(*d);
                return;
            }
            gdev = std::fabs(sub(e, m<double>(0x1405c81d0))); // pi (.data)
            if (kNearPi > gdev) {
                fun_1400839c6(p, x, &nvfin, &code, &sgn);
                if (sgn < 0) {
                    *d = -std::fabs(*d);
                } else {
                    *d = std::fabs(*d);
                }
            } else if (e > kPiHi) {
                *d = std::fabs(*d);
            } else if (kPiLo > e && e > kSmallAngle) {
                *d = -std::fabs(*d);
            }
        }
    }
    if (*dbg == 1) {
        fio::Write(msg_unit()).a("...Found polygon  = ").i(*poly).end();
        fio::Write(msg_unit()).a("...Min distance   = ").d(*d).end();
    }
}

} // namespace def::cmd::ctc3d
