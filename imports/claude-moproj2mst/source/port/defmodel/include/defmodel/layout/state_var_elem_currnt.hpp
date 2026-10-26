// StateVarElemCURRNT (RTTI .?AVStateVarElemCURRNT@@, base StateVarElem):
// element state variable with an owner pointer; its slot 2 is a separate copy of the base slot 2.
//
//   vtable   0x14054e058 (5 slots: 14032d0f0, 140301710, 14032cf40, 140301770, 140301850)
//   ctor     14032ceb0(this, id, slot, owner)
//   dtor     14032cf10 (scalar deleting dtor 14032d0f0 in slot 0)
//   size     0x40
#pragma once

#include <cstddef>
#include <cstdint>

#include <defmodel/layout/layout_check.hpp>
#include <defmodel/layout/state_var_elem.hpp>

namespace def::model {

class StateVarElemCURRNT : public StateVarElem {
public:
    // @src 14032ceb0  ctor
    StateVarElemCURRNT(std::int32_t id, void* slot, void* owner);
    // @src 14032d0f0  slot 0: scalar deleting dtor (dtor body 14032cf10)
    ~StateVarElemCURRNT() override;
    // @src 14032cf40  slot 2
    void vslot2_140301690(std::int32_t n, const std::int32_t* per_id, std::int32_t which) override;

    void* owner;   // 0x38 the element state-variable container (LLOBJ_DATA + 0x9e0, Cls_140182880)
};

static_assert(sizeof(StateVarElemCURRNT) == 0x40);
DEF_LAYOUT_OFFSET(StateVarElemCURRNT, owner, 0x38);

} // namespace def::model
