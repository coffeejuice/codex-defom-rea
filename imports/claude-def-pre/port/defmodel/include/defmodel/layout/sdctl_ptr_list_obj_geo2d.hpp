// SdctlPtrList_OBJ_GEO2D = RTTI .?AV?$SdctlPtrList@V_OBJ_GEO2D@@@@ (SdctlPtrList<_OBJ_GEO2D>,
// bases SdctlGList, SdctlPtrCollection): list of 2D geometry objects, embedded in LLOBJ_DATA at
// 0x3a60. Written as a plain class named after the instantiation; make it a template once a
// second instantiation is ported (SdctlPtrList<CsdspmSimulationProcessMonitorDataSource::CRecord>
// also exists, but it is process-monitor code and out of scope).
//
//   vtable   0x1405230c0 (6 slots)
//   ctor     14019aae0 (SdctlGList ctor 140330ee0, then vptr)
//   dtor     14019ab10 (virtual; scalar deleting dtor 14019ad70 in slot 2: clear, base dtor)
//   size     0x38 (= SdctlGList; the ctor writes nothing else; LLOBJ_DATA's next field is at
//            0x3a98 = 0x3a60 + 0x38).
#pragma once

#include <cstddef>
#include <cstdint>

#include <defmodel/layout/layout_check.hpp>
#include <defmodel/layout/sdctl_g_list.hpp>

namespace def::model {

class SdctlPtrList_OBJ_GEO2D : public SdctlGList {
public:
    // @src 14019aae0  ctor
    SdctlPtrList_OBJ_GEO2D();

    // @src 14019ab50  slot 0: -> 14019ab70 (count)
    std::int32_t count() const override;
    // @src 14019abb0  slot 1: -> 1403321a0 (clear)
    void clear() override;
    // @src 14019ad70  slot 2: scalar deleting dtor (dtor body 14019ab10)
    ~SdctlPtrList_OBJ_GEO2D() override;
    // slot 3: inherited (140333f50)
    // @src 14019b980  slot 4: if owns and item != 0: item's virtual scalar deleting dtor (flag 1)
    void delete_item(void* item) override;
    // slot 5: inherited (140330ea0)
};

static_assert(sizeof(SdctlPtrList_OBJ_GEO2D) == 0x38);

} // namespace def::model
