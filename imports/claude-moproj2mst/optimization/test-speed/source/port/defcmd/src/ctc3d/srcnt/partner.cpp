// Partner-partner contact FUN_14003e7f6 (GENCTC_3d_search.md sec. 7.4). Packet p67-7, group
// "srcnt".
#include "srcnt_internal.hpp"

#include <defcmd/ctc3d/cube.hpp>
#include <defcmd/ctc3d/pipeline.hpp>
#include <defcmd/ctc3d/search.hpp>
#include <defcmd/ctc3d/srcnt.hpp>

#include <defrt/ftn/fio.hpp>
#include <defrt/ftn/geoutl_pending.hpp>

namespace def::cmd::ctc3d {

using namespace srcnt_detail;
namespace fio = def::rt::ftn::fio;
namespace geo = def::rt::geoutl;

namespace {

// start / end of partition k (1-based): DAT_14066ae58 + 8k / DAT_14066ae5c + 8k
i32& part_start(std::int64_t k) { return I(0x14066ae58ull + 8 * static_cast<u64>(k)); }
i32& part_end(std::int64_t k) { return I(0x14066ae5cull + 8 * static_cast<u64>(k)); }

// box +- tol around point pt of a (into DAT_14066ae00 / DAT_14066ae20) and the candidate count of
// b's cube in it (DAT_14066ae50)
void probe(i32* a, i32* ainst, i32* b, i32* binst, i32 pt, double* tol)
{
    i32 p = pt;
    fun_140227fc0(a, ainst, &p, DP(0x14066ae00));
    i32 one_n = 1;
    i32 one_i = 1;
    geo::fun_1404d1b1c(DP(0x14066ae00), DP(0x14066ae20), tol, &one_n, &one_i);
    i32 z1 = 0;
    i32 z2 = 0;
    fun_140228c60(b, binst, DP(0x14066ae20), IP(0x14066ae50), &z1, &z2);
}

} // namespace

// @src 14003e7f6  points of a (NI3 != 0) within *tol of b's surface, or inside it, get the code -b
// and the polygon number times sign(d) in the first free NI4 slot z, y, x; message 1001 on the
// message unit when any point was coded. The partition branch (DAT_14075bc80 > 0) is dead in
// DEF_PRE and kept as in the binary.
void fun_14003e7f6(i32* a, i32* b, i32* ainst, i32* binst, i32* an, i32* bnpoly, double* tol, i32* p8)
{
    const i32 nparts = I(0x14075bc80);
    I(0x14066ade4) = *an;
    I(0x14066add8) = 0; // any point coded
    I(0x14066addc) = 0; // partitioned
    I(0x14066ade0) = 1;
    if (0 < nparts && 0 < *p8 && static_cast<i32>(static_cast<std::uint32_t>(nparts) * 15u) < I(0x14066ade4)) {
        const i32 lim = static_cast<i32>(static_cast<std::uint32_t>(nparts) * 15u);
        I(0x14066addc) = 1;
        I(0x14066ade8) = 0;
        I(0x14066adec) = 1;
        if (0 < I(0x14066ade4)) {
            do {
                i32 pt = I(0x14066adec);
                i32 f = 3;
                fun_140228830(a, ainst, &pt, &f, IP(0x14066adf0), IP(0x14066adf4));
                if (I(0x14066adf0) != 0) {
                    probe(a, ainst, b, binst, I(0x14066adec), tol);
                    if (I(0x14066ae50) != 0) {
                        I(0x14066ade8) += 1;
                    }
                }
                I(0x14066adec) += 1;
            } while (I(0x14066adec) <= *an);
        }
        bool by_count; // L_14003e92e: split by candidate points, else L_14003f1fd: equal split
        if (lim < 100) {
            I(0x14066ae54) = 100;
            by_count = !(I(0x14066ade8) < 0x65);
        } else {
            I(0x14066ae54) = lim;
            by_count = lim < I(0x14066ade8);
        }
        i32 b660 = I(0x14066b660);
        if (by_count) {
            I(0x14066ae58) = I(0x14066ade8) / nparts;
            const i32 n = *an;
            part_start(1) = 1;
            I(0x14066b660) = 1;
            I(0x14066b664) = 0;
            part_end(nparts) = n;
            I(0x14066adec) = 1;
            if (0 < n) {
                do {
                    i32 pt = I(0x14066adec);
                    i32 f = 3;
                    fun_140228830(a, ainst, &pt, &f, IP(0x14066adf0), IP(0x14066adf4));
                    if (I(0x14066adf0) != 0) {
                        probe(a, ainst, b, binst, I(0x14066adec), tol);
                        if (I(0x14066ae50) != 0) {
                            I(0x14066b664) += 1;
                            if (I(0x14066ae58) <= I(0x14066b664)) {
                                I(0x14066b660) += 1;
                                part_start(I(0x14066b660)) = I(0x14066adec);
                                I(0x14066b664) = 0;
                                if (nparts <= I(0x14066b660)) {
                                    break;
                                }
                            }
                        }
                    }
                    I(0x14066adec) += 1;
                } while (I(0x14066adec) <= *an);
            }
            const i32 m1 = nparts - 1;
            b660 = 1;
            for (i32 k = 1; k <= m1; ++k) {
                part_end(k) = part_start(k + 1) - 1;
                b660 = k + 1;
            }
        } else {
            const i32 q = *an / nparts;
            part_start(1) = 1;
            I(0x14066adec) = 2;
            I(0x14066ae58) = q;
            part_end(1) = q;
            for (i32 k = 2; k <= nparts; ++k) {
                part_end(k) = part_end(k - 1) + q;
                part_start(k) = part_end(k - 1) + 1;
                I(0x14066adec) = k + 1;
            }
            part_end(nparts) = *an;
        }
        I(0x14066b660) = b660;
        I(0x14066b668) = I(0x14075bc84) + 1;
        I(0x14066ade4) = part_end(I(0x14066b668));
        I(0x14066ade0) = part_start(I(0x14066b668));
    }

    I(0x14066adec) = I(0x14066ade0);
    if (I(0x14066ade4) < I(0x14066ade0)) {
        if (I(0x14066addc) < 1) {
            return;
        }
    } else {
        do {
            i32 pt = I(0x14066adec);
            i32 f = 3;
            fun_140228830(a, ainst, &pt, &f, IP(0x14066adf0), IP(0x14066adf4));
            if (I(0x14066adf0) != 0) {
                pt = I(0x14066adec);
                fun_140227fc0(a, ainst, &pt, DP(0x14066ae00));
                pt = I(0x14066adec);
                f = 4;
                fun_140228830(a, ainst, &pt, &f, IP(0x14066b670), IP(0x14066adf4));
                i32 one_n = 1;
                i32 one_i = 1;
                geo::fun_1404d1b1c(DP(0x14066ae00), DP(0x14066ae20), tol, &one_n, &one_i);
                i32 z1 = 0;
                i32 z2 = 0;
                fun_140228c60(b, binst, DP(0x14066ae20), IP(0x14066ae50), &z1, &z2);
                if (I(0x14066ae50) != 0) {
                    I(0x14066b698) = 0;
                    I(0x14066b69c) = 0;
                    D(0x14066b690) = 1000000.0; // DAT_140bad320
                    i32 three = 3;
                    fun_14006798b(b, binst, &three, bnpoly, DP(0x14066ae00), DP(0x14066b6a0), IP(0x14066b6c0), DP(0x14066b690),
                                  IP(0x14066b6c4), DP(0x14066b6d0), DP(0x1405c69e0), IP(0x14066b69c), IP(0x14066b698));
                    if (I(0x14066b6c4) != -1) {
                        const double d = D(0x14066b690);
                        if (dabs(d) < *tol || d < 0.0) { // 14003f026 / 14003f030
                            I(0x14066add8) = 1;
                            I(0x14066b6f0) = 1;
                            if (d < 0.0) {
                                I(0x14066b6f0) = -1;
                            }
                            const i32 code = ineg(*b);
                            const i32 pol = static_cast<i32>(static_cast<std::uint32_t>(I(0x14066b6c0)) * static_cast<std::uint32_t>(I(0x14066b6f0)));
                            if (I(0x14066b678) == 0) {
                                I(0x14066b678) = code;
                                I(0x14066b684) = pol;
                            } else if (I(0x14066b674) == 0) {
                                I(0x14066b674) = code;
                                I(0x14066b680) = pol;
                            } else if (I(0x14066b670) == 0) {
                                I(0x14066b670) = code;
                                I(0x14066b67c) = pol;
                            }
                        }
                        i32 cnt = 6;
                        f = 4;
                        pt = I(0x14066adec);
                        fun_140228690(a, ainst, &pt, &f, IP(0x14066b670), &cnt);
                    }
                }
            }
            I(0x14066adec) += 1;
        } while (I(0x14066adec) <= I(0x14066ade4));
        if (I(0x14066addc) < 1) {
            goto message;
        }
    }
    fun_14008a8c4();
message:
    if (I(0x14066add8) == 1) {
        // str 1405c6aa0: 1001( 'Info: Contact Between Object Pair',2I5,  'has been Established')
        fio::Write(dat_14077a7bc, "( 'Info: Contact Between Object Pair',2I5,  'has been Established')").i(*a).i(*b).end();
    }
}

} // namespace def::cmd::ctc3d
