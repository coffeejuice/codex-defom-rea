// defcmd/drop/globals_a.hpp: module statics of the drop kernels (packet p67-3, worker A) whose
// value survives from one call to the next. Everything else those kernels keep in statics is
// written before it is read in every call and is a C++ local in the port.
#pragma once

namespace def::cmd::drop {

// Frames T(3,3) of the plane/axis projection and mirror helpers (§5.3, §5.12). FUN_1404cf5c0
// returns without writing T when the direction is zero, so the previous call's frame is used.
// @data 140705270  T of 140365a10 (projection onto a plane)
inline double g_t_140705270[9]{};
// @data 1407052e0  T of 140365b02 (projection onto an axis)
inline double g_t_1407052e0[9]{};
// @data 140705350  T of 140365bfc (mirror of a moment)
inline double g_t_140705350[9]{};
// @data 1407053c0  T of 140365d0c (mirror of a point)
inline double g_t_1407053c0[9]{};

// Resets the globals of this header (tests).
inline void reset_globals_a()
{
    for (int i = 0; i < 9; ++i) {
        g_t_140705270[i] = 0.0;
        g_t_1407052e0[i] = 0.0;
        g_t_140705350[i] = 0.0;
        g_t_1407053c0[i] = 0.0;
    }
}

} // namespace def::cmd::drop
