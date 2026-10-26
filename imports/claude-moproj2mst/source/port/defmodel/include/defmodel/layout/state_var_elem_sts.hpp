// StateVarElemSTS (RTTI .?AVStateVarElemSTS@@, base StateVarElem): element state variable
// its slot 2 sets the third dimension to 140183c20(owner, 2) (1, 4 or 8).
//
//   vtable   0x14054df98 (5 slots: 14032cff0, 140301710, 14032c9b0, 140301770, 140301850)
//   ctor     14032c950(this, id, slot, owner)
//   dtor     14032ca50 (scalar deleting dtor 14032cff0 in slot 0)
//   size     0x40
#pragma once

#include <cstddef>
#include <cstdint>

#include <defmodel/layout/layout_check.hpp>
#include <defmodel/layout/state_var_elem.hpp>

namespace def::model {

class StateVarElemSTS : public StateVarElem {
public:
    // @src 14032c950  ctor
    StateVarElemSTS(std::int32_t id, void* slot, void* owner);
    // @src 14032cff0  slot 0: scalar deleting dtor (dtor body 14032ca50)
    ~StateVarElemSTS() override;
    // @src 14032c9b0  slot 2
    void vslot2_140301690(std::int32_t n, const std::int32_t* per_id, std::int32_t which) override;

    void* owner;   // 0x38 the element state-variable container (LLOBJ_DATA + 0x9e0, Cls_140182880)
};

static_assert(sizeof(StateVarElemSTS) == 0x40);
DEF_LAYOUT_OFFSET(StateVarElemSTS, owner, 0x38);

} // namespace def::model
