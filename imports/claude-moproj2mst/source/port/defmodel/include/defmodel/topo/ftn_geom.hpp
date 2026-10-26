// defmodel/topo/ftn_geom.hpp: four Fortran (PathScale) geometry routines of packet p1-11 and
// their SAVE/COMMON state. Arguments follow the Fortran convention (everything by address,
// arrays column-major, 1-based in the comments).
#pragma once

#include <defrt/ftn/geoutl_basic.hpp>

#include <cstdint>

namespace def::model::topo {

// @data 14070d460  SAVE state of reverse_point_rows (14041f590)
struct RevPointsState {
    std::int32_t i;         // 0x00 (14070d460) loop index
    std::int32_t pad04[3];
    double tmp[6];          // 0x10 (14070d470) one row
    std::int32_t type;      // 0x40 (14070d4a0) int(row(1)) in the 6-column pass
    std::int32_t j;         // 0x44 (14070d4a4) mirrored index
};
inline RevPointsState g_rev_points{};

// SAVE state of direction_matrix (1404cf5c0), rotate_2d (1404d71d7) and transform_points
// (1404ded0c): the one copy lives in defrt/ftn/geoutl_basic.hpp (data 140730310, 140731518,
// 1407548a4); these names refer to it.
using DirMatrixState = def::rt::geoutl::DirMatrixState;
using Rot2dState = def::rt::geoutl::Rot2dState;
using XformState = def::rt::geoutl::XformState;
inline DirMatrixState& g_dir_matrix = def::rt::geoutl::g_dir_matrix;
inline Rot2dState& g_rot2d = def::rt::geoutl::g_rot2d;
inline XformState& g_xform = def::rt::geoutl::g_xform;

// Resets the SAVE state above (tests).
void reset_globals_p1_11();

// 14041f590: reverse the order of the npts points (columns) of a(ncol, npts); called by the
// _OBJ_GEO2D reverse 1402788a0. With ncol == 6 (arc/line records: type, x1, y1, x2/xc, y2/yc,
// angle) each record is first turned around: lines swap their ends, arcs move the start point to
// the end point by rotating it about the centre, and the angle changes sign; the angles are first
// converted by 14045a140 (degrees to radians for type 2, else 0).
void reverse_point_rows(const std::int32_t* ncol, const std::int32_t* npts, double* a);

// 1404cf5c0 (forwards to def::rt::geoutl::fun_1404cf5c0): 3x3 rotation matrix m (column-major, 9 doubles) of the direction v(3); a zero v sets
// g_dir_matrix.zero_vector = 1 and leaves m untouched.
void direction_matrix(const double* v, double* m);

// 1404d71d7 (forwards to def::rt::geoutl::fun_1404d71d7): rotate the point p(2) by the angle *a about the origin.
void rotate_2d(double* p, const double* a);

// 1404ded0c (forwards to def::rt::geoutl::fun_1404ded0c): mode < 2: b(:,i) = R * (a(:,i) - c); else b(:,i) = R^T * a(:,i) + c, for i = 1..n,
// with a, b (3, n), c (3), R (3, 3).
void transform_points(const double* a, double* b, const double* c, const double* r,
                      const std::int32_t* n, const std::int32_t* mode);

} // namespace def::model::topo
