// Packet p67-7, group "pipeline": stage 1 of FUN_1400512aa, FUN_14005c3a7 (EI3 per-edge BC class
// of the slave's polygons, GENCTC_3d_search.md sec. 6.1; the rule there is an outline, this is
// the transcription of the listing).
//
// Per polygon p of the slave (conn1 >= 0): W1(3, nv) = BC codes of p's nodes, a degenerate quad
// (conn1 == |conn4|) counts as 3 nodes; L = 14003a580(W1, 1..nv) the distinct common codes; the
// polygon code c1 is the LAST positive code of L (with its direction d1), or, without one, the
// last negative code (direction 0). With c1 != 0, for each edge k of p with a neighbour q > 0
// (EI1(p)[k]) that has p as neighbour on its edge j: the same is done for q, but with the node
// count of p (the binary passes p's count for q too: kept) and without the degenerate test,
// giving (c2, d2); then
//     c1 == c2, d1 != d2, c1 > 0  ->  EI3(q)[j] = EI3(p)[k] = 1
//     c1 == c2 otherwise          ->  2, 2
//     c1 != c2                    ->  1, 1
// EI3(q) is stored at once (count = q's node count); EI3(p) at the end of p (count = p's node
// count after the degenerate test). All working storage is the routine's SAVE area at
// DAT_14069473c..DAT_140694974, addressed through the image as in the binary.
#include <defcmd/ctc3d/cube.hpp>
#include <defcmd/ctc3d/pipeline.hpp>

