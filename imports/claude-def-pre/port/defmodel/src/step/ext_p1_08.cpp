// p1-03 calls into p1-08 (see ext.hpp), wired to the owner's material functions at the Phase 1
// gate.
#include "ext.hpp"

#include <defmodel/mesh2d/material_ops.hpp>

namespace def::model::ext {

// part of @src 14029b500  material list reset
void fun_14029b500(MaterialList* l) { material_list_reset(l); }

// part of @src 14029c310  material list clear
void fun_14029c310(MaterialList* l) { material_list_clear(l); }

// part of @src 14029c260  material list node n
Material* fun_14029c260(MaterialList* l, std::int32_t n) { return material_list_nth(l, n); }

// part of @src 14029c2c0  material by id
Material* fun_14029c2c0(MaterialList* l, std::int32_t id) { return material_list_find(l, id); }

// part of @src 14029b300  material: number of ODF records
std::int32_t fun_14029b300(Material* m) { return material_num_odfs(m); }

// part of @src 14029bcd0  material list: grain record count
std::int32_t fun_14029bcd0(MaterialList* l, const std::int32_t* id, std::int32_t* out)
{
    return material_list_grain_record_count(l, id, out);
}

} // namespace def::model::ext
