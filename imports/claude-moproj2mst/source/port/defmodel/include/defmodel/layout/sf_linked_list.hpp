// SfLinkedList (RTTI .?AVSfLinkedList@@): polymorphic list of SfLinkedData nodes (derived:
// OpStopLinkedList, DynAryLinkedList). Owns its nodes: clearing deletes each through its virtual
// scalar deleting dtor.
//
//   vtable   0x140534e48 (2 slots)
//   ctor     14025a680 (vptr, 14025a6f0: head = tail = 0, count = 0)
//   dtor     14025a6c0 (virtual; scalar deleting dtor 14025bfc0 in slot 0; 14025ab20 clears)
//   size     0x20: 14025a6f0 writes 0x08, 0x10 (8 bytes each) and the int at 0x18.
#pragma once

#include <cstddef>
#include <cstdint>

#include <defmodel/layout/layout_check.hpp>
#include <defmodel/layout/sf_linked_data.hpp>

namespace def::model {

class SfLinkedList {
public:
    // @src 14025a680  ctor
    SfLinkedList();

    // @src 14025bfc0  slot 0: scalar deleting dtor (dtor body 14025a6c0)
    virtual ~SfLinkedList();
    // @src 1404852f0  slot 1: _purecall (derived lists: allocate a new node)
    virtual SfLinkedData* create_item() = 0;

    // @src 14025ab20  deletes every node through its virtual dtor, then head = tail = 0, count = 0
    void clear();

    // vptr at 0x00
    SfLinkedData* head;       // 0x08
    SfLinkedData* tail;       // 0x10
    std::int32_t count;       // 0x18
    std::byte pad_1c[4];      // 0x1c
};

static_assert(sizeof(SfLinkedList) == 0x20);
DEF_LAYOUT_OFFSET(SfLinkedList, head, 0x08);
DEF_LAYOUT_OFFSET(SfLinkedList, tail, 0x10);
DEF_LAYOUT_OFFSET(SfLinkedList, count, 0x18);

} // namespace def::model
