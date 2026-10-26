// defcmd/ifd/ifd2d.hpp: 2D interference kernel of OBJPOS modes 2/12/102/112 (packet p67-2).
// Spec: docs/re/commands/OBJPOS_interference.md section 7, OBJPOS_interference_kernels.md
// section 4. Bodies in src/ifd/d2/.
#pragma once

#include <cstdint>

namespace def::model {
class DB_STEP_DATA;
class LLOBJ_DATA;
} // namespace def::model

namespace def::cmd::ifd {

// @src 1401a0c70  2D driver: copy/convert die curves, run 1401a1370(model, B, A, D, *tol, 1000, r,
// flag), restore the curves; on success *dist = sqrt(r0*r0 + r1*r1), disp[0..1] = r. Returns
// true when *dist < 1e9. Moves nothing. a = moving object, b = target.
bool fun_1401a0c70(model::DB_STEP_DATA* m, model::LLOBJ_DATA* a, model::LLOBJ_DATA* b,
                   double* dir, double* tol, double* dist, double* disp, char flag);

} // namespace def::cmd::ifd
