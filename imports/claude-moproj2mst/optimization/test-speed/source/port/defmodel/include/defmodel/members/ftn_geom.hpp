// defmodel/members/ftn_geom.hpp: two Fortran geometry routines of packet p1-10.
#pragma once

#include <cstdint>

namespace def::model {

// FUN_140325691: rotates the point p(3) by *angle_deg degrees about the axis through origin(3)
// with direction dir(3): transform into the axis frame (1404cf5c0, 1404ded0c mode 1), rotate
// x/y by the angle, transform back (mode 2). Result also left in g_work_1407017f0.
void fun_140325691(double* p, const double* origin, const double* dir, const double* angle_deg);

// FUN_140345f5b: dat_140b84730 = larger of the x and y extents of n points xyz(3, n).
void fun_140345f5b(const double* xyz, const std::int32_t* n);

} // namespace def::model
