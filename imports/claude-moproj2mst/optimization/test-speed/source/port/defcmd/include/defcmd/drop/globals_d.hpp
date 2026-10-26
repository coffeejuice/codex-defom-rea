// defcmd/drop/globals_d.hpp: statics of 140369c19 / 14036d3fd (packet p67-3) that can carry a
// value from one call to the next. Every other static of the two routines is written before it
// is read in each call and is a C++ local (see the file comments of settle_eqm.cpp and
// settle_step.cpp).
#pragma once

#include <defcmd/drop/state.hpp>

namespace def::cmd::drop {

// @data 140705be0  frame T(3,3) of d built by 1404cf5c0 in 140369c19. 1404cf5c0 leaves T
// untouched for a zero vector, so the previous call's T would be used (d is never zero after the
// set-up check).
inline double g_t_140705be0[9]{};

// @data 140705d40  6 doubles in the binary: best direction of the "most opposite" search
// (copied from 140705d00 when q < best) and the output of the cross product of the side test.
// When no q < 2.0 (only for NaN directions) the search result is the value left by the
// previous call's last cross product. Only the first 3 doubles are ever read.
inline double g_d40_140705d40[3]{};

// @data 1407061e4  interrupt flag of 14036d3fd, the argument of the empty stub 14001cd40 and
// never written anywhere: always 0 (the return at 0x14036ebe2 is dead).
inline i32 g_intr_1407061e4 = 0;

inline void reset_globals_d()
{
    for (double& x : g_t_140705be0) x = 0.0;
    for (double& x : g_d40_140705d40) x = 0.0;
    g_intr_1407061e4 = 0;
}

} // namespace def::cmd::drop
