// defmodel/geo_kw/boundary.hpp: boundary extraction of an object's FE mesh (packet kw-g2geo,
// boundary family). ELMCON's reader calls it after the connectivity is read.
//
// Data produced (offsets in LLOBJ_DATA; the record class is Cls_14017e740, 0x30 bytes, 100 of
// them at +0xbe0; the slot functions over it are in defmodel/bc/boundary_topology.hpp):
//   2D  +0xbd8 = number of boundary node chains (at most 100). Record i (+0xbe0 + i*0x30):
//       +0x00 node count n_i, +0x08 int[n_i] the chain's node numbers (1-based mesh nodes) in
//       the order FL2_FNDBND walks them; every chain is closed (last node == first node), so it
//       has n_i - 1 edges; each edge (a, b) is oriented as in its element (a = conn[i-1],
//       b = conn[i % 4]). +0x10/+0x18/+0x20 untouched (except reallocation by 14017e8a0).
//       Edge slot of the EC* keywords (140200c90): chains concatenated, slot = off_i + j for the
//       edge (nodes[j], nodes[j+1]), off_i = sum over earlier chains of (n_k + 1), so every chain
//       leaves two unused slots after it; NS = sum(n_i - 1) - 2 + 2 * nchains (140165170).
//   3D  +0xbd8 = 1, record 0 only: +0x10 face count F, +0x18 int[4*F] face nodes (a triangle is
//       stored "a b c a"), +0x20 int[F] element of each face (1-based, see fl3_fndbnd).
//       Face slot = face index (140200ff0); NS = F.
//   Both: +0x88c = sum of the segment counts (140165d30), and the 12 EC* arrays at +0xae8 are
//       reallocated with NS entries (1401631e0).
#pragma once

#include <cstdint>

namespace def::model {
class LLOBJ_DATA;
}

namespace def::model::geo_kw {

// 1401fc290: build the boundary: 2D node chains (obj+0xbd8 count, 0x30-byte records at
// obj+0xbe0...), 3D boundary faces (obj+0xbf8 / obj+0xc00), then 140165d30 and 1401631e0.
void build_boundary(LLOBJ_DATA& obj);

// The Fortran finders, 1-based arrays passed as in the binary (exposed for tests).

// 14032d65e (FL2_FNDBND, kernel 14032da62): boundary node chains of a 2D mesh of nelem quads
// (conn: 4 nodes per element; a triangle repeats its first node as the 4th). Out: nchain,
// ranges[2k-2], ranges[2k-1] = first / last 1-based position of chain k in `nodes`; `nodes`
// holds each chain followed by a 0, and the slot after the last chain is -1. Returns 0, or 0xb
// when the work array cannot be allocated (nothing written then).
// Sizes the caller provides: ranges 2 * (4 * nelem) ints, nodes 6 * nelem + 2 ints at least.
std::int32_t fl2_fndbnd(const std::int32_t* nelem, const std::int32_t* conn, std::int32_t* nchain,
                        std::int32_t* ranges, std::int32_t* nodes);

// 140354ed0 (FL3_FNDBND; kernels 140355cbf tet, 14035672f brick): boundary faces of a 3D mesh
// (nnpe 4: tet, 8: brick, possibly degenerate, 10: 10-node tet). Out: nface,
// faces[4*nface] (triangles "a b c a" for tets; 10-node tets leave the 4th 0), elem[nface].
// Faces with equal node sums are put in (first node, second node) order afterwards.
// Sizes the caller provides: faces 4 * nfe * nelem, elem nfe * nelem (nfe = 6 brick, else 4).
// Returns 0, or 0xb when a work array cannot be allocated.
std::int32_t fl3_fndbnd(const std::int32_t* nelem, const std::int32_t* conn,
                        const std::int32_t* nnpe, std::int32_t* nface, std::int32_t* faces,
                        std::int32_t* elem);

// 14043c760: qsort of n (key, value) int pairs by key (comparator 14043c7f0) with the MSVC CRT
// qsort (140484bd0, not stable: the order of equal keys is part of the result).
void sort_key_pairs(std::int32_t* pairs, const std::int32_t* n);

}  // namespace def::model::geo_kw
