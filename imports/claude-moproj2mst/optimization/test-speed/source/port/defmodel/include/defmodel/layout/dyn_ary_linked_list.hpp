// DynAryLinkedList (RTTI .?AVDynAryLinkedList@@, base SfLinkedList): list of DYNARY_DATA, embedded
// in LLOBJ_DATA at 0xba8.
//
//   vtable   0x140534ec8 (2 slots)
//   ctor     14025b9d0 (SfLinkedList ctor 14025a680, then vptr; no own fields)
//   dtor     14025ba10 (virtual; scalar deleting dtor 14025c0c0 in slot 0)
//   size     0x20 (= SfLinkedList; the ctor writes nothing else). LLOBJ_DATA's next member is at
//            0xbe0 = 0xba8 + 0x38, so the 0x18 bytes after it belong to LLOBJ_DATA (not written).
#pragma once

#include <cstddef>
#include <cstdint>

#include <defmodel/layout/layout_check.hpp>
#include <defmodel/layout/sf_linked_list.hpp>

namespace def::model {

class DynAryLinkedList : public SfLinkedList {
public:
    // @src 14025b9d0  ctor
    DynAryLinkedList();

    // @src 14025c0c0  slot 0: scalar deleting dtor (dtor body 14025ba10)
    ~DynAryLinkedList() override;
    // @src 14025ba40  slot 1: return new DYNARY_DATA (operator new(0x40), 14025afe0)
    SfLinkedData* create_item() override;
};

static_assert(sizeof(DynAryLinkedList) == 0x20);

} // namespace def::model
