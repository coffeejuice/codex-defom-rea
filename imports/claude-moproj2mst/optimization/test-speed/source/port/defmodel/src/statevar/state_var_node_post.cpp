// StateVarNodePost (p1-09). Layout: defmodel/layout/state_var_node_post.hpp.
#include <defmodel/layout/state_var_node_post.hpp>

namespace def::model {

// @src 14032aa20  ctor: StateVarNode ctor, own vptr
StateVarNodePost::StateVarNodePost(std::int32_t id_, void* slot_) : StateVarNode(id_, slot_) {}

// @src 14032ab50  slot 0: scalar deleting dtor
// @src 14032aa70  dtor (vptr reset, then the StateVarNode dtor 14032a9f0)
StateVarNodePost::~StateVarNodePost() = default;

// @src 14032aaa0  slot 2: like 140301690, but only for its own id (-1 does not match)
void StateVarNodePost::vslot2_140301690(std::int32_t n, const std::int32_t* per_id, std::int32_t which)
{
    if (which == get_id()) {
        set_dim0(n);
        set_dim1(per_id[get_id()]);
        resize_values(false);
    }
}

} // namespace def::model
