// p4-04's call into a Phase 3 function: forwarded to its port (Phase 4 gate).
#include "ext_p4_04.hpp"

#include <defmodel/llobj/object_list.hpp>

namespace def::dbio::gendb::ext {

// 140160f10: kw-g4b llobj_not_kind7
bool fun_140160f10(const LLOBJ_DATA* obj) { return model::llobj_not_kind7(obj); }

} // namespace def::dbio::gendb::ext
