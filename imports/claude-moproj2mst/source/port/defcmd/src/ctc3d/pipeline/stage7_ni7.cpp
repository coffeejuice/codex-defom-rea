// Packet p67-7, group "pipeline": stage 7 of FUN_1400512aa, partner feature nodes (NI7),
// FUN_14003fadc with FUN_1400501d7 (GENCTC_3d_search.md sec. 8).
//
// NI7 of a partner node = (class, object): class 1 = plain node, 2 = end node of a boundary edge
// (an edge of the partner's polygons with EI2 < 1 that no other such polygon shares); object = the
// object behind that edge (a negative NI4 contact code of the node, or |EI2| of the polygon for
// -1000 < EI2 < 0). FUN_14003fadc then copies |EI2| of contact polygons to their nodes and, for
// disabled polygons (EI2 == 0) with two or more class-2 nodes that are not all tagged, projects
// the polygon centroid (MDSPS2_REC mode 1) onto every other partner whose box overlaps the
// centroid's box and tags both polygons' nodes with each other's object when the nearest
// distance is within r = sqrt(min ER2 of the slave).
#include <defcmd/ctc3d/cube.hpp>
#include <defcmd/ctc3d/pipeline.hpp>
#include <defcmd/ctc3d/search.hpp>

#include <defrt/ftn/geoutl_pending.hpp>
#include <defrt/ftn/sse_ops.hpp>

#include <cmath>
#include <vector>

