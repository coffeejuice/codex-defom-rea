// The point count of StateVarElemSTS/TEPS/DAMAG (defmodel/statevar/elem_owner.hpp): p1-04's
// 140183c20 on the owning element container (Cls_140182880), wired at the Phase 1 gate in place
// of p1-09's local copy.
#include <defmodel/statevar/elem_owner.hpp>

#include <defmodel/llobj/cls_140182880.hpp>

namespace def::model::statevar {

// part of @src 140183c20  (Cls_140182880::fun_140183c20)
std::int32_t owner_point_count(const void* owner, std::int32_t k)
{
    return static_cast<const Cls_140182880*>(owner)->fun_140183c20(k);
}

} // namespace def::model::statevar
