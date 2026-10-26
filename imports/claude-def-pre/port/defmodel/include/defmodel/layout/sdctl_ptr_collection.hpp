// SdctlPtrCollection (RTTI .?AVSdctlPtrCollection@@): abstract pointer collection, base of
// SdctlGList.
//
//   vtable   0x14054f020 (5 slots)
//   ctor     140333090 (vptr, owns = 0)
//   dtor     140333100 (virtual; scalar deleting dtor 1403330c0 in slot 2)
//   size     0x10: the ctor writes the byte at 0x08; SdctlGList's own fields start at 0x10.
//   slots    pure slots are named after what SdctlGList / SdctlPtrList<_OBJ_GEO2D> put there.
#pragma once

#include <cstddef>
#include <cstdint>

#include <defmodel/layout/layout_check.hpp>

namespace def::model {

class SdctlPtrCollection {
public:
    // @src 140333090  ctor
    SdctlPtrCollection();

    // @src 1404852f0  slot 0: _purecall (SdctlGList: returns the count at 0x2c)
    virtual std::int32_t count() const = 0;
    // @src 1404852f0  slot 1: _purecall (SdctlGList: remove all, calling slot 4 per item)
    virtual void clear() = 0;
    // @src 1403330c0  slot 2: scalar deleting dtor (dtor body 140333100)
    virtual ~SdctlPtrCollection();
    // @src 140333f50  slot 3: returns its argument unchanged
    virtual void* vslot3_140333f50(void* item);
    // @src 1404852f0  slot 4: _purecall (SdctlPtrList: delete the item when owns != 0)
    virtual void delete_item(void* item) = 0;

    // vptr at 0x00
    std::uint8_t owns;        // 0x08 =0; SdctlPtrList's slot 4 deletes items only when set
    std::byte pad_09[7];      // 0x09
};

static_assert(sizeof(SdctlPtrCollection) == 0x10);
DEF_LAYOUT_OFFSET(SdctlPtrCollection, owns, 0x08);

} // namespace def::model
