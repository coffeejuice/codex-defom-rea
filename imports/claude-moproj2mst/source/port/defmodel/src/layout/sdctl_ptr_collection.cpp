// SdctlPtrCollection: abstract pointer collection, base of SdctlGList (p1-05).
// Layout: defmodel/layout/sdctl_ptr_collection.hpp, docs/re/model-layout.md.
#include <defmodel/layout/sdctl_ptr_collection.hpp>

namespace def::model {

// @src 140333090  ctor: vptr SdctlPtrCollection, owns = 0
SdctlPtrCollection::SdctlPtrCollection() : owns(0), pad_09{} {}

// @src 140333100  dtor: only resets the vptr
// @src 1403330c0  slot 2: scalar deleting dtor (dtor, then free when flag & 1)
SdctlPtrCollection::~SdctlPtrCollection() = default;

// @src 140333f50  slot 3: returns its argument unchanged
void* SdctlPtrCollection::vslot3_140333f50(void* item) { return item; }

} // namespace def::model
