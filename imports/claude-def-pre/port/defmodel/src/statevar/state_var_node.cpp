// StateVarNode (p1-09). Layout: defmodel/layout/state_var_node.hpp.
#include <defmodel/layout/state_var_node.hpp>

namespace def::model {

// @src 14032a990  ctor: base ctor, kind 1
StateVarNode::StateVarNode(std::int32_t id_, void* slot_) : StateVarBase(id_, slot_) { set_kind(1); }

// @src 14032ab10  slot 0: scalar deleting dtor
// @src 14032a9f0  dtor (vptr reset, then the base dtor 140300d50)
StateVarNode::~StateVarNode() = default;

} // namespace def::model
