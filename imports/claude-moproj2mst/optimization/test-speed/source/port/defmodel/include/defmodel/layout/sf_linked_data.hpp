// SfLinkedData (RTTI .?AVSfLinkedData@@): polymorphic node base of the SfLinkedList lists
// (derived: OPSTOP_DATA, DYNARY_DATA).
//
//   vtable   0x140534e20 (4 slots)
//   ctor     14025a580 (vptr, then 14025a5e0: 0x08 = 1, 0x0c = 0, next = 0)
//   dtor     14025a5c0 (virtual; scalar deleting dtor 14025bf80 in slot 0)
//   size     0x18: 14025a5e0 writes the 8-byte next pointer at 0x10; SfLinkedList walks nodes
//            through node[2] (14025ab20); DYNARY_DATA's own fields start at 0x18 (14025b0d0).
#pragma once

#include <cstddef>
#include <cstdint>

#include <defmodel/layout/layout_check.hpp>

namespace def::model {

class SfLinkedData {
public:
    // @src 14025a580  ctor
    SfLinkedData();

    // @src 14025bf80  slot 0: scalar deleting dtor (dtor body 14025a5c0)
    virtual ~SfLinkedData();
    // @src 14025a610  slot 1: copies other->u0c into this->u0c
    virtual void copy_from(const SfLinkedData* other);
    // 1404852f0  slot 2: _purecall; fill the node from a value record (OPSTOP_DATA 14025ad20 reads
    // rec[1], rec[2]; DYNARY_DATA 14025b190 is empty)
    virtual void from_values(const double* rec) = 0;
    // 1404852f0  slot 3: _purecall; store the node into a value record (OPSTOP_DATA 14025ad80;
    // DYNARY_DATA 14025b1b0 is empty)
    virtual void to_values(double* rec) const = 0;

    // vptr at 0x00
    std::uint8_t u08;         // 0x08 =1
    std::int32_t u0c;         // 0x0c =0; copied by slot 1
    SfLinkedData* next;       // 0x10 =0; list link
};

static_assert(sizeof(SfLinkedData) == 0x18);
DEF_LAYOUT_OFFSET(SfLinkedData, u08, 0x08);
DEF_LAYOUT_OFFSET(SfLinkedData, u0c, 0x0c);
DEF_LAYOUT_OFFSET(SfLinkedData, next, 0x10);

} // namespace def::model
