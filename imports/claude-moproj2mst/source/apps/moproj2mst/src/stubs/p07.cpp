// P07 stubs: the paste-path list copies of 0x1400500b0 (not reached on open: 0x140046cc0 passes
// src = 0). Coordinator 2026-10-22: leave NotPorted. Decompiles: gui_mo/out_p07/lists.c, copy.c.
#include "m2m/model.hpp"

namespace m2m {
// 0x140066930: clear (0x14020db90); per entry new(0x1c8) 0x14004d1e0(*src[i]) copies number, type,
// +0x08, name, hasGeo/hasMesh, +0x1b8..+0x1bd, +0x1c1, matIds, equipmentId and the state variables
// (0x14004e4d0, not decompiled); +0x1be..+0x1c0 are left as allocated. autoDelete = 1.
void copyObjList(Operation&, const Operation&) { M2M_STUB(0x140066930, "OBJ list copy (paste path)"); }
// 0x140066ab0: clear; per entry new(0xd8) 0x140066b90(*src[i]) (list +0xd0, then 0x140066be0,
// not decompiled). autoDelete = 1.
void copyObjposList(Operation&, const Operation&) { M2M_STUB(0x140066ab0, "OBJPOS list copy (paste path)"); }
}  // namespace m2m
