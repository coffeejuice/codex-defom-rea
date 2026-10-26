// defmodel/geoprim/triangulate.hpp: the polygon triangulator of DEF_PRE_64 at 0x14041f830 and
// 0x14045a270..0x140460e80 (packet p9-05).
//
// It is a trapezoidation triangulator (randomised incremental segment insertion, monotone
// decomposition, greedy triangulation of each monotone piece) with every "equal" test done
// with the tolerance 1e-12 (DAT_14051f260). All state lives in globals, as in the binary
// (`g_tri`, below); one call allocates and frees everything.
//
// The segment insertion order is a random permutation made with the CRT rand() after
// srand((unsigned)time(NULL)) (14045c530). The time comes from def::rt::now(), so tests pin it
// with def::rt::pin_clock().
#pragma once

#include <cstdint>

namespace def::model::geoprim {

// @src 14041f830  triangulate ncontours polygons.
//   cntr[c]       number of points of contour c (cntr points at prim+0x1700 for one profile)
//   vertices      1-based points, 2 doubles each (16-byte stride; vertices[0] unused), the
//                 contours one after the other (layout of prim+0x1708)
//   triangles     out: 3 ints per triangle (1-based point ids); room for the caller to size
// Returns the number of triangles written.
int fun_14041f830(int ncontours, const int* cntr, const double* vertices, int* triangles);

// Test hook: the CRT rand() state the triangulator uses (srand/rand of the static CRT).
namespace tri_detail {
void crt_srand(std::uint32_t seed);
int crt_rand();
} // namespace tri_detail

} // namespace def::model::geoprim
