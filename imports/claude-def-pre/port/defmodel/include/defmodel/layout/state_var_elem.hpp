// StateVarElem (RTTI .?AVStateVarElem@@, base StateVarBase): element state variable; kind (0x0c) = 2.
//
//   vtable   0x14054df68 (5 slots: 14032cfb0, 140301710, 140301690, 140301770, 140301850)
//   ctor     14032c8c0(this, id, slot)
//   dtor     14032c920 (scalar deleting dtor 14032cfb0 in slot 0)
//   size     0x38
#pragma once

#include <cstddef>
#include <cstdint>

#include <defmodel/layout/layout_check.hpp>
#include <defmodel/layout/state_var_base.hpp>

namespace def::model {

class StateVarElem : public StateVarBase {
public:
    // @src 14032c8c0  ctor
    StateVarElem(std::int32_t id, void* slot);
    // @src 14032cfb0  slot 0: scalar deleting dtor (dtor body 14032c920)
    ~StateVarElem() override;
};

static_assert(sizeof(StateVarElem) == 0x38);

} // namespace def::model
