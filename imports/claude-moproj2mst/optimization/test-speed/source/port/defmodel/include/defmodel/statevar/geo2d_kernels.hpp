// 2D geometry helpers and Fortran curve kernels that the linker placed in the StateVar ranges
// (p1-09). Their callers are the 2D geometry classes of p1-07 (1402789f0, 140277c50, 1402763d0).
//
// Arrays follow the Fortran layout of the original: a point record is 3 doubles (x, y, r) and an
// entity record is 6 doubles (type, x1, y1, x2, y2, angle). Counts and indexes are 1-based where
// the original is Fortran.
//
//   entity type 1: line   (x1, y1) -> (x2, y2)
//   entity type 2: arc    start (x1, y1), centre (x2, y2), signed sweep angle in radians
//   point r:              fillet radius at that corner (0 = sharp)
//
// Math library: the Fortran kernels call the PathScale libm (sincos 1404b3b30, tan 1404b3df0,
// atan2 1404b3878, acos 1404b3d58) and the C++ helpers the static MSVC CRT (sin 140bb3000,
// cos 140bb3490). The port uses <cmath>; the last bit can differ until a shared runtime math
// module reproduces those implementations.
#pragma once

#include <cstdint>

namespace def::model::geo2d {

// 1403077e0: out = (b - a) rotated by deg degrees (C++, MSVC CRT sin/cos)
void rotate_difference_deg(const double* a, const double* b, double* out, double deg);
// 140308730: copy a 2D vector (the original also copies 8 undefined stack bytes after it)
void copy_vec(double* dst, const double* src);
// 1403089b0: dst = b - a
void sub_vec(double* dst, const double* b, const double* a);
// 140308a20: dst = a + b
void add_vec(double* dst, const double* a, const double* b);
// 140309550: signed area of a closed polygon of n (x, y) pairs (shoelace, 0.5 per term)
double polygon_area(std::int32_t n, const double* xy);

// 14032ab90: rotate point p about centre c by *deg degrees (angle = deg / 45 * pi/4)
void rotate_point_deg(double* p, const double* deg, const double* c);

// 14032ac75: entities -> points. Lines give their two end points (r = 0); arcs are split into 1..3
// pieces of at most 120 degrees, each giving start, tangent-intersection control point (r = arc
// radius) and end. Close points are then merged (14032c4a0) and the freed records zeroed.
// Arcs that need more than 3 pieces print a message and stop (1403c4504).
void entities_to_points(std::int32_t* npts, double* pts, const std::int32_t* nent, const double* ents);
// 14032b34b: points -> entities: line segments between corners, with an arc fillet of radius r at
// each corner whose r exceeds the tolerance.
void points_to_entities(const std::int32_t* npts, const double* pts, std::int32_t* nent, double* ents);
// 14032bba1: move the end of entity (p1, p2, angle) of type *type to the point e (line: p2 = e;
// arc: new sweep angle to e, and e is moved onto the arc).
void trim_entity_end(const double* p1, double* p2, double* e, double* angle, const std::int32_t* type);
// 14032c4a0: remove point i+1 while it is closer than the tolerance to point i (the last point is
// never compared); *merged = 1 when a point was removed.
void merge_close_points(std::int32_t* npts, double* pts, std::int32_t* merged);

} // namespace def::model::geo2d
