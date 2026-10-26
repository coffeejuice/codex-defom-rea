// StateVarElemDAMAG (RTTI .?AVStateVarElemDAMAG@@, base StateVarElem): element state variable
// its slot 2 sets the third dimension to 140183c20(owner, 0) (1, 4 or 8).
//
//   vtable   0x14054dff8 (5 slots: 14032d070, 140301710, 14032cc40, 140301770, 140301850)
//   ctor     14032cbb0(this, id, slot, owner)
//   dtor     14032cc10 (scalar deleting dtor 14032d070 in slot 0)
//   size     0x40
#pragma once

#include <cstddef>
#include <cstdint>

#include <defmodel/layout/layout_check.hpp>
#include <defmodel/layout/state_var_elem.hpp>

namespace def::model {

class StateVarElemDAMAG : public StateVarElem {
public:
    // @src 14032cbb0  ctor
    StateVarElemDAMAG(std::int32_t id, void* slot, void* owner);
    // @src 14032d070  slot 0: scalar deleting dtor (dtor body 14032cc10)
    ~StateVarElemDAMAG() override;
    // @src 14032cc40  slot 2
    void vslot2_140301690(std::int32_t n, const std::int32_t* per_id, std::int32_t which) override;

    void* owner;   // 0x38 the element state-variable container (LLOBJ_DATA + 0x9e0, Cls_140182880)
};

static_assert(sizeof(StateVarElemDAMAG) == 0x40);
DEF_LAYOUT_OFFSET(StateVarElemDAMAG, owner, 0x38);

} // namespace def::model
