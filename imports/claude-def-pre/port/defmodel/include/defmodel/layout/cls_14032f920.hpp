// Cls_14032f920 (no RTTI, not polymorphic): owning handle to a heap std::list (VS2010 layout),
// member of Cls_14032f100.
//
//   ctor     14032f920: p = operator new(0x18); 1403303a0(p) builds an empty std::list:
//              p+0x00 allocator byte (140001000 is a no-op ctor), p+0x08 sentinel node
//              (operator new(0x48), 140330880; node->next = node->prev = node), p+0x10 size = 0.
//            List nodes are 0x48 bytes: next, prev, value (0x38 bytes) at +0x10. The value ctor
//            14032ece0 builds a string-like member at +0x08 (140005040) and an 8-byte member at
//            +0x30 (14032f7d0), then 14032eee0(value, 0).
//   dtor     14032f990 (-> 14032fe10(p): destroys the list, frees p)
//   size     8 (one pointer written by the ctor).
//   value    CustomVarDef (defmodel/members/custom_state_var.hpp): the rep is a refcounted
//            VarListRep; the "allocator byte" at p+0x00 is its refcount (140001000 sets 1).
//   open     meaning of the value; it sits next to the StateVarCustom code (vtable 0x14054ecb0,
//            slot 0 14032eca0), so it may describe user-defined state variables.
#pragma once

#include <cstddef>
#include <cstdint>

#include <defmodel/layout/layout_check.hpp>
#include <defmodel/members/custom_state_var.hpp>

namespace def::model {

class Cls_14032f920 {
public:
    // @src 14032f920  ctor (allocates the list)
    Cls_14032f920();
    // @src 14032f990  dtor (14032fe10 frees the list)
    ~Cls_14032f920();
    Cls_14032f920(const Cls_14032f920&) = delete;
    Cls_14032f920& operator=(const Cls_14032f920&) = delete;

    VarListRep* list;  // 0x00 heap refcounted list (0x18-byte rep, see above)
};

static_assert(sizeof(Cls_14032f920) == 0x8);
DEF_LAYOUT_OFFSET(Cls_14032f920, list, 0x0);

} // namespace def::model
