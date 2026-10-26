// OpStopLinkedList (RTTI .?AVOpStopLinkedList@@, base SfLinkedList): list of OPSTOP_DATA
// (LLOBJ_DATA+0x20b8 per the KEY writer 1402a4020; the LLOBJ_DATA dtor 14038b8a0 clears it).
//
//   vtable   0x140534e88 (2 slots)
//   ctor     14025ae30 (SfLinkedList ctor 14025a680, then vptr; no own fields)
//   dtor     14025ae70 (virtual; scalar deleting dtor 14025c040 in slot 0)
//   size     0x20 (= SfLinkedList).
#pragma once

#include <cstddef>
#include <cstdint>

#include <defmodel/layout/layout_check.hpp>
#include <defmodel/layout/sf_linked_list.hpp>

namespace def::model {

class OpStopLinkedList : public SfLinkedList {
public:
    // @src 14025ae30  ctor
    OpStopLinkedList();

    // @src 14025c040  slot 0: scalar deleting dtor (dtor body 14025ae70)
    ~OpStopLinkedList() override;
    // @src 14025aea0  slot 1: return new OPSTOP_DATA (operator new(0x20), 14025ac10)
    SfLinkedData* create_item() override;
};

static_assert(sizeof(OpStopLinkedList) == 0x20);

} // namespace def::model
