// GENDB keyword command: model pre-steps (defdbio/gendb/pre_steps.hpp).
#include <defdbio/gendb/pre_steps.hpp>

#include <cstdint>
#include <cstdlib>

#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/layout/material.hpp>
#include <defmodel/mesh2d/material_ops.hpp>
#include <defmodel/step/db_step.hpp>

namespace def::dbio::gendb {

// @src 1401d7f40  GENDB command pre-steps
void gendb_command_pre_steps(model::DB_STEP_DATA* m)
{
    model::sim_ctrl_update_u240(m->sim_ctrl); // 1401ee510 (m + 0x750)
    mixture_material_cleanup(m);              // 1401d6250
    objects_surface_normals(m, 0);            // 1401d63c0(m, 0)
    if (m->u2f88 != 0) {
        if (m->sim_ctrl.nstart >= 0) { // +0x8b0
            // 1401d7f99: ADD ECX,1; NEG ECX (32-bit wraparound)
            m->sim_ctrl.nstart = static_cast<std::int32_t>(
                0u - (static_cast<std::uint32_t>(m->sim_ctrl.nstart) + 1u));
        }
        m->u744 = m->sim_ctrl.nstart;
    }
}

// @src 1401d6250  mixture-material cleanup
void mixture_material_cleanup(model::DB_STEP_DATA* m)
{
    for (std::int32_t i = 0; i < m->n_materials; i = i + 1) { // +0x1320
        model::Material* mix = model::material_list_nth(&m->materials, i + 1); // 14029c260
        if (mix != nullptr && mix->mstmtr_flag == 1) {
            // the count and the list are re-read every iteration (a component may be mix itself)
            for (std::int32_t k = 0; k < mix->mstmtr_count; k = k + 1) {
                model::Material* c = model::material_list_find(&m->materials, mix->mstmtr_list[k]); // 14029c2c0
                if (c != nullptr && c->mstmtr_flag != 2) {
                    if (c->mstmtr_count > 0) {
                        if (c->mstmtr_list != nullptr) {
                            std::free(c->mstmtr_list); // 140479910
                        }
                        c->mstmtr_list = nullptr;
                        c->mstmtr_list = nullptr; // stored twice in the original
                        c->mstmtr_count = 0;
                    }
                    c->mstmtr_flag = 2;
                }
            }
        }
    }
}

// @src 1401d63c0  surface normals of every object not of type 7
void objects_surface_normals(model::DB_STEP_DATA* m, char flag)
{
    model::LLOBJ_DATA* obj = nullptr;
    for (std::int32_t i = 0; i < m->u1378; i = i + 1) {
        obj = m->objects.at(i + 1); // 14017aa80 (m + 0x1380)
        if (obj != nullptr && model::field<std::int32_t>(obj, 0xc) != 7) {
            object_surface_normals(obj, flag); // 1402059a0
        }
    }
}

} // namespace def::dbio::gendb
