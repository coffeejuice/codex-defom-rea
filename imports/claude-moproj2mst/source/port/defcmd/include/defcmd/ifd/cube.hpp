// defcmd/ifd/cube.hpp: the bucket search grid "cfnCUBE" (C library linked into DEF_PRE, called
// from Fortran: every argument by pointer), as the 3D interference kernel 1403539a1 / 1403541fc
// uses it. Spec: docs/re/commands/OBJPOS_interference_kernels.md section 2.
//
// State: the global cube table DAT_1406fe4f0 (0x1a0-byte cube objects) with its size
// DAT_1406fe4f8 and used count DAT_1406fe4fc (@data in src/ifd/cube/).
#pragma once

#include <cstdint>

namespace def::cmd::cube {

// @src 140225640  cfnCUBE_CreateCubes_Obj(&nobj, &verbose): frees an existing table, allocates nobj
void fun_140225640(std::int32_t* nobj, std::int32_t* verbose);

// @src 140225ff0  build object (id, sub) of `type` (1 = polygons) from npts points pts(3, npts) and
// npoly polygons polys(npp, npoly) (1-based point ids, `offset` = id of pts(:,1)); calls 140226150
// with create = 1
void fun_140225ff0(std::int32_t* id, std::int32_t* sub, std::int32_t* type, std::int32_t* npts,
                   double* pts, std::int32_t* npoly, std::int32_t* npp, std::int32_t* polys,
                   std::int32_t* offset, std::int32_t* verbose);

// @src 140228c60  candidates of object (id, sub) in the box (x0, y0, z0, x1, y1, z1): count in *nc
// (the list is kept in the cube's result list for 140229f30)
void fun_140228c60(std::int32_t* id, std::int32_t* sub, double* box, std::int32_t* nc,
                   std::int32_t* exact, std::int32_t* verbose);

// @src 140229f30  copy the result list of the last query into list[0..nc-1], sorted ascending
void fun_140229f30(std::int32_t* id, std::int32_t* sub, std::int32_t* list, std::int32_t* verbose);

// @src 140225820  cfnCUBE_CleanCubes_AllObj(&verbose)
void fun_140225820(std::int32_t* verbose);

} // namespace def::cmd::cube
