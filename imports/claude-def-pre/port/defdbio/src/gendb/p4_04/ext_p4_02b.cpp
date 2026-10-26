// p4-04's calls into p4-02b: forwarded (Phase 4 gate).
#include "ext_p4_04.hpp"

#include <defdbio/gendb/counts_frame.hpp>

namespace def::dbio::gendb::ext {

// 14001cd60: p4-02b diag_14001cd60 (an empty function)
void fun_14001cd60(const std::int32_t* code, const char* keyword, const std::uint32_t* value, const std::int32_t* b)
{
    diag_14001cd60(code, keyword, value, b);
}
// 1401f4610: p4-02b object_count_field
std::int32_t fun_1401f4610(const DB_STEP_DATA* s) { return object_count_field(const_cast<DB_STEP_DATA*>(s)); }
// 1401f98d0: p4-02b inter_object_window_count
std::int32_t fun_1401f98d0(const Cls_1401f46e0* rec) { return inter_object_window_count(const_cast<Cls_1401f46e0*>(rec)); }
// 14029c410: p4-02b material_list_max_number
std::int32_t fun_14029c410(MaterialList* l) { return material_list_max_number(l); }

} // namespace def::dbio::gendb::ext
