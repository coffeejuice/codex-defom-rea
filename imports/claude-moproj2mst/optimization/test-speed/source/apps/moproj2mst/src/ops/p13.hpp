// Internal entry points of the P13 writers (Cycle, Group, DoeOutput, Distortion), for the writers
// themselves and the P13 tier-1 tests. Owner P13. Spec: s1-04 2, 3, 6.
#pragma once

#include <vector>

#include "m2m/model.hpp"
#include "m2m/mst/writer.hpp"
#include "m2m/props.hpp"

namespace m2m {
// @src 0x1400b94f0  operations (OPI*) of a sub-project, from its flow chart. Owner P03 (routed by
// the coordinator); P03 declares it in m2m/model.hpp (claude/m2m-p03 9590da1a); the same
// declaration is repeated here until that merges.
std::vector<Operation*> flowOperations(const SubProject& sub);
}  // namespace m2m

namespace m2m::mst::p13 {

// @src 0x1400b82e0  last operation of the sub-project's flow (0x1400b5db0, 0x140047dc0), or null
// (noop.cpp; also the SfOprReport vt+0x190 of MDT L7)
Operation* lastOperation(const SubProject& sub);

// @src 0x141362f60  Distortion pass blocks (the writer body behind 0x141362e90). `self` is the
// Distortion class singleton (its `current` names the SIMNAM, 0x141363fc0).
void writeDistortionPasses(OpClass& self, W& w, Project& prj, const String& db, int* sim,
                           Operation* item, DistortionProps& dp, int dbreadArg, int onlySim);

// @src 0x141363fc0  SIMNAM of a Distortion stage: stage 0 "PS%02d"(pass), 1 "UNLOAD", 2 "LOAD";
// bare ? suffix : current operation name + suffix.
String distortionSimnam(const OpClass& self, int stage, int pass, bool bare);

}  // namespace m2m::mst::p13
