// StateVarElem (p1-09). Layout: defmodel/layout/state_var_elem.hpp.
#include <defmodel/layout/state_var_elem.hpp>

namespace def::model {

// @src 14032c8c0  ctor: base ctor, kind 2
StateVarElem::StateVarElem(std::int32_t id_, void* slot_) : StateVarBase(id_, slot_) { set_kind(2); }

// @src 14032cfb0  slot 0: scalar deleting dtor
// @src 14032c920  dtor (vptr reset, then the base dtor 140300d50)
StateVarElem::~StateVarElem() = default;

} // namespace def::model
