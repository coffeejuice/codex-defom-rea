// OPSTOP_DATA (RTTI .?AVOPSTOP_DATA@@, base SfLinkedData): node of OpStopLinkedList, one
// per-operation stop value (KEY keyword id 0x109, written by 1402a4020 as
// "%-6s %7i %7i %7i\n" then " %20.10E\n" for nodes whose flag u0c is 1).
//
//   vtable   0x140534e60 (4 slots)
//   ctor     14025ac10 (SfLinkedData ctor 14025a580, vptr, 14025ac90: u0c = 0, value = 0)
//   dtor     14025ac60 (virtual; scalar deleting dtor 14025c000 in slot 0)
//   size     0x20: OpStopLinkedList::create_item (14025aea0) does operator new(0x20); the value
//            at 0x18 is the only own field (8 bytes, printed with %E).
#pragma once

#include <cstddef>
#include <cstdint>

#include <defmodel/layout/layout_check.hpp>
#include <defmodel/layout/sf_linked_data.hpp>

namespace def::model {

class OPSTOP_DATA : public SfLinkedData {
public:
    // @src 14025ac10  ctor
    OPSTOP_DATA();

    // @src 14025c000  slot 0: scalar deleting dtor (dtor body 14025ac60)
    ~OPSTOP_DATA() override;
    // @src 14025acc0  slot 1: SfLinkedData::copy_from, then the value if other is an OPSTOP_DATA
    void copy_from(const SfLinkedData* other) override;
    // @src 14025ad20  slot 2: u0c = (int)rec[1]; value = rec[2] when u0c == 1
    void from_values(const double* rec) override;
    // @src 14025ad80  slot 3: rec[1] = (double)u0c; rec[2] = value when u0c == 1
    void to_values(double* rec) const override;

    // @src 14025ac90  init: u0c = 0, value = 0
    void reset();
    // @src 14025ade0  dynamic_cast<OPSTOP_DATA*> of a list node (null stays null)
    static OPSTOP_DATA* cast(SfLinkedData* node);
    static const OPSTOP_DATA* cast(const SfLinkedData* node);

    double value;             // 0x18 stop value (u0c is the on/off flag)
};

static_assert(sizeof(OPSTOP_DATA) == 0x20);
DEF_LAYOUT_OFFSET(OPSTOP_DATA, value, 0x18);

} // namespace def::model
