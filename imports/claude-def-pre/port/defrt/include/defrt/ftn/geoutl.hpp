// defrt/ftn/geoutl.hpp: the Fortran geometry kernels (FTN_geoutl.f and neighbours,
// 0x1404c0000..0x1404e7fff) shared by OBJPOS, the drop solver and 3D/2D GENCTC
// (docs/re/commands/*.md). Ported from the machine code, operation order from the listings, so
// results are bit-identical to the original.
//
// API contract (stable; porting threads may declare against it before the bodies land):
//   * name: def::rt::geoutl::fun_<address>; the comment says what the routine does.
//   * parameters: exactly the binary routine's argument list in order. Fortran routines take
//     everything by pointer (INTEGER = std::int32_t, REAL*8 = double, REAL*4 = float); arrays
//     are column-major, 1-based in the comments.
//   * return: double when the routine returns in xmm0, std::int32_t when it returns in eax,
//     else void.
//   * SAVE/COMMON state the routine keeps lives in structs tagged @data below, one copy only
//     (the three routines first ported in defmodel/topo/ftn_geom.hpp now live here).
// The bodies are split over src/ftn/geoutl_*.cpp; tier-1 tests in tests/test_ftn_geoutl_*.cpp.
// Elementary functions come from defrt/stdlib/libm.hpp (ps_* Fortran runtime, crt_* MSVC CRT).
//
// Status per routine: docs/re/status/geoutl.md.
#pragma once

#include <cstdint>

#include <defrt/ftn/geoutl_basic.hpp> // vectors, normals, frames, transforms
#include <defrt/ftn/geoutl_poly.hpp>  // point-in-polygon, point-segment, bilinear (quad) solve
#include <defrt/ftn/geoutl_tri.hpp>   // triangle / barycentric kernels, projection drivers
#include <defrt/ftn/geoutl_proj.hpp>  // quadrilateral projection library (1404d203b family)
