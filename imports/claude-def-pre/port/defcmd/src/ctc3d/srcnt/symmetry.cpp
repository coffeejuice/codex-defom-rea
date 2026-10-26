// Symmetry planes of the SRCNT stage: SYMGRP2 14002a857, symmetry polygons 14003d2ee, the
// symmetry-die routines 14003dabe / 14005d44f (GENCTC_3d_search.md sec. 7.2 steps 3, 4, 7;
// docs/re/commands/oracle/ov-ctc.md corrections 4, 5). Packet p67-7, group "srcnt".
#include "srcnt_internal.hpp"

#include <defcmd/ctc3d/cube.hpp>
#include <defcmd/ctc3d/pipeline.hpp>
#include <defcmd/ctc3d/search.hpp>
#include <defcmd/ctc3d/srcnt.hpp>

#include <defrt/ftn/fio.hpp>
#include <defrt/ftn/geoutl.hpp>
#include <defrt/ftn/geoutl_pending.hpp>
#include <defrt/ftn/sse_ops.hpp>

#include <vector>

namespace def::cmd::ctc3d {

using namespace srcnt_detail;
namespace fio = def::rt::ftn::fio;
namespace sse = def::rt::sse;
namespace geo = def::rt::geoutl;

namespace {

// axis table DAT_1405c63e8: row r (1..6) at 0x1405c63e8 + 0x18*r = +e_x, -e_x, +e_y, -e_y, +e_z, -e_z
const double* axis_row(i32 r) { return DP(0x1405c63e8ull + 0x18 * static_cast<std::int64_t>(r)); }

// ((a1*b1 + a2*b2) + a3*b3) as compiled: t = a2*b2 + a1*b1; s = a3*b3 + t (dest operands first)
double dot3(const double* a, const double* b)
{
    return sse::add(sse::mul(a[2], b[2]), sse::add(sse::mul(a[1], b[1]), sse::mul(a[0], b[0])));
}

} // namespace

// @src 14002a857  SYMGRP2: symmetry planes (point, unit normal, tolerance) of the slave from its
// polygons whose nodes share the fixed code 1 in one direction; planes crossed by a free surface
// node are deleted
void fun_14002a857(i32* obj, i32* inst, double* x, i32* nface, i32* bc, i32* ndof, double* pnt, double* nrm, double* ptol,
                   i32* nplane, double* size, i32* n)
{
    (void)size;
    const std::int64_t ld = *ndof > 0 ? *ndof : 0;
    I(0x14064c280) = 0;
    *nplane = 0;
    // per direction k: c418(k) planes with -e_k... counters at 14064c418 + 8k, 14064c41c + 8k,
    // polygons with that single common code at 14064c43c + 4k
    for (i32 k = 1; k <= 3; ++k) {
        I(0x14064c43cull + 4 * static_cast<u64>(k)) = 0;
        I(0x14064c41cull + 8 * static_cast<u64>(k)) = 0;
        I(0x14064c418ull + 8 * static_cast<u64>(k)) = 0;
    }
    I(0x14064c410) = 4;
    I(0x14064c5d0) = 0x15;
    I(0x14064c5d4) = 1;
    if (0 < *nface) {
        do {
            i32 p = I(0x14064c5d4);
            fun_140227e00(obj, inst, &p, IP(0x14064c5e0), IP(0x14064c610));
            if (-1 < I(0x14064c5e0)) {
                const i32 cnt = I(0x14064c610);
                I(0x14064c614) = cnt;
                if (iabs(I(0x14064c5dcull + 4 * static_cast<std::int64_t>(cnt))) == iabs(I(0x14064c5e0))) {
                    I(0x14064c614) = cnt - 1;
                }
                I(0x14064c618) = 1;
                if (0 < I(0x14064c614)) {
                    do {
                        const std::int64_t j = I(0x14064c618);
                        I(0x14064c61c) = I(0x14064c5dcull + 4 * j);
                        const std::int64_t b = (static_cast<std::int64_t>(I(0x14064c61c)) - 1) * ld;
                        // codes(k, j) at 14064c620 + 4*(k + 3j), k = 1..3 (column j starts at 14064c630)
                        for (std::int64_t k = 1; k <= 3; ++k) {
                            I(0x14064c620ull + 4 * (k + 3 * j)) = bc[b + k - 1];
                        }
                        I(0x14064c620) = 4;
                        I(0x14064c618) += 1;
                    } while (I(0x14064c618) <= I(0x14064c614));
                }
                i32 three = 3;
                fun_14003a580(mp<i32>(0x14064c630), mp<i32>(0x1405c63f0), IP(0x14064c614), &three, IP(0x14064c6b0),
                              mp<i32>(0x14064c6c0));
                if (1 < I(0x14064c6b0)) {
                    return;
                }
                if (0 < I(0x14064c6b0)) {
                    I(0x14064c6f0) = 1;
                    const double* xl = x + 3 * (static_cast<std::int64_t>(I(0x14064c61c)) - 1);
                    do {
                        I(0x14064c6f4) = I(0x14064c6b8ull + 8 * static_cast<std::int64_t>(I(0x14064c6f0)));
                        I(0x14064c6f8) = I(0x14064c6bcull + 8 * static_cast<std::int64_t>(I(0x14064c6f0)));
                        if (I(0x14064c6f4) == 1) {
                            i32 e = I(0x14064c5d4);
                            i32 f = 1;
                            fun_140228540(obj, inst, &e, &f, DP(0x14064c700), IP(0x14064c720));
                            e = I(0x14064c5d4);
                            f = 2;
                            fun_140228540(obj, inst, &e, &f, DP(0x14064c728), IP(0x14064c730));
                            const std::int64_t d = I(0x14064c6f8);
                            if (I(0x14064c6b0) == 1) {
                                I(0x14064c43cull + 4 * d) += 1;
                            }
                            I(0x14064c738) = I(0x14064c6f8) * 2;
                            I(0x14064c734) = I(0x14064c738) - 1;
                            const double* e1 = axis_row(I(0x14064c734));
                            const double* e2 = axis_row(I(0x14064c738));
                            D(0x14064c748) = dot3(DP(0x14064c700), e2);
                            D(0x14064c740) = dot3(DP(0x14064c700), e1);
                            // 14002ae97: |c740| < 0.99f -> next; 14002aecb: 0.99f > |c748| -> next
                            if (dabs(D(0x14064c740)) < 0.9900000095367432 || 0.9900000095367432 > dabs(D(0x14064c748))) {
                                // next code
                            } else {
                                I(0x14064c750) = 1;
                                const bool more = I(0x14064c6b0) != 1;
                                i32 np = 0;
                                bool add = false;
                                i32 code = 0;
                                if (!(D(0x14064c748) > 0.9900000095367432)) { // 14002af1a: JP/JBE
                                    const i32 c0 = I(0x14064c418ull + 8 * d);
                                    I(0x14064c754) = I(0x14064c734);
                                    D(0x14064c760) = e1[0];
                                    D(0x14064c768) = e1[1];
                                    I(0x14064c418ull + 8 * d) = c0 + 1;
                                    D(0x14064c770) = e1[2];
                                    if (more) {
                                        I(0x14064c43cull + 4 * d) += 1;
                                    }
                                    add = c0 + 1 == 1;
                                    code = I(0x14064c6f8);
                                } else {
                                    I(0x14064c750) = 2;
                                    const i32 c0 = I(0x14064c41cull + 8 * d);
                                    I(0x14064c754) = I(0x14064c738);
                                    D(0x14064c760) = e2[0];
                                    D(0x14064c768) = e2[1];
                                    I(0x14064c41cull + 8 * d) = c0 + 1;
                                    D(0x14064c770) = e2[2];
                                    if (more) {
                                        I(0x14064c43cull + 4 * d) += 1;
                                    }
                                    add = c0 + 1 == 1;
                                    code = ineg(I(0x14064c6f8));
                                }
                                if (add) {
                                    np = *nplane + 1;
                                    *nplane = np;
                                    const std::int64_t l = np;
                                    I(0x14064c27cull + 4 * l) = code;
                                    const double r = sse::sqrt(D(0x14064c728));
                                    double* nl = nrm + 3 * (l - 1);
                                    double* pl = pnt + 3 * (l - 1);
                                    nl[0] = D(0x14064c760);
                                    nl[1] = D(0x14064c768);
                                    nl[2] = D(0x14064c770);
                                    pl[0] = xl[0];
                                    pl[1] = xl[1];
                                    I(0x14064c27cull + 4 * (l + 1)) = 0;
                                    pl[2] = xl[2];
                                    ptol[l - 1] = sse::mul(r, 0.10000000149011612); // DAT_140bad2d0
                                    if (np < 100) {
                                        I(0x14064c77cull + 4 * l) = 1;
                                    } else {
                                        fio::Write(6).a("Potential error in SYMGRP2").end();
                                    }
                                }
                            }
                        }
                        I(0x14064c6f0) += 1;
                    } while (I(0x14064c6f0) <= I(0x14064c6b0));
                }
            }
            I(0x14064c5d4) += 1;
        } while (I(0x14064c5d4) <= *nface);

        // a plane needs at least half of its direction's polygons aligned, and more than one
        const i32 cnt = *nplane;
        I(0x14064c410) = 1;
        if (0 < cnt) {
            do {
                const std::int64_t l = I(0x14064c410);
                I(0x14064c6f8) = I(0x14064c27cull + 4 * l);
                I(0x14064c750) = 1;
                if (I(0x14064c6f8) < 1) {
                    I(0x14064c6f8) = ineg(I(0x14064c6f8));
                    I(0x14064c750) = 2;
                }
                const std::int64_t d = I(0x14064c6f8);
                D(0x14064c910) = static_cast<double>(I(0x14064c418ull + 8 * d));
                D(0x14064c918) = static_cast<double>(I(0x14064c41cull + 8 * d));
                D(0x14064c920) = sse::add(D(0x14064c910), D(0x14064c918));
                if (0.0 >= D(0x14064c920)) { // 14002b449: JP -> division, JNC -> delete
                    I(0x14064c77cull + 4 * l) = 0;
                } else {
                    D(0x14064c928) = static_cast<double>(I(0x14064c43cull + 4 * d));
                    D(0x14064c930) = sse::div(D(0x14064c920), D(0x14064c928));
                    if (D(0x14064c930) < 0.5 || D(0x14064c928) <= 1.0) { // DAT_140bad260, DAT_140bad138
                        I(0x14064c77cull + 4 * l) = 0;
                    }
                }
                I(0x14064c410) += 1;
            } while (I(0x14064c410) <= cnt);
        }
    }

    // a surface node without fixed code 1 on the outer side of a plane deletes it
    I(0x14064c410) = 1;
    if (1 <= *n) {
        do {
            I(0x14064c618) = 1;
            if (0 < *nplane) {
                const std::int64_t i = I(0x14064c410);
                const i32* bi = bc + (i - 1) * ld;
                const double* xi = x + 3 * (i - 1);
                do {
                    i32 pt = I(0x14064c410);
                    i32 f = 3;
                    fun_140228830(obj, inst, &pt, &f, IP(0x14064c938), IP(0x14064c93c));
                    if (I(0x14064c938) == 0 || bi[0] == 1 || bi[1] == 1 || bi[2] == 1) {
                        break;
                    }
                    const std::int64_t l = I(0x14064c618);
                    if (0 < I(0x14064c77cull + 4 * l)) {
                        const double* pl = pnt + 3 * (l - 1);
                        const double* nl = nrm + 3 * (l - 1);
                        D(0x14064c700) = sse::sub(xi[0], pl[0]);
                        D(0x14064c708) = sse::sub(xi[1], pl[1]);
                        D(0x14064c710) = sse::sub(xi[2], pl[2]);
                        D(0x14064c940) = dneg(dot3(DP(0x14064c700), nl));
                        if (!(0.0 < D(0x14064c940))) { // 14002b1a9: JP/JC keep
                            I(0x14064c77cull + 4 * l) = 0;
                        }
                    }
                    I(0x14064c618) += 1;
                } while (I(0x14064c618) <= *nplane);
            }
            I(0x14064c410) += 1;
        } while (I(0x14064c410) <= *n);
    }

    // compaction of the kept planes
    I(0x14064c410) = 1;
    I(0x14064c6f0) = 0;
    if (0 < *nplane) {
        do {
            while (I(0x14064c77cull + 4 * static_cast<std::int64_t>(I(0x14064c410))) == 1) {
                I(0x14064c6f0) += 1;
                const std::int64_t s = I(0x14064c410);
                const std::int64_t d = I(0x14064c6f0);
                I(0x14064c410) = static_cast<i32>(s + 1);
                for (int c = 0; c < 3; ++c) {
                    nrm[3 * (d - 1) + c] = nrm[3 * (s - 1) + c];
                    pnt[3 * (d - 1) + c] = pnt[3 * (s - 1) + c];
                }
                ptol[d - 1] = ptol[s - 1];
                if (*nplane < s + 1) {
                    *nplane = I(0x14064c6f0);
                    return;
                }
            }
            fio::Write(dat_14077a7bc).a("Delete Symmetry definition in Symgrp2").end();
            I(0x14064c410) += 1;
        } while (I(0x14064c410) <= *nplane);
    }
    *nplane = I(0x14064c6f0);
}

// @src 14003d2ee  EI2 = 0 for the valid polygons of obj that lie in one of the symmetry planes:
// |ER1 . N_k| >= 0.995f and centroid distance |(P_k - c) . N_k| <= tol_k
void fun_14003d2ee(i32* obj, i32* inst, i32* npoly, double* pnt, double* nrm, double* ptol, i32* nplane, double* size)
{
    (void)npoly;
    // ALLOCATABLE candidate list (descriptor at DAT_14066aa70; flags DAT_14066aa80 bit 0 =
    // allocated); the storage itself is a std::vector
    static std::vector<i32> list;
    m<std::uint64_t>(0x14066aa70) = 0;
    m<std::uint64_t>(0x14066aa78) = 0x20;
    I(0x14066aa80) = 0x18;
    I(0x14066aa84) = 0x20000000;
    m<std::uint64_t>(0x14066aa88) = 0x2000200000000ull;
    m<std::uint64_t>(0x14066aa90) = 0;
    m<std::uint64_t>(0x14066aa98) = 0;
    if (*nplane == 0) {
        return;
    }
    D(0x14066aac0) = sse::mul(*size, 9.999999747378752e-05); // DAT_140bad1f8
    I(0x14066aac8) = 1;
    if (*nplane < 1) {
        return;
    }
    do {
        const std::int64_t k = I(0x14066aac8);
        D(0x14066aac0) = ptol[k - 1];
        double* pk = pnt + 3 * (k - 1);
        double* nk = nrm + 3 * (k - 1);
        fun_140080a83(pk, nk, size, DP(0x14066aad0));
        i32 z1 = 0;
        i32 z2 = 0;
        fun_140228c60(obj, inst, DP(0x14066aad0), IP(0x14066ab00), &z1, &z2);
        I(0x14066ab04) = 0;
        if (0 < I(0x14066ab00)) {
            const std::int64_t cnt = I(0x14066ab00);
            I(0x14066aa80) = (I(0x14066aa80) & ~0x10) | 0x10;
            m<std::uint64_t>(0x14066aaa0) = 1;
            m<std::uint64_t>(0x14066aaa8) = static_cast<std::uint64_t>(cnt);
            m<std::uint64_t>(0x14066aab0) = 1;
            list.assign(static_cast<std::size_t>(cnt), 0);
            m<std::uint64_t>(0x14066aa70) = reinterpret_cast<std::uint64_t>(list.data());
            m<std::uint64_t>(0x14066aa90) = reinterpret_cast<std::uint64_t>(list.data());
            m<std::uint64_t>(0x14066aa98) = static_cast<std::uint64_t>(cnt) << 5;
            I(0x14066aa80) = (I(0x14066aa80) & ~1) | 1;
            i32 z = 0;
            fun_14022a5c0(obj, inst, list.data(), reinterpret_cast<std::uint32_t*>(IP(0x14066ab00)), &z);
        }
        I(0x14066ab08) = 0;
        I(0x14066ab0c) = 1;
        if (0 < I(0x14066ab00)) {
            do {
                I(0x14066ab10) = list[static_cast<std::size_t>(I(0x14066ab0c) - 1)];
                if (I(0x14066ab08) != I(0x14066ab10)) {
                    I(0x14066ab08) = I(0x14066ab10);
                    i32 f = 2;
                    fun_140228260(obj, inst, IP(0x14066ab10), &f, IP(0x14066ab14), IP(0x14066ab18));
                    if (I(0x14066ab14) == 1) {
                        fun_14004f363(obj, inst, IP(0x14066ab10), DP(0x14066ab20), IP(0x14066ab80), IP(0x14066ab90));
                        i32 one = 1;
                        fun_140228540(obj, inst, IP(0x14066ab10), &one, DP(0x14066aba0), IP(0x14066abc0));
                        D(0x14066abc8) = dabs(dot3(DP(0x14066aba0), nk));
                        if (!(D(0x14066abc8) < 0.9950000047683716)) { // DAT_140bad7f0; JP proceed, JC skip
                            I(0x14066abf0) = 1;
                            D(0x14066abd0) = 0.0;
                            D(0x14066abd8) = 0.0;
                            D(0x14066abe0) = 0.0;
                            if (0 < I(0x14066ab90)) {
                                do {
                                    const double* xp = col3(0x14066ab20, I(0x14066abf0));
                                    D(0x14066abd0) = sse::add(xp[0], D(0x14066abd0));
                                    D(0x14066abd8) = sse::add(xp[1], D(0x14066abd8));
                                    I(0x14066abf0) += 1;
                                    D(0x14066abe0) = sse::add(xp[2], D(0x14066abe0));
                                } while (I(0x14066abf0) <= I(0x14066ab90));
                            }
                            const double c = static_cast<double>(I(0x14066ab90));
                            D(0x14066abd0) = sse::div(D(0x14066abd0), c);
                            D(0x14066abd8) = sse::div(D(0x14066abd8), c);
                            D(0x14066abe0) = sse::div(D(0x14066abe0), c);
                            D(0x14066aba0) = sse::sub(pk[0], D(0x14066abd0));
                            D(0x14066aba8) = sse::sub(pk[1], D(0x14066abd8));
                            D(0x14066abb0) = sse::sub(pk[2], D(0x14066abe0));
                            const double* dv = DP(0x14066aba0);
                            D(0x14066abf8) = sse::sqrt(sse::add(sse::mul(dv[2], dv[2]), sse::add(sse::mul(dv[1], dv[1]), sse::mul(dv[0], dv[0]))));
                            D(0x14066abc8) = dabs(dot3(dv, nk));
                            I(0x14066ab14) = 1;
                            if (!(D(0x14066aac0) < D(0x14066abc8))) { // 14003d9e7: JP/JC skip
                                i32 cnt1 = 1;
                                i32 f2 = 2;
                                I(0x14066ab14) = 0;
                                fun_1402280d0(obj, inst, IP(0x14066ab10), &f2, IP(0x14066ab14), &cnt1);
                            }
                        }
                    }
                }
                I(0x14066ab0c) += 1;
            } while (I(0x14066ab0c) <= I(0x14066ab00));
        }
        if ((I(0x14066aa80) & 1) == 1) { // DEALLOCATE (FUN_1404b6d74)
            list.clear();
            list.shrink_to_fit();
            I(0x14066aa80) &= ~1;
            m<std::uint64_t>(0x14066aa70) = 0;
        }
        I(0x14066aac8) += 1;
    } while (I(0x14066aac8) <= *nplane);
}

// @src 14003dabe  symmetry die: polygons of obj in front of a plane get EI2 0 / 2, polygons with
// EI2 = -2000 near several planes get EI2 = 1; *flag = 1 when no polygon is left valid. Reached
// only with DAT_14076abb8 > 0, which no DEF_PRE routine sets.
void fun_14003dabe(i32* obj, i32* inst, i32* npoly, double* pnt, double* nrm, i32* nplane, i32* pobj, i32* flag, double* tol, double* box)
{
    if (*nplane == 0) {
        return;
    }
    I(0x14066ac00) = 0;
    I(0x14066ac04) = 1;
    if (0 < *nplane) {
        do {
            I(0x14066ac08) = 1;
            const std::int64_t k = I(0x14066ac04);
            const double* nk = nrm + 3 * (k - 1);
            geo::fun_1404cf5c0(nk, DP(0x14066ac10));
            const i32 po = pobj[k - 1];
            if (*obj != po && -1 < po) {
                I(0x14066ac60) = 1;
                if (0 < *npoly) {
                    const double mt = dneg(*tol);
                    do {
                        i32 e = I(0x14066ac60);
                        i32 f = 2;
                        fun_140228260(obj, inst, &e, &f, IP(0x14066ac64), IP(0x14066ac68));
                        if (I(0x14066ac64) == 1) {
                            e = I(0x14066ac60);
                            fun_14004f363(obj, inst, &e, DP(0x14066ac70), IP(0x14066acd0), IP(0x14066ace0));
                            geo::fun_1404d0939(DP(0x14066ac70), pnt + 3 * (k - 1), DP(0x14066ac10), DP(0x14066acf0), IP(0x14066ac08),
                                               IP(0x14066ace0));
                            I(0x14066ad50) = 0;
                            I(0x14066ad54) = 0;
                            I(0x14066ad58) = 0;
                            I(0x14066ad5c) = 1;
                            bool go;
                            if (I(0x14066ace0) < 1) {
                                go = I(0x14066ace0) == 0;
                            } else {
                                const double at = dabs(*tol);
                                do {
                                    const double z = col3(0x14066acf0, I(0x14066ad5c))[2];
                                    if (z >= mt) {
                                        I(0x14066ad50) += 1;
                                    }
                                    if (z > 0.0) {
                                        I(0x14066ad54) += 1;
                                    }
                                    if (z <= at && z >= sse::mul(mt, 0.01)) { // DAT_140bad740
                                        I(0x14066ad58) += 1;
                                    }
                                    I(0x14066ad5c) += 1;
                                } while (I(0x14066ad5c) <= I(0x14066ace0));
                                go = I(0x14066ad50) == I(0x14066ace0);
                            }
                            if (go) {
                                i32 one = 1;
                                e = I(0x14066ac60);
                                fun_140228540(obj, inst, &e, &one, DP(0x14066ad60), IP(0x14066ad80));
                                if (I(0x14066ad54) == I(0x14066ace0)) {
                                    I(0x14066ac64) = 2;
                                }
                                D(0x14066ad88) = dot3(nk, DP(0x14066ad60));
                                if (D(0x14066ad88) > 0.9950000047683716 && I(0x14066ad58) == I(0x14066ace0)) {
                                    I(0x14066ac64) = 0;
                                }
                                i32 cnt1 = 1;
                                i32 f2 = 2;
                                e = I(0x14066ac60);
                                fun_1402280d0(obj, inst, &e, &f2, IP(0x14066ac64), &cnt1);
                            }
                        }
                        I(0x14066ac60) += 1;
                    } while (I(0x14066ac60) <= *npoly);
                }
            }
            I(0x14066ac04) += 1;
        } while (I(0x14066ac04) <= *nplane);
    }
    const i32 np = *npoly;
    I(0x14066ac60) = 1;
    D(0x14066ad90) = 9.99999993922529e-09; // DAT_140bad7f8
    if (np < 1) {
        I(0x14066ad50) = 0;
        I(0x14066ac60) = 1;
        *flag = 0;
        if (np == 0) {
            *flag = 1;
        }
        return;
    }
    do {
        i32 e = I(0x14066ac60);
        i32 f = 2;
        fun_140228260(obj, inst, &e, &f, IP(0x14066ac64), IP(0x14066ad98));
        if (I(0x14066ac64) == -2000) {
            e = I(0x14066ac60);
            fun_140227e00(obj, inst, &e, IP(0x14066acd0), IP(0x14066ace0));
            if (-1 < I(0x14066acccull + 4 * static_cast<std::int64_t>(I(0x14066ace0)))) {
                e = I(0x14066ac60);
                fun_14004f363(obj, inst, &e, DP(0x14066ac70), IP(0x14066acd0), IP(0x14066ace0));
                i32 four = 4;
                geo::fun_1404d1b1c(DP(0x14066ac70), DP(0x14066ada0), DP(0x14066ad90), &four, IP(0x1405c69d0));
                geo::fun_1404d1ed7(box, DP(0x14066ada0), IP(0x14066add0));
                if (I(0x14066add0) != 0) {
                    I(0x14066ac08) = 1;
                    I(0x14066ad54) = 0;
                    i32 one = 1;
                    e = I(0x14066ac60);
                    fun_140228540(obj, inst, &e, &one, DP(0x14066ad60), IP(0x14066ad80));
                    I(0x14066ac04) = 1;
                    if (0 < *nplane) {
                        do {
                            const std::int64_t k = I(0x14066ac04);
                            if (-1 < pobj[k - 1]) {
                                const double* nk = nrm + 3 * (k - 1);
                                D(0x14066ad88) = dot3(nk, DP(0x14066ad60));
                                geo::fun_1404cf5c0(nk, DP(0x14066ac10));
                                geo::fun_1404d0939(DP(0x14066ac70), pnt + 3 * (k - 1), DP(0x14066ac10), DP(0x14066acf0), IP(0x14066ac08),
                                                   IP(0x14066ace0));
                                I(0x14066ad50) = 0;
                                I(0x14066add4) = I(0x14066ace0);
                                if (I(0x14066acd0) == iabs(I(0x14066acccull + 4 * static_cast<std::int64_t>(I(0x14066ace0))))) {
                                    I(0x14066add4) = I(0x14066ace0) - 1;
                                }
                                I(0x14066ad5c) = 1;
                                if (0 < I(0x14066add4)) {
                                    do {
                                        if (col3(0x14066acf0, I(0x14066ad5c))[2] <= 0.0) {
                                            I(0x14066ad50) += 1;
                                        }
                                        I(0x14066ad5c) += 1;
                                    } while (I(0x14066ad5c) <= I(0x14066add4));
                                    if (0 < I(0x14066ad50) && 0.8999999761581421 > dabs(D(0x14066ad88))) { // DAT_140bad800
                                        I(0x14066ad54) += 1;
                                    }
                                }
                            }
                            I(0x14066ac04) += 1;
                        } while (I(0x14066ac04) <= *nplane);
                        if (1 < I(0x14066ad54)) {
                            I(0x14066ac64) = 1;
                            i32 f2 = 2;
                            e = I(0x14066ac60);
                            fun_1402280d0(obj, inst, &e, &f2, IP(0x14066ac64), IP(0x14066ac68));
                        }
                    }
                }
            }
        }
        I(0x14066ac60) += 1;
    } while (I(0x14066ac60) <= *npoly);
    I(0x14066ad50) = 0;
    I(0x14066ac60) = 1;
    do {
        i32 e = I(0x14066ac60);
        i32 f = 2;
        fun_140228260(obj, inst, &e, &f, IP(0x14066ac64), IP(0x14066ad98));
        if (I(0x14066ac64) != 1) {
            I(0x14066ad50) += 1;
        }
        I(0x14066ac60) += 1;
    } while (I(0x14066ac60) <= *npoly);
    *flag = 0;
    if (*npoly == I(0x14066ad50)) {
        *flag = 1;
    }
}

// @src 14005d44f  EI3 of obj's polygons with EI2 0 or 2: per neighbour (EI1, pairs) 2 when the
// neighbour has the same EI2, 1 when not, 0 without neighbour
void fun_14005d44f(i32* obj, i32* inst, i32* npoly)
{
    for (std::int64_t k = 1; k <= 4; ++k) {
        I(0x14069497cull + 4 * k) = 0;
    }
    I(0x140694978) = 5;
    I(0x140694990) = 1;
    if (*npoly < 1) {
        return;
    }
    do {
        i32 e = I(0x140694990);
        i32 f = 3;
        i32 c = 4;
        fun_1402280d0(obj, inst, &e, &f, IP(0x140694980), &c);
        I(0x140694990) += 1;
    } while (I(0x140694990) <= *npoly);
    I(0x140694990) = 1;
    do {
        i32 e = I(0x140694990);
        i32 f = 2;
        fun_140228260(obj, inst, &e, &f, IP(0x140694994), IP(0x140694998));
        if (I(0x140694994) == 0 || I(0x140694994) == 2) {
            i32 f1 = 1;
            i32 e1 = I(0x140694990);
            fun_140228260(obj, inst, &e1, &f1, IP(0x1406949a0), IP(0x1406949c0));
            i32 f3 = 3;
            e1 = I(0x140694990);
            fun_140228260(obj, inst, &e1, &f3, IP(0x140694980), IP(0x1406949c4));
            const i32 h = I(0x1406949c0) / 2;
            I(0x1406949c8) = h;
            I(0x140694978) = 1;
            if (0 < h) {
                do {
                    const std::int64_t k = I(0x140694978);
                    I(0x1406949cc) = I(0x14069499cull + 4 * k);
                    if (0 < I(0x1406949cc)) {
                        i32 f2 = 2;
                        fun_140228260(obj, inst, IP(0x1406949cc), &f2, IP(0x1406949d0), IP(0x140694998));
                        I(0x14069497cull + 4 * k) = I(0x140694994) == I(0x1406949d0) ? 2 : 1;
                    }
                    I(0x140694978) += 1;
                } while (I(0x140694978) <= h);
            }
            i32 c4 = 4;
            f3 = 3;
            e1 = I(0x140694990);
            fun_1402280d0(obj, inst, &e1, &f3, IP(0x140694980), &c4);
        }
        I(0x140694990) += 1;
    } while (I(0x140694990) <= *npoly);
}

} // namespace def::cmd::ctc3d
