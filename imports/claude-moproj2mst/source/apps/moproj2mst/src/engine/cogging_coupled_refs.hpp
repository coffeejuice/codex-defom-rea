#pragma once

namespace def::model {
class LLOBJ_DATA;
}

namespace m2m::engine::detail {

// Apply the native MAN_COUPLE.DAT consequence at the existing object-specific
// point in Cogging placement. The caller supplies the qualified existence bit.
void applyCoupledObject5Reference(def::model::LLOBJ_DATA* object, bool coupled);
void applyCoupledObject7Reference(def::model::LLOBJ_DATA* object, bool coupled);

}  // namespace m2m::engine::detail
