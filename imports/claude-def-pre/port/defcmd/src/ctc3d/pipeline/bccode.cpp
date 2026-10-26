// Packet p67-7, group "pipeline": common boundary-condition codes of a node list.
//   FUN_14003a978  first BC code shared by all nodes of a list (group "decide", ported here by
//                  agreement with the packet lead: every stage of this group calls it)
//   FUN_14003a580  all distinct common BC codes of a node list (pairs code, direction)
//
// BC codes: bcc(k, node) = bcc[(node-1)*ld + k-1], k = 1..3 (ld = *ndof, 3 for the slave's
// object+0x990 array). Code 1 is "fixed in direction k" and is matched per component; any other
// code is matched in any of the three components. SAVE locals live in the image at their
// addresses (DAT_1406559c4.., DAT_14065596c..), so their values persist between calls as in the
// binary (14003a978 hands back DAT_1406559dc as the direction of a code 1 found on a single node:
// a stale value, kept).
#include <defcmd/ctc3d/decide.hpp>
#include <defcmd/ctc3d/pipeline.hpp>

namespace def::cmd::ctc3d {

namespace {

inline i32& I(std::uint64_t a) { return m<i32>(a); }

// bcc(k, node) with the binary's 64-bit index arithmetic (node may be 0 or negative: the binary
// reads before the array then, and so does the port)
inline i32 bc_at(const i32* bcc, std::int64_t ld, i32 node, i32 k)
{
    return bcc[(static_cast<std::int64_t>(node) - 1) * ld + k - 1];
}

} // namespace

// @src 14003a978  common BC code of the nodes conn(1..n): code (and its direction for code 1)
void fun_14003a978(const i32* bcc, const i32* conn, const i32* n, const i32* ndof, i32* code,
                   i32* dir)
{
    const i32 node1 = conn[0];
    I(0x1406559c4) = 0;
    const std::int64_t ld = *ndof >= 0 ? static_cast<std::int64_t>(*ndof) : 0;
    I(0x1406559c8) = node1;
    *dir = 0;
    *code = 0;
    if (node1 < 0) {
        return;
    }
    // 14003a9ee: node 1 without any code
    if (bc_at(bcc, ld, node1, 1) == 0 && bc_at(bcc, ld, node1, 2) == 0 &&
        bc_at(bcc, ld, node1, 3) == 0) {
        return;
    }
    I(0x1406559cc) = 1;
    do {
        const i32 c = bc_at(bcc, ld, node1, I(0x1406559cc));
        I(0x1406559c4) = c;
        if (c != 0) {
            const i32 nn = *n;
            I(0x1406559d0) = 1;
            I(0x1406559d4) = 2;
            bool found = false;
            if (nn >= 2) {
                do {
                    const i32 nd = conn[I(0x1406559d4) - 1];
                    I(0x1406559d8) = nd;
                    if (I(0x1406559c4) == 1) {
                        // 14003ab7a: code 1 must sit in the same component
                        if (bc_at(bcc, ld, nd, I(0x1406559cc)) == 1) {
                            I(0x1406559d0) = I(0x1406559d0) + 1;
                            I(0x1406559dc) = I(0x1406559cc);
                        }
                    } else {
                        I(0x1406559e0) = 1;
                        do {
                            if (bc_at(bcc, ld, nd, I(0x1406559e0)) == I(0x1406559c4)) {
                                I(0x1406559d0) = I(0x1406559d0) + 1;
                                break;
                            }
                            I(0x1406559e0) = I(0x1406559e0) + 1;
                        } while (I(0x1406559e0) <= 3);
                    }
                    I(0x1406559d4) = I(0x1406559d4) + 1;
                } while (I(0x1406559d4) <= *n);
                // 14003aa0f: EDI = *n read in the loop's last test
                found = (*n == I(0x1406559d0));
            } else {
                found = (nn == 1);
            }
            if (found) {
                // 14003aa83
                *code = I(0x1406559c4);
                if (I(0x1406559c4) == 1) {
                    *dir = I(0x1406559dc);
                }
                return;
            }
        }
        I(0x1406559cc) = I(0x1406559cc) + 1;
    } while (I(0x1406559cc) <= 3);
}

// @src 14003a580  distinct common BC codes of the nodes conn(1..n): *count pairs out(1..2, i) =
// (code, direction) in the order 14003a978 finds them
void fun_14003a580(const i32* bcc, const i32* conn, const i32* n, const i32* ndof, i32* count,
                   i32* out)
{
    // work array W(4, n) at DAT_140655980 (leading dimension 4), W(k, i) = bcc(k, conn(i))
    auto W = [](i32 k, i32 i) -> i32& {
        return m<i32>(0x140655980 + 4 * (static_cast<std::int64_t>(k) - 1) +
                      16 * (static_cast<std::int64_t>(i) - 1));
    };
    const std::int64_t ld = *ndof >= 0 ? static_cast<std::int64_t>(*ndof) : 0;
    *count = 0;
    i32* const code = mp<i32>(0x14065596c);
    i32* const cdir = mp<i32>(0x140655970);
    fun_14003a978(bcc, conn, n, ndof, code, cdir);
    if (*code == 0) {
        return;
    }
    I(0x140655974) = 1;
    if (*n >= 1) {
        do {
            const i32 nd = conn[I(0x140655974) - 1];
            I(0x140655978) = nd;
            I(0x14065597c) = 1;
            if (*ndof >= 1) {
                do {
                    const i32 k = I(0x14065597c);
                    I(0x14065597c) = k + 1;
                    W(k, I(0x140655974)) = bc_at(bcc, ld, nd, k);
                } while (I(0x14065597c) <= *ndof);
            }
            I(0x140655974) = I(0x140655974) + 1;
        } while (I(0x140655974) <= *n);
    }
    // 14003a714: fourth row cleared when ndof < 4
    if (*ndof <= 3) {
        I(0x140655974) = 1;
        if (*n >= 1) {
            do {
                const i32 i = I(0x140655974);
                W(4, i) = 0;
                I(0x140655974) = i + 1;
            } while (I(0x140655974) <= *n);
        }
    }
    while (*code != 0) {
        // 14003a7b2: remove the code just found from W (code 1: rows 1..dir only)
        I(0x1406559c0) = 0;
        I(0x14065597c) = 1;
        if (*ndof >= 1) {
            for (;;) {
                I(0x140655974) = 1;
                if (*n >= 1) {
                    do {
                        i32& w = W(I(0x14065597c), I(0x140655974));
                        if (w == *code) {
                            w = 0;
                        }
                        if (*code == 1 && *cdir == I(0x14065597c)) {
                            I(0x1406559c0) = 1;
                        }
                        I(0x140655974) = I(0x140655974) + 1;
                    } while (I(0x140655974) <= *n);
                }
                // 14003a898
                if (I(0x1406559c0) == 1) {
                    break;
                }
                I(0x14065597c) = I(0x14065597c) + 1;
                if (I(0x14065597c) > *ndof) {
                    break;
                }
            }
        }
        // 14003a7d7: append (code, dir), look for the next code in W with the identity node list
        const i32 c = *code;
        const i32 d = *cdir;
        const i32 k = *count + 1;
        *count = k;
        out[2 * (k - 1)] = c;
        out[2 * (k - 1) + 1] = d;
        i32 four = 4;
        fun_14003a978(mp<i32>(0x140655980), mp<i32>(0x1405c6870), n, &four, code, cdir);
    }
}

} // namespace def::cmd::ctc3d
