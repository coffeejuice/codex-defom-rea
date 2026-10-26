// defmodel/globals_p1_10.hpp: Fortran SAVE storage and globals used by p1-10 code
// (docs/port-conventions.md section 5). Moved to their owners' headers at merge if another
// packet declares the same address.
#pragma once

#include <defmodel/globals_p1_09.hpp>

#include <cstdint>

namespace def::model {

// ---- 140325691 (rotate a point about an axis) -------------------------------------------------
// @data 140701790  rotation matrix rot(3,3) built by 1404cf5c0
inline double g_rot_140701790[9] = {};
// @data 1407017e0  sin of the angle
inline double dat_1407017e0 = 0.0;
// @data 1407017e8  cos of the angle
inline double dat_1407017e8 = 0.0;
// @data 1407017f0  work point w(3) (1407017f0, 1407017f8, 140701800)
inline double g_work_1407017f0[3] = {};

// ---- 140345f5b (largest coordinate range of a point set) ---------------------------------------
// @data 140b74428  read only here; written elsewhere
inline double dat_140b74428 = 0.0;
// @data 140b74430  read only here; written elsewhere
inline double dat_140b74430 = 0.0;
// 140b84720..140b84730 are the Fortran COMMON that the 2D curve kernels read as their tolerances
// (owned by defrt: def::rt::geoutl::g_common_140b84720, defrt/ftn/common_140b84720.hpp): the
// names below are views of that one object, so
// the extent written here scales the tolerances of 14032b34b / 14032c4a0 as in the binary.
// 140b84720 (owner: def::rt::geoutl::g_common_140b84720)  copy of 140b74428 (u00)
inline double& dat_140b84720 = def::rt::geoutl::g_common_140b84720.u00;
// 140b84728 (owner: def::rt::geoutl::g_common_140b84720)  copy of 140b74430 (u08)
inline double& dat_140b84728 = def::rt::geoutl::g_common_140b84720.u08;
// 140b84730 (owner: def::rt::geoutl::g_common_140b84720)  result: max(range of column 2, range of column 1) (u10)
inline double& dat_140b84730 = def::rt::geoutl::g_common_140b84720.u10;
// @data 140702710  min of column 1
inline double dat_140702710 = 0.0;
// @data 140702718  max of column 1
inline double dat_140702718 = 0.0;
// @data 140702720  min of column 2
inline double dat_140702720 = 0.0;
// @data 140702728  max of column 2
inline double dat_140702728 = 0.0;

// Resets the globals above (tests).
void reset_globals_p1_10();

} // namespace def::model
