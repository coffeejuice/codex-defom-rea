// Current-material helpers of the TEXT_PRE material menu (packet p10-01).
#include <defmodel/step/material_select.hpp>

#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/material/material_ctor.hpp>
#include <defmodel/material/state_remap.hpp>
#include <defmodel/mesh2d/material_ops.hpp>

namespace def::model {

// @src 14019cfb0  find or add a material
Material* step_material_get_or_add(DB_STEP_DATA& s, std::int32_t number)
{
    Material* m = material_list_find(&s.materials, number); // 14029c2c0
    if (m == nullptr) {
        material_list_rebuild_index(&s.materials, false); // 14029ca10(list, 0)
        m = material_list_add(&s.materials);              // 14029c060
        m->number = number;
        s.n_materials = s.materials.count;                // 140199c50: list +0x10
        step_remap_material_state(s, true);               // 1401d0fc0(step, 1)
        material_list_rebuild_index(&s.materials, true);  // 14029ca10(list, 1)
    }
    return m;
}

// @src 1401ca9a0  +0x1b48 = number
void step_set_current_material(DB_STEP_DATA& s, std::int32_t number)
{
    s.u1b48 = number;
}

// @src 1401ca9c0  +0x1b48
std::int32_t step_current_material(const DB_STEP_DATA& s)
{
    return s.u1b48;
}

} // namespace def::model
