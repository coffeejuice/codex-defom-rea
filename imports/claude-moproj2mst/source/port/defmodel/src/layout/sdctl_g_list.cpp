// SdctlGList: singly linked pointer list with a cached current index (p1-05).
// Layout: defmodel/layout/sdctl_g_list.hpp, docs/re/model-layout.md; node: defmodel/sdctl/sdctl_node.hpp.
#include <defmodel/layout/sdctl_g_list.hpp>

#include <cstdlib>

#include <defmodel/members/sdctl_index.hpp>
#include <defmodel/port.hpp>
#include <defmodel/sdctl/sdctl_node.hpp>

namespace def::model {

// @src 140330ee0  ctor: SdctlPtrCollection ctor 140333090, vptr, u20 = tail = head = 0,
//                 n = 0, cur_index = -1, u30 = 0
SdctlGList::SdctlGList()
{
    u20 = nullptr;
    tail = nullptr;
    head = nullptr;
    n = 0;
    cur_index = -1;
    u30 = nullptr;
}

// @src 14019ab70  slot 0: return the count at 0x2c
std::int32_t SdctlGList::count() const { return n; }

// @src 1403321a0  slot 1: detach the chain, reset the fields, reset the helper at 0x30
//                 (140333230(u30, 0)), then for each node: slot 4 (delete_item) on the item,
//                 free the node, follow node + 0x10
void SdctlGList::clear()
{
    auto* node = static_cast<SdctlGListNode*>(head);
    u20 = nullptr;
    tail = nullptr;
    head = nullptr;
    n = 0;
    cur_index = -1;
    if (u30 != nullptr) {
        // 14033220d: 140333230(u30, 0), p1-10's Cls_1403331d0::detach_records. The helper object
        // is created by code outside Phase 1; nothing in Phase 1 sets 0x30.
        static_cast<Cls_1403331d0*>(u30)->detach_records(false);
    }
    while (node != nullptr) {
        delete_item(node->item);   // virtual slot 4 (vtable + 0x20)
        SdctlGListNode* next = node->next;
        std::free(node);
        node = next;
    }
}

// @src 140331030  dtor: vptr SdctlGList, clear (1403321a0, called directly), delete the helper
//                 at 0x30 (140333190(p, 1)), u30 = 0, SdctlPtrCollection dtor 140333100
// @src 140333120  slot 2: scalar deleting dtor (dtor, then free when flag & 1)
SdctlGList::~SdctlGList()
{
    SdctlGList::clear();
    if (u30 != nullptr) Cls_1403331d0::destroy(static_cast<Cls_1403331d0*>(u30), 1); // 140333190(p, 1), p1-10
    u30 = nullptr;
}

// @src 140330ea0  slot 5: returns a != b
bool SdctlGList::differs(const void* a, const void* b) const { return a != b; }

} // namespace def::model
