// StateVarElemMATAXI (RTTI .?AVStateVarElemMATAXI@@, base StateVarElem):
// element state variable; with 9 values per element its slot 2 initialises each row to the 3x3 identity.
//
//   vtable   0x14054e028 (5 slots: 14032d0b0, 140301710, 14032cd50, 140301770, 140301850)
//   ctor     14032ccd0(this, id, slot)
//   dtor     14032cd20 (scalar deleting dtor 14032d0b0 in slot 0)
//   size     0x38
#pragma once

#include <cstddef>
#include <cstdint>

#include <defmodel/layout/layout_check.hpp>
#include <defmodel/layout/state_var_elem.hpp>

namespace def::model {

class StateVarElemMATAXI : public StateVarElem {
public:
    // @src 14032ccd0  ctor
    StateVarElemMATAXI(std::int32_t id, void* slot);
    // @src 14032d0b0  slot 0: scalar deleting dtor (dtor body 14032cd20)
    ~StateVarElemMATAXI() override;
    // @src 14032cd50  slot 2
    void vslot2_140301690(std::int32_t n, const std::int32_t* per_id, std::int32_t which) override;
};

static_assert(sizeof(StateVarElemMATAXI) == 0x38);

} // namespace def::model
