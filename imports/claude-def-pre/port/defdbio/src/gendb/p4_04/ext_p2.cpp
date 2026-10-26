// p4-04's calls into Phase 2 functions: forwarded to their ports (Phase 4 gate).
#include "ext_p4_04.hpp"

#include <defkey/gendb_deps.hpp>
#include <defmodel/step/db_step.hpp>
#include <defmodel/window/inter_object_ops.hpp>
#include <defmodel/window/window_list.hpp>

namespace def::dbio::gendb::ext {

// 1401ee580: kw-g1a time_flag (S+0x240)
std::int32_t fun_1401ee580(const Cls_1401e7b10* sim) { return key::g1a::ops::time_flag(*sim); }

// 1401f3c10 / 1401f3c30: kw-g1a trans0 / trans1
std::int32_t fun_1401f3c10(const Cls_1401e7b10* sim) { return key::g1a::ops::trans0(*sim); }
std::int32_t fun_1401f3c30(const Cls_1401e7b10* sim) { return key::g1a::ops::trans1(*sim); }

// 1400015d0 / 1401fac90 / 1401facf0: the inter-object list ports (defmodel/window)
std::int32_t fun_1400015d0(const Cls_1401fa020* l) { return model::pair_count(l); }
Cls_1401f46e0* fun_1401fac90(Cls_1401fa020* l, std::int32_t n) { return model::pair_nth(l, n); }
Cls_1401f46e0* fun_1401facf0(Cls_1401fa020* l, std::int32_t a, std::int32_t b) { return model::pair_find(l, a, b); }

// 140269900: window list port
void* fun_140269900(void* window_list, std::int32_t n)
{
    return model::winlist_nth(static_cast<const model::Cls_140269250*>(window_list), n);
}

// 1401d6ea0: kw-g1a find_object
LLOBJ_DATA* fun_1401d6ea0(DB_STEP_DATA& s, std::int32_t number) { return key::g1a::ops::find_object(s, number); }

} // namespace def::dbio::gendb::ext
