// Packet p67-7, group "pipeline": stage 2 of FUN_1400512aa, FUN_140052f49 (NI5 node feature
// class of the slave, "Critical Corner Node Idenfied", GENCTC_3d_search.md sec. 6.2).
//
// 1. NI5 := 0 for nodes 1..N.
// 2. Every polygon edge k (k = 1..nv, nv - 1 for a degenerate quad, the edge runs to node k+1 or
//    back to node 1) with EI3[k] == 1: for each common code of its two nodes (14003a580 on the
//    raw node numbers), w = 1 for code 1, > 600 or 4..8, w = DAT_14076abb8[|code|] for a code
//    < 1, else w keeps its previous value (SAVE, starts at 0 per call); both nodes NI5 += w.
// 3. NI5 := NI5 / 4 (rounding toward zero).
// 4. Nodes with NI5 == 2: over all edges (a, b) of the polygons in NI1(node) (directed as
//    stored), NI5(a) == 1 and NI5(b) == 2 marks the node, NI5(a) == NI5(b) == 2 is counted; a
//    marked node with a count < 3 is listed (messages 1000/1001 on unit DAT_14077a7bc) and gets
//    NI5 = 3.
// 5. Every polygon edge with EI3 == 2: the common code of the two nodes (14003a978, node numbers
//    with abs) goes into NI5 of a node still 0 if it is negative, 4 if positive. The edge count
//    of this loop is DAT_1406769b4 as left by the earlier loops (not recomputed per polygon in
//    the binary: kept).
#include <defcmd/ctc3d/cube.hpp>
#include <defcmd/ctc3d/decide.hpp>
#include <defcmd/ctc3d/pipeline.hpp>

#include <defrt/ftn/fio.hpp>

