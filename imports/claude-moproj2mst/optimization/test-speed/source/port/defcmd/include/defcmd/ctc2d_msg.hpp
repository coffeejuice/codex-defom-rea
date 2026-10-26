// defcmd/ctc2d_msg.hpp: the Fortran warnings of the 2D GENCTC kernels (format labels 1001, 1002,
// 1004 of 140350f12 and 14034fc56), written to the unit in DAT_140b847a4 (stderr).
#pragma once

#include <cstdint>

namespace def::cmd::ftn {

// @data 140b847a4  Fortran unit of the 2D warnings of GENCTC and of the 2D interference kernel
// (OBJPOS): 0 in the image and never assigned (unit 0 is the Fortran runtime's stderr)
inline std::int32_t dat_140b847a4 = 0;

// 1001: "Unable to change BCC for Node" (node, x code, y code)
void ctc2d_warn_1001(std::int32_t node, std::int32_t c0, std::int32_t c1);
// 1002: node pulled to object surface (node, master object, segment, distance)
void ctc2d_warn_1002(std::int32_t node, std::int32_t obj, std::int32_t entity, double dist);
// 1004: as 1002 plus "Invalid contact boundary condition on mesh"
void ctc2d_warn_1004(std::int32_t node, std::int32_t obj, std::int32_t entity, double dist);

} // namespace def::cmd::ftn
