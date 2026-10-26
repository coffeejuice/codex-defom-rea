// GENCTC 3D: the contact decision of one node (packet p67-7, group "decide"): FUN_140040d77 and
// its helpers FUN_14003abfd, FUN_140046471, FUN_140046625, FUN_140046a93. GENCTC_3d_decide.md
// sec. 4.6 (control flow and NaN behaviour of every compare taken from the listing; the labels
// carry the listing addresses). Every local is a SAVE static at its binary address, except the
// stack temporaries of FUN_140040d77 (ad = [RSP+0x2f0], x1/x2 = [RSP+0x2f8]/[RSP+0x300]).
#include "decide_util.hpp"

#include <defcmd/ctc3d/cube.hpp>
#include <defcmd/ctc3d/decide.hpp>
#include <defcmd/ctc3d/search.hpp>
#include <defcmd/ctc3d/surface.hpp>
#include <defrt/ftn/fio.hpp>
#include <defrt/ftn/geoutl.hpp>
#include <defrt/ftn/sse_ops.hpp>

namespace def::cmd::ctc3d {

using namespace decide_detail;
namespace fio = def::rt::ftn::fio;
namespace sse = def::rt::sse;
namespace geo = def::rt::geoutl;

namespace {

// FUN_14006790a(&obj, &inst, box(6), &flag): declared without arguments in search.hpp (provisional).
void call_14006790a(i32* o, i32* e, double* box, i32* flg)
{
    using Fn = void (*)(i32*, i32*, double*, i32*);
    reinterpret_cast<Fn>(&fun_14006790a)(o, e, box, flg);
}

} // namespace

// @src 14003abfd  code of node list(1) that all of list(2..n) share: *code (*dir for code 1)
void fun_14003abfd(i32* bc, i32* list, i32* n, i32* ndof, i32* code, i32* dir)
{
    const i32 n1 = list[0];
    I4(0x1406559e4) = 0;
    I4(0x1406559e8) = 0;
    i32& cntnz = I4(0x1406559ec);
    cntnz = 0;
    const i64 ld = ld_of(ndof);
    I4(0x1406559f0) = n1;
    *dir = 0;
    *code = 0;
    if (n1 < 0) {
        return;
    }
    const i64 b1 = (static_cast<i64>(n1) - 1) * ld;
    if (bc[b1] == 0 && bc[b1 + 1] == 0 && bc[b1 + 2] == 0) {
        return;
    }
    const i32 nn = *n;
    i32& k = I4(0x1406559f4);
    i32& c = I4(0x1406559e4);
    i32& cnt = I4(0x1406559fc);
    i32& j = I4(0x140655a00);
    i32 lim = nn - 1;
    k = 1;
    do {
        I4(0x1406559f8) = 0;
        c = bc[b1 + k - 1];
        if (c != 0) {
            cnt = 0;
            ++lim;
            ++cntnz;
            j = 2;
            I4(0x1406559e8) = c;
            if (*n < 2) {
                if (*n == 1) {
                    goto found;
                }
            } else {
                do {
                    i32& nb = I4(0x140655a04);
                    nb = list[j - 1];
                    const i64 bj = (static_cast<i64>(nb) - 1) * ld;
                    if (c == 1) {
                        if (bc[bj + k - 1] == 1) {
                            ++cnt;
                            I4(0x140655a08) = k;
                        }
                    } else {
                        i32& l = I4(0x140655a0c);
                        l = 1;
                        do {
                            if (bc[bj + l - 1] == c) {
                                ++cnt;
                                break;
                            }
                            ++l;
                        } while (l < 4);
                    }
                    ++j;
                } while (j <= *n);
                if (cnt == nn - 1) {
                    goto found;
                }
            }
            I4(0x1406559f8) = cnt;
            if (cnt == lim) {
                *code = c;
            }
        }
        ++k;
    } while (k < 4);
    return;
found:
    *code = c;
    if (c != 1) {
        return;
    }
    *dir = I4(0x140655a08);
}

// @src 140046471  common NI4 code of the nodes of polygon *poly (via FUN_14003a978)
void fun_140046471(i32* obj, i32* inst, i32* poly, i32* code)
{
    i32& nv = I4(0x14066e690);
    i32& l = I4(0x14066e694);
    fun_140227e00(obj, inst, poly, PI(0x14066e680), &nv);
    l = 1;
    if (nv >= 1) {
        do {
            const i32 c = I4(0x14066e67cull + 4ull * static_cast<u64>(l));
            i32 f = 4;
            I4(0x14066e698) = iabs(c);
            fun_140228830(obj, inst, PI(0x14066e698), &f, PI(0x14066e6a0), PI(0x14066e6c0));
            const i32 a = I4(0x14066e6a0);
            const i32 b = I4(0x14066e6a4);
            const i32 cc = I4(0x14066e6a8);
            const i32 cur = l;
            ++l;
            I4(0x14066e6c4ull + 12ull * static_cast<u64>(cur)) = a;
            I4(0x14066e6c8ull + 12ull * static_cast<u64>(cur)) = b;
            I4(0x14066e6ccull + 12ull * static_cast<u64>(cur)) = cc;
        } while (l <= nv);
    }
    i32 three = 3;
    // node list DAT_1405c6d80 = (1, 2, 3, 1) in .data
    fun_14003a978(PI(0x14066e6d0), PI(0x1405c6d80), &nv, &three, code, PI(0x14066e700));
}

// @src 140046625  un-accept the contacts with object *evict and compact the slots
void fun_140046625(i32* evict, i32* ni4, i32* codes, i32* slot_dir, i32* nacc, double* q)
{
    i32& i = I4(0x14066e704);
    i32& m = I4(0x14066e70c);
    i = 1;
    if (*nacc < 1) {
        m = 0;
    } else {
        const i32 ev = *evict;
        do {
            i32& d = I4(0x14066e708);
            d = slot_dir[i - 1];
            if (codes[d - 1] == ineg(ev)) {
                codes[d - 1] = 0;
                ni4[d - 1] = 0;
                ni4[d + 2] = 0;
                slot_dir[i - 1] = 0;
            }
            ++i;
        } while (i <= *nacc);
        m = 0;
        i = 1;
        do {
            const i32 d = slot_dir[i - 1];
            if (d != 0) {
                ++m;
                double* to = q + 3 * (static_cast<i64>(m) - 1);
                const double* from = q + 3 * (static_cast<i64>(i) - 1);
                to[0] = from[0];
                to[1] = from[1];
                const double z = from[2];
                slot_dir[m - 1] = d;
                to[2] = z;
            }
            ++i;
        } while (i <= *nacc);
    }
    const i32 mm = m;
    double* to = q + 3 * static_cast<i64>(m);
    const double* from = q + 3 * static_cast<i64>(*nacc);
    to[0] = from[0];
    to[1] = from[1];
    const double z = from[2];
    *evict = 0;
    *nacc = mm;
    to[2] = z;
}

// @src 140046a93  *sg = -1 when ER1(a)·ER1(b) < -0.9f, else +1 (NaN: +1)
void fun_140046a93(i32* obj_a, i32* inst_a, i32* poly_a, i32* obj_b, i32* inst_b, i32* poly_b, double* sg)
{
    i32 f = 1;
    fun_140228540(obj_a, inst_a, poly_a, &f, PD(0x14066e750), PI(0x14066e770));
    f = 1;
    fun_140228540(obj_b, inst_b, poly_b, &f, PD(0x14066e780), PI(0x14066e770));
    const double d = dot_l(PD(0x14066e750), PD(0x14066e780));
    if (-0.8999999761581421 > d) { // DAT_140bad8c0
        *sg = -1.0;
        return;
    }
    *sg = 1.0;
}

// @src 140040d77  decision of node *node against the partner objects (sec. 4.6)
void fun_140040d77(double* P, i32* obj, i32* inst, i32* bc, i32* ndof, i32* node, i32* dbg, i32* /*flag*/,
                   double* tol, double* pn, i32* codes, i32* mode, i32* nacc, i32* sep)
{
    const i64 ld = ld_of(ndof);
    *nacc = 0;
    {
        const i32* row = bc_row(bc, ld, *node);
        if (row[0] != 0 && row[1] != 0 && row[2] != 0) {
            return;
        }
    }
    i32& e = I4(0x14066bc08);
    i32& erf = I4(0x14066bc0c);
    e = 1;
    erf = 3;
    i32 f = 2;
    I4(0x14066bc10) = *node;
    pn[0] = P[0];
    pn[1] = P[1];
    pn[2] = P[2];
    i32& nnb = I4(0x14066cbb0);
    fun_140228830(obj, inst, node, &f, PI(0x14066bc14), &nnb);
    if (nnb == 0) {
        return;
    }
    nnb = nnb + 1;
    fun_14003abfd(bc, PI(0x14066bc10), &nnb, ndof, PI(0x14066cbb4), PI(0x14066cbb8));
    if (I4(0x14066cbb4) != 0) {
        return;
    }
    I4(0x14066cbbc) = 0;
    {
        const i32* row = bc_row(bc, ld, *node);
        codes[0] = row[0];
        codes[1] = row[1];
        codes[2] = row[2];
    }
    double& rbox = R8(0x14066cbc0);
    rbox = 9.999999717180685e-10; // DAT_140bad530
    if (*tol > 9.999999717180685e-10) {
        rbox = *tol;
    }
    double* box = PD(0x14066cbd0);
    {
        i32 one_n = 1;
        i32 one_i = 1;
        geo::fun_1404d1b1c(pn, box, &rbox, &one_n, &one_i);
    }
    i32* I = PI(0x1405c6c10); // NI4: code(r) = I[r-1], poly(r) = I[r+2]
    double* M = PD(0x14066cc10);
    f = 4;
    fun_140228830(obj, inst, node, &f, I, PI(0x14066cc00));
    f = 4;
    fun_140228b10(obj, inst, node, &f, M, PI(0x14066cc70));
    f = 1;
    fun_140228b10(obj, inst, node, &f, PD(0x14066cc80), PI(0x14066cca0));
    i32& negC = I4(0x14066cca4);
    i32& negDir = I4(0x14066cca8);
    i32& posDir = I4(0x14066ccac);
    {
        const i32* row = bc_row(bc, ld, *node);
        const i32 c1 = row[0];
        const i32 c2 = row[1];
        const i32 c3 = row[2];
        negC = 0;
        negDir = 0;
        if (c1 < 0) {
            negC = c1;
            negDir = 1;
        }
        if (c2 < 0) {
            negC = c2;
            negDir = 2;
        }
        if (c3 < 0) {
            negC = c3;
            negDir = 3;
        }
        posDir = 0;
        if (c1 > 0) {
            posDir = 1;
        }
        if (c2 > 0) {
            posDir = 2;
        }
        if (c3 > 0) {
            posDir = 3;
        }
    }
    i32& bestObj = I4(0x14066ccb4);
    i32& evict = I4(0x14066ccb8);
    double& bestD = R8(0x14066ccc0);
    I4(0x14066ccb0) = 0;
    bestObj = 0;
    evict = 0;
    bestD = 100000.0; // DAT_140bad3c8
    I4(0x14066ccbc) = 0;
    R8(0x14066ccd0) = 100000.0;
    R8(0x14066ccc8) = 0.05000000074505806; // DAT_140bad758
    double* nOld = PD(0x14066cce0);
    nOld[0] = 0.0;
    nOld[1] = 0.0;
    nOld[2] = 0.0;
    if (negC != 0) {
        bestObj = iabs(negC);
        bestD = sse::mul(*tol, 0.05000000074505806);
        R8(0x14066ccd0) = bestD;
        if (negC != ineg(*obj)) {
            e = inst_of(iabs(negC));
            I4(0x14066cd00) = I[negDir + 2];
            i32 one = 1;
            fun_140228540(&bestObj, &e, PI(0x14066cd00), &one, nOld, PI(0x14066cd04));
        }
    }
    const i32 npart = I4(0x14077867c);
    e = 1;
    i32& ip = I4(0x14066cd08);
    ip = 1;
    if (npart < 1) {
        return;
    }
    const i32 mnegC = ineg(negC); // local_90 / [RSP+0x2a8]
    i32& p = I4(0x14066cd30);
    i32& cc = I4(0x14066cd34);
    i32& npoly = I4(0x14066cd44);
    i32& kmode = I4(0x14066cd48);
    double& d = R8(0x14066cd50);
    i32& poly = I4(0x14066cdb0);
    i32& st = I4(0x14066cdb4);
    i32* conn = PI(0x14066cdc0);
    auto connk = [](i32 j) -> i32& { return I4(0x14066cdbcull + 4ull * static_cast<u64>(static_cast<i64>(j))); };
    i32& nv = I4(0x14066cdf0);
    i32& nvv = I4(0x14066cdf4);
    i32& k = I4(0x14066cdf8);
    i32* ni7a = PI(0x14066ce00);
    i32* ni7b = PI(0x14066ce10);
    i32& nbP = I4(0x14066ce64);
    i32& stNb = I4(0x14066ce68);
    double& sg = R8(0x14066d0a0);
    i32& st2 = I4(0x14066d0ac);
    double* lc = PD(0x14066cd10);
    double* zero4 = PD(0x1405c6c30);
    double ad = 0.0; // [RSP+0x2f0]
    double* Qs = nullptr;

    for (; ip <= npart; ++ip) {
        p = I4(0x140771918ull + 4ull * static_cast<u64>(ip));
        cc = 0;
        if (*obj == p || p == 0) {
            continue;
        }
        if (*sep != 0 && *sep == p) {
            continue;
        }
        {
            const i32* row = bc_row(bc, ld, *node);
            const i32 mp = ineg(p);
            if (row[0] == mp || row[1] == mp || row[2] == mp) {
                continue;
            }
        }
        if (I4(0x14075bc70) == 1) {
            // node -> object table (never written in DEF_PRE)
            const i64 idx = m<std::int64_t>(0x14075bc74) + *node;
            i32& t = I4(0x14066cd38);
            t = I4(0x14075bc60ull + 4ull * static_cast<u64>(idx) - 4ull);
            if (t != p && t != 1000) {
                continue;
            }
        }
        e = inst_of(p);
        if (I4(0x140772728ull + 4ull * static_cast<u64>(static_cast<i64>(e) * 100 + p)) == 1) {
            continue;
        }
        I4(0x14066cd3c) = I4(0x14076b6a8ull + 4ull * static_cast<u64>(static_cast<i64>(p) * 3 + static_cast<i64>(e) * 300));
        if (I4(0x14066cd3c) <= 0) {
            continue;
        }
        call_14006790a(&p, &e, box, PI(0x14066cd40));
        if (I4(0x14066cd40) == 1) {
            continue;
        }
        npoly = npoly_of(p, e);
        if (npoly == 0) {
            continue;
        }
        geo::fun_1404d1ed7(box, PD(0x14075a9b8ull + 8ull * static_cast<u64>(static_cast<i64>(e) * 600 + static_cast<i64>(p) * 6)),
                           PI(0x14066cbb4));
        if (I4(0x14066cbb4) == 0) {
            continue;
        }
        kmode = 0;
        I4(0x14066cd04) = 0;
        d = 100000.0;
        Qs = PD(0x14066cd48ull + 24ull * static_cast<u64>(static_cast<i64>(*nacc) + 1));
        fun_14006798b(&p, &e, &erf, &npoly, P, lc, &poly, &d, &st, Qs, zero4, &kmode, dbg);
        if (poly == 0) {
            fio::Write(6).a("Invalid Contact search at Node ").i(*node).a(" wrt Die No. ").i(p).end();
            continue;
        }
        if (poly < 0) { // 1400429d0
            ad = fabs_b(d);
            k = 0;
            nbP = 0;
            goto L_R;
        }
        // ---- classification of the foot point (1400441cd7..) ----
        fun_140227e00(&p, &e, &poly, conn, &nv);
        nvv = nv;
        if (nv == 4 && conn[0] == iabs(conn[3])) {
            nvv = 3;
            geo::fun_1404e37c4(lc, PD(0x14066cd18), &k);
        } else if (nv == 4) {
            geo::fun_1404d3e13(lc, PD(0x14066cd18), &k);
        } else {
            geo::fun_1404e37c4(lc, PD(0x14066cd18), &k);
        }
        if (k < 0) { // 140043e1b
            fun_1400446b4(&p, &e, &poly, conn, &k, ni7a, ni7b);
            if (k < 0) {
                continue;
            }
            if (ni7a[1] > 0 && ni7b[1] > 0) {
                posDir = 0;
                goto L_1f6f;
            }
        }
        // 140041da9
        if (posDir != 0 && k > 0) {
            I4(0x14066ce20) = k;
            i32& na = I4(0x14066ce24);
            i32& kn = I4(0x14066ce28);
            i32& nb = I4(0x14066ce2c);
            na = iabs(connk(k));
            kn = k + 1;
            if (nvv < kn) {
                kn = 1;
            }
            nb = iabs(connk(kn));
            f = 2;
            fun_140228260(&p, &e, &poly, &f, PI(0x14066ce30), PI(0x14066ce34));
            f = 7;
            fun_140228830(&p, &e, &na, &f, ni7a, PI(0x14066ce38));
            f = 7;
            fun_140228830(&p, &e, &nb, &f, ni7b, PI(0x14066ce38));
            if (I4(0x14066ce30) != 1 && ni7a[0] == 2 && ni7b[0] == 2 && ni7a[1] > 0 && ni7b[1] > 0) {
                posDir = 0;
            }
        }
    L_1f6f:
        if (k <= 0) { // 1400434f3
            ad = fabs_b(d);
            goto L_R;
        }
        f = 1;
        fun_140228260(&p, &e, &poly, &f, PI(0x14066ce40), PI(0x14066ce60));
        nbP = I4(0x14066ce3cull + 4ull * static_cast<u64>(k));
        stNb = 1;
        if (nbP > 0) {
            f = 2;
            fun_140228260(&p, &e, &nbP, &f, &stNb, PI(0x14066ce34));
            if (nbP != 0 && stNb != 0) {
                ad = fabs_b(d);
                goto L_R;
            }
        } else if (nbP != 0) {
            ad = fabs_b(d);
            goto L_R;
        }
        // 140043523: neighbour missing or disabled
        if (posDir != 0) {
            ad = fabs_b(d);
            goto L_R;
        }
        {
            f = 1;
            fun_140228540(&p, &e, &poly, &f, PD(0x14066ce70), PI(0x14066cd04));
            i32& na = I4(0x14066ce90);
            na = iabs(connk(k));
            fun_140227fc0(&p, &e, &na, PD(0x14066cea0));
            i32& kn = I4(0x14066cec0);
            kn = k + 1;
            if (nv < kn) {
                kn = 1;
            }
            i32& nb = I4(0x14066cec4);
            nb = iabs(connk(kn));
            fun_140227fc0(&p, &e, &nb, PD(0x14066ced0));
            geo::fun_1404d68dd(PD(0x14066ced0), PD(0x14066cea0), PD(0x14066cef0), PD(0x14066cf10));
            geo::fun_1404cce5e(PD(0x14066cef0), PD(0x14066ce70), PD(0x14066cf20), PD(0x14066cf40));
            double* v = PD(0x14066cf50);
            v[0] = sse::sub(Qs[0], P[0]);
            v[1] = sse::sub(Qs[1], P[1]);
            v[2] = sse::sub(Qs[2], P[2]);
            double& dv = R8(0x14066cf70);
            dv = dot_l(v, PD(0x14066cf20));
            if (dv > 0.0) {
                d = fabs_b(d);
            }
            ad = fabs_b(d);
            if (nbP == 0) {
                goto L_R;
            }
            fun_14004f363(&p, &e, &nbP, PD(0x14066cf80), PI(0x14066cfe0), PI(0x14066d000));
            i32& tri = I4(0x14066d004);
            tri = 0;
            if (I4(0x14066d000) == 3) {
                tri = 1;
            } else if (iabs(I4(0x14066cfe0)) == iabs(I4(0x14066cfec))) {
                tri = 1;
            }
            f = 1;
            fun_140228540(&p, &e, &nbP, &f, PD(0x14066d010), PI(0x14066d030));
            I4(0x14066d034) = 0;
            R8(0x14066d038) = sse::mul(ad, 5.0); // DAT_140bad498
            fun_14006fb9b(PD(0x14066cf80), PD(0x14066d010), &tri, P, lc, PD(0x14066cd18), PD(0x14066cea0),
                          PD(0x14066d038), PD(0x14066d040), PI(0x14066d034), PI(0x14066d048), PI(0x14066d04c));
            const double ad2 = fabs_b(R8(0x14066d040));
            if (!(ad > ad2)) {
                goto L_R;
            }
            dv = dot_l(nOld, PD(0x14066d010));
            if (!(-0.8 >= dv)) { // DAT_140bad748
                goto L_R;
            }
            if (!(bestD > ad2)) {
                goto L_R;
            }
            bestD = ad2;
            bestObj = p;
            d = fneg_b(ad2);
            goto L_accept; // (A)
        }

    L_R: // 140042a0b
        {
            I4(0x14066d050) = 0;
            const double b = fabs_b(bestD);
            if (b > ad || (b == ad && d < 0.0)) {
                I4(0x14066d050) = 1;
                bestObj = p;
                bestD = d;
                i32 one = 1;
                fun_140228540(&p, &e, &poly, &one, PD(0x14066d060), PI(0x14066d080));
                if (evict != 0 && *nacc > 0) {
                    i32& ae = I4(0x14066d084);
                    ae = iabs(evict);
                    fun_140046625(&evict, I, codes, PI(0x14066d090), nacc, PD(0x14066cd60));
                    fio::Write(msg_unit()).a(" Info: Intialize Contact at Node : ").i(*node).a(" With Object : ").i(ae).end();
                }
            }
        }
        // 140042ce2
        if (k < 0 && d > 0.0) {
            goto L_accept;
        }
        I4(0x14066d0a8) = 0;
        sg = 1.0;
        if (poly <= 0) {
            goto L_T;
        }
        f = 2;
        fun_140228260(&p, &e, &poly, &f, &st2, PI(0x14066d0b0));
        if (st2 == 2) {
            st2 = 1;
            goto L_2dba;
        }
        if (st2 == -1000) {
            continue;
        }
        if (st2 == 0 || st2 == -2000) { // 140043b76
            if (bestObj != p) {
                continue;
            }
            I4(0x14066d0a8) = 1;
            evict = p;
            i32 one = 1;
            fun_140228540(&p, &e, &poly, &one, PD(0x14066d100), PI(0x14066d120));
            i32 three = 3;
            one = 1;
            fun_14007e62b(Qs, PD(0x14066d130), &three, &one);
            goto L_T;
        }
    L_2dba:
        if (ad > *tol) {
            continue;
        }
        {
            i32 one = 1;
            fun_140228540(&p, &e, &poly, &one, PD(0x14066d0c0), PI(0x14066d080));
            i32 three = 3;
            one = 1;
            fun_14007e62b(Qs, PD(0x14066d0e0), &three, &one);
            I4(0x14066ccbc) = p;
        }
    L_T: // 140042e90
        sg = 1.0;
        if (0.0 > d) {
            sg = -1.0;
        }
        if (k == 0) {
            goto L_3aa0;
        }
        if (k < 0 || st2 == 1) {
            goto L_3036;
        }
        f = 1;
        fun_140228260(&p, &e, &poly, &f, PI(0x14066ce40), PI(0x14066ce60));
        nbP = I4(0x14066ce3cull + 4ull * static_cast<u64>(k));
        if (nbP <= 0) {
            if (posDir == 0) {
                continue;
            }
            goto L_3036;
        }
        f = 2;
        fun_140228260(&p, &e, &nbP, &f, &stNb, PI(0x14066d0b0));
        if (stNb == 2) {
            stNb = 1;
            goto L_3036;
        }
        if (st2 == stNb && stNb < 1) {
            sg = -1.0;
        }
    L_3036:
        if (st2 > 0) {
            goto L_3048;
        }
        goto L_3ae3;
    L_3aa0:
        if (st2 == 0 || st2 == -2000) {
            sg = -1.0;
        }
        if (st2 < 0 && st2 != ineg(*obj)) {
            sg = -1.0;
        }
        if (st2 > 0) {
            goto L_3048;
        }
    L_3ae3:
        if (st2 == 0 || st2 == -2000) { // 1400440f9
            cc = 0;
            fun_140046471(&p, &e, &poly, &cc);
            if (cc <= 0) {
                ad = 0.0;
                sg = -1.0;
            }
            goto L_3b0a;
        }
        if (st2 < 0) { // 140043f8a
            if (st2 <= -1000) {
                goto L_3b0a;
            }
            i32& kobj = I4(0x14066d150);
            i32& kinst = I4(0x14066d154);
            if (negC != st2) { // 140044208
                kobj = ineg(st2);
                kinst = inst_of(kobj);
                kmode = 0;
                double& d3 = R8(0x14066d160);
                d3 = 100000.0;
                i32& poly3 = I4(0x14066d190);
                fun_14006798b(&kobj, &kinst, &erf, &npoly, P, PD(0x14066d170), &poly3, &d3, &st, PD(0x14066d1a0), zero4,
                              &kmode, dbg);
                if (poly3 == 0) {
                    continue;
                }
                f = 2;
                fun_140228260(&kobj, &kinst, &poly3, &f, PI(0x14066d1c0), PI(0x14066d0b0));
                const double ad3 = fabs_b(d3);
                if (ad > ad3 && ad3 > *tol) {
                    continue;
                }
                if (*tol >= ad && *tol >= ad3 && I4(0x14066d1c0) == ineg(p) && st2 == ineg(kobj)) {
                    sg = -1.0;
                } else {
                    fun_140046a93(&p, &e, &poly, &kobj, &kinst, &poly3, &sg);
                }
                goto L_3b0a;
            }
            // 140043fa9: same object as the node's own contact
            kobj = mnegC;
            kinst = inst_of(mnegC);
            i32& dsel = I4(0x14066d158);
            dsel = 1;
            if (negC == I[1]) {
                dsel = 2;
            }
            if (negC == I[2]) {
                dsel = 3;
            }
            i32& kpoly = I4(0x14066d15c);
            kpoly = iabs(I[dsel + 2]);
            fun_140046a93(&p, &e, &poly, &kobj, &kinst, &kpoly, &sg);
        }
    L_3b0a:
        d = sse::mul(ad, sg);
        if (st2 < 0 && st2 == ineg(*obj) && st2 != -2000) {
            st2 = 1;
        }
        goto L_306d;
    L_3048:
        d = sse::mul(ad, sg);
    L_306d:
        if (*mode < 0 && d >= *tol) {
            continue;
        }
        if (sg <= 0.0) {
            kmode = ineg(*obj);
            double& d4 = R8(0x14066d1c8);
            fun_14006798b(&p, &e, &erf, &npoly, P, lc, &poly, &d4, &st, Qs, zero4, &kmode, dbg);
            double& tsmall = R8(0x14066d1d0);
            tsmall = sse::mul(*tol, 1e-06); // DAT_140bad888 (result unused)
            if (tsmall <= 0.0) {
                tsmall = 1e-10; // DAT_140bad4a0
            }
            if (st == -1) {
                fio::Write(msg_unit()).a("Potential DIEGEO interference at Node").i(*node).end();
                continue;
            }
            if (st2 <= 0) {
                d4 = fabs_b(d);
            }
            d = sse::mul(fabs_b(d4), sg);
        }
    L_accept: // 1400432ab
        {
            i32& acc = I4(0x14066d1d8);
            acc = 0;
            if (*mode == 0 && *tol > fabs_b(d)) {
                acc = 1;
            }
            if (*mode < 0 && 0.0 >= d && *tol > fabs_b(d)) {
                acc = 1;
            }
            if (*mode > 0 && 0.0 >= d) {
                acc = 1;
            }
            if (acc != 1) {
                continue;
            }
            // str 1405c6d30: 1001(' Info: Node ',I7,' of Object ',I6,       ' is inside Object ',I6)
            fio::Write(msg_unit(), "(' Info: Node ',I7,' of Object ',I6,       ' is inside Object ',I6)")
                .i(*node)
                .i(*obj)
                .i(p)
                .end();
            const i32 s = *nacc + 1;
            *nacc = s;
            auto slot = [](i32 ss) -> i32& { return I4(0x14066d08cull + 4ull * static_cast<u64>(ss)); };
            if (codes[2] == 0) {
                slot(s) = 3;
                I[2] = ineg(p);
                I[5] = poly;
                codes[2] = ineg(p);
                fun_1400483ed(&p, &e, PD(0x14066cc40), &poly);
            } else if (codes[1] == 0) {
                slot(s) = 2;
                I[1] = ineg(p);
                I[4] = poly;
                codes[1] = ineg(p);
                fun_1400483ed(&p, &e, PD(0x14066cc28), &poly);
            } else if (codes[0] == 0) {
                slot(s) = 1;
                I[0] = ineg(p);
                I[3] = poly;
                codes[0] = ineg(p);
                fun_1400483ed(&p, &e, PD(0x14066cc10), &poly);
            }
        }
    }

    // ---- after the partner loop (1400413cd) ----
    const i32 na = *nacc;
    if (na == 0) {
        return;
    }
    i32& reject = I4(0x14066d1dc);
    i32& keep = I4(0x14066d158);
    reject = 0;
    double* S = PD(0x14066d1f0);
    double* T = PD(0x14066d220);
    if (na == 1) {
        if (*mode == 0) {
            f = 4;
            fun_140228690(obj, inst, node, &f, I, PI(0x14066cc00));
            pn[0] = R8(0x14066cd60);
            pn[1] = R8(0x14066cd68);
            pn[2] = R8(0x14066cd70);
            return;
        }
        reject = 0;
        const i32 dir = I4(0x14066d090);
        nbP = I[dir + 2];
        i32& o1 = I4(0x14066d1e0);
        o1 = iabs(I[dir - 1]);
        I4(0x14066d154) = inst_of(o1);
        f = 1;
        fun_140228540(&o1, PI(0x14066d154), &nbP, &f, S, PI(0x14066d210));
        i32& r = I4(0x14066d214);
        for (r = 1; r <= 3; ++r) {
            if (!(I[r - 1] == negC && negC != 0)) {
                continue;
            }
            I4(0x14066d21c) = I[r + 2];
            I4(0x14066d218) = inst_of(mnegC);
            i32 lo = mnegC;
            f = 1;
            fun_140228540(&lo, PI(0x14066d218), PI(0x14066d21c), &f, T, PI(0x14066d210));
            double& dv = R8(0x14066cf70);
            dv = dot_l(T, S);
            if (-0.8999999761581421 > dv) { // DAT_140bad8c0
                reject = 1;
            }
        }
        if (reject == 1) {
            keep = dir;
            codes[dir - 1] = 0;
            I[dir + 2] = 0;
            I[dir - 1] = 0;
            *nacc = 0;
            return;
        }
    }
    keep = 1;
    if (na == 2) {
        i32& o1 = I4(0x14066d240);
        i32& o2 = I4(0x14066d1e0);
        o1 = ineg(codes[I4(0x14066d090) - 1]);
        o2 = ineg(codes[I4(0x14066d094) - 1]);
        double& l1 = R8(0x14066cf10);
        double& l2 = R8(0x14066d290);
        double* u1 = PD(0x14066d250);
        double* u2 = PD(0x14066d270);
        geo::fun_1404d68dd(P, PD(0x14066cd60), u1, &l1);
        geo::fun_1404d68dd(P, PD(0x14066cd78), u2, &l2);
        if (*mode == 0 && *tol >= l1 && *tol >= l2) {
            fio::Write(msg_unit()).a("Node ").i(*node).a(" is inside of two Objects:").i(o1).i(o2).end();
            return;
        }
        R8(0x14066d298) = dot_l(u2, S);
        const double x2 = sse::mul(R8(0x14066d298), l2); // [RSP+0x300]
        const double x1 = sse::mul(dot_l(u1, T), l1);    // [RSP+0x2f8]
        R8(0x14066d2a8) = fneg_b(x2);
        R8(0x14066d2a0) = fneg_b(x1);
        if (*mode < 0 && *tol >= l1 && *tol > l2 && 0.0 > R8(0x14066d2a0) && 0.0 > R8(0x14066d2a8)) {
            fio::Write(msg_unit()).a("Node ").i(*node).a(" is inside of two Objects:").i(o1).i(o2).end();
        }
        f = 1;
        fun_140228b10(obj, inst, node, &f, PD(0x14066cc80), PI(0x14066cca0));
        f = 2;
        fun_140228b10(obj, inst, node, &f, PD(0x14066d2b0), PI(0x14066d2b8));
        double& lq = R8(0x14066d2c0);
        lq = sse::sqrt(R8(0x14066d2b0));
        if (*tol > lq) {
            lq = *tol;
        }
        R8(0x14066d2c8) = fabs_b(dot_l(T, S));
        i32& changed = I4(0x14066d2d0);
        changed = 0;
        if (R8(0x14066d2c8) > 0.5 && *tol > l1 && *tol > l2) {
            fun_14004522c(obj, inst, P, codes, node, &changed, tol);
            if (changed == 1) {
                return;
            }
        }
        I4(0x14066d2e0) = inst_of(o1);
        I4(0x14066d2e4) = inst_of(o2);
        i32& drop = I4(0x14066d2f0);
        keep = 1;
        drop = 2;
        if (fabs_b(x1) > fabs_b(x2)) {
            keep = 2;
            drop = 1;
        }
        i32& dd = I4(0x14066d2f4);
        dd = I4(0x14066d08cull + 4ull * static_cast<u64>(drop));
        *nacc = na - 1;
        codes[dd - 1] = 0;
        double* mc = PD(0x14066cc10ull + 24ull * static_cast<u64>(static_cast<i64>(dd) - 1));
        mc[0] = 0.0;
        I[dd - 1] = 0;
        mc[1] = 0.0;
        I[dd + 2] = 0;
        mc[2] = 0.0;
    }
    const double* qk = PD(0x14066cd48ull + 24ull * static_cast<u64>(keep));
    pn[2] = qk[2];
    pn[1] = qk[1];
    pn[0] = qk[0];
    i32 n9 = 9;
    f = 4;
    fun_140228980(obj, inst, node, &f, M, &n9);
    f = 4;
    fun_140228690(obj, inst, node, &f, I, PI(0x14066cc00));
}

} // namespace def::cmd::ctc3d
