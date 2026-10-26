// Node normals ININDP (NR1, NR2, NR5, NI2) and the folding check (GENCTC_3d_search.md sec. 5.9,
// step 11). Group "surface", packet p67-7.
#include <defcmd/ctc3d/cube.hpp>
#include <defcmd/ctc3d/search.hpp>
#include <defcmd/ctc3d/surface.hpp>

#include <defrt/ftn/fio.hpp>
#include <defrt/ftn/geoutl.hpp>
#include <defrt/ftn/sse_ops.hpp>

#include <cmath>
#include <cstdint>

#include "surf_util.hpp"

namespace def::cmd::ctc3d {

using namespace surf;
namespace sse = def::rt::sse;

namespace {

double fabs_bits(double v) { return std::fabs(v); } // ANDPD 140bad200

} // namespace

// @src 14005815c  ININDP: area-weighted node normal, nodal area, neighbour list, convexity
void fun_14005815c(i32* obj, i32* inst, const i32* npts, const i32* /*npoly*/)
{
    i32& f1 = I(0x1406843f8);
    i32& f2 = I(0x140684400);
    i32& i = I(0x140684404);
    i32& node = I(0x140684408);
    i32* plist = IP(0x140684410); // NI1, plist(j) = I(0x14068440c + 4j)
    i32& np = I(0x140689230);
    double* xi = DP(0x140689240);
    double& cmax = D(0x140689260);
    double& cmin = D(0x140689268);
    i32& nn = I(0x140689270);
    double& nr2 = D(0x140689278);
    double* sn = DP(0x140689280); // S(3), then conv at 140689298
    i32& j = I(0x1406892a0);
    i32& poly = I(0x1406892a4);
    i32* conn = IP(0x1406892b0); // conn(k) = I(0x1406892ac + 4k)
    i32& ccnt = I(0x1406892c0);
    i32& cflag = I(0x1406892c4);
    i32& nv = I(0x1406892c8);
    i32& k = I(0x1406892cc);
    i32* nbr = IP(0x1406892d0); // nbr(m) = I(0x1406892cc + 4m), 5000 entries
    double& area = D(0x14068e0f0);
    double* pn = DP(0x14068e100);
    double& fnv = D(0x14068e128);
    double& slen = D(0x14068e130);
    double* xj = DP(0x14068e150);
    double& c = D(0x14068e178);
    i32& na = I(0x14068e198);
    i32& nb = I(0x14068e19c);
    i32* ei = IP(0x14068e1a0); // EI1, ei(k) = I(0x14068e19c + 4k)
    i32& nvp = I(0x14068e1c4);
    i32& kk = I(0x14068e1c8);
    double* er3 = DP(0x14068e1d0);
    double& conv = D(0x14068e1f8);
    auto CN = [](i32 m) -> i32 { return I(0x1406892ac + 4ull * static_cast<std::uint32_t>(m)); };
    auto NB = [](i32 m) -> i32& { return I(0x1406892cc + 4ull * static_cast<std::uint32_t>(m)); };
    auto EI = [](i32 m) -> i32 { return I(0x14068e19c + 4ull * static_cast<std::uint32_t>(m)); };

    f1 = 1;
    I(0x1406843fc) = 1;
    f2 = 2;
    i = 1;
    if (*npts < 1) {
        return;
    }
    do {
        i32 one = 1;
        node = i;
        fun_140228830(obj, inst, &node, &one, plist, &np);
        if (np != 0) {
            fun_140227fc0(obj, inst, &node, xi);
            nn = 0;
            j = 1;
            cmax = -1.0; // 140bad880
            nr2 = 0.0;
            cmin = 1.0; // 140bad138
            sn[2] = 0.0;
            sn[1] = 0.0;
            sn[0] = 0.0;
            if (np > 0) {
                do {
                    poly = I(0x14068440c + 4ull * static_cast<std::uint32_t>(j));
                    fun_140227e00(obj, inst, &poly, conn, &ccnt);
                    fun_140062fb2(conn, &ccnt, &cflag);
                    if (cflag != 0) {
                        nv = ccnt;
                        if (ccnt == 4 && conn[0] == conn[3]) {
                            nv = 3;
                        }
                        k = 1;
                        if (nv > 0) {
                            do {
                                const i32 v = CN(k);
                                if (i != v) {
                                    nn = nn + 1;
                                    NB(nn) = v;
                                    if (nn > 5000) {
                                        def::rt::ftn::fio::Write(msg_unit())
                                            .a("Check Connectivity around Node ", 31)
                                            .i(i)
                                            .end();
                                        def::rt::ftn::fio::Write(msg_unit())
                                            .a("Error: Increase Array Size in ININDP", 36)
                                            .end();
                                        return;
                                    }
                                }
                                k = k + 1;
                            } while (k <= nv);
                        }
                        i32 f = 2;
                        fun_140228540(obj, inst, &poly, &f, &area, IP(0x14068e0f8));
                        f = 1;
                        fun_140228540(obj, inst, &poly, &f, pn, IP(0x14068e120));
                        // 1400585cf: CVTSI2SS then CVTSS2SD
                        fnv = static_cast<double>(static_cast<float>(nv));
                        nr2 = sse::add(sse::div(area, fnv), nr2);
                        sn[0] = sse::add(sse::mul(area, pn[0]), sn[0]);
                        sn[1] = sse::add(sse::mul(area, pn[1]), sn[1]);
                        sn[2] = sse::add(sse::mul(area, pn[2]), sn[2]);
                    }
                    j = j + 1;
                } while (j <= np);
            }
            slen = sse::sqrt(sse::add(sse::mul(sn[2], sn[2]),
                                      sse::add(sse::mul(sn[1], sn[1]), sse::mul(sn[0], sn[0]))));
            // 14005835e: COMISD 1.000000013351432e-10 (140bad130), |S|; NaN goes on
            if (!(1.000000013351432e-10 >= slen)) {
                sn[2] = sse::div(sn[2], slen);
                sn[0] = sse::div(sn[0], slen);
                sn[1] = sse::div(sn[1], slen);
                i32 cnt1 = 1;
                fun_140228980(obj, inst, &node, &f2, &nr2, &cnt1); // NR2
                fun_14005a6bf(nbr, &nn);
                I(0x14068e138) = 2;
                fun_140228690(obj, inst, &node, IP(0x14068e138), nbr, &nn); // NI2
                const i32 nnl = nn;
                I(0x14068e13c) = 0;
                I(0x14068e140) = 0;
                I(0x14068e144) = 0;
                i32& m = I(0x14068e148);
                m = 1;
                if (nn > 0) {
                    do {
                        i32& jn = I(0x14068e14c);
                        jn = NB(m);
                        if (*npts < jn) {
                            def::rt::ftn::fio::Write(msg_unit()).a("ERROR", 5).i(nn).i(node).end();
                            ftn_stop();
                        }
                        fun_140227fc0(obj, inst, &jn, xj);
                        // u = unit(X_i - X_j) into 14068e100 (1404d68dd(p, q) gives q - p)
                        def::rt::geoutl::fun_1404d68dd(xj, xi, pn, DP(0x14068e170));
                        c = dot3(pn, sn);
                        if (c > cmax) {
                            cmax = c;
                        }
                        if (cmin > c) {
                            cmin = c;
                        }
                        D(0x14068e180) = 0.1; // 140badc80
                        if (0.1 >= fabs_bits(c)) {
                            I(0x14068e13c) = I(0x14068e13c) + 1;
                        } else if (-0.1 > c) { // 140badc88
                            I(0x14068e140) = I(0x14068e140) + 1;
                        } else if (c > 0.1) {
                            I(0x14068e144) = I(0x14068e144) + 1;
                        }
                        m = m + 1;
                    } while (m <= nnl);
                }
                na = 0;
                nb = 0;
                j = 1;
                D(0x14068e188) = 1.8849540948867798; // 140badc90
                D(0x14068e190) = 4.398226261138916;  // 140badc98
                if (np < 1) {
                    conv = 0.0;
                } else {
                    do {
                        poly = I(0x14068440c + 4ull * static_cast<std::uint32_t>(j));
                        fun_140227e00(obj, inst, &poly, conn, &ccnt);
                        i32 f = 1;
                        fun_140228260(obj, inst, &poly, &f, ei, IP(0x14068e1c0));
                        nvp = ccnt;
                        if (conn[0] == iabs(conn[3])) {
                            nvp = 3;
                        }
                        kk = 1;
                        if (nvp > 0) {
                            do {
                                if (CN(kk) == node && EI(kk) != 0) {
                                    i32 f3 = 3;
                                    fun_140228540(obj, inst, &poly, &f3, er3, IP(0x14068e1f0));
                                    const double a = er3[kk - 1];
                                    if (a > 4.398226261138916) {
                                        na = na + 1;
                                    }
                                    if (1.8849540948867798 > a) {
                                        nb = nb + 1;
                                    }
                                    break;
                                }
                                kk = kk + 1;
                            } while (kk <= nvp);
                        }
                        j = j + 1;
                    } while (j <= np);
                    conv = 0.0;
                    if (na < nb) {
                        conv = static_cast<double>(-nb);
                        if (na > 0) {
                            conv = sse::add(conv, -0.6000000238418579); // 140badca0
                        }
                    } else if (nb < na) {
                        conv = static_cast<double>(na);
                        if (nb != 0) {
                            conv = sse::add(conv, 0.6000000238418579); // 140badca8
                        }
                    }
                }
                sn[3] = conv; // 140689298
                i32 c4 = 4;
                fun_140228980(obj, inst, &node, &f1, sn, &c4); // NR1
                i32 c2 = 2;
                i32 f5 = 5;
                fun_140228980(obj, inst, &node, &f5, DP(0x140689260), &c2); // NR5 = (cmax, cmin)
            } else {
                def::rt::ftn::fio::Write(msg_unit()).a("Potential Problem at node ", 26).i(i).end();
                i32 c4 = 4;
                fun_140228980(obj, inst, &node, &f1, DP(0x1405c7f60), &c4); // NR1 = 0
                i32 c1 = 1;
                nr2 = 0.0;
                fun_140228980(obj, inst, &node, &f2, &nr2, &c1); // NR2 = 0
            }
        }
        i = i + 1;
    } while (i <= *npts);
}

// @src 14007d97c  folding check: tiny flat polygons are made invisible, folded nodes marked
void fun_14007d97c(double* /*x*/, const i32* /*npts*/, const i32* npoly, i32* obj, i32* inst)
{
    double& tot = D(0x1406d2600);
    i32& p = I(0x1406d2608);
    double& area = D(0x1406d2610);
    i32* conn = IP(0x1406d2620); // conn(k) = I(0x1406d261c + 4k)
    i32& ccnt = I(0x1406d2650);
    i32& cflag = I(0x1406d2654);
    double* er3 = DP(0x1406d2660);
    i32* ei = IP(0x1406d2990);
    i32& nv = I(0x1406d29c4);
    double& rel = D(0x1406d29c8);
    i32& stat = I(0x1406d29d0);
    i32& nfold = I(0x1406d29d4);
    i32& k = I(0x1406d29d8);
    i32& k3 = I(0x1406d29dc);
    i32& fnode = I(0x1406d2a14);
    auto CN = [](i32 m) -> i32 { return I(0x1406d261c + 4ull * static_cast<std::uint32_t>(m)); };
    auto OPP = [](i32 m) -> i32& { return I(0x1406d29dc + 4ull * static_cast<std::uint32_t>(m)); };

    tot = 0.0;
    p = 1;
    if (*npoly < 1) {
        p = 1;
        return;
    }
    do {
        i32 f2 = 2;
        i32 ent = p;
        fun_140228540(obj, inst, &ent, &f2, &area, IP(0x1406d2618));
        tot = sse::add(area, tot);
        p = p + 1;
    } while (p <= *npoly);
    const double mean = sse::div(tot, static_cast<double>(*npoly));
    I(0x1406d261c) = 1;
    p = 1;
    do {
        i32 ent = p;
        fun_140227e00(obj, inst, &ent, conn, &ccnt);
        fun_140062fb2(conn, &ccnt, &cflag);
        if (cflag != 0) {
            i32 f = 3;
            ent = p;
            fun_140228540(obj, inst, &ent, &f, er3, IP(0x1406d2980));
            f = 1;
            ent = p;
            fun_140228260(obj, inst, &ent, &f, ei, IP(0x1406d29c0));
            nv = ccnt;
            f = 2;
            ent = p;
            fun_140228540(obj, inst, &ent, &f, &area, IP(0x1406d2618));
            rel = sse::div(area, mean);
            // 14007dc6e..: invisible when rel <= 0.001 and ER3(1..3) <= 1.5 (no NaN)
            const bool tiny = 0.0010000000474974513 >= rel && 1.5 >= er3[0] && 1.5 >= er3[1] &&
                              1.5 >= er3[2];
            if (!tiny) {
                if (iabs(CN(ccnt)) == iabs(conn[0])) {
                    nv = ccnt - 1;
                }
                nfold = 0;
                k = 1;
                if (nv > 0) {
                    i32 c = 3;
                    do {
                        k3 = c;
                        if (nv < c) {
                            k3 = c - nv;
                        }
                        const i32 eid = ei[k - 1];
                        I(0x1406d2a10) = eid;
                        OPP(k) = CN(k3);
                        if (eid > 0) {
                            const double a = std::fabs(er3[k - 1]);
                            // 0.10000000149011612 (140bad2d0), 6.28000020980835 (140bae3f0)
                            if (a < 0.10000000149011612 || a > 6.28000020980835) {
                                nfold = nfold + 1;
                                OPP(k) = 0;
                            }
                        }
                        k = k + 1;
                        c = c + 1;
                    } while (k <= nv);
                    if (nfold == 2) {
                        k = 1;
                        if (nv > 0) {
                            do {
                                if (OPP(k) != 0) {
                                    fnode = OPP(k);
                                }
                                k = k + 1;
                            } while (k <= nv);
                        }
                        i32 f1 = 1;
                        fun_140228b10(obj, inst, &fnode, &f1, er3, IP(0x1406d2980)); // NR1
                        D(0x1406d2678) = 2.0;                                       // 140bad4d0
                        i32 c4 = 4;
                        f1 = 1;
                        fun_140228980(obj, inst, &fnode, &f1, er3, &c4);
                        def::rt::ftn::fio::Write(msg_unit())
                            .a("Info: Folding Node Identified : ", 32)
                            .i(fnode)
                            .end();
                        stat = -1000;
                        i32 fe = 2;
                        ent = p;
                        fun_1402280d0(obj, inst, &ent, &fe, &stat, IP(0x1406d261c)); // EI2
                    }
                }
            } else {
                def::rt::ftn::fio::Write(msg_unit()).a(" Polygon is set to invisible:", 29).i(p).end();
                stat = -1000;
                i32 fe = 2;
                ent = p;
                fun_1402280d0(obj, inst, &ent, &fe, &stat, IP(0x1406d261c)); // EI2
            }
        }
        p = p + 1;
    } while (p <= *npoly);
}

} // namespace def::cmd::ctc3d
