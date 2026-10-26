// StateVarNode (RTTI .?AVStateVarNode@@, base StateVarBase): nodal state variable; kind (0x0c) = 1.
//
//   vtable   0x14054d1e0 (5 slots: 14032ab10, 140301710, 140301690, 140301770, 140301850)
//   ctor     14032a990(this, id, slot)
//   dtor     14032a9f0 (scalar deleting dtor 14032ab10 in slot 0)
//   size     0x38
#pragma once

#include <cstddef>
#include <cstdint>

#include <defmodel/layout/layout_check.hpp>
#include <defmodel/layout/state_var_base.hpp>

namespace def::model {

class StateVarNode : public StateVarBase {
public:
    // @src 14032a990  ctor
    StateVarNode(std::int32_t id, void* slot);
    // @src 14032ab10  slot 0: scalar deleting dtor (dtor body 14032a9f0)
    ~StateVarNode() override;
};

static_assert(sizeof(StateVarNode) == 0x38);

} // namespace def::model
