// StateVarElemCURRNT (p1-09). Layout: defmodel/layout/state_var_elem_currnt.hpp.
#include <defmodel/layout/state_var_elem_currnt.hpp>

namespace def::model {

// @src 14032ceb0  ctor: StateVarElem ctor, own vptr, owner at 0x38
StateVarElemCURRNT::StateVarElemCURRNT(std::int32_t id_, void* slot_, void* owner_)
    : StateVarElem(id_, slot_), owner(owner_)
{
}

// @src 14032d0f0  slot 0: scalar deleting dtor
// @src 14032cf10  dtor (vptr reset, then the StateVarElem dtor 14032c920)
StateVarElemCURRNT::~StateVarElemCURRNT() = default;

// @src 14032cf40  slot 2: the same code as 140301690 (a separate copy in the binary)
void StateVarElemCURRNT::vslot2_140301690(std::int32_t n, const std::int32_t* per_id, std::int32_t which)
{
    if (which == get_id() || which == -1) {
        set_dim0(n);
        set_dim1(per_id[get_id()]);
        resize_values(false);
    }
}

} // namespace def::model
