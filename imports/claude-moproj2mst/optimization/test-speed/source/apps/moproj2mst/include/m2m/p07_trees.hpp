// m2m/p07_trees.hpp: default property tree of SfOprDistortionProperties
// (owner: P07, src/ops/p07_trees.cpp). The class's makeProps puts the tree in Props::tree, then
// makes the base adder calls listed with each function (m2m/props_base.hpp), in that order.
#pragma once

#include <vector>

#include "m2m/props.hpp"

namespace m2m::trees {

// @src 0x141366100  SfOprDistortionProperties tree build (from the props init 0x141365a70; templates
// 0x141365c90). classKey = OB vt+0x38 (the root's name). Then in makeProps, after the tree:
//   0x1405f9eb0 addStepCounts(p, false, false, 2, 1, "1.0 sec")
//   0x1405fa560 addEnvTemp(p, unit, "68.00 F", "20.00 C")
//   0x1405faaa0 addConvection(p, unit, "0.0 Btu/sec/in^2/F", "0.0 N/sec/mm/C")
//   0x1405fb020 addStepBy(p, unit, 0, "0.0 in", "0.0 mm", "0.0 sec")
std::vector<PropNode> distortionPropTree(const qt::String& classKey);

}  // namespace m2m::trees
