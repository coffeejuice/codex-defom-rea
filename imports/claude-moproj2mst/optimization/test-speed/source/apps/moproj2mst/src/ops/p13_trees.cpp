// Default property trees of SfOprGroup and SfOprBoolean (owner P13). Decompiles:
// deform-rea-work gui_mo/s1/p13/t1.c .. t3.c. Neither class adds a node or calls a base adder.
#include "m2m/props_tree.hpp"

namespace m2m::tree {

// @src 0x141352b80  SfOprGroupProperties ctor -> 0x141352c50: props+0x38 = new 8-byte data
// (0x140bc4a80: a list, 0x14015d170), then 0x141352d90 and 0x141352e10, which compare the
// parent's name with "SfOprGroup" and add nothing either way.
std::vector<PropNode> treeGroup()
{
    return {};
}

// @src 0x1411e0780  SfOprBooleanProperties ctor -> 0x1411e0850: only props+0x38 = BooleanData
// (0x1411e23f0); no tree builder.
std::vector<PropNode> treeBoolean(const String&)
{
    return {};
}

}  // namespace m2m::tree
