// DB_STEP_DATA-level GENDB steps of packet p4-06 (see defdbio/gendb/p4_06_frame.hpp).
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <defdbio/gendb/p4_06_frame.hpp>

#include <cstring>

#include <defdbio/gendb/ext_p4_06.hpp>
#include <defdbio/gendb/p4_06_interobj.hpp>
#include <defdbio/gendb/p4_06_material.hpp>
#include <defdbio/gendb/p4_06_obj2d.hpp>
#include <defdbio/gendb/p4_06_simctrl.hpp>
#include <defmodel/step/db_step.hpp>
#include <defrt/stdlib/refcount.hpp>

namespace def::dbio::gendb {

using model::DB_STEP_DATA;
using model::field;

namespace {

// 14017b370 (p4-05): the 3D object writer, same arguments as 14017cb60
void write_objects_3d(model::Cls_14017a6c0* list, std::int32_t* mar, double* a, std::int32_t* mt, std::int32_t* at,
                      std::int32_t* iifnc, double* llfnc, double* llwin, double* llwrz)
{
    ext::call(0x14017b370, {ext::u(list), ext::u(mar), ext::u(a), ext::u(mt), ext::u(at), ext::u(iifnc),
                            ext::u(llfnc), ext::u(llwin), ext::u(llwrz)});
}

// A address table word k of the model (m+0x1dc0 + 4k)
inline std::int32_t* a_table(DB_STEP_DATA* m, std::int32_t k) { return m->u1dc0 + k; }

// one block of 64-character names (1401c4a30 / 1401c4be0)
void write_names(char* out, std::int32_t count, char** names)
{
    for (std::int32_t i = 0; i < count; ++i) {
        for (std::int32_t k = 0; k < 0x40; ++k) {
            out[i * 0x40 + k] = ' ';
        }
        if (names != nullptr) {
            std::int32_t n = static_cast<std::int32_t>(std::strlen(names[i]));
            if (0x3f < n) {
                n = 0x40;
            }
            if (0 < n) {
                std::strncpy(out + i * 0x40, names[i], static_cast<std::size_t>(n));
            }
        }
    }
}

} // namespace

// @src 1401c4950
void write_step_sim_ctrl(DB_STEP_DATA* m, std::int32_t* iisim, double* llsim, double* llcpu, double* llfnc,
                         std::int32_t* iifnc, std::int32_t* iiuvl)
{
    (void)llcpu; // passed in, not used
    write_sim_ctrl_to_db(&m->sim_ctrl, iisim, llsim, iiuvl, iifnc, llfnc);
}

// @src 1401c49b0
void write_step_materials(DB_STEP_DATA* m, double* llmpr, double* llfnc, std::int32_t* iifnc)
{
    std::int32_t dim = m->dimension; // 140158e90
    model::select_index_tables(&dim);
    write_material_list(&m->materials, llmpr, iifnc, llfnc, a_table(m, model::g_a_index[60])); // DAT_1406e83c0
}

// @src 1401c4a30
void write_step_node_var_names(DB_STEP_DATA* m)
{
    if (m->u3398 <= 0) {
        return;
    }
    double* a = static_cast<double*>(m->u058);
    char* out = nullptr;
    if (!rt::dimension_is_2(m)) {
        out = reinterpret_cast<char*>(a + (field<std::int32_t>(m, 0x1e20) - 1 + m->u13e8 * m->u3398));
    } else {
        out = reinterpret_cast<char*>(a + (field<std::int32_t>(m, 0x1ea4) - 1));
    }
    write_names(out, m->u3398, m->u33a0);
}

// @src 1401c4be0
void write_step_elem_var_names(DB_STEP_DATA* m)
{
    if (m->u33a8 <= 0) {
        return;
    }
    double* a = static_cast<double*>(m->u058);
    char* out = nullptr;
    if (!rt::dimension_is_2(m)) {
        out = reinterpret_cast<char*>(a + (field<std::int32_t>(m, 0x1e60) - 1 + m->u13ec * m->u33a8));
    } else {
        out = reinterpret_cast<char*>(a + (field<std::int32_t>(m, 0x1ea0) - 1));
    }
    write_names(out, m->u33a8, m->u33b0);
}

// @src 1401c4d90
void write_step_objects(DB_STEP_DATA* m, std::int32_t* mar, double* a, std::int32_t* mt, std::int32_t* at,
                        double* llfnc, std::int32_t* iifnc, double* llwin, double* llwrz)
{
    if (!rt::dimension_is_3(m)) {
        write_objects_2d(&m->objects, mar, a, mt, at, iifnc, llfnc, llwin, llwrz);
    } else {
        write_objects_3d(&m->objects, mar, a, mt, at, iifnc, llfnc, llwin, llwrz);
    }
    write_step_node_var_names(m);
    write_step_elem_var_names(m);
}

// @src 1401c4ea0
void write_step_inter_objects(DB_STEP_DATA* m, double* llbrt, double* llfnc, std::int32_t* iifnc, double* llwin,
                              double* llwrz, double* llbnd)
{
    std::int32_t dim = m->dimension;
    model::select_index_tables(&dim);
    write_interobj_list(&m->inter_obj, llbrt, &field<std::int32_t>(m, 0x80), iifnc, llfnc, llwin, llwrz,
                        a_table(m, model::g_a_index[66]),  // DAT_1406e83d8
                        a_table(m, model::g_a_index[68]),  // DAT_1406e83e0
                        llbnd, a_table(m, model::g_a_index[65])); // DAT_1406e83d4
}

// @src 1401c4fa0
void write_step_inter_materials(DB_STEP_DATA* m, double* lltfm, double* llfnc, std::int32_t* iifnc)
{
    std::int32_t dim = m->dimension;
    model::select_index_tables(&dim);
    write_phase_pair_list(&m->phase_pairs, lltfm, iifnc, llfnc, a_table(m, model::g_a_index[61])); // DAT_1406e83c4
}

} // namespace def::dbio::gendb
