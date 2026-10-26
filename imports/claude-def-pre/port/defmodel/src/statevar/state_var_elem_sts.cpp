// StateVarElemSTS (p1-09). Layout: defmodel/layout/state_var_elem_sts.hpp.
#include <defmodel/layout/state_var_elem_sts.hpp>

#include <defmodel/statevar/elem_owner.hpp>

namespace def::model {

// @src 14032c950  ctor: StateVarElem ctor, own vptr, owner at 0x38
StateVarElemSTS::StateVarElemSTS(std::int32_t id_, void* slot_, void* owner_) : StateVarElem(id_, slot_), owner(owner_) {}

// @src 14032cff0  slot 0: scalar deleting dtor
// @src 14032ca50  dtor (vptr reset, then the StateVarElem dtor 14032c920)
StateVarElemSTS::~StateVarElemSTS() = default;

// @src 14032c9b0  slot 2: like 140301690, and dim2 = 140183c20(owner, 2)
void StateVarElemSTS::vslot2_140301690(std::int32_t n, const std::int32_t* per_id, std::int32_t which)
{
    if (which == get_id() || which == -1) {
        set_dim0(n);
        set_dim1(per_id[get_id()]);
        set_dim2(statevar::owner_point_count(owner, 2));
        resize_values(false);
    }
}

} // namespace def::model
