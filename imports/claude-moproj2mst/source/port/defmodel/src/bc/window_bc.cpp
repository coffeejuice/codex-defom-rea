// Window-list model functions for the window BCs (kw-g2bc): defmodel/bc/window_bc.hpp.
#include <defmodel/bc/window_bc.hpp>

#include <defmodel/linked/accessors.hpp>
#include <defmodel/port.hpp>

namespace def::model {

// @src 1402632d0  window has shape data
bool window_has_shape(const LLWINDEF_DATA* w)
{
    bool r;
    if (!window_is_3d(w)) { // 14025c970
        r = w->pts != nullptr;
    } else {
        if (w->shape == 3) {
            // 14026a3c0(w+0x220): node data of the _OBJ_GEO3D (p1-06's accessor)
            if (linked::member10_array_first(&w->geo3d) == nullptr) return false;
        } else if (w->pts == nullptr) {
            return false;
        }
        r = true;
    }
    return r;
}

// @src 1402696f0  unlink and delete a window
std::int32_t winlist_remove(Cls_140269250* l, LLWINDEF_DATA* w)
{
    LLWINDEF_DATA* prev = l->head;
    LLWINDEF_DATA* cur = l->head;
    while (cur != nullptr && cur != w) {
        prev = cur;
        cur = cur->next;
    }
    if (cur == nullptr) return 0;
    // __RTDynamicCast(cur, LLWINDEF_DATA -> LLRANGEDEF_DATA): when the node is an LLRANGEDEF_DATA,
    // its +0x920/+0x928 links are handed to the next (or previous) range node. The window lists
    // only hold plain LLWINDEF_DATA nodes (winlist_new, 1402694a0), so the cast fails and nothing
    // happens; LLRANGEDEF_DATA is not part of the port.
    prev->next = cur->next;
    if (l->head == cur) l->head = cur->next;
    if (l->tail == cur) l->tail = prev;
    if (cur != nullptr) delete cur; // vtable slot 0: scalar deleting dtor 14026a380(cur, 1)
    l->count = l->count + -1;
    return 1;
}

} // namespace def::model
