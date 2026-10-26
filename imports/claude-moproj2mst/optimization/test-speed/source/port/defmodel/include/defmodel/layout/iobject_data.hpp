// IObjectData (RTTI .?AVIObjectData@@): interface base of LLOBJ_DATA; three pure virtual slots,
// no data, no virtual dtor.
//
//   vtable   0x140522f20 (3 slots, all _purecall 1404852f0)
//   ctor     140199a70 (stores the vptr only; called by LLOBJ_DATA's ctor 14015ec80)
//   dtor     none in the binary (LLOBJ_DATA's dtor 14015f790 never restores this vptr)
//   size     8 (vptr only): LLOBJ_DATA's ctor writes nothing at 0x08..0x0f.
//   slots    named after LLOBJ_DATA's overrides (14015fa60 get_name, 14015faa0 set_name,
//            14015fbd0 returns LLOBJ_DATA+0x3c00).
#pragma once

#include <cstddef>
#include <cstdint>

#include <defmodel/layout/layout_check.hpp>

namespace def::model {

class IObjectData {
public:
    // @src 140199a70  ctor (vptr only)
    IObjectData();

    // @src 1404852f0  slot 0: _purecall (LLOBJ_DATA: copy the name out)
    virtual void get_name(char* out) const = 0;
    // @src 1404852f0  slot 1: _purecall (LLOBJ_DATA: set the name)
    virtual void set_name(const char* name) = 0;
    // @src 1404852f0  slot 2: _purecall (LLOBJ_DATA: returns the 8-byte field at 0x3c00)
    virtual void* vslot2() = 0;

protected:
    ~IObjectData() = default;  // no virtual dtor in the binary
};

static_assert(sizeof(IObjectData) == 0x8);

} // namespace def::model
