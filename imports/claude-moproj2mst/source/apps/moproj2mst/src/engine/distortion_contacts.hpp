#pragma once

#include "engine_internal.hpp"

namespace m2m::engine {

// @src 0x141364140 SfOprDistortion vt+0x198. The caller supplies the active
// operation represented by the class's current pointer. Invalid property/data
// state remains unsupported; it is not an original empty-result branch.
std::vector<ContactPair> distortionDefaultPairs(Operation& op);

} // namespace m2m::engine
