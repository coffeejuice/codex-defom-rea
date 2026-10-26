// GENCTC 3D per-node driver and constraint handling (packet p67-7, group "decide"):
// FUN_14005190e, FUN_14004946f, FUN_14004e10c, FUN_14005266d. GENCTC_3d_decide.md sec. 4.1, 4.2,
// 4.7. Every local of these Fortran routines is a SAVE static at its binary address.
#include "decide_util.hpp"

#include <defcmd/ctc3d/cube.hpp>
#include <defcmd/ctc3d/decide.hpp>
#include <defcmd/ctc3d/search.hpp>
#include <defrt/ftn/fio.hpp>
#include <defrt/ftn/geoutl.hpp>
#include <defrt/ftn/sse_ops.hpp>

namespace def::cmd::ctc3d {

using namespace decide_detail;
namespace fio = def::rt::ftn::fio;
namespace sse = def::rt::sse;

// @src 14005190e  per-node decision and write-back (GENCTC_3d_decide.md sec. 4.1)
void fun_14005190e(i32* obj, i32* inst, double* x, i32* bc, i32* ndof, i32* node, i32* move, i32* mode,
                   i32* dbg, i32* flag, double* tol)
{
    i32& n = I4(0x14067671c);
    double* P = PD(0x140676720);   // working position (3)
    double* nr2 = PD(0x140676740); // NR2 of the node
    double& L = R8(0x140676750);   // sqrt(NR2)
    i32* sep = PI(0x140676758);
    double& tolS = R8(0x140676760);
    double& tol2 = R8(0x140676768);
    double* Pn = PD(0x140676780);
    i32* codes = PI(0x1406767a0);
    i32& nacc = I4(0x1406767b0);

    n = *node;
    const i64 ld = ld_of(ndof);
    *dbg = 0;
    *flag = 0;
    double* xn = x + 3 * (static_cast<i64>(*node) - 1);
    P[2] = xn[2];
    P[1] = xn[1];
    P[0] = xn[0];
    fun_14004946f(xn, obj, inst, bc, ndof, node, reinterpret_cast<i32*>(P), dbg, flag);
    i32 f2 = 2;
    fun_140228b10(obj, inst, node, &f2, nr2, PI(0x140676748));
    L = sse::sqrt(nr2[0]);
    fun_140049b2d(x + 3 * (static_cast<i64>(n) - 1), obj, inst, bc, ndof, node, P, &L, dbg, flag, sep);
    tolS = *tol;
    if (*tol <= 9.999999717180685e-10) { // DAT_140bad530 (REAL*4 1e-9); NaN keeps tol
        tolS = L;
    }
    tol2 = tolS * 0.20000000298023224; // DAT_140bada60
    fun_14004522c(obj, inst, x + 3 * (static_cast<i64>(*node) - 1), bc_row(bc, ld, *node), node,
                  PI(0x140676770), &tol2);
    if (*move == 1) {
        double* xw = x + 3 * (static_cast<i64>(n) - 1);
        xw[0] = P[0];
        xw[1] = P[1];
        xw[2] = P[2];
    }
    if (*dbg == 1) {
        // dead in GENCTC (dbg is cleared above and no callee sets it)
        const int u = msg_unit();
        fio::Write(u).a("Before IODIE2").end();
        const i32* row = bc_row(bc, ld, n);
        fio::Write(u).i(row[0]).i(row[1]).i(row[2]).end();
        const double* xw = x + 3 * (static_cast<i64>(n) - 1);
        fio::Write(u).d(xw[0]).d(xw[1]).d(xw[2]).end();
    }
    fun_140040d77(P, obj, inst, bc, ndof, node, dbg, flag, &tolS, Pn, codes, mode, &nacc, sep);
    if (*move == 1 && nacc != 0) {
        double* xw = x + 3 * (static_cast<i64>(n) - 1);
        fun_14005266d(obj, inst, xw, Pn, codes, node);
        *flag = -1;
        i32* row = bc_row(bc, ld, n);
        row[0] = codes[0];
        row[2] = codes[2];
        row[1] = codes[1];
        // str 1405c7930: 1001('Node ',I7,' Old coord. ',3F15.7,/,        12X,      ' New coord. ',3F15.7)
        fio::Write(msg_unit(), "('Node ',I7,' Old coord. ',3F15.7,/,        12X,      ' New coord. ',3F15.7)")
            .i(n)
            .d(xw[0])
            .d(xw[1])
            .d(xw[2])
            .d(Pn[0])
            .d(Pn[1])
            .d(Pn[2])
            .end();
        if (I4(0x1405c16f4) == 0) {
            xw[0] = Pn[0];
            xw[1] = Pn[1];
            xw[2] = Pn[2];
        }
        if (*dbg == 1) {
            const int u = msg_unit();
            fio::Write(u).a("After IODIE2").end();
            fio::Write(u).i(row[0]).i(row[1]).i(row[2]).end();
            fio::Write(u).d(xw[0]).d(xw[1]).d(xw[2]).end();
        }
    }
}

// @src 14004946f  constraint rows NR4 and code table NI4 of the node from its BCCDEF codes
void fun_14004946f(double* x, i32* obj, i32* inst, i32* bc, i32* ndof, i32* node, i32* p7, i32* /*dbg*/,
                   i32* flag)
{
    double* P = reinterpret_cast<double*>(p7); // REAL*8 P(3) (typed INTEGER in the shared header)
    i32& nd = I4(0x14066fbc8);
    i32& r = I4(0x14066fbd0);
    i32& c = I4(0x14066fc00);
    i32* I = PI(0x1405c7000);   // NI4: code(1..3), poly(1..3)
    double* M = PD(0x14066fc10); // NR4: M(c, r), column r = direction r
    double* nrm = PD(0x14066fc70);
    auto e = [](i32 cc, i32 rr) -> double& { return R8(0x1405c7000ull + 8ull * static_cast<u64>(cc + 3 * rr)); };
    auto Mcr = [](i32 cc, i32 rr) -> double& { return R8(0x14066fbf0ull + 8ull * static_cast<u64>(cc + 3 * rr)); };

    nd = *node;
    I4(0x14066fbcc) = 4;
    *flag = 0;
    const i64 ld = ld_of(ndof);
    for (r = 1; r <= 3; ++r) {
        const double v = x[r - 1];
        c = 1;
        I[r - 1] = 0;
        I[r + 2] = 0;
        R8(0x14066fbd8ull + 8ull * static_cast<u64>(r)) = v;
        P[r - 1] = v;
        for (; c <= 3; ++c) {
            Mcr(c, r) = 0.0;
        }
    }
    i32 f4 = 4;
    i32 n9 = 9;
    fun_140228980(obj, inst, &nd, &f4, M, &n9);
    i32 n6 = 6;
    fun_140228690(obj, inst, &nd, &f4, I, &n6);
    i32 f1 = 1;
    fun_140228b10(obj, inst, &nd, &f1, nrm, PI(0x14066fc90));
    i32* row = bc_row(bc, ld, nd);
    i32& sum = I4(0x14066fc94);
    sum = static_cast<i32>(static_cast<std::uint32_t>(iabs(row[2])) + static_cast<std::uint32_t>(iabs(row[0])) +
                           static_cast<std::uint32_t>(iabs(row[1])));
    if (sum == 0) {
        return;
    }
    i32& any = I4(0x14066fc98);
    i32& rr = I4(0x14066fc9c);
    i32& cc = I4(0x14066fcb0);
    i32& k = I4(0x14066fcb4);
    any = 0;
    for (rr = 1; rr <= 3; ++rr) {
        const i32 code = row[rr - 1];
        if (code == 1) {
            double& s = R8(0x14066fca0);
            double& sg = R8(0x14066fca8);
            s = dot_l(&e(1, rr), nrm);
            sg = -1.0; // DAT_140bad880
            if (s < 0.0) {
                sg = 1.0; // DAT_140bad138
            }
            any = 1;
            I[rr - 1] = 1;
            for (cc = 1; cc <= 3; ++cc) {
                Mcr(cc, rr) = sse::mul(e(cc, rr), sg);
            }
        } else if (code > 600) {
            any = 1;
            k = 1;
            if (I4(0x14076abb8) >= 1) {
                do {
                    if (I4(0x14076aa24ull + 4ull * static_cast<u64>(k)) == code) {
                        I[rr - 1] = code;
                        for (cc = 1; cc <= 3; ++cc) {
                            Mcr(cc, rr) = fneg_b(R8(0x14076a0a8ull + 8ull * static_cast<u64>(cc + 3 * k)));
                        }
                        I[rr + 2] = k;
                    }
                    ++k;
                } while (k <= I4(0x14076abb8));
            }
        }
    }
    fun_140228980(obj, inst, node, &f4, M, &n9);
    fun_140228690(obj, inst, node, &f4, I, &n6);
    if (any != 0) {
        fun_14004e10c(obj, inst, node, bc_row(bc, ld, *node));
    }
}

// @src 14004e10c  node normal of a constrained node by its feature class NI5 (sec. 4.2)
void fun_14004e10c(i32* obj, i32* inst, i32* node, i32* c)
{
    i32& cls = I4(0x140675094);
    double* nrm = PD(0x1406750a0);
    double* t = PD(0x140675140);
    i32& r = I4(0x140675138);
    auto Mcol = [](i32 kk) { return PD(0x1406750b8ull + 24ull * static_cast<u64>(static_cast<i64>(kk))); };

    i32 f = 5;
    fun_140228830(obj, inst, node, &f, &cls, PI(0x140675098));
    if (cls == 0) {
        return;
    }
    f = 1;
    fun_140228b10(obj, inst, node, &f, nrm, PI(0x1406750c0));
    f = 4;
    fun_140228b10(obj, inst, node, &f, PD(0x1406750d0), PI(0x140675130));
    if (cls == 1) {
        i32& k = I4(0x140675134);
        k = 0;
        for (r = 1; r <= 3; ++r) {
            if (c[r - 1] > 599 || c[r - 1] == 1) {
                k = r;
            }
        }
        double* mk = Mcol(k);
        def::rt::geoutl::fun_1404cce5e(nrm, mk, t, PD(0x140675160));
        def::rt::geoutl::fun_1404cce5e(mk, t, nrm, PD(0x140675168));
    }
    if (cls == 3) {
        i32& cnt = I4(0x140675170);
        cnt = 0;
        for (r = 1; r <= 3; ++r) {
            if (c[r - 1] > 599 || c[r - 1] == 1) {
                ++cnt;
                I4(0x14067517cull + 4ull * static_cast<u64>(cnt)) = r;
            }
        }
        if (cnt != 2) {
            return;
        }
        I4(0x140675190) = I4(0x140675180);
        I4(0x140675194) = I4(0x140675184);
        def::rt::geoutl::fun_1404cce5e(Mcol(I4(0x140675180)), Mcol(I4(0x140675184)), t, PD(0x140675160));
        double& s = R8(0x140675198);
        double& sg = R8(0x1406751a0);
        s = dot_l(t, nrm);
        sg = 1.0;
        if (s < 0.0) {
            sg = -1.0;
        }
        nrm[0] = sse::mul(sg, t[0]);
        nrm[1] = sse::mul(sg, t[1]);
        nrm[2] = sse::mul(sg, t[2]);
    }
    if (c[0] == 1) {
        nrm[0] = 0.0;
    }
    if (c[1] == 1) {
        nrm[1] = 0.0;
    }
    if (c[2] == 1) {
        nrm[2] = 0.0;
    }
    f = 1;
    fun_140228980(obj, inst, node, &f, nrm, PI(0x1406750c0));
}

// @src 14005266d  final correction of the projected point Pn for fixed / symmetry directions
void fun_14005266d(i32* obj, i32* inst, double* x, double* pn, i32* codes, i32* node)
{
    using def::rt::geoutl::fun_1404d68dd;
    i32& nd = I4(0x1406767b4);
    i32& nfix = I4(0x1406767bc);
    i32& nneg = I4(0x1406767c0);
    i32& r = I4(0x1406767c4);
    auto fix = [](i32 j) -> i32& { return I4(0x1406767ccull + 4ull * static_cast<u64>(j)); };
    auto neg = [](i32 j) -> i32& { return I4(0x1406767dcull + 4ull * static_cast<u64>(j)); };
    double* nrm = PD(0x1406767f0);
    auto Mcol = [](i32 kk) { return PD(0x140676808ull + 24ull * static_cast<u64>(static_cast<i64>(kk))); };
    double* t = PD(0x1406768c0);
    double& s = R8(0x1406768e8);
    double& sg = R8(0x1406768f8);

    nd = *node;
    I4(0x1406767b8) = 0;
    nfix = 0;
    nneg = 0;
    for (r = 1; r <= 3; ++r) {
        const i32 v = codes[r - 1];
        if (v == 1 || v > 600) {
            ++nfix;
            fix(nfix) = r;
        } else if (v < 0) {
            ++nneg;
            neg(nneg) = r;
        }
    }
    if (!(nneg > 0 && nfix > 0)) {
        return;
    }
    i32 f = 1;
    fun_140228b10(obj, inst, node, &f, nrm, PI(0x140676810));
    f = 4;
    fun_140228b10(obj, inst, &nd, &f, PD(0x140676820), PI(0x140676880));
    f = 4;
    fun_140228830(obj, inst, &nd, &f, PI(0x140676890), PI(0x1406768b0));
    if (nfix == 2 && nneg == 1) {
        I4(0x1406768b4) = fix(1);
        I4(0x1406768b8) = fix(2);
        fun_140066ff1(Mcol(fix(1)), Mcol(fix(2)), t, PD(0x1406768e0));
        s = dot_l(t, nrm);
        sg = 1.0;
        I4(0x1406768f0) = neg(1);
        if (s < 0.0) {
            sg = -1.0;
        }
        double* mn = Mcol(neg(1));
        mn[0] = sse::mul(sg, t[0]);
        mn[1] = sse::mul(sg, t[1]);
        f = 4;
        mn[2] = sse::mul(sg, t[2]);
        fun_140228980(obj, inst, &nd, &f, PD(0x140676820), PI(0x140676880));
    }
    if (nfix == 1 && nneg == 1) {
        I4(0x1406768b4) = fix(1);
        I4(0x1406768b8) = neg(1);
        double* w = PD(0x140676900);
        fun_140066ff1(Mcol(fix(1)), Mcol(neg(1)), t, PD(0x1406768e0));
        fun_140066ff1(Mcol(fix(1)), t, w, PD(0x140676920));
        s = dot_l(w, nrm);
        sg = 1.0;
        if (s < 0.0) {
            sg = -1.0;
        }
        nrm[0] = sse::mul(sg, w[0]);
        nrm[1] = sse::mul(sg, w[1]);
        nrm[2] = sse::mul(sg, w[2]);
    }
    if (nfix == 3) {
        pn[0] = x[0];
        pn[1] = x[1];
        pn[2] = x[2];
    }
    double* v = PD(0x140676930);
    if (nfix == 2) {
        v[0] = sse::sub(pn[0], x[0]);
        v[1] = sse::sub(pn[1], x[1]);
        v[2] = sse::sub(pn[2], x[2]);
        s = dot_l(v, nrm);
        pn[0] = sse::add(sse::mul(nrm[0], s), x[0]);
        pn[1] = sse::add(sse::mul(nrm[1], s), x[1]);
        pn[2] = sse::add(sse::mul(nrm[2], s), x[2]);
    }
    if (nfix == 1) {
        const i32 f1 = fix(1);
        i32& kp = I4(0x140676950);
        kp = I4(0x140676898ull + 4ull * static_cast<u64>(f1)); // I.poly(f1)
        I4(0x1406768b4) = f1;
        const double* X = PD(0x140769750ull + 24ull * static_cast<u64>(static_cast<i64>(kp))); // plane point k
        v[0] = sse::sub(pn[0], X[0]);
        v[1] = sse::sub(pn[1], X[1]);
        v[2] = sse::sub(pn[2], X[2]);
        const double* mf = Mcol(f1);
        s = dot_l(v, mf);
        pn[0] = sse::sub(pn[0], sse::mul(mf[0], s));
        pn[1] = sse::sub(pn[1], sse::mul(mf[1], s));
        pn[2] = sse::sub(pn[2], sse::mul(mf[2], s));
    }
    if (codes[0] == 1) {
        pn[0] = x[0];
    }
    if (codes[1] == 1) {
        pn[1] = x[1];
    }
    if (codes[2] == 1) {
        pn[2] = x[2];
    }
}

} // namespace def::cmd::ctc3d
