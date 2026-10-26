// gendb3d/movement.hpp: helpers of the LLBND movement writer (FUN_1401912a0, movement.cpp),
// internal to packet p4-05. `mv` is the movement sub-object at LLOBJ_DATA+0x2160
// (Cls_14018b220, OMV_DATA in the mapping notes); offsets as in the binary.
#pragma once

#include <cstdint>

namespace def::model {
class DB_STEP_DATA;
class DYNARY;
} // namespace def::model

namespace def::dbio::g3d {

// @src 140134de0  DYNARY of type 0x79/0x7a/0x7b whose first variable is 0x1a: axis[0] becomes
//                 the running sum of segment length (first 2 coordinates, 3 when ncomp == 4)
//                 divided by the old axis value at the segment start, and var[0] = 1; true when
//                 converted
bool dynary_speed_axis_to_time(model::DYNARY* f);
// @src 14018c850  2D movement direction (mv+0x50, 0x58): components within 1e-12 of 0 or 1 snap
void snap_direction_2d(void* mv);
// @src 14018c940  3D movement direction mv+0x50..0x60 to unit length (unless its norm is 0)
void normalize_direction_3d(void* mv);
// @src 14018ca10  CNTRAX axis mv+0x648..0x658 to unit length
void normalize_axis_648(void* mv);
// @src 14018cb10  CNTRA2 axis mv+0x778..0x788 to unit length
void normalize_axis_778(void* mv);
// @src 1401d6030  six sim-control words (m+0x8d0, 0x8d8, 0x8e8, 0x8f0, 0x900, 0x908) into
//                 mv+0x1b8..0x1e0
void copy_sim_ctrl_to_movement(const model::DB_STEP_DATA* m, void* mv);

} // namespace def::dbio::g3d