namespace def::cmd::ctc3d {

namespace {

inline i32& I(std::uint64_t a) { return m<i32>(a); }
inline i32* P(std::uint64_t a) { return mp<i32>(a); }
inline double& D(std::uint64_t a) { return m<double>(a); }
inline i32 iabs(i32 v) { return v < 0 ? -v : v; }
inline std::int64_t at(i32 k) { return static_cast<std::int64_t>(k) - 1; }

} // namespace

// @src 1400501d7  NI7 of partner *obj: (1, 0) for all points, (2, object) for the end nodes of
// the single (unshared) edges of its polygons with EI2 < 1
void fun_1400501d7(i32* obj, i32* inst, const i32* npts, const i32* nface)
{
    // 1400501f7: the dope vector of the allocatable edge array E(3, 4*nface) at DAT_1406765a0 is
    // reset; the port keeps E in a local vector (the binary's E is uninitialised scratch, only
    // the entries 1..DAT_140676618 written here are read)
    I(0x140676600) = 0;
    I(0x140676604) = 0;
    I(0x140676610) = 2;
    I(0x140676614) = 1;
    if (*npts >= 1) {
        do {
            i32 node = I(0x140676614);
            i32 f7 = 7;
            fun_140228690(obj, inst, &node, &f7, P(0x140676600), P(0x140676610));
            I(0x140676614) = I(0x140676614) + 1;
        } while (I(0x140676614) <= *npts);
    }
    // 140050356: extent 4*nface (32-bit shift), at least 0
    std::int64_t extent = static_cast<std::int32_t>(static_cast<std::uint32_t>(*nface) << 2);
    if (extent < 0) {
        extent = 0;
    }
    std::vector<i32> e(static_cast<std::size_t>(extent) * 3 + 3, 0);
    auto E = [&e](i32 r, i32 j) -> i32& { return e[static_cast<std::size_t>(at(j) * 3 + at(r))]; };
    I(0x140676618) = 0;
    I(0x140676614) = 1;
    if (*nface >= 1) {
        do {
            // 140050b12
            i32 p = I(0x140676614);
            fun_140227e00(obj, inst, &p, P(0x140676620), P(0x140676630));
            I(0x140676634) = 1;
            if (I(0x140676630) >= 1) {
                do {
                    I(0x140676638) = iabs(I(0x140676620 + 4 * at(I(0x140676634))));
                    I(0x140676600) = 1;
                    i32 f7 = 7;
                    i32 two = 2;
                    fun_140228690(obj, inst, P(0x140676638), &f7, P(0x140676600), &two);
                    I(0x140676634) = I(0x140676634) + 1;
                } while (I(0x140676634) <= I(0x140676630));
            }
            i32 p2 = I(0x140676614);
            i32 f2 = 2;
            fun_140228260(obj, inst, &p2, &f2, P(0x14067663c), P(0x140676640));
            if (I(0x14067663c) < 1) {
                const i32 nn = I(0x140676630);
                I(0x140676644) = nn;
                if (I(0x140676620) == iabs(I(0x140676620 + 4 * at(nn)))) {
                    I(0x140676644) = nn - 1;
                }
                I(0x140676634) = 1;
                if (I(0x140676644) >= 1) {
                    i32 r = 2;
                    do {
                        const i32 last = I(0x140676644);
                        const i32 poly = I(0x140676614);
                        I(0x140676648) = iabs(I(0x140676620 + 4 * at(I(0x140676634))));
                        I(0x14067664c) = r;
                        if (I(0x140676644) < r) {
                            I(0x14067664c) = 1;
                        }
                        I(0x140676650) = iabs(I(0x140676620 + 4 * at(I(0x14067664c))));
                        I(0x140676618) = I(0x140676618) + 1;
                        I(0x140676654) = I(0x140676648);
                        I(0x140676658) = I(0x140676650);
                        if (I(0x140676648) > I(0x140676650)) {
                            I(0x140676654) = I(0x140676650);
                            I(0x140676658) = I(0x140676648);
                        }
                        const i32 j = I(0x140676618);
                        E(1, j) = I(0x140676654);
                        E(2, j) = I(0x140676658);
                        E(3, j) = poly;
                        ++r;
                        I(0x140676634) = I(0x140676634) + 1;
                        if (I(0x140676634) > last) {
                            break;
                        }
                    } while (true);
                }
            }
            I(0x140676614) = I(0x140676614) + 1;
        } while (I(0x140676614) <= *nface);
    }
    // 14005048e: sort the edges (ascending, rows 1, 2, 3)
    {
        i32 three = 3;
        i32 one = 1;
        fun_14008b65b(e.data(), &three, P(0x140676618), &one);
    }
    // 140050493: an edge found twice: both copies get a negative polygon number
    I(0x140676614) = 1;
    {
        const i32 last = I(0x140676618) - 1;
        if (last >= 1) {
            i32 r = 2;
            do {
                const i32 j = I(0x140676614);
                I(0x14067665c) = r;
                if (E(1, j) == E(1, r) && E(2, j) == E(2, r)) {
                    E(3, j) = -iabs(E(3, j));
                    E(3, r) = -iabs(E(3, r));
                }
                ++r;
                I(0x140676614) = j + 1;
            } while (I(0x140676614) <= last);
        }
    }
    // 14005059e: the end nodes of single edges
    I(0x140676614) = 1;
    if (I(0x140676618) >= 1) {
        do {
            const i32 j = I(0x140676614);
            const i32 poly = E(3, j);
            if (poly > 0) {
                I(0x140676660) = poly;
                I(0x140676634) = 1;
                do {
                    I(0x140676638) = E(I(0x140676634), j);
                    i32 f7 = 7;
                    fun_140228830(obj, inst, P(0x140676638), &f7, P(0x140676600), P(0x140676664));
                    I(0x140676600) = 2;
                    i32 f4 = 4;
                    fun_140228830(obj, inst, P(0x140676638), &f4, P(0x140676670), P(0x140676690));
                    I(0x140676694) = 0;
                    if (I(0x140676670) < 0) {
                        I(0x140676694) = 1;
                    }
                    if (I(0x140676674) < 0) {
                        I(0x140676694) = 2;
                    }
                    if (I(0x140676678) < 0) {
                        I(0x140676694) = 3;
                    }
                    if (I(0x140676694) != 0) {
                        // 1400506cd: object of the last negative contact code
                        I(0x140676604) = iabs(I(0x140676670 + 4 * at(I(0x140676694))));
                    } else {
                        i32 f2 = 2;
                        fun_140228260(obj, inst, P(0x140676660), &f2, P(0x14067663c), P(0x140676640));
                        const i32 ei2 = I(0x14067663c);
                        if (ei2 < 0 && ei2 > -1000) {
                            I(0x140676604) = iabs(ei2);
                        }
                    }
                    i32 f7b = 7;
                    i32 two = 2;
                    fun_140228690(obj, inst, P(0x140676638), &f7b, P(0x140676600), &two);
                    I(0x140676634) = I(0x140676634) + 1;
                } while (I(0x140676634) <= 2);
            }
            I(0x140676614) = I(0x140676614) + 1;
        } while (I(0x140676614) <= I(0x140676618));
    }
    // 1400505d4: DEALLOCATE (FUN_1404b6d74): the local vector goes out of scope
}

// @src 14003fadc  stage 7: NI7 feature flags of the partners' nodes
void fun_14003fadc(i32* obj, i32* inst, const i32* n, const i32* nface, i32* dbg)
{
    (void)n;
    namespace sse = def::rt::sse;
    // r = sqrt(min ER2 over the slave's polygons), start 1e8 (DAT_140bad808)
    I(0x14066b850) = 1;
    D(0x14066b848) = 1.0e8;
    if (*nface >= 1) {
        do {
            i32 p = I(0x14066b850);
            i32 f2 = 2;
            fun_140228540(obj, inst, &p, &f2, mp<double>(0x14066b858), P(0x14066b860));
            if (D(0x14066b858) <= D(0x14066b848)) {
                D(0x14066b848) = D(0x14066b858);
            }
            I(0x14066b850) = I(0x14066b850) + 1;
        } while (I(0x14066b850) <= *nface);
    }
    const i32 npart = dat_14077867c; // 14003fb69: read once
    D(0x14066b868) = std::sqrt(D(0x14066b848));
    I(0x14066b870) = 1;
    if (npart < 1) {
        return;
    }
    do {
        const i32 k = dat_140771918[I(0x14066b870)];
        I(0x14066b874) = k;
        I(0x14066b878) = dat_140778684[k];
        I(0x14066b87c) = dat_14076e588[k * 3 + I(0x14066b878) * 300];
        I(0x14066b880) = dat_14076b6a8[k * 3 + I(0x14066b878) * 300];
        if (I(0x14066b87c) != 0 && I(0x14066b880) != 0) {
            fun_1400501d7(P(0x14066b874), P(0x14066b878), P(0x14066b880), P(0x14066b87c));
            I(0x14066b884) = 1;
            if (I(0x14066b87c) >= 1) {
                do {
                    i32 p = I(0x14066b884);
                    i32 f2 = 2;
                    fun_140228260(P(0x14066b874), P(0x14066b878), &p, &f2, P(0x14066b888), P(0x14066b88c));
                    i32 p2 = I(0x14066b884);
                    fun_14004f363(P(0x14066b874), P(0x14066b878), &p2, mp<double>(0x14066b890),
                                  P(0x14066b950), P(0x14066b960));
                    const i32 cnt = I(0x14066b960);
                    I(0x14066b964) = cnt;
                    if (I(0x14066b950) == iabs(I(0x14066b950 + 4 * at(cnt)))) {
                        I(0x14066b964) = cnt - 1;
                    }
                    const i32 ei2 = I(0x14066b888);
                    if (ei2 > -1000 && ei2 != 1) {
                        if (ei2 < 0) {
                            // contact polygon: NI7[2] := |EI2| for its nodes
                            I(0x14066b968) = 1;
                            if (I(0x14066b964) >= 1) {
                                const i32 obj2 = iabs(I(0x14066b888));
                                do {
                                    I(0x14066b96c) = iabs(I(0x14066b950 + 4 * at(I(0x14066b968))));
                                    i32 f7 = 7;
                                    fun_140228830(P(0x14066b874), P(0x14066b878), P(0x14066b96c), &f7,
                                                  P(0x14066b970), P(0x14066b980));
                                    I(0x14066b974) = obj2;
                                    i32 f7b = 7;
                                    fun_140228690(P(0x14066b874), P(0x14066b878), P(0x14066b96c), &f7b,
                                                  P(0x14066b970), P(0x14066b980));
                                    I(0x14066b968) = I(0x14066b968) + 1;
                                } while (I(0x14066b968) <= I(0x14066b964));
                            }
                        } else {
                            I(0x14066b9b0) = 0;
                            I(0x14066b9b4) = 0;
                            I(0x14066b968) = 1;
                            D(0x14066b990) = 0.0;
                            D(0x14066b998) = 0.0;
                            D(0x14066b9a0) = 0.0;
                            if (I(0x14066b964) >= 1) {
                                double s1 = 0.0;
                                double s2 = 0.0;
                                double s3 = 0.0;
                                do {
                                    const std::int64_t j = at(I(0x14066b968));
                                    // 140040005: X(c, j) + S_c (ADDSD X, [S])
                                    s1 = sse::add(D(0x14066b890 + 24 * j), D(0x14066b990));
                                    D(0x14066b990) = s1;
                                    s2 = sse::add(D(0x14066b898 + 24 * j), D(0x14066b998));
                                    D(0x14066b998) = s2;
                                    s3 = sse::add(D(0x14066b8a0 + 24 * j), D(0x14066b9a0));
                                    D(0x14066b9a0) = s3;
                                    I(0x14066b96c) = iabs(I(0x14066b950 + 4 * j));
                                    i32 f7 = 7;
                                    fun_140228830(P(0x14066b874), P(0x14066b878), P(0x14066b96c), &f7,
                                                  P(0x14066b970), P(0x14066b980));
                                    if (I(0x14066b970) == 2) {
                                        I(0x14066b9b0) = I(0x14066b9b0) + 1;
                                    }
                                    if (I(0x14066b974) > 0) {
                                        I(0x14066b9b4) = I(0x14066b9b4) + 1;
                                    }
                                    I(0x14066b968) = I(0x14066b968) + 1;
                                } while (I(0x14066b968) <= I(0x14066b964));
                                if (I(0x14066b9b0) > 1 && I(0x14066b9b4) != I(0x14066b964)) {
                                    // 14004012f: centroid = S / (double)nv
                                    const double dn = static_cast<double>(I(0x14066b964));
                                    D(0x14066b9a0) = sse::div(s3, dn);
                                    D(0x14066b998) = sse::div(s2, dn);
                                    D(0x14066b990) = sse::div(s1, dn);
                                    def::rt::geoutl::fun_1404d1b1c(mp<double>(0x14066b890), mp<double>(0x14066b9c0),
                                                                   mp<double>(0x14066b868), P(0x14066b964),
                                                                   P(0x1405c6be0));
                                    const i32 npart2 = dat_14077867c; // 1400401dd: read once
                                    I(0x14066b9f8) = 1; // MDSPS2 mode
                                    I(0x14066b9fc) = 0;
                                    I(0x14066ba00) = 1;
                                    D(0x14066b9f0) = 1.0e5; // DAT_140bad3c8
                                    if (npart2 >= 1) {
                                        do {
                                            if (I(0x14066b870) != I(0x14066ba00)) {
                                                const i32 k2 = dat_140771918[I(0x14066ba00)];
                                                I(0x14066ba04) = k2;
                                                I(0x14066ba08) = dat_140778684[k2];
                                                const std::int64_t ix = static_cast<std::int64_t>(k2) * 3 +
                                                                        static_cast<std::int64_t>(I(0x14066ba08)) * 300;
                                                I(0x14066ba0c) = m<i32>(0x14076e588 + 4 * ix);
                                                I(0x14066ba10) = m<i32>(0x14076b6a8 + 4 * ix);
                                                if (I(0x14066ba0c) != 0 && I(0x14066ba10) != 0) {
                                                    const std::uint64_t box =
                                                        0x14075a9b8 +
                                                        8 * static_cast<std::uint64_t>(static_cast<std::int64_t>(k2) * 6 +
                                                                                       static_cast<std::int64_t>(I(0x14066ba08)) * 600);
                                                    def::rt::geoutl::fun_1404d1ed7(mp<double>(0x14066b9c0),
                                                                                   mp<double>(box), P(0x14066ba14));
                                                    I(0x14066ba18) = 0;
                                                    if (I(0x14066ba14) != 0) {
                                                        i32 three = 3;
                                                        D(0x14066ba20) = 1.0e5; // DAT_140bad3c8
                                                        fun_14006798b(P(0x14066ba04), P(0x14066ba08), &three,
                                                                      P(0x14066ba0c), mp<double>(0x14066b990),
                                                                      mp<double>(0x14066ba30), P(0x14066ba18),
                                                                      mp<double>(0x14066ba20), P(0x14066ba50),
                                                                      mp<double>(0x14066ba60), mp<double>(0x1405c6bf0),
                                                                      P(0x14066b9f8), dbg);
                                                        // 1400407a3: ANDPD with DAT_140bad200
                                                        const double ad = std::fabs(D(0x14066ba20));
                                                        if (ad <= D(0x14066b9f0)) {
                                                            I(0x14066b9fc) = I(0x14066ba04);
                                                            D(0x14066b9f0) = ad;
                                                        }
                                                    }
                                                }
                                            }
                                            I(0x14066ba00) = I(0x14066ba00) + 1;
                                        } while (I(0x14066ba00) <= npart2);
                                    }
                                    // 14004021d: uses the LAST partner looked at (DAT_14066ba04)
                                    // and its polygon, not the nearest one (DAT_14066b9fc): kept
                                    if (D(0x14066b9f0) <= D(0x14066b868) && I(0x14066ba18) > 0) {
                                        I(0x14066b968) = 1;
                                        if (I(0x14066b964) >= 1) {
                                            do {
                                                I(0x14066b96c) = iabs(I(0x14066b950 + 4 * at(I(0x14066b968))));
                                                i32 f7 = 7;
                                                fun_140228830(P(0x14066b874), P(0x14066b878), P(0x14066b96c), &f7,
                                                              P(0x14066b970), P(0x14066b980));
                                                I(0x14066b974) = I(0x14066ba04);
                                                i32 f7b = 7;
                                                fun_140228690(P(0x14066b874), P(0x14066b878), P(0x14066b96c), &f7b,
                                                              P(0x14066b970), P(0x14066b980));
                                                I(0x14066b968) = I(0x14066b968) + 1;
                                            } while (I(0x14066b968) <= I(0x14066b964));
                                        }
                                        fun_140227e00(P(0x14066ba04), P(0x14066ba08), P(0x14066ba18),
                                                      P(0x14066b950), P(0x14066ba80));
                                        I(0x14066b968) = 1;
                                        if (I(0x14066ba80) >= 1) {
                                            do {
                                                I(0x14066b96c) = iabs(I(0x14066b950 + 4 * at(I(0x14066b968))));
                                                i32 f7 = 7;
                                                fun_140228830(P(0x14066ba04), P(0x14066ba08), P(0x14066b96c), &f7,
                                                              P(0x14066b970), P(0x14066b980));
                                                I(0x14066b974) = I(0x14066b874);
                                                i32 f7b = 7;
                                                fun_140228690(P(0x14066ba04), P(0x14066ba08), P(0x14066b96c), &f7b,
                                                              P(0x14066b970), P(0x14066b980));
                                                I(0x14066b968) = I(0x14066b968) + 1;
                                            } while (I(0x14066b968) <= I(0x14066ba80));
                                        }
                                    }
                                }
                            }
                        }
                    }
                    I(0x14066b884) = I(0x14066b884) + 1;
                } while (I(0x14066b884) <= I(0x14066b87c));
            }
        }
        I(0x14066b870) = I(0x14066b870) + 1;
    } while (I(0x14066b870) <= npart);
}

} // namespace def::cmd::ctc3d
