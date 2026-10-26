// SRCNT2 FUN_14002cdee and its subdivision test FUN_14002de5d (GENCTC_3d_search.md sec. 7.3,
// 11.1). Packet p67-7, group "srcnt".
#include "srcnt_internal.hpp"

#include <defcmd/ctc3d/cube.hpp>
#include <defcmd/ctc3d/decide.hpp>
#include <defcmd/ctc3d/pipeline.hpp>
#include <defcmd/ctc3d/search.hpp>
#include <defcmd/ctc3d/srcnt.hpp>

#include <defrt/ftn/fio.hpp>
#include <defrt/ftn/geoutl_pending.hpp>
#include <defrt/ftn/sse_ops.hpp>

namespace def::cmd::ctc3d {

using namespace srcnt_detail;
namespace fio = def::rt::ftn::fio;
namespace sse = def::rt::sse;
namespace geo = def::rt::geoutl;

namespace {

i32 inst_of(i32 k) { return I(0x140778684ull + 4 * static_cast<std::int64_t>(k)); }

// SRCNT2 work arrays (1-based): node list conn(k) at 14064d2cc + 4k, node coordinates
// X(:, j) at 14064d318 + 24j (X(1,1) = 14064d330), flags at 14064d30c + 4j
i32& conn2(std::int64_t k) { return I(0x14064d2ccull + 4 * static_cast<u64>(k)); }
double* xnode(std::int64_t j) { return DP(0x14064d318ull + 24 * static_cast<u64>(j)); }
i32& flag2(std::int64_t j) { return I(0x14064d30cull + 4 * static_cast<u64>(j)); }

// 14002de5d arrays: A(j,k) at 14064d590 + 8((j-1)+3(k-1)); B(j,i) (subdivision triangle
// corners) at 14064d5f0 + 8((j-1)+3(i-1)); W(k) at 14064d688 + 8k; C(k) at 14064d6c8 + 8k
double& amat(std::int64_t j, std::int64_t k) { return D(0x14064d590ull + 8 * static_cast<u64>((j - 1) + 3 * (k - 1))); }
double& bmat(std::int64_t j, std::int64_t i) { return D(0x14064d5f0ull + 8 * static_cast<u64>((j - 1) + 3 * (i - 1))); }
double& wvec(std::int64_t k) { return D(0x14064d688ull + 8 * static_cast<u64>(k)); }
double& cvec(std::int64_t k) { return D(0x14064d6c8ull + 8 * static_cast<u64>(k)); }

} // namespace

// @src 14002cdee  SRCNT2: polygons of the partner obj whose nodes already touch another object
// (negative NI4 polygon numbers, or a common negative code) and that overlap the slave's box are
// tested by subdivision against that object (14002de5d), which may set EI2 = -k. SRCNT2.DAT
// (unit 92, two INTEGER*4, once per process) overrides the subdivision limits.
void fun_14002cdee(i32* obj, i32* inst, i32* npoly, double* tol, double* box, i32* oobj, i32* p7)
{
    if (I(0x1405c66b0) == 0) {
        // OPEN(UNIT=92, FILE='SRCNT2.DAT', STATUS='OLD', ERR=...); READ(92,*) N1, N2; CLOSE(92)
        I(0x14064ca7c) = 0x5c;
        if (fio::open_old(0x5c, "SRCNT2.DAT")) {
            const int st = fio::read_list_i32(0x5c, IP(0x14064ca80), 2);
            if (st != 1 && st != 2) {
                I(0x1405c66b4) = I(0x14064ca80);
                I(0x1405c66b8) = I(0x14064ca84);
            }
            fio::close(0x5c);
        }
    }
    I(0x1405c66b0) = 1;
    const double h = sse::mul(*tol, 20.0); // DAT_140bad490
    I(0x14064ca9c) = *npoly;
    I(0x14064ca90) = 1;
    I(0x14064ca94) = 0;
    I(0x14064ca98) = 1;
    D(0x14064ca88) = h;
    const i32 nparts = I(0x14075bc80);
    if (0 < nparts && 0 < *p7 && 1000 < I(0x14064ca9c)) {
        // partitions: start(k) at 14064caa8 + 8k, end(k) at 14064caac + 8k (dead in DEF_PRE)
        auto pstart = [](std::int64_t k) -> i32& { return I(0x14064caa8ull + 8 * static_cast<u64>(k)); };
        auto pend = [](std::int64_t k) -> i32& { return I(0x14064caacull + 8 * static_cast<u64>(k)); };
        I(0x14064ca94) = 1;
        I(0x14064caa0) = 0;
        const i32 q = I(0x14064ca9c) / nparts;
        pstart(1) = 1;
        I(0x14064d2b0) = 2;
        I(0x14064caa4) = q;
        pend(1) = q;
        for (i32 k = 2; k <= nparts; ++k) {
            pend(k) = pend(k - 1) + q;
            pstart(k) = pend(k - 1) + 1;
        }
        I(0x14064d2b4) = I(0x14075bc84) + 1;
        pend(nparts) = *npoly;
        I(0x14064ca9c) = pend(I(0x14064d2b4));
        I(0x14064ca98) = pstart(I(0x14064d2b4));
    }
    I(0x14064d2b8) = 0;
    I(0x14064d2b0) = I(0x14064ca98);
    if (I(0x14064ca98) <= I(0x14064ca9c)) {
        do {
            i32 pp = I(0x14064d2b0);
            i32 f = 2;
            fun_140228260(obj, inst, &pp, &f, IP(0x14064d2bc), IP(0x14064ca90));
            if (I(0x14064d2bc) == 0) {
                goto next_poly;
            }
            D(0x14064d2c0) = *tol;
            if (I(0x14064d2bc) == -2000) {
                D(0x14064d2c0) = sse::mul(D(0x14064d2c0), 5.0); // DAT_140bad498
            }
            pp = I(0x14064d2b0);
            fun_140227e00(obj, inst, &pp, IP(0x14064d2d0), IP(0x14064d300));
            I(0x14064d304) = I(0x14064d300);
            if (iabs(conn2(I(0x14064d300))) == iabs(I(0x14064d2d0))) {
                I(0x14064d304) = I(0x14064d300) - 1;
            }
            I(0x14064d308) = 0;
            I(0x14064d30c) = 1;
            if (0 < I(0x14064d304)) {
                do {
                    const std::int64_t j = I(0x14064d30c);
                    I(0x14064d320) = iabs(conn2(j));
                    flag2(j) = 0;
                    fun_140227fc0(obj, inst, IP(0x14064d320), xnode(j));
                    i32 f4 = 4;
                    fun_140228830(obj, inst, IP(0x14064d320), &f4, IP(0x14064d390), IP(0x14064d3c0));
                    const i32 px = I(0x14064d39c);
                    const i32 py = I(0x14064d3a0);
                    const i32 pz = I(0x14064d3a4);
                    // codes(:, j) at 14064d3c4 + 12j (column 1 at 14064d3d0), |polygons| at 14064d424 + 12j
                    I(0x14064d3c4ull + 12 * static_cast<u64>(j)) = I(0x14064d390);
                    I(0x14064d3c8ull + 12 * static_cast<u64>(j)) = I(0x14064d394);
                    I(0x14064d3ccull + 12 * static_cast<u64>(j)) = I(0x14064d398);
                    I(0x14064d424ull + 12 * static_cast<u64>(j)) = iabs(px);
                    I(0x14064d428ull + 12 * static_cast<u64>(j)) = iabs(py);
                    I(0x14064d460) = 0;
                    I(0x14064d42cull + 12 * static_cast<u64>(j)) = iabs(pz);
                    if (px < 0 || py < 0 || pz < 0) {
                        // the last slot (x, y, z) with a negative polygon number (node inside)
                        I(0x14064d460) = pz < 0 ? 3 : (py < 0 ? 2 : 1);
                        I(0x14064d308) += 1;
                        I(0x14064d464) = ineg(I(0x14064d38cull + 4 * static_cast<u64>(I(0x14064d460))));
                        flag2(j) = 1;
                    }
                    I(0x14064d30c) += 1;
                } while (I(0x14064d30c) <= I(0x14064d304));
            }
            {
                i32 three = 3;
                fun_14003a978(mp<i32>(0x14064d3d0), IP(0x1405c66c0), IP(0x14064d304), &three, IP(0x14064d468), IP(0x14064d46c));
            }
            if (!(I(0x14064d468) < 0 || I(0x14064d308) != 0)) {
                goto next_poly;
            }
            geo::fun_1404d1b1c(xnode(1), DP(0x14064d470), DP(0x14064ca88), IP(0x14064d300), IP(0x1405c66c0));
            geo::fun_1404d1ed7(box, DP(0x14064d470), IP(0x14064d4a0));
            if (I(0x14064d4a0) == 0) {
                goto next_poly;
            }
            if (I(0x14064d468) < 0) {
                I(0x14064d464) = ineg(I(0x14064d468));
                if (I(0x14064d464) < 0) {
                    I(0x14064d464) = I(0x14064d468);
                }
            }
            if (I(0x14064d464) == 0) {
                goto next_poly;
            }
            I(0x14064d4a4) = inst_of(I(0x14064d464));
            D(0x14064ca88) = h;
            {
                i32 two = 2;
                i32 p2 = I(0x14064d2b0);
                fun_140228540(obj, inst, &p2, &two, DP(0x14064d4a8), IP(0x14064d4b0));
            }
            if (D(0x14064d4a8) < 1e-10) { // DAT_140bad4a0; 14002d511: JP proceed, JC skip
                goto next_poly;
            }
            I(0x14064d4b4) = cvtt(sse::div(D(0x14064d4a8), h));
            I(0x14064d4b8) = I(0x14064d2bc);
            I(0x14064d4bc) = I(0x14064d2b0);
            if (I(0x14064d468) == 0) {
                // nodes not yet inside: their nearest point on the touching object and on the slave
                I(0x14064d4c4) = inst_of(*oobj);
                I(0x14064d4c0) = 0;
                I(0x14064d4c8) = 0;
                I(0x14064d4cc) = 1;
                I(0x14064d30c) = 1;
                if (0 < I(0x14064d304)) {
                    i32 it = 2;
                    do {
                        bool done = false;
                        while (flag2(I(0x14064d30c)) != 0) {
                            I(0x14064d30c) += 1;
                            ++it;
                            if (I(0x14064d304) < I(0x14064d30c)) {
                                done = true;
                                break;
                            }
                        }
                        if (done) {
                            break;
                        }
                        I(0x14064d4d0) = it;
                        if (I(0x14064d304) < it) {
                            I(0x14064d4d0) = it - I(0x14064d304);
                        }
                        double* pj = xnode(I(0x14064d30c));
                        fun_14007a47d(pj, xnode(I(0x14064d4d0)), DP(0x14064d4d8));
                        D(0x14064d4e0) = sse::add(h, D(0x14064d4d8));
                        i32 three = 3;
                        fun_14006798b(IP(0x14064d464), IP(0x14064d4a4), &three, IP(0x14064d4c8), pj, DP(0x14064d4f0), IP(0x14064d510),
                                      DP(0x14064d4e0), IP(0x14064d514), DP(0x14064d520), DP(0x1405c66d0), IP(0x14064d4cc),
                                      IP(0x14064d4c0));
                        if (I(0x14064d510) == 0) {
                            fio::Write(6).a("SRCNT2").i(*obj).i(I(0x14064d2b0)).i(I(0x14064d464)).end();
                        }
                        fun_14022d770(oobj, IP(0x14064d4c4), pj, DP(0x14064d540), reinterpret_cast<std::uint32_t*>(IP(0x14064d548)),
                                      IP(0x14064d4c0));
                        // 14002da44: COMISD |d4e0|, |d540|; JBE continue
                        if (dabs(D(0x14064d4e0)) > dabs(D(0x14064d540))) {
                            goto next_poly;
                        }
                        I(0x14064d30c) += 1;
                        ++it;
                    } while (I(0x14064d30c) <= I(0x14064d304));
                }
            }
            fun_14002de5d(obj, inst, IP(0x14064d4bc), IP(0x14064d464), oobj, xnode(1), IP(0x14064d4b4), DP(0x14064ca88), DP(0x14064d2c0),
                          IP(0x1405c66b4), IP(0x1405c66b8), IP(0x14064d4b8));
            if (I(0x14064d2bc) != I(0x14064d4b8)) {
                I(0x14064d2b8) = 1;
            }
        next_poly:
            I(0x14064d2b0) += 1;
        } while (I(0x14064d2b0) <= I(0x14064ca9c));
    }
    if (0 < I(0x14064ca94)) {
        fun_14008a8c3();
    }
}

// @src 14002de5d  subdivision test of polygon *poly of obj against object *kobj: points of
// *nsub nested triangles (and their edges, at most *npmax points each) are projected on kobj;
// when every one lies within [-4 tol, tol] on a polygon with opposite normal (ER1 . ER1' <=
// -0.95f) EI2(poly) = *ei2 = -kobj, else nothing changes
void fun_14002de5d(i32* obj, i32* inst, i32* poly, i32* kobj, i32* oobj, double* xp, i32* nsub, double* h, double* tol, i32* nsubmax,
                   i32* npmax, i32* ei2)
{
    if (*nsub == 0) {
        *nsub = 1;
        if (!(0 < *nsubmax)) {
            *nsub = *nsubmax;
        }
    } else if (!(*nsub <= *nsubmax)) {
        *nsub = *nsubmax;
    }
    constexpr double third = 0x1.55555554c2bb5p-2; // DAT_140bad4c0 = 0.3333333333
    constexpr double base = 0x1.47ae140000000p-9;  // DAT_140bad4c8 = 0.0025f
    D(0x14064d550) = third;
    D(0x14064d558) = sse::div(third, static_cast<double>(static_cast<i32>(static_cast<std::uint32_t>(*nsub) + 1u)));
    I(0x14064d568) = inst_of(*kobj);
    I(0x14064d56c) = inst_of(*oobj);
    D(0x14064d560) = base;
    I(0x14064d570) = 1;
    if (*nsub < 1) {
        goto done;
    }
    do {
        {
            // triangle i: barycentric weights s (off-diagonal) and 1 - 2s (diagonal)
            const double s = sse::add(sse::mul(static_cast<double>(I(0x14064d570) - 1), D(0x14064d558)), base);
            D(0x14064d578) = s;
            D(0x14064d580) = sse::sub(1.0, sse::mul(s, 2.0)); // DAT_140bad138, DAT_140bad4d0
            for (std::int64_t k = 1; k <= 3; ++k) {
                for (std::int64_t j = 1; j <= 3; ++j) {
                    amat(j, k) = s;
                }
            }
            I(0x14064d58c) = 4;
            for (std::int64_t j = 1; j <= 3; ++j) {
                amat(j, j) = D(0x14064d580);
            }
            I(0x14064d588) = 4;
            for (std::int64_t i = 1; i <= 3; ++i) {
                for (std::int64_t j = 1; j <= 3; ++j) {
                    bmat(j, i) = 0.0;
                }
            }
            // B(j,i) = sum_l X(j,l) * A(l,i), accumulated in place
            for (std::int64_t i = 1; i <= 3; ++i) {
                for (std::int64_t j = 1; j <= 3; ++j) {
                    double acc = bmat(j, i);
                    for (std::int64_t l = 1; l <= 3; ++l) {
                        acc = sse::add(acc, sse::mul(xp[(j - 1) + 3 * (l - 1)], amat(l, i)));
                        bmat(j, i) = acc;
                    }
                    I(0x14064d650) = 4;
                }
            }
            I(0x14064d5e4) = 4;
            I(0x14064d5e0) = 4;
        }
        I(0x14064d654) = 3;
        if (*nsub == I(0x14064d570)) {
            I(0x14064d654) = 1; // innermost triangle: its centroid only
        }
        I(0x14064d658) = 1;
        {
            i32 it = 2;
            do {
                const bool wrap = 3 < it;
                I(0x14064d65c) = wrap ? 1 : it;
                fun_14007a47d(&bmat(1, I(0x14064d658)), &bmat(1, I(0x14064d65c)), DP(0x14064d660));
                I(0x14064d668) = cvtt(sse::div(D(0x14064d660), *h));
                if (0 < I(0x14064d668) || I(0x14064d654) != 3) {
                    I(0x14064d67c) = it;
                    if (I(0x14064d654) == 1) {
                        I(0x14064d668) = 1;
                        I(0x14064d678) = I(0x14064d658);
                        D(0x14064d670) = 1.0;
                        if (wrap) {
                            I(0x14064d67c) = 1;
                        }
                        I(0x14064d680) = I(0x14064d67c) + 1;
                        if (3 < I(0x14064d680)) {
                            I(0x14064d680) = 1;
                        }
                        I(0x14064d588) = 1;
                    } else {
                        if (I(0x14064d668) == 0) {
                            I(0x14064d668) = 1;
                        }
                        I(0x14064d668) += 1;
                        if (*npmax < I(0x14064d668)) {
                            I(0x14064d668) = *npmax;
                        }
                        I(0x14064d678) = I(0x14064d658);
                        // 14002e4c2..14002e4d8: REAL*4 1.0 / REAL(n - 1), widened
                        D(0x14064d670) = static_cast<double>(1.0f / static_cast<float>(I(0x14064d668) - 1)); // DAT_140bad4d8
                        if (wrap) {
                            I(0x14064d67c) = 1;
                        }
                        I(0x14064d680) = I(0x14064d67c) + 1;
                        if (3 < I(0x14064d680)) {
                            I(0x14064d680) = 1;
                        }
                        I(0x14064d588) = 1;
                        if (I(0x14064d668) < 1) {
                            goto next_edge;
                        }
                    }
                    I(0x14064d678) = I(0x14064d658);
                    I(0x14064d588) = 1;
                    const double r = sse::add(*h, D(0x14064d660));
                    i32 iv = 0;
                    do {
                        if (I(0x14064d654) == 1) {
                            constexpr double c3 = 0x1.5555554f9b516p-2; // DAT_140bad4e0 = 0.333333333
                            D(0x14064d6c0) = c3;
                            D(0x14064d6b8) = c3;
                            D(0x14064d6b0) = c3;
                        } else {
                            D(0x14064d6b8) = sse::mul(static_cast<double>(iv), D(0x14064d670));
                            D(0x14064d6b0) = sse::sub(1.0, D(0x14064d6b8));
                            D(0x14064d6c0) = 0.0;
                            D(0x14064d6c8) = D(0x14064d6b8);
                        }
                        wvec(I(0x14064d658)) = D(0x14064d6b0);
                        wvec(I(0x14064d67c)) = D(0x14064d6b8);
                        wvec(I(0x14064d680)) = D(0x14064d6c0);
                        for (std::int64_t k = 1; k <= 3; ++k) {
                            cvec(k) = 0.0;
                        }
                        // C(k) = sum_l B(k,l) * W(l)
                        for (std::int64_t k = 1; k <= 3; ++k) {
                            double acc = cvec(k);
                            for (std::int64_t l = 1; l <= 3; ++l) {
                                acc = sse::add(acc, sse::mul(bmat(k, l), wvec(l)));
                                cvec(k) = acc;
                            }
                        }
                        I(0x14064d5e4) = 4;
                        I(0x14064d5e0) = 4;
                        I(0x14064d6f0) = 0;
                        I(0x14064d6f4) = 0;
                        D(0x14064d700) = r;
                        I(0x14064d6f8) = 0;
                        i32 three = 3;
                        fun_14006798b(kobj, IP(0x14064d568), &three, IP(0x14064d6f4), &cvec(1), DP(0x14064d710), IP(0x14064d730),
                                      DP(0x14064d700), IP(0x14064d734), DP(0x14064d740), DP(0x1405c6770), IP(0x14064d6f8),
                                      IP(0x14064d6f0));
                        if (D(0x14064d700) > *tol) { // 14002e8b2: JA return
                            return;
                        }
                        if (sse::mul(*tol, -4.0) > D(0x14064d700)) { // DAT_140bad4e8
                            return;
                        }
                        i32 one = 1;
                        fun_140228540(obj, inst, poly, &one, DP(0x14064d760), IP(0x14064d780));
                        one = 1;
                        fun_140228540(kobj, IP(0x14064d568), IP(0x14064d730), &one, DP(0x14064d790), IP(0x14064d7b0));
                        D(0x14064d7b8) = sse::add(sse::mul(D(0x14064d7a0), D(0x14064d770)),
                                                  sse::add(sse::mul(D(0x14064d798), D(0x14064d768)), sse::mul(D(0x14064d790), D(0x14064d760))));
                        if (D(0x14064d7b8) > -0.949999988079071) { // DAT_140bad4f0
                            return;
                        }
                        I(0x14064d588) += 1;
                        ++iv;
                    } while (I(0x14064d588) <= I(0x14064d668));
                }
            next_edge:
                I(0x14064d658) += 1;
                ++it;
            } while (I(0x14064d658) <= I(0x14064d654));
        }
        I(0x14064d570) += 1;
    } while (I(0x14064d570) <= *nsub);
done:
    {
        i32 two = 2;
        I(0x14064d7c0) = 1;
        *ei2 = ineg(*kobj);
        fun_1402280d0(obj, inst, poly, &two, ei2, IP(0x14064d7c0));
    }
}

} // namespace def::cmd::ctc3d
