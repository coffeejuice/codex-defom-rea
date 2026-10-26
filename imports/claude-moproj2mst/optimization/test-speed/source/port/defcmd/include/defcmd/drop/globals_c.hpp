// defcmd/drop/globals_c.hpp: module data of the mode-8 axis point (surface mass properties,
// 1403e8510 and its Fortran callees), packet p67-3. Spec: docs/re/commands/OBJPOS_drop_mode8.md.
//
// The routines' other SAVE scratch (0x140708070..0x14070af70) is written before it is read in
// every call, so it stays in C++ locals.
#pragma once

#include <cstdint>

namespace def::cmd::drop {

// @data 140b44d80  mass-property block written by 1403ea82d (22 doubles, 0x140b44d80..0x140b44e30):
//   [0..8]   inertia tensor I(3,3) about the centroid, column-major (d80 I11, d98 I12, da0 I22,
//            db0 I13, db8 I23, dc0 I33; d88/d90/da8 are copies of the upper triangle)
//   [9..17]  M(3,3) = V * identity (dc8, de8, e08 = V, the rest 0.0)
//   [18]     e10 volume V
//   [19..21] e18/e20/e28 centroid
// Also read by 1403edb4f (the centroid, integrands k = 5..10). Process lifetime; the other
// readers of the block (if any) are not known.
inline double g_massprop[22] = {};

// Resets g_massprop (tests).
inline void reset_globals_c()
{
    for (double& v : g_massprop) v = 0.0;
}

} // namespace def::cmd::drop