namespace def::cmd::ctc3d {

namespace {

inline i32& I(std::uint64_t a) { return m<i32>(a); }
inline i32* P(std::uint64_t a) { return mp<i32>(a); }

constexpr std::uint64_t k_cnt = 0x140694840; // number of pairs from 14003a580
constexpr std::uint64_t k_out = 0x140694850; // pairs (code, dir), 8 bytes each
constexpr std::uint64_t k_j = 0x140694878;   // scan counter

inline i32& code_at(i32 j) { return I(k_out + 8 * (static_cast<std::int64_t>(j) - 1)); }
inline i32& dir_at(i32 j) { return I(k_out + 8 * (static_cast<std::int64_t>(j) - 1) + 4); }

// part of @src 14005c3a7: 14005c68b..14005c89d (and 14005cdf2..14005d004 for the neighbour):
// the polygon's code from the pairs, scanned as compiled (blocks of eight, then one by one; the
// pair array overlaps the scan variables from pair 5 on, so the order of reads is kept)
void scan_codes(std::uint64_t ca, std::uint64_t da)
{
    I(ca) = 0;
    I(da) = 0;
    I(k_j) = 1;
    const i32 cnt = I(k_cnt);
    const i32 r9 = (cnt / 8) * 8 + 1; // SAR/AND/ADD/SAR: division toward zero
    const bool unroll = r9 > 1;
    bool rem = false;
    if (unroll) {
        do {
            const i32 j = I(k_j);
            for (i32 t = 0; t < 8; ++t) {
                if (code_at(j + t) > 0) {
                    const i32 c = code_at(j + t);
                    const i32 d = dir_at(j + t);
                    I(ca) = c;
                    I(da) = d;
                }
            }
            I(k_j) = j + 8;
        } while (I(k_j) < r9);
        rem = I(k_cnt) >= I(k_j);
    } else {
        rem = cnt >= 1;
    }
    if (rem) {
        do {
            const i32 j = I(k_j);
            if (code_at(j) > 0) {
                const i32 c = code_at(j);
                const i32 d = dir_at(j);
                I(ca) = c;
                I(da) = d;
            }
            I(k_j) = j + 1;
        } while (I(k_cnt) >= I(k_j));
    }
    if (I(ca) != 0) {
        return;
    }
    // 14005d21f / 14005d0b9: no positive code, take the last negative one
    if (I(k_cnt) <= 0) {
        return;
    }
    I(k_j) = 1;
    rem = false;
    if (unroll) {
        do {
            const i32 j = I(k_j);
            for (i32 t = 0; t < 8; ++t) {
                if (code_at(j + t) < 0) {
                    I(ca) = code_at(j + t);
                }
            }
            I(k_j) = j + 8;
        } while (I(k_j) < r9);
        rem = I(k_cnt) >= I(k_j);
    } else {
        rem = I(k_cnt) >= 1;
    }
    if (rem) {
        do {
            const i32 j = I(k_j);
            if (code_at(j) < 0) {
                I(ca) = code_at(j);
            }
            I(k_j) = j + 1;
        } while (I(k_cnt) >= I(k_j));
    }
}

// W(3, *) at base, W(k, i) = bcc(k, node_i) for the nodes at conn_base, i = 1..*count, using
// the loop variables DAT_140694744 (i), DAT_1406947d8 (node) and DAT_1406947dc (k)
void fill_codes(std::uint64_t base, std::uint64_t conn_base, std::uint64_t count_addr,
                const i32* bcc, std::int64_t ld)
{
    do {
        const i32 i = I(0x140694744);
        const i32 nd = I(conn_base + 4 * (static_cast<std::int64_t>(i) - 1));
        I(0x1406947d8) = nd;
        I(0x1406947dc) = 1;
        do {
            const i32 k = I(0x1406947dc);
            I(0x1406947dc) = k + 1;
            I(base + 4 * (static_cast<std::int64_t>(k) - 1) + 12 * (static_cast<std::int64_t>(i) - 1)) =
                bcc[(static_cast<std::int64_t>(nd) - 1) * ld + k - 1];
        } while (I(0x1406947dc) <= 3);
        I(0x140694744) = i + 1;
    } while (I(0x140694744) <= I(count_addr));
}

} // namespace

// @src 14005c3a7  stage 1: EI3 (per-edge BC class 1/2) of the slave's polygons
void fun_14005c3a7(i32* obj, i32* inst, const i32* nface, const i32* bc, const i32* ndof)
{
    I(0x14069473c) = 1; // field EI1
    I(0x140694740) = 3; // field EI3
    I(0x140694744) = 1;
    const std::int64_t ld = *ndof >= 0 ? static_cast<std::int64_t>(*ndof) : 0;
    do {
        const i32 i = I(0x140694744);
        I(0x140694750 + 4 * (static_cast<std::int64_t>(i) - 1)) = 0;
        I(0x140694744) = i + 1;
    } while (I(0x140694744) <= 4);
    I(0x140694760) = 1;
    if (*nface < 1) {
        return;
    }
    // 14005c4a4: EI3 := (0,0,0,0) for every polygon
    do {
        i32 four = 4;
        i32 p = I(0x140694760);
        fun_1402280d0(obj, inst, &p, P(0x140694740), P(0x140694750), &four);
        I(0x140694760) = I(0x140694760) + 1;
    } while (I(0x140694760) <= *nface);
    I(0x140694760) = 1;
    do {
        // 14005c948
        i32 e1 = I(0x140694760);
        fun_140228260(obj, inst, &e1, P(0x14069473c), P(0x140694770), P(0x140694790));
        i32 e2 = I(0x140694760);
        fun_140227e00(obj, inst, &e2, P(0x1406947a0), P(0x1406947d0));
        i32 e3 = I(0x140694760);
        fun_140228260(obj, inst, &e3, P(0x140694740), P(0x140694750), P(0x1406947d4));
        if (I(0x1406947a0) >= 0) {
            const i32 c4 = I(0x1406947ac);
            const i32 a4 = c4 < 0 ? -c4 : c4;
            if (I(0x1406947a0) == a4 && I(0x1406947d0) == 4) {
                I(0x1406947d0) = 3;
                I(0x140694744) = 1;
                fill_codes(0x1406947e0, 0x1406947a0, 0x1406947d0, bc, ld);
            } else {
                I(0x140694744) = 1;
                if (I(0x1406947d0) >= 1) {
                    fill_codes(0x1406947e0, 0x1406947a0, 0x1406947d0, bc, ld);
                }
            }
            i32 three = 3;
            fun_14003a580(P(0x1406947e0), P(0x1405c7f80), P(0x1406947d0), &three, P(k_cnt), P(k_out));
            scan_codes(0x140694870, 0x140694874);
            if (I(0x140694870) != 0) {
                const i32 nedge = I(0x1406947d0); // 14005c8cd: kept in [RSP+0x88]
                I(0x14069487c) = 1;
                if (nedge >= 1) {
                    do {
                        // 14005cb59
                        const i32 k = I(0x14069487c);
                        I(0x140694880) = I(0x140694770 + 4 * (static_cast<std::int64_t>(k) - 1));
                        if (I(0x140694880) > 0) {
                            fun_140228260(obj, inst, P(0x140694880), P(0x14069473c), P(0x140694890),
                                          P(0x140694790));
                            fun_140227e00(obj, inst, P(0x140694880), P(0x1406948b0), P(0x1406948e0));
                            fun_140228260(obj, inst, P(0x140694880), P(0x140694740), P(0x1406948f0),
                                          P(0x140694900));
                            I(0x140694904) = 0;
                            I(0x140694744) = 1;
                            if (I(0x1406948e0) >= 1) {
                                do {
                                    const i32 i = I(0x140694744);
                                    if (I(0x140694890 + 4 * (static_cast<std::int64_t>(i) - 1)) ==
                                        I(0x140694760)) {
                                        I(0x140694904) = i;
                                    }
                                    // one node of the fill loop (the loop variable is shared)
                                    const i32 nd = I(0x1406948b0 + 4 * (static_cast<std::int64_t>(i) - 1));
                                    I(0x1406947d8) = nd;
                                    I(0x1406947dc) = 1;
                                    do {
                                        const i32 kk = I(0x1406947dc);
                                        I(0x1406947dc) = kk + 1;
                                        I(0x140694910 + 4 * (static_cast<std::int64_t>(kk) - 1) +
                                          12 * (static_cast<std::int64_t>(i) - 1)) =
                                            bc[(static_cast<std::int64_t>(nd) - 1) * ld + kk - 1];
                                    } while (I(0x1406947dc) <= 3);
                                    I(0x140694744) = i + 1;
                                } while (I(0x140694744) <= I(0x1406948e0));
                                const i32 jq = I(0x140694904);
                                if (jq != 0) {
                                    i32 three2 = 3;
                                    // 14005cded: node count of p (DAT_1406947d0), not of q
                                    fun_14003a580(P(0x140694910), P(0x1405c7f80), P(0x1406947d0), &three2,
                                                  P(k_cnt), P(k_out));
                                    scan_codes(0x140694970, 0x140694974);
                                    i32& eq = I(0x1406948f0 + 4 * (static_cast<std::int64_t>(jq) - 1));
                                    i32& ep = I(0x140694750 + 4 * (static_cast<std::int64_t>(k) - 1));
                                    if (I(0x140694870) == I(0x140694970) &&
                                        I(0x140694874) != I(0x140694974) && I(0x140694870) > 0) {
                                        eq = 1;
                                        ep = 1;
                                    } else {
                                        if (I(0x140694870) == I(0x140694970)) {
                                            eq = 2;
                                            ep = 2;
                                        }
                                        if (I(0x140694870) != I(0x140694970)) {
                                            eq = 1;
                                            ep = 1;
                                        }
                                    }
                                    fun_1402280d0(obj, inst, P(0x140694880), P(0x140694740), P(0x1406948f0),
                                                  P(0x1406948e0));
                                }
                            }
                        }
                        I(0x14069487c) = I(0x14069487c) + 1;
                    } while (I(0x14069487c) <= nedge);
                }
            }
            // 14005c8db
            i32 e4 = I(0x140694760);
            fun_1402280d0(obj, inst, &e4, P(0x140694740), P(0x140694750), P(0x1406947d0));
        }
        I(0x140694760) = I(0x140694760) + 1;
    } while (I(0x140694760) <= *nface);
}

} // namespace def::cmd::ctc3d