namespace def::cmd::ctc3d {

namespace {

inline i32& I(std::uint64_t a) { return m<i32>(a); }
inline i32* P(std::uint64_t a) { return mp<i32>(a); }
inline i32 iabs(i32 v) { return v < 0 ? -v : v; }

// polygon conn at DAT_140676990 (1-based)
inline i32& conn(i32 k) { return I(0x140676990 + 4 * (static_cast<std::int64_t>(k) - 1)); }

// DAT_1406769b4 = node count, minus one for a degenerate quad (|conn(n)| == |conn(1)|)
void edge_count()
{
    const i32 n = I(0x1406769b0);
    I(0x1406769b4) = n;
    if (iabs(conn(n)) == iabs(conn(1))) {
        I(0x1406769b4) = n - 1;
    }
}

} // namespace

// @src 140052f49  stage 2: NI5 feature class of the slave's nodes, critical corner nodes
void fun_140052f49(i32* obj, i32* inst, const i32* n, const i32* bc, const i32* ndof)
{
    I(0x140676954) = 0; // w
    I(0x140676958) = 1;
    if (*n >= 1) {
        do {
            i32 node = I(0x140676958);
            i32 f5 = 5;
            i32 one = 1;
            fun_140228690(obj, inst, &node, &f5, P(0x140676954), &one);
            I(0x140676958) = I(0x140676958) + 1;
        } while (I(0x140676958) <= *n);
    }
    // 140053025: polygon count of the cube (obj, inst)
    I(0x14067695c) = dat_14076e588[*obj * 3 + *inst * 300];
    I(0x140676960) = 1;
    if (I(0x14067695c) >= 1) {
        do {
            // 1400538ab
            i32 p = I(0x140676960);
            i32 f3 = 3;
            fun_140228260(obj, inst, &p, &f3, P(0x140676970), P(0x140676980));
            i32 p2 = I(0x140676960);
            fun_140227e00(obj, inst, &p2, P(0x140676990), P(0x1406769b0));
            edge_count();
            I(0x1406769b8) = 1;
            if (I(0x1406769b4) >= 1) {
                i32 r12 = 2;
                do {
                    const i32 k = I(0x1406769b8);
                    I(0x1406769bc) = I(0x140676970 + 4 * (static_cast<std::int64_t>(k) - 1));
                    if (I(0x1406769bc) == 1) {
                        I(0x1406769c0) = conn(k);
                        I(0x1406769c4) = r12;
                        if (I(0x1406769b4) < r12) {
                            I(0x1406769c4) = 1;
                        }
                        I(0x1406769c8) = conn(I(0x1406769c4));
                        I(0x1406769d0) = I(0x1406769c0);
                        I(0x1406769d4) = I(0x1406769c8);
                        i32 two = 2;
                        fun_14003a580(bc, P(0x1406769d0), &two, ndof, P(0x1406769e0), P(0x1406769f0));
                        I(0x140676a10) = 1;
                        if (I(0x1406769e0) >= 1) {
                            do {
                                // 140053c09
                                const i32 j = I(0x140676a10);
                                I(0x140676a14) = I(0x1406769f0 + 8 * (static_cast<std::int64_t>(j) - 1));
                                i32 f5a = 5;
                                fun_140228830(obj, inst, P(0x1406769c0), &f5a, P(0x140676a18), P(0x140676a1c));
                                i32 f5b = 5;
                                fun_140228830(obj, inst, P(0x1406769c8), &f5b, P(0x140676a20), P(0x140676a1c));
                                const i32 code = I(0x140676a14);
                                if (code == 1) {
                                    I(0x140676954) = 1;
                                }
                                if (I(0x140676a14) > 600) {
                                    I(0x140676954) = 1;
                                }
                                if (I(0x140676a14) >= 4 && I(0x140676a14) <= 8) {
                                    I(0x140676954) = 1;
                                }
                                if (I(0x140676a14) < 1) {
                                    // 140053d22: symmetry plane count of object |code|
                                    I(0x140676a24) = iabs(I(0x140676a14));
                                    I(0x140676954) = dat_14076abb8[I(0x140676a24)];
                                }
                                I(0x140676a18) = I(0x140676954) + I(0x140676a18);
                                I(0x140676a20) = I(0x140676a20) + I(0x140676954);
                                i32 f5c = 5;
                                i32 one = 1;
                                fun_140228690(obj, inst, P(0x1406769c0), &f5c, P(0x140676a18), &one);
                                i32 f5d = 5;
                                i32 one2 = 1;
                                fun_140228690(obj, inst, P(0x1406769c8), &f5d, P(0x140676a20), &one2);
                                I(0x140676a10) = I(0x140676a10) + 1;
                            } while (I(0x140676a10) <= I(0x1406769e0));
                        }
                    }
                    I(0x1406769b8) = I(0x1406769b8) + 1;
                    ++r12;
                } while (I(0x1406769b8) <= I(0x1406769b4));
            }
            I(0x140676960) = I(0x140676960) + 1;
        } while (I(0x140676960) <= I(0x14067695c));
    }
    // 14005306d: NI5 := NI5 / 4
    I(0x140676958) = 1;
    if (*n >= 1) {
        do {
            i32 node = I(0x140676958);
            i32 f5 = 5;
            fun_140228830(obj, inst, &node, &f5, P(0x140676a28), P(0x140676a1c));
            I(0x140676a28) = I(0x140676a28) / 4;
            i32 node2 = I(0x140676958);
            i32 f5b = 5;
            fun_140228690(obj, inst, &node2, &f5b, P(0x140676a28), P(0x140676a1c));
            I(0x140676958) = I(0x140676958) + 1;
        } while (I(0x140676958) <= *n);
    }
    // 140053162: critical corner nodes
    I(0x140676a2c) = 0;
    I(0x140676958) = 1;
    if (*n >= 1) {
        do {
            i32 node = I(0x140676958);
            i32 f5 = 5;
            fun_140228830(obj, inst, &node, &f5, P(0x140676a28), P(0x140676a1c));
            const i32 cls = I(0x140676a28); // R12D
            if (cls == 2) {
                // 140053d55
                i32 node2 = I(0x140676958);
                i32 f1 = 1;
                fun_140228830(obj, inst, &node2, &f1, P(0x140676a30), P(0x1406779d0));
                I(0x1406779d4) = cls;
                I(0x1406779d8) = 0;
                I(0x1406779dc) = 0;
                I(0x1406779e0) = 1;
                if (I(0x1406779d0) >= 1) {
                    do {
                        const i32 j = I(0x1406779e0);
                        I(0x140676960) = I(0x140676a30 + 4 * (static_cast<std::int64_t>(j) - 1));
                        fun_140227e00(obj, inst, P(0x140676960), P(0x140676990), P(0x1406769b0));
                        edge_count();
                        I(0x1406779e4) = 1;
                        if (I(0x1406769b4) >= 1) {
                            i32 r12 = 2;
                            do {
                                I(0x1406779e8) = I(0x1406779e4);
                                I(0x1406779ec) = r12;
                                if (I(0x1406769b4) < r12) {
                                    I(0x1406779ec) = 1;
                                }
                                I(0x1406769c0) = conn(I(0x1406779e4));
                                I(0x1406769c8) = conn(I(0x1406779ec));
                                i32 f5a = 5;
                                fun_140228830(obj, inst, P(0x1406769c0), &f5a, P(0x140676a18), P(0x140676a1c));
                                i32 f5b = 5;
                                fun_140228830(obj, inst, P(0x1406769c8), &f5b, P(0x140676a20), P(0x1406779f0));
                                if (I(0x140676a18) == 1 && I(0x140676a20) == 2) {
                                    I(0x1406779d4) = 3;
                                }
                                if (I(0x140676a18) == 2 && I(0x140676a20) == 2) {
                                    I(0x1406779d8) = I(0x1406779d8) + 1;
                                }
                                I(0x1406779e4) = I(0x1406779e4) + 1;
                                ++r12;
                            } while (I(0x1406779e4) <= I(0x1406769b4));
                        }
                        I(0x1406779e0) = I(0x1406779e0) + 1;
                    } while (I(0x1406779e0) <= I(0x1406779d0));
                    // 1400540b7
                    if (I(0x1406779d4) == 3 && I(0x1406779d8) <= 2) {
                        I(0x140676a2c) = I(0x140676a2c) + 1;
                        I(0x140677a00 + 4 * (static_cast<std::int64_t>(I(0x140676a2c)) - 1)) =
                            I(0x140676958);
                    }
                }
                // 140053de4: with an empty NI1 the binary tests the register copy of NI5 (== 2)
                // against 3: never listed
            }
            I(0x140676958) = I(0x140676958) + 1;
        } while (I(0x140676958) <= *n);
    }
    I(0x1406779d4) = 3;
    if (I(0x140676a2c) > 0) {
        // 14005322b: WRITE(DAT_14077a7bc, 1000) obj    (str 1405c7a20)
        def::rt::ftn::fio::Write(dat_14077a7bc,
                                 "(' Info: Critical Corner Node Idenfied - Object Number :'       , I5)")
            .i(*obj)
            .end();
        I(0x140676958) = 1;
        if (I(0x140676a2c) >= 1) {
            do {
                I(0x1406789a0) = I(0x140677a00 + 4 * (static_cast<std::int64_t>(I(0x140676958)) - 1));
                // WRITE(DAT_14077a7bc, 1001) node    (str 1405c7ae0)
                def::rt::ftn::fio::Write(dat_14077a7bc, "(' Info: Node: ', I7)").i(I(0x1406789a0)).end();
                i32 f5 = 5;
                fun_140228690(obj, inst, P(0x1406789a0), &f5, P(0x1406779d4), P(0x140676a1c));
                I(0x140676958) = I(0x140676958) + 1;
            } while (I(0x140676958) <= I(0x140676a2c));
        }
    }
    // 140053461: edges with EI3 == 2
    I(0x140676960) = 1;
    if (I(0x14067695c) >= 1) {
        do {
            i32 p = I(0x140676960);
            fun_140227e00(obj, inst, &p, P(0x140676990), P(0x1406769b0));
            i32 p2 = I(0x140676960);
            i32 f3 = 3;
            fun_140228260(obj, inst, &p2, &f3, P(0x140676970), P(0x140676980));
            I(0x1406769b8) = 1;
            if (I(0x1406769b4) >= 1) {
                i32 r12 = 2;
                do {
                    const i32 k = I(0x1406769b8);
                    I(0x1406769bc) = I(0x140676970 + 4 * (static_cast<std::int64_t>(k) - 1));
                    if (I(0x1406769bc) == 2) {
                        I(0x1406769c0) = iabs(conn(k));
                        I(0x1406769c4) = r12;
                        if (I(0x1406769b4) < r12) {
                            I(0x1406769c4) = 1;
                        }
                        I(0x1406769c8) = iabs(conn(I(0x1406769c4)));
                        I(0x1406769d0) = I(0x1406769c0);
                        I(0x1406769d4) = I(0x1406769c8);
                        i32 two = 2;
                        fun_14003a978(bc, P(0x1406769d0), &two, ndof, P(0x140676a14), P(0x1406789a4));
                        i32 f5a = 5;
                        fun_140228830(obj, inst, P(0x1406769c0), &f5a, P(0x140676a18), P(0x140676a1c));
                        i32 f5b = 5;
                        fun_140228830(obj, inst, P(0x1406769c8), &f5b, P(0x140676a20), P(0x140676a1c));
                        if (I(0x140676a18) == 0 && I(0x140676a14) < 0) {
                            I(0x140676a18) = I(0x140676a14);
                        }
                        if (I(0x140676a20) == 0 && I(0x140676a14) < 0) {
                            I(0x140676a20) = I(0x140676a14);
                        }
                        if (I(0x140676a18) == 0 && I(0x140676a14) > 0) {
                            I(0x140676a18) = 4;
                        }
                        if (I(0x140676a20) == 0 && I(0x140676a14) > 0) {
                            I(0x140676a20) = 4;
                        }
                        i32 f5c = 5;
                        fun_140228690(obj, inst, P(0x1406769c0), &f5c, P(0x140676a18), P(0x140676a1c));
                        i32 f5d = 5;
                        fun_140228690(obj, inst, P(0x1406769c8), &f5d, P(0x140676a20), P(0x140676a1c));
                    }
                    I(0x1406769b8) = I(0x1406769b8) + 1;
                    ++r12;
                } while (I(0x1406769b8) <= I(0x1406769b4));
            }
            I(0x140676960) = I(0x140676960) + 1;
        } while (I(0x140676960) <= I(0x14067695c));
    }
}

} // namespace def::cmd::ctc3d
