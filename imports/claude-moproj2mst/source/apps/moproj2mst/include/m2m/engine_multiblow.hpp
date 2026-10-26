// m2m/engine_multiblow.hpp: load slots of SfOprMultiBlow / SfOprMultiBlow2D / SfOprMultiBlowCommon
// (one vtable layout for the three: packet MDT L4, docs/re/moproj2mst/mdt/L4/spec.md). The class
// (src/ops/multiblow.cpp) overrides its OpClass slots with these.
#pragma once

#include <cstdint>

#include "m2m/engine.hpp"

namespace m2m::mst {
class OpClass;
}

namespace m2m::engine {

// vt+0xb8  @src 0x140601f70  base prologue, TRANS 2/1 again, DTTYPE mode 0, NSTEP 10000
void multiBlowLoadPrologue(mst::OpClass& c, Project& p, Operation& op);
// vt+0x78  @src 0x140602760  nothing
void multiBlowLoadClassData(mst::OpClass& c, Project& p, Operation& op);
// vt+0xc0  @src 0x140602010  first three objects present, PDIE = vt+0x120, slot vt+0x120 assigns
// movement, lubricant id 0. `vt120` is the class's vt+0x120 (0x140601f50).
void multiBlowPostLoad(mst::OpClass& c, Project& p, Operation& op, FileEffects& fx, int (*vt120)());
// props vt+0x58  @src 0x140612ba0  through 0x140054350 (OPI+0x10)
void multiBlowPropsLoad(mst::OpClass& c, Project& p, Operation& op);
// save vt+0xd8  @src 0x140602780  base 0x1405f22e0, then PDIE = vt+0x120 (MDT L4S)
void multiBlowSaveNames(mst::OpClass& c, Project& p, Operation& op, FileEffects& fx, int (*vt120)());
// save vt+0x80  @src 0x140602770  empty (save_current.cpp lets MultiBlow through as empty)
// OB vt+0xc8  @src 0x140602230  the base (0x1405f1e10: empty), then 0x140602260: the primary die's
// movement into the MultiBlow data (hammer or screw); otherwise the GUI's log message (MSG type 3)
// becomes a warning. Called at the end of the DB / restart sync 0x1400538f0 (MDT DB-4, DB-5).
bool multiBlowSyncFromModelTail(mst::OpClass& c, Project& p, Ctx& ctx, int (*vt120)());

}  // namespace m2m::engine

namespace m2m {
struct MultiBlowProps;
// @src 0x14060b6b0  the movement block `mv` (LLOBJ_DATA+0x2160) into the props' MultiBlow data,
// with the EquipmentType / EquipmentSpec children made again; true for a hammer or a screw
bool multiBlowEquipmentFromMovement(MultiBlowProps& pr, const std::uint8_t* mv);
}  // namespace m2m
