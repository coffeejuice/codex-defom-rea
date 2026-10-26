// SdctlGList (RTTI .?AVSdctlGList@@, base SdctlPtrCollection): generic singly linked pointer list
// with a cached current index; base of SdctlPtrList<T>.
//
//   vtable   0x14054efe8 (6 slots)
//   ctor     140330ee0 (SdctlPtrCollection ctor, vptr, zero 0x10/0x18/0x20/0x2c, 0x28 = -1,
//            0x30 = 0)
//   dtor     140331030 (virtual; scalar deleting dtor 140333120 in slot 2: clear, delete the
//            object at 0x30 via 140333190(p, 1), base dtor)
//   size     0x38: the ctor writes 8 bytes at 0x30; SdctlPtrList<_OBJ_GEO2D> adds nothing and
//            fills LLOBJ_DATA 0x3a60..0x3a98 exactly.
//   nodes    malloc'd {item, ?, next}: clear frees them and follows node+0x10.
#pragma once

#include <cstddef>
#include <cstdint>

#include <defmodel/layout/layout_check.hpp>
#include <defmodel/layout/sdctl_ptr_collection.hpp>

namespace def::model {

class SdctlGList : public SdctlPtrCollection {
public:
    // @src 140330ee0  ctor
    SdctlGList();

    // @src 14019ab70  slot 0: return the count (0x2c)
    std::int32_t count() const override;
    // @src 1403321a0  slot 1: reset fields, 140333230(u30, 0), slot 4 per item, free the nodes
    void clear() override;
    // @src 140333120  slot 2: scalar deleting dtor (dtor body 140331030)
    ~SdctlGList() override;
    // slot 3: inherited (140333f50)
    // slot 4: still pure (_purecall 1404852f0)
    // @src 140330ea0  slot 5: returns a != b
    virtual bool differs(const void* a, const void* b) const;

    void* head;               // 0x10 first node
    void* tail;               // 0x18
    void* u20;                // 0x20 (current node?)
    std::int32_t cur_index;   // 0x28 =-1
    std::int32_t n;           // 0x2c count (slot 0)
    void* u30;                // 0x30 owned helper object (deleted via 140333190(p, 1))
};

static_assert(sizeof(SdctlGList) == 0x38);
DEF_LAYOUT_OFFSET(SdctlGList, head, 0x10);
DEF_LAYOUT_OFFSET(SdctlGList, tail, 0x18);
DEF_LAYOUT_OFFSET(SdctlGList, u20, 0x20);
DEF_LAYOUT_OFFSET(SdctlGList, cur_index, 0x28);
DEF_LAYOUT_OFFSET(SdctlGList, n, 0x2c);
DEF_LAYOUT_OFFSET(SdctlGList, u30, 0x30);

} // namespace def::model
