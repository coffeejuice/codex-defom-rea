// p4-04's call into p4-02a: forwarded (Phase 4 gate).
#include "ext_p4_04.hpp"

#include <defdbio/gendb/obj_counts.hpp>

namespace def::dbio::gendb::ext {

// 1401f3cb0: p4-02a simctrl_u118
std::int32_t fun_1401f3cb0(const Cls_1401e7b10* sim) { return simctrl_u118(sim); }

} // namespace def::dbio::gendb::ext
