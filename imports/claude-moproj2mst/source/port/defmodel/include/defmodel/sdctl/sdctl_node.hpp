// defmodel/sdctl/sdctl_node.hpp: the node of SdctlGList (layout/sdctl_g_list.hpp).
// Nodes come from malloc and SdctlGList::clear (1403321a0) frees them with free(), following
// node + 0x10. The node-creating methods (append/insert) are not Phase 1 functions; the field at
// 0x08 is not read by any Phase 1 code (likely the previous node).
#pragma once

#include <cstddef>
#include <cstdint>

#include <defmodel/layout/layout_check.hpp>

namespace def::model {

struct SdctlGListNode {
    void* item;              // 0x00 the stored pointer (passed to slot 4 by clear)
    void* u08;               // 0x08
    SdctlGListNode* next;    // 0x10
};

static_assert(sizeof(SdctlGListNode) == 0x18);
DEF_LAYOUT_OFFSET(SdctlGListNode, next, 0x10);

} // namespace def::model
