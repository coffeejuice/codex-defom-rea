// StateVarNodePost (RTTI .?AVStateVarNodePost@@, base StateVarNode):
// nodal state variable whose slot 2 reacts only to its own id (not to -1).
//
//   vtable   0x14054d210 (5 slots: 14032ab50, 140301710, 14032aaa0, 140301770, 140301850)
//   ctor     14032aa20(this, id, slot)
//   dtor     14032aa70 (scalar deleting dtor 14032ab50 in slot 0)
//   size     0x38
#pragma once

#include <cstddef>
#include <cstdint>

#include <defmodel/layout/layout_check.hpp>
#include <defmodel/layout/state_var_node.hpp>

namespace def::model {

class StateVarNodePost : public StateVarNode {
public:
    // @src 14032aa20  ctor
    StateVarNodePost(std::int32_t id, void* slot);
    // @src 14032ab50  slot 0: scalar deleting dtor (dtor body 14032aa70)
    ~StateVarNodePost() override;
    // @src 14032aaa0  slot 2
    void vslot2_140301690(std::int32_t n, const std::int32_t* per_id, std::int32_t which) override;
};

static_assert(sizeof(StateVarNodePost) == 0x38);

} // namespace def::model
