// kw-g2geo boundary family: FL2_FNDBND, the boundary node chains of a 2D mesh (Fortran module
// procedure 14032d65e with its kernel 14032da62). The module variables the kernel uses as loop
// counters (DAT_140701c68..DAT_140701c8c) are locals here; nothing else reads them.
#include <defmodel/geo_kw/boundary.hpp>

#include <cstdint>
#include <vector>

namespace def::model::geo_kw {

namespace {

// @src 14032da62  FNDBND kernel: edges of every element, remove the edges met twice in
// opposite directions, then chain the remaining (boundary) edges.
//   conn(4, nelem), work(2, 4*nelem): edge list, nchain, ranges(2, *), nodes(*).
// The timing output (flag *param_7 == 1, FUN_140235710 and "FL3_S_FNDBND TIME" WRITEs) is not
// ported: the only caller (1401fc290 via 14032d65e) passes 0.
void fndbnd2_kernel(const std::int32_t* nelem, const std::int32_t* conn, std::int32_t* nchain,
                    std::int32_t* ranges, std::int32_t* nodes, std::int32_t* work)
{
    // 1-based views: C(i, e) = conn[(e-1)*4 + i-1], W(1|2, k) = work[(k-1)*2 + 0|1],
    // R(1|2, k) = ranges[(k-1)*2 + 0|1], N(k) = nodes[k-1]
    auto C = [&](std::int32_t i, std::int32_t e) -> std::int32_t {
        return conn[(static_cast<std::int64_t>(e) - 1) * 4 + (i - 1)];
    };
    auto W = [&](std::int32_t j, std::int32_t k) -> std::int32_t& {
        return work[(static_cast<std::int64_t>(k) - 1) * 2 + (j - 1)];
    };
    auto N = [&](std::int32_t k) -> std::int32_t& { return nodes[static_cast<std::int64_t>(k) - 1]; };

    // 1. all element edges (C(i, e), C(mod(i, 4) + 1, e)), i = 1..4; a degenerate quad
    //    (C(1) == C(4)) stops before its 4th edge
    std::int32_t nedge = 0;  // DAT_140701c68
    for (std::int32_t e = 1; e <= *nelem; ++e) {
        std::int32_t i = 1;  // DAT_140701c70
        do {
            if (i == 4 && C(1, e) == C(4, e)) break;
            ++nedge;
            const std::int32_t b = C(i % 4 + 1, e);  // 14032dab0: C remainder of i by 4
            W(1, nedge) = C(i, e);
            W(2, nedge) = b;
            ++i;
        } while (i < 5);
    }

    // 2. drop each edge whose reverse comes later (and that reverse); compact in place
    std::int32_t nbnd = 0;  // DAT_140701c80
    for (std::int32_t k = 1; k <= nedge; ++k) {
        const std::int32_t a = W(1, k);
        if (a == 0) continue;
        bool shared = false;
        if (k != nedge) {
            for (std::int32_t j = k + 1; j <= nedge; ++j) {
                if (W(1, j) == W(2, k) && W(2, j) == a) {
                    W(1, j) = 0;
                    shared = true;
                    break;
                }
            }
        }
        if (shared) continue;
        ++nbnd;
        W(1, nbnd) = a;
        W(2, nbnd) = W(2, k);
    }

    // 3. chains: start at the first unused edge, follow the first later edge (searched from
    //    the start edge + 1) that begins where the current one ends
    std::int32_t pos = 0;  // DAT_140701c84
    *nchain = 0;
    for (std::int32_t k = 1; k <= nbnd; ++k) {
        const std::int32_t a = W(1, k);
        if (a == 0) continue;
        std::int32_t cur = k;  // DAT_140701c88
        ++pos;
        const std::int32_t ch = ++*nchain;
        ranges[(static_cast<std::int64_t>(ch) - 1) * 2] = pos;
        N(pos) = a;
        for (;;) {
            ++pos;
            const std::int32_t b = W(2, cur);
            W(1, cur) = 0;
            N(pos) = b;
            std::int32_t j = k + 1;  // DAT_140701c70
            while (j <= nbnd && W(1, j) != b) ++j;
            if (j > nbnd) break;
            cur = j;
        }
        ranges[(static_cast<std::int64_t>(ch) - 1) * 2 + 1] = pos;
        ++pos;
        N(pos) = 0;
    }
    // 14032de1e: with no edge (nbnd <= 0) or no chain position the terminator goes to slot 1
    if (nbnd <= 0 || pos <= 0) pos = 1;
    N(pos) = -1;
}

} // namespace

// @src 14032d65e  FL2_FNDBND: allocate the edge work array (2 x 4*nelem ints, module array
// DAT_140701be0, ALLOCATE through 1404b62c4), run the kernel, deallocate (1404b6d74).
std::int32_t fl2_fndbnd(const std::int32_t* nelem, const std::int32_t* conn, std::int32_t* nchain,
                        std::int32_t* ranges, std::int32_t* nodes)
{
    // 14032d6c7: extent = max(nelem << 2, 0) (32-bit shift, sign-extended)
    std::int64_t extent = static_cast<std::int64_t>(static_cast<std::int32_t>(
        static_cast<std::uint32_t>(*nelem) << 2));
    if (extent < 0) extent = 0;
    std::vector<std::int32_t> work;
    try {
        work.assign(static_cast<std::size_t>(extent) * 2 + 2, 0);
    } catch (...) {
        return 0xb;  // 14032d74c: stat != 0 -> DAT_140701c50 = 0xb, return
    }
    fndbnd2_kernel(nelem, conn, nchain, ranges, nodes, work.data());
    return 0;
}

} // namespace def::model::geo_kw
