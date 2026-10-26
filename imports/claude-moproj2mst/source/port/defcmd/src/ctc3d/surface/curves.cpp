// Edge classes and curves (FTCURV): EI1(nv+k), EI3, EI5, EI6, NI6 (GENCTC_3d_search.md sec. 5,
// step 12). Group "surface", packet p67-7.
#include <defcmd/ctc3d/cube.hpp>
#include <defcmd/ctc3d/surface.hpp>

#include <defrt/ftn/fio.hpp>

#include <emmintrin.h>

#include <cmath>
#include <cstdint>
#include <vector>

#include "surf_util.hpp"

namespace def::cmd::ctc3d {

using namespace surf;

namespace {

// Record r (1-based) of the curve-edge table, 9 INTEGERs: n1, n2, p, q, k, kq, class, curve,
// position in the curve.
inline i32& R9(i32* t, std::int64_t r, int f) { return t[(r - 1) * 9 + f]; }
inline i32 R9(const i32* t, std::int64_t r, int f) { return t[(r - 1) * 9 + f]; }

// Point j (1-based) of a curve, 6 INTEGERs: node a, node b, +-polygon, neighbour, k, kq.
inline i32& P6(i32* p, std::int64_t j, int f) { return p[(j - 1) * 6 + f]; }

// 14005dfa4..14005e01b: NINT as compiled (CMPNLTSD / ANDPD selects, CVTTSD2SI)
i32 nint_listing(double x)
{
    const double h = !(x < 0.0) ? 0.5 : -0.5; // 140bad660 / 140bad668
    const double t = def::rt::sse::add(h, x);
    const double sel = !(std::fabs(x) < 4503599627370496.0) ? x : t; // 140bad678
    return _mm_cvttsd_si32(_mm_set_sd(sel));
}

} // namespace

// @src 140060038  *found = 1 when *node is in aa(1, 1..*n)
void fun_140060038(const i32* aa, const i32* n, const i32* node, i32* found)
{
    i32& i = I(0x140696cfc);
    i = 1;
    *found = 0;
    if (*n > 0) {
        do {
            if (aa[(i - 1) * 2] == *node) {
                *found = 1;
                return;
            }
            i = i + 1;
        } while (i <= *n);
    }
}

// @src 14005fe15  nodes on a number of open edges other than 0 and 2 become special nodes
void fun_14005fe15(i32* /*obj*/, i32* /*inst*/, const i32* rec, const i32* nrec, const i32* npts,
                   i32* cnt, i32* naa, i32* aa)
{
    i32& i = I(0x140696ce8);
    auto C = [&](i32 v) -> i32* {
        // the original indexes its work array with the raw node number
        return (v >= 1 && v <= *npts) ? &cnt[v - 1] : nullptr;
    };
    i = 1;
    if (*npts > 0) {
        do {
            cnt[i - 1] = 0;
            i = i + 1;
        } while (i <= *npts);
    }
    i = 1;
    if (*nrec > 0) {
        do {
            if (R9(rec, i, 7) == 0) {
                const i32 a = R9(rec, i, 0);
                const i32 b = R9(rec, i, 1);
                I(0x140696cec) = a;
                I(0x140696cf0) = b;
                if (i32* c = C(a)) {
                    *c = *c + 1;
                }
                if (i32* c = C(b)) {
                    *c = *c + 1;
                }
            }
            i = i + 1;
        } while (i <= *nrec);
    }
    const i32 n0 = *naa;
    I(0x140696cf4) = n0;
    i = 1;
    if (*npts > 0) {
        do {
            const i32 v = cnt[i - 1];
            if (v != 0 && v != 2) {
                i32& j = I(0x140696cf8);
                j = 1;
                bool have = false;
                if (n0 > 0) {
                    do {
                        if (aa[(j - 1) * 2] == i) {
                            have = true;
                            break;
                        }
                        j = j + 1;
                    } while (j <= n0);
                }
                if (!have) {
                    const i32 m = *naa + 1;
                    *naa = m;
                    aa[(m - 1) * 2] = i;
                }
            }
            i = i + 1;
        } while (i <= *npts);
    }
}

// @src 1400600bb  trace one curve from *start over the open edge records; EI5 / EI6 / NI6
void fun_1400600bb(i32* obj, i32* inst, i32* rec, const i32* nrec, i32* p, i32* ncurve, i32* np,
                   i32* start, i32* aa, i32* naa)
{
    i32& r = I(0x140696d00);
    i32& cur = I(0x140696d04);
    i32& found = I(0x140696d08);
    i32& ext = I(0x140696d0c);
    i32& r2 = I(0x140696d10);
    i32& nxt = I(0x140696d14);

    r = 1;
    *np = 0;
    if (*nrec < 1) {
        return;
    }
    bool finish = false;
    while (!finish) {
        if (R9(rec, r, 7) == 0) {
            const i32 a = R9(rec, r, 0);
            const i32 b = R9(rec, r, 1);
            const i32 s = *start;
            const bool ea = (s == a);
            const bool eb = (s == b);
            if (ea || eb) {
                R9(rec, r, 8) = 1;
                *np = 1;
                const i32 c = *ncurve + 1;
                R9(rec, r, 7) = c;
                *ncurve = c;
                if (ea) {
                    cur = b;
                }
                if (eb) {
                    cur = a;
                }
                P6(p, 1, 2) = R9(rec, r, 2);
                P6(p, 1, 1) = cur;
                P6(p, 1, 0) = *start;
                if (R9(rec, r, 6) == 1) {
                    P6(p, 1, 2) = -R9(rec, r, 2);
                }
                P6(p, 1, 3) = R9(rec, r, 3);
                P6(p, 1, 4) = R9(rec, r, 4);
                P6(p, 1, 5) = R9(rec, r, 5);
                // 140060244: follow the curve
                for (;;) {
                    fun_140060038(aa, naa, &cur, &found);
                    if (found == 1) {
                        finish = true;
                        break;
                    }
                    ext = 0;
                    r2 = 1;
                    bool done = false;
                    while (r2 <= *nrec) {
                        if (R9(rec, r2, 7) == 0) {
                            const i32 a2 = R9(rec, r2, 0);
                            const i32 b2 = R9(rec, r2, 1);
                            const i32 s2 = *start;
                            const bool closing = (s2 == a2 && cur == b2) || (s2 == b2 && cur == a2);
                            if (closing) {
                                if (*np != 1) {
                                    // 1400604c6: close the loop back to start
                                    if (P6(p, *np, 0) != cur) {
                                        const i32 m = *np + 1;
                                        *np = m;
                                        P6(p, m, 2) = R9(rec, r2, 2);
                                        P6(p, m, 0) = cur;
                                        P6(p, m, 1) = *start;
                                        if (R9(rec, r2, 6) == 1) {
                                            P6(p, m, 2) = -R9(rec, r2, 2);
                                        }
                                        R9(rec, r2, 8) = m;
                                        P6(p, m, 5) = R9(rec, r2, 5);
                                        P6(p, m, 4) = R9(rec, r2, 4);
                                        P6(p, m, 3) = R9(rec, r2, 3);
                                        R9(rec, r2, 7) = *ncurve;
                                    }
                                    const i32 m = *np + 1;
                                    P6(p, m, 0) = *start;
                                    P6(p, m, 1) = P6(p, 1, 1);
                                    P6(p, m, 2) = P6(p, 1, 2);
                                    P6(p, m, 3) = P6(p, 1, 3);
                                    P6(p, m, 4) = P6(p, 1, 4);
                                    R9(rec, r2, 7) = *ncurve;
                                    R9(rec, r2, 8) = m;
                                    *np = m;
                                    P6(p, m, 5) = P6(p, 1, 5);
                                    done = true;
                                    break;
                                }
                            } else {
                                const bool xa = (cur == a2);
                                const bool xb = (cur == b2);
                                if (xa || xb) {
                                    // 1400602bb: extend the curve by record r2
                                    const i32 m = *np + 1;
                                    *np = m;
                                    ext = 1;
                                    R9(rec, r2, 7) = *ncurve;
                                    P6(p, m, 0) = cur;
                                    R9(rec, r2, 8) = m;
                                    if (xa) {
                                        nxt = b2;
                                    }
                                    if (xb) {
                                        nxt = a2;
                                    }
                                    cur = nxt;
                                    P6(p, m, 1) = cur;
                                    P6(p, m, 2) = R9(rec, r2, 2);
                                    if (R9(rec, r2, 6) < 0) { // 140060354: sign bit
                                        P6(p, m, 2) = -R9(rec, r2, 2);
                                    }
                                    P6(p, m, 3) = R9(rec, r2, 3);
                                    P6(p, m, 4) = R9(rec, r2, 4);
                                    P6(p, m, 5) = R9(rec, r2, 5);
                                    if (*start == cur) {
                                        done = true;
                                        break;
                                    }
                                    i32 one = 1;
                                    fun_140228b10(obj, inst, &cur, &one, DP(0x140696d20),
                                                  IP(0x140696d40)); // NR1
                                    // 14006040d: |NR1(4)| > 2.0 (140bad4d0) ends at a corner
                                    if (std::fabs(D(0x140696d38)) > 2.0) {
                                        const i32 m2 = m + 1;
                                        P6(p, m2, 1) = 0;
                                        P6(p, m2, 0) = cur;
                                        *np = m2;
                                        done = true;
                                        break;
                                    }
                                }
                            }
                        }
                        r2 = r2 + 1;
                    }
                    if (done) {
                        finish = true;
                        break;
                    }
                    if (ext != 1) {
                        break; // 140060123 -> 140060135: next record of the outer loop
                    }
                }
                if (finish) {
                    break;
                }
            }
        }
        r = r + 1;
        if (*nrec < r) {
            // 140060a72
            if (*np == 0) {
                return;
            }
            if (*np == 1) {
                P6(p, 2, 0) = cur;
                P6(p, 2, 1) = *start;
                P6(p, 2, 2) = P6(p, 1, 2);
                P6(p, 2, 3) = P6(p, 1, 3);
                P6(p, 2, 4) = P6(p, 1, 4);
                *np = 2;
                P6(p, 2, 5) = P6(p, 1, 5);
            }
            break;
        }
    }

    // 1400605cc: store the curve
    const std::int64_t ix = static_cast<std::int64_t>(*obj) * 3 + static_cast<std::int64_t>(*inst) * 300;
    i32& npoly = I(0x14076e588 + 4 * ix);
    if (*ncurve <= npoly) {
        i32 f5 = 5;
        i32 n6 = *np * 6;
        fun_1402280d0(obj, inst, ncurve, &f5, p, &n6); // EI5(curve) = points
    } else {
        def::rt::ftn::fio::Write(msg_unit())
            .a("WARNING: CURVE NUM=", 19)
            .i(*ncurve)
            .a(" EXCEEDS TOTAL NUMBER OF ELEMENTS=", 34)
            .i(npoly)
            .a(" FOR OBJECT ", 12)
            .i(*obj)
            .end();
    }
    i32& j = I(0x140696d44);
    j = 1;
    if (*np < 1) {
        return;
    }
    i32* e6 = IP(0x140696d50);
    auto E6 = [](i32 m) -> i32& { return I(0x140696d4c + 4ull * static_cast<std::uint32_t>(m)); };
    do {
        const i32 b = P6(p, j, 1);
        I(0x140696d48) = iabs(P6(p, j, 2));
        I(0x140696d4c) = b;
        if (I(0x140696d48) > 0 && b > 0) {
            i32 f6 = 6;
            fun_140228260(obj, inst, IP(0x140696d48), &f6, e6, IP(0x140696d70));
            I(0x140696d74) = P6(p, j, 4);
            {
                const i32 kk = I(0x140696d74);
                const i32 c = *ncurve;
                E6(2 * kk) = j;     // 140060711
                E6(2 * kk - 1) = c; // 140060720
            }
            f6 = 6;
            fun_1402280d0(obj, inst, IP(0x140696d48), &f6, e6, IP(0x140696d70));
            I(0x140696d78) = P6(p, j, 3);
            if (P6(p, j, 3) > 0) {
                f6 = 6;
                fun_140228260(obj, inst, IP(0x140696d78), &f6, e6, IP(0x140696d70));
                I(0x140696d7c) = P6(p, j, 5);
                const i32 kk = I(0x140696d7c);
                const i32 c = *ncurve;
                E6(2 * kk) = j;
                E6(2 * kk - 1) = c;
                f6 = 6;
                fun_1402280d0(obj, inst, IP(0x140696d78), &f6, e6, IP(0x140696d70));
            }
            I(0x140696d80) = P6(p, j, 0);
            I(0x140696d84) = b;
            i32* l = IP(0x140696d90);
            i32& lc = I(0x140697d30);
            f6 = 6;
            fun_140228830(obj, inst, IP(0x140696d80), &f6, l, &lc); // NI6
            lc = lc + 1;
            I(0x140696d8c + 4ull * static_cast<std::uint32_t>(lc)) = *ncurve;
            f6 = 6;
            fun_140228690(obj, inst, IP(0x140696d80), &f6, l, &lc);
            if (b > 0) {
                f6 = 6;
                fun_140228830(obj, inst, IP(0x140696d84), &f6, l, &lc);
                lc = lc + 1;
                I(0x140696d8c + 4ull * static_cast<std::uint32_t>(lc)) = *ncurve;
                f6 = 6;
                fun_140228690(obj, inst, IP(0x140696d84), &f6, l, &lc);
            }
        }
        j = j + 1;
    } while (j <= *np);
}

// @src 14005d848  FTCURV: open-edge records, special nodes, curves, NI6 cleanup
void fun_14005d848(i32* obj, i32* inst, const i32* npts, const i32* npoly, i32* rec,
                   i32* ncurve, i32* maxpts)
{
    const i32 n = *npts;
    // dope 140694aa0: aa(2, npts); 1406949e0: p(6, 20000); 140694a40: e(4, b04)
    std::vector<i32> aa(static_cast<std::size_t>(n > 0 ? 2 * n : 0), 0);
    i32 b04 = idiv(n, 4) + 1;
    if (b04 <= *npoly) {
        b04 = *npoly;
    }
    I(0x140694b04) = b04;
    I(0x140694b00) = 20000;
    std::vector<i32> pts(static_cast<std::size_t>(6 * 20000), 0);
    std::vector<i32> e(static_cast<std::size_t>(b04 > 0 ? 4 * b04 : 0), 0);
    auto E = [&](std::int64_t k, std::int64_t pp) -> i32& { return e[(pp - 1) * 4 + k - 1]; };
    auto AA = [&](std::int64_t f, std::int64_t i) -> i32& { return aa[(i - 1) * 2 + f - 1]; };

    I(0x140694b08) = 8;
    for (int q = 0; q < 8; ++q) {
        I(0x1405c7fa0 + 4ull * q) = 0; // the (2, 4) zero table for EI6
    }
    i32& p = I(0x140694b0c);
    i32& k = I(0x140694b10);
    p = 1;
    *maxpts = 0;
    if (*npoly > 0) {
        do {
            for (k = 1; k <= 4; k = k + 1) {
                E(k, p) = 0;
            }
            i32 f6 = 6;
            i32 ent = p;
            fun_1402280d0(obj, inst, &ent, &f6, IP(0x1405c7fa0), IP(0x140694b08)); // EI6 = 0
            p = p + 1;
        } while (p <= *npoly);
    }
    i32& nspec = I(0x140694b14);
    i32& nrec = I(0x140694b18);
    nspec = 0;
    nrec = 0;
    I(0x140694b1c) = 0;
    i32& i = I(0x140694b20);
    i = 1;
    if (n > 0) {
        do {
            i32 f6 = 6;
            i32 one = 1;
            i32 ent = i;
            fun_140228690(obj, inst, &ent, &f6, IP(0x140694b1c), &one); // NI6 = 0
            i32 f1 = 1;
            ent = i;
            fun_140228b10(obj, inst, &ent, &f1, DP(0x140694b30), IP(0x140694b50)); // NR1
            const i32 code = nint_listing(D(0x140694b48));
            I(0x140694b54) = code;
            if (iabs(code) > 2) {
                nspec = nspec + 1;
                AA(2, nspec) = code;
                AA(1, nspec) = i;
            }
            i = i + 1;
        } while (i <= n);
    }

    i32* conn = IP(0x140694b60); // conn(k) = I(0x140694b5c + 4k)
    i32& ccnt = I(0x140694b70);
    i32& nvp = I(0x140694b74);
    i32* ei = IP(0x140694b80); // ei(k) = I(0x140694b7c + 4k)
    i32& ecnt = I(0x140694ba0);
    i32& q = I(0x140694bac);
    i32* qconn = IP(0x140694bb0); // qconn(k) = I(0x140694bac + 4k)
    i32& qcnt = I(0x140694bc0);
    i32& nvq = I(0x140694bc4);
    i32* eq = IP(0x140694bd0); // eq(k) = I(0x140694bcc + 4k)
    i32& kq = I(0x140694bf0);
    i32& jq = I(0x140694bf4);
    auto CN = [](i32 m) -> i32 { return I(0x140694b5c + 4ull * static_cast<std::uint32_t>(m)); };
    auto EIv = [](i32 m) -> i32 { return I(0x140694b7c + 4ull * static_cast<std::uint32_t>(m)); };
    auto QC = [](i32 m) -> i32 { return I(0x140694bac + 4ull * static_cast<std::uint32_t>(m)); };
    auto EQ = [](i32 m) -> i32 { return I(0x140694bcc + 4ull * static_cast<std::uint32_t>(m)); };

    p = 1;
    if (*npoly > 0) {
        do {
            i32 ent = p;
            fun_140227e00(obj, inst, &ent, conn, &ccnt);
            nvp = ccnt;
            if (conn[0] == iabs(CN(ccnt))) {
                nvp = ccnt - 1;
            }
            i32 f1 = 1;
            ent = p;
            fun_140228260(obj, inst, &ent, &f1, ei, &ecnt);
            i32 ic = idiv(ecnt, 2);
            I(0x140694ba4) = ic;
            k = 1;
            if (nvp > 0) {
                i32 c = 2;
                do {
                    ic = ic + 1;
                    const i32 kc = k;
                    if (E(kc, p) < 1) {
                        const i32 cls = EIv(ic);
                        E(kc, p) = 1;
                        if (cls != 0) {
                            i32& k2 = I(0x140694ba8);
                            k2 = c;
                            if (nvp < c) {
                                k2 = 1;
                            }
                            nrec = nrec + 1;
                            const i32 rr = nrec;
                            const i32 qq = EIv(kc);
                            R9(rec, rr, 4) = kc;
                            q = qq;
                            R9(rec, rr, 3) = qq;
                            R9(rec, rr, 2) = p;
                            R9(rec, rr, 0) = CN(kc);
                            R9(rec, rr, 1) = CN(k2);
                            if (qq == 0) {
                                R9(rec, rr, 5) = 0;
                                R9(rec, rr, 6) = 0;
                                R9(rec, rr, 7) = 0;
                            } else {
                                fun_140227e00(obj, inst, &q, qconn, &qcnt);
                                nvq = qcnt;
                                if (qconn[0] == iabs(QC(qcnt))) {
                                    nvq = qcnt - 1;
                                }
                                i32 o1 = 1;
                                fun_140228260(obj, inst, &q, &o1, eq, &ecnt);
                                kq = 0;
                                for (jq = 1; jq <= nvq; jq = jq + 1) {
                                    if (EQ(jq) == p) {
                                        kq = jq;
                                    }
                                }
                                if (kq != 0) {
                                    R9(rec, rr, 6) = cls;
                                    R9(rec, rr, 5) = kq;
                                    R9(rec, rr, 7) = 0;
                                    E(kq, q) = 1;
                                } else {
                                    R9(rec, rr, 5) = 0;
                                    R9(rec, rr, 6) = 0;
                                    R9(rec, rr, 7) = 0;
                                    // list-directed WRITE(6,*), str "Info:  Error in Topology"
                                    def::rt::ftn::fio::Write(6).a("Info:  Error in Topology", 24).end();
                                }
                            }
                        }
                    }
                    k = k + 1;
                    c = c + 1;
                } while (k <= nvp);
            }
            p = p + 1;
        } while (p <= *npoly);
    }

    fun_14005fe15(obj, inst, rec, &nrec, npts, e.data(), &nspec, aa.data());
    const i32 ns = nspec;
    i32& nc = I(0x140694bf8);
    i32& npc = I(0x140694c00);
    nc = 0;
    if (ns > 0) {
        p = 1;
        do {
            do {
                I(0x140694bfc) = AA(1, p);
                fun_1400600bb(obj, inst, rec, &nrec, pts.data(), &nc, &npc, IP(0x140694bfc),
                              aa.data(), &nspec);
                if (*maxpts < npc) {
                    *maxpts = npc;
                }
                if (npc > 20000) {
                    def::rt::ftn::fio::Write(6)
                        .a("Error:Number of points exceeds the array                  limit-FTCURV",
                           70)
                        .end();
                }
            } while (npc > 0);
            p = p + 1;
        } while (p <= ns);
    }
    const i32 nr = nrec;
    if (nr > -1) {
        p = 1;
        while (p <= nr) {
            if (R9(rec, p, 7) == 0) {
                I(0x140694bfc) = R9(rec, p, 0);
                fun_1400600bb(obj, inst, rec, &nrec, pts.data(), &nc, &npc, IP(0x140694bfc),
                              aa.data(), &nspec);
                if (*maxpts < npc) {
                    *maxpts = npc;
                }
            }
            p = p + 1;
        }
    }
    i = 1;
    *ncurve = nc;
    if (n > 0) {
        do {
            i32 f6 = 6;
            i32 ent = i;
            i32& lc = I(0x140694c04);
            fun_140228830(obj, inst, &ent, &f6, aa.data(), &lc); // NI6 into the aa buffer
            if (lc > 1 || aa[0] != 0) {
                fun_14005a6bf(aa.data(), &lc);
                f6 = 6;
                ent = i;
                fun_140228690(obj, inst, &ent, &f6, aa.data(), &lc);
            }
            i = i + 1;
        } while (i <= n);
    }
    I(0x140694c08) = 0;
}

// @src 14005bbf2  edge classes EI1(nv+k), EI3 at disabled polygons, then the curves
void fun_14005bbf2(i32* obj, const i32* npoly, const i32* npts, const i32* nvert, i32* inst,
                   i32* fer3, i32* fei1, i32* table)
{
    i32& p = I(0x140694634);
    i32* ei = IP(0x140694640); // ei(k) = I(0x14069463c + 4k)
    i32& ecnt = I(0x140694660);
    i32& k = I(0x140694664);
    double* er3 = DP(0x140694670);
    i32* conn = IP(0x1406946a0); // conn(k) = I(0x14069469c + 4k)
    i32& ccnt = I(0x1406946d0);
    i32* ei3 = IP(0x1406946e0);  // ei3(k) = I(0x1406946dc + 4k)
    i32& cls = I(0x1406946f0);
    auto EI = [](i32 m) -> i32& { return I(0x14069463c + 4ull * static_cast<std::uint32_t>(m)); };
    auto CN = [](i32 m) -> i32 { return I(0x14069469c + 4ull * static_cast<std::uint32_t>(m)); };

    I(0x140694630) = 3;
    p = 1;
    if (*npoly > 0) {
        const i32 nv = *nvert;
        do {
            i32 ent = p;
            fun_140228260(obj, inst, &ent, fei1, ei, &ecnt);
            for (k = 1; k <= *nvert; k = k + 1) {
                EI(nv + k) = 0; // clear the edge ids of 14007aa22
            }
            ent = p;
            fun_1402280d0(obj, inst, &ent, fei1, ei, &ecnt);
            p = p + 1;
        } while (p <= *npoly);
        p = 1;
        do {
            i32 ent = p;
            fun_140228260(obj, inst, &ent, fei1, ei, &ecnt);
            ent = p;
            fun_140228540(obj, inst, &ent, fer3, er3, IP(0x140694690));
            ent = p;
            fun_140227e00(obj, inst, &ent, conn, &ccnt);
            for (i32 j = 1; j <= ccnt; ++j) {
                ei3[j - 1] = 0;
            }
            I(0x1406946d4) = ccnt + 1;
            if (conn[0] == iabs(CN(ccnt))) {
                ccnt = ccnt - 1;
            }
            if (CN(*nvert) < 1) {
                for (k = 1; k <= ccnt; k = k + 1) {
                    I(0x1406946f4) = EI(k);
                    if (EI(k) == 0) {
                        continue;
                    }
                    fun_140227e00(obj, inst, IP(0x1406946f4), IP(0x140694700), IP(0x140694730));
                    const i32 qc = I(0x140694730);
                    ei3[k - 1] = (I(0x1406946fc + 4ull * static_cast<std::uint32_t>(qc)) < 1) ? 2 : 1;
                }
            } else {
                k = 1;
                if (ccnt > 0) {
                    i32 idx = *nvert;
                    do {
                        idx = idx + 1;
                        cls = 0;
                        if (EI(k) == 0) {
                            cls = 3;
                        } else {
                            const double a = er3[k - 1];
                            if (a >= 4.0122222900390625) { // 140badcb0
                                cls = 2;
                            }
                            if (2.267777919769287 >= a) { // 140badcb8
                                cls = 1;
                            }
                            if (0.009999999776482582 >= a || a > 6.269999980926514) { // 140bad100, cc0
                                cls = 4;
                            }
                        }
                        k = k + 1;
                        EI(idx) = cls;
                    } while (k <= ccnt);
                }
            }
            ent = p;
            fun_1402280d0(obj, inst, &ent, fei1, ei, &ecnt);
            ent = p;
            fun_1402280d0(obj, inst, &ent, IP(0x140694630), ei3, const_cast<i32*>(nvert)); // EI3
            p = p + 1;
        } while (p <= *npoly);
    }
    if (dat_14077a7c0 == 1) {
        const std::int64_t ix = static_cast<std::int64_t>(*obj) + static_cast<std::int64_t>(*inst) * 100;
        I(0x140694738) = I(0x140779624 + 4 * ix);
        I(0x140694734) = I(0x140778684 + 4 * ix);
    }
    fun_14005d848(obj, inst, npts, npoly, table, IP(0x140694734), IP(0x140694738));
    const std::int64_t ix = static_cast<std::int64_t>(*obj) + static_cast<std::int64_t>(*inst) * 100;
    I(0x140778684 + 4 * ix) = I(0x140694734); // number of curves of (obj, inst)
    I(0x140779624 + 4 * ix) = I(0x140694738); // most points in a curve
}

} // namespace def::cmd::ctc3d
