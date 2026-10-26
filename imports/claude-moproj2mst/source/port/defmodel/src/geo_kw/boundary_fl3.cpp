// kw-g2geo boundary family: FL3_FNDBND, the boundary faces of a 3D mesh (Fortran module
// procedure 140354ed0 with the kernels 140355cbf (tets, FL3_S_FEMSU) and 14035672f (bricks,
// possibly degenerate: wedges and tets stored as bricks)). Module variables used only as loop
// counters inside one call are locals here. The timing output (FUN_140235710 and the
// "FL3_S_FEMSU / FL3_FNDBND TIME" WRITEs) is not ported: the only caller (1401fc290) passes 0.
//
// Argument lists of the kernels come from the disassembly of 140354ed0 (Ghidra's decompile of
// the caller shows only 4 of them):
//   140355cbf(nelem, conn, nnpe, &nfn, &nfe, nface, w16, w8, faces, elem, table, &flag)
//   14035672f(nelem, conn, nnpe, &nfn, &nfe, nface, w16, w8b, w8, faces, elem, hex_table,
//             tet_table, wedge_table, flag)
// with w16 = DAT_140703ef0 (4 ints per element face), w8 = DAT_140703e90 (key, slot) pairs,
// w8b = DAT_140703f50 (element, element) pairs, each with nfe * nelem entries.
// All arithmetic is integer (no floating point in this family).
#include <defmodel/geo_kw/boundary.hpp>

#include <cstdint>
#include <vector>

