// SfOprCogging default property tree (packet P11). Decompile: gui_mo/out_p11/props_d0.c, root_d2.c.
#include "m2m/props_tree.hpp"

namespace m2m::tree {

// @src 0x14075eb30 (this = props+0x10): the class root from template 0x143da65a0 (made by the
// initializer 0x14075e170: group "Cogging", display "[3D] Cogging"), kept at props+0x38
// (0x1400ac5a0). The base groups that follow are added by makeProps (cogging.cpp).
std::vector<PropNode> treeCogging()
{
    std::vector<PropNode> t;
    add(t, group("Cogging"));      // 0x142678120
    return t;
}

}  // namespace m2m::tree
