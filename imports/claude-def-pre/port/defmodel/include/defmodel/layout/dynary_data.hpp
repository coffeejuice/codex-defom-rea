// DYNARY_DATA (RTTI .?AVDYNARY_DATA@@, base SfLinkedData): node of DynAryLinkedList (a dynamic
// array record of an object).
//
//   vtable   0x140534ea0 (4 slots)
//   ctor     14025afe0 (SfLinkedData ctor 14025a580, vptr, handles at 0x30/0x38, 14025b0b0 ->
//            14025b0d0 init)
//   dtor     14025b050 (virtual; scalar deleting dtor 14025c080 in slot 0)
//   size     0x40: DynAryLinkedList::create_item (14025ba40) does operator new(0x40).
//   arrays   0x30 and 0x38 are reference-counted array handles (p1-05's CowVector, defmodel/sdctl/cow.hpp);
//            0x30 clones with 8-byte elements (1400033f0), 0x38 with 4-byte elements (140002ba0).
//   open     what the elements are (nothing in Phase 1 fills them).
#pragma once

#include <cstddef>
#include <cstdint>

#include <defmodel/layout/layout_check.hpp>
#include <defmodel/layout/sf_linked_data.hpp>
#include <defmodel/sdctl/cow.hpp>

namespace def::model {

class DYNARY_DATA : public SfLinkedData {
public:
    // @src 14025afe0  ctor
    DYNARY_DATA();

    // @src 14025c080  slot 0: scalar deleting dtor (dtor body 14025b050)
    ~DYNARY_DATA() override;
    // @src 14025b150  slot 1: SfLinkedData::copy_from, then 14025b1d0(other)
    void copy_from(const SfLinkedData* other) override;
    // @src 14025b190  slot 2: empty
    void from_values(const double* rec) override;
    // @src 14025b1b0  slot 3: empty
    void to_values(double* rec) const override;

    // @src 14025b0b0  (thunk) / 14025b0d0: zero the fields, clear both arrays
    void reset();
    // @src 14025b1d0  dynamic_cast<DYNARY_DATA*> of a list node (null stays null)
    static DYNARY_DATA* cast(SfLinkedData* node);
    static const DYNARY_DATA* cast(const SfLinkedData* node);

    std::int32_t u18;         // 0x18 14025b0d0 =0
    std::uint8_t u1c;         // 0x1c =0
    std::int32_t u20;         // 0x20 =0
    std::int32_t u24;         // 0x24 =0
    std::int32_t u28;         // 0x28 =0
    CowVector<std::uint64_t> u30; // 0x30 8-byte-element array (14019a6b0), cleared by 1402568b0
    CowVector<std::int32_t> u38;  // 0x38 4-byte-element array (140159ad0), cleared by 14019a7d0
};

static_assert(sizeof(DYNARY_DATA) == 0x40);
DEF_LAYOUT_OFFSET(DYNARY_DATA, u18, 0x18);
DEF_LAYOUT_OFFSET(DYNARY_DATA, u1c, 0x1c);
DEF_LAYOUT_OFFSET(DYNARY_DATA, u20, 0x20);
DEF_LAYOUT_OFFSET(DYNARY_DATA, u28, 0x28);
DEF_LAYOUT_OFFSET(DYNARY_DATA, u30, 0x30);
DEF_LAYOUT_OFFSET(DYNARY_DATA, u38, 0x38);

} // namespace def::model
