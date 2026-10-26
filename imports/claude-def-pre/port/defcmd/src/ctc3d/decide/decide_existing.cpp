// GENCTC 3D: contacts the node already has (packet p67-7, group "decide"): FUN_140049b2d and its
// callees FUN_14004ce7a, FUN_14004c429, FUN_14004c564, FUN_14005ee88 (CRVPLY), FUN_14004ad46,
// FUN_1400446b4, FUN_14004bc99, FUN_1400483ed. GENCTC_3d_decide.md sec. 4.3, 4.4. Every local of
// these Fortran routines is a SAVE static at its binary address.
#include "decide_util.hpp"

#include <defcmd/ctc3d/cube.hpp>
#include <defcmd/ctc3d/decide.hpp>
#include <defcmd/ctc3d/search.hpp>
#include <defrt/ftn/fio.hpp>
#include <defrt/ftn/geoutl.hpp>
#include <defrt/ftn/sse_ops.hpp>

#include <cstdlib>

namespace def::cmd::ctc3d {

using namespace decide_detail;
namespace fio = def::rt::ftn::fio;
namespace sse = def::rt::sse;
namespace geo = def::rt::geoutl;

// @src 140049b2d  existing contacts: re-project onto the contact object, release or change it
void fun_140049b2d(double* x, i32* obj, i32* inst, i32* bc, i32* ndof, i32* node, double* p, double* len,
                   i32* dbg, i32* flag, i32* sep)
{
    i32& nd = I4(0x14066fcb8);
    i32* I = PI(0x14066fd30); // NI4: code(r) at fd2c+4r, poly(r) at fd38+4r
    auto code = [](i32 rr) -> i32& { return I4(0x14066fd2cull + 4ull * static_cast<u64>(rr)); };
    auto ipoly = [](i32 rr) -> i32& { return I4(0x14066fd38ull + 4ull * static_cast<u64>(rr)); };
    auto Mcol = [](i32 rr) { return PD(0x14066fca8ull + 24ull * static_cast<u64>(rr)); };
    double* nrm = PD(0x14066fd60); // NR1 (4)
    double* nr5 = PD(0x14066fd90);
    double& off = R8(0x14066fda8);
    i32& nfix = I4(0x14066fdb0);
    i32& ifix = I4(0x14066fdb4);
    i32& nneg = I4(0x14066fdb8);
    i32& r = I4(0x14066fdbc);
    double* mfix = PD(0x14066fdc0);
    i32& rdup = I4(0x14066fde0);
    i32& dupobj = I4(0x14066fde4);
    i32& pobj = I4(0x14066fde8);
    i32& ndup = I4(0x14066fdec);
    i32& pos = I4(0x14066fdf0);
    i32& e = I4(0x14066fdf4);
    double* lcdup = PD(0x14066fe00);
    i32& npoly = I4(0x14066fe20);
    i32& erf = I4(0x14066fe24);
    i32& poly = I4(0x14066fe28);
    double* lc = PD(0x140670de0);
    double& d = R8(0x140670e00);
    double* Q = PD(0x140670e10);

    nd = *node;
    const i64 ld = ld_of(ndof);
    *flag = 0;
    I4(0x14066fcbc) = 4;
    {
        const i32* row = bc_row(bc, ld, *node);
        if (!(row[0] < 0 || row[1] < 0 || row[2] < 0)) {
            return;
        }
    }
    i32 f = 4;
    fun_140228b10(obj, inst, &nd, &f, PD(0x14066fcc0), PI(0x14066fd20));
    f = 4;
    fun_140228830(obj, inst, &nd, &f, I, PI(0x14066fd50));
    f = 1;
    fun_140228b10(obj, inst, &nd, &f, nrm, PI(0x14066fd80));
    f = 5;
    fun_140228b10(obj, inst, &nd, &f, nr5, PI(0x14066fda0));
    off = sse::mul(sse::add(nr5[0], nr5[1]), 0.5); // DAT_140bad260
    if (off <= 0.0) {
        off = 0.0;
    }
    nfix = 0;
    ifix = 0;
    nneg = 0;
    for (r = 1; r <= 3; ++r) {
        const i32 v = bc_row(bc, ld, nd)[r - 1];
        if (v == 1 || v > 600) {
            const double* mc = Mcol(r);
            mfix[0] = fneg_b(mc[0]);
            ++nfix;
            mfix[1] = fneg_b(mc[1]);
            ifix = r;
            mfix[2] = fneg_b(mc[2]);
        } else if (v < 0) {
            ++nneg;
        }
    }
    rdup = 0;
    dupobj = 0;
    pobj = 0;
    p[2] = x[2];
    p[1] = x[1];
    p[0] = x[0];
    const i32 ifix0 = ifix; // local_80 (read once before the loop)
    for (r = 1; r <= 3; ++r) {
        i32* row = bc_row(bc, ld, nd);
        if (row[r - 1] >= 0) {
            continue;
        }
        i32 rl = r;
        fun_14004c429(row, &dupobj, &ndup, &rl, &pos);
        if (ndup > 0 && pos == 1) {
            rdup = r;
        }
        if (ndup < 1 || pos < 2) {
            pobj = ineg(row[r - 1]);
            if (*obj == pobj) {
                continue;
            }
            e = inst_of(pobj);
            if (e <= 0) {
                continue;
            }
            npoly = npoly_of(pobj, e);
            erf = 3;
            poly = 0;
            if (nneg > 1) {
                nrm[2] = 0.0;
                off = -0.05999999865889549; // DAT_140bad9b0
                nrm[1] = 0.0;
                nrm[0] = 0.0;
            }
            f = 1;
            fun_140228830(obj, inst, &nd, &f, PI(0x14066fe30), PI(0x140670dd0));
            fun_14004ce7a(obj, inst, bc, ndof, &pobj, PI(0x14066fe30), PI(0x140670dd0), nrm);
            nrm[3] = off;
            I4(0x140670dd4) = ineg(*obj);
            fun_14006798b(&pobj, &e, &erf, &npoly, p, lc, &poly, &d, PI(0x140670e08), Q, nrm, PI(0x140670dd4), dbg);
            double ad = fabs_b(d);
            if (ad > sse::mul(*len, 2.0)) { // DAT_140bad4d0
                i32 zero = 0;
                fun_14006798b(&pobj, &e, &erf, &npoly, p, PD(0x140670e30), PI(0x140670e50), PD(0x140670e58),
                              PI(0x140670e08), PD(0x140670e60), PD(0x1405c70b0), &zero, dbg);
                const double ad2 = fabs_b(R8(0x140670e58));
                if (ad2 < ad) {
                    i32& k = I4(0x140670e80);
                    for (k = 1; k <= 3; ++k) {
                        Q[k - 1] = R8(0x140670e58ull + 8ull * static_cast<u64>(k));
                        lc[k - 1] = R8(0x140670e28ull + 8ull * static_cast<u64>(k));
                    }
                    poly = I4(0x140670e50);
                    d = R8(0x140670e58);
                    ad = ad2;
                }
            }
            if (ad > sse::mul(*len, 5.0)) { // DAT_140bad498; NaN keeps the contact
                code(r) = 0;
                ipoly(r) = 0;
                bc_row(bc, ld, nd)[r - 1] = 0;
                fio::Write(msg_unit())
                    .a("Release Contact Boundary Condition of")
                    .i(*node)
                    .a("from die ")
                    .i(pobj)
                    .a(" because of large distance")
                    .end();
                continue;
            }
            I4(0x140670e84) = r;
            fun_14004ad46(&pobj, &e, obj, inst, p, Q, lc, &poly, bc, ndof, PI(0x140670e84), node, len, sep);
            if (pobj == 0) {
                continue;
            }
            e = inst_of(pobj);
            fun_1400483ed(&pobj, &e, Mcol(r), &poly);
            if (nneg == 1 && nfix == 0) {
                fun_1400483ed(&pobj, &e, nrm, &poly);
                nrm[0] = fneg_b(nrm[0]);
                nrm[1] = fneg_b(nrm[1]);
                nrm[2] = fneg_b(nrm[2]);
                f = 1;
                fun_140228980(obj, inst, &nd, &f, nrm, PI(0x14066fd80));
            }
            code(r) = ineg(pobj);
            ipoly(r) = poly;
            row = bc_row(bc, ld, nd);
            if (row[0] != 1) {
                p[0] = Q[0];
            }
            if (row[1] != 1) {
                p[1] = Q[1];
            }
            if (row[2] != 1) {
                p[2] = Q[2];
            }
            if (pobj == dupobj && r == rdup) {
                lcdup[2] = lc[2];
                lcdup[1] = lc[1];
                lcdup[0] = lc[0];
            }
            if (ifix > 0 && row[ifix0 - 1] > 600) {
                double* v = PD(0x140670e90);
                double& s = R8(0x140670eb0);
                if (nfix == 1) {
                    i32& kp = I4(0x140670e88);
                    kp = I4(0x14066fd38ull + 4ull * static_cast<u64>(ifix0)); // I.poly(ifix)
                    const double* X = PD(0x140769750ull + 24ull * static_cast<u64>(static_cast<i64>(kp)));
                    v[0] = sse::sub(p[0], X[0]);
                    v[1] = sse::sub(p[1], X[1]);
                    v[2] = sse::sub(p[2], X[2]);
                    s = dot_l(v, mfix);
                    p[0] = sse::sub(p[0], sse::mul(mfix[0], s));
                    p[1] = sse::sub(p[1], sse::mul(mfix[1], s));
                    p[2] = sse::sub(p[2], sse::mul(mfix[2], s));
                }
                if (nfix == 2) {
                    v[0] = sse::sub(p[0], x[0]);
                    v[1] = sse::sub(p[1], x[1]);
                    v[2] = sse::sub(p[2], x[2]);
                    s = dot_l(v, nrm);
                    p[0] = sse::add(sse::mul(nrm[0], s), x[0]);
                    p[1] = sse::add(sse::mul(nrm[1], s), x[1]);
                    p[2] = sse::add(sse::mul(nrm[2], s), x[2]);
                }
            }
        } else {
            e = inst_of(dupobj);
            i32 rl2 = r;
            fun_14004c564(obj, inst, &dupobj, &e, &rdup, &rl2, lcdup, I, PD(0x14066fcc0));
        }
    }
    i32 n9 = 9;
    f = 4;
    fun_140228980(obj, inst, &nd, &f, PD(0x14066fcc0), &n9);
    i32 n6 = 6;
    f = 4;
    fun_140228690(obj, inst, &nd, &f, I, &n6);
}

// @src 14004ce7a  contact normal: area-weighted ER1 of the node's polygons whose common code is -p;
// list entries of the other polygons are negated
void fun_14004ce7a(i32* obj, i32* inst, i32* bc, i32* ndof, i32* p, i32* list, i32* cnt, double* nrm)
{
    i32& j = I4(0x140674e50);
    i32& q = I4(0x140674e54);
    double* acc = PD(0x140674e30);
    j = 1;
    acc[2] = 0.0;
    acc[1] = 0.0;
    acc[0] = 0.0;
    if (*cnt > 0) {
        const i32 pv = *p;
        do {
            q = list[j - 1];
            fun_140227e00(obj, inst, &q, PI(0x140674e60), PI(0x140674e70));
            fun_14003a978(bc, PI(0x140674e60), PI(0x140674e70), ndof, PI(0x140674e74), PI(0x140674e78));
            if (I4(0x140674e74) == ineg(pv)) {
                i32 f = 2;
                fun_140228540(obj, inst, &q, &f, PD(0x140674e80), PI(0x140674e88));
                f = 1;
                fun_140228540(obj, inst, &q, &f, PD(0x140674e90), PI(0x140674eb0));
                i32& k = I4(0x140674eb4);
                const double area = R8(0x140674e80);
                for (k = 1; k <= 3; ++k) {
                    acc[k - 1] = sse::add(sse::mul(R8(0x140674e88ull + 8ull * static_cast<u64>(k)), area), acc[k - 1]);
                }
            } else {
                list[j - 1] = ineg(list[j - 1]);
            }
            ++j;
        } while (j <= *cnt);
    }
    const double xx = sse::mul(acc[0], acc[0]);
    const double yy = sse::mul(acc[1], acc[1]);
    const double zz = sse::mul(acc[2], acc[2]);
    const double l = sse::sqrt(sse::add(zz, sse::add(yy, xx)));
    if (l > 9.99999993922529e-09) { // DAT_140bad7f8
        const double a = sse::div(acc[0], l);
        const double b = sse::div(acc[1], l);
        const double c = sse::div(acc[2], l);
        nrm[0] = a;
        nrm[1] = b;
        nrm[2] = c;
    }
}

// @src 14004c429  equal negative codes in the pairs (x,y), (y,z), (x,z): count, object, position of r
void fun_14004c429(i32* c, i32* obj, i32* ndup, i32* r, i32* pos)
{
    const i32 c1 = c[0];
    const i32 c2 = c[1];
    *ndup = 0;
    *pos = 0;
    if (c1 == c2 && c1 < 0) {
        *ndup = 1;
        *obj = ineg(c1);
        if (*r == 1) {
            *pos = 1;
        }
        if (*r == 2) {
            *pos = 2;
        }
    }
    const i32 c3 = c[2];
    if (c2 == c3 && c2 < 0) {
        *ndup = *ndup + 1;
        *obj = ineg(c2);
        if (*r == 2) {
            *pos = 1;
        }
        if (*r == 3) {
            *pos = 2;
        }
    }
    if (c1 == c3 && c3 < 0) {
        *ndup = *ndup + 1;
        *obj = ineg(c3);
        if (*r == 1) {
            *pos = 1;
        }
        if (*r == 3) {
            *pos = 2;
        }
    }
    if (*ndup != 2) {
        return;
    }
    *pos = *r;
}

// @src 14004c564  second direction *r with the same contact object as direction *rdup
void fun_14004c564(i32* /*obj*/, i32* /*inst*/, i32* p, i32* e, i32* rdup, i32* r, double* lc, i32* ni4,
                   double* nr4)
{
    i32& q = I4(0x140674040);
    i32* ei1 = PI(0x140674050);
    i32* conn = PI(0x140674080);
    i32& nv = I4(0x140674090);
    i32& nvv = I4(0x140674094);
    i32& k = I4(0x140674098);
    auto connk = [](i32 j) -> i32& { return I4(0x14067407cull + 4ull * static_cast<u64>(static_cast<i64>(j))); };
    auto ei1k = [](i32 j) -> i32& { return I4(0x14067404cull + 4ull * static_cast<u64>(static_cast<i64>(j))); };

    i32 f = 1;
    i32* dupcol = ni4 + *rdup; // [-1] = code(rdup), [2] = poly(rdup)
    q = ni4[*rdup + 2];
    fun_140228260(p, e, &q, &f, ei1, PI(0x140674070));
    fun_140227e00(p, e, &q, conn, &nv);
    nvv = nv;
    if (iabs(connk(nv)) == iabs(conn[0])) {
        nvv = nv - 1;
    }
    if (nvv == 4) {
        geo::fun_1404d3e13(lc, lc + 1, &k);
    } else {
        geo::fun_1404e37c4(lc, lc + 1, &k);
    }
    if (k < 1) {
        if (k == 0) {
            const i32 rr = *r;
            ni4[rr + 2] = 0;
            ni4[rr - 1] = 0;
            return;
        }
        // corner: CRVPLY gives the polygon and edge to use
        i32& cn = I4(0x1406740a4);
        cn = iabs(connk(iabs(k)));
        fun_14005ee88(p, e, &cn, PI(0x1406740a8), &q, &k);
        dupcol[-1] = ineg(*p);
        dupcol[2] = q;
        f = 1;
        fun_140228260(p, e, &q, &f, ei1, PI(0x140674070));
        f = 1;
        fun_140228540(p, e, &q, &f, nr4 + 3 * (static_cast<i64>(*rdup) - 1), PI(0x1406740a0));
    }
    const i32 rr = *r;
    i32& nb = I4(0x14067409c);
    nb = ei1k(k);
    f = 1;
    ni4[rr - 1] = ineg(*p);
    ni4[rr + 2] = nb;
    fun_140228540(p, e, &nb, &f, nr4 + 3 * (static_cast<i64>(rr) - 1), PI(0x1406740a0));
}

// @src 14005ee88  CRVPLY: along the curves through corner node *node, the valid polygon and its
// edge *edge that continue curve *crv (WRITE 'SKIPPING CRV #:' for curve numbers out of range)
void fun_14005ee88(i32* obj, i32* inst, i32* node, i32* crv, i32* poly, i32* edge)
{
    // ALLOCATABLE INTEGER buf(6, n): PathScale dope vector at 140694c10
    auto& dv_ptr = m<std::uint64_t>(0x140694c10);
    i32& n = I4(0x140694c70);
    n = I4(0x140779624ull + 4ull * static_cast<u64>(static_cast<i64>(*obj) + static_cast<i64>(*inst) * 100));
    const i64 ext = n < 0 ? 0 : static_cast<i64>(n);
    m<std::uint64_t>(0x140694c18) = 0x20;
    I4(0x140694c24) = 0x40000000;
    m<std::uint64_t>(0x140694c28) = 0x2000200000000ull;
    m<std::uint64_t>(0x140694c30) = 0;
    m<std::uint64_t>(0x140694c38) = 0;
    I4(0x140694c20) = 0x18;
    m<std::int64_t>(0x140694c40) = 1;
    m<std::int64_t>(0x140694c48) = 6;
    m<std::int64_t>(0x140694c50) = 1;
    m<std::int64_t>(0x140694c58) = 1;
    m<std::int64_t>(0x140694c68) = 6;
    m<std::int64_t>(0x140694c60) = ext;
    // 1404b62c4: ALLOCATE (runtime; contents undefined)
    void* mem = std::malloc(static_cast<std::size_t>(ext * 24 > 0 ? ext * 24 : 1));
    dv_ptr = reinterpret_cast<std::uint64_t>(mem);
    m<std::int64_t>(0x140694c38) = ext * 0xc0;
    m<std::uint64_t>(0x140694c30) = dv_ptr;
    *edge = 0;
    I4(0x140694c20) = static_cast<i32>((static_cast<std::uint32_t>(I4(0x140694c20)) & 0xfffffffeu) | (mem ? 1u : 0u));
    *poly = 0;
    I4(0x140694c80) = 0;
    auto dealloc = [&]() {
        // 1404b6d74: DEALLOCATE
        std::free(reinterpret_cast<void*>(dv_ptr));
        dv_ptr = 0;
        I4(0x140694c20) = static_cast<i32>(static_cast<std::uint32_t>(I4(0x140694c20)) & 0xfffffffeu);
    };
    i32 f = 6;
    i32& cnt = I4(0x140695c20);
    fun_140228830(obj, inst, node, &f, PI(0x140694c80), &cnt);
    if (cnt < 2 && I4(0x140694c80) == 0) {
        dealloc();
        return;
    }
    i32& i = I4(0x140695c24);
    for (i = 1; i <= cnt; ++i) {
        const i32 c = I4(0x140694c7cull + 4ull * static_cast<u64>(i));
        if (c <= 0) {
            continue;
        }
        const i32 np = npoly_of(*obj, *inst);
        *crv = c;
        if (np < c) {
            fio::Write(msg_unit()).a("SKIPPING CRV #:").i(*crv).a("IN CRVPLY").end();
            continue;
        }
        f = 5;
        i32* buf = reinterpret_cast<i32*>(dv_ptr);
        fun_140228260(obj, inst, crv, &f, buf, PI(0x140695c28));
        const i32 nrec = I4(0x140695c28) / 6;
        I4(0x140695c2c) = nrec;
        i32& j = I4(0x140695c30);
        for (j = 1; j <= nrec; ++j) {
            const i64 b = (static_cast<i64>(j) - m<std::int64_t>(0x140694c58)) * m<std::int64_t>(0x140694c48) -
                          m<std::int64_t>(0x140694c40);
            buf = reinterpret_cast<i32*>(dv_ptr);
            const i32 r1 = buf[b + 2];
            if (r1 == 0 || !(*node == buf[b + 1] || *node == r1)) {
                continue;
            }
            f = 2;
            *poly = iabs(buf[b + 3]);
            fun_140228260(obj, inst, poly, &f, PI(0x140695c34), PI(0x140695c38));
            i32& p2 = I4(0x140695c3c);
            p2 = buf[(static_cast<i64>(j) - m<std::int64_t>(0x140694c58)) * m<std::int64_t>(0x140694c48) -
                     m<std::int64_t>(0x140694c40) + 4];
            f = 2;
            fun_140228260(obj, inst, &p2, &f, PI(0x140695c40), PI(0x140695c38));
            if (I4(0x140695c34) != 1 || I4(0x140695c40) != 1) {
                continue;
            }
            f = 6;
            fun_140228260(obj, inst, poly, &f, PI(0x140695c50), PI(0x140695c70));
            f = 1;
            fun_140228260(obj, inst, poly, &f, PI(0x140695c80), PI(0x140695ca0));
            const i32 c70 = I4(0x140695c70);
            const i32 half = static_cast<i32>((static_cast<std::uint32_t>(c70) >> 31) + static_cast<std::uint32_t>(c70)) >> 1;
            I4(0x140695ca4) = half;
            i32& mm = I4(0x140695ca8);
            for (mm = 1; mm <= half; ++mm) {
                if (I4(0x140695c48ull + 8ull * static_cast<u64>(mm)) == *crv) {
                    const bool hit = I4(0x140695c7cull + 4ull * static_cast<u64>(mm)) == p2;
                    *edge = mm;
                    if (hit) {
                        dealloc();
                        return;
                    }
                }
            }
        }
    }
    dealloc();
}

// @src 14004ad46  separation / change of the contact object for direction *dir of object *p
void fun_14004ad46(i32* p, i32* e, i32* /*obj*/, i32* /*inst*/, double* x, double* q, double* lc, i32* poly,
                   i32* bc, i32* ndof, i32* dir, i32* node, double* len, i32* sep)
{
    const i32 n0 = *node;
    I4(0x140670eb8) = 0;
    i32& cntneg = I4(0x140670ebc);
    i32& r = I4(0x140670ec0);
    cntneg = 0;
    const i64 ld = ld_of(ndof);
    *sep = 0;
    for (r = 3; r > 0; --r) {
        if (bc_row(bc, ld, n0)[r - 1] < 0) {
            ++cntneg;
        }
        if (cntneg > 1) {
            return;
        }
    }
    double* u = PD(0x140670ed0);
    double& lu = R8(0x140670ef0);
    geo::fun_1404d68dd(x, q, u, &lu);
    I4(0x140670ef8) = 0;
    i32* conn = PI(0x140670f00);
    i32& nv = I4(0x140670f10);
    i32& k = I4(0x140670f18);
    auto connk = [](i32 j) -> i32& { return I4(0x140670efcull + 4ull * static_cast<u64>(static_cast<i64>(j))); };
    fun_140227e00(p, e, poly, conn, &nv);
    I4(0x140670f14) = nv;
    if (nv == 4 && conn[0] == iabs(conn[3])) {
        I4(0x140670f14) = 3;
        geo::fun_1404e37c4(lc, lc + 1, &k);
    } else if (nv == 4) {
        geo::fun_1404d3e13(lc, lc + 1, &k);
    } else {
        geo::fun_1404e37c4(lc, lc + 1, &k);
    }
    i32* ni7a = PI(0x140670f20);
    i32* ni7b = PI(0x140670f30);
    if (k < 0) {
        fun_1400446b4(p, e, poly, conn, &k, ni7a, ni7b);
    }
    if (k < 0) {
        I4(0x140670f40) = ineg(k);
        i32& cn = I4(0x140670f44);
        cn = iabs(connk(I4(0x140670f40)));
        fun_14004bc99(p, e, &cn, &k, poly, conn);
    }
    if (k < 1) {
        return;
    }
    i32& na = I4(0x140670f48);
    i32& kn = I4(0x140670f4c);
    i32& nb = I4(0x140670f50);
    na = connk(k);
    kn = k + 1;
    if (nv < kn) {
        kn = 1;
    }
    nb = connk(kn);
    i32 f = 7;
    fun_140228830(p, e, &na, &f, ni7a, PI(0x140670f54));
    f = 7;
    fun_140228830(p, e, &nb, &f, ni7b, PI(0x140670f54));
    if (ni7a[0] != 2) {
        return;
    }
    if (ni7b[0] != 2) {
        return;
    }
    i32& qobj = I4(0x140670f58);
    qobj = ni7a[1];
    fun_140227fc0(p, e, &na, PD(0x140670f60));
    fun_140227fc0(p, e, &nb, PD(0x140670f80));
    geo::fun_1404d68dd(PD(0x140670f60), PD(0x140670f80), PD(0x140670fa0), PD(0x140670fc0));
    f = 1;
    fun_140228540(p, e, poly, &f, PD(0x140670fd0), PI(0x140670ff0));
    fun_140066ff1(PD(0x140670fa0), PD(0x140670fd0), PD(0x140671000), PD(0x140671020));
    double& s = R8(0x140671028);
    s = dot_l(u, PD(0x140671000));
    if (ni7a[1] != ni7b[1]) {
        return;
    }
    if (qobj == 0) {
        return;
    }
    i32& e2 = I4(0x140671030);
    e2 = inst_of(qobj);
    i32 f3 = 3;
    i32& np2 = I4(0x140671044);
    np2 = npoly_of(qobj, e2);
    I4(0x140671040) = 0;
    double& d2 = R8(0x140671038);
    d2 = 1000000.0; // DAT_140bad320
    i32& poly2 = I4(0x140671070);
    fun_14006798b(&qobj, &e2, &f3, &np2, x, PD(0x140671050), &poly2, &d2, PI(0x140671074), PD(0x140671080),
                  PD(0x1405c70d0), PI(0x140671040), PI(0x140670ef8));
    R8(0x1406710a8) = 1.0;
    R8(0x1406710a0) = 1e-08; // DAT_140bada20
    if (poly2 > 0) {
        f = 1;
        fun_140228540(&qobj, &e2, &poly2, &f, PD(0x1406710b0), PI(0x140670ff0));
        R8(0x1406710a8) = dot_l(PD(0x140670fd0), PD(0x1406710b0));
        if (fabs_b(R8(0x1406710a8)) < 0.5) { // DAT_140bad260
            return;
        }
    }
    i32& sepf = I4(0x1406710d0);
    sepf = 0;
    if (d2 < lu && 1e-08 < d2) {
        sepf = 1;
    }
    if (d2 < sse::mul(*len, 0.30000001192092896) && s < -0.5) { // DAT_140bada28, DAT_140bad628
        sepf = 1;
    }
    if (sepf == 1) {
        bc_row(bc, ld, *node)[*dir - 1] = 0;
        // str 1405c7180
        fio::Write(6, "(' Info : Node separation is detected. Node number is ',I10)").i(*node).end();
        const i32 pv = *p;
        *p = 0;
        *sep = pv;
    }
    if (sepf != 0) {
        return;
    }
    if (1e-08 < d2) {
        return;
    }
    // str 1405c7270, 1405c73f0
    fio::Write(6, "(' Info : Node #',I7, 'Contact condition is changed')").i(*node).end();
    fio::Write(6, "(' Info : Node contact is moved from Poly # ',I7,        'of Object (',I3,'to Poly #', I7, 'of Object ',I3)")
        .i(*poly)
        .i(*p)
        .i(poly2)
        .i(qobj)
        .end();
    i32* row = bc_row(bc, ld, *node);
    i32& which = I4(0x1406710d4);
    which = 0;
    const i32 mq = ineg(qobj);
    bool found = false;
    if (row[0] == mq) {
        which = 1;
        found = true;
    }
    if (row[1] == mq) {
        which = 2;
        found = true;
    }
    if (row[2] == mq) {
        which = 3;
        found = true;
    }
    if (found) {
        i32 mp = ineg(*p);
        row[*dir - 1] = 0;
        // str 1405c7540
        fio::Write(6, "(' Info : Node #',I7, 'Contact condition ',I7,' is changed')").i(*node).i(mp).end();
    } else {
        row[*dir - 1] = mq;
    }
    *p = qobj;
    lc[0] = R8(0x140671050);
    *poly = poly2;
    q[0] = R8(0x140671080);
    q[1] = R8(0x140671088);
    lc[1] = R8(0x140671058);
    lc[2] = R8(0x140671060);
    q[2] = R8(0x140671090);
}

// @src 1400446b4  corner -k of polygon *poly: among the polygons around the corner node, the
// first valid one with an edge whose end nodes are both feature nodes (NI7 class 2)
void fun_1400446b4(i32* obj, i32* inst, i32* poly, i32* conn, i32* k, i32* ni7a, i32* ni7b)
{
    i32& c = I4(0x14066d2f8);
    i32* list = PI(0x14066d300);
    i32& nlist = I4(0x14066e2a0);
    i32& found = I4(0x14066e2a4);
    i32& altk = I4(0x14066e2a8);
    i32& altpoly = I4(0x14066e2ac);
    i32& i = I4(0x14066e2b0);
    i32& q = I4(0x14066e2b4);
    i32* cq = PI(0x14066e2c0);
    i32& nv = I4(0x14066e2d0);
    i32& nvv = I4(0x14066e304);
    i32& j = I4(0x14066e308);
    i32& l = I4(0x14066e30c);
    i32& a = I4(0x14066e310);
    i32& jn = I4(0x14066e314);
    i32& b = I4(0x14066e318);
    i32* na7 = PI(0x14066e330);
    i32* nb7 = PI(0x14066e350);
    auto cqk = [](i32 jj) -> i32& { return I4(0x14066e2bcull + 4ull * static_cast<u64>(static_cast<i64>(jj))); };

    i32 f = 1;
    c = conn[iabs(*k) - 1];
    fun_140228830(obj, inst, &c, &f, list, &nlist);
    found = 0;
    altk = 0;
    altpoly = 0;
    i = 1;
    if (nlist > 0) {
        do {
            q = list[i - 1];
            f = 2;
            fun_140228260(obj, inst, &q, &f, PI(0x14066e2b8), PI(0x14066e2bc));
            if (I4(0x14066e2b8) == 1) {
                fun_140227e00(obj, inst, &q, cq, &nv);
                const i32 last = cqk(nv);
                if (last >= 0) {
                    f = 6;
                    fun_140228260(obj, inst, &q, &f, PI(0x14066e2e0), PI(0x14066e300));
                    nvv = nv;
                    if (iabs(cq[0]) == iabs(last)) {
                        nvv = nv - 1;
                    }
                    j = 0;
                    l = 1;
                    if (nvv >= 1) {
                        for (; l <= nvv; ++l) {
                            if (c == iabs(cqk(l))) {
                                j = l;
                            }
                        }
                    }
                    if (j != 0) {
                        const i32 j0 = j;
                        a = iabs(cqk(j0));
                        jn = j0 + 1;
                        if (nvv < jn) {
                            jn = 1;
                        }
                        b = iabs(cqk(jn));
                        if (I4(0x14066e2e0ull + 8ull * static_cast<u64>(j0) - 8ull) != 0) { // EI6(2j-1)
                            altk = j0;
                            altpoly = q;
                            I4(0x14066e31c) = a;
                            I4(0x14066e320) = b;
                        }
                        f = 7;
                        fun_140228830(obj, inst, &a, &f, na7, PI(0x14066e340));
                        f = 7;
                        fun_140228830(obj, inst, &b, &f, nb7, PI(0x14066e340));
                        i32 fnd = j0;
                        if (na7[0] != 2 || nb7[0] != 2) {
                            j = j0 - 1;
                            if (j == 0) {
                                j = nvv;
                            }
                            jn = j + 1;
                            if (nvv < jn) {
                                jn = 1;
                            }
                            a = iabs(cqk(j));
                            f = 7;
                            b = iabs(cqk(jn));
                            fun_140228830(obj, inst, &a, &f, na7, PI(0x14066e340));
                            f = 7;
                            fun_140228830(obj, inst, &b, &f, nb7, PI(0x14066e340));
                            fnd = found;
                            if (na7[0] == 2 && nb7[0] == 2) {
                                fnd = j;
                            }
                        }
                        found = fnd;
                        if (found > 0) {
                            l = 1;
                            *k = found;
                            *poly = q;
                            for (; l <= 2; ++l) {
                                ni7a[l - 1] = na7[l - 1];
                                ni7b[l - 1] = nb7[l - 1];
                            }
                            l = 1;
                            for (; l <= nv; ++l) {
                                conn[l - 1] = cqk(l);
                            }
                            return;
                        }
                    }
                }
            }
            ++i;
        } while (i <= nlist);
        if (found > 0) {
            goto tail;
        }
    }
    f = 5;
    fun_140228830(obj, inst, &c, &f, PI(0x14066e360), PI(0x14066e364));
    if (I4(0x14066e360) == 3) {
        *k = ineg(*k);
    }
tail:
    if (*k < 0 && altk != 0) {
        *k = altk;
        f = 7;
        *poly = altpoly;
        fun_140228830(obj, inst, PI(0x14066e31c), &f, ni7a, PI(0x14066e340));
        f = 7;
        fun_140228830(obj, inst, PI(0x14066e320), &f, ni7b, PI(0x14066e340));
        fun_140227e00(obj, inst, poly, conn, &nv);
    }
    if (*k < 0) {
        *k = ineg(*k);
    }
}

// @src 14004bc99  corner node *node: through a neighbour feature node of the same object, the
// polygon of the node's NI1 list that contains that neighbour and the edge between them
void fun_14004bc99(i32* obj, i32* inst, i32* node, i32* k, i32* poly, i32* conn)
{
    i32* nb2 = PI(0x1406710e0);
    i32& nnb = I4(0x140673020);
    i32* c7 = PI(0x140673030);
    i32* pl = PI(0x140673050);
    i32& npl = I4(0x140673ff0);
    i32& i = I4(0x140673ff4);
    i32& nb = I4(0x140673ff8);
    i32* n7 = PI(0x140674000);
    i32& pf = I4(0x140674010);
    i32& j = I4(0x140674014);
    i32& qq = I4(0x140674018);
    i32& nc = I4(0x14067401c);
    i32& l = I4(0x140674020);

    i32 f = 2;
    fun_140228830(obj, inst, node, &f, nb2, &nnb);
    f = 7;
    fun_140228830(obj, inst, node, &f, c7, PI(0x140673040));
    if (c7[0] != 2 || c7[1] == 0) {
        return;
    }
    f = 1;
    fun_140228830(obj, inst, node, &f, pl, &npl);
    i = 1;
    if (nnb < 1) {
        return;
    }
    do {
        nb = nb2[i - 1];
        f = 7;
        fun_140228830(obj, inst, &nb, &f, n7, PI(0x140673040));
        if (n7[0] == 2 && c7[1] == n7[1]) {
            pf = 0;
            j = 1;
            if (npl < 1) {
                pf = 0;
                j = 1;
                return;
            }
            for (;;) {
                qq = pl[j - 1];
                fun_140227e00(obj, inst, &qq, conn, &nc);
                for (l = 1; l <= nc; ++l) {
                    if (nb == iabs(conn[l - 1])) {
                        pf = qq;
                    }
                }
                if (pf != 0) {
                    break;
                }
                ++j;
                if (npl < j) {
                    return;
                }
            }
            qq = pf;
            fun_140227e00(obj, inst, &qq, conn, &nc);
            const i32 qsel = qq;
            i32& nvv = I4(0x140674024);
            nvv = nc;
            if (iabs(conn[nc - 1]) == iabs(conn[0])) {
                nvv = nc - 1;
            }
            i32& ll = I4(0x140674028);
            ll = 1;
            if (nvv > 0) {
                i32 prev = 0;
                i32 next = 2;
                do {
                    i32& lnx = I4(0x14067402c);
                    i32& lpv = I4(0x140674030);
                    lnx = next;
                    if (nvv < next) {
                        lnx = 1;
                    }
                    lpv = prev;
                    if (prev < 1) {
                        lpv = nvv;
                    }
                    i32& va = I4(0x140674034);
                    i32& vb = I4(0x140674038);
                    va = iabs(conn[ll - 1]);
                    vb = iabs(conn[lnx - 1]);
                    i32& ed = I4(0x14067403c);
                    ed = 0;
                    if (*node == va && vb == nb) {
                        ed = ll;
                    }
                    if (va == nb && *node == vb) {
                        ed = lpv;
                    }
                    if (ed != 0) {
                        *k = ed;
                        *poly = qsel;
                        return;
                    }
                    ++ll;
                    ++prev;
                    ++next;
                } while (ll <= nvv);
            }
        }
        ++i;
    } while (i <= nnb);
}

// @src 1400483ed  contact normal of (obj, inst): ER1(poly) for poly >= 0, NR1(node -poly) else
void fun_1400483ed(i32* obj, i32* inst, double* nrm, i32* poly)
{
    i32 f = 1;
    double* buf = PD(0x14066fa20);
    if (*poly >= 0) {
        fun_140228540(obj, inst, poly, &f, buf, PI(0x14066fa40));
        nrm[0] = buf[0];
        nrm[1] = buf[1];
        nrm[2] = buf[2];
        return;
    }
    I4(0x14066fa10) = ineg(*poly);
    fun_140228b10(obj, inst, PI(0x14066fa10), &f, buf, PI(0x14066fa40));
    nrm[2] = buf[2];
    nrm[1] = buf[1];
    nrm[0] = buf[0];
}

} // namespace def::cmd::ctc3d
