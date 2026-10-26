// p4-04's calls into p4-03 (object checks): forwarded (Phase 4 gate).
#include "ext_p4_04.hpp"

#include <defdbio/objcheck/objcheck.hpp>

namespace def::dbio::gendb::ext {

bool fun_1401792d0(LLOBJ_DATA* o) { return objcheck::obj_has_translation(o); } // 1401792d0
bool fun_1401797e0(LLOBJ_DATA* o) { return objcheck::obj_has_rotation(o); }    // 1401797e0
bool fun_140179280(LLOBJ_DATA* o) { return objcheck::obj_has_movement(o); }    // 140179280

// 1401bfdf0 / 1401bfe60 / 1401bfed0: checks 2, 3 and 4 (the binary passes the check index as the
// unused second argument)
void fun_1401bfdf0(DB_STEP_DATA& s, const std::int32_t* index, std::int32_t* err, std::int32_t* warn)
{
    if (g_checks_234_hook != nullptr) return g_checks_234_hook(2, s, err, warn);
    objcheck::check_material_data(&s, const_cast<std::int32_t*>(index), err, warn);
}
void fun_1401bfe60(DB_STEP_DATA& s, const std::int32_t* index, std::int32_t* err, std::int32_t* warn)
{
    if (g_checks_234_hook != nullptr) return g_checks_234_hook(3, s, err, warn);
    objcheck::check_inter_material_data(&s, const_cast<std::int32_t*>(index), err, warn);
}
void fun_1401bfed0(DB_STEP_DATA& s, const std::int32_t* index, std::int32_t* err, std::int32_t* warn)
{
    if (g_checks_234_hook != nullptr) return g_checks_234_hook(4, s, err, warn);
    objcheck::check_object_data(&s, const_cast<std::int32_t*>(index), err, warn);
}

// 14001cdc0: p4-03's port (an empty function)
void fun_14001cdc0(const std::int32_t*, std::int32_t*) { objcheck::fun_14001cdc0(); }

} // namespace def::dbio::gendb::ext
