// defcmd/ctc2d_save.hpp: SAVE storage of the 2D GENCTC Fortran kernels (ctc2d_ftn.hpp) that this
// packet ports. PathScale keeps a routine's locals at fixed addresses; most are written before
// they are read in every call, but some carry a value from one call to the next (marked
// "carried"), so all of them are kept here, one struct per routine, named after the address of
// the first member. reset_ctc2d_save() zeroes them (the image's .bss state) for tests.
#pragma once

#include <cstdint>

namespace def::cmd::ftn {

// @data 14070d630  SAVE locals of 1404345c0 (line kernel)
struct Save1404345c0 {
    double u[2];          // 14070d630 b - a, then the unit direction
    double p[2];          // 14070d640 p - a
    std::int32_t where;   // 14070d650 0 inside, 1 nearer a, 2 nearer b
    std::int32_t pad;
    double h;             // 14070d658 signed normal distance
    double s;             // 14070d660 position along the segment
    double db;            // 14070d668 distance to b
};
inline Save1404345c0 g_save_1404345c0{};

// @data 14070d788  SAVE locals of 14043556b (rotate)
struct Save14043556b {
    double s;             // 14070d788 sin
    double c;             // 14070d790 cos
    double x;             // 14070d798 input x
    double y;             // 14070d7a0 input y
};
inline Save14043556b g_save_14043556b{};

// @data 14070d834  SAVE local of 14043574f (arc kernel): which point the last call took, 0 the
// foot on the arc, 1 the start, 2 the end; carried: a mode-2 call that finds both end points
// 1e10 or farther away moves the point to the one chosen by the previous call
inline std::int32_t g_arc_where_14070d834 = 0;

void reset_ctc2d_save();

} // namespace def::cmd::ftn