namespace def::model::geo_kw {

namespace {

// Local face node tables (Fortran DATA, read with DumpTable at 0x140604f10..0x140605150).
// @data 140604f10  tet faces as used by the brick kernel (3 per face, 6 rows, last 2 zero)
constexpr std::int32_t kTetFaces3[6][3] = {{1, 3, 2}, {1, 2, 4}, {1, 4, 3}, {2, 3, 4}, {0, 0, 0}, {0, 0, 0}};
// @data 140604fd0  brick faces (4 per face)
constexpr std::int32_t kHexFaces[6][4] = {{1, 4, 3, 2}, {5, 6, 7, 8}, {1, 2, 6, 5},
                                          {2, 3, 7, 6}, {3, 4, 8, 7}, {4, 1, 5, 8}};
// @data 140605030  wedge faces (4 per face, triangles repeat their first node)
constexpr std::int32_t kWedgeFaces[6][4] = {{1, 3, 2, 1}, {4, 5, 6, 4}, {1, 2, 5, 4},
                                            {2, 3, 6, 5}, {3, 1, 4, 6}, {3, 3, 6, 6}};
// @data 140605090  4-node tet faces for the tet kernel (nfn = 3)
constexpr std::int32_t kTet4Table[12] = {1, 3, 2, 1, 2, 4, 1, 4, 3, 2, 3, 4};
// @data 1406050f0  10-node tet faces for the tet kernel (nfn = 6; only the first 3 are used)
constexpr std::int32_t kTet10Table[24] = {1, 6, 5, 4, 3, 2, 5, 9, 10, 8, 3, 4,
                                          10, 9, 5, 6, 1, 7, 3, 8, 10, 7, 1, 2};

using i64 = std::int64_t;

// @src 140355cbf  FL3_S_FEMSU for tets: element faces (first 3 table nodes, 4th only when
// nfn == 4), sort by node sum, drop each face that has an equal-sum later face containing all
// its nodes (and that face), output the rest; triangles get their 4th node = 1st.
void femsu_tet(const std::int32_t* nelem, const std::int32_t* conn, const std::int32_t* nnpe,
               const std::int32_t* nfn_p, const std::int32_t* nfe, std::int32_t* nface,
               std::int32_t* w16, std::int32_t* w8, std::int32_t* faces, std::int32_t* elem,
               const std::int32_t* table)
{
    const std::int32_t nfn = *nfn_p;  // uVar1
    const i64 ld_t = nfn < 0 ? 0 : nfn;     // lVar12
    const i64 ld_c = *nnpe < 0 ? 0 : *nnpe;  // lVar9
    auto T = [&](std::int32_t k, std::int32_t f) { return table[(f - 1) * ld_t + (k - 1)]; };
    auto CN = [&](std::int32_t k, std::int32_t e) { return conn[(e - 1) * ld_c + (k - 1)]; };
    // w16 flat 1-based: F(l, s) = w16[(s-1)*4 + l-1] (l may exceed 4 for nfn = 6: next face)
    auto F = [&](std::int32_t l, std::int32_t s) -> std::int32_t& { return w16[(i64(s) - 1) * 4 + (l - 1)]; };
    auto K = [&](std::int32_t j, std::int32_t s) -> std::int32_t& { return w8[(i64(s) - 1) * 2 + (j - 1)]; };

    std::int32_t cnt = 0;  // DAT_14070403c
    for (std::int32_t e = 1; e <= *nelem; ++e) {
        for (std::int32_t f = 1; f <= *nfe; ++f) {
            ++cnt;
            const std::int32_t n1 = CN(T(1, f), e);
            const std::int32_t n3 = CN(T(3, f), e);
            const std::int32_t n2 = CN(T(2, f), e);
            K(1, cnt) = 0;
            F(1, cnt) = n1;
            F(2, cnt) = n2;
            F(3, cnt) = n3;
            std::int32_t n4 = 0;
            if (nfn == 4) {
                n4 = CN(T(4, f), e);
                F(4, cnt) = n4;
            } else {
                F(4, cnt) = 0;
            }
            K(2, cnt) = cnt;
            // 140355e9a: ((n1 + n2) + n3) + n4, 32-bit wraparound
            K(1, cnt) = static_cast<std::int32_t>(static_cast<std::uint32_t>(n1) + static_cast<std::uint32_t>(n2) +
                                                  static_cast<std::uint32_t>(n3) + static_cast<std::uint32_t>(n4));
        }
    }

    sort_key_pairs(w8, &cnt);  // 14043c760

    *nface = 0;
    for (std::int32_t i = 1; i <= cnt; ++i) {
        const std::int32_t s = K(2, i);
        if (F(1, s) == 0) continue;
        bool dropped = false;
        if (i != cnt) {
            std::int32_t m = i + 1;  // DAT_14070404c: first later entry with a larger key
            while (m <= cnt && !(K(1, i) < K(1, m))) ++m;
            for (std::int32_t j = i + 1; j <= m - 1 && !dropped; ++j) {
                const std::int32_t t = K(2, j);
                std::int32_t l = 1;
                if (nfn < 1) {
                    F(1, t) = 0;
                    dropped = true;
                    break;
                }
                for (;;) {
                    const std::int32_t v = F(l, s);  // DAT_140704054
                    if (!(v == F(1, t) || F(2, t) == v || F(3, t) == v || (nfn == 4 && F(4, t) == v)))
                        break;  // face t lacks node v
                    ++l;
                    if (nfn < l) {
                        F(1, t) = 0;  // LAB_140356329
                        dropped = true;
                        break;
                    }
                }
            }
        }
        if (dropped) continue;  // LAB_1403560bf: face s is not output
        const std::int32_t n = ++*nface;
        faces[(i64(n) - 1) * 4 + 2] = F(3, s);
        faces[(i64(n) - 1) * 4 + 1] = F(2, s);
        faces[(i64(n) - 1) * 4 + 0] = F(1, s);
        if (nfn == 4) faces[(i64(n) - 1) * 4 + 3] = F(4, s);
        elem[n - 1] = (s + -1) / *nfe + 1;
    }
    if (nfn == 3) {
        for (std::int32_t n = 1; n <= *nface; ++n) faces[(i64(n) - 1) * 4 + 3] = faces[(i64(n) - 1) * 4 + 0];
    }
}

// @src 14035672f  FL3_S_FEMSU for bricks: classify each element (degenerate brick -> wedge or
// tet), take its faces from the matching table, keep faces with fewer than 2 equal node pairs
// and fewer than 2 zero nodes, sort by node sum, drop matching pairs as the tet kernel (node
// counts per face from the elements), output, then fix up triangles.
std::int32_t femsu_hex(const std::int32_t* nelem, const std::int32_t* conn, const std::int32_t* nnpe_p,
                       const std::int32_t* nfn_p, const std::int32_t* nfe, std::int32_t* nface,
                       std::int32_t* w16, std::int32_t* w8b, std::int32_t* w8, std::int32_t* faces,
                       std::int32_t* elem)
{
    const std::int32_t nnpe = *nnpe_p;  // iVar23
    const std::int32_t nfn = *nfn_p;    // iVar6
    const i64 ld_c = nnpe < 0 ? 0 : nnpe;
    const i64 ne = *nelem < 0 ? 0 : *nelem;
    // module arrays DAT_140704060 (A: element type 8/6/4), DAT_140704100 (B: faces 6/5/4),
    // DAT_1407040b0 (C: nodes per face 4/3), lower bounds 1
    std::vector<std::int32_t> A, B, C;
    try {
        A.assign(static_cast<std::size_t>(ne) + 1, 0);
        B.assign(static_cast<std::size_t>(ne) + 1, 0);
        C.assign(static_cast<std::size_t>(ne) + 1, 0);
    } catch (...) {
        return 0xb;  // DAT_140704160 = 0xb
    }
    // conn(k, e); the tet table rows 5 and 6 are zero, so a tet-like brick reads node 0 of its
    // element (conn(0, e), the previous element's last node; before the array for e = 1). Such
    // faces are rejected whatever the value (3 equal nodes), so 0 is used out of range.
    auto CN = [&](std::int32_t k, std::int32_t e) -> std::int32_t {
        const i64 x = (i64(e) - 1) * ld_c + (k - 1);
        return x < 0 ? 0 : conn[x];
    };
    auto F = [&](std::int32_t l, std::int32_t s) -> std::int32_t& { return w16[(i64(s) - 1) * 4 + (l - 1)]; };
    auto K = [&](std::int32_t j, std::int32_t s) -> std::int32_t& { return w8[(i64(s) - 1) * 2 + (j - 1)]; };
    auto E = [&](std::int32_t j, std::int32_t s) -> std::int32_t& { return w8b[(i64(s) - 1) * 2 + (j - 1)]; };

    std::int32_t cnt = 0;  // DAT_140704164
    for (std::int32_t e = 1; e <= *nelem; ++e) {
        std::int32_t maxmult = 0;  // DAT_14070416c
        std::int32_t zeros = 0;    // DAT_140704170
        std::int32_t type;         // iVar14
        std::int32_t enfn = nfn;   // iVar28
        if (0 < nnpe) {
            for (std::int32_t t = 1; t <= nnpe; ++t) {
                const std::int32_t v = CN(t, e);
                if (v == 0) ++zeros;
                std::int32_t mult = 0;  // DAT_140704178
                for (std::int32_t s = 1; s <= nnpe; ++s)
                    if (CN(s, e) == v) ++mult;
                if (maxmult <= mult) maxmult = mult;
            }
            C[e] = nfn;
            A[e] = nnpe;
            B[e] = *nfe;
            type = *nfe;
            if (zeros == 2 || maxmult == 2) {
                B[e] = 5;
                A[e] = 6;
                type = 6;
                goto faces_loop;
            }
        } else {
            A[e] = nnpe;
            B[e] = *nfe;
            C[e] = nfn;
        }
        // LAB_140356ff0
        type = nnpe;
        if (zeros == 4 || maxmult == 4) {
            type = 4;
            A[e] = 4;
            C[e] = 3;
            B[e] = 4;
            enfn = 3;
        }
    faces_loop:
        for (std::int32_t f = 1; f <= *nfe; ++f) {
            const std::int32_t saved = cnt;
            const std::int32_t s = ++cnt;
            K(1, s) = 0;
            std::int32_t n1, n2, n3;
            if (type == 4) {
                n1 = CN(kTetFaces3[f - 1][0], e);
                n3 = CN(kTetFaces3[f - 1][2], e);
                n2 = CN(kTetFaces3[f - 1][1], e);
            } else if (type != 6) {
                n1 = CN(kHexFaces[f - 1][0], e);
                n2 = CN(kHexFaces[f - 1][1], e);
                n3 = CN(kHexFaces[f - 1][2], e);
            } else {
                n1 = CN(kWedgeFaces[f - 1][0], e);
                n2 = CN(kWedgeFaces[f - 1][1], e);
                n3 = CN(kWedgeFaces[f - 1][2], e);
            }
            F(1, s) = n1;
            F(2, s) = n2;
            F(3, s) = n3;
            std::int32_t n4;
            if (enfn == 4) {
                n4 = type != 6 ? CN(kHexFaces[f - 1][3], e) : CN(kWedgeFaces[f - 1][3], e);
                F(4, s) = n4;
            } else {
                n4 = 0;
                F(4, s) = 0;
            }
            // pairs of equal nodes and zero nodes of the face
            std::int32_t pairs = 0;  // DAT_14070416c
            std::int32_t fz = 0;     // DAT_140704170
            for (std::int32_t t = 1; t < 5; ++t) {
                const std::int32_t v = F(t, s);
                if (v == 0) ++fz;
                for (std::int32_t u = t + 1; u < 5; ++u)
                    if (F(u, s) == v) ++pairs;
            }
            if (pairs < 2 && fz < 2) {
                K(2, s) = s;
                // 14035768a: ((n1 + n2) + n3) + n4, 32-bit wraparound
                K(1, s) = static_cast<std::int32_t>(static_cast<std::uint32_t>(n1) + static_cast<std::uint32_t>(n2) +
                                                    static_cast<std::uint32_t>(n3) + static_cast<std::uint32_t>(n4));
                E(2, s) = e;
                E(1, s) = e;
            } else {
                cnt = saved;  // the slot is reused by the next face
            }
        }
    }

    sort_key_pairs(w8, &cnt);  // 14043c760

    *nface = 0;
    for (std::int32_t i = 1; i <= cnt; ++i) {
        const std::int32_t s = K(2, i);
        if (F(1, s) == 0) continue;
        bool dropped = false;
        if (i != cnt) {
            std::int32_t m = i + 1;  // DAT_140704188
            while (m <= cnt && !(K(1, i) < K(1, m))) ++m;
            const std::int32_t sn = C[E(1, s)];  // iVar14: nodes per face of s's element
            for (std::int32_t j = i + 1; j <= m - 1 && !dropped; ++j) {
                const std::int32_t t = K(2, j);
                std::int32_t l = 1;
                if (sn < 1) {
                    F(1, t) = 0;
                    dropped = true;
                    break;
                }
                for (;;) {
                    const std::int32_t v = F(l, s);  // DAT_140704190
                    if (!(v == F(1, t) || F(2, t) == v || F(3, t) == v ||
                          (sn == 4 && C[E(1, t)] == 4 && F(4, t) == v)))
                        break;
                    ++l;
                    if (sn < l) {
                        F(1, t) = 0;  // LAB_140357d4f
                        dropped = true;
                        break;
                    }
                }
            }
        }
        if (dropped) continue;  // LAB_140357829
        const std::int32_t el = E(1, s);
        const std::int32_t n = ++*nface;
        faces[(i64(n) - 1) * 4 + 1] = F(2, s);
        faces[(i64(n) - 1) * 4 + 2] = F(3, s);
        faces[(i64(n) - 1) * 4 + 0] = F(1, s);
        if (C[el] == 4) faces[(i64(n) - 1) * 4 + 3] = F(4, s);
        E(2, n) = el;  // 140357807: the output face's element goes to w8b(2, n)
        // 14035780b: (s - 1) / B(element) + 1, the slot index (after rejected faces were
        // squeezed out) divided by the element's face count; the element number only when
        // every element keeps all its faces
        elem[n - 1] = (s + -1) / B[el] + 1;
    }
    for (std::int32_t n = 1; n <= *nface; ++n) {
        const std::int32_t el = E(2, n);
        std::int32_t* fn = faces + (i64(n) - 1) * 4;
        if (C[el] == 3) {
            fn[3] = fn[0];
        } else if (A[el] == 6) {
            std::int32_t tmp[4];  // DAT_1407041a0
            for (int l = 0; l < 4; ++l) tmp[l] = fn[l];
            std::int32_t k = 0;  // DAT_14070417c
            for (int l = 0; l < 4; ++l) {
                if (0 < tmp[l]) {
                    ++k;
                    fn[k - 1] = tmp[l];
                }
            }
            if (k < 4) fn[3] = fn[0];
        }
    }
    return 0;
}

} // namespace

// @src 140354ed0  FL3_FNDBND
std::int32_t fl3_fndbnd(const std::int32_t* nelem, const std::int32_t* conn,
                        const std::int32_t* nnpe, std::int32_t* nface, std::int32_t* faces,
                        std::int32_t* elem)
{
    // DAT_140703fb0 nodes per face, DAT_140703fb4 faces per element, DAT_140703fb8 table row
    std::int32_t nfn = 3, nfe = 4, table = 1;
    if (*nnpe == 10) {
        nfn = 6;
        nfe = 4;
        table = 2;
    } else if (*nnpe == 8) {
        nfn = 4;
        table = 3;
        nfe = 6;
    }
    // three work arrays of max(nfe * nelem, 0) entries (32-bit product)
    std::int64_t extent = static_cast<std::int32_t>(static_cast<std::uint32_t>(nfe) * static_cast<std::uint32_t>(*nelem));
    if (extent < 0) extent = 0;
    std::vector<std::int32_t> w16, w8b, w8;
    try {
        // +2 ints: the tet kernel with nfn = 6 reads 2 nodes past a face (into the next one; past
        // the array for the last face, where the original reads heap bytes)
        w16.assign(static_cast<std::size_t>(extent) * 4 + 2, 0);
        w8b.assign(static_cast<std::size_t>(extent) * 2, 0);
        w8.assign(static_cast<std::size_t>(extent) * 2, 0);
    } catch (...) {
        return 0xb;  // DAT_140703fcc = 0xb
    }
    std::int32_t st = 0;
    if (*nnpe == 8) {
        // table argument 0x140604f10 + (3 - 1) * 0x60 = the brick table
        st = femsu_hex(nelem, conn, nnpe, &nfn, &nfe, nface, w16.data(), w8b.data(), w8.data(),
                       faces, elem);
    } else {
        femsu_tet(nelem, conn, nnpe, &nfn, &nfe, nface, w16.data(), w8.data(), faces, elem,
                  table == 2 ? kTet10Table : kTet4Table);
    }
    if (st != 0) return st;

    // order the faces with equal node sums by (first node, second node): for each i, every
    // following face j with the same sum is swapped with i when it is smaller
    auto sum = [&](std::int32_t n) -> std::int32_t {
        const std::int32_t* p = faces + (std::int64_t(n) - 1) * 4;
        std::uint32_t s = static_cast<std::uint32_t>(p[2]) + static_cast<std::uint32_t>(p[0]) +
                          static_cast<std::uint32_t>(p[1]);  // 140355830: (f1 + f0) + f2
        if (*nnpe == 8) s = static_cast<std::uint32_t>(p[3]) + s;
        return static_cast<std::int32_t>(s);
    };
    for (std::int32_t i = 1; i <= *nface; ++i) {
        const std::int32_t si = sum(i);  // DAT_140703fe0, computed once per i
        for (std::int32_t j = i + 1; j <= *nface; ++j) {
            if (si != sum(j)) break;
            std::int32_t* a = faces + (std::int64_t(i) - 1) * 4;
            std::int32_t* b = faces + (std::int64_t(j) - 1) * 4;
            const bool swap = a[0] > b[0] || (a[0] == b[0] && a[1] > b[1]);
            if (swap) {
                for (int l = 0; l < 4; ++l) {
                    const std::int32_t t = a[l];
                    a[l] = b[l];
                    b[l] = t;
                }
                const std::int32_t t = elem[i - 1];
                elem[i - 1] = elem[j - 1];
                elem[j - 1] = t;
            }
        }
    }
    return 0;
}

} // namespace def::model::geo_kw
