// SRCNT5 FUN_140030d0e on the slave's polygons (GENCTC_3d_search.md sec. 7.6). Packet p67-7,
// group "srcnt".
#include "srcnt_internal.hpp"

#include <defcmd/ctc3d/cube.hpp>
#include <defcmd/ctc3d/decide.hpp>
#include <defcmd/ctc3d/pipeline.hpp>
#include <defcmd/ctc3d/search.hpp>
#include <defcmd/ctc3d/srcnt.hpp>

#include <defrt/ftn/sse_ops.hpp>

namespace def::cmd::ctc3d {

using namespace srcnt_detail;
namespace sse = def::rt::sse;

// @src 140030d0e  SRCNT5: a slave polygon with a negative last node gets EI2 = 0; one whose nodes
// share a negative contact code -k (k != obj, with a cube) gets EI2 = -k when its centroid
// projects on k within [-4 t, t] onto a polygon with opposite normal (ER1 . ER1' <= -0.95f);
// t = tol, 5 tol for EI2 = -2000. The partition branch (DAT_14075bc80 > 0) is dead in DEF_PRE.
void fun_140030d0e(i32* obj, i32* inst, i32* nface, double* tol, i32* bc, i32* ndof, i32* p7)
{
    I(0x14064f5e0) = *nface;
    I(0x14064f5dc) = 1;
    const std::int64_t ld = *ndof > 0 ? *ndof : 0;
    I(0x14064f5e4) = 0;
    const i32 nparts = I(0x14075bc80);
    if (0 < nparts && 0 < *p7) {
        // partitions: start(k) at 14064f5e8 + 8k, end(k) at 14064f5ec + 8k
        auto pstart = [](std::int64_t k) -> i32& { return I(0x14064f5e8ull + 8 * static_cast<u64>(k)); };
        auto pend = [](std::int64_t k) -> i32& { return I(0x14064f5ecull + 8 * static_cast<u64>(k)); };
        I(0x14064f5e4) = 1;
        const i32 q = *nface / nparts;
        pstart(1) = 1;
        I(0x14064fdf0) = 2;
        I(0x14064f5e8) = q;
        pend(1) = q;
        for (i32 k = 2; k <= nparts; ++k) {
            pend(k) = pend(k - 1) + q;
            pstart(k) = pend(k - 1) + 1;
        }
        I(0x14064fdf4) = I(0x14075bc84) + 1;
        pend(nparts) = *nface;
        I(0x14064f5e0) = pend(I(0x14064fdf4));
        I(0x14064f5dc) = pstart(I(0x14064fdf4));
    }
    I(0x14064fdf8) = 1;
    I(0x14064fdf0) = I(0x14064f5dc);
    if (I(0x14064f5dc) <= I(0x14064f5e0)) {
        do {
            i32 pp = I(0x14064fdf0);
            i32 f = 2;
            fun_140228260(obj, inst, &pp, &f, IP(0x14064fdfc), IP(0x14064fdf8));
            if (I(0x14064fdfc) != 0) {
                D(0x14064fe00) = *tol;
                if (I(0x14064fdfc) == -2000) {
                    D(0x14064fe00) = sse::mul(D(0x14064fe00), 5.0); // DAT_140bad498
                }
                // centroid sum S(k) at 14064fe08 + 8k, node point P(k) at 14064fe28 + 8k
                for (std::int64_t k = 1; k <= 3; ++k) {
                    D(0x14064fe08ull + 8 * static_cast<u64>(k)) = 0.0;
                    D(0x14064fe28ull + 8 * static_cast<u64>(k)) = 0.0;
                }
                I(0x14064fe08) = 4;
                pp = I(0x14064fdf0);
                fun_140227e00(obj, inst, &pp, IP(0x14064fe50), IP(0x14064fe80));
                I(0x14064fe84) = I(0x14064fe80);
                const i32 last = I(0x14064fe4cull + 4 * static_cast<std::int64_t>(I(0x14064fe80)));
                if (last < 0) {
                    i32 two = 2;
                    i32 p2 = I(0x14064fdf0);
                    i32 zero = 0;
                    fun_1402280d0(obj, inst, &p2, &two, &zero, IP(0x14064fdf8));
                } else {
                    if (iabs(I(0x14064fe50)) == iabs(last)) {
                        I(0x14064fe84) = I(0x14064fe80) - 1;
                    }
                    I(0x14064fe88) = 0;
                    I(0x14064fe8c) = 1;
                    if (0 < I(0x14064fe84)) {
                        do {
                            const std::int64_t j = I(0x14064fe8c);
                            I(0x14064fe90) = iabs(I(0x14064fe4cull + 4 * static_cast<u64>(j)));
                            fun_140227fc0(obj, inst, IP(0x14064fe90), DP(0x14064fe30));
                            for (std::int64_t k = 1; k <= 3; ++k) {
                                double& s = D(0x14064fe08ull + 8 * static_cast<u64>(k));
                                s = sse::add(s, D(0x14064fe28ull + 8 * static_cast<u64>(k)));
                            }
                            I(0x14064fe08) = 4;
                            const std::int64_t b = (static_cast<std::int64_t>(I(0x14064fe90)) - 1) * ld;
                            I(0x14064fe8c) = static_cast<i32>(j + 1);
                            // codes(:, j) at 14064fe94 + 12j (column 1 at 14064fea0)
                            I(0x14064fe94ull + 12 * static_cast<u64>(j)) = bc[b];
                            I(0x14064fe98ull + 12 * static_cast<u64>(j)) = bc[b + 1];
                            I(0x14064fe9cull + 12 * static_cast<u64>(j)) = bc[b + 2];
                        } while (I(0x14064fe8c) <= I(0x14064fe84));
                    }
                    i32 three = 3;
                    fun_14003a978(mp<i32>(0x14064fea0), IP(0x1405c67f0), IP(0x14064fe84), &three, IP(0x14064ff00), IP(0x14064ff04));
                    if (I(0x14064ff00) < 0) {
                        I(0x14064ff08) = ineg(I(0x14064ff00));
                        if (I(0x14064ff08) < 0) {
                            I(0x14064ff08) = I(0x14064ff00);
                        }
                        if (*obj != I(0x14064ff08)) {
                            I(0x14064ff0c) = I(0x140778684ull + 4 * static_cast<std::int64_t>(I(0x14064ff08)));
                            if (0 < I(0x14064ff0c)) {
                                const double nv = static_cast<double>(I(0x14064fe84));
                                I(0x14064ff10) = 0;
                                I(0x14064ff14) = 0;
                                I(0x14064ff18) = 0;
                                D(0x14064fe10) = sse::div(D(0x14064fe10), nv);
                                D(0x14064fe18) = sse::div(D(0x14064fe18), nv);
                                D(0x14064fe20) = sse::div(D(0x14064fe20), nv);
                                // the distance DAT_14064ff48 enters MDSPS2_REC with its previous value
                                three = 3;
                                fun_14006798b(IP(0x14064ff08), IP(0x14064ff0c), &three, IP(0x14064ff14), DP(0x14064fe10), DP(0x14064ff20),
                                              IP(0x14064ff40), DP(0x14064ff48), IP(0x14064ff50), DP(0x14064fe30), DP(0x1405c6800),
                                              IP(0x14064ff18), IP(0x14064ff10));
                                const double d = D(0x14064ff48);
                                const double t = D(0x14064fe00);
                                if (!(d > t) && !(d < sse::mul(t, -4.0))) { // 140031548 JA skip; 140031564 JC skip
                                    i32 one = 1;
                                    i32 p2 = I(0x14064fdf0);
                                    fun_140228540(obj, inst, &p2, &one, DP(0x14064ff60), IP(0x14064ff80));
                                    one = 1;
                                    fun_140228540(IP(0x14064ff08), IP(0x14064ff0c), IP(0x14064ff40), &one, DP(0x14064ff90), IP(0x14064ffb0));
                                    D(0x14064ffb8) = sse::add(sse::mul(D(0x14064ffa0), D(0x14064ff70)),
                                                              sse::add(sse::mul(D(0x14064ff98), D(0x14064ff68)), sse::mul(D(0x14064ff90), D(0x14064ff60))));
                                    if (!(D(0x14064ffb8) > -0.949999988079071)) { // DAT_140bad4f0; 140031685 JA skip
                                        I(0x14064fdfc) = ineg(I(0x14064ff08));
                                        i32 two = 2;
                                        p2 = I(0x14064fdf0);
                                        fun_1402280d0(obj, inst, &p2, &two, IP(0x14064fdfc), IP(0x14064fdf8));
                                    }
                                }
                            }
                        }
                    }
                }
            }
            I(0x14064fdf0) += 1;
        } while (I(0x14064fdf0) <= I(0x14064f5e0));
    }
    if (0 < I(0x14064f5e4)) {
        fun_14008a8c1();
    }
}

} // namespace def::cmd::ctc3d
